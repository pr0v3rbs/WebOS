#ifndef _PE_SHP_REG_H15A0_H_
#define _PE_SHP_REG_H15A0_H_
/*-----------------------------------------------------------------------------
		0xc800d000L shp_top_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 reg_shp_out_actv_sel               : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_pe1_hif_lr_access_en           : 1 ;  //     8     
} PE_H15A0_SHP_SHP_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d004L e_disp_sync_dly_addr00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_vsync                     : 16;  // 15: 0     
	UINT32 dly_sync_vfp                       : 16;  // 31:16     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR00_T;

/*-----------------------------------------------------------------------------
		0xc800d008L e_disp_sync_dly_addr01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_va                        : 16;  // 15: 0     
	UINT32 dly_sync_vbp                       : 16;  // 31:16     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR01_T;

/*-----------------------------------------------------------------------------
		0xc800d00cL e_disp_sync_dly_addr02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_hsync                     : 16;  // 15: 0     
	UINT32 dly_sync_hfp                       : 16;  // 31:16     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR02_T;

/*-----------------------------------------------------------------------------
		0xc800d010L e_disp_sync_dly_addr03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_ha                        : 16;  // 15: 0     
	UINT32 dly_sync_hbp                       : 16;  // 31:16     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR03_T;

/*-----------------------------------------------------------------------------
		0xc800d014L e_disp_sync_dly_addr04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_line_start                : 16;  // 15: 0     
	UINT32 dly_sync_actv_sel_in_h_src         : 1 ;  //    16     
	UINT32 dly_sync_actv_sel_in_v_src         : 1 ;  //    17     
	UINT32 dly_sync_actv_sel_in_h_inv         : 1 ;  //    18     
	UINT32 dly_sync_actv_sel_in_v_inv         : 1 ;  //    19     
	UINT32 dly_sync_actv_sel_out_ha           : 1 ;  //    20     
	UINT32 dly_sync_actv_sel_out_va           : 1 ;  //    21     
	UINT32 dly_sync_actv_sel_out_hs           : 1 ;  //    22     
	UINT32 dly_sync_actv_sel_out_vs           : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 cnt_reset_h_value_en               : 1 ;  //    28     
	UINT32 cnt_reset_v_value_en               : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 dly_sync_force_update              : 1 ;  //    31     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR04_T;

/*-----------------------------------------------------------------------------
		0xc800d018L e_disp_sync_dly_addr05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_h_dly                     : 16;  // 15: 0     
	UINT32 dly_sync_v_dly                     : 16;  // 31:16     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR05_T;

/*-----------------------------------------------------------------------------
		0xc800d01cL e_disp_sync_dly_addr06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cnt_h_reset_value                  : 16;  // 15: 0     
	UINT32 cnt_v_reset_value                  : 16;  // 31:16     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR06_T;

/*-----------------------------------------------------------------------------
		0xc800d020L e_disp_sync_dly_addr07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_dly_actv_h_dly                : 16;  // 15: 0     
	UINT32 auto_dly_actv_v_dly                : 16;  // 31:16     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR07_T;

/*-----------------------------------------------------------------------------
		0xc800d024L e_disp_sync_dly_addr08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_dly_sync_h_dly                : 16;  // 15: 0     
	UINT32 auto_dly_sync_v_dly                : 16;  // 31:16     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR08_T;

/*-----------------------------------------------------------------------------
		0xc800d028L e_disp_sync_dly_addr09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_normal_number                  : 16;  // 15: 0     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR09_T;

/*-----------------------------------------------------------------------------
		0xc800d02cL e_disp_sync_dly_addr0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cnt_h_reset_position               : 16;  // 15: 0     
	UINT32 cnt_v_reset_position               : 16;  // 31:16     
} PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR0A_T;

/*-----------------------------------------------------------------------------
		0xc800d030L o_disp_sync_dly_addr00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_vsync                     : 16;  // 15: 0     
	UINT32 dly_sync_vfp                       : 16;  // 31:16     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR00_T;

/*-----------------------------------------------------------------------------
		0xc800d034L o_disp_sync_dly_addr01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_va                        : 16;  // 15: 0     
	UINT32 dly_sync_vbp                       : 16;  // 31:16     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR01_T;

/*-----------------------------------------------------------------------------
		0xc800d038L o_disp_sync_dly_addr02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_hsync                     : 16;  // 15: 0     
	UINT32 dly_sync_hfp                       : 16;  // 31:16     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR02_T;

/*-----------------------------------------------------------------------------
		0xc800d03cL o_disp_sync_dly_addr03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_ha                        : 16;  // 15: 0     
	UINT32 dly_sync_hbp                       : 16;  // 31:16     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR03_T;

/*-----------------------------------------------------------------------------
		0xc800d040L o_disp_sync_dly_addr04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_line_start                : 16;  // 15: 0     
	UINT32 dly_sync_actv_sel_in_h_src         : 1 ;  //    16     
	UINT32 dly_sync_actv_sel_in_v_src         : 1 ;  //    17     
	UINT32 dly_sync_actv_sel_in_h_inv         : 1 ;  //    18     
	UINT32 dly_sync_actv_sel_in_v_inv         : 1 ;  //    19     
	UINT32 dly_sync_actv_sel_out_ha           : 1 ;  //    20     
	UINT32 dly_sync_actv_sel_out_va           : 1 ;  //    21     
	UINT32 dly_sync_actv_sel_out_hs           : 1 ;  //    22     
	UINT32 dly_sync_actv_sel_out_vs           : 1 ;  //    23     
	UINT32 tp_update_hv_zero                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 cnt_reset_h_value_en               : 1 ;  //    28     
	UINT32 cnt_reset_v_value_en               : 1 ;  //    29     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 dly_sync_force_update              : 1 ;  //    31     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR04_T;

/*-----------------------------------------------------------------------------
		0xc800d044L o_disp_sync_dly_addr05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dly_sync_h_dly                     : 16;  // 15: 0     
	UINT32 dly_sync_v_dly                     : 16;  // 31:16     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR05_T;

/*-----------------------------------------------------------------------------
		0xc800d048L o_disp_sync_dly_addr06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cnt_h_reset_value                  : 16;  // 15: 0     
	UINT32 cnt_v_reset_value                  : 16;  // 31:16     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR06_T;

/*-----------------------------------------------------------------------------
		0xc800d04cL o_disp_sync_dly_addr07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_dly_actv_h_dly                : 16;  // 15: 0     
	UINT32 auto_dly_actv_v_dly                : 16;  // 31:16     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR07_T;

/*-----------------------------------------------------------------------------
		0xc800d050L o_disp_sync_dly_addr08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_dly_sync_h_dly                : 16;  // 15: 0     
	UINT32 auto_dly_sync_v_dly                : 16;  // 31:16     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR08_T;

/*-----------------------------------------------------------------------------
		0xc800d054L o_disp_sync_dly_addr09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_normal_number                  : 16;  // 15: 0     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR09_T;

/*-----------------------------------------------------------------------------
		0xc800d058L o_disp_sync_dly_addr0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cnt_h_reset_position               : 16;  // 15: 0     
	UINT32 cnt_v_reset_position               : 16;  // 31:16     
} PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR0A_T;

/*-----------------------------------------------------------------------------
		0xc800d05cL pre_pmux_e_y_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4     
	UINT32 start_parity                       : 1 ;  //     6     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16     
	UINT32 alpha0_gain                        : 8 ;  // 31:24     
} PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d060L pre_pmux_e_y_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_main_window_h_en               : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_main_window_v_en               : 1 ;  //    31     
} PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d064L pre_pmux_e_y_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d068L pre_pmux_e_y_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_sub_window_h_en                : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_sub_window_v_en                : 1 ;  //    31     
} PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d06cL pre_pmux_e_y_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d070L pre_pmux_e_y_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_cr                        : 8 ;  //  7: 0     
	UINT32 bg_color_cb                        : 8 ;  // 15: 8     
	UINT32 bg_color_y                         : 8 ;  // 23:16     
} PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d074L pre_pmux_e_c_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4     
	UINT32 start_parity                       : 1 ;  //     6     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16     
	UINT32 alpha0_gain                        : 8 ;  // 31:24     
} PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d078L pre_pmux_e_c_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_main_window_h_en               : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_main_window_v_en               : 1 ;  //    31     
} PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d07cL pre_pmux_e_c_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d080L pre_pmux_e_c_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_sub_window_h_en                : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_sub_window_v_en                : 1 ;  //    31     
} PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d084L pre_pmux_e_c_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d088L pre_pmux_e_c_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_cr                        : 8 ;  //  7: 0     
	UINT32 bg_color_cb                        : 8 ;  // 15: 8     
	UINT32 bg_color_y                         : 8 ;  // 23:16     
} PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d08cL pre_pmux_o_y_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4     
	UINT32 start_parity                       : 1 ;  //     6     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16     
	UINT32 alpha0_gain                        : 8 ;  // 31:24     
} PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d090L pre_pmux_o_y_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_main_window_h_en               : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_main_window_v_en               : 1 ;  //    31     
} PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d094L pre_pmux_o_y_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d098L pre_pmux_o_y_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_sub_window_h_en                : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_sub_window_v_en                : 1 ;  //    31     
} PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d09cL pre_pmux_o_y_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d0a0L pre_pmux_o_y_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_cr                        : 8 ;  //  7: 0     
	UINT32 bg_color_cb                        : 8 ;  // 15: 8     
	UINT32 bg_color_y                         : 8 ;  // 23:16     
} PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d0a4L pre_pmux_o_c_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4     
	UINT32 start_parity                       : 1 ;  //     6     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16     
	UINT32 alpha0_gain                        : 8 ;  // 31:24     
} PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d0a8L pre_pmux_o_c_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_main_window_h_en               : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_main_window_v_en               : 1 ;  //    31     
} PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d0acL pre_pmux_o_c_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d0b0L pre_pmux_o_c_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_sub_window_h_en                : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_sub_window_v_en                : 1 ;  //    31     
} PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d0b4L pre_pmux_o_c_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d0b8L pre_pmux_o_c_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_cr                        : 8 ;  //  7: 0     
	UINT32 bg_color_cb                        : 8 ;  // 15: 8     
	UINT32 bg_color_y                         : 8 ;  // 23:16     
} PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d0bcL eo2lr_y_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_l_max                          : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_r_min                          : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 reg_eo2lr_y_detour_en              : 1 ;  //    31     
} PE_H15A0_SHP_EO2LR_Y_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800d0c0L eo2lr_c_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_l_max                          : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_r_min                          : 13;  // 28:16     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 reg_eo2lr_c_detour_en              : 1 ;  //    31     
} PE_H15A0_SHP_EO2LR_C_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800d0c4L lr2eo_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_line_end_pos                   : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_boundary_size                  : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_lr2eo_detour_en                : 1 ;  //    31     
} PE_H15A0_SHP_LR2EO_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800d0c8L post_pmux_e_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4     
	UINT32 start_parity                       : 1 ;  //     6     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16     
	UINT32 alpha0_gain                        : 8 ;  // 31:24     
} PE_H15A0_SHP_POST_PMUX_E_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d0ccL post_pmux_e_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_main_window_h_en               : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_main_window_v_en               : 1 ;  //    31     
} PE_H15A0_SHP_POST_PMUX_E_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d0d0L post_pmux_e_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_POST_PMUX_E_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d0d4L post_pmux_e_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_sub_window_h_en                : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_sub_window_v_en                : 1 ;  //    31     
} PE_H15A0_SHP_POST_PMUX_E_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d0d8L post_pmux_e_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_POST_PMUX_E_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d0dcL post_pmux_e_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_cr                        : 8 ;  //  7: 0     
	UINT32 bg_color_cb                        : 8 ;  // 15: 8     
	UINT32 bg_color_y                         : 8 ;  // 23:16     
} PE_H15A0_SHP_POST_PMUX_E_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d0e0L post_pmux_o_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4     
	UINT32 start_parity                       : 1 ;  //     6     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16     
	UINT32 alpha0_gain                        : 8 ;  // 31:24     
} PE_H15A0_SHP_POST_PMUX_O_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d0e4L post_pmux_o_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_main_window_h_en               : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_main_window_v_en               : 1 ;  //    31     
} PE_H15A0_SHP_POST_PMUX_O_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d0e8L post_pmux_o_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_POST_PMUX_O_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d0ecL post_pmux_o_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_sub_window_h_en                : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_sub_window_v_en                : 1 ;  //    31     
} PE_H15A0_SHP_POST_PMUX_O_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d0f0L post_pmux_o_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHP_POST_PMUX_O_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d0f4L post_pmux_o_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_cr                        : 8 ;  //  7: 0     
	UINT32 bg_color_cb                        : 8 ;  // 15: 8     
	UINT32 bg_color_y                         : 8 ;  // 23:16     
} PE_H15A0_SHP_POST_PMUX_O_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d400L pe1_top_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} PE_H15A0_SHPL_PE1_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d404L pe1_top_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 width                              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 height                             : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_TOP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d408L pe1_top_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 pe1_detour                         : 1 ;  //     2     
	UINT32 line_memory_detour                 : 1 ;  //     3     
	UINT32                                    : 3 ;  //  6: 4     reserved
	UINT32 mode_3d                            : 1 ;  //     7     
	UINT32 vdpulse_pos                        : 12;  // 19: 8     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 master_en                          : 1 ;  //    24     
} PE_H15A0_SHPL_PE1_TOP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d40cL pe1_top_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt                               : 12;  // 11: 0     
	UINT32 va_pe1_in                          : 1 ;  //    12     
	UINT32 va_pe1_out                         : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 vcnt2                              : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_TOP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d410L pe1_top_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_to_cnt                           : 12;  // 11: 0     
} PE_H15A0_SHPL_PE1_TOP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d420L pe1_win1_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win0_en                            : 1 ;  //     0     
	UINT32 win1_en                            : 1 ;  //     1     
	UINT32 win01_en                           : 1 ;  //     2     
	UINT32                                    : 3 ;  //  5: 3     reserved
	UINT32 win_outside                        : 1 ;  //     6     
	UINT32 win_en                             : 1 ;  //     7     
	UINT32 bdr_alpha                          : 5 ;  // 12: 8     
	UINT32 bdr_wid                            : 2 ;  // 14:13     
	UINT32 bdr_en                             : 1 ;  //    15     
	UINT32 cr5                                : 5 ;  // 20:16     
	UINT32 cb5                                : 5 ;  // 25:21     
	UINT32 yy6                                : 6 ;  // 31:26     
} PE_H15A0_SHPL_PE1_WIN1_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d424L pe1_win1_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_WIN1_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d428L pe1_win1_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_WIN1_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d42cL pe1_win1_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_WIN1_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d430L pe1_win1_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_WIN1_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d440L pe1_msc_m1_y_ctrl ''
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
} PE_H15A0_SHPL_PE1_MSC_M1_Y_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800d444L pe1_msc_m2_y_ctrl ''
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
} PE_H15A0_SHPL_PE1_MSC_M2_Y_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800d448L pe1_msc_m1_c_ctrl ''
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
} PE_H15A0_SHPL_PE1_MSC_M1_C_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800d44cL pe1_msc_m2_c_ctrl ''
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
} PE_H15A0_SHPL_PE1_MSC_M2_C_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800d490L pe1_win3_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win0_en                            : 1 ;  //     0     
	UINT32 win1_en                            : 1 ;  //     1     
	UINT32 win01_en                           : 1 ;  //     2     
	UINT32                                    : 3 ;  //  5: 3     reserved
	UINT32 win_outside                        : 1 ;  //     6     
	UINT32 win_en                             : 1 ;  //     7     
	UINT32 bdr_alpha                          : 5 ;  // 12: 8     
	UINT32 bdr_wid                            : 2 ;  // 14:13     
	UINT32 bdr_en                             : 1 ;  //    15     
	UINT32 cr5                                : 5 ;  // 20:16     
	UINT32 cb5                                : 5 ;  // 25:21     
	UINT32 yy6                                : 6 ;  // 31:26     
} PE_H15A0_SHPL_PE1_WIN3_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d494L pe1_win3_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_WIN3_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d498L pe1_win3_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_WIN3_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d49cL pe1_win3_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_WIN3_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d4a0L pe1_win3_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_WIN3_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d4b0L pe1_derh_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsum_mode                          : 2 ;  //  1: 0     
	UINT32 hsum_mode                          : 2 ;  //  3: 2     
	UINT32 vsum121_en                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 vertical_min_max                   : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 a_flt_mux                          : 2 ;  // 17:16     
	UINT32 a_e_mux                            : 2 ;  // 19:18     
	UINT32 t_mux                              : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 a_gen_width                        : 4 ;  // 27:24     
} PE_H15A0_SHPL_PE1_DERH_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d4b4L pe1_derh_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_gen_of_y                         : 8 ;  //  7: 0     
	UINT32 a_gen_of_c                         : 8 ;  // 15: 8     
	UINT32 avg_flt_mode                       : 3 ;  // 18:16     
	UINT32 avg_flt_en                         : 1 ;  //    19     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 max_flt_mode                       : 3 ;  // 26:24     
} PE_H15A0_SHPL_PE1_DERH_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d4b8L pe1_derh_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_exp_gain_ctrl_y0                 : 4 ;  //  3: 0     
	UINT32 a_exp2_gain_ctrl_x0                : 4 ;  //  7: 4     
	UINT32 a_exp2_gain_ctrl_y1                : 4 ;  // 11: 8     
	UINT32 a_exp2_gain_ctrl_x1                : 4 ;  // 15:12     
	UINT32 a_avg2_flt_wid                     : 4 ;  // 19:16     
	UINT32 a_avg2_flt_en                      : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 a_exp_max2_en                      : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 y_sum_mode                         : 2 ;  // 27:26     
} PE_H15A0_SHPL_PE1_DERH_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d4bcL pe1_derh_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 5 ;  //  4: 0     reserved
	UINT32 a_mux_for_edge_gaintable           : 3 ;  //  7: 5     
	UINT32                                    : 9 ;  // 16: 8     reserved
	UINT32 a_mux_for_detail_filter            : 3 ;  // 19:17     
	UINT32 tedge_mux                          : 1 ;  //    20     
	UINT32 a_mux_for_edge_filter              : 3 ;  // 23:21     
	UINT32 a_scaling_flat_filter_             : 4 ;  // 27:24     
	UINT32                                    : 1 ;  //    28     reserved
	UINT32 a_mux_for_flat_filter              : 3 ;  // 31:29     
} PE_H15A0_SHPL_PE1_DERH_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d4c0L pe1_derh_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 reg_dbg_scale                      : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sum_mux                            : 4 ;  //  7: 4     
	UINT32 reg_enh_en_cc                      : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_enh_en_yy                      : 3 ;  // 14:12     
} PE_H15A0_SHPL_PE1_DERH_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d4c4L pe1_derh_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vmm_param                      : 2 ;  //  1: 0     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_csft_gain                      : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_th_gain_edge                   : 6 ;  // 21:16     
	UINT32 reg_th_gain_flat                   : 2 ;  // 23:22     
	UINT32 reg_th_manual_th                   : 7 ;  // 30:24     
	UINT32 reg_th_manual_en                   : 1 ;  //    31     
} PE_H15A0_SHPL_PE1_DERH_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d4c8L pe1_derh_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 edge_filter_white_gain             : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 edge_filter_black_gain             : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_amean_en                       : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 edge_filter_v_tap                  : 2 ;  // 27:26     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 edge_c_filter_en                   : 1 ;  //    30     
	UINT32 edge_y_filter_en                   : 1 ;  //    31     
} PE_H15A0_SHPL_PE1_DERH_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800d4ccL pe1_derh_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_filter_tap                     : 2 ;  //  1: 0     
	UINT32                                    : 22;  // 23: 2     reserved
	UINT32 flat_filter_gain                   : 6 ;  // 29:24     
	UINT32 flat_filter_type                   : 1 ;  //    30     
	UINT32 flat_filter_en                     : 1 ;  //    31     
} PE_H15A0_SHPL_PE1_DERH_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800d4d0L pe1_derh_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 amod_ctrl0_y0                      : 8 ;  //  7: 0     
	UINT32 amod_ctrl0_x0                      : 8 ;  // 15: 8     
	UINT32 amod_ctrl0_y1                      : 8 ;  // 23:16     
	UINT32 amod_ctrl0_x1                      : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_DERH_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800d4d4L pe1_derh_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 amod_ctrl1_x0                      : 8 ;  //  7: 0     
	UINT32 amod_ctrl1_x1                      : 8 ;  // 15: 8     
	UINT32 y_c_mux_control                    : 3 ;  // 18:16     
	UINT32 chroma_weight                      : 5 ;  // 23:19     
} PE_H15A0_SHPL_PE1_DERH_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800d4d8L pe1_derh_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a2g_mode_e                         : 1 ;  //     0     
	UINT32 a2g_mode_f                         : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 a2g_mgain_e                        : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 a2g_mgain_f                        : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 debug_mode                         : 4 ;  // 27:24     
} PE_H15A0_SHPL_PE1_DERH_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc800d4dcL pe1_derh_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 e_gain_th1                         : 8 ;  //  7: 0     
	UINT32 e_gain_th2                         : 8 ;  // 15: 8     
	UINT32 f_gain_th1                         : 8 ;  // 23:16     
	UINT32 f_gain_th2                         : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_DERH_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc800d4e0L pe1_derh_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 e_gain_max                         : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 f_gain_max                         : 6 ;  // 13: 8     
} PE_H15A0_SHPL_PE1_DERH_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc800d4e4L pe1_coring_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y2                                 : 8 ;  //  7: 0     
	UINT32 x2                                 : 8 ;  // 15: 8     
	UINT32 y3                                 : 8 ;  // 23:16     
	UINT32 x3                                 : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800d4f0L pe1_derv_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 der_v_en                           : 1 ;  //     0     
	UINT32 der_gain_mapping                   : 2 ;  //  2: 1     
	UINT32 bif_en                             : 1 ;  //     3     
	UINT32 output_mux                         : 4 ;  //  7: 4     
	UINT32 bif_manual_th                      : 8 ;  // 15: 8     
	UINT32 th_gain                            : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 th_mode                            : 1 ;  //    24     
} PE_H15A0_SHPL_PE1_DERV_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800d4f4L pe1_derv_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csft_gain                          : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 csft_mode                          : 1 ;  //     8     
} PE_H15A0_SHPL_PE1_DERV_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800d4f8L pe1_derv_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mmd_sel                            : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 max_sel                            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 avg_sel                            : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 gain_th1                           : 8 ;  // 23:16     
	UINT32 gain_th2                           : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_DERV_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800d4fcL pe1_derv_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gain_b                             : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 gain_w                             : 7 ;  // 14: 8     
} PE_H15A0_SHPL_PE1_DERV_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800d510L pe1_dp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_display                      : 3 ;  //  2: 0     
	UINT32                                    : 27;  // 29: 3     reserved
	UINT32 reg_a_gain_balance_en              : 1 ;  //    30     
	UINT32 reg_t_gain_balance_en              : 1 ;  //    31     
} PE_H15A0_SHPL_PE1_DP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d514L pe1_dp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_gain_b                        : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 edge_gain_w                        : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 texture_gain_b                     : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 texture_gain_w                     : 7 ;  // 30:24     
} PE_H15A0_SHPL_PE1_DP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d518L pe1_dp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ptic_gain_e_b                  : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_ptic_gain_e_w                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_ptic_gain_t_b                  : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_ptic_gain_t_w                  : 7 ;  // 30:24     
} PE_H15A0_SHPL_PE1_DP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d51cL pe1_dp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y0                                 : 8 ;  //  7: 0     
	UINT32 x0                                 : 8 ;  // 15: 8     
	UINT32 y1                                 : 8 ;  // 23:16     
	UINT32 x1                                 : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_DP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d530L pe1_sp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_enhance_enable                : 1 ;  //     0     
	UINT32                                    : 11;  // 11: 1     reserved
	UINT32 edge_operator_selection            : 2 ;  // 13:12     
} PE_H15A0_SHPL_PE1_SP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d534L pe1_sp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 white_gain                         : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 black_gain                         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 horizontal_gain                    : 8 ;  // 23:16     
	UINT32 vertical_gain                      : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_SP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d538L pe1_sp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sobel_weight                       : 8 ;  //  7: 0     
	UINT32 laplacian_weight                   : 8 ;  // 15: 8     
	UINT32 sobel_manual_mode_en               : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 sobel_manual_gain                  : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_SP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d53cL pe1_sp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPL_PE1_SP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d540L pe1_sp_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 display_mode                       : 4 ;  // 11: 8     
	UINT32 gx_weight_manual_mode              : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 gx_weight_manual_gain              : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_SP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d544L pe1_sp_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPL_PE1_SP_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d548L pe1_sp_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lap_h_mode                         : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 lap_v_mode                         : 1 ;  //     4     
} PE_H15A0_SHPL_PE1_SP_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800d54cL pe1_sp_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_en                              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 gb_mode                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 gb_x1                              : 8 ;  // 15: 8     
	UINT32 gb_y1                              : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_SP_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800d550L pe1_sp_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_x2                              : 8 ;  //  7: 0     
	UINT32 gb_y2                              : 8 ;  // 15: 8     
	UINT32 gb_y3                              : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_SP_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800d554L pe1_sp_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_x_l0                          : 8 ;  //  7: 0     
	UINT32 lum1_x_l1                          : 8 ;  // 15: 8     
	UINT32 lum1_x_h0                          : 8 ;  // 23:16     
	UINT32 lum1_x_h1                          : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_SP_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800d558L pe1_sp_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_y0                            : 8 ;  //  7: 0     
	UINT32 lum1_y1                            : 8 ;  // 15: 8     
	UINT32 lum1_y2                            : 8 ;  // 23:16     
	UINT32 lum2_x_l0                          : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_SP_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc800d55cL pe1_sp_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_x_l1                          : 8 ;  //  7: 0     
	UINT32 lum2_x_h0                          : 8 ;  // 15: 8     
	UINT32 lum2_x_h1                          : 8 ;  // 23:16     
	UINT32 lum2_y0                            : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_SP_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc800d560L pe1_sp_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_y1                            : 8 ;  //  7: 0     
	UINT32 lum2_y2                            : 8 ;  // 15: 8     
} PE_H15A0_SHPL_PE1_SP_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc800d570L pe1_mp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_enhance_enable                : 1 ;  //     0     
	UINT32                                    : 11;  // 11: 1     reserved
	UINT32 edge_operator_selection            : 2 ;  // 13:12     
} PE_H15A0_SHPL_PE1_MP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d574L pe1_mp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 white_gain                         : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 black_gain                         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 horizontal_gain                    : 8 ;  // 23:16     
	UINT32 vertical_gain                      : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_MP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d578L pe1_mp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sobel_weight                       : 8 ;  //  7: 0     
	UINT32 laplacian_weight                   : 8 ;  // 15: 8     
	UINT32 sobel_manual_mode_en               : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 sobel_manual_gain                  : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_MP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d57cL pe1_mp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPL_PE1_MP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d580L pe1_mp_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 display_mode                       : 4 ;  // 11: 8     
	UINT32 gx_weight_manual_mode              : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 gx_weight_manual_gain              : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_MP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d584L pe1_mp_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPL_PE1_MP_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d588L pe1_mp_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lap_h_mode                         : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 lap_v_mode                         : 3 ;  //  6: 4     
} PE_H15A0_SHPL_PE1_MP_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800d58cL pe1_mp_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_en                              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 gb_mode                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 gb_x1                              : 8 ;  // 15: 8     
	UINT32 gb_y1                              : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_MP_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800d590L pe1_mp_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_x2                              : 8 ;  //  7: 0     
	UINT32 gb_y2                              : 8 ;  // 15: 8     
	UINT32 gb_y3                              : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_MP_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800d594L pe1_mp_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_x_l0                          : 8 ;  //  7: 0     
	UINT32 lum1_x_l1                          : 8 ;  // 15: 8     
	UINT32 lum1_x_h0                          : 8 ;  // 23:16     
	UINT32 lum1_x_h1                          : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_MP_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800d598L pe1_mp_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_y0                            : 8 ;  //  7: 0     
	UINT32 lum1_y1                            : 8 ;  // 15: 8     
	UINT32 lum1_y2                            : 8 ;  // 23:16     
	UINT32 lum2_x_l0                          : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_MP_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc800d59cL pe1_mp_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_x_l1                          : 8 ;  //  7: 0     
	UINT32 lum2_x_h0                          : 8 ;  // 15: 8     
	UINT32 lum2_x_h1                          : 8 ;  // 23:16     
	UINT32 lum2_y0                            : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_MP_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc800d5a0L pe1_mp_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_y1                            : 8 ;  //  7: 0     
	UINT32 lum2_y2                            : 8 ;  // 15: 8     
} PE_H15A0_SHPL_PE1_MP_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc800d5b0L pe1_coring_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_map_h_size                   : 2 ;  //  1: 0     
	UINT32 reg_a_nomalize_en                  : 1 ;  //     2     
	UINT32 reg_t_nomalize_en                  : 1 ;  //     3     
	UINT32 reg_t_cut_resolution               : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 reg_cross_th_sc                    : 8 ;  // 23:16     
	UINT32 reg_cross_th                       : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d5b4L pe1_coring_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_detail_beta_mul                : 8 ;  //  7: 0     
	UINT32 reg_detail_alpha_mul               : 8 ;  // 15: 8     
	UINT32 reg_edge_beta_mul                  : 8 ;  // 23:16     
	UINT32 reg_edge_alpha_mul                 : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d5b8L pe1_coring_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_edge_y0                  : 8 ;  //  7: 0     
	UINT32 reg_a_lut_edge_x0                  : 8 ;  // 15: 8     
	UINT32 reg_a_lut_edge_y1                  : 8 ;  // 23:16     
	UINT32 reg_a_lut_edge_x1                  : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d5bcL pe1_coring_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_detail_y0                : 8 ;  //  7: 0     
	UINT32 reg_a_lut_detail_x0                : 8 ;  // 15: 8     
	UINT32 reg_a_lut_detail_y1                : 8 ;  // 23:16     
	UINT32 reg_a_lut_detail_x1                : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d5c0L pe1_coring_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_edge_y0                  : 8 ;  //  7: 0     
	UINT32 reg_t_lut_edge_x0                  : 8 ;  // 15: 8     
	UINT32 reg_t_lut_edge_y1                  : 8 ;  // 23:16     
	UINT32 reg_t_lut_edge_x1                  : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d5c4L pe1_coring_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_detail_y0                : 8 ;  //  7: 0     
	UINT32 reg_t_lut_detail_x0                : 8 ;  // 15: 8     
	UINT32 reg_t_lut_detail_y1                : 8 ;  // 23:16     
	UINT32 reg_t_lut_detail_x1                : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d5c8L pe1_coring_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_edge_minmax_y0                 : 8 ;  //  7: 0     
	UINT32 reg_edge_minmax_x0                 : 8 ;  // 15: 8     
	UINT32 reg_edge_minmax_y1                 : 8 ;  // 23:16     
	UINT32 reg_edge_minmax_x1                 : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800d5ccL pe1_coring_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_detail_minmax_y0               : 8 ;  //  7: 0     
	UINT32 reg_detail_minmax_x0               : 8 ;  // 15: 8     
	UINT32 reg_detail_minmax_y1               : 8 ;  // 23:16     
	UINT32 reg_detail_minmax_x1               : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800d5d0L pe1_coring_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mp_coring_mode                     : 2 ;  //  1: 0     
	UINT32 sp_coring_mode                     : 2 ;  //  3: 2     
	UINT32 mp_coring_en                       : 1 ;  //     4     
	UINT32 sp_coring_en                       : 1 ;  //     5     
	UINT32 pti_coring_en                      : 1 ;  //     6     
	UINT32 pti_coring_mode                    : 2 ;  //  8: 7     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_mul_base_t                     : 8 ;  // 23:16     
	UINT32 reg_mul_base_e                     : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800d5d4L pe1_coring_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_gain_b                        : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 edge_gain_w                        : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 texture_gain_b                     : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 texture_gain_w                     : 7 ;  // 30:24     
} PE_H15A0_SHPL_PE1_CORING_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800d5e0L pe1_coring_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_map_frame_sum                    : 32;  // 31: 0
} PE_H15A0_SHPL_PE1_CORING_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc800d5e4L pe1_coring_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 t_map_frame_sum                    : 32;  // 31: 0
} PE_H15A0_SHPL_PE1_CORING_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc800d5e8L pe1_coring_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_frame_sum                     : 32;  // 31: 0
} PE_H15A0_SHPL_PE1_CORING_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc800d5ecL pe1_coring_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 detail_frame_sum                   : 32;  // 31: 0
} PE_H15A0_SHPL_PE1_CORING_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc800d600L pe1_pti_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ti_enable                      : 1 ;  //     0     
	UINT32 reg_debug_map_en                   : 1 ;  //     1     
	UINT32 reg_mm_tap_size                    : 2 ;  //  3: 2     
	UINT32 reg_avg_tap_size                   : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_master_gain                    : 8 ;  // 15: 8     
} PE_H15A0_SHPL_PE1_PTI_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d604L pe1_pti_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ti_enable                      : 1 ;  //     0     
	UINT32 reg_debug_map_en                   : 1 ;  //     1     
	UINT32 reg_mm_tap_size                    : 2 ;  //  3: 2     
	UINT32 reg_avg_tap_size                   : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_master_gain                    : 8 ;  // 15: 8     
} PE_H15A0_SHPL_PE1_PTI_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d608L pe1_sti_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ti_enable                      : 1 ;  //     0     
	UINT32 reg_debug_map_en                   : 1 ;  //     1     
	UINT32 reg_mm_tap_size                    : 2 ;  //  3: 2     
	UINT32 reg_avg_tap_size                   : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_master_gain                    : 8 ;  // 15: 8     
	UINT32 reg_texture_gain                   : 8 ;  // 23:16     
	UINT32 reg_edge_gain                      : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_STI_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d60cL pe1_sti_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPL_PE1_STI_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d620L pe1_dj_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edf_en                             : 1 ;  //     0     
	UINT32 hv_filter_en                       : 1 ;  //     1     
	UINT32 g0_feature_norm_en                 : 1 ;  //     2     
	UINT32 g0_f2g_mapping_en                  : 1 ;  //     3     
	UINT32 line_variation_mode                : 1 ;  //     4     
	UINT32 l_type_protection                  : 1 ;  //     5     
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 direction_type_mode                : 1 ;  //     7     
	UINT32 count_diff_th                      : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 output_mux                         : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 center_blur_mode                   : 2 ;  // 21:20     
	UINT32 n_avg_mode                         : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 line_variation_diff_threshold      : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_DJ_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d624L pe1_dj_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 level_th                           : 8 ;  //  7: 0     
	UINT32 protect_th                         : 8 ;  // 15: 8     
	UINT32 n_avg_gain                         : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_DJ_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d628L pe1_dj_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g0_cnt_min                     : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_g0_mul                         : 5 ;  // 12: 8     
} PE_H15A0_SHPL_PE1_DJ_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d62cL pe1_dj_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g1_protect_min                 : 8 ;  //  7: 0     
	UINT32 reg_g1_mul                         : 7 ;  // 14: 8     
} PE_H15A0_SHPL_PE1_DJ_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d630L pe1_cti_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 filter_tap_size                    : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8     
} PE_H15A0_SHPL_PE1_CTI_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d634L pe1_cti_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 coring_th0                         : 8 ;  //  7: 0     
	UINT32 coring_th1                         : 8 ;  // 15: 8     
	UINT32 coring_map_filter                  : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 coring_tap_size                    : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 debug_mode                         : 2 ;  // 25:24     
} PE_H15A0_SHPL_PE1_CTI_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d638L pe1_cti_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ycm_en0                            : 1 ;  //     0     
	UINT32 ycm_en1                            : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 ycm_band_sel                       : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 ycm_diff_th                        : 8 ;  // 15: 8     
	UINT32 ycm_y_gain                         : 4 ;  // 19:16     
	UINT32 ycm_c_gain                         : 4 ;  // 23:20     
} PE_H15A0_SHPL_PE1_CTI_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d650L pe1_debug_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_debug_mode                  : 1 ;  //     0     
	UINT32 status_enable                      : 1 ;  //     1     
	UINT32 debug_mode                         : 1 ;  //     2     
	UINT32 show_dce_blur                      : 1 ;  //     3     
	UINT32                                    : 5 ;  //  8: 4     reserved
	UINT32 show_sqm2                          : 1 ;  //     9     
	UINT32 sqm2_debug_image                   : 4 ;  // 13:10     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 show_bbd                           : 2 ;  // 16:15     
	UINT32 bbd_line_width                     : 3 ;  // 19:17     
	UINT32 show_apl                           : 1 ;  //    20     
} PE_H15A0_SHPL_PE1_DEBUG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d654L pe1_debug_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 force_yy_enable                    : 1 ;  //     1     
	UINT32 force_cb_enable                    : 1 ;  //     2     
	UINT32 force_cr_enable                    : 1 ;  //     3     
	UINT32 yc_debug_mode_sel                  : 3 ;  //  6: 4     
} PE_H15A0_SHPL_PE1_DEBUG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d658L pe1_debug_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 10;  //  9: 0     reserved
	UINT32 force_cb                           : 10;  // 19:10     
	UINT32 force_cr                           : 10;  // 29:20     
} PE_H15A0_SHPL_PE1_DEBUG_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d660L pe1_sqm_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 outmux_debugmap                    : 4 ;  //  3: 0     
	UINT32 iir                                : 4 ;  //  7: 4     
	UINT32 i_score                            : 8 ;  // 15: 8     
	UINT32 v_score                            : 8 ;  // 23:16     
	UINT32 l_score                            : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_SQM_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d664L pe1_sqm_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_limit                            : 8 ;  //  7: 0     
	UINT32 l_limit                            : 8 ;  // 15: 8     
	UINT32 score_threshold                    : 8 ;  // 23:16     
	UINT32 flat_threshold                     : 8 ;  // 31:24     
} PE_H15A0_SHPL_PE1_SQM_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d668L pe1_sqm_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_x0                             : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 win_y0                             : 11;  // 26:16     
} PE_H15A0_SHPL_PE1_SQM_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d66cL pe1_sqm_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_x1                             : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 win_y1                             : 11;  // 26:16     
} PE_H15A0_SHPL_PE1_SQM_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d670L pe1_sqm_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 charcter_delta_limit               : 8 ;  //  7: 0     
	UINT32 boundary_width                     : 8 ;  // 15: 8     
	UINT32 video_height                       : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_SQM_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d674L pe1_sqm_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pc_threshold                       : 8 ;  //  7: 0     
	UINT32 pc_delta_limit                     : 8 ;  // 15: 8     
	UINT32 pc_pixel_minimal_limit             : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_SQM_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d678L pe1_sqm_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 blur_pixel                         : 10;  //  9: 0     
	UINT32 sharp_pixel                        : 10;  // 19:10     
	UINT32 sqm_output                         : 10;  // 29:20     
	UINT32 pc_detect                          : 1 ;  //    30     
} PE_H15A0_SHPL_PE1_SQM_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc800d680L pe1_bbd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bbd_en                         : 1 ;  //     0     
	UINT32 reg_hys_mode                       : 5 ;  //  5: 1     
	UINT32 reg_op_mode                        : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cnt_th                         : 8 ;  // 15: 8     
	UINT32 reg_diff_th                        : 8 ;  // 23:16     
} PE_H15A0_SHPL_PE1_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d684L pe1_bbd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_x0                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 bbd_y0                             : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d688L pe1_bbd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_x1                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 bbd_y1                             : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d68cL pe1_bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc800d690L pe1_bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHPL_PE1_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc800d6a0L pe1_tgen_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tgen_on_off                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 tgen_initial_seed_mode             : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 tgen_table_sel                     : 4 ;  // 19:16     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800d6a4L pe1_tgen_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_delta_h_bound                   : 5 ;  //  4: 0     
	UINT32                                    : 4 ;  //  8: 5     reserved
	UINT32 tg_delta_l_bound                   : 4 ;  // 12: 9     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 tg_delta_max                       : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 tg_delta_scale                     : 3 ;  // 26:24     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800d6a8L pe1_tgen_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_rnd_th                          : 8 ;  //  7: 0     
	UINT32 tg_delta_wcurr                     : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 tg_lc_ldr_th                       : 6 ;  // 21:16     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800d6acL pe1_tgen_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rand_x0                            : 32;  // 31: 0     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800d6b0L pe1_tgen_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_dr_ratio                        : 3 ;  //  2: 0     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 tg_dr_th                           : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 tg_mm_offset                       : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 tg_mm_sel_mode                     : 2 ;  // 25:24     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800d6b4L pe1_tgen_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_lavg_th                         : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 tg_gs_rnd                          : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 tg_gs_mm                           : 5 ;  // 28:24     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800d6b8L pe1_tgen_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_blur_coef_x2                    : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 tg_blur_coef_x1                    : 6 ;  // 13: 8     
	UINT32                                    : 10;  // 23:14     reserved
	UINT32 tg_blur_coef_x0                    : 6 ;  // 29:24     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800d6bcL pe1_tgen_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_blur_coef_y2                    : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 tg_blur_coef_y1                    : 6 ;  // 13: 8     
	UINT32                                    : 10;  // 23:14     reserved
	UINT32 tg_blur_coef_y0                    : 6 ;  // 29:24     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800d6c0L pe1_tgen_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_map_edge_gain                   : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 tg_map_detail_gain                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 tg_map_offset                      : 8 ;  // 23:16     
	UINT32 tg_gs_noise                        : 6 ;  // 29:24     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800d6c4L pe1_tgen_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_gain7                           : 4 ;  //  3: 0     
	UINT32 tg_gain6                           : 4 ;  //  7: 4     
	UINT32 tg_gain5                           : 4 ;  // 11: 8     
	UINT32 tg_gain4                           : 4 ;  // 15:12     
	UINT32 tg_gain3                           : 4 ;  // 19:16     
	UINT32 tg_gain2                           : 4 ;  // 23:20     
	UINT32 tg_gain1                           : 4 ;  // 27:24     
	UINT32 tg_gain0                           : 4 ;  // 31:28     
} PE_H15A0_SHPL_PE1_TGEN_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800da00L pe1_top_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} PE_H15A0_SHPR_PE1_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800da04L pe1_top_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 width                              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 height                             : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_TOP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800da08L pe1_top_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 pe1_detour                         : 1 ;  //     2     
	UINT32 line_memory_detour                 : 1 ;  //     3     
	UINT32                                    : 3 ;  //  6: 4     reserved
	UINT32 mode_3d                            : 1 ;  //     7     
	UINT32 vdpulse_pos                        : 12;  // 19: 8     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 master_en                          : 1 ;  //    24     
} PE_H15A0_SHPR_PE1_TOP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800da0cL pe1_top_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt                               : 12;  // 11: 0     
	UINT32 va_pe1_in                          : 1 ;  //    12     
	UINT32 va_pe1_out                         : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 vcnt2                              : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_TOP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800da10L pe1_top_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_to_cnt                           : 12;  // 11: 0     
} PE_H15A0_SHPR_PE1_TOP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800da20L pe1_win1_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win0_en                            : 1 ;  //     0     
	UINT32 win1_en                            : 1 ;  //     1     
	UINT32 win01_en                           : 1 ;  //     2     
	UINT32                                    : 3 ;  //  5: 3     reserved
	UINT32 win_outside                        : 1 ;  //     6     
	UINT32 win_en                             : 1 ;  //     7     
	UINT32 bdr_alpha                          : 5 ;  // 12: 8     
	UINT32 bdr_wid                            : 2 ;  // 14:13     
	UINT32 bdr_en                             : 1 ;  //    15     
	UINT32 cr5                                : 5 ;  // 20:16     
	UINT32 cb5                                : 5 ;  // 25:21     
	UINT32 yy6                                : 6 ;  // 31:26     
} PE_H15A0_SHPR_PE1_WIN1_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800da24L pe1_win1_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_WIN1_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800da28L pe1_win1_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_WIN1_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800da2cL pe1_win1_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_WIN1_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800da30L pe1_win1_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_WIN1_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800da40L pe1_fsw_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nd0_hue_bin_6                      : 8 ;  //  7: 0
	UINT32 nd0_hue_bin_7                      : 8 ;  // 15: 8
	UINT32 nd0_hue_bin_8                      : 8 ;  // 23:16
	UINT32 nd0_hue_bin_9                      : 8 ;  // 31:24
} PE_H15A0_SHPR_PE1_FSW_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800da44L pe1_fsw_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nd1_hue_bin_6                      : 8 ;  //  7: 0
	UINT32 nd1_hue_bin_7                      : 8 ;  // 15: 8
	UINT32 nd1_hue_bin_8                      : 8 ;  // 23:16
	UINT32 nd1_hue_bin_9                      : 8 ;  // 31:24
} PE_H15A0_SHPR_PE1_FSW_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800da48L pe1_fsw_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPR_PE1_FSW_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800da4cL pe1_fsw_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPR_PE1_FSW_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800da90L pe1_win3_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win0_en                            : 1 ;  //     0     
	UINT32 win1_en                            : 1 ;  //     1     
	UINT32 win01_en                           : 1 ;  //     2     
	UINT32                                    : 3 ;  //  5: 3     reserved
	UINT32 win_outside                        : 1 ;  //     6     
	UINT32 win_en                             : 1 ;  //     7     
	UINT32 bdr_alpha                          : 5 ;  // 12: 8     
	UINT32 bdr_wid                            : 2 ;  // 14:13     
	UINT32 bdr_en                             : 1 ;  //    15     
	UINT32 cr5                                : 5 ;  // 20:16     
	UINT32 cb5                                : 5 ;  // 25:21     
	UINT32 yy6                                : 6 ;  // 31:26     
} PE_H15A0_SHPR_PE1_WIN3_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800da94L pe1_win3_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_WIN3_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800da98L pe1_win3_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_WIN3_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800da9cL pe1_win3_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_WIN3_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800daa0L pe1_win3_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_WIN3_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800dab0L pe1_derh_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsum_mode                          : 2 ;  //  1: 0     
	UINT32 hsum_mode                          : 2 ;  //  3: 2     
	UINT32 vsum121_en                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 vertical_min_max                   : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 a_flt_mux                          : 2 ;  // 17:16     
	UINT32 a_e_mux                            : 2 ;  // 19:18     
	UINT32 t_mux                              : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 a_gen_width                        : 4 ;  // 27:24     
} PE_H15A0_SHPR_PE1_DERH_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dab4L pe1_derh_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_gen_of_y                         : 8 ;  //  7: 0     
	UINT32 a_gen_of_c                         : 8 ;  // 15: 8     
	UINT32 avg_flt_mode                       : 3 ;  // 18:16     
	UINT32 avg_flt_en                         : 1 ;  //    19     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 max_flt_mode                       : 3 ;  // 26:24     
} PE_H15A0_SHPR_PE1_DERH_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dab8L pe1_derh_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_exp_gain_ctrl_y0                 : 4 ;  //  3: 0     
	UINT32 a_exp2_gain_ctrl_x0                : 4 ;  //  7: 4     
	UINT32 a_exp2_gain_ctrl_y1                : 4 ;  // 11: 8     
	UINT32 a_exp2_gain_ctrl_x1                : 4 ;  // 15:12     
	UINT32 a_avg2_flt_wid                     : 4 ;  // 19:16     
	UINT32 a_avg2_flt_en                      : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 a_exp_max2_en                      : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 y_sum_mode                         : 2 ;  // 27:26     
} PE_H15A0_SHPR_PE1_DERH_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800dabcL pe1_derh_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 5 ;  //  4: 0     reserved
	UINT32 a_mux_for_edge_gaintable           : 3 ;  //  7: 5     
	UINT32                                    : 9 ;  // 16: 8     reserved
	UINT32 a_mux_for_detail_filter            : 3 ;  // 19:17     
	UINT32 tedge_mux                          : 1 ;  //    20     
	UINT32 a_mux_for_edge_filter              : 3 ;  // 23:21     
	UINT32 a_scaling_flat_filter_             : 4 ;  // 27:24     
	UINT32                                    : 1 ;  //    28     reserved
	UINT32 a_mux_for_flat_filter              : 3 ;  // 31:29     
} PE_H15A0_SHPR_PE1_DERH_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800dac0L pe1_derh_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 reg_dbg_scale                      : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sum_mux                            : 4 ;  //  7: 4     
	UINT32 reg_enh_en_cc                      : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_enh_en_yy                      : 3 ;  // 14:12     
} PE_H15A0_SHPR_PE1_DERH_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800dac4L pe1_derh_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vmm_param                      : 2 ;  //  1: 0     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_csft_gain                      : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_th_gain_edge                   : 6 ;  // 21:16     
	UINT32 reg_th_gain_flat                   : 2 ;  // 23:22     
	UINT32 reg_th_manual_th                   : 7 ;  // 30:24     
	UINT32 reg_th_manual_en                   : 1 ;  //    31     
} PE_H15A0_SHPR_PE1_DERH_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800dac8L pe1_derh_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 edge_filter_white_gain             : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 edge_filter_black_gain             : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_amean_en                       : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 edge_filter_v_tap                  : 2 ;  // 27:26     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 edge_c_filter_en                   : 1 ;  //    30     
	UINT32 edge_y_filter_en                   : 1 ;  //    31     
} PE_H15A0_SHPR_PE1_DERH_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800daccL pe1_derh_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_filter_tap                     : 2 ;  //  1: 0     
	UINT32                                    : 22;  // 23: 2     reserved
	UINT32 flat_filter_gain                   : 6 ;  // 29:24     
	UINT32 flat_filter_type                   : 1 ;  //    30     
	UINT32 flat_filter_en                     : 1 ;  //    31     
} PE_H15A0_SHPR_PE1_DERH_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800dad0L pe1_derh_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 amod_ctrl0_y0                      : 8 ;  //  7: 0     
	UINT32 amod_ctrl0_x0                      : 8 ;  // 15: 8     
	UINT32 amod_ctrl0_y1                      : 8 ;  // 23:16     
	UINT32 amod_ctrl0_x1                      : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_DERH_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800dad4L pe1_derh_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 amod_ctrl1_x0                      : 8 ;  //  7: 0     
	UINT32 amod_ctrl1_x1                      : 8 ;  // 15: 8     
	UINT32 y_c_mux_control                    : 3 ;  // 18:16     
	UINT32 chroma_weight                      : 5 ;  // 23:19     
} PE_H15A0_SHPR_PE1_DERH_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800dad8L pe1_derh_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a2g_mode_e                         : 1 ;  //     0     
	UINT32 a2g_mode_f                         : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 a2g_mgain_e                        : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 a2g_mgain_f                        : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 debug_mode                         : 4 ;  // 27:24     
} PE_H15A0_SHPR_PE1_DERH_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc800dadcL pe1_derh_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 e_gain_th1                         : 8 ;  //  7: 0     
	UINT32 e_gain_th2                         : 8 ;  // 15: 8     
	UINT32 f_gain_th1                         : 8 ;  // 23:16     
	UINT32 f_gain_th2                         : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_DERH_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc800dae0L pe1_derh_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 e_gain_max                         : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 f_gain_max                         : 6 ;  // 13: 8     
} PE_H15A0_SHPR_PE1_DERH_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc800dae4L pe1_coring_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y2                                 : 8 ;  //  7: 0     
	UINT32 x2                                 : 8 ;  // 15: 8     
	UINT32 y3                                 : 8 ;  // 23:16     
	UINT32 x3                                 : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800daf0L pe1_derv_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 der_v_en                           : 1 ;  //     0     
	UINT32 der_gain_mapping                   : 2 ;  //  2: 1     
	UINT32 bif_en                             : 1 ;  //     3     
	UINT32 output_mux                         : 4 ;  //  7: 4     
	UINT32 bif_manual_th                      : 8 ;  // 15: 8     
	UINT32 th_gain                            : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 th_mode                            : 1 ;  //    24     
} PE_H15A0_SHPR_PE1_DERV_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800daf4L pe1_derv_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csft_gain                          : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 csft_mode                          : 1 ;  //     8     
} PE_H15A0_SHPR_PE1_DERV_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800daf8L pe1_derv_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mmd_sel                            : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 max_sel                            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 avg_sel                            : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 gain_th1                           : 8 ;  // 23:16     
	UINT32 gain_th2                           : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_DERV_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800dafcL pe1_derv_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gain_b                             : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 gain_w                             : 7 ;  // 14: 8     
} PE_H15A0_SHPR_PE1_DERV_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800db10L pe1_dp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_display                      : 3 ;  //  2: 0     
	UINT32                                    : 27;  // 29: 3     reserved
	UINT32 reg_a_gain_balance_en              : 1 ;  //    30     
	UINT32 reg_t_gain_balance_en              : 1 ;  //    31     
} PE_H15A0_SHPR_PE1_DP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800db14L pe1_dp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_gain_b                        : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 edge_gain_w                        : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 texture_gain_b                     : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 texture_gain_w                     : 7 ;  // 30:24     
} PE_H15A0_SHPR_PE1_DP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800db18L pe1_dp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ptic_gain_e_b                  : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_ptic_gain_e_w                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_ptic_gain_t_b                  : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_ptic_gain_t_w                  : 7 ;  // 30:24     
} PE_H15A0_SHPR_PE1_DP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800db1cL pe1_dp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y0                                 : 8 ;  //  7: 0     
	UINT32 x0                                 : 8 ;  // 15: 8     
	UINT32 y1                                 : 8 ;  // 23:16     
	UINT32 x1                                 : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_DP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800db30L pe1_sp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_enhance_enable                : 1 ;  //     0     
	UINT32                                    : 11;  // 11: 1     reserved
	UINT32 edge_operator_selection            : 2 ;  // 13:12     
} PE_H15A0_SHPR_PE1_SP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800db34L pe1_sp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 white_gain                         : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 black_gain                         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 horizontal_gain                    : 8 ;  // 23:16     
	UINT32 vertical_gain                      : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_SP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800db38L pe1_sp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sobel_weight                       : 8 ;  //  7: 0     
	UINT32 laplacian_weight                   : 8 ;  // 15: 8     
	UINT32 sobel_manual_mode_en               : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 sobel_manual_gain                  : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_SP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800db3cL pe1_sp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPR_PE1_SP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800db40L pe1_sp_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 display_mode                       : 4 ;  // 11: 8     
	UINT32 gx_weight_manual_mode              : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 gx_weight_manual_gain              : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_SP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800db44L pe1_sp_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPR_PE1_SP_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800db48L pe1_sp_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lap_h_mode                         : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 lap_v_mode                         : 1 ;  //     4     
} PE_H15A0_SHPR_PE1_SP_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800db4cL pe1_sp_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_en                              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 gb_mode                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 gb_x1                              : 8 ;  // 15: 8     
	UINT32 gb_y1                              : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_SP_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800db50L pe1_sp_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_x2                              : 8 ;  //  7: 0     
	UINT32 gb_y2                              : 8 ;  // 15: 8     
	UINT32 gb_y3                              : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_SP_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800db54L pe1_sp_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_x_l0                          : 8 ;  //  7: 0     
	UINT32 lum1_x_l1                          : 8 ;  // 15: 8     
	UINT32 lum1_x_h0                          : 8 ;  // 23:16     
	UINT32 lum1_x_h1                          : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_SP_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800db58L pe1_sp_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_y0                            : 8 ;  //  7: 0     
	UINT32 lum1_y1                            : 8 ;  // 15: 8     
	UINT32 lum1_y2                            : 8 ;  // 23:16     
	UINT32 lum2_x_l0                          : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_SP_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc800db5cL pe1_sp_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_x_l1                          : 8 ;  //  7: 0     
	UINT32 lum2_x_h0                          : 8 ;  // 15: 8     
	UINT32 lum2_x_h1                          : 8 ;  // 23:16     
	UINT32 lum2_y0                            : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_SP_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc800db60L pe1_sp_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_y1                            : 8 ;  //  7: 0     
	UINT32 lum2_y2                            : 8 ;  // 15: 8     
} PE_H15A0_SHPR_PE1_SP_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc800db70L pe1_mp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_enhance_enable                : 1 ;  //     0     
	UINT32                                    : 11;  // 11: 1     reserved
	UINT32 edge_operator_selection            : 2 ;  // 13:12     
} PE_H15A0_SHPR_PE1_MP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800db74L pe1_mp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 white_gain                         : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 black_gain                         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 horizontal_gain                    : 8 ;  // 23:16     
	UINT32 vertical_gain                      : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_MP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800db78L pe1_mp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sobel_weight                       : 8 ;  //  7: 0     
	UINT32 laplacian_weight                   : 8 ;  // 15: 8     
	UINT32 sobel_manual_mode_en               : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 sobel_manual_gain                  : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_MP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800db7cL pe1_mp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPR_PE1_MP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800db80L pe1_mp_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 display_mode                       : 4 ;  // 11: 8     
	UINT32 gx_weight_manual_mode              : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 gx_weight_manual_gain              : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_MP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800db84L pe1_mp_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPR_PE1_MP_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800db88L pe1_mp_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lap_h_mode                         : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 lap_v_mode                         : 3 ;  //  6: 4     
} PE_H15A0_SHPR_PE1_MP_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800db8cL pe1_mp_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_en                              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 gb_mode                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 gb_x1                              : 8 ;  // 15: 8     
	UINT32 gb_y1                              : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_MP_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800db90L pe1_mp_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_x2                              : 8 ;  //  7: 0     
	UINT32 gb_y2                              : 8 ;  // 15: 8     
	UINT32 gb_y3                              : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_MP_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800db94L pe1_mp_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_x_l0                          : 8 ;  //  7: 0     
	UINT32 lum1_x_l1                          : 8 ;  // 15: 8     
	UINT32 lum1_x_h0                          : 8 ;  // 23:16     
	UINT32 lum1_x_h1                          : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_MP_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800db98L pe1_mp_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_y0                            : 8 ;  //  7: 0     
	UINT32 lum1_y1                            : 8 ;  // 15: 8     
	UINT32 lum1_y2                            : 8 ;  // 23:16     
	UINT32 lum2_x_l0                          : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_MP_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc800db9cL pe1_mp_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_x_l1                          : 8 ;  //  7: 0     
	UINT32 lum2_x_h0                          : 8 ;  // 15: 8     
	UINT32 lum2_x_h1                          : 8 ;  // 23:16     
	UINT32 lum2_y0                            : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_MP_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc800dba0L pe1_mp_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_y1                            : 8 ;  //  7: 0     
	UINT32 lum2_y2                            : 8 ;  // 15: 8     
} PE_H15A0_SHPR_PE1_MP_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc800dbb0L pe1_coring_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_map_h_size                   : 2 ;  //  1: 0     
	UINT32 reg_a_nomalize_en                  : 1 ;  //     2     
	UINT32 reg_t_nomalize_en                  : 1 ;  //     3     
	UINT32 reg_t_cut_resolution               : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 reg_cross_th_sc                    : 8 ;  // 23:16     
	UINT32 reg_cross_th                       : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dbb4L pe1_coring_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_detail_beta_mul                : 8 ;  //  7: 0     
	UINT32 reg_detail_alpha_mul               : 8 ;  // 15: 8     
	UINT32 reg_edge_beta_mul                  : 8 ;  // 23:16     
	UINT32 reg_edge_alpha_mul                 : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dbb8L pe1_coring_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_edge_y0                  : 8 ;  //  7: 0     
	UINT32 reg_a_lut_edge_x0                  : 8 ;  // 15: 8     
	UINT32 reg_a_lut_edge_y1                  : 8 ;  // 23:16     
	UINT32 reg_a_lut_edge_x1                  : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800dbbcL pe1_coring_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_detail_y0                : 8 ;  //  7: 0     
	UINT32 reg_a_lut_detail_x0                : 8 ;  // 15: 8     
	UINT32 reg_a_lut_detail_y1                : 8 ;  // 23:16     
	UINT32 reg_a_lut_detail_x1                : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800dbc0L pe1_coring_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_edge_y0                  : 8 ;  //  7: 0     
	UINT32 reg_t_lut_edge_x0                  : 8 ;  // 15: 8     
	UINT32 reg_t_lut_edge_y1                  : 8 ;  // 23:16     
	UINT32 reg_t_lut_edge_x1                  : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800dbc4L pe1_coring_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_detail_y0                : 8 ;  //  7: 0     
	UINT32 reg_t_lut_detail_x0                : 8 ;  // 15: 8     
	UINT32 reg_t_lut_detail_y1                : 8 ;  // 23:16     
	UINT32 reg_t_lut_detail_x1                : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800dbc8L pe1_coring_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_edge_minmax_y0                 : 8 ;  //  7: 0     
	UINT32 reg_edge_minmax_x0                 : 8 ;  // 15: 8     
	UINT32 reg_edge_minmax_y1                 : 8 ;  // 23:16     
	UINT32 reg_edge_minmax_x1                 : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800dbccL pe1_coring_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_detail_minmax_y0               : 8 ;  //  7: 0     
	UINT32 reg_detail_minmax_x0               : 8 ;  // 15: 8     
	UINT32 reg_detail_minmax_y1               : 8 ;  // 23:16     
	UINT32 reg_detail_minmax_x1               : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800dbd0L pe1_coring_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mp_coring_mode                     : 2 ;  //  1: 0     
	UINT32 sp_coring_mode                     : 2 ;  //  3: 2     
	UINT32 mp_coring_en                       : 1 ;  //     4     
	UINT32 sp_coring_en                       : 1 ;  //     5     
	UINT32 pti_coring_en                      : 1 ;  //     6     
	UINT32 pti_coring_mode                    : 2 ;  //  8: 7     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_mul_base_t                     : 8 ;  // 23:16     
	UINT32 reg_mul_base_e                     : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800dbd4L pe1_coring_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_gain_b                        : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 edge_gain_w                        : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 texture_gain_b                     : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 texture_gain_w                     : 7 ;  // 30:24     
} PE_H15A0_SHPR_PE1_CORING_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc800dbe0L pe1_coring_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_map_frame_sum                    : 32;  // 31: 0
} PE_H15A0_SHPR_PE1_CORING_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc800dbe4L pe1_coring_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 t_map_frame_sum                    : 32;  // 31: 0
} PE_H15A0_SHPR_PE1_CORING_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc800dbe8L pe1_coring_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_frame_sum                     : 32;  // 31: 0
} PE_H15A0_SHPR_PE1_CORING_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc800dbecL pe1_coring_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 detail_frame_sum                   : 32;  // 31: 0
} PE_H15A0_SHPR_PE1_CORING_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc800dc00L pe1_pti_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ti_enable                      : 1 ;  //     0     
	UINT32 reg_debug_map_en                   : 1 ;  //     1     
	UINT32 reg_mm_tap_size                    : 2 ;  //  3: 2     
	UINT32 reg_avg_tap_size                   : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_master_gain                    : 8 ;  // 15: 8     
} PE_H15A0_SHPR_PE1_PTI_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dc04L pe1_pti_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ti_enable                      : 1 ;  //     0     
	UINT32 reg_debug_map_en                   : 1 ;  //     1     
	UINT32 reg_mm_tap_size                    : 2 ;  //  3: 2     
	UINT32 reg_avg_tap_size                   : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_master_gain                    : 8 ;  // 15: 8     
} PE_H15A0_SHPR_PE1_PTI_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dc08L pe1_sti_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ti_enable                      : 1 ;  //     0     
	UINT32 reg_debug_map_en                   : 1 ;  //     1     
	UINT32 reg_mm_tap_size                    : 2 ;  //  3: 2     
	UINT32 reg_avg_tap_size                   : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_master_gain                    : 8 ;  // 15: 8     
	UINT32 reg_texture_gain                   : 8 ;  // 23:16     
	UINT32 reg_edge_gain                      : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_STI_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dc0cL pe1_sti_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_SHPR_PE1_STI_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dc20L pe1_dj_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edf_en                             : 1 ;  //     0     
	UINT32 hv_filter_en                       : 1 ;  //     1     
	UINT32 g0_feature_norm_en                 : 1 ;  //     2     
	UINT32 g0_f2g_mapping_en                  : 1 ;  //     3     
	UINT32 line_variation_mode                : 1 ;  //     4     
	UINT32 l_type_protection                  : 1 ;  //     5     
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 direction_type_mode                : 1 ;  //     7     
	UINT32 count_diff_th                      : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 output_mux                         : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 center_blur_mode                   : 2 ;  // 21:20     
	UINT32 n_avg_mode                         : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 line_variation_diff_threshold      : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_DJ_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dc24L pe1_dj_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 level_th                           : 8 ;  //  7: 0     
	UINT32 protect_th                         : 8 ;  // 15: 8     
	UINT32 n_avg_gain                         : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_DJ_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dc28L pe1_dj_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g0_cnt_min                     : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_g0_mul                         : 5 ;  // 12: 8     
} PE_H15A0_SHPR_PE1_DJ_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800dc2cL pe1_dj_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g1_protect_min                 : 8 ;  //  7: 0     
	UINT32 reg_g1_mul                         : 7 ;  // 14: 8     
} PE_H15A0_SHPR_PE1_DJ_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800dc30L pe1_cti_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 filter_tap_size                    : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8     
} PE_H15A0_SHPR_PE1_CTI_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dc34L pe1_cti_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 coring_th0                         : 8 ;  //  7: 0     
	UINT32 coring_th1                         : 8 ;  // 15: 8     
	UINT32 coring_map_filter                  : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 coring_tap_size                    : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 debug_mode                         : 2 ;  // 25:24     
} PE_H15A0_SHPR_PE1_CTI_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dc38L pe1_cti_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ycm_en0                            : 1 ;  //     0     
	UINT32 ycm_en1                            : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 ycm_band_sel                       : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 ycm_diff_th                        : 8 ;  // 15: 8     
	UINT32 ycm_y_gain                         : 4 ;  // 19:16     
	UINT32 ycm_c_gain                         : 4 ;  // 23:20     
} PE_H15A0_SHPR_PE1_CTI_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800dc50L pe1_debug_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_debug_mode                  : 1 ;  //     0     
	UINT32 status_enable                      : 1 ;  //     1     
	UINT32 debug_mode                         : 1 ;  //     2     
	UINT32 show_dce_blur                      : 1 ;  //     3     
	UINT32                                    : 5 ;  //  8: 4     reserved
	UINT32 show_sqm2                          : 1 ;  //     9     
	UINT32 sqm2_debug_image                   : 4 ;  // 13:10     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 show_bbd                           : 2 ;  // 16:15     
	UINT32 bbd_line_width                     : 3 ;  // 19:17     
	UINT32 show_apl                           : 1 ;  //    20     
} PE_H15A0_SHPR_PE1_DEBUG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dc54L pe1_debug_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 force_yy_enable                    : 1 ;  //     1     
	UINT32 force_cb_enable                    : 1 ;  //     2     
	UINT32 force_cr_enable                    : 1 ;  //     3     
	UINT32 yc_debug_mode_sel                  : 3 ;  //  6: 4     
} PE_H15A0_SHPR_PE1_DEBUG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dc58L pe1_debug_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 10;  //  9: 0     reserved
	UINT32 force_cb                           : 10;  // 19:10     
	UINT32 force_cr                           : 10;  // 29:20     
} PE_H15A0_SHPR_PE1_DEBUG_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800dc60L pe1_sqm_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 outmux_debugmap                    : 4 ;  //  3: 0     
	UINT32 iir                                : 4 ;  //  7: 4     
	UINT32 i_score                            : 8 ;  // 15: 8     
	UINT32 v_score                            : 8 ;  // 23:16     
	UINT32 l_score                            : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_SQM_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dc64L pe1_sqm_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_limit                            : 8 ;  //  7: 0     
	UINT32 l_limit                            : 8 ;  // 15: 8     
	UINT32 score_threshold                    : 8 ;  // 23:16     
	UINT32 flat_threshold                     : 8 ;  // 31:24     
} PE_H15A0_SHPR_PE1_SQM_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dc68L pe1_sqm_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_x0                             : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 win_y0                             : 11;  // 26:16     
} PE_H15A0_SHPR_PE1_SQM_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800dc6cL pe1_sqm_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_x1                             : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 win_y1                             : 11;  // 26:16     
} PE_H15A0_SHPR_PE1_SQM_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800dc70L pe1_sqm_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 charcter_delta_limit               : 8 ;  //  7: 0     
	UINT32 boundary_width                     : 8 ;  // 15: 8     
	UINT32 video_height                       : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_SQM_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800dc74L pe1_sqm_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pc_threshold                       : 8 ;  //  7: 0     
	UINT32 pc_delta_limit                     : 8 ;  // 15: 8     
	UINT32 pc_pixel_minimal_limit             : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_SQM_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800dc78L pe1_sqm_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 blur_pixel                         : 10;  //  9: 0     
	UINT32 sharp_pixel                        : 10;  // 19:10     
	UINT32 sqm_output                         : 10;  // 29:20     
	UINT32 pc_detect                          : 1 ;  //    30     
} PE_H15A0_SHPR_PE1_SQM_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc800dc80L pe1_bbd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bbd_en                         : 1 ;  //     0     
	UINT32 reg_hys_mode                       : 5 ;  //  5: 1     
	UINT32 reg_op_mode                        : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cnt_th                         : 8 ;  // 15: 8     
	UINT32 reg_diff_th                        : 8 ;  // 23:16     
} PE_H15A0_SHPR_PE1_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dc84L pe1_bbd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_x0                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 bbd_y0                             : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dc88L pe1_bbd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_x1                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 bbd_y1                             : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800dc8cL pe1_bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc800dc90L pe1_bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H15A0_SHPR_PE1_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc800dca0L pe1_tgen_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tgen_on_off                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 tgen_initial_seed_mode             : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 tgen_table_sel                     : 4 ;  // 19:16     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc800dca4L pe1_tgen_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_delta_h_bound                   : 5 ;  //  4: 0     
	UINT32                                    : 4 ;  //  8: 5     reserved
	UINT32 tg_delta_l_bound                   : 4 ;  // 12: 9     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 tg_delta_max                       : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 tg_delta_scale                     : 3 ;  // 26:24     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc800dca8L pe1_tgen_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_rnd_th                          : 8 ;  //  7: 0     
	UINT32 tg_delta_wcurr                     : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 tg_lc_ldr_th                       : 6 ;  // 21:16     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800dcacL pe1_tgen_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 rand_x0                            : 32;  // 31: 0     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc800dcb0L pe1_tgen_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_dr_ratio                        : 3 ;  //  2: 0     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 tg_dr_th                           : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 tg_mm_offset                       : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 tg_mm_sel_mode                     : 2 ;  // 25:24     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc800dcb4L pe1_tgen_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_lavg_th                         : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 tg_gs_rnd                          : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 tg_gs_mm                           : 5 ;  // 28:24     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc800dcb8L pe1_tgen_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_blur_coef_x2                    : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 tg_blur_coef_x1                    : 6 ;  // 13: 8     
	UINT32                                    : 10;  // 23:14     reserved
	UINT32 tg_blur_coef_x0                    : 6 ;  // 29:24     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800dcbcL pe1_tgen_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_blur_coef_y2                    : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 tg_blur_coef_y1                    : 6 ;  // 13: 8     
	UINT32                                    : 10;  // 23:14     reserved
	UINT32 tg_blur_coef_y0                    : 6 ;  // 29:24     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc800dcc0L pe1_tgen_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_map_edge_gain                   : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 tg_map_detail_gain                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 tg_map_offset                      : 8 ;  // 23:16     
	UINT32 tg_gs_noise                        : 6 ;  // 29:24     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc800dcc4L pe1_tgen_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tg_gain7                           : 4 ;  //  3: 0     
	UINT32 tg_gain6                           : 4 ;  //  7: 4     
	UINT32 tg_gain5                           : 4 ;  // 11: 8     
	UINT32 tg_gain4                           : 4 ;  // 15:12     
	UINT32 tg_gain3                           : 4 ;  // 19:16     
	UINT32 tg_gain2                           : 4 ;  // 23:20     
	UINT32 tg_gain1                           : 4 ;  // 27:24     
	UINT32 tg_gain0                           : 4 ;  // 31:28     
} PE_H15A0_SHPR_PE1_TGEN_CTRL_09_T;

typedef struct {
	PE_H15A0_SHP_SHP_TOP_CTRL_00_T        	shp_top_ctrl_00;          //0xc800d000L
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR00_T 	e_disp_sync_dly_addr00;   //0xc800d004L
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR01_T 	e_disp_sync_dly_addr01;   //0xc800d008L
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR02_T 	e_disp_sync_dly_addr02;   //0xc800d00cL
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR03_T 	e_disp_sync_dly_addr03;   //0xc800d010L
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR04_T 	e_disp_sync_dly_addr04;   //0xc800d014L
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR05_T 	e_disp_sync_dly_addr05;   //0xc800d018L
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR06_T 	e_disp_sync_dly_addr06;   //0xc800d01cL
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR07_T 	e_disp_sync_dly_addr07;   //0xc800d020L
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR08_T 	e_disp_sync_dly_addr08;   //0xc800d024L
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR09_T 	e_disp_sync_dly_addr09;   //0xc800d028L
	PE_H15A0_SHP_E_DISP_SYNC_DLY_ADDR0A_T 	e_disp_sync_dly_addr0a;   //0xc800d02cL
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR00_T 	o_disp_sync_dly_addr00;   //0xc800d030L
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR01_T 	o_disp_sync_dly_addr01;   //0xc800d034L
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR02_T 	o_disp_sync_dly_addr02;   //0xc800d038L
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR03_T 	o_disp_sync_dly_addr03;   //0xc800d03cL
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR04_T 	o_disp_sync_dly_addr04;   //0xc800d040L
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR05_T 	o_disp_sync_dly_addr05;   //0xc800d044L
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR06_T 	o_disp_sync_dly_addr06;   //0xc800d048L
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR07_T 	o_disp_sync_dly_addr07;   //0xc800d04cL
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR08_T 	o_disp_sync_dly_addr08;   //0xc800d050L
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR09_T 	o_disp_sync_dly_addr09;   //0xc800d054L
	PE_H15A0_SHP_O_DISP_SYNC_DLY_ADDR0A_T 	o_disp_sync_dly_addr0a;   //0xc800d058L
	PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_00_T   	pre_pmux_e_y_ctrl_00;     //0xc800d05cL
	PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_01_T   	pre_pmux_e_y_ctrl_01;     //0xc800d060L
	PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_02_T   	pre_pmux_e_y_ctrl_02;     //0xc800d064L
	PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_03_T   	pre_pmux_e_y_ctrl_03;     //0xc800d068L
	PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_04_T   	pre_pmux_e_y_ctrl_04;     //0xc800d06cL
	PE_H15A0_SHP_PRE_PMUX_E_Y_CTRL_05_T   	pre_pmux_e_y_ctrl_05;     //0xc800d070L
	PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_00_T   	pre_pmux_e_c_ctrl_00;     //0xc800d074L
	PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_01_T   	pre_pmux_e_c_ctrl_01;     //0xc800d078L
	PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_02_T   	pre_pmux_e_c_ctrl_02;     //0xc800d07cL
	PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_03_T   	pre_pmux_e_c_ctrl_03;     //0xc800d080L
	PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_04_T   	pre_pmux_e_c_ctrl_04;     //0xc800d084L
	PE_H15A0_SHP_PRE_PMUX_E_C_CTRL_05_T   	pre_pmux_e_c_ctrl_05;     //0xc800d088L
	PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_00_T   	pre_pmux_o_y_ctrl_00;     //0xc800d08cL
	PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_01_T   	pre_pmux_o_y_ctrl_01;     //0xc800d090L
	PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_02_T   	pre_pmux_o_y_ctrl_02;     //0xc800d094L
	PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_03_T   	pre_pmux_o_y_ctrl_03;     //0xc800d098L
	PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_04_T   	pre_pmux_o_y_ctrl_04;     //0xc800d09cL
	PE_H15A0_SHP_PRE_PMUX_O_Y_CTRL_05_T   	pre_pmux_o_y_ctrl_05;     //0xc800d0a0L
	PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_00_T   	pre_pmux_o_c_ctrl_00;     //0xc800d0a4L
	PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_01_T   	pre_pmux_o_c_ctrl_01;     //0xc800d0a8L
	PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_02_T   	pre_pmux_o_c_ctrl_02;     //0xc800d0acL
	PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_03_T   	pre_pmux_o_c_ctrl_03;     //0xc800d0b0L
	PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_04_T   	pre_pmux_o_c_ctrl_04;     //0xc800d0b4L
	PE_H15A0_SHP_PRE_PMUX_O_C_CTRL_05_T   	pre_pmux_o_c_ctrl_05;     //0xc800d0b8L
	PE_H15A0_SHP_EO2LR_Y_CTRL_T           	eo2lr_y_ctrl;             //0xc800d0bcL
	PE_H15A0_SHP_EO2LR_C_CTRL_T           	eo2lr_c_ctrl;             //0xc800d0c0L
	PE_H15A0_SHP_LR2EO_CTRL_T             	lr2eo_ctrl;               //0xc800d0c4L
	PE_H15A0_SHP_POST_PMUX_E_CTRL_00_T    	post_pmux_e_ctrl_00;      //0xc800d0c8L
	PE_H15A0_SHP_POST_PMUX_E_CTRL_01_T    	post_pmux_e_ctrl_01;      //0xc800d0ccL
	PE_H15A0_SHP_POST_PMUX_E_CTRL_02_T    	post_pmux_e_ctrl_02;      //0xc800d0d0L
	PE_H15A0_SHP_POST_PMUX_E_CTRL_03_T    	post_pmux_e_ctrl_03;      //0xc800d0d4L
	PE_H15A0_SHP_POST_PMUX_E_CTRL_04_T    	post_pmux_e_ctrl_04;      //0xc800d0d8L
	PE_H15A0_SHP_POST_PMUX_E_CTRL_05_T    	post_pmux_e_ctrl_05;      //0xc800d0dcL
	PE_H15A0_SHP_POST_PMUX_O_CTRL_00_T    	post_pmux_o_ctrl_00;      //0xc800d0e0L
	PE_H15A0_SHP_POST_PMUX_O_CTRL_01_T    	post_pmux_o_ctrl_01;      //0xc800d0e4L
	PE_H15A0_SHP_POST_PMUX_O_CTRL_02_T    	post_pmux_o_ctrl_02;      //0xc800d0e8L
	PE_H15A0_SHP_POST_PMUX_O_CTRL_03_T    	post_pmux_o_ctrl_03;      //0xc800d0ecL
	PE_H15A0_SHP_POST_PMUX_O_CTRL_04_T    	post_pmux_o_ctrl_04;      //0xc800d0f0L
	PE_H15A0_SHP_POST_PMUX_O_CTRL_05_T    	post_pmux_o_ctrl_05;      //0xc800d0f4L
}PE_SHP_TOP_REG_H15A0_T;

typedef struct {
	PE_H15A0_SHPL_PE1_TOP_CTRL_00_T       	pe1_top_ctrl_00;         //0xc800d400L
	PE_H15A0_SHPL_PE1_TOP_CTRL_01_T       	pe1_top_ctrl_01;         //0xc800d404L
	PE_H15A0_SHPL_PE1_TOP_CTRL_02_T       	pe1_top_ctrl_02;         //0xc800d408L
	PE_H15A0_SHPL_PE1_TOP_CTRL_03_T       	pe1_top_ctrl_03;         //0xc800d40cL
	PE_H15A0_SHPL_PE1_TOP_CTRL_04_T       	pe1_top_ctrl_04;         //0xc800d410L
	UINT32                             	    reserved00[3];           //0xc800d414L~0xc800d41cL
	PE_H15A0_SHPL_PE1_WIN1_CTRL_00_T      	pe1_win1_ctrl_00;        //0xc800d420L
	PE_H15A0_SHPL_PE1_WIN1_CTRL_01_T      	pe1_win1_ctrl_01;        //0xc800d424L
	PE_H15A0_SHPL_PE1_WIN1_CTRL_02_T      	pe1_win1_ctrl_02;        //0xc800d428L
	PE_H15A0_SHPL_PE1_WIN1_CTRL_03_T      	pe1_win1_ctrl_03;        //0xc800d42cL
	PE_H15A0_SHPL_PE1_WIN1_CTRL_04_T      	pe1_win1_ctrl_04;        //0xc800d430L
	UINT32                             	    reserved01[3];           //0xc800d434L~0xc800d43cL
	PE_H15A0_SHPL_PE1_MSC_M1_Y_CTRL_T       pe1_msc_m1_y_ctrl;       //0xc800d440L
	PE_H15A0_SHPL_PE1_MSC_M2_Y_CTRL_T       pe1_msc_m2_y_ctrl;       //0xc800d444L
	PE_H15A0_SHPL_PE1_MSC_M1_C_CTRL_T       pe1_msc_m1_c_ctrl;       //0xc800d448L
	PE_H15A0_SHPL_PE1_MSC_M2_C_CTRL_T       pe1_msc_m2_c_ctrl;       //0xc800d44cL
	UINT32                             	    reserved02[16];          //0xc800d450L~0xc800d48cL
	PE_H15A0_SHPL_PE1_WIN3_CTRL_00_T      	pe1_win3_ctrl_00;        //0xc800d490L
	PE_H15A0_SHPL_PE1_WIN3_CTRL_01_T      	pe1_win3_ctrl_01;        //0xc800d494L
	PE_H15A0_SHPL_PE1_WIN3_CTRL_02_T      	pe1_win3_ctrl_02;        //0xc800d498L
	PE_H15A0_SHPL_PE1_WIN3_CTRL_03_T      	pe1_win3_ctrl_03;        //0xc800d49cL
	PE_H15A0_SHPL_PE1_WIN3_CTRL_04_T      	pe1_win3_ctrl_04;        //0xc800d4a0L
	UINT32                             	    reserved03[3];           //0xc800d4a4L~0xc800d4acL
	PE_H15A0_SHPL_PE1_DERH_CTRL_00_T      	pe1_derh_ctrl_00;        //0xc800d4b0L
	PE_H15A0_SHPL_PE1_DERH_CTRL_01_T      	pe1_derh_ctrl_01;        //0xc800d4b4L
	PE_H15A0_SHPL_PE1_DERH_CTRL_02_T      	pe1_derh_ctrl_02;        //0xc800d4b8L
	PE_H15A0_SHPL_PE1_DERH_CTRL_03_T      	pe1_derh_ctrl_03;        //0xc800d4bcL
	PE_H15A0_SHPL_PE1_DERH_CTRL_04_T      	pe1_derh_ctrl_04;        //0xc800d4c0L
	PE_H15A0_SHPL_PE1_DERH_CTRL_05_T      	pe1_derh_ctrl_05;        //0xc800d4c4L
	PE_H15A0_SHPL_PE1_DERH_CTRL_06_T      	pe1_derh_ctrl_06;        //0xc800d4c8L
	PE_H15A0_SHPL_PE1_DERH_CTRL_07_T      	pe1_derh_ctrl_07;        //0xc800d4ccL
	PE_H15A0_SHPL_PE1_DERH_CTRL_08_T      	pe1_derh_ctrl_08;        //0xc800d4d0L
	PE_H15A0_SHPL_PE1_DERH_CTRL_09_T      	pe1_derh_ctrl_09;        //0xc800d4d4L
	PE_H15A0_SHPL_PE1_DERH_CTRL_0A_T      	pe1_derh_ctrl_0a;        //0xc800d4d8L
	PE_H15A0_SHPL_PE1_DERH_CTRL_0B_T      	pe1_derh_ctrl_0b;        //0xc800d4dcL
	PE_H15A0_SHPL_PE1_DERH_CTRL_0C_T      	pe1_derh_ctrl_0c;        //0xc800d4e0L
	PE_H15A0_SHPL_PE1_CORING_CTRL_10_T    	pe1_coring_ctrl_10;      //0xc800d4e4L
	UINT32                             	    reserved04[2];           //0xc800d4e8L~0xc800d4ecL
	PE_H15A0_SHPL_PE1_DERV_CTRL_0_T       	pe1_derv_ctrl_0;         //0xc800d4f0L
	PE_H15A0_SHPL_PE1_DERV_CTRL_1_T       	pe1_derv_ctrl_1;         //0xc800d4f4L
	PE_H15A0_SHPL_PE1_DERV_CTRL_2_T       	pe1_derv_ctrl_2;         //0xc800d4f8L
	PE_H15A0_SHPL_PE1_DERV_CTRL_3_T       	pe1_derv_ctrl_3;         //0xc800d4fcL
	UINT32                             	    reserved05[4];           //0xc800d500L~0xc800d50cL
	PE_H15A0_SHPL_PE1_DP_CTRL_00_T        	pe1_dp_ctrl_00;          //0xc800d510L
	PE_H15A0_SHPL_PE1_DP_CTRL_01_T        	pe1_dp_ctrl_01;          //0xc800d514L
	PE_H15A0_SHPL_PE1_DP_CTRL_02_T        	pe1_dp_ctrl_02;          //0xc800d518L
	PE_H15A0_SHPL_PE1_DP_CTRL_03_T        	pe1_dp_ctrl_03;          //0xc800d51cL
	UINT32                             	    reserved06[4];           //0xc800d520L~0xc800d52cL
	PE_H15A0_SHPL_PE1_SP_CTRL_00_T        	pe1_sp_ctrl_00;          //0xc800d530L
	PE_H15A0_SHPL_PE1_SP_CTRL_01_T        	pe1_sp_ctrl_01;          //0xc800d534L
	PE_H15A0_SHPL_PE1_SP_CTRL_02_T        	pe1_sp_ctrl_02;          //0xc800d538L
	PE_H15A0_SHPL_PE1_SP_CTRL_03_T        	pe1_sp_ctrl_03;          //0xc800d53cL
	PE_H15A0_SHPL_PE1_SP_CTRL_04_T        	pe1_sp_ctrl_04;          //0xc800d540L
	PE_H15A0_SHPL_PE1_SP_CTRL_05_T        	pe1_sp_ctrl_05;          //0xc800d544L
	PE_H15A0_SHPL_PE1_SP_CTRL_06_T        	pe1_sp_ctrl_06;          //0xc800d548L
	PE_H15A0_SHPL_PE1_SP_CTRL_07_T        	pe1_sp_ctrl_07;          //0xc800d54cL
	PE_H15A0_SHPL_PE1_SP_CTRL_08_T        	pe1_sp_ctrl_08;          //0xc800d550L
	PE_H15A0_SHPL_PE1_SP_CTRL_09_T        	pe1_sp_ctrl_09;          //0xc800d554L
	PE_H15A0_SHPL_PE1_SP_CTRL_0A_T        	pe1_sp_ctrl_0a;          //0xc800d558L
	PE_H15A0_SHPL_PE1_SP_CTRL_0B_T        	pe1_sp_ctrl_0b;          //0xc800d55cL
	PE_H15A0_SHPL_PE1_SP_CTRL_0C_T        	pe1_sp_ctrl_0c;          //0xc800d560L
	UINT32                             	    reserved07[3];           //0xc800d564L~0xc800d56cL
	PE_H15A0_SHPL_PE1_MP_CTRL_00_T        	pe1_mp_ctrl_00;          //0xc800d570L
	PE_H15A0_SHPL_PE1_MP_CTRL_01_T        	pe1_mp_ctrl_01;          //0xc800d574L
	PE_H15A0_SHPL_PE1_MP_CTRL_02_T        	pe1_mp_ctrl_02;          //0xc800d578L
	PE_H15A0_SHPL_PE1_MP_CTRL_03_T        	pe1_mp_ctrl_03;          //0xc800d57cL
	PE_H15A0_SHPL_PE1_MP_CTRL_04_T        	pe1_mp_ctrl_04;          //0xc800d580L
	PE_H15A0_SHPL_PE1_MP_CTRL_05_T        	pe1_mp_ctrl_05;          //0xc800d584L
	PE_H15A0_SHPL_PE1_MP_CTRL_06_T        	pe1_mp_ctrl_06;          //0xc800d588L
	PE_H15A0_SHPL_PE1_MP_CTRL_07_T        	pe1_mp_ctrl_07;          //0xc800d58cL
	PE_H15A0_SHPL_PE1_MP_CTRL_08_T        	pe1_mp_ctrl_08;          //0xc800d590L
	PE_H15A0_SHPL_PE1_MP_CTRL_09_T        	pe1_mp_ctrl_09;          //0xc800d594L
	PE_H15A0_SHPL_PE1_MP_CTRL_0A_T        	pe1_mp_ctrl_0a;          //0xc800d598L
	PE_H15A0_SHPL_PE1_MP_CTRL_0B_T        	pe1_mp_ctrl_0b;          //0xc800d59cL
	PE_H15A0_SHPL_PE1_MP_CTRL_0C_T        	pe1_mp_ctrl_0c;          //0xc800d5a0L
	UINT32                             	    reserved08[3];           //0xc800d5a4L~0xc800d5acL
	PE_H15A0_SHPL_PE1_CORING_CTRL_00_T    	pe1_coring_ctrl_00;      //0xc800d5b0L
	PE_H15A0_SHPL_PE1_CORING_CTRL_01_T    	pe1_coring_ctrl_01;      //0xc800d5b4L
	PE_H15A0_SHPL_PE1_CORING_CTRL_02_T    	pe1_coring_ctrl_02;      //0xc800d5b8L
	PE_H15A0_SHPL_PE1_CORING_CTRL_03_T    	pe1_coring_ctrl_03;      //0xc800d5bcL
	PE_H15A0_SHPL_PE1_CORING_CTRL_04_T    	pe1_coring_ctrl_04;      //0xc800d5c0L
	PE_H15A0_SHPL_PE1_CORING_CTRL_05_T    	pe1_coring_ctrl_05;      //0xc800d5c4L
	PE_H15A0_SHPL_PE1_CORING_CTRL_06_T    	pe1_coring_ctrl_06;      //0xc800d5c8L
	PE_H15A0_SHPL_PE1_CORING_CTRL_07_T    	pe1_coring_ctrl_07;      //0xc800d5ccL
	PE_H15A0_SHPL_PE1_CORING_CTRL_08_T    	pe1_coring_ctrl_08;      //0xc800d5d0L
	PE_H15A0_SHPL_PE1_CORING_CTRL_09_T    	pe1_coring_ctrl_09;      //0xc800d5d4L
	UINT32                             	    reserved09[2];           //0xc800d5d8L~0xc800d5dcL
	PE_H15A0_SHPL_PE1_CORING_STAT_00_T    	pe1_coring_stat_00;      //0xc800d5e0L
	PE_H15A0_SHPL_PE1_CORING_STAT_01_T    	pe1_coring_stat_01;      //0xc800d5e4L
	PE_H15A0_SHPL_PE1_CORING_STAT_02_T    	pe1_coring_stat_02;      //0xc800d5e8L
	PE_H15A0_SHPL_PE1_CORING_STAT_03_T    	pe1_coring_stat_03;      //0xc800d5ecL
	UINT32                             	    reserved10[4];           //0xc800d5f0L~0xc800d5fcL
	PE_H15A0_SHPL_PE1_PTI_CTRL_00_T       	pe1_pti_ctrl_00;         //0xc800d600L
	PE_H15A0_SHPL_PE1_PTI_CTRL_01_T       	pe1_pti_ctrl_01;         //0xc800d604L
	PE_H15A0_SHPL_PE1_STI_CTRL_00_T       	pe1_sti_ctrl_00;         //0xc800d608L
	PE_H15A0_SHPL_PE1_STI_CTRL_01_T       	pe1_sti_ctrl_01;         //0xc800d60cL
	UINT32                             	    reserved11[4];           //0xc800d610L~0xc800d61cL
	PE_H15A0_SHPL_PE1_DJ_CTRL_00_T        	pe1_dj_ctrl_00;          //0xc800d620L
	PE_H15A0_SHPL_PE1_DJ_CTRL_01_T        	pe1_dj_ctrl_01;          //0xc800d624L
	PE_H15A0_SHPL_PE1_DJ_CTRL_02_T        	pe1_dj_ctrl_02;          //0xc800d628L
	PE_H15A0_SHPL_PE1_DJ_CTRL_03_T        	pe1_dj_ctrl_03;          //0xc800d62cL
	PE_H15A0_SHPL_PE1_CTI_CTRL_00_T       	pe1_cti_ctrl_00;         //0xc800d630L
	PE_H15A0_SHPL_PE1_CTI_CTRL_01_T       	pe1_cti_ctrl_01;         //0xc800d634L
	PE_H15A0_SHPL_PE1_CTI_CTRL_02_T       	pe1_cti_ctrl_02;         //0xc800d638L
	UINT32                             	    reserved12[5];           //0xc800d63cL~0xc800d64cL
	PE_H15A0_SHPL_PE1_DEBUG_CTRL_00_T     	pe1_debug_ctrl_00;       //0xc800d650L
	PE_H15A0_SHPL_PE1_DEBUG_CTRL_01_T     	pe1_debug_ctrl_01;       //0xc800d654L
	PE_H15A0_SHPL_PE1_DEBUG_CTRL_02_T     	pe1_debug_ctrl_02;       //0xc800d658L
	UINT32                             	    reserved13;              //0xc800d65cL
	PE_H15A0_SHPL_PE1_SQM_CTRL_00_T       	pe1_sqm_ctrl_00;         //0xc800d660L
	PE_H15A0_SHPL_PE1_SQM_CTRL_01_T       	pe1_sqm_ctrl_01;         //0xc800d664L
	PE_H15A0_SHPL_PE1_SQM_CTRL_02_T       	pe1_sqm_ctrl_02;         //0xc800d668L
	PE_H15A0_SHPL_PE1_SQM_CTRL_03_T       	pe1_sqm_ctrl_03;         //0xc800d66cL
	PE_H15A0_SHPL_PE1_SQM_CTRL_04_T       	pe1_sqm_ctrl_04;         //0xc800d670L
	PE_H15A0_SHPL_PE1_SQM_CTRL_05_T       	pe1_sqm_ctrl_05;         //0xc800d674L
	PE_H15A0_SHPL_PE1_SQM_STAT_00_T       	pe1_sqm_stat_00;         //0xc800d678L
	UINT32                             	    reserved14;              //0xc800d67cL
	PE_H15A0_SHPL_PE1_BBD_CTRL_00_T       	pe1_bbd_ctrl_00;         //0xc800d680L
	PE_H15A0_SHPL_PE1_BBD_CTRL_01_T       	pe1_bbd_ctrl_01;         //0xc800d684L
	PE_H15A0_SHPL_PE1_BBD_CTRL_02_T       	pe1_bbd_ctrl_02;         //0xc800d688L
	PE_H15A0_SHPL_PE1_BBD_STAT_00_T       	pe1_bbd_stat_00;         //0xc800d68cL
	PE_H15A0_SHPL_PE1_BBD_STAT_01_T       	pe1_bbd_stat_01;         //0xc800d690L
	UINT32                             	    reserved15[3];           //0xc800d694L~0xc800d69cL
	PE_H15A0_SHPL_PE1_TGEN_CTRL_00_T      	pe1_tgen_ctrl_00;        //0xc800d6a0L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_01_T      	pe1_tgen_ctrl_01;        //0xc800d6a4L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_02_T      	pe1_tgen_ctrl_02;        //0xc800d6a8L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_03_T      	pe1_tgen_ctrl_03;        //0xc800d6acL
	PE_H15A0_SHPL_PE1_TGEN_CTRL_04_T      	pe1_tgen_ctrl_04;        //0xc800d6b0L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_05_T      	pe1_tgen_ctrl_05;        //0xc800d6b4L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_06_T      	pe1_tgen_ctrl_06;        //0xc800d6b8L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_07_T      	pe1_tgen_ctrl_07;        //0xc800d6bcL
	PE_H15A0_SHPL_PE1_TGEN_CTRL_08_T      	pe1_tgen_ctrl_08;        //0xc800d6c0L
	PE_H15A0_SHPL_PE1_TGEN_CTRL_09_T      	pe1_tgen_ctrl_09;        //0xc800d6c4L
}PE_SHPL_REG_H15A0_T;

typedef struct {
	PE_H15A0_SHPR_PE1_TOP_CTRL_00_T       	pe1_top_ctrl_00;         //0xc800da00L
	PE_H15A0_SHPR_PE1_TOP_CTRL_01_T       	pe1_top_ctrl_01;         //0xc800da04L
	PE_H15A0_SHPR_PE1_TOP_CTRL_02_T       	pe1_top_ctrl_02;         //0xc800da08L
	PE_H15A0_SHPR_PE1_TOP_CTRL_03_T       	pe1_top_ctrl_03;         //0xc800da0cL
	PE_H15A0_SHPR_PE1_TOP_CTRL_04_T       	pe1_top_ctrl_04;         //0xc800da10L
	UINT32                             	    reserved00[3];           //0xc800da14L~0xc800da1cL
	PE_H15A0_SHPR_PE1_WIN1_CTRL_00_T      	pe1_win1_ctrl_00;        //0xc800da20L
	PE_H15A0_SHPR_PE1_WIN1_CTRL_01_T      	pe1_win1_ctrl_01;        //0xc800da24L
	PE_H15A0_SHPR_PE1_WIN1_CTRL_02_T      	pe1_win1_ctrl_02;        //0xc800da28L
	PE_H15A0_SHPR_PE1_WIN1_CTRL_03_T      	pe1_win1_ctrl_03;        //0xc800da2cL
	PE_H15A0_SHPR_PE1_WIN1_CTRL_04_T      	pe1_win1_ctrl_04;        //0xc800da30L
	UINT32                             	    reserved01[3];           //0xc800da34L~0xc800da3cL
	PE_H15A0_SHPR_PE1_FSW_CTRL_00_T       	pe1_fsw_ctrl_00;         //0xc800da40L
	PE_H15A0_SHPR_PE1_FSW_CTRL_01_T       	pe1_fsw_ctrl_01;         //0xc800da44L
	PE_H15A0_SHPR_PE1_FSW_CTRL_02_T       	pe1_fsw_ctrl_02;         //0xc800da48L
	PE_H15A0_SHPR_PE1_FSW_CTRL_03_T       	pe1_fsw_ctrl_03;         //0xc800da4cL
	UINT32                             	    reserved02[16];          //0xc800da50L~0xc800da8cL
	PE_H15A0_SHPR_PE1_WIN3_CTRL_00_T      	pe1_win3_ctrl_00;        //0xc800da90L
	PE_H15A0_SHPR_PE1_WIN3_CTRL_01_T      	pe1_win3_ctrl_01;        //0xc800da94L
	PE_H15A0_SHPR_PE1_WIN3_CTRL_02_T      	pe1_win3_ctrl_02;        //0xc800da98L
	PE_H15A0_SHPR_PE1_WIN3_CTRL_03_T      	pe1_win3_ctrl_03;        //0xc800da9cL
	PE_H15A0_SHPR_PE1_WIN3_CTRL_04_T      	pe1_win3_ctrl_04;        //0xc800daa0L
	UINT32                             	    reserved03[3];           //0xc800daa4L~0xc800daacL
	PE_H15A0_SHPR_PE1_DERH_CTRL_00_T      	pe1_derh_ctrl_00;        //0xc800dab0L
	PE_H15A0_SHPR_PE1_DERH_CTRL_01_T      	pe1_derh_ctrl_01;        //0xc800dab4L
	PE_H15A0_SHPR_PE1_DERH_CTRL_02_T      	pe1_derh_ctrl_02;        //0xc800dab8L
	PE_H15A0_SHPR_PE1_DERH_CTRL_03_T      	pe1_derh_ctrl_03;        //0xc800dabcL
	PE_H15A0_SHPR_PE1_DERH_CTRL_04_T      	pe1_derh_ctrl_04;        //0xc800dac0L
	PE_H15A0_SHPR_PE1_DERH_CTRL_05_T      	pe1_derh_ctrl_05;        //0xc800dac4L
	PE_H15A0_SHPR_PE1_DERH_CTRL_06_T      	pe1_derh_ctrl_06;        //0xc800dac8L
	PE_H15A0_SHPR_PE1_DERH_CTRL_07_T      	pe1_derh_ctrl_07;        //0xc800daccL
	PE_H15A0_SHPR_PE1_DERH_CTRL_08_T      	pe1_derh_ctrl_08;        //0xc800dad0L
	PE_H15A0_SHPR_PE1_DERH_CTRL_09_T      	pe1_derh_ctrl_09;        //0xc800dad4L
	PE_H15A0_SHPR_PE1_DERH_CTRL_0A_T      	pe1_derh_ctrl_0a;        //0xc800dad8L
	PE_H15A0_SHPR_PE1_DERH_CTRL_0B_T      	pe1_derh_ctrl_0b;        //0xc800dadcL
	PE_H15A0_SHPR_PE1_DERH_CTRL_0C_T      	pe1_derh_ctrl_0c;        //0xc800dae0L
	PE_H15A0_SHPR_PE1_CORING_CTRL_10_T    	pe1_coring_ctrl_10;      //0xc800dae4L
	UINT32                             	    reserved04[2];           //0xc800dae8L~0xc800daecL
	PE_H15A0_SHPR_PE1_DERV_CTRL_0_T       	pe1_derv_ctrl_0;         //0xc800daf0L
	PE_H15A0_SHPR_PE1_DERV_CTRL_1_T       	pe1_derv_ctrl_1;         //0xc800daf4L
	PE_H15A0_SHPR_PE1_DERV_CTRL_2_T       	pe1_derv_ctrl_2;         //0xc800daf8L
	PE_H15A0_SHPR_PE1_DERV_CTRL_3_T       	pe1_derv_ctrl_3;         //0xc800dafcL
	UINT32                             	    reserved05[4];           //0xc800db00L~0xc800db0cL
	PE_H15A0_SHPR_PE1_DP_CTRL_00_T        	pe1_dp_ctrl_00;          //0xc800db10L
	PE_H15A0_SHPR_PE1_DP_CTRL_01_T        	pe1_dp_ctrl_01;          //0xc800db14L
	PE_H15A0_SHPR_PE1_DP_CTRL_02_T        	pe1_dp_ctrl_02;          //0xc800db18L
	PE_H15A0_SHPR_PE1_DP_CTRL_03_T        	pe1_dp_ctrl_03;          //0xc800db1cL
	UINT32                             	    reserved06[4];           //0xc800db20L~0xc800db2cL
	PE_H15A0_SHPR_PE1_SP_CTRL_00_T        	pe1_sp_ctrl_00;          //0xc800db30L
	PE_H15A0_SHPR_PE1_SP_CTRL_01_T        	pe1_sp_ctrl_01;          //0xc800db34L
	PE_H15A0_SHPR_PE1_SP_CTRL_02_T        	pe1_sp_ctrl_02;          //0xc800db38L
	PE_H15A0_SHPR_PE1_SP_CTRL_03_T        	pe1_sp_ctrl_03;          //0xc800db3cL
	PE_H15A0_SHPR_PE1_SP_CTRL_04_T        	pe1_sp_ctrl_04;          //0xc800db40L
	PE_H15A0_SHPR_PE1_SP_CTRL_05_T        	pe1_sp_ctrl_05;          //0xc800db44L
	PE_H15A0_SHPR_PE1_SP_CTRL_06_T        	pe1_sp_ctrl_06;          //0xc800db48L
	PE_H15A0_SHPR_PE1_SP_CTRL_07_T        	pe1_sp_ctrl_07;          //0xc800db4cL
	PE_H15A0_SHPR_PE1_SP_CTRL_08_T        	pe1_sp_ctrl_08;          //0xc800db50L
	PE_H15A0_SHPR_PE1_SP_CTRL_09_T        	pe1_sp_ctrl_09;          //0xc800db54L
	PE_H15A0_SHPR_PE1_SP_CTRL_0A_T        	pe1_sp_ctrl_0a;          //0xc800db58L
	PE_H15A0_SHPR_PE1_SP_CTRL_0B_T        	pe1_sp_ctrl_0b;          //0xc800db5cL
	PE_H15A0_SHPR_PE1_SP_CTRL_0C_T        	pe1_sp_ctrl_0c;          //0xc800db60L
	UINT32                             	    reserved07[3];           //0xc800db64L~0xc800db6cL
	PE_H15A0_SHPR_PE1_MP_CTRL_00_T        	pe1_mp_ctrl_00;          //0xc800db70L
	PE_H15A0_SHPR_PE1_MP_CTRL_01_T        	pe1_mp_ctrl_01;          //0xc800db74L
	PE_H15A0_SHPR_PE1_MP_CTRL_02_T        	pe1_mp_ctrl_02;          //0xc800db78L
	PE_H15A0_SHPR_PE1_MP_CTRL_03_T        	pe1_mp_ctrl_03;          //0xc800db7cL
	PE_H15A0_SHPR_PE1_MP_CTRL_04_T        	pe1_mp_ctrl_04;          //0xc800db80L
	PE_H15A0_SHPR_PE1_MP_CTRL_05_T        	pe1_mp_ctrl_05;          //0xc800db84L
	PE_H15A0_SHPR_PE1_MP_CTRL_06_T        	pe1_mp_ctrl_06;          //0xc800db88L
	PE_H15A0_SHPR_PE1_MP_CTRL_07_T        	pe1_mp_ctrl_07;          //0xc800db8cL
	PE_H15A0_SHPR_PE1_MP_CTRL_08_T        	pe1_mp_ctrl_08;          //0xc800db90L
	PE_H15A0_SHPR_PE1_MP_CTRL_09_T        	pe1_mp_ctrl_09;          //0xc800db94L
	PE_H15A0_SHPR_PE1_MP_CTRL_0A_T        	pe1_mp_ctrl_0a;          //0xc800db98L
	PE_H15A0_SHPR_PE1_MP_CTRL_0B_T        	pe1_mp_ctrl_0b;          //0xc800db9cL
	PE_H15A0_SHPR_PE1_MP_CTRL_0C_T        	pe1_mp_ctrl_0c;          //0xc800dba0L
	UINT32                             	    reserved08[3];           //0xc800dba4L~0xc800dbacL
	PE_H15A0_SHPR_PE1_CORING_CTRL_00_T    	pe1_coring_ctrl_00;      //0xc800dbb0L
	PE_H15A0_SHPR_PE1_CORING_CTRL_01_T    	pe1_coring_ctrl_01;      //0xc800dbb4L
	PE_H15A0_SHPR_PE1_CORING_CTRL_02_T    	pe1_coring_ctrl_02;      //0xc800dbb8L
	PE_H15A0_SHPR_PE1_CORING_CTRL_03_T    	pe1_coring_ctrl_03;      //0xc800dbbcL
	PE_H15A0_SHPR_PE1_CORING_CTRL_04_T    	pe1_coring_ctrl_04;      //0xc800dbc0L
	PE_H15A0_SHPR_PE1_CORING_CTRL_05_T    	pe1_coring_ctrl_05;      //0xc800dbc4L
	PE_H15A0_SHPR_PE1_CORING_CTRL_06_T    	pe1_coring_ctrl_06;      //0xc800dbc8L
	PE_H15A0_SHPR_PE1_CORING_CTRL_07_T    	pe1_coring_ctrl_07;      //0xc800dbccL
	PE_H15A0_SHPR_PE1_CORING_CTRL_08_T    	pe1_coring_ctrl_08;      //0xc800dbd0L
	PE_H15A0_SHPR_PE1_CORING_CTRL_09_T    	pe1_coring_ctrl_09;      //0xc800dbd4L
	UINT32                             	    reserved09[2];           //0xc800dbd8L~0xc800dbdcL
	PE_H15A0_SHPR_PE1_CORING_STAT_00_T    	pe1_coring_stat_00;      //0xc800dbe0L
	PE_H15A0_SHPR_PE1_CORING_STAT_01_T    	pe1_coring_stat_01;      //0xc800dbe4L
	PE_H15A0_SHPR_PE1_CORING_STAT_02_T    	pe1_coring_stat_02;      //0xc800dbe8L
	PE_H15A0_SHPR_PE1_CORING_STAT_03_T    	pe1_coring_stat_03;      //0xc800dbecL
	UINT32                             	    reserved10[4];           //0xc800dbf0L~0xc800dbfcL
	PE_H15A0_SHPR_PE1_PTI_CTRL_00_T       	pe1_pti_ctrl_00;         //0xc800dc00L
	PE_H15A0_SHPR_PE1_PTI_CTRL_01_T       	pe1_pti_ctrl_01;         //0xc800dc04L
	PE_H15A0_SHPR_PE1_STI_CTRL_00_T       	pe1_sti_ctrl_00;         //0xc800dc08L
	PE_H15A0_SHPR_PE1_STI_CTRL_01_T       	pe1_sti_ctrl_01;         //0xc800dc0cL
	UINT32                             	    reserved11[4];           //0xc800dc10L~0xc800dc1cL
	PE_H15A0_SHPR_PE1_DJ_CTRL_00_T        	pe1_dj_ctrl_00;          //0xc800dc20L
	PE_H15A0_SHPR_PE1_DJ_CTRL_01_T        	pe1_dj_ctrl_01;          //0xc800dc24L
	PE_H15A0_SHPR_PE1_DJ_CTRL_02_T        	pe1_dj_ctrl_02;          //0xc800dc28L
	PE_H15A0_SHPR_PE1_DJ_CTRL_03_T        	pe1_dj_ctrl_03;          //0xc800dc2cL
	PE_H15A0_SHPR_PE1_CTI_CTRL_00_T       	pe1_cti_ctrl_00;         //0xc800dc30L
	PE_H15A0_SHPR_PE1_CTI_CTRL_01_T       	pe1_cti_ctrl_01;         //0xc800dc34L
	PE_H15A0_SHPR_PE1_CTI_CTRL_02_T       	pe1_cti_ctrl_02;         //0xc800dc38L
	UINT32                             	    reserved12[5];           //0xc800dc3cL~0xc800dc4cL
	PE_H15A0_SHPR_PE1_DEBUG_CTRL_00_T     	pe1_debug_ctrl_00;       //0xc800dc50L
	PE_H15A0_SHPR_PE1_DEBUG_CTRL_01_T     	pe1_debug_ctrl_01;       //0xc800dc54L
	PE_H15A0_SHPR_PE1_DEBUG_CTRL_02_T     	pe1_debug_ctrl_02;       //0xc800dc58L
	UINT32                             	    reserved13;              //0xc800dc5cL
	PE_H15A0_SHPR_PE1_SQM_CTRL_00_T       	pe1_sqm_ctrl_00;         //0xc800dc60L
	PE_H15A0_SHPR_PE1_SQM_CTRL_01_T       	pe1_sqm_ctrl_01;         //0xc800dc64L
	PE_H15A0_SHPR_PE1_SQM_CTRL_02_T       	pe1_sqm_ctrl_02;         //0xc800dc68L
	PE_H15A0_SHPR_PE1_SQM_CTRL_03_T       	pe1_sqm_ctrl_03;         //0xc800dc6cL
	PE_H15A0_SHPR_PE1_SQM_CTRL_04_T       	pe1_sqm_ctrl_04;         //0xc800dc70L
	PE_H15A0_SHPR_PE1_SQM_CTRL_05_T       	pe1_sqm_ctrl_05;         //0xc800dc74L
	PE_H15A0_SHPR_PE1_SQM_STAT_00_T       	pe1_sqm_stat_00;         //0xc800dc78L
	UINT32                             	    reserved14;              //0xc800dc7cL
	PE_H15A0_SHPR_PE1_BBD_CTRL_00_T       	pe1_bbd_ctrl_00;         //0xc800dc80L
	PE_H15A0_SHPR_PE1_BBD_CTRL_01_T       	pe1_bbd_ctrl_01;         //0xc800dc84L
	PE_H15A0_SHPR_PE1_BBD_CTRL_02_T       	pe1_bbd_ctrl_02;         //0xc800dc88L
	PE_H15A0_SHPR_PE1_BBD_STAT_00_T       	pe1_bbd_stat_00;         //0xc800dc8cL
	PE_H15A0_SHPR_PE1_BBD_STAT_01_T       	pe1_bbd_stat_01;         //0xc800dc90L
	UINT32                             	    reserved15[3];           //0xc800dc94L~0xc800dc9cL
	PE_H15A0_SHPR_PE1_TGEN_CTRL_00_T      	pe1_tgen_ctrl_00;        //0xc800dca0L
	PE_H15A0_SHPR_PE1_TGEN_CTRL_01_T      	pe1_tgen_ctrl_01;        //0xc800dca4L
	PE_H15A0_SHPR_PE1_TGEN_CTRL_02_T      	pe1_tgen_ctrl_02;        //0xc800dca8L
	PE_H15A0_SHPR_PE1_TGEN_CTRL_03_T      	pe1_tgen_ctrl_03;        //0xc800dcacL
	PE_H15A0_SHPR_PE1_TGEN_CTRL_04_T      	pe1_tgen_ctrl_04;        //0xc800dcb0L
	PE_H15A0_SHPR_PE1_TGEN_CTRL_05_T      	pe1_tgen_ctrl_05;        //0xc800dcb4L
	PE_H15A0_SHPR_PE1_TGEN_CTRL_06_T      	pe1_tgen_ctrl_06;        //0xc800dcb8L
	PE_H15A0_SHPR_PE1_TGEN_CTRL_07_T      	pe1_tgen_ctrl_07;        //0xc800dcbcL
	PE_H15A0_SHPR_PE1_TGEN_CTRL_08_T      	pe1_tgen_ctrl_08;        //0xc800dcc0L
	PE_H15A0_SHPR_PE1_TGEN_CTRL_09_T      	pe1_tgen_ctrl_09;        //0xc800dcc4L
}PE_SHPR_REG_H15A0_T;
#endif

