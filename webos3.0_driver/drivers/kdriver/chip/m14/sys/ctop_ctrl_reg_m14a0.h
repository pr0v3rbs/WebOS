#ifndef __CTOP_CTRL_REG_M14A0_H__
#define __CTOP_CTRL_REG_M14A0_H__

/*-----------------------------------------------------------------------------
	0x1000 ctr00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_afifo_r                     : 1,	//     0
	reg_aud                         : 1,	//     1
	reg_vfifo_r                     : 1,	//     2
	reg_pix_pip                     : 1,	//     3
	reg_afifo_w                     : 1,	//     4
	reg_vfifo_w                     : 1,	//     5
	                                : 1,	//     6 reserved
	reg_tmds                        : 1,	//     7
	swrst_hdcp                      : 1,	//     8
	swrst_sys                       : 1,	//     9
	swrst_hdmi_dto                  : 1,	//    10
	                                : 1,	//    11 reserved
	aud_hdmi_mclk_sel               : 1,	//    12
	swrst_hdp                       : 1,	//    13
	swrst_m1                        : 1,	//    14
	swrst_m0                        : 1,	//    15
	swrst_adto                      : 1,	//    16
	swrst_cpubus                    : 1,	//    17
	swrst_gpu                       : 1,	//    18
	swrst_gfx                       : 1,	//    19
	swrst_apb                       : 1,	//    20
	swrst_srv                       : 1,	//    21
	                                : 1,	//    22 reserved
	swrst_bus                       : 1,	//    23
	swrst_codec                     : 1,	//    24
	swrst_disp                      : 1,	//    25
	swrst_m1_d100                   : 1,	//    26
	swrst_usb_bt_init               : 1,	//    27
	swrst_m0_d100                   : 1,	//    28
	swrst_usb2_init                 : 1,	//    29
	swrst_usb1_init                 : 1,	//    30
	swrst_usb0_init                 : 1;	//    31
} CTR00_M14A0;

/*-----------------------------------------------------------------------------
	0x1004 ctr01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ext_en_i                        : 1,	//     0
	f27_18_sel                      : 1,	//     1
	f24_sel                         : 1,	//     2
	sc_ext_sel                      : 3,	//  3: 5
	                                : 1,	//     6 reserved
	dco_sel                         : 1,	//     7
	                                :10,	//  8:17 reserved
	phy_i2c_clk_sel                 : 1,	//    18
	link_sys_off_sel                : 1,	//    19
	apb_clk_sel                     : 3,	// 20:22
	                                : 5,	// 23:27 reserved
	bus_clk_sel                     : 2,	// 28:29
	sel_sc_27m                      : 2;	// 30:31
} CTR01_M14A0;

/*-----------------------------------------------------------------------------
	0x1008 ctr02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :29,	//  0:28 reserved
	swrst_gpu_mem                   : 1,	//    29
	swrst_gpu_core                  : 1,	//    30
	swrst_gpu_sys                   : 1;	//    31
} CTR02_M14A0;

/*-----------------------------------------------------------------------------
	0x100c ctr03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :28,	//  0:27 reserved
	gpu_sys_clk_sel                 : 2;	// 28:29
} CTR03_M14A0;

/*-----------------------------------------------------------------------------
	0x1010 ctr04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :13,	//  0:12 reserved
	reg_sys                         : 1,	//    13
	reg_s_te                        : 1,	//    14
	reg_swrst_se                    : 1,	//    15
	                                : 2,	// 16:17 reserved
	swrst_vd_te                     : 1,	//    18
	swrst_vda                       : 1,	//    19
	                                : 2,	// 20:21 reserved
	swrst_vd                        : 1,	//    22
	swrst_icoda                     : 1,	//    23
	swrst_icod                      : 1,	//    24
	                                : 1,	//    25 reserved
	swrst_teah                      : 1,	//    26
	swrst_te_te1                    : 1,	//    27
	swrst_te_te                     : 1,	//    28
	swrst_te_ve                     : 1,	//    29
	swrst_vea                       : 1,	//    30
	swrst_ve                        : 1;	//    31
} CTR04_M14A0;

/*-----------------------------------------------------------------------------
	0x1014 ctr05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	vd_teclk_sel                    : 3,	//  2: 4
	vd_aclk_sel                     : 2,	//  5: 6
	                                : 7,	//  7:13 reserved
	vdclk_sel                       : 3,	// 14:16
	                                : 3,	// 17:19 reserved
	aud_teclk_sel                   : 3,	// 20:22
	                                : 6,	// 23:28 reserved
	aud_dsp0aclk_sel                : 2;	// 29:30
} CTR05_M14A0;

/*-----------------------------------------------------------------------------
	0x1018 ctr06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :10,	//  0: 9 reserved
	icodclk_sel                     : 3,	// 10:12
	                                : 3,	// 13:15 reserved
	te_teclk_sel                    : 3,	// 16:18
	                                : 6,	// 19:24 reserved
	veclk_sel                       : 3,	// 25:27
	reg_seclk_inv                   : 1,	//    28
	sel_seclk                       : 2;	// 29:30
} CTR06_M14A0;

/*-----------------------------------------------------------------------------
	0x101c ctr07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	gfx_aclk_sel                    : 2,	//  4: 5
	                                :21,	//  6:26 reserved
	swrst_gfx                       : 1,	//    27
	                                : 3,	// 28:30 reserved
	swrst_gfx_a                     : 1;	//    31
} CTR07_M14A0;

/*-----------------------------------------------------------------------------
	0x1020 ctr08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :20,	//  0:19 reserved
	trace_sel                       : 3,	// 20:22
	dbgvsoc_sel                     : 3,	// 23:25
	atvsoc_sel                      : 3,	// 26:28
	atclk_sel                       : 3;	// 29:31
} CTR08_M14A0;

/*-----------------------------------------------------------------------------
	0x1024 ctr09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	isol_en_sel                     : 3,	//  1: 3
	                                : 5,	//  4: 8 reserved
	ahbclken_sel                    : 3,	//  9:11
	                                : 5,	// 12:16 reserved
	swrst_ptmpg                     : 1,	//    17
	swrst_ptm                       : 1,	//    18
	swrst_prsys                     : 1,	//    19
	swrst_prdbg                     : 1,	//    20
	swrst_dbg1                      : 1,	//    21
	swrst_dbg0                      : 1,	//    22
	swrst_dbgpg                     : 1,	//    23
	                                : 7,	// 24:30 reserved
	swrst_at                        : 1;	//    31
} CTR09_M14A0;

/*-----------------------------------------------------------------------------
	0x1028 ctr10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	ahb7_sel                        : 3,	//  1: 3
	                                : 5,	//  4: 8 reserved
	ahb5_sel                        : 3,	//  9:11
	                                : 1,	//    12 reserved
	ahb4_sel                        : 3,	// 13:15
	                                : 1,	//    16 reserved
	ahb3_sel                        : 3,	// 17:19
	                                : 1,	//    20 reserved
	ahb2_sel                        : 3,	// 21:23
	                                : 1,	//    24 reserved
	ahb1_sel                        : 3,	// 25:27
	                                : 1,	//    28 reserved
	ahb0_sel                        : 3;	// 29:31
} CTR10_M14A0;

/*-----------------------------------------------------------------------------
	0x102c ctr11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	rmii_clkoff_sel                 : 1,	//     0
	ahb22_sel                       : 3,	//  1: 3
	                                : 1,	//     4 reserved
	ahb21_sel                       : 3,	//  5: 7
	                                : 1,	//     8 reserved
	ahb20_sel                       : 3,	//  9:11
	                                : 1,	//    12 reserved
	ahb19_sel                       : 3,	// 13:15
	                                : 9,	// 16:24 reserved
	ahb9_sel                        : 3,	// 25:27
	                                : 1,	//    28 reserved
	ahb8_sel                        : 3;	// 29:31
} CTR11_M14A0;

/*-----------------------------------------------------------------------------
	0x1030 ctr12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	ahb46_sel                       : 3,	//  1: 3
	                                : 1,	//     4 reserved
	ahb42_sel                       : 3,	//  5: 7
	                                : 1,	//     8 reserved
	ahb30_sel                       : 3,	//  9:11
	                                :17,	// 12:28 reserved
	ahb24_sel                       : 3;	// 29:31
} CTR12_M14A0;

/*-----------------------------------------------------------------------------
	0x1034 ctr13 ''
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
} CTR13_M14A0;

/*-----------------------------------------------------------------------------
	0x1038 ctr14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :17,	//  0:16 reserved
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
} CTR14_M14A0;

/*-----------------------------------------------------------------------------
	0x103c ctr15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	bus7_sel                        : 3,	//  1: 3
	                                : 1,	//     4 reserved
	bus6_sel                        : 3,	//  5: 7
	                                : 1,	//     8 reserved
	bus5_sel                        : 3,	//  9:11
	                                : 1,	//    12 reserved
	bus4_sel                        : 3,	// 13:15
	                                : 1,	//    16 reserved
	bus3_sel                        : 3,	// 17:19
	                                : 1,	//    20 reserved
	bus2_sel                        : 3,	// 21:23
	                                : 1,	//    24 reserved
	bus1_sel                        : 3,	// 25:27
	                                : 1,	//    28 reserved
	bus0_sel                        : 3;	// 29:31
} CTR15_M14A0;

/*-----------------------------------------------------------------------------
	0x1040 ctr16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :21,	//  0:20 reserved
	bus10_sel                       : 3,	// 21:23
	                                : 1,	//    24 reserved
	bus9_sel                        : 3,	// 25:27
	                                : 1,	//    28 reserved
	bus8_sel                        : 3;	// 29:31
} CTR16_M14A0;

/*-----------------------------------------------------------------------------
	0x1044 ctr17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :21,	//  0:20 reserved
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
} CTR17_M14A0;

/*-----------------------------------------------------------------------------
	0x1048 ctr18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	apb0_9_sel                      : 3,	//  2: 4
	apb0_8_sel                      : 3,	//  5: 7
	apb0_7_sel                      : 3,	//  8:10
	apb0_6_sel                      : 3,	// 11:13
	apb0_5_sel                      : 3,	// 14:16
	apb0_4_sel                      : 3,	// 17:19
	apb0_3_sel                      : 3,	// 20:22
	apb0_2_sel                      : 3,	// 23:25
	apb0_1_sel                      : 3,	// 26:28
	apb0_0_sel                      : 3;	// 29:31
} CTR18_M14A0;

/*-----------------------------------------------------------------------------
	0x104c ctr19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	apb1_7_sel                      : 3,	//  1: 3
	                                : 1,	//     4 reserved
	apb1_6_sel                      : 3,	//  5: 7
	                                : 1,	//     8 reserved
	apb1_5_sel                      : 3,	//  9:11
	                                : 1,	//    12 reserved
	apb1_4_sel                      : 3,	// 13:15
	                                : 1,	//    16 reserved
	apb1_3_sel                      : 3,	// 17:19
	                                : 1,	//    20 reserved
	apb1_2_sel                      : 3,	// 21:23
	                                : 1,	//    24 reserved
	apb1_1_sel                      : 3,	// 25:27
	                                : 1,	//    28 reserved
	apb1_0_sel                      : 3;	// 29:31
} CTR19_M14A0;

/*-----------------------------------------------------------------------------
	0x1050 ctr20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	apb1_15_sel                     : 3,	//  1: 3
	                                : 1,	//     4 reserved
	apb1_14_sel                     : 3,	//  5: 7
	                                : 1,	//     8 reserved
	apb1_13_sel                     : 3,	//  9:11
	                                : 1,	//    12 reserved
	apb1_12_sel                     : 3,	// 13:15
	                                : 1,	//    16 reserved
	apb1_11_sel                     : 3,	// 17:19
	                                : 1,	//    20 reserved
	apb1_10_sel                     : 3,	// 21:23
	                                : 1,	//    24 reserved
	apb1_9_sel                      : 3,	// 25:27
	                                : 1,	//    28 reserved
	apb1_8_sel                      : 3;	// 29:31
} CTR20_M14A0;

/*-----------------------------------------------------------------------------
	0x1054 ctr21 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :13,	//  0:12 reserved
	apb1_20_sel                     : 3,	// 13:15
	                                : 1,	//    16 reserved
	apb1_19_sel                     : 3,	// 17:19
	                                : 1,	//    20 reserved
	apb1_18_sel                     : 3,	// 21:23
	                                : 1,	//    24 reserved
	apb1_17_sel                     : 3,	// 25:27
	                                : 1,	//    28 reserved
	apb1_16_sel                     : 3;	// 29:31
} CTR21_M14A0;

/*-----------------------------------------------------------------------------
	0x1058 ctr22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
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
} CTR22_M14A0;

/*-----------------------------------------------------------------------------
	0x105c ctr23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tcon_video_clk_sel              : 2,	//  0: 1
	                                : 3,	//  2: 4 reserved
	reg_dppa_det                    : 1,	//     5
	reg_sre_sys_det                 : 1,	//     6
	reg_frc_dvi_det                 : 1,	//     7
	reg_frc_cho_det                 : 1,	//     8
	reg_h3d_det                     : 1,	//     9
	reg_tcon_video                  : 1,	//    10
	reg_dppa_pe1                    : 1,	//    11
	reg_tcon_osd                    : 1,	//    12
	reg_tcon_pix                    : 1,	//    13
	reg_tcon_pix2                   : 1,	//    14
	reg_disp_ded_p                  : 1,	//    15
	reg_disp_ded                    : 1,	//    16
	reg_disp_dp                     : 1,	//    17
	reg_disp_de                     : 1,	//    18
	reg_dppa_osd                    : 1,	//    19
	reg_dppa_osd60hz                : 1,	//    20
	reg_dppb                        : 1,	//    21
	reg_dppb_if                     : 1,	//    22
	reg_dppa                        : 1,	//    23
	reg_sre_sys                     : 1,	//    24
	reg_frc_dvi                     : 1,	//    25
	reg_frc_cho                     : 1,	//    26
	reg_frc_dvo                     : 1,	//    27
	                                : 2,	// 28:29 reserved
	reg_h3d                         : 1,	//    30
	reg_slt                         : 1;	//    31
} CTR23_M14A0;

/*-----------------------------------------------------------------------------
	0x1060 ctr24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tx_combo_sel                    : 3,	//  0: 2
	                                : 1,	//     3 reserved
	dppa_pe1_clk_sel                : 2,	//  4: 5
	ded_disp_clk_sel                : 2,	//  6: 7
	deb_disp_clk_sel                : 2,	//  8: 9
	dppa_osd60hz_sel                : 2,	// 10:11
	dppa_osd_clk_sel                : 2,	// 12:13
	dppb_if_clk_sel                 : 2,	// 14:15
	dppa_clk_sel                    : 2,	// 16:17
	dppb_clk_sel                    : 2,	// 18:19
	sre_sys_clk_sel                 : 2,	// 20:21
	frc_dvi_clk_sel                 : 2,	// 22:23
	frc_cho_clk_sel                 : 2,	// 24:25
	frc_dvo_clk_sel                 : 2,	// 26:27
	h3d_clk_sel                     : 2,	// 28:29
	tcon_osd_clk_sel                : 2;	// 30:31
} CTR24_M14A0;

/*-----------------------------------------------------------------------------
	0x1064 ctr25 ''
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
	reg_swrst_cvd27                 : 1,	//    10
	reg_swrst_cvd54                 : 1,	//    11
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
} CTR25_M14A0;

/*-----------------------------------------------------------------------------
	0x1068 ctr26 ''
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
	                                : 2,	// 25:26 reserved
	cvd54_sel                       : 1,	//    27
	                                : 2,	// 28:29 reserved
	phy_ppll_sel                    : 2;	// 30:31
} CTR26_M14A0;

/*-----------------------------------------------------------------------------
	0x106c ctr27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	cvda_clk_sel                    : 3,	//  8:10
	                                : 9,	// 11:19 reserved
	dec_mif_dea_clk_sel             : 3,	// 20:22
	                                : 1,	//    23 reserved
	cve27_inv_sel                   : 1,	//    24
	cve54_inv_sel                   : 1,	//    25
	sel_ded_cve27                   : 2;	// 26:27
} CTR27_M14A0;

/*-----------------------------------------------------------------------------
	0x1070 ctr28 ''
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
} CTR28_M14A0;

/*-----------------------------------------------------------------------------
	0x1074 ctr29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :15,	//  0:14 reserved
	swrst_tcon_apb                  : 1,	//    15
	swrst_tcon_axi                  : 1,	//    16
	swrst_vd_de                     : 1,	//    17
	swrst_dppb_cpu                  : 1,	//    18
	swrst_dppb_axi                  : 1,	//    19
	swrst_dppb_apb                  : 1,	//    20
	swrst_dppa_axi                  : 1,	//    21
	swrst_dppa_apb                  : 1,	//    22
	swrst_sre_axi                   : 1,	//    23
	swrst_sre_apb                   : 1,	//    24
	swrst_frc_sys                   : 1,	//    25
	swrst_frc_axi                   : 1,	//    26
	swrst_frc_apb                   : 1,	//    27
	swrst_frc_mc                    : 1,	//    28
	swrst_frc_mb                    : 1,	//    29
	swrst_frc_ma                    : 1,	//    30
	swrst_h3d_apb                   : 1;	//    31
} CTR29_M14A0;

/*-----------------------------------------------------------------------------
	0x1078 ctr30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	dppa_apb_sel                    : 3,	//  2: 4
	                                : 3,	//  5: 7 reserved
	sre_apb_sel                     : 3,	//  8:10
	                                : 6,	// 11:16 reserved
	frc_apb_sel                     : 3,	// 17:19
	frc_mc_sel                      : 2,	// 20:21
	                                : 1,	//    22 reserved
	frc_mb_sel                      : 2,	// 23:24
	                                : 1,	//    25 reserved
	frc_ma_sel                      : 2,	// 26:27
	                                : 1,	//    28 reserved
	h3d_apb_sel                     : 3;	// 29:31
} CTR30_M14A0;

/*-----------------------------------------------------------------------------
	0x107c ctr31 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :14,	//  0:13 reserved
	tcon_axi_sel                    : 3,	// 14:16
	vd_declk_sel                    : 3,	// 17:19
	                                : 6,	// 20:25 reserved
	dppb_apb_sel                    : 3;	// 26:28
} CTR31_M14A0;

/*-----------------------------------------------------------------------------
	0x1080 ctr32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ext_intr_pol_sel0               : 1,	//     0
	ext_intr_pol_sel1               : 1,	//     1
	ext_intr_pol_sel2               : 1,	//     2
	ext_intr_pol_sel3               : 1,	//     3
	cam_vcc_pol                     : 1,	//     4
	                                : 1,	//     5 reserved
	pll_sel_m2                      : 1,	//     6
	pll_sel_m1                      : 1,	//     7
	jtag1_sel                       : 3,	//  8:10
	vdec_jtag_no                    : 1,	//    11
	aud_jtag_no                     : 1,	//    12
	de_jtag_no                      : 1,	//    13
	frc_jtag_no                     : 1,	//    14
	gpu_jtag_no                     : 1,	//    15
	jtag0_sel                       : 3,	// 16:18
	                                : 2,	// 19:20 reserved
	pll_sel_core                    : 1;	//    21
} CTR32_M14A0;

/*-----------------------------------------------------------------------------
	0x1084 ctr33 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp4_0_mux_en                    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp4_1_mux_en                    : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp4_2_mux_en                    : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp4_3_mux_en                    : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp4_4_mux_en                    : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp4_5_mux_en                    : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp4_6_mux_en                    : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp4_7_mux_en                    : 1;	//    31
} CTR33_M14A0;

/*-----------------------------------------------------------------------------
	0x1088 ctr34 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp5_0_mux_en                    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp5_1_mux_en                    : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp5_2_mux_en                    : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp5_3_mux_en                    : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp5_4_mux_en                    : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp5_5_mux_en                    : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp5_6_mux_en                    : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp5_7_mux_en                    : 1;	//    31
} CTR34_M14A0;

/*-----------------------------------------------------------------------------
	0x108c ctr35 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp6_0_mux_en                    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp6_1_mux_en                    : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp6_2_mux_en                    : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp6_3_mux_en                    : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp6_4_mux_en                    : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp6_5_mux_en                    : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp6_6_mux_en                    : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp6_7_mux_en                    : 1;	//    31
} CTR35_M14A0;

/*-----------------------------------------------------------------------------
	0x1090 ctr36 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp7_0_mux_en                    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp7_1_mux_en                    : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp7_2_mux_en                    : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp7_3_mux_en                    : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp7_4_mux_en                    : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp7_5_mux_en                    : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp7_6_mux_en                    : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp7_7_mux_en                    : 1;	//    31
} CTR36_M14A0;

/*-----------------------------------------------------------------------------
	0x1094 ctr37 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp8_0_mux_en                    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp8_1_mux_en                    : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp8_2_mux_en                    : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp8_3_mux_en                    : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp8_4_mux_en                    : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp8_5_mux_en                    : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp8_6_mux_en                    : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp8_7_mux_en                    : 1;	//    31
} CTR37_M14A0;

/*-----------------------------------------------------------------------------
	0x1098 ctr38 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp9_0_mux_en                    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp9_1_mux_en                    : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp9_2_mux_en                    : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp9_3_mux_en                    : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp9_4_mux_en                    : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp9_5_mux_en                    : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp9_6_mux_en                    : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp9_7_mux_en                    : 1;	//    31
} CTR38_M14A0;

/*-----------------------------------------------------------------------------
	0x109c ctr39 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp10_0_mux_en                   : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp10_1_mux_en                   : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp10_2_mux_en                   : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp10_3_mux_en                   : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp10_4_mux_en                   : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp10_5_mux_en                   : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp10_6_mux_en                   : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp10_7_mux_en                   : 1;	//    31
} CTR39_M14A0;

/*-----------------------------------------------------------------------------
	0x10a0 ctr40 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp11_0_mux_en                   : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp11_1_mux_en                   : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp11_2_mux_en                   : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp11_3_mux_en                   : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp11_4_mux_en                   : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp11_5_mux_en                   : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp11_6_mux_en                   : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp11_7_mux_en                   : 1;	//    31
} CTR40_M14A0;

/*-----------------------------------------------------------------------------
	0x10a4 ctr41 ''
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
} CTR41_M14A0;

/*-----------------------------------------------------------------------------
	0x10a8 ctr42 ''
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
} CTR42_M14A0;

/*-----------------------------------------------------------------------------
	0x10ac ctr43 ''
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
} CTR43_M14A0;

/*-----------------------------------------------------------------------------
	0x10b0 ctr44 ''
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
} CTR44_M14A0;

/*-----------------------------------------------------------------------------
	0x10b4 ctr45 ''
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
} CTR45_M14A0;

/*-----------------------------------------------------------------------------
	0x10b8 ctr46 ''
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
} CTR46_M14A0;

/*-----------------------------------------------------------------------------
	0x10bc ctr47 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pull_up_control_tpio_data       : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pull_up_control_tpio_clk        : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control_tpi_data        : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_tpi_clk         : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_stpi1_clk       : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_stpi1           : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_stpi0_clk       : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_stpi0           : 1;	//    31
} CTR47_M14A0;

/*-----------------------------------------------------------------------------
	0x10c0 ctr48 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pull_up_control_spi_cs1         : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pull_up_control_spi_sclk1       : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control_spi_do1         : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_spi_di1         : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_spi_cs0         : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_spi_sclk0       : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_spi_do0         : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_spi_di0         : 1;	//    31
} CTR48_M14A0;

/*-----------------------------------------------------------------------------
	0x10c4 ctr49 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	pull_up_controlemmc_resetn      : 1,	//  7
	                                : 7,	//  8:14 reserved
	pull_up_control_emmc_data       : 1,	//    15
	                                : 7,	// 16:22 reserved
	pull_up_control_emmc_cmd        : 1,	//    23
	                                : 7,	// 24:30 reserved
	pull_up_controlemmc_clk         : 1;	//    31
} CTR49_M14A0;

/*-----------------------------------------------------------------------------
	0x10c8 ctr50 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	pull_up_control_rmii_txen       : 1,	//    7
	                                : 3,	//  8:10 reserved
	pull_up_control_rmii_txd        : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_rmii_rxd        : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_rmii_crs_dv     : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_rmii_mdio       : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_rmii_mdc        : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_rmii_ref_clk    : 1;	//    31
} CTR50_M14A0;

/*-----------------------------------------------------------------------------
	0x10cc ctr51 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :11,	//  0:10 reserved
	pull_up_control_sc_data         : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_sc_detect       : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_sc_vcc_sel      : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_sc_vccen        : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_sc_clk          : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_sc_rst          : 1;	//    31
} CTR51_M14A0;

/*-----------------------------------------------------------------------------
	0x10d0 ctr52 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	pull_up_control_eb_addr         : 1,	// 19
	                                : 3,	// 20:22 reserved
	pull_up_control_eb_n            : 1,	// 23
	                                : 3,	// 24:26 reserved
	pull_up_control_eb_data         : 1,	// 27
	                                : 3,	// 28:30 reserved
	pull_up_control_eb_cs           : 1;	// 31
} CTR52_M14A0;

/*-----------------------------------------------------------------------------
	0x10d4 ctr53 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pull_up_control_cam_inpack_n    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pull_up_control_cam_ireq_n      : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control_cam_reg_n       : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_cam_vccen_n     : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_cam_cd_n        : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_cam_vs_n        : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_cam_iois16      : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_cam_wait_n      : 1;	//    31
} CTR53_M14A0;

/*-----------------------------------------------------------------------------
	0x10d8 ctr54 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	pull_up_control_uart1_rx        : 1,	// 19
	                                : 3,	// 20:22 reserved
	pull_up_control_uart1_tx        : 1,	// 23
	                                : 3,	// 24:26 reserved
	pull_up_control_uart0_rx        : 1,	// 27
	                                : 3,	// 28:30 reserved
	pull_up_control_uart0_tx        : 1;	// 31
} CTR54_M14A0;

/*-----------------------------------------------------------------------------
	0x10dc ctr55 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	pull_up_control_dac             : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control_pwm_in          : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_pwm             : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_ext_intr3       : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_ext_intr2       : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_ext_intr1       : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_ext_intr0       : 1;	//    31
} CTR55_M14A0;

/*-----------------------------------------------------------------------------
	0x10e0 ctr56 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :11,	//  0:10 reserved
	pull_up_control_sda2            : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control_scl2            : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control_sda1            : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_scl1            : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_sda0            : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_scl0            : 1;	//    31
} CTR56_M14A0;

/*-----------------------------------------------------------------------------
	0x10e4 ctr57 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pull_up_control_gpio39          : 1,	//     0
	pull_up_control_gpio38          : 1,	//     1
	pull_up_control_gpio37          : 1,	//     2
	pull_up_control_gpio36          : 1,	//     3
	                                : 7,	//  4:10 reserved
	pull_up_control_sd_data         : 1,	//    11
	                                : 7,	// 12:18 reserved
	pull_up_control_sd_wp_n         : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_sd_cd_n         : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_sd_cmd          : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_sd_clk          : 1;	//    31
} CTR57_M14A0;

/*-----------------------------------------------------------------------------
	0x10e8 ctr58 ''
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
} CTR58_M14A0;

/*-----------------------------------------------------------------------------
	0x10ec ctr59 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
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
	                                : 3,	// 16:18 reserved
	usb_mon_en                      : 1,	//    19
	                                : 2,	// 20:21 reserved
	mon_dppb_en                     : 1,	//    22
	mon_dppa_en                     : 1,	//    23
	mon_sre_en                      : 1,	//    24
	mon_frc_en                      : 1,	//    25
	mon_h3d_en                      : 1,	//    26
	mon_gfx_en                      : 1,	//    27
	mon_aad_en                      : 1,	//    28
	mon_aud_en                      : 1,	//    29
	mon_cpu_en                      : 1,	//    30
	mon_de_en                       : 1;	//    31
} CTR59_M14A0;

/*-----------------------------------------------------------------------------
	0x10f0 ctr60 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	testdatain                      : 8,	//  4:11
	testdataoutsel                  : 1,	//    12
	testaddr                        : 4,	// 13:16
	testclk                         : 1,	//    17
	txpreemphasistune               : 2,	// 18:19
	txrisetune                      : 2,	// 20:21
	commononn                       : 1,	//    22
	fsel                            : 3,	// 23:25
	refclksel                       : 2,	// 26:27
	testen                          : 1,	//    28
	vatestenb                       : 2,	// 29:30
	siddq_en                        : 1;	//    31
} CTR60_M14A0;

/*-----------------------------------------------------------------------------
	0x10f4 ctr61 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	bypassel0                       : 1,	//     4
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
} CTR61_M14A0;

/*-----------------------------------------------------------------------------
	0x10f8 ctr62 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	testdatain                      : 8,	//  4:11
	testdataoutsel                  : 1,	//    12
	testaddr                        : 4,	// 13:16
	testclk                         : 1,	//    17
	txpreemphasistune               : 2,	// 18:19
	txrisetune                      : 2,	// 20:21
	commononn                       : 1,	//    22
	fsel                            : 3,	// 23:25
	refclksel                       : 2,	// 26:27
	testen                          : 1,	//    28
	vatestenb                       : 2,	// 29:30
	siddq_en                        : 1;	//    31
} CTR62_M14A0;

/*-----------------------------------------------------------------------------
	0x10fc ctr63 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	bypassel0                       : 1,	//     4
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
} CTR63_M14A0;

/*-----------------------------------------------------------------------------
	0x1100 ctr64 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	testdatain                      : 8,	//  4:11
	testdataoutsel                  : 1,	//    12
	testaddr                        : 4,	// 13:16
	testclk                         : 1,	//    17
	txpreemphasistune               : 2,	// 18:19
	txrisetune                      : 2,	// 20:21
	commononn                       : 1,	//    22
	fsel                            : 3,	// 23:25
	refclksel                       : 2,	// 26:27
	testen                          : 1,	//    28
	vatestenb                       : 2,	// 29:30
	siddq_en                        : 1;	//    31
} CTR64_M14A0;

/*-----------------------------------------------------------------------------
	0x1104 ctr65 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	bypassel0                       : 1,	//     4
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
} CTR65_M14A0;

/*-----------------------------------------------------------------------------
	0x1108 ctr66 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto_loop_value                 :20;	//  0:19
} CTR66_M14A0;

/*-----------------------------------------------------------------------------
	0x110c ctr67 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto_add_value                  ;   	// 31: 0
} CTR67_M14A0;

/*-----------------------------------------------------------------------------
	0x1110 ctr68 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	adto_err_value                  :12;	//  0:11
} CTR68_M14A0;

/*-----------------------------------------------------------------------------
	0x1114 ctr69 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :22,	//  0:21 reserved
	dppa_det_clk_sel                : 2,	// 22:23
	sre_sys_det_clk_sel             : 2,	// 24:25
	frc_dvi_det_clk_sel             : 2,	// 26:27
	frc_cho_det_clk_sel             : 2,	// 28:29
	h3d_det_clk_sel                 : 2;	// 30:31
} CTR69_M14A0;

/*-----------------------------------------------------------------------------
	0x1118 ctr70 ''
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
} CTR70_M14A0;

/*-----------------------------------------------------------------------------
	0x111c ctr71 ''
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
} CTR71_M14A0;

/*-----------------------------------------------------------------------------
	0x1120 ctr72 ''
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
} CTR72_M14A0;

/*-----------------------------------------------------------------------------
	0x1124 ctr73 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	hub_port_overcurrent_pol        : 2,	//  0: 1
	hub_vbus_pol                    : 2,	//  2: 3
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
} CTR73_M14A0;

/*-----------------------------------------------------------------------------
	0x1128 ctr74 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	c_dr3p_m_ctrl                   : 5,	// 19:23
	c_dr3p_pre_fd_ctrl              : 2,	// 24:25
	c_dr3p_od_fout_ctrl             : 2;	// 26:27
} CTR74_M14A0;

/*-----------------------------------------------------------------------------
	0x112c ctr75 ''
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
} CTR75_M14A0;

/*-----------------------------------------------------------------------------
	0x1130 ctr76 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	m1_dr3p_m_ctrl                  : 5,	// 19:23
	m1_dr3p_pre_fd_ctrl             : 2,	// 24:25
	m1_dr3p_od_fout_ctrl            : 2;	// 26:27
} CTR76_M14A0;

/*-----------------------------------------------------------------------------
	0x1134 ctr77 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	m1_dr3p_udex_ctrl               : 5,	//  0: 4
	m1_dr3p_msex_ctrl               : 9,	//  5:13
	m1_dr3p_nsc_ctrl                : 4,	// 14:17
	m1_dr3p_npc_ctrl                : 6,	// 18:23
	                                : 3,	// 24:26 reserved
	m1_dr3p_cih_ctrl                : 3,	// 27:29
	m1_dr3p_lf_ctrl                 : 1,	//    30
	m1_dr3p_pdb_ctrl                : 1;	//    31
} CTR77_M14A0;

/*-----------------------------------------------------------------------------
	0x1138 ctr78 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	m2_dr3p_m_ctrl                  : 5,	// 19:23
	m2_dr3p_pre_fd_ctrl             : 2,	// 24:25
	m2_dr3p_od_fout_ctrl            : 2;	// 26:27
} CTR78_M14A0;

/*-----------------------------------------------------------------------------
	0x113c ctr79 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	m2_dr3p_udex_ctrl               : 5,	//  0: 4
	m2_dr3p_msex_ctrl               : 9,	//  5:13
	m2_dr3p_nsc_ctrl                : 4,	// 14:17
	m2_dr3p_npc_ctrl                : 6,	// 18:23
	                                : 3,	// 24:26 reserved
	m2_dr3p_cih_ctrl                : 3,	// 27:29
	m2_dr3p_lf_ctrl                 : 1,	//    30
	m2_dr3p_pdb_ctrl                : 1;	//    31
} CTR79_M14A0;

/*-----------------------------------------------------------------------------
	0x1140 ctr80 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
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
} CTR80_M14A0;

/*-----------------------------------------------------------------------------
	0x1144 ctr81 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
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
} CTR81_M14A0;

/*-----------------------------------------------------------------------------
	0x1148 ctr82 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	disp_m_ctrl                     : 5,	// 19:23
	disp_pre_fd_ctrl                : 2,	// 24:25
	disp_od_fout_ctrl               : 2;	// 26:27
} CTR82_M14A0;

/*-----------------------------------------------------------------------------
	0x114c ctr83 ''
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
} CTR83_M14A0;

/*-----------------------------------------------------------------------------
	0x1150 ctr84 ''
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
} CTR84_M14A0;

/*-----------------------------------------------------------------------------
	0x1154 ctr85 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sd_dco_fcw_ctrl                 :22;	//  0:21
} CTR85_M14A0;

/*-----------------------------------------------------------------------------
	0x1158 ctr86 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	lv_flip_en_ctrl                 : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	link1_ch_swap                   : 3,	//  8:10
	                                : 9,	// 11:19 reserved
	lv_pemp_te1_ctrl                : 2,	// 20:21
	lv_pemp_td1_ctrl                : 2,	// 22:23
	lv_pemp_tclk1_ctrl              : 2,	// 24:25
	lv_pemp_tc1_ctrl                : 2,	// 26:27
	lv_pemp_tb1_ctrl                : 2,	// 28:29
	lv_pemp_ta1_ctrl                : 2;	// 30:31
} CTR86_M14A0;

/*-----------------------------------------------------------------------------
	0x115c ctr87 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	lv_rf_ctrl                      : 1,	//     0
	lv_itune5_ctrl                  : 3,	//  1: 3
	lv_itune4_ctrl                  : 3,	//  4: 6
	lv_itune3_ctrl                  : 3,	//  7: 9
	lv_itune2_ctrl                  : 3,	// 10:12
	lv_itune1_ctrl                  : 3,	// 13:15
	lv_itune0_ctrl                  : 3,	// 16:18
	lv_pdb_lane_ctrl                : 6,	// 19:24
	lv_pdb_ch_ctrl                  : 6,	// 25:30
	lv_pdb_ctrl                     : 1;	//    31
} CTR87_M14A0;

/*-----------------------------------------------------------------------------
	0x1160 ctr88 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	arc_odt_sel                     : 2;	// 19:20
} CTR88_M14A0;

/*-----------------------------------------------------------------------------
	0x1164 ctr89 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x1168 ctr90 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x116c ctr91 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x1170 ctr92 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x1174 ctr93 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :12,	//  0:11 reserved
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
} CTR93_M14A0;

/*-----------------------------------------------------------------------------
	0x1178 ctr94 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	gpu_clk_sel                     : 1,	//    19
	                                : 3,	// 20:22 reserved
	pd                              : 1,	//    23
	                                : 3,	// 24:26 reserved
	en_aud_dacrlrch                 : 1,	//    27
	                                : 3,	// 28:30 reserved
	sc_clk_sel                      : 1;	//    31
} CTR94_M14A0;

/*-----------------------------------------------------------------------------
	0x117c ctr95 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	led_vs_en                       : 1,	//     0
	led_i2c_en                      : 1,	//     1
	                                : 1,	//     2 reserved
	hdpmcu1_gpio_en                 : 1,	//     3
	                                : 7,	//  4:10 reserved
	uart2_en                        : 1,	//    11
	                                : 7,	// 12:18 reserved
	i2c6_en                         : 1;	//    19
} CTR95_M14A0;

/*-----------------------------------------------------------------------------
	0x1180 ctr96 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	jtag0_disable                   : 1,	//     0
	jtag1_disable                   : 1,	//     1
	sdio_en                         : 1,	//     2
	swrst_edid                      : 1,	//     3
	reg_tmds_sel                    : 1;	//     4
} CTR96_M14A0;

/*-----------------------------------------------------------------------------
	0x1184 ctr97 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x1188 ctr98 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x118c ctr99 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x1190 ctr100 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :13,	//  0:12 reserved
	c4_count_txlock2                :13,	// 13:25
	c4_sel_clkpix1                  : 3,	// 26:28
	c4_sel_clkpix2                  : 3;	// 29:31
} CTR100_M14A0;

/*-----------------------------------------------------------------------------
	0x1194 ctr101 ''
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
} CTR101_M14A0;

/*-----------------------------------------------------------------------------
	0x1198 ctr102 ''
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
} CTR102_M14A0;

/*-----------------------------------------------------------------------------
	0x119c ctr103 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_sel_lock                     : 1,	//     0
	c4_sel_fin                      : 1,	//     1
	c4_sel_rst                      : 1,	//     2
	c4_sel_lpf                      : 1,	//     3
	c4_sel_bw                       : 1,	//     4
	c4_rf                           : 1,	//     5
	c4_term_en                      : 1,	//     6
	c4_adj_term                     : 4,	//  7:10
	c4_sel_mode                     : 2,	// 11:12
	c4_sw_pud                       : 1,	//    13
	                                : 1,	//    14 reserved
	c4_ib_oft                       : 1,	//    15
	c4_cih                          : 3,	// 16:18
	c4_pll_byte                     : 2,	// 19:20
	c4_pll_byte_vx1                 : 2,	// 21:22
	c4_sel_prediv                   : 1,	//    23
	c4_sw_rext                      : 1,	//    24
	c4_pll_ic                       : 2;	// 25:26
} CTR103_M14A0;

/*-----------------------------------------------------------------------------
	0x11a0 ctr104 ''
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
} CTR104_M14A0;

/*-----------------------------------------------------------------------------
	0x11a4 ctr105 ''
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
} CTR105_M14A0;

/*-----------------------------------------------------------------------------
	0x11a8 ctr106 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_itune20                      : 3,	//  0: 2
	c4_itune21                      : 3,	//  3: 5
	c4_itune22                      : 3,	//  6: 8
	c4_itune23                      : 3;	//  9:11
} CTR106_M14A0;

/*-----------------------------------------------------------------------------
	0x11ac ctr107 ''
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
} CTR107_M14A0;

/*-----------------------------------------------------------------------------
	0x11b0 ctr108 ''
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
	c4_link10_en                     : 1,	//    22
	c4_link11_en                     : 1;	//    23
} CTR108_M14A0;

/*-----------------------------------------------------------------------------
	0x11b4 ctr109 ''
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
} CTR109_M14A0;

/*-----------------------------------------------------------------------------
	0x11b8 ctr110 ''
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
} CTR110_M14A0;

/*-----------------------------------------------------------------------------
	0x11bc ctr111 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c4_pdb_en_ch0                   : 1,	//     0
	c4_pdb_en_ch1                   : 1,	//     1
	c4_pdb_en_ch2                   : 1,	//     2
	c4_pdb_en_ch3                   : 1,	//     3
	c4_pdb_en_ch4                   : 1,	//     4
	c4_pdb_en_ch5                   : 1,	//     5
	c4_pdb_en_ch6                   : 1,	//     6
	c4_pdb_en_ch7                   : 1,	//     7
	c4_pdb_en_ch8                   : 1,	//     8
	c4_pdb_en_ch9                   : 1,	//     9
	c4_pdb_en_ch10                  : 1,	//    10
	c4_pdb_en_ch11                  : 1,	//    11
	c4_pdb_en_ch12                  : 1,	//    12
	c4_pdb_en_ch13                  : 1,	//    13
	c4_pdb_en_ch14                  : 1,	//    14
	c4_pdb_en_ch15                  : 1,	//    15
	c4_pdb_en_ch16                  : 1,	//    16
	c4_pdb_en_ch17                  : 1,	//    17
	c4_pdb_en_ch18                  : 1,	//    18
	c4_pdb_en_ch19                  : 1,	//    19
	c4_pdb_en_ch20                  : 1,	//    20
	c4_pdb_en_ch21                  : 1,	//    21
	c4_pdb_en_ch22                  : 1,	//    22
	c4_pdb_en_ch23                  : 1;	//    23
} CTR111_M14A0;

typedef struct {
	CTR00_M14A0                           	ctr00                           ;	// 0x1000 : ''
	CTR01_M14A0                           	ctr01                           ;	// 0x1004 : ''
	CTR02_M14A0                           	ctr02                           ;	// 0x1008 : ''
	CTR03_M14A0                           	ctr03                           ;	// 0x100c : ''
	CTR04_M14A0                           	ctr04                           ;	// 0x1010 : ''
	CTR05_M14A0                           	ctr05                           ;	// 0x1014 : ''
	CTR06_M14A0                           	ctr06                           ;	// 0x1018 : ''
	CTR07_M14A0                           	ctr07                           ;	// 0x101c : ''
	CTR08_M14A0                           	ctr08                           ;	// 0x1020 : ''
	CTR09_M14A0                           	ctr09                           ;	// 0x1024 : ''
	CTR10_M14A0                           	ctr10                           ;	// 0x1028 : ''
	CTR11_M14A0                           	ctr11                           ;	// 0x102c : ''
	CTR12_M14A0                           	ctr12                           ;	// 0x1030 : ''
	CTR13_M14A0                           	ctr13                           ;	// 0x1034 : ''
	CTR14_M14A0                           	ctr14                           ;	// 0x1038 : ''
	CTR15_M14A0                           	ctr15                           ;	// 0x103c : ''
	CTR16_M14A0                           	ctr16                           ;	// 0x1040 : ''
	CTR17_M14A0                           	ctr17                           ;	// 0x1044 : ''
	CTR18_M14A0                           	ctr18                           ;	// 0x1048 : ''
	CTR19_M14A0                           	ctr19                           ;	// 0x104c : ''
	CTR20_M14A0                           	ctr20                           ;	// 0x1050 : ''
	CTR21_M14A0                           	ctr21                           ;	// 0x1054 : ''
	CTR22_M14A0                           	ctr22                           ;	// 0x1058 : ''
	CTR23_M14A0                           	ctr23                           ;	// 0x105c : ''
	CTR24_M14A0                           	ctr24                           ;	// 0x1060 : ''
	CTR25_M14A0                           	ctr25                           ;	// 0x1064 : ''
	CTR26_M14A0                           	ctr26                           ;	// 0x1068 : ''
	CTR27_M14A0                           	ctr27                           ;	// 0x106c : ''
	CTR28_M14A0                           	ctr28                           ;	// 0x1070 : ''
	CTR29_M14A0                           	ctr29                           ;	// 0x1074 : ''
	CTR30_M14A0                           	ctr30                           ;	// 0x1078 : ''
	CTR31_M14A0                           	ctr31                           ;	// 0x107c : ''
	CTR32_M14A0                           	ctr32                           ;	// 0x1080 : ''
	CTR33_M14A0                           	ctr33                           ;	// 0x1084 : ''
	CTR34_M14A0                           	ctr34                           ;	// 0x1088 : ''
	CTR35_M14A0                           	ctr35                           ;	// 0x108c : ''
	CTR36_M14A0                           	ctr36                           ;	// 0x1090 : ''
	CTR37_M14A0                           	ctr37                           ;	// 0x1094 : ''
	CTR38_M14A0                           	ctr38                           ;	// 0x1098 : ''
	CTR39_M14A0                           	ctr39                           ;	// 0x109c : ''
	CTR40_M14A0                           	ctr40                           ;	// 0x10a0 : ''
	CTR41_M14A0                           	ctr41                           ;	// 0x10a4 : ''
	CTR42_M14A0                           	ctr42                           ;	// 0x10a8 : ''
	CTR43_M14A0                           	ctr43                           ;	// 0x10ac : ''
	CTR44_M14A0                           	ctr44                           ;	// 0x10b0 : ''
	CTR45_M14A0                           	ctr45                           ;	// 0x10b4 : ''
	CTR46_M14A0                           	ctr46                           ;	// 0x10b8 : ''
	CTR47_M14A0                           	ctr47                           ;	// 0x10bc : ''
	CTR48_M14A0                           	ctr48                           ;	// 0x10c0 : ''
	CTR49_M14A0                           	ctr49                           ;	// 0x10c4 : ''
	CTR50_M14A0                           	ctr50                           ;	// 0x10c8 : ''
	CTR51_M14A0                           	ctr51                           ;	// 0x10cc : ''
	CTR52_M14A0                           	ctr52                           ;	// 0x10d0 : ''
	CTR53_M14A0                           	ctr53                           ;	// 0x10d4 : ''
	CTR54_M14A0                           	ctr54                           ;	// 0x10d8 : ''
	CTR55_M14A0                           	ctr55                           ;	// 0x10dc : ''
	CTR56_M14A0                           	ctr56                           ;	// 0x10e0 : ''
	CTR57_M14A0                           	ctr57                           ;	// 0x10e4 : ''
	CTR58_M14A0                           	ctr58                           ;	// 0x10e8 : ''
	CTR59_M14A0                           	ctr59                           ;	// 0x10ec : ''
	CTR60_M14A0                           	ctr60                           ;	// 0x10f0 : ''
	CTR61_M14A0                           	ctr61                           ;	// 0x10f4 : ''
	CTR62_M14A0                           	ctr62                           ;	// 0x10f8 : ''
	CTR63_M14A0                           	ctr63                           ;	// 0x10fc : ''
	CTR64_M14A0                           	ctr64                           ;	// 0x1100 : ''
	CTR65_M14A0                           	ctr65                           ;	// 0x1104 : ''
	CTR66_M14A0                        		ctr66                           ;	// 0x1108 : ''
	CTR67_M14A0                        		ctr67                           ;	// 0x110c : ''
	CTR68_M14A0                        		ctr68                           ;	// 0x1110 : ''
	CTR69_M14A0                        		ctr69                           ;	// 0x1114 : ''
	CTR70_M14A0                           	ctr70                           ;	// 0x1118 : ''
	CTR71_M14A0                           	ctr71                           ;	// 0x111c : ''
	CTR72_M14A0                           	ctr72                           ;	// 0x1120 : ''
	CTR73_M14A0                           	ctr73                           ;	// 0x1124 : ''
	CTR74_M14A0                           	ctr74                           ;	// 0x1128 : ''
	CTR75_M14A0                           	ctr75                           ;	// 0x112c : ''
	CTR76_M14A0                           	ctr76                           ;	// 0x1130 : ''
	CTR77_M14A0                           	ctr77                           ;	// 0x1134 : ''
	CTR78_M14A0                           	ctr78                           ;	// 0x1138 : ''
	CTR79_M14A0                           	ctr79                           ;	// 0x113c : ''
	CTR80_M14A0                        		ctr80                           ;	// 0x1140 : ''
	CTR81_M14A0                        		ctr81                           ;	// 0x1144 : ''
	CTR82_M14A0                           	ctr82                           ;	// 0x1148 : ''
	CTR83_M14A0                           	ctr83                           ;	// 0x114c : ''
	CTR84_M14A0                           	ctr84                           ;	// 0x1150 : ''
	CTR85_M14A0                           	ctr85                           ;	// 0x1154 : ''
	CTR86_M14A0                           	ctr86                           ;	// 0x1158 : ''
	CTR87_M14A0                           	ctr87                           ;	// 0x115c : ''
	CTR88_M14A0                           	ctr88                           ;	// 0x1160 : ''
	UINT32                           		ctr89                           ;	// 0x1164 : ''
	UINT32                           		ctr90                           ;	// 0x1168 : ''
	UINT32                           		ctr91                           ;	// 0x116c : ''
	UINT32                           		ctr92                           ;	// 0x1170 : ''
	CTR93_M14A0                           	ctr93                           ;	// 0x1174 : ''
	CTR94_M14A0                           	ctr94                           ;	// 0x1178 : ''
	CTR95_M14A0                           	ctr95                           ;	// 0x117c : ''
	CTR96_M14A0                          	ctr96                           ;	// 0x1180 : ''
	UINT32                          		ctr97                           ;	// 0x1184 : ''
	UINT32                          		ctr98                           ;	// 0x1188 : ''
	UINT32                          		ctr99                           ;	// 0x118c : ''
	CTR100_M14A0                          	ctr100                          ;	// 0x1190 : ''
	CTR101_M14A0                          	ctr101                          ;	// 0x1194 : ''
	CTR102_M14A0                          	ctr102                          ;	// 0x1198 : ''
	CTR103_M14A0                          	ctr103                          ;	// 0x119c : ''
	CTR104_M14A0                          	ctr104                          ;	// 0x11a0 : ''
	CTR105_M14A0                          	ctr105                          ;	// 0x11a4 : ''
	CTR106_M14A0                          	ctr106                          ;	// 0x11a8 : ''
	CTR107_M14A0                          	ctr107                          ;	// 0x11ac : ''
	CTR108_M14A0                          	ctr108                          ;	// 0x11b0 : ''
	CTR109_M14A0                          	ctr109                          ;	// 0x11b4 : ''
	CTR110_M14A0                          	ctr110                          ;	// 0x11b8 : ''
	CTR111_M14A0                          	ctr111                          ;	// 0x11bc : ''
} CTOP_CTRL_REG_M14A0_T;


#endif	/* __CTOP_CTRL_REG_M14A0_H__ */

