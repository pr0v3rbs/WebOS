#ifndef _REG_CTOP_CTRL_L9B0_h
#define _REG_CTOP_CTRL_L9B0_h

/*-----------------------------------------------------------------------------
	0x0010 ctr00_reg_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	gfx_aclk_sel                    : 3,	//  1: 3
	                                : 1,	//     4 reserved
	gfx_pclk_sel                    : 3,	//  5: 7
	                                : 5,	//  8:12 reserved
	cvda_clk_sel                    : 3,	// 13:15
	                                : 5,	// 16:20 reserved
	de_apb_clk_sel                  : 3,	// 21:23
	                                : 3,	// 24:26 reserved
	swrst_usb_bt_init               : 1,	//    27
	slt_apbclk_sel                  : 3,	// 28:30
	swrst_usb_init                  : 1;	//    31
} L9B0_CTR00_REG_CLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0014 ctr01_reg_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	te_ahclk_sel                    : 3,	//  1: 3
	                                : 1,	//     4 reserved
	te_teclk_sel                    : 3,	//  5: 7
	ve_aclk_sel                     : 3,	//  8:10
	ve270clk_sel                    : 3,	// 11:13
	veclk_sel                       : 3,	// 14:16
	sive_apbclk_sel                 : 3,	// 17:19
	gpu1clk_off_sel                 : 1,	//    20
	gpuclk_sel                      : 3,	// 21:23
	                                : 1,	//    24 reserved
	gpu_apbclk_sel                  : 3,	// 25:27
	gpu0clk_off_sel                 : 1,	//    28
	gfx_clk_sel                     : 3;	// 29:31
} L9B0_CTR01_REG_CLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0018 ctr02_reg_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	vd_declk_sel                    : 3,	//  2: 4
	vd_teclk_sel                    : 3,	//  5: 7
	vd_aclk_sel                     : 3,	//  8:10
	vd2clk_sel                      : 3,	// 11:13
	vd1clk_sel                      : 3,	// 14:16
	vdclk_sel                       : 3,	// 17:19
	vd_apbclk_sel                   : 3,	// 20:22
	icod_aclk_sel                   : 3,	// 23:25
	icodclk_sel                     : 3,	// 26:28
	te_aclk_sel                     : 3;	// 29:31
} L9B0_CTR02_REG_CLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x001c ctr03_reg_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	f0_aclk_sel                     : 3,	//  2: 4
	flexnoc2_aclk_sel               : 3,	//  5: 7
	flexnoc1_aclk_sel               : 3,	//  8:10
	flexnoc0_aclk_sel               : 3,	// 11:13
	aud_veclk_sel                   : 3,	// 14:16
	aud_teclk_sel                   : 3,	// 17:19
	aud_apbclk_sel                  : 3,	// 20:22
	aud_aclk_sel                    : 3,	// 23:25
	aud_dsp1aclk_sel                : 3,	// 26:28
	aud_dsp0aclk_sel                : 3;	// 29:31
} L9B0_CTR03_REG_CLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0020 ctr04_reg_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	at2_apbclk_sel                  : 3,	//  8:10
	at1_apbclk_sel                  : 3,	// 11:13
	at0_apbclk_sel                  : 3,	// 14:16
	s2_aclk_sel                     : 3,	// 17:19
	s1_aclk_sel                     : 3,	// 20:22
	s0_aclk_sel                     : 3,	// 23:25
	f2_aclk_sel                     : 3,	// 26:28
	f1_aclk_sel                     : 3;	// 29:31
} L9B0_CTR04_REG_CLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0024 ctr05_reg_swrs_of_de ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :13,	//  0:12 reserved
	swrst_def_vcr_de_dp             : 1,	//    13
	swrst_def_mif_de_dp             : 1,	//    14
	swrst_def_mif_de                : 1,	//    15
	swrst_def_mif_dea               : 1,	//    16
	swrst_dee_de_dp                 : 1,	//    17
	swrst_dee_de                    : 1,	//    18
	swrst_ded_de_dp                 : 1,	//    19
	swrst_ded_de                    : 1,	//    20
	swrst_ded_dea_dp                : 1,	//    21
	swrst_ded_dea                   : 1,	//    22
	swrst_dec_ssc_de                : 1,	//    23
	swrst_dec_pe0_de                : 1,	//    24
	swrst_dec_mif_de                : 1,	//    25
	swrst_dec_mif_dea               : 1,	//    26
	swrst_deb_de_dp                 : 1,	//    27
	swrst_deb_de                    : 1,	//    28
	swrst_deb_dea                   : 1,	//    29
	swrst_dea_de                    : 1,	//    30
	swrst_dea_dea                   : 1;	//    31
} L9B0_CTR05_REG_SWRS_OF_DE;

/*-----------------------------------------------------------------------------
	0x0028 ctr06_reg_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	swrst_te_te1                    : 1,	//     2
	swrst_noc2                      : 1,	//     3
	swrst_noc1                      : 1,	//     4
	swrst_noc0                      : 1,	//     5
	swrst_de_vd                     : 1,	//     6
	swrst_te_vd                     : 1,	//     7
	swrst_vda                       : 1,	//     8
	swrst_vd1                       : 1,	//     9
	swrst_vd2                       : 1,	//    10
	swrst_vd                        : 1,	//    11
	swrst_vd_apb                    : 1,	//    12
	swrst_icoda                     : 1,	//    13
	swrst_icod                      : 1,	//    14
	swrst_tea                       : 1,	//    15
	swrst_teah                      : 1,	//    16
	swrst_te_te                     : 1,	//    17
	swrst_te_ve                     : 1,	//    18
	swrst_vea                       : 1,	//    19
	swrst_ve                        : 1,	//    20
	swrst_icod_p                    : 1,	//    21
	swrst_te_apb                    : 1,	//    22
	swrst_ve_apb                    : 1,	//    23
	swrst_gpu1                      : 1,	//    24
	swrst_gpu0                      : 1,	//    25
	swrst_gpu_p                     : 1,	//    26
	swrst_gfx                       : 1,	//    27
	swrst_gfx_a                     : 1,	//    28
	swrst_gfx_p                     : 1,	//    29
	swrst_cvda                      : 1,	//    30
	swrst_de_apb                    : 1;	//    31
} L9B0_CTR06_REG_SWRST;

/*-----------------------------------------------------------------------------
	0x002c ctr07_reg_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :24,	//  0:23 reserved
	swrst_f2_a                      : 1,	//    24
	swrst_f1_a                      : 1,	//    25
	swrst_f0_a                      : 1,	//    26
	swrst_slt_p                     : 1,	//    27
	                                : 1,	//    28 reserved
	swrst_at2_p                     : 1,	//    29
	swrst_at1_p                     : 1,	//    30
	swrst_at0_p                     : 1;	//    31
} L9B0_CTR07_REG_SWRST;

/*-----------------------------------------------------------------------------
	0x0030 ctr08_reg_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 8,	//  0: 7 reserved
	def_vcr_de_clk_sel              : 3,	//  8:10
	def_mif_de_clk_sel              : 3,	// 11:13
	dee_de_clk_sel                  : 3,	// 14:16
	ded_de_clk_sel                  : 3,	// 17:19
	trace_sel                       : 3,	// 20:22
	dbgvsoc_sel                     : 3,	// 23:25
	atvsoc_sel                      : 3,	// 26:28
	atclk_sel                       : 3;	// 29:31
} L9B0_CTR08_REG_CLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0034 ctr09_reg_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	isol_en_sel                     : 3,	//  1: 3
	                                : 1,	//     4 reserved
	pclken_sel                      : 3,	//  5: 7
	                                : 1,	//     8 reserved
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
} L9B0_CTR09_REG_SWRST;

/*-----------------------------------------------------------------------------
	0x0038 ctr10_reg_ahbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	ahb7_sel                        : 3,	//  1: 3
	                                : 1,	//     4 reserved
	ahb6_sel                        : 3,	//  5: 7
	                                : 1,	//     8 reserved
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
} L9B0_CTR10_REG_AHBCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x003c ctr11_reg_ahbclk_div_sel ''
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
	                                : 1,	//    16 reserved
	ahb18_sel                       : 3,	// 17:19
	                                : 1,	//    20 reserved
	ahb17_sel                       : 3,	// 21:23
	                                : 1,	//    24 reserved
	ahb9_sel                        : 3,	// 25:27
	                                : 1,	//    28 reserved
	ahb8_sel                        : 3;	// 29:31
} L9B0_CTR11_REG_AHBCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0040 ctr12_reg_rmiiclk_off ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	dec_ssc_de_clk_sel              : 3,	//  2: 4
	dec_pe0_de_clk_sel              : 3,	//  5: 7
	dec_mif_de_clk_sel              : 3,	//  8:10
	deb_de_clk_sel                  : 3,	// 11:13
	dea_de_clk_sel                  : 3,	// 14:16
	def_mif_dea_clk_sel             : 3,	// 17:19
	ded_dea_clk_sel                 : 3,	// 20:22
	dec_mif_dea_clk_sel             : 3,	// 23:25
	deb_dea_clk_sel                 : 3,	// 26:28
	dea_dea_clk_sel                 : 3;	// 29:31
} L9B0_CTR12_REG_RMIICLK_OFF;

/*-----------------------------------------------------------------------------
	0x0044 ctr13_reg_ahbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 5,	//  0: 4 reserved
	ahb30_sel                       : 3,	//  5: 7
	                                : 1,	//     8 reserved
	ahb29_sel                       : 3,	//  9:11
	                                :17,	// 12:28 reserved
	ahb24_sel                       : 3;	// 29:31
} L9B0_CTR13_REG_AHBCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0048 ctr14_reg_ahb_swrst ''
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
} L9B0_CTR14_REG_AHB_SWRST;

/*-----------------------------------------------------------------------------
	0x004c ctr15_reg_ahb_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :26,	//  0:25 reserved
	swrst_ahb37                     : 1,	//    26
	swrst_ahb36                     : 1,	//    27
	swrst_ahb35                     : 1,	//    28
	swrst_ahb34                     : 1,	//    29
	swrst_ahb33                     : 1,	//    30
	swrst_ahb32                     : 1;	//    31
} L9B0_CTR15_REG_AHB_SWRST;

/*-----------------------------------------------------------------------------
	0x0050 ctr16_reg_busclk_div_sel ''
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
} L9B0_CTR16_REG_BUSCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0054 ctr17_reg_busclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :21,	//  0:20 reserved
	bus10_sel                       : 3,	// 21:23
	                                : 1,	//    24 reserved
	bus9_sel                        : 3,	// 25:27
	                                : 1,	//    28 reserved
	bus8_sel                        : 3;	// 29:31
} L9B0_CTR17_REG_BUSCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0058 ctr18_reb_cpubus_swrst ''
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
} L9B0_CTR18_REB_CPUBUS_SWRST;

/*-----------------------------------------------------------------------------
	0x005c ctr19_reg_apbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	apb0_7_sel                      : 3,	//  1: 3
	                                : 1,	//     4 reserved
	apb0_6_sel                      : 3,	//  5: 7
	                                : 1,	//     8 reserved
	apb0_5_sel                      : 3,	//  9:11
	                                : 1,	//    12 reserved
	apb0_4_sel                      : 3,	// 13:15
	                                : 1,	//    16 reserved
	apb0_3_sel                      : 3,	// 17:19
	                                : 1,	//    20 reserved
	apb0_2_sel                      : 3,	// 21:23
	                                : 1,	//    24 reserved
	apb0_1_sel                      : 3,	// 25:27
	                                : 1,	//    28 reserved
	apb0_0_sel                      : 3;	// 29:31
} L9B0_CTR19_REG_APBCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0060 ctr20_reg_apbclk_div_sel ''
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
} L9B0_CTR20_REG_APBCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0064 ctr21_reg_apbclk_div_sel ''
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
} L9B0_CTR21_REG_APBCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x0068 ctr22_reg_apbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :17,	//  0:16 reserved
	apb1_19_sel                     : 3,	// 17:19
	                                : 1,	//    20 reserved
	apb1_18_sel                     : 3,	// 21:23
	                                : 1,	//    24 reserved
	apb1_17_sel                     : 3,	// 25:27
	                                : 1,	//    28 reserved
	apb1_16_sel                     : 3;	// 29:31
} L9B0_CTR22_REG_APBCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x006c ctr23_reserved ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0070 ctr24_reg_apb_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	swrst_apb1_19                   : 1,	//     4
	swrst_apb1_18                   : 1,	//     5
	swrst_apb1_17                   : 1,	//     6
	swrst_apb1_16                   : 1,	//     7
	swrst_apb1_15                   : 1,	//     8
	swrst_apb1_14                   : 1,	//     9
	swrst_apb1_13                   : 1,	//    10
	swrst_apb1_12                   : 1,	//    11
	swrst_apb1_11                   : 1,	//    12
	swrst_apb1_10                   : 1,	//    13
	swrst_apb1_9                    : 1,	//    14
	swrst_apb1_8                    : 1,	//    15
	swrst_apb1_7                    : 1,	//    16
	swrst_apb1_6                    : 1,	//    17
	swrst_apb1_5                    : 1,	//    18
	swrst_apb1_4                    : 1,	//    19
	swrst_apb1_3                    : 1,	//    20
	swrst_apb1_2                    : 1,	//    21
	swrst_apb1_1                    : 1,	//    22
	swrst_apb1_0                    : 1,	//    23
	swrst_apb0_7                    : 1,	//    24
	swrst_apb0_6                    : 1,	//    25
	swrst_apb0_5                    : 1,	//    26
	swrst_apb0_4                    : 1,	//    27
	swrst_apb0_3                    : 1,	//    28
	swrst_apb0_2                    : 1,	//    29
	swrst_apb0_1                    : 1,	//    30
	swrst_apb0_0                    : 1;	//    31
} L9B0_CTR24_REG_APB_SWRST;

/*-----------------------------------------------------------------------------
	0x0074 ctr25_reg_de_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	swrst_def_disp                  : 1,	//    19
	swrst_ded_disp                  : 1,	//    20
	swrst_decssc_disp               : 1,	//    21
	swrst_decmif_disp               : 1,	//    22
	swrst_deb_disp                  : 1;	//    23
} L9B0_CTR25_REG_DE_SWRST;

/*-----------------------------------------------------------------------------
	0x0078 ctr26_reg_extclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 4,	//  0: 3 reserved
	lvds_clk_sel                    : 1,	//     4
	dco_sel                         : 1,	//     5
	ssclk_sel                       : 2,	//  6: 7
	                                : 2,	//  8: 9 reserved
	msclk_sel                       : 2,	// 10:11
	extb_sel                        : 2,	// 12:13
	exta_sel                        : 2,	// 14:15
	                                : 6,	// 16:21 reserved
	reg_ch3pix_clk_sel              : 1,	//    22
	reg_ch3pix_clk_inv              : 1,	//    23
	cvbs27_sel_inv                  : 1,	//    24
	cvbs54_sel                      : 2,	// 25:26
	reg_se_clk_inv                  : 1,	//    27
	reg_cve27_clk_inv               : 1,	//    28
	reg_cve54_clk_inv               : 1,	//    29
	reg_cvd27_clk_inv               : 1,	//    30
	reg_cvd54_clk_inv               : 1;	//    31
} L9B0_CTR26_REG_EXTCLK_DIV_SEL;

/*-----------------------------------------------------------------------------
	0x007c ctr27_reg_extclk_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ext_en_i                        : 1,	//     0
	f27_18_sel                      : 1,	//     1
	sc_ext_sel                      : 3,	//  2: 4
	                                : 1,	//     5 reserved
	reg_slt_disp                    : 1,	//     6
	reg_s_te                        : 1,	//     7
	reg_sys                         : 1,	//     8
	reg_disp_h                      : 1,	//     9
	reg_disp_def_p                  : 1,	//    10
	reg_disp_def                    : 1,	//    11
	reg_disp_ded_p                  : 1,	//    12
	reg_disp_ded                    : 1,	//    13
	reg_disp_ssc                    : 1,	//    14
	reg_disp_mif                    : 1,	//    15
	reg_disp_dp                     : 1,	//    16
	reg_disp_de                     : 1,	//    17
	reg_swrst_se                    : 1,	//    18
	reg_swrst_chb                   : 1,	//    19
	reg_swrst_cha                   : 1,	//    20
	reg_swrst_extb                  : 1,	//    21
	reg_swrst_exta                  : 1,	//    22
	reg_swrst_ch3                   : 1,	//    23
	reg_swrst_scart                 : 1,	//    24
	reg_swrst_vbi                   : 1,	//    25
	reg_swrst_cvbs27                : 1,	//    26
	reg_swrst_cvbs54                : 1,	//    27
	reg_swrst_cvd27                 : 1,	//    28
	reg_swrst_cvd54                 : 1,	//    29
	reg_swrst_cve27                 : 1,	//    30
	reg_swrst_cve54                 : 1;	//    31
} L9B0_CTR27_REG_EXTCLK_SWRST;

/*-----------------------------------------------------------------------------
	0x0080 ctr28_reg_cpu_swrst ''
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
} L9B0_CTR28_REG_CPU_SWRST;

/*-----------------------------------------------------------------------------
	0x0084 ctr29_reg_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ext_intr_pol_sel0               : 1,	//     0
	ext_intr_pol_sel1               : 1,	//     1
	ext_intr_pol_sel2               : 1,	//     2
	ext_intr_pol_sel3               : 1,	//     3
	cam_vcc_pol                     : 1,	//     4
	pll_sel_core                    : 1,	//     5
	pll_sel_m2                      : 1,	//     6
	pll_sel_m1                      : 1,	//     7
	jtag1_sel                       : 3,	//  8:10
	aud_jtag_no                     : 1,	//    11
	jtag0_sel                       : 1,	//    12
	                                : 1,	//    13 reserved
	gpu_high_speed                  : 1,	//    14
	reg_m2_q_reset                  : 1,	//    15
	reg_m2_h_reset                  : 1,	//    16
	reg_m2_reset                    : 1,	//    17
	a_stdby_m2                      : 2,	// 18:19
	                                : 3,	// 20:22 reserved
	reg_m1_h_reset                  : 1,	//    23
	reg_m1_reset                    : 1,	//    24
	a_stdby_m1                      : 2,	// 25:26
	reg_m0_q_reset                  : 1,	//    27
	reg_m0_h_reset                  : 1,	//    28
	reg_m0_reset                    : 1,	//    29
	a_stdby_m0                      : 2;	// 30:31
} L9B0_CTR29_REG_SWRST;

/*-----------------------------------------------------------------------------
	0x0088 ctr30_reg_block_moniter ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sd_int_en                       : 1,	//     0
	                                :14,	//  1:14 reserved
	trace8bit_en                    : 1,	//    15
	                                : 3,	// 16:18 reserved
	cpu_mon_31_en                   : 1,	//    19
	traceclk_en                     : 1,	//    20
	trace_en                        : 1,	//    21
	trace_en1                       : 1,	//    22
	trace_en2                       : 1,	//    23
	trace_en3                       : 1,	//    24
	usb_mon_en                      : 1,	//    25
	usb_mon_en1                     : 1,	//    26
	gfx_mon_en                      : 1,	//    27
	aad_mon_en                      : 1,	//    28
	aud_mon_en                      : 1,	//    29
	cpu_mon_en                      : 1,	//    30
	de_mon_en                       : 1;	//    31
} L9B0_CTR30_REG_BLOCK_MONITER;

/*-----------------------------------------------------------------------------
	0x008c ctr31_reg_gpio_pullup_ctrl ''
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
} L9B0_CTR31_REG_GPIO_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x0090 ctr32_reg_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :23,	//  0:22 reserved
	stpio_en_ctrl                   : 1,	//    23
	gp4_0_mux_en                    : 1,	//    24
	gp4_1_mux_en                    : 1,	//    25
	gp4_2_mux_en                    : 1,	//    26
	gp4_3_mux_en                    : 1,	//    27
	gp4_4_mux_en                    : 1,	//    28
	gp4_5_mux_en                    : 1,	//    29
	gp4_6_mux_en                    : 1,	//    30
	gp4_7_mux_en                    : 1;	//    31
} L9B0_CTR32_REG_GPIO_MUX_ENABLE;

/*-----------------------------------------------------------------------------
	0x0094 ctr33_reserved ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0098 ctr34_reg_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gp6_3_mux_en                    : 1,	//     0
	                                : 6,	//  1: 6 reserved
	gp6_4_mux_en                    : 1,	//     7
	                                : 7,	//  8:14 reserved
	gp6_5_mux_en                    : 1,	//    15
	                                : 7,	// 16:22 reserved
	gp6_6_mux_en                    : 1,	//    23
	                                : 7,	// 24:30 reserved
	gp6_7_mux_en                    : 1;	//    31
} L9B0_CTR34_REG_GPIO_MUX_ENABLE;

/*-----------------------------------------------------------------------------
	0x009c ctr35_reg_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :11,	//  0:10 reserved
	gp7_5_mux_en                    : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp7_6_mux_en                    : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp7_7_mux_en                    : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp6_0_mux_en                    : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp6_1_mux_en                    : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp6_2_mux_en                    : 1;	//    31
} L9B0_CTR35_REG_GPIO_MUX_ENABLE;

/*-----------------------------------------------------------------------------
	0x00a0 ctr36_reg_uart_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :10,	//  0: 9 reserved
	rx_sel_de                       : 2,	// 10:11
	                                : 6,	// 12:17 reserved
	rx_sel_sp                       : 2,	// 18:19
	                                : 2,	// 20:21 reserved
	uart2_sel                       : 2,	// 22:23
	                                : 2,	// 24:25 reserved
	uart1_sel                       : 2,	// 26:27
	                                : 2,	// 28:29 reserved
	uart0_sel                       : 2;	// 30:31
} L9B0_CTR36_REG_UART_SEL;

/*-----------------------------------------------------------------------------
	0x00a4 ctr37_reg_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	gp8_5_mux_en                    : 1,	//     3
	                                : 3,	//  4: 6 reserved
	gp8_6_mux_en                    : 1,	//     7
	                                : 3,	//  8:10 reserved
	gp8_7_mux_en                    : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp7_0_mux_en                    : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp7_1_mux_en                    : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp7_2_mux_en                    : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp7_3_mux_en                    : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp7_4_mux_en                    : 1;	//    31
} L9B0_CTR37_REG_GPIO_MUX_ENABLE;

/*-----------------------------------------------------------------------------
	0x00a8 ctr38_reg_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	gp8_1_mux_en                    : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp8_2_mux_en                    : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp8_3_mux_en                    : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp8_4_mux_en                    : 1;	//    31
} L9B0_CTR38_REG_GPIO_MUX_ENABLE;

/*-----------------------------------------------------------------------------
	0x00ac ctr39_reg_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :16,	//  0:15 reserved
	sc_clk_sel                      : 1,	//    16
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
	gp9_5_mux_en                    : 1,	//    28
	gp9_6_mux_en                    : 1,	//    29
	gp9_7_mux_en                    : 1,	//    30
	gp8_0_mux_en                    : 1;	//    31
} L9B0_CTR39_REG_GPIO_MUX_ENABLE;

/*-----------------------------------------------------------------------------
	0x00b0 ctr40_reg_gpu_dft_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :15,	//  0:14 reserved
	gpu_pwrdnbypass                 : 1,	//    15
	                                : 7,	// 16:22 reserved
	gpu_testin                      : 1,	//    23
	                                : 7,	// 24:30 reserved
	gpu_testin1                     : 1;	//    31
} L9B0_CTR40_REG_GPU_DFT_CTRL;

/*-----------------------------------------------------------------------------
	0x00b4 ctr41_reg_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	pull_up_control                 : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control1                : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control2                : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control3                : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control4                : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control5                : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control6                : 1;	//    31
} L9B0_CTR41_REG_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x00b8 ctr42_reg_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pull_up_control7                : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pull_up_control8                : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control9                : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control10               : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control11               : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control12               : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control13               : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control14               : 1;	//    31
} L9B0_CTR42_REG_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x00bc ctr43_reg_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :15,	//  0:14 reserved
	pull_up_control15               : 1,	//    15
	                                : 7,	// 16:22 reserved
	pull_up_control16               : 1,	//    23
	                                : 7,	// 24:30 reserved
	pull_up_control17               : 1;	//    31
} L9B0_CTR43_REG_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x00c0 ctr44_reg_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :15,	//  0:14 reserved
	pull_up_control18               : 1,	//    15
	                                : 7,	// 16:22 reserved
	pull_up_control19               : 1,	//    23
	                                : 7,	// 24:30 reserved
	pull_up_control20               : 1;	//    31
} L9B0_CTR44_REG_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x00c4 ctr45_reg_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :11,	//  0:10 reserved
	pull_up_control21               : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control22               : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control23               : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control24               : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control25               : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control26               : 1;	//    31
} L9B0_CTR45_REG_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x00c8 ctr46_reg_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :23,	//  0:22 reserved
	pull_up_control27               : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control28               : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control29               : 1;	//    31
} L9B0_CTR46_REG_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x00cc ctr47_reg_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	pull_up_control30               : 1,	//     3
	                                : 3,	//  4: 6 reserved
	pull_up_control31               : 1,	//     7
	                                : 3,	//  8:10 reserved
	pull_up_control32               : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control33               : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control34               : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control35               : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control36               : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control37               : 1;	//    31
} L9B0_CTR47_REG_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x00d0 ctr48_reg_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :11,	//  0:10 reserved
	pull_up_control38               : 1,	//    11
	                                : 3,	// 12:14 reserved
	pull_up_control39               : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control40               : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control41               : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control42               : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control43               : 1;	//    31
} L9B0_CTR48_REG_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x00d4 ctr49_reg_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 7,	//  0: 6 reserved
	pull_up_control44               : 1,	//     7
	                                : 7,	//  8:14 reserved
	pull_up_control45               : 1,	//    15
	                                : 3,	// 16:18 reserved
	pull_up_control46               : 1,	//    19
	                                : 3,	// 20:22 reserved
	pull_up_control47               : 1,	//    23
	                                : 3,	// 24:26 reserved
	pull_up_control48               : 1,	//    27
	                                : 3,	// 28:30 reserved
	pull_up_control49               : 1;	//    31
} L9B0_CTR49_REG_PULLUP_CTRL;

/*-----------------------------------------------------------------------------
	0x00d8 ctr50_reg_tpio_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :22,	//  0:21 reserved
	tpio_sel_ctrl                   : 1,	//    22
	tpo_sel_ctrl                    : 1,	//    23
									: 2,	// 24:25 reserved
	tp1_in_sel						: 2,	// 26:27
	tp0_in_sel 						: 2,	// 28:29
	                                : 1,	//    30 reserved
	swrst                           : 1;	//    31
} L9B0_CTR50_REG_TPIO_SEL;

/*-----------------------------------------------------------------------------
	0x00dc ctr51_reg_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :19,	//  0:18 reserved
	gp11_6_mux_en                   : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp11_7_mux_en                   : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp10_0_mux_en                   : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp10_1_mux_en                   : 1;	//    31
} L9B0_CTR51_REG_GPIO_MUX_ENABLE;

/*-----------------------------------------------------------------------------
	0x00e0 ctr52_reg_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	en_aud_dacrlrch                 : 1,	//     0
	                                :10,	//  1:10 reserved
	gp11_0_mux_en                   : 1,	//    11
	                                : 3,	// 12:14 reserved
	gp11_1_mux_en                   : 1,	//    15
	                                : 3,	// 16:18 reserved
	gp11_2_mux_en                   : 1,	//    19
	                                : 3,	// 20:22 reserved
	gp11_3_mux_en                   : 1,	//    23
	                                : 3,	// 24:26 reserved
	gp11_4_mux_en                   : 1,	//    27
	                                : 3,	// 28:30 reserved
	gp11_5_mux_en                   : 1;	//    31
} L9B0_CTR52_REG_GPIO_MUX_ENABLE;

/*-----------------------------------------------------------------------------
	0x00e4 ctr53_reg_gpio_mux_enable ''
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
} L9B0_CTR53_REG_GPIO_MUX_ENABLE;

/*-----------------------------------------------------------------------------
	0x00e8 ctr54_reg_bt_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dftclk_ctrl                     : 1,	//     0
	                                :14,	//  1:14 reserved
	pd                              : 1,	//    15
	                                : 4,	// 16:19 reserved
	bt_testclk0_ctrl                : 1,	//    20
	bt_txpreemphasistune0_ctrl      : 2,	// 21:22
	bt_txrisetune0_ctrl             : 1,	//    23
	bt_commononn_ctrl               : 1,	//    24
	bt_refclksel_ctrl               : 2,	// 25:26
	bt_refclkdiv_ctrl               : 2,	// 27:28
	bt_testen_en_ctrl               : 1,	//    29
	bt_vatestenb_en_ctrl            : 1,	//    30
	bt_siddq_en_ctrl                : 1;	//    31
} L9B0_CTR54_REG_BT_USB_CTRL;

/*-----------------------------------------------------------------------------
	0x00ec ctr55_rgg_sdec_dco_fcw_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sd_dco_fcw_ctrl                 :22;	//  0:21
} L9B0_CTR55_RGG_SDEC_DCO_FCW_CTRL;

/*-----------------------------------------------------------------------------
	0x00f0 ctr56_reg_bt_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :28,	//  0:27 reserved
	testaddr                        : 4;	// 28:31
} L9B0_CTR56_REG_BT_USB_CTRL;

/*-----------------------------------------------------------------------------
	0x00f4 ctr57_reg_bt_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	testdatain                      : 8,	//  3:10
	testdataoutsel                  : 1,	//    11
	txvreftune                      : 4,	// 12:15
	txfslstune                      : 4,	// 16:19
	vregtune                        : 1,	//    20
	txhsxvtune                      : 2,	// 21:22
	sqrxtune                        : 3,	// 23:25
	compdistune                     : 3,	// 26:28
	loopbackenb                     : 1,	//    29
	otgdisable                      : 1,	//    30
	portreset                       : 1;	//    31
} L9B0_CTR57_REG_BT_USB_CTRL;

/*-----------------------------------------------------------------------------
	0x00f8 ctr58_reg_main_0_1_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :16,	//  0:15 reserved
	testclk1_ctrl                   : 1,	//    16
	txpreemphasistune1_ctrl         : 2,	// 17:18
	txrisetune1_ctrl                : 1,	//    19
	testclk0_ctrl                   : 1,	//    20
	txpreemphasistune0_ctrl         : 2,	// 21:22
	txrisetune0_ctrl                : 1,	//    23
	commononn_ctrl                  : 1,	//    24
	refclksel_ctrl                  : 2,	// 25:26
	refclkdiv_ctrl                  : 2,	// 27:28
	testen_en_ctrl                  : 1,	//    29
	vatestenb_en_ctrl               : 1,	//    30
	siddq_en_ctrl                   : 1;	//    31
} L9B0_CTR58_REG_MAIN_0_1_USB_CTRL;

/*-----------------------------------------------------------------------------
	0x00fc ctr59_reg_bt_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :28,	//  0:27 reserved
	testaddr1                       : 4;	// 28:31
} L9B0_CTR59_REG_BT_USB_CTRL;

/*-----------------------------------------------------------------------------
	0x0100 ctr60_reg_main_0_1_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	testdatain1                     : 8,	//  0: 7
	testdataoutsel1                 : 1,	//     8
	txvreftune1                     : 4,	//  9:12
	txfslstune1                     : 4,	// 13:16
	vregtune1                       : 1,	//    17
	txhsxvtune1                     : 2,	// 18:19
	sqrxtune1                       : 3,	// 20:22
	compdistune1                    : 3,	// 23:25
	loopbackenb1                    : 1,	//    26
	portreset1                      : 1,	//    27
	testaddr0                       : 4;	// 28:31
} L9B0_CTR60_REG_MAIN_0_1_USB_CTRL;

/*-----------------------------------------------------------------------------
	0x0104 ctr61_reg_main_0_1_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	testdatain0                     : 8,	//  3:10
	testdataoutsel0                 : 1,	//    11
	txvreftune0                     : 4,	// 12:15
	txfslstune0                     : 4,	// 16:19
	vregtune0                       : 1,	//    20
	txhsxvtune0                     : 2,	// 21:22
	sqrxtune0                       : 3,	// 23:25
	compdistune0                    : 3,	// 26:28
	loopbackenb0                    : 1,	//    29
	otgdisable0                     : 1,	//    30
	portreset0                      : 1;	//    31
} L9B0_CTR61_REG_MAIN_0_1_USB_CTRL;

/*-----------------------------------------------------------------------------
	0x0108 ctr62_reg_display_pll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :28,	//  0:27 reserved
	disp_pre_fd_ctrl                : 2,	// 28:29
	disp_od_fout_ctrl               : 2;	// 30:31
} L9B0_CTR62_REG_DISPLAY_PLL_CTRL;

/*-----------------------------------------------------------------------------
	0x010c ctr63_reg_display_pll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	disp_udex_ctrl                  : 5,	//  0: 4
	disp_msex_ctrl                  : 9,	//  5:13
	disp_nsc_ctrl                   : 4,	// 14:17
	disp_npc_ctrl                   : 6,	// 18:23
	disp_m_ctrl                     : 3,	// 24:26
	disp_cih_ctrl                   : 3,	// 27:29
	disp_lf_ctrl                    : 1,	//    30
	disp_pdb_ctrl                   : 1;	//    31
} L9B0_CTR63_REG_DISPLAY_PLL_CTRL;

/*-----------------------------------------------------------------------------
	0x0110 usb0_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	usb0_endian_ahbms_ehci_bufacc   : 1,	//     1
	usb0_endian_ahbms_ohci_bufacc   : 1,	//     2
	usb0_app_start_clk_i            : 1,	//     3
	usb0_ohci_susp_lgcy_i           : 1,	//     4
	usb0_ss_simulation_mode_i       : 1,	//     5
	usb0_ss_word_if_i               : 1,	//     6
	usb0_ss_utmi_backward_enb_i     : 1,	//     7
	usb0_ss_fladj_val_host_i        : 6,	//  8:13
	                                : 2,	// 14:15 reserved
	usb0_ss_fladj_val_i             : 6,	// 16:21
	usb0_ss_resume_utmi_pls_dis_i   : 1,	//    22
	usb0_ss_autoppd_on_overcur_en_i : 1,	//    23
	usb0_ss_ena_incr16_i            : 1,	//    24
	usb0_ss_ena_incr8_i             : 1,	//    25
	usb0_ss_ena_incr4_i             : 1,	//    26
	usb0_ss_ena_incrx_align_i       : 1,	//    27
	usb0_app_prt_ovrcur_i           : 1,	//    28
	usb0_endian_ahbsl               : 1,	//    29
	usb0_endian_ahbms_ehci          : 1,	//    30
	usb0_endian_ahbms_ohci          : 1;	//    31
} L9B0_USB0_CTRL0;

/*-----------------------------------------------------------------------------
	0x0114 usb0_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	usb0_ohci_0_globalsuspend_o     : 1,	//     0
	usb0_ohci_0_drwe_o              : 1,	//     1
	usb0_ohci_0_rwe_o               : 1,	//     2
	usb0_ohci_0_rmtwkp_o            : 1,	//     3
	usb0_ohci_0_smi_o_n             : 1,	//     4
	usb0_ohci_0_sof_o_n             : 1,	//     5
	usb0_ohci_0_bufacc_o            : 1,	//     6
	usb0_ehci_prt_pwr_o             : 1,	//     7
	usb0_ehci_xfer_cnt_o            :11,	//  8:18
	                                : 1,	//    19 reserved
	usb0_ehci_xfer_prdc_o           : 1,	//    20
	usb0_ehci_bufacc_o              : 1,	//    21
	usb0_ehci_pme_status_o          : 1,	//    22
	usb0_ehci_power_state_ack_o     : 1,	//    23
	usb0_ohci_0_ccs_o               : 1;	//    24
} L9B0_USB0_STATUS0;

/*-----------------------------------------------------------------------------
	0x0118 usb0_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	usb0_ehci_usbsts_o              : 6,	//  0: 5
	usb0_ohci_1_drwe_o              : 1,	//     1
	usb0_ohci_1_rwe_o               : 1,	//     2
	usb0_ohci_1_rmtwkp_o            : 1,	//     3
	usb0_ohci_1_smi_o_n             : 1,	//     4
	usb0_ohci_1_sof_o_n             : 1,	//     5
	usb0_ohci_1_bufacc_o            : 1,	//     6
	usb0_ehci_prt_pwr_o_p1          : 1,	//     7
	usb0_ehci_lpsmc_state_o         : 4,	//  8:11
	                                : 4,	// 12:15 reserved
	usb0_ss_fladj_val_i_p1          : 6,	// 16:21
	                                : 2,	// 22:23 reserved
	usb0_ohci_1_ccs_o               : 1,	//    24
	                                : 3,	// 25:27 reserved
	usb0_app_prt_ovrcur_i_p1        : 1,	//    28
	                                : 2,	// 29:30 reserved
	usb0_endian_ahbms_ohci_1        : 1;	//    31
} L9B0_USB0_STATUS1;

/*-----------------------------------------------------------------------------
	0x0128 io_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ci_en                           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	monitor_sig_sel                 : 4,	//  4: 7
	                                : 8,	//  8:15 reserved
	ahb_port_sel                    :14;	// 16:29
} L9B0_IO_CTRL;

/*-----------------------------------------------------------------------------
	0x0138 apb_time_out_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tout_en                         : 1,	//     0
	tout_intr_clr                   : 1,	//     1
	                                :14,	//  2:15 reserved
	tout_cnt                        :16;	// 16:31
} L9B0_APB_TIME_OUT_CTRL;

/*-----------------------------------------------------------------------------
	0x013c apb_time_out_src_31_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	psel0_tout                      : 1,	//     0
	psel1_tout                      : 1,	//     1
	psel2_tout                      : 1,	//     2
	psel3_tout                      : 1,	//     3
	psel4_tout                      : 1,	//     4
	psel5_tout                      : 1,	//     5
	                                : 2,	//  6: 7 reserved
	psel8_tout                      : 1,	//     8
	psel9_tout                      : 1,	//     9
	psel10_tout                     : 1,	//    10
	psel11_tout                     : 1,	//    11
	psel12_tout                     : 1,	//    12
	psel13_tout                     : 1,	//    13
	psel14_tout                     : 1,	//    14
	psel15_tout                     : 1,	//    15
	psel16_tout                     : 1,	//    16
	psel17_tout                     : 1,	//    17
	psel18_tout                     : 1,	//    18
	psel19_tout                     : 1,	//    19
	psel20_tout                     : 1,	//    20
	psel21_tout                     : 1,	//    21
	psel22_tout                     : 1,	//    22
	psel23_tout                     : 1,	//    23
	psel24_tout                     : 1,	//    24
	psel25_tout                     : 1,	//    25
	psel26_tout                     : 1,	//    26
	psel27_tout                     : 1,	//    27
	psel28_tout                     : 1,	//    28
	psel29_tout                     : 1,	//    29
	psel30_tout                     : 1;	//    30
} L9B0_APB_TIME_OUT_SRC_31_0;

/*-----------------------------------------------------------------------------
	0x0140 apb_time_out_src_41_32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	psel32_tout                     : 1,	//     0
	psel33_tout                     : 1,	//     1
	psel34_tout                     : 1,	//     2
	psel35_tout                     : 1,	//     3
	psel36_tout                     : 1,	//     4
	psel37_tout                     : 1;	//     5
} L9B0_APB_TIME_OUT_SRC_41_32;

/*-----------------------------------------------------------------------------
	0x0148 stcc_ref_time_stamp_32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc_ref_time_stamp_high        : 1;	//     0
} L9B0_STCC_REF_TIME_STAMP_32;

/*-----------------------------------------------------------------------------
	0x014c stcc_ref_time_stamp_31_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc_ref_time_stamp_low         ;   	// 31: 0
} L9B0_STCC_REF_TIME_STAMP_31_0;

/*-----------------------------------------------------------------------------
	0x0150 stcc_real_time_stamp_32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc_real_time_stamp_high       : 1;	//     0
} L9B0_STCC_REAL_TIME_STAMP_32;

/*-----------------------------------------------------------------------------
	0x0154 stcc_real_time_stamp_31_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc_real_time_stamp_low        ;   	// 31: 0
} L9B0_STCC_REAL_TIME_STAMP_31_0;

/*-----------------------------------------------------------------------------
	0x0158 flush_done_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sc_flush_done                   : 1,	//     0
	                                : 1,	//     1 reserved
	vdec_flush_done                 : 1,	//     2
	icod_flush_done                 : 1,	//     3
	venc_flush_done                 : 1,	//     4
	te_flush_done                   : 1,	//     5
	gpu_flush_done                  : 1,	//     6
	gfx_flush_done                  : 1,	//     7
	de_gmau_flush_done_dp           : 1,	//     8
	de_gmau_flush_done              : 1,	//     9
	aud_dsp1_flush_done             : 1,	//    10
	aud_dsp0_flush_done             : 1;	//    11
} L9B0_FLUSH_DONE_STATUS;

/*-----------------------------------------------------------------------------
	0x015c flush_req ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sc_flush_en                     : 1,	//     0
	                                : 1,	//     1 reserved
	vdec_flush_en                   : 1,	//     2
	icod_flush_en                   : 1,	//     3
	venc_flush_en                   : 1,	//     4
	te_flush_en                     : 1,	//     5
	gpu_flush_en                    : 1,	//     6
	gfx_flush_en                    : 1,	//     7
	de_gmau_flush_en_dp             : 1,	//     8
	de_gmau_flush_en                : 1,	//     9
	aud_dsp1_flush_en               : 1,	//    10
	aud_dsp0_flush_en               : 1;	//    11
} L9B0_FLUSH_REQ;

/*-----------------------------------------------------------------------------
	0x0164 bandwidth_limiter_axi0_dly_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bw_platform0_raxi_dly_cnt       :16,	//  0:15
	bw_platform0_waxi_dly_cnt       :16;	// 16:31
} L9B0_BANDWIDTH_LIMITER_AXI0_DLY_CNT;

/*-----------------------------------------------------------------------------
	0x0168 bandwidth_limiter_axi1_dly_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bw_platform1_raxi_dly_cnt       :16,	//  0:15
	bw_platform1_waxi_dly_cnt       :16;	// 16:31
} L9B0_BANDWIDTH_LIMITER_AXI1_DLY_CNT;

/*-----------------------------------------------------------------------------
	0x016c bandwidth_limiter_cpu_axi0_dly_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bw_cpu_raxi0_dly_cnt            :16,	//  0:15
	bw_cpu_waxi0_dly_cnt            :16;	// 16:31
} L9B0_BANDWIDTH_LIMITER_CPU_AXI0_DLY_CNT;

/*-----------------------------------------------------------------------------
	0x0170 bandwidth_limiter_cpu_axi1_dly_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bw_cpu_raxi1_dly_cnt            :16,	//  0:15
	bw_cpu_waxi1_dly_cnt            :16;	// 16:31
} L9B0_BANDWIDTH_LIMITER_CPU_AXI1_DLY_CNT;

/*-----------------------------------------------------------------------------
	0x0174 gpio_interrupt_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	gpio0_intr                      : 1,	//     0
	gpio1_intr                      : 1,	//     1
	gpio2_intr                      : 1,	//     2
	gpio3_intr                      : 1,	//     3
	gpio4_intr                      : 1,	//     4
	gpio5_intr                      : 1,	//     5
	gpio6_intr                      : 1,	//     6
	gpio7_intr                      : 1,	//     7
	gpio8_intr                      : 1,	//     8
	gpio9_intr                      : 1,	//     9
	gpio10_intr                     : 1,	//    10
	gpio11_intr                     : 1;	//    11
} L9B0_GPIO_INTERRUPT_STATUS;

/*-----------------------------------------------------------------------------
	0x0178 urgency ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	peri1_aw_urgency                : 2,	//  0: 1
	peri1_ar_urgency                : 2,	//  2: 3
	                                : 4,	//  4: 7 reserved
	peri0_aw_urgency                : 2,	//  8: 9
	peri0_ar_urgency                : 2,	// 10:11
	                                : 4,	// 12:15 reserved
	cpu1_aw_urgency                 : 2,	// 16:17
	cpu1_ar_urgency                 : 2,	// 18:19
	                                : 4,	// 20:23 reserved
	cpu0_aw_urgency                 : 2,	// 24:25
	cpu0_ar_urgency                 : 2,	// 26:27
	sc_aw_urgency                   : 2,	// 28:29
	sc_ar_urgency                   : 2;	// 30:31
} L9B0_URGENCY;

/*-----------------------------------------------------------------------------
	0x017c l2_cache_drreq_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_drreq_evnt_cnt               ;   	// 31: 0
} L9B0_L2_CACHE_DRREQ_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0180 l2_cache_drhit_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_drhit_evnt_cnt               ;   	// 31: 0
} L9B0_L2_CACHE_DRHIT_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0184 l2_cache_dwreq_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_dwreq_evnt_cnt               ;   	// 31: 0
} L9B0_L2_CACHE_DWREQ_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0188 l2_cache_dwtreq_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_dwtreq_evnt_cnt              ;   	// 31: 0
} L9B0_L2_CACHE_DWTREQ_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x018c l2_cache_dwhit_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_dwhit_evnt_cnt               ;   	// 31: 0
} L9B0_L2_CACHE_DWHIT_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0190 l2_cache_irreq_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_irreq_evnt_cnt               ;   	// 31: 0
} L9B0_L2_CACHE_IRREQ_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0194 l2_cache_irhit_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_irhit_evnt_cnt               ;   	// 31: 0
} L9B0_L2_CACHE_IRHIT_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0198 l2_cache_wa_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_wa_evnt_cnt                  ;   	// 31: 0
} L9B0_L2_CACHE_WA_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x019c l2_cache_co_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_co_evnt_cnt                  ;   	// 31: 0
} L9B0_L2_CACHE_CO_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x01a4 l2_cache_event_monitor_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	evnt_en                         : 1,	//     0
	evnt_cnt_reset                  : 1;	//     1
} L9B0_L2_CACHE_EVENT_MONITOR_CTRL;

/*-----------------------------------------------------------------------------
	0x01a8 usb1_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	usb1_endian_ahbms_ehci_bufacc   : 1,	//     1
	usb1_endian_ahbms_ohci_bufacc   : 1,	//     2
	usb1_app_start_clk_i            : 1,	//     3
	usb1_ohci_susp_lgcy_i           : 1,	//     4
	usb1_ss_simulation_mode_i       : 1,	//     5
	usb1_ss_word_if_i               : 1,	//     6
	usb1_ss_utmi_backward_enb_i     : 1,	//     7
	usb1_ss_fladj_val_host_i        : 6,	//  8:13
	                                : 2,	// 14:15 reserved
	usb1_ss_fladj_val_i             : 6,	// 16:21
	usb1_ss_resume_utmi_pls_dis_i   : 1,	//    22
	usb1_ss_autoppd_on_overcur_en_i : 1,	//    23
	usb1_ss_ena_incr16_i            : 1,	//    24
	usb1_ss_ena_incr8_i             : 1,	//    25
	usb1_ss_ena_incr4_i             : 1,	//    26
	usb1_ss_ena_incrx_align_i       : 1,	//    27
	usb1_app_prt_ovrcur_i           : 1,	//    28
	usb1_endian_ahbsl               : 1,	//    29
	usb1_endian_ahbms_ehci          : 1,	//    30
	usb1_endian_ahbms_ohci          : 1;	//    31
} L9B0_USB1_CTRL0;

/*-----------------------------------------------------------------------------
	0x01ac usb1_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	usb1_ohci_0_globalsuspend_o     : 1,	//     0
	usb1_ohci_0_drwe_o              : 1,	//     1
	usb1_ohci_0_rwe_o               : 1,	//     2
	usb1_ohci_0_rmtwkp_o            : 1,	//     3
	usb1_ohci_0_smi_o_n             : 1,	//     4
	usb1_ohci_0_sof_o_n             : 1,	//     5
	usb1_ohci_0_bufacc_o            : 1,	//     6
	usb1_ehci_prt_pwr_o             : 1,	//     7
	usb1_ehci_xfer_cnt_o            :11,	//  8:18
	                                : 1,	//    19 reserved
	usb1_ehci_xfer_prdc_o           : 1,	//    20
	usb1_ehci_bufacc_o              : 1,	//    21
	usb1_ehci_pme_status_o          : 1,	//    22
	usb1_ehci_power_state_ack_o     : 1,	//    23
	usb1_ohci_0_ccs_o               : 1;	//    24
} L9B0_USB1_STATUS0;

/*-----------------------------------------------------------------------------
	0x01b0 usb1_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	usb1_ehci_usbsts_o              : 6,	//  0: 5
	usb1_ohci_1_drwe_o              : 1,	//     1
	usb1_ohci_1_rwe_o               : 1,	//     2
	usb1_ohci_1_rmtwkp_o            : 1,	//     3
	scuramemaw                      : 2,	//  4: 5
	usb1_ohci_1_sof_o_n             : 1,	//     5
	usb1_ohci_1_bufacc_o            : 1,	//     6
	usb1_ehci_prt_pwr_o_p1          : 1,	//     7
	usb1_ehci_lpsmc_state_o         : 4,	//  8:11
	                                : 4,	// 12:15 reserved
	usb1_ss_fladj_val_i_p1          : 6,	// 16:21
	                                : 2,	// 22:23 reserved
	usb1_ohci_1_ccs_o               : 1;	//    24
} L9B0_USB1_STATUS1;

/*-----------------------------------------------------------------------------
	0x01bc ctr64_reg_dco_ctrl ''
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
} L9B0_CTR64_REG_DCO_CTRL;

/*-----------------------------------------------------------------------------
	0x01c0 ctr65_reserved ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x01c4 ctr66_reg_main2_syspll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :24,	//  0:23 reserved
	m2_dr3p_pre_fd_ctrl             : 2,	// 24:25
	m2_dr3p_od_fout_ctrl            : 2,	// 26:27
	m2_dr3p_dtr_sel_ctrl            : 4;	// 28:31
} L9B0_CTR66_REG_MAIN2_SYSPLL_CTRL;

/*-----------------------------------------------------------------------------
	0x01c8 ctr67_reg_main2_syspll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	m2_dr3p_udex_ctrl               : 5,	//  0: 4
	m2_dr3p_msex_ctrl               : 9,	//  5:13
	m2_dr3p_nsc_ctrl                : 4,	// 14:17
	m2_dr3p_npc_ctrl                : 6,	// 18:23
	m2_dr3p_m_ctrl                  : 3,	// 24:26
	m2_dr3p_cih_ctrl                : 3,	// 27:29
	m2_dr3p_lf_ctrl                 : 1,	//    30
	m2_dr3p_pdb_ctrl                : 1;	//    31
} L9B0_CTR67_REG_MAIN2_SYSPLL_CTRL;

/*-----------------------------------------------------------------------------
	0x01cc ctr68_reg_main2_syspll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :24,	//  0:23 reserved
	m1_dr3p_pre_fd_ctrl             : 2,	// 24:25
	m1_dr3p_od_fout_ctrl            : 2,	// 26:27
	m1_dr3p_dtr_sel_ctrl            : 4;	// 28:31
} L9B0_CTR68_REG_MAIN2_SYSPLL_CTRL;

/*-----------------------------------------------------------------------------
	0x01d0 ctr69_reg_main2_syspll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	m1_dr3p_udex_ctrl               : 5,	//  0: 4
	m1_dr3p_msex_ctrl               : 9,	//  5:13
	m1_dr3p_nsc_ctrl                : 4,	// 14:17
	m1_dr3p_npc_ctrl                : 6,	// 18:23
	m1_dr3p_m_ctrl                  : 3,	// 24:26
	m1_dr3p_cih_ctrl                : 3,	// 27:29
	m1_dr3p_lf_ctrl                 : 1,	//    30
	m1_dr3p_pdb_ctrl                : 1;	//    31
} L9B0_CTR69_REG_MAIN2_SYSPLL_CTRL;

/*-----------------------------------------------------------------------------
	0x01d4 ctr70_reg_cpupll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :24,	//  0:23 reserved
	c_dr3p_pre_fd_ctrl              : 2,	// 24:25
	c_dr3p_od_fout_ctrl             : 2,	// 26:27
	c_dr3p_dtr_sel_ctrl             : 4;	// 28:31
} L9B0_CTR70_REG_CPUPLL_CTRL;

/*-----------------------------------------------------------------------------
	0x01d8 ctr71_reg_cpupll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	c_dr3p_udex_ctrl                : 5,	//  0: 4
	c_dr3p_msex_ctrl                : 9,	//  5:13
	c_dr3p_nsc_ctrl                 : 4,	// 14:17
	c_dr3p_npc_ctrl                 : 6,	// 18:23
	c_dr3p_m_ctrl                   : 3,	// 24:26
	c_dr3p_cih_ctrl                 : 3,	// 27:29
	c_dr3p_lf_ctrl                  : 1,	//    30
	c_dr3p_pdb_ctrl                 : 1;	//    31
} L9B0_CTR71_REG_CPUPLL_CTRL;

/*-----------------------------------------------------------------------------
	0x01dc ctr72_reg_hslvds tx_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	lv_flip_en_ctrl                 :12,	//  0:11
	                                : 6,	// 12:17 reserved
	lv_pemp_te2_ctrl                : 2,	// 18:19
	lv_pemp_td2_ctrl                : 2,	// 20:21
	lv_pemp_tclk2_ctrl              : 2,	// 22:23
	lv_pemp_tc2_ctrl                : 2,	// 24:25
	lv_pemp_tb2_ctrl                : 2,	// 26:27
	lv_pemp_ta2_ctrl                : 2,	// 28:29
	lv_pemp_te1_ctrl                : 2;	// 30:31
} L9B0_CTR72_REG_HSLVDS_TX_CTRL;

/*-----------------------------------------------------------------------------
	0x01e0 ctr73_reg_hslvds tx_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	lv_pemp_td1_ctrl                : 2,	//  1: 2
	lv_pemp_tclk1_ctrl              : 2,	//  3: 4
	lv_pemp_tc1_ctrl                : 2,	//  5: 6
	lv_pemp_tb1_ctrl                : 2,	//  7: 8
	lv_pemp_ta1_ctrl                : 2,	//  9:10
	                                : 1,	//    11 reserved
	lv_rf_ctrl                      : 1,	//    12
	lv_rmlvds_en_ctrl               : 1,	//    13
	lv_swc_te2_ctrl                 : 3,	// 14:16
	lv_swc_td2_ctrl                 : 3,	// 17:19
	lv_swc_tclk2_ctrl               : 3,	// 20:22
	lv_swc_tc2_ctrl                 : 3,	// 23:25
	lv_swc_tb2_ctrl                 : 3,	// 26:28
	lv_swc_ta2_ctrl                 : 3;	// 29:31
} L9B0_CTR73_REG_HSLVDS_TX_CTRL;

/*-----------------------------------------------------------------------------
	0x01e4 ctr74_reg_hslvds_tx_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	lv_swc_te1_ctrl                 : 3,	//  1: 3
	lv_swc_td1_ctrl                 : 3,	//  4: 6
	lv_swc_tclk1_ctrl               : 3,	//  7: 9
	lv_swc_tc1_ctrl                 : 3,	// 10:12
	lv_swc_tb1_ctrl                 : 3,	// 13:15
	lv_swc_ta1_ctrl                 : 3,	// 16:18
	lv_pdb_ch_ctrl                  :12,	// 19:30
	lv_pdb_ctrl                     : 1;	//    31
} L9B0_CTR74_REG_HSLVDS_TX_CTRL;

/*-----------------------------------------------------------------------------
	0x01e8 ctr75_reg_hslvds_rx_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 5,	//  0: 4 reserved
	link2_ch_swap                   : 3,	//  5: 7
	                                : 5,	//  8:12 reserved
	link1_ch_swap                   : 3,	// 13:15
	                                : 5,	// 16:20 reserved
	rx1_sel_speed_en                : 1,	//    21
	rx0_sel_speed_en                : 1,	//    22
	                                : 1,	//    23 reserved
	rx1_sel_8b10b_en                : 1,	//    24
	rx0_sel_8b10b_en                : 1,	//    25
	                                : 1,	//    26 reserved
	rx1_rf_en                       : 1,	//    27
	rx0_rf_en                       : 1,	//    28
	                                : 1,	//    29 reserved
	rx1_pdb_en                      : 1,	//    30
	rx0_pdb_en                      : 1;	//    31
} L9B0_CTR75_REG_HSLVDS_RX_CTRL;

/*-----------------------------------------------------------------------------
	0x01ec l2_cache_epfalloc_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_epfalloc_event_cnt           ;   	// 31: 0
} L9B0_L2_CACHE_EPFALLOC_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x01f0 l2_cache_epfhit_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_epfhit_event_cnt             ;   	// 31: 0
} L9B0_L2_CACHE_EPFHIT_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x01f4 l2_cache_epfrcvds0_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_epfrcvds0_event_cnt          ;   	// 31: 0
} L9B0_L2_CACHE_EPFRCVDS0_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x01f8 l2_cache_epfrcvds1_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_epfrcvds1_event_cnt          ;   	// 31: 0
} L9B0_L2_CACHE_EPFRCVDS1_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x01fc l2_cache_srconfs0_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_srconfs0_event_cnt           ;   	// 31: 0
} L9B0_L2_CACHE_SRCONFS0_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0200 l2_cache_srconfs1_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_srconfs1_event_cnt           ;   	// 31: 0
} L9B0_L2_CACHE_SRCONFS1_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0204 l2_cache_srrcvds0_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_srrcvds0_event_cnt           ;   	// 31: 0
} L9B0_L2_CACHE_SRRCVDS0_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0208 l2_cache_srrcvds1_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_srrcvds1_event_cnt           ;   	// 31: 0
} L9B0_L2_CACHE_SRRCVDS1_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x020c l2_cache_ipfalloc_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_ipfalloc_cnt                 ;   	// 31: 0
} L9B0_L2_CACHE_IPFALLOC_CNT;

/*-----------------------------------------------------------------------------
	0x0210 l2_cache_clock_stoped ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :31,	//  0:30 reserved
	l2_clock_stoped                 : 1;	//    31
} L9B0_L2_CACHE_CLOCK_STOPED;

/*-----------------------------------------------------------------------------
	0x0240 cpu0_addr_switch0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	cpu0_addr_sw0_9_3               : 7,	//  3: 9
	                                : 6,	// 10:15 reserved
	cpu0_default_bus                : 2,	// 16:17
	                                : 6,	// 18:23 reserved
	cpu0_addr_sw0_29_24             : 6;	// 24:29
} L9B0_CPU0_ADDR_SWITCH0;

/*-----------------------------------------------------------------------------
	0x0244 cpu0_addr_switch1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	cpu0_addr_sw1_9_3               : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	cpu0_addr_sw1_19_15             : 5,	// 15:19
	                                : 4,	// 20:23 reserved
	cpu0_addr_sw1_29_24             : 6;	// 24:29
} L9B0_CPU0_ADDR_SWITCH1;

/*-----------------------------------------------------------------------------
	0x0248 cpu0_addr_switch2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	cpu0_addr_sw2_9_3               : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	cpu0_addr_sw2_19_15             : 5,	// 15:19
	                                : 6,	// 20:25 reserved
	cpu0_addr_sw2_29_26             : 4;	// 26:29
} L9B0_CPU0_ADDR_SWITCH2;

/*-----------------------------------------------------------------------------
	0x024c cpu1_addr_switch0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	cpu1_addr_sw0_9_3               : 7,	//  3: 9
	                                : 6,	// 10:15 reserved
	cpu1_default_bus                : 2,	// 16:17
	                                : 6,	// 18:23 reserved
	cpu1_addr_sw0_29_24             : 6;	// 24:29
} L9B0_CPU1_ADDR_SWITCH0;

/*-----------------------------------------------------------------------------
	0x0250 cpu1_addr_switch1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	cpu1_addr_sw1_9_3               : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	cpu1_addr_sw1_19_15             : 5,	// 15:19
	                                : 4,	// 20:23 reserved
	cpu1_addr_sw1_29_24             : 6;	// 24:29
} L9B0_CPU1_ADDR_SWITCH1;

/*-----------------------------------------------------------------------------
	0x0254 cpu1_addr_switch2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	cpu1_addr_sw2_9_3               : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	cpu1_addr_sw2_19_15             : 5,	// 15:19
	                                : 6,	// 20:25 reserved
	cpu1_addr_sw2_29_26             : 4;	// 26:29
} L9B0_CPU1_ADDR_SWITCH2;

/*-----------------------------------------------------------------------------
	0x0258 peri0_addr_switch0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	peri0_addr_sw0_9_3              : 7,	//  3: 9
	                                : 6,	// 10:15 reserved
	peri0_default_bus               : 2,	// 16:17
	                                : 6,	// 18:23 reserved
	peri0_addr_sw0_29_24            : 6;	// 24:29
} L9B0_PERI0_ADDR_SWITCH0;

/*-----------------------------------------------------------------------------
	0x025c peri0_addr_switch1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	peri0_addr_sw1_9_3              : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	peri0_addr_sw1_19_15            : 5,	// 15:19
	                                : 4,	// 20:23 reserved
	peri0_addr_sw1_29_24            : 6;	// 24:29
} L9B0_PERI0_ADDR_SWITCH1;

/*-----------------------------------------------------------------------------
	0x0260 peri0_addr_switch2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	peri1_addr_sw2_9_3              : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	peri1_addr_sw2_19_15            : 5,	// 15:19
	                                : 6,	// 20:25 reserved
	peri1_addr_sw2_29_26            : 4;	// 26:29
} L9B0_PERI0_ADDR_SWITCH2;

/*-----------------------------------------------------------------------------
	0x0264 peri1_addr_switch0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	peri1_addr_sw0_9_3              : 7,	//  3: 9
	                                : 6,	// 10:15 reserved
	peri1_default_bus               : 2,	// 16:17
	                                : 6,	// 18:23 reserved
	peri1_addr_sw0_29_24            : 6;	// 24:29
} L9B0_PERI1_ADDR_SWITCH0;

/*-----------------------------------------------------------------------------
	0x0268 peri1_addr_switch1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	peri1_addr_sw1_9_3              : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	peri1_addr_sw1_19_15            : 5,	// 15:19
	                                : 4,	// 20:23 reserved
	peri1_addr_sw1_29_24            : 6;	// 24:29
} L9B0_PERI1_ADDR_SWITCH1;

/*-----------------------------------------------------------------------------
	0x026c peri1_addr_switch2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	peri1_addr_sw2_9_3              : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	peri1_addr_sw2_19_15            : 5,	// 15:19
	                                : 6,	// 20:25 reserved
	peri1_addr_sw2_29_26            : 4;	// 26:29
} L9B0_PERI1_ADDR_SWITCH2;

/*-----------------------------------------------------------------------------
	0x0270 isol_apb_addr_switch0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	isol_apb_addr_sw0_9_3           : 7,	//  3: 9
	                                : 6,	// 10:15 reserved
	isol_apb_default_bus            : 2,	// 16:17
	                                : 6,	// 18:23 reserved
	isol_apb_addr_sw0_29_24         : 6;	// 24:29
} L9B0_ISOL_APB_ADDR_SWITCH0;

/*-----------------------------------------------------------------------------
	0x0274 isol_apb_addr_switch1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	isol_apb_addr_sw1_9_3           : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	isol_apb_addr_sw1_19_15         : 5,	// 15:19
	                                : 4,	// 20:23 reserved
	isol_apb_addr_sw1_29_24         : 6;	// 24:29
} L9B0_ISOL_APB_ADDR_SWITCH1;

/*-----------------------------------------------------------------------------
	0x0278 isol_apb_addr_switch2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	isol_apb_addr_sw2_9_3           : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	isol_apb_addr_sw2_19_15         : 5,	// 15:19
	                                : 6,	// 20:25 reserved
	isol_apb_addr_sw2_29_26         : 4;	// 26:29
} L9B0_ISOL_APB_ADDR_SWITCH2;

/*-----------------------------------------------------------------------------
	0x027c sc_addr_switch0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	sc_addr_sw0_9_3                 : 7,	//  3: 9
	                                : 6,	// 10:15 reserved
	sc_default_bus                  : 2,	// 16:17
	                                : 6,	// 18:23 reserved
	sc_addr_sw0_29_24               : 6;	// 24:29
} L9B0_SC_ADDR_SWITCH0;

/*-----------------------------------------------------------------------------
	0x0280 sc_addr_switch1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	sc_addr_sw1_9_3                 : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	sc_addr_sw1_19_15               : 5,	// 15:19
	                                : 4,	// 20:23 reserved
	sc_addr_sw1_29_24               : 6;	// 24:29
} L9B0_SC_ADDR_SWITCH1;

/*-----------------------------------------------------------------------------
	0x0284 sc_addr_switch2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 3,	//  0: 2 reserved
	sc_addr_sw2_9_3                 : 7,	//  3: 9
	                                : 5,	// 10:14 reserved
	sc_addr_sw2_19_15               : 5,	// 15:19
	                                : 6,	// 20:25 reserved
	sc_addr_sw2_29_26               : 4;	// 26:29
} L9B0_SC_ADDR_SWITCH2;

/*-----------------------------------------------------------------------------
	0x0288 emmc_host_control0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	fb_clk_en                       : 4,	//  0: 3
	ip_tap_ctrl                     : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	ip_tap_delay                    : 6,	//  8:13
	                                : 1,	//    14 reserved
	ip_tap_en                       : 1,	//    15
	op_tap_ctrl                     : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	op_tap_delay                    : 4,	// 20:23
	op_tap_en                       : 1,	//    24
	                                : 3,	// 25:27 reserved
	retuning_require                : 1,	//    28
	                                : 2,	// 29:30 reserved
	cmd_conflict_dis                : 1;	//    31
} L9B0_EMMC_HOST_CONTROL0;

/*-----------------------------------------------------------------------------
	0x028c emmc_host_control1 & cap_reg_in_s1_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cap_reg_in_s1_h                 :21,	//  0:20
	                                : 3,	// 21:23 reserved
	tuning_cmd_success_cnt_s2       : 4,	// 24:27
	tuning_cmd_success_cnt_s1       : 4;	// 28:31
} L9B0_EMMC_HOST_CONTROL1_CAP_REG_IN_S1_H;

/*-----------------------------------------------------------------------------
	0x0290 cap_reg_in_s1_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cap_reg_in_s1_l                 ;   	// 31: 0
} L9B0_CAP_REG_IN_S1_L;

/*-----------------------------------------------------------------------------
	0x0294 init_preset_in_s1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	init_preset_in_s1               :13;	//  0:12
} L9B0_INIT_PRESET_IN_S1;

/*-----------------------------------------------------------------------------
	0x0298 default_preset_in_s1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	default_preset_in_s1            :13;	//  0:12
} L9B0_DEFAULT_PRESET_IN_S1;

/*-----------------------------------------------------------------------------
	0x029c high_speed_preset_in_s1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	high_speed_preset_in_s1         :13;	//  0:12
} L9B0_HIGH_SPEED_PRESET_IN_S1;

/*-----------------------------------------------------------------------------
	0x02a0 sdr12_preset_in_s1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sdr12_preset_in_s1              :13;	//  0:12
} L9B0_SDR12_PRESET_IN_S1;

/*-----------------------------------------------------------------------------
	0x02a4 sdr25_preset_in_s1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sdr25_preset_in_s1              :13;	//  0:12
} L9B0_SDR25_PRESET_IN_S1;

/*-----------------------------------------------------------------------------
	0x02a8 sdr50_preset_in_s1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sdr50_preset_in_s1              :13;	//  0:12
} L9B0_SDR50_PRESET_IN_S1;

/*-----------------------------------------------------------------------------
	0x02ac sdr104_preset_in_s1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sdr104_preset_in_s1             :13;	//  0:12
} L9B0_SDR104_PRESET_IN_S1;

/*-----------------------------------------------------------------------------
	0x02b0 ddr50_preset_in_s1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ddr50_preset_in_s1              :13;	//  0:12
} L9B0_DDR50_PRESET_IN_S1;

/*-----------------------------------------------------------------------------
	0x02b4 cap_reg_in_s2_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cap_reg_in_s2_h                 :21;	//  0:20
} L9B0_CAP_REG_IN_S2_H;

/*-----------------------------------------------------------------------------
	0x02b8 cap_reg_in_s2_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cap_reg_in_s2_l                 ;   	// 31: 0
} L9B0_CAP_REG_IN_S2_L;

/*-----------------------------------------------------------------------------
	0x02bc init_preset_in_s2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	init_preset_in_s2               :13;	//  0:12
} L9B0_INIT_PRESET_IN_S2;

/*-----------------------------------------------------------------------------
	0x02c0 default_preset_in_s2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	default_preset_in_s2            :13;	//  0:12
} L9B0_DEFAULT_PRESET_IN_S2;

/*-----------------------------------------------------------------------------
	0x02c4 high_speed_preset_in_s2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	high_speed_preset_in_s2         :13;	//  0:12
} L9B0_HIGH_SPEED_PRESET_IN_S2;

/*-----------------------------------------------------------------------------
	0x02c8 sdr12_preset_in_s2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sdr12_preset_in_s2              :13;	//  0:12
} L9B0_SDR12_PRESET_IN_S2;

/*-----------------------------------------------------------------------------
	0x02cc sdr25_preset_in_s2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sdr25_preset_in_s2              :13;	//  0:12
} L9B0_SDR25_PRESET_IN_S2;

/*-----------------------------------------------------------------------------
	0x02d0 sdr50_preset_in_s2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sdr50_preset_in_s2              :13;	//  0:12
} L9B0_SDR50_PRESET_IN_S2;

/*-----------------------------------------------------------------------------
	0x02d4 sdr104_preset_in_s2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sdr104_preset_in_s2             :13;	//  0:12
} L9B0_SDR104_PRESET_IN_S2;

/*-----------------------------------------------------------------------------
	0x02d8 ddr50_preset_in_s2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ddr50_preset_in_s2              :13;	//  0:12
} L9B0_DDR50_PRESET_IN_S2;

/*-----------------------------------------------------------------------------
	0x02dc sync_reg0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sync_reg0                       ;   	// 31: 0
} L9B0_SYNC_REG0;

/*-----------------------------------------------------------------------------
	0x02e0 sync_reg1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sync_reg1                       ;   	// 31: 0
} L9B0_SYNC_REG1;

/*-----------------------------------------------------------------------------
	0x02e4 osprey_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :23,	//  0:22 reserved
	npwrupcpu0d7                    : 1,	//    23
	npwrupcpu0d6                    : 1,	//    24
	npwrupcpu0d5                    : 1,	//    25
	npwrupcpu0d4                    : 1,	//    26
	npwrupcpu0d3                    : 1,	//    27
	npwrupcpu0d2                    : 1,	//    28
	npwrupcpu0d1                    : 1,	//    29
	npwrupcpu0d0                    : 1,	//    30
	nisolatecpu0                    : 1;	//    31
} L9B0_OSPREY_CTRL0;

/*-----------------------------------------------------------------------------
	0x02e8 osprey_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :23,	//  0:22 reserved
	npwrupcpu1d7                    : 1,	//    23
	npwrupcpu1d6                    : 1,	//    24
	npwrupcpu1d5                    : 1,	//    25
	npwrupcpu1d4                    : 1,	//    26
	npwrupcpu1d3                    : 1,	//    27
	npwrupcpu1d2                    : 1,	//    28
	npwrupcpu1d1                    : 1,	//    29
	npwrupcpu1d0                    : 1,	//    30
	nisolatecpu0                    : 1;	//    31
} L9B0_OSPREY_CTRL1;

/*-----------------------------------------------------------------------------
	0x02ec osprey_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :23,	//  0:22 reserved
	npwrupneon0d7                   : 1,	//    23
	npwrupneon0d6                   : 1,	//    24
	npwrupneon0d5                   : 1,	//    25
	npwrupneon0d4                   : 1,	//    26
	npwrupneon0d3                   : 1,	//    27
	npwrupneon0d2                   : 1,	//    28
	npwrupneon0d1                   : 1,	//    29
	npwrupneon0d0                   : 1,	//    30
	nisolateneon0                   : 1;	//    31
} L9B0_OSPREY_CTRL2;

/*-----------------------------------------------------------------------------
	0x02f0 osprey_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :23,	//  0:22 reserved
	npwrupneon1d7                   : 1,	//    23
	npwrupneon1d6                   : 1,	//    24
	npwrupneon1d5                   : 1,	//    25
	npwrupneon1d4                   : 1,	//    26
	npwrupneon1d3                   : 1,	//    27
	npwrupneon1d2                   : 1,	//    28
	npwrupneon1d1                   : 1,	//    29
	npwrupneon1d0                   : 1,	//    30
	nisolateneon1                   : 1;	//    31
} L9B0_OSPREY_CTRL3;

/*-----------------------------------------------------------------------------
	0x02f4 osprey_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	npwrupdbgvsocd0                 : 1,	//     0
	                                :30,	//  1:30 reserved
	nisolatedbgvsoc                 : 1;	//    31
} L9B0_OSPREY_CTRL4;

/*-----------------------------------------------------------------------------
	0x02f8 osprey_ctrl5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :23,	//  0:22 reserved
	npwrupscupl310d7                : 1,	//    23
	npwrupscupl310d6                : 1,	//    24
	npwrupscupl310d5                : 1,	//    25
	npwrupscupl310d4                : 1,	//    26
	npwrupscupl310d3                : 1,	//    27
	npwrupscupl310d2                : 1,	//    28
	npwrupscupl310d1                : 1,	//    29
	npwrupscupl310d0                : 1,	//    30
	nisolatescupl310                : 1;	//    31
} L9B0_OSPREY_CTRL5;

/*-----------------------------------------------------------------------------
	0x02fc osprey_ctrl6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :27,	//  0:26 reserved
	npwrupdbgpd3                    : 1,	//    27
	npwrupdbgpd2                    : 1,	//    28
	npwrupdbgpd1                    : 1,	//    29
	npwrupdbgpd0                    : 1,	//    30
	nisolatesdbgp                   : 1;	//    31
} L9B0_OSPREY_CTRL6;

/*-----------------------------------------------------------------------------
	0x0300 osprey_ctrl7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :29,	//  0:28 reserved
	npwrupdbgmd1                    : 1,	//    29
	npwrupdbgmd0                    : 1,	//    30
	nisolatesdbgm                   : 1;	//    31
} L9B0_OSPREY_CTRL7;

/*-----------------------------------------------------------------------------
	0x0304 osprey_ctrl8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cfgfiltend                      :12,	//  0:11
	cfgfiltstart                    :12,	// 12:23
	                                : 7,	// 24:30 reserved
	cfgaddrfilten                   : 1;	//    31
} L9B0_OSPREY_CTRL8;

/*-----------------------------------------------------------------------------
	0x0308 osprey_ctrl9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	csysreqm1vsoc                   : 1,	//     0
	                                :30,	//  1:30 reserved
	csysreqm0vsoc                   : 1;	//    31
} L9B0_OSPREY_CTRL9;

/*-----------------------------------------------------------------------------
	0x030c osprey_ctrl10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tpmaxdatasize                   : 5;	//  0: 4
} L9B0_OSPREY_CTRL10;

/*-----------------------------------------------------------------------------
	0x0310 osprey_ctrl11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cihsbypass                      : 4,	//  0: 3
	                                :27,	//  4:30 reserved
	cisbypass                       : 1;	//    31
} L9B0_OSPREY_CTRL11;

/*-----------------------------------------------------------------------------
	0x0314 osprey_ctrl12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :31,	//  0:30 reserved
	cdbgrstack                      : 1;	//    31
} L9B0_OSPREY_CTRL12;

/*-----------------------------------------------------------------------------
	0x0318 cpu_flexnoc_bypass_control ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	asyncbypass_m00_rvs             : 1,	//     0
	                                :30,	//  1:30 reserved
	asyncbypass_m00_fwd             : 1;	//    31
} L9B0_CPU_FLEXNOC_BYPASS_CONTROL;

/*-----------------------------------------------------------------------------
	0x0334 osprey_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	smpnamp                         : 2,	//  0: 1
	pl310idle                       : 1,	//     2
	                                : 6,	//  3: 8 reserved
	deflags0                        : 7,	//  9:15
	deflags1                        : 7,	// 16:22
	pmusecure                       : 2,	// 23:24
	pmupriv                         : 2,	// 25:26
	ptmpwrup1                       : 1,	//    27
	ptmidleack1                     : 1,	//    28
	ptmpwrup0                       : 1,	//    29
	ptmidleack0                     : 1,	//    30
	scuevabort                      : 1;	//    31
} L9B0_OSPREY_STATUS0;

/*-----------------------------------------------------------------------------
	0x0338 osprey_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	datawritelat                    : 3,	//  0: 2
	datareadlat                     : 3,	//  3: 5
	datasetuplat                    : 3,	//  6: 8
	tagwritelat                     : 3,	//  9:11
	tagreadlat                      : 3,	// 12:14
	tagsetuplat                     : 3,	// 15:17
	pwrctlo0                        : 2,	// 18:19
	                                : 2,	// 20:21 reserved
	pwrctlo1                        : 2,	// 22:23
	                                : 2,	// 24:25 reserved
	standbywfi                      : 2,	// 26:27
	                                : 2,	// 28:29 reserved
	standbywfe                      : 2;	// 30:31
} L9B0_OSPREY_STATUS1;

/*-----------------------------------------------------------------------------
	0x033c osprey_status2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	csysackm1vsoc                   : 1,	//     0
	cdbgrstreq                      : 1,	//     1
	                                :18,	//  2:19 reserved
	nfiqout                         : 2,	// 20:21
	nirqout                         : 2,	// 22:23
	                                : 2,	// 24:25 reserved
	dbgnoppwrdn                     : 2,	// 26:27
	                                : 1,	//    28 reserved
	dbgcpudone                      : 2,	// 29:30
	csysackm0vsoc                   : 1;	//    31
} L9B0_OSPREY_STATUS2;

/*-----------------------------------------------------------------------------
	0x0340 osprey_status3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cpuready                        : 2,	//  0: 1
	neonready                       : 2,	//  2: 3
	                                :27,	//  4:30 reserved
	scuready                        : 1;	//    31
} L9B0_OSPREY_STATUS3;

/*-----------------------------------------------------------------------------
	0x0344 boot_mode ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x0348 usb2_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	usb2_endian_ahbms_ehci_bufacc   : 1,	//     1
	usb2_endian_ahbms_ohci_bufacc   : 1,	//     2
	usb2_app_start_clk_i            : 1,	//     3
	usb2_ohci_susp_lgcy_i           : 1,	//     4
	usb2_ss_simulation_mode_i       : 1,	//     5
	usb2_ss_word_if_i               : 1,	//     6
	usb2_ss_utmi_backward_enb_i     : 1,	//     7
	usb2_ss_fladj_val_host_i        : 6,	//  8:13
	                                : 2,	// 14:15 reserved
	usb2_ss_fladj_val_i             : 6,	// 16:21
	usb2_ss_resume_utmi_pls_dis_i   : 1,	//    22
	usb2_ss_autoppd_on_overcur_en_i : 1,	//    23
	usb2_ss_ena_incr16_i            : 1,	//    24
	usb2_ss_ena_incr8_i             : 1,	//    25
	usb2_ss_ena_incr4_i             : 1,	//    26
	usb2_ss_ena_incrx_align_i       : 1,	//    27
	usb2_app_prt_ovrcur_i           : 1,	//    28
	usb2_endian_ahbsl               : 1,	//    29
	usb2_endian_ahbms_ehci          : 1,	//    30
	usb2_endian_ahbms_ohci          : 1;	//    31
} L9B0_USB2_CTRL0;

/*-----------------------------------------------------------------------------
	0x034c usb2_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	usb2_ohci_0_globalsuspend_o     : 1,	//     0
	usb2_ohci_0_drwe_o              : 1,	//     1
	usb2_ohci_0_rwe_o               : 1,	//     2
	usb2_ohci_0_rmtwkp_o            : 1,	//     3
	usb2_ohci_0_smi_o_n             : 1,	//     4
	usb2_ohci_0_sof_o_n             : 1,	//     5
	usb2_ohci_0_bufacc_o            : 1,	//     6
	usb2_ehci_prt_pwr_o             : 1,	//     7
	usb2_ehci_xfer_cnt_o            :11,	//  8:18
	                                : 1,	//    19 reserved
	usb2_ehci_xfer_prdc_o           : 1,	//    20
	usb2_ehci_bufacc_o              : 1,	//    21
	usb2_ehci_pme_status_o          : 1,	//    22
	usb2_ehci_power_state_ack_o     : 1,	//    23
	usb2_ohci_0_ccs_o               : 1;	//    24
} L9B0_USB2_STATUS0;

/*-----------------------------------------------------------------------------
	0x0350 usb2_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	inputed_data3_from_ddr          ,   	// 31: 0
	osd3_process_state              : 3;	// 16:18
} L9B0_USB2_STATUS1;

typedef struct {
	UINT32                          	                     __rsvd_00[   4];	// 0x0000 ~ 0x000c
	L9B0_CTR00_REG_CLK_DIV_SEL           	ctr00_reg_clk_div_sel           ;	// 0x0010 : ''
	L9B0_CTR01_REG_CLK_DIV_SEL           	ctr01_reg_clk_div_sel           ;	// 0x0014 : ''
	L9B0_CTR02_REG_CLK_DIV_SEL           	ctr02_reg_clk_div_sel           ;	// 0x0018 : ''
	L9B0_CTR03_REG_CLK_DIV_SEL           	ctr03_reg_clk_div_sel           ;	// 0x001c : ''
	L9B0_CTR04_REG_CLK_DIV_SEL           	ctr04_reg_clk_div_sel           ;	// 0x0020 : ''
	L9B0_CTR05_REG_SWRS_OF_DE            	ctr05_reg_swrs_of_de            ;	// 0x0024 : ''
	L9B0_CTR06_REG_SWRST                 	ctr06_reg_swrst                 ;	// 0x0028 : ''
	L9B0_CTR07_REG_SWRST                 	ctr07_reg_swrst                 ;	// 0x002c : ''
	L9B0_CTR08_REG_CLK_DIV_SEL           	ctr08_reg_clk_div_sel           ;	// 0x0030 : ''
	L9B0_CTR09_REG_SWRST                 	ctr09_reg_swrst                 ;	// 0x0034 : ''
	L9B0_CTR10_REG_AHBCLK_DIV_SEL        	ctr10_reg_ahbclk_div_sel        ;	// 0x0038 : ''
	L9B0_CTR11_REG_AHBCLK_DIV_SEL        	ctr11_reg_ahbclk_div_sel        ;	// 0x003c : ''
	L9B0_CTR12_REG_RMIICLK_OFF           	ctr12_reg_rmiiclk_off           ;	// 0x0040 : ''
	L9B0_CTR13_REG_AHBCLK_DIV_SEL        	ctr13_reg_ahbclk_div_sel        ;	// 0x0044 : ''
	L9B0_CTR14_REG_AHB_SWRST             	ctr14_reg_ahb_swrst             ;	// 0x0048 : ''
	L9B0_CTR15_REG_AHB_SWRST             	ctr15_reg_ahb_swrst             ;	// 0x004c : ''
	L9B0_CTR16_REG_BUSCLK_DIV_SEL        	ctr16_reg_busclk_div_sel        ;	// 0x0050 : ''
	L9B0_CTR17_REG_BUSCLK_DIV_SEL        	ctr17_reg_busclk_div_sel        ;	// 0x0054 : ''
	L9B0_CTR18_REB_CPUBUS_SWRST          	ctr18_reb_cpubus_swrst          ;	// 0x0058 : ''
	L9B0_CTR19_REG_APBCLK_DIV_SEL        	ctr19_reg_apbclk_div_sel        ;	// 0x005c : ''
	L9B0_CTR20_REG_APBCLK_DIV_SEL        	ctr20_reg_apbclk_div_sel        ;	// 0x0060 : ''
	L9B0_CTR21_REG_APBCLK_DIV_SEL        	ctr21_reg_apbclk_div_sel        ;	// 0x0064 : ''
	L9B0_CTR22_REG_APBCLK_DIV_SEL        	ctr22_reg_apbclk_div_sel        ;	// 0x0068 : ''
	UINT32                          	    ctr23_reserved                  ;	// 0x006c : ''
	L9B0_CTR24_REG_APB_SWRST             	ctr24_reg_apb_swrst             ;	// 0x0070 : ''
	L9B0_CTR25_REG_DE_SWRST              	ctr25_reg_de_swrst              ;	// 0x0074 : ''
	L9B0_CTR26_REG_EXTCLK_DIV_SEL        	ctr26_reg_extclk_div_sel        ;	// 0x0078 : ''
	L9B0_CTR27_REG_EXTCLK_SWRST          	ctr27_reg_extclk_swrst          ;	// 0x007c : ''
	L9B0_CTR28_REG_CPU_SWRST             	ctr28_reg_cpu_swrst             ;	// 0x0080 : ''
	L9B0_CTR29_REG_SWRST                 	ctr29_reg_swrst                 ;	// 0x0084 : ''
	L9B0_CTR30_REG_BLOCK_MONITER         	ctr30_reg_block_moniter         ;	// 0x0088 : ''
	L9B0_CTR31_REG_GPIO_PULLUP_CTRL      	ctr31_reg_gpio_pullup_ctrl      ;	// 0x008c : ''
	L9B0_CTR32_REG_GPIO_MUX_ENABLE       	ctr32_reg_gpio_mux_enable       ;	// 0x0090 : ''
	UINT32                          	    ctr33_reserved                  ;	// 0x0094 : ''
	L9B0_CTR34_REG_GPIO_MUX_ENABLE       	ctr34_reg_gpio_mux_enable       ;	// 0x0098 : ''
	L9B0_CTR35_REG_GPIO_MUX_ENABLE       	ctr35_reg_gpio_mux_enable       ;	// 0x009c : ''
	L9B0_CTR36_REG_UART_SEL              	ctr36_reg_uart_sel              ;	// 0x00a0 : ''
	L9B0_CTR37_REG_GPIO_MUX_ENABLE       	ctr37_reg_gpio_mux_enable       ;	// 0x00a4 : ''
	L9B0_CTR38_REG_GPIO_MUX_ENABLE       	ctr38_reg_gpio_mux_enable       ;	// 0x00a8 : ''
	L9B0_CTR39_REG_GPIO_MUX_ENABLE       	ctr39_reg_gpio_mux_enable       ;	// 0x00ac : ''
	L9B0_CTR40_REG_GPU_DFT_CTRL          	ctr40_reg_gpu_dft_ctrl          ;	// 0x00b0 : ''
	L9B0_CTR41_REG_PULLUP_CTRL           	ctr41_reg_pullup_ctrl           ;	// 0x00b4 : ''
	L9B0_CTR42_REG_PULLUP_CTRL           	ctr42_reg_pullup_ctrl           ;	// 0x00b8 : ''
	L9B0_CTR43_REG_PULLUP_CTRL           	ctr43_reg_pullup_ctrl           ;	// 0x00bc : ''
	L9B0_CTR44_REG_PULLUP_CTRL           	ctr44_reg_pullup_ctrl           ;	// 0x00c0 : ''
	L9B0_CTR45_REG_PULLUP_CTRL           	ctr45_reg_pullup_ctrl           ;	// 0x00c4 : ''
	L9B0_CTR46_REG_PULLUP_CTRL           	ctr46_reg_pullup_ctrl           ;	// 0x00c8 : ''
	L9B0_CTR47_REG_PULLUP_CTRL           	ctr47_reg_pullup_ctrl           ;	// 0x00cc : ''
	L9B0_CTR48_REG_PULLUP_CTRL           	ctr48_reg_pullup_ctrl           ;	// 0x00d0 : ''
	L9B0_CTR49_REG_PULLUP_CTRL           	ctr49_reg_pullup_ctrl           ;	// 0x00d4 : ''
	L9B0_CTR50_REG_TPIO_SEL              	ctr50_reg_tpio_sel              ;	// 0x00d8 : ''
	L9B0_CTR51_REG_GPIO_MUX_ENABLE       	ctr51_reg_gpio_mux_enable       ;	// 0x00dc : ''
	L9B0_CTR52_REG_GPIO_MUX_ENABLE       	ctr52_reg_gpio_mux_enable       ;	// 0x00e0 : ''
	L9B0_CTR53_REG_GPIO_MUX_ENABLE       	ctr53_reg_gpio_mux_enable       ;	// 0x00e4 : ''
	L9B0_CTR54_REG_BT_USB_CTRL           	ctr54_reg_bt_usb_ctrl           ;	// 0x00e8 : ''
	L9B0_CTR55_RGG_SDEC_DCO_FCW_CTRL     	ctr55_rgg_sdec_dco_fcw_ctrl     ;	// 0x00ec : ''
	L9B0_CTR56_REG_BT_USB_CTRL           	ctr56_reg_bt_usb_ctrl           ;	// 0x00f0 : ''
	L9B0_CTR57_REG_BT_USB_CTRL           	ctr57_reg_bt_usb_ctrl           ;	// 0x00f4 : ''
	L9B0_CTR58_REG_MAIN_0_1_USB_CTRL     	ctr58_reg_main_0_1_usb_ctrl     ;	// 0x00f8 : ''
	L9B0_CTR59_REG_BT_USB_CTRL           	ctr59_reg_bt_usb_ctrl           ;	// 0x00fc : ''
	L9B0_CTR60_REG_MAIN_0_1_USB_CTRL     	ctr60_reg_main_0_1_usb_ctrl     ;	// 0x0100 : ''
	L9B0_CTR61_REG_MAIN_0_1_USB_CTRL     	ctr61_reg_main_0_1_usb_ctrl     ;	// 0x0104 : ''
	L9B0_CTR62_REG_DISPLAY_PLL_CTRL      	ctr62_reg_display_pll_ctrl      ;	// 0x0108 : ''
	L9B0_CTR63_REG_DISPLAY_PLL_CTRL      	ctr63_reg_display_pll_ctrl      ;	// 0x010c : ''
	L9B0_USB0_CTRL0                      	usb0_ctrl0                      ;	// 0x0110 : ''
	L9B0_USB0_STATUS0                    	usb0_status0                    ;	// 0x0114 : ''
	L9B0_USB0_STATUS1                    	usb0_status1                    ;	// 0x0118 : ''
	UINT32                          	                     __rsvd_01[   3];	// 0x011c ~ 0x0124
	L9B0_IO_CTRL                         	io_ctrl                         ;	// 0x0128 : ''
	UINT32                          	                     __rsvd_02[   3];	// 0x012c ~ 0x0134
	L9B0_APB_TIME_OUT_CTRL               	apb_time_out_ctrl               ;	// 0x0138 : ''
	L9B0_APB_TIME_OUT_SRC_31_0           	apb_time_out_src_31_0           ;	// 0x013c : ''
	L9B0_APB_TIME_OUT_SRC_41_32          	apb_time_out_src_41_32          ;	// 0x0140 : ''
	UINT32                          	                     __rsvd_03[   1];	// 0x0144
	L9B0_STCC_REF_TIME_STAMP_32          	stcc_ref_time_stamp_32          ;	// 0x0148 : ''
	L9B0_STCC_REF_TIME_STAMP_31_0        	stcc_ref_time_stamp_31_0        ;	// 0x014c : ''
	L9B0_STCC_REAL_TIME_STAMP_32         	stcc_real_time_stamp_32         ;	// 0x0150 : ''
	L9B0_STCC_REAL_TIME_STAMP_31_0       	stcc_real_time_stamp_31_0       ;	// 0x0154 : ''
	L9B0_FLUSH_DONE_STATUS               	flush_done_status               ;	// 0x0158 : ''
	L9B0_FLUSH_REQ                       	flush_req                       ;	// 0x015c : ''
	UINT32                          	                     __rsvd_04[   1];	// 0x0160
	L9B0_BANDWIDTH_LIMITER_AXI0_DLY_CNT  	bandwidth_limiter_axi0_dly_cnt  ;	// 0x0164 : ''
	L9B0_BANDWIDTH_LIMITER_AXI1_DLY_CNT  	bandwidth_limiter_axi1_dly_cnt  ;	// 0x0168 : ''
	L9B0_BANDWIDTH_LIMITER_CPU_AXI0_DLY_CNT	bandwidth_limiter_cpu_axi0_dly_cnt;	// 0x016c : ''
	L9B0_BANDWIDTH_LIMITER_CPU_AXI1_DLY_CNT	bandwidth_limiter_cpu_axi1_dly_cnt;	// 0x0170 : ''
	L9B0_GPIO_INTERRUPT_STATUS           	gpio_interrupt_status           ;	// 0x0174 : ''
	L9B0_URGENCY                         	urgency                         ;	// 0x0178 : ''
	L9B0_L2_CACHE_DRREQ_EVENT_CNT        	l2_cache_drreq_event_cnt        ;	// 0x017c : ''
	L9B0_L2_CACHE_DRHIT_EVENT_CNT        	l2_cache_drhit_event_cnt        ;	// 0x0180 : ''
	L9B0_L2_CACHE_DWREQ_EVENT_CNT        	l2_cache_dwreq_event_cnt        ;	// 0x0184 : ''
	L9B0_L2_CACHE_DWTREQ_EVENT_CNT       	l2_cache_dwtreq_event_cnt       ;	// 0x0188 : ''
	L9B0_L2_CACHE_DWHIT_EVENT_CNT        	l2_cache_dwhit_event_cnt        ;	// 0x018c : ''
	L9B0_L2_CACHE_IRREQ_EVENT_CNT        	l2_cache_irreq_event_cnt        ;	// 0x0190 : ''
	L9B0_L2_CACHE_IRHIT_EVENT_CNT        	l2_cache_irhit_event_cnt        ;	// 0x0194 : ''
	L9B0_L2_CACHE_WA_EVENT_CNT           	l2_cache_wa_event_cnt           ;	// 0x0198 : ''
	L9B0_L2_CACHE_CO_EVENT_CNT           	l2_cache_co_event_cnt           ;	// 0x019c : ''
	UINT32                          	                     __rsvd_05[   1];	// 0x01a0
	L9B0_L2_CACHE_EVENT_MONITOR_CTRL     	l2_cache_event_monitor_ctrl     ;	// 0x01a4 : ''
	L9B0_USB1_CTRL0                      	usb1_ctrl0                      ;	// 0x01a8 : ''
	L9B0_USB1_STATUS0                    	usb1_status0                    ;	// 0x01ac : ''
	L9B0_USB1_STATUS1                    	usb1_status1                    ;	// 0x01b0 : ''	
	UINT32                          	                      __rsvd_06[  2];	// 0x01b4 ~ 0x01b8
	L9B0_CTR64_REG_DCO_CTRL              	ctr64_reg_dco_ctrl              ;	// 0x01bc : ''
	UINT32                          	    ctr65_reserved                  ;	// 0x01c0 : ''
	L9B0_CTR66_REG_MAIN2_SYSPLL_CTRL     	ctr66_reg_main2_syspll_ctrl     ;	// 0x01c4 : ''
	L9B0_CTR67_REG_MAIN2_SYSPLL_CTRL     	ctr67_reg_main2_syspll_ctrl     ;	// 0x01c8 : ''
	L9B0_CTR68_REG_MAIN2_SYSPLL_CTRL     	ctr68_reg_main2_syspll_ctrl     ;	// 0x01cc : ''
	L9B0_CTR69_REG_MAIN2_SYSPLL_CTRL     	ctr69_reg_main2_syspll_ctrl     ;	// 0x01d0 : ''
	L9B0_CTR70_REG_CPUPLL_CTRL           	ctr70_reg_cpupll_ctrl           ;	// 0x01d4 : ''
	L9B0_CTR71_REG_CPUPLL_CTRL           	ctr71_reg_cpupll_ctrl           ;	// 0x01d8 : ''
	L9B0_CTR72_REG_HSLVDS_TX_CTRL        	ctr72_reg_hslvds_tx_ctrl        ;	// 0x01dc : ''
	L9B0_CTR73_REG_HSLVDS_TX_CTRL        	ctr73_reg_hslvds_tx_ctrl        ;	// 0x01e0 : ''
	L9B0_CTR74_REG_HSLVDS_TX_CTRL        	ctr74_reg_hslvds_tx_ctrl        ;	// 0x01e4 : ''
	L9B0_CTR75_REG_HSLVDS_RX_CTRL        	ctr75_reg_hslvds_rx_ctrl        ;	// 0x01e8 : ''
	L9B0_L2_CACHE_EPFALLOC_EVENT_CNT     	l2_cache_epfalloc_event_cnt     ;	// 0x01ec : ''
	L9B0_L2_CACHE_EPFHIT_EVENT_CNT       	l2_cache_epfhit_event_cnt       ;	// 0x01f0 : ''
	L9B0_L2_CACHE_EPFRCVDS0_EVENT_CNT    	l2_cache_epfrcvds0_event_cnt    ;	// 0x01f4 : ''
	L9B0_L2_CACHE_EPFRCVDS1_EVENT_CNT    	l2_cache_epfrcvds1_event_cnt    ;	// 0x01f8 : ''
	L9B0_L2_CACHE_SRCONFS0_EVENT_CNT     	l2_cache_srconfs0_event_cnt     ;	// 0x01fc : ''
	L9B0_L2_CACHE_SRCONFS1_EVENT_CNT     	l2_cache_srconfs1_event_cnt     ;	// 0x0200 : ''
	L9B0_L2_CACHE_SRRCVDS0_EVENT_CNT     	l2_cache_srrcvds0_event_cnt     ;	// 0x0204 : ''
	L9B0_L2_CACHE_SRRCVDS1_EVENT_CNT     	l2_cache_srrcvds1_event_cnt     ;	// 0x0208 : ''
	L9B0_L2_CACHE_IPFALLOC_CNT           	l2_cache_ipfalloc_cnt           ;	// 0x020c : ''
	L9B0_L2_CACHE_CLOCK_STOPED           	l2_cache_clock_stoped           ;	// 0x0210 : ''
	UINT32                          	                     __rsvd_07[  11];	// 0x0214 ~ 0x023c
	L9B0_CPU0_ADDR_SWITCH0               	cpu0_addr_switch0               ;	// 0x0240 : ''
	L9B0_CPU0_ADDR_SWITCH1               	cpu0_addr_switch1               ;	// 0x0244 : ''
	L9B0_CPU0_ADDR_SWITCH2               	cpu0_addr_switch2               ;	// 0x0248 : ''
	L9B0_CPU1_ADDR_SWITCH0               	cpu1_addr_switch0               ;	// 0x024c : ''
	L9B0_CPU1_ADDR_SWITCH1               	cpu1_addr_switch1               ;	// 0x0250 : ''
	L9B0_CPU1_ADDR_SWITCH2               	cpu1_addr_switch2               ;	// 0x0254 : ''
	L9B0_PERI0_ADDR_SWITCH0              	peri0_addr_switch0              ;	// 0x0258 : ''
	L9B0_PERI0_ADDR_SWITCH1              	peri0_addr_switch1              ;	// 0x025c : ''
	L9B0_PERI0_ADDR_SWITCH2              	peri0_addr_switch2              ;	// 0x0260 : ''
	L9B0_PERI1_ADDR_SWITCH0              	peri1_addr_switch0              ;	// 0x0264 : ''
	L9B0_PERI1_ADDR_SWITCH1              	peri1_addr_switch1              ;	// 0x0268 : ''
	L9B0_PERI1_ADDR_SWITCH2              	peri1_addr_switch2              ;	// 0x026c : ''
	L9B0_ISOL_APB_ADDR_SWITCH0           	isol_apb_addr_switch0           ;	// 0x0270 : ''
	L9B0_ISOL_APB_ADDR_SWITCH1           	isol_apb_addr_switch1           ;	// 0x0274 : ''
	L9B0_ISOL_APB_ADDR_SWITCH2           	isol_apb_addr_switch2           ;	// 0x0278 : ''
	L9B0_SC_ADDR_SWITCH0                 	sc_addr_switch0                 ;	// 0x027c : ''
	L9B0_SC_ADDR_SWITCH1                 	sc_addr_switch1                 ;	// 0x0280 : ''
	L9B0_SC_ADDR_SWITCH2                 	sc_addr_switch2                 ;	// 0x0284 : ''
	L9B0_EMMC_HOST_CONTROL0              	emmc_host_control0              ;	// 0x0288 : ''
	L9B0_EMMC_HOST_CONTROL1_CAP_REG_IN_S1_H 	emmc_host_control1_cap_reg_in_s1_h;	// 0x028c : ''
	L9B0_CAP_REG_IN_S1_L                 	cap_reg_in_s1_l                 ;	// 0x0290 : ''
	L9B0_INIT_PRESET_IN_S1               	init_preset_in_s1               ;	// 0x0294 : ''
	L9B0_DEFAULT_PRESET_IN_S1            	default_preset_in_s1            ;	// 0x0298 : ''
	L9B0_HIGH_SPEED_PRESET_IN_S1         	high_speed_preset_in_s1         ;	// 0x029c : ''
	L9B0_SDR12_PRESET_IN_S1              	sdr12_preset_in_s1              ;	// 0x02a0 : ''
	L9B0_SDR25_PRESET_IN_S1              	sdr25_preset_in_s1              ;	// 0x02a4 : ''
	L9B0_SDR50_PRESET_IN_S1              	sdr50_preset_in_s1              ;	// 0x02a8 : ''
	L9B0_SDR104_PRESET_IN_S1             	sdr104_preset_in_s1             ;	// 0x02ac : ''
	L9B0_DDR50_PRESET_IN_S1              	ddr50_preset_in_s1              ;	// 0x02b0 : ''
	L9B0_CAP_REG_IN_S2_H                 	cap_reg_in_s2_h                 ;	// 0x02b4 : ''
	L9B0_CAP_REG_IN_S2_L                 	cap_reg_in_s2_l                 ;	// 0x02b8 : ''
	L9B0_INIT_PRESET_IN_S2               	init_preset_in_s2               ;	// 0x02bc : ''
	L9B0_DEFAULT_PRESET_IN_S2            	default_preset_in_s2            ;	// 0x02c0 : ''
	L9B0_HIGH_SPEED_PRESET_IN_S2         	high_speed_preset_in_s2         ;	// 0x02c4 : ''
	L9B0_SDR12_PRESET_IN_S2              	sdr12_preset_in_s2              ;	// 0x02c8 : ''
	L9B0_SDR25_PRESET_IN_S2              	sdr25_preset_in_s2              ;	// 0x02cc : ''
	L9B0_SDR50_PRESET_IN_S2              	sdr50_preset_in_s2              ;	// 0x02d0 : ''
	L9B0_SDR104_PRESET_IN_S2             	sdr104_preset_in_s2             ;	// 0x02d4 : ''
	L9B0_DDR50_PRESET_IN_S2              	ddr50_preset_in_s2              ;	// 0x02d8 : ''
	L9B0_SYNC_REG0                       	sync_reg0                       ;	// 0x02dc : ''
	L9B0_SYNC_REG1                       	sync_reg1                       ;	// 0x02e0 : ''
	L9B0_OSPREY_CTRL0                    	osprey_ctrl0                    ;	// 0x02e4 : ''
	L9B0_OSPREY_CTRL1                    	osprey_ctrl1                    ;	// 0x02e8 : ''
	L9B0_OSPREY_CTRL2                    	osprey_ctrl2                    ;	// 0x02ec : ''
	L9B0_OSPREY_CTRL3                    	osprey_ctrl3                    ;	// 0x02f0 : ''
	L9B0_OSPREY_CTRL4                    	osprey_ctrl4                    ;	// 0x02f4 : ''
	L9B0_OSPREY_CTRL5                    	osprey_ctrl5                    ;	// 0x02f8 : ''
	L9B0_OSPREY_CTRL6                    	osprey_ctrl6                    ;	// 0x02fc : ''
	L9B0_OSPREY_CTRL7                    	osprey_ctrl7                    ;	// 0x0300 : ''
	L9B0_OSPREY_CTRL8                    	osprey_ctrl8                    ;	// 0x0304 : ''
	L9B0_OSPREY_CTRL9                    	osprey_ctrl9                    ;	// 0x0308 : ''
	L9B0_OSPREY_CTRL10                   	osprey_ctrl10                   ;	// 0x030c : ''
	L9B0_OSPREY_CTRL11                   	osprey_ctrl11                   ;	// 0x0310 : ''
	L9B0_OSPREY_CTRL12                   	osprey_ctrl12                   ;	// 0x0314 : ''
	L9B0_CPU_FLEXNOC_BYPASS_CONTROL      	cpu_flexnoc_bypass_control      ;	// 0x0318 : ''
	UINT32                              	                 __rsvd_08[   6];	// 0x031c ~ 0x0330
	L9B0_OSPREY_STATUS0                  	osprey_status0                  ;	// 0x0334 : ''
	L9B0_OSPREY_STATUS1                  	osprey_status1                  ;	// 0x0338 : ''
	L9B0_OSPREY_STATUS2                  	osprey_status2                  ;	// 0x033c : ''
	L9B0_OSPREY_STATUS3                  	osprey_status3                  ;	// 0x0340 : ''
	UINT32                              	boot_mode                       ;	// 0x0344 : ''
	L9B0_USB2_CTRL0                      	usb2_ctrl0                      ;	// 0x0348 : ''
	L9B0_USB2_STATUS0                    	usb2_status0                    ;	// 0x034c : ''
	L9B0_USB2_STATUS1                    	usb2_status1                    ;	// 0x0350 : ''	
} CTOP_CTRL_REG_L9B0_T;

#endif	/* _#MOD#_REG_H_ */

/* from 'LG1152B0-CTOP-SUM-01-v1.0(TOP_CTRL_REG).csv' 20110822 09:38:38   대한민국 표준시 by getregs v2.7 */


