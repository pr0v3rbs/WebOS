#ifndef _PE_ND_REG_H15A0_H_
#define _PE_ND_REG_H15A0_H_
/*-----------------------------------------------------------------------------
		0xc8004100L pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0
	UINT32 load_enable                        : 1 ;  //     1
	UINT32 load_type                          : 1 ;  //     2
} PE_H15A0_ND1_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc8004104L pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 timer_rst_sel                      : 1 ;  //     8
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16
} PE_H15A0_ND1_PE0_INTR_T;

/*-----------------------------------------------------------------------------
		0xc8004108L pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                       : 4 ;  //  3: 0
	UINT32 ipc_frame_id                       : 4 ;  //  7: 4
	UINT32 tnrw_y_frame_id                    : 4 ;  // 11: 8
	UINT32 tnrw_c_frame_id                    : 4 ;  // 15:12
	UINT32 dnr_frame_id                       : 8 ;  // 23:16
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24
} PE_H15A0_ND1_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc800410cL pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 timer_cnt                          : 16;  // 31:16
} PE_H15A0_ND1_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc8004110L pe0_opmode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_c_en                           : 1 ;  //     0
	UINT32 tnr_y_en                           : 1 ;  //     1
	UINT32 ipc_c_en                           : 1 ;  //     2
	UINT32 ipc_y_en                           : 1 ;  //     3
	UINT32 tnr_c_444_as_420                   : 1 ;  //     4
	UINT32 tnr_y2_en                          : 1 ;  //     5
	UINT32 force_422_to_420                   : 1 ;  //     6
	UINT32 ipc0_y_en_prog                     : 1 ;  //     7
	UINT32 mif_last_mode                      : 3 ;  // 10: 8
	UINT32 tnrw_off                           : 1 ;  //    11
	UINT32                                    : 1 ;  //    12     reserved
	UINT32 tnr_m_en                           : 1 ;  //    13
	UINT32 force_444_to_422                   : 1 ;  //    14
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 detour_enable                      : 1 ;  //    16
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 ibuf_en                            : 2 ;  // 21:20
	UINT32 minfo_vhalf_mode                   : 2 ;  // 23:22
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 tnr_in_lsb_mode                    : 2 ;  // 29:28
	UINT32 m_run_id                           : 2 ;  // 31:30
} PE_H15A0_ND1_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc8004114L pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0
	UINT32                                    : 1 ;  //    12     reserved
	UINT32 cs_type                            : 3 ;  // 15:13
	UINT32 vsize                              : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31
} PE_H15A0_ND1_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8004118L pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4
	UINT32 field_inv                          : 1 ;  //     6
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 secam_line                         : 1 ;  //     8
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 secam_line_mode                    : 2 ;  // 13:12
} PE_H15A0_ND1_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
		0xc8004120L pe0_in_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset                            : 12;  // 11: 0
	UINT32                                    : 18;  // 29:12     reserved
	UINT32 hsize_sel                          : 1 ;  //    30
	UINT32 crop_en                            : 1 ;  //    31
} PE_H15A0_ND1_PE0_IN_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc8004124L pe0_in_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vsize                              : 12;  // 27:16
} PE_H15A0_ND1_PE0_IN_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8004128L fbc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fbc_core_en_y                      : 1 ;  //     0
	UINT32 fbc_core_20p_y                     : 1 ;  //     1
	UINT32                                    : 13;  // 14: 2     reserved
	UINT32 fbc_en_y                           : 1 ;  //    15
	UINT32 fbc_core_en_c                      : 1 ;  //    16
	UINT32 fbc_core_20p_c                     : 1 ;  //    17
	UINT32                                    : 13;  // 30:18     reserved
	UINT32 fbc_en_y_c                         : 1 ;  //    31
} PE_H15A0_ND1_FBC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8004130L pe0_debug_mem_req ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_rd_t1_y                         : 1 ;  //     0
	UINT32 en_rd_t1_c                         : 1 ;  //     1
	UINT32 en_rd_t2_y                         : 1 ;  //     2
	UINT32 en_rd_t2_c                         : 1 ;  //     3
	UINT32 en_rd_t3_y                         : 1 ;  //     4
	UINT32 en_rd_t3_c                         : 1 ;  //     5
	UINT32 en_rd_t4_y                         : 1 ;  //     6
	UINT32 en_rd_t1_m                         : 1 ;  //     7
	UINT32 en_wr_tnrw_y                       : 1 ;  //     8
	UINT32 en_wr_tnrw_c                       : 1 ;  //     9
	UINT32 en_wr_tnrw_m                       : 1 ;  //    10
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 manual_mem_en                      : 1 ;  //    15
} PE_H15A0_ND1_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
		0xc8004134L pe0_debug_lreq_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                         : 11;  // 10: 0
	UINT32 lreq_dly_c_en                      : 1 ;  //    11
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 lreq_dly_y                         : 11;  // 26:16
	UINT32 lreq_dly_y_en                      : 1 ;  //    27
} PE_H15A0_ND1_PE0_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
		0xc8004140L tnr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tnr_en                         : 1 ;  //     0
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 reg_sc_detect_hres                 : 1 ;  //     2
	UINT32 reg_clc_nt_pal                     : 1 ;  //     3
	UINT32 reg_clc_mem_wr_mode                : 2 ;  //  5: 4
	UINT32 reg_sad_8x3_res                    : 2 ;  //  7: 6
	UINT32 reg_sad_8x3_iir_en                 : 1 ;  //     8
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 reg_minfo_sw                       : 2 ;  // 11:10
	UINT32 reg_minfo_mode10                   : 2 ;  // 13:12
	UINT32 reg_minfo_flt                      : 2 ;  // 15:14
	UINT32 reg_dnr_motion_scale               : 2 ;  // 17:16
	UINT32 reg_minfo_mode32                   : 2 ;  // 19:18
	UINT32 reg_minfo_mode74                   : 4 ;  // 23:20
	UINT32 reg_sad_8x3_iir_alpha              : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8004144L tnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_show                       : 4 ;  //  3: 0
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 reg_dbg_tnrw                       : 2 ;  // 17:16
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_tnri_debug_mode                : 2 ;  // 21:20
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_bsum_scale                     : 2 ;  // 25:24
} PE_H15A0_ND1_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8004148L tnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sc_en                              : 1 ;  //     0
	UINT32 sc_max_flt_en                      : 1 ;  //     1
	UINT32 sc_calc_prev_hist                  : 1 ;  //     2
	UINT32 sc_chroma_en                       : 1 ;  //     3
	UINT32 sc_scale                           : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sc_max_flt_tap                     : 3 ;  // 10: 8
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 sc_chroma_gain                     : 3 ;  // 14:12
	UINT32 sc_out_mode                        : 1 ;  //    15
	UINT32 sc_th_ma                           : 8 ;  // 23:16
	UINT32 sc_th_memc                         : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800414cL tnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_scale                 : 3 ;  //  2: 0
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 show_dbg_bar_en                    : 4 ;  //  7: 4
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 show_dbg_bar0_mux                  : 4 ;  // 19:16
	UINT32 show_dbg_bar1_mux                  : 4 ;  // 23:20
	UINT32 show_dbg_bar2_mux                  : 4 ;  // 27:24
	UINT32 show_dbg_bar3_mux                  : 4 ;  // 31:28
} PE_H15A0_ND1_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8004150L tnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_debug_bar_09                : 8 ;  //  7: 0
	UINT32 manual_debug_bar_10                : 8 ;  // 15: 8
	UINT32 manual_debug_bar_11                : 8 ;  // 23:16
	UINT32 manual_debug_bar_12                : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8004154L tnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mamc_blend_mode                : 2 ;  //  1: 0
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_mamc_alpha                     : 8 ;  // 15: 8
	UINT32 reg_mv_pv_fix_v                    : 4 ;  // 19:16
	UINT32 reg_mv_pv_fix_h                    : 5 ;  // 24:20
	UINT32                                    : 6 ;  // 30:25     reserved
	UINT32 reg_mv_pv_fix_en                   : 1 ;  //    31
} PE_H15A0_ND1_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8004158L tnr_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 me_lb_sad_base                     : 8 ;  //  7: 0
	UINT32 pv_smooth_sad_base                 : 8 ;  // 15: 8
	UINT32 reg_me_lb_polarity                 : 1 ;  //    16
} PE_H15A0_ND1_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800415cL tnr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 reg_sc_en                          : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_5x5_sad_cut                    : 1 ;  //     8
} PE_H15A0_ND1_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8004160L tnr_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_h_fix_val                   : 5 ;  //  4: 0
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_mv_v_fix_val                   : 4 ;  // 11: 8
	UINT32 reg_mv_h_fix_fb_val                : 5 ;  // 16:12
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_mv_v_fix_fb_val                : 4 ;  // 23:20
	UINT32 reg_debug_sel                      : 4 ;  // 27:24
	UINT32 reg_lb_monitor_sel                 : 2 ;  // 29:28
	UINT32 reg_mv_fb_fix                      : 1 ;  //    30
	UINT32 reg_mv_fix                         : 1 ;  //    31
} PE_H15A0_ND1_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8004164L tnr_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_base                           : 7 ;  //  6: 0
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 lb_smooth_clip                     : 6 ;  // 13: 8
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pv_smooth_clip                     : 6 ;  // 21:16
} PE_H15A0_ND1_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8004168L tnr_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_bm_off                       : 2 ;  //  1: 0
	UINT32 phase_sm_off                       : 2 ;  //  3: 2
	UINT32 phase_lb_off                       : 2 ;  //  5: 4
	UINT32 phase_pv_off                       : 2 ;  //  7: 6
	UINT32 phase_mc_off                       : 2 ;  //  9: 8
} PE_H15A0_ND1_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800416cL tnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lb_smooth_en                       : 1 ;  //     0
	UINT32 pv_smooth_en                       : 1 ;  //     1
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 lb_mv_h_off                        : 2 ;  //  5: 4
	UINT32 lb_mv_v_off                        : 2 ;  //  7: 6
	UINT32 pv_mv_h_off                        : 2 ;  //  9: 8
	UINT32 pv_mv_v_off                        : 2 ;  // 11:10
	UINT32 reg_mv_fb_reverse                  : 1 ;  //    12
} PE_H15A0_ND1_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8004170L tnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8004174L tnr_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc8004178L tnr_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc800417cL tnr_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc8004180L tnr_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sad_acc_mode                   : 2 ;  //  1: 0
	UINT32 reg_bmv_fix_en                     : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_bmv_fix_v                      : 4 ;  //  7: 4
	UINT32 reg_bmv_fix_h                      : 5 ;  // 12: 8
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_n_hblk                         : 4 ;  // 19:16
	UINT32 reg_n_vblk                         : 5 ;  // 24:20
} PE_H15A0_ND1_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc8004184L tnr_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_blk_vsize                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_blk_hsize                      : 12;  // 27:16
} PE_H15A0_ND1_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc8004188L tnr_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bsad_scale                         : 3 ;  //  2: 0
	UINT32 bmv_rst_mode                       : 1 ;  //     3
	UINT32 gsad_scale                         : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 gmv_gmv_tl                         : 7 ;  // 14: 8
	UINT32 gmv_gmv_tl_en                      : 1 ;  //    15
	UINT32 bmv_gmv_tl                         : 7 ;  // 22:16
	UINT32 bmv_gmv_tl_en                      : 1 ;  //    23
	UINT32 bmv_pmv_tl                         : 7 ;  // 30:24
	UINT32 bmv_pmv_tl_en                      : 1 ;  //    31
} PE_H15A0_ND1_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc800418cL tnr_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 gmv_0mv_tl                         : 7 ;  // 22:16
	UINT32 gmv_0mv_tl_en                      : 1 ;  //    23
	UINT32 bmv_0mv_tl                         : 7 ;  // 30:24
	UINT32 bmv_0mv_tl_en                      : 1 ;  //    31
} PE_H15A0_ND1_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc8004190L tnr_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_detail_beta_mul                : 8 ;  //  7: 0
	UINT32 reg_detail_alpha_mul               : 8 ;  // 15: 8
	UINT32 reg_edge_beta_mul                  : 8 ;  // 23:16
	UINT32 reg_edge_alpha_mul                 : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc8004194L tnr_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_svar_scale                     : 3 ;  //  2: 0
} PE_H15A0_ND1_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc8004198L tnr_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc800419cL tnr_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc80041a0L tnr_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 reg_luma_gain_en                   : 1 ;  //     4
	UINT32 reg_skin_gain_en                   : 1 ;  //     5
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_sad_sum_cut                    : 2 ;  //  9: 8
	UINT32 reg_sad_cut                        : 2 ;  // 11:10
	UINT32 reg_5x5_sad_cut                    : 2 ;  // 13:12
	UINT32                                    : 6 ;  // 19:14     reserved
	UINT32 reg_debug_sel                      : 4 ;  // 23:20
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 chroma_sad_en                      : 1 ;  //    31
} PE_H15A0_ND1_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc80041a4L tnr_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_flat_y_blur_en                 : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_flat_y_cut_resolution          : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_flat_y_G0                      : 6 ;  // 13: 8
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_flat_y_G4                      : 4 ;  // 19:16
	UINT32 reg_flat_y_G3                      : 4 ;  // 23:20
	UINT32 reg_flat_y_G2                      : 4 ;  // 27:24
	UINT32 reg_flat_y_G1                      : 4 ;  // 31:28
} PE_H15A0_ND1_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc80041a8L tnr_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_flat_c_blur_en                 : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_flat_c_cut_resolution          : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_flat_c_G0                      : 6 ;  // 13: 8
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_flat_c_G4                      : 4 ;  // 19:16
	UINT32 reg_flat_c_G3                      : 4 ;  // 23:20
	UINT32 reg_flat_c_G2                      : 4 ;  // 27:24
	UINT32 reg_flat_c_G1                      : 4 ;  // 31:28
} PE_H15A0_ND1_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc80041acL tnr_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc80041b0L tnr_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre_filtered_en                    : 1 ;  //     0
	UINT32 luma_gain_en                       : 1 ;  //     1
	UINT32 cut_resolution                     : 2 ;  //  3: 2
	UINT32                                    : 1 ;  //     4     reserved
	UINT32 reg_pre_detail_cttl_g0             : 7 ;  // 11: 5
	UINT32 reg_pre_detail_cttl_g4             : 5 ;  // 16:12
	UINT32 reg_pre_detail_cttl_g3             : 5 ;  // 21:17
	UINT32 reg_pre_detail_cttl_g2             : 5 ;  // 26:22
	UINT32 reg_pre_detail_cttl_g1             : 5 ;  // 31:27
} PE_H15A0_ND1_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc80041b4L tnr_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dir_blur_en                    : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_dir_blur_gain                  : 8 ;  // 15: 8
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 reg_diff_limit                     : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc80041b8L tnr_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denoise_filter_en              : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_denoise_cut_resolution         : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_denoise_gain                   : 8 ;  // 15: 8
	UINT32 reg_denoise_G0                     : 7 ;  // 22:16
} PE_H15A0_ND1_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc80041bcL tnr_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denoise_G4                     : 5 ;  //  4: 0
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_denoise_G3                     : 5 ;  // 12: 8
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_denoise_G2                     : 5 ;  // 20:16
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_denoise_G1                     : 5 ;  // 28:24
} PE_H15A0_ND1_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc80041c0L tnr_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_gsad                             : 8 ;  //  7: 0
	UINT32 w_gmv                              : 9 ;  // 16: 8
} PE_H15A0_ND1_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc80041c4L tnr_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_apl_motion                       : 8 ;  //  7: 0
	UINT32 w_apl_mc_sad                       : 8 ;  // 15: 8
	UINT32 w_apl_var                          : 8 ;  // 23:16
} PE_H15A0_ND1_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc80041c8L tnr_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_ND1_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc80041ccL tnr_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_ND1_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc80041d0L tnr_main_lut_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr                : 6 ;  //  5: 0
	UINT32                                    : 6 ;  // 11: 6     reserved
	UINT32 main_lut_ai_enable                 : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 main_lut_load                      : 1 ;  //    15
} PE_H15A0_ND1_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
		0xc80041d4L tnr_main_lut_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0               : 32;  // 31: 0
} PE_H15A0_ND1_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
		0xc80041dcL tnr_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sc_measure                         : 8 ;  //  7: 0
	UINT32 apl_y                              : 8 ;  // 15: 8
	UINT32 nlvl                               : 6 ;  // 21:16
} PE_H15A0_ND1_TNR_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc80041e0L tnr_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 reg_sad_sum_cut                    : 2 ;  //  9: 8
	UINT32 reg_sad_cut                        : 2 ;  // 11:10
} PE_H15A0_ND1_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc80041e4L tnr_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc80041e8L tnr_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc80041ecL tnr_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc80041f0L tnr_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc80041f4L tnr_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc80041f8L tnr_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc80041fcL tnr_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND1_TNR_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc8004200L ipc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_luma_mode                       : 2 ;  //  1: 0
	UINT32                                    : 1 ;  //     2     reserved
	UINT32 cr_n1_cc_blend_en                  : 1 ;  //     3
	UINT32 cr_chroma_mode                     : 2 ;  //  5: 4
	UINT32 cr_integer_edi                     : 1 ;  //     6
	UINT32 cr_chroma_edi                      : 1 ;  //     7
	UINT32 cr_st_2d_v_flt_range2              : 4 ;  // 11: 8
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 15:12
	UINT32 cr_ori_3d_blur_y                   : 2 ;  // 17:16
	UINT32 cr_ori_3d_blur_c                   : 2 ;  // 19:18
	UINT32 cr_tava_v_edge_adap                : 1 ;  //    20
	UINT32 cr_tava_debug_mode                 : 1 ;  //    21
	UINT32 cr_tava_st_adap                    : 1 ;  //    22
	UINT32 cr_tava_en                         : 1 ;  //    23
	UINT32 cr_hmc_force_fmd                   : 1 ;  //    24
	UINT32 cr_st_2d_v_flt_en                  : 1 ;  //    25
	UINT32 cr_clc_fmd_off                     : 1 ;  //    26
	UINT32 cr_chroma_tava_only                : 1 ;  //    27
	UINT32 cr_film_auto_gain                  : 1 ;  //    28
	UINT32 cr_film_apply_c                    : 1 ;  //    29
	UINT32 cr_film_apply_y                    : 1 ;  //    30
	UINT32 cr_film_mode_enable                : 1 ;  //    31
} PE_H15A0_ND1_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8004204L ipc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_en_sel1                     : 2 ;  //  1: 0
	UINT32 cr_hmc_en_sel2                     : 2 ;  //  3: 2
	UINT32                                    : 2 ;  //  5: 4     reserved
	UINT32 cr_fmd_vfilter_en                  : 1 ;  //     6
	UINT32 cr_fend_auto                       : 1 ;  //     7
	UINT32 cr_st_iir_dec_gain                 : 4 ;  // 11: 8
	UINT32 cr_st_iir_inc_gain                 : 4 ;  // 15:12
	UINT32 cr_st_flt_enable                   : 1 ;  //    16
	UINT32 cr_st_iir_en                       : 1 ;  //    17
	UINT32 cr_hmc_flt_enable                  : 1 ;  //    18
	UINT32 cr_game_mode                       : 1 ;  //    19
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 23:20
	UINT32                                    : 1 ;  //    24     reserved
	UINT32 cr_black_mask                      : 1 ;  //    25
	UINT32 cr_ipc_debug_show                  : 5 ;  // 30:26
	UINT32 cr_5f_mode                         : 1 ;  //    31
} PE_H15A0_ND1_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8004208L ipc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                : 8 ;  //  7: 0
	UINT32 fuzzy_ctrl_point_x4                : 8 ;  // 15: 8
	UINT32 fuzzy_ctrl_point_x1                : 8 ;  // 23:16
	UINT32 fuzzy_ctrl_point_y1                : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800420cL ipc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                : 8 ;  //  7: 0
	UINT32 fuzzy_ctrl_point_y2                : 8 ;  // 15: 8
	UINT32 fuzzy_ctrl_point_x3                : 8 ;  // 23:16
	UINT32 fuzzy_ctrl_point_y3                : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8004210L ipc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset__s7                      : 8 ;  //  7: 0
	UINT32 motion_x_tearing_gain              : 8 ;  // 15: 8
	UINT32 tearing_gain                       : 8 ;  // 23:16
	UINT32 motion_gain                        : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8004214L ipc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 12;  // 11: 0     reserved
	UINT32 cr_t_grad_weight                   : 4 ;  // 15:12
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 cr_v_grad_weight                   : 4 ;  // 23:20
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 cr_h_grad_weight                   : 4 ;  // 31:28
} PE_H15A0_ND1_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8004218L ipc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_normalize_en                    : 1 ;  //     0
	UINT32 cr_flat_only                       : 1 ;  //     1
	UINT32 cr_motion_iir_en                   : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 cr_tearing_normalize_en            : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cr_normalize_base                  : 8 ;  // 15: 8
	UINT32 cr_normalize_center                : 8 ;  // 23:16
	UINT32 cr_motion_coring                   : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800421cL ipc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cr_v_en                            : 1 ;  //     1
	UINT32 cr_45_en                           : 1 ;  //     2
	UINT32 cr_15_en                           : 1 ;  //     3
	UINT32                                    : 20;  // 23: 4     reserved
	UINT32 cr_iir_motion_gain                 : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8004220L ipc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 fmd_caption_range_bot              : 12;  // 27:16
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 caption_flow_detected              : 1 ;  //    30
	UINT32 hmc_when_cap_not_detected          : 1 ;  //    31
} PE_H15A0_ND1_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8004224L ipc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                        : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pn_variance_th                     : 8 ;  // 23:16
	UINT32 pn_fmd_motion_th                   : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8004228L ipc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                      : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pcnc_variance_th                   : 8 ;  // 23:16
	UINT32 pcnc_fmd_motion_th                 : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800422cL ipc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                       : 8 ;  //  7: 0
	UINT32 cr_center_bound_ctrl               : 8 ;  // 15: 8
	UINT32 cr_dn_bound_ctrl                   : 8 ;  // 23:16
	UINT32 cr_up_bound_ctrl                   : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8004230L ipc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8004234L ipc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_tava_chroma_mul                 : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 cr_hmc_alpha_th                    : 8 ;  // 19:12
	UINT32 cr_hmc_debug_mode                  : 1 ;  //    20
	UINT32 cr_hmc_debug_gain                  : 6 ;  // 26:21
	UINT32 cr_hmc_debug_dir                   : 5 ;  // 31:27
} PE_H15A0_ND1_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc8004238L ipc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0
	UINT32 reserved                           : 1 ;  //     1
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8
	UINT32 reg_same_th                        : 8 ;  // 23:16
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc800423cL ipc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0
	UINT32 reg_3_2_detect_only                : 1 ;  //     1
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3
	UINT32 reg_hd_mode                        : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc8004240L ipc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31
} PE_H15A0_ND1_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc8004244L ipc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution                  : 2 ;  //  1: 0
	UINT32 cr_field_manual_mode               : 2 ;  //  3: 2
	UINT32 cr_field_inverse                   : 1 ;  //     4
	UINT32                                    : 19;  // 23: 5     reserved
	UINT32 cr_tearing_a_th                    : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc8004248L ipc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                         : 8 ;  //  7: 0
	UINT32 cr_lr_gain                         : 8 ;  // 15: 8
	UINT32 cr_h_grad_gain                     : 8 ;  // 23:16
	UINT32 cr_v_grad_gain                     : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc800424cL ipc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_errs_gain                      : 8 ;  //  7: 0
	UINT32 reg_confidance_gain                : 8 ;  // 15: 8
	UINT32 reg_2d_offset                      : 8 ;  // 23:16
	UINT32 reg_flicker_gain                   : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc8004250L ipc_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count                       : 16;  // 15: 0
	UINT32 r_nc_v_count                       : 16;  // 31:16
} PE_H15A0_ND1_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc8004254L ipc_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same                       : 16;  // 15: 0
	UINT32 r_pn_v_count                       : 16;  // 31:16
} PE_H15A0_ND1_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc8004258L ipc_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0
	UINT32 r_fmd_32_min_th_flag_u             : 1 ;  //     6
	UINT32 r_fmd_32_max_th_flag_u             : 1 ;  //     7
	UINT32 r_fmd_still_limit_flag_u           : 1 ;  //     8
	UINT32 r_fmd_still_pn_th2_flag_u          : 1 ;  //     9
	UINT32 r_fmd_still_pn_th_flag_u           : 1 ;  //    10
	UINT32 r_fmd_still_prev_ratio_flag_u      : 1 ;  //    11
	UINT32 r_fmd_bigger_flag_u                : 1 ;  //    12
	UINT32 r_fmd_smaller_flag_u               : 1 ;  //    13
	UINT32 r_fmd_3_2_fmd_comb_mode_u          : 2 ;  // 15:14
	UINT32 r_pn_diff_count                    : 16;  // 31:16
} PE_H15A0_ND1_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc800425cL ipc_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 same_ng_flag_d                     : 1 ;  //     0
	UINT32 too_small_flag_d                   : 1 ;  //     1
	UINT32 r_fmd_still_flag_d                 : 1 ;  //     2
	UINT32 r_fmd_possibility_flag_d           : 1 ;  //     3
	UINT32 base_pn_ratio_flag_d               : 1 ;  //     4
	UINT32 r_32_detected_flag_d               : 1 ;  //     5
	UINT32 r_fmd_comb_mode_d                  : 2 ;  //  7: 6
	UINT32 r_fmd_lock_count_d                 : 8 ;  // 15: 8
	UINT32 same_ng_flag_u                     : 1 ;  //    16
	UINT32 too_small_flag_u                   : 1 ;  //    17
	UINT32 r_fmd_still_flag_u                 : 1 ;  //    18
	UINT32 r_fmd_possibility_flag_u           : 1 ;  //    19
	UINT32 base_pn_ratio_flag_u               : 1 ;  //    20
	UINT32 r_32_uetected_flag_u               : 1 ;  //    21
	UINT32 r_fmd_comb_mode_u                  : 2 ;  // 23:22
	UINT32 r_fmd_lock_count_u                 : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc8004260L clc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cl_filter_enable                   : 1 ;  //     1
	UINT32 clc_detection_enable               : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 motion_window                      : 2 ;  //  5: 4
	UINT32 cl_pattern_dist                    : 2 ;  //  7: 6
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 cl_ab_ratio                        : 4 ;  // 15:12
	UINT32 cl_y_hdistance                     : 2 ;  // 17:16
	UINT32 cl_c_hdistance                     : 2 ;  // 19:18
	UINT32 cl_y_protect_en                    : 1 ;  //    20
	UINT32                                    : 7 ;  // 27:21     reserved
	UINT32 cl_motion_a_filter                 : 1 ;  //    28
	UINT32 cl_motion_b_filter                 : 1 ;  //    29
	UINT32 cl_motion_a_mode                   : 1 ;  //    30
} PE_H15A0_ND1_CLC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8004264L clc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_motion_a_th                     : 8 ;  //  7: 0
	UINT32 cl_motion_b_th                     : 8 ;  // 15: 8
	UINT32 cl_y_protect_th0                   : 8 ;  // 23:16
	UINT32 cl_y_protect_th1                   : 8 ;  // 31:24
} PE_H15A0_ND1_CLC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8004268L clc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_y_bound_th                      : 7 ;  //  6: 0
	UINT32 cl_c_bound_use_4line               : 1 ;  //     7
	UINT32 cl_c_bound_th                      : 7 ;  // 14: 8
	UINT32 cl_c_bound_type                    : 1 ;  //    15
	UINT32 cl_y_bound_width_chroma            : 2 ;  // 17:16
	UINT32 cl_y_bound_ignore_cc               : 1 ;  //    18
	UINT32 cl_y_bound_ignore_p2               : 1 ;  //    19
	UINT32 cl_y_bound_ignore_p4               : 1 ;  //    20
	UINT32 reg_bin3x1_flt_en                  : 1 ;  //    21
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_bin5x1_flt_en                  : 1 ;  //    24
	UINT32 reg_bin5x1_flt_th                  : 3 ;  // 27:25
	UINT32 reg_vm_original_mode               : 1 ;  //    28
	UINT32 reg_vm_other_mode                  : 1 ;  //    29
	UINT32 cl_motionp2n2_th_pal_high          : 2 ;  // 31:30
} PE_H15A0_ND1_CLC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800426cL clc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_sat_min                         : 8 ;  //  7: 0
	UINT32                                    : 2 ;  //  9: 8     reserved
	UINT32 cl_var_measure_th1                 : 10;  // 19:10
	UINT32 cl_var_measure_th2                 : 10;  // 29:20
	UINT32 cl_pt_enable                       : 2 ;  // 31:30
} PE_H15A0_ND1_CLC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8004270L clc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_tearing_pt_th1                  : 12;  // 11: 0
	UINT32 cl_motionp2n2_th_pal_low           : 8 ;  // 19:12
	UINT32 cl_frame_min                       : 12;  // 31:20
} PE_H15A0_ND1_CLC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8004274L clc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_data_line_switching_sel         : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 clc_bin_flt_th                     : 3 ;  // 10: 8
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 clc_bin_flt_en                     : 1 ;  //    15
	UINT32 clc_dbg_show_mask                  : 13;  // 28:16
} PE_H15A0_ND1_CLC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8004278L clc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_c_filter_gain                   : 8 ;  //  7: 0
	UINT32 cc_y_filter_when_cc_detected       : 1 ;  //     8
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 cl_cc_original_en                  : 1 ;  //    10
	UINT32 clc_force_non_fmd                  : 1 ;  //    11
	UINT32 cl_c_filter_when_cl_detected       : 1 ;  //    12
	UINT32 cl_y_filter_when_cl_detected       : 1 ;  //    13
	UINT32 clc_filter_mode                    : 2 ;  // 15:14
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cl_cc_protection_th                : 8 ;  // 31:24
} PE_H15A0_ND1_CLC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800427cL clc_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 cl_detection_count                 : 16;  // 31:16
} PE_H15A0_ND1_CLC_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8004280L ipc_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tava_v_diff_y0                     : 8 ;  //  7: 0
	UINT32 tava_v_diff_x0                     : 8 ;  // 15: 8
	UINT32 tava_v_diff_y1                     : 8 ;  // 23:16
	UINT32 tava_v_diff_x1                     : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc8004284L ipc_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_div_en                         : 1 ;  //     0
	UINT32 cr_bound_exp_en                    : 1 ;  //     1
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cr_sub_main_sel                    : 1 ;  //     4
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 reg_manual_en                      : 1 ;  //    16
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_manual_mode2                   : 2 ;  // 21:20
	UINT32 reg_manual_mode1                   : 2 ;  // 23:22
	UINT32 reg_div_position                   : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc8004288L ipc_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc800428cL ipc_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0
	UINT32 reserved                           : 1 ;  //     1
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8
	UINT32 reg_same_th                        : 8 ;  // 23:16
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc8004290L ipc_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0
	UINT32 reg_3_2_detect_only                : 1 ;  //     1
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3
	UINT32 reg_hd_mode                        : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc8004294L ipc_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31
} PE_H15A0_ND1_IPC_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc8004298L ipc_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8
	UINT32 reg_pndiff_disable                 : 1 ;  //    16
} PE_H15A0_ND1_IPC_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc800429cL ipc_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fend_position                  : 11;  // 10: 0
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 cr_sc_length                       : 3 ;  // 14:12
	UINT32                                    : 11;  // 25:15     reserved
	UINT32 cr_vmc_dir                         : 2 ;  // 27:26
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 cr_vmc_en                          : 1 ;  //    31
} PE_H15A0_ND1_IPC_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc80042a0L ipc_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_yy_diff_gain                   : 4 ;  //  3: 0
	UINT32 reg_hv_add_gain                    : 4 ;  //  7: 4
	UINT32 reg_v_motion_gain                  : 4 ;  // 11: 8
	UINT32 reg_h_motion_gain                  : 4 ;  // 15:12
	UINT32 reg_cc_diff_gain                   : 4 ;  // 19:16
} PE_H15A0_ND1_IPC_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0xc80042a4L ipc_ctrl_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 n1b_motion_gain_y0                 : 8 ;  //  7: 0
	UINT32 n1b_motion_gain_x0                 : 8 ;  // 15: 8
	UINT32 n1b_motion_gain_y1                 : 8 ;  // 23:16
	UINT32 n1b_motion_gain_x1                 : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_43_T;

/*-----------------------------------------------------------------------------
		0xc80042a8L ipc_ctrl_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 n1b_variance_gain_y0               : 8 ;  //  7: 0
	UINT32 n1b_variance_gain_x0               : 8 ;  // 15: 8
	UINT32 n1b_variance_gain_y1               : 8 ;  // 23:16
	UINT32 n1b_variance_gain_x1               : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_44_T;

/*-----------------------------------------------------------------------------
		0xc80042f0L ipc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cubic_coef0                    : 8 ;  //  7: 0
	UINT32 reg_cubic_coef1                    : 8 ;  // 15: 8
	UINT32 reg_va_blending_ctrl_lv1           : 3 ;  // 18:16
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_va_blending_ctrl_lv2           : 3 ;  // 22:20
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_va_blending_ctrl_lv3           : 3 ;  // 26:24
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_va_blending_ctrl_lv4           : 3 ;  // 30:28
} PE_H15A0_ND1_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc80042f4L ipc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_cubic                       : 1 ;  //     0
	UINT32 reg_boundary_window                : 1 ;  //     1
	UINT32 reg_force_va                       : 1 ;  //     2
	UINT32 reg_half_pel_va                    : 1 ;  //     3
	UINT32 reg_dir_med_tap                    : 2 ;  //  5: 4
	UINT32 reg_diff_med_tap                   : 2 ;  //  7: 6
	UINT32 reg_va_blending_ctrl1              : 3 ;  // 10: 8
	UINT32 reg_wide_angle_protection_1        : 1 ;  //    11
	UINT32 reg_va_blending_ctrl2              : 3 ;  // 14:12
	UINT32 reg_wide_angle_protection_2        : 1 ;  //    15
	UINT32 reg_errs_chroma_blend_coef         : 8 ;  // 23:16
	UINT32 reg_strong_global_th               : 6 ;  // 29:24
	UINT32 narrow_angle_week                  : 1 ;  //    30
	UINT32 narrow_angle_protection            : 1 ;  //    31
} PE_H15A0_ND1_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc80042f8L ipc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain               : 8 ;  //  7: 0
	UINT32 cr_flicker_amp_gain                : 8 ;  // 15: 8
} PE_H15A0_ND1_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc80042fcL ipc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                      : 1 ;  //     0
	UINT32 cr_lock_th                         : 7 ;  //  7: 1
	UINT32 cr_lr_ratio_th                     : 8 ;  // 15: 8
	UINT32 cr_hmc_expend                      : 3 ;  // 18:16
	UINT32 cr_lr_ratio_check_en               : 1 ;  //    19
	UINT32 cr_robustness                      : 4 ;  // 23:20
	UINT32 cr_center_weight                   : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc8004300L ipc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                        : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_noise_th                        : 8 ;  // 15: 8
	UINT32 cr_gds_resolution                  : 2 ;  // 17:16
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 cr_hmc_h_range                     : 12;  // 31:20
} PE_H15A0_ND1_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc8004304L ipc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdiff_en                       : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_vdiff_th                       : 8 ;  // 15: 8
	UINT32 reg_hmc_err_max                    : 8 ;  // 23:16
	UINT32 reg_hmc_err_min                    : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc8004308L ipc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                    : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_fmd_ath                         : 8 ;  // 15: 8
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cr_fmd_ratio_th                    : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc800430cL ipc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0              : 8 ;  //  7: 0
	UINT32 chroma_st_flt_ctrl_x1              : 8 ;  // 15: 8
	UINT32 luma_st_flt_ctrl_x0                : 8 ;  // 23:16
	UINT32 luma_st_flt_ctrl_x1                : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc8004310L ipc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hmc_bonus_es_en                : 1 ;  //     0
	UINT32 reg_hmc_bonus_et_en                : 1 ;  //     1
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_hmc_bonus_es                   : 8 ;  // 15: 8
	UINT32 reg_hmc_bonus_th                   : 8 ;  // 23:16
	UINT32 reg_hmc_bonus_et                   : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc8004314L ipc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 hmc_flt_ctrl_x0                    : 8 ;  // 23:16
	UINT32 hmc_st_flt_ctrl_x1                 : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc8004318L ipc_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vmc_motion_th                  : 8 ;  //  7: 0
	UINT32 reg_vmc_debug                      : 1 ;  //     8
	UINT32 reg_vmc_en                         : 1 ;  //     9
	UINT32 reg_vmc_still_check                : 1 ;  //    10
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_vmc_mul                        : 2 ;  // 13:12
	UINT32 reg_vmc_out_mul                    : 2 ;  // 15:14
	UINT32 reg_vmc_protect_th                 : 8 ;  // 23:16
	UINT32 reg_protection_en                  : 1 ;  //    24
	UINT32 reg_2filed_check_en                : 1 ;  //    25
	UINT32 reg_vmc_tnrmotion_dis              : 1 ;  //    26
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_vmc_mm_ratio                   : 4 ;  // 31:28
} PE_H15A0_ND1_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc800431cL ipc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8
	UINT32 reg_pndiff_disable                 : 1 ;  //    16
} PE_H15A0_ND1_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc8004320L ipc_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tava_errt_y2                       : 8 ;  //  7: 0
	UINT32 tava_errt_x2                       : 8 ;  // 15: 8
	UINT32 tava_errt_y3                       : 8 ;  // 23:16
	UINT32 tava_errt_x3                       : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc8004324L ipc_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tava_errt_y0                       : 8 ;  //  7: 0
	UINT32 tava_errt_x0                       : 8 ;  // 15: 8
	UINT32 tava_errt_y1                       : 8 ;  // 23:16
	UINT32 tava_errt_x1                       : 8 ;  // 31:24
} PE_H15A0_ND1_IPC_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc8004328L ipc_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count_d                     : 16;  // 15: 0
	UINT32 r_nc_v_count_d                     : 16;  // 31:16
} PE_H15A0_ND1_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc800432cL ipc_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same_d                     : 16;  // 15: 0
	UINT32 r_pn_v_count_d                     : 16;  // 31:16
} PE_H15A0_ND1_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc8004330L ipc_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0
	UINT32 r_fmd_32_min_th_flag_d             : 1 ;  //     6
	UINT32 r_fmd_32_max_th_flag_d             : 1 ;  //     7
	UINT32 r_fmd_still_limit_flag_d           : 1 ;  //     8
	UINT32 r_fmd_still_pn_th2_flag_d          : 1 ;  //     9
	UINT32 r_fmd_still_pn_th_flag_d           : 1 ;  //    10
	UINT32 r_fmd_still_prev_ratio_flag_d      : 1 ;  //    11
	UINT32 r_fmd_bigger_flag_d                : 1 ;  //    12
	UINT32 r_fmd_smaller_flag_d               : 1 ;  //    13
	UINT32 r_fmd_3_2_fmd_comb_mode_d          : 2 ;  // 15:14
	UINT32 r_pn_diff_count_d                  : 16;  // 31:16
} PE_H15A0_ND1_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc8004334L ipc_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt                 : 16;  // 15: 0
	UINT32 r_inv_original_cnt                 : 16;  // 31:16
} PE_H15A0_ND1_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc8004338L ipc_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_ND1_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc800433cL ipc_status_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_ND1_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
		0xc8004340L tpd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_show_scale                     : 3 ;  //  2: 0
	UINT32 reg_bbd_show                       : 1 ;  //     3
	UINT32 reg_show_bar_1                     : 1 ;  //     4
	UINT32 reg_show_bar_2                     : 1 ;  //     5
	UINT32 reg_show_bar_3                     : 1 ;  //     6
	UINT32 reg_bar1_data_mux                  : 2 ;  //  8: 7
	UINT32 reg_bar2_data_mux                  : 2 ;  // 10: 9
	UINT32 reg_bar3_data_mux                  : 2 ;  // 12:11
	UINT32 reg_hist0_data_mux                 : 4 ;  // 16:13
	UINT32 reg_hist1_data_mux                 : 4 ;  // 20:17
	UINT32 reg_hist2_data_mux                 : 4 ;  // 24:21
	UINT32 reg_hist0_win_sel                  : 2 ;  // 26:25
	UINT32 reg_hist1_win_sel                  : 2 ;  // 28:27
	UINT32 reg_hist2_win_sel                  : 2 ;  // 30:29
	UINT32 hist_rd_en                         : 1 ;  //    31
} PE_H15A0_ND1_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8004344L tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                            : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_wina_hen                       : 1 ;  //    15
	UINT32 wina_y0                            : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_wina_ven                       : 1 ;  //    31
} PE_H15A0_ND1_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8004348L tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                            : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wina_y1                            : 12;  // 27:16
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 reg_hist_hsv_hsl_sel               : 1 ;  //    30
	UINT32 reg_hist_vscaling_en               : 1 ;  //    31
} PE_H15A0_ND1_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800434cL tpd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x0                            : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winb_hen                       : 1 ;  //    15
	UINT32 winb_y0                            : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winb_ven                       : 1 ;  //    31
} PE_H15A0_ND1_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8004350L tpd_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x1                            : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winb_y1                            : 12;  // 27:16
} PE_H15A0_ND1_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8004354L tpd_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x0                            : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winc_hen                       : 1 ;  //    15
	UINT32 winc_y0                            : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winc_ven                       : 1 ;  //    31
} PE_H15A0_ND1_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8004358L tpd_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x1                            : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winc_y1                            : 12;  // 27:16
} PE_H15A0_ND1_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800435cL tpd_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_y                        : 10;  //  9: 0
	UINT32 reg_force_y_max_th                 : 10;  // 19:10
	UINT32 reg_force_y_min_th                 : 10;  // 29:20
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_y_enable                 : 1 ;  //    31
} PE_H15A0_ND1_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8004360L tpd_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cb                       : 10;  //  9: 0
	UINT32 reg_force_cb_max_th                : 10;  // 19:10
	UINT32 reg_force_cb_min_th                : 10;  // 29:20
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cb_enable                : 1 ;  //    31
} PE_H15A0_ND1_TPD_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8004364L tpd_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cr                       : 10;  //  9: 0
	UINT32 reg_force_cr_max_th                : 10;  // 19:10
	UINT32 reg_force_cr_min_th                : 10;  // 29:20
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cr_enable                : 1 ;  //    31
} PE_H15A0_ND1_TPD_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8004368L tpd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_pixel_count                   : 22;  // 21: 0
} PE_H15A0_ND1_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc800436cL clc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_lcount_enable                   : 1 ;  //     0
	UINT32 cl_lcnt_mode                       : 2 ;  //  2: 1
	UINT32 cl_scene_change_enable             : 1 ;  //     3
	UINT32 cl_scene_change_mode               : 1 ;  //     4
	UINT32 cl_scene_change_manual             : 1 ;  //     5
	UINT32 cl_lcnt_th                         : 4 ;  //  9: 6
	UINT32 cl_plus_lcount                     : 3 ;  // 12:10
	UINT32 cl_minus_lcount                    : 2 ;  // 14:13
	UINT32 cl_scene_change_protect            : 1 ;  //    15
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 cl_tearing_pt_th2                  : 12;  // 31:20
} PE_H15A0_ND1_CLC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8004370L bbd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bbd_en                         : 1 ;  //     0
	UINT32 reg_hys_mode                       : 5 ;  //  5: 1
	UINT32 reg_op_mode                        : 1 ;  //     6
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cnt_th                         : 8 ;  // 15: 8
	UINT32 reg_diff_th                        : 8 ;  // 23:16
	UINT32 reg_bbd_mux                        : 3 ;  // 26:24
	UINT32 reg_apl_mux                        : 3 ;  // 29:27
	UINT32 reg_apl_win_sel                    : 2 ;  // 31:30
} PE_H15A0_ND1_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8004374L bbd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16
} PE_H15A0_ND1_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8004378L bbd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_ND1_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800437cL bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16
} PE_H15A0_ND1_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8004380L bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_ND1_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc8004384L apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                          : 10;  //  9: 0
	UINT32 cb_b_s_apl                         : 10;  // 19:10
	UINT32 cr_r_s_apl                         : 10;  // 29:20
} PE_H15A0_ND1_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8004390L tpd_hist_ai_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 5 ;  //  4: 0
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32 reg_read_hist_sel                  : 2 ;  // 14:13
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_available                  : 1 ;  //    28
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 hist_read_done                     : 1 ;  //    31
} PE_H15A0_ND1_TPD_HIST_AI_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8004394L tpd_hist_ai_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_status                        : 22;  // 21: 0
} PE_H15A0_ND1_TPD_HIST_AI_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8004398L lvcrtl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 y_offset                           : 10;  // 13: 4
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 center_position                    : 8 ;  // 23:16
	UINT32 y_gain                             : 8 ;  // 31:24
} PE_H15A0_ND1_LVCRTL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80043a0L tnr_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 block_motion_03                    : 8 ;  //  7: 0
	UINT32 block_motion_02                    : 8 ;  // 15: 8
	UINT32 block_motion_01                    : 8 ;  // 23:16
	UINT32 block_motion_00                    : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc80043a4L tnr_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 block_motion_13                    : 8 ;  //  7: 0
	UINT32 block_motion_12                    : 8 ;  // 15: 8
	UINT32 block_motion_11                    : 8 ;  // 23:16
	UINT32 block_motion_10                    : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc80043a8L tnr_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 block_motion_23                    : 8 ;  //  7: 0
	UINT32 block_motion_22                    : 8 ;  // 15: 8
	UINT32 block_motion_21                    : 8 ;  // 23:16
	UINT32 block_motion_20                    : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc80043acL tnr_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 block_motion_33                    : 8 ;  //  7: 0
	UINT32 block_motion_32                    : 8 ;  // 15: 8
	UINT32 block_motion_31                    : 8 ;  // 23:16
	UINT32 block_motion_30                    : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc80043d0L tnr_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 reg_a_nomalize_en                  : 1 ;  //     2
	UINT32 reg_t_nomalize_en                  : 1 ;  //     3
	UINT32 reg_t_cut_resolution               : 2 ;  //  5: 4
	UINT32 reg_ne_nomalize_en                 : 1 ;  //     6
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 reg_cross_th_ne                    : 8 ;  // 23:16
	UINT32 reg_cross_th                       : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc80043d4L tnr_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mul_base_t                     : 8 ;  //  7: 0
	UINT32 reg_mul_base_e                     : 8 ;  // 15: 8
} PE_H15A0_ND1_TNR_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc80043d8L tnr_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_edge_y0                  : 8 ;  //  7: 0
	UINT32 reg_a_lut_edge_x0                  : 8 ;  // 15: 8
	UINT32 reg_a_lut_edge_y1                  : 8 ;  // 23:16
	UINT32 reg_a_lut_edge_x1                  : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0xc80043dcL tnr_ctrl_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_detail_y0                : 8 ;  //  7: 0
	UINT32 reg_a_lut_detail_x0                : 8 ;  // 15: 8
	UINT32 reg_a_lut_detail_y1                : 8 ;  // 23:16
	UINT32 reg_a_lut_detail_x1                : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_43_T;

/*-----------------------------------------------------------------------------
		0xc80043e0L tnr_ctrl_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_edge_y0                  : 8 ;  //  7: 0
	UINT32 reg_t_lut_edge_x0                  : 8 ;  // 15: 8
	UINT32 reg_t_lut_edge_y1                  : 8 ;  // 23:16
	UINT32 reg_t_lut_edge_x1                  : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_44_T;

/*-----------------------------------------------------------------------------
		0xc80043e4L tnr_ctrl_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_detail_y0                : 8 ;  //  7: 0
	UINT32 reg_t_lut_detail_x0                : 8 ;  // 15: 8
	UINT32 reg_t_lut_detail_y1                : 8 ;  // 23:16
	UINT32 reg_t_lut_detail_x1                : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_45_T;

/*-----------------------------------------------------------------------------
		0xc80043e8L tnr_ctrl_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_final_lut_edge_y0              : 8 ;  //  7: 0
	UINT32 reg_final_lut_edge_x0              : 8 ;  // 15: 8
	UINT32 reg_final_lut_edge_y1              : 8 ;  // 23:16
	UINT32 reg_final_lut_edge_x1              : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_46_T;

/*-----------------------------------------------------------------------------
		0xc80043ecL tnr_ctrl_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_final_lut_detail_y0            : 8 ;  //  7: 0
	UINT32 reg_final_lut_detail_x0            : 8 ;  // 15: 8
	UINT32 reg_final_lut_detail_y1            : 8 ;  // 23:16
	UINT32 reg_final_lut_detail_x1            : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_47_T;

/*-----------------------------------------------------------------------------
		0xc80043f0L tnr_ctrl_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_atmap_mode                     : 2 ;  //  1: 0
} PE_H15A0_ND1_TNR_CTRL_48_T;

/*-----------------------------------------------------------------------------
		0xc80043f4L tnr_ctrl_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_atmap_th                       : 8 ;  //  7: 0
	UINT32 reg_noise_th2                      : 8 ;  // 15: 8
	UINT32 reg_noise_th1                      : 8 ;  // 23:16
	UINT32 reg_noise_th0                      : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_49_T;

/*-----------------------------------------------------------------------------
		0xc80043f8L tnr_ctrl_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_atmap_mode                     : 2 ;  //  1: 0
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_csc_en                         : 1 ;  //     4
	UINT32 reg_csc_sel                        : 1 ;  //     5
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_c_diff_mode                    : 2 ;  //  9: 8
} PE_H15A0_ND1_TNR_CTRL_50_T;

/*-----------------------------------------------------------------------------
		0xc80043fcL tnr_ctrl_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_atmap_th                       : 8 ;  //  7: 0
	UINT32 reg_noise_th2                      : 8 ;  // 15: 8
	UINT32 reg_noise_th1                      : 8 ;  // 23:16
	UINT32 reg_noise_th0                      : 8 ;  // 31:24
} PE_H15A0_ND1_TNR_CTRL_51_T;

/*-----------------------------------------------------------------------------
		0xc8004400L tnr_ne_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th0                    : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8004404L tnr_ne_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th1                    : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc8004408L tnr_ne_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th2                    : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc800440cL tnr_ne_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th3                    : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc8004410L tnr_ne_stat_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th0                   : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_04_T;

/*-----------------------------------------------------------------------------
		0xc8004414L tnr_ne_stat_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th1                   : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_05_T;

/*-----------------------------------------------------------------------------
		0xc8004418L tnr_ne_stat_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th2                   : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_06_T;

/*-----------------------------------------------------------------------------
		0xc800441cL tnr_ne_stat_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th3                   : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_07_T;

/*-----------------------------------------------------------------------------
		0xc8004420L tnr_ne_stat_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th0                    : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_08_T;

/*-----------------------------------------------------------------------------
		0xc8004424L tnr_ne_stat_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th1                    : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_09_T;

/*-----------------------------------------------------------------------------
		0xc8004428L tnr_ne_stat_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th2                    : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_10_T;

/*-----------------------------------------------------------------------------
		0xc800442cL tnr_ne_stat_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th3                    : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_11_T;

/*-----------------------------------------------------------------------------
		0xc8004430L tnr_ne_stat_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th0                   : 32;  // 31: 0
} PE_H15A0_ND1_TNR_NE_STAT_12_T;

/*-----------------------------------------------------------------------------
		0xc8005100L pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0
	UINT32 load_enable                        : 1 ;  //     1
	UINT32 load_type                          : 1 ;  //     2
} PE_H15A0_ND2_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc8005104L pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 timer_rst_sel                      : 1 ;  //     8
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16
} PE_H15A0_ND2_PE0_INTR_T;

/*-----------------------------------------------------------------------------
		0xc8005108L pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                       : 4 ;  //  3: 0
	UINT32 ipc_frame_id                       : 4 ;  //  7: 4
	UINT32 tnrw_y_frame_id                    : 4 ;  // 11: 8
	UINT32 tnrw_c_frame_id                    : 4 ;  // 15:12
	UINT32 dnr_frame_id                       : 8 ;  // 23:16
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24
} PE_H15A0_ND2_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc800510cL pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 timer_cnt                          : 16;  // 31:16
} PE_H15A0_ND2_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc8005110L pe0_opmode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_c_en                           : 1 ;  //     0
	UINT32 tnr_y_en                           : 1 ;  //     1
	UINT32 ipc_c_en                           : 1 ;  //     2
	UINT32 ipc_y_en                           : 1 ;  //     3
	UINT32 tnr_c_444_as_420                   : 1 ;  //     4
	UINT32 tnr_y2_en                          : 1 ;  //     5
	UINT32 force_422_to_420                   : 1 ;  //     6
	UINT32 ipc0_y_en_prog                     : 1 ;  //     7
	UINT32 mif_last_mode                      : 3 ;  // 10: 8
	UINT32 tnrw_off                           : 1 ;  //    11
	UINT32                                    : 1 ;  //    12     reserved
	UINT32 tnr_m_en                           : 1 ;  //    13
	UINT32 force_444_to_422                   : 1 ;  //    14
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 detour_enable                      : 1 ;  //    16
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 ibuf_en                            : 2 ;  // 21:20
	UINT32 minfo_vhalf_mode                   : 2 ;  // 23:22
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 tnr_in_lsb_mode                    : 2 ;  // 29:28
	UINT32 m_run_id                           : 2 ;  // 31:30
} PE_H15A0_ND2_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc8005114L pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0
	UINT32                                    : 1 ;  //    12     reserved
	UINT32 cs_type                            : 3 ;  // 15:13
	UINT32 vsize                              : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31
} PE_H15A0_ND2_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8005118L pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4
	UINT32 field_inv                          : 1 ;  //     6
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 secam_line                         : 1 ;  //     8
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 secam_line_mode                    : 2 ;  // 13:12
} PE_H15A0_ND2_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
		0xc8005120L pe0_in_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset                            : 12;  // 11: 0
	UINT32                                    : 18;  // 29:12     reserved
	UINT32 hsize_sel                          : 1 ;  //    30
	UINT32 crop_en                            : 1 ;  //    31
} PE_H15A0_ND2_PE0_IN_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc8005124L pe0_in_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vsize                              : 12;  // 27:16
} PE_H15A0_ND2_PE0_IN_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8005128L fbc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fbc_core_en_y                      : 1 ;  //     0
	UINT32 fbc_core_20p_y                     : 1 ;  //     1
	UINT32                                    : 13;  // 14: 2     reserved
	UINT32 fbc_en_y                           : 1 ;  //    15
	UINT32 fbc_core_en_c                      : 1 ;  //    16
	UINT32 fbc_core_20p_c                     : 1 ;  //    17
	UINT32                                    : 13;  // 30:18     reserved
	UINT32 fbc_en_y_c                         : 1 ;  //    31
} PE_H15A0_ND2_FBC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8005130L pe0_debug_mem_req ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_rd_t1_y                         : 1 ;  //     0
	UINT32 en_rd_t1_c                         : 1 ;  //     1
	UINT32 en_rd_t2_y                         : 1 ;  //     2
	UINT32 en_rd_t2_c                         : 1 ;  //     3
	UINT32 en_rd_t3_y                         : 1 ;  //     4
	UINT32 en_rd_t3_c                         : 1 ;  //     5
	UINT32 en_rd_t4_y                         : 1 ;  //     6
	UINT32 en_rd_t1_m                         : 1 ;  //     7
	UINT32 en_wr_tnrw_y                       : 1 ;  //     8
	UINT32 en_wr_tnrw_c                       : 1 ;  //     9
	UINT32 en_wr_tnrw_m                       : 1 ;  //    10
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 manual_mem_en                      : 1 ;  //    15
} PE_H15A0_ND2_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
		0xc8005134L pe0_debug_lreq_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                         : 11;  // 10: 0
	UINT32 lreq_dly_c_en                      : 1 ;  //    11
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 lreq_dly_y                         : 11;  // 26:16
	UINT32 lreq_dly_y_en                      : 1 ;  //    27
} PE_H15A0_ND2_PE0_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
		0xc8005140L tnr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tnr_en                         : 1 ;  //     0
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 reg_sc_detect_hres                 : 1 ;  //     2
	UINT32 reg_clc_nt_pal                     : 1 ;  //     3
	UINT32 reg_clc_mem_wr_mode                : 2 ;  //  5: 4
	UINT32 reg_sad_8x3_res                    : 2 ;  //  7: 6
	UINT32 reg_sad_8x3_iir_en                 : 1 ;  //     8
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 reg_minfo_sw                       : 2 ;  // 11:10
	UINT32 reg_minfo_mode10                   : 2 ;  // 13:12
	UINT32 reg_minfo_flt                      : 2 ;  // 15:14
	UINT32 reg_dnr_motion_scale               : 2 ;  // 17:16
	UINT32 reg_minfo_mode32                   : 2 ;  // 19:18
	UINT32 reg_minfo_mode74                   : 4 ;  // 23:20
	UINT32 reg_sad_8x3_iir_alpha              : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8005144L tnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_show                       : 4 ;  //  3: 0
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 reg_dbg_tnrw                       : 2 ;  // 17:16
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_tnri_debug_mode                : 2 ;  // 21:20
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_bsum_scale                     : 2 ;  // 25:24
} PE_H15A0_ND2_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8005148L tnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sc_en                              : 1 ;  //     0
	UINT32 sc_max_flt_en                      : 1 ;  //     1
	UINT32 sc_calc_prev_hist                  : 1 ;  //     2
	UINT32 sc_chroma_en                       : 1 ;  //     3
	UINT32 sc_scale                           : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sc_max_flt_tap                     : 3 ;  // 10: 8
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 sc_chroma_gain                     : 3 ;  // 14:12
	UINT32 sc_out_mode                        : 1 ;  //    15
	UINT32 sc_th_ma                           : 8 ;  // 23:16
	UINT32 sc_th_memc                         : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800514cL tnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_scale                 : 3 ;  //  2: 0
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 show_dbg_bar_en                    : 4 ;  //  7: 4
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 show_dbg_bar0_mux                  : 4 ;  // 19:16
	UINT32 show_dbg_bar1_mux                  : 4 ;  // 23:20
	UINT32 show_dbg_bar2_mux                  : 4 ;  // 27:24
	UINT32 show_dbg_bar3_mux                  : 4 ;  // 31:28
} PE_H15A0_ND2_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8005150L tnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 manual_debug_bar_09                : 8 ;  //  7: 0
	UINT32 manual_debug_bar_10                : 8 ;  // 15: 8
	UINT32 manual_debug_bar_11                : 8 ;  // 23:16
	UINT32 manual_debug_bar_12                : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8005154L tnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mamc_blend_mode                : 2 ;  //  1: 0
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_mamc_alpha                     : 8 ;  // 15: 8
	UINT32 reg_mv_pv_fix_v                    : 4 ;  // 19:16
	UINT32 reg_mv_pv_fix_h                    : 5 ;  // 24:20
	UINT32                                    : 6 ;  // 30:25     reserved
	UINT32 reg_mv_pv_fix_en                   : 1 ;  //    31
} PE_H15A0_ND2_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8005158L tnr_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 me_lb_sad_base                     : 8 ;  //  7: 0
	UINT32 pv_smooth_sad_base                 : 8 ;  // 15: 8
	UINT32 reg_me_lb_polarity                 : 1 ;  //    16
} PE_H15A0_ND2_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800515cL tnr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 reg_sc_en                          : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_5x5_sad_cut                    : 1 ;  //     8
} PE_H15A0_ND2_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8005160L tnr_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_h_fix_val                   : 5 ;  //  4: 0
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_mv_v_fix_val                   : 4 ;  // 11: 8
	UINT32 reg_mv_h_fix_fb_val                : 5 ;  // 16:12
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_mv_v_fix_fb_val                : 4 ;  // 23:20
	UINT32 reg_debug_sel                      : 4 ;  // 27:24
	UINT32 reg_lb_monitor_sel                 : 2 ;  // 29:28
	UINT32 reg_mv_fb_fix                      : 1 ;  //    30
	UINT32 reg_mv_fix                         : 1 ;  //    31
} PE_H15A0_ND2_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8005164L tnr_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_base                           : 7 ;  //  6: 0
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 lb_smooth_clip                     : 6 ;  // 13: 8
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pv_smooth_clip                     : 6 ;  // 21:16
} PE_H15A0_ND2_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8005168L tnr_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 phase_bm_off                       : 2 ;  //  1: 0
	UINT32 phase_sm_off                       : 2 ;  //  3: 2
	UINT32 phase_lb_off                       : 2 ;  //  5: 4
	UINT32 phase_pv_off                       : 2 ;  //  7: 6
	UINT32 phase_mc_off                       : 2 ;  //  9: 8
} PE_H15A0_ND2_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800516cL tnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lb_smooth_en                       : 1 ;  //     0
	UINT32 pv_smooth_en                       : 1 ;  //     1
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 lb_mv_h_off                        : 2 ;  //  5: 4
	UINT32 lb_mv_v_off                        : 2 ;  //  7: 6
	UINT32 pv_mv_h_off                        : 2 ;  //  9: 8
	UINT32 pv_mv_v_off                        : 2 ;  // 11:10
	UINT32 reg_mv_fb_reverse                  : 1 ;  //    12
} PE_H15A0_ND2_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8005170L tnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8005174L tnr_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc8005178L tnr_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc800517cL tnr_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc8005180L tnr_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sad_acc_mode                   : 2 ;  //  1: 0
	UINT32 reg_bmv_fix_en                     : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_bmv_fix_v                      : 4 ;  //  7: 4
	UINT32 reg_bmv_fix_h                      : 5 ;  // 12: 8
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_n_hblk                         : 4 ;  // 19:16
	UINT32 reg_n_vblk                         : 5 ;  // 24:20
} PE_H15A0_ND2_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc8005184L tnr_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_blk_vsize                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_blk_hsize                      : 12;  // 27:16
} PE_H15A0_ND2_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc8005188L tnr_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bsad_scale                         : 3 ;  //  2: 0
	UINT32 bmv_rst_mode                       : 1 ;  //     3
	UINT32 gsad_scale                         : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 gmv_gmv_tl                         : 7 ;  // 14: 8
	UINT32 gmv_gmv_tl_en                      : 1 ;  //    15
	UINT32 bmv_gmv_tl                         : 7 ;  // 22:16
	UINT32 bmv_gmv_tl_en                      : 1 ;  //    23
	UINT32 bmv_pmv_tl                         : 7 ;  // 30:24
	UINT32 bmv_pmv_tl_en                      : 1 ;  //    31
} PE_H15A0_ND2_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc800518cL tnr_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 gmv_0mv_tl                         : 7 ;  // 22:16
	UINT32 gmv_0mv_tl_en                      : 1 ;  //    23
	UINT32 bmv_0mv_tl                         : 7 ;  // 30:24
	UINT32 bmv_0mv_tl_en                      : 1 ;  //    31
} PE_H15A0_ND2_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc8005190L tnr_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_detail_beta_mul                : 8 ;  //  7: 0
	UINT32 reg_detail_alpha_mul               : 8 ;  // 15: 8
	UINT32 reg_edge_beta_mul                  : 8 ;  // 23:16
	UINT32 reg_edge_alpha_mul                 : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc8005194L tnr_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_svar_scale                     : 3 ;  //  2: 0
} PE_H15A0_ND2_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc8005198L tnr_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc800519cL tnr_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc80051a0L tnr_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 reg_luma_gain_en                   : 1 ;  //     4
	UINT32 reg_skin_gain_en                   : 1 ;  //     5
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_sad_sum_cut                    : 2 ;  //  9: 8
	UINT32 reg_sad_cut                        : 2 ;  // 11:10
	UINT32 reg_5x5_sad_cut                    : 2 ;  // 13:12
	UINT32                                    : 6 ;  // 19:14     reserved
	UINT32 reg_debug_sel                      : 4 ;  // 23:20
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 chroma_sad_en                      : 1 ;  //    31
} PE_H15A0_ND2_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc80051a4L tnr_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_flat_y_blur_en                 : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_flat_y_cut_resolution          : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_flat_y_G0                      : 6 ;  // 13: 8
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_flat_y_G4                      : 4 ;  // 19:16
	UINT32 reg_flat_y_G3                      : 4 ;  // 23:20
	UINT32 reg_flat_y_G2                      : 4 ;  // 27:24
	UINT32 reg_flat_y_G1                      : 4 ;  // 31:28
} PE_H15A0_ND2_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc80051a8L tnr_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_flat_c_blur_en                 : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_flat_c_cut_resolution          : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_flat_c_G0                      : 6 ;  // 13: 8
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_flat_c_G4                      : 4 ;  // 19:16
	UINT32 reg_flat_c_G3                      : 4 ;  // 23:20
	UINT32 reg_flat_c_G2                      : 4 ;  // 27:24
	UINT32 reg_flat_c_G1                      : 4 ;  // 31:28
} PE_H15A0_ND2_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc80051acL tnr_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc80051b0L tnr_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre_filtered_en                    : 1 ;  //     0
	UINT32 luma_gain_en                       : 1 ;  //     1
	UINT32 cut_resolution                     : 2 ;  //  3: 2
	UINT32                                    : 1 ;  //     4     reserved
	UINT32 reg_pre_detail_cttl_g0             : 7 ;  // 11: 5
	UINT32 reg_pre_detail_cttl_g4             : 5 ;  // 16:12
	UINT32 reg_pre_detail_cttl_g3             : 5 ;  // 21:17
	UINT32 reg_pre_detail_cttl_g2             : 5 ;  // 26:22
	UINT32 reg_pre_detail_cttl_g1             : 5 ;  // 31:27
} PE_H15A0_ND2_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc80051b4L tnr_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dir_blur_en                    : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_dir_blur_gain                  : 8 ;  // 15: 8
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 reg_diff_limit                     : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc80051b8L tnr_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denoise_filter_en              : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_denoise_cut_resolution         : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_denoise_gain                   : 8 ;  // 15: 8
	UINT32 reg_denoise_G0                     : 7 ;  // 22:16
} PE_H15A0_ND2_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc80051bcL tnr_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_denoise_G4                     : 5 ;  //  4: 0
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_denoise_G3                     : 5 ;  // 12: 8
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_denoise_G2                     : 5 ;  // 20:16
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_denoise_G1                     : 5 ;  // 28:24
} PE_H15A0_ND2_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc80051c0L tnr_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_gsad                             : 8 ;  //  7: 0
	UINT32 w_gmv                              : 9 ;  // 16: 8
} PE_H15A0_ND2_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc80051c4L tnr_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_apl_motion                       : 8 ;  //  7: 0
	UINT32 w_apl_mc_sad                       : 8 ;  // 15: 8
	UINT32 w_apl_var                          : 8 ;  // 23:16
} PE_H15A0_ND2_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc80051c8L tnr_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_ND2_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc80051ccL tnr_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_ND2_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc80051d0L tnr_main_lut_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr                : 6 ;  //  5: 0
	UINT32                                    : 6 ;  // 11: 6     reserved
	UINT32 main_lut_ai_enable                 : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 main_lut_load                      : 1 ;  //    15
} PE_H15A0_ND2_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
		0xc80051d4L tnr_main_lut_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0               : 32;  // 31: 0
} PE_H15A0_ND2_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
		0xc80051dcL tnr_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sc_measure                         : 8 ;  //  7: 0
	UINT32 apl_y                              : 8 ;  // 15: 8
	UINT32 nlvl                               : 6 ;  // 21:16
} PE_H15A0_ND2_TNR_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc80051e0L tnr_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 reg_sad_sum_cut                    : 2 ;  //  9: 8
	UINT32 reg_sad_cut                        : 2 ;  // 11:10
} PE_H15A0_ND2_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc80051e4L tnr_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc80051e8L tnr_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc80051ecL tnr_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc80051f0L tnr_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc80051f4L tnr_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc80051f8L tnr_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc80051fcL tnr_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_ND2_TNR_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc8005200L ipc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_luma_mode                       : 2 ;  //  1: 0
	UINT32                                    : 1 ;  //     2     reserved
	UINT32 cr_n1_cc_blend_en                  : 1 ;  //     3
	UINT32 cr_chroma_mode                     : 2 ;  //  5: 4
	UINT32 cr_integer_edi                     : 1 ;  //     6
	UINT32 cr_chroma_edi                      : 1 ;  //     7
	UINT32 cr_st_2d_v_flt_range2              : 4 ;  // 11: 8
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 15:12
	UINT32 cr_ori_3d_blur_y                   : 2 ;  // 17:16
	UINT32 cr_ori_3d_blur_c                   : 2 ;  // 19:18
	UINT32 cr_tava_v_edge_adap                : 1 ;  //    20
	UINT32 cr_tava_debug_mode                 : 1 ;  //    21
	UINT32 cr_tava_st_adap                    : 1 ;  //    22
	UINT32 cr_tava_en                         : 1 ;  //    23
	UINT32 cr_hmc_force_fmd                   : 1 ;  //    24
	UINT32 cr_st_2d_v_flt_en                  : 1 ;  //    25
	UINT32 cr_clc_fmd_off                     : 1 ;  //    26
	UINT32 cr_chroma_tava_only                : 1 ;  //    27
	UINT32 cr_film_auto_gain                  : 1 ;  //    28
	UINT32 cr_film_apply_c                    : 1 ;  //    29
	UINT32 cr_film_apply_y                    : 1 ;  //    30
	UINT32 cr_film_mode_enable                : 1 ;  //    31
} PE_H15A0_ND2_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8005204L ipc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_en_sel1                     : 2 ;  //  1: 0
	UINT32 cr_hmc_en_sel2                     : 2 ;  //  3: 2
	UINT32                                    : 2 ;  //  5: 4     reserved
	UINT32 cr_fmd_vfilter_en                  : 1 ;  //     6
	UINT32 cr_fend_auto                       : 1 ;  //     7
	UINT32 cr_st_iir_dec_gain                 : 4 ;  // 11: 8
	UINT32 cr_st_iir_inc_gain                 : 4 ;  // 15:12
	UINT32 cr_st_flt_enable                   : 1 ;  //    16
	UINT32 cr_st_iir_en                       : 1 ;  //    17
	UINT32 cr_hmc_flt_enable                  : 1 ;  //    18
	UINT32 cr_game_mode                       : 1 ;  //    19
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 23:20
	UINT32                                    : 1 ;  //    24     reserved
	UINT32 cr_black_mask                      : 1 ;  //    25
	UINT32 cr_ipc_debug_show                  : 5 ;  // 30:26
	UINT32 cr_5f_mode                         : 1 ;  //    31
} PE_H15A0_ND2_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8005208L ipc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                : 8 ;  //  7: 0
	UINT32 fuzzy_ctrl_point_x4                : 8 ;  // 15: 8
	UINT32 fuzzy_ctrl_point_x1                : 8 ;  // 23:16
	UINT32 fuzzy_ctrl_point_y1                : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800520cL ipc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                : 8 ;  //  7: 0
	UINT32 fuzzy_ctrl_point_y2                : 8 ;  // 15: 8
	UINT32 fuzzy_ctrl_point_x3                : 8 ;  // 23:16
	UINT32 fuzzy_ctrl_point_y3                : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8005210L ipc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset__s7                      : 8 ;  //  7: 0
	UINT32 motion_x_tearing_gain              : 8 ;  // 15: 8
	UINT32 tearing_gain                       : 8 ;  // 23:16
	UINT32 motion_gain                        : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8005214L ipc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 12;  // 11: 0     reserved
	UINT32 cr_t_grad_weight                   : 4 ;  // 15:12
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 cr_v_grad_weight                   : 4 ;  // 23:20
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 cr_h_grad_weight                   : 4 ;  // 31:28
} PE_H15A0_ND2_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8005218L ipc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_normalize_en                    : 1 ;  //     0
	UINT32 cr_flat_only                       : 1 ;  //     1
	UINT32 cr_motion_iir_en                   : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 cr_tearing_normalize_en            : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cr_normalize_base                  : 8 ;  // 15: 8
	UINT32 cr_normalize_center                : 8 ;  // 23:16
	UINT32 cr_motion_coring                   : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800521cL ipc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cr_v_en                            : 1 ;  //     1
	UINT32 cr_45_en                           : 1 ;  //     2
	UINT32 cr_15_en                           : 1 ;  //     3
	UINT32                                    : 20;  // 23: 4     reserved
	UINT32 cr_iir_motion_gain                 : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8005220L ipc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 fmd_caption_range_bot              : 12;  // 27:16
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 caption_flow_detected              : 1 ;  //    30
	UINT32 hmc_when_cap_not_detected          : 1 ;  //    31
} PE_H15A0_ND2_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8005224L ipc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                        : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pn_variance_th                     : 8 ;  // 23:16
	UINT32 pn_fmd_motion_th                   : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8005228L ipc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                      : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pcnc_variance_th                   : 8 ;  // 23:16
	UINT32 pcnc_fmd_motion_th                 : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800522cL ipc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                       : 8 ;  //  7: 0
	UINT32 cr_center_bound_ctrl               : 8 ;  // 15: 8
	UINT32 cr_dn_bound_ctrl                   : 8 ;  // 23:16
	UINT32 cr_up_bound_ctrl                   : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8005230L ipc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8005234L ipc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_tava_chroma_mul                 : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 cr_hmc_alpha_th                    : 8 ;  // 19:12
	UINT32 cr_hmc_debug_mode                  : 1 ;  //    20
	UINT32 cr_hmc_debug_gain                  : 6 ;  // 26:21
	UINT32 cr_hmc_debug_dir                   : 5 ;  // 31:27
} PE_H15A0_ND2_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc8005238L ipc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0
	UINT32 reserved                           : 1 ;  //     1
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8
	UINT32 reg_same_th                        : 8 ;  // 23:16
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc800523cL ipc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0
	UINT32 reg_3_2_detect_only                : 1 ;  //     1
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3
	UINT32 reg_hd_mode                        : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc8005240L ipc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31
} PE_H15A0_ND2_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc8005244L ipc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution                  : 2 ;  //  1: 0
	UINT32 cr_field_manual_mode               : 2 ;  //  3: 2
	UINT32 cr_field_inverse                   : 1 ;  //     4
	UINT32                                    : 19;  // 23: 5     reserved
	UINT32 cr_tearing_a_th                    : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc8005248L ipc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                         : 8 ;  //  7: 0
	UINT32 cr_lr_gain                         : 8 ;  // 15: 8
	UINT32 cr_h_grad_gain                     : 8 ;  // 23:16
	UINT32 cr_v_grad_gain                     : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc800524cL ipc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_errs_gain                      : 8 ;  //  7: 0
	UINT32 reg_confidance_gain                : 8 ;  // 15: 8
	UINT32 reg_2d_offset                      : 8 ;  // 23:16
	UINT32 reg_flicker_gain                   : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc8005250L ipc_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count                       : 16;  // 15: 0
	UINT32 r_nc_v_count                       : 16;  // 31:16
} PE_H15A0_ND2_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc8005254L ipc_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same                       : 16;  // 15: 0
	UINT32 r_pn_v_count                       : 16;  // 31:16
} PE_H15A0_ND2_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc8005258L ipc_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0
	UINT32 r_fmd_32_min_th_flag_u             : 1 ;  //     6
	UINT32 r_fmd_32_max_th_flag_u             : 1 ;  //     7
	UINT32 r_fmd_still_limit_flag_u           : 1 ;  //     8
	UINT32 r_fmd_still_pn_th2_flag_u          : 1 ;  //     9
	UINT32 r_fmd_still_pn_th_flag_u           : 1 ;  //    10
	UINT32 r_fmd_still_prev_ratio_flag_u      : 1 ;  //    11
	UINT32 r_fmd_bigger_flag_u                : 1 ;  //    12
	UINT32 r_fmd_smaller_flag_u               : 1 ;  //    13
	UINT32 r_fmd_3_2_fmd_comb_mode_u          : 2 ;  // 15:14
	UINT32 r_pn_diff_count                    : 16;  // 31:16
} PE_H15A0_ND2_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc800525cL ipc_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 same_ng_flag_d                     : 1 ;  //     0
	UINT32 too_small_flag_d                   : 1 ;  //     1
	UINT32 r_fmd_still_flag_d                 : 1 ;  //     2
	UINT32 r_fmd_possibility_flag_d           : 1 ;  //     3
	UINT32 base_pn_ratio_flag_d               : 1 ;  //     4
	UINT32 r_32_detected_flag_d               : 1 ;  //     5
	UINT32 r_fmd_comb_mode_d                  : 2 ;  //  7: 6
	UINT32 r_fmd_lock_count_d                 : 8 ;  // 15: 8
	UINT32 same_ng_flag_u                     : 1 ;  //    16
	UINT32 too_small_flag_u                   : 1 ;  //    17
	UINT32 r_fmd_still_flag_u                 : 1 ;  //    18
	UINT32 r_fmd_possibility_flag_u           : 1 ;  //    19
	UINT32 base_pn_ratio_flag_u               : 1 ;  //    20
	UINT32 r_32_uetected_flag_u               : 1 ;  //    21
	UINT32 r_fmd_comb_mode_u                  : 2 ;  // 23:22
	UINT32 r_fmd_lock_count_u                 : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc8005260L clc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cl_filter_enable                   : 1 ;  //     1
	UINT32 clc_detection_enable               : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 motion_window                      : 2 ;  //  5: 4
	UINT32 cl_pattern_dist                    : 2 ;  //  7: 6
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 cl_ab_ratio                        : 4 ;  // 15:12
	UINT32 cl_y_hdistance                     : 2 ;  // 17:16
	UINT32 cl_c_hdistance                     : 2 ;  // 19:18
	UINT32 cl_y_protect_en                    : 1 ;  //    20
	UINT32                                    : 7 ;  // 27:21     reserved
	UINT32 cl_motion_a_filter                 : 1 ;  //    28
	UINT32 cl_motion_b_filter                 : 1 ;  //    29
	UINT32 cl_motion_a_mode                   : 1 ;  //    30
} PE_H15A0_ND2_CLC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8005264L clc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_motion_a_th                     : 8 ;  //  7: 0
	UINT32 cl_motion_b_th                     : 8 ;  // 15: 8
	UINT32 cl_y_protect_th0                   : 8 ;  // 23:16
	UINT32 cl_y_protect_th1                   : 8 ;  // 31:24
} PE_H15A0_ND2_CLC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8005268L clc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_y_bound_th                      : 7 ;  //  6: 0
	UINT32 cl_c_bound_use_4line               : 1 ;  //     7
	UINT32 cl_c_bound_th                      : 7 ;  // 14: 8
	UINT32 cl_c_bound_type                    : 1 ;  //    15
	UINT32 cl_y_bound_width_chroma            : 2 ;  // 17:16
	UINT32 cl_y_bound_ignore_cc               : 1 ;  //    18
	UINT32 cl_y_bound_ignore_p2               : 1 ;  //    19
	UINT32 cl_y_bound_ignore_p4               : 1 ;  //    20
	UINT32 reg_bin3x1_flt_en                  : 1 ;  //    21
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_bin5x1_flt_en                  : 1 ;  //    24
	UINT32 reg_bin5x1_flt_th                  : 3 ;  // 27:25
	UINT32 reg_vm_original_mode               : 1 ;  //    28
	UINT32 reg_vm_other_mode                  : 1 ;  //    29
	UINT32 cl_motionp2n2_th_pal_high          : 2 ;  // 31:30
} PE_H15A0_ND2_CLC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800526cL clc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_sat_min                         : 8 ;  //  7: 0
	UINT32                                    : 2 ;  //  9: 8     reserved
	UINT32 cl_var_measure_th1                 : 10;  // 19:10
	UINT32 cl_var_measure_th2                 : 10;  // 29:20
	UINT32 cl_pt_enable                       : 2 ;  // 31:30
} PE_H15A0_ND2_CLC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8005270L clc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_tearing_pt_th1                  : 12;  // 11: 0
	UINT32 cl_motionp2n2_th_pal_low           : 8 ;  // 19:12
	UINT32 cl_frame_min                       : 12;  // 31:20
} PE_H15A0_ND2_CLC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8005274L clc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_data_line_switching_sel         : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 clc_bin_flt_th                     : 3 ;  // 10: 8
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 clc_bin_flt_en                     : 1 ;  //    15
	UINT32 clc_dbg_show_mask                  : 13;  // 28:16
} PE_H15A0_ND2_CLC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8005278L clc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_c_filter_gain                   : 8 ;  //  7: 0
	UINT32 cc_y_filter_when_cc_detected       : 1 ;  //     8
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 cl_cc_original_en                  : 1 ;  //    10
	UINT32 clc_force_non_fmd                  : 1 ;  //    11
	UINT32 cl_c_filter_when_cl_detected       : 1 ;  //    12
	UINT32 cl_y_filter_when_cl_detected       : 1 ;  //    13
	UINT32 clc_filter_mode                    : 2 ;  // 15:14
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cl_cc_protection_th                : 8 ;  // 31:24
} PE_H15A0_ND2_CLC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800527cL clc_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 cl_detection_count                 : 16;  // 31:16
} PE_H15A0_ND2_CLC_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8005280L ipc_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tava_v_diff_y0                     : 8 ;  //  7: 0
	UINT32 tava_v_diff_x0                     : 8 ;  // 15: 8
	UINT32 tava_v_diff_y1                     : 8 ;  // 23:16
	UINT32 tava_v_diff_x1                     : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc8005284L ipc_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_div_en                         : 1 ;  //     0
	UINT32 cr_bound_exp_en                    : 1 ;  //     1
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cr_sub_main_sel                    : 1 ;  //     4
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 reg_manual_en                      : 1 ;  //    16
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_manual_mode2                   : 2 ;  // 21:20
	UINT32 reg_manual_mode1                   : 2 ;  // 23:22
	UINT32 reg_div_position                   : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc8005288L ipc_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc800528cL ipc_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0
	UINT32 reserved                           : 1 ;  //     1
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8
	UINT32 reg_same_th                        : 8 ;  // 23:16
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc8005290L ipc_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0
	UINT32 reg_3_2_detect_only                : 1 ;  //     1
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3
	UINT32 reg_hd_mode                        : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc8005294L ipc_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31
} PE_H15A0_ND2_IPC_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc8005298L ipc_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8
	UINT32 reg_pndiff_disable                 : 1 ;  //    16
} PE_H15A0_ND2_IPC_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc800529cL ipc_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fend_position                  : 11;  // 10: 0
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 cr_sc_length                       : 3 ;  // 14:12
	UINT32                                    : 11;  // 25:15     reserved
	UINT32 cr_vmc_dir                         : 2 ;  // 27:26
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 cr_vmc_en                          : 1 ;  //    31
} PE_H15A0_ND2_IPC_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc80052a0L ipc_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_yy_diff_gain                   : 4 ;  //  3: 0
	UINT32 reg_hv_add_gain                    : 4 ;  //  7: 4
	UINT32 reg_v_motion_gain                  : 4 ;  // 11: 8
	UINT32 reg_h_motion_gain                  : 4 ;  // 15:12
	UINT32 reg_cc_diff_gain                   : 4 ;  // 19:16
} PE_H15A0_ND2_IPC_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0xc80052a4L ipc_ctrl_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 n1b_motion_gain_y0                 : 8 ;  //  7: 0
	UINT32 n1b_motion_gain_x0                 : 8 ;  // 15: 8
	UINT32 n1b_motion_gain_y1                 : 8 ;  // 23:16
	UINT32 n1b_motion_gain_x1                 : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_43_T;

/*-----------------------------------------------------------------------------
		0xc80052a8L ipc_ctrl_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 n1b_variance_gain_y0               : 8 ;  //  7: 0
	UINT32 n1b_variance_gain_x0               : 8 ;  // 15: 8
	UINT32 n1b_variance_gain_y1               : 8 ;  // 23:16
	UINT32 n1b_variance_gain_x1               : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_44_T;

/*-----------------------------------------------------------------------------
		0xc80052f0L ipc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cubic_coef0                    : 8 ;  //  7: 0
	UINT32 reg_cubic_coef1                    : 8 ;  // 15: 8
	UINT32 reg_va_blending_ctrl_lv1           : 3 ;  // 18:16
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_va_blending_ctrl_lv2           : 3 ;  // 22:20
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_va_blending_ctrl_lv3           : 3 ;  // 26:24
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_va_blending_ctrl_lv4           : 3 ;  // 30:28
} PE_H15A0_ND2_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc80052f4L ipc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_cubic                       : 1 ;  //     0
	UINT32 reg_boundary_window                : 1 ;  //     1
	UINT32 reg_force_va                       : 1 ;  //     2
	UINT32 reg_half_pel_va                    : 1 ;  //     3
	UINT32 reg_dir_med_tap                    : 2 ;  //  5: 4
	UINT32 reg_diff_med_tap                   : 2 ;  //  7: 6
	UINT32 reg_va_blending_ctrl1              : 3 ;  // 10: 8
	UINT32 reg_wide_angle_protection_1        : 1 ;  //    11
	UINT32 reg_va_blending_ctrl2              : 3 ;  // 14:12
	UINT32 reg_wide_angle_protection_2        : 1 ;  //    15
	UINT32 reg_errs_chroma_blend_coef         : 8 ;  // 23:16
	UINT32 reg_strong_global_th               : 6 ;  // 29:24
	UINT32 narrow_angle_week                  : 1 ;  //    30
	UINT32 narrow_angle_protection            : 1 ;  //    31
} PE_H15A0_ND2_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc80052f8L ipc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain               : 8 ;  //  7: 0
	UINT32 cr_flicker_amp_gain                : 8 ;  // 15: 8
} PE_H15A0_ND2_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc80052fcL ipc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                      : 1 ;  //     0
	UINT32 cr_lock_th                         : 7 ;  //  7: 1
	UINT32 cr_lr_ratio_th                     : 8 ;  // 15: 8
	UINT32 cr_hmc_expend                      : 3 ;  // 18:16
	UINT32 cr_lr_ratio_check_en               : 1 ;  //    19
	UINT32 cr_robustness                      : 4 ;  // 23:20
	UINT32 cr_center_weight                   : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc8005300L ipc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                        : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_noise_th                        : 8 ;  // 15: 8
	UINT32 cr_gds_resolution                  : 2 ;  // 17:16
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 cr_hmc_h_range                     : 12;  // 31:20
} PE_H15A0_ND2_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc8005304L ipc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdiff_en                       : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_vdiff_th                       : 8 ;  // 15: 8
	UINT32 reg_hmc_err_max                    : 8 ;  // 23:16
	UINT32 reg_hmc_err_min                    : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc8005308L ipc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                    : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_fmd_ath                         : 8 ;  // 15: 8
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cr_fmd_ratio_th                    : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc800530cL ipc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0              : 8 ;  //  7: 0
	UINT32 chroma_st_flt_ctrl_x1              : 8 ;  // 15: 8
	UINT32 luma_st_flt_ctrl_x0                : 8 ;  // 23:16
	UINT32 luma_st_flt_ctrl_x1                : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc8005310L ipc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hmc_bonus_es_en                : 1 ;  //     0
	UINT32 reg_hmc_bonus_et_en                : 1 ;  //     1
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_hmc_bonus_es                   : 8 ;  // 15: 8
	UINT32 reg_hmc_bonus_th                   : 8 ;  // 23:16
	UINT32 reg_hmc_bonus_et                   : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc8005314L ipc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 hmc_flt_ctrl_x0                    : 8 ;  // 23:16
	UINT32 hmc_st_flt_ctrl_x1                 : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc8005318L ipc_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vmc_motion_th                  : 8 ;  //  7: 0
	UINT32 reg_vmc_debug                      : 1 ;  //     8
	UINT32 reg_vmc_en                         : 1 ;  //     9
	UINT32 reg_vmc_still_check                : 1 ;  //    10
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_vmc_mul                        : 2 ;  // 13:12
	UINT32 reg_vmc_out_mul                    : 2 ;  // 15:14
	UINT32 reg_vmc_protect_th                 : 8 ;  // 23:16
	UINT32 reg_protection_en                  : 1 ;  //    24
	UINT32 reg_2filed_check_en                : 1 ;  //    25
	UINT32 reg_vmc_tnrmotion_dis              : 1 ;  //    26
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_vmc_mm_ratio                   : 4 ;  // 31:28
} PE_H15A0_ND2_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc800531cL ipc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8
	UINT32 reg_pndiff_disable                 : 1 ;  //    16
} PE_H15A0_ND2_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc8005320L ipc_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tava_errt_y2                       : 8 ;  //  7: 0
	UINT32 tava_errt_x2                       : 8 ;  // 15: 8
	UINT32 tava_errt_y3                       : 8 ;  // 23:16
	UINT32 tava_errt_x3                       : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc8005324L ipc_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tava_errt_y0                       : 8 ;  //  7: 0
	UINT32 tava_errt_x0                       : 8 ;  // 15: 8
	UINT32 tava_errt_y1                       : 8 ;  // 23:16
	UINT32 tava_errt_x1                       : 8 ;  // 31:24
} PE_H15A0_ND2_IPC_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc8005328L ipc_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count_d                     : 16;  // 15: 0
	UINT32 r_nc_v_count_d                     : 16;  // 31:16
} PE_H15A0_ND2_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc800532cL ipc_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same_d                     : 16;  // 15: 0
	UINT32 r_pn_v_count_d                     : 16;  // 31:16
} PE_H15A0_ND2_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc8005330L ipc_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0
	UINT32 r_fmd_32_min_th_flag_d             : 1 ;  //     6
	UINT32 r_fmd_32_max_th_flag_d             : 1 ;  //     7
	UINT32 r_fmd_still_limit_flag_d           : 1 ;  //     8
	UINT32 r_fmd_still_pn_th2_flag_d          : 1 ;  //     9
	UINT32 r_fmd_still_pn_th_flag_d           : 1 ;  //    10
	UINT32 r_fmd_still_prev_ratio_flag_d      : 1 ;  //    11
	UINT32 r_fmd_bigger_flag_d                : 1 ;  //    12
	UINT32 r_fmd_smaller_flag_d               : 1 ;  //    13
	UINT32 r_fmd_3_2_fmd_comb_mode_d          : 2 ;  // 15:14
	UINT32 r_pn_diff_count_d                  : 16;  // 31:16
} PE_H15A0_ND2_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc8005334L ipc_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt                 : 16;  // 15: 0
	UINT32 r_inv_original_cnt                 : 16;  // 31:16
} PE_H15A0_ND2_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc8005338L ipc_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_ND2_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc800533cL ipc_status_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_ND2_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
		0xc8005340L tpd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_show_scale                     : 3 ;  //  2: 0
	UINT32 reg_bbd_show                       : 1 ;  //     3
	UINT32 reg_show_bar_1                     : 1 ;  //     4
	UINT32 reg_show_bar_2                     : 1 ;  //     5
	UINT32 reg_show_bar_3                     : 1 ;  //     6
	UINT32 reg_bar1_data_mux                  : 2 ;  //  8: 7
	UINT32 reg_bar2_data_mux                  : 2 ;  // 10: 9
	UINT32 reg_bar3_data_mux                  : 2 ;  // 12:11
	UINT32 reg_hist0_data_mux                 : 4 ;  // 16:13
	UINT32 reg_hist1_data_mux                 : 4 ;  // 20:17
	UINT32 reg_hist2_data_mux                 : 4 ;  // 24:21
	UINT32 reg_hist0_win_sel                  : 2 ;  // 26:25
	UINT32 reg_hist1_win_sel                  : 2 ;  // 28:27
	UINT32 reg_hist2_win_sel                  : 2 ;  // 30:29
	UINT32 hist_rd_en                         : 1 ;  //    31
} PE_H15A0_ND2_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8005344L tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                            : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_wina_hen                       : 1 ;  //    15
	UINT32 wina_y0                            : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_wina_ven                       : 1 ;  //    31
} PE_H15A0_ND2_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8005348L tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                            : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wina_y1                            : 12;  // 27:16
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 reg_hist_hsv_hsl_sel               : 1 ;  //    30
	UINT32 reg_hist_vscaling_en               : 1 ;  //    31
} PE_H15A0_ND2_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800534cL tpd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x0                            : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winb_hen                       : 1 ;  //    15
	UINT32 winb_y0                            : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winb_ven                       : 1 ;  //    31
} PE_H15A0_ND2_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8005350L tpd_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x1                            : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winb_y1                            : 12;  // 27:16
} PE_H15A0_ND2_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8005354L tpd_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x0                            : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winc_hen                       : 1 ;  //    15
	UINT32 winc_y0                            : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winc_ven                       : 1 ;  //    31
} PE_H15A0_ND2_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8005358L tpd_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x1                            : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winc_y1                            : 12;  // 27:16
} PE_H15A0_ND2_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800535cL tpd_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_y                        : 10;  //  9: 0
	UINT32 reg_force_y_max_th                 : 10;  // 19:10
	UINT32 reg_force_y_min_th                 : 10;  // 29:20
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_y_enable                 : 1 ;  //    31
} PE_H15A0_ND2_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8005360L tpd_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cb                       : 10;  //  9: 0
	UINT32 reg_force_cb_max_th                : 10;  // 19:10
	UINT32 reg_force_cb_min_th                : 10;  // 29:20
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cb_enable                : 1 ;  //    31
} PE_H15A0_ND2_TPD_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8005364L tpd_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cr                       : 10;  //  9: 0
	UINT32 reg_force_cr_max_th                : 10;  // 19:10
	UINT32 reg_force_cr_min_th                : 10;  // 29:20
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cr_enable                : 1 ;  //    31
} PE_H15A0_ND2_TPD_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8005368L tpd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_pixel_count                   : 22;  // 21: 0
} PE_H15A0_ND2_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc800536cL clc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_lcount_enable                   : 1 ;  //     0
	UINT32 cl_lcnt_mode                       : 2 ;  //  2: 1
	UINT32 cl_scene_change_enable             : 1 ;  //     3
	UINT32 cl_scene_change_mode               : 1 ;  //     4
	UINT32 cl_scene_change_manual             : 1 ;  //     5
	UINT32 cl_lcnt_th                         : 4 ;  //  9: 6
	UINT32 cl_plus_lcount                     : 3 ;  // 12:10
	UINT32 cl_minus_lcount                    : 2 ;  // 14:13
	UINT32 cl_scene_change_protect            : 1 ;  //    15
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 cl_tearing_pt_th2                  : 12;  // 31:20
} PE_H15A0_ND2_CLC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8005370L bbd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bbd_en                         : 1 ;  //     0
	UINT32 reg_hys_mode                       : 5 ;  //  5: 1
	UINT32 reg_op_mode                        : 1 ;  //     6
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cnt_th                         : 8 ;  // 15: 8
	UINT32 reg_diff_th                        : 8 ;  // 23:16
	UINT32 reg_bbd_mux                        : 3 ;  // 26:24
	UINT32 reg_apl_mux                        : 3 ;  // 29:27
	UINT32 reg_apl_win_sel                    : 2 ;  // 31:30
} PE_H15A0_ND2_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8005374L bbd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16
} PE_H15A0_ND2_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8005378L bbd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_ND2_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800537cL bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16
} PE_H15A0_ND2_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8005380L bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_ND2_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc8005384L apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                          : 10;  //  9: 0
	UINT32 cb_b_s_apl                         : 10;  // 19:10
	UINT32 cr_r_s_apl                         : 10;  // 29:20
} PE_H15A0_ND2_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8005390L tpd_hist_ai_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 5 ;  //  4: 0
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32 reg_read_hist_sel                  : 2 ;  // 14:13
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_available                  : 1 ;  //    28
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 hist_read_done                     : 1 ;  //    31
} PE_H15A0_ND2_TPD_HIST_AI_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8005394L tpd_hist_ai_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_status                        : 22;  // 21: 0
} PE_H15A0_ND2_TPD_HIST_AI_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8005398L lvcrtl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 y_offset                           : 10;  // 13: 4
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 center_position                    : 8 ;  // 23:16
	UINT32 y_gain                             : 8 ;  // 31:24
} PE_H15A0_ND2_LVCRTL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80053a0L tnr_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 block_motion_03                    : 8 ;  //  7: 0
	UINT32 block_motion_02                    : 8 ;  // 15: 8
	UINT32 block_motion_01                    : 8 ;  // 23:16
	UINT32 block_motion_00                    : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc80053a4L tnr_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 block_motion_13                    : 8 ;  //  7: 0
	UINT32 block_motion_12                    : 8 ;  // 15: 8
	UINT32 block_motion_11                    : 8 ;  // 23:16
	UINT32 block_motion_10                    : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc80053a8L tnr_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 block_motion_23                    : 8 ;  //  7: 0
	UINT32 block_motion_22                    : 8 ;  // 15: 8
	UINT32 block_motion_21                    : 8 ;  // 23:16
	UINT32 block_motion_20                    : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc80053acL tnr_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 block_motion_33                    : 8 ;  //  7: 0
	UINT32 block_motion_32                    : 8 ;  // 15: 8
	UINT32 block_motion_31                    : 8 ;  // 23:16
	UINT32 block_motion_30                    : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc80053d0L tnr_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 reg_a_nomalize_en                  : 1 ;  //     2
	UINT32 reg_t_nomalize_en                  : 1 ;  //     3
	UINT32 reg_t_cut_resolution               : 2 ;  //  5: 4
	UINT32 reg_ne_nomalize_en                 : 1 ;  //     6
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 reg_cross_th_ne                    : 8 ;  // 23:16
	UINT32 reg_cross_th                       : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc80053d4L tnr_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mul_base_t                     : 8 ;  //  7: 0
	UINT32 reg_mul_base_e                     : 8 ;  // 15: 8
} PE_H15A0_ND2_TNR_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc80053d8L tnr_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_edge_y0                  : 8 ;  //  7: 0
	UINT32 reg_a_lut_edge_x0                  : 8 ;  // 15: 8
	UINT32 reg_a_lut_edge_y1                  : 8 ;  // 23:16
	UINT32 reg_a_lut_edge_x1                  : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0xc80053dcL tnr_ctrl_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_a_lut_detail_y0                : 8 ;  //  7: 0
	UINT32 reg_a_lut_detail_x0                : 8 ;  // 15: 8
	UINT32 reg_a_lut_detail_y1                : 8 ;  // 23:16
	UINT32 reg_a_lut_detail_x1                : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_43_T;

/*-----------------------------------------------------------------------------
		0xc80053e0L tnr_ctrl_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_edge_y0                  : 8 ;  //  7: 0
	UINT32 reg_t_lut_edge_x0                  : 8 ;  // 15: 8
	UINT32 reg_t_lut_edge_y1                  : 8 ;  // 23:16
	UINT32 reg_t_lut_edge_x1                  : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_44_T;

/*-----------------------------------------------------------------------------
		0xc80053e4L tnr_ctrl_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_t_lut_detail_y0                : 8 ;  //  7: 0
	UINT32 reg_t_lut_detail_x0                : 8 ;  // 15: 8
	UINT32 reg_t_lut_detail_y1                : 8 ;  // 23:16
	UINT32 reg_t_lut_detail_x1                : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_45_T;

/*-----------------------------------------------------------------------------
		0xc80053e8L tnr_ctrl_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_final_lut_edge_y0              : 8 ;  //  7: 0
	UINT32 reg_final_lut_edge_x0              : 8 ;  // 15: 8
	UINT32 reg_final_lut_edge_y1              : 8 ;  // 23:16
	UINT32 reg_final_lut_edge_x1              : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_46_T;

/*-----------------------------------------------------------------------------
		0xc80053ecL tnr_ctrl_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_final_lut_detail_y0            : 8 ;  //  7: 0
	UINT32 reg_final_lut_detail_x0            : 8 ;  // 15: 8
	UINT32 reg_final_lut_detail_y1            : 8 ;  // 23:16
	UINT32 reg_final_lut_detail_x1            : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_47_T;

/*-----------------------------------------------------------------------------
		0xc80053f0L tnr_ctrl_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_atmap_mode                     : 2 ;  //  1: 0
} PE_H15A0_ND2_TNR_CTRL_48_T;

/*-----------------------------------------------------------------------------
		0xc80053f4L tnr_ctrl_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_atmap_th                       : 8 ;  //  7: 0
	UINT32 reg_noise_th2                      : 8 ;  // 15: 8
	UINT32 reg_noise_th1                      : 8 ;  // 23:16
	UINT32 reg_noise_th0                      : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_49_T;

/*-----------------------------------------------------------------------------
		0xc80053f8L tnr_ctrl_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_atmap_mode                     : 2 ;  //  1: 0
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_csc_en                         : 1 ;  //     4
	UINT32 reg_csc_sel                        : 1 ;  //     5
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_c_diff_mode                    : 2 ;  //  9: 8
} PE_H15A0_ND2_TNR_CTRL_50_T;

/*-----------------------------------------------------------------------------
		0xc80053fcL tnr_ctrl_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_atmap_th                       : 8 ;  //  7: 0
	UINT32 reg_noise_th2                      : 8 ;  // 15: 8
	UINT32 reg_noise_th1                      : 8 ;  // 23:16
	UINT32 reg_noise_th0                      : 8 ;  // 31:24
} PE_H15A0_ND2_TNR_CTRL_51_T;

/*-----------------------------------------------------------------------------
		0xc8005400L tnr_ne_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th0                    : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8005404L tnr_ne_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th1                    : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc8005408L tnr_ne_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th2                    : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc800540cL tnr_ne_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th3                    : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc8005410L tnr_ne_stat_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th0                   : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_04_T;

/*-----------------------------------------------------------------------------
		0xc8005414L tnr_ne_stat_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th1                   : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_05_T;

/*-----------------------------------------------------------------------------
		0xc8005418L tnr_ne_stat_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th2                   : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_06_T;

/*-----------------------------------------------------------------------------
		0xc800541cL tnr_ne_stat_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th3                   : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_07_T;

/*-----------------------------------------------------------------------------
		0xc8005420L tnr_ne_stat_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th0                    : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_08_T;

/*-----------------------------------------------------------------------------
		0xc8005424L tnr_ne_stat_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th1                    : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_09_T;

/*-----------------------------------------------------------------------------
		0xc8005428L tnr_ne_stat_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th2                    : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_10_T;

/*-----------------------------------------------------------------------------
		0xc800542cL tnr_ne_stat_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_f_ncnt_th3                    : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_11_T;

/*-----------------------------------------------------------------------------
		0xc8005430L tnr_ne_stat_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_ed_ncnt_th0                   : 32;  // 31: 0
} PE_H15A0_ND2_TNR_NE_STAT_12_T;

/*-----------------------------------------------------------------------------
		0xc8006100L pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0
	UINT32 load_enable                        : 1 ;  //     1
	UINT32 load_type                          : 1 ;  //     2
} PE_H15A0_NDS_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc8006104L pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 timer_rst_sel                      : 1 ;  //     8
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16
} PE_H15A0_NDS_PE0_INTR_T;

/*-----------------------------------------------------------------------------
		0xc8006108L pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ipc_frame_id                       : 4 ;  //  7: 4
	UINT32 tnrw_y_frame_id                    : 4 ;  // 11: 8
	UINT32 tnrw_c_frame_id                    : 4 ;  // 15:12
	UINT32 dnr_frame_id                       : 8 ;  // 23:16
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24
} PE_H15A0_NDS_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc800610cL pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 timer_cnt                          : 16;  // 31:16
} PE_H15A0_NDS_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc8006110L pe0_opmode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_c_en                           : 1 ;  //     0
	UINT32 tnr_y_en                           : 1 ;  //     1
	UINT32 ipc_c_en                           : 1 ;  //     2
	UINT32 ipc_y_en                           : 1 ;  //     3
	UINT32 tnr_c_422_as_420                   : 1 ;  //     4
	UINT32 tnr_y2_en                          : 1 ;  //     5
	UINT32 force_422_to_420                   : 1 ;  //     6
	UINT32 ipc0_y_en_prog                     : 1 ;  //     7
	UINT32 mif_last_mode                      : 3 ;  // 10: 8
	UINT32 tnrw_off                           : 1 ;  //    11
	UINT32 dnr_out_force_44x                  : 1 ;  //    12
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 detour_enable                      : 1 ;  //    16
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 ibuf_en                            : 2 ;  // 21:20
	UINT32                                    : 6 ;  // 27:22     reserved
	UINT32 tnr_in_lsb_mode                    : 2 ;  // 29:28
} PE_H15A0_NDS_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc8006114L pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 11;  // 10: 0
	UINT32                                    : 2 ;  // 12:11     reserved
	UINT32 cs_type                            : 3 ;  // 15:13
	UINT32 vsize                              : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31
} PE_H15A0_NDS_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8006118L pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4
	UINT32 field_inv                          : 1 ;  //     6
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 secam_line                         : 1 ;  //     8
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 secam_line_mode                    : 2 ;  // 13:12
} PE_H15A0_NDS_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
		0xc8006120L pe0_in_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset                            : 12;  // 11: 0
	UINT32                                    : 18;  // 29:12     reserved
	UINT32 hsize_sel                          : 1 ;  //    30
	UINT32 crop_en                            : 1 ;  //    31
} PE_H15A0_NDS_PE0_IN_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc8006124L pe0_in_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vsize                              : 12;  // 27:16
} PE_H15A0_NDS_PE0_IN_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8006128L fbc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fbc_core_en_y                      : 1 ;  //     0
	UINT32 fbc_core_20p_y                     : 1 ;  //     1
	UINT32                                    : 13;  // 14: 2     reserved
	UINT32 fbc_en_y                           : 1 ;  //    15
	UINT32 fbc_core_en_c                      : 1 ;  //    16
	UINT32 fbc_core_20p_c                     : 1 ;  //    17
	UINT32                                    : 13;  // 30:18     reserved
	UINT32 fbc_en_y_c                         : 1 ;  //    31
} PE_H15A0_NDS_FBC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8006130L pe0_debug_mem_req ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_rd_t1_y                         : 1 ;  //     0
	UINT32 en_rd_t1_c                         : 1 ;  //     1
	UINT32 en_rd_t2_y                         : 1 ;  //     2
	UINT32 en_rd_t2_c                         : 1 ;  //     3
	UINT32 en_rd_t3_y                         : 1 ;  //     4
	UINT32 en_rd_t3_c                         : 1 ;  //     5
	UINT32 en_rd_t4_y                         : 1 ;  //     6
	UINT32 en_rd_t1_m                         : 1 ;  //     7
	UINT32 en_wr_tnrw_y                       : 1 ;  //     8
	UINT32 en_wr_tnrw_c                       : 1 ;  //     9
	UINT32 en_wr_tnrw_m                       : 1 ;  //    10
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 manual_mem_en                      : 1 ;  //    15
} PE_H15A0_NDS_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
		0xc8006134L pe0_debug_lreq_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                         : 11;  // 10: 0
	UINT32 lreq_dly_c_en                      : 1 ;  //    11
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 lreq_dly_y                         : 11;  // 26:16
	UINT32 lreq_dly_y_en                      : 1 ;  //    27
} PE_H15A0_NDS_PE0_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
		0xc8006140L tnr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tnr_en                         : 1 ;  //     0
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 reg_sc_detect_hres                 : 1 ;  //     2
} PE_H15A0_NDS_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8006144L tnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_show                       : 3 ;  //  2: 0
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 reg_dbg_tnrw                       : 2 ;  // 17:16
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_tnri_debug_mode                : 2 ;  // 21:20
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_bsum_scale                     : 2 ;  // 25:24
	UINT32                                    : 1 ;  //    26     reserved
	UINT32 reg_bsum_chroma_en                 : 1 ;  //    27
} PE_H15A0_NDS_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8006148L tnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sc_en                              : 1 ;  //     0
	UINT32 sc_max_flt_en                      : 1 ;  //     1
	UINT32 sc_calc_prev_hist                  : 1 ;  //     2
	UINT32 sc_chroma_en                       : 1 ;  //     3
	UINT32 sc_scale                           : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sc_max_flt_tap                     : 3 ;  // 10: 8
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 sc_chroma_gain                     : 3 ;  // 14:12
	UINT32 sc_out_mode                        : 1 ;  //    15
	UINT32 sc_th_ma                           : 8 ;  // 23:16
	UINT32 sc_th_memc                         : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800614cL tnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_scale                 : 3 ;  //  2: 0
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 show_dbg_bar_en                    : 4 ;  //  7: 4
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 show_dbg_bar0_mux                  : 4 ;  // 19:16
	UINT32 show_dbg_bar1_mux                  : 4 ;  // 23:20
	UINT32 show_dbg_bar2_mux                  : 4 ;  // 27:24
} PE_H15A0_NDS_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8006150L tnr_ctrl_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nlvl_motion_scale                  : 2 ;  //  1: 0
	UINT32 nlvl_3pct_sel                      : 2 ;  //  3: 2
	UINT32 nlvl_0peak_x_th                    : 4 ;  //  7: 4
	UINT32 nlvl_0peak_y_th                    : 8 ;  // 15: 8
	UINT32 nlvl_y_th0                         : 8 ;  // 23:16
	UINT32 nlvl_y_th1                         : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_CTRL_51_T;

/*-----------------------------------------------------------------------------
		0xc8006154L tnr_ctrl_52 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_show_bar0                        : 8 ;  //  7: 0
	UINT32 w_show_bar1                        : 8 ;  // 15: 8
	UINT32 w_show_bar2                        : 8 ;  // 23:16
	UINT32 w_show_bar3                        : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_CTRL_52_T;

/*-----------------------------------------------------------------------------
		0xc8006160L tnr_ma_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mc_enable                      : 1 ;  //     0
	UINT32 reg_ma_enable                      : 1 ;  //     1
	UINT32 reg_ma_force_en                    : 1 ;  //     2
	UINT32 reg_ma_err_mw_sel                  : 1 ;  //     3
	UINT32 reg_low_2bit_fix                   : 1 ;  //     4
	UINT32 reg_secam_line_toggle              : 1 ;  //     5
	UINT32 reg_low_2bit                       : 2 ;  //  7: 6
	UINT32 reg_luma_adap_en                   : 1 ;  //     8
	UINT32 reg_g_motion_mode                  : 1 ;  //     9
	UINT32 reg_mw_4bit_mode                   : 1 ;  //    10
	UINT32 reg_skin_adap_en                   : 1 ;  //    11
	UINT32 reg_secam_mode_en                  : 1 ;  //    12
	UINT32 reg_secam_xn                       : 3 ;  // 15:13
	UINT32 reg_ma_force_gain                  : 8 ;  // 23:16
	UINT32 reg_sc_reset_mode                  : 2 ;  // 25:24
	UINT32 reg_sc_mc_off                      : 1 ;  //    26
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_debug_show                     : 4 ;  // 31:28
} PE_H15A0_NDS_TNR_MA_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8006164L tnr_ma_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_preblur_en              : 1 ;  //     0
	UINT32 reg_var_preblur_en                 : 1 ;  //     1
	UINT32 reg_lc_diff_blur_en                : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_lc_diff_res                    : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_f_variance_res                 : 3 ;  // 10: 8
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_f_motion_res                   : 3 ;  // 14:12
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_chroma_gain                    : 8 ;  // 23:16
	UINT32 reg_g_blend_a                      : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8006168L tnr_ma_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 reg_cr_max_gain                    : 8 ;  // 15: 8
	UINT32 reg_chroma_en                      : 1 ;  //    16
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_cut_resolution                 : 3 ;  // 22:20
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_h_gain                         : 4 ;  // 27:24
	UINT32 reg_v_gain                         : 4 ;  // 31:28
} PE_H15A0_NDS_TNR_MA_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800616cL tnr_ma_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_normalize_en                   : 1 ;  //     0
	UINT32 reg_inc_only_en                    : 1 ;  //     1
	UINT32 reg_iir_en                         : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_motion_gain                    : 4 ;  //  7: 4
	UINT32 reg_offset                         : 8 ;  // 15: 8
	UINT32 reg_var_base                       : 8 ;  // 23:16
	UINT32 reg_var_center                     : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8006170L tnr_ma_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_iir_max_gain                   : 8 ;  //  7: 0
	UINT32 reg_iir_diff_min                   : 8 ;  // 15: 8
	UINT32 reg_iir_diff_max                   : 8 ;  // 23:16
	UINT32 reg_iir_decrease_ratio             : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8006174L tnr_ma_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmotion_adap_en                : 1 ;  //     0
	UINT32 reg_lut_sel                        : 1 ;  //     1
	UINT32                                    : 4 ;  //  5: 2     reserved
	UINT32 reg_filter_size                    : 2 ;  //  7: 6
	UINT32 reg_strong_255_fix                 : 1 ;  //     8
	UINT32 reg_weak_000_fix                   : 1 ;  //     9
} PE_H15A0_NDS_TNR_MA_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8006178L tnr_ma_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 reg_y_mot_secam_th                 : 8 ;  // 15: 8
	UINT32 reg_c_v_secam_th                   : 8 ;  // 23:16
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 reg_c_v_calc_mode                  : 2 ;  // 31:30
} PE_H15A0_NDS_TNR_MA_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800617cL tnr_ma_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_chroma_en                      : 1 ;  //     0
	UINT32 reg_iir_en                         : 1 ;  //     1
	UINT32 reg_sc_reset_en                    : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_cut_resolution                 : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cut_noise_es                   : 4 ;  // 11: 8
	UINT32 reg_cut_secam                      : 4 ;  // 15:12
	UINT32 reg_iir_alpha                      : 8 ;  // 23:16
	UINT32 reg_iir_diff_max                   : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8006180L tnr_ma_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 3 ;  //  2: 0     reserved
	UINT32 reg_skin_motion_y0                 : 5 ;  //  7: 3
	UINT32 reg_skin_motion_x0                 : 8 ;  // 15: 8
	UINT32                                    : 3 ;  // 18:16     reserved
	UINT32 reg_skin_motion_y1                 : 5 ;  // 23:19
	UINT32 reg_skin_motion_x1                 : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8006184L tnr_ma_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_luma_ctrl_x3                   : 8 ;  //  7: 0
	UINT32 reg_luma_ctrl_x2                   : 8 ;  // 15: 8
	UINT32 reg_luma_ctrl_x1                   : 8 ;  // 23:16
	UINT32 reg_luma_ctrl_x0                   : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8006188L tnr_ma_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_luma_ctrl_y3                   : 8 ;  //  7: 0
	UINT32 reg_luma_ctrl_y2                   : 8 ;  // 15: 8
	UINT32 reg_luma_ctrl_y1                   : 8 ;  // 23:16
	UINT32 reg_luma_ctrl_y0                   : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800618cL tnr_ma_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 skin_lut_y0                        : 8 ;  //  7: 0
	UINT32 skin_lut_y1                        : 8 ;  // 15: 8
	UINT32 skin_lut_y2                        : 8 ;  // 23:16
	UINT32 skin_lut_y3                        : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8006190L tnr_ma_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 skin_lut_cb0                       : 8 ;  //  7: 0
	UINT32 skin_lut_cb1                       : 8 ;  // 15: 8
	UINT32 skin_lut_cb2                       : 8 ;  // 23:16
	UINT32 skin_lut_cb3                       : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8006194L tnr_ma_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 skin_lut_cr0                       : 8 ;  //  7: 0
	UINT32 skin_lut_cr1                       : 8 ;  // 15: 8
	UINT32 skin_lut_cr2                       : 8 ;  // 23:16
	UINT32 skin_lut_cr3                       : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_MA_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc80061a0L reg_ma_lut_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ma_lut0_x4                         : 8 ;  //  7: 0
	UINT32 ma_lut0_x5                         : 8 ;  // 15: 8
	UINT32 ma_lut0_x6                         : 8 ;  // 23:16
	UINT32 ma_lut0_x7                         : 8 ;  // 31:24
} PE_H15A0_NDS_REG_MA_LUT_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80061a4L reg_ma_lut_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ma_lut0_x3                         : 8 ;  //  7: 0
	UINT32 ma_lut0_x2                         : 8 ;  // 15: 8
	UINT32 ma_lut0_x1                         : 8 ;  // 23:16
	UINT32 ma_lut0_x0                         : 8 ;  // 31:24
} PE_H15A0_NDS_REG_MA_LUT_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80061a8L reg_ma_lut_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ma_lut0_y4                         : 8 ;  //  7: 0
	UINT32 ma_lut0_y5                         : 8 ;  // 15: 8
	UINT32 ma_lut0_y6                         : 8 ;  // 23:16
	UINT32 ma_lut0_y7                         : 8 ;  // 31:24
} PE_H15A0_NDS_REG_MA_LUT_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80061acL reg_ma_lut_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ma_lut0_y3                         : 8 ;  //  7: 0
	UINT32 ma_lut0_y2                         : 8 ;  // 15: 8
	UINT32 ma_lut0_y1                         : 8 ;  // 23:16
	UINT32 ma_lut0_y0                         : 8 ;  // 31:24
} PE_H15A0_NDS_REG_MA_LUT_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc80061b0L reg_ma_lut_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ma_lut1_x4                         : 8 ;  //  7: 0
	UINT32 ma_lut1_x5                         : 8 ;  // 15: 8
	UINT32 ma_lut1_x6                         : 8 ;  // 23:16
	UINT32 ma_lut1_x7                         : 8 ;  // 31:24
} PE_H15A0_NDS_REG_MA_LUT_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc80061b4L reg_ma_lut_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ma_lut1_x3                         : 8 ;  //  7: 0
	UINT32 ma_lut1_x2                         : 8 ;  // 15: 8
	UINT32 ma_lut1_x1                         : 8 ;  // 23:16
	UINT32 ma_lut1_x0                         : 8 ;  // 31:24
} PE_H15A0_NDS_REG_MA_LUT_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc80061b8L reg_ma_lut_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ma_lut1_y4                         : 8 ;  //  7: 0
	UINT32 ma_lut1_y5                         : 8 ;  // 15: 8
	UINT32 ma_lut1_y6                         : 8 ;  // 23:16
	UINT32 ma_lut1_y7                         : 8 ;  // 31:24
} PE_H15A0_NDS_REG_MA_LUT_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc80061bcL reg_ma_lut_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ma_lut1_y3                         : 8 ;  //  7: 0
	UINT32 ma_lut1_y2                         : 8 ;  // 15: 8
	UINT32 ma_lut1_y1                         : 8 ;  // 23:16
	UINT32 ma_lut1_y0                         : 8 ;  // 31:24
} PE_H15A0_NDS_REG_MA_LUT_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc80061c8L tnr_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 ma_var                             : 8 ;  // 23:16
	UINT32 ma_motion                          : 8 ;  // 31:24
} PE_H15A0_NDS_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc80061d0L tnr_lut_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_NDS_TNR_LUT_00_T;

/*-----------------------------------------------------------------------------
		0xc80061d4L tnr_lut_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_NDS_TNR_LUT_01_T;

/*-----------------------------------------------------------------------------
		0xc80061d8L tnr_lut_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_NDS_TNR_LUT_02_T;

/*-----------------------------------------------------------------------------
		0xc80061dcL tnr_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 apl_y                              : 8 ;  // 15: 8
	UINT32 nlvl                               : 6 ;  // 21:16
} PE_H15A0_NDS_TNR_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc8006200L ipc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_luma_mode                       : 2 ;  //  1: 0
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cr_chroma_mode                     : 2 ;  //  5: 4
	UINT32 cr_integer_edi                     : 1 ;  //     6
	UINT32 cr_chroma_edi                      : 1 ;  //     7
	UINT32 cr_st_2d_v_flt_range2              : 4 ;  // 11: 8
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 15:12
	UINT32 cr_ori_3d_blur_y                   : 2 ;  // 17:16
	UINT32 cr_ori_3d_blur_c                   : 2 ;  // 19:18
	UINT32 cr_tava_v_edge_adap                : 1 ;  //    20
	UINT32 cr_tava_debug_mode                 : 1 ;  //    21
	UINT32 cr_tava_st_adap                    : 1 ;  //    22
	UINT32 cr_tava_en                         : 1 ;  //    23
	UINT32 cr_hmc_force_fmd                   : 1 ;  //    24
	UINT32 cr_st_2d_v_flt_en                  : 1 ;  //    25
	UINT32 cr_clc_fmd_off                     : 1 ;  //    26
	UINT32 cr_chroma_tava_only                : 1 ;  //    27
	UINT32 cr_film_auto_gain                  : 1 ;  //    28
	UINT32 cr_film_apply_c                    : 1 ;  //    29
	UINT32 cr_film_apply_y                    : 1 ;  //    30
	UINT32 cr_film_mode_enable                : 1 ;  //    31
} PE_H15A0_NDS_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8006204L ipc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 15;  // 14: 0     reserved
	UINT32 cr_fend_auto                       : 1 ;  //    15
	UINT32 cr_st_flt_enable                   : 1 ;  //    16
	UINT32 cr_st_2d_v_flt_en                  : 1 ;  //    17
	UINT32 cr_hmc_flt_enable                  : 1 ;  //    18
	UINT32 cr_game_mode                       : 1 ;  //    19
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 23:20
	UINT32                                    : 1 ;  //    24     reserved
	UINT32 cr_black_mask                      : 1 ;  //    25
	UINT32 cr_ipc_debug_show                  : 5 ;  // 30:26
	UINT32 cr_5f_mode                         : 1 ;  //    31
} PE_H15A0_NDS_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8006208L ipc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                : 8 ;  //  7: 0
	UINT32 fuzzy_ctrl_point_x4                : 8 ;  // 15: 8
	UINT32 fuzzy_ctrl_point_x1                : 8 ;  // 23:16
	UINT32 fuzzy_ctrl_point_y1                : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800620cL ipc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                : 8 ;  //  7: 0
	UINT32 fuzzy_ctrl_point_y2                : 8 ;  // 15: 8
	UINT32 fuzzy_ctrl_point_x3                : 8 ;  // 23:16
	UINT32 fuzzy_ctrl_point_y3                : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8006210L ipc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset__s7                      : 8 ;  //  7: 0
	UINT32 motion_x_tearing_gain              : 8 ;  // 15: 8
	UINT32 tearing_gain                       : 8 ;  // 23:16
	UINT32 motion_gain                        : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8006214L ipc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 12;  // 11: 0     reserved
	UINT32 cr_t_grad_weight                   : 4 ;  // 15:12
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 cr_v_grad_weight                   : 4 ;  // 23:20
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 cr_h_grad_weight                   : 4 ;  // 31:28
} PE_H15A0_NDS_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8006218L ipc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_normalize_en                    : 1 ;  //     0
	UINT32 cr_flat_only                       : 1 ;  //     1
	UINT32 cr_motion_iir_en                   : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 cr_tearing_normalize_en            : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cr_normalize_base                  : 8 ;  // 15: 8
	UINT32 cr_normalize_center                : 8 ;  // 23:16
	UINT32 cr_motion_coring                   : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc800621cL ipc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cr_v_en                            : 1 ;  //     1
	UINT32 cr_45_en                           : 1 ;  //     2
	UINT32 cr_15_en                           : 1 ;  //     3
	UINT32                                    : 20;  // 23: 4     reserved
	UINT32 cr_iir_motion_gain                 : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8006220L ipc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 fmd_caption_range_bot              : 12;  // 27:16
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 caption_flow_detected              : 1 ;  //    30
	UINT32 hmc_when_cap_not_detected          : 1 ;  //    31
} PE_H15A0_NDS_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8006224L ipc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                        : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pn_variance_th                     : 8 ;  // 23:16
	UINT32 pn_fmd_motion_th                   : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8006228L ipc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                      : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pcnc_variance_th                   : 8 ;  // 23:16
	UINT32 pcnc_fmd_motion_th                 : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800622cL ipc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                       : 8 ;  //  7: 0
	UINT32 cr_center_bound_ctrl               : 8 ;  // 15: 8
	UINT32 cr_dn_bound_ctrl                   : 8 ;  // 23:16
	UINT32 cr_up_bound_ctrl                   : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8006230L ipc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8006234L ipc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_debug_view                  : 13;  // 12: 0
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cr_hmc_debug_mode                  : 1 ;  //    16
	UINT32                                    : 2 ;  // 18:17     reserved
	UINT32 cr_hmc_debug_dir                   : 5 ;  // 23:19
	UINT32 cr_hmc_debug_gain                  : 6 ;  // 29:24
} PE_H15A0_NDS_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc8006238L ipc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0
	UINT32 reserved                           : 1 ;  //     1
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8
	UINT32 reg_same_th                        : 8 ;  // 23:16
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc800623cL ipc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0
	UINT32 reg_3_2_detect_only                : 1 ;  //     1
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3
	UINT32 reg_hd_mode                        : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc8006240L ipc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31
} PE_H15A0_NDS_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc8006244L ipc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution                  : 2 ;  //  1: 0
	UINT32 cr_field_manual_mode               : 2 ;  //  3: 2
	UINT32 cr_field_inverse                   : 1 ;  //     4
	UINT32                                    : 19;  // 23: 5     reserved
	UINT32 cr_tearing_a_th                    : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc8006248L ipc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                         : 8 ;  //  7: 0
	UINT32 cr_lr_gain                         : 8 ;  // 15: 8
	UINT32 cr_h_grad_gain                     : 8 ;  // 23:16
	UINT32 cr_v_grad_gain                     : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc800624cL ipc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_errs_gain                      : 8 ;  //  7: 0
	UINT32 reg_confidance_gain                : 8 ;  // 15: 8
	UINT32 reg_2d_offset                      : 8 ;  // 23:16
	UINT32 reg_flicker_gain                   : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc8006250L ipc_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count_u                     : 16;  // 15: 0
	UINT32 r_nc_v_count_u                     : 16;  // 31:16
} PE_H15A0_NDS_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc8006254L ipc_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same_u                     : 16;  // 15: 0
	UINT32 r_pn_v_count_u                     : 16;  // 31:16
} PE_H15A0_NDS_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc8006258L ipc_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0
	UINT32 r_fmd_32_min_th_flag_u             : 1 ;  //     6
	UINT32 r_fmd_32_max_th_flag_u             : 1 ;  //     7
	UINT32 r_fmd_still_limit_flag_u           : 1 ;  //     8
	UINT32 r_fmd_still_pn_th2_flag_u          : 1 ;  //     9
	UINT32 r_fmd_still_pn_th_flag_u           : 1 ;  //    10
	UINT32 r_fmd_still_prev_ratio_flag_u      : 1 ;  //    11
	UINT32 r_fmd_bigger_flag_u                : 1 ;  //    12
	UINT32 r_fmd_smaller_flag_u               : 1 ;  //    13
	UINT32 r_fmd_3_2_fmd_comb_mode_u          : 2 ;  // 15:14
	UINT32 r_pn_diff_count_u                  : 16;  // 31:16
} PE_H15A0_NDS_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc800625cL ipc_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 same_ng_flag_d                     : 1 ;  //     0
	UINT32 too_small_flag_d                   : 1 ;  //     1
	UINT32 r_fmd_still_flag_d                 : 1 ;  //     2
	UINT32 r_fmd_possibility_flag_d           : 1 ;  //     3
	UINT32 base_pn_ratio_flag_d               : 1 ;  //     4
	UINT32 r_32_detected_flag_d               : 1 ;  //     5
	UINT32 r_fmd_comb_mode_d                  : 2 ;  //  7: 6
	UINT32 r_fmd_lock_count_d                 : 8 ;  // 15: 8
	UINT32 same_ng_flag_u                     : 1 ;  //    16
	UINT32 too_small_flag_u                   : 1 ;  //    17
	UINT32 r_fmd_still_flag_u                 : 1 ;  //    18
	UINT32 r_fmd_possibility_flag_u           : 1 ;  //    19
	UINT32 base_pn_ratio_flag_u               : 1 ;  //    20
	UINT32 r_32_uetected_flag_u               : 1 ;  //    21
	UINT32 r_fmd_comb_mode_u                  : 2 ;  // 23:22
	UINT32 r_fmd_lock_count_u                 : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc8006280L ipc_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tava_v_diff_y0                     : 8 ;  //  7: 0
	UINT32 tava_v_diff_x0                     : 8 ;  // 15: 8
	UINT32 tava_v_diff_y1                     : 8 ;  // 23:16
	UINT32 tava_v_diff_x1                     : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc8006284L ipc_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_div_en                         : 1 ;  //     0
	UINT32 cr_bound_exp_en                    : 1 ;  //     1
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cr_sub_main_sel                    : 1 ;  //     4
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 reg_manual_en                      : 1 ;  //    16
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_manual_mode2                   : 2 ;  // 21:20
	UINT32 reg_manual_mode1                   : 2 ;  // 23:22
	UINT32 reg_div_position                   : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc8006288L ipc_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc800628cL ipc_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0
	UINT32 reserved                           : 1 ;  //     1
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8
	UINT32 reg_same_th                        : 8 ;  // 23:16
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc8006290L ipc_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0
	UINT32 reg_3_2_detect_only                : 1 ;  //     1
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3
	UINT32 reg_hd_mode                        : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc8006294L ipc_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31
} PE_H15A0_NDS_IPC_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc8006298L ipc_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8
	UINT32 reg_pndiff_disable                 : 1 ;  //    16
} PE_H15A0_NDS_IPC_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc800629cL ipc_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fend_position                  : 11;  // 10: 0
} PE_H15A0_NDS_IPC_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc80062a0L ipc_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_yy_diff_gain                   : 4 ;  //  3: 0
	UINT32 reg_hv_add_gain                    : 4 ;  //  7: 4
	UINT32 reg_v_motion_gain                  : 4 ;  // 11: 8
	UINT32 reg_h_motion_gain                  : 4 ;  // 15:12
	UINT32 reg_cc_diff_gain                   : 4 ;  // 19:16
} PE_H15A0_NDS_IPC_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0xc80062f0L ipc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cubic_coef0                    : 8 ;  //  7: 0
	UINT32 reg_cubic_coef1                    : 8 ;  // 15: 8
	UINT32 reg_va_blending_ctrl_lv1           : 3 ;  // 18:16
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_va_blending_ctrl_lv2           : 3 ;  // 22:20
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_va_blending_ctrl_lv3           : 3 ;  // 26:24
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_va_blending_ctrl_lv4           : 3 ;  // 30:28
} PE_H15A0_NDS_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc80062f4L ipc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_cubic                       : 1 ;  //     0
	UINT32 reg_boundary_window                : 1 ;  //     1
	UINT32 reg_force_va                       : 1 ;  //     2
	UINT32 reg_half_pel_va                    : 1 ;  //     3
	UINT32 reg_dir_med_tap                    : 2 ;  //  5: 4
	UINT32 reg_diff_med_tap                   : 2 ;  //  7: 6
	UINT32 reg_va_blending_ctrl1              : 3 ;  // 10: 8
	UINT32 reg_wide_angle_protection_1        : 1 ;  //    11
	UINT32 reg_va_blending_ctrl2              : 3 ;  // 14:12
	UINT32 reg_wide_angle_protection_2        : 1 ;  //    15
	UINT32 reg_errs_chroma_blend_coef         : 8 ;  // 23:16
	UINT32 reg_strong_global_th               : 6 ;  // 29:24
	UINT32 narrow_angle_week                  : 1 ;  //    30
	UINT32 narrow_angle_protection            : 1 ;  //    31
} PE_H15A0_NDS_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc80062f8L ipc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain               : 8 ;  //  7: 0
	UINT32 cr_flicker_amp_gain                : 8 ;  // 15: 8
} PE_H15A0_NDS_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc80062fcL ipc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                      : 1 ;  //     0
	UINT32 cr_lock_th                         : 7 ;  //  7: 1
	UINT32 cr_g_motion_th                     : 8 ;  // 15: 8
	UINT32 cr_hmc_expend                      : 3 ;  // 18:16
	UINT32 cr_hmc_half_en                     : 1 ;  //    19
	UINT32 cr_robustness                      : 4 ;  // 23:20
	UINT32 cr_center_weight                   : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc8006300L ipc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                        : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_noise_th                        : 8 ;  // 15: 8
	UINT32 cr_gds_resolution                  : 3 ;  // 18:16
} PE_H15A0_NDS_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc8006304L ipc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdiff_en                       : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_vdiff_th                       : 8 ;  // 15: 8
	UINT32 reg_hmc_err_max                    : 8 ;  // 23:16
	UINT32 reg_hmc_err_min                    : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc8006308L ipc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                    : 1 ;  //     0
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_fmd_ath                         : 8 ;  // 15: 8
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cr_fmd_ratio_th                    : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc800630cL ipc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0              : 8 ;  //  7: 0
	UINT32 chroma_st_flt_ctrl_x1              : 8 ;  // 15: 8
	UINT32 luma_st_flt_ctrl_x0                : 8 ;  // 23:16
	UINT32 luma_st_flt_ctrl_x1                : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc8006310L ipc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hmc_bonus_es_en                : 1 ;  //     0
	UINT32 reg_hmc_bonus_et_en                : 1 ;  //     1
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_hmc_bonus_gain                 : 8 ;  // 15: 8
	UINT32 reg_hmc_bonus_th                   : 8 ;  // 23:16
} PE_H15A0_NDS_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc8006314L ipc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 hmc_flt_ctrl_x0                    : 8 ;  // 23:16
	UINT32 hmc_st_flt_ctrl_x1                 : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc8006318L ipc_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vmc_motion_th                  : 8 ;  //  7: 0
	UINT32 reg_vmc_debug                      : 1 ;  //     8
	UINT32 reg_vmc_en                         : 1 ;  //     9
	UINT32 reg_vmc_still_check                : 1 ;  //    10
	UINT32 reg_vmc_mul                        : 3 ;  // 13:11
	UINT32 reg_vmc_out_mul                    : 2 ;  // 15:14
	UINT32 reg_vmc_protect_th                 : 8 ;  // 23:16
	UINT32 reg_protection_en                  : 1 ;  //    24
} PE_H15A0_NDS_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc800631cL ipc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8
	UINT32 reg_pndiff_disable                 : 1 ;  //    16
} PE_H15A0_NDS_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc8006320L ipc_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tava_errt_y2                       : 8 ;  //  7: 0
	UINT32 tava_errt_x2                       : 8 ;  // 15: 8
	UINT32 tava_errt_y3                       : 8 ;  // 23:16
	UINT32 tava_errt_x3                       : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc8006324L ipc_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tava_errt_y0                       : 8 ;  //  7: 0
	UINT32 tava_errt_x0                       : 8 ;  // 15: 8
	UINT32 tava_errt_y1                       : 8 ;  // 23:16
	UINT32 tava_errt_x1                       : 8 ;  // 31:24
} PE_H15A0_NDS_IPC_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc8006328L ipc_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count_d                     : 16;  // 15: 0
	UINT32 r_nc_v_count_d                     : 16;  // 31:16
} PE_H15A0_NDS_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc800632cL ipc_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same_d                     : 16;  // 15: 0
	UINT32 r_pn_v_count_d                     : 16;  // 31:16
} PE_H15A0_NDS_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc8006330L ipc_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0
	UINT32 r_fmd_32_min_th_flag_d             : 1 ;  //     6
	UINT32 r_fmd_32_max_th_flag_d             : 1 ;  //     7
	UINT32 r_fmd_still_limit_flag_d           : 1 ;  //     8
	UINT32 r_fmd_still_pn_th2_flag_d          : 1 ;  //     9
	UINT32 r_fmd_still_pn_th_flag_d           : 1 ;  //    10
	UINT32 r_fmd_still_prev_ratio_flag_d      : 1 ;  //    11
	UINT32 r_fmd_bigger_flag_d                : 1 ;  //    12
	UINT32 r_fmd_smaller_flag_d               : 1 ;  //    13
	UINT32 r_fmd_3_2_fmd_comb_mode_d          : 2 ;  // 15:14
	UINT32 r_pn_diff_count_d                  : 16;  // 31:16
} PE_H15A0_NDS_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc8006334L ipc_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt                 : 16;  // 15: 0
	UINT32 r_inv_original_cnt                 : 16;  // 31:16
} PE_H15A0_NDS_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc8006338L ipc_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_NDS_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc800633cL ipc_status_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_NDS_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
		0xc8006344L tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                            : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_wina_hen                       : 1 ;  //    15
	UINT32 wina_y0                            : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_wina_ven                       : 1 ;  //    31
} PE_H15A0_NDS_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8006348L tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                            : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wina_y1                            : 12;  // 27:16
} PE_H15A0_NDS_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc8006384L apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_apl                              : 10;  //  9: 0
	UINT32 cb_apl                             : 10;  // 19:10
	UINT32 cr_apl                             : 10;  // 29:20
} PE_H15A0_NDS_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8006398L lvcrtl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 y_offset                           : 8 ;  // 11: 4
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 center_position                    : 8 ;  // 23:16
	UINT32 y_gain                             : 8 ;  // 31:24
} PE_H15A0_NDS_LVCRTL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80063a0L tnr_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_NDS_TNR_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc80063a4L tnr_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_NDS_TNR_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc80063a8L tnr_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_NDS_TNR_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc80063acL tnr_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_NDS_TNR_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc8007100L pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 load_enable                        : 1 ;  //     1
	UINT32 load_type                          : 1 ;  //     2
} PE_H15A0_DNR1_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc8007104L pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 timer_rst_sel                      : 1 ;  //     8
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16
} PE_H15A0_DNR1_PE0_INTR_T;

/*-----------------------------------------------------------------------------
		0xc8007108L pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 dnr_frame_id                       : 8 ;  // 23:16
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24
} PE_H15A0_DNR1_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc800710cL pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 timer_cnt                          : 16;  // 31:16
} PE_H15A0_DNR1_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc8007110L pe0_opmode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 12;  // 11: 0     reserved
	UINT32 dnr_out_420_vsample_mode           : 1 ;  //    12
	UINT32 dnr_out_cs_type                    : 3 ;  // 15:13
	UINT32 detour_enable                      : 1 ;  //    16
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 dnr_out_422_hsample_mode           : 1 ;  //    20
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 dnr_in_lsb_mode                    : 2 ;  // 25:24
	UINT32 dnr_out_lsb_mode                   : 2 ;  // 27:26
} PE_H15A0_DNR1_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc8007114L pe0_in_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0
	UINT32                                    : 1 ;  //    12     reserved
	UINT32 dnr_in_cs_type                     : 3 ;  // 15:13
	UINT32 vsize                              : 12;  // 27:16
} PE_H15A0_DNR1_PE0_IN_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8007118L pe0_in_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset                            : 12;  // 11: 0
	UINT32                                    : 18;  // 29:12     reserved
	UINT32 hsize_sel                          : 1 ;  //    30
	UINT32 crop_en                            : 1 ;  //    31
} PE_H15A0_DNR1_PE0_IN_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc800711cL pe0_out_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vsize                              : 12;  // 27:16
} PE_H15A0_DNR1_PE0_OUT_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8007120L win_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16
} PE_H15A0_DNR1_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007124L win_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_DNR1_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007128L win_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16
	UINT32 ac_bnr_feature_cal_mode            : 2 ;  // 29:28
} PE_H15A0_DNR1_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800712cL win_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_DNR1_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc8007130L cup_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bypass                             : 1 ;  //     0
	UINT32 v_upsample_en                      : 1 ;  //     1
	UINT32 h_upsample_en                      : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 v_cubic_en                         : 1 ;  //     4
	UINT32 h_cubic_en                         : 1 ;  //     5
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sd_mode_en                         : 1 ;  //     8
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 odd_val                            : 1 ;  //    12
} PE_H15A0_DNR1_CUP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8007134L cup_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cubic_coef0                        : 8 ;  //  7: 0
	UINT32 cubic_coef1                        : 8 ;  // 15: 8
} PE_H15A0_DNR1_CUP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8007138L cup_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_DNR1_CUP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc8007150L fd3d_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aw_offset_x                        : 8 ;  //  7: 0
	UINT32 aw_offset_y                        : 8 ;  // 15: 8
	UINT32 cb_th1                             : 8 ;  // 23:16
	UINT32 cb_th2                             : 8 ;  // 31:24
} PE_H15A0_DNR1_3DFD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8007154L fd3d_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_th1                             : 8 ;  //  7: 0
	UINT32 cc_th2                             : 8 ;  // 15: 8
	UINT32 ll_th1                             : 8 ;  // 23:16
	UINT32 ll_th2                             : 8 ;  // 31:24
} PE_H15A0_DNR1_3DFD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8007158L fd3d_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pt_th                              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 sstb_th                            : 8 ;  // 23:16
	UINT32 debug_mode                         : 3 ;  // 26:24
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 div_mode                           : 1 ;  //    28
} PE_H15A0_DNR1_3DFD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800715cL fd3d_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 small_delta_th                     : 19;  // 18: 0
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 small_delta_en                     : 1 ;  //    24
} PE_H15A0_DNR1_3DFD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8007160L fd3d_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_count                           : 16;  // 15: 0
	UINT32 cc_count                           : 16;  // 31:16
} PE_H15A0_DNR1_3DFD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8007164L fd3d_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ll_count                           : 16;  // 15: 0
	UINT32 trid_format                        : 4 ;  // 19:16
} PE_H15A0_DNR1_3DFD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc8007168L fd3d_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_h                         : 23;  // 22: 0
} PE_H15A0_DNR1_3DFD_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc800716cL fd3d_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_v                         : 23;  // 22: 0
} PE_H15A0_DNR1_3DFD_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc8007170L fd3d_stat_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_v                         : 23;  // 22: 0
} PE_H15A0_DNR1_3DFD_STAT_04_T;

/*-----------------------------------------------------------------------------
		0xc8007174L fd3d_stat_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_h                         : 23;  // 22: 0
} PE_H15A0_DNR1_3DFD_STAT_05_T;

/*-----------------------------------------------------------------------------
		0xc8007178L fd3d_stat_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_ratio                           : 23;  // 22: 0
} PE_H15A0_DNR1_3DFD_STAT_06_T;

/*-----------------------------------------------------------------------------
		0xc800717cL fd3d_stat_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_ratio                           : 23;  // 22: 0
} PE_H15A0_DNR1_3DFD_STAT_07_T;

/*-----------------------------------------------------------------------------
		0xc8007180L vfilter_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_enable                  : 1 ;  //     0
	UINT32 cr_vfilter_local_en                : 1 ;  //     1
	UINT32 cr_vfilter_strength                : 1 ;  //     2
	UINT32 cr_vfilter_max_clipping            : 1 ;  //     3
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 cr_vfilter_ath                     : 8 ;  // 15: 8
	UINT32 cr_vfilter_in_field_var            : 8 ;  // 23:16
	UINT32 cr_vfilter_use_mc_mask             : 1 ;  //    24
	UINT32 cr_vfilter_expand                  : 1 ;  //    25
	UINT32 cr_vfilter_center                  : 1 ;  //    26
	UINT32 cr_vfilter_u14                     : 1 ;  //    27
	UINT32 cr_dbg_show_level                  : 2 ;  // 29:28
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 cr_dbg_show_en                     : 1 ;  //    31
} PE_H15A0_DNR1_VFILTER_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007184L vfilter_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en               : 1 ;  //     0
	UINT32 cr_vfilter_force_en                : 1 ;  //     1
	UINT32 cr_global_frame_num_th             : 4 ;  //  5: 2
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cr_vfilter_global_th               : 12;  // 19: 8
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 cr_vfilter_max_th                  : 8 ;  // 31:24
} PE_H15A0_DNR1_VFILTER_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007188L vfilter_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_protect_enable              : 1 ;  //     0
	UINT32 cr_hmc_only_hmc_region             : 1 ;  //     1
	UINT32 reg_v_filter_master_gain           : 6 ;  //  7: 2
	UINT32 cr_hmc_protect_motion_th           : 8 ;  // 15: 8
	UINT32 cr_hmc_protect_th_max              : 8 ;  // 23:16
	UINT32 cr_hmc_protect_th_min              : 8 ;  // 31:24
} PE_H15A0_DNR1_VFILTER_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800718cL cti_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0
	UINT32 cti_clipping_mode                  : 1 ;  //     1
	UINT32 cti_pre_flt_mode                   : 2 ;  //  3: 2
	UINT32 cti_tap                            : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8
	UINT32 cti_debug_mode                     : 3 ;  // 18:16
} PE_H15A0_DNR1_CTI_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007190L cti_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_coring_th0                     : 8 ;  //  7: 0
	UINT32 cti_coring_th1                     : 8 ;  // 15: 8
	UINT32 cti_coring_smooth                  : 3 ;  // 18:16
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_cti_tap2                       : 2 ;  // 21:20
} PE_H15A0_DNR1_CTI_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007194L dnr_max_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dnr_max_enable                 : 1 ;  //     0
	UINT32 reg_ifc_max_enable                 : 1 ;  //     1
	UINT32 reg_dnr_ifc_sel                    : 1 ;  //     2
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 reg_max_dbg_en                     : 1 ;  //     8
	UINT32 reg_out_motion_ctrl                : 1 ;  //     9
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_win_en                         : 1 ;  //    16
	UINT32 reg_bdr_en                         : 1 ;  //    17
	UINT32 reg_win_inout                      : 1 ;  //    18
	UINT32 mnr_motion_ctrl                    : 1 ;  //    19
	UINT32 bnr_motion_ctrl                    : 1 ;  //    20
	UINT32 reg_motion_mux_set                 : 1 ;  //    21
	UINT32 reg_motion_bit_ctrl                : 3 ;  // 24:22
	UINT32 reg_delta_ifc_th                   : 7 ;  // 31:25
} PE_H15A0_DNR1_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007198L dnr_dbar_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_en                         : 1 ;  //     0
	UINT32 reg_show_f3d                       : 1 ;  //     1
	UINT32 reg_show_bnr                       : 1 ;  //     2
	UINT32 reg_f3d_mode                       : 1 ;  //     3
	UINT32 reg_acness_or_acness_gain          : 1 ;  //     4
} PE_H15A0_DNR1_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800719cL reg_c420_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tearing_count_th               : 8 ;  //  7: 0
	UINT32 reg_frame_count_th                 : 4 ;  // 11: 8
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_detect_level_th                : 10;  // 25:16
} PE_H15A0_DNR1_REG_C420_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80071a0L mnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_enable                     : 1 ;  //     0
	UINT32 reg_mnr_debug                      : 2 ;  //  2: 1
	UINT32 reg_mnr_motion_view                : 1 ;  //     3
	UINT32 reg_h_expand                       : 2 ;  //  5: 4
	UINT32 reg_gain_flt_size                  : 1 ;  //     6
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_mnr_master_gain                : 8 ;  // 15: 8
	UINT32 reg_chroma_master_gain             : 8 ;  // 23:16
} PE_H15A0_DNR1_MNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc80071a4L mnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_s1_mmd_min                 : 8 ;  //  7: 0
	UINT32 reg_mnr_s2_ratio_min               : 8 ;  // 15: 8
	UINT32 reg_mnr_s2_ratio_max               : 8 ;  // 23:16
	UINT32 reg_mnr_s2_mmd_min                 : 8 ;  // 31:24
} PE_H15A0_DNR1_MNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc80071a8L mnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_motion_y_max               : 8 ;  //  7: 0
	UINT32 reg_mnr_motion_y_min               : 8 ;  // 15: 8
	UINT32 reg_mnr_v_gain                     : 8 ;  // 23:16
	UINT32 reg_mnr_h_gain                     : 8 ;  // 31:24
} PE_H15A0_DNR1_MNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc80071acL mnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_filter_x0                      : 8 ;  //  7: 0
	UINT32 reg_filter_x1                      : 8 ;  // 15: 8
	UINT32 reg_filter_y0                      : 8 ;  // 23:16
	UINT32 reg_filter_y1                      : 8 ;  // 31:24
} PE_H15A0_DNR1_MNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc80071b0L mnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_en                  : 1 ;  //     0
	UINT32 reg_motion_mnr_filter              : 2 ;  //  2: 1
	UINT32 reg_motion_mnr_filter_c            : 2 ;  //  4: 3
	UINT32 reg_mnr_filter                     : 2 ;  //  6: 5
	UINT32 reg_mnr_filter_c                   : 2 ;  //  8: 7
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_mnr_motion_min                 : 8 ;  // 23:16
	UINT32 reg_mnr_motion_max                 : 8 ;  // 31:24
} PE_H15A0_DNR1_MNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc80071b4L mnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_x0                  : 8 ;  //  7: 0
	UINT32 reg_motion_mnr_x1                  : 8 ;  // 15: 8
	UINT32 reg_motion_mnr_y0                  : 8 ;  // 23:16
	UINT32 reg_motion_mnr_y1                  : 8 ;  // 31:24
} PE_H15A0_DNR1_MNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc80071b8L detail_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_detail_max              : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_detail_min              : 8 ;  // 15: 8
	UINT32 reg_bnr_diff_l                     : 8 ;  // 23:16
	UINT32 reg_bnr_diff_p                     : 8 ;  // 31:24
} PE_H15A0_DNR1_DETAIL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80071bcL dc_bnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_enable                  : 1 ;  //     0
	UINT32 reg_dc_blur_debug_mode             : 1 ;  //     1
	UINT32 reg_dc_blur_sel                    : 2 ;  //  3: 2
	UINT32 reg_dc_output_debug_mode           : 4 ;  //  7: 4
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_dc_motion_max                  : 8 ;  // 23:16
	UINT32 reg_dc_motion_min                  : 8 ;  // 31:24
} PE_H15A0_DNR1_DC_BNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc80071c0L dc_bnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_detail_max                  : 8 ;  //  7: 0
	UINT32 reg_dc_detail_min                  : 8 ;  // 15: 8
	UINT32 reg_var_chroma_en                  : 1 ;  //    16
	UINT32                                    : 2 ;  // 18:17     reserved
	UINT32 reg_var_range_sel                  : 1 ;  //    19
	UINT32 reg_var_v_gain                     : 4 ;  // 23:20
	UINT32 reg_var_h_gain                     : 4 ;  // 27:24
	UINT32 reg_var_cut_resolution             : 4 ;  // 31:28
} PE_H15A0_DNR1_DC_BNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc80071c4L dc_bnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_var_en                      : 1 ;  //     0
	UINT32 reg_dc_motion_en                   : 1 ;  //     1
	UINT32 reg_dc_protection_en               : 1 ;  //     2
	UINT32 reg_dc_detail_en                   : 1 ;  //     3
	UINT32 reg_dc_global_motion_th            : 4 ;  //  7: 4
	UINT32 reg_detail_sel                     : 1 ;  //     8
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 reg_dc_bnr_mastergain              : 6 ;  // 15:10
	UINT32 reg_dc_bnr_chromagain              : 8 ;  // 23:16
	UINT32 reg_dc_protection_th               : 8 ;  // 31:24
} PE_H15A0_DNR1_DC_BNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc80071c8L dc_bnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_var_th3                 : 8 ;  //  7: 0
	UINT32 reg_dc_bnr_var_th2                 : 8 ;  // 15: 8
	UINT32 reg_dc_bnr_var_th1                 : 8 ;  // 23:16
	UINT32 reg_dc_bnr_var_th0                 : 8 ;  // 31:24
} PE_H15A0_DNR1_DC_BNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc80071ccL dc_bnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_motion_y_min                : 8 ;  //  7: 0
	UINT32 reg_dc_motion_y_max                : 8 ;  // 15: 8
	UINT32 reg_dc_detail_y_min                : 8 ;  // 23:16
	UINT32 reg_dc_detail_y_max                : 8 ;  // 31:24
} PE_H15A0_DNR1_DC_BNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc80071d0L dc_bnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_var_y_th0                   : 8 ;  //  7: 0
	UINT32 reg_dc_var_y_th1                   : 8 ;  // 15: 8
	UINT32 reg_dc_var_y_th2                   : 8 ;  // 23:16
	UINT32 reg_dc_var_y_th3                   : 8 ;  // 31:24
} PE_H15A0_DNR1_DC_BNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc80071d4L ac_bnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_h_en                    : 1 ;  //     0
	UINT32 reg_bnr_ac_v_en                    : 1 ;  //     1
	UINT32 reg_bnr_ac_h_chroma_en             : 1 ;  //     2
	UINT32 reg_bnr_ac_v_chroma_en             : 1 ;  //     3
	UINT32 reg_bnr_ac_acness_resol_h          : 2 ;  //  5: 4
	UINT32 reg_bnr_ac_sdhd_sel                : 1 ;  //     6
	UINT32 reg_ac_bnr_gain_sel                : 1 ;  //     7
	UINT32 reg_bnr_ac_diff_min_v_th           : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_diff_min_h_th           : 8 ;  // 23:16
	UINT32 reg_bnr_ac_iir_gain                : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc80071d8L ac_bnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_global_motion_th        : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_h_acness_max            : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_h_acness_min            : 8 ;  // 23:16
	UINT32 reg_bnr_ac_bin_th                  : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc80071dcL ac_bnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_motion_3                : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_motion_2                : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_motion_1                : 8 ;  // 23:16
	UINT32 reg_bnr_ac_motion_0                : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc80071e0L dnr_stat_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c420_tearing_cnt                   : 16;  // 15: 0
	UINT32 global_motion_iir                  : 8 ;  // 23:16
	UINT32 global_dc_gain                     : 8 ;  // 31:24
} PE_H15A0_DNR1_DNR_STAT_0_T;

/*-----------------------------------------------------------------------------
		0xc80071e4L ac_bnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_motion_y_3              : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_motion_y_2              : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_motion_y_1              : 8 ;  // 23:16
	UINT32 reg_bnr_ac_motion_y_0              : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc80071e8L ac_bnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_v_init_offset           : 3 ;  //  2: 0
	UINT32 reg_bnr_ac_h_init_offset           : 3 ;  //  5: 3
	UINT32 reg_bnr_ac_acness_resol_v          : 2 ;  //  7: 6
	UINT32 reg_bnr_ac_debug_mode              : 4 ;  // 11: 8
	UINT32 reg_bnr_ac_hsy_mode                : 4 ;  // 15:12
	UINT32 reg_bnr_ac_v_acness_max            : 8 ;  // 23:16
	UINT32 reg_bnr_ac_v_acness_min            : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc80071ecL dnr_stat_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 acness_or_gain_v_l                 : 8 ;  //  7: 0
	UINT32 acness_or_gain_h_l                 : 8 ;  // 15: 8
	UINT32 acness_or_gain_v_g                 : 8 ;  // 23:16
	UINT32 acness_or_gain_h_g                 : 8 ;  // 31:24
} PE_H15A0_DNR1_DNR_STAT_1_T;

/*-----------------------------------------------------------------------------
		0xc80071f0L ac_bnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_detail_th4              : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_detail_th3              : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_detail_th2              : 8 ;  // 23:16
	UINT32 reg_bnr_ac_detail_th1              : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc80071f4L ac_bnr_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_pos_gain_h2             : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_pos_gain_h1             : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_pos_gain_h0             : 8 ;  // 23:16
	UINT32 reg_bnr_ac_detail_gain_th          : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc80071f8L ac_bnr_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_pos_gain_l2             : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_pos_gain_l1             : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_pos_gain_l0             : 8 ;  // 23:16
	UINT32 reg_bnr_ac_pos_gain_h3             : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc80071fcL dnr_stat_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_init                             : 8 ;  //  7: 0
	UINT32 v_init                             : 8 ;  // 15: 8
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 reg_vfilter_tearing_cnt            : 12;  // 31:20
} PE_H15A0_DNR1_DNR_STAT_2_T;

/*-----------------------------------------------------------------------------
		0xc8007200L ac_bnr_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ac_master_c_gain               : 8 ;  //  7: 0
	UINT32 reg_ac_bnr_enable                  : 1 ;  //     8
	UINT32 reg_ac_bnr_motion_en               : 1 ;  //     9
	UINT32 reg_ac_bnr_acness_en               : 1 ;  //    10
	UINT32 reg_ac_bnr_position_en             : 1 ;  //    11
	UINT32 reg_detail_sel                     : 1 ;  //    12
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_bnr_ac_manual_init_v           : 3 ;  // 18:16
	UINT32 reg_ac_manual_init_en_v            : 1 ;  //    19
	UINT32 reg_bnr_ac_manual_init_h           : 3 ;  // 22:20
	UINT32 reg_ac_manual_init_en_h            : 1 ;  //    23
	UINT32 reg_bnr_ac_pos_gain_l3             : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc8007204L ac_bnr_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_strong_filter_sel              : 1 ;  //     0
	UINT32 reg_weak_filter_sel                : 1 ;  //     1
	UINT32                                    : 1 ;  //     2     reserved
	UINT32 reg_acness_sel                     : 2 ;  //  4: 3
	UINT32                                    : 2 ;  //  6: 5     reserved
	UINT32 reg_acness_or_gain_sel             : 1 ;  //     7
	UINT32 reg_final_ac_bnr_gain_debug_h_en   : 1 ;  //     8
	UINT32 reg_final_ac_bnr_gain_debug_v_en   : 1 ;  //     9
	UINT32 reg_ac_master_gain                 : 6 ;  // 15:10
	UINT32 reg_g_motion_scale                 : 3 ;  // 18:16
	UINT32 reg_g_motion_div                   : 2 ;  // 20:19
} PE_H15A0_DNR1_AC_BNR_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc8007208L ac_bnr_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ac_detail_gain_y_min           : 8 ;  //  7: 0
	UINT32 reg_ac_detail_gain_y_max           : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_acness_y_min            : 8 ;  // 23:16
	UINT32 reg_bnr_ac_acness_y_max            : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800720cL ac_bnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ac_detail_alpha2_y_min         : 8 ;  //  7: 0
	UINT32 reg_ac_detail_alpha2_y_max         : 8 ;  // 15: 8
	UINT32 reg_ac_detail_alpha4_y_min         : 8 ;  // 23:16
	UINT32 reg_ac_detail_alpha4_y_max         : 8 ;  // 31:24
} PE_H15A0_DNR1_AC_BNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8007210L ac_bnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g_motion_manual                : 8 ;  //  7: 0
	UINT32 reg_acness_scale_mul               : 8 ;  // 15: 8
	UINT32                                    : 15;  // 30:16     reserved
	UINT32 reg_g_motion_manual_en             : 1 ;  //    31
} PE_H15A0_DNR1_AC_BNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8007214L ifc_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_y_drvt_hcoef_sel               : 3 ;  //  2: 0
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_y_ddrvt_hcoef_sel              : 2 ;  //  5: 4
	UINT32 reg_y_edge_gain_res                : 1 ;  //     6
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_y_drvt2_hcoef_sel              : 3 ;  // 10: 8
	UINT32 reg_medge_meth                     : 1 ;  //    11
	UINT32 reg_medge_th                       : 8 ;  // 19:12
	UINT32 reg_medge_coring_th                : 8 ;  // 27:20
	UINT32 reg_medge_ch_tap                   : 2 ;  // 29:28
	UINT32 reg_edge_meth                      : 2 ;  // 31:30
} PE_H15A0_DNR1_IFC_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007218L ifc_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_edge_ddrvt_gain_y1             : 8 ;  //  7: 0
	UINT32 reg_edge_ddrvt_gain_x1             : 8 ;  // 15: 8
	UINT32 reg_edge_ddrvt_gain_y0             : 8 ;  // 23:16
	UINT32 reg_edge_ddrvt_gain_x0             : 8 ;  // 31:24
} PE_H15A0_DNR1_IFC_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800721cL ifc_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_c_drvt_hcoef_sel               : 3 ;  //  2: 0
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_c_ddrvt_hcoef_sel              : 2 ;  //  5: 4
	UINT32 reg_c_edge_gain_res                : 1 ;  //     6
} PE_H15A0_DNR1_IFC_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc8007220L ifc_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_c_edge_gain_y1                 : 8 ;  //  7: 0
	UINT32 reg_c_edge_gain_x1                 : 8 ;  // 15: 8
	UINT32 reg_c_edge_gain_y0                 : 8 ;  // 23:16
	UINT32 reg_c_edge_gain_x0                 : 8 ;  // 31:24
} PE_H15A0_DNR1_IFC_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc8007224L ifc_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_crs_para_tap                   : 2 ;  //  1: 0
	UINT32 reg_cdst_th                        : 10;  // 11: 2
	UINT32 reg_crs_op_tap                     : 2 ;  // 13:12
	UINT32 reg_crs_gain_res                   : 1 ;  //    14
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_cdiff_hcoef_sel                : 2 ;  // 17:16
	UINT32                                    : 10;  // 27:18     reserved
	UINT32 reg_crs_point                      : 2 ;  // 29:28
	UINT32 reg_crs_meth                       : 2 ;  // 31:30
} PE_H15A0_DNR1_IFC_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc8007228L ifc_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_crs_gain_y1                    : 8 ;  //  7: 0
	UINT32 reg_crs_gain_x1                    : 8 ;  // 15: 8
	UINT32 reg_crs_gain_y0                    : 8 ;  // 23:16
	UINT32 reg_crs_gain_x0                    : 8 ;  // 31:24
} PE_H15A0_DNR1_IFC_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800722cL ifc_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_IFC_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc8007230L ifc_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_IFC_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc8007234L ifc_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ifc_en                         : 1 ;  //     0
	UINT32 reg_clp_on                         : 1 ;  //     1
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_edge_en                        : 1 ;  //     4
	UINT32                                    : 15;  // 19: 5     reserved
	UINT32 reg_clp_tap                        : 2 ;  // 21:20
	UINT32 reg_ifc_flt_sel                    : 2 ;  // 23:22
	UINT32 reg_debug_en                       : 2 ;  // 25:24
	UINT32 reg_debug_mode                     : 4 ;  // 29:26
	UINT32 reg_bld_tap                        : 2 ;  // 31:30
} PE_H15A0_DNR1_IFC_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc8007238L ifc_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_IFC_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800723cL ifc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_IFC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc8007240L ifc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_IFC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8007244L ifc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 reg_crs_zero_th                    : 8 ;  // 23:16
	UINT32 reg_dst_cdst_th                    : 8 ;  // 31:24
} PE_H15A0_DNR1_IFC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8007248L ifc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_IFC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc800724cL ifc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_IFC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc8007250L ifc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_medge_gain_y1                  : 8 ;  //  7: 0
	UINT32 reg_medge_gain_x1                  : 8 ;  // 15: 8
	UINT32 reg_medge_gain_y0                  : 8 ;  // 23:16
	UINT32 reg_medge_gain_x0                  : 8 ;  // 31:24
} PE_H15A0_DNR1_IFC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc8007254L ifc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_crs_op_gain_isel               : 2 ;  //  1: 0
	UINT32 reg_crs_op_gain_res                : 1 ;  //     2
	UINT32 reg_crs_cdst_flt                   : 3 ;  //  5: 3
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 reg_crs_cdst_vmdn                  : 1 ;  //     7
	UINT32 reg_crs_cdst_op_flt                : 3 ;  // 10: 8
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 reg_op_wgain_coef1                 : 2 ;  // 17:16
	UINT32 reg_op_wgain_coef2                 : 2 ;  // 19:18
	UINT32 reg_op_wgain_coef3                 : 2 ;  // 21:20
} PE_H15A0_DNR1_IFC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc8007258L ifc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_crs_op_gain_y1                 : 8 ;  //  7: 0
	UINT32 reg_crs_op_gain_x1                 : 8 ;  // 15: 8
	UINT32 reg_crs_op_gain_y0                 : 8 ;  // 23:16
	UINT32 reg_crs_op_gain_x0                 : 8 ;  // 31:24
} PE_H15A0_DNR1_IFC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc800725cL ifc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_IFC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc8007260L ifc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 reg_medge_op_th                    : 8 ;  // 23:16
	UINT32 reg_mmd_gain_en                    : 1 ;  //    24
	UINT32 reg_mmd_gain_flt                   : 2 ;  // 26:25
	UINT32 reg_mmd_gain_tap                   : 2 ;  // 28:27
	UINT32 reg_mmd_gain_res                   : 1 ;  //    29
} PE_H15A0_DNR1_IFC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc8007264L ifc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_IFC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc8007268L ifc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mmd_gain_y1                    : 8 ;  //  7: 0
	UINT32 reg_mmd_gain_x1                    : 8 ;  // 15: 8
	UINT32 reg_mmd_gain_y0                    : 8 ;  // 23:16
	UINT32 reg_mmd_gain_x0                    : 8 ;  // 31:24
} PE_H15A0_DNR1_IFC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc800726cL sc_bnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_bnr_en                      : 1 ;  //     0
	UINT32 reg_sc_bnr_manual_en               : 1 ;  //     1
	UINT32 reg_scene_ch_flag                  : 1 ;  //     2
	UINT32 reg_avg_scale_div_v                : 3 ;  //  5: 3
	UINT32 reg_avg_scale_mul_v                : 3 ;  //  8: 6
	UINT32                                    : 2 ;  // 10: 9     reserved
	UINT32 reg_scale_mul_v                    : 3 ;  // 13:11
	UINT32 reg_scale_div_v                    : 3 ;  // 16:14
	UINT32                                    : 1 ;  //    17     reserved
	UINT32 reg_avg_scale_mul_h                : 3 ;  // 20:18
	UINT32 reg_avg_scale_div_h                : 3 ;  // 23:21
	UINT32 reg_max_index_diff_th              : 3 ;  // 26:24
} PE_H15A0_DNR1_SC_BNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007270L sc_bnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_post2_iir_gain                 : 8 ;  //  7: 0
	UINT32 reg_frm_cnt_th                     : 8 ;  // 15: 8
	UINT32 reg_bin_ratio_th                   : 8 ;  // 23:16
	UINT32 reg_detect_th                      : 8 ;  // 31:24
} PE_H15A0_DNR1_SC_BNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007274L sc_bnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bin_ratio_div                  : 8 ;  //  7: 0
	UINT32 reg_bin_ratio_mul                  : 8 ;  // 15: 8
	UINT32 reg_min_bin_th                     : 8 ;  // 23:16
	UINT32 reg_detector_iir_gain              : 8 ;  // 31:24
} PE_H15A0_DNR1_SC_BNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc8007278L sc_bnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_grid_th1                       : 8 ;  //  7: 0
	UINT32 reg_grid_th2                       : 8 ;  // 15: 8
	UINT32 reg_grid_th3                       : 8 ;  // 23:16
	UINT32 reg_sc_bnr_x_min                   : 8 ;  // 31:24
} PE_H15A0_DNR1_SC_BNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800727cL sc_bnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pos_gain_th                    : 8 ;  //  7: 0
	UINT32 reg_sc_bnr_y_max                   : 8 ;  // 15: 8
	UINT32 reg_sc_bnr_y_min                   : 8 ;  // 23:16
	UINT32 reg_sc_bnr_x_max                   : 8 ;  // 31:24
} PE_H15A0_DNR1_SC_BNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc8007280L sc_bnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pos_gain_3                     : 8 ;  //  7: 0
	UINT32 reg_pos_gain_2                     : 8 ;  // 15: 8
	UINT32 reg_pos_gain_1                     : 8 ;  // 23:16
	UINT32 reg_scale_div_h                    : 3 ;  // 26:24
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_scale_mul_h                    : 3 ;  // 30:28
	UINT32 reg_iir_en                         : 1 ;  //    31
} PE_H15A0_DNR1_SC_BNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc8007284L sc_bnr_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_bnr_debug_en                : 1 ;  //     0
	UINT32 reg_sc_bnr_debug_mode              : 2 ;  //  2: 1
	UINT32                                    : 21;  // 23: 3     reserved
	UINT32 reg_acness_th                      : 8 ;  // 31:24
} PE_H15A0_DNR1_SC_BNR_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc8007288L c_pre_blur_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre_121_blur_en                    : 1 ;  //     0
	UINT32 pre_median_en                      : 1 ;  //     1
} PE_H15A0_DNR1_C_PRE_BLUR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800728cL texture_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_texture_cnt_div                : 3 ;  //  2: 0
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 reg_texture_cnt_mul                : 8 ;  // 15: 8
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 reg_cross_th                       : 8 ;  // 31:24
} PE_H15A0_DNR1_TEXTURE_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007290L texture_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_TEXTURE_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007294L texture_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_TEXTURE_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc8007298L texture_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_TEXTURE_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc80072d0L dnr_stat_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_acness_or_gain_h_t3              : 8 ;  //  7: 0
	UINT32 w_acness_or_gain_h_t2              : 8 ;  // 15: 8
	UINT32 w_acness_or_gain_h_t1              : 8 ;  // 23:16
	UINT32 w_acness_or_gain_h_t0              : 8 ;  // 31:24
} PE_H15A0_DNR1_DNR_STAT_3_T;

/*-----------------------------------------------------------------------------
		0xc80072d4L dnr_stat_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_acness_or_gain_h_b3              : 8 ;  //  7: 0
	UINT32 w_acness_or_gain_h_b2              : 8 ;  // 15: 8
	UINT32 w_acness_or_gain_h_b1              : 8 ;  // 23:16
	UINT32 w_acness_or_gain_h_b0              : 8 ;  // 31:24
} PE_H15A0_DNR1_DNR_STAT_4_T;

/*-----------------------------------------------------------------------------
		0xc80072d8L dnr_stat_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_acness_or_gain_v_t3              : 8 ;  //  7: 0
	UINT32 w_acness_or_gain_v_t2              : 8 ;  // 15: 8
	UINT32 w_acness_or_gain_v_t1              : 8 ;  // 23:16
	UINT32 w_acness_or_gain_v_t0              : 8 ;  // 31:24
} PE_H15A0_DNR1_DNR_STAT_5_T;

/*-----------------------------------------------------------------------------
		0xc80072dcL dnr_stat_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_acness_or_gain_v_b3              : 8 ;  //  7: 0
	UINT32 w_acness_or_gain_v_b2              : 8 ;  // 15: 8
	UINT32 w_acness_or_gain_v_b1              : 8 ;  // 23:16
	UINT32 w_acness_or_gain_v_b0              : 8 ;  // 31:24
} PE_H15A0_DNR1_DNR_STAT_6_T;

/*-----------------------------------------------------------------------------
		0xc80072e0L dnr_stat_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 acness_gain_v                      : 8 ;  //  7: 0
	UINT32 acness_gain_h                      : 8 ;  // 15: 8
	UINT32 acness_v_iir                       : 8 ;  // 23:16
	UINT32 acness_h_iir                       : 8 ;  // 31:24
} PE_H15A0_DNR1_DNR_STAT_7_T;

/*-----------------------------------------------------------------------------
		0xc80072e4L dnr_stat_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 bin_ratio_iir                      : 8 ;  // 15: 8
	UINT32 bin_ratio                          : 8 ;  // 23:16
	UINT32 hys_cnt                            : 3 ;  // 26:24
	UINT32                                    : 4 ;  // 30:27     reserved
	UINT32 sc_bnr_flag                        : 1 ;  //    31
} PE_H15A0_DNR1_DNR_STAT_8_T;

/*-----------------------------------------------------------------------------
		0xc80072e8L dnr_stat_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_index_iir                      : 7 ;  //  6: 0
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 max_index                          : 7 ;  // 14: 8
} PE_H15A0_DNR1_DNR_STAT_9_T;

/*-----------------------------------------------------------------------------
		0xc80072ecL dnr_stat_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR1_DNR_STAT_10_T;

/*-----------------------------------------------------------------------------
		0xc8007300L pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 load_enable                        : 1 ;  //     1
	UINT32 load_type                          : 1 ;  //     2
} PE_H15A0_DNR2_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc8007304L pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 timer_rst_sel                      : 1 ;  //     8
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16
} PE_H15A0_DNR2_PE0_INTR_T;

/*-----------------------------------------------------------------------------
		0xc8007308L pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 dnr_frame_id                       : 8 ;  // 23:16
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24
} PE_H15A0_DNR2_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc800730cL pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 timer_cnt                          : 16;  // 31:16
} PE_H15A0_DNR2_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc8007310L pe0_opmode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 12;  // 11: 0     reserved
	UINT32 dnr_out_420_vsample_mode           : 1 ;  //    12
	UINT32 dnr_out_cs_type                    : 3 ;  // 15:13
	UINT32 detour_enable                      : 1 ;  //    16
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 dnr_out_422_hsample_mode           : 1 ;  //    20
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 dnr_in_lsb_mode                    : 2 ;  // 25:24
	UINT32 dnr_out_lsb_mode                   : 2 ;  // 27:26
} PE_H15A0_DNR2_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc8007314L pe0_in_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0
	UINT32                                    : 1 ;  //    12     reserved
	UINT32 dnr_in_cs_type                     : 3 ;  // 15:13
	UINT32 vsize                              : 12;  // 27:16
} PE_H15A0_DNR2_PE0_IN_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8007318L pe0_in_offset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset                            : 12;  // 11: 0
	UINT32                                    : 18;  // 29:12     reserved
	UINT32 hsize_sel                          : 1 ;  //    30
	UINT32 crop_en                            : 1 ;  //    31
} PE_H15A0_DNR2_PE0_IN_OFFSET_T;

/*-----------------------------------------------------------------------------
		0xc800731cL pe0_out_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vsize                              : 12;  // 27:16
} PE_H15A0_DNR2_PE0_OUT_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc8007320L win_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16
} PE_H15A0_DNR2_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007324L win_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_DNR2_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007328L win_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16
	UINT32 ac_bnr_feature_cal_mode            : 2 ;  // 29:28
} PE_H15A0_DNR2_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800732cL win_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_DNR2_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc8007330L cup_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bypass                             : 1 ;  //     0
	UINT32 v_upsample_en                      : 1 ;  //     1
	UINT32 h_upsample_en                      : 1 ;  //     2
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 v_cubic_en                         : 1 ;  //     4
	UINT32 h_cubic_en                         : 1 ;  //     5
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sd_mode_en                         : 1 ;  //     8
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 odd_val                            : 1 ;  //    12
} PE_H15A0_DNR2_CUP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8007334L cup_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cubic_coef0                        : 8 ;  //  7: 0
	UINT32 cubic_coef1                        : 8 ;  // 15: 8
} PE_H15A0_DNR2_CUP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8007338L cup_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0
} PE_H15A0_DNR2_CUP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc8007350L fd3d_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aw_offset_x                        : 8 ;  //  7: 0
	UINT32 aw_offset_y                        : 8 ;  // 15: 8
	UINT32 cb_th1                             : 8 ;  // 23:16
	UINT32 cb_th2                             : 8 ;  // 31:24
} PE_H15A0_DNR2_3DFD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8007354L fd3d_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_th1                             : 8 ;  //  7: 0
	UINT32 cc_th2                             : 8 ;  // 15: 8
	UINT32 ll_th1                             : 8 ;  // 23:16
	UINT32 ll_th2                             : 8 ;  // 31:24
} PE_H15A0_DNR2_3DFD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8007358L fd3d_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pt_th                              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 sstb_th                            : 8 ;  // 23:16
	UINT32 debug_mode                         : 3 ;  // 26:24
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 div_mode                           : 1 ;  //    28
} PE_H15A0_DNR2_3DFD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc800735cL fd3d_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 small_delta_th                     : 19;  // 18: 0
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 small_delta_en                     : 1 ;  //    24
} PE_H15A0_DNR2_3DFD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8007360L fd3d_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_count                           : 16;  // 15: 0
	UINT32 cc_count                           : 16;  // 31:16
} PE_H15A0_DNR2_3DFD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8007364L fd3d_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ll_count                           : 16;  // 15: 0
	UINT32 trid_format                        : 4 ;  // 19:16
} PE_H15A0_DNR2_3DFD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc8007368L fd3d_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_h                         : 23;  // 22: 0
} PE_H15A0_DNR2_3DFD_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc800736cL fd3d_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_v                         : 23;  // 22: 0
} PE_H15A0_DNR2_3DFD_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc8007370L fd3d_stat_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_v                         : 23;  // 22: 0
} PE_H15A0_DNR2_3DFD_STAT_04_T;

/*-----------------------------------------------------------------------------
		0xc8007374L fd3d_stat_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_h                         : 23;  // 22: 0
} PE_H15A0_DNR2_3DFD_STAT_05_T;

/*-----------------------------------------------------------------------------
		0xc8007378L fd3d_stat_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_ratio                           : 23;  // 22: 0
} PE_H15A0_DNR2_3DFD_STAT_06_T;

/*-----------------------------------------------------------------------------
		0xc800737cL fd3d_stat_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_ratio                           : 23;  // 22: 0
} PE_H15A0_DNR2_3DFD_STAT_07_T;

/*-----------------------------------------------------------------------------
		0xc8007380L vfilter_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_enable                  : 1 ;  //     0
	UINT32 cr_vfilter_local_en                : 1 ;  //     1
	UINT32 cr_vfilter_strength                : 1 ;  //     2
	UINT32 cr_vfilter_max_clipping            : 1 ;  //     3
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 cr_vfilter_ath                     : 8 ;  // 15: 8
	UINT32 cr_vfilter_in_field_var            : 8 ;  // 23:16
	UINT32 cr_vfilter_use_mc_mask             : 1 ;  //    24
	UINT32 cr_vfilter_expand                  : 1 ;  //    25
	UINT32 cr_vfilter_center                  : 1 ;  //    26
	UINT32 cr_vfilter_u14                     : 1 ;  //    27
	UINT32 cr_dbg_show_level                  : 2 ;  // 29:28
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 cr_dbg_show_en                     : 1 ;  //    31
} PE_H15A0_DNR2_VFILTER_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007384L vfilter_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en               : 1 ;  //     0
	UINT32 cr_vfilter_force_en                : 1 ;  //     1
	UINT32 cr_global_frame_num_th             : 4 ;  //  5: 2
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cr_vfilter_global_th               : 12;  // 19: 8
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 cr_vfilter_max_th                  : 8 ;  // 31:24
} PE_H15A0_DNR2_VFILTER_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007388L vfilter_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_protect_enable              : 1 ;  //     0
	UINT32 cr_hmc_only_hmc_region             : 1 ;  //     1
	UINT32 reg_v_filter_master_gain           : 6 ;  //  7: 2
	UINT32 cr_hmc_protect_motion_th           : 8 ;  // 15: 8
	UINT32 cr_hmc_protect_th_max              : 8 ;  // 23:16
	UINT32 cr_hmc_protect_th_min              : 8 ;  // 31:24
} PE_H15A0_DNR2_VFILTER_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc800738cL cti_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0
	UINT32 cti_clipping_mode                  : 1 ;  //     1
	UINT32 cti_pre_flt_mode                   : 2 ;  //  3: 2
	UINT32 cti_tap                            : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8
	UINT32 cti_debug_mode                     : 3 ;  // 18:16
} PE_H15A0_DNR2_CTI_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007390L cti_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_coring_th0                     : 8 ;  //  7: 0
	UINT32 cti_coring_th1                     : 8 ;  // 15: 8
	UINT32 cti_coring_smooth                  : 3 ;  // 18:16
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_cti_tap2                       : 2 ;  // 21:20
} PE_H15A0_DNR2_CTI_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007394L dnr_max_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dnr_max_enable                 : 1 ;  //     0
	UINT32 reg_ifc_max_enable                 : 1 ;  //     1
	UINT32 reg_dnr_ifc_sel                    : 1 ;  //     2
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 reg_max_dbg_en                     : 1 ;  //     8
	UINT32 reg_out_motion_ctrl                : 1 ;  //     9
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_win_en                         : 1 ;  //    16
	UINT32 reg_bdr_en                         : 1 ;  //    17
	UINT32 reg_win_inout                      : 1 ;  //    18
	UINT32 mnr_motion_ctrl                    : 1 ;  //    19
	UINT32 bnr_motion_ctrl                    : 1 ;  //    20
	UINT32 reg_motion_mux_set                 : 1 ;  //    21
	UINT32 reg_motion_bit_ctrl                : 3 ;  // 24:22
	UINT32 reg_delta_ifc_th                   : 7 ;  // 31:25
} PE_H15A0_DNR2_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8007398L dnr_dbar_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_en                         : 1 ;  //     0
	UINT32 reg_show_f3d                       : 1 ;  //     1
	UINT32 reg_show_bnr                       : 1 ;  //     2
	UINT32 reg_f3d_mode                       : 1 ;  //     3
	UINT32 reg_acness_or_acness_gain          : 1 ;  //     4
} PE_H15A0_DNR2_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800739cL reg_c420_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tearing_count_th               : 8 ;  //  7: 0
	UINT32 reg_frame_count_th                 : 4 ;  // 11: 8
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_detect_level_th                : 10;  // 25:16
} PE_H15A0_DNR2_REG_C420_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80073a0L mnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_enable                     : 1 ;  //     0
	UINT32 reg_mnr_debug                      : 2 ;  //  2: 1
	UINT32 reg_mnr_motion_view                : 1 ;  //     3
	UINT32 reg_h_expand                       : 2 ;  //  5: 4
	UINT32 reg_gain_flt_size                  : 1 ;  //     6
	UINT32 reg_flt_sel_opt                    : 1 ;  //     7
	UINT32 reg_mnr_master_gain                : 8 ;  // 15: 8
	UINT32 reg_chroma_master_gain             : 8 ;  // 23:16
} PE_H15A0_DNR2_MNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc80073a4L mnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_s1_mmd_min                 : 8 ;  //  7: 0
	UINT32 reg_mnr_s2_ratio_min               : 8 ;  // 15: 8
	UINT32 reg_mnr_s2_ratio_max               : 8 ;  // 23:16
	UINT32 reg_mnr_s2_mmd_min                 : 8 ;  // 31:24
} PE_H15A0_DNR2_MNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc80073a8L mnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_motion_y_max               : 8 ;  //  7: 0
	UINT32 reg_mnr_motion_y_min               : 8 ;  // 15: 8
	UINT32 reg_mnr_v_gain                     : 8 ;  // 23:16
	UINT32 reg_mnr_h_gain                     : 8 ;  // 31:24
} PE_H15A0_DNR2_MNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc80073acL mnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_filter_x0                      : 8 ;  //  7: 0
	UINT32 reg_filter_x1                      : 8 ;  // 15: 8
	UINT32 reg_filter_y0                      : 8 ;  // 23:16
	UINT32 reg_filter_y1                      : 8 ;  // 31:24
} PE_H15A0_DNR2_MNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc80073b0L mnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_en                  : 1 ;  //     0
	UINT32 reg_motion_mnr_filter              : 2 ;  //  2: 1
	UINT32 reg_motion_mnr_filter_c            : 2 ;  //  4: 3
	UINT32 reg_mnr_filter                     : 2 ;  //  6: 5
	UINT32 reg_mnr_filter_c                   : 2 ;  //  8: 7
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_mnr_motion_min                 : 8 ;  // 23:16
	UINT32 reg_mnr_motion_max                 : 8 ;  // 31:24
} PE_H15A0_DNR2_MNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc80073b4L mnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_x0                  : 8 ;  //  7: 0
	UINT32 reg_motion_mnr_x1                  : 8 ;  // 15: 8
	UINT32 reg_motion_mnr_y0                  : 8 ;  // 23:16
	UINT32 reg_motion_mnr_y1                  : 8 ;  // 31:24
} PE_H15A0_DNR2_MNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc80073b8L detail_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_detail_max              : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_detail_min              : 8 ;  // 15: 8
	UINT32 reg_bnr_diff_l                     : 8 ;  // 23:16
	UINT32 reg_bnr_diff_p                     : 8 ;  // 31:24
} PE_H15A0_DNR2_DETAIL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80073bcL dc_bnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_enable                  : 1 ;  //     0
	UINT32 reg_dc_blur_debug_mode             : 1 ;  //     1
	UINT32 reg_dc_blur_sel                    : 2 ;  //  3: 2
	UINT32 reg_dc_output_debug_mode           : 4 ;  //  7: 4
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_dc_motion_max                  : 8 ;  // 23:16
	UINT32 reg_dc_motion_min                  : 8 ;  // 31:24
} PE_H15A0_DNR2_DC_BNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc80073c0L dc_bnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_detail_max                  : 8 ;  //  7: 0
	UINT32 reg_dc_detail_min                  : 8 ;  // 15: 8
	UINT32 reg_var_chroma_en                  : 1 ;  //    16
	UINT32                                    : 2 ;  // 18:17     reserved
	UINT32 reg_var_range_sel                  : 1 ;  //    19
	UINT32 reg_var_v_gain                     : 4 ;  // 23:20
	UINT32 reg_var_h_gain                     : 4 ;  // 27:24
	UINT32 reg_var_cut_resolution             : 4 ;  // 31:28
} PE_H15A0_DNR2_DC_BNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc80073c4L dc_bnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_var_en                      : 1 ;  //     0
	UINT32 reg_dc_motion_en                   : 1 ;  //     1
	UINT32 reg_dc_protection_en               : 1 ;  //     2
	UINT32 reg_dc_detail_en                   : 1 ;  //     3
	UINT32 reg_dc_global_motion_th            : 4 ;  //  7: 4
	UINT32 reg_detail_sel                     : 1 ;  //     8
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 reg_dc_bnr_mastergain              : 6 ;  // 15:10
	UINT32 reg_dc_bnr_chromagain              : 8 ;  // 23:16
	UINT32 reg_dc_protection_th               : 8 ;  // 31:24
} PE_H15A0_DNR2_DC_BNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc80073c8L dc_bnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_var_th3                 : 8 ;  //  7: 0
	UINT32 reg_dc_bnr_var_th2                 : 8 ;  // 15: 8
	UINT32 reg_dc_bnr_var_th1                 : 8 ;  // 23:16
	UINT32 reg_dc_bnr_var_th0                 : 8 ;  // 31:24
} PE_H15A0_DNR2_DC_BNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc80073ccL dc_bnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_motion_y_min                : 8 ;  //  7: 0
	UINT32 reg_dc_motion_y_max                : 8 ;  // 15: 8
	UINT32 reg_dc_detail_y_min                : 8 ;  // 23:16
	UINT32 reg_dc_detail_y_max                : 8 ;  // 31:24
} PE_H15A0_DNR2_DC_BNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc80073d0L dc_bnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_var_y_th0                   : 8 ;  //  7: 0
	UINT32 reg_dc_var_y_th1                   : 8 ;  // 15: 8
	UINT32 reg_dc_var_y_th2                   : 8 ;  // 23:16
	UINT32 reg_dc_var_y_th3                   : 8 ;  // 31:24
} PE_H15A0_DNR2_DC_BNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc80073d4L ac_bnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_h_en                    : 1 ;  //     0
	UINT32 reg_bnr_ac_v_en                    : 1 ;  //     1
	UINT32 reg_bnr_ac_h_chroma_en             : 1 ;  //     2
	UINT32 reg_bnr_ac_v_chroma_en             : 1 ;  //     3
	UINT32 reg_bnr_ac_acness_resol_h          : 2 ;  //  5: 4
	UINT32 reg_bnr_ac_sdhd_sel                : 1 ;  //     6
	UINT32 reg_ac_bnr_gain_sel                : 1 ;  //     7
	UINT32 reg_bnr_ac_diff_min_v_th           : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_diff_min_h_th           : 8 ;  // 23:16
	UINT32 reg_bnr_ac_iir_gain                : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc80073d8L ac_bnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_global_motion_th        : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_h_acness_max            : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_h_acness_min            : 8 ;  // 23:16
	UINT32 reg_bnr_ac_bin_th                  : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc80073dcL ac_bnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_motion_3                : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_motion_2                : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_motion_1                : 8 ;  // 23:16
	UINT32 reg_bnr_ac_motion_0                : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc80073e0L dnr_stat_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c420_tearing_cnt                   : 16;  // 15: 0
	UINT32 global_motion_iir                  : 8 ;  // 23:16
	UINT32 global_dc_gain                     : 8 ;  // 31:24
} PE_H15A0_DNR2_DNR_STAT_0_T;

/*-----------------------------------------------------------------------------
		0xc80073e4L ac_bnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_motion_y_3              : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_motion_y_2              : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_motion_y_1              : 8 ;  // 23:16
	UINT32 reg_bnr_ac_motion_y_0              : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc80073e8L ac_bnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_v_init_offset           : 3 ;  //  2: 0
	UINT32 reg_bnr_ac_h_init_offset           : 3 ;  //  5: 3
	UINT32 reg_bnr_ac_acness_resol_v          : 2 ;  //  7: 6
	UINT32 reg_bnr_ac_debug_mode              : 4 ;  // 11: 8
	UINT32 reg_bnr_ac_hsy_mode                : 4 ;  // 15:12
	UINT32 reg_bnr_ac_v_acness_max            : 8 ;  // 23:16
	UINT32 reg_bnr_ac_v_acness_min            : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc80073ecL dnr_stat_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 acness_or_gain_v_l                 : 8 ;  //  7: 0
	UINT32 acness_or_gain_h_l                 : 8 ;  // 15: 8
	UINT32 acness_or_gain_v_g                 : 8 ;  // 23:16
	UINT32 acness_or_gain_h_g                 : 8 ;  // 31:24
} PE_H15A0_DNR2_DNR_STAT_1_T;

/*-----------------------------------------------------------------------------
		0xc80073f0L ac_bnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_detail_th4              : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_detail_th3              : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_detail_th2              : 8 ;  // 23:16
	UINT32 reg_bnr_ac_detail_th1              : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc80073f4L ac_bnr_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_pos_gain_h2             : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_pos_gain_h1             : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_pos_gain_h0             : 8 ;  // 23:16
	UINT32 reg_bnr_ac_detail_gain_th          : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc80073f8L ac_bnr_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_pos_gain_l2             : 8 ;  //  7: 0
	UINT32 reg_bnr_ac_pos_gain_l1             : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_pos_gain_l0             : 8 ;  // 23:16
	UINT32 reg_bnr_ac_pos_gain_h3             : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc80073fcL dnr_stat_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_init                             : 8 ;  //  7: 0
	UINT32 v_init                             : 8 ;  // 15: 8
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 reg_vfilter_tearing_cnt            : 12;  // 31:20
} PE_H15A0_DNR2_DNR_STAT_2_T;

/*-----------------------------------------------------------------------------
		0xc8007400L ac_bnr_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ac_master_c_gain               : 8 ;  //  7: 0
	UINT32 reg_ac_bnr_enable                  : 1 ;  //     8
	UINT32 reg_ac_bnr_motion_en               : 1 ;  //     9
	UINT32 reg_ac_bnr_acness_en               : 1 ;  //    10
	UINT32 reg_ac_bnr_position_en             : 1 ;  //    11
	UINT32 reg_detail_sel                     : 1 ;  //    12
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_bnr_ac_manual_init_v           : 3 ;  // 18:16
	UINT32 reg_ac_manual_init_en_v            : 1 ;  //    19
	UINT32 reg_bnr_ac_manual_init_h           : 3 ;  // 22:20
	UINT32 reg_ac_manual_init_en_h            : 1 ;  //    23
	UINT32 reg_bnr_ac_pos_gain_l3             : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc8007404L ac_bnr_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_strong_filter_sel              : 1 ;  //     0
	UINT32 reg_weak_filter_sel                : 1 ;  //     1
	UINT32                                    : 1 ;  //     2     reserved
	UINT32 reg_acness_sel                     : 2 ;  //  4: 3
	UINT32                                    : 2 ;  //  6: 5     reserved
	UINT32 reg_acness_or_gain_sel             : 1 ;  //     7
	UINT32 reg_final_ac_bnr_gain_debug_h_en   : 1 ;  //     8
	UINT32 reg_final_ac_bnr_gain_debug_v_en   : 1 ;  //     9
	UINT32 reg_ac_master_gain                 : 6 ;  // 15:10
	UINT32 reg_g_motion_scale                 : 3 ;  // 18:16
	UINT32 reg_g_motion_div                   : 2 ;  // 20:19
} PE_H15A0_DNR2_AC_BNR_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc8007408L ac_bnr_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ac_detail_gain_y_min           : 8 ;  //  7: 0
	UINT32 reg_ac_detail_gain_y_max           : 8 ;  // 15: 8
	UINT32 reg_bnr_ac_acness_y_min            : 8 ;  // 23:16
	UINT32 reg_bnr_ac_acness_y_max            : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc800740cL ac_bnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ac_detail_alpha2_y_min         : 8 ;  //  7: 0
	UINT32 reg_ac_detail_alpha2_y_max         : 8 ;  // 15: 8
	UINT32 reg_ac_detail_alpha4_y_min         : 8 ;  // 23:16
	UINT32 reg_ac_detail_alpha4_y_max         : 8 ;  // 31:24
} PE_H15A0_DNR2_AC_BNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8007410L ac_bnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g_motion_manual                : 8 ;  //  7: 0
	UINT32 reg_acness_scale_mul               : 8 ;  // 15: 8
	UINT32                                    : 15;  // 30:16     reserved
	UINT32 reg_g_motion_manual_en             : 1 ;  //    31
} PE_H15A0_DNR2_AC_BNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8007414L ifc_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007418L ifc_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc800741cL ifc_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc8007420L ifc_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc8007424L ifc_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc8007428L ifc_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc800742cL ifc_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc8007430L ifc_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc8007434L ifc_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc8007438L ifc_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc800743cL ifc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc8007440L ifc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8007444L ifc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8007448L ifc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc800744cL ifc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc8007450L ifc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc8007454L ifc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc8007458L ifc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc800745cL ifc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc8007460L ifc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc8007464L ifc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc8007468L ifc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_IFC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc800746cL sc_bnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_bnr_en                      : 1 ;  //     0
	UINT32 reg_sc_bnr_manual_en               : 1 ;  //     1
	UINT32 reg_scene_ch_flag                  : 1 ;  //     2
	UINT32 reg_avg_scale_div_v                : 3 ;  //  5: 3
	UINT32 reg_avg_scale_mul_v                : 3 ;  //  8: 6
	UINT32                                    : 2 ;  // 10: 9     reserved
	UINT32 reg_scale_mul_v                    : 3 ;  // 13:11
	UINT32 reg_scale_div_v                    : 3 ;  // 16:14
	UINT32                                    : 1 ;  //    17     reserved
	UINT32 reg_avg_scale_mul_h                : 3 ;  // 20:18
	UINT32 reg_avg_scale_div_h                : 3 ;  // 23:21
	UINT32 reg_max_index_diff_th              : 3 ;  // 26:24
} PE_H15A0_DNR2_SC_BNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007470L sc_bnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_post2_iir_gain                 : 8 ;  //  7: 0
	UINT32 reg_frm_cnt_th                     : 8 ;  // 15: 8
	UINT32 reg_bin_ratio_th                   : 8 ;  // 23:16
	UINT32 reg_detect_th                      : 8 ;  // 31:24
} PE_H15A0_DNR2_SC_BNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007474L sc_bnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bin_ratio_div                  : 8 ;  //  7: 0
	UINT32 reg_bin_ratio_mul                  : 8 ;  // 15: 8
	UINT32 reg_min_bin_th                     : 8 ;  // 23:16
	UINT32 reg_detector_iir_gain              : 8 ;  // 31:24
} PE_H15A0_DNR2_SC_BNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc8007478L sc_bnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_grid_th1                       : 8 ;  //  7: 0
	UINT32 reg_grid_th2                       : 8 ;  // 15: 8
	UINT32 reg_grid_th3                       : 8 ;  // 23:16
	UINT32 reg_sc_bnr_x_min                   : 8 ;  // 31:24
} PE_H15A0_DNR2_SC_BNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc800747cL sc_bnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pos_gain_th                    : 8 ;  //  7: 0
	UINT32 reg_sc_bnr_y_max                   : 8 ;  // 15: 8
	UINT32 reg_sc_bnr_y_min                   : 8 ;  // 23:16
	UINT32 reg_sc_bnr_x_max                   : 8 ;  // 31:24
} PE_H15A0_DNR2_SC_BNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc8007480L sc_bnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pos_gain_3                     : 8 ;  //  7: 0
	UINT32 reg_pos_gain_2                     : 8 ;  // 15: 8
	UINT32 reg_pos_gain_1                     : 8 ;  // 23:16
	UINT32 reg_scale_div_h                    : 3 ;  // 26:24
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_scale_mul_h                    : 3 ;  // 30:28
	UINT32 reg_iir_en                         : 1 ;  //    31
} PE_H15A0_DNR2_SC_BNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc8007484L sc_bnr_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_bnr_debug_en                : 1 ;  //     0
	UINT32 reg_sc_bnr_debug_mode              : 2 ;  //  2: 1
	UINT32                                    : 21;  // 23: 3     reserved
	UINT32 reg_acness_th                      : 8 ;  // 31:24
} PE_H15A0_DNR2_SC_BNR_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc8007488L c_pre_blur_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre_121_blur_en                    : 1 ;  //     0
	UINT32 pre_median_en                      : 1 ;  //     1
} PE_H15A0_DNR2_C_PRE_BLUR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc800748cL texture_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_texture_cnt_div                : 3 ;  //  2: 0
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 reg_texture_cnt_mul                : 8 ;  // 15: 8
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 reg_cross_th                       : 8 ;  // 31:24
} PE_H15A0_DNR2_TEXTURE_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc8007490L texture_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_TEXTURE_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc8007494L texture_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_TEXTURE_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc8007498L texture_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_TEXTURE_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc80074d0L dnr_stat_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_acness_or_gain_h_t3              : 8 ;  //  7: 0
	UINT32 w_acness_or_gain_h_t2              : 8 ;  // 15: 8
	UINT32 w_acness_or_gain_h_t1              : 8 ;  // 23:16
	UINT32 w_acness_or_gain_h_t0              : 8 ;  // 31:24
} PE_H15A0_DNR2_DNR_STAT_3_T;

/*-----------------------------------------------------------------------------
		0xc80074d4L dnr_stat_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_acness_or_gain_h_b3              : 8 ;  //  7: 0
	UINT32 w_acness_or_gain_h_b2              : 8 ;  // 15: 8
	UINT32 w_acness_or_gain_h_b1              : 8 ;  // 23:16
	UINT32 w_acness_or_gain_h_b0              : 8 ;  // 31:24
} PE_H15A0_DNR2_DNR_STAT_4_T;

/*-----------------------------------------------------------------------------
		0xc80074d8L dnr_stat_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_acness_or_gain_v_t3              : 8 ;  //  7: 0
	UINT32 w_acness_or_gain_v_t2              : 8 ;  // 15: 8
	UINT32 w_acness_or_gain_v_t1              : 8 ;  // 23:16
	UINT32 w_acness_or_gain_v_t0              : 8 ;  // 31:24
} PE_H15A0_DNR2_DNR_STAT_5_T;

/*-----------------------------------------------------------------------------
		0xc80074dcL dnr_stat_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w_acness_or_gain_v_b3              : 8 ;  //  7: 0
	UINT32 w_acness_or_gain_v_b2              : 8 ;  // 15: 8
	UINT32 w_acness_or_gain_v_b1              : 8 ;  // 23:16
	UINT32 w_acness_or_gain_v_b0              : 8 ;  // 31:24
} PE_H15A0_DNR2_DNR_STAT_6_T;

/*-----------------------------------------------------------------------------
		0xc80074e0L dnr_stat_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 acness_gain_v                      : 8 ;  //  7: 0
	UINT32 acness_gain_h                      : 8 ;  // 15: 8
	UINT32 acness_v_iir                       : 8 ;  // 23:16
	UINT32 acness_h_iir                       : 8 ;  // 31:24
} PE_H15A0_DNR2_DNR_STAT_7_T;

/*-----------------------------------------------------------------------------
		0xc80074e4L dnr_stat_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 bin_ratio_iir                      : 8 ;  // 15: 8
	UINT32 bin_ratio                          : 8 ;  // 23:16
	UINT32 hys_cnt                            : 3 ;  // 26:24
	UINT32                                    : 4 ;  // 30:27     reserved
	UINT32 sc_bnr_flag                        : 1 ;  //    31
} PE_H15A0_DNR2_DNR_STAT_8_T;

/*-----------------------------------------------------------------------------
		0xc80074e8L dnr_stat_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_index_iir                      : 7 ;  //  6: 0
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 max_index                          : 7 ;  // 14: 8
} PE_H15A0_DNR2_DNR_STAT_9_T;

/*-----------------------------------------------------------------------------
		0xc80074ecL dnr_stat_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_DNR2_DNR_STAT_10_T;

typedef struct {
	PE_H15A0_ND1_PE0_LOAD_T               	pe0_load;                 //0xc8004100L
	PE_H15A0_ND1_PE0_INTR_T               	pe0_intr;                 //0xc8004104L
	PE_H15A0_ND1_PE0_STATUS0_T            	pe0_status0;              //0xc8004108L
	PE_H15A0_ND1_PE0_STATUS1_T            	pe0_status1;              //0xc800410cL
	PE_H15A0_ND1_PE0_OPMODE_T             	pe0_opmode;               //0xc8004110L
	PE_H15A0_ND1_PE0_SRC_SIZE_T           	pe0_src_size;             //0xc8004114L
	PE_H15A0_ND1_PE0_FIELD_T              	pe0_field;                //0xc8004118L
	UINT32                             	  	reserved00;               //0xc800411cL
	PE_H15A0_ND1_PE0_IN_OFFSET_T          	pe0_in_offset;            //0xc8004120L
	PE_H15A0_ND1_PE0_IN_SIZE_T            	pe0_in_size;              //0xc8004124L
	PE_H15A0_ND1_FBC_CTRL_T               	fbc_ctrl;                 //0xc8004128L
	UINT32                             	  	reserved01;               //0xc800412cL
	PE_H15A0_ND1_PE0_DEBUG_MEM_REQ_T      	pe0_debug_mem_req;        //0xc8004130L
	PE_H15A0_ND1_PE0_DEBUG_LREQ_DLY_T     	pe0_debug_lreq_dly;       //0xc8004134L
	UINT32                             	  	reserved02[2];            //0xc8004138L~0xc800413cL
	PE_H15A0_ND1_TNR_CTRL_00_T            	tnr_ctrl_00;              //0xc8004140L
	PE_H15A0_ND1_TNR_CTRL_01_T            	tnr_ctrl_01;              //0xc8004144L
	PE_H15A0_ND1_TNR_CTRL_02_T            	tnr_ctrl_02;              //0xc8004148L
	PE_H15A0_ND1_TNR_CTRL_03_T            	tnr_ctrl_03;              //0xc800414cL
	PE_H15A0_ND1_TNR_CTRL_04_T            	tnr_ctrl_04;              //0xc8004150L
	PE_H15A0_ND1_TNR_CTRL_05_T            	tnr_ctrl_05;              //0xc8004154L
	PE_H15A0_ND1_TNR_CTRL_06_T            	tnr_ctrl_06;              //0xc8004158L
	PE_H15A0_ND1_TNR_CTRL_07_T            	tnr_ctrl_07;              //0xc800415cL
	PE_H15A0_ND1_TNR_CTRL_08_T            	tnr_ctrl_08;              //0xc8004160L
	PE_H15A0_ND1_TNR_CTRL_09_T            	tnr_ctrl_09;              //0xc8004164L
	PE_H15A0_ND1_TNR_CTRL_10_T            	tnr_ctrl_10;              //0xc8004168L
	PE_H15A0_ND1_TNR_CTRL_11_T            	tnr_ctrl_11;              //0xc800416cL
	PE_H15A0_ND1_TNR_CTRL_12_T            	tnr_ctrl_12;              //0xc8004170L
	PE_H15A0_ND1_TNR_CTRL_13_T            	tnr_ctrl_13;              //0xc8004174L
	PE_H15A0_ND1_TNR_CTRL_14_T            	tnr_ctrl_14;              //0xc8004178L
	PE_H15A0_ND1_TNR_CTRL_15_T            	tnr_ctrl_15;              //0xc800417cL
	PE_H15A0_ND1_TNR_CTRL_16_T            	tnr_ctrl_16;              //0xc8004180L
	PE_H15A0_ND1_TNR_CTRL_17_T            	tnr_ctrl_17;              //0xc8004184L
	PE_H15A0_ND1_TNR_CTRL_18_T            	tnr_ctrl_18;              //0xc8004188L
	PE_H15A0_ND1_TNR_CTRL_19_T            	tnr_ctrl_19;              //0xc800418cL
	PE_H15A0_ND1_TNR_CTRL_20_T            	tnr_ctrl_20;              //0xc8004190L
	PE_H15A0_ND1_TNR_CTRL_21_T            	tnr_ctrl_21;              //0xc8004194L
	PE_H15A0_ND1_TNR_CTRL_22_T            	tnr_ctrl_22;              //0xc8004198L
	PE_H15A0_ND1_TNR_CTRL_23_T            	tnr_ctrl_23;              //0xc800419cL
	PE_H15A0_ND1_TNR_CTRL_24_T            	tnr_ctrl_24;              //0xc80041a0L
	PE_H15A0_ND1_TNR_CTRL_25_T            	tnr_ctrl_25;              //0xc80041a4L
	PE_H15A0_ND1_TNR_CTRL_26_T            	tnr_ctrl_26;              //0xc80041a8L
	PE_H15A0_ND1_TNR_CTRL_27_T            	tnr_ctrl_27;              //0xc80041acL
	PE_H15A0_ND1_TNR_CTRL_28_T            	tnr_ctrl_28;              //0xc80041b0L
	PE_H15A0_ND1_TNR_CTRL_29_T            	tnr_ctrl_29;              //0xc80041b4L
	PE_H15A0_ND1_TNR_CTRL_30_T            	tnr_ctrl_30;              //0xc80041b8L
	PE_H15A0_ND1_TNR_CTRL_31_T            	tnr_ctrl_31;              //0xc80041bcL
	PE_H15A0_ND1_TNR_STATUS_00_T          	tnr_status_00;            //0xc80041c0L
	PE_H15A0_ND1_TNR_STATUS_01_T          	tnr_status_01;            //0xc80041c4L
	PE_H15A0_ND1_TNR_STATUS_02_T          	tnr_status_02;            //0xc80041c8L
	PE_H15A0_ND1_TNR_STATUS_03_T          	tnr_status_03;            //0xc80041ccL
	PE_H15A0_ND1_TNR_MAIN_LUT_00_T        	tnr_main_lut_00;          //0xc80041d0L
	PE_H15A0_ND1_TNR_MAIN_LUT_01_T        	tnr_main_lut_01;          //0xc80041d4L
	UINT32                             	  	reserved03;               //0xc80041d8L
	PE_H15A0_ND1_TNR_STATUS_04_T          	tnr_status_04;            //0xc80041dcL
	PE_H15A0_ND1_TNR_CTRL_32_T            	tnr_ctrl_32;              //0xc80041e0L
	PE_H15A0_ND1_TNR_CTRL_33_T            	tnr_ctrl_33;              //0xc80041e4L
	PE_H15A0_ND1_TNR_CTRL_34_T            	tnr_ctrl_34;              //0xc80041e8L
	PE_H15A0_ND1_TNR_CTRL_35_T            	tnr_ctrl_35;              //0xc80041ecL
	PE_H15A0_ND1_TNR_CTRL_36_T            	tnr_ctrl_36;              //0xc80041f0L
	PE_H15A0_ND1_TNR_CTRL_37_T            	tnr_ctrl_37;              //0xc80041f4L
	PE_H15A0_ND1_TNR_CTRL_38_T            	tnr_ctrl_38;              //0xc80041f8L
	PE_H15A0_ND1_TNR_CTRL_39_T            	tnr_ctrl_39;              //0xc80041fcL
	PE_H15A0_ND1_IPC_CTRL_00_T            	ipc_ctrl_00;              //0xc8004200L
	PE_H15A0_ND1_IPC_CTRL_01_T            	ipc_ctrl_01;              //0xc8004204L
	PE_H15A0_ND1_IPC_CTRL_02_T            	ipc_ctrl_02;              //0xc8004208L
	PE_H15A0_ND1_IPC_CTRL_03_T            	ipc_ctrl_03;              //0xc800420cL
	PE_H15A0_ND1_IPC_CTRL_04_T            	ipc_ctrl_04;              //0xc8004210L
	PE_H15A0_ND1_IPC_CTRL_05_T            	ipc_ctrl_05;              //0xc8004214L
	PE_H15A0_ND1_IPC_CTRL_06_T            	ipc_ctrl_06;              //0xc8004218L
	PE_H15A0_ND1_IPC_CTRL_07_T            	ipc_ctrl_07;              //0xc800421cL
	PE_H15A0_ND1_IPC_CTRL_08_T            	ipc_ctrl_08;              //0xc8004220L
	PE_H15A0_ND1_IPC_CTRL_09_T            	ipc_ctrl_09;              //0xc8004224L
	PE_H15A0_ND1_IPC_CTRL_10_T            	ipc_ctrl_10;              //0xc8004228L
	PE_H15A0_ND1_IPC_CTRL_11_T            	ipc_ctrl_11;              //0xc800422cL
	PE_H15A0_ND1_IPC_CTRL_12_T            	ipc_ctrl_12;              //0xc8004230L
	PE_H15A0_ND1_IPC_CTRL_13_T            	ipc_ctrl_13;              //0xc8004234L
	PE_H15A0_ND1_IPC_CTRL_14_T            	ipc_ctrl_14;              //0xc8004238L
	PE_H15A0_ND1_IPC_CTRL_15_T            	ipc_ctrl_15;              //0xc800423cL
	PE_H15A0_ND1_IPC_CTRL_16_T            	ipc_ctrl_16;              //0xc8004240L
	PE_H15A0_ND1_IPC_CTRL_17_T            	ipc_ctrl_17;              //0xc8004244L
	PE_H15A0_ND1_IPC_CTRL_18_T            	ipc_ctrl_18;              //0xc8004248L
	PE_H15A0_ND1_IPC_CTRL_19_T            	ipc_ctrl_19;              //0xc800424cL
	PE_H15A0_ND1_IPC_STATUS_00_T          	ipc_status_00;            //0xc8004250L
	PE_H15A0_ND1_IPC_STATUS_01_T          	ipc_status_01;            //0xc8004254L
	PE_H15A0_ND1_IPC_STATUS_02_T          	ipc_status_02;            //0xc8004258L
	PE_H15A0_ND1_IPC_STATUS_03_T          	ipc_status_03;            //0xc800425cL
	PE_H15A0_ND1_CLC_CTRL_00_T            	clc_ctrl_00;              //0xc8004260L
	PE_H15A0_ND1_CLC_CTRL_01_T            	clc_ctrl_01;              //0xc8004264L
	PE_H15A0_ND1_CLC_CTRL_02_T            	clc_ctrl_02;              //0xc8004268L
	PE_H15A0_ND1_CLC_CTRL_03_T            	clc_ctrl_03;              //0xc800426cL
	PE_H15A0_ND1_CLC_CTRL_04_T            	clc_ctrl_04;              //0xc8004270L
	PE_H15A0_ND1_CLC_CTRL_05_T            	clc_ctrl_05;              //0xc8004274L
	PE_H15A0_ND1_CLC_CTRL_06_T            	clc_ctrl_06;              //0xc8004278L
	PE_H15A0_ND1_CLC_STAT_00_T            	clc_stat_00;              //0xc800427cL
	PE_H15A0_ND1_IPC_CTRL_34_T            	ipc_ctrl_34;              //0xc8004280L
	PE_H15A0_ND1_IPC_CTRL_35_T            	ipc_ctrl_35;              //0xc8004284L
	PE_H15A0_ND1_IPC_CTRL_36_T            	ipc_ctrl_36;              //0xc8004288L
	PE_H15A0_ND1_IPC_CTRL_37_T            	ipc_ctrl_37;              //0xc800428cL
	PE_H15A0_ND1_IPC_CTRL_38_T            	ipc_ctrl_38;              //0xc8004290L
	PE_H15A0_ND1_IPC_CTRL_39_T            	ipc_ctrl_39;              //0xc8004294L
	PE_H15A0_ND1_IPC_CTRL_40_T            	ipc_ctrl_40;              //0xc8004298L
	PE_H15A0_ND1_IPC_CTRL_41_T            	ipc_ctrl_41;              //0xc800429cL
	PE_H15A0_ND1_IPC_CTRL_42_T            	ipc_ctrl_42;              //0xc80042a0L
	PE_H15A0_ND1_IPC_CTRL_43_T            	ipc_ctrl_43;              //0xc80042a4L
	PE_H15A0_ND1_IPC_CTRL_44_T            	ipc_ctrl_44;              //0xc80042a8L
	UINT32                             	  	reserved04[17];           //0xc80042acL~0xc80042ecL
	PE_H15A0_ND1_IPC_CTRL_20_T            	ipc_ctrl_20;              //0xc80042f0L
	PE_H15A0_ND1_IPC_CTRL_21_T            	ipc_ctrl_21;              //0xc80042f4L
	PE_H15A0_ND1_IPC_CTRL_22_T            	ipc_ctrl_22;              //0xc80042f8L
	PE_H15A0_ND1_IPC_CTRL_23_T            	ipc_ctrl_23;              //0xc80042fcL
	PE_H15A0_ND1_IPC_CTRL_24_T            	ipc_ctrl_24;              //0xc8004300L
	PE_H15A0_ND1_IPC_CTRL_25_T            	ipc_ctrl_25;              //0xc8004304L
	PE_H15A0_ND1_IPC_CTRL_26_T            	ipc_ctrl_26;              //0xc8004308L
	PE_H15A0_ND1_IPC_CTRL_27_T            	ipc_ctrl_27;              //0xc800430cL
	PE_H15A0_ND1_IPC_CTRL_28_T            	ipc_ctrl_28;              //0xc8004310L
	PE_H15A0_ND1_IPC_CTRL_29_T            	ipc_ctrl_29;              //0xc8004314L
	PE_H15A0_ND1_IPC_CTRL_30_T            	ipc_ctrl_30;              //0xc8004318L
	PE_H15A0_ND1_IPC_CTRL_31_T            	ipc_ctrl_31;              //0xc800431cL
	PE_H15A0_ND1_IPC_CTRL_32_T            	ipc_ctrl_32;              //0xc8004320L
	PE_H15A0_ND1_IPC_CTRL_33_T            	ipc_ctrl_33;              //0xc8004324L
	PE_H15A0_ND1_IPC_STATUS_04_T          	ipc_status_04;            //0xc8004328L
	PE_H15A0_ND1_IPC_STATUS_05_T          	ipc_status_05;            //0xc800432cL
	PE_H15A0_ND1_IPC_STATUS_06_T          	ipc_status_06;            //0xc8004330L
	PE_H15A0_ND1_IPC_STATUS_07_T          	ipc_status_07;            //0xc8004334L
	PE_H15A0_ND1_IPC_STATUS_08_T          	ipc_status_08;            //0xc8004338L
	PE_H15A0_ND1_IPC_STATUS_09_T          	ipc_status_09;            //0xc800433cL
	PE_H15A0_ND1_TPD_CTRL_00_T            	tpd_ctrl_00;              //0xc8004340L
	PE_H15A0_ND1_TPD_CTRL_01_T            	tpd_ctrl_01;              //0xc8004344L
	PE_H15A0_ND1_TPD_CTRL_02_T            	tpd_ctrl_02;              //0xc8004348L
	PE_H15A0_ND1_TPD_CTRL_03_T            	tpd_ctrl_03;              //0xc800434cL
	PE_H15A0_ND1_TPD_CTRL_04_T            	tpd_ctrl_04;              //0xc8004350L
	PE_H15A0_ND1_TPD_CTRL_05_T            	tpd_ctrl_05;              //0xc8004354L
	PE_H15A0_ND1_TPD_CTRL_06_T            	tpd_ctrl_06;              //0xc8004358L
	PE_H15A0_ND1_TPD_CTRL_07_T            	tpd_ctrl_07;              //0xc800435cL
	PE_H15A0_ND1_TPD_CTRL_08_T            	tpd_ctrl_08;              //0xc8004360L
	PE_H15A0_ND1_TPD_CTRL_09_T            	tpd_ctrl_09;              //0xc8004364L
	PE_H15A0_ND1_TPD_STAT_00_T            	tpd_stat_00;              //0xc8004368L
	PE_H15A0_ND1_CLC_CTRL_07_T            	clc_ctrl_07;              //0xc800436cL
	PE_H15A0_ND1_BBD_CTRL_00_T            	bbd_ctrl_00;              //0xc8004370L
	PE_H15A0_ND1_BBD_CTRL_01_T            	bbd_ctrl_01;              //0xc8004374L
	PE_H15A0_ND1_BBD_CTRL_02_T            	bbd_ctrl_02;              //0xc8004378L
	PE_H15A0_ND1_BBD_STAT_00_T            	bbd_stat_00;              //0xc800437cL
	PE_H15A0_ND1_BBD_STAT_01_T            	bbd_stat_01;              //0xc8004380L
	PE_H15A0_ND1_APL_STAT_00_T            	apl_stat_00;              //0xc8004384L
	UINT32                             	  	reserved05[2];            //0xc8004388L~0xc800438cL
	PE_H15A0_ND1_TPD_HIST_AI_CTRL_T       	tpd_hist_ai_ctrl;         //0xc8004390L
	PE_H15A0_ND1_TPD_HIST_AI_DATA_T       	tpd_hist_ai_data;         //0xc8004394L
	PE_H15A0_ND1_LVCRTL_CTRL_00_T         	lvcrtl_ctrl_00;           //0xc8004398L
	UINT32                             	  	reserved06;               //0xc800439cL
	PE_H15A0_ND1_TNR_STATUS_05_T          	tnr_status_05;            //0xc80043a0L
	PE_H15A0_ND1_TNR_STATUS_06_T          	tnr_status_06;            //0xc80043a4L
	PE_H15A0_ND1_TNR_STATUS_07_T          	tnr_status_07;            //0xc80043a8L
	PE_H15A0_ND1_TNR_STATUS_08_T          	tnr_status_08;            //0xc80043acL
	UINT32                             	  	reserved07[8];            //0xc80043b0L~0xc80043ccL
	PE_H15A0_ND1_TNR_CTRL_40_T            	tnr_ctrl_40;              //0xc80043d0L
	PE_H15A0_ND1_TNR_CTRL_41_T            	tnr_ctrl_41;              //0xc80043d4L
	PE_H15A0_ND1_TNR_CTRL_42_T            	tnr_ctrl_42;              //0xc80043d8L
	PE_H15A0_ND1_TNR_CTRL_43_T            	tnr_ctrl_43;              //0xc80043dcL
	PE_H15A0_ND1_TNR_CTRL_44_T            	tnr_ctrl_44;              //0xc80043e0L
	PE_H15A0_ND1_TNR_CTRL_45_T            	tnr_ctrl_45;              //0xc80043e4L
	PE_H15A0_ND1_TNR_CTRL_46_T            	tnr_ctrl_46;              //0xc80043e8L
	PE_H15A0_ND1_TNR_CTRL_47_T            	tnr_ctrl_47;              //0xc80043ecL
	PE_H15A0_ND1_TNR_CTRL_48_T            	tnr_ctrl_48;              //0xc80043f0L
	PE_H15A0_ND1_TNR_CTRL_49_T            	tnr_ctrl_49;              //0xc80043f4L
	PE_H15A0_ND1_TNR_CTRL_50_T            	tnr_ctrl_50;              //0xc80043f8L
	PE_H15A0_ND1_TNR_CTRL_51_T            	tnr_ctrl_51;              //0xc80043fcL
	PE_H15A0_ND1_TNR_NE_STAT_00_T         	tnr_ne_stat_00;           //0xc8004400L
	PE_H15A0_ND1_TNR_NE_STAT_01_T         	tnr_ne_stat_01;           //0xc8004404L
	PE_H15A0_ND1_TNR_NE_STAT_02_T         	tnr_ne_stat_02;           //0xc8004408L
	PE_H15A0_ND1_TNR_NE_STAT_03_T         	tnr_ne_stat_03;           //0xc800440cL
	PE_H15A0_ND1_TNR_NE_STAT_04_T         	tnr_ne_stat_04;           //0xc8004410L
	PE_H15A0_ND1_TNR_NE_STAT_05_T         	tnr_ne_stat_05;           //0xc8004414L
	PE_H15A0_ND1_TNR_NE_STAT_06_T         	tnr_ne_stat_06;           //0xc8004418L
	PE_H15A0_ND1_TNR_NE_STAT_07_T         	tnr_ne_stat_07;           //0xc800441cL
	PE_H15A0_ND1_TNR_NE_STAT_08_T         	tnr_ne_stat_08;           //0xc8004420L
	PE_H15A0_ND1_TNR_NE_STAT_09_T         	tnr_ne_stat_09;           //0xc8004424L
	PE_H15A0_ND1_TNR_NE_STAT_10_T         	tnr_ne_stat_10;           //0xc8004428L
	PE_H15A0_ND1_TNR_NE_STAT_11_T         	tnr_ne_stat_11;           //0xc800442cL
	PE_H15A0_ND1_TNR_NE_STAT_12_T         	tnr_ne_stat_12;           //0xc8004430L
}PE_ND_ND1_REG_H15A0_T;

typedef struct {
	PE_H15A0_ND2_PE0_LOAD_T               	pe0_load;                 //0xc8005100L
	PE_H15A0_ND2_PE0_INTR_T               	pe0_intr;                 //0xc8005104L
	PE_H15A0_ND2_PE0_STATUS0_T            	pe0_status0;              //0xc8005108L
	PE_H15A0_ND2_PE0_STATUS1_T            	pe0_status1;              //0xc800510cL
	PE_H15A0_ND2_PE0_OPMODE_T             	pe0_opmode;               //0xc8005110L
	PE_H15A0_ND2_PE0_SRC_SIZE_T           	pe0_src_size;             //0xc8005114L
	PE_H15A0_ND2_PE0_FIELD_T              	pe0_field;                //0xc8005118L
	UINT32                             	  	reserved00;               //0xc800511cL
	PE_H15A0_ND2_PE0_IN_OFFSET_T          	pe0_in_offset;            //0xc8005120L
	PE_H15A0_ND2_PE0_IN_SIZE_T            	pe0_in_size;              //0xc8005124L
	PE_H15A0_ND2_FBC_CTRL_T               	fbc_ctrl;                 //0xc8005128L
	UINT32                             	  	reserved01;               //0xc800512cL
	PE_H15A0_ND2_PE0_DEBUG_MEM_REQ_T      	pe0_debug_mem_req;        //0xc8005130L
	PE_H15A0_ND2_PE0_DEBUG_LREQ_DLY_T     	pe0_debug_lreq_dly;       //0xc8005134L
	UINT32                             	  	reserved02[2];            //0xc8005138L~0xc800513cL
	PE_H15A0_ND2_TNR_CTRL_00_T            	tnr_ctrl_00;              //0xc8005140L
	PE_H15A0_ND2_TNR_CTRL_01_T            	tnr_ctrl_01;              //0xc8005144L
	PE_H15A0_ND2_TNR_CTRL_02_T            	tnr_ctrl_02;              //0xc8005148L
	PE_H15A0_ND2_TNR_CTRL_03_T            	tnr_ctrl_03;              //0xc800514cL
	PE_H15A0_ND2_TNR_CTRL_04_T            	tnr_ctrl_04;              //0xc8005150L
	PE_H15A0_ND2_TNR_CTRL_05_T            	tnr_ctrl_05;              //0xc8005154L
	PE_H15A0_ND2_TNR_CTRL_06_T            	tnr_ctrl_06;              //0xc8005158L
	PE_H15A0_ND2_TNR_CTRL_07_T            	tnr_ctrl_07;              //0xc800515cL
	PE_H15A0_ND2_TNR_CTRL_08_T            	tnr_ctrl_08;              //0xc8005160L
	PE_H15A0_ND2_TNR_CTRL_09_T            	tnr_ctrl_09;              //0xc8005164L
	PE_H15A0_ND2_TNR_CTRL_10_T            	tnr_ctrl_10;              //0xc8005168L
	PE_H15A0_ND2_TNR_CTRL_11_T            	tnr_ctrl_11;              //0xc800516cL
	PE_H15A0_ND2_TNR_CTRL_12_T            	tnr_ctrl_12;              //0xc8005170L
	PE_H15A0_ND2_TNR_CTRL_13_T            	tnr_ctrl_13;              //0xc8005174L
	PE_H15A0_ND2_TNR_CTRL_14_T            	tnr_ctrl_14;              //0xc8005178L
	PE_H15A0_ND2_TNR_CTRL_15_T            	tnr_ctrl_15;              //0xc800517cL
	PE_H15A0_ND2_TNR_CTRL_16_T            	tnr_ctrl_16;              //0xc8005180L
	PE_H15A0_ND2_TNR_CTRL_17_T            	tnr_ctrl_17;              //0xc8005184L
	PE_H15A0_ND2_TNR_CTRL_18_T            	tnr_ctrl_18;              //0xc8005188L
	PE_H15A0_ND2_TNR_CTRL_19_T            	tnr_ctrl_19;              //0xc800518cL
	PE_H15A0_ND2_TNR_CTRL_20_T            	tnr_ctrl_20;              //0xc8005190L
	PE_H15A0_ND2_TNR_CTRL_21_T            	tnr_ctrl_21;              //0xc8005194L
	PE_H15A0_ND2_TNR_CTRL_22_T            	tnr_ctrl_22;              //0xc8005198L
	PE_H15A0_ND2_TNR_CTRL_23_T            	tnr_ctrl_23;              //0xc800519cL
	PE_H15A0_ND2_TNR_CTRL_24_T            	tnr_ctrl_24;              //0xc80051a0L
	PE_H15A0_ND2_TNR_CTRL_25_T            	tnr_ctrl_25;              //0xc80051a4L
	PE_H15A0_ND2_TNR_CTRL_26_T            	tnr_ctrl_26;              //0xc80051a8L
	PE_H15A0_ND2_TNR_CTRL_27_T            	tnr_ctrl_27;              //0xc80051acL
	PE_H15A0_ND2_TNR_CTRL_28_T            	tnr_ctrl_28;              //0xc80051b0L
	PE_H15A0_ND2_TNR_CTRL_29_T            	tnr_ctrl_29;              //0xc80051b4L
	PE_H15A0_ND2_TNR_CTRL_30_T            	tnr_ctrl_30;              //0xc80051b8L
	PE_H15A0_ND2_TNR_CTRL_31_T            	tnr_ctrl_31;              //0xc80051bcL
	PE_H15A0_ND2_TNR_STATUS_00_T          	tnr_status_00;            //0xc80051c0L
	PE_H15A0_ND2_TNR_STATUS_01_T          	tnr_status_01;            //0xc80051c4L
	PE_H15A0_ND2_TNR_STATUS_02_T          	tnr_status_02;            //0xc80051c8L
	PE_H15A0_ND2_TNR_STATUS_03_T          	tnr_status_03;            //0xc80051ccL
	PE_H15A0_ND2_TNR_MAIN_LUT_00_T        	tnr_main_lut_00;          //0xc80051d0L
	PE_H15A0_ND2_TNR_MAIN_LUT_01_T        	tnr_main_lut_01;          //0xc80051d4L
	UINT32                             	  	reserved03;               //0xc80051d8L
	PE_H15A0_ND2_TNR_STATUS_04_T          	tnr_status_04;            //0xc80051dcL
	PE_H15A0_ND2_TNR_CTRL_32_T            	tnr_ctrl_32;              //0xc80051e0L
	PE_H15A0_ND2_TNR_CTRL_33_T            	tnr_ctrl_33;              //0xc80051e4L
	PE_H15A0_ND2_TNR_CTRL_34_T            	tnr_ctrl_34;              //0xc80051e8L
	PE_H15A0_ND2_TNR_CTRL_35_T            	tnr_ctrl_35;              //0xc80051ecL
	PE_H15A0_ND2_TNR_CTRL_36_T            	tnr_ctrl_36;              //0xc80051f0L
	PE_H15A0_ND2_TNR_CTRL_37_T            	tnr_ctrl_37;              //0xc80051f4L
	PE_H15A0_ND2_TNR_CTRL_38_T            	tnr_ctrl_38;              //0xc80051f8L
	PE_H15A0_ND2_TNR_CTRL_39_T            	tnr_ctrl_39;              //0xc80051fcL
	PE_H15A0_ND2_IPC_CTRL_00_T            	ipc_ctrl_00;              //0xc8005200L
	PE_H15A0_ND2_IPC_CTRL_01_T            	ipc_ctrl_01;              //0xc8005204L
	PE_H15A0_ND2_IPC_CTRL_02_T            	ipc_ctrl_02;              //0xc8005208L
	PE_H15A0_ND2_IPC_CTRL_03_T            	ipc_ctrl_03;              //0xc800520cL
	PE_H15A0_ND2_IPC_CTRL_04_T            	ipc_ctrl_04;              //0xc8005210L
	PE_H15A0_ND2_IPC_CTRL_05_T            	ipc_ctrl_05;              //0xc8005214L
	PE_H15A0_ND2_IPC_CTRL_06_T            	ipc_ctrl_06;              //0xc8005218L
	PE_H15A0_ND2_IPC_CTRL_07_T            	ipc_ctrl_07;              //0xc800521cL
	PE_H15A0_ND2_IPC_CTRL_08_T            	ipc_ctrl_08;              //0xc8005220L
	PE_H15A0_ND2_IPC_CTRL_09_T            	ipc_ctrl_09;              //0xc8005224L
	PE_H15A0_ND2_IPC_CTRL_10_T            	ipc_ctrl_10;              //0xc8005228L
	PE_H15A0_ND2_IPC_CTRL_11_T            	ipc_ctrl_11;              //0xc800522cL
	PE_H15A0_ND2_IPC_CTRL_12_T            	ipc_ctrl_12;              //0xc8005230L
	PE_H15A0_ND2_IPC_CTRL_13_T            	ipc_ctrl_13;              //0xc8005234L
	PE_H15A0_ND2_IPC_CTRL_14_T            	ipc_ctrl_14;              //0xc8005238L
	PE_H15A0_ND2_IPC_CTRL_15_T            	ipc_ctrl_15;              //0xc800523cL
	PE_H15A0_ND2_IPC_CTRL_16_T            	ipc_ctrl_16;              //0xc8005240L
	PE_H15A0_ND2_IPC_CTRL_17_T            	ipc_ctrl_17;              //0xc8005244L
	PE_H15A0_ND2_IPC_CTRL_18_T            	ipc_ctrl_18;              //0xc8005248L
	PE_H15A0_ND2_IPC_CTRL_19_T            	ipc_ctrl_19;              //0xc800524cL
	PE_H15A0_ND2_IPC_STATUS_00_T          	ipc_status_00;            //0xc8005250L
	PE_H15A0_ND2_IPC_STATUS_01_T          	ipc_status_01;            //0xc8005254L
	PE_H15A0_ND2_IPC_STATUS_02_T          	ipc_status_02;            //0xc8005258L
	PE_H15A0_ND2_IPC_STATUS_03_T          	ipc_status_03;            //0xc800525cL
	PE_H15A0_ND2_CLC_CTRL_00_T            	clc_ctrl_00;              //0xc8005260L
	PE_H15A0_ND2_CLC_CTRL_01_T            	clc_ctrl_01;              //0xc8005264L
	PE_H15A0_ND2_CLC_CTRL_02_T            	clc_ctrl_02;              //0xc8005268L
	PE_H15A0_ND2_CLC_CTRL_03_T            	clc_ctrl_03;              //0xc800526cL
	PE_H15A0_ND2_CLC_CTRL_04_T            	clc_ctrl_04;              //0xc8005270L
	PE_H15A0_ND2_CLC_CTRL_05_T            	clc_ctrl_05;              //0xc8005274L
	PE_H15A0_ND2_CLC_CTRL_06_T            	clc_ctrl_06;              //0xc8005278L
	PE_H15A0_ND2_CLC_STAT_00_T            	clc_stat_00;              //0xc800527cL
	PE_H15A0_ND2_IPC_CTRL_34_T            	ipc_ctrl_34;              //0xc8005280L
	PE_H15A0_ND2_IPC_CTRL_35_T            	ipc_ctrl_35;              //0xc8005284L
	PE_H15A0_ND2_IPC_CTRL_36_T            	ipc_ctrl_36;              //0xc8005288L
	PE_H15A0_ND2_IPC_CTRL_37_T            	ipc_ctrl_37;              //0xc800528cL
	PE_H15A0_ND2_IPC_CTRL_38_T            	ipc_ctrl_38;              //0xc8005290L
	PE_H15A0_ND2_IPC_CTRL_39_T            	ipc_ctrl_39;              //0xc8005294L
	PE_H15A0_ND2_IPC_CTRL_40_T            	ipc_ctrl_40;              //0xc8005298L
	PE_H15A0_ND2_IPC_CTRL_41_T            	ipc_ctrl_41;              //0xc800529cL
	PE_H15A0_ND2_IPC_CTRL_42_T            	ipc_ctrl_42;              //0xc80052a0L
	PE_H15A0_ND2_IPC_CTRL_43_T            	ipc_ctrl_43;              //0xc80052a4L
	PE_H15A0_ND2_IPC_CTRL_44_T            	ipc_ctrl_44;              //0xc80052a8L
	UINT32                             	  	reserved04[17];           //0xc80052acL~0xc80052ecL
	PE_H15A0_ND2_IPC_CTRL_20_T            	ipc_ctrl_20;              //0xc80052f0L
	PE_H15A0_ND2_IPC_CTRL_21_T            	ipc_ctrl_21;              //0xc80052f4L
	PE_H15A0_ND2_IPC_CTRL_22_T            	ipc_ctrl_22;              //0xc80052f8L
	PE_H15A0_ND2_IPC_CTRL_23_T            	ipc_ctrl_23;              //0xc80052fcL
	PE_H15A0_ND2_IPC_CTRL_24_T            	ipc_ctrl_24;              //0xc8005300L
	PE_H15A0_ND2_IPC_CTRL_25_T            	ipc_ctrl_25;              //0xc8005304L
	PE_H15A0_ND2_IPC_CTRL_26_T            	ipc_ctrl_26;              //0xc8005308L
	PE_H15A0_ND2_IPC_CTRL_27_T            	ipc_ctrl_27;              //0xc800530cL
	PE_H15A0_ND2_IPC_CTRL_28_T            	ipc_ctrl_28;              //0xc8005310L
	PE_H15A0_ND2_IPC_CTRL_29_T            	ipc_ctrl_29;              //0xc8005314L
	PE_H15A0_ND2_IPC_CTRL_30_T            	ipc_ctrl_30;              //0xc8005318L
	PE_H15A0_ND2_IPC_CTRL_31_T            	ipc_ctrl_31;              //0xc800531cL
	PE_H15A0_ND2_IPC_CTRL_32_T            	ipc_ctrl_32;              //0xc8005320L
	PE_H15A0_ND2_IPC_CTRL_33_T            	ipc_ctrl_33;              //0xc8005324L
	PE_H15A0_ND2_IPC_STATUS_04_T          	ipc_status_04;            //0xc8005328L
	PE_H15A0_ND2_IPC_STATUS_05_T          	ipc_status_05;            //0xc800532cL
	PE_H15A0_ND2_IPC_STATUS_06_T          	ipc_status_06;            //0xc8005330L
	PE_H15A0_ND2_IPC_STATUS_07_T          	ipc_status_07;            //0xc8005334L
	PE_H15A0_ND2_IPC_STATUS_08_T          	ipc_status_08;            //0xc8005338L
	PE_H15A0_ND2_IPC_STATUS_09_T          	ipc_status_09;            //0xc800533cL
	PE_H15A0_ND2_TPD_CTRL_00_T            	tpd_ctrl_00;              //0xc8005340L
	PE_H15A0_ND2_TPD_CTRL_01_T            	tpd_ctrl_01;              //0xc8005344L
	PE_H15A0_ND2_TPD_CTRL_02_T            	tpd_ctrl_02;              //0xc8005348L
	PE_H15A0_ND2_TPD_CTRL_03_T            	tpd_ctrl_03;              //0xc800534cL
	PE_H15A0_ND2_TPD_CTRL_04_T            	tpd_ctrl_04;              //0xc8005350L
	PE_H15A0_ND2_TPD_CTRL_05_T            	tpd_ctrl_05;              //0xc8005354L
	PE_H15A0_ND2_TPD_CTRL_06_T            	tpd_ctrl_06;              //0xc8005358L
	PE_H15A0_ND2_TPD_CTRL_07_T            	tpd_ctrl_07;              //0xc800535cL
	PE_H15A0_ND2_TPD_CTRL_08_T            	tpd_ctrl_08;              //0xc8005360L
	PE_H15A0_ND2_TPD_CTRL_09_T            	tpd_ctrl_09;              //0xc8005364L
	PE_H15A0_ND2_TPD_STAT_00_T            	tpd_stat_00;              //0xc8005368L
	PE_H15A0_ND2_CLC_CTRL_07_T            	clc_ctrl_07;              //0xc800536cL
	PE_H15A0_ND2_BBD_CTRL_00_T            	bbd_ctrl_00;              //0xc8005370L
	PE_H15A0_ND2_BBD_CTRL_01_T            	bbd_ctrl_01;              //0xc8005374L
	PE_H15A0_ND2_BBD_CTRL_02_T            	bbd_ctrl_02;              //0xc8005378L
	PE_H15A0_ND2_BBD_STAT_00_T            	bbd_stat_00;              //0xc800537cL
	PE_H15A0_ND2_BBD_STAT_01_T            	bbd_stat_01;              //0xc8005380L
	PE_H15A0_ND2_APL_STAT_00_T            	apl_stat_00;              //0xc8005384L
	UINT32                             	  	reserved05[2];            //0xc8005388L~0xc800538cL
	PE_H15A0_ND2_TPD_HIST_AI_CTRL_T       	tpd_hist_ai_ctrl;         //0xc8005390L
	PE_H15A0_ND2_TPD_HIST_AI_DATA_T       	tpd_hist_ai_data;         //0xc8005394L
	PE_H15A0_ND2_LVCRTL_CTRL_00_T         	lvcrtl_ctrl_00;           //0xc8005398L
	UINT32                             	  	reserved06;               //0xc800539cL
	PE_H15A0_ND2_TNR_STATUS_05_T          	tnr_status_05;            //0xc80053a0L
	PE_H15A0_ND2_TNR_STATUS_06_T          	tnr_status_06;            //0xc80053a4L
	PE_H15A0_ND2_TNR_STATUS_07_T          	tnr_status_07;            //0xc80053a8L
	PE_H15A0_ND2_TNR_STATUS_08_T          	tnr_status_08;            //0xc80053acL
	UINT32                             	  	reserved07[8];            //0xc80053b0L~0xc80053ccL
	PE_H15A0_ND2_TNR_CTRL_40_T            	tnr_ctrl_40;              //0xc80053d0L
	PE_H15A0_ND2_TNR_CTRL_41_T            	tnr_ctrl_41;              //0xc80053d4L
	PE_H15A0_ND2_TNR_CTRL_42_T            	tnr_ctrl_42;              //0xc80053d8L
	PE_H15A0_ND2_TNR_CTRL_43_T            	tnr_ctrl_43;              //0xc80053dcL
	PE_H15A0_ND2_TNR_CTRL_44_T            	tnr_ctrl_44;              //0xc80053e0L
	PE_H15A0_ND2_TNR_CTRL_45_T            	tnr_ctrl_45;              //0xc80053e4L
	PE_H15A0_ND2_TNR_CTRL_46_T            	tnr_ctrl_46;              //0xc80053e8L
	PE_H15A0_ND2_TNR_CTRL_47_T            	tnr_ctrl_47;              //0xc80053ecL
	PE_H15A0_ND2_TNR_CTRL_48_T            	tnr_ctrl_48;              //0xc80053f0L
	PE_H15A0_ND2_TNR_CTRL_49_T            	tnr_ctrl_49;              //0xc80053f4L
	PE_H15A0_ND2_TNR_CTRL_50_T            	tnr_ctrl_50;              //0xc80053f8L
	PE_H15A0_ND2_TNR_CTRL_51_T            	tnr_ctrl_51;              //0xc80053fcL
	PE_H15A0_ND2_TNR_NE_STAT_00_T         	tnr_ne_stat_00;           //0xc8005400L
	PE_H15A0_ND2_TNR_NE_STAT_01_T         	tnr_ne_stat_01;           //0xc8005404L
	PE_H15A0_ND2_TNR_NE_STAT_02_T         	tnr_ne_stat_02;           //0xc8005408L
	PE_H15A0_ND2_TNR_NE_STAT_03_T         	tnr_ne_stat_03;           //0xc800540cL
	PE_H15A0_ND2_TNR_NE_STAT_04_T         	tnr_ne_stat_04;           //0xc8005410L
	PE_H15A0_ND2_TNR_NE_STAT_05_T         	tnr_ne_stat_05;           //0xc8005414L
	PE_H15A0_ND2_TNR_NE_STAT_06_T         	tnr_ne_stat_06;           //0xc8005418L
	PE_H15A0_ND2_TNR_NE_STAT_07_T         	tnr_ne_stat_07;           //0xc800541cL
	PE_H15A0_ND2_TNR_NE_STAT_08_T         	tnr_ne_stat_08;           //0xc8005420L
	PE_H15A0_ND2_TNR_NE_STAT_09_T         	tnr_ne_stat_09;           //0xc8005424L
	PE_H15A0_ND2_TNR_NE_STAT_10_T         	tnr_ne_stat_10;           //0xc8005428L
	PE_H15A0_ND2_TNR_NE_STAT_11_T         	tnr_ne_stat_11;           //0xc800542cL
	PE_H15A0_ND2_TNR_NE_STAT_12_T         	tnr_ne_stat_12;           //0xc8005430L
}PE_ND_ND2_REG_H15A0_T;

typedef struct {
	PE_H15A0_NDS_PE0_LOAD_T               	pe0_load;                 //0xc8006100L
	PE_H15A0_NDS_PE0_INTR_T               	pe0_intr;                 //0xc8006104L
	PE_H15A0_NDS_PE0_STATUS0_T            	pe0_status0;              //0xc8006108L
	PE_H15A0_NDS_PE0_STATUS1_T            	pe0_status1;              //0xc800610cL
	PE_H15A0_NDS_PE0_OPMODE_T             	pe0_opmode;               //0xc8006110L
	PE_H15A0_NDS_PE0_SRC_SIZE_T           	pe0_src_size;             //0xc8006114L
	PE_H15A0_NDS_PE0_FIELD_T              	pe0_field;                //0xc8006118L
	UINT32                             	  	reserved00;               //0xc800611cL
	PE_H15A0_NDS_PE0_IN_OFFSET_T          	pe0_in_offset;            //0xc8006120L
	PE_H15A0_NDS_PE0_IN_SIZE_T            	pe0_in_size;              //0xc8006124L
	PE_H15A0_NDS_FBC_CTRL_T               	fbc_ctrl;                 //0xc8006128L
	UINT32                             	  	reserved01;               //0xc800612cL
	PE_H15A0_NDS_PE0_DEBUG_MEM_REQ_T      	pe0_debug_mem_req;        //0xc8006130L
	PE_H15A0_NDS_PE0_DEBUG_LREQ_DLY_T     	pe0_debug_lreq_dly;       //0xc8006134L
	UINT32                             	  	reserved02[2];            //0xc8006138L~0xc800613cL
	PE_H15A0_NDS_TNR_CTRL_00_T            	tnr_ctrl_00;              //0xc8006140L
	PE_H15A0_NDS_TNR_CTRL_01_T            	tnr_ctrl_01;              //0xc8006144L
	PE_H15A0_NDS_TNR_CTRL_02_T            	tnr_ctrl_02;              //0xc8006148L
	PE_H15A0_NDS_TNR_CTRL_03_T            	tnr_ctrl_03;              //0xc800614cL
	PE_H15A0_NDS_TNR_CTRL_51_T            	tnr_ctrl_51;              //0xc8006150L
	PE_H15A0_NDS_TNR_CTRL_52_T            	tnr_ctrl_52;              //0xc8006154L
	UINT32                             	  	reserved03[2];            //0xc8006158L~0xc800615cL
	PE_H15A0_NDS_TNR_MA_CTRL_00_T         	tnr_ma_ctrl_00;           //0xc8006160L
	PE_H15A0_NDS_TNR_MA_CTRL_01_T         	tnr_ma_ctrl_01;           //0xc8006164L
	PE_H15A0_NDS_TNR_MA_CTRL_02_T         	tnr_ma_ctrl_02;           //0xc8006168L
	PE_H15A0_NDS_TNR_MA_CTRL_03_T         	tnr_ma_ctrl_03;           //0xc800616cL
	PE_H15A0_NDS_TNR_MA_CTRL_04_T         	tnr_ma_ctrl_04;           //0xc8006170L
	PE_H15A0_NDS_TNR_MA_CTRL_05_T         	tnr_ma_ctrl_05;           //0xc8006174L
	PE_H15A0_NDS_TNR_MA_CTRL_06_T         	tnr_ma_ctrl_06;           //0xc8006178L
	PE_H15A0_NDS_TNR_MA_CTRL_07_T         	tnr_ma_ctrl_07;           //0xc800617cL
	PE_H15A0_NDS_TNR_MA_CTRL_08_T         	tnr_ma_ctrl_08;           //0xc8006180L
	PE_H15A0_NDS_TNR_MA_CTRL_09_T         	tnr_ma_ctrl_09;           //0xc8006184L
	PE_H15A0_NDS_TNR_MA_CTRL_10_T         	tnr_ma_ctrl_10;           //0xc8006188L
	PE_H15A0_NDS_TNR_MA_CTRL_11_T         	tnr_ma_ctrl_11;           //0xc800618cL
	PE_H15A0_NDS_TNR_MA_CTRL_12_T         	tnr_ma_ctrl_12;           //0xc8006190L
	PE_H15A0_NDS_TNR_MA_CTRL_13_T         	tnr_ma_ctrl_13;           //0xc8006194L
	UINT32                             	  	reserved04[2];            //0xc8006198L~0xc800619cL
	PE_H15A0_NDS_REG_MA_LUT_CTRL_00_T     	reg_ma_lut_ctrl_00;       //0xc80061a0L
	PE_H15A0_NDS_REG_MA_LUT_CTRL_01_T     	reg_ma_lut_ctrl_01;       //0xc80061a4L
	PE_H15A0_NDS_REG_MA_LUT_CTRL_02_T     	reg_ma_lut_ctrl_02;       //0xc80061a8L
	PE_H15A0_NDS_REG_MA_LUT_CTRL_03_T     	reg_ma_lut_ctrl_03;       //0xc80061acL
	PE_H15A0_NDS_REG_MA_LUT_CTRL_04_T     	reg_ma_lut_ctrl_04;       //0xc80061b0L
	PE_H15A0_NDS_REG_MA_LUT_CTRL_05_T     	reg_ma_lut_ctrl_05;       //0xc80061b4L
	PE_H15A0_NDS_REG_MA_LUT_CTRL_06_T     	reg_ma_lut_ctrl_06;       //0xc80061b8L
	PE_H15A0_NDS_REG_MA_LUT_CTRL_07_T     	reg_ma_lut_ctrl_07;       //0xc80061bcL
	UINT32                             	  	reserved05[2];            //0xc80061c0L~0xc80061c4L
	PE_H15A0_NDS_TNR_STATUS_02_T          	tnr_status_02;            //0xc80061c8L
	UINT32                             	  	reserved06;               //0xc80061ccL
	PE_H15A0_NDS_TNR_LUT_00_T             	tnr_lut_00;               //0xc80061d0L
	PE_H15A0_NDS_TNR_LUT_01_T             	tnr_lut_01;               //0xc80061d4L
	PE_H15A0_NDS_TNR_LUT_02_T             	tnr_lut_02;               //0xc80061d8L
	PE_H15A0_NDS_TNR_STATUS_04_T          	tnr_status_04;            //0xc80061dcL
	UINT32                             	  	reserved07[8];            //0xc80061e0L~0xc80061fcL
	PE_H15A0_NDS_IPC_CTRL_00_T            	ipc_ctrl_00;              //0xc8006200L
	PE_H15A0_NDS_IPC_CTRL_01_T            	ipc_ctrl_01;              //0xc8006204L
	PE_H15A0_NDS_IPC_CTRL_02_T            	ipc_ctrl_02;              //0xc8006208L
	PE_H15A0_NDS_IPC_CTRL_03_T            	ipc_ctrl_03;              //0xc800620cL
	PE_H15A0_NDS_IPC_CTRL_04_T            	ipc_ctrl_04;              //0xc8006210L
	PE_H15A0_NDS_IPC_CTRL_05_T            	ipc_ctrl_05;              //0xc8006214L
	PE_H15A0_NDS_IPC_CTRL_06_T            	ipc_ctrl_06;              //0xc8006218L
	PE_H15A0_NDS_IPC_CTRL_07_T            	ipc_ctrl_07;              //0xc800621cL
	PE_H15A0_NDS_IPC_CTRL_08_T            	ipc_ctrl_08;              //0xc8006220L
	PE_H15A0_NDS_IPC_CTRL_09_T            	ipc_ctrl_09;              //0xc8006224L
	PE_H15A0_NDS_IPC_CTRL_10_T            	ipc_ctrl_10;              //0xc8006228L
	PE_H15A0_NDS_IPC_CTRL_11_T            	ipc_ctrl_11;              //0xc800622cL
	PE_H15A0_NDS_IPC_CTRL_12_T            	ipc_ctrl_12;              //0xc8006230L
	PE_H15A0_NDS_IPC_CTRL_13_T            	ipc_ctrl_13;              //0xc8006234L
	PE_H15A0_NDS_IPC_CTRL_14_T            	ipc_ctrl_14;              //0xc8006238L
	PE_H15A0_NDS_IPC_CTRL_15_T            	ipc_ctrl_15;              //0xc800623cL
	PE_H15A0_NDS_IPC_CTRL_16_T            	ipc_ctrl_16;              //0xc8006240L
	PE_H15A0_NDS_IPC_CTRL_17_T            	ipc_ctrl_17;              //0xc8006244L
	PE_H15A0_NDS_IPC_CTRL_18_T            	ipc_ctrl_18;              //0xc8006248L
	PE_H15A0_NDS_IPC_CTRL_19_T            	ipc_ctrl_19;              //0xc800624cL
	PE_H15A0_NDS_IPC_STATUS_00_T          	ipc_status_00;            //0xc8006250L
	PE_H15A0_NDS_IPC_STATUS_01_T          	ipc_status_01;            //0xc8006254L
	PE_H15A0_NDS_IPC_STATUS_02_T          	ipc_status_02;            //0xc8006258L
	PE_H15A0_NDS_IPC_STATUS_03_T          	ipc_status_03;            //0xc800625cL
	UINT32                             	  	reserved08[8];            //0xc8006260L~0xc800627cL
	PE_H15A0_NDS_IPC_CTRL_34_T            	ipc_ctrl_34;              //0xc8006280L
	PE_H15A0_NDS_IPC_CTRL_35_T            	ipc_ctrl_35;              //0xc8006284L
	PE_H15A0_NDS_IPC_CTRL_36_T            	ipc_ctrl_36;              //0xc8006288L
	PE_H15A0_NDS_IPC_CTRL_37_T            	ipc_ctrl_37;              //0xc800628cL
	PE_H15A0_NDS_IPC_CTRL_38_T            	ipc_ctrl_38;              //0xc8006290L
	PE_H15A0_NDS_IPC_CTRL_39_T            	ipc_ctrl_39;              //0xc8006294L
	PE_H15A0_NDS_IPC_CTRL_40_T            	ipc_ctrl_40;              //0xc8006298L
	PE_H15A0_NDS_IPC_CTRL_41_T            	ipc_ctrl_41;              //0xc800629cL
	PE_H15A0_NDS_IPC_CTRL_42_T            	ipc_ctrl_42;              //0xc80062a0L
	UINT32                             	  	reserved09[19];           //0xc80062a4L~0xc80062ecL
	PE_H15A0_NDS_IPC_CTRL_20_T            	ipc_ctrl_20;              //0xc80062f0L
	PE_H15A0_NDS_IPC_CTRL_21_T            	ipc_ctrl_21;              //0xc80062f4L
	PE_H15A0_NDS_IPC_CTRL_22_T            	ipc_ctrl_22;              //0xc80062f8L
	PE_H15A0_NDS_IPC_CTRL_23_T            	ipc_ctrl_23;              //0xc80062fcL
	PE_H15A0_NDS_IPC_CTRL_24_T            	ipc_ctrl_24;              //0xc8006300L
	PE_H15A0_NDS_IPC_CTRL_25_T            	ipc_ctrl_25;              //0xc8006304L
	PE_H15A0_NDS_IPC_CTRL_26_T            	ipc_ctrl_26;              //0xc8006308L
	PE_H15A0_NDS_IPC_CTRL_27_T            	ipc_ctrl_27;              //0xc800630cL
	PE_H15A0_NDS_IPC_CTRL_28_T            	ipc_ctrl_28;              //0xc8006310L
	PE_H15A0_NDS_IPC_CTRL_29_T            	ipc_ctrl_29;              //0xc8006314L
	PE_H15A0_NDS_IPC_CTRL_30_T            	ipc_ctrl_30;              //0xc8006318L
	PE_H15A0_NDS_IPC_CTRL_31_T            	ipc_ctrl_31;              //0xc800631cL
	PE_H15A0_NDS_IPC_CTRL_32_T            	ipc_ctrl_32;              //0xc8006320L
	PE_H15A0_NDS_IPC_CTRL_33_T            	ipc_ctrl_33;              //0xc8006324L
	PE_H15A0_NDS_IPC_STATUS_04_T          	ipc_status_04;            //0xc8006328L
	PE_H15A0_NDS_IPC_STATUS_05_T          	ipc_status_05;            //0xc800632cL
	PE_H15A0_NDS_IPC_STATUS_06_T          	ipc_status_06;            //0xc8006330L
	PE_H15A0_NDS_IPC_STATUS_07_T          	ipc_status_07;            //0xc8006334L
	PE_H15A0_NDS_IPC_STATUS_08_T          	ipc_status_08;            //0xc8006338L
	PE_H15A0_NDS_IPC_STATUS_09_T          	ipc_status_09;            //0xc800633cL
	UINT32                             	  	reserved10;               //0xc8006340L
	PE_H15A0_NDS_TPD_CTRL_01_T            	tpd_ctrl_01;              //0xc8006344L
	PE_H15A0_NDS_TPD_CTRL_02_T            	tpd_ctrl_02;              //0xc8006348L
	UINT32                             	  	reserved11[14];           //0xc800634cL~0xc8006380L
	PE_H15A0_NDS_APL_STAT_00_T            	apl_stat_00;              //0xc8006384L
	UINT32                             	  	reserved12[4];            //0xc8006388L~0xc8006394L
	PE_H15A0_NDS_LVCRTL_CTRL_00_T         	lvcrtl_ctrl_00;           //0xc8006398L
	UINT32                             	  	reserved13;               //0xc800639cL
	PE_H15A0_NDS_TNR_STATUS_05_T          	tnr_status_05;            //0xc80063a0L
	PE_H15A0_NDS_TNR_STATUS_06_T          	tnr_status_06;            //0xc80063a4L
	PE_H15A0_NDS_TNR_STATUS_07_T          	tnr_status_07;            //0xc80063a8L
	PE_H15A0_NDS_TNR_STATUS_08_T          	tnr_status_08;            //0xc80063acL
}PE_ND_NDS_REG_H15A0_T;

typedef struct {
	PE_H15A0_DNR1_PE0_LOAD_T               	pe0_load;                //0xc8007100L
	PE_H15A0_DNR1_PE0_INTR_T               	pe0_intr;                //0xc8007104L
	PE_H15A0_DNR1_PE0_STATUS0_T            	pe0_status0;             //0xc8007108L
	PE_H15A0_DNR1_PE0_STATUS1_T            	pe0_status1;             //0xc800710cL
	PE_H15A0_DNR1_PE0_OPMODE_T             	pe0_opmode;              //0xc8007110L
	PE_H15A0_DNR1_PE0_IN_SIZE_T            	pe0_in_size;             //0xc8007114L
	PE_H15A0_DNR1_PE0_IN_OFFSET_T          	pe0_in_offset;           //0xc8007118L
	PE_H15A0_DNR1_PE0_OUT_SIZE_T           	pe0_out_size;            //0xc800711cL
	PE_H15A0_DNR1_WIN_CTRL_0_T             	win_ctrl_0;              //0xc8007120L
	PE_H15A0_DNR1_WIN_CTRL_1_T             	win_ctrl_1;              //0xc8007124L
	PE_H15A0_DNR1_WIN_CTRL_2_T             	win_ctrl_2;              //0xc8007128L
	PE_H15A0_DNR1_WIN_CTRL_3_T             	win_ctrl_3;              //0xc800712cL
	PE_H15A0_DNR1_CUP_CTRL_00_T            	cup_ctrl_00;             //0xc8007130L
	PE_H15A0_DNR1_CUP_CTRL_01_T            	cup_ctrl_01;             //0xc8007134L
	PE_H15A0_DNR1_CUP_CTRL_02_T            	cup_ctrl_02;             //0xc8007138L
	UINT32                               	reserved00[5];           //0xc800713cL~0xc800714cL
	PE_H15A0_DNR1_3DFD_CTRL_00_T           	fd3d_ctrl_00;            //0xc8007150L
	PE_H15A0_DNR1_3DFD_CTRL_01_T           	fd3d_ctrl_01;            //0xc8007154L
	PE_H15A0_DNR1_3DFD_CTRL_02_T           	fd3d_ctrl_02;            //0xc8007158L
	PE_H15A0_DNR1_3DFD_CTRL_03_T           	fd3d_ctrl_03;            //0xc800715cL
	PE_H15A0_DNR1_3DFD_STAT_00_T           	fd3d_stat_00;            //0xc8007160L
	PE_H15A0_DNR1_3DFD_STAT_01_T           	fd3d_stat_01;            //0xc8007164L
	PE_H15A0_DNR1_3DFD_STAT_02_T           	fd3d_stat_02;            //0xc8007168L
	PE_H15A0_DNR1_3DFD_STAT_03_T           	fd3d_stat_03;            //0xc800716cL
	PE_H15A0_DNR1_3DFD_STAT_04_T           	fd3d_stat_04;            //0xc8007170L
	PE_H15A0_DNR1_3DFD_STAT_05_T           	fd3d_stat_05;            //0xc8007174L
	PE_H15A0_DNR1_3DFD_STAT_06_T           	fd3d_stat_06;            //0xc8007178L
	PE_H15A0_DNR1_3DFD_STAT_07_T           	fd3d_stat_07;            //0xc800717cL
	PE_H15A0_DNR1_VFILTER_CTRL_0_T         	vfilter_ctrl_0;          //0xc8007180L
	PE_H15A0_DNR1_VFILTER_CTRL_1_T         	vfilter_ctrl_1;          //0xc8007184L
	PE_H15A0_DNR1_VFILTER_CTRL_2_T         	vfilter_ctrl_2;          //0xc8007188L
	PE_H15A0_DNR1_CTI_CTRL_0_T             	cti_ctrl_0;              //0xc800718cL
	PE_H15A0_DNR1_CTI_CTRL_1_T             	cti_ctrl_1;              //0xc8007190L
	PE_H15A0_DNR1_DNR_MAX_CTRL_T           	dnr_max_ctrl;            //0xc8007194L
	PE_H15A0_DNR1_DNR_DBAR_CTRL_T          	dnr_dbar_ctrl;           //0xc8007198L
	PE_H15A0_DNR1_REG_C420_CTRL_T          	reg_c420_ctrl;           //0xc800719cL
	PE_H15A0_DNR1_MNR_CTRL_0_T             	mnr_ctrl_0;              //0xc80071a0L
	PE_H15A0_DNR1_MNR_CTRL_1_T             	mnr_ctrl_1;              //0xc80071a4L
	PE_H15A0_DNR1_MNR_CTRL_2_T             	mnr_ctrl_2;              //0xc80071a8L
	PE_H15A0_DNR1_MNR_CTRL_3_T             	mnr_ctrl_3;              //0xc80071acL
	PE_H15A0_DNR1_MNR_CTRL_4_T             	mnr_ctrl_4;              //0xc80071b0L
	PE_H15A0_DNR1_MNR_CTRL_5_T             	mnr_ctrl_5;              //0xc80071b4L
	PE_H15A0_DNR1_DETAIL_CTRL_T            	detail_ctrl;             //0xc80071b8L
	PE_H15A0_DNR1_DC_BNR_CTRL_0_T          	dc_bnr_ctrl_0;           //0xc80071bcL
	PE_H15A0_DNR1_DC_BNR_CTRL_1_T          	dc_bnr_ctrl_1;           //0xc80071c0L
	PE_H15A0_DNR1_DC_BNR_CTRL_2_T          	dc_bnr_ctrl_2;           //0xc80071c4L
	PE_H15A0_DNR1_DC_BNR_CTRL_3_T          	dc_bnr_ctrl_3;           //0xc80071c8L
	PE_H15A0_DNR1_DC_BNR_CTRL_4_T          	dc_bnr_ctrl_4;           //0xc80071ccL
	PE_H15A0_DNR1_DC_BNR_CTRL_5_T          	dc_bnr_ctrl_5;           //0xc80071d0L
	PE_H15A0_DNR1_AC_BNR_CTRL_0_T          	ac_bnr_ctrl_0;           //0xc80071d4L
	PE_H15A0_DNR1_AC_BNR_CTRL_1_T          	ac_bnr_ctrl_1;           //0xc80071d8L
	PE_H15A0_DNR1_AC_BNR_CTRL_2_T          	ac_bnr_ctrl_2;           //0xc80071dcL
	PE_H15A0_DNR1_DNR_STAT_0_T             	dnr_stat_0;              //0xc80071e0L
	PE_H15A0_DNR1_AC_BNR_CTRL_3_T          	ac_bnr_ctrl_3;           //0xc80071e4L
	PE_H15A0_DNR1_AC_BNR_CTRL_4_T          	ac_bnr_ctrl_4;           //0xc80071e8L
	PE_H15A0_DNR1_DNR_STAT_1_T             	dnr_stat_1;              //0xc80071ecL
	PE_H15A0_DNR1_AC_BNR_CTRL_5_T          	ac_bnr_ctrl_5;           //0xc80071f0L
	PE_H15A0_DNR1_AC_BNR_CTRL_6_T          	ac_bnr_ctrl_6;           //0xc80071f4L
	PE_H15A0_DNR1_AC_BNR_CTRL_7_T          	ac_bnr_ctrl_7;           //0xc80071f8L
	PE_H15A0_DNR1_DNR_STAT_2_T             	dnr_stat_2;              //0xc80071fcL
	PE_H15A0_DNR1_AC_BNR_CTRL_8_T          	ac_bnr_ctrl_8;           //0xc8007200L
	PE_H15A0_DNR1_AC_BNR_CTRL_9_T          	ac_bnr_ctrl_9;           //0xc8007204L
	PE_H15A0_DNR1_AC_BNR_CTRL_10_T         	ac_bnr_ctrl_10;          //0xc8007208L
	PE_H15A0_DNR1_AC_BNR_CTRL_11_T         	ac_bnr_ctrl_11;          //0xc800720cL
	PE_H15A0_DNR1_AC_BNR_CTRL_12_T         	ac_bnr_ctrl_12;          //0xc8007210L
	PE_H15A0_DNR1_IFC_CTRL_0_T             	ifc_ctrl_0;              //0xc8007214L
	PE_H15A0_DNR1_IFC_CTRL_1_T             	ifc_ctrl_1;              //0xc8007218L
	PE_H15A0_DNR1_IFC_CTRL_2_T             	ifc_ctrl_2;              //0xc800721cL
	PE_H15A0_DNR1_IFC_CTRL_3_T             	ifc_ctrl_3;              //0xc8007220L
	PE_H15A0_DNR1_IFC_CTRL_4_T             	ifc_ctrl_4;              //0xc8007224L
	PE_H15A0_DNR1_IFC_CTRL_5_T             	ifc_ctrl_5;              //0xc8007228L
	PE_H15A0_DNR1_IFC_CTRL_6_T             	ifc_ctrl_6;              //0xc800722cL
	PE_H15A0_DNR1_IFC_CTRL_7_T             	ifc_ctrl_7;              //0xc8007230L
	PE_H15A0_DNR1_IFC_CTRL_8_T             	ifc_ctrl_8;              //0xc8007234L
	PE_H15A0_DNR1_IFC_CTRL_9_T             	ifc_ctrl_9;              //0xc8007238L
	PE_H15A0_DNR1_IFC_CTRL_10_T            	ifc_ctrl_10;             //0xc800723cL
	PE_H15A0_DNR1_IFC_CTRL_11_T            	ifc_ctrl_11;             //0xc8007240L
	PE_H15A0_DNR1_IFC_CTRL_12_T            	ifc_ctrl_12;             //0xc8007244L
	PE_H15A0_DNR1_IFC_CTRL_13_T            	ifc_ctrl_13;             //0xc8007248L
	PE_H15A0_DNR1_IFC_CTRL_14_T            	ifc_ctrl_14;             //0xc800724cL
	PE_H15A0_DNR1_IFC_CTRL_15_T            	ifc_ctrl_15;             //0xc8007250L
	PE_H15A0_DNR1_IFC_CTRL_16_T            	ifc_ctrl_16;             //0xc8007254L
	PE_H15A0_DNR1_IFC_CTRL_17_T            	ifc_ctrl_17;             //0xc8007258L
	PE_H15A0_DNR1_IFC_CTRL_18_T            	ifc_ctrl_18;             //0xc800725cL
	PE_H15A0_DNR1_IFC_CTRL_19_T            	ifc_ctrl_19;             //0xc8007260L
	PE_H15A0_DNR1_IFC_CTRL_20_T            	ifc_ctrl_20;             //0xc8007264L
	PE_H15A0_DNR1_IFC_CTRL_21_T            	ifc_ctrl_21;             //0xc8007268L
	PE_H15A0_DNR1_SC_BNR_CTRL_0_T          	sc_bnr_ctrl_0;           //0xc800726cL
	PE_H15A0_DNR1_SC_BNR_CTRL_1_T          	sc_bnr_ctrl_1;           //0xc8007270L
	PE_H15A0_DNR1_SC_BNR_CTRL_2_T          	sc_bnr_ctrl_2;           //0xc8007274L
	PE_H15A0_DNR1_SC_BNR_CTRL_3_T          	sc_bnr_ctrl_3;           //0xc8007278L
	PE_H15A0_DNR1_SC_BNR_CTRL_4_T          	sc_bnr_ctrl_4;           //0xc800727cL
	PE_H15A0_DNR1_SC_BNR_CTRL_5_T          	sc_bnr_ctrl_5;           //0xc8007280L
	PE_H15A0_DNR1_SC_BNR_CTRL_6_T          	sc_bnr_ctrl_6;           //0xc8007284L
	PE_H15A0_DNR1_C_PRE_BLUR_CTRL_0_T      	c_pre_blur_ctrl_0;       //0xc8007288L
	PE_H15A0_DNR1_TEXTURE_CTRL_0_T         	texture_ctrl_0;          //0xc800728cL
	PE_H15A0_DNR1_TEXTURE_CTRL_1_T         	texture_ctrl_1;          //0xc8007290L
	PE_H15A0_DNR1_TEXTURE_CTRL_2_T         	texture_ctrl_2;          //0xc8007294L
	PE_H15A0_DNR1_TEXTURE_CTRL_3_T         	texture_ctrl_3;          //0xc8007298L
	UINT32                               	reserved01[13];          //0xc800729cL~0xc80072ccL
	PE_H15A0_DNR1_DNR_STAT_3_T             	dnr_stat_3;              //0xc80072d0L
	PE_H15A0_DNR1_DNR_STAT_4_T             	dnr_stat_4;              //0xc80072d4L
	PE_H15A0_DNR1_DNR_STAT_5_T             	dnr_stat_5;              //0xc80072d8L
	PE_H15A0_DNR1_DNR_STAT_6_T             	dnr_stat_6;              //0xc80072dcL
	PE_H15A0_DNR1_DNR_STAT_7_T             	dnr_stat_7;              //0xc80072e0L
	PE_H15A0_DNR1_DNR_STAT_8_T             	dnr_stat_8;              //0xc80072e4L
	PE_H15A0_DNR1_DNR_STAT_9_T             	dnr_stat_9;              //0xc80072e8L
	PE_H15A0_DNR1_DNR_STAT_10_T            	dnr_stat_10;             //0xc80072ecL
}PE_ND_DNR1_REG_H15A0_T    ;

typedef struct {
	PE_H15A0_DNR2_PE0_LOAD_T              	pe0_load;                //0xc8007300L
	PE_H15A0_DNR2_PE0_INTR_T              	pe0_intr;                //0xc8007304L
	PE_H15A0_DNR2_PE0_STATUS0_T           	pe0_status0;             //0xc8007308L
	PE_H15A0_DNR2_PE0_STATUS1_T           	pe0_status1;             //0xc800730cL
	PE_H15A0_DNR2_PE0_OPMODE_T            	pe0_opmode;              //0xc8007310L
	PE_H15A0_DNR2_PE0_IN_SIZE_T           	pe0_in_size;             //0xc8007314L
	PE_H15A0_DNR2_PE0_IN_OFFSET_T         	pe0_in_offset;           //0xc8007318L
	PE_H15A0_DNR2_PE0_OUT_SIZE_T          	pe0_out_size;            //0xc800731cL
	PE_H15A0_DNR2_WIN_CTRL_0_T            	win_ctrl_0;              //0xc8007320L
	PE_H15A0_DNR2_WIN_CTRL_1_T            	win_ctrl_1;              //0xc8007324L
	PE_H15A0_DNR2_WIN_CTRL_2_T            	win_ctrl_2;              //0xc8007328L
	PE_H15A0_DNR2_WIN_CTRL_3_T            	win_ctrl_3;              //0xc800732cL
	PE_H15A0_DNR2_CUP_CTRL_00_T           	cup_ctrl_00;             //0xc8007330L
	PE_H15A0_DNR2_CUP_CTRL_01_T           	cup_ctrl_01;             //0xc8007334L
	PE_H15A0_DNR2_CUP_CTRL_02_T           	cup_ctrl_02;             //0xc8007338L
	UINT32                             	    reserved00[5];           //0xc800733cL~0xc800734cL
	PE_H15A0_DNR2_3DFD_CTRL_00_T          	fd3d_ctrl_00;            //0xc8007350L
	PE_H15A0_DNR2_3DFD_CTRL_01_T          	fd3d_ctrl_01;            //0xc8007354L
	PE_H15A0_DNR2_3DFD_CTRL_02_T          	fd3d_ctrl_02;            //0xc8007358L
	PE_H15A0_DNR2_3DFD_CTRL_03_T          	fd3d_ctrl_03;            //0xc800735cL
	PE_H15A0_DNR2_3DFD_STAT_00_T          	fd3d_stat_00;            //0xc8007360L
	PE_H15A0_DNR2_3DFD_STAT_01_T          	fd3d_stat_01;            //0xc8007364L
	PE_H15A0_DNR2_3DFD_STAT_02_T          	fd3d_stat_02;            //0xc8007368L
	PE_H15A0_DNR2_3DFD_STAT_03_T          	fd3d_stat_03;            //0xc800736cL
	PE_H15A0_DNR2_3DFD_STAT_04_T          	fd3d_stat_04;            //0xc8007370L
	PE_H15A0_DNR2_3DFD_STAT_05_T          	fd3d_stat_05;            //0xc8007374L
	PE_H15A0_DNR2_3DFD_STAT_06_T          	fd3d_stat_06;            //0xc8007378L
	PE_H15A0_DNR2_3DFD_STAT_07_T          	fd3d_stat_07;            //0xc800737cL
	PE_H15A0_DNR2_VFILTER_CTRL_0_T        	vfilter_ctrl_0;          //0xc8007380L
	PE_H15A0_DNR2_VFILTER_CTRL_1_T        	vfilter_ctrl_1;          //0xc8007384L
	PE_H15A0_DNR2_VFILTER_CTRL_2_T        	vfilter_ctrl_2;          //0xc8007388L
	PE_H15A0_DNR2_CTI_CTRL_0_T            	cti_ctrl_0;              //0xc800738cL
	PE_H15A0_DNR2_CTI_CTRL_1_T            	cti_ctrl_1;              //0xc8007390L
	PE_H15A0_DNR2_DNR_MAX_CTRL_T          	dnr_max_ctrl;            //0xc8007394L
	PE_H15A0_DNR2_DNR_DBAR_CTRL_T         	dnr_dbar_ctrl;           //0xc8007398L
	PE_H15A0_DNR2_REG_C420_CTRL_T         	reg_c420_ctrl;           //0xc800739cL
	PE_H15A0_DNR2_MNR_CTRL_0_T            	mnr_ctrl_0;              //0xc80073a0L
	PE_H15A0_DNR2_MNR_CTRL_1_T            	mnr_ctrl_1;              //0xc80073a4L
	PE_H15A0_DNR2_MNR_CTRL_2_T            	mnr_ctrl_2;              //0xc80073a8L
	PE_H15A0_DNR2_MNR_CTRL_3_T            	mnr_ctrl_3;              //0xc80073acL
	PE_H15A0_DNR2_MNR_CTRL_4_T            	mnr_ctrl_4;              //0xc80073b0L
	PE_H15A0_DNR2_MNR_CTRL_5_T            	mnr_ctrl_5;              //0xc80073b4L
	PE_H15A0_DNR2_DETAIL_CTRL_T           	detail_ctrl;             //0xc80073b8L
	PE_H15A0_DNR2_DC_BNR_CTRL_0_T         	dc_bnr_ctrl_0;           //0xc80073bcL
	PE_H15A0_DNR2_DC_BNR_CTRL_1_T         	dc_bnr_ctrl_1;           //0xc80073c0L
	PE_H15A0_DNR2_DC_BNR_CTRL_2_T         	dc_bnr_ctrl_2;           //0xc80073c4L
	PE_H15A0_DNR2_DC_BNR_CTRL_3_T         	dc_bnr_ctrl_3;           //0xc80073c8L
	PE_H15A0_DNR2_DC_BNR_CTRL_4_T         	dc_bnr_ctrl_4;           //0xc80073ccL
	PE_H15A0_DNR2_DC_BNR_CTRL_5_T         	dc_bnr_ctrl_5;           //0xc80073d0L
	PE_H15A0_DNR2_AC_BNR_CTRL_0_T         	ac_bnr_ctrl_0;           //0xc80073d4L
	PE_H15A0_DNR2_AC_BNR_CTRL_1_T         	ac_bnr_ctrl_1;           //0xc80073d8L
	PE_H15A0_DNR2_AC_BNR_CTRL_2_T         	ac_bnr_ctrl_2;           //0xc80073dcL
	PE_H15A0_DNR2_DNR_STAT_0_T            	dnr_stat_0;              //0xc80073e0L
	PE_H15A0_DNR2_AC_BNR_CTRL_3_T         	ac_bnr_ctrl_3;           //0xc80073e4L
	PE_H15A0_DNR2_AC_BNR_CTRL_4_T         	ac_bnr_ctrl_4;           //0xc80073e8L
	PE_H15A0_DNR2_DNR_STAT_1_T            	dnr_stat_1;              //0xc80073ecL
	PE_H15A0_DNR2_AC_BNR_CTRL_5_T         	ac_bnr_ctrl_5;           //0xc80073f0L
	PE_H15A0_DNR2_AC_BNR_CTRL_6_T         	ac_bnr_ctrl_6;           //0xc80073f4L
	PE_H15A0_DNR2_AC_BNR_CTRL_7_T         	ac_bnr_ctrl_7;           //0xc80073f8L
	PE_H15A0_DNR2_DNR_STAT_2_T            	dnr_stat_2;              //0xc80073fcL
	PE_H15A0_DNR2_AC_BNR_CTRL_8_T         	ac_bnr_ctrl_8;           //0xc8007400L
	PE_H15A0_DNR2_AC_BNR_CTRL_9_T         	ac_bnr_ctrl_9;           //0xc8007404L
	PE_H15A0_DNR2_AC_BNR_CTRL_10_T        	ac_bnr_ctrl_10;          //0xc8007408L
	PE_H15A0_DNR2_AC_BNR_CTRL_11_T        	ac_bnr_ctrl_11;          //0xc800740cL
	PE_H15A0_DNR2_AC_BNR_CTRL_12_T        	ac_bnr_ctrl_12;          //0xc8007410L
	PE_H15A0_DNR2_IFC_CTRL_0_T            	ifc_ctrl_0;              //0xc8007414L
	PE_H15A0_DNR2_IFC_CTRL_1_T            	ifc_ctrl_1;              //0xc8007418L
	PE_H15A0_DNR2_IFC_CTRL_2_T            	ifc_ctrl_2;              //0xc800741cL
	PE_H15A0_DNR2_IFC_CTRL_3_T            	ifc_ctrl_3;              //0xc8007420L
	PE_H15A0_DNR2_IFC_CTRL_4_T            	ifc_ctrl_4;              //0xc8007424L
	PE_H15A0_DNR2_IFC_CTRL_5_T            	ifc_ctrl_5;              //0xc8007428L
	PE_H15A0_DNR2_IFC_CTRL_6_T            	ifc_ctrl_6;              //0xc800742cL
	PE_H15A0_DNR2_IFC_CTRL_7_T            	ifc_ctrl_7;              //0xc8007430L
	PE_H15A0_DNR2_IFC_CTRL_8_T            	ifc_ctrl_8;              //0xc8007434L
	PE_H15A0_DNR2_IFC_CTRL_9_T            	ifc_ctrl_9;              //0xc8007438L
	PE_H15A0_DNR2_IFC_CTRL_10_T           	ifc_ctrl_10;             //0xc800743cL
	PE_H15A0_DNR2_IFC_CTRL_11_T           	ifc_ctrl_11;             //0xc8007440L
	PE_H15A0_DNR2_IFC_CTRL_12_T           	ifc_ctrl_12;             //0xc8007444L
	PE_H15A0_DNR2_IFC_CTRL_13_T           	ifc_ctrl_13;             //0xc8007448L
	PE_H15A0_DNR2_IFC_CTRL_14_T           	ifc_ctrl_14;             //0xc800744cL
	PE_H15A0_DNR2_IFC_CTRL_15_T           	ifc_ctrl_15;             //0xc8007450L
	PE_H15A0_DNR2_IFC_CTRL_16_T           	ifc_ctrl_16;             //0xc8007454L
	PE_H15A0_DNR2_IFC_CTRL_17_T           	ifc_ctrl_17;             //0xc8007458L
	PE_H15A0_DNR2_IFC_CTRL_18_T           	ifc_ctrl_18;             //0xc800745cL
	PE_H15A0_DNR2_IFC_CTRL_19_T           	ifc_ctrl_19;             //0xc8007460L
	PE_H15A0_DNR2_IFC_CTRL_20_T           	ifc_ctrl_20;             //0xc8007464L
	PE_H15A0_DNR2_IFC_CTRL_21_T           	ifc_ctrl_21;             //0xc8007468L
	PE_H15A0_DNR2_SC_BNR_CTRL_0_T         	sc_bnr_ctrl_0;           //0xc800746cL
	PE_H15A0_DNR2_SC_BNR_CTRL_1_T         	sc_bnr_ctrl_1;           //0xc8007470L
	PE_H15A0_DNR2_SC_BNR_CTRL_2_T         	sc_bnr_ctrl_2;           //0xc8007474L
	PE_H15A0_DNR2_SC_BNR_CTRL_3_T         	sc_bnr_ctrl_3;           //0xc8007478L
	PE_H15A0_DNR2_SC_BNR_CTRL_4_T         	sc_bnr_ctrl_4;           //0xc800747cL
	PE_H15A0_DNR2_SC_BNR_CTRL_5_T         	sc_bnr_ctrl_5;           //0xc8007480L
	PE_H15A0_DNR2_SC_BNR_CTRL_6_T         	sc_bnr_ctrl_6;           //0xc8007484L
	PE_H15A0_DNR2_C_PRE_BLUR_CTRL_0_T     	c_pre_blur_ctrl_0;       //0xc8007488L
	PE_H15A0_DNR2_TEXTURE_CTRL_0_T        	texture_ctrl_0;          //0xc800748cL
	PE_H15A0_DNR2_TEXTURE_CTRL_1_T        	texture_ctrl_1;          //0xc8007490L
	PE_H15A0_DNR2_TEXTURE_CTRL_2_T        	texture_ctrl_2;          //0xc8007494L
	PE_H15A0_DNR2_TEXTURE_CTRL_3_T        	texture_ctrl_3;          //0xc8007498L
	UINT32                             	    reserved01[13];          //0xc800749cL~0xc80074ccL
	PE_H15A0_DNR2_DNR_STAT_3_T            	dnr_stat_3;              //0xc80074d0L
	PE_H15A0_DNR2_DNR_STAT_4_T            	dnr_stat_4;              //0xc80074d4L
	PE_H15A0_DNR2_DNR_STAT_5_T            	dnr_stat_5;              //0xc80074d8L
	PE_H15A0_DNR2_DNR_STAT_6_T            	dnr_stat_6;              //0xc80074dcL
	PE_H15A0_DNR2_DNR_STAT_7_T            	dnr_stat_7;              //0xc80074e0L
	PE_H15A0_DNR2_DNR_STAT_8_T            	dnr_stat_8;              //0xc80074e4L
	PE_H15A0_DNR2_DNR_STAT_9_T            	dnr_stat_9;              //0xc80074e8L
	PE_H15A0_DNR2_DNR_STAT_10_T           	dnr_stat_10;             //0xc80074ecL
}PE_ND_DNR2_REG_H15A0_T;
#endif
