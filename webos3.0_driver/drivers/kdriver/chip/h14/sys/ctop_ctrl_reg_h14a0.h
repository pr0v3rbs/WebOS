#ifndef __CTOP_CTRL_REG_H14A0_H__
#define __CTOP_CTRL_REG_H14A0_H__


/*-----------------------------------------------------------------------------
	0x0000 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	link_sys_off_sel                : 1,	//     0
	swrst_cpu_bus                   : 1,	//     1
	swrst_cci                       : 1,	//     2
	swrst_apb4                      : 1,	//     3
	swrst_apb                       : 1,	//     4
	swrst_srv                       : 1,	//     5
	swrst_bus                       : 1,	//     6
	reg_afifo_r                     : 1,	//     7
	reg_aud                         : 1,	//     8
	reg_vfifo_r                     : 1,	//     9
	reg_pix_pip                     : 1,	//    10
	reg_afifo_w                     : 1,	//    11
	reg_vfifo_w                     : 1,	//    12
	reg_tmds                        : 1,	//    13
	swrst_hdcp                      : 1,	//    14
	swrst_sys                       : 1,	//    15
	swrst_hdmi_dto                  : 1,	//    16
	swrst_adto                      : 1,	//    17
	swrst_hdp                       : 1,	//    18
	swrst_cpubus                    : 1,	//    19
	swrst_gpu                       : 1,	//    20
	swrst_gfx                       : 1,	//    21
	swrst_codec                     : 1,	//    22
	swrst_disp                      : 1,	//    23
	swrst_m1                        : 1,	//    24
	swrst_m0                        : 1,	//    25
	swrst_m1_d100                   : 1,	//    26
	swrst_m0_d100                   : 1,	//    27
	swrst_usb_bt_init               : 1,	//    28
	swrst_usb2_init                 : 1,	//    29
	swrst_usb1_init                 : 1,	//    30
	swrst_usb0_init                 : 1;	//    31
} CTR00_H14A0;

/*-----------------------------------------------------------------------------
	0x0004 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sel_sc_27m                      : 2,	//  0: 1
	f24_sel                         : 1,	//     2
	f27_18_sel                      : 1,	//     3
	sc_ext_sel                      : 3,	//  4: 6
	ext_en_i                        : 1,	//     7
	dco_sel                         : 1,	//     8
	adder_value                     : 8,	//  9:16
	adder_div_en                    : 1,	//    17
	apb4_clk_sel                    : 3,	// 18:20
	apb_clk_sel                     : 3,	// 21:23
	mmur456reg_clk_sel              : 1,	//    24
	mmureg_clk_sel                  : 2,	// 25:26
	bus_clk_sel                     : 3,	// 27:29
	phy_i2c_clk_sel                 : 1,	//    30
	aud_hdmi_mclk_sel               : 1;	//    31
} CTR01_H14A0;

/*-----------------------------------------------------------------------------
	0x0008 ctr02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :29,	//  0:28 reserved
	swrst_gpu_mem                   : 1,	//    29
	swrst_gpu_core                  : 1,	//    30
	swrst_gpu_sys                   : 1;	//    31
} CTR02_H14A0;

/*-----------------------------------------------------------------------------
	0x000c ctr03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adder_value                     : 8,	//  0: 7
	adder_div_en                    : 1,	//     8
	                                :19,	//  9:27 reserved
	gpu_sys_clk_sel                 : 2;	// 28:29
} CTR03_H14A0;

/*-----------------------------------------------------------------------------
	0x0010 ctr04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	xtal_ds0                        : 1,	//     0
	xtal_ds1                        : 1,	//     1
	                                : 5,	//  2: 6 reserved
	swrst_gfxa                      : 1,	//     7
	swrst_gfx                       : 1,	//     8
	reg_sys                         : 1,	//     9
	swrst_vd_te                     : 1,	//    10
	swrst_vda                       : 1,	//    11
	swrst_vd                        : 1,	//    12
	adder_value                     : 8,	// 13:20
	adder_div_en                    : 1,	//    21
	reg_s_te                        : 1,	//    22
	reg_swrst_se                    : 1,	//    23
	swrst_icoda                     : 1,	//    24
	swrst_icod                      : 1,	//    25
	swrst_teah                      : 1,	//    26
	swrst_te_te1                    : 1,	//    27
	swrst_te_te                     : 1,	//    28
	swrst_te_ve                     : 1,	//    29
	swrst_vea                       : 1,	//    30
	swrst_ve                        : 1;	//    31
} CTR04_H14A0;

/*-----------------------------------------------------------------------------
	0x0014 ctr05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	vd_teclk_sel                    : 3,	//  2: 4
	                                : 1,	//     5 reserved
	vd_aclk_sel                     : 3,	//  6: 8
	                                :20,	//  9:28 reserved
	aud_teclk_sel                   : 3;	// 29:31
} CTR05_H14A0;

/*-----------------------------------------------------------------------------
	0x0018 ctr06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adder_value                     : 8,	//  0: 7
	adder_div_en                    : 1,	//     8
	                                :10,	//  9:18 reserved
	sel_seclk                       : 2,	// 19:20
	icodclk_sel                     : 3,	// 21:23
	                                : 2,	// 24:25 reserved
	te_teclk_sel                    : 3,	// 26:28
	veclk_sel                       : 2,	// 29:30
	reg_seclk_inv                   : 1;	//    31
} CTR06_H14A0;

/*-----------------------------------------------------------------------------
	0x001c ctr07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
                                    : 4,	//  0: 3 reserved
	gfx_aclk_sel                    : 2,	//  4: 5
                                    :21,	//  6:26 reserved
	swrst_gfx                       : 1,	//    27
                                    : 3,	// 28:30 reserved
	swrst_gfx_a                     : 1;	//    31
} CTR07_H14A0;

/*-----------------------------------------------------------------------------
	0x0020 ctr08 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0024 ctr09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	isol_en_sel                     : 3,	//  1: 3
	                                : 5,	//  4: 8 reserved
	ahbclken_sel                    : 3,	//  9:11
	                                : 5,	// 12:16 reserved
	swrst_ptmpg                     : 1,	//    17
	swrst_ptm                       : 1;	//    18
} CTR09_H14A0;

/*-----------------------------------------------------------------------------
	0x0028 ctr10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :18,	//  0:17 reserved
	div3clk_sel                     : 3,	// 18:20
	cm3clk_sel                      : 3,	// 21:23
	                                : 1,	//    24 reserved
	emmcclk_sel                     : 3,	// 25:27
	                                : 1,	//    28 reserved
	qrtclk_sel                      : 3;	// 29:31
} CTR10_H14A0;

/*-----------------------------------------------------------------------------
	0x002c ctr11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_cm3_soft                  : 1,	//     0
	swrst_cm3                       : 1,	//     1
	                                : 3,	//  2: 4 reserved
	cm50m_sel                       : 3,	//  5: 7
	                                :21,	//  8:28 reserved
	qrtinv_sel                      : 3;	// 29:31
} CTR11_H14A0;

/*-----------------------------------------------------------------------------
	0x0030 ctr12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	swrst_resetsync                 : 1,	//     7
	swrst_presetdbg                 : 1,	//     8
	swrst_cntreset                  : 1,	//     9
	swrst_atreset                   : 1,	//    10
	swrst_gem_rmii_ref              : 1,	//    11
	swrst_gem_rgmii_rx_n            : 1,	//    12
	swrst_gem_rgmii_tx_n            : 1,	//    13
	swrst_gem_rx                    : 1,	//    14
	swrst_gem_tx                    : 1,	//    15
	                                : 1,	//    16 reserved
	suspend3_sel                    : 3,	// 17:19
	                                : 5,	// 20:24 reserved
	suspend1_sel                    : 3,	// 25:27
	                                : 1,	//    28 reserved
	suspend0_sel                    : 3;	// 29:31
} CTR12_H14A0;

/*-----------------------------------------------------------------------------
	0x0034 ctr13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_ahb31                     : 1,	//     0
	swrst_ahb30                     : 1,	//     1
	swrst_ahb29                     : 1,	//     2
	swrst_ahb28                     : 1,	//     3
	swrst_ahb27                     : 1,	//     4
	swrst_ahb26                     : 1,	//     5
	swrst_ahb25                     : 1,	//     6
	swrst_ahb24                     : 1,	//     7
	swrst_ahb23                     : 1,	//     8
	swrst_ahb22                     : 1,	//     9
	swrst_ahb21                     : 1,	//    10
	swrst_ahb20                     : 1,	//    11
	swrst_ahb19                     : 1,	//    12
	swrst_ahb18                     : 1,	//    13
	swrst_ahb17                     : 1,	//    14
	swrst_ahb16                     : 1,	//    15
	swrst_ahb15                     : 1,	//    16
	swrst_ahb14                     : 1,	//    17
	swrst_ahb13                     : 1,	//    18
	swrst_ahb12                     : 1,	//    19
	swrst_ahb11                     : 1,	//    20
	swrst_ahb10                     : 1,	//    21
	swrst_ahb9                      : 1,	//    22
	swrst_ahb8                      : 1,	//    23
	swrst_ahb7                      : 1,	//    24
	swrst_ahb6                      : 1,	//    25
	swrst_ahb5                      : 1,	//    26
	swrst_ahb4                      : 1,	//    27
	swrst_ahb3                      : 1,	//    28
	swrst_ahb2                      : 1,	//    29
	swrst_ahb1                      : 1,	//    30
	swrst_ahb0                      : 1;	//    31
} CTR13_H14A0;

/*-----------------------------------------------------------------------------
	0x0038 ctr14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :16,	//  0:15 reserved
	swrst_usbbus                    : 1,	//    16
	swrst_ahb46                     : 1,	//    17
	swrst_ahb45                     : 1,	//    18
	swrst_ahb44                     : 1,	//    19
	swrst_ahb43                     : 1,	//    20
	swrst_ahb42                     : 1,	//    21
	swrst_ahb41                     : 1,	//    22
	swrst_ahb40                     : 1,	//    23
	swrst_ahb39                     : 1,	//    24
	swrst_ahb38                     : 1,	//    25
	swrst_ahb37                     : 1,	//    26
	swrst_ahb36                     : 1,	//    27
	swrst_ahb35                     : 1,	//    28
	swrst_ahb34                     : 1,	//    29
	swrst_ahb33                     : 1,	//    30
	swrst_ahb32                     : 1;	//    31
} CTR14_H14A0;

/*-----------------------------------------------------------------------------
	0x003c ctr15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :14,	//  0:13 reserved
	atclken_sel                     : 3,	// 14:16
	rxclk_sel                       : 1,	//    16
	trace_sel                       : 3,	// 17:19
	rx_clkoff_sel                   : 1,	//    18
	rmii_clkoff_sel                 : 1,	//    19
	                                : 1,	//    20 reserved
	halfclk_sel                     : 3,	// 21:23
	                                : 1,	//    24 reserved
	otclk_sel                       : 3;	// 25:27
} CTR15_H14A0;

/*-----------------------------------------------------------------------------
	0x0040 ctr16 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0044 ctr17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :20,	//  0:19 reserved
	swrst_apb0_10                   : 1,	//    20
	swrst_bus10                     : 1,	//    21
	swrst_bus9                      : 1,	//    22
	swrst_bus8                      : 1,	//    23
	swrst_bus7                      : 1,	//    24
	swrst_bus6                      : 1,	//    25
	swrst_bus5                      : 1,	//    26
	swrst_bus4                      : 1,	//    27
	swrst_bus3                      : 1,	//    28
	swrst_bus2                      : 1,	//    29
	swrst_bus1                      : 1,	//    30
	swrst_bus0                      : 1;	//    31
} CTR17_H14A0;

/*-----------------------------------------------------------------------------
	0x0048 ctr18 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x004c ctr19 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0050 ctr20 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0054 ctr21 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0058 ctr22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_apb1_21                   : 1,	//     0
	swrst_apb1_20                   : 1,	//     1
	swrst_apb1_19                   : 1,	//     2
	swrst_apb1_18                   : 1,	//     3
	swrst_apb1_17                   : 1,	//     4
	swrst_apb1_16                   : 1,	//     5
	swrst_apb1_15                   : 1,	//     6
	swrst_apb1_14                   : 1,	//     7
	swrst_apb1_13                   : 1,	//     8
	swrst_apb1_12                   : 1,	//     9
	swrst_apb1_11                   : 1,	//    10
	swrst_apb1_10                   : 1,	//    11
	swrst_apb1_9                    : 1,	//    12
	swrst_apb1_8                    : 1,	//    13
	swrst_apb1_7                    : 1,	//    14
	swrst_apb1_6                    : 1,	//    15
	swrst_apb1_5                    : 1,	//    16
	swrst_apb1_4                    : 1,	//    17
	swrst_apb1_3                    : 1,	//    18
	swrst_apb1_2                    : 1,	//    19
	swrst_apb1_1                    : 1,	//    20
	swrst_apb1_0                    : 1,	//    21
	swrst_apb0_9                    : 1,	//    22
	swrst_apb0_8                    : 1,	//    23
	swrst_apb0_7                    : 1,	//    24
	swrst_apb0_6                    : 1,	//    25
	swrst_apb0_5                    : 1,	//    26
	swrst_apb0_4                    : 1,	//    27
	swrst_apb0_3                    : 1,	//    28
	swrst_apb0_2                    : 1,	//    29
	swrst_apb0_1                    : 1,	//    30
	swrst_apb0_0                    : 1;	//    31
} CTR22_H14A0;

/*-----------------------------------------------------------------------------
	0x005c ctr23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tcon_osd_clk_sel                : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_tcon_video                  : 1,	//     4
	reg_tcon_osd                    : 1,	//     5
	reg_tcon_pix                    : 1,	//     6
	reg_tcon_pix2                   : 1,	//     7
	tcon_pix2_clk_sel               : 2,	//  8: 9
	reg_disp_ded_p                  : 1,	//    10
	reg_disp_ded                    : 1,	//    11
	reg_disp_dp                     : 1,	//    12
	reg_disp_de                     : 1,	//    13
	reg_dppa_det                    : 1,	//    14
	reg_dppa_pe1                    : 1,	//    15
	reg_dppa_osd                    : 1,	//    16
	reg_dppa_osd60hz                : 1,	//    17
	reg_dppb                        : 1,	//    18
	reg_dppb_if                     : 1,	//    19
	reg_dppa                        : 1,	//    20
	reg_sre_det                     : 1,	//    21
	reg_sre_sys                     : 1,	//    22
	reg_frc_dvi_det                 : 1,	//    23
	reg_frc_cho_det                 : 1,	//    24
	reg_frc_dvi                     : 1,	//    25
	reg_frc_cho                     : 1,	//    26
	reg_frc_dvo                     : 1,	//    27
	reg_frc_dvo_ax                  : 1,	//    28
	reg_h3d_det                     : 1,	//    29
	reg_h3d                         : 1,	//    30
	reg_slt                         : 1;	//    31
} CTR23_H14A0;

/*-----------------------------------------------------------------------------
	0x0060 ctr24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 5,	//  0: 4 reserved
	dppb_if_clk_sel                 : 2,	//  5: 6
	dppb_det_clk_sel                : 2,	//  7: 8
	dppa_pe1_clk_sel                : 2,	//  9:10
	dppa_osd60hz_sel                : 2,	// 11:12
	dppa_osd_clk_sel                : 2,	// 13:14
	dppa_clk_sel                    : 2,	// 15:16
	sre_sys_clk_sel                 : 2,	// 17:18
	frc_dvi_clk_sel                 : 2,	// 19:20
	frc_cho_clk_sel                 : 2,	// 21:22
	frc_dvo_clk_sel                 : 2,	// 23:24
	frc_dvo_axi_clk_sel             : 2,	// 25:26
	h3d_clk_sel                     : 2,	// 27:28
	tx_combo_sel                    : 3;	// 29:31
} CTR24_H14A0;

/*-----------------------------------------------------------------------------
	0x0064 ctr25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	reg_swrst_lvds                  : 1,	//     1
	reg_swrst_hdmi_h                : 1,	//     2
	reg_swrst_hdmi                  : 1,	//     3
	swrst_dee_de_dp                 : 1,	//     4
	swrst_dee_de                    : 1,	//     5
	swrst_cvda                      : 1,	//     6
	reg_swrst_ch3                   : 1,	//     7
	reg_swrst_scart                 : 1,	//     8
	reg_swrst_vbi                   : 1,	//     9
	reg_swrst_cvbs27                : 1,	//    10
	reg_swrst_cvbs54                : 1,	//    11
	swrst_dec_ssc_de                : 1,	//    12
	swrst_dec_pe0_de                : 1,	//    13
	swrst_dec_mif_de                : 1,	//    14
	swrst_dec_mif_dea               : 1,	//    15
	reg_swrst_cve27                 : 1,	//    16
	reg_swrst_cve54                 : 1,	//    17
	swrst_ded_de_dp                 : 1,	//    18
	swrst_ded_de                    : 1,	//    19
	swrst_ded_dea_dp                : 1,	//    20
	swrst_ded_dea                   : 1,	//    21
	swrst_deb_de_dp                 : 1,	//    22
	swrst_deb_de                    : 1,	//    23
	swrst_deb_dea_dp                : 1,	//    24
	swrst_deb_dea                   : 1,	//    25
	swrst_dea_de                    : 1,	//    26
	swrst_dea_dea                   : 1,	//    27
	reg_swrst_chb                   : 1,	//    28
	reg_swrst_cha                   : 1,	//    29
	reg_swrst_extb                  : 1,	//    30
	reg_swrst_exta                  : 1;	//    31
} CTR25_H14A0;

/*-----------------------------------------------------------------------------
	0x0068 ctr26 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	deb_dea_clk_sel                 : 3,	//  3: 5
	                                : 3,	//  6: 8 reserved
	dea_dea_clk_sel                 : 3,	//  9:11
	                                : 3,	// 12:14 reserved
	ssclk_sel                       : 2,	// 15:16
	extb_sel                        : 2,	// 17:18
	                                : 1,	//    19 reserved
	msclk_sel                       : 2,	// 20:21
	exta_sel                        : 2,	// 22:23
	lvds_clk_sel                    : 1,	//    24
	cve54m_inv_sel                  : 1,	//    25
	cvd_cve_sel                     : 1,	//    26
	cvd54_sel                       : 1,	//    27
	                                : 2,	// 28:29 reserved
	phy_pll_sel                     : 2;	// 30:31
} CTR26_H14A0;

/*-----------------------------------------------------------------------------
	0x006c ctr27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	cvda_clk_sel                    : 3,	//  8:10
	                                :13,	// 11:23 reserved
	cve27_inv_sel                   : 1,	//    24
	cve54_inv_sel                   : 1,	//    25
	del_ded_cve27                   : 2;	// 26:27
} CTR27_H14A0;

/*-----------------------------------------------------------------------------
	0x0070 ctr28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	aclkens_sel                     : 1,	//     0
	aclkenm_sel                     : 1,	//     1
	                                : 4,	//  2: 5 reserved
	swrst_presetdbgcore             : 1,	//     6
	swrst_creset                    : 1,	//     7
	swrst_atresetcore0              : 1,	//     8
	swrst_atresetcore1              : 1,	//     9
	reg_pll_xtal_sel                : 1,	//    10
	reg_l2_reset                    : 1,	//    11
	reg_dbg1_reset                  : 1,	//    12
	reg_dbg0_reset                  : 1,	//    13
	                                : 2,	// 14:15 reserved
	reg_cx1_reset                   : 1,	//    16
	reg_cx0_reset                   : 1,	//    17
	reg_core1_reset                 : 1,	//    18
	reg_core0_reset                 : 1,	//    19
	reg_cpu1_reset                  : 1,	//    20
	reg_cpu0_reset                  : 1,	//    21
	periphclken_sel                 : 2,	// 22:23
	atclkencore_sel                 : 1,	//    24
	                                : 1,	//    25 reserved
	dbgencore_sel                   : 1,	//    26
	dbgen_sel                       : 1,	//    27
	                                : 2,	// 28:29 reserved
	a_stdby_cpu                     : 2;	// 30:31
} CTR28_H14A0;

/*-----------------------------------------------------------------------------
	0x0074 ctr29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :13,	//  0:12 reserved
	swrst_tcon_apb                  : 1,	//    13
	swrst_tcon_axi                  : 1,	//    14
	swrst_vd_de                     : 1,	//    15
	swrst_dppb_cpu                  : 1,	//    16
	swrst_dppb_axi                  : 1,	//    17
	swrst_dppb_apb                  : 1,	//    18
	swrst_dppa_axi                  : 1,	//    19
	swrst_dppa_apb                  : 1,	//    20
	swrst_sre_axi                   : 1,	//    21
	swrst_sre_apb                   : 1,	//    22
	swrst_frc_sys                   : 1,	//    23
	swrst_frc_mc_axi                : 1,	//    24
	swrst_frc_me_axi                : 1,	//    25
	swrst_frc_dvi_axi               : 1,	//    26
	swrst_frc_apb                   : 1,	//    27
	swrst_frc_mc                    : 1,	//    28
	swrst_frc_mb                    : 1,	//    29
	swrst_frc_ma                    : 1,	//    30
	swrst_h3d_apb                   : 1;	//    31
} CTR29_H14A0;

/*-----------------------------------------------------------------------------
	0x0078 ctr30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	sre_apb_sel                     : 3,	//  8:10
	                                : 3,	// 11:13 reserved
	frc_apb_sel                     : 3,	// 14:16
	frc_mc_axi_sel                  : 2,	// 17:18
	frc_mc_sel                      : 2,	// 19:20
	frc_me_axi_sel                  : 2,	// 21:22
	frc_mb_sel                      : 2,	// 23:24
	                                : 1,	//    25 reserved
	frc_ma_sel                      : 2,	// 26:27
	                                : 1,	//    28 reserved
	h3d_apb_sel                     : 3;	// 29:31
} CTR30_H14A0;

/*-----------------------------------------------------------------------------
	0x007c ctr31 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ext_intr_pol_sel0               : 1,	//     0
	ext_intr_pol_sel1               : 1,	//     1
	ext_intr_pol_sel2               : 1,	//     2
	ext_intr_pol_sel3               : 1,	//     3
	cam_vcc_pol                     : 1,	//     4
	div_dcoin                       : 2,	//  5: 6
	                                : 7,	//  7:13 reserved
	tcon_axi_sel                    : 3,	// 14:16
	vd_declk_sel                    : 3,	// 17:19
	                                : 6,	// 20:25 reserved
	dppb_apb_sel                    : 3;	// 26:28
} CTR31_H14A0;

/*-----------------------------------------------------------------------------
	0x0080 ctr32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 6,	//  0: 5 reserved
	frc_div_sel                     : 2,	//  6: 7
	pll_sel_frc                     : 2,	//  8: 9
	c456_div_sel                    : 2,	// 10:11
	pll_sel_c456                    : 2,	// 12:13
	aud_div_sel                     : 2,	// 14:15
	pll_sel_aud                     : 2,	// 16:17
	gfx_div_sel                     : 2,	// 18:19
	pll_sel_gfx                     : 2,	// 20:21
	gpu_div_sel                     : 2,	// 22:23
	pll_sel_gpu                     : 2,	// 24:25
	pll_sel_core                    : 2,	// 26:27
	pll_sel_m1                      : 2,	// 28:29
	pll_sel_m0                      : 2;	// 30:31
} CTR32_H14A0;

/*-----------------------------------------------------------------------------
	0x0084 ctr33 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gp7_0_mux_en                    : 1,	//     0
	gp7_1_mux_en                    : 1,	//     1
	gp7_2_mux_en                    : 1,	//     2
	gp7_3_mux_en                    : 1,	//     3
	gp7_4_mux_en                    : 1,	//     4
	gp7_5_mux_en                    : 1,	//     5
	gp7_6_mux_en                    : 1,	//     6
	gp7_7_mux_en                    : 1,	//     7
	gp6_0_mux_en                    : 1,	//     8
	gp6_1_mux_en                    : 1,	//     9
	gp6_2_mux_en                    : 1,	//    10
	gp6_3_mux_en                    : 1,	//    11
	gp6_4_mux_en                    : 1,	//    12
	gp6_5_mux_en                    : 1,	//    13
	gp6_6_mux_en                    : 1,	//    14
	gp6_7_mux_en                    : 1,	//    15
	gp5_0_mux_en                    : 1,	//    16
	gp5_1_mux_en                    : 1,	//    17
	gp5_2_mux_en                    : 1,	//    18
	gp5_3_mux_en                    : 1,	//    19
	gp5_4_mux_en                    : 1,	//    20
	gp5_5_mux_en                    : 1,	//    21
	gp5_6_mux_en                    : 1,	//    22
	gp5_7_mux_en                    : 1,	//    23
	gp4_0_mux_en                    : 1,	//    24
	gp4_1_mux_en                    : 1,	//    25
	gp4_2_mux_en                    : 1,	//    26
	gp4_3_mux_en                    : 1,	//    27
	gp4_4_mux_en                    : 1,	//    28
	gp4_5_mux_en                    : 1,	//    29
	gp4_6_mux_en                    : 1,	//    30
	gp4_7_mux_en                    : 1;	//    31
} CTR33_H14A0;

/*-----------------------------------------------------------------------------
	0x0088 ctr34 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ds_eb7                          : 2,	//  0: 1
	ds_eb6                          : 2,	//  2: 3
	ds_eb5                          : 2,	//  4: 5
	ds_eb4                          : 2,	//  6: 7
	ds_eb3                          : 2,	//  8: 9
	ds_eb2                          : 2,	// 10:11
	ds_eb1                          : 2,	// 12:13
	ds_eb0                          : 2,	// 14:15
	ds_tp7                          : 2,	// 16:17
	ds_tp6                          : 2,	// 18:19
	ds_tp5                          : 2,	// 20:21
	ds_tp4                          : 2,	// 22:23
	ds_tp3                          : 2,	// 24:25
	ds_tp2                          : 2,	// 26:27
	ds_tp1                          : 2,	// 28:29
	ds_tp0                          : 2;	// 30:31
} CTR34_H14A0;

/*-----------------------------------------------------------------------------
	0x008c ctr35 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :10,	//  0: 9 reserved
	ds_uart2                        : 2,	// 10:11
	ds_uart1                        : 2,	// 12:13
	ds_uart0                        : 2,	// 14:15
	ds_cd2                          : 2,	// 16:17
	ds_cd1                          : 2,	// 18:19
	ds_vs2                          : 2,	// 20:21
	ds_vs1                          : 2,	// 22:23
	ds_iois                         : 2,	// 24:25
	ds_wait                         : 2,	// 26:27
	ds_inpack                       : 2,	// 28:29
	ds_ireq                         : 2;	// 30:31
} CTR35_H14A0;

/*-----------------------------------------------------------------------------
	0x0090 ctr36 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gpu_ts_cal                      : 1,	//     0
	                                : 3,	//  1: 3 reserved
	cpu_ts_cal                      : 1,	//     4
	                                : 3,	//  5: 7 reserved
	ds_sda5                         : 2,	//  8: 9
	ds_scl5                         : 2,	// 10:11
	ds_sda4                         : 2,	// 12:13
	ds_scl4                         : 2,	// 14:15
	ds_sda3                         : 2,	// 16:17
	ds_scl3                         : 2,	// 18:19
	ds_sda2                         : 2,	// 20:21
	ds_scl2                         : 2,	// 22:23
	ds_sda1                         : 2,	// 24:25
	ds_scl1                         : 2,	// 26:27
	ds_sda0                         : 2,	// 28:29
	ds_scl0                         : 2;	// 30:31
} CTR36_H14A0;

/*-----------------------------------------------------------------------------
	0x0094 ctr37 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gp11_0_mux_en                   : 1,	//     0
	gp11_1_mux_en                   : 1,	//     1
	gp11_2_mux_en                   : 1,	//     2
	gp11_3_mux_en                   : 1,	//     3
	gp11_4_mux_en                   : 1,	//     4
	gp11_5_mux_en                   : 1,	//     5
	gp11_6_mux_en                   : 1,	//     6
	gp11_7_mux_en                   : 1,	//     7
	gp10_0_mux_en                   : 1,	//     8
	gp10_1_mux_en                   : 1,	//     9
	gp10_2_mux_en                   : 1,	//    10
	gp10_3_mux_en                   : 1,	//    11
	gp10_4_mux_en                   : 1,	//    12
	gp10_5_mux_en                   : 1,	//    13
	gp10_6_mux_en                   : 1,	//    14
	gp10_7_mux_en                   : 1,	//    15
	gp9_0_mux_en                    : 1,	//    16
	gp9_1_mux_en                    : 1,	//    17
	gp9_2_mux_en                    : 1,	//    18
	gp9_3_mux_en                    : 1,	//    19
	gp9_4_mux_en                    : 1,	//    20
	gp9_5_mux_en                    : 1,	//    21
	gp9_6_mux_en                    : 1,	//    22
	gp9_7_mux_en                    : 1,	//    23
	gp8_0_mux_en                    : 1,	//    24
	gp8_1_mux_en                    : 1,	//    25
	gp8_2_mux_en                    : 1,	//    26
	gp8_3_mux_en                    : 1,	//    27
	gp8_4_mux_en                    : 1,	//    28
	gp8_5_mux_en                    : 1,	//    29
	gp8_6_mux_en                    : 1,	//    30
	gp8_7_mux_en                    : 1;	//    31
} CTR37_H14A0;

/*-----------------------------------------------------------------------------
	0x0098 ctr38 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ds_gpio15                       : 2,	//  0: 1
	ds_gpio14                       : 2,	//  2: 3
	ds_gpio13                       : 2,	//  4: 5
	ds_gpio12                       : 2,	//  6: 7
	ds_gpio11                       : 2,	//  8: 9
	ds_gpio10                       : 2,	// 10:11
	ds_gpio9                        : 2,	// 12:13
	ds_gpio8                        : 2,	// 14:15
	ds_gpio7                        : 2,	// 16:17
	ds_gpio6                        : 2,	// 18:19
	ds_gpio5                        : 2,	// 20:21
	ds_gpio4                        : 2,	// 22:23
	ds_gpio3                        : 2,	// 24:25
	ds_gpio2                        : 2,	// 26:27
	ds_gpio1                        : 2,	// 28:29
	ds_gpio0                        : 2;	// 30:31
} CTR38_H14A0;

/*-----------------------------------------------------------------------------
	0x009c ctr39 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ds_gpio31                       : 2,	//  0: 1
	ds_gpio30                       : 2,	//  2: 3
	ds_gpio29                       : 2,	//  4: 5
	ds_gpio28                       : 2,	//  6: 7
	ds_gpio27                       : 2,	//  8: 9
	ds_gpio26                       : 2,	// 10:11
	ds_gpio25                       : 2,	// 12:13
	ds_gpio24                       : 2,	// 14:15
	ds_gpio23                       : 2,	// 16:17
	ds_gpio22                       : 2,	// 18:19
	ds_gpio21                       : 2,	// 20:21
	ds_gpio20                       : 2,	// 22:23
	ds_gpio19                       : 2,	// 24:25
	ds_gpio18                       : 2,	// 26:27
	ds_gpio17                       : 2,	// 28:29
	ds_gpio16                       : 2;	// 30:31
} CTR39_H14A0;

/*-----------------------------------------------------------------------------
	0x00a0 ctr40 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :20,	//  0:19 reserved
	ds_ddcda                        : 2,	// 20:21
	ds_ddcck                        : 2,	// 22:23
	ds_gpio35                       : 2,	// 24:25
	ds_gpio34                       : 2,	// 26:27
	ds_gpio33                       : 2,	// 28:29
	ds_gpio32                       : 2;	// 30:31
} CTR40_H14A0;

/*-----------------------------------------------------------------------------
	0x00a4 ctr41 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :18,	//  0:17 reserved
	gp12_0_mux_en                   : 1,	//    18
	gp12_1_mux_en                   : 1,	//    19
	gp12_2_mux_en                   : 1,	//    20
	gp12_3_mux_en                   : 1,	//    21
	gp12_4_mux_en                   : 1,	//    22
	gp12_5_mux_en                   : 1,	//    23
	gp12_6_mux_en                   : 1,	//    24
	gp12_7_mux_en                   : 1;	//    25
} CTR41_H14A0;

/*-----------------------------------------------------------------------------
	0x00a8 ctr42 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp13_0_mux_en                   : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp13_1_mux_en                   : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp13_2_mux_en                   : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp13_3_mux_en                   : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp13_4_mux_en                   : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp13_5_mux_en                   : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp13_6_mux_en                   : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp13_7_mux_en                   : 1;	//    31
} CTR42_H14A0;

/*-----------------------------------------------------------------------------
	0x00ac ctr43 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp14_0_mux_en                   : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp14_1_mux_en                   : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp14_2_mux_en                   : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp14_3_mux_en                   : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp14_4_mux_en                   : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp14_5_mux_en                   : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp14_6_mux_en                   : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp14_7_mux_en                   : 1;	//    31
} CTR43_H14A0;

/*-----------------------------------------------------------------------------
	0x00b0 ctr44 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp15_0_mux_en                   : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp15_1_mux_en                   : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp15_2_mux_en                   : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp15_3_mux_en                   : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp15_4_mux_en                   : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp15_5_mux_en                   : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp15_6_mux_en                   : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp15_7_mux_en                   : 1;	//    31
} CTR44_H14A0;

/*-----------------------------------------------------------------------------
	0x00b4 ctr45 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp16_0_mux_en                   : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp16_1_mux_en                   : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp16_2_mux_en                   : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp16_3_mux_en                   : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp16_4_mux_en                   : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp16_5_mux_en                   : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp16_6_mux_en                   : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp16_7_mux_en                   : 1;	//    31
} CTR45_H14A0;

/*-----------------------------------------------------------------------------
	0x00b8 ctr46 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gp0_ren_ctrl                    : 1,	//     0
	gp1_ren_ctrl                    : 1,	//     1
	gp2_ren_ctrl                    : 1,	//     2
	gp3_ren_ctrl                    : 1,	//     3
	gp4_ren_ctrl                    : 1,	//     4
	gp5_ren_ctrl                    : 1,	//     5
	gp6_ren_ctrl                    : 1,	//     6
	gp7_ren_ctrl                    : 1,	//     7
	gp8_ren_ctrl                    : 1,	//     8
	gp9_ren_ctrl                    : 1,	//     9
	gp10_ren_ctrl                   : 1,	//    10
	gp11_ren_ctrl                   : 1,	//    11
	gp12_ren_ctrl                   : 1,	//    12
	gp13_ren_ctrl                   : 1,	//    13
	gp14_ren_ctrl                   : 1,	//    14
	gp15_ren_ctrl                   : 1,	//    15
	gp16_ren_ctrl                   : 1,	//    16
	gp17_ren_ctrl                   : 1,	//    17
	gp18_ren_ctrl                   : 1,	//    18
	gp19_ren_ctrl                   : 1,	//    19
	gp20_ren_ctrl                   : 1,	//    20
	gp21_ren_ctrl                   : 1,	//    21
	gp22_ren_ctrl                   : 1,	//    22
	gp23_ren_ctrl                   : 1,	//    23
	gp24_ren_ctrl                   : 1,	//    24
	gp25_ren_ctrl                   : 1,	//    25
	gp26_ren_ctrl                   : 1,	//    26
	gp27_ren_ctrl                   : 1,	//    27
	gp28_ren_ctrl                   : 1,	//    28
	gp29_ren_ctrl                   : 1,	//    29
	gp30_ren_ctrl                   : 1,	//    30
	gp31_ren_ctrl                   : 1;	//    31
} CTR46_H14A0;

/*-----------------------------------------------------------------------------
	0x00bc ctr47 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gp35_ren_ctrl                   : 1,	//     0
	gp34_ren_ctrl                   : 1,	//     1
	gp33_ren_ctrl                   : 1,	//     2
	gp32_ren_ctrl                   : 1,	//     3
	stp_renctrl3                    : 1,	//     4
	stp_renctrl2                    : 1,	//     5
	stp_renctrl1                    : 1,	//     6
	stp_renctrl0                    : 1,	//     7
	cit_renctrl0                    : 1,	//     8
	cit_renctrl1                    : 1,	//     9
	cit_renctrl2                    : 1,	//    10
	cit_renctrl3                    : 1,	//    11
	cit_renctrl4                    : 1,	//    12
	cit_renctrl5                    : 1,	//    13
	cit_renctrl6                    : 1,	//    14
	cit_renctrl7                    : 1,	//    15
	cit_renctrl8                    : 1,	//    16
	cit_renctrl9                    : 1,	//    17
	cit_renctrl10                   : 1,	//    18
	cit_renctrl11                   : 1,	//    19
	cit_renctrl12                   : 1,	//    20
	cit_renctrl13                   : 1,	//    21
	cit_renctrl14                   : 1,	//    22
	cit_renctrl15                   : 1,	//    23
	cit_renctrl16                   : 1,	//    24
	cit_renctrl17                   : 1,	//    25
	cit_renctrl18                   : 1,	//    26
	cit_renctrl19                   : 1,	//    27
	cit_renctrl20                   : 1,	//    28
	cit_renctrl21                   : 1,	//    29
	cit_renctrl22                   : 1,	//    30
	cit_renctrl23                   : 1;	//    31
} CTR47_H14A0;

/*-----------------------------------------------------------------------------
	0x00c0 ctr48 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	emmc_renctrl5                   : 1,	//     0
	emmc_renctrl4                   : 1,	//     1
	emmc_renctrl3                   : 1,	//     2
	emmc_renctrl2                   : 1,	//     3
	emmc_renctrl1                   : 1,	//     4
	emmc_renctrl0                   : 1,	//     5
	spi_renctrl7                    : 1,	//     6
	spi_renctrl6                    : 1,	//     7
	spi_renctrl5                    : 1,	//     8
	spi_renctrl4                    : 1,	//     9
	spi_renctrl3                    : 1,	//    10
	spi_renctrl2                    : 1,	//    11
	spi_renctrl1                    : 1,	//    12
	spi_renctrl0                    : 1,	//    13
	stp_renctrl21                   : 1,	//    14
	stp_renctrl20                   : 1,	//    15
	stp_renctrl19                   : 1,	//    16
	stp_renctrl18                   : 1,	//    17
	stp_renctrl17                   : 1,	//    18
	stp_renctrl16                   : 1,	//    19
	stp_renctrl15                   : 1,	//    20
	stp_renctrl14                   : 1,	//    21
	stp_renctrl13                   : 1,	//    22
	stp_renctrl12                   : 1,	//    23
	stp_renctrl11                   : 1,	//    24
	stp_renctrl10                   : 1,	//    25
	stp_renctrl9                    : 1,	//    26
	stp_renctrl8                    : 1,	//    27
	stp_renctrl7                    : 1,	//    28
	stp_renctrl6                    : 1,	//    29
	stp_renctrl5                    : 1,	//    30
	stp_renctrl4                    : 1;	//    31
} CTR48_H14A0;

/*-----------------------------------------------------------------------------
	0x00c4 ctr49 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	sdio_renctrl7                   : 1,	//     4
	sdio_renctrl6                   : 1,	//     5
	sdio_renctrl5                   : 1,	//     6
	sdio_renctrl4                   : 1,	//     7
	sdio_renctrl3                   : 1,	//     8
	sdio_renctrl2                   : 1,	//     9
	sdio_renctrl1                   : 1,	//    10
	sdio_renctrl0                   : 1,	//    11
	smc_renctrl5                    : 1,	//    12
	smc_renctrl4                    : 1,	//    13
	smc_renctrl3                    : 1,	//    14
	smc_renctrl2                    : 1,	//    15
	smc_renctrl1                    : 1,	//    16
	smc_renctrl0                    : 1,	//    17
	rmii_renctrl13                  : 1,	//    18
	rmii_renctrl12                  : 1,	//    19
	rmii_renctrl11                  : 1,	//    20
	rmii_renctrl10                  : 1,	//    21
	rmii_renctrl9                   : 1,	//    22
	rmii_renctrl8                   : 1,	//    23
	rmii_renctrl7                   : 1,	//    24
	rmii_renctrl6                   : 1,	//    25
	rmii_renctrl5                   : 1,	//    26
	rmii_renctrl4                   : 1,	//    27
	rmii_renctrl3                   : 1,	//    28
	rmii_renctrl2                   : 1,	//    29
	rmii_renctrl1                   : 1,	//    30
	rmii_renctrl0                   : 1;	//    31
} CTR49_H14A0;

/*-----------------------------------------------------------------------------
	0x00c8 ctr50 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	a_dr3p_dtr_ctrl                 : 4,	//  0: 3
	a_dr3p_post_fd_ctrl             : 2,	//  4: 5
	                                :13,	//  6:18 reserved
	a_dr3p_m_ctrl                   : 5,	// 19:23
	a_dr3p_pre_fd_ctrl              : 2,	// 24:25
	a_dr3p_od_fout_ctrl             : 2;	// 26:27
} CTR50_H14A0;

/*-----------------------------------------------------------------------------
	0x00cc ctr51 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	a_dr3p_udex_ctrl                : 5,	//  0: 4
	a_dr3p_msex_ctrl                : 9,	//  5:13
	a_dr3p_nsc_ctrl                 : 4,	// 14:17
	a_dr3p_npc_ctrl                 : 6,	// 18:23
	                                : 1,	//    24 reserved
	a_dr3p_cih_ctrl                 : 4,	// 25:28
	a_dr3p_lbwb_ctrl                : 1,	//    29
	a_dr3p_pdb_ctrl                 : 1;	//    30
} CTR51_H14A0;

/*-----------------------------------------------------------------------------
	0x00d0 ctr52 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	eb_renctrl31                    : 1,	//     0
	eb_renctrl30                    : 1,	//     1
	eb_renctrl29                    : 1,	//     2
	eb_renctrl28                    : 1,	//     3
	eb_renctrl27                    : 1,	//     4
	eb_renctrl26                    : 1,	//     5
	eb_renctrl25                    : 1,	//     6
	eb_renctrl24                    : 1,	//     7
	eb_renctrl23                    : 1,	//     8
	eb_renctrl22                    : 1,	//     9
	eb_renctrl21                    : 1,	//    10
	eb_renctrl20                    : 1,	//    11
	eb_renctrl19                    : 1,	//    12
	eb_renctrl18                    : 1,	//    13
	eb_renctrl17                    : 1,	//    14
	eb_renctrl16                    : 1,	//    15
	eb_renctrl15                    : 1,	//    16
	eb_renctrl14                    : 1,	//    17
	eb_renctrl13                    : 1,	//    18
	eb_renctrl12                    : 1,	//    19
	eb_renctrl11                    : 1,	//    20
	eb_renctrl10                    : 1,	//    21
	eb_renctrl9                     : 1,	//    22
	eb_renctrl8                     : 1,	//    23
	eb_renctrl7                     : 1,	//    24
	eb_renctrl6                     : 1,	//    25
	eb_renctrl5                     : 1,	//    26
	eb_renctrl4                     : 1,	//    27
	eb_renctrl3                     : 1,	//    28
	eb_renctrl2                     : 1,	//    29
	eb_renctrl1                     : 1,	//    30
	eb_renctrl0                     : 1;	//    31
} CTR52_H14A0;

/*-----------------------------------------------------------------------------
	0x00d4 ctr53 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	pms_renctrl1                    : 1,	//     8
	pms_renctrl0                    : 1,	//     9
	i2c_renctrl11                   : 1,	//    10
	i2c_renctrl10                   : 1,	//    11
	i2c_renctrl9                    : 1,	//    12
	i2c_renctrl8                    : 1,	//    13
	i2c_renctrl7                    : 1,	//    14
	i2c_renctrl6                    : 1,	//    15
	i2c_renctrl5                    : 1,	//    16
	i2c_renctrl4                    : 1,	//    17
	i2c_renctrl3                    : 1,	//    18
	i2c_renctrl2                    : 1,	//    19
	i2c_renctrl1                    : 1,	//    20
	i2c_renctrl0                    : 1,	//    21
	cam_renctrl9                    : 1,	//    22
	cam_renctrl8                    : 1,	//    23
	cam_renctrl7                    : 1,	//    24
	cam_renctrl6                    : 1,	//    25
	cam_renctrl5                    : 1,	//    26
	cam_renctrl4                    : 1,	//    27
	cam_renctrl3                    : 1,	//    28
	cam_renctrl2                    : 1,	//    29
	cam_renctrl1                    : 1,	//    30
	cam_renctrl0                    : 1;	//    31
} CTR53_H14A0;

/*-----------------------------------------------------------------------------
	0x00d8 ctr54 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	los_level                       : 5,	//  1: 5
	txvreftune0                     : 4,	//  6: 9
	                                : 2,	// 10:11 reserved
	txrestune0                      : 2,	// 12:13
	txpreemppulsetune0              : 1,	//    14
	                                : 2,	// 15:16 reserved
	txhsxvtune0                     : 2,	// 17:18
	txfslstune0                     : 4,	// 19:22
	sqrxtune0                       : 3,	// 23:25
	otgtune0                        : 3,	// 26:28
	compdistune0                    : 3;	// 29:31
} CTR54_H14A0;

/*-----------------------------------------------------------------------------
	0x00dc ctr55 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	uart_renctrl6                   : 1,	//     0
	uart_renctrl5                   : 1,	//     1
	uart_renctrl4                   : 1,	//     2
	uart_renctrl3                   : 1,	//     3
	uart_renctrl2                   : 1,	//     4
	uart_renctrl1                   : 1,	//     5
	uart_renctrl0                   : 1,	//     6
	                                :12,	//  7:18 reserved
	amp_renctrl4                    : 1,	//    19
	amp_renctrl3                    : 1,	//    20
	amp_renctrl2                    : 1,	//    21
	amp_renctrl1                    : 1,	//    22
	amp_renctrl0                    : 1,	//    23
	pwm_renctrl3                    : 1,	//    24
	pwm_renctrl2                    : 1,	//    25
	pwm_renctrl1                    : 1,	//    26
	pwm_renctrl0                    : 1,	//    27
	extintr_renctrl3                : 1,	//    28
	extintr_renctrl2                : 1,	//    29
	extintr_renctrl1                : 1,	//    30
	extintr_renctrl0                : 1;	//    31
} CTR55_H14A0;

/*-----------------------------------------------------------------------------
	0x00e0 ctr56 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	g_dr3p_dtr_ctrl                 : 4,	//  0: 3
	g_dr3p_post_fd_ctrl             : 2,	//  4: 5
	                                :13,	//  6:18 reserved
	g_dr3p_m_ctrl                   : 5,	// 19:23
	g_dr3p_pre_fd_ctrl              : 2,	// 24:25
	g_dr3p_od_fout_ctrl             : 2;	// 26:27
} CTR56_H14A0;

/*-----------------------------------------------------------------------------
	0x00e4 ctr57 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	g_dr3p_udex_ctrl                : 5,	//  0: 4
	g_dr3p_msex_ctrl                : 9,	//  5:13
	g_dr3p_nsc_ctrl                 : 4,	// 14:17
	g_dr3p_npc_ctrl                 : 6,	// 18:23
	                                : 1,	//    24 reserved
	g_dr3p_cih_ctrl                 : 4,	// 25:28
	g_dr3p_lbwb_ctrl                : 1,	//    29
	g_dr3p_pdb_ctrl                 : 1;	//    30
} CTR57_H14A0;

/*-----------------------------------------------------------------------------
	0x00e8 ctr58 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tpio_sel_ctrl                   : 1,	//     0
	tpo_sel_ctrl                    : 1,	//     1
	stpio_en_ctrl                   : 1,	//     2
	                                : 1,	//     3 reserved
	ts_sel                          : 4,	//  4: 7
	rx_sel_uart1                    : 1,	//     8
	rx_sel_uart0                    : 1,	//     9
	rx_sel_bve                      : 2,	// 10:11
	rx_sel_de                          : 2,	// 12:13 
	rx_sel_pms                      : 2,	// 14:15
	rx_sel_vdec                     : 2,	// 16:17
	rx_sel_tz                       : 2,	// 18:19
	                                : 1,	//    20 reserved
	uart2_sel                       : 3,	// 21:23
	                                : 1,	//    24 reserved
	uart1_sel                       : 3,	// 25:27
	                                : 1,	//    28 reserved
	uart0_sel                       : 3;	// 29:31
} CTR58_H14A0;

/*-----------------------------------------------------------------------------
	0x00ec ctr59 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ddr1_mon_en                     : 1,	//     0
	ddr0_mon_en                     : 1,	//     1
	                                : 1,	//     2 reserved
	trace16bit_en                   : 1,	//     3
	                                : 3,	//  4: 6 reserved
	trace8bit_en                    : 1,	//     7
	                                : 2,	//  8: 9 reserved
	cpu_mon_31_en                   : 1,	//    10
	traceclk_en                     : 1,	//    11
	trace_en0                        : 1,	//    12
	trace_en1                        : 1,	//    13
	trace_en2                        : 1,	//    14
	trace_en3                        : 1,	//    15
	                                : 6,	// 16:21 reserved
	dppb_mon_en                     : 1,	//    22
	dppa_mon_en                     : 1,	//    23
	sre_mon_en                      : 1,	//    24
	frc_mon_en                      : 1,	//    25
	h3d_mon_en                      : 1,	//    26
	tcon_mon_en                     : 1,	//    27
	aad_mon_en                      : 1,	//    28
	aud_mon_en                      : 1,	//    29
	cpu_mon_en                      : 1,	//    30
	de_mon_en                       : 1;	//    31
} CTR59_H14A0;

/*-----------------------------------------------------------------------------
	0x00f0 ctr60 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 6,	//  0: 5 reserved
	hub_port_perm_attach            : 2,	//  6: 7
	                                : 2,	//  8: 9 reserved
	bypasssel0                      : 1,	//    10
	bypassdmen0                     : 1,	//    11
	bypassdpen0                     : 1,	//    12
	bypassdmdata0                   : 1,	//    13
	bypassdpdata0                   : 1,	//    14
	txpreempamptune0                : 2,	// 15:16
	txrisetune0                     : 2,	// 17:18
	fsel                            : 6,	// 19:24
	ssc_en                          : 1,	//    25
	ref_ssp_en                      : 1,	//    26
	commononn                       : 1,	//    27
	vatestenb                       : 2,	// 28:29
	test_powerdown_ssp              : 1,	//    30
	test_powerdown_hsp              : 1;	//    31
} CTR60_H14A0;

/*-----------------------------------------------------------------------------
	0x00f4 ctr61 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	over_current_control            : 2,	//  0: 1
	ssc_ref_clk_sel                 : 9,	//  2:10
	ssc_range                       : 3,	// 11:13
	ref_clkdiv2                     : 1,	//    14
	mpll_refssc_clk_en              : 1,	//    15
	mpll_multiplier                 : 7,	// 16:22
	retenablen                      : 1,	//    23
	                                : 1,	//    24 reserved
	lane0_tx2rx_loopbk              : 1,	//    25
	lane0_ext_pclk_req              : 1,	//    26
	adpprbenb0                      : 1,	//    27
	adpdischrg0                     : 1,	//    28
	adpchrg0                        : 1,	//    29
	otgdisable0                     : 1,	//    30
	loopbackenb0                    : 1;	//    31
} CTR61_H14A0;

/*-----------------------------------------------------------------------------
	0x00f8 ctr62 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 6,	//  0: 5 reserved
	hub_port_perm_attach            : 2,	//  6: 7
	                                : 2,	//  8: 9 reserved
	bypasssel0                      : 1,	//    10
	bypassdmen0                     : 1,	//    11
	bypassdpen0                     : 1,	//    12
	bypassdmdata0                   : 1,	//    13
	bypassdpdata0                   : 1,	//    14
	txpreempamptune0                : 2,	// 15:16
	txrisetune0                     : 2,	// 17:18
	fsel                            : 6,	// 19:24
	ssc_en                          : 1,	//    25
	ref_ssp_en                      : 1,	//    26
	commononn                       : 1,	//    27
	vatestenb                       : 2,	// 28:29
	test_powerdown_ssp              : 1,	//    30
	test_powerdown_hsp              : 1;	//    31
} CTR62_H14A0;

/*-----------------------------------------------------------------------------
	0x00fc ctr63 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	ssc_ref_clk_sel                 : 9,	//  2:10
	ssc_range                       : 3,	// 11:13
	ref_clkdiv2                     : 1,	//    14
	mpll_refssc_clk_en              : 1,	//    15
	mpll_multiplier                 : 7,	// 16:22
	retenablen                      : 1,	//    23
	                                : 1,	//    24 reserved
	lane0_tx2rx_loopbk              : 1,	//    25
	lane0_ext_pclk_req              : 1,	//    26
	adpprbenb0                      : 1,	//    27
	adpdischrg0                     : 1,	//    28
	adpchrg0                        : 1,	//    29
	otgdisable0                     : 1,	//    30
	loopbackenb0                    : 1;	//    31
} CTR63_H14A0;

/*-----------------------------------------------------------------------------
	0x0100 ctr64 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	isol_output_data_gr             : 2,	//  0: 1
	isol_interrupt_output_gr        : 3,	//  2: 4
	isol_interrupt_input_gr         : 3,	//  5: 7
	hub_port_perm_attach            : 2,	//  8: 9
	bypasssel0                      : 1,	//    10
	bypassdmen0                     : 1,	//    11
	bypassdpen0                     : 1,	//    12
	bypassdmdata0                   : 1,	//    13
	bypassdpdata0                   : 1,	//    14
	txpreempamptune0                : 2,	// 15:16
	txrisetune0                     : 2,	// 17:18
	fsel                            : 6,	// 19:24
	ssc_en                          : 1,	//    25
	ref_ssp_en                      : 1,	//    26
	commononn                       : 1,	//    27
	vatestenb                       : 2,	// 28:29
	test_powerdown_ssp              : 1,	//    30
	test_powerdown_hsp              : 1;	//    31
} CTR64_H14A0;

/*-----------------------------------------------------------------------------
	0x0104 ctr65 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	ssc_ref_clk_sel                 : 9,	//  2:10
	ssc_range                       : 3,	// 11:13
	ref_clkdiv2                     : 1,	//    14
	mpll_refssc_clk_en              : 1,	//    15
	mpll_multiplier                 : 7,	// 16:22
	retenablen                      : 1,	//    23
	                                : 1,	//    24 reserved
	lane0_tx2rx_loopbk              : 1,	//    25
	lane0_ext_pclk_req              : 1,	//    26
	adpprbenb0                      : 1,	//    27
	adpdischrg0                     : 1,	//    28
	adpchrg0                        : 1,	//    29
	otgdisable0                     : 1,	//    30
	loopbackenb0                    : 1;	//    31
} CTR65_H14A0;

/*-----------------------------------------------------------------------------
	0x0108 ctr66 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto_loop_value                 :20;	//  0:19
} CTR66_H14A0;

/*-----------------------------------------------------------------------------
	0x010c ctr67 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto_add_value                  ;   	// 31: 0
} CTR67_H14A0;

/*-----------------------------------------------------------------------------
	0x0110 ctr68 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto_err_value                  :12;	//  0:11
} CTR68_H14A0;

/*-----------------------------------------------------------------------------
	0x0114 ctr69 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :22,	//  0:21 reserved
	dppa_det_clk_sel                : 2,	// 22:23
	sre_det_clk_sel                 : 2,	// 24:25
	frc_dvi_det_clk_sel             : 2,	// 26:27
	frc_cho_det_clk_sel             : 2,	// 28:29
	h3d_det_clk_sel                 : 2;	// 30:31
} CTR69_H14A0;

/*-----------------------------------------------------------------------------
	0x0118 ctr70 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	los_level                       : 5,	//  1: 5
	txvreftune0                     : 4,	//  6: 9
	                                : 2,	// 10:11 reserved
	txrestune0                      : 2,	// 12:13
	txpreemppulsetune0              : 1,	//    14
	                                : 2,	// 15:16 reserved
	txhsxvtune0                     : 2,	// 17:18
	txfslstune0                     : 4,	// 19:22
	sqrxtune0                       : 3,	// 23:25
	otgtune0                        : 3,	// 26:28
	compdistune0                    : 3;	// 29:31
} CTR70_H14A0;

/*-----------------------------------------------------------------------------
	0x011c ctr71 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tx_vboost_lvl                   : 3,	//  0: 2
	                                : 1,	//     3 reserved
	los_bias                        : 3,	//  4: 6
	                                : 1,	//     7 reserved
	lane0_tx_term_offset            : 5,	//  8:12
	pcs_tx_swign_full               : 7,	// 13:19
	pcs_tx_deempth_6db              : 6,	// 20:25
	pcs_tx_deempth_3p5db            : 6;	// 26:31
} CTR71_H14A0;

/*-----------------------------------------------------------------------------
	0x0120 ctr72 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	ssc_ref_clk_sel                 : 9,	//  2:10
	ssc_range                       : 3,	// 11:13
	ref_clkdiv2                     : 1,	//    14
	mpll_refssc_clk_en              : 1,	//    15
	mpll_multiplier                 : 7,	// 16:22
	retenablen                      : 1,	//    23
	                                : 1,	//    24 reserved
	lane0_tx2rx_loopbk              : 1,	//    25
	lane0_ext_pclk_req              : 1,	//    26
	adpprbenb0                      : 1,	//    27
	adpdischrg0                     : 1,	//    28
	adpchrg0                        : 1,	//    29
	otgdisable0                     : 1,	//    30
	loopbackenb0                    : 1;	//    31
} CTR72_H14A0;

/*-----------------------------------------------------------------------------
	0x0124 ctr73 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	hub_port_overcurrent_pol        : 2,	//  0: 1
	                                : 1,	//     2 reserved
	hub_vbus_pol                    : 1,	//     3
	                                : 2,	//  4: 5 reserved
	hub_port_perm_attach            : 2,	//  6: 7
	                                : 2,	//  8: 9 reserved
	bypasssel0                      : 1,	//    10
	bypassdmen0                     : 1,	//    11
	bypassdpen0                     : 1,	//    12
	bypassdmdata0                   : 1,	//    13
	bypassdpdata0                   : 1,	//    14
	txpreempamptune0                : 2,	// 15:16
	txrisetune0                     : 2,	// 17:18
	fsel                            : 6,	// 19:24
	ssc_en                          : 1,	//    25
	ref_ssp_en                      : 1,	//    26
	commononn                       : 1,	//    27
	vatestenb                       : 2,	// 28:29
	test_powerdown_ssp              : 1,	//    30
	test_powerdown_hsp              : 1;	//    31
} CTR73_H14A0;

/*-----------------------------------------------------------------------------
	0x0128 ctr74 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c_dr3p_dtr_ctrl                 : 4,	//  0: 3
	c_dr3p_post_fd_ctrl             : 2,	//  4: 5
	                                :13,	//  6:18 reserved
	c_dr3p_m_ctrl                   : 5,	// 19:23
	c_dr3p_pre_fd_ctrl              : 2,	// 24:25
	c_dr3p_od_fout_ctrl             : 2;	// 26:27
} CTR74_H14A0;

/*-----------------------------------------------------------------------------
	0x012c ctr75 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c_dr3p_udex_ctrl                : 5,	//  0: 4
	c_dr3p_msex_ctrl                : 9,	//  5:13
	c_dr3p_nsc_ctrl                 : 4,	// 14:17
	c_dr3p_npc_ctrl                 : 6,	// 18:23
	                                : 2,	// 24:25 reserved
	c_dr3p_cih_ctrl                 : 4,	// 26:29
	c_dr3p_lbwb_ctrl                : 1,	//    30
	c_dr3p_pdb_ctrl                 : 1;	//    31
} CTR75_H14A0;

/*-----------------------------------------------------------------------------
	0x0130 ctr76 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	m0_dr3p_dtr_ctrl                : 4,	//  1: 4
	m0_dr3p_post_fd_ctrl            : 2,	//  5: 6
	                                :12,	//  7:18 reserved
	m0_dr3p_m_ctrl                  : 5,	// 19:23
	m0_dr3p_pre_fd_ctrl             : 2,	// 24:25
	m0_dr3p_od_fout_ctrl            : 2;	// 26:27
} CTR76_H14A0;

/*-----------------------------------------------------------------------------
	0x0134 ctr77 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	m0_dr3p_udex_ctrl               : 5,	//  0: 4
	m0_dr3p_msex_ctrl               : 9,	//  5:13
	m0_dr3p_nsc_ctrl                : 4,	// 14:17
	m0_dr3p_npc_ctrl                : 6,	// 18:23
	                                : 2,	// 24:25 reserved
	m0_dr3p_cih_ctrl                : 4,	// 26:29
	m0_dr3p_lbwb_ctrl               : 1,	//    30
	m0_dr3p_pdb_ctrl                : 1;	//    31
} CTR77_H14A0;

/*-----------------------------------------------------------------------------
	0x0138 ctr78 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	m1_dr3p_dtr_ctrl                : 4,	//  0: 3
	m1_dr3p_post_fd_ctrl            : 2,	//  4: 5
	                                :13,	//  6:18 reserved
	m1_dr3p_m_ctrl                  : 5,	// 19:23
	m1_dr3p_pre_fd_ctrl             : 2,	// 24:25
	m1_dr3p_od_fout_ctrl            : 2;	// 26:27
} CTR78_H14A0;

/*-----------------------------------------------------------------------------
	0x013c ctr79 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	m1_dr3p_udex_ctrl               : 5,	//  0: 4
	m1_dr3p_msex_ctrl               : 9,	//  5:13
	m1_dr3p_nsc_ctrl                : 4,	// 14:17
	m1_dr3p_npc_ctrl                : 6,	// 18:23
	                                : 2,	// 24:25 reserved
	m1_dr3p_cih_ctrl                : 4,	// 26:29
	m1_dr3p_lbwb_ctrl               : 1,	//    30
	m1_dr3p_pdb_ctrl                : 1;	//    31
} CTR79_H14A0;

/*-----------------------------------------------------------------------------
	0x0140 ctr80 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sel_aud_fs27clk_src             : 3,	//  0: 2
	sel_audclk_sub                  : 1,	//     3
	                                : 2,	//  4: 5 reserved
	sel_audclk_sub_div1             : 2,	//  6: 7
	sel_audclk_sub_src1             : 3,	//  8:10
	sel_audclk_sub_div0             : 2,	// 11:12
	sel_audclk_sub_src0             : 3,	// 13:15
	                                : 3,	// 16:18 reserved
	sel_audclk                      : 1,	//    19
	                                : 2,	// 20:21 reserved
	sel_audclk_div1                 : 2,	// 22:23
	sel_audclk_src1                 : 3,	// 24:26
	sel_audclk_div0                 : 2,	// 27:28
	sel_audclk_src0                 : 3;	// 29:31
} CTR80_H14A0;

/*-----------------------------------------------------------------------------
	0x0144 ctr81 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sel_aud_fs27clk_div             : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	sel_aud_fs26clk_src             : 3,	//  4: 6
	sel_aud_fs25clk_div             : 2,	//  7: 8
	sel_aud_fs25clk_src             : 3,	//  9:11
	sel_aud_fs24clk_div             : 2,	// 12:13
	sel_aud_fs24clk_src             : 3,	// 14:16
	sel_aud_fs23clk_div             : 2,	// 17:18
	sel_aud_fs23clk_src             : 3,	// 19:21
	sel_aud_fs21clk_div             : 2,	// 22:23
	sel_aud_fs21clk_src             : 3,	// 24:26
	sel_aud_fs20clk_div             : 2,	// 27:28
	sel_aud_fs20clk_src             : 3;	// 29:31
} CTR81_H14A0;

/*-----------------------------------------------------------------------------
	0x0148 ctr82 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :20,	//  0:19 reserved
	disp_pre_fd                     : 2,	// 20:21
	disp_rstb                       : 1,	//    22
	disp_od_pck                     : 2,	// 23:24
	disp_od_tck                     : 2,	// 25:26
	disp_m_ctrl                     : 5;	// 27:31
} CTR82_H14A0;

/*-----------------------------------------------------------------------------
	0x014c ctr83 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	disp_udex_ctrl                  : 5,	//  0: 4
	disp_msex_ctrl                  : 9,	//  5:13
	disp_nsc_ctrl                   : 4,	// 14:17
	disp_npc_ctrl                   : 6,	// 18:23
	                                : 2,	// 24:25 reserved
	disp_cih_ctrl                   : 4,	// 26:29
	disp_lbwb_ctrl                  : 1,	//    30
	disp_pdb_ctrl                   : 1;	//    31
} CTR83_H14A0;

/*-----------------------------------------------------------------------------
	0x0150 ctr84 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :18,	//  0:17 reserved
	sd_dco_g_ctrl                   : 3,	// 18:20
	sd_dco_lpf_ctrl                 : 2,	// 21:22
	sd_dco_resetb_ctrl              : 1,	//    23
	sd_dco_rom_test_ctrl            : 1,	//    24
	de_dco_g_ctrl                   : 3,	// 25:27
	de_dco_lpf_ctrl                 : 2,	// 28:29
	de_dco_resetb_ctrl              : 1,	//    30
	de_dco_rom_test_ctrl            : 1;	//    31
} CTR84_H14A0;

/*-----------------------------------------------------------------------------
	0x0154 ctr85 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sd_dco_fcw_ctrl                 :22;	//  0:21
} CTR85_H14A0;

/*-----------------------------------------------------------------------------
	0x0158 ctr86 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pms0_gp0_mux_en                 : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pms0_gp1_mux_en                 : 1,	//     7
	                                : 3,	//  8:10 reserved
	pms0_gp2_mux_en                 : 1,	//    11
	                                : 3,	// 12:14 reserved
	pms0_gp3_mux_en                 : 1,	//    15
	                                : 3,	// 16:18 reserved
	pms0_gp4_mux_en                 : 1,	//    19
	                                : 3,	// 20:22 reserved
	pms0_gp5_mux_en                 : 1,	//    23
	                                : 3,	// 24:26 reserved
	pms0_gp6_mux_en                 : 1,	//    27
	                                : 3,	// 28:30 reserved
	pms0_gp7_mux_en                 : 1;	//    31
} CTR86_H14A0;

/*-----------------------------------------------------------------------------
	0x015c ctr87 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pm_osc_out_en                   : 1,	//     0
	                                : 2,	//  1: 2 reserved
	pms1_gp0_mux_en                 : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pms1_gp1_mux_en                 : 1,	//     7
	                                : 3,	//  8:10 reserved
	pms1_gp2_mux_en                 : 1,	//    11
	                                : 3,	// 12:14 reserved
	pms1_gp3_mux_en                 : 1,	//    15
	                                : 3,	// 16:18 reserved
	pms1_gp4_mux_en                 : 1,	//    19
	                                : 3,	// 20:22 reserved
	pms1_gp5_mux_en                 : 1,	//    23
	                                : 3,	// 24:26 reserved
	pms1_gp6_mux_en                 : 1,	//    27
	                                : 3,	// 28:30 reserved
	pms1_gp7_mux_en                 : 1;	//    31
} CTR87_H14A0;

/*-----------------------------------------------------------------------------
	0x0160 ctr88 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gpuhpm_i2c_rstn                 : 1,	//     0
	cpuhpm_i2c_rstn                 : 1,	//     1
	                                : 5,	//  2: 6 reserved
	arc_in_mux                      : 1,	//     7
	                                : 8,	//  8:15 reserved
	mhl_pp_in                       : 1,	//    16
	mhl_mode_in                     : 1,	//    17
	arc_odt_sel                     : 3;	// 18:20
} CTR88_H14A0;

/*-----------------------------------------------------------------------------
	0x0164 ctr89 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pcs_rx_los_mask_val             :10,	//  0: 9
	                                : 1,	//    10 reserved
	dftwse                          : 1,	//    11
	dftse                           : 1,	//    12
	rstdisable                      : 1,	//    13
	dramhold                        : 1,	//    14
	drambyp                         : 1,	//    15
	pllreset                        : 1,	//    16
	pllbypass                       : 1,	//    17
	l2clkdisable                    : 1,	//    18
	cxclkdisable                    : 1,	//    19
	crclkdisable                    : 1,	//    20
	clkbypass                       : 1,	//    21
	atspeedenable                   : 1,	//    22
	dftcfg_0                        : 1,	//    23
	dftcfg_1                        : 1,	//    24
	dftcfg_2                        : 1,	//    25
	tmod_cpu_mbist                  : 1,	//    26
	mbistreset2                     : 1,	//    27
	mbistreset1                     : 1,	//    28
	mbistreq2                       : 1,	//    29
	mbistreq1_0                     : 1,	//    30
	mbistreq1_1                     : 1;	//    31
} CTR89_H14A0;

/*-----------------------------------------------------------------------------
	0x0168 ctr90 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	los_level                       : 5,	//  1: 5
	txvreftune0                     : 4,	//  6: 9
	                                : 2,	// 10:11 reserved
	txrestune0                      : 2,	// 12:13
	txpreemppulsetune0              : 1,	//    14
	                                : 2,	// 15:16 reserved
	txhsxvtune0                     : 2,	// 17:18
	txfslstune0                     : 4,	// 19:22
	sqrxtune0                       : 3,	// 23:25
	otgtune0                        : 3,	// 26:28
	compdistune0                    : 3;	// 29:31
} CTR90_H14A0;

/*-----------------------------------------------------------------------------
	0x016c ctr91 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tx_vboost_lvl                   : 3,	//  0: 2
	                                : 1,	//     3 reserved
	los_bias                        : 3,	//  4: 6
	                                : 1,	//     7 reserved
	lane0_tx_term_offset            : 5,	//  8:12
	pcs_tx_swign_full               : 7,	// 13:19
	pcs_tx_deempth_6db              : 6,	// 20:25
	pcs_tx_deempth_3p5db            : 6;	// 26:31
} CTR91_H14A0;

/*-----------------------------------------------------------------------------
	0x0170 ctr92 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	los_level                       : 5,	//  1: 5
	txvreftune0                     : 4,	//  6: 9
	                                : 2,	// 10:11 reserved
	txrestune0                      : 2,	// 12:13
	txpreemppulsetune0              : 1,	//    14
	                                : 2,	// 15:16 reserved
	txhsxvtune0                     : 2,	// 17:18
	txfslstune0                     : 4,	// 19:22
	sqrxtune0                       : 3,	// 23:25
	otgtune0                        : 3,	// 26:28
	compdistune0                    : 3;	// 29:31
} CTR92_H14A0;

/*-----------------------------------------------------------------------------
	0x0174 ctr93 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rx0_data_off                    : 1,	//     0
	                                :11,	//  1:11 reserved
	rx0_ch_sw                       : 1,	//    12
	                                : 2,	// 13:14 reserved
	rx0_sel_pol_clk_en              : 1,	//    15
	                                : 2,	// 16:17 reserved
	rx0_sel_pol_d                   : 1,	//    18
	                                : 2,	// 19:20 reserved
	rx0_sel_speed_en                : 1,	//    21
	                                : 2,	// 22:23 reserved
	rx0_sel_8b10b_en                : 1,	//    24
	                                : 2,	// 25:26 reserved
	rx0_rf_en                       : 1,	//    27
	                                : 2,	// 28:29 reserved
	rx0_pdb_en                      : 1;	//    30
} CTR93_H14A0;

/*-----------------------------------------------------------------------------
	0x0178 ctr94 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	en_eb_addr_out                  : 1,	//     3
	                                : 3,	//  4: 6 reserved
	en_sub_en                       : 1,	//     7
	                                : 2,	//  8: 9 reserved
	pcs_rx_los_mask_val             :10,	// 10:19
	                                : 3,	// 20:22 reserved
	pd                              : 1,	//    23
	                                : 3,	// 24:26 reserved
	en_aud_dacrlrch                 : 1,	//    27
	                                : 3,	// 28:30 reserved
	sc_clk_sel                      : 1;	//    31
} CTR94_H14A0;

/*-----------------------------------------------------------------------------
	0x017c ctr95 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	led_vs_en                       : 1,	//     0
	led_i2c_sel                     : 1,	//     1
	                                : 1,	//     2 reserved
	bvegp_mux_en                    : 1,	//     3
	                                :15,	//  4:18 reserved
	i2c_en                          : 1,	//    19
	jtag1_sel                       : 3,	// 20:22
	gpu_jtag                        : 1,	//    23
	bve_jtag                        : 1,	//    24
	de_jtag                         : 1,	//    25
	pms_jtag                        : 1,	//    26
	vdec_jtag                       : 1,	//    27
	aud_jtag                        : 1,	//    28
	jtag0_sel                       : 3;	// 29:31
} CTR95_H14A0;

/*-----------------------------------------------------------------------------
	0x0180 ctr96 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0184 ctr97 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0188 ctr98 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tx_vboost_lvl                   : 3,	//  0: 2
	                                : 1,	//     3 reserved
	los_bias                        : 3,	//  4: 6
	                                : 1,	//     7 reserved
	lane0_tx_term_offset            : 5,	//  8:12
	pcs_tx_swign_full               : 7,	// 13:19
	pcs_tx_deempth_6db              : 6,	// 20:25
	pcs_tx_deempth_3p5db            : 6;	// 26:31
} CTR98_H14A0;

/*-----------------------------------------------------------------------------
	0x018c ctr99 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tx_vboost_lvl                   : 3,	//  0: 2
	                                : 1,	//     3 reserved
	los_bias                        : 3,	//  4: 6
	                                : 1,	//     7 reserved
	lane0_tx_term_offset            : 5,	//  8:12
	pcs_tx_swign_full               : 7,	// 13:19
	pcs_tx_deempth_6db              : 6,	// 20:25
	pcs_tx_deempth_3p5db            : 6;	// 26:31
} CTR99_H14A0;

/*-----------------------------------------------------------------------------
	0x0190 ctr100 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	count_txlock1                   :13,	//  0:12
	count_txlock2                   :13,	// 13:25
	sel_clkpix1                     : 3,	// 26:28
	sel_clkpix2                     : 3;	// 29:31
} CTR100_H14A0;

/*-----------------------------------------------------------------------------
	0x0194 ctr101 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	count_rxlock1                   : 6,	//  0: 5
	count_rxlock2                   : 6,	//  6:11
	txlock_fixen2                   : 1,	//    12
	txlock_fixen1                   : 1,	//    13
	bytemode1                       : 2,	// 14:15
	bytemode2                       : 2,	// 16:17
	enc_off1                        : 1,	//    18
	enc_off2                        : 1,	//    19
	enc_rst1                        : 1,	//    20
	enc_rst2                        : 1,	//    21
	scr_off1                        : 1,	//    22
	scr_off2                        : 1,	//    23
	scr_rst1                        : 1,	//    24
	scr_rst2                        : 1,	//    25
	rst_vtxlink1                    : 1,	//    26
	rst_vtxlink2                    : 1,	//    27
	test_mode1                      : 2,	// 28:29
	test_mode2                      : 2;	// 30:31
} CTR101_H14A0;

/*-----------------------------------------------------------------------------
	0x0198 ctr102 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	test_di1                        :10,	//  0: 9
	test_di2                        :10,	// 10:19
	read_address_offset1            : 3,	// 20:22
	read_address_offset2            : 3,	// 23:25
	pix1x_rf1                       : 1,	//    26
	pix1x_rf2                       : 1,	//    27
	                                : 3,	// 28:30 reserved
	pdb                             : 1;	//    31
} CTR102_H14A0;

/*-----------------------------------------------------------------------------
	0x019c ctr103 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sel_lock                        : 1,	//     0
	sel_fin                         : 1,	//     1
	sel_rst                         : 1,	//     2
	sel_lpf                         : 1,	//     3
	sel_bw                          : 1,	//     4
	term_en                         : 1,	//     5
	c4tx_rf                         : 1,	//     6
	adj_term                        : 4,	//  7:10
	sel_mode                        : 2,	// 11:12
	sw_pud                          : 1,	//    13
	ib_oft                          : 1,	//    14
	c4tx_cih                        : 3,	// 15:17
	sel_prediv                      : 1,	//    18
	lock_ctl                        : 2,	// 19:20
	pll_byte                        : 2,	// 21:22
	sw_rext                         : 1,	//    23
	pll_ic                          : 3;	// 24:26
} CTR103_H14A0;

/*-----------------------------------------------------------------------------
	0x01a0 ctr104 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	epi_sel_mode                    : 1,	//     3
	pll_60_mode                     : 1,	//     4
	itune0                          : 3,	//  5: 7
	itune1                          : 3,	//  8:10
	itune2                          : 3,	// 11:13
	itune3                          : 3,	// 14:16
	itune4                          : 3,	// 17:19
	itune5                          : 3,	// 20:22
	itune6                          : 3,	// 23:25
	itune7                          : 3,	// 26:28
	itune8                          : 3;	// 29:31
} CTR104_H14A0;

/*-----------------------------------------------------------------------------
	0x01a4 ctr105 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	itune9                          : 3,	//  2: 4
	itune10                         : 3,	//  5: 7
	itune11                         : 3,	//  8:10
	itune12                         : 3,	// 11:13
	itune13                         : 3,	// 14:16
	itune14                         : 3,	// 17:19
	itune15                         : 3,	// 20:22
	itune16                         : 3,	// 23:25
	itune17                         : 3,	// 26:28
	itune18                         : 3;	// 29:31
} CTR105_H14A0;

/*-----------------------------------------------------------------------------
	0x01a8 ctr106 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	mclk_en_ch0                     : 1,	//     0
	mclk_en_ch1                     : 1,	//     1
	mclk_en_ch2                     : 1,	//     2
	mclk_en_ch3                     : 1,	//     3
	mclk_en_ch4                     : 1,	//     4
	mclk_en_ch5                     : 1,	//     5
	mclk_en_ch6                     : 1,	//     6
	mclk_en_ch7                     : 1,	//     7
	ctl1_0                          : 1,	//     8
	ctl1_1                          : 1,	//     9
	ctl1_2                          : 1,	//    10
	ctl1_3                          : 1,	//    11
	ctl1_4                          : 1,	//    12
	ctl1_5                          : 1,	//    13
	ctl1_6                          : 1,	//    14
	ctl1_7                          : 1,	//    15
	ctl1_8                          : 1,	//    16
	ctl1_9                          : 1,	//    17
	ctl1_10                         : 1,	//    18
	ctl1_11                         : 1,	//    19
	ctl1_12                         : 1,	//    20
	ctl1_13                         : 1,	//    21
	ctl1_14                         : 1,	//    22
	ctl1_15                         : 1,	//    23
	ctl1_16                         : 1,	//    24
	ctl1_17                         : 1,	//    25
	ctl1_18                         : 1,	//    26
	ctl1_19                         : 1,	//    27
	ctl1_20                         : 1,	//    28
	ctl1_21                         : 1,	//    29
	ctl1_22                         : 1,	//    30
	ctl1_23                         : 1;	//    31
} CTR106_H14A0;

/*-----------------------------------------------------------------------------
	0x01ac ctr107 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	itune19                         : 3,	//  1: 3
	itune20                         : 3,	//  4: 6
	itune21                         : 3,	//  7: 9
	itune22                         : 3,	// 10:12
	itune23                         : 3,	// 13:15
	pemp_ch0                        : 2,	// 16:17
	pemp_ch1                        : 2,	// 18:19
	pemp_ch2                        : 2,	// 20:21
	pemp_ch3                        : 2,	// 22:23
	pemp_ch4                        : 2,	// 24:25
	pemp_ch5                        : 2,	// 26:27
	pemp_ch6                        : 2,	// 28:29
	pemp_ch7                        : 2;	// 30:31
} CTR107_H14A0;

/*-----------------------------------------------------------------------------
	0x01b0 ctr108 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pemp_ch8                        : 2,	//  0: 1
	pemp_ch9                        : 2,	//  2: 3
	pemp_ch10                       : 2,	//  4: 5
	pemp_ch11                       : 2,	//  6: 7
	pemp_ch12                       : 2,	//  8: 9
	pemp_ch13                       : 2,	// 10:11
	pemp_ch14                       : 2,	// 12:13
	pemp_ch15                       : 2,	// 14:15
	pemp_ch16                       : 2,	// 16:17
	pemp_ch17                       : 2,	// 18:19
	pemp_ch18                       : 2,	// 20:21
	pemp_ch19                       : 2,	// 22:23
	pemp_ch20                       : 2,	// 24:25
	pemp_ch21                       : 2,	// 26:27
	pemp_ch22                       : 2,	// 28:29
	pemp_ch23                       : 2;	// 30:31
} CTR108_H14A0;

/*-----------------------------------------------------------------------------
	0x01b4 ctr109 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	flip_en_ch0                     : 1,	//     0
	flip_en_ch1                     : 1,	//     1
	flip_en_ch2                     : 1,	//     2
	flip_en_ch3                     : 1,	//     3
	flip_en_ch4                     : 1,	//     4
	flip_en_ch5                     : 1,	//     5
	flip_en_ch6                     : 1,	//     6
	flip_en_ch7                     : 1,	//     7
	flip_en_ch8                     : 1,	//     8
	flip_en_ch9                     : 1,	//     9
	flip_en_ch10                    : 1,	//    10
	flip_en_ch11                    : 1,	//    11
	flip_en_ch12                    : 1,	//    12
	flip_en_ch13                    : 1,	//    13
	flip_en_ch14                    : 1,	//    14
	flip_en_ch15                    : 1,	//    15
	flip_en_ch16                    : 1,	//    16
	flip_en_ch17                    : 1,	//    17
	flip_en_ch18                    : 1,	//    18
	flip_en_ch19                    : 1,	//    19
	flip_en_ch20                    : 1,	//    20
	flip_en_ch21                    : 1,	//    21
	flip_en_ch22                    : 1,	//    22
	flip_en_ch23                    : 1,	//    23
	link0_en                        : 1,	//    24
	link1_en                        : 1,	//    25
	link2_en                        : 1,	//    26
	link3_en                        : 1,	//    27
	link4_en                        : 1,	//    28
	link5_en                        : 1;	//    29
} CTR109_H14A0;

/*-----------------------------------------------------------------------------
	0x01b8 ctr110 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ctl2_0                          : 1,	//     0
	ctl2_1                          : 1,	//     1
	ctl2_2                          : 1,	//     2
	ctl2_3                          : 1,	//     3
	ctl2_4                          : 1,	//     4
	ctl2_5                          : 1,	//     5
	ctl2_6                          : 1,	//     6
	ctl2_7                          : 1,	//     7
	ctl2_8                          : 1,	//     8
	ctl2_9                          : 1,	//     9
	ctl2_10                         : 1,	//    10
	ctl2_11                         : 1,	//    11
	ctl2_12                         : 1,	//    12
	ctl2_13                         : 1,	//    13
	ctl2_14                         : 1,	//    14
	ctl2_15                         : 1,	//    15
	ctl2_16                         : 1,	//    16
	ctl2_17                         : 1,	//    17
	ctl2_18                         : 1,	//    18
	ctl2_19                         : 1,	//    19
	ctl2_20                         : 1,	//    20
	ctl2_21                         : 1,	//    21
	ctl2_22                         : 1,	//    22
	ctl2_23                         : 1,	//    23
	mclk_en_ch8                     : 1,	//    24
	mclk_en_ch9                     : 1,	//    25
	mclk_en_ch10                    : 1,	//    26
	mclk_en_ch11                    : 1,	//    27
	mclk_en_ch12                    : 1,	//    28
	mclk_en_ch13                    : 1,	//    29
	mclk_en_ch14                    : 1,	//    30
	mclk_en_ch15                    : 1;	//    31
} CTR110_H14A0;

/*-----------------------------------------------------------------------------
	0x01bc ctr111 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pdb_ch0                         : 1,	//     0
	pdb_ch1                         : 1,	//     1
	pdb_ch2                         : 1,	//     2
	pdb_ch3                         : 1,	//     3
	pdb_ch4                         : 1,	//     4
	pdb_ch5                         : 1,	//     5
	pdb_ch6                         : 1,	//     6
	pdb_ch7                         : 1,	//     7
	pdb_ch8                         : 1,	//     8
	pdb_ch9                         : 1,	//     9
	pdb_ch10                        : 1,	//    10
	pdb_ch11                        : 1,	//    11
	pdb_ch12                        : 1,	//    12
	pdb_ch13                        : 1,	//    13
	pdb_ch14                        : 1,	//    14
	pdb_ch15                        : 1,	//    15
	pdb_ch16                        : 1,	//    16
	pdb_ch17                        : 1,	//    17
	pdb_ch18                        : 1,	//    18
	pdb_ch19                        : 1,	//    19
	pdb_ch20                        : 1,	//    20
	pdb_ch21                        : 1,	//    21
	pdb_ch22                        : 1,	//    22
	pdb_ch23                        : 1,	//    23
	mclk_en_ch16                    : 1,	//    24
	mclk_en_ch17                    : 1,	//    25
	mclk_en_ch18                    : 1,	//    26
	mclk_en_ch19                    : 1,	//    27
	mclk_en_ch20                    : 1,	//    28
	mclk_en_ch21                    : 1,	//    29
	mclk_en_ch22                    : 1,	//    30
	mclk_en_ch23                    : 1;	//    31
} CTR111_H14A0;

typedef struct {
	CTR00_H14A0                           	ctr00                           ;	// 0x0000 : ''
	CTR01_H14A0                           	ctr01                           ;	// 0x0004 : ''
	CTR02_H14A0                           	ctr02                           ;	// 0x0008 : ''
	CTR03_H14A0                           	ctr03                           ;	// 0x000c : ''
	CTR04_H14A0                           	ctr04                           ;	// 0x0010 : ''
	CTR05_H14A0                           	ctr05                           ;	// 0x0014 : ''
	CTR06_H14A0                           	ctr06                           ;	// 0x0018 : ''
	CTR07_H14A0                        	    ctr07                           ;	// 0x001c : ''
	UINT32                          	    ctr08                           ;	// 0x0020 : ''
	CTR09_H14A0                           	ctr09                           ;	// 0x0024 : ''
	CTR10_H14A0                           	ctr10                           ;	// 0x0028 : ''
	CTR11_H14A0                           	ctr11                           ;	// 0x002c : ''
	CTR12_H14A0                           	ctr12                           ;	// 0x0030 : ''
	CTR13_H14A0                           	ctr13                           ;	// 0x0034 : ''
	CTR14_H14A0                           	ctr14                           ;	// 0x0038 : ''
	CTR15_H14A0                           	ctr15                           ;	// 0x003c : ''
	UINT32                          	    ctr16                           ;	// 0x0040 : ''
	CTR17_H14A0                           	ctr17                           ;	// 0x0044 : ''
	UINT32                          	    ctr18                           ;	// 0x0048 : ''
	UINT32                          	    ctr19                           ;	// 0x004c : ''
	UINT32                          	    ctr20                           ;	// 0x0050 : ''
	UINT32                          	    ctr21                           ;	// 0x0054 : ''
	CTR22_H14A0                           	ctr22                           ;	// 0x0058 : ''
	CTR23_H14A0                           	ctr23                           ;	// 0x005c : ''
	CTR24_H14A0                           	ctr24                           ;	// 0x0060 : ''
	CTR25_H14A0                           	ctr25                           ;	// 0x0064 : ''
	CTR26_H14A0                           	ctr26                           ;	// 0x0068 : ''
	CTR27_H14A0                           	ctr27                           ;	// 0x006c : ''
	CTR28_H14A0                           	ctr28                           ;	// 0x0070 : ''
	CTR29_H14A0                           	ctr29                           ;	// 0x0074 : ''
	CTR30_H14A0                           	ctr30                           ;	// 0x0078 : ''
	CTR31_H14A0                           	ctr31                           ;	// 0x007c : ''
	CTR32_H14A0                           	ctr32                           ;	// 0x0080 : ''
	CTR33_H14A0                           	ctr33                           ;	// 0x0084 : ''
	CTR34_H14A0                           	ctr34                           ;	// 0x0088 : ''
	CTR35_H14A0                           	ctr35                           ;	// 0x008c : ''
	CTR36_H14A0                           	ctr36                           ;	// 0x0090 : ''
	CTR37_H14A0                           	ctr37                           ;	// 0x0094 : ''
	CTR38_H14A0                           	ctr38                           ;	// 0x0098 : ''
	CTR39_H14A0                           	ctr39                           ;	// 0x009c : ''
	CTR40_H14A0                           	ctr40                           ;	// 0x00a0 : ''
	CTR41_H14A0                           	ctr41                           ;	// 0x00a4 : ''
	CTR42_H14A0                           	ctr42                           ;	// 0x00a8 : ''
	CTR43_H14A0                           	ctr43                           ;	// 0x00ac : ''
	CTR44_H14A0                           	ctr44                           ;	// 0x00b0 : ''
	CTR45_H14A0                           	ctr45                           ;	// 0x00b4 : ''
	CTR46_H14A0                           	ctr46                           ;	// 0x00b8 : ''
	CTR47_H14A0                           	ctr47                           ;	// 0x00bc : ''
	CTR48_H14A0                           	ctr48                           ;	// 0x00c0 : ''
	CTR49_H14A0                           	ctr49                           ;	// 0x00c4 : ''
	CTR50_H14A0                           	ctr50                           ;	// 0x00c8 : ''
	CTR51_H14A0                           	ctr51                           ;	// 0x00cc : ''
	CTR52_H14A0                           	ctr52                           ;	// 0x00d0 : ''
	CTR53_H14A0                           	ctr53                           ;	// 0x00d4 : ''
	CTR54_H14A0                           	ctr54                           ;	// 0x00d8 : ''
	CTR55_H14A0                           	ctr55                           ;	// 0x00dc : ''
	CTR56_H14A0                           	ctr56                           ;	// 0x00e0 : ''
	CTR57_H14A0                           	ctr57                           ;	// 0x00e4 : ''
	CTR58_H14A0                           	ctr58                           ;	// 0x00e8 : ''
	CTR59_H14A0                           	ctr59                           ;	// 0x00ec : ''
	CTR60_H14A0                           	ctr60                           ;	// 0x00f0 : ''
	CTR61_H14A0                           	ctr61                           ;	// 0x00f4 : ''
	CTR62_H14A0                           	ctr62                           ;	// 0x00f8 : ''
	CTR63_H14A0                           	ctr63                           ;	// 0x00fc : ''
	CTR64_H14A0                           	ctr64                           ;	// 0x0100 : ''
	CTR65_H14A0                           	ctr65                           ;	// 0x0104 : ''
	CTR66_H14A0                           	ctr66                           ;	// 0x0108 : ''
	CTR67_H14A0                           	ctr67                           ;	// 0x010c : ''
	CTR68_H14A0                           	ctr68                           ;	// 0x0110 : ''
	CTR69_H14A0                           	ctr69                           ;	// 0x0114 : ''
	CTR70_H14A0                           	ctr70                           ;	// 0x0118 : ''
	CTR71_H14A0                           	ctr71                           ;	// 0x011c : ''
	CTR72_H14A0                           	ctr72                           ;	// 0x0120 : ''
	CTR73_H14A0                           	ctr73                           ;	// 0x0124 : ''
	CTR74_H14A0                           	ctr74                           ;	// 0x0128 : ''
	CTR75_H14A0                           	ctr75                           ;	// 0x012c : ''
	CTR76_H14A0                           	ctr76                           ;	// 0x0130 : ''
	CTR77_H14A0                           	ctr77                           ;	// 0x0134 : ''
	CTR78_H14A0                           	ctr78                           ;	// 0x0138 : ''
	CTR79_H14A0                           	ctr79                           ;	// 0x013c : ''
	CTR80_H14A0                           	ctr80                           ;	// 0x0140 : ''
	CTR81_H14A0                           	ctr81                           ;	// 0x0144 : ''
	CTR82_H14A0                           	ctr82                           ;	// 0x0148 : ''
	CTR83_H14A0                           	ctr83                           ;	// 0x014c : ''
	CTR84_H14A0                           	ctr84                           ;	// 0x0150 : ''
	CTR85_H14A0                           	ctr85                           ;	// 0x0154 : ''
	CTR86_H14A0                           	ctr86                           ;	// 0x0158 : ''
	CTR87_H14A0                           	ctr87                           ;	// 0x015c : ''
	CTR88_H14A0                           	ctr88                           ;	// 0x0160 : ''
	CTR89_H14A0                           	ctr89                           ;	// 0x0164 : ''
	CTR90_H14A0                           	ctr90                           ;	// 0x0168 : ''
	CTR91_H14A0                           	ctr91                           ;	// 0x016c : ''
	CTR92_H14A0                           	ctr92                           ;	// 0x0170 : ''
	CTR93_H14A0                           	ctr93                           ;	// 0x0174 : ''
	CTR94_H14A0                           	ctr94                           ;	// 0x0178 : ''
	CTR95_H14A0                           	ctr95                           ;	// 0x017c : ''
	UINT32                          	    ctr96                           ;	// 0x0180 : ''
	UINT32                          	    ctr97                           ;	// 0x0184 : ''
	CTR98_H14A0                           	ctr98                           ;	// 0x0188 : ''
	CTR99_H14A0                           	ctr99                           ;	// 0x018c : ''
	CTR100_H14A0                          	ctr100                          ;	// 0x0190 : ''
	CTR101_H14A0                          	ctr101                          ;	// 0x0194 : ''
	CTR102_H14A0                          	ctr102                          ;	// 0x0198 : ''
	CTR103_H14A0                          	ctr103                          ;	// 0x019c : ''
	CTR104_H14A0                          	ctr104                          ;	// 0x01a0 : ''
	CTR105_H14A0                          	ctr105                          ;	// 0x01a4 : ''
	CTR106_H14A0                          	ctr106                          ;	// 0x01a8 : ''
	CTR107_H14A0                          	ctr107                          ;	// 0x01ac : ''
	CTR108_H14A0                          	ctr108                          ;	// 0x01b0 : ''
	CTR109_H14A0                          	ctr109                          ;	// 0x01b4 : ''
	CTR110_H14A0                          	ctr110                          ;	// 0x01b8 : ''
	CTR111_H14A0                          	ctr111                          ;	// 0x01bc : ''
} CTOP_CTRL_REG_H14A0_T;
/* 112 regs, 112 types */

/* 112 regs, 112 types in Total*/



#endif	/* __CTOP_CTRL_REG_H14A0_H__ */

/* from 'LG1156-CH-TOPCTRL_reg-ko-ver1.1_10312012.csv'  대한민국 표준시 by getregs v2.8 */
