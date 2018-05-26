const PE_REG_PARAM_T shp_l_av_pal_default_h13b0[] =
{
	/* scaler : min-max clipping(V)*/
	{0x1180, 0x00000281},
	/* cva cti */
	{0x0184, 0x30000030},
	/* p0l cti */
	{0x0584, 0x00001810},
	{0x05E4, 0x00282001},
	/* pe1 shp, 0x53D0~0x54CC,0x54F0~0x5550,0x5560~0x559C */
	{0x53D0, 0x07030109}, // PE1_DERH_CTRL_00
                          // [ 1: 0]=0x1(1) : vsum_mode
                          // [ 3: 2]=0x2(2) : hsum_mode
                          // [    4]=0x0(0) : vsum121_en
                          // [ 9: 8]=0x1(1) : vertical_min_max
                          // [17:16]=0x3(3) : a_flt_mux
                          // [19:18]=0x0(0) : a_e_mux
                          // [   20]=0x0(0) : t_mux
                          // [27:24]=0xa(10) : a_gen_width
	{0x53D4, 0x00004100}, // PE1_DERH_CTRL_01
                          // [ 7: 0]=0x0(0) : a_gen_of_Y
                          // [15: 8]=0x41(65) : a_gen_of_C
                          // [18:16]=0x0(0) : avg_flt_mode
                          // [   19]=0x0(0) : avg_flt_en
                          // [26:24]=0x0(0) : max_flt_mode
	{0x53D8, 0x0801F808}, // PE1_DERH_CTRL_02
                          // [ 3: 0]=0x8(8) : A_exp_gain_ctrl_y0
                          // [ 7: 4]=0x0(0) : A_exp2_gain_ctrl_x0
                          // [11: 8]=0x8(8) : A_exp2_gain_ctrl_y1
                          // [15:12]=0xf(15) : A_exp2_gain_ctrl_x1
                          // [19:16]=0x1(1) : A_avg2_flt_wid
                          // [   20]=0x0(0) : A_avg2_flt_en
                          // [   24]=0x0(0) : A_exp_max2_en
                          // [27:26]=0x2(2) : y_sum_mode
	{0x53DC, 0xC80C0000}, // PE1_DERH_CTRL_03
                          // [ 7: 5]=0x0(0) : A_mux_for_EDGE_GainTable
                          // [19:17]=0x6(6) : A_mux_for_DETAIL_filter
                          // [23:21]=0x0(0) : A_mux_for_EDGE_filter
                          // [27:24]=0x8(8) : A_scaling_FLAT_filter_
                          // [31:29]=0x6(6) : A_mux_for_FLAT_filter
	{0x53E0, 0x0000FF01}, // PE1_DERH_CTRL_04
                          // [    1]=0x0(0) : reg_dbg_scale
                          // [ 7: 4]=0x0(0) : sum_mux
                          // [10: 8]=0x7(7) : reg_enh_en_cc
                          // [14:12]=0x7(7) : reg_enh_en_yy
	{0x53E4, 0x1E100001}, // PE1_DERH_CTRL_05
                          // [ 1: 0]=0x1(1) : reg_vmm_param
                          // [13: 8]=0x0(0) : reg_csft_gain
                          // [21:16]=0x10(16) : reg_th_gain_Edge
                          // [23:22]=0x0(0) : reg_th_gain_Flat
                          // [30:24]=0x1e(30) : reg_th_manual_th
                          // [   31]=0x0(0) : reg_th_manual_en
	{0x53E8, 0x85040300}, // PE1_DERH_CTRL_06
                          // [13: 8]=0x4(4) : edge_filter_white_gain
                          // [21:16]=0x6(6) : edge_filter_black_gain
                          // [   24]=0x1(1) : reg_amean_en
                          // [27:26]=0x1(1) : edge_filter_V_tap
                          // [   30]=0x0(0) : edge_C_filter_en
                          // [   31]=0x1(1) : edge_Y_filter_en
	{0x53EC, 0xE0000000}, // PE1_DERH_CTRL_07
                          // [ 1: 0]=0x0(0) : avg_filter_tap
                          // [29:24]=0x20(32) : flat_filter_gain
                          // [   30]=0x1(1) : flat_filter_type
                          // [   31]=0x1(1) : flat_filter_en
	{0x53F0, 0x30FF1010}, // PE1_DERH_CTRL_08
                          // [ 7: 0]=0x10(16) : amod_ctrl0_y0
                          // [15: 8]=0x10(16) : amod_ctrl0_x0
                          // [23:16]=0xff(255) : amod_ctrl0_y1
                          // [31:24]=0x30(48) : amod_ctrl0_x1
	{0x53F4, 0x00004020}, // PE1_DERH_CTRL_09
                          // [ 7: 0]=0x20(32) : amod_ctrl1_x0
                          // [15: 8]=0x40(64) : amod_ctrl1_x1
                          // [18:16]=0x0(0) : Y_C_Mux_control
                          // [23:19]=0x0(0) : Chroma_Weight
	{0x5400, 0x0006500B}, // PE1_DERV_CTRL_0
                          // [    0]=0x1(1) : der_v_en
                          // [ 2: 1]=0x1(1) : der_gain_mapping
                          // [    3]=0x1(1) : bif_en
                          // [ 7: 4]=0x0(0) : output_mux
                          // [15: 8]=0x50(80) : bif_manual_th
                          // [21:16]=0x6(6) : th_gain
                          // [   24]=0x0(0) : th_mode
	{0x5404, 0x00000010}, // PE1_DERV_CTRL_1
                          // [ 5: 0]=0x10(16) : csft_gain
                          // [    8]=0x0(0) : csft_mode
	{0x5408, 0x40200010}, // PE1_DERV_CTRL_2
                          // [ 2: 0]=0x0(0) : mmd_sel
                          // [ 6: 4]=0x1(1) : max_sel
                          // [ 9: 8]=0x0(0) : avg_sel
                          // [23:16]=0x20(32) : gain_th1
                          // [31:24]=0x40(64) : gain_th2
	{0x540C, 0x00002020}, // PE1_DERV_CTRL_3
                          // [ 6: 0]=0x20(32) : gain_b
                          // [14: 8]=0x20(32) : gain_w
	{0x5410, 0x00000000}, // PE1_DP_CTRL_00
                          // [ 2: 0]=0x0(0) : debug_display
	{0x5414, 0x20200810}, // PE1_DP_CTRL_01
                          // [ 6: 0]=0x10(16) : edge_gain_b
                          // [14: 8]=0x8(8) : edge_gain_w
                          // [22:16]=0x20(32) : texture_gain_b
                          // [30:24]=0x20(32) : texture_gain_w
	{0x5470, 0x06101000}, // PE1_DERH_CTRL_0A
                          // [    0]=0x0(0) : a2g_mode_e
                          // [    1]=0x0(0) : a2g_mode_f
                          // [13: 8]=0x10(16) : a2g_mgain_e
                          // [21:16]=0x10(16) : a2g_mgain_f
                          // [27:24]=0x6(6) : debug_mode
	{0x5474, 0x08040000}, // PE1_DERH_CTRL_0B
                          // [ 7: 0]=0x0(0) : e_gain_th1
                          // [15: 8]=0x0(0) : e_gain_th2
                          // [23:16]=0x4(4) : f_gain_th1
                          // [31:24]=0x8(8) : f_gain_th2
	{0x5478, 0x00002018}, // PE1_DERH_CTRL_0C
                          // [ 5: 0]=0x18(24) : e_gain_max
                          // [13: 8]=0x20(32) : f_gain_max
	{0x547C, 0x00000000}, // PE1_DERH_CTRL_0D
	{0x5490, 0x00000001}, // PE1_SP_CTRL_00
                          // [    0]=0x1(1) : edge_enhance_enable
                          // [13:12]=0x0(0) : Edge_operator_selection
	{0x5494, 0x08282020}, // PE1_SP_CTRL_01
                          // [ 6: 0]=0x20(32) : white_gain
                          // [14: 8]=0x20(32) : black_gain
                          // [23:16]=0x28(40) : Horizontal_gain
                          // [31:24]=0x8(8) : Vertical_gain
	{0x5498, 0x20013020}, // PE1_SP_CTRL_02
                          // [ 7: 0]=0x20(32) : sobel_weight
                          // [15: 8]=0x30(48) : laplacian_weight
                          // [   16]=0x1(1) : sobel_manual_mode_en
                          // [31:24]=0x20(32) : sobel_manual_gain
	{0x549C, 0x00000000}, // PE1_SP_CTRL_03
	{0x54A0, 0x01401000}, // PE1_SP_CTRL_04
                          // [11: 8]=0x0(0) : display_mode
                          // [   12]=0x1(1) : gx_weight_manual_mode
                          // [23:16]=0x40(64) : gx_weight_manual_gain
	{0x54A8, 0x00000000}, // PE1_SP_CTRL_05
	{0x54AC, 0x00001011}, // PE1_SP_CTRL_06
                          // [ 1: 0]=0x1(1) : lap_h_mode
                          // [    4]=0x1(1) : lap_v_mode
	{0x54B0, 0x00281001}, // PE1_SP_CTRL_07
                          // [    0]=0x1(1) : gb_en
                          // [    4]=0x0(0) : gb_mode
                          // [15: 8]=0x10(16) : gb_x1
                          // [23:16]=0x28(40) : gb_y1
	{0x54B4, 0x00B04020}, // PE1_SP_CTRL_08
                          // [ 7: 0]=0x20(32) : gb_x2
                          // [15: 8]=0x40(64) : gb_y2
                          // [23:16]=0xb0(176) : gb_y3
	{0x54B8, 0xD8D06040}, // PE1_SP_CTRL_09
                          // [ 7: 0]=0x40(64) : lum1_x_L0
                          // [15: 8]=0x60(96) : lum1_x_L1
                          // [23:16]=0xd0(208) : lum1_x_H0
                          // [31:24]=0xd8(216) : lum1_x_H1
	{0x54BC, 0x38FFFFFF}, // PE1_SP_CTRL_0A
                          // [ 7: 0]=0xff(255) : lum1_y0
                          // [15: 8]=0xff(255) : lum1_y1
                          // [23:16]=0xff(255) : lum1_y2
                          // [31:24]=0x38(56) : lum2_x_L0
	{0x54C0, 0xFFD8D048}, // PE1_SP_CTRL_0B
                          // [ 7: 0]=0x48(72) : lum2_x_L1
                          // [15: 8]=0xd0(208) : lum2_x_H0
                          // [23:16]=0xd8(216) : lum2_x_H1
                          // [31:24]=0xff(255) : lum2_y0
	{0x54C4, 0x0000FFFF}, // PE1_SP_CTRL_0C
                          // [ 7: 0]=0xff(255) : lum2_y1
                          // [15: 8]=0xff(255) : lum2_y2
	{0x54F0, 0x00000001}, // PE1_MP_CTRL_00
                          // [    0]=0x1(1) : edge_enhance_enable
                          // [13:12]=0x0(0) : Edge_operator_selection
	{0x54F4, 0x08282020}, // PE1_MP_CTRL_01
                          // [ 6: 0]=0x20(32) : white_gain
                          // [14: 8]=0x20(32) : black_gain
                          // [23:16]=0x28(40) : horizontal_gain
                          // [31:24]=0x8(8) : vertical_gain
	{0x54F8, 0x20013824}, // PE1_MP_CTRL_02
                          // [ 7: 0]=0x24(36) : sobel_weight
                          // [15: 8]=0x38(56) : laplacian_weight
                          // [   16]=0x1(1) : sobel_manual_mode_en
                          // [31:24]=0x20(32) : sobel_manual_gain
	{0x54FC, 0x00000000}, // PE1_MP_CTRL_03
	{0x5500, 0x01401000}, // PE1_MP_CTRL_04
                          // [11: 8]=0x0(0) : display_mode
                          // [   12]=0x1(1) : gx_weight_manual_mode
                          // [23:16]=0x40(64) : gx_weight_manual_gain
	{0x5508, 0x00000000}, // PE1_MP_CTRL_05
	{0x550C, 0x00001011}, // PE1_MP_CTRL_06
                          // [ 2: 0]=0x1(1) : lap_h_mode
                          // [ 6: 4]=0x1(1) : lap_v_mode
	{0x5510, 0x00281001}, // PE1_MP_CTRL_07
                          // [    0]=0x1(1) : gb_en
                          // [    4]=0x0(0) : gb_mode
                          // [15: 8]=0x10(16) : gb_x1
                          // [23:16]=0x28(40) : gb_y1
	{0x5514, 0x00B04020}, // PE1_MP_CTRL_08
                          // [ 7: 0]=0x20(32) : gb_x2
                          // [15: 8]=0x40(64) : gb_y2
                          // [23:16]=0xb0(176) : gb_y3
	{0x5518, 0xD8D06040}, // PE1_MP_CTRL_09
                          // [ 7: 0]=0x40(64) : lum1_x_L0
                          // [15: 8]=0x60(96) : lum1_x_L1
                          // [23:16]=0xd0(208) : lum1_x_H0
                          // [31:24]=0xd8(216) : lum1_x_H1
	{0x551C, 0x38FFFFFF}, // PE1_MP_CTRL_0A
                          // [ 7: 0]=0xff(255) : lum1_y0
                          // [15: 8]=0xff(255) : lum1_y1
                          // [23:16]=0xff(255) : lum1_y2
                          // [31:24]=0x38(56) : lum2_x_L0
	{0x5520, 0xFFD8D049}, // PE1_MP_CTRL_0B
                          // [ 7: 0]=0x49(73) : lum2_x_L1
                          // [15: 8]=0xd0(208) : lum2_x_H0
                          // [23:16]=0xd8(216) : lum2_x_H1
                          // [31:24]=0xff(255) : lum2_y0
	{0x5524, 0x0000FFFF}, // PE1_MP_CTRL_0C
                          // [ 7: 0]=0xff(255) : lum2_y1
                          // [15: 8]=0xff(255) : lum2_y2
	{0x5530, 0x80206001}, // PE1_CTI_CTRL_00
                          // [    0]=0x1(1) : cti_en
                          // [ 6: 4]=0x5(5) : filter_tap_size
                          // [15: 8]=0x10(16) : cti_gain
	{0x5534, 0x00015010}, // PE1_CTI_CTRL_01
                          // [15: 8]=0x18(24) : coring_th1
                          // [18:16]=0x1(1) : coring_map_filter
                          // [22:20]=0x0(0) : coring_tap_size
                          // [25:24]=0x0(0) : debug mode
	{0x5538, 0x00880852}, // PE1_CTI_CTRL_02
                          // [    0]=0x0(0) : ycm_en0
                          // [    1]=0x1(1) : ycm_en1
                          // [ 6: 4]=0x5(5) : ycm_band_sel
                          // [15: 8]=0x8(8) : ycm_diff_th
                          // [19:16]=0x8(8) : ycm_y_gain
                          // [23:20]=0x8(8) : ycm_c_gain
	{0x5540, 0x30300050}, // PE1_TI_CTRL_0
                          // [    0]=0x0(0) : enable
                          // [    1]=0x0(0) : debug_coring
                          // [ 6: 4]=0x5(5) : Coring_Step
                          // [15: 8]=0x0(0) : Coring_Th
                          // [23:16]=0x30(48) : Y_gain
                          // [31:24]=0x30(48) : C_gain
	{0x5544, 0x50200003}, // PE1_TI_CTRL_1
                          // [    0]=0x1(1) : gain0_en
                          // [    1]=0x1(1) : gain1_en
                          // [ 5: 4]=0x0(0) : output_mux
                          // [23:16]=0x20(32) : gain0_th0
                          // [31:24]=0x50(80) : gain0_th1
	{0x5548, 0x00000000}, // PE1_TI_CTRL_2
                          // [ 1: 0]=0x0(0) : gain1_div_mode
	{0x5550, 0x18102003}, // PE1_BLUR_CTRL
                          // [    0]=0x1(1) : enable
                          // [    1]=0x1(1) : coring_en
                          // [ 5: 4]=0x0(0) : direction
                          // [15: 8]=0x20(32) : gain
                          // [23:16]=0x10(16) : coring_min
                          // [31:24]=0x18(24) : coring_max
	{0x5560, 0x02630D28}, // PE1_CORING_CTRL_00
                          // [ 5: 0]=0x28(40) : ga_max
                          // [15: 8]=0xd(13) : ga_th0
                          // [23:16]=0x63(99) : ga_th1
                          // [25:24]=0x2(2) : amap1_sel
                          // [27:26]=0x0(0) : amap2_sel
                          // [   28]=0x0(0) : edge_a_sel
                          // [   29]=0x0(0) : debug_a_sel
	{0x5564, 0x48180A27}, // PE1_CORING_CTRL_01
                          // [ 2: 0]=0x7(7) : max_sel
                          // [ 5: 4]=0x2(2) : avg_sel
                          // [13: 8]=0xa(10) : a2tw_min
                          // [23:16]=0x18(24) : a2tw_th0
                          // [31:24]=0x48(72) : a2tw_th1
	{0x5568, 0x00040C01}, // PE1_CORING_CTRL_02
                          // [    0]=0x1(1) : nrv_en
                          // [13: 8]=0xc(12) : nrv_th1
                          // [21:16]=0x4(4) : nrv_th2
	{0x556C, 0x00040C01}, // PE1_CORING_CTRL_03
                          // [    0]=0x1(1) : nrh_en
                          // [13: 8]=0xc(12) : nrh_th1
                          // [21:16]=0x4(4) : nrh_th2
	{0x5570, 0x00210483}, // PE1_CORING_CTRL_04
                          // [ 1: 0]=0x3(3) : dflt_sel
                          // [14: 4]=0x48(72) : var_th
                          // [17:16]=0x1(1) : coring_mode1
                          // [19:18]=0x0(0) : coring_mode2
                          // [   20]=0x0(0) : ac_direction
                          // [   21]=0x1(1) : tflt_en
                          // [25:24]=0x0(0) : exp_mode
	{0x5574, 0x0140020C}, // PE1_CORING_CTRL_05
                          // [ 5: 0]=0xc(12) : gt_max
                          // [15: 8]=0x2(2) : gt_th0
                          // [23:16]=0x40(64) : gt_th1
                          // [   24]=0x1(1) : a2tw_en
	{0x5578, 0x08042008}, // PE1_CORING_CTRL_06
                          // [ 7: 0]=0x8(8) : gt_th0a
                          // [15: 8]=0x20(32) : gt_th0b
                          // [21:16]=0x4(4) : gt_gain0a
                          // [29:24]=0x8(8) : gt_gain0b
	{0x557C, 0x40203B2A}, // PE1_CORING_CTRL_07
                          // [ 7: 0]=0x2a(42) : g_th0
                          // [15: 8]=0x3b(59) : g_th1
                          // [22:16]=0x20(32) : amap_gain
                          // [30:24]=0x40(64) : tmap_gain
	{0x5580, 0x0000003F}, // PE1_CORING_CTRL_08
                          // [ 1: 0]=0x3(3) : mp_coring_mode
                          // [ 3: 2]=0x3(3) : sp_coring_mode
                          // [    4]=0x1(1) : mp_coring_en
                          // [    5]=0x1(1) : sp_coring_en
	{0x5584, 0x20200810}, // PE1_CORING_CTRL_09
                          // [ 6: 0]=0x10(16) : edge_gain_b
                          // [14: 8]=0x8(8) : edge_gain_w
                          // [22:16]=0x20(32) : texture_gain_b
                          // [30:24]=0x20(32) : texture_gain_w
	{0x5590, 0xB0400467}, // PE1_DJ_CTRL_00
                          // [    0]=0x1(1) : edf_en
                          // [    1]=0x1(1) : hv_filter_en
                          // [    2]=0x1(1) : g0_feature_norm_en
                          // [    3]=0x0(0) : g0_f2g_mapping_en
                          // [    4]=0x0(0) : line_variation_mode
                          // [    5]=0x1(1) : L_type_protection
                          // [    6]=0x1(1) : center_blur_en
                          // [    7]=0x0(0) : direction_type_mode
                          // [12: 8]=0x4(4) : count_diff_th
                          // [18:16]=0x0(0) : output_mux
                          // [   22]=0x1(1) : n_avg_mode
                          // [31:24]=0xb0(176) : line_variation_diff_threshold
	{0x5594, 0x00801800}, // PE1_DJ_CTRL_01
                          // [ 7: 0]=0x0(0) : level_th
                          // [15: 8]=0x18(24) : protect_th
                          // [23:16]=0x80(128) : n_avg_gain
	{0x5598, 0x0000140D}, // PE1_DJ_CTRL_02
                          // [ 4: 0]=0xd(13) : edf_count_min
                          // [12: 8]=0x14(20) : edf_count_max
	{0x559C, 0x080B100B}  // PE1_DJ_CTRL_03
                          // [ 4: 0]=0xb(11) : dj_h_count_min
                          // [12: 8]=0x10(16) : dj_h_count_max
                          // [20:16]=0xb(11) : dj_v_count_min
                          // [28:24]=0x8(8) : dj_v_count_max
};

