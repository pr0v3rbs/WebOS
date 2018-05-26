#ifndef __CTOP_CTRL_REG_H13A0_H__
#define __CTOP_CTRL_REG_H13A0_H__

/*-----------------------------------------------------------------------------
	0x000 ctr00 ''
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
	                                : 9,	// 11:19 reserved
	swrst_apb                       : 1,	//    20
	swrst_srv                       : 1,	//    21
	                                : 1,	//    22 reserved
	swrst_bus                       : 1,	//    23
	                                : 3,	// 24:26 reserved
	swrst_usb_bt_init               : 1,	//    27
	                                : 1,	//    28 reserved
	swrst_usb2_init                 : 1,	//    29
	swrst_usb1_init                 : 1,	//    30
	swrst_usb0_init                 : 1;	//    31
} CTR00_H13A0;

/*-----------------------------------------------------------------------------
	0x004 ctr01 ''
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
	bus_clk_sel                     : 2;	// 28:29
} CTR01_H13A0;

/*-----------------------------------------------------------------------------
	0x008 ctr02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :29,	//  0:28 reserved
	swrst_gpu_mem                   : 1,	//    29
	swrst_gpu_core                  : 1,	//    30
	swrst_gpu_sys                   : 1;	//    31
} CTR02_H13A0;

/*-----------------------------------------------------------------------------
	0x00c ctr03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :28,	//  0:27 reserved
	gpu_sys_clk_sel                 : 2;	// 28:29
} CTR03_H13A0;

/*-----------------------------------------------------------------------------
	0x010 ctr04 ''
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
} CTR04_H13A0;

/*-----------------------------------------------------------------------------
	0x014 ctr05 ''
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
} CTR05_H13A0;

/*-----------------------------------------------------------------------------
	0x018 ctr06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :10,	//  0: 9 reserved
	icodclk_sel                     : 3,	// 10:12
	                                : 3,	// 13:15 reserved
	te_teclk_sel                    : 3,	// 16:18
	                                : 6,	// 19:24 reserved
	veclk_sel                       : 3,	// 25:27
	reg_seclk_inv                   : 1;	//    28
} CTR06_H13A0;

/*-----------------------------------------------------------------------------
	0x01c ctr07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	gfx_aclk_sel                    : 2,	//  4: 5
	                                :21,	//  6:26 reserved
	swrst_gfx                       : 1,	//    27
	                                : 3,	// 28:30 reserved
	swrst_gfx_a                     : 1;	//    31
} CTR07_H13A0;

/*-----------------------------------------------------------------------------
	0x020 ctr08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :20,	//  0:19 reserved
	trace_sel                       : 3,	// 20:22
	dbgvsoc_sel                     : 3,	// 23:25
	atvsoc_sel                      : 3,	// 26:28
	atclk_sel                       : 3;	// 29:31
} CTR08_H13A0;

/*-----------------------------------------------------------------------------
	0x024 ctr09 ''
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
} CTR09_H13A0;

/*-----------------------------------------------------------------------------
	0x028 ctr10 ''
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
} CTR10_H13A0;

/*-----------------------------------------------------------------------------
	0x02c ctr11 ''
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
} CTR11_H13A0;

/*-----------------------------------------------------------------------------
	0x030 ctr12 ''
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
} CTR12_H13A0;

/*-----------------------------------------------------------------------------
	0x034 ctr13 ''
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
} CTR13_H13A0;

/*-----------------------------------------------------------------------------
	0x038 ctr14 ''
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
} CTR14_H13A0;

/*-----------------------------------------------------------------------------
	0x03c ctr15 ''
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
} CTR15_H13A0;

/*-----------------------------------------------------------------------------
	0x040 ctr16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :21,	//  0:20 reserved
	bus10_sel                       : 3,	// 21:23
	                                : 1,	//    24 reserved
	bus9_sel                        : 3,	// 25:27
	                                : 1,	//    28 reserved
	bus8_sel                        : 3;	// 29:31
} CTR16_H13A0;

/*-----------------------------------------------------------------------------
	0x044 ctr17 ''
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
} CTR17_H13A0;

/*-----------------------------------------------------------------------------
	0x048 ctr18 ''
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
} CTR18_H13A0;

/*-----------------------------------------------------------------------------
	0x04c ctr19 ''
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
} CTR19_H13A0;

/*-----------------------------------------------------------------------------
	0x050 ctr20 ''
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
} CTR20_H13A0;

/*-----------------------------------------------------------------------------
	0x054 ctr21 ''
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
} CTR21_H13A0;

/*-----------------------------------------------------------------------------
	0x058 ctr22 ''
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
} CTR22_H13A0;

/*-----------------------------------------------------------------------------
	0x05c ctr23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :15,	//  0:14 reserved
	reg_disp_ded_p                  : 1,	//    15
	reg_disp_ded                    : 1,	//    16
	reg_disp_dp                     : 1,	//    17
	reg_disp_de                     : 1,	//    18
	                                : 2,	// 19:20 reserved
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
} CTR23_H13A0;

/*-----------------------------------------------------------------------------
	0x060 ctr24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	swrst_ded_disp                  : 1,	//     7
	swrst_deb_disp                  : 1,	//     8
	                                : 4,	//  9:12 reserved
	dppb_if_clk_sel                 : 2,	// 13:14
	                                : 1,	//    15 reserved
	dppa_clk_sel                    : 2,	// 16:17
	                                : 1,	//    18 reserved
	sre_sys_clk_sel                 : 2,	// 19:20
	                                : 1,	//    21 reserved
	frc_dvi_clk_sel                 : 2,	// 22:23
	frc_cho_clk_sel                 : 2,	// 24:25
	                                : 1,	//    26 reserved
	frc_dvo_off_sel                 : 1,	//    27
	h3d_clk_sel                     : 2;	// 28:29
} CTR24_H13A0;

/*-----------------------------------------------------------------------------
	0x064 ctr25 ''
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
} CTR25_H13A0;

/*-----------------------------------------------------------------------------
	0x068 ctr26 ''
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
} CTR26_H13A0;

/*-----------------------------------------------------------------------------
	0x06c ctr27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	cvda_clk_sel                    : 3,	//  8:10
	                                : 9,	// 11:19 reserved
	dec_mif_dea_clk_sel             : 3,	// 20:22
	                                : 1,	//    23 reserved
	cve27_inv_sel                   : 1,	//    24
	cve54_inv_sel                   : 1;	//    25
} CTR27_H13A0;

/*-----------------------------------------------------------------------------
	0x070 ctr28 ''
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
} CTR28_H13A0;

/*-----------------------------------------------------------------------------
	0x074 ctr29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :17,	//  0:16 reserved
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
} CTR29_H13A0;

/*-----------------------------------------------------------------------------
	0x078 ctr30 ''
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
} CTR30_H13A0;

/*-----------------------------------------------------------------------------
	0x07c ctr31 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :17,	//  0:16 reserved
	vd_declk_sel                    : 3,	// 17:19
	                                : 6,	// 20:25 reserved
	dppb_apb_sel                    : 3;	// 26:28
} CTR31_H13A0;

/*-----------------------------------------------------------------------------
	0x080 ctr32 ''
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
	aud_jtag_no                     : 1,	//    11
	jtag0_sel                       : 1,	//    12
	                                : 8,	// 13:20 reserved
	pll_sel_core                    : 1;	//    21
} CTR32_H13A0;

/*-----------------------------------------------------------------------------
	0x084 ctr33 ''
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
} CTR33_H13A0;

/*-----------------------------------------------------------------------------
	0x088 ctr34 ''
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
} CTR34_H13A0;

/*-----------------------------------------------------------------------------
	0x08c ctr35 ''
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
} CTR35_H13A0;

/*-----------------------------------------------------------------------------
	0x090 ctr36 ''
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
} CTR36_H13A0;

/*-----------------------------------------------------------------------------
	0x094 ctr37 ''
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
} CTR37_H13A0;

/*-----------------------------------------------------------------------------
	0x098 ctr38 ''
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
} CTR38_H13A0;

/*-----------------------------------------------------------------------------
	0x09c ctr39 ''
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
} CTR39_H13A0;

/*-----------------------------------------------------------------------------
	0x0a0 ctr40 ''
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
} CTR40_H13A0;

/*-----------------------------------------------------------------------------
	0x0a4 ctr41 ''
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
} CTR41_H13A0;

/*-----------------------------------------------------------------------------
	0x0a8 ctr42 ''
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
} CTR42_H13A0;

/*-----------------------------------------------------------------------------
	0x0ac ctr43 ''
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
} CTR43_H13A0;

/*-----------------------------------------------------------------------------
	0x0b0 ctr44 ''
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
} CTR44_H13A0;

/*-----------------------------------------------------------------------------
	0x0b4 ctr45 ''
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
} CTR45_H13A0;

/*-----------------------------------------------------------------------------
	0x0b8 ctr46 ''
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
} CTR46_H13A0;

/*-----------------------------------------------------------------------------
	0x0bc ctr47 ''
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
} CTR47_H13A0;

/*-----------------------------------------------------------------------------
	0x0c0 ctr48 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pull_up_control_spi_cs1         : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pull_up_control_spi_sclk1       : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_controh_spi_do1         : 1,	//    11
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
} CTR48_H13A0;

/*-----------------------------------------------------------------------------
	0x0c4 ctr49 ''
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
} CTR49_H13A0;

/*-----------------------------------------------------------------------------
	0x0c8 ctr50 ''
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
} CTR50_H13A0;

/*-----------------------------------------------------------------------------
	0x0cc ctr51 ''
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
} CTR51_H13A0;

/*-----------------------------------------------------------------------------
	0x0d0 ctr52 ''
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
} CTR52_H13A0;

/*-----------------------------------------------------------------------------
	0x0d4 ctr53 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pull_up_control_cam_inpack_n    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pull_up_control_cam_ireq_n      : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_controh_cam_reg_n       : 1,	//    11
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
} CTR53_H13A0;

/*-----------------------------------------------------------------------------
	0x0d8 ctr54 ''
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
} CTR54_H13A0;

/*-----------------------------------------------------------------------------
	0x0dc ctr55 ''
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
} CTR55_H13A0;

/*-----------------------------------------------------------------------------
	0x0e0 ctr56 ''
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
} CTR56_H13A0;

/*-----------------------------------------------------------------------------
	0x0e4 ctr57 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :11,	//  0:10 reserved
	pull_up_control_sd_data         : 1,	//    15
	                                : 7,	// 12:18 reserved
	pull_up_control_sd_wp_n         : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control_sd_cd_n         : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control_sd_cmd          : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control_sd_clk          : 1;	//    31
} CTR57_H13A0;

/*-----------------------------------------------------------------------------
	0x0e8 ctr58 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tpio_sel_ctrl                   : 1,	//     0
	tpo_sel_ctrl                    : 1,	//     1
	stpio_en_ctrl                   : 1,	//     2
	                                : 1,	//     3 reserved
	ts_sel                          : 4,	//  4: 7
	                                : 2,	//  8: 9 reserved
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
} CTR58_H13A0;

/*-----------------------------------------------------------------------------
	0x0ec ctr59 ''
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
} CTR59_H13A0;

/*-----------------------------------------------------------------------------
	0x0f0 ctr60 ''
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
} CTR60_H13A0;

/*-----------------------------------------------------------------------------
	0x0f4 ctr61 ''
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
} CTR61_H13A0;

/*-----------------------------------------------------------------------------
	0x0f8 ctr62 ''
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
} CTR62_H13A0;

/*-----------------------------------------------------------------------------
	0x0fc ctr63 ''
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
} CTR63_H13A0;

/*-----------------------------------------------------------------------------
	0x100 ctr64 ''
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
} CTR64_H13A0;

/*-----------------------------------------------------------------------------
	0x104 ctr65 ''
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
} CTR65_H13A0;

/*-----------------------------------------------------------------------------
	0x108 ctr66 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x10c ctr67 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x110 ctr68 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x114 ctr69 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x118 ctr70 ''
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
} CTR70_H13A0;

/*-----------------------------------------------------------------------------
	0x11c ctr71 ''
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
} CTR71_H13A0;

/*-----------------------------------------------------------------------------
	0x120 ctr72 ''
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
} CTR72_H13A0;

/*-----------------------------------------------------------------------------
	0x124 ctr73 ''
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
} CTR73_H13A0;

/*-----------------------------------------------------------------------------
	0x128 ctr74 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	c_dr3p_m_ctrl                   : 5,	// 19:23
	c_dr3p_pre_fd_ctrl              : 2,	// 24:25
	c_dr3p_od_fout_ctrl             : 2;	// 26:27
} CTR74_H13A0;

/*-----------------------------------------------------------------------------
	0x12c ctr75 ''
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
} CTR75_H13A0;

/*-----------------------------------------------------------------------------
	0x130 ctr76 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	m1_dr3p_m_ctrl                  : 5,	// 19:23
	m1_dr3p_pre_fd_ctrl             : 2,	// 24:25
	m1_dr3p_od_fout_ctrl            : 2;	// 26:27
} CTR76_H13A0;

/*-----------------------------------------------------------------------------
	0x134 ctr77 ''
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
} CTR77_H13A0;

/*-----------------------------------------------------------------------------
	0x138 ctr78 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	m2_dr3p_m_ctrl                  : 5,	// 19:23
	m2_dr3p_pre_fd_ctrl             : 2,	// 24:25
	m2_dr3p_od_fout_ctrl            : 2;	// 26:27
} CTR78_H13A0;

/*-----------------------------------------------------------------------------
	0x13c ctr79 ''
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
} CTR79_H13A0;

/*-----------------------------------------------------------------------------
	0x140 ctr80 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x144 ctr81 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x148 ctr82 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	disp_m_ctrl                     : 5,	// 19:23
	disp_pre_fd_ctrl                : 2,	// 24:25
	disp_od_fout_ctrl               : 2;	// 26:27
} CTR82_H13A0;

/*-----------------------------------------------------------------------------
	0x14c ctr83 ''
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
} CTR83_H13A0;

/*-----------------------------------------------------------------------------
	0x150 ctr84 ''
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
} CTR84_H13A0;

/*-----------------------------------------------------------------------------
	0x154 ctr85 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sd_dco_fcw_ctrl                 :22;	//  0:21
} CTR85_H13A0;

/*-----------------------------------------------------------------------------
	0x158 ctr86 ''
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
} CTR86_H13A0;

/*-----------------------------------------------------------------------------
	0x15c ctr87 ''
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
} CTR87_H13A0;

/*-----------------------------------------------------------------------------
	0x160 ctr88 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	arc_odt_sel                     : 2;	// 19:20
} CTR88_H13A0;

/*-----------------------------------------------------------------------------
	0x164 ctr89 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	txpll_nsc                       : 4,	//  0: 3
	txpll_npc                       : 6,	//  4: 9
	txpll_udex                      : 5,	// 10:14
	txpll_msex                      : 9,	// 15:23
	txpll_m                         : 5,	// 24:28
	txpll_cih                       : 3;	// 29:31
} CTR89_H13A0;

/*-----------------------------------------------------------------------------
	0x168 ctr90 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	viref_use_rext                  : 1,	//     7
	rf                              : 1,	//     8
	viref_itune                     : 2,	//  9:10
	pemp                            : 2,	// 11:12
	                                : 1,	//    13 reserved
	pdb_lane                        : 6,	// 14:19
	pdb                             : 1,	//    20
	flip_en                         : 1,	//    21
	adj_ter                         : 4,	// 22:25
	                                : 1,	//    26 reserved
	txpll_ibw                       : 1,	//    27
	                                : 1,	//    28 reserved
	txpll_ckin_sel                  : 1,	//    29
	txpll_ckin_fdiv                 : 2;	// 30:31
} CTR90_H13A0;

/*-----------------------------------------------------------------------------
	0x16c ctr91 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	swrst                           : 1,	//     7
	                                : 2,	//  8: 9 reserved
	encode_off                      : 1,	//    10
	scr_off                         : 1,	//    11
	test_di                         :10,	// 12:21
	                                : 2,	// 22:23 reserved
	phy_cal                         : 8;	// 24:31
} CTR91_H13A0;

/*-----------------------------------------------------------------------------
	0x170 ctr92 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	test_mode                       : 2,	//  0: 1
	                                : 1,	//     2 reserved
	enc_rst                         : 1,	//     3
	scr_rst                         : 1,	//     4
	sel_clkpix                      : 3,	//  5: 7
	txlock_fixen                    : 1,	//     8
	txlock_count                    :13,	//  9:21
	rxlock_count                    : 6,	// 22:27
	link3_en                        : 1,	//    28
	link2_en                        : 1,	//    29
	link1_en                        : 1,	//    30
	link0_en                        : 1;	//    31
} CTR92_H13A0;

/*-----------------------------------------------------------------------------
	0x174 ctr93 ''
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
} CTR93_H13A0;

/*-----------------------------------------------------------------------------
	0x178 ctr94 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	hdmi_pd                         : 1,	//    19
	                                : 3,	// 20:22 reserved
	pd                              : 1,	//    23
	                                : 3,	// 24:26 reserved
	en_aud_dacrlrch                 : 1,	//    27
	                                : 3,	// 28:30 reserved
	sc_clk_sel                      : 1;	//    31
} CTR94_H13A0;

/*-----------------------------------------------------------------------------
	0x17c ctr95 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :10,	//  0: 9 reserved
	uart_sel                        : 1,	//    10
	uart_en                         : 1,	//    11
	                                : 3,	// 12:14 reserved
	ext_tp_en                       : 1,	//    15
	                                : 3,	// 16:18 reserved
	i2c_en3                          : 1,	//    19
	                                : 3,	// 20:22 reserved
	i2c_en4                          : 1,	//    23
	                                : 3,	// 24:26 reserved
	i2c_en5                          : 1,	//    27
	                                : 3,	// 28:30 reserved
	i2c_en6                          : 1;	//    31
} CTR95_H13A0;

/*-----------------------------------------------------------------------------
	0x180 ctr96 ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x184 ctr97 ''
------------------------------------------------------------------------------*/
/*	no field */

typedef struct {
	CTR00_H13A0                           	ctr00                           ;	// 0x000 : ''
	CTR01_H13A0                           	ctr01                           ;	// 0x004 : ''
	CTR02_H13A0                           	ctr02                           ;	// 0x008 : ''
	CTR03_H13A0                           	ctr03                           ;	// 0x00c : ''
	CTR04_H13A0                           	ctr04                           ;	// 0x010 : ''
	CTR05_H13A0                           	ctr05                           ;	// 0x014 : ''
	CTR06_H13A0                           	ctr06                           ;	// 0x018 : ''
	CTR07_H13A0                           	ctr07                           ;	// 0x01c : ''
	CTR08_H13A0                           	ctr08                           ;	// 0x020 : ''
	CTR09_H13A0                           	ctr09                           ;	// 0x024 : ''
	CTR10_H13A0                           	ctr10                           ;	// 0x028 : ''
	CTR11_H13A0                           	ctr11                           ;	// 0x02c : ''
	CTR12_H13A0                           	ctr12                           ;	// 0x030 : ''
	CTR13_H13A0                           	ctr13                           ;	// 0x034 : ''
	CTR14_H13A0                           	ctr14                           ;	// 0x038 : ''
	CTR15_H13A0                           	ctr15                           ;	// 0x03c : ''
	CTR16_H13A0                           	ctr16                           ;	// 0x040 : ''
	CTR17_H13A0                           	ctr17                           ;	// 0x044 : ''
	CTR18_H13A0                           	ctr18                           ;	// 0x048 : ''
	CTR19_H13A0                           	ctr19                           ;	// 0x04c : ''
	CTR20_H13A0                           	ctr20                           ;	// 0x050 : ''
	CTR21_H13A0                           	ctr21                           ;	// 0x054 : ''
	CTR22_H13A0                           	ctr22                           ;	// 0x058 : ''
	CTR23_H13A0                           	ctr23                           ;	// 0x05c : ''
	CTR24_H13A0                           	ctr24                           ;	// 0x060 : ''
	CTR25_H13A0                           	ctr25                           ;	// 0x064 : ''
	CTR26_H13A0                           	ctr26                           ;	// 0x068 : ''
	CTR27_H13A0                           	ctr27                           ;	// 0x06c : ''
	CTR28_H13A0                           	ctr28                           ;	// 0x070 : ''
	CTR29_H13A0                           	ctr29                           ;	// 0x074 : ''
	CTR30_H13A0                           	ctr30                           ;	// 0x078 : ''
	CTR31_H13A0                           	ctr31                           ;	// 0x07c : ''
	CTR32_H13A0                           	ctr32                           ;	// 0x080 : ''
	CTR33_H13A0                           	ctr33                           ;	// 0x084 : ''
	CTR34_H13A0                           	ctr34                           ;	// 0x088 : ''
	CTR35_H13A0                           	ctr35                           ;	// 0x08c : ''
	CTR36_H13A0                           	ctr36                           ;	// 0x090 : ''
	CTR37_H13A0                           	ctr37                           ;	// 0x094 : ''
	CTR38_H13A0                           	ctr38                           ;	// 0x098 : ''
	CTR39_H13A0                           	ctr39                           ;	// 0x09c : ''
	CTR40_H13A0                           	ctr40                           ;	// 0x0a0 : ''
	CTR41_H13A0                           	ctr41                           ;	// 0x0a4 : ''
	CTR42_H13A0                           	ctr42                           ;	// 0x0a8 : ''
	CTR43_H13A0                           	ctr43                           ;	// 0x0ac : ''
	CTR44_H13A0                           	ctr44                           ;	// 0x0b0 : ''
	CTR45_H13A0                           	ctr45                           ;	// 0x0b4 : ''
	CTR46_H13A0                           	ctr46                           ;	// 0x0b8 : ''
	CTR47_H13A0                           	ctr47                           ;	// 0x0bc : ''
	CTR48_H13A0                           	ctr48                           ;	// 0x0c0 : ''
	CTR49_H13A0                           	ctr49                           ;	// 0x0c4 : ''
	CTR50_H13A0                           	ctr50                           ;	// 0x0c8 : ''
	CTR51_H13A0                           	ctr51                           ;	// 0x0cc : ''
	CTR52_H13A0                           	ctr52                           ;	// 0x0d0 : ''
	CTR53_H13A0                           	ctr53                           ;	// 0x0d4 : ''
	CTR54_H13A0                           	ctr54                           ;	// 0x0d8 : ''
	CTR55_H13A0                           	ctr55                           ;	// 0x0dc : ''
	CTR56_H13A0                           	ctr56                           ;	// 0x0e0 : ''
	CTR57_H13A0                           	ctr57                           ;	// 0x0e4 : ''
	CTR58_H13A0                           	ctr58                           ;	// 0x0e8 : ''
	CTR59_H13A0                           	ctr59                           ;	// 0x0ec : ''
	CTR60_H13A0                           	ctr60                           ;	// 0x0f0 : ''
	CTR61_H13A0                           	ctr61                           ;	// 0x0f4 : ''
	CTR62_H13A0                           	ctr62                           ;	// 0x0f8 : ''
	CTR63_H13A0                           	ctr63                           ;	// 0x0fc : ''
	CTR64_H13A0                           	ctr64                           ;	// 0x100 : ''
	CTR65_H13A0                           	ctr65                           ;	// 0x104 : ''
	UINT32                              	ctr66                           ;	// 0x108 : ''
	UINT32                              	ctr67                           ;	// 0x10c : ''
	UINT32                              	ctr68                           ;	// 0x110 : ''
	UINT32                              	ctr69                           ;	// 0x114 : ''
	CTR70_H13A0                           	ctr70                           ;	// 0x118 : ''
	CTR71_H13A0                           	ctr71                           ;	// 0x11c : ''
	CTR72_H13A0                           	ctr72                           ;	// 0x120 : ''
	CTR73_H13A0                           	ctr73                           ;	// 0x124 : ''
	CTR74_H13A0                           	ctr74                           ;	// 0x128 : ''
	CTR75_H13A0                           	ctr75                           ;	// 0x12c : ''
	CTR76_H13A0                           	ctr76                           ;	// 0x130 : ''
	CTR77_H13A0                           	ctr77                           ;	// 0x134 : ''
	CTR78_H13A0                           	ctr78                           ;	// 0x138 : ''
	CTR79_H13A0                           	ctr79                           ;	// 0x13c : ''
	UINT32                              	ctr80                           ;	// 0x140 : ''
	UINT32                              	ctr81                           ;	// 0x144 : ''
	CTR82_H13A0                           	ctr82                           ;	// 0x148 : ''
	CTR83_H13A0                           	ctr83                           ;	// 0x14c : ''
	CTR84_H13A0                           	ctr84                           ;	// 0x150 : ''
	CTR85_H13A0                           	ctr85                           ;	// 0x154 : ''
	CTR86_H13A0                           	ctr86                           ;	// 0x158 : ''
	CTR87_H13A0                           	ctr87                           ;	// 0x15c : ''
	CTR88_H13A0                           	ctr88                           ;	// 0x160 : ''
	CTR89_H13A0                           	ctr89                           ;	// 0x164 : ''
	CTR90_H13A0                           	ctr90                           ;	// 0x168 : ''
	CTR91_H13A0                           	ctr91                           ;	// 0x16c : ''
	CTR92_H13A0                           	ctr92                           ;	// 0x170 : ''
	CTR93_H13A0                           	ctr93                           ;	// 0x174 : ''
	CTR94_H13A0                           	ctr94                           ;	// 0x178 : ''
	CTR95_H13A0                           	ctr95                           ;	// 0x17c : ''
	UINT32                              	ctr96                           ;	// 0x180 : ''
	UINT32                              	ctr97                           ;	// 0x184 : ''
} CTOP_CTRL_REG_H13A0_T;

#endif	// _REG_CTOP_CTRL_H13A0_H

/* from 'H13-CTOP-SUM-02-(TOP_CTRL_REG).csv'  대한민국 표준시 by getregs v2.8 */
