#ifndef __CTOP_CTRL_REG_M14B0_H__
#define __CTOP_CTRL_REG_M14B0_H__


/*-----------------------------------------------------------------------------
	0xc0019000 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	c_dr3p_m_ctrl                   : 5,	// 19:23
	c_dr3p_pre_fd_ctrl              : 2,	// 24:25
	c_dr3p_od_fout_ctrl             : 2;	// 26:27
} CTOP_TOP_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019004 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c_dr3p_udex_ctrl                : 5,	//  0: 4
	c_dr3p_msex_ctrl                : 9,	//  5:13
	c_dr3p_nsc_ctrl                 : 4,	// 14:17
	c_dr3p_npc_ctrl                 : 6,	// 18:23
	                                : 3,	// 24:26 reserved
	c_dr3p_cih_ctrl                 : 3,	// 27:29
	c_dr3p_ibw_ctrl                 : 1,	//    30
	c_dr3p_pdb_ctrl                 : 1;	//    31
} CTOP_TOP_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019008 ctr02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ddr3pll_temp1                   : 8,	//  0: 7
	ddr3pll_temp1_1                 : 8,	//  8:15
	ddr3pll_updn_max                : 7,	// 16:22
	ddr3pll_nsc                     : 4,	// 23:26
	                                : 1,	//    27 reserved
	ddr3pll_m                       : 2,	// 28:29
	ddr3pll_accuracy                : 2;	// 30:31
} CTOP_TOP_CTR02_M14B0;

/*-----------------------------------------------------------------------------
	0xc001900c ctr03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ddr3pll_mod_freq                : 9,	//  0: 8
	                                : 1,	//     9 reserved
	ddr3pll_npc                     : 6,	// 10:15
	ddr3pll_dss                     : 1,	//    16
	                                : 2,	// 17:18 reserved
	ddr3pll_cih                     : 3,	// 19:21
	                                : 2,	// 22:23 reserved
	ddr3pll_fine_div                : 1,	//    24
	ddr3pll_fine_control            : 3,	// 25:27
	                                : 1,	//    28 reserved
	ddr3pll_od_fout                 : 2,	// 29:30
	ddr3pll_pdb                     : 1;	//    31
} CTOP_TOP_CTR03_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019010 ctr04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ddr3pll_temp1                   : 8,	//  0: 7
	ddr3pll_temp1_1                 : 8,	//  8:15
	ddr3pll_updn_max                : 7,	// 16:22
	ddr3pll_nsc                     : 4,	// 23:26
	                                : 1,	//    27 reserved
	ddr3pll_m                       : 2,	// 28:29
	ddr3pll_accuracy                : 2;	// 30:31
} CTOP_TOP_CTR04_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019014 ctr05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ddr3pll_mod_freq                : 9,	//  0: 8
	                                : 1,	//     9 reserved
	ddr3pll_npc                     : 6,	// 10:15
	ddr3pll_dss                     : 1,	//    16
	                                : 2,	// 17:18 reserved
	ddr3pll_cih                     : 3,	// 19:21
	                                : 2,	// 22:23 reserved
	ddr3pll_fine_div                : 1,	//    24
	ddr3pll_fine_control            : 3,	// 25:27
	                                : 1,	//    28 reserved
	ddr3pll_od_fout                 : 2,	// 29:30
	ddr3pll_pdb                     : 1;	//    31
} CTOP_TOP_CTR05_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019018 ctr06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adder_value                     : 8,	//  0: 7
	f24_sel                         : 1,	//     8
	f27_18_sel                      : 1,	//     9
	sc_ext_sel                      : 3,	// 10:12
	dco_ext_sel                     : 1,	//    13
	adder_div_en                    : 1,	//    14
	venc_320_sel                    : 1,	//    15
	                                : 5,	// 16:20 reserved
	apb_clk_sel                     : 2,	// 21:22
	f400_clk_sel                    : 2,	// 23:24
	gpu_320_sel                     : 2,	// 25:26
	phy_i2c_clk_sel                 : 1,	//    27
	bus_clk_sel                     : 2,	// 28:29
	trace_clk_sel                   : 2;	// 30:31
} CTOP_TOP_CTR06_M14B0;

/*-----------------------------------------------------------------------------
	0xc001901c ctr07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	swrst_apb                       : 1,	//     1
	swrst_f400                      : 1,	//     2
	swrst_idsmcu                    : 1,	//     3
	swrst_ptmpg                     : 1,	//     4
	swrst_ptm                       : 1,	//     5
	swrst_bus                       : 1,	//     6
	swrst_hdp                       : 1,	//     7
	swrst_codec                     : 1,	//     8
	swrst_disp                      : 1,	//     9
	swrst_venc                      : 1,	//    10
	swrst_vdec1                     : 1,	//    11
	swrst_vdec0                     : 1,	//    12
	swrst_usb                       : 1,	//    13
	swrst_te                        : 1,	//    14
	swrst_m1_d100                   : 1,	//    15
	swrst_m0_d100                   : 1,	//    16
	swrst_m1                        : 1,	//    17
	swrst_m0                        : 1,	//    18
	swrst_gpu                       : 1,	//    19
	swrst_gfx                       : 1,	//    20
	swrst_frcd                      : 1,	//    21
	swrst_frcc                      : 1,	//    22
	swrst_frcb                      : 1,	//    23
	swrst_frca                      : 1,	//    24
	swrst_dpp                       : 1,	//    25
	swrst_dpe                       : 1,	//    26
	swrst_deo                       : 1,	//    27
	swrst_dem                       : 1,	//    28
	swrst_dei                       : 1,	//    29
	swrst_cpuperi                   : 1,	//    30
	swrst_aud                       : 1;	//    31
} CTOP_TOP_CTR07_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019020 ctr08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tpio_sel_ctrl                   : 1,	//     0
	tpo_sel_ctrl                    : 1,	//     1
	stpio_en_ctrl                   : 1,	//     2
	stpi1_ren_ctrl                  : 1,	//     3
	stpi0_ren_ctrl                  : 1,	//     4
	gp5_7_mux_en                    : 1,	//     5
	gp5_6_mux_en                    : 1,	//     6
	gp5_5_mux_en                    : 1,	//     7
	gp5_4_mux_en                    : 1,	//     8
	gp5_3_mux_en                    : 1,	//     9
	gp5_2_mux_en                    : 1,	//    10
	gp5_1_mux_en                    : 1,	//    11
	gp5_0_mux_en                    : 1,	//    12
	gp6_7_mux_en                    : 1,	//    13
	gp6_6_mux_en                    : 1,	//    14
	gp6_5_mux_en                    : 1,	//    15
	gp6_4_mux_en                    : 1,	//    16
	gp6_3_mux_en                    : 1,	//    17
	gp6_2_mux_en                    : 1,	//    18
	gp6_1_mux_en                    : 1,	//    19
	gp6_0_mux_en                    : 1,	//    20
	gp7_7_mux_en                    : 1,	//    21
	gp7_6_mux_en                    : 1,	//    22
	gp7_5_mux_en                    : 1,	//    23
	gp7_4_mux_en                    : 1,	//    24
	gp7_3_mux_en                    : 1,	//    25
	gp7_2_mux_en                    : 1,	//    26
	gp8_6_mux_en                    : 1,	//    27
	gp8_5_mux_en                    : 1,	//    28
	gp8_4_mux_en                    : 1,	//    29
	gp8_3_mux_en                    : 1,	//    30
	te_tp_output_en                 : 1;	//    31
} CTOP_TOP_CTR08_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019024 ctr09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tpi_clk_polarity_con            : 1,	//     0
	                                : 6,	//  1: 6 reserved
	gp11_0_mux_en                   : 1,	//     7
	gp11_1_mux_en                   : 1,	//     8
	gp11_2_mux_en                   : 1,	//     9
	gp11_3_mux_en                   : 1,	//    10
	gp11_4_mux_en                   : 1,	//    11
	gp11_5_mux_en                   : 1,	//    12
	gp11_6_mux_en                   : 1,	//    13
	gp11_7_mux_en                   : 1,	//    14
	gp10_0_mux_en                   : 1,	//    15
	gp10_1_mux_en                   : 1,	//    16
	gp10_2_mux_en                   : 1,	//    17
	gp10_3_mux_en                   : 1,	//    18
	gp10_4_mux_en                   : 1,	//    19
	gp10_5_mux_en                   : 1,	//    20
	gp10_6_mux_en                   : 1,	//    21
	gp10_7_mux_en                   : 1,	//    22
	gp9_0_mux_en                    : 1,	//    23
	gp9_1_mux_en                    : 1,	//    24
	gp9_2_mux_en                    : 1,	//    25
	gp9_3_mux_en                    : 1,	//    26
	gp9_4_mux_en                    : 1,	//    27
	traceclk_en                     : 1,	//    28
	trace16bit_en                   : 1,	//    29
	trace8bit_en                    : 1,	//    30
	vccen_n_pol                     : 1;	//    31
} CTOP_TOP_CTR09_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019028 ctr10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ir_enable                       : 1,	//     0
	                                : 9,	//  1: 9 reserved
	gp14_2_mux_en                   : 1,	//    10
	gp14_3_mux_en                   : 1,	//    11
	gp14_4_mux_en                   : 1,	//    12
	gp14_5_mux_en                   : 1,	//    13
	gp14_6_mux_en                   : 1,	//    14
	gp14_7_mux_en                   : 1,	//    15
	gp13_0_mux_en                   : 1,	//    16
	gp13_1_mux_en                   : 1,	//    17
	gp13_2_mux_en                   : 1,	//    18
	gp13_3_mux_en                   : 1,	//    19
	gp13_4_mux_en                   : 1,	//    20
	gp13_5_mux_en                   : 1,	//    21
	gp13_6_mux_en                   : 1,	//    22
	gp13_7_mux_en                   : 1,	//    23
	gp12_0_mux_en                   : 1,	//    24
	gp12_1_mux_en                   : 1,	//    25
	gp12_2_mux_en                   : 1,	//    26
	gp12_3_mux_en                   : 1,	//    27
	gp12_4_mux_en                   : 1,	//    28
	gp12_5_mux_en                   : 1,	//    29
	gp12_6_mux_en                   : 1,	//    30
	gp12_7_mux_en                   : 1;	//    31
} CTOP_TOP_CTR10_M14B0;

/*-----------------------------------------------------------------------------
	0xc001902c ctr11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :15,	//  0:14 reserved
	sc_clk_sel                      : 1,	//    15
	gp15_0_mux_en                   : 1,	//    16
	gp15_1_mux_en                   : 1,	//    17
	gp15_2_mux_en                   : 1,	//    18
	gp15_3_mux_en                   : 1,	//    19
	gp15_4_mux_en                   : 1,	//    20
	gp15_5_mux_en                   : 1,	//    21
	gp16_6_mux_en                   : 1,	//    22
	gp16_7_mux_en                   : 1,	//    23
	                                : 7,	// 24:30 reserved
	sdio_en                         : 1;	//    31
} CTOP_TOP_CTR11_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019030 ctr12 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc0019034 ctr13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :23,	//  0:22 reserved
	pd                              : 1;	//    23
} CTOP_TOP_CTR13_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019038 ctr14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pull_up_control                 : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pull_up_control_1               : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control_2               : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_3               : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_4               : 1,	//    19
	                                : 7,	// 20:26 reserved
	pull_up_control_5               : 1;	//    27
} CTOP_TOP_CTR14_M14B0;

/*-----------------------------------------------------------------------------
	0xc001903c ctr15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	pull_up_control                 : 1,	//     7
	                                : 7,	//  8:14 reserved
	pull_up_control_1               : 1,	//    15
	                                : 7,	// 16:22 reserved
	pull_up_control_2               : 1,	//    23
	                                : 7,	// 24:30 reserved
	emmc_clk_driving_strength       : 1;	//    31
} CTOP_TOP_CTR15_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019040 ctr16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pull_up_control                 : 1,	//     0
	                                :10,	//  1:10 reserved
	pull_up_control_1               : 1,	//    11
	                                : 7,	// 12:18 reserved
	pull_up_control_2               : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_3               : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_4               : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_5               : 1;	//    31
} CTOP_TOP_CTR16_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019044 ctr17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	pull_up_control                 : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_1               : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_2               : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_3               : 1;	//    31
} CTOP_TOP_CTR17_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019048 ctr18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pull_up_control                 : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pull_up_control_1               : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control_2               : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_3               : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_4               : 1,	//    19
	                                :11,	// 20:30 reserved
	pull_up_control_5               : 1;	//    31
} CTOP_TOP_CTR18_M14B0;

/*-----------------------------------------------------------------------------
	0xc001904c ctr19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	ts_sel                          : 4;	//  4: 7
} CTOP_TOP_CTR19_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019050 ctr20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	pull_up_control                 : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control_1               : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_2               : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_3               : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_4               : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_5               : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_6               : 1;	//    31
} CTOP_TOP_CTR20_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019054 ctr21 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc0019058 ctr22 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc001905c ctr23 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc0019060 ctr24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pll_sel_m1                      : 1,	//     0
	pll_sel_m2                      : 1,	//     1
	pll_sel_core                    : 1,	//     2
	pll_sel_dco                     : 1,	//     3
	pll_sel_dto                     : 1;	//     4
} CTOP_TOP_CTR24_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019064 ctr25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	txclkdrv_pdb                    : 1,	//     0
	txclkdrv_icon                   : 3;	//  1: 3
} CTOP_TOP_CTR25_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa00 ctr80 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :13,	//  0:12 reserved
	c4_count_txlock2                :13,	// 13:25
	c4_sel_clkpix1                  : 3,	// 26:28
	c4_sel_clkpix2                  : 3;	// 29:31
} CTOP_LEFT_CTR80_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa04 ctr81 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_count_rxlock1                : 6,	//  0: 5
	c4_count_rxlock2                : 6,	//  6:11
	c4_txlock_fixen1                : 1,	//    12
	c4_txlock_fixen2                : 1,	//    13
	c4_lanemode                     : 2,	// 14:15
	c4_bytemode2                    : 2,	// 16:17
	c4_enc_off1                     : 1,	//    18
	c4_enc_off2                     : 1,	//    19
	c4_enc_rst1                     : 1,	//    20
	c4_enc_rst2                     : 1,	//    21
	c4_scr_off1                     : 1,	//    22
	c4_scr_off2                     : 1,	//    23
	c4_scr_rst1                     : 1,	//    24
	c4_scr_rst2                     : 1,	//    25
	c4_rst_vtxlink1                 : 1,	//    26
	c4_rst_vtxlink2                 : 1,	//    27
	c4_test_mode1                   : 2,	// 28:29
	c4_test_mode2                   : 2;	// 30:31
} CTOP_LEFT_CTR81_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa08 ctr82 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_test_di1                     :10,	//  0: 9
	c4_test_di2                     :10,	// 10:19
	c4_read_address_offset1         : 3,	// 20:22
	c4_read_address_offset2         : 3,	// 23:25
	c4_clk_pix1x_rf1                : 1,	//    26
	c4_clk_pix1x_rf2                : 1,	//    27
	                                : 3,	// 28:30 reserved
	c4_pdb11                        : 1;	//    31
} CTOP_LEFT_CTR82_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa0c ctr83 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_sel_lock                     : 1,	//     0
	c4_sel_fin                      : 1,	//     1
	c4_pll_60_mode                  : 1,	//     2
	c4_sel_lpf                      : 1,	//     3
	c4_epi_sel_mode                 : 1,	//     4
	c4_rf                           : 1,	//     5
	c4_term_en                      : 1,	//     6
	c4_adj_term                     : 4,	//  7:10
	c4_sel_mode                     : 2,	// 11:12
	c4_sw_pud                       : 1,	//    13
	                                : 1,	//    14 reserved
	c4_ib_oft                       : 1,	//    15
	c4_cih                          : 3,	// 16:18
	c4_ctl_lock                     : 2,	// 19:20
	reserved                        : 2,	// 21:22
	c4_sel_prediv                   : 1,	//    23
	c4_sw_rext                      : 1,	//    24
	c4_pll_ic                       : 2,	// 25:26
	c4_sel_bw                       : 1;	//    27
} CTOP_LEFT_CTR83_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa10 ctr84 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_itune0                       : 3,	//  0: 2
	c4_itune1                       : 3,	//  3: 5
	c4_itune2                       : 3,	//  6: 8
	c4_itune3                       : 3,	//  9:11
	c4_itune4                       : 3,	// 12:14
	c4_itune5                       : 3,	// 15:17
	c4_itune6                       : 3,	// 18:20
	c4_itune7                       : 3,	// 21:23
	c4_itune8                       : 3,	// 24:26
	c4_itune9                       : 3;	// 27:29
} CTOP_LEFT_CTR84_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa14 ctr85 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_itune10                      : 3,	//  0: 2
	c4_itune11                      : 3,	//  3: 5
	c4_itune12                      : 3,	//  6: 8
	c4_itune13                      : 3,	//  9:11
	c4_itune14                      : 3,	// 12:14
	c4_itune15                      : 3,	// 15:17
	c4_itune16                      : 3,	// 18:20
	c4_itune17                      : 3,	// 21:23
	c4_itune18                      : 3,	// 24:26
	c4_itune19                      : 3;	// 27:29
} CTOP_LEFT_CTR85_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa18 ctr86 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_itune20                      : 3,	//  0: 2
	c4_itune21                      : 3,	//  3: 5
	c4_itune22                      : 3,	//  6: 8
	c4_itune23                      : 3;	//  9:11
} CTOP_LEFT_CTR86_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa1c ctr87 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_pemp_ch0                     : 2,	//  0: 1
	c4_pemp_ch1                     : 2,	//  2: 3
	c4_pemp_ch2                     : 2,	//  4: 5
	c4_pemp_ch3                     : 2,	//  6: 7
	c4_pemp_ch4                     : 2,	//  8: 9
	c4_pemp_ch5                     : 2,	// 10:11
	c4_pemp_ch6                     : 2,	// 12:13
	c4_pemp_ch7                     : 2,	// 14:15
	c4_pemp_ch8                     : 2,	// 16:17
	c4_pemp_ch9                     : 2,	// 18:19
	c4_pemp_ch10                    : 2,	// 20:21
	c4_pemp_ch11                    : 2,	// 22:23
	c4_pemp_ch12                    : 2,	// 24:25
	c4_pemp_ch13                    : 2,	// 26:27
	c4_pemp_ch14                    : 2,	// 28:29
	c4_pemp_ch15                    : 2;	// 30:31
} CTOP_LEFT_CTR87_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa20 ctr88 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_pemp_ch16                    : 2,	//  0: 1
	c4_pemp_ch17                    : 2,	//  2: 3
	c4_pemp_ch18                    : 2,	//  4: 5
	c4_pemp_ch19                    : 2,	//  6: 7
	c4_pemp_ch20                    : 2,	//  8: 9
	c4_pemp_ch21                    : 2,	// 10:11
	c4_pemp_ch22                    : 2,	// 12:13
	c4_pemp_ch23                    : 2,	// 14:15
	c4_link4_en                     : 1,	//    16
	c4_link5_en                     : 1,	//    17
	c4_link6_en                     : 1,	//    18
	c4_link7_en                     : 1,	//    19
	c4_link8_en                     : 1,	//    20
	c4_link9_en                     : 1,	//    21
	c4_link10_en                    : 1,	//    22
	c4_link11_en                    : 1;	//    23
} CTOP_LEFT_CTR88_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa24 ctr89 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_flip_en_ch0                  : 1,	//     0
	c4_flip_en_ch1                  : 1,	//     1
	c4_flip_en_ch2                  : 1,	//     2
	c4_flip_en_ch3                  : 1,	//     3
	c4_flip_en_ch4                  : 1,	//     4
	c4_flip_en_ch5                  : 1,	//     5
	c4_flip_en_ch6                  : 1,	//     6
	c4_flip_en_ch7                  : 1,	//     7
	c4_flip_en_ch8                  : 1,	//     8
	c4_flip_en_ch9                  : 1,	//     9
	c4_flip_en_ch10                 : 1,	//    10
	c4_flip_en_ch11                 : 1,	//    11
	c4_flip_en_ch12                 : 1,	//    12
	c4_flip_en_ch13                 : 1,	//    13
	c4_flip_en_ch14                 : 1,	//    14
	c4_flip_en_ch15                 : 1,	//    15
	c4_flip_en_ch16                 : 1,	//    16
	c4_flip_en_ch17                 : 1,	//    17
	c4_flip_en_ch18                 : 1,	//    18
	c4_flip_en_ch19                 : 1,	//    19
	c4_flip_en_ch20                 : 1,	//    20
	c4_flip_en_ch21                 : 1,	//    21
	c4_flip_en_ch22                 : 1,	//    22
	c4_flip_en_ch23                 : 1;	//    23
} CTOP_LEFT_CTR89_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa28 ctr90 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_mclk_en_ch0                  : 1,	//     0
	c4_mclk_en_ch1                  : 1,	//     1
	c4_mclk_en_ch2                  : 1,	//     2
	c4_mclk_en_ch3                  : 1,	//     3
	c4_mclk_en_ch4                  : 1,	//     4
	c4_mclk_en_ch5                  : 1,	//     5
	c4_mclk_en_ch6                  : 1,	//     6
	c4_mclk_en_ch7                  : 1,	//     7
	c4_mclk_en_ch8                  : 1,	//     8
	c4_mclk_en_ch9                  : 1,	//     9
	c4_mclk_en_ch10                 : 1,	//    10
	c4_mclk_en_ch11                 : 1,	//    11
	c4_mclk_en_ch12                 : 1,	//    12
	c4_mclk_en_ch13                 : 1,	//    13
	c4_mclk_en_ch14                 : 1,	//    14
	c4_mclk_en_ch15                 : 1,	//    15
	c4_mclk_en_ch16                 : 1,	//    16
	c4_mclk_en_ch17                 : 1,	//    17
	c4_mclk_en_ch18                 : 1,	//    18
	c4_mclk_en_ch19                 : 1,	//    19
	c4_mclk_en_ch20                 : 1,	//    20
	c4_mclk_en_ch21                 : 1,	//    21
	c4_mclk_en_ch22                 : 1,	//    22
	c4_mclk_en_ch23                 : 1;	//    23
} CTOP_LEFT_CTR90_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa2c ctr91 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc001fa30 ctr92 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	disp_m_ctrl                     : 5,	// 19:23
	disp_pre_fd_ctrl                : 2,	// 24:25
	disp_od_fout_ctrl               : 2;	// 26:27
} CTOP_LEFT_CTR92_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa34 ctr93 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	disp_udex_ctrl                  : 5,	//  0: 4
	disp_msex_ctrl                  : 9,	//  5:13
	disp_nsc_ctrl                   : 4,	// 14:17
	disp_npc_ctrl                   : 6,	// 18:23
	                                : 3,	// 24:26 reserved
	disp_cih_ctrl                   : 3,	// 27:29
	disp_ibw_ctrl                   : 1,	//    30
	disp_pdb_ctrl                   : 1;	//    31
} CTOP_LEFT_CTR93_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa38 ctr94 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :25,	//  0:24 reserved
	de_dco_g_ctrl                   : 3,	// 25:27
	de_dco_lpf_ctrl                 : 2,	// 28:29
	de_dco_resetb_ctrl              : 1,	//    30
	de_dco_rom_test_ctrl            : 1;	//    31
} CTOP_LEFT_CTR94_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa3c ctr95 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	jtag0_disable                   : 1,	//     0
	jtag1_disable                   : 1,	//     1
	                                : 6,	//  2: 7 reserved
	jtag1_sel                       : 3,	//  8:10
	jtag_vdec_sel                   : 1,	//    11
	jtag_aud_sel                    : 1,	//    12
	jtag_de_sel                     : 1,	//    13
	jtag_frc_sel                    : 1,	//    14
	jtag_gpu_sel                    : 1,	//    15
	jtag0_sel                       : 3,	// 16:18
	                                :12,	// 19:30 reserved
	jtag_hdmi_debug_sel             : 1;	//    31
} CTOP_LEFT_CTR95_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa40 ctr96 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	uart1_rxd_ren_ctrl              : 1,	//     0
	uart1_txd_ren_ctrl              : 1,	//     1
	uart0_rxd_ren_ctrl              : 1,	//     2
	uart0_txd_ren_ctrl              : 1,	//     3
	                                : 4,	//  4: 7 reserved
	rx_sel_uart1                    : 1,	//     8
	rx_sel_uart0                    : 1,	//     9
	rx_sel_hdpmcu1                  : 2,	// 10:11
	rx_sel_hdpmcu0                  : 2,	// 12:13
	rx_sel_venc                     : 2,	// 14:15
	rx_sel_vdec                     : 2,	// 16:17
	rx_sel_tz                       : 2,	// 18:19
	                                : 1,	//    20 reserved
	uart2_sel                       : 3,	// 21:23
	                                : 1,	//    24 reserved
	uart1_sel                       : 3,	// 25:27
	                                : 1,	//    28 reserved
	uart0_sel                       : 3;	// 29:31
} CTOP_LEFT_CTR96_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa44 ctr97 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp7_0_mux_en                    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp7_1_mux_en                    : 1,	//     7
	                                : 7,	//  8:14 reserved
	pull_up_control                 : 1,	//    15
	                                :15,	// 16:30 reserved
	gp8_7_mux_en                    : 1;	//    31
} CTOP_LEFT_CTR97_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa48 ctr98 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp14_0_mux_en                   : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp14_1_mux_en                   : 1,	//     7
	                                :19,	//  8:26 reserved
	gp15_6_mux_en                   : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp15_7_mux_en                   : 1;	//    31
} CTOP_LEFT_CTR98_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa4c ctr99 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sel_mon_de                      : 2,	//  0: 1
	sel_mon_frc                     : 2,	//  2: 3
	sel_mon_a3c_dmc                 : 1,	//     4
	sel_mon_dpp_svp                 : 1,	//     5
	sel_mon_dvo                     : 1,	//     6
	sel_mon_dvi                     : 1,	//     7
	gp16_ren_ctrl                   : 1,	//     8
	gp17_ren_ctrl                   : 1,	//     9
	gp18_ren_ctrl                   : 1,	//    10
	gp19_ren_ctrl                   : 1,	//    11
	gp20_ren_ctrl                   : 1,	//    12
	gp21_ren_ctrl                   : 1,	//    13
	gp22_ren_ctrl                   : 1,	//    14
	gp23_ren_ctrl                   : 1,	//    15
	                                : 2,	// 16:17 reserved
	aud_dacrlrch_enable             : 1,	//    18
	trace_en                        : 1,	//    19
	mon_dppa_en                     : 1,	//    20
	mon_frc_en                      : 1,	//    21
	mon_h3d_en                      : 1,	//    22
	mon_aad_en                      : 1,	//    23
	mon_aud_en                      : 1,	//    24
	mon_cpu_en                      : 1,	//    25
	mon_usb_en                      : 1,	//    26
	mon_de_en                       : 1,	//    27
	led_vs_en                       : 1,	//    28
	led_i2c_en                      : 1,	//    29
	uart2_en                        : 1,	//    30
	int_tp_en                       : 1;	//    31
} CTOP_LEFT_CTR99_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa50 ctr100 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :18,	//  0:17 reserved
	gp9_5_mux_en                    : 1,	//    18
	gp9_6_mux_en                    : 1,	//    19
	gp9_7_mux_en                    : 1,	//    20
	gp8_0_mux_en                    : 1,	//    21
	gp8_1_mux_en                    : 1,	//    22
	gp8_2_mux_en                    : 1,	//    23
	gp24_ren_ctrl                   : 1,	//    24
	gp25_ren_ctrl                   : 1,	//    25
	gp26_ren_ctrl                   : 1,	//    26
	gp27_ren_ctrl                   : 1,	//    27
	gp28_ren_ctrl                   : 1,	//    28
	gp29_ren_ctrl                   : 1,	//    29
	gp30_ren_ctrl                   : 1,	//    30
	gp31_ren_ctrl                   : 1;	//    31
} CTOP_LEFT_CTR100_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa54 ctr101 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	usb2_0_por                      : 1,	//     3
	usb2_0_testdatain_ctrl          : 8,	//  4:11
	usb2_0_testdataoutsel_ctrl      : 1,	//    12
	usb2_0_testaddr_ctrl            : 4,	// 13:16
	usb2_0_testclk_ctrl             : 1,	//    17
	usb2_0_txpreemphasistune        : 2,	// 18:19
	usb2_0_txrisetune_ctrl          : 2,	// 20:21
	usb2_0_commononn_ctrl           : 1,	//    22
	usb2_0_fsel_ctrl                : 3,	// 23:25
	usb2_0_refclksel_ctrl           : 2,	// 26:27
	usb2_0_testen_en_ctrl           : 1,	//    28
	usb2_0_vatestenb_en_ctrl        : 2,	// 29:30
	usb2_0_siddq_en_ctrl            : 1;	//    31
} CTOP_LEFT_CTR101_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa58 ctr102 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ext_intr_pol_sel0               : 1,	//     0
	ext_intr_pol_sel1               : 1,	//     1
	ext_intr_pol_sel2               : 1,	//     2
	ext_intr_pol_sel3               : 1,	//     3
	                                :17,	//  4:20 reserved
	pull_up_control                 : 1,	//    21
	pull_up_control_1               : 1,	//    22
	pull_up_control_2               : 1,	//    23
	pull_up_control_3               : 1,	//    24
	pull_up_control_4               : 1,	//    25
	pull_up_control_5               : 1,	//    26
	                                : 1,	//    27 reserved
	pull_up_control_6               : 1,	//    28
	pull_up_control_7               : 1,	//    29
	pull_up_control_8               : 1,	//    30
	pull_up_control_9               : 1;	//    31
} CTOP_LEFT_CTR102_M14B0;

/*-----------------------------------------------------------------------------
	0xc001fa5c ctr103 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 5,	//  0: 4 reserved
	bypassdmen0                     : 1,	//     5
	bypassdpen0                     : 1,	//     6
	bypassdmdata0                   : 1,	//     7
	bypassdpdata0                   : 1,	//     8
	txrestune0                      : 2,	//  9:10
	txpreemppulsetune0              : 1,	//    11
	txvreftune0                     : 4,	// 12:15
	txfslstune0                     : 4,	// 16:19
	                                : 1,	//    20 reserved
	txhsxvtune0                     : 2,	// 21:22
	sqrxtune0                       : 3,	// 23:25
	compdistune0                    : 3,	// 26:28
	loopbackenb0                    : 1,	//    29
	otgdisable0                     : 1,	//    30
	portreset0                      : 1;	//    31
} CTOP_LEFT_CTR103_M14B0;

/*-----------------------------------------------------------------------------
	0xc001cc00 ctr120 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	phy_arc_odt_sel                 : 2,	//  0: 1
	                                :18,	//  2:19 reserved
	hdmi_4_sda_pull_up              : 1,	//    20
	hdmi_4_scl_pull_up              : 1,	//    21
	                                : 1,	//    22 reserved
	hdmi_3_sda_pull_up              : 1,	//    23
	hdmi_3_scl_pull_up              : 1,	//    24
	                                : 1,	//    25 reserved
	hdmi_2_sda_pull_up              : 1,	//    26
	hdmi_2_scl_pull_up              : 1,	//    27
	                                : 1,	//    28 reserved
	hdmi_1_sda_pull_up              : 1,	//    29
	hdmi_1_scl_pull_up              : 1,	//    30
	phy_arc_in_mux                  : 1;	//    31
} CTOP_RIGHT_CTR120_M14B0;

/*-----------------------------------------------------------------------------
	0xc001cc04 ctr121 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc001cc08 ctr122 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	i2c6_en                         : 1,	//     4
	aud_sub_en                      : 1,	//     5
	en_eb_addr_out                  : 1,	//     6
	trace_en                        : 1,	//     7
	ren_ctrl_in                     : 8,	//  8:15
	                                : 7,	// 16:22 reserved
	mon_dppa_en                     : 1,	//    23
	                                : 1,	//    24 reserved
	mon_frc_en                      : 1,	//    25
	mon_h3d_en                      : 1,	//    26
	mon_aad_en                      : 1,	//    27
	mon_aud_en                      : 1,	//    28
	mon_cpu_31_en                   : 1,	//    29
	mon_cpu_en                      : 1,	//    30
	mon_de_en                       : 1;	//    31
} CTOP_RIGHT_CTR122_M14B0;

/*-----------------------------------------------------------------------------
	0xc001cc0c ctr123 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc001cc10 ctr124 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	trace_en                        : 1,	//     7
	ren_ctrl_in                     : 8;	//  8:15
} CTOP_RIGHT_CTR124_M14B0;

/*-----------------------------------------------------------------------------
	0xc001cc14 ctr125 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	trace_en                        : 1,	//     8
	                                :12,	//  9:20 reserved
	mon_dppb_en                     : 1,	//    21
	mon_dpe_en                      : 1,	//    22
	mon_dppa_en                     : 1,	//    23
	mon_sre_en                      : 1,	//    24
	mon_frc_en                      : 1,	//    25
	mon_h3d_en                      : 1,	//    26
	mon_aad_en                      : 1,	//    27
	mon_aud_en                      : 1,	//    28
	                                : 1,	//    29 reserved
	mon_cpu_en                      : 1,	//    30
	mon_de_en                       : 1;	//    31
} CTOP_RIGHT_CTR125_M14B0;

/*-----------------------------------------------------------------------------
	0xc001cc18 ctr126 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ren_ctrl_in                     : 8,	//  0: 7
	                                : 8,	//  8:15 reserved
	gp4_mux_en                      : 8,	// 16:23
	ren_ctrl_in_1                   : 8;	// 24:31
} CTOP_RIGHT_CTR126_M14B0;

/*-----------------------------------------------------------------------------
	0xc001cc1c ctr127 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc001cc20 ctr128 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc001cc24 ctr129 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0xc001cc28 ctr130 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rxclkdrv_pdb                    : 1,	//     0
	rxclkdrv_outsel                 : 2,	//  1: 2
	rxclkdrv_smv                    : 1,	//     3
	                                :23,	//  4:26 reserved
	scart_feedback_enb              : 1,	//    27
	eeprom_write_sel                : 2,	// 28:29
	                                : 1,	//    30 reserved
	ddc_write_enb                   : 1;	//    31
} CTOP_RIGHT_CTR130_M14B0;

/*-----------------------------------------------------------------------------
	0xfd300218 ctr28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	reg_wd1_reset                   : 1,	//     7
	reg_wd0_reset                   : 1,	//     8
	reg_neon1_reset                 : 1,	//     9
	reg_neon0_reset                 : 1,	//    10
	reg_scu_reset                   : 1,	//    11
	reg_cpu_h                       : 1,	//    12
	reg_periph_reset                : 1,	//    13
	reg_cpu1_reset                  : 1,	//    14
	reg_cpu0_reset                  : 1,	//    15
	                                : 1,	//    16 reserved
	reg_l2_reset                    : 1,	//    17
	a_stdby_cpu                     : 2;	// 18:19
} CPU_CORE_SUB_CRG_CTR28_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019c00 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gfx_clk_sel                     : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	icod_clk_sel                    : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	apb_clk_sel                     : 2;	//  8: 9
} GFX_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019c04 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_gfxa                      : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_gfx                       : 1,	//     4
	                                : 3,	//  5: 7 reserved
	swrst_icoda                     : 1,	//     8
	                                : 3,	//  9:11 reserved
	swrst_icod                      : 1,	//    12
	                                : 3,	// 13:15 reserved
	swrst_apb                       : 1;	//    16
} GFX_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001a000 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdclk_sel                       : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	vd_aclk_sel                     : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	teclk_sel                       : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	hevc_aclk_sel                   : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	swrst_apb                       : 1,	//    16
	swrst_vd                        : 1,	//    17
	swrst_vda                       : 1,	//    18
	swrst_te                        : 1,	//    19
	swrst_sys                       : 1,	//    20
	swrst_hevca                     : 1,	//    21
	                                : 2,	// 22:23 reserved
	swrst_soc                       : 1,	//    24
	swrst_soc_half                  : 1,	//    25
	                                : 2,	// 26:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} VDEC_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001a400 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vdclk_sel                       : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	vd_aclk_sel                     : 2,	//  4: 5
	                                :10,	//  6:15 reserved
	swrst_apb                       : 1,	//    16
	swrst_vd                        : 1,	//    17
	swrst_vda                       : 1,	//    18
	                                : 5,	// 19:23 reserved
	swrst_soc                       : 1,	//    24
	swrst_soc_half                  : 1,	//    25
	                                : 2,	// 26:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} HEVC_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001a800 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	teclk_sel                       : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	fixed_sclk_sel                  : 1,	//     4
	                                : 3,	//  5: 7 reserved
	te_ciout_clk_sel                : 3,	//  8:10
	                                :17,	// 11:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} TE_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001a804 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_te1                       : 1,	//     0
	                                : 1,	//     1 reserved
	swrst_te3                       : 1,	//     2
	                                : 1,	//     3 reserved
	swrst_te2                       : 1,	//     4
	                                : 1,	//     5 reserved
	swrst_ciout                     : 1,	//     6
	                                : 1,	//     7 reserved
	swrst_axi                       : 1,	//     8
	                                : 3,	//  9:11 reserved
	swrst_s                         : 1,	//    12
	                                : 3,	// 13:15 reserved
	swrst_apb                       : 1,	//    16
	                                : 3,	// 17:19 reserved
	swrst_f_s                       : 1,	//    20
	                                : 7,	// 21:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} TE_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001ac00 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	teclk_sel                       : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	veclk_sel                       : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	seclk_inv_sel                   : 1,	//     8
	                                :19,	//  9:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} VENC_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001ac04 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_apb                       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_ve                        : 1,	//     4
	                                : 3,	//  5: 7 reserved
	swrst_te                        : 1,	//     8
	                                : 3,	//  9:11 reserved
	swrst_se                        : 1,	//    12
	                                :15,	// 13:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} VENC_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b000 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	aud_aclk_sel                    : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	aud_teclk_sel                   : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	soc_clk_sel                     : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	soc_half_clk_sel                : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	swrst_adto                      : 1,	//    16
	swrst_adto1                     : 1,	//    17
	swrst_adto2                     : 1,	//    18
	swrst_soc                       : 1,	//    19
	swrst_soc_half                  : 1,	//    20
	swrst_axi                       : 1,	//    21
	swrst_te                        : 1;	//    22
} AUD_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b004 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sel_audclk_src0                 : 3,	//  0: 2
	                                : 1,	//     3 reserved
	sel_audclk_div0                 : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	sel_audclk_src1                 : 3,	//  8:10
	                                : 1,	//    11 reserved
	sel_audclk_div1                 : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	sel_audclk                      : 1;	//    16
} AUD_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b008 ctr02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sel_audclk_sub_src0             : 3,	//  0: 2
	                                : 1,	//     3 reserved
	sel_audclk_sub_div0             : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	sel_audclk_sub_src1             : 3,	//  8:10
	                                : 1,	//    11 reserved
	sel_audclk_sub_div1             : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	sel_audclk_sub                  : 1;	//    16
} AUD_SUB_CRG_CTR02_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b00c ctr03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sel_aud_fs20clk_src             : 3,	//  0: 2
	                                : 1,	//     3 reserved
	sel_aud_fs20clk_div             : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	sel_aud_fs21clk_src             : 3,	//  8:10
	                                : 1,	//    11 reserved
	sel_aud_fs21clk_div             : 2;	// 12:13
} AUD_SUB_CRG_CTR03_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b010 ctr04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sel_aud_fs23clk_src             : 3,	//  0: 2
	                                : 1,	//     3 reserved
	sel_aud_fs23clk_div             : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	sel_aud_fs24clk_src             : 3,	//  8:10
	                                : 1,	//    11 reserved
	sel_aud_fs24clk_div             : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	sel_aud_fs25clk_src             : 3,	// 16:18
	                                : 1,	//    19 reserved
	sel_aud_fs25clk_div             : 2;	// 20:21
} AUD_SUB_CRG_CTR04_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b018 ctr06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto_loop_value                 :20;	//  0:19
} AUD_SUB_CRG_CTR06_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b01c ctr07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto_add_value                  ;   	// 31: 0
} AUD_SUB_CRG_CTR07_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b020 ctr08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto_err_value                  :12;	//  0:11
} AUD_SUB_CRG_CTR08_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b024 ctr09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto1_loop_value                :20;	//  0:19
} AUD_SUB_CRG_CTR09_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b028 ctr10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto1_add_value                 ;   	// 31: 0
} AUD_SUB_CRG_CTR10_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b02c ctr11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto1_err_value                 :12;	//  0:11
} AUD_SUB_CRG_CTR11_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b030 ctr12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto2_loop_value                :20;	//  0:19
} AUD_SUB_CRG_CTR12_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b034 ctr13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto2_add_value                 ;   	// 31: 0
} AUD_SUB_CRG_CTR13_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b038 ctr14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto2_err_value                 :12;	//  0:11
} AUD_SUB_CRG_CTR14_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b400 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	exta_sel                        : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	extb_sel                        : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	de_clk_sel                      : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	te_clk_sel                      : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	phy_ppll_sel                    : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	ch3pix_inv_sel                  : 1;	//    20
} DEI_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b404 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd54_inv_sel                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	aud_hdmi_mclk_inv_sel           : 1;	//     4
} DEI_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b408 ctr02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_hdmi_dto                  : 1,	//     0
	swrst_sys                       : 1,	//     1
	swrst_hdcp                      : 1,	//     2
	swrst_edid                      : 1,	//     3
	swrst_tmds_sel                  : 1,	//     4
	swrst_tmds                      : 1,	//     5
	swrst_vfifo_w                   : 1,	//     6
	swrst_afifo_w                   : 1,	//     7
	swrst_pix_pip                   : 1,	//     8
	swrst_vfifo_r                   : 1,	//     9
	swrst_aud                       : 1,	//    10
	swrst_afifo_r                   : 1;	//    11
} DEI_SUB_CRG_CTR02_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b40c ctr03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_hdmi                      : 1,	//     0
	swrst_apb                       : 1,	//     1
	swrst_armapb                    : 1,	//     2
	swrst_ch3pix                    : 1,	//     3
	swrst_de                        : 1,	//     4
	swrst_axi                       : 1,	//     5
	swrst_cvd54                     : 1,	//     6
	swrst_cvd27                     : 1,	//     7
	swrst_cvd_vbi                   : 1,	//     8
	swrst_exta                      : 1,	//     9
	swrst_extb                      : 1,	//    10
	swrst_vdo                       : 1,	//    11
	swrst_te                        : 1,	//    12
	swrst_cvd_mif                   : 1,	//    13
	swrst_hdmi_h                    : 1;	//    14
} DEI_SUB_CRG_CTR03_M14B0;

/*-----------------------------------------------------------------------------
	0xc001b800 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gpu_clk_sel                     : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	gpu_sys_clk_sel                 : 2,	//  4: 5
	                                :10,	//  6:15 reserved
	swrst_gpu_sys                   : 1,	//    16
	                                : 3,	// 17:19 reserved
	swrst_gpu_core                  : 1;	//    20
} GPU_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001c000 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mcu_clk_sel                     : 2,	//  0: 1
	                                :26,	//  2:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} IDSMCU_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001c004 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_apb                       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_axi                       : 1,	//     4
	                                :23,	//  5:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} IDSMCU_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001c800 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	de_clk_sel                      : 2,	//  0: 1
	                                :26,	//  2:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} DEM_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001c804 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_apb                       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_de                        : 1,	//     4
	                                : 3,	//  5: 7 reserved
	swrst_axi                       : 1,	//     8
	                                :19,	//  9:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} DEM_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001d000 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	de_clk_sel                      : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	de3m_clk_sel                    : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	dvi_clk_sel                     : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	cve27_inv_sel                   : 1,	//    12
	                                :15,	// 13:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} DEO_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001d004 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_apb                       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_de                        : 1,	//     4
	                                : 3,	//  5: 7 reserved
	swrst_axi                       : 1,	//     8
	                                : 3,	//  9:11 reserved
	swrst_de3m                      : 1,	//    12
	                                : 3,	// 13:15 reserved
	swrst_cve27                     : 1,	//    16
	                                : 3,	// 17:19 reserved
	swrst_dvi                       : 1,	//    20
	                                : 7,	// 21:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} DEO_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001d400 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	de_clk_sel                      : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	det_clk_sel                     : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	disp_clk_sel                    : 2,	//  8: 9
	                                :18,	// 10:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} DPP_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001d404 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_apb                       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_de                        : 1,	//     4
	                                : 3,	//  5: 7 reserved
	swrst_axi                       : 1,	//     8
	                                : 3,	//  9:11 reserved
	swrst_det                       : 1,	//    12
	                                : 3,	// 13:15 reserved
	swrst_disp                      : 1,	//    16
	                                :11,	// 17:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} DPP_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001d800 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frc_core_sel                    : 2,	//  0: 1
	                                :26,	//  2:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} FDA_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001d804 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_frc_apb                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_frc_core                  : 1,	//     4
	                                :23,	//  5:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} FDA_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001e000 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frc_core_sel                    : 2,	//  0: 1
	                                :26,	//  2:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} FDB_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001e004 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_frc_apb                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_frc_core                  : 1,	//     4
	                                :23,	//  5:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} FDB_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001e800 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frc_core_sel                    : 2,	//  0: 1
	                                :26,	//  2:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} FDC_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001e804 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_frc_apb                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_frc_core                  : 1,	//     4
	                                :23,	//  5:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} FDC_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001f000 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	frc_core_sel                    : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	frc_dvo_sel                     : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	frc_ddo_sel                     : 2,	//  8: 9
	                                :18,	// 10:27 reserved
	soc_clk_sel                     : 2,	// 28:29
	soc_half_clk_sel                : 2;	// 30:31
} FDD_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001f004 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_frc_apb                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_frc_core                  : 1,	//     4
	                                : 3,	//  5: 7 reserved
	swrst_frc_dvo                   : 1,	//     8
	                                : 3,	//  9:11 reserved
	swrst_frc_ddo                   : 1,	//    12
	                                :15,	// 13:27 reserved
	swrst_soc                       : 1,	//    28
	                                : 1,	//    29 reserved
	swrst_soc_half                  : 1;	//    30
} FDD_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001f800 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dpe_clk_sel                     : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	dpe_pxl_clk_sel                 : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	dpe_c4tx_clk_sel                : 3,	//  8:10
	                                : 1,	//    11 reserved
	dpe_cp_clk_sel                  : 2;	// 12:13
} DPE_SUB_CRG_CTR00_M14B0;

/*-----------------------------------------------------------------------------
	0xc001f804 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_dpe_apb                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	swrst_dpe_axi                   : 1,	//     4
	                                : 3,	//  5: 7 reserved
	swrst_dpe_pxl_led               : 1,	//     8
	                                : 3,	//  9:11 reserved
	swrst_dpe_c4tx                  : 1,	//    12
	                                : 3,	// 13:15 reserved
	swrst_dpe_pxl_tcon              : 1,	//    16
	                                : 3,	// 17:19 reserved
	swrst_dpe_cp                    : 1;	//    20
} DPE_SUB_CRG_CTR01_M14B0;

/*-----------------------------------------------------------------------------
	0xc001908c ctr35 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bus2_sel                        : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	ahb0_sel                        : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	ahb1_sel                        : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	ahb8_sel                        : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	ahb20_sel                       : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	ahb21_sel                       : 2,	// 20:21
	                                : 6,	// 22:27 reserved
	rmii_clkoff_sel                 : 1;	//    28
} CPU_PERI_SUB_CRG_CTR35_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019090 ctr36 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_bus0                      : 1,	//     0
	swrst_bus1                      : 1,	//     1
	swrst_bus2                      : 1,	//     2
	swrst_bus3                      : 1,	//     3
	swrst_bus4                      : 1,	//     4
	swrst_bus5                      : 1,	//     5
	swrst_bus6                      : 1,	//     6
	swrst_bus7                      : 1,	//     7
	swrst_bus8                      : 1,	//     8
	swrst_bus9                      : 1,	//     9
	swrst_bus10                     : 1;	//    10
} CPU_PERI_SUB_CRG_CTR36_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019094 ctr37 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_ahb0                      : 1,	//     0
	swrst_ahb1                      : 1,	//     1
	swrst_ahb2                      : 1,	//     2
	swrst_ahb3                      : 1,	//     3
	swrst_ahb4                      : 1,	//     4
	swrst_ahb5                      : 1,	//     5
	swrst_ahb6                      : 1,	//     6
	swrst_ahb7                      : 1,	//     7
	swrst_ahb8                      : 1,	//     8
	swrst_ahb9                      : 1,	//     9
	swrst_ahb10                     : 1,	//    10
	swrst_ahb11                     : 1,	//    11
	swrst_ahb12                     : 1,	//    12
	swrst_ahb13                     : 1,	//    13
	swrst_ahb14                     : 1,	//    14
	swrst_ahb15                     : 1,	//    15
	swrst_ahb16                     : 1,	//    16
	swrst_ahb17                     : 1,	//    17
	swrst_ahb18                     : 1,	//    18
	swrst_ahb19                     : 1,	//    19
	swrst_ahb20                     : 1,	//    20
	swrst_ahb21                     : 1,	//    21
	swrst_ahb22                     : 1,	//    22
	swrst_ahb23                     : 1;	//    23
} CPU_PERI_SUB_CRG_CTR37_M14B0;

/*-----------------------------------------------------------------------------
	0xc0019098 ctr38 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_apb0_0                    : 1,	//     0
	swrst_apb0_1                    : 1,	//     1
	swrst_apb0_2                    : 1,	//     2
	swrst_apb0_3                    : 1,	//     3
	swrst_apb0_4                    : 1,	//     4
	swrst_apb0_5                    : 1,	//     5
	swrst_apb0_6                    : 1,	//     6
	swrst_apb0_7                    : 1,	//     7
	swrst_apb0_8                    : 1,	//     8
	swrst_apb0_9                    : 1,	//     9
	swrst_apb0_10                   : 1;	//    10
} CPU_PERI_SUB_CRG_CTR38_M14B0;

/*-----------------------------------------------------------------------------
	0xc001909c ctr39 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_apb1_0                    : 1,	//     0
	swrst_apb1_1                    : 1,	//     1
	swrst_apb1_2                    : 1,	//     2
	swrst_apb1_3                    : 1,	//     3
	swrst_apb1_4                    : 1,	//     4
	swrst_apb1_5                    : 1,	//     5
	swrst_apb1_6                    : 1,	//     6
	swrst_apb1_7                    : 1,	//     7
	swrst_apb1_8                    : 1,	//     8
	swrst_apb1_9                    : 1,	//     9
	swrst_apb1_10                   : 1,	//    10
	swrst_apb1_11                   : 1,	//    11
	swrst_apb1_12                   : 1,	//    12
	swrst_apb1_13                   : 1,	//    13
	swrst_apb1_14                   : 1,	//    14
	swrst_apb1_15                   : 1,	//    15
	swrst_apb1_16                   : 1,	//    16
	swrst_apb1_17                   : 1,	//    17
	swrst_apb1_18                   : 1,	//    18
	swrst_apb1_19                   : 1,	//    19
	swrst_apb1_20                   : 1,	//    20
	swrst_apb1_21                   : 1;	//    21
} CPU_PERI_SUB_CRG_CTR39_M14B0;

typedef struct {
	CTOP_TOP_CTR00_M14B0            	ctr00                           ;	// 0xc0019000 : ''
	CTOP_TOP_CTR01_M14B0            	ctr01                           ;	// 0xc0019004 : ''
	CTOP_TOP_CTR02_M14B0            	ctr02                           ;	// 0xc0019008 : ''
	CTOP_TOP_CTR03_M14B0            	ctr03                           ;	// 0xc001900c : ''
	CTOP_TOP_CTR04_M14B0            	ctr04                           ;	// 0xc0019010 : ''
	CTOP_TOP_CTR05_M14B0            	ctr05                           ;	// 0xc0019014 : ''
	CTOP_TOP_CTR06_M14B0            	ctr06                           ;	// 0xc0019018 : ''
	CTOP_TOP_CTR07_M14B0            	ctr07                           ;	// 0xc001901c : ''
	CTOP_TOP_CTR08_M14B0            	ctr08                           ;	// 0xc0019020 : ''
	CTOP_TOP_CTR09_M14B0            	ctr09                           ;	// 0xc0019024 : ''
	CTOP_TOP_CTR10_M14B0            	ctr10                           ;	// 0xc0019028 : ''
	CTOP_TOP_CTR11_M14B0            	ctr11                           ;	// 0xc001902c : ''
	UINT32                          	ctr12                           ;	// 0xc0019030 : ''
	CTOP_TOP_CTR13_M14B0            	ctr13                           ;	// 0xc0019034 : ''
	CTOP_TOP_CTR14_M14B0            	ctr14                           ;	// 0xc0019038 : ''
	CTOP_TOP_CTR15_M14B0            	ctr15                           ;	// 0xc001903c : ''
	CTOP_TOP_CTR16_M14B0            	ctr16                           ;	// 0xc0019040 : ''
	CTOP_TOP_CTR17_M14B0            	ctr17                           ;	// 0xc0019044 : ''
	CTOP_TOP_CTR18_M14B0            	ctr18                           ;	// 0xc0019048 : ''
	CTOP_TOP_CTR19_M14B0            	ctr19                           ;	// 0xc001904c : ''
	CTOP_TOP_CTR20_M14B0            	ctr20                           ;	// 0xc0019050 : ''
	UINT32                          	ctr21                           ;	// 0xc0019054 : ''
	UINT32                          	ctr22                           ;	// 0xc0019058 : ''
	UINT32                          	ctr23                           ;	// 0xc001905c : ''
	CTOP_TOP_CTR24_M14B0            	ctr24                           ;	// 0xc0019060 : ''
	CTOP_TOP_CTR25_M14B0            	ctr25                           ;	// 0xc0019064 : ''
} CTOP_TOP_REG_M14B0_T;
/* 26 regs, 26 types */

typedef struct {
	CTOP_LEFT_CTR80_M14B0           	ctr80                           ;	// 0xc001fa00 : ''
	CTOP_LEFT_CTR81_M14B0           	ctr81                           ;	// 0xc001fa04 : ''
	CTOP_LEFT_CTR82_M14B0           	ctr82                           ;	// 0xc001fa08 : ''
	CTOP_LEFT_CTR83_M14B0           	ctr83                           ;	// 0xc001fa0c : ''
	CTOP_LEFT_CTR84_M14B0           	ctr84                           ;	// 0xc001fa10 : ''
	CTOP_LEFT_CTR85_M14B0           	ctr85                           ;	// 0xc001fa14 : ''
	CTOP_LEFT_CTR86_M14B0           	ctr86                           ;	// 0xc001fa18 : ''
	CTOP_LEFT_CTR87_M14B0           	ctr87                           ;	// 0xc001fa1c : ''
	CTOP_LEFT_CTR88_M14B0           	ctr88                           ;	// 0xc001fa20 : ''
	CTOP_LEFT_CTR89_M14B0           	ctr89                           ;	// 0xc001fa24 : ''
	CTOP_LEFT_CTR90_M14B0           	ctr90                           ;	// 0xc001fa28 : ''
	UINT32                          	ctr91                           ;	// 0xc001fa2c : ''
	CTOP_LEFT_CTR92_M14B0           	ctr92                           ;	// 0xc001fa30 : ''
	CTOP_LEFT_CTR93_M14B0           	ctr93                           ;	// 0xc001fa34 : ''
	CTOP_LEFT_CTR94_M14B0           	ctr94                           ;	// 0xc001fa38 : ''
	CTOP_LEFT_CTR95_M14B0           	ctr95                           ;	// 0xc001fa3c : ''
	CTOP_LEFT_CTR96_M14B0           	ctr96                           ;	// 0xc001fa40 : ''
	CTOP_LEFT_CTR97_M14B0           	ctr97                           ;	// 0xc001fa44 : ''
	CTOP_LEFT_CTR98_M14B0           	ctr98                           ;	// 0xc001fa48 : ''
	CTOP_LEFT_CTR99_M14B0           	ctr99                           ;	// 0xc001fa4c : ''
	CTOP_LEFT_CTR100_M14B0          	ctr100                          ;	// 0xc001fa50 : ''
	CTOP_LEFT_CTR101_M14B0          	ctr101                          ;	// 0xc001fa54 : ''
	CTOP_LEFT_CTR102_M14B0          	ctr102                          ;	// 0xc001fa58 : ''
	CTOP_LEFT_CTR103_M14B0          	ctr103                          ;	// 0xc001fa5c : ''
} CTOP_LEFT_REG_M14B0_T;
/* 24 regs, 24 types */

typedef struct {
	CTOP_RIGHT_CTR120_M14B0         	ctr120                          ;	// 0xc001cc00 : ''
	UINT32                          	ctr121                          ;	// 0xc001cc04 : ''
	CTOP_RIGHT_CTR122_M14B0         	ctr122                          ;	// 0xc001cc08 : ''
	UINT32                          	ctr123                          ;	// 0xc001cc0c : ''
	CTOP_RIGHT_CTR124_M14B0         	ctr124                          ;	// 0xc001cc10 : ''
	CTOP_RIGHT_CTR125_M14B0         	ctr125                          ;	// 0xc001cc14 : ''
	CTOP_RIGHT_CTR126_M14B0         	ctr126                          ;	// 0xc001cc18 : ''
	UINT32                          	ctr127                          ;	// 0xc001cc1c : ''
	UINT32                          	ctr128                          ;	// 0xc001cc20 : ''
	UINT32                          	ctr129                          ;	// 0xc001cc24 : ''
	CTOP_RIGHT_CTR130_M14B0         	ctr130                          ;	// 0xc001cc28 : ''
} CTOP_RIGHT_REG_M14B0_T;
/* 11 regs, 11 types */

typedef struct {
	CPU_CORE_SUB_CRG_CTR28_M14B0    	ctr28                           ;	// 0xfd300218 : ''
} CPU_CORE_SUB_CRG_REG_M14B0_T;
/* 1 regs, 1 types */

typedef struct {
	GFX_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc0019c00 : ''
	GFX_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc0019c04 : ''
} GFX_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	VDEC_SUB_CRG_CTR00_M14B0        	ctr00                           ;	// 0xc001a000 : ''
} VDEC_SUB_CRG_REG_M14B0_T;
/* 1 regs, 1 types */

typedef struct {
	HEVC_SUB_CRG_CTR00_M14B0        	ctr00                           ;	// 0xc001a400 : ''
} HEVC_SUB_CRG_REG_M14B0_T;
/* 1 regs, 1 types */

typedef struct {
	TE_SUB_CRG_CTR00_M14B0          	ctr00                           ;	// 0xc001a800 : ''
	TE_SUB_CRG_CTR01_M14B0          	ctr01                           ;	// 0xc001a804 : ''
} TE_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	VENC_SUB_CRG_CTR00_M14B0        	ctr00                           ;	// 0xc001ac00 : ''
	VENC_SUB_CRG_CTR01_M14B0        	ctr01                           ;	// 0xc001ac04 : ''
} VENC_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	AUD_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001b000 : ''
	AUD_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001b004 : ''
	AUD_SUB_CRG_CTR02_M14B0         	ctr02                           ;	// 0xc001b008 : ''
	AUD_SUB_CRG_CTR03_M14B0         	ctr03                           ;	// 0xc001b00c : ''
	AUD_SUB_CRG_CTR04_M14B0         	ctr04                           ;	// 0xc001b010 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0xc001b014
	AUD_SUB_CRG_CTR06_M14B0         	ctr06                           ;	// 0xc001b018 : ''
	AUD_SUB_CRG_CTR07_M14B0         	ctr07                           ;	// 0xc001b01c : ''
	AUD_SUB_CRG_CTR08_M14B0         	ctr08                           ;	// 0xc001b020 : ''
	AUD_SUB_CRG_CTR09_M14B0         	ctr09                           ;	// 0xc001b024 : ''
	AUD_SUB_CRG_CTR10_M14B0         	ctr10                           ;	// 0xc001b028 : ''
	AUD_SUB_CRG_CTR11_M14B0         	ctr11                           ;	// 0xc001b02c : ''
	AUD_SUB_CRG_CTR12_M14B0         	ctr12                           ;	// 0xc001b030 : ''
	AUD_SUB_CRG_CTR13_M14B0         	ctr13                           ;	// 0xc001b034 : ''
	AUD_SUB_CRG_CTR14_M14B0         	ctr14                           ;	// 0xc001b038 : ''
} AUD_SUB_CRG_REG_M14B0_T;
/* 14 regs, 14 types */

typedef struct {
	DEI_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001b400 : ''
	DEI_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001b404 : ''
	DEI_SUB_CRG_CTR02_M14B0         	ctr02                           ;	// 0xc001b408 : ''
	DEI_SUB_CRG_CTR03_M14B0         	ctr03                           ;	// 0xc001b40c : ''
} DEI_SUB_CRG_REG_M14B0_T;
/* 4 regs, 4 types */

typedef struct {
	GPU_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001b800 : ''
} GPU_SUB_CRG_REG_M14B0_T;
/* 1 regs, 1 types */

typedef struct {
	IDSMCU_SUB_CRG_CTR00_M14B0      	ctr00                           ;	// 0xc001c000 : ''
	IDSMCU_SUB_CRG_CTR01_M14B0      	ctr01                           ;	// 0xc001c004 : ''
} IDSMCU_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	DEM_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001c800 : ''
	DEM_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001c804 : ''
} DEM_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	DEO_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001d000 : ''
	DEO_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001d004 : ''
} DEO_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	DPP_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001d400 : ''
	DPP_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001d404 : ''
} DPP_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	FDA_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001d800 : ''
	FDA_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001d804 : ''
} FDA_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	FDB_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001e000 : ''
	FDB_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001e004 : ''
} FDB_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	FDC_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001e800 : ''
	FDC_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001e804 : ''
} FDC_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	FDD_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001f000 : ''
	FDD_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001f004 : ''
} FDD_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	DPE_SUB_CRG_CTR00_M14B0         	ctr00                           ;	// 0xc001f800 : ''
	DPE_SUB_CRG_CTR01_M14B0         	ctr01                           ;	// 0xc001f804 : ''
} DPE_SUB_CRG_REG_M14B0_T;
/* 2 regs, 2 types */

typedef struct {
	CPU_PERI_SUB_CRG_CTR35_M14B0    	ctr35                           ;	// 0xc001908c : ''
	CPU_PERI_SUB_CRG_CTR36_M14B0    	ctr36                           ;	// 0xc0019090 : ''
	CPU_PERI_SUB_CRG_CTR37_M14B0    	ctr37                           ;	// 0xc0019094 : ''
	CPU_PERI_SUB_CRG_CTR38_M14B0    	ctr38                           ;	// 0xc0019098 : ''
	CPU_PERI_SUB_CRG_CTR39_M14B0    	ctr39                           ;	// 0xc001909c : ''
} CPU_PERI_SUB_CRG_REG_M14B0_T;
/* 5 regs, 5 types */

/* 112 regs, 112 types in Total*/

/******* ADDED by hand *********/

typedef struct {
	CTOP_TOP_REG_M14B0_T			*TOP;
	CTOP_LEFT_REG_M14B0_T			*LEFT;
	CTOP_RIGHT_REG_M14B0_T			*RIGHT;
	GFX_SUB_CRG_REG_M14B0_T 		*GFX;
	VDEC_SUB_CRG_REG_M14B0_T		*VDEC;
	HEVC_SUB_CRG_REG_M14B0_T		*HEVC;
	TE_SUB_CRG_REG_M14B0_T			*TE;
	VENC_SUB_CRG_REG_M14B0_T		*VENC;
	AUD_SUB_CRG_REG_M14B0_T 		*AUD;
	DEI_SUB_CRG_REG_M14B0_T 		*DEI;
	GPU_SUB_CRG_REG_M14B0_T 		*GPU;
	IDSMCU_SUB_CRG_REG_M14B0_T		*IDSMCU;
	DEM_SUB_CRG_REG_M14B0_T 		*DEM;
	DEO_SUB_CRG_REG_M14B0_T 		*DEO;
	DPP_SUB_CRG_REG_M14B0_T 		*DPP;
	FDA_SUB_CRG_REG_M14B0_T 		*FDA;
	FDB_SUB_CRG_REG_M14B0_T 		*FDB;
	FDC_SUB_CRG_REG_M14B0_T 		*FDC;
	FDD_SUB_CRG_REG_M14B0_T 		*FDD;
	DPE_SUB_CRG_REG_M14B0_T 		*DPE;
	CPU_PERI_SUB_CRG_REG_M14B0_T	*CPU_PERI;
} CTOP_REG_M14B0_T;

#define M14_B0_CTOP_TOP_BASE		0xC0019000
#define M14_B0_CTOP_LEFT_BASE		0xC001FA00
#define M14_B0_CTOP_RIGHT_BASE		0xC001CC00
#define M14_B0_CTOP_GFX_BASE		0xC0019C00
#define M14_B0_CTOP_VDEC_BASE		0xC001A000
#define M14_B0_CTOP_HEVC_BASE		0xC001A400
#define M14_B0_CTOP_TE_BASE			0xC001A800
#define M14_B0_CTOP_VENC_BASE		0xC001AC00
#define M14_B0_CTOP_AUD_BASE		0xC001B000
#define M14_B0_CTOP_DEI_BASE		0xC001B400
#define M14_B0_CTOP_GPU_BASE		0xC001B800
#define M14_B0_CTOP_IDSMCU_BASE		0xC001C000
#define M14_B0_CTOP_DEM_BASE		0xC001C800
#define M14_B0_CTOP_DEO_BASE		0xC001D000
#define M14_B0_CTOP_DPP_BASE		0xC001D400
#define M14_B0_CTOP_FDA_BASE		0xC001D800
#define M14_B0_CTOP_FDB_BASE		0xC001E000
#define M14_B0_CTOP_FDC_BASE		0xC001E800
#define M14_B0_CTOP_FDD_BASE		0xC001F000
#define M14_B0_CTOP_DPE_BASE		0xC001F800
#define M14_B0_CTOP_CPU_PERI_BASE	0xC001908C

#define M14_B0_CTOP_TOP_TYPE		CTOP_TOP_REG_M14B0_T
#define M14_B0_CTOP_LEFT_TYPE		CTOP_LEFT_REG_M14B0_T
#define M14_B0_CTOP_RIGHT_TYPE		CTOP_RIGHT_REG_M14B0_T
#define M14_B0_CTOP_GFX_TYPE		GFX_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_VDEC_TYPE		VDEC_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_HEVC_TYPE		HEVC_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_TE_TYPE			TE_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_VENC_TYPE		VENC_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_AUD_TYPE		AUD_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_DEI_TYPE		DEI_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_GPU_TYPE		GPU_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_IDSMCU_TYPE		IDSMCU_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_DEM_TYPE		DEM_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_DEO_TYPE		DEO_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_DPP_TYPE		DPP_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_FDA_TYPE		FDA_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_FDB_TYPE		FDB_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_FDC_TYPE		FDC_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_FDD_TYPE		FDD_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_DPE_TYPE		DPE_SUB_CRG_REG_M14B0_T
#define M14_B0_CTOP_CPU_PERI_TYPE	CPU_PERI_SUB_CRG_REG_M14B0_T

#endif	/* __CTOP_CTRL_REG_M14B0_H__ */

