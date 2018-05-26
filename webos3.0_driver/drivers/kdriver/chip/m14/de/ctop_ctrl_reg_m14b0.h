/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 

#ifndef _REG_CTOP_CTRL_M14B0_h
#define _REG_CTOP_CTRL_M14B0_h

/*-----------------------------------------------------------------------------
	0xfd300010 ctr00_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 gfx_aclk_sel                : 3;   //  1: 3
	UINT32                             : 1;   //     4 reserved
	UINT32 gfx_pclk_sel                : 3;   //  5: 7
	UINT32                             : 5;   //  8:12 reserved
	UINT32 cvda_clk_sel                : 3;   // 13:15
	UINT32                             : 5;   // 16:20 reserved
	UINT32 de_apb_clk_sel              : 3;   // 21:23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 swrst_usb_bt_init           : 1;   //    27
	UINT32 slt_apbclk_sel              : 3;   // 28:30
	UINT32 swrst_usb_init              : 1;   //    31
} M14B0_CTR00_CLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300014 ctr01_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 te_ahclk_sel                : 3;   //  1: 3
	UINT32                             : 1;   //     4 reserved
	UINT32 te_teclk_sel                : 3;   //  5: 7
	UINT32 ve_aclk_sel                 : 3;   //  8:10
	UINT32 ve270clk_sel                : 3;   // 11:13
	UINT32 veclk_sel                   : 3;   // 14:16
	UINT32 sive_apbclk_sel             : 3;   // 17:19
	UINT32 gpu1clk_off_sel             : 1;   //    20
	UINT32 gpuclk_sel                  : 3;   // 21:23
	UINT32                             : 1;   //    24 reserved
	UINT32 gpu_apbclk_sel              : 3;   // 25:27
	UINT32 gpu0clk_off_sel             : 1;   //    28
	UINT32 gfx_clk_sel                 : 3;   // 29:31
} M14B0_CTR01_CLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300018 ctr02_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 2;   //  0: 1 reserved
	UINT32 vd_declk_sel                : 3;   //  2: 4
	UINT32 vd_teclk_sel                : 3;   //  5: 7
	UINT32 vd_aclk_sel                 : 3;   //  8:10
	UINT32 vd2clk_sel                  : 3;   // 11:13
	UINT32 vd1clk_sel                  : 3;   // 14:16
	UINT32 vdclk_sel                   : 3;   // 17:19
	UINT32 vd_apbclk_sel               : 3;   // 20:22
	UINT32 icod_aclk_sel               : 3;   // 23:25
	UINT32 icodclk_sel                 : 3;   // 26:28
	UINT32 te_aclk_sel                 : 3;   // 29:31
} M14B0_CTR02_CLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd30001c ctr03_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 2;   //  0: 1 reserved
	UINT32 f0_aclk_sel                 : 3;   //  2: 4
	UINT32 flexnoc2_aclk_sel           : 3;   //  5: 7
	UINT32 flexnoc1_aclk_sel           : 3;   //  8:10
	UINT32 flexnoc0_aclk_sel           : 3;   // 11:13
	UINT32 aud_veclk_sel               : 3;   // 14:16
	UINT32 aud_teclk_sel               : 3;   // 17:19
	UINT32 aud_apbclk_sel              : 3;   // 20:22
	UINT32 aud_aclk_sel                : 3;   // 23:25
	UINT32 aud_dsp1aclk_sel            : 3;   // 26:28
	UINT32 aud_dsp0aclk_sel            : 3;   // 29:31
} M14B0_CTR03_CLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300020 ctr04_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 8;   //  0: 7 reserved
	UINT32 at2_apbclk_sel              : 3;   //  8:10
	UINT32 at1_apbclk_sel              : 3;   // 11:13
	UINT32 at0_apbclk_sel              : 3;   // 14:16
	UINT32 s2_aclk_sel                 : 3;   // 17:19
	UINT32 s1_aclk_sel                 : 3;   // 20:22
	UINT32 s0_aclk_sel                 : 3;   // 23:25
	UINT32 f2_aclk_sel                 : 3;   // 26:28
	UINT32 f1_aclk_sel                 : 3;   // 29:31
} M14B0_CTR04_CLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300024 ctr05_swrs_of_de ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 13;  //  0:12 reserved
	UINT32 swrst_def_vcr_de_dp         : 1;   //    13
	UINT32 swrst_def_mif_de_dp         : 1;   //    14
	UINT32 swrst_def_mif_de            : 1;   //    15
	UINT32 swrst_def_mif_dea           : 1;   //    16
	UINT32 swrst_dee_de_dp             : 1;   //    17
	UINT32 swrst_dee_de                : 1;   //    18
	UINT32 swrst_ded_de_dp             : 1;   //    19
	UINT32 swrst_ded_de                : 1;   //    20
	UINT32 swrst_ded_dea_dp            : 1;   //    21
	UINT32 swrst_ded_dea               : 1;   //    22
	UINT32 swrst_dec_ssc_de            : 1;   //    23
	UINT32 swrst_dec_pe0_de            : 1;   //    24
	UINT32 swrst_dec_mif_de            : 1;   //    25
	UINT32 swrst_dec_mif_dea           : 1;   //    26
	UINT32 swrst_deb_de_dp             : 1;   //    27
	UINT32 swrst_deb_de                : 1;   //    28
	UINT32 swrst_deb_dea               : 1;   //    29
	UINT32 swrst_dea_de                : 1;   //    30
	UINT32 swrst_dea_dea               : 1;   //    31
} M14B0_CTR05_SWRS_OF_DE_T;

/*-----------------------------------------------------------------------------
	0xfd300028 ctr06_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 2;   //  0: 1 reserved
	UINT32 swrst_te_te1                : 1;   //     2
	UINT32 swrst_noc2                  : 1;   //     3
	UINT32 swrst_noc1                  : 1;   //     4
	UINT32 swrst_noc0                  : 1;   //     5
	UINT32 swrst_de_vd                 : 1;   //     6
	UINT32 swrst_te_vd                 : 1;   //     7
	UINT32 swrst_vda                   : 1;   //     8
	UINT32 swrst_vd1                   : 1;   //     9
	UINT32 swrst_vd2                   : 1;   //    10
	UINT32 swrst_vd                    : 1;   //    11
	UINT32 swrst_vd_apb                : 1;   //    12
	UINT32 swrst_icoda                 : 1;   //    13
	UINT32 swrst_icod                  : 1;   //    14
	UINT32 swrst_tea                   : 1;   //    15
	UINT32 swrst_teah                  : 1;   //    16
	UINT32 swrst_te_te                 : 1;   //    17
	UINT32 swrst_te_ve                 : 1;   //    18
	UINT32 swrst_vea                   : 1;   //    19
	UINT32 swrst_ve                    : 1;   //    20
	UINT32 swrst_icod_p                : 1;   //    21
	UINT32 swrst_te_apb                : 1;   //    22
	UINT32 swrst_ve_apb                : 1;   //    23
	UINT32 swrst_gpu1                  : 1;   //    24
	UINT32 swrst_gpu0                  : 1;   //    25
	UINT32 swrst_gpu_p                 : 1;   //    26
	UINT32 swrst_gfx                   : 1;   //    27
	UINT32 swrst_gfx_a                 : 1;   //    28
	UINT32 swrst_gfx_p                 : 1;   //    29
	UINT32 swrst_cvda                  : 1;   //    30
	UINT32 swrst_de_apb                : 1;   //    31
} M14B0_CTR06_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd30002c ctr07_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 24;  //  0:23 reserved
	UINT32 swrst_f2_a                  : 1;   //    24
	UINT32 swrst_f1_a                  : 1;   //    25
	UINT32 swrst_f0_a                  : 1;   //    26
	UINT32 swrst_slt_p                 : 1;   //    27
	UINT32                             : 1;   //    28 reserved
	UINT32 swrst_at2_p                 : 1;   //    29
	UINT32 swrst_at1_p                 : 1;   //    30
	UINT32 swrst_at0_p                 : 1;   //    31
} M14B0_CTR07_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd300030 ctr08_clk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 8;   //  0: 7 reserved
	UINT32 def_vcr_de_clk_sel          : 3;   //  8:10
	UINT32 def_mif_de_clk_sel          : 3;   // 11:13
	UINT32 dee_de_clk_sel              : 3;   // 14:16
	UINT32 ded_de_clk_sel              : 3;   // 17:19
	UINT32 trace_sel                   : 3;   // 20:22
	UINT32 dbgvsoc_sel                 : 3;   // 23:25
	UINT32 atvsoc_sel                  : 3;   // 26:28
	UINT32 atclk_sel                   : 3;   // 29:31
} M14B0_CTR08_CLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300034 ctr09_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 isol_en_sel                 : 3;   //  1: 3
	UINT32                             : 1;   //     4 reserved
	UINT32 pclken_sel                  : 3;   //  5: 7
	UINT32                             : 1;   //     8 reserved
	UINT32 ahbclken_sel                : 3;   //  9:11
	UINT32                             : 5;   // 12:16 reserved
	UINT32 swrst_ptmpg                 : 1;   //    17
	UINT32 swrst_ptm                   : 1;   //    18
	UINT32 swrst_prsys                 : 1;   //    19
	UINT32 swrst_prdbg                 : 1;   //    20
	UINT32 swrst_dbg1                  : 1;   //    21
	UINT32 swrst_dbg0                  : 1;   //    22
	UINT32 swrst_dbgpg                 : 1;   //    23
	UINT32                             : 7;   // 24:30 reserved
	UINT32 swrst_at                    : 1;   //    31
} M14B0_CTR09_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd300038 ctr10_ahbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 ahb7_sel                    : 3;   //  1: 3
	UINT32                             : 1;   //     4 reserved
	UINT32 ahb6_sel                    : 3;   //  5: 7
	UINT32                             : 1;   //     8 reserved
	UINT32 ahb5_sel                    : 3;   //  9:11
	UINT32                             : 1;   //    12 reserved
	UINT32 ahb4_sel                    : 3;   // 13:15
	UINT32                             : 1;   //    16 reserved
	UINT32 ahb3_sel                    : 3;   // 17:19
	UINT32                             : 1;   //    20 reserved
	UINT32 ahb2_sel                    : 3;   // 21:23
	UINT32                             : 1;   //    24 reserved
	UINT32 ahb1_sel                    : 3;   // 25:27
	UINT32                             : 1;   //    28 reserved
	UINT32 ahb0_sel                    : 3;   // 29:31
} M14B0_CTR10_AHBCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd30003c ctr11_ahbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 rmii_clkoff_sel             : 1;   //     0
	UINT32 ahb22_sel                   : 3;   //  1: 3
	UINT32                             : 1;   //     4 reserved
	UINT32 ahb21_sel                   : 3;   //  5: 7
	UINT32                             : 1;   //     8 reserved
	UINT32 ahb20_sel                   : 3;   //  9:11
	UINT32                             : 1;   //    12 reserved
	UINT32 ahb19_sel                   : 3;   // 13:15
	UINT32                             : 1;   //    16 reserved
	UINT32 ahb18_sel                   : 3;   // 17:19
	UINT32                             : 1;   //    20 reserved
	UINT32 ahb17_sel                   : 3;   // 21:23
	UINT32                             : 1;   //    24 reserved
	UINT32 ahb9_sel                    : 3;   // 25:27
	UINT32                             : 1;   //    28 reserved
	UINT32 ahb8_sel                    : 3;   // 29:31
} M14B0_CTR11_AHBCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300040 ctr12_rmiiclk_off ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 2;   //  0: 1 reserved
	UINT32 dec_ssc_de_clk_sel          : 3;   //  2: 4
	UINT32 dec_pe0_de_clk_sel          : 3;   //  5: 7
	UINT32 dec_mif_de_clk_sel          : 3;   //  8:10
	UINT32 deb_de_clk_sel              : 3;   // 11:13
	UINT32 dea_de_clk_sel              : 3;   // 14:16
	UINT32 def_mif_dea_clk_sel         : 3;   // 17:19
	UINT32 ded_dea_clk_sel             : 3;   // 20:22
	UINT32 dec_mif_dea_clk_sel         : 3;   // 23:25
	UINT32 deb_dea_clk_sel             : 3;   // 26:28
	UINT32 dea_dea_clk_sel             : 3;   // 29:31
} M14B0_CTR12_RMIICLK_OFF_T;

/*-----------------------------------------------------------------------------
	0xfd300044 ctr13_ahbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 5;   //  0: 4 reserved
	UINT32 ahb30_sel                   : 3;   //  5: 7
	UINT32                             : 1;   //     8 reserved
	UINT32 ahb29_sel                   : 3;   //  9:11
	UINT32                             : 17;  // 12:28 reserved
	UINT32 ahb24_sel                   : 3;   // 29:31
} M14B0_CTR13_AHBCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300048 ctr14_ahb_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 swrst_ahb31                 : 1;   //     0
	UINT32 swrst_ahb30                 : 1;   //     1
	UINT32 swrst_ahb29                 : 1;   //     2
	UINT32 swrst_ahb28                 : 1;   //     3
	UINT32 swrst_ahb27                 : 1;   //     4
	UINT32 swrst_ahb26                 : 1;   //     5
	UINT32 swrst_ahb25                 : 1;   //     6
	UINT32 swrst_ahb24                 : 1;   //     7
	UINT32 swrst_ahb23                 : 1;   //     8
	UINT32 swrst_ahb22                 : 1;   //     9
	UINT32 swrst_ahb21                 : 1;   //    10
	UINT32 swrst_ahb20                 : 1;   //    11
	UINT32 swrst_ahb19                 : 1;   //    12
	UINT32 swrst_ahb18                 : 1;   //    13
	UINT32 swrst_ahb17                 : 1;   //    14
	UINT32 swrst_ahb16                 : 1;   //    15
	UINT32 swrst_ahb15                 : 1;   //    16
	UINT32 swrst_ahb14                 : 1;   //    17
	UINT32 swrst_ahb13                 : 1;   //    18
	UINT32 swrst_ahb12                 : 1;   //    19
	UINT32 swrst_ahb11                 : 1;   //    20
	UINT32 swrst_ahb10                 : 1;   //    21
	UINT32 swrst_ahb9                  : 1;   //    22
	UINT32 swrst_ahb8                  : 1;   //    23
	UINT32 swrst_ahb7                  : 1;   //    24
	UINT32 swrst_ahb6                  : 1;   //    25
	UINT32 swrst_ahb5                  : 1;   //    26
	UINT32 swrst_ahb4                  : 1;   //    27
	UINT32 swrst_ahb3                  : 1;   //    28
	UINT32 swrst_ahb2                  : 1;   //    29
	UINT32 swrst_ahb1                  : 1;   //    30
	UINT32 swrst_ahb0                  : 1;   //    31
} M14B0_CTR14_AHB_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd30004c ctr15_ahb_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 26;  //  0:25 reserved
	UINT32 swrst_ahb37                 : 1;   //    26
	UINT32 swrst_ahb36                 : 1;   //    27
	UINT32 swrst_ahb35                 : 1;   //    28
	UINT32 swrst_ahb34                 : 1;   //    29
	UINT32 swrst_ahb33                 : 1;   //    30
	UINT32 swrst_ahb32                 : 1;   //    31
} M14B0_CTR15_AHB_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd300050 ctr16_busclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 bus7_sel                    : 3;   //  1: 3
	UINT32                             : 1;   //     4 reserved
	UINT32 bus6_sel                    : 3;   //  5: 7
	UINT32                             : 1;   //     8 reserved
	UINT32 bus5_sel                    : 3;   //  9:11
	UINT32                             : 1;   //    12 reserved
	UINT32 bus4_sel                    : 3;   // 13:15
	UINT32                             : 1;   //    16 reserved
	UINT32 bus3_sel                    : 3;   // 17:19
	UINT32                             : 1;   //    20 reserved
	UINT32 bus2_sel                    : 3;   // 21:23
	UINT32                             : 1;   //    24 reserved
	UINT32 bus1_sel                    : 3;   // 25:27
	UINT32                             : 1;   //    28 reserved
	UINT32 bus0_sel                    : 3;   // 29:31
} M14B0_CTR16_BUSCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300054 ctr17_busclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 21;  //  0:20 reserved
	UINT32 bus10_sel                   : 3;   // 21:23
	UINT32                             : 1;   //    24 reserved
	UINT32 bus9_sel                    : 3;   // 25:27
	UINT32                             : 1;   //    28 reserved
	UINT32 bus8_sel                    : 3;   // 29:31
} M14B0_CTR17_BUSCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300058 ctr18_reb_cpubus_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 21;  //  0:20 reserved
	UINT32 swrst_bus10                 : 1;   //    21
	UINT32 swrst_bus9                  : 1;   //    22
	UINT32 swrst_bus8                  : 1;   //    23
	UINT32 swrst_bus7                  : 1;   //    24
	UINT32 swrst_bus6                  : 1;   //    25
	UINT32 swrst_bus5                  : 1;   //    26
	UINT32 swrst_bus4                  : 1;   //    27
	UINT32 swrst_bus3                  : 1;   //    28
	UINT32 swrst_bus2                  : 1;   //    29
	UINT32 swrst_bus1                  : 1;   //    30
	UINT32 swrst_bus0                  : 1;   //    31
} M14B0_CTR18_REB_CPUBUS_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd30005c ctr19_apbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 apb0_7_sel                  : 3;   //  1: 3
	UINT32                             : 1;   //     4 reserved
	UINT32 apb0_6_sel                  : 3;   //  5: 7
	UINT32                             : 1;   //     8 reserved
	UINT32 apb0_5_sel                  : 3;   //  9:11
	UINT32                             : 1;   //    12 reserved
	UINT32 apb0_4_sel                  : 3;   // 13:15
	UINT32                             : 1;   //    16 reserved
	UINT32 apb0_3_sel                  : 3;   // 17:19
	UINT32                             : 1;   //    20 reserved
	UINT32 apb0_2_sel                  : 3;   // 21:23
	UINT32                             : 1;   //    24 reserved
	UINT32 apb0_1_sel                  : 3;   // 25:27
	UINT32                             : 1;   //    28 reserved
	UINT32 apb0_0_sel                  : 3;   // 29:31
} M14B0_CTR19_APBCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300060 ctr20_apbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 apb1_7_sel                  : 3;   //  1: 3
	UINT32                             : 1;   //     4 reserved
	UINT32 apb1_6_sel                  : 3;   //  5: 7
	UINT32                             : 1;   //     8 reserved
	UINT32 apb1_5_sel                  : 3;   //  9:11
	UINT32                             : 1;   //    12 reserved
	UINT32 apb1_4_sel                  : 3;   // 13:15
	UINT32                             : 1;   //    16 reserved
	UINT32 apb1_3_sel                  : 3;   // 17:19
	UINT32                             : 1;   //    20 reserved
	UINT32 apb1_2_sel                  : 3;   // 21:23
	UINT32                             : 1;   //    24 reserved
	UINT32 apb1_1_sel                  : 3;   // 25:27
	UINT32                             : 1;   //    28 reserved
	UINT32 apb1_0_sel                  : 3;   // 29:31
} M14B0_CTR20_APBCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300064 ctr21_apbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 apb1_15_sel                 : 3;   //  1: 3
	UINT32                             : 1;   //     4 reserved
	UINT32 apb1_14_sel                 : 3;   //  5: 7
	UINT32                             : 1;   //     8 reserved
	UINT32 apb1_13_sel                 : 3;   //  9:11
	UINT32                             : 1;   //    12 reserved
	UINT32 apb1_12_sel                 : 3;   // 13:15
	UINT32                             : 1;   //    16 reserved
	UINT32 apb1_11_sel                 : 3;   // 17:19
	UINT32                             : 1;   //    20 reserved
	UINT32 apb1_10_sel                 : 3;   // 21:23
	UINT32                             : 1;   //    24 reserved
	UINT32 apb1_9_sel                  : 3;   // 25:27
	UINT32                             : 1;   //    28 reserved
	UINT32 apb1_8_sel                  : 3;   // 29:31
} M14B0_CTR21_APBCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300068 ctr22_apbclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 17;  //  0:16 reserved
	UINT32 apb1_19_sel                 : 3;   // 17:19
	UINT32                             : 1;   //    20 reserved
	UINT32 apb1_18_sel                 : 3;   // 21:23
	UINT32                             : 1;   //    24 reserved
	UINT32 apb1_17_sel                 : 3;   // 25:27
	UINT32                             : 1;   //    28 reserved
	UINT32 apb1_16_sel                 : 3;   // 29:31
} M14B0_CTR22_APBCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd300070 ctr24_apb_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 4;   //  0: 3 reserved
	UINT32 swrst_apb1_19               : 1;   //     4
	UINT32 swrst_apb1_18               : 1;   //     5
	UINT32 swrst_apb1_17               : 1;   //     6
	UINT32 swrst_apb1_16               : 1;   //     7
	UINT32 swrst_apb1_15               : 1;   //     8
	UINT32 swrst_apb1_14               : 1;   //     9
	UINT32 swrst_apb1_13               : 1;   //    10
	UINT32 swrst_apb1_12               : 1;   //    11
	UINT32 swrst_apb1_11               : 1;   //    12
	UINT32 swrst_apb1_10               : 1;   //    13
	UINT32 swrst_apb1_9                : 1;   //    14
	UINT32 swrst_apb1_8                : 1;   //    15
	UINT32 swrst_apb1_7                : 1;   //    16
	UINT32 swrst_apb1_6                : 1;   //    17
	UINT32 swrst_apb1_5                : 1;   //    18
	UINT32 swrst_apb1_4                : 1;   //    19
	UINT32 swrst_apb1_3                : 1;   //    20
	UINT32 swrst_apb1_2                : 1;   //    21
	UINT32 swrst_apb1_1                : 1;   //    22
	UINT32 swrst_apb1_0                : 1;   //    23
	UINT32 swrst_apb0_7                : 1;   //    24
	UINT32 swrst_apb0_6                : 1;   //    25
	UINT32 swrst_apb0_5                : 1;   //    26
	UINT32 swrst_apb0_4                : 1;   //    27
	UINT32 swrst_apb0_3                : 1;   //    28
	UINT32 swrst_apb0_2                : 1;   //    29
	UINT32 swrst_apb0_1                : 1;   //    30
	UINT32 swrst_apb0_0                : 1;   //    31
} M14B0_CTR24_APB_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd300074 ctr25_de_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 19;  //  0:18 reserved
	UINT32 swrst_def_disp              : 1;   //    19
	UINT32 swrst_ded_disp              : 1;   //    20
	UINT32 swrst_decssc_disp           : 1;   //    21
	UINT32 swrst_decmif_disp           : 1;   //    22
	UINT32 swrst_deb_disp              : 1;   //    23
} M14B0_CTR25_DE_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd300078 ctr26_extclk_div_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 4;   //  0: 3 reserved
	UINT32 lvds_clk_sel                : 1;   //     4
	UINT32 dco_sel                     : 1;   //     5
	UINT32 ssclk_sel                   : 2;   //  6: 7
	UINT32                             : 2;   //  8: 9 reserved
	UINT32 msclk_sel                   : 2;   // 10:11
	UINT32 extb_sel                    : 2;   // 12:13
	UINT32 exta_sel                    : 2;   // 14:15
	UINT32                             : 6;   // 16:21 reserved
	UINT32 reg_ch3pix_clk_sel          : 1;   //    22
	UINT32 reg_ch3pix_clk_inv          : 1;   //    23
	UINT32 cvbs27_sel_inv              : 1;   //    24
	UINT32 cvbs54_sel                  : 2;   // 25:26
	UINT32 reg_se_clk_inv              : 1;   //    27
	UINT32 reg_cve27_clk_inv           : 1;   //    28
	UINT32 reg_cve54_clk_inv           : 1;   //    29
	UINT32 reg_cvd27_clk_inv           : 1;   //    30
	UINT32 reg_cvd54_clk_inv           : 1;   //    31
} M14B0_CTR26_EXTCLK_DIV_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd30007c ctr27_extclk_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ext_en_i                    : 1;   //     0
	UINT32 f27_18_sel                  : 1;   //     1
	UINT32 sc_ext_sel                  : 3;   //  2: 4
	UINT32                             : 1;   //     5 reserved
	UINT32 reg_slt_disp                : 1;   //     6
	UINT32 reg_s_te                    : 1;   //     7
	UINT32 reg_sys                     : 1;   //     8
	UINT32 reg_disp_h                  : 1;   //     9
	UINT32 reg_disp_def_p              : 1;   //    10
	UINT32 reg_disp_def                : 1;   //    11
	UINT32 reg_disp_ded_p              : 1;   //    12
	UINT32 reg_disp_ded                : 1;   //    13
	UINT32 reg_disp_ssc                : 1;   //    14
	UINT32 reg_disp_mif                : 1;   //    15
	UINT32 reg_disp_dp                 : 1;   //    16
	UINT32 reg_disp_de                 : 1;   //    17
	UINT32 reg_swrst_se                : 1;   //    18
	UINT32 reg_swrst_chb               : 1;   //    19
	UINT32 reg_swrst_cha               : 1;   //    20
	UINT32 reg_swrst_extb              : 1;   //    21
	UINT32 reg_swrst_exta              : 1;   //    22
	UINT32 reg_swrst_ch3               : 1;   //    23
	UINT32 reg_swrst_scart             : 1;   //    24
	UINT32 reg_swrst_vbi               : 1;   //    25
	UINT32 reg_swrst_cvbs27            : 1;   //    26
	UINT32 reg_swrst_cvbs54            : 1;   //    27
	UINT32 reg_swrst_cvd27             : 1;   //    28
	UINT32 reg_swrst_cvd54             : 1;   //    29
	UINT32 reg_swrst_cve27             : 1;   //    30
	UINT32 reg_swrst_cve54             : 1;   //    31
} M14B0_CTR27_EXTCLK_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd300080 ctr28_cpu_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 7;   //  0: 6 reserved
	UINT32 reg_wd1_reset               : 1;   //     7
	UINT32 reg_wd0_reset               : 1;   //     8
	UINT32 reg_neon1_reset             : 1;   //     9
	UINT32 reg_neon0_reset             : 1;   //    10
	UINT32 reg_scu_reset               : 1;   //    11
	UINT32 reg_cpu_h                   : 1;   //    12
	UINT32 reg_periph_reset            : 1;   //    13
	UINT32 reg_cpu1_reset              : 1;   //    14
	UINT32 reg_cpu0_reset              : 1;   //    15
	UINT32                             : 1;   //    16 reserved
	UINT32 reg_l2_reset                : 1;   //    17
	UINT32 a_stdby_cpu                 : 2;   // 18:19
} M14B0_CTR28_CPU_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd300084 ctr29_swrst ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ext_intr_pol_sel0           : 1;   //     0
	UINT32 ext_intr_pol_sel1           : 1;   //     1
	UINT32 ext_intr_pol_sel2           : 1;   //     2
	UINT32 ext_intr_pol_sel3           : 1;   //     3
	UINT32 cam_vcc_pol                 : 1;   //     4
	UINT32 pll_sel_core                : 1;   //     5
	UINT32 pll_sel_m2                  : 1;   //     6
	UINT32 pll_sel_m1                  : 1;   //     7
	UINT32 jtag1_sel                   : 3;   //  8:10
	UINT32 aud_jtag_no                 : 1;   //    11
	UINT32 jtag0_sel                   : 1;   //    12
	UINT32                             : 1;   //    13 reserved
	UINT32 gpu_high_speed              : 1;   //    14
	UINT32 reg_m2_q_reset              : 1;   //    15
	UINT32 reg_m2_h_reset              : 1;   //    16
	UINT32 reg_m2_reset                : 1;   //    17
	UINT32 a_stdby_m2                  : 2;   // 18:19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 reg_m1_h_reset              : 1;   //    23
	UINT32 reg_m1_reset                : 1;   //    24
	UINT32 a_stdby_m1                  : 2;   // 25:26
	UINT32 reg_m0_q_reset              : 1;   //    27
	UINT32 reg_m0_h_reset              : 1;   //    28
	UINT32 reg_m0_reset                : 1;   //    29
	UINT32 a_stdby_m0                  : 2;   // 30:31
} M14B0_CTR29_SWRST_T;

/*-----------------------------------------------------------------------------
	0xfd300088 ctr30_block_moniter ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_int_en                   : 1;   //     0
	UINT32                             : 14;  //  1:14 reserved
	UINT32 trace8bit_en                : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 cpu_mon_31_en               : 1;   //    19
	UINT32 traceclk_en                 : 1;   //    20
	UINT32 trace_en4                   : 1;   //    21
	UINT32 trace_en3                   : 1;   //    22
	UINT32 trace_en2                   : 1;   //    23
	UINT32 trace_en1                   : 1;   //    24
	UINT32 usb_mon_en2                 : 1;   //    25
	UINT32 usb_mon_en1                 : 1;   //    26
	UINT32 gfx_mon_en                  : 1;   //    27
	UINT32 aad_mon_en                  : 1;   //    28
	UINT32 aud_mon_en                  : 1;   //    29
	UINT32 cpu_mon_en                  : 1;   //    30
	UINT32 de_mon_en                   : 1;   //    31
} M14B0_CTR30_BLOCK_MONITER_T;

/*-----------------------------------------------------------------------------
	0xfd30008c ctr31_gpio_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 gp0_ren_ctrl                : 1;   //     0
	UINT32 gp1_ren_ctrl                : 1;   //     1
	UINT32 gp2_ren_ctrl                : 1;   //     2
	UINT32 gp3_ren_ctrl                : 1;   //     3
	UINT32 gp4_ren_ctrl                : 1;   //     4
	UINT32 gp5_ren_ctrl                : 1;   //     5
	UINT32 gp6_ren_ctrl                : 1;   //     6
	UINT32 gp7_ren_ctrl                : 1;   //     7
	UINT32 gp8_ren_ctrl                : 1;   //     8
	UINT32 gp9_ren_ctrl                : 1;   //     9
	UINT32 gp10_ren_ctrl               : 1;   //    10
	UINT32 gp11_ren_ctrl               : 1;   //    11
	UINT32 gp12_ren_ctrl               : 1;   //    12
	UINT32 gp13_ren_ctrl               : 1;   //    13
	UINT32 gp14_ren_ctrl               : 1;   //    14
	UINT32 gp15_ren_ctrl               : 1;   //    15
	UINT32 gp16_ren_ctrl               : 1;   //    16
	UINT32 gp17_ren_ctrl               : 1;   //    17
	UINT32 gp18_ren_ctrl               : 1;   //    18
	UINT32 gp19_ren_ctrl               : 1;   //    19
	UINT32 gp20_ren_ctrl               : 1;   //    20
	UINT32 gp21_ren_ctrl               : 1;   //    21
	UINT32 gp22_ren_ctrl               : 1;   //    22
	UINT32 gp23_ren_ctrl               : 1;   //    23
	UINT32 gp24_ren_ctrl               : 1;   //    24
	UINT32 gp25_ren_ctrl               : 1;   //    25
	UINT32 gp26_ren_ctrl               : 1;   //    26
	UINT32 gp27_ren_ctrl               : 1;   //    27
	UINT32 gp28_ren_ctrl               : 1;   //    28
	UINT32 gp29_ren_ctrl               : 1;   //    29
	UINT32 gp30_ren_ctrl               : 1;   //    30
	UINT32 gp31_ren_ctrl               : 1;   //    31
} M14B0_CTR31_GPIO_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd300090 ctr32_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 23;  //  0:22 reserved
	UINT32 stpio_en_ctrl               : 1;   //    23
	UINT32 gp4_0_mux_en                : 1;   //    24
	UINT32 gp4_1_mux_en                : 1;   //    25
	UINT32 gp4_2_mux_en                : 1;   //    26
	UINT32 gp4_3_mux_en                : 1;   //    27
	UINT32 gp4_4_mux_en                : 1;   //    28
	UINT32 gp4_5_mux_en                : 1;   //    29
	UINT32 gp4_6_mux_en                : 1;   //    30
	UINT32 gp4_7_mux_en                : 1;   //    31
} M14B0_CTR32_GPIO_MUX_ENABLE_T;

/*-----------------------------------------------------------------------------
	0xfd300098 ctr34_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 gp6_3_mux_en                : 1;   //     0
	UINT32                             : 6;   //  1: 6 reserved
	UINT32 gp6_4_mux_en                : 1;   //     7
	UINT32                             : 7;   //  8:14 reserved
	UINT32 gp6_5_mux_en                : 1;   //    15
	UINT32                             : 7;   // 16:22 reserved
	UINT32 gp6_6_mux_en                : 1;   //    23
	UINT32                             : 7;   // 24:30 reserved
	UINT32 gp6_7_mux_en                : 1;   //    31
} M14B0_CTR34_GPIO_MUX_ENABLE_T;

/*-----------------------------------------------------------------------------
	0xfd30009c ctr35_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 11;  //  0:10 reserved
	UINT32 gp7_5_mux_en                : 1;   //    11
	UINT32                             : 3;   // 12:14 reserved
	UINT32 gp7_6_mux_en                : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 gp7_7_mux_en                : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 gp6_0_mux_en                : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 gp6_1_mux_en                : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 gp6_2_mux_en                : 1;   //    31
} M14B0_CTR35_GPIO_MUX_ENABLE_T;

/*-----------------------------------------------------------------------------
	0xfd3000a0 ctr36_uart_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 10;  //  0: 9 reserved
	UINT32 rx_sel_de                   : 2;   // 10:11
	UINT32                             : 6;   // 12:17 reserved
	UINT32 rx_sel_sp                   : 2;   // 18:19
	UINT32                             : 2;   // 20:21 reserved
	UINT32 uart2_sel                   : 2;   // 22:23
	UINT32                             : 2;   // 24:25 reserved
	UINT32 uart1_sel                   : 2;   // 26:27
	UINT32                             : 2;   // 28:29 reserved
	UINT32 uart0_sel                   : 2;   // 30:31
} M14B0_CTR36_UART_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd3000a4 ctr37_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 3;   //  0: 2 reserved
	UINT32 gp8_5_mux_en                : 1;   //     3
	UINT32                             : 3;   //  4: 6 reserved
	UINT32 gp8_6_mux_en                : 1;   //     7
	UINT32                             : 3;   //  8:10 reserved
	UINT32 gp8_7_mux_en                : 1;   //    11
	UINT32                             : 3;   // 12:14 reserved
	UINT32 gp7_0_mux_en                : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 gp7_1_mux_en                : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 gp7_2_mux_en                : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 gp7_3_mux_en                : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 gp7_4_mux_en                : 1;   //    31
} M14B0_CTR37_GPIO_MUX_ENABLE_T;

/*-----------------------------------------------------------------------------
	0xfd3000a8 ctr38_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 19;  //  0:18 reserved
	UINT32 gp8_1_mux_en                : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 gp8_2_mux_en                : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 gp8_3_mux_en                : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 gp8_4_mux_en                : 1;   //    31
} M14B0_CTR38_GPIO_MUX_ENABLE_T;

/*-----------------------------------------------------------------------------
	0xfd3000ac ctr39_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 16;  //  0:15 reserved
	UINT32 sc_clk_sel                  : 1;   //    16
	UINT32 gp10_2_mux_en               : 1;   //    17
	UINT32 gp10_3_mux_en               : 1;   //    18
	UINT32 gp10_4_mux_en               : 1;   //    19
	UINT32 gp10_5_mux_en               : 1;   //    20
	UINT32 gp10_6_mux_en               : 1;   //    21
	UINT32 gp10_7_mux_en               : 1;   //    22
	UINT32 gp9_0_mux_en                : 1;   //    23
	UINT32 gp9_1_mux_en                : 1;   //    24
	UINT32 gp9_2_mux_en                : 1;   //    25
	UINT32 gp9_3_mux_en                : 1;   //    26
	UINT32 gp9_4_mux_en                : 1;   //    27
	UINT32 gp9_5_mux_en                : 1;   //    28
	UINT32 gp9_6_mux_en                : 1;   //    29
	UINT32 gp9_7_mux_en                : 1;   //    30
	UINT32 gp8_0_mux_en                : 1;   //    31
} M14B0_CTR39_GPIO_MUX_ENABLE_T;

/*-----------------------------------------------------------------------------
	0xfd3000b0 ctr40_gpu_dft_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 15;  //  0:14 reserved
	UINT32 gpu_pwrdnbypass             : 1;   //    15
	UINT32                             : 7;   // 16:22 reserved
	UINT32 gpu_testin0                 : 1;   //    23
	UINT32                             : 7;   // 24:30 reserved
	UINT32 gpu_testin1                 : 1;   //    31
} M14B0_CTR40_GPU_DFT_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000b4 ctr41_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 7;   //  0: 6 reserved
	UINT32 pull_up_ctrl7               : 1;   //     7
	UINT32                             : 3;   //  8:10 reserved
	UINT32 pull_up_ctrl6               : 1;   //    11
	UINT32                             : 3;   // 12:14 reserved
	UINT32 pull_up_ctrl5               : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 pull_up_ctrl4               : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 pull_up_ctrl3               : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 pull_up_ctrl2               : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 pull_up_ctrl1               : 1;   //    31
} M14B0_CTR41_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000b8 ctr42_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 3;   //  0: 2 reserved
	UINT32 pull_up_ctrl15              : 1;   //     3
	UINT32                             : 3;   //  4: 6 reserved
	UINT32 pull_up_ctrl14              : 1;   //     7
	UINT32                             : 3;   //  8:10 reserved
	UINT32 pull_up_ctrl13              : 1;   //    11
	UINT32                             : 3;   // 12:14 reserved
	UINT32 pull_up_ctrl12              : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 pull_up_ctrl11              : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 pull_up_ctrl10              : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 pull_up_ctrl9               : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 pull_up_ctrl8               : 1;   //    31
} M14B0_CTR42_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000bc ctr43_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 15;  //  0:14 reserved
	UINT32 pull_up_ctrl18              : 1;   //    15
	UINT32                             : 7;   // 16:22 reserved
	UINT32 pull_up_ctrl17              : 1;   //    23
	UINT32                             : 7;   // 24:30 reserved
	UINT32 pull_up_ctrl16              : 1;   //    31
} M14B0_CTR43_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000c0 ctr44_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 15;  //  0:14 reserved
	UINT32 pull_up_ctrl21              : 1;   //    15
	UINT32                             : 7;   // 16:22 reserved
	UINT32 pull_up_ctrl20              : 1;   //    23
	UINT32                             : 7;   // 24:30 reserved
	UINT32 pull_up_ctrl19              : 1;   //    31
} M14B0_CTR44_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000c4 ctr45_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 11;  //  0:10 reserved
	UINT32 pull_up_ctrl27              : 1;   //    11
	UINT32                             : 3;   // 12:14 reserved
	UINT32 pull_up_ctrl26              : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 pull_up_ctrl25              : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 pull_up_ctrl24              : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 pull_up_ctrl23              : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 pull_up_ctrl22              : 1;   //    31
} M14B0_CTR45_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000c8 ctr46_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 23;  //  0:22 reserved
	UINT32 pull_up_ctrl30              : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 pull_up_ctrl29              : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 pull_up_ctrl28              : 1;   //    31
} M14B0_CTR46_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000cc ctr47_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 3;   //  0: 2 reserved
	UINT32 pull_up_ctrl38              : 1;   //     3
	UINT32                             : 3;   //  4: 6 reserved
	UINT32 pull_up_ctrl37              : 1;   //     7
	UINT32                             : 3;   //  8:10 reserved
	UINT32 pull_up_ctrl36              : 1;   //    11
	UINT32                             : 3;   // 12:14 reserved
	UINT32 pull_up_ctrl35              : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 pull_up_ctrl34              : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 pull_up_ctrl33              : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 pull_up_ctrl32              : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 pull_up_ctrl31              : 1;   //    31
} M14B0_CTR47_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000d0 ctr48_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 11;  //  0:10 reserved
	UINT32 pull_up_ctrl44              : 1;   //    11
	UINT32                             : 3;   // 12:14 reserved
	UINT32 pull_up_ctrl43              : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 pull_up_ctrl42              : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 pull_up_ctrl41              : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 pull_up_ctrl40              : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 pull_up_ctrl39              : 1;   //    31
} M14B0_CTR48_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000d4 ctr49_pullup_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 7;   //  0: 6 reserved
	UINT32 pull_up_ctrl50              : 1;   //     7
	UINT32                             : 7;   //  8:14 reserved
	UINT32 pull_up_ctrl49              : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 pull_up_ctrl48              : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 pull_up_ctrl47              : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 pull_up_ctrl46              : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 pull_up_ctrl45              : 1;   //    31
} M14B0_CTR49_PULLUP_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000d8 ctr50_tpio_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 22;  //  0:21 reserved
	UINT32 tpio_sel_ctrl               : 1;   //    22
	UINT32 tpo_sel_ctrl                : 1;   //    23
	UINT32                             : 7;   // 24:30 reserved
	UINT32 swrst                       : 1;   //    31
} M14B0_CTR50_TPIO_SEL_T;

/*-----------------------------------------------------------------------------
	0xfd3000dc ctr51_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 19;  //  0:18 reserved
	UINT32 gp11_6_mux_en               : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 gp11_7_mux_en               : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 gp10_0_mux_en               : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 gp10_1_mux_en               : 1;   //    31
} M14B0_CTR51_GPIO_MUX_ENABLE_T;

/*-----------------------------------------------------------------------------
	0xfd3000e0 ctr52_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_aud_dacrlrch             : 1;   //     0
	UINT32                             : 10;  //  1:10 reserved
	UINT32 gp11_0_mux_en               : 1;   //    11
	UINT32                             : 3;   // 12:14 reserved
	UINT32 gp11_1_mux_en               : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 gp11_2_mux_en               : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 gp11_3_mux_en               : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 gp11_4_mux_en               : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 gp11_5_mux_en               : 1;   //    31
} M14B0_CTR52_GPIO_MUX_ENABLE_T;

/*-----------------------------------------------------------------------------
	0xfd3000e4 ctr53_gpio_mux_enable ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 3;   //  0: 2 reserved
	UINT32 gp5_0_mux_en                : 1;   //     3
	UINT32                             : 3;   //  4: 6 reserved
	UINT32 gp5_1_mux_en                : 1;   //     7
	UINT32                             : 3;   //  8:10 reserved
	UINT32 gp5_2_mux_en                : 1;   //    11
	UINT32                             : 3;   // 12:14 reserved
	UINT32 gp5_3_mux_en                : 1;   //    15
	UINT32                             : 3;   // 16:18 reserved
	UINT32 gp5_4_mux_en                : 1;   //    19
	UINT32                             : 3;   // 20:22 reserved
	UINT32 gp5_5_mux_en                : 1;   //    23
	UINT32                             : 3;   // 24:26 reserved
	UINT32 gp5_6_mux_en                : 1;   //    27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 gp5_7_mux_en                : 1;   //    31
} M14B0_CTR53_GPIO_MUX_ENABLE_T;

/*-----------------------------------------------------------------------------
	0xfd3000e8 ctr54_bt_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dftclk_ctrl                 : 1;   //     0
	UINT32                             : 14;  //  1:14 reserved
	UINT32 pd                          : 1;   //    15
	UINT32                             : 4;   // 16:19 reserved
	UINT32 bt_testclk0_ctrl            : 1;   //    20
	UINT32 bt_txpreemphasistune0_ctrl  : 2;   // 21:22
	UINT32 bt_txrisetune0_ctrl         : 1;   //    23
	UINT32 bt_commononn_ctrl           : 1;   //    24
	UINT32 bt_refclksel_ctrl           : 2;   // 25:26
	UINT32 bt_refclkdiv_ctrl           : 2;   // 27:28
	UINT32 bt_testen_en_ctrl           : 1;   //    29
	UINT32 bt_vatestenb_en_ctrl        : 1;   //    30
	UINT32 bt_siddq_en_ctrl            : 1;   //    31
} M14B0_CTR54_BT_USB_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000ec ctr55_rgg_sdec_dco_fcw_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_dco_fcw_ctrl             : 22;  //  0:21
} M14B0_CTR55_RGG_SDEC_DCO_FCW_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000f0 ctr56_bt_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 28;  //  0:27 reserved
	UINT32 testaddr                    : 4;   // 28:31
} M14B0_CTR56_BT_USB_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000f4 ctr57_bt_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 3;   //  0: 2 reserved
	UINT32 testdatain                  : 8;   //  3:10
	UINT32 testdataoutsel              : 1;   //    11
	UINT32 txvreftune                  : 4;   // 12:15
	UINT32 txfslstune                  : 4;   // 16:19
	UINT32 vregtune                    : 1;   //    20
	UINT32 txhsxvtune                  : 2;   // 21:22
	UINT32 sqrxtune                    : 3;   // 23:25
	UINT32 compdistune                 : 3;   // 26:28
	UINT32 loopbackenb                 : 1;   //    29
	UINT32 otgdisable                  : 1;   //    30
	UINT32 portreset                   : 1;   //    31
} M14B0_CTR57_BT_USB_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000f8 ctr58_main_0_1_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 16;  //  0:15 reserved
	UINT32 testclk1_ctrl               : 1;   //    16
	UINT32 txpreemphasistune1_ctrl     : 2;   // 17:18
	UINT32 txrisetune1_ctrl            : 1;   //    19
	UINT32 testclk0_ctrl               : 1;   //    20
	UINT32 txpreemphasistune0_ctrl     : 2;   // 21:22
	UINT32 txrisetune0_ctrl            : 1;   //    23
	UINT32 commononn_ctrl              : 1;   //    24
	UINT32 refclksel_ctrl              : 2;   // 25:26
	UINT32 refclkdiv_ctrl              : 2;   // 27:28
	UINT32 testen_en_ctrl              : 1;   //    29
	UINT32 vatestenb_en_ctrl           : 1;   //    30
	UINT32 siddq_en_ctrl               : 1;   //    31
} M14B0_CTR58_MAIN_0_1_USB_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3000fc ctr59_bt_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 28;  //  0:27 reserved
	UINT32 testaddr1                   : 4;   // 28:31
} M14B0_CTR59_BT_USB_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd300100 ctr60_main_0_1_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 testdatain1                 : 8;   //  0: 7
	UINT32 testdataoutsel1             : 1;   //     8
	UINT32 txvreftune1                 : 4;   //  9:12
	UINT32 txfslstune1                 : 4;   // 13:16
	UINT32 vregtune1                   : 1;   //    17
	UINT32 txhsxvtune1                 : 2;   // 18:19
	UINT32 sqrxtune1                   : 3;   // 20:22
	UINT32 compdistune1                : 3;   // 23:25
	UINT32 loopbackenb1                : 1;   //    26
	UINT32 portreset1                  : 1;   //    27
	UINT32 testaddr0                   : 4;   // 28:31
} M14B0_CTR60_MAIN_0_1_USB_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd300104 ctr61_main_0_1_usb_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 3;   //  0: 2 reserved
	UINT32 testdatain0                 : 8;   //  3:10
	UINT32 testdataoutsel0             : 1;   //    11
	UINT32 txvreftune0                 : 4;   // 12:15
	UINT32 txfslstune0                 : 4;   // 16:19
	UINT32 vregtune0                   : 1;   //    20
	UINT32 txhsxvtune0                 : 2;   // 21:22
	UINT32 sqrxtune0                   : 3;   // 23:25
	UINT32 compdistune0                : 3;   // 26:28
	UINT32 loopbackenb0                : 1;   //    29
	UINT32 otgdisable0                 : 1;   //    30
	UINT32 portreset0                  : 1;   //    31
} M14B0_CTR61_MAIN_0_1_USB_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd300108 ctr62_display_pll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 28;  //  0:27 reserved
	UINT32 disp_pre_fd_ctrl            : 2;   // 28:29
	UINT32 disp_od_fout_ctrl           : 2;   // 30:31
} M14B0_CTR62_DISPLAY_PLL_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd30010c ctr63_display_pll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 disp_udex_ctrl              : 5;   //  0: 4
	UINT32 disp_msex_ctrl              : 9;   //  5:13
	UINT32 disp_nsc_ctrl               : 4;   // 14:17
	UINT32 disp_npc_ctrl               : 6;   // 18:23
	UINT32 disp_m_ctrl                 : 3;   // 24:26
	UINT32 disp_cih_ctrl               : 3;   // 27:29
	UINT32 disp_lf_ctrl                : 1;   //    30
	UINT32 disp_pdb_ctrl               : 1;   //    31
} M14B0_CTR63_DISPLAY_PLL_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001bc ctr64_dco_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 18;  //  0:17 reserved
	UINT32 sd_dco_g_ctrl               : 3;   // 18:20
	UINT32 sd_dco_lpf_ctrl             : 2;   // 21:22
	UINT32 sd_dco_resetb_ctrl          : 1;   //    23
	UINT32 sd_dco_rom_test_ctrl        : 1;   //    24
	UINT32 de_dco_g_ctrl               : 3;   // 25:27
	UINT32 de_dco_lpf_ctrl             : 2;   // 28:29
	UINT32 de_dco_resetb_ctrl          : 1;   //    30
	UINT32 de_dco_rom_test_ctrl        : 1;   //    31
} M14B0_CTR64_DCO_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001c4 ctr66_main2_syspll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 24;  //  0:23 reserved
	UINT32 m2_dr3p_pre_fd_ctrl         : 2;   // 24:25
	UINT32 m2_dr3p_od_fout_ctrl        : 2;   // 26:27
	UINT32 m2_dr3p_dtr_sel_ctrl        : 4;   // 28:31
} M14B0_CTR66_MAIN2_SYSPLL_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001c8 ctr67_main2_syspll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m2_dr3p_udex_ctrl           : 5;   //  0: 4
	UINT32 m2_dr3p_msex_ctrl           : 9;   //  5:13
	UINT32 m2_dr3p_nsc_ctrl            : 4;   // 14:17
	UINT32 m2_dr3p_npc_ctrl            : 6;   // 18:23
	UINT32 m2_dr3p_m_ctrl              : 3;   // 24:26
	UINT32 m2_dr3p_cih_ctrl            : 3;   // 27:29
	UINT32 m2_dr3p_lf_ctrl             : 1;   //    30
	UINT32 m2_dr3p_pdb_ctrl            : 1;   //    31
} M14B0_CTR67_MAIN2_SYSPLL_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001cc ctr68_main2_syspll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 24;  //  0:23 reserved
	UINT32 m1_dr3p_pre_fd_ctrl         : 2;   // 24:25
	UINT32 m1_dr3p_od_fout_ctrl        : 2;   // 26:27
	UINT32 m1_dr3p_dtr_sel_ctrl        : 4;   // 28:31
} M14B0_CTR68_MAIN2_SYSPLL_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001d0 ctr69_main2_syspll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m1_dr3p_udex_ctrl           : 5;   //  0: 4
	UINT32 m1_dr3p_msex_ctrl           : 9;   //  5:13
	UINT32 m1_dr3p_nsc_ctrl            : 4;   // 14:17
	UINT32 m1_dr3p_npc_ctrl            : 6;   // 18:23
	UINT32 m1_dr3p_m_ctrl              : 3;   // 24:26
	UINT32 m1_dr3p_cih_ctrl            : 3;   // 27:29
	UINT32 m1_dr3p_lf_ctrl             : 1;   //    30
	UINT32 m1_dr3p_pdb_ctrl            : 1;   //    31
} M14B0_CTR69_MAIN2_SYSPLL_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001d4 ctr70_cpupll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 24;  //  0:23 reserved
	UINT32 c_dr3p_pre_fd_ctrl          : 2;   // 24:25
	UINT32 c_dr3p_od_fout_ctrl         : 2;   // 26:27
	UINT32 c_dr3p_dtr_sel_ctrl         : 4;   // 28:31
} M14B0_CTR70_CPUPLL_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001d8 ctr71_cpupll_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_dr3p_udex_ctrl            : 5;   //  0: 4
	UINT32 c_dr3p_msex_ctrl            : 9;   //  5:13
	UINT32 c_dr3p_nsc_ctrl             : 4;   // 14:17
	UINT32 c_dr3p_npc_ctrl             : 6;   // 18:23
	UINT32 c_dr3p_m_ctrl               : 3;   // 24:26
	UINT32 c_dr3p_cih_ctrl             : 3;   // 27:29
	UINT32 c_dr3p_lf_ctrl              : 1;   //    30
	UINT32 c_dr3p_pdb_ctrl             : 1;   //    31
} M14B0_CTR71_CPUPLL_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001dc ctr72_hslvds_tx_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 lv_flip_en_ctrl             : 12;  //  0:11
	UINT32                             : 6;   // 12:17 reserved
	UINT32 lv_pemp_te2_ctrl            : 2;   // 18:19
	UINT32 lv_pemp_td2_ctrl            : 2;   // 20:21
	UINT32 lv_pemp_tclk2_ctrl          : 2;   // 22:23
	UINT32 lv_pemp_tc2_ctrl            : 2;   // 24:25
	UINT32 lv_pemp_tb2_ctrl            : 2;   // 26:27
	UINT32 lv_pemp_ta2_ctrl            : 2;   // 28:29
	UINT32 lv_pemp_te1_ctrl            : 2;   // 30:31
} M14B0_CTR72_HSLVDS_TX_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001e0 ctr73_hslvds_tx_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 lv_pemp_td1_ctrl            : 2;   //  1: 2
	UINT32 lv_pemp_tclk1_ctrl          : 2;   //  3: 4
	UINT32 lv_pemp_tc1_ctrl            : 2;   //  5: 6
	UINT32 lv_pemp_tb1_ctrl            : 2;   //  7: 8
	UINT32 lv_pemp_ta1_ctrl            : 2;   //  9:10
	UINT32                             : 1;   //    11 reserved
	UINT32 lv_rf_ctrl                  : 1;   //    12
	UINT32 lv_rmlvds_en_ctrl           : 1;   //    13
	UINT32 lv_swc_te2_ctrl             : 3;   // 14:16
	UINT32 lv_swc_td2_ctrl             : 3;   // 17:19
	UINT32 lv_swc_tclk2_ctrl           : 3;   // 20:22
	UINT32 lv_swc_tc2_ctrl             : 3;   // 23:25
	UINT32 lv_swc_tb2_ctrl             : 3;   // 26:28
	UINT32 lv_swc_ta2_ctrl             : 3;   // 29:31
} M14B0_CTR73_HSLVDS_TX_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001e4 ctr74_hslvds_tx_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 1;   //     0 reserved
	UINT32 lv_swc_te1_ctrl             : 3;   //  1: 3
	UINT32 lv_swc_td1_ctrl             : 3;   //  4: 6
	UINT32 lv_swc_tclk1_ctrl           : 3;   //  7: 9
	UINT32 lv_swc_tc1_ctrl             : 3;   // 10:12
	UINT32 lv_swc_tb1_ctrl             : 3;   // 13:15
	UINT32 lv_swc_ta1_ctrl             : 3;   // 16:18
	UINT32 lv_pdb_ch_ctrl              : 12;  // 19:30
	UINT32 lv_pdb_ctrl                 : 1;   //    31
} M14B0_CTR74_HSLVDS_TX_CTRL_T;

/*-----------------------------------------------------------------------------
	0xfd3001e8 ctr75_hslvds_rx_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 5;   //  0: 4 reserved
	UINT32 link2_ch_swap               : 3;   //  5: 7
	UINT32                             : 5;   //  8:12 reserved
	UINT32 link1_ch_swap               : 3;   // 13:15
	UINT32                             : 5;   // 16:20 reserved
	UINT32 rx1_sel_speed_en            : 1;   //    21
	UINT32 rx0_sel_speed_en            : 1;   //    22
	UINT32                             : 1;   //    23 reserved
	UINT32 rx1_sel_8b10b_en            : 1;   //    24
	UINT32 rx0_sel_8b10b_en            : 1;   //    25
	UINT32                             : 1;   //    26 reserved
	UINT32 rx1_rf_en                   : 1;   //    27
	UINT32 rx0_rf_en                   : 1;   //    28
	UINT32                             : 1;   //    29 reserved
	UINT32 rx1_pdb_en                  : 1;   //    30
	UINT32 rx0_pdb_en                  : 1;   //    31
} M14B0_CTR75_HSLVDS_RX_CTRL_T;

typedef struct {
	M14B0_CTR00_CLK_DIV_SEL_T               ctr00_clk_div_sel;                       // 0xfd300010
	M14B0_CTR01_CLK_DIV_SEL_T               ctr01_clk_div_sel;                       // 0xfd300014
	M14B0_CTR02_CLK_DIV_SEL_T               ctr02_clk_div_sel;                       // 0xfd300018
	M14B0_CTR03_CLK_DIV_SEL_T               ctr03_clk_div_sel;                       // 0xfd30001c
	M14B0_CTR04_CLK_DIV_SEL_T               ctr04_clk_div_sel;                       // 0xfd300020
	M14B0_CTR05_SWRS_OF_DE_T                ctr05_swrs_of_de;                        // 0xfd300024
	M14B0_CTR06_SWRST_T                     ctr06_swrst;                             // 0xfd300028
	M14B0_CTR07_SWRST_T                     ctr07_swrst;                             // 0xfd30002c
	M14B0_CTR08_CLK_DIV_SEL_T               ctr08_clk_div_sel;                       // 0xfd300030
	M14B0_CTR09_SWRST_T                     ctr09_swrst;                             // 0xfd300034
	M14B0_CTR10_AHBCLK_DIV_SEL_T            ctr10_ahbclk_div_sel;                    // 0xfd300038
	M14B0_CTR11_AHBCLK_DIV_SEL_T            ctr11_ahbclk_div_sel;                    // 0xfd30003c
	M14B0_CTR12_RMIICLK_OFF_T               ctr12_rmiiclk_off;                       // 0xfd300040
	M14B0_CTR13_AHBCLK_DIV_SEL_T            ctr13_ahbclk_div_sel;                    // 0xfd300044
	M14B0_CTR14_AHB_SWRST_T                 ctr14_ahb_swrst;                         // 0xfd300048
	M14B0_CTR15_AHB_SWRST_T                 ctr15_ahb_swrst;                         // 0xfd30004c
	M14B0_CTR16_BUSCLK_DIV_SEL_T            ctr16_busclk_div_sel;                    // 0xfd300050
	M14B0_CTR17_BUSCLK_DIV_SEL_T            ctr17_busclk_div_sel;                    // 0xfd300054
	M14B0_CTR18_REB_CPUBUS_SWRST_T          ctr18_reb_cpubus_swrst;                  // 0xfd300058
	M14B0_CTR19_APBCLK_DIV_SEL_T            ctr19_apbclk_div_sel;                    // 0xfd30005c
	M14B0_CTR20_APBCLK_DIV_SEL_T            ctr20_apbclk_div_sel;                    // 0xfd300060
	M14B0_CTR21_APBCLK_DIV_SEL_T            ctr21_apbclk_div_sel;                    // 0xfd300064
	M14B0_CTR22_APBCLK_DIV_SEL_T            ctr22_apbclk_div_sel;                    // 0xfd300068
	UINT32                                  reserved0;                               // 0xfd30006c
	M14B0_CTR24_APB_SWRST_T                 ctr24_apb_swrst;                         // 0xfd300070
	M14B0_CTR25_DE_SWRST_T                  ctr25_de_swrst;                          // 0xfd300074
	M14B0_CTR26_EXTCLK_DIV_SEL_T            ctr26_extclk_div_sel;                    // 0xfd300078
	M14B0_CTR27_EXTCLK_SWRST_T              ctr27_extclk_swrst;                      // 0xfd30007c
	M14B0_CTR28_CPU_SWRST_T                 ctr28_cpu_swrst;                         // 0xfd300080
	M14B0_CTR29_SWRST_T                     ctr29_swrst;                             // 0xfd300084
	M14B0_CTR30_BLOCK_MONITER_T             ctr30_block_moniter;                     // 0xfd300088
	M14B0_CTR31_GPIO_PULLUP_CTRL_T          ctr31_gpio_pullup_ctrl;                  // 0xfd30008c
	M14B0_CTR32_GPIO_MUX_ENABLE_T           ctr32_gpio_mux_enable;                   // 0xfd300090
	UINT32                                  reserved1;                               // 0xfd300094
	M14B0_CTR34_GPIO_MUX_ENABLE_T           ctr34_gpio_mux_enable;                   // 0xfd300098
	M14B0_CTR35_GPIO_MUX_ENABLE_T           ctr35_gpio_mux_enable;                   // 0xfd30009c
	M14B0_CTR36_UART_SEL_T                  ctr36_uart_sel;                          // 0xfd3000a0
	M14B0_CTR37_GPIO_MUX_ENABLE_T           ctr37_gpio_mux_enable;                   // 0xfd3000a4
	M14B0_CTR38_GPIO_MUX_ENABLE_T           ctr38_gpio_mux_enable;                   // 0xfd3000a8
	M14B0_CTR39_GPIO_MUX_ENABLE_T           ctr39_gpio_mux_enable;                   // 0xfd3000ac
	M14B0_CTR40_GPU_DFT_CTRL_T              ctr40_gpu_dft_ctrl;                      // 0xfd3000b0
	M14B0_CTR41_PULLUP_CTRL_T               ctr41_pullup_ctrl;                       // 0xfd3000b4
	M14B0_CTR42_PULLUP_CTRL_T               ctr42_pullup_ctrl;                       // 0xfd3000b8
	M14B0_CTR43_PULLUP_CTRL_T               ctr43_pullup_ctrl;                       // 0xfd3000bc
	M14B0_CTR44_PULLUP_CTRL_T               ctr44_pullup_ctrl;                       // 0xfd3000c0
	M14B0_CTR45_PULLUP_CTRL_T               ctr45_pullup_ctrl;                       // 0xfd3000c4
	M14B0_CTR46_PULLUP_CTRL_T               ctr46_pullup_ctrl;                       // 0xfd3000c8
	M14B0_CTR47_PULLUP_CTRL_T               ctr47_pullup_ctrl;                       // 0xfd3000cc
	M14B0_CTR48_PULLUP_CTRL_T               ctr48_pullup_ctrl;                       // 0xfd3000d0
	M14B0_CTR49_PULLUP_CTRL_T               ctr49_pullup_ctrl;                       // 0xfd3000d4
	M14B0_CTR50_TPIO_SEL_T                  ctr50_tpio_sel;                          // 0xfd3000d8
	M14B0_CTR51_GPIO_MUX_ENABLE_T           ctr51_gpio_mux_enable;                   // 0xfd3000dc
	M14B0_CTR52_GPIO_MUX_ENABLE_T           ctr52_gpio_mux_enable;                   // 0xfd3000e0
	M14B0_CTR53_GPIO_MUX_ENABLE_T           ctr53_gpio_mux_enable;                   // 0xfd3000e4
	M14B0_CTR54_BT_USB_CTRL_T               ctr54_bt_usb_ctrl;                       // 0xfd3000e8
	M14B0_CTR55_RGG_SDEC_DCO_FCW_CTRL_T     ctr55_rgg_sdec_dco_fcw_ctrl;             // 0xfd3000ec
	M14B0_CTR56_BT_USB_CTRL_T               ctr56_bt_usb_ctrl;                       // 0xfd3000f0
	M14B0_CTR57_BT_USB_CTRL_T               ctr57_bt_usb_ctrl;                       // 0xfd3000f4
	M14B0_CTR58_MAIN_0_1_USB_CTRL_T         ctr58_main_0_1_usb_ctrl;                 // 0xfd3000f8
	M14B0_CTR59_BT_USB_CTRL_T               ctr59_bt_usb_ctrl;                       // 0xfd3000fc
	M14B0_CTR60_MAIN_0_1_USB_CTRL_T         ctr60_main_0_1_usb_ctrl;                 // 0xfd300100
	M14B0_CTR61_MAIN_0_1_USB_CTRL_T         ctr61_main_0_1_usb_ctrl;                 // 0xfd300104
	M14B0_CTR62_DISPLAY_PLL_CTRL_T          ctr62_display_pll_ctrl;                  // 0xfd300108
	M14B0_CTR63_DISPLAY_PLL_CTRL_T          ctr63_display_pll_ctrl;                  // 0xfd30010c
	UINT32                                  reserved2[43];                           // 0xfd300110~0xfd3001b8
	M14B0_CTR64_DCO_CTRL_T                  ctr64_dco_ctrl;                          // 0xfd3001bc
	UINT32                                  reserved3;                               // 0xfd3001c0
	M14B0_CTR66_MAIN2_SYSPLL_CTRL_T         ctr66_main2_syspll_ctrl;                 // 0xfd3001c4
	M14B0_CTR67_MAIN2_SYSPLL_CTRL_T         ctr67_main2_syspll_ctrl;                 // 0xfd3001c8
	M14B0_CTR68_MAIN2_SYSPLL_CTRL_T         ctr68_main2_syspll_ctrl;                 // 0xfd3001cc
	M14B0_CTR69_MAIN2_SYSPLL_CTRL_T         ctr69_main2_syspll_ctrl;                 // 0xfd3001d0
	M14B0_CTR70_CPUPLL_CTRL_T               ctr70_cpupll_ctrl;                       // 0xfd3001d4
	M14B0_CTR71_CPUPLL_CTRL_T               ctr71_cpupll_ctrl;                       // 0xfd3001d8
	M14B0_CTR72_HSLVDS_TX_CTRL_T            ctr72_hslvds_tx_ctrl;                    // 0xfd3001dc
	M14B0_CTR73_HSLVDS_TX_CTRL_T            ctr73_hslvds_tx_ctrl;                    // 0xfd3001e0
	M14B0_CTR74_HSLVDS_TX_CTRL_T            ctr74_hslvds_tx_ctrl;                    // 0xfd3001e4
	M14B0_CTR75_HSLVDS_RX_CTRL_T            ctr75_hslvds_rx_ctrl;                    // 0xfd3001e8
} CTOP_CTRL_REG_M14B0_T;

#endif
