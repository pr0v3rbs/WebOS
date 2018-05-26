#ifndef _REG_CTOP_CTRL_L9A0_h
#define _REG_CTOP_CTRL_L9A0_h

typedef struct { // 0xfd300010
	UINT32                             : 1;   //     0
	UINT32 gfx_aclk_sel                : 3;   //  3: 1
	UINT32                             : 1;   //     4
	UINT32 gfx_pclk_sel                : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 de_de_clk_sel               : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 cvda_clk_sel                : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 dea_clk_sel                 : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 de_apb_clk_sel              : 3;   // 23:21
	UINT32                             : 3;   // 26:24
	UINT32 swrst_usb_bt_init           : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 swrst_usb_init              : 1;   //    31
} L9A0_CTR00_T;

typedef struct { // 0xfd300014
	UINT32                             : 1;   //     0
	UINT32 te_ahclk_sel                : 3;   //  3: 1
	UINT32                             : 1;   //     4
	UINT32 te_teclk_sel                : 3;   //  7: 5
	UINT32 ve_aclk_sel                 : 3;   // 10: 8
	UINT32 ve270clk_sel                : 3;   // 13:11
	UINT32 veclk_sel                   : 3;   // 16:14
	UINT32 sive_apbclk_sel             : 3;   // 19:17
	UINT32 gpuclk_off_sel              : 1;   //    20
	UINT32 gpuclk_sel                  : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 gpu_apbclk_sel              : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 gfx_clk_sel                 : 3;   // 31:29
} L9A0_CTR01_T;

typedef struct { // 0xfd300018
	UINT32                             : 1;   //     0
	UINT32 vd_declk_sel                : 3;   //  3: 1
	UINT32                             : 1;   //     4
	UINT32 vd_teclk_sel                : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 vd_aclk_sel                 : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 vdclk_sel                   : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 vd_apbclk_sel               : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 icod_aclk_sel               : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 icodclk_sel                 : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 te_aclk_sel                 : 3;   // 31:29
} L9A0_CTR02_T;

typedef struct { // 0xfd30001c
	UINT32                             : 1;   //     0
	UINT32 flexnoc1_aclk_sel           : 3;   //  3: 1
	UINT32                             : 1;   //     4
	UINT32 flexnoc0_aclk_sel           : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 aud_veclk_sel               : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 aud_teclk_sel               : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 aud_apbclk_sel              : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 aud_aclk_sel                : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 aud_dsp1aclk_sel            : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 aud_dsp0aclk_sel            : 3;   // 31:29
} L9A0_CTR03_T;

typedef struct { // 0xfd300020
	UINT32 at2_apbclk_sel              : 3;   //  2: 0
	UINT32                             : 1;   //     3
	UINT32 at1_apbclk_sel              : 3;   //  6: 4
	UINT32                             : 1;   //     7
	UINT32 at0_apbclk_sel              : 3;   // 10: 8
	UINT32 s2_aclk_sel                 : 3;   // 13:11
	UINT32 s1_aclk_sel                 : 3;   // 16:14
	UINT32 s0_aclk_sel                 : 3;   // 19:17
	UINT32 f2_aclk_sel                 : 3;   // 22:20
	UINT32 f1_aclk_sel                 : 3;   // 25:23
	UINT32 f0_aclk_sel                 : 3;   // 28:26
	UINT32 flexnoc2_aclk_sel           : 3;   // 31:29
} L9A0_CTR04_T;

typedef struct { // 0xfd300024
	UINT32                             : 29;  // 28: 0
	UINT32 slt_apbclk_sel              : 3;   // 31:29
} L9A0_CTR05_T;

typedef struct { // 0xfd300028
	UINT32                             : 2;   //  1: 0
	UINT32 swrst_noc2                  : 1;   //     2
	UINT32 swrst_noc1                  : 1;   //     3
	UINT32 swrst_noc0                  : 1;   //     4
	UINT32 swrst_de_vd                 : 1;   //     5
	UINT32 swrst_te_vd                 : 1;   //     6
	UINT32 swrst_vda                   : 1;   //     7
	UINT32 swrst_vd                    : 1;   //     8
	UINT32 swrst_vd_apb                : 1;   //     9
	UINT32 swrst_icoda                 : 1;   //    10
	UINT32 swrst_icod                  : 1;   //    11
	UINT32 swrst_tea                   : 1;   //    12
	UINT32 swrst_teah                  : 1;   //    13
	UINT32 swrst_te_te                 : 1;   //    14
	UINT32 swrst_te_ve                 : 1;   //    15
	UINT32 swrst_vea                   : 1;   //    16
	UINT32 swrst_ve                    : 1;   //    17
	UINT32 swrst_icod_p                : 1;   //    18
	UINT32 swrst_te_apb                : 1;   //    19
	UINT32 swrst_ve_apb                : 1;   //    20
	UINT32 swrst_gpu                   : 1;   //    21
	UINT32 swrst_gpu_p                 : 1;   //    22
	UINT32 swrst_gfx                   : 1;   //    23
	UINT32 swrst_gfx_a                 : 1;   //    24
	UINT32 swrst_gfx_p                 : 1;   //    25
	UINT32 swrst_de_dp                 : 1;   //    26
	UINT32 swrst_de_de                 : 1;   //    27
	UINT32 swrst_cvda                  : 1;   //    28
	UINT32 swrst_dea_dp                : 1;   //    29
	UINT32 swrst_dea                   : 1;   //    30
	UINT32 swrst_de_apb                : 1;   //    31
} L9A0_CTR06_T;

typedef struct { // 0xfd30002c
	UINT32                             : 24;  // 23: 0
	UINT32 swrst_f2_a                  : 1;   //    24
	UINT32 swrst_f1_a                  : 1;   //    25
	UINT32 swrst_f0_a                  : 1;   //    26
	UINT32 swrst_slt_p                 : 1;   //    27
	UINT32                             : 1;   //    28
	UINT32 swrst_at2_p                 : 1;   //    29
	UINT32 swrst_at1_p                 : 1;   //    30
	UINT32 swrst_at0_p                 : 1;   //    31
} L9A0_CTR07_T;

typedef struct { // 0xfd300030
	UINT32                             : 17;  // 16: 0
	UINT32 trace_sel                   : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 dbgvsoc_sel                 : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 atvsoc_sel                  : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 atclk_sel                   : 3;   // 31:29
} L9A0_CTR08_T;

typedef struct { // 0xfd300034
	UINT32                             : 5;   //  4: 0
	UINT32 pclken_sel                  : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 ahbclken_sel                : 3;   // 11: 9
	UINT32                             : 3;   // 14:12
	UINT32 swrst_trace                 : 1;   //    15
	UINT32                             : 7;   // 22:16
	UINT32 swrst_dbgv                  : 1;   //    23
	UINT32                             : 7;   // 30:24
	UINT32 swrst_at                    : 1;   //    31
} L9A0_CTR09_T;

typedef struct { // 0xfd300038
	UINT32 ahb7_sel                    : 4;   //  3: 0
	UINT32                             : 1;   //     4
	UINT32 ahb6_sel                    : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 ahb5_sel                    : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 ahb4_sel                    : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 ahb3_sel                    : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 ahb2_sel                    : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 ahb1_sel                    : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 ahb0_sel                    : 3;   // 31:29
} L9A0_CTR10_T;

typedef struct { // 0xfd30003c
	UINT32 ahb22_sel                   : 4;   //  3: 0
	UINT32                             : 1;   //     4
	UINT32 ahb21_sel                   : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 ahb20_sel                   : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 ahb19_sel                   : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 ahb18_sel                   : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 ahb17_sel                   : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 ahb9_sel                    : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 ahb8_sel                    : 3;   // 31:29
} L9A0_CTR11_T;

typedef struct { // 0xfd300040
	UINT32                             : 31;  // 30: 0
	UINT32 rmii_clkoff_sel             : 1;   //    31
} L9A0_CTR12_T;

typedef struct { // 0xfd300044
	UINT32                             : 5;   //  4: 0
	UINT32 ahb30_sel                   : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 ahb29_sel                   : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 ahb28_sel                   : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 ahb27_sel                   : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 ahb26_sel                   : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 ahb25_sel                   : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 ahb24_sel                   : 3;   // 31:29
} L9A0_CTR13_T;

typedef struct { // 0xfd300048
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
} L9A0_CTR14_T;

typedef struct { // 0xfd30004c
	UINT32                             : 26;  // 25: 0
	UINT32 swrst_ahb37                 : 1;   //    26
	UINT32 swrst_ahb36                 : 1;   //    27
	UINT32 swrst_ahb35                 : 1;   //    28
	UINT32 swrst_ahb34                 : 1;   //    29
	UINT32 swrst_ahb33                 : 1;   //    30
	UINT32 swrst_ahb32                 : 1;   //    31
} L9A0_CTR15_T;

typedef struct { // 0xfd300050
	UINT32                             : 1;   //     0
	UINT32 bus7_sel                    : 3;   //  3: 1
	UINT32                             : 1;   //     4
	UINT32 bus6_sel                    : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 bus5_sel                    : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 bus4_sel                    : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 bus3_sel                    : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 bus2_sel                    : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 bus1_sel                    : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 bus0_sel                    : 3;   // 31:29
} L9A0_CTR16_T;

typedef struct { // 0xfd300054
	UINT32                             : 21;  // 20: 0
	UINT32 bus10_sel                   : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 bus9_sel                    : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 bus8_sel                    : 3;   // 31:29
} L9A0_CTR17_T;

typedef struct { // 0xfd300058
	UINT32                             : 21;  // 20: 0
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
} L9A0_CTR18_T;

typedef struct { // 0xfd30005c
	UINT32                             : 1;   //     0
	UINT32 apb0_7_sel                  : 3;   //  3: 1
	UINT32                             : 1;   //     4
	UINT32 apb0_6_sel                  : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 apb0_5_sel                  : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 apb0_4_sel                  : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 apb0_3_sel                  : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 apb0_2_sel                  : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 apb0_1_sel                  : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 apb0_0_sel                  : 3;   // 31:29
} L9A0_CTR19_T;

typedef struct { // 0xfd300060
	UINT32                             : 1;   //     0
	UINT32 apb1_7_sel                  : 3;   //  3: 1
	UINT32                             : 1;   //     4
	UINT32 apb1_6_sel                  : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 apb1_5_sel                  : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 apb1_4_sel                  : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 apb1_3_sel                  : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 apb1_2_sel                  : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 apb1_1_sel                  : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 apb1_0_sel                  : 3;   // 31:29
} L9A0_CTR20_T;

typedef struct { // 0xfd300064
	UINT32                             : 1;   //     0
	UINT32 apb1_15_sel                 : 3;   //  3: 1
	UINT32                             : 1;   //     4
	UINT32 apb1_14_sel                 : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 apb1_13_sel                 : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 apb1_12_sel                 : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 apb1_11_sel                 : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 apb1_10_sel                 : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 apb1_9_sel                  : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 apb1_8_sel                  : 3;   // 31:29
} L9A0_CTR21_T;

typedef struct { // 0xfd300068
	UINT32                             : 1;   //     0
	UINT32 apb1_23_sel                 : 3;   //  3: 1
	UINT32                             : 1;   //     4
	UINT32 apb1_22_sel                 : 3;   //  7: 5
	UINT32                             : 1;   //     8
	UINT32 apb1_21_sel                 : 3;   // 11: 9
	UINT32                             : 1;   //    12
	UINT32 apb1_20_sel                 : 3;   // 15:13
	UINT32                             : 1;   //    16
	UINT32 apb1_19_sel                 : 3;   // 19:17
	UINT32                             : 1;   //    20
	UINT32 apb1_18_sel                 : 3;   // 23:21
	UINT32                             : 1;   //    24
	UINT32 apb1_17_sel                 : 3;   // 27:25
	UINT32                             : 1;   //    28
	UINT32 apb1_16_sel                 : 3;   // 31:29
} L9A0_CTR22_T;

typedef struct { // 0xfd30006c
	UINT32                             : 29;  // 28: 0
	UINT32 apb1_24_sel                 : 3;   // 31:29
} L9A0_CTR23_T;

typedef struct { // 0xfd300070
	UINT32 swrst_apb1_23               : 1;   //     0
	UINT32 swrst_apb1_22               : 1;   //     1
	UINT32 swrst_apb1_21               : 1;   //     2
	UINT32 swrst_apb1_20               : 1;   //     3
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
} L9A0_CTR24_T;

typedef struct { // 0xfd300074
	UINT32                             : 31;  // 30: 0
	UINT32 swrst_apb1_24               : 1;   //    31
} L9A0_CTR25_T;

typedef struct { // 0xfd300078
	UINT32                             : 6;   //  5: 0
	UINT32 ssclk_sel                   : 2;   //  7: 6
	UINT32                             : 2;   //  9: 8
	UINT32 msclk_sel                   : 2;   // 11:10
	UINT32 extb_sel                    : 2;   // 13:12
	UINT32 exta_sel                    : 2;   // 15:14
	UINT32                             : 3;   // 18:16
	UINT32 ch3pix_clk_inv              : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 cvd27_clk_inv               : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 cvd54_clk_inv               : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 cvd54_clk_sel               : 1;   //    31
} L9A0_CTR26_T;

typedef struct { // 0xfd30007c
	UINT32 ext_en_i                    : 1;   //     0
	UINT32                             : 2;   //  2: 1
	UINT32 f27_18_sel                  : 1;   //     3
	UINT32                             : 1;   //     4
	UINT32 sc_ext_sel                  : 3;   //  7: 5
	UINT32                             : 3;   // 10: 8
	UINT32 slt_disp                    : 1;   //    11
	UINT32 reg_s_te                    : 1;   //    12
	UINT32 reg_sys                     : 1;   //    13
	UINT32 disp_h_dp                   : 1;   //    14
	UINT32 disp_h                      : 1;   //    15
	UINT32 disp_dp                     : 1;   //    16
	UINT32 disp_de                     : 1;   //    17
	UINT32 lvds_clk_sel                : 1;   //    18
	UINT32 dco_sel                     : 1;   //    19
	UINT32 swrst_cvd                   : 1;   //    20
	UINT32 swrst_se                    : 1;   //    21
	UINT32 swrst_cve                   : 1;   //    22
	UINT32 swrst_chb                   : 1;   //    23
	UINT32 swrst_cha                   : 1;   //    24
	UINT32 swrst_extb                  : 1;   //    25
	UINT32 swrst_exta                  : 1;   //    26
	UINT32 swrst_ch3                   : 1;   //    27
	UINT32 swrst_scart                 : 1;   //    28
	UINT32 swrst_vbi                   : 1;   //    29
	UINT32 swrst_cvd27                 : 1;   //    30
	UINT32 swrst_cvd54                 : 1;   //    31
} L9A0_CTR27_T;

typedef struct { // 0xfd300080
	UINT32 wd_reset                    : 1;   //     0
	UINT32                             : 2;   //  2: 1
	UINT32 neon_reset                  : 1;   //     3
	UINT32                             : 3;   //  6: 4
	UINT32 scu_reset                   : 1;   //     7
	UINT32                             : 3;   // 10: 8
	UINT32 cpu_h                       : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 cpu_reset                   : 1;   //    15
	UINT32                             : 1;   //    16
	UINT32 l2_reset                    : 1;   //    17
	UINT32 a_stdby_cpu                 : 2;   // 19:18
	UINT32                             : 3;   // 22:20
	UINT32 pll_sel_core                : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 pll_sel_m2                  : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 pll_sel_m1                  : 1;   //    31
} L9A0_CTR28_T;

typedef struct { // 0xfd300084
	UINT32 ext_intr_pol_sel0           : 1;   //     0
	UINT32 ext_intr_pol_sel1           : 1;   //     1
	UINT32 ext_intr_pol_sel2           : 1;   //     2
	UINT32 ext_intr_pol_sel3           : 1;   //     3
	UINT32                             : 4;   //  7: 4
	UINT32 jtag_sel                    : 3;   // 10: 8
	UINT32                             : 1;   //    11
	UINT32 m2_h_reset                  : 1;   //    12
	UINT32 m2_reset                    : 1;   //    13
	UINT32 a_stdby_m2                  : 2;   // 15:14
	UINT32                             : 4;   // 19:16
	UINT32 m1_h_reset                  : 1;   //    20
	UINT32 m1_reset                    : 1;   //    21
	UINT32 a_stdby_m1                  : 2;   // 23:22
	UINT32                             : 4;   // 27:24
	UINT32 m0_h_reset                  : 1;   //    28
	UINT32 m0_reset                    : 1;   //    29
	UINT32 a_stdby_m0                  : 2;   // 31:30
} L9A0_CTR29_T;

typedef struct { // 0xfd300088
	UINT32                             : 3;   //  2: 0
	UINT32 trace_en                    : 1;   //     3
	UINT32                             : 7;   // 10: 4
	UINT32 usb_mon_en                  : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 gfx_mon_en                  : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 aad_mon_en                  : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 aud_mon_en                  : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 cpu_mon_en                  : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 de_mon_en                   : 1;   //    31
} L9A0_CTR30_T;

typedef struct { // 0xfd30008c
	UINT32 trace_en_b00                : 1;   //     0
	UINT32                             : 3;   //  3: 1
	UINT32 aad_mon_en_b04              : 1;   //     4
	UINT32 aud_mon_en_b05              : 1;   //     5
	UINT32 cpu_mon_en_b06              : 1;   //     6
	UINT32 de_mon_en_b07               : 1;   //     7
	UINT32                             : 2;   //  9: 8
	UINT32 trace_en_b10                : 1;   //    10
	UINT32 usb_mon_en                  : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 gfx_mon_en                  : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 aad_mon_en_b19              : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 aud_mon_en_b23              : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 cpu_mon_en_b27              : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 de_mon_en_b31               : 1;   //    31
} L9A0_CTR31_T;

typedef struct { // 0xfd300090
	UINT32                             : 23;  // 22: 0
	UINT32 stpio_en_ctrl               : 1;   //    23
	UINT32 gp4_0_mux_en                : 1;   //    24
	UINT32 gp4_1_mux_en                : 1;   //    25
	UINT32 gp4_2_mux_en                : 1;   //    26
	UINT32 gp4_3_mux_en                : 1;   //    27
	UINT32 gp4_4_mux_en                : 1;   //    28
	UINT32 gp4_5_mux_en                : 1;   //    29
	UINT32 gp4_6_mux_en                : 1;   //    30
	UINT32 gp4_7_mux_en                : 1;   //    31
} L9A0_CTR32_T;

typedef struct { // 0xfd300094
	UINT32 trace_en                    : 1;   //     0
	UINT32                             : 14;  // 14: 1
	UINT32 cpu_mon_31_en               : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 aad_mon_en                  : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 aud_mon_en                  : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 cpu_mon_en                  : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 de_mon_en                   : 1;   //    31
} L9A0_CTR33_T;

typedef struct { // 0xfd300098
	UINT32 gp6_3_mux_en                : 1;   //     0
	UINT32                             : 6;   //  6: 1
	UINT32 gp6_4_mux_en                : 1;   //     7
	UINT32                             : 7;   // 14: 8
	UINT32 gp6_5_mux_en                : 1;   //    15
	UINT32                             : 7;   // 22:16
	UINT32 gp6_6_mux_en                : 1;   //    23
	UINT32                             : 7;   // 30:24
	UINT32 gp6_7_mux_en                : 1;   //    31
} L9A0_CTR34_T;

typedef struct { // 0xfd30009c
	UINT32                             : 11;  // 10: 0
	UINT32 gp7_5_mux_en                : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 gp7_6_mux_en                : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 gp7_7_mux_en                : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 gp6_0_mux_en                : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 gp6_1_mux_en                : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 gp6_2_mux_en                : 1;   //    31
} L9A0_CTR35_T;

typedef struct { // 0xfd3000a0
	UINT32                             : 10;  //  9: 0
	UINT32 rx_sel_de                   : 2;   // 11:10
	UINT32                             : 6;   // 17:12
	UINT32 rx_sel_sp                   : 2;   // 19:18
	UINT32                             : 2;   // 21:20
	UINT32 uart2_sel                   : 2;   // 23:22
	UINT32                             : 2;   // 25:24
	UINT32 uart1_sel                   : 2;   // 27:26
	UINT32                             : 2;   // 29:28
	UINT32 uart0_sel                   : 2;   // 31:30
} L9A0_CTR36_T;

typedef struct { // 0xfd3000a4
	UINT32                             : 3;   //  2: 0
	UINT32 gp8_5_mux_en                : 1;   //     3
	UINT32                             : 3;   //  6: 4
	UINT32 gp8_6_mux_en                : 1;   //     7
	UINT32                             : 3;   // 10: 8
	UINT32 gp8_7_mux_en                : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 gp7_0_mux_en                : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 gp7_1_mux_en                : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 gp7_2_mux_en                : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 gp7_3_mux_en                : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 gp7_4_mux_en                : 1;   //    31
} L9A0_CTR37_T;

typedef struct { // 0xfd3000a8
	UINT32                             : 19;  // 18: 0
	UINT32 gp8_1_mux_en                : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 gp8_2_mux_en                : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 gp8_3_mux_en                : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 gp8_4_mux_en                : 1;   //    31
} L9A0_CTR38_T;

typedef struct { // 0xfd3000ac
	UINT32                             : 16;  // 15: 0
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
} L9A0_CTR39_T;

typedef struct { // 0xfd3000b0
	UINT32                             : 15;  // 14: 0
	UINT32 gpu_pwrdnbypass             : 1;   //    15
	UINT32                             : 7;   // 22:16
	UINT32 gpu_testin_b23              : 1;   //    23
	UINT32                             : 7;   // 30:24
	UINT32 gpu_testin_b31              : 1;   //    31
} L9A0_CTR40_T;

typedef struct { // 0xfd3000b4
	UINT32                             : 7;   //  6: 0
	UINT32 pull_up_control_b07         : 1;   //     7
	UINT32                             : 3;   // 10: 8
	UINT32 pull_up_control_b11         : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 pull_up_control_b15         : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 pull_up_control_b19         : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 pull_up_control_b23         : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 pull_up_control_b27         : 1;   //    27
} L9A0_CTR41_T;

typedef struct { // 0xfd3000b8
	UINT32                             : 3;   //  2: 0
	UINT32 pull_up_control_b03         : 1;   //     3
	UINT32                             : 3;   //  6: 4
	UINT32 pull_up_control_b07         : 1;   //     7
	UINT32                             : 3;   // 10: 8
	UINT32 pull_up_control_b11         : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 pull_up_control_b15         : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 pull_up_control_b19         : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 pull_up_control_b23         : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 pull_up_control_b27         : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 pull_up_control_b31         : 1;   //    31
} L9A0_CTR42_T;

typedef struct { // 0xfd3000bc
	UINT32                             : 15;  // 14: 0
	UINT32 pull_up_control_b15         : 1;   //    15
	UINT32                             : 7;   // 22:16
	UINT32 pull_up_control_b23         : 1;   //    23
	UINT32                             : 7;   // 30:24
	UINT32 pull_up_control_b31         : 1;   //    31
} L9A0_CTR43_T;

typedef struct { // 0xfd3000c0
	UINT32                             : 15;  // 14: 0
	UINT32 pull_up_control_b15         : 1;   //    15
	UINT32                             : 7;   // 22:16
	UINT32 pull_up_control_b23         : 1;   //    23
	UINT32                             : 7;   // 30:24
	UINT32 pull_up_control_b31         : 1;   //    31
} L9A0_CTR44_T;

typedef struct { // 0xfd3000c4
	UINT32                             : 11;  // 10: 0
	UINT32 pull_up_control_b11         : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 pull_up_control_b15         : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 pull_up_control_b19         : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 pull_up_control_b23         : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 pull_up_control_b27         : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 pull_up_control_b31         : 1;   //    31
} L9A0_CTR45_T;

typedef struct { // 0xfd3000c8
	UINT32                             : 23;  // 22: 0
	UINT32 pull_up_control_b23         : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 pull_up_control_b27         : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 pull_up_control_b31         : 1;   //    31
} L9A0_CTR46_T;

typedef struct { // 0xfd3000cc
	UINT32                             : 3;   //  2: 0
	UINT32 pull_up_control_b03         : 1;   //     3
	UINT32                             : 3;   //  6: 4
	UINT32 pull_up_control_b07         : 1;   //     7
	UINT32                             : 3;   // 10: 8
	UINT32 pull_up_control_b11         : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 pull_up_control_b15         : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 pull_up_control_b19         : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 pull_up_control_b23         : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 pull_up_control_b27         : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 pull_up_control_b31         : 1;   //    31
} L9A0_CTR47_T;

typedef struct { // 0xfd3000d0
	UINT32                             : 11;  // 10: 0
	UINT32 pull_up_control_b11         : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 pull_up_control_b15         : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 pull_up_control_b19         : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 pull_up_control_b23         : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 pull_up_control_b27         : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 pull_up_control_b31         : 1;   //    31
} L9A0_CTR48_T;

typedef struct { // 0xfd3000d4
	UINT32                             : 7;   //  6: 0
	UINT32 pull_up_control_b07         : 1;   //     7
	UINT32                             : 7;   // 14: 8
	UINT32 pull_up_control_b15         : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 pull_up_control_b19         : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 pull_up_control_b23         : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 pull_up_control_b27         : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 pull_up_control_b31         : 1;   //    31
} L9A0_CTR49_T;

typedef struct { // 0xfd3000d8
	UINT32                             : 23;  // 22: 0
	UINT32 tpo_sel_ctrl                : 1;   //    23
	UINT32                             : 7;   // 30:24
	UINT32 swrst                       : 1;   //    31
} L9A0_CTR50_T;

typedef struct { // 0xfd3000dc
	UINT32                             : 19;  // 18: 0
	UINT32 gp11_6_mux_en               : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 gp11_7_mux_en               : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 gp10_0_mux_en               : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 gp10_1_mux_en               : 1;   //    31
} L9A0_CTR51_T;

typedef struct { // 0xfd3000e0
	UINT32                             : 11;  // 10: 0
	UINT32 gp11_0_mux_en               : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 gp11_1_mux_en               : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 gp11_2_mux_en               : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 gp11_3_mux_en               : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 gp11_4_mux_en               : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 gp11_5_mux_en               : 1;   //    31
} L9A0_CTR52_T;

typedef struct { // 0xfd3000e4
	UINT32                             : 3;   //  2: 0
	UINT32 gp5_0_mux_en                : 1;   //     3
	UINT32                             : 3;   //  6: 4
	UINT32 gp5_1_mux_en                : 1;   //     7
	UINT32                             : 3;   // 10: 8
	UINT32 gp5_2_mux_en                : 1;   //    11
	UINT32                             : 3;   // 14:12
	UINT32 gp5_3_mux_en                : 1;   //    15
	UINT32                             : 3;   // 18:16
	UINT32 gp5_4_mux_en                : 1;   //    19
	UINT32                             : 3;   // 22:20
	UINT32 gp5_5_mux_en                : 1;   //    23
	UINT32                             : 3;   // 26:24
	UINT32 gp5_6_mux_en                : 1;   //    27
	UINT32                             : 3;   // 30:28
	UINT32 gp5_7_mux_en                : 1;   //    31
} L9A0_CTR53_T;

typedef struct { // 0xfd3000e8
	UINT32 dftclk_ctrl                 : 1;   //     0
	UINT32                             : 14;  // 14: 1
	UINT32 pd                          : 1;   //    15
	UINT32                             : 2;   // 17:16
	UINT32 bt_testclk1_ctrl            : 1;   //    18
	UINT32 bt_txpreemphasistune1_ctrl  : 1;   //    19
	UINT32 bt_txrisetune1_ctrl         : 1;   //    20
	UINT32 bt_testclk0_ctrl            : 1;   //    21
	UINT32 bt_txpreemphasistune0_ctrl  : 1;   //    22
	UINT32 bt_txrisetune0_ctrl         : 1;   //    23
	UINT32 bt_commononn_ctrl           : 1;   //    24
	UINT32 bt_refclksel_ctrl           : 2;   // 26:25
	UINT32 bt_refclkdiv_ctrl           : 2;   // 28:27
	UINT32 bt_testen_en_ctrl           : 1;   //    29
	UINT32 bt_vatestenb_en_ctrl        : 1;   //    30
	UINT32 bt_siddq_en_ctrl            : 1;   //    31
} L9A0_CTR54_T;

typedef struct { // 0xfd3000ec
	UINT32                             : 28;  // 27: 0
	UINT32 testaddr1                   : 4;   // 31:28
} L9A0_CTR55_T;

typedef struct { // 0xfd3000f0
	UINT32 testdatain1                 : 8;   //  7: 0
	UINT32 testdataoutsel1             : 1;   //     8
	UINT32 txvreftune1                 : 4;   // 12: 9
	UINT32 txfslstune1                 : 4;   // 16:13
	UINT32 vregtune1                   : 1;   //    17
	UINT32 txhsxvtune1                 : 2;   // 19:18
	UINT32 sqrxtune1                   : 3;   // 22:20
	UINT32 compdistune1                : 3;   // 25:23
	UINT32 loopbackenb1                : 1;   //    26
	UINT32 portreset1                  : 1;   //    27
	UINT32 testaddr0                   : 4;   // 31:28
} L9A0_CTR56_T;

typedef struct { // 0xfd3000f4
	UINT32                             : 3;   //  2: 0
	UINT32 testdatain0                 : 8;   // 10: 3
	UINT32 testdataoutsel0             : 1;   //    11
	UINT32 txvreftune0                 : 4;   // 15:12
	UINT32 txfslstune0                 : 4;   // 19:16
	UINT32 vregtune0                   : 1;   //    20
	UINT32 txhsxvtune0                 : 2;   // 22:21
	UINT32 sqrxtune0                   : 3;   // 25:23
	UINT32 compdistune0                : 3;   // 28:26
	UINT32 loopbackenb0                : 1;   //    29
	UINT32 otgdisable0                 : 1;   //    30
	UINT32 portreset0                  : 1;   //    31
} L9A0_CTR57_T;

typedef struct { // 0xfd3000f8
	UINT32                             : 18;  // 17: 0
	UINT32 testclk1_ctrl               : 1;   //    18
	UINT32 txpreemphasistune1_ctrl     : 1;   //    19
	UINT32 txrisetune1_ctrl            : 1;   //    20
	UINT32 testclk0_ctrl               : 1;   //    21
	UINT32 txpreemphasistune0_ctrl     : 1;   //    22
	UINT32 txrisetune0_ctrl            : 1;   //    23
	UINT32 commononn_ctrl              : 1;   //    24
	UINT32 refclksel_ctrl              : 2;   // 26:25
	UINT32 refclkdiv_ctrl              : 2;   // 28:27
	UINT32 testen_en_ctrl              : 1;   //    29
	UINT32 vatestenb_en_ctrl           : 1;   //    30
	UINT32 siddq_en_ctrl               : 1;   //    31
} L9A0_CTR58_T;

typedef struct { // 0xfd3000fc
	UINT32                             : 28;  // 27: 0
	UINT32 testaddr1                   : 4;   // 31:28
} L9A0_CTR59_T;

typedef struct { // 0xfd300100
	UINT32 testdatain1                 : 8;   //  7: 0
	UINT32 testdataoutsel1             : 1;   //     8
	UINT32 txvreftune1                 : 4;   // 12: 9
	UINT32 txfslstune1                 : 4;   // 16:13
	UINT32 vregtune1                   : 1;   //    17
	UINT32 txhsxvtune1                 : 2;   // 19:18
	UINT32 sqrxtune1                   : 3;   // 22:20
	UINT32 compdistune1                : 3;   // 25:23
	UINT32 loopbackenb1                : 1;   //    26
	UINT32 portreset1                  : 1;   //    27
	UINT32 testaddr0                   : 4;   // 31:28
} L9A0_CTR60_T;

typedef struct { // 0xfd300104
	UINT32                             : 3;   //  2: 0
	UINT32 testdatain0                 : 8;   // 10: 3
	UINT32 testdataoutsel0             : 1;   //    11
	UINT32 txvreftune0                 : 4;   // 15:12
	UINT32 txfslstune0                 : 4;   // 19:16
	UINT32 vregtune0                   : 1;   //    20
	UINT32 txhsxvtune0                 : 2;   // 22:21
	UINT32 sqrxtune0                   : 3;   // 25:23
	UINT32 compdistune0                : 3;   // 28:26
	UINT32 loopbackenb0                : 1;   //    29
	UINT32 otgdisable0                 : 1;   //    30
	UINT32 portreset0                  : 1;   //    31
} L9A0_CTR61_T;

typedef struct { // 0xfd300108
	UINT32                             : 28;  // 27: 0
	UINT32 disp_pre_fd_ctrl            : 2;   // 29:28
	UINT32 disp_od_fout_ctrl           : 2;   // 31:30
} L9A0_CTR62_T;

typedef struct { // 0xfd30010c
	UINT32 disp_udex_ctrl              : 5;   //  4: 0
	UINT32 disp_msex_ctrl              : 9;   // 13: 5
	UINT32 disp_nsc_ctrl               : 4;   // 17:14
	UINT32 disp_npc_ctrl               : 6;   // 23:18
	UINT32 disp_m_ctrl                 : 3;   // 26:24
	UINT32 disp_cih_ctrl               : 3;   // 29:27
	UINT32 disp_lf_ctrl                : 1;   //    30
	UINT32 disp_pdb_ctrl               : 1;   //    31
} L9A0_CTR63_T;


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
} L9A0_USB0_CTRL0;


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
} L9A0_USB0_STATUS0;

/*-----------------------------------------------------------------------------
	0x0118 usb0_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	usb0_ehci_usbsts_o              : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	usb0_ehci_lpsmc_state_o         : 4;	//  8:11
} L9A0_USB0_STATUS1;

/*-----------------------------------------------------------------------------
	0x011c usb0_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	usb0_endian_ahbms_ohci_1_bufacc : 1,	//     2
	                                :13,	//  3:15 reserved
	usb0_ss_fladj_val_i_p1          : 6,	// 16:21
	                                : 6,	// 22:27 reserved
	usb0_app_prt_ovrcur_i_p1        : 1,	//    28
	                                : 2,	// 29:30 reserved
	usb0_endian_ahbms_ohci_1        : 1;	//    31
} L9A0_USB0_CTRL1;

/*-----------------------------------------------------------------------------
	0x0120 usb0_status2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	usb0_ohci_1_globalsuspend_o     : 1,	//     0
	usb0_ohci_1_drwe_o              : 1,	//     1
	usb0_ohci_1_rwe_o               : 1,	//     2
	usb0_ohci_1_rmtwkp_o            : 1,	//     3
	usb0_ohci_1_smi_o_n             : 1,	//     4
	usb0_ohci_1_sof_o_n             : 1,	//     5
	usb0_ohci_1_bufacc_o            : 1,	//     6
	usb0_ehci_prt_pwr_o_p1          : 1,	//     7
	                                :16,	//  8:23 reserved
	usb0_ohci_1_ccs_o               : 1;	//    24
} L9A0_USB0_STATUS2;

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
} L9A0_IO_CTRL;

/*-----------------------------------------------------------------------------
	0x0138 apb_time_out_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tout_en                         : 1,	//     0
	tout_intr_clr                   : 1,	//     1
	                                :14,	//  2:15 reserved
	tout_cnt                        :16;	// 16:31
} L9A0_APB_TIME_OUT_CTRL;

/*-----------------------------------------------------------------------------
	0x013c apb_time_out_src_31_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pselxx_tout                     ;   	// 31: 0
} L9A0_APB_TIME_OUT_SRC_31_0;

/*-----------------------------------------------------------------------------
	0x0140 apb_time_out_src ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pselxx_tout              	   : 6;   	// 31: 0
} L9A0_APB_TIME_OUT_SRC_41_32;




/*-----------------------------------------------------------------------------
	0x0148 stcc_ref_time_stamp_32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc_ref_time_stamp_high        : 1;	//     0
} L9A0_STCC_REF_TIME_STAMP_32;

/*-----------------------------------------------------------------------------
	0x014c stcc_ref_time_stamp_31_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc_ref_time_stamp_low         ;   	// 31: 0
} L9A0_STCC_REF_TIME_STAMP_31_0;

/*-----------------------------------------------------------------------------
	0x0150 stcc_real_time_stamp_32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc_real_time_stamp_high       : 1;	//     0
} L9A0_STCC_REAL_TIME_STAMP_32;

/*-----------------------------------------------------------------------------
	0x0154 stcc_real_time_stamp_31_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	stcc_real_time_stamp_low        ;   	// 31: 0
} L9A0_STCC_REAL_TIME_STAMP_31_0;

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
} L9A0_FLUSH_DONE_STATUS;

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
} L9A0_FLUSH_REQ;

/*-----------------------------------------------------------------------------
	0x0164 bandwidth_limiter_axi0_dly_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bw_platform0_raxi_dly_cnt       :16,	//  0:15
	bw_platform0_waxi_dly_cnt       :16;	// 16:31
} L9A0_BANDWIDTH_LIMITER_AXI0_DLY_CNT;

/*-----------------------------------------------------------------------------
	0x0168 bandwidth_limiter_axi1_dly_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bw_platform1_raxi_dly_cnt       :16,	//  0:15
	bw_platform1_waxi_dly_cnt       :16;	// 16:31
} L9A0_BANDWIDTH_LIMITER_AXI1_DLY_CNT;

/*-----------------------------------------------------------------------------
	0x016c bandwidth_limiter_cpu_axi0_dly_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bw_cpu_raxi0_dly_cnt            :16,	//  0:15
	bw_cpu_waxi0_dly_cnt            :16;	// 16:31
} L9A0_BANDWIDTH_LIMITER_CPU_AXI0_DLY_CNT;

/*-----------------------------------------------------------------------------
	0x0170 bandwidth_limiter_cpu_axi1_dly_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	bw_cpu_raxi1_dly_cnt            :16,	//  0:15
	bw_cpu_waxi1_dly_cnt            :16;	// 16:31
} L9A0_BANDWIDTH_LIMITER_CPU_AXI1_DLY_CNT;

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
} L9A0_GPIO_INTERRUPT_STATUS;

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
} L9A0_URGENCY;

/*-----------------------------------------------------------------------------
	0x017c l2_cache_drreq_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_drreq_evnt_cnt               ;   	// 31: 0
} L9A0_L2_CACHE_DRREQ_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0180 l2_cache_drhit_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_drhit_evnt_cnt               ;   	// 31: 0
} L9A0_L2_CACHE_DRHIT_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0184 l2_cache_dwreq_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_dwreq_evnt_cnt               ;   	// 31: 0
} L9A0_L2_CACHE_DWREQ_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0188 l2_cache_dwtreq_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_dwtreq_evnt_cnt              ;   	// 31: 0
} L9A0_L2_CACHE_DWTREQ_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x018c l2_cache_dwhit_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_dwhit_evnt_cnt               ;   	// 31: 0
} L9A0_L2_CACHE_DWHIT_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0190 l2_cache_irreq_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_irreq_evnt_cnt               ;   	// 31: 0
} L9A0_L2_CACHE_IRREQ_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0194 l2_cache_irhit_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_irhit_evnt_cnt               ;   	// 31: 0
} L9A0_L2_CACHE_IRHIT_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x0198 l2_cache_wa_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_wa_evnt_cnt                  ;   	// 31: 0
} L9A0_L2_CACHE_WA_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x019c l2_cache_co_event_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	l2_co_evnt_cnt                  ;   	// 31: 0
} L9A0_L2_CACHE_CO_EVENT_CNT;

/*-----------------------------------------------------------------------------
	0x01a4 l2_cache_event_monitor_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	evnt_en                         : 1,	//     0
	evnt_cnt_reset                  : 1;	//     1
} L9A0_L2_CACHE_EVENT_MONITOR_CTRL;

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
} L9A0_USB1_CTRL0;

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
} L9A0_USB1_STATUS0;

/*-----------------------------------------------------------------------------
	0x01b0 usb1_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	usb1_ehci_usbsts_o              : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	usb1_ehci_lpsmc_state_o         : 4;	//  8:11
} L9A0_USB1_STATUS1;

/*-----------------------------------------------------------------------------
	0x01b4 usb1_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 2,	//  0: 1 reserved
	usb1_endian_ahbms_ohci_1_bufacc : 1,	//     2
	                                :13,	//  3:15 reserved
	usb1_ss_fladj_val_i_p1          : 6,	// 16:21
	                                : 6,	// 22:27 reserved
	usb1_app_prt_ovrcur_i_p1        : 1,	//    28
	                                : 2,	// 29:30 reserved
	usb1_endian_ahbms_ohci_1        : 1;	//    31
} L9A0_USB1_CTRL1;

/*-----------------------------------------------------------------------------
	0x01b8 usb1_status2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	usb1_ohci_1_globalsuspend_o     : 1,	//     0
	usb1_ohci_1_drwe_o              : 1,	//     1
	usb1_ohci_1_rwe_o               : 1,	//     2
	usb1_ohci_1_rmtwkp_o            : 1,	//     3
	usb1_ohci_1_smi_o_n             : 1,	//     4
	usb1_ohci_1_sof_o_n             : 1,	//     5
	usb1_ohci_1_bufacc_o            : 1,	//     6
	usb1_ehci_prt_pwr_o_p1          : 1,	//     7
	scuramemas                      : 1,	//     8
	                                :15,	//  9:23 reserved
	usb1_ohci_1_ccs_o               : 1;	//    24
} L9A0_USB1_STATUS2;

typedef struct { // 0xfd3001bc
	UINT32                             : 18;  // 17: 0
	UINT32 sd_dco_g_ctrl               : 3;   // 20:18
	UINT32 sd_dco_lpf_ctrl             : 2;   // 22:21
	UINT32 sd_dco_resetb_ctrl          : 1;   //    23
	UINT32 sd_dco_rom_test_ctrl        : 1;   //    24
	UINT32 de_dco_g_ctrl               : 3;   // 27:25
	UINT32 de_dco_lpf_ctrl             : 2;   // 29:28
	UINT32 de_dco_resetb_ctrl          : 1;   //    30
	UINT32 de_dco_rom_test_ctrl        : 1;   //    31
} L9A0_CTR64_T;

typedef struct { // 0xfd3001c0
	UINT32 sd_dco_fcw_ctrl             : 22;  // 21: 0
} L9A0_CTR65_T;

typedef struct { // 0xfd3001c4
	UINT32                             : 24;  // 23: 0
	UINT32 m2_dr3p_pre_fd_ctrl         : 2;   // 25:24
	UINT32 m2_dr3p_od_fout_ctrl        : 2;   // 27:26
	UINT32 m2_dr3p_dtr_sel_ctrl        : 4;   // 31:28
} L9A0_CTR66_T;

typedef struct { // 0xfd3001c8
	UINT32 m2_dr3p_udex_ctrl           : 5;   //  4: 0
	UINT32 m2_dr3p_msex_ctrl           : 9;   // 13: 5
	UINT32 m2_dr3p_nsc_ctrl            : 4;   // 17:14
	UINT32 m2_dr3p_npc_ctrl            : 6;   // 23:18
	UINT32 m2_dr3p_m_ctrl              : 3;   // 26:24
	UINT32 m2_dr3p_cih_ctrl            : 3;   // 29:27
	UINT32 m2_dr3p_lf_ctrl             : 1;   //    30
	UINT32 m2_dr3p_pdb_ctrl            : 1;   //    31
} L9A0_CTR67_T;

typedef struct { // 0xfd3001cc
	UINT32                             : 24;  // 23: 0
	UINT32 m1_dr3p_pre_fd_ctrl         : 2;   // 25:24
	UINT32 m1_dr3p_od_fout_ctrl        : 2;   // 27:26
	UINT32 m1_dr3p_dtr_sel_ctrl        : 4;   // 31:28
} L9A0_CTR68_T;

typedef struct { // 0xfd3001d0
	UINT32 m1_dr3p_udex_ctrl           : 5;   //  4: 0
	UINT32 m1_dr3p_msex_ctrl           : 9;   // 13: 5
	UINT32 m1_dr3p_nsc_ctrl            : 4;   // 17:14
	UINT32 m1_dr3p_npc_ctrl            : 6;   // 23:18
	UINT32 m1_dr3p_m_ctrl              : 3;   // 26:24
	UINT32 m1_dr3p_cih_ctrl            : 3;   // 29:27
	UINT32 m1_dr3p_lf_ctrl             : 1;   //    30
	UINT32 m1_dr3p_pdb_ctrl            : 1;   //    31
} L9A0_CTR69_T;

typedef struct { // 0xfd3001d4
	UINT32                             : 24;  // 23: 0
	UINT32 c_dr3p_pre_fd_ctrl          : 2;   // 25:24
	UINT32 c_dr3p_od_fout_ctrl         : 2;   // 27:26
	UINT32 c_dr3p_dtr_sel_ctrl         : 4;   // 31:28
} L9A0_CTR70_T;

typedef struct { // 0xfd3001d8
	UINT32 c_dr3p_udex_ctrl            : 5;   //  4: 0
	UINT32 c_dr3p_msex_ctrl            : 9;   // 13: 5
	UINT32 c_dr3p_nsc_ctrl             : 4;   // 17:14
	UINT32 c_dr3p_npc_ctrl             : 6;   // 23:18
	UINT32 c_dr3p_m_ctrl               : 3;   // 26:24
	UINT32 c_dr3p_cih_ctrl             : 3;   // 29:27
	UINT32 c_dr3p_lf_ctrl              : 1;   //    30
	UINT32 c_dr3p_pdb_ctrl             : 1;   //    31
} L9A0_CTR71_T;

typedef struct { // 0xfd3001dc
	UINT32 lv_flip_en_ctrl             : 12;  // 11: 0
	UINT32                             : 6;   // 17:12
	UINT32 lv_pemp_te2_ctrl            : 2;   // 19:18
	UINT32 lv_pemp_td2_ctrl            : 2;   // 21:20
	UINT32 lv_pemp_tclk2_ctrl          : 2;   // 23:22
	UINT32 lv_pemp_tc2_ctrl            : 2;   // 25:24
	UINT32 lv_pemp_tb2_ctrl            : 2;   // 27:26
	UINT32 lv_pemp_ta2_ctrl            : 2;   // 29:28
	UINT32 lv_pemp_te1_ctrl            : 2;   // 31:30
} L9A0_CTR72_T;

typedef struct { // 0xfd3001e0
	UINT32                             : 1;   //     0
	UINT32 lv_pemp_td1_ctrl            : 2;   //  2: 1
	UINT32 lv_pemp_tclk1_ctrl          : 2;   //  4: 3
	UINT32 lv_pemp_tc1_ctrl            : 2;   //  6: 5
	UINT32 lv_pemp_tb1_ctrl            : 2;   //  8: 7
	UINT32 lv_pemp_ta1_ctrl            : 2;   // 10: 9
	UINT32                             : 1;   //    11
	UINT32 lv_rf_ctrl                  : 1;   //    12
	UINT32 lv_rmlvds_en_ctrl           : 1;   //    13
	UINT32 lv_swc_te2_ctrl             : 3;   // 16:14
	UINT32 lv_swc_td2_ctrl             : 3;   // 19:17
	UINT32 lv_swc_tclk2_ctrl           : 3;   // 22:20
	UINT32 lv_swc_tc2_ctrl             : 3;   // 25:23
	UINT32 lv_swc_tb2_ctrl             : 3;   // 28:26
	UINT32 lv_swc_ta2_ctrl             : 3;   // 31:29
} L9A0_CTR73_T;

typedef struct { // 0xfd3001e4
	UINT32                             : 1;   //     0
	UINT32 lv_swc_te1_ctrl             : 3;   //  3: 1
	UINT32 lv_swc_td1_ctrl             : 3;   //  6: 4
	UINT32 lv_swc_tclk1_ctrl           : 3;   //  9: 7
	UINT32 lv_swc_tc1_ctrl             : 3;   // 12:10
	UINT32 lv_swc_tb1_ctrl             : 3;   // 15:13
	UINT32 lv_swc_ta1_ctrl             : 3;   // 18:16
	UINT32 lv_pdb_ch_ctrl              : 12;  // 30:19
	UINT32 lv_pdb_ctrl                 : 1;   //    31
} L9A0_CTR74_T;

typedef struct { // 0xfd3001e8
	UINT32                             : 20;  // 19: 0
	UINT32 rx2_sel_speed_en            : 1;   //    20
	UINT32 rx1_sel_speed_en            : 1;   //    21
	UINT32 rx0_sel_speed_en            : 1;   //    22
	UINT32 rx2_sel_8b10b_en            : 1;   //    23
	UINT32 rx1_sel_8b10b_en            : 1;   //    24
	UINT32 rx0_sel_8b10b_en            : 1;   //    25
	UINT32 rx2_rf_en                   : 1;   //    26
	UINT32 rx1_rf_en                   : 1;   //    27
	UINT32 rx0_rf_en                   : 1;   //    28
	UINT32 rx2_pdb_en                  : 1;   //    29
	UINT32 rx1_pdb_en                  : 1;   //    30
	UINT32 rx0_pdb_en                  : 1;   //    31
} L9A0_CTR75_T;

typedef struct {
	UINT32                          	                 __rsvd_00[   4];	// 0x0000 ~ 0x000c
	L9A0_CTR00_T    ctr00;           // 0xfd300010
	L9A0_CTR01_T    ctr01;           // 0xfd300014
	L9A0_CTR02_T    ctr02;           // 0xfd300018
	L9A0_CTR03_T    ctr03;           // 0xfd30001c
	L9A0_CTR04_T    ctr04;           // 0xfd300020
	L9A0_CTR05_T    ctr05;           // 0xfd300024
	L9A0_CTR06_T    ctr06;           // 0xfd300028
	L9A0_CTR07_T    ctr07;           // 0xfd30002c
	L9A0_CTR08_T    ctr08;           // 0xfd300030
	L9A0_CTR09_T    ctr09;           // 0xfd300034
	L9A0_CTR10_T    ctr10;           // 0xfd300038
	L9A0_CTR11_T    ctr11;           // 0xfd30003c
	L9A0_CTR12_T    ctr12;           // 0xfd300040
	L9A0_CTR13_T    ctr13;           // 0xfd300044
	L9A0_CTR14_T    ctr14;           // 0xfd300048
	L9A0_CTR15_T    ctr15;           // 0xfd30004c
	L9A0_CTR16_T    ctr16;           // 0xfd300050
	L9A0_CTR17_T    ctr17;           // 0xfd300054
	L9A0_CTR18_T    ctr18;           // 0xfd300058
	L9A0_CTR19_T    ctr19;           // 0xfd30005c
	L9A0_CTR20_T    ctr20;           // 0xfd300060
	L9A0_CTR21_T    ctr21;           // 0xfd300064
	L9A0_CTR22_T    ctr22;           // 0xfd300068
	L9A0_CTR23_T    ctr23;           // 0xfd30006c
	L9A0_CTR24_T    ctr24;           // 0xfd300070
	L9A0_CTR25_T    ctr25;           // 0xfd300074
	L9A0_CTR26_T    ctr26;           // 0xfd300078
	L9A0_CTR27_T    ctr27;           // 0xfd30007c
	L9A0_CTR28_T    ctr28;           // 0xfd300080
	L9A0_CTR29_T    ctr29;           // 0xfd300084
	L9A0_CTR30_T    ctr30;           // 0xfd300088
	L9A0_CTR31_T    ctr31;           // 0xfd30008c
	L9A0_CTR32_T    ctr32;           // 0xfd300090
	L9A0_CTR33_T    ctr33;           // 0xfd300094
	L9A0_CTR34_T    ctr34;           // 0xfd300098
	L9A0_CTR35_T    ctr35;           // 0xfd30009c
	L9A0_CTR36_T    ctr36;           // 0xfd3000a0
	L9A0_CTR37_T    ctr37;           // 0xfd3000a4
	L9A0_CTR38_T    ctr38;           // 0xfd3000a8
	L9A0_CTR39_T    ctr39;           // 0xfd3000ac
	L9A0_CTR40_T    ctr40;           // 0xfd3000b0
	L9A0_CTR41_T    ctr41;           // 0xfd3000b4
	L9A0_CTR42_T    ctr42;           // 0xfd3000b8
	L9A0_CTR43_T    ctr43;           // 0xfd3000bc
	L9A0_CTR44_T    ctr44;           // 0xfd3000c0
	L9A0_CTR45_T    ctr45;           // 0xfd3000c4
	L9A0_CTR46_T    ctr46;           // 0xfd3000c8
	L9A0_CTR47_T    ctr47;           // 0xfd3000cc
	L9A0_CTR48_T    ctr48;           // 0xfd3000d0
	L9A0_CTR49_T    ctr49;           // 0xfd3000d4
	L9A0_CTR50_T    ctr50;           // 0xfd3000d8
	L9A0_CTR51_T    ctr51;           // 0xfd3000dc
	L9A0_CTR52_T    ctr52;           // 0xfd3000e0
	L9A0_CTR53_T    ctr53;           // 0xfd3000e4
	L9A0_CTR54_T    ctr54;           // 0xfd3000e8
	L9A0_CTR55_T    ctr55;           // 0xfd3000ec
	L9A0_CTR56_T    ctr56;           // 0xfd3000f0
	L9A0_CTR57_T    ctr57;           // 0xfd3000f4
	L9A0_CTR58_T    ctr58;           // 0xfd3000f8
	L9A0_CTR59_T    ctr59;           // 0xfd3000fc
	L9A0_CTR60_T    ctr60;           // 0xfd300100
	L9A0_CTR61_T    ctr61;           // 0xfd300104
	L9A0_CTR62_T    ctr62;           // 0xfd300108
	L9A0_CTR63_T    ctr63;           // 0xfd30010c
	L9A0_USB0_CTRL0                       	usb0_ctrl0                       ;	// 0x0110 : ''
	L9A0_USB0_STATUS0                    	usb0_status0                    ;	// 0x0114 : ''
	L9A0_USB0_STATUS1                    	usb0_status1                    ;	// 0x0118 : ''
	L9A0_USB0_CTRL1                       	usb0_ctrl                       ;	// 0x011c : ''
	L9A0_USB0_STATUS2                    	usb0_status2                    ;	// 0x0120 : ''
	UINT32                    		reserved_0                      ;	// 0x0124 : ''
	L9A0_IO_CTRL                         	io_ctrl                         ;	// 0x0128 : ''
	UINT32                   		reserved_1                   	;	// 0x012c : ''
	UINT32                   		reserved_2                   	;	// 0x0130 : ''
	UINT32                   		reserved_3                   	;	// 0x0134 : ''
	L9A0_APB_TIME_OUT_CTRL               	apb_time_out_ctrl               ;	// 0x0138 : ''
	L9A0_APB_TIME_OUT_SRC_31_0         	apb_time_out_src_31_0         	;	// 0x013c : ''
	L9A0_APB_TIME_OUT_SRC_41_32        	apb_time_out_src_41_32        	;	// 0x0140 : ''
	UINT32                         		reserved_4                      ;	// 0x0144 : ''
	L9A0_STCC_REF_TIME_STAMP_32          	stcc_ref_time_stamp_32          ;	// 0x0148 : ''
	L9A0_STCC_REF_TIME_STAMP_31_0        	stcc_ref_time_stamp_31_0        ;	// 0x014c : ''
	L9A0_STCC_REAL_TIME_STAMP_32         	stcc_real_time_stamp_32         ;	// 0x0150 : ''
	L9A0_STCC_REAL_TIME_STAMP_31_0       	stcc_real_time_stamp_31_0       ;	// 0x0154 : ''
	L9A0_FLUSH_DONE_STATUS               	flush_done_status               ;	// 0x0158 : ''
	L9A0_FLUSH_REQ                       	flush_req                       ;	// 0x015c : ''
	UINT32                         		reserved_5                      ;	// 0x0160 : ''
	L9A0_BANDWIDTH_LIMITER_AXI0_DLY_CNT	bandwidth_limiter_axi0_dly_cnt	;	// 0x0164 : ''
	L9A0_BANDWIDTH_LIMITER_AXI1_DLY_CNT	bandwidth_limiter_axi1_dly_cnt	;	// 0x0168 : ''
	L9A0_BANDWIDTH_LIMITER_CPU_AXI0_DLY_CNT	bandwidth_limiter_cpu_axi0_dly_cnt;	// 0x016c : ''
	L9A0_BANDWIDTH_LIMITER_CPU_AXI1_DLY_CNT	bandwidth_limiter_cpu_axi1_dly_cnt;	// 0x0170 : ''
	L9A0_GPIO_INTERRUPT_STATUS           	gpio_interrupt_status           ;	// 0x0174 : ''
	L9A0_URGENCY            		urgency							;	// 0x0178 : ''
	L9A0_L2_CACHE_DRREQ_EVENT_CNT        	l2_cache_drreq_event_cnt        ;	// 0x017c : ''
	L9A0_L2_CACHE_DRHIT_EVENT_CNT        	l2_cache_drhit_event_cnt        ;	// 0x0180 : ''
	L9A0_L2_CACHE_DWREQ_EVENT_CNT        	l2_cache_dwreq_event_cnt        ;	// 0x0184 : ''
	L9A0_L2_CACHE_DWTREQ_EVENT_CNT       	l2_cache_dwtreq_event_cnt       ;	// 0x0188 : ''
	L9A0_L2_CACHE_DWHIT_EVENT_CNT        	l2_cache_dwhit_event_cnt        ;	// 0x018c : ''
	L9A0_L2_CACHE_IRREQ_EVENT_CNT        	l2_cache_irreq_event_cnt        ;	// 0x0190 : ''
	L9A0_L2_CACHE_IRHIT_EVENT_CNT        	l2_cache_irhit_event_cnt        ;	// 0x0194 : ''
	L9A0_L2_CACHE_WA_EVENT_CNT           	l2_cache_wa_event_cnt           ;	// 0x0198 : ''
	L9A0_L2_CACHE_CO_EVENT_CNT           	l2_cache_co_event_cnt           ;	// 0x019c : ''
	UINT32           			reserved_6           			;	// 0x01a0 : ''
	L9A0_L2_CACHE_EVENT_MONITOR_CTRL     	l2_cache_event_monitor_ctrl     ;	// 0x01a4 : ''
	L9A0_USB1_CTRL0                     	usb1_ctrl0                      ;	// 0x01a8 : ''
	L9A0_USB1_STATUS0                  	usb1_status0                    ;	// 0x01ac : ''
	L9A0_USB1_STATUS1                  	usb1_status1                    ;	// 0x01b0 : ''
	L9A0_USB1_CTRL1                    	usb1_ctrl1                      ;	// 0x01b4 : ''
	L9A0_USB1_STATUS2                  	usb1_status2                    ;	// 0x01b8 : ''
	L9A0_CTR64_T    ctr64;           // 0xfd3001bc
	L9A0_CTR65_T    ctr65;           // 0xfd3001c0
	L9A0_CTR66_T    ctr66;           // 0xfd3001c4
	L9A0_CTR67_T    ctr67;           // 0xfd3001c8
	L9A0_CTR68_T    ctr68;           // 0xfd3001cc
	L9A0_CTR69_T    ctr69;           // 0xfd3001d0
	L9A0_CTR70_T    ctr70;           // 0xfd3001d4
	L9A0_CTR71_T    ctr71;           // 0xfd3001d8
	L9A0_CTR72_T    ctr72;           // 0xfd3001dc
	L9A0_CTR73_T    ctr73;           // 0xfd3001e0
	L9A0_CTR74_T    ctr74;           // 0xfd3001e4
	L9A0_CTR75_T    ctr75;           // 0xfd3001e8
} CTOP_CTRL_REG_L9A0_T;


/*
 B0 CTOP
*/
typedef struct { // 0xfd300024
	UINT32							   : 29;  // 28: 0
	UINT32 slt_apbclk_sel			   : 3;   // 31:29
} L9B0_CTR05_T;

typedef struct { // 0xfd300028
	UINT32                             : 2;   //  1: 0
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
} L9B0_CTR06_T;

typedef struct {
	UINT32                          	                 __rsvd_00[   4];	// 0x0000 ~ 0x000c
	L9A0_CTR00_T    ctr00;           // 0xfd300010
	L9A0_CTR01_T    ctr01;           // 0xfd300014
	L9A0_CTR02_T    ctr02;           // 0xfd300018
	L9A0_CTR03_T    ctr03;           // 0xfd30001c
	L9A0_CTR04_T    ctr04;           // 0xfd300020
	L9B0_CTR05_T    ctr05;           // 0xfd300024
	L9B0_CTR06_T    ctr06;           // 0xfd300028
	L9A0_CTR07_T    ctr07;           // 0xfd30002c
	L9A0_CTR08_T    ctr08;           // 0xfd300030
	L9A0_CTR09_T    ctr09;           // 0xfd300034
	L9A0_CTR10_T    ctr10;           // 0xfd300038
	L9A0_CTR11_T    ctr11;           // 0xfd30003c
	L9A0_CTR12_T    ctr12;           // 0xfd300040
	L9A0_CTR13_T    ctr13;           // 0xfd300044
	L9A0_CTR14_T    ctr14;           // 0xfd300048
	L9A0_CTR15_T    ctr15;           // 0xfd30004c
	L9A0_CTR16_T    ctr16;           // 0xfd300050
	L9A0_CTR17_T    ctr17;           // 0xfd300054
	L9A0_CTR18_T    ctr18;           // 0xfd300058
	L9A0_CTR19_T    ctr19;           // 0xfd30005c
	L9A0_CTR20_T    ctr20;           // 0xfd300060
	L9A0_CTR21_T    ctr21;           // 0xfd300064
	L9A0_CTR22_T    ctr22;           // 0xfd300068
	L9A0_CTR23_T    ctr23;           // 0xfd30006c
	L9A0_CTR24_T    ctr24;           // 0xfd300070
	L9A0_CTR25_T    ctr25;           // 0xfd300074
	L9A0_CTR26_T    ctr26;           // 0xfd300078
	L9A0_CTR27_T    ctr27;           // 0xfd30007c
	L9A0_CTR28_T    ctr28;           // 0xfd300080
	L9A0_CTR29_T    ctr29;           // 0xfd300084
	L9A0_CTR30_T    ctr30;           // 0xfd300088
	L9A0_CTR31_T    ctr31;           // 0xfd30008c
	L9A0_CTR32_T    ctr32;           // 0xfd300090
	L9A0_CTR33_T    ctr33;           // 0xfd300094
	L9A0_CTR34_T    ctr34;           // 0xfd300098
	L9A0_CTR35_T    ctr35;           // 0xfd30009c
	L9A0_CTR36_T    ctr36;           // 0xfd3000a0
	L9A0_CTR37_T    ctr37;           // 0xfd3000a4
	L9A0_CTR38_T    ctr38;           // 0xfd3000a8
	L9A0_CTR39_T    ctr39;           // 0xfd3000ac
	L9A0_CTR40_T    ctr40;           // 0xfd3000b0
	L9A0_CTR41_T    ctr41;           // 0xfd3000b4
	L9A0_CTR42_T    ctr42;           // 0xfd3000b8
	L9A0_CTR43_T    ctr43;           // 0xfd3000bc
	L9A0_CTR44_T    ctr44;           // 0xfd3000c0
	L9A0_CTR45_T    ctr45;           // 0xfd3000c4
	L9A0_CTR46_T    ctr46;           // 0xfd3000c8
	L9A0_CTR47_T    ctr47;           // 0xfd3000cc
	L9A0_CTR48_T    ctr48;           // 0xfd3000d0
	L9A0_CTR49_T    ctr49;           // 0xfd3000d4
	L9A0_CTR50_T    ctr50;           // 0xfd3000d8
	L9A0_CTR51_T    ctr51;           // 0xfd3000dc
	L9A0_CTR52_T    ctr52;           // 0xfd3000e0
	L9A0_CTR53_T    ctr53;           // 0xfd3000e4
	L9A0_CTR54_T    ctr54;           // 0xfd3000e8
	L9A0_CTR55_T    ctr55;           // 0xfd3000ec
	L9A0_CTR56_T    ctr56;           // 0xfd3000f0
	L9A0_CTR57_T    ctr57;           // 0xfd3000f4
	L9A0_CTR58_T    ctr58;           // 0xfd3000f8
	L9A0_CTR59_T    ctr59;           // 0xfd3000fc
	L9A0_CTR60_T    ctr60;           // 0xfd300100
	L9A0_CTR61_T    ctr61;           // 0xfd300104
	L9A0_CTR62_T    ctr62;           // 0xfd300108
	L9A0_CTR63_T    ctr63;           // 0xfd30010c
	L9A0_USB0_CTRL0                       	usb0_ctrl0                       ;	// 0x0110 : ''
	L9A0_USB0_STATUS0                    	usb0_status0                    ;	// 0x0114 : ''
	L9A0_USB0_STATUS1                    	usb0_status1                    ;	// 0x0118 : ''
	L9A0_USB0_CTRL1                       	usb0_ctrl                       ;	// 0x011c : ''
	L9A0_USB0_STATUS2                    	usb0_status2                    ;	// 0x0120 : ''
	UINT32                    		reserved_0                      ;	// 0x0124 : ''
	L9A0_IO_CTRL                         	io_ctrl                         ;	// 0x0128 : ''
	UINT32                   		reserved_1                   	;	// 0x012c : ''
	UINT32                   		reserved_2                   	;	// 0x0130 : ''
	UINT32                   		reserved_3                   	;	// 0x0134 : ''
	L9A0_APB_TIME_OUT_CTRL               	apb_time_out_ctrl               ;	// 0x0138 : ''
	L9A0_APB_TIME_OUT_SRC_31_0         	apb_time_out_src_31_0         	;	// 0x013c : ''
	L9A0_APB_TIME_OUT_SRC_41_32        	apb_time_out_src_41_32        	;	// 0x0140 : ''
	UINT32                         		reserved_4                      ;	// 0x0144 : ''
	L9A0_STCC_REF_TIME_STAMP_32          	stcc_ref_time_stamp_32          ;	// 0x0148 : ''
	L9A0_STCC_REF_TIME_STAMP_31_0        	stcc_ref_time_stamp_31_0        ;	// 0x014c : ''
	L9A0_STCC_REAL_TIME_STAMP_32         	stcc_real_time_stamp_32         ;	// 0x0150 : ''
	L9A0_STCC_REAL_TIME_STAMP_31_0       	stcc_real_time_stamp_31_0       ;	// 0x0154 : ''
	L9A0_FLUSH_DONE_STATUS               	flush_done_status               ;	// 0x0158 : ''
	L9A0_FLUSH_REQ                       	flush_req                       ;	// 0x015c : ''
	UINT32                         		reserved_5                      ;	// 0x0160 : ''
	L9A0_BANDWIDTH_LIMITER_AXI0_DLY_CNT	bandwidth_limiter_axi0_dly_cnt	;	// 0x0164 : ''
	L9A0_BANDWIDTH_LIMITER_AXI1_DLY_CNT	bandwidth_limiter_axi1_dly_cnt	;	// 0x0168 : ''
	L9A0_BANDWIDTH_LIMITER_CPU_AXI0_DLY_CNT	bandwidth_limiter_cpu_axi0_dly_cnt;	// 0x016c : ''
	L9A0_BANDWIDTH_LIMITER_CPU_AXI1_DLY_CNT	bandwidth_limiter_cpu_axi1_dly_cnt;	// 0x0170 : ''
	L9A0_GPIO_INTERRUPT_STATUS           	gpio_interrupt_status           ;	// 0x0174 : ''
	L9A0_URGENCY            		urgency							;	// 0x0178 : ''
	L9A0_L2_CACHE_DRREQ_EVENT_CNT        	l2_cache_drreq_event_cnt        ;	// 0x017c : ''
	L9A0_L2_CACHE_DRHIT_EVENT_CNT        	l2_cache_drhit_event_cnt        ;	// 0x0180 : ''
	L9A0_L2_CACHE_DWREQ_EVENT_CNT        	l2_cache_dwreq_event_cnt        ;	// 0x0184 : ''
	L9A0_L2_CACHE_DWTREQ_EVENT_CNT       	l2_cache_dwtreq_event_cnt       ;	// 0x0188 : ''
	L9A0_L2_CACHE_DWHIT_EVENT_CNT        	l2_cache_dwhit_event_cnt        ;	// 0x018c : ''
	L9A0_L2_CACHE_IRREQ_EVENT_CNT        	l2_cache_irreq_event_cnt        ;	// 0x0190 : ''
	L9A0_L2_CACHE_IRHIT_EVENT_CNT        	l2_cache_irhit_event_cnt        ;	// 0x0194 : ''
	L9A0_L2_CACHE_WA_EVENT_CNT           	l2_cache_wa_event_cnt           ;	// 0x0198 : ''
	L9A0_L2_CACHE_CO_EVENT_CNT           	l2_cache_co_event_cnt           ;	// 0x019c : ''
	UINT32           			reserved_6           			;	// 0x01a0 : ''
	L9A0_L2_CACHE_EVENT_MONITOR_CTRL     	l2_cache_event_monitor_ctrl     ;	// 0x01a4 : ''
	L9A0_USB1_CTRL0                     	usb1_ctrl0                      ;	// 0x01a8 : ''
	L9A0_USB1_STATUS0                  	usb1_status0                    ;	// 0x01ac : ''
	L9A0_USB1_STATUS1                  	usb1_status1                    ;	// 0x01b0 : ''
	L9A0_USB1_CTRL1                    	usb1_ctrl1                      ;	// 0x01b4 : ''
	L9A0_USB1_STATUS2                  	usb1_status2                    ;	// 0x01b8 : ''
	L9A0_CTR64_T    ctr64;           // 0xfd3001bc
	L9A0_CTR65_T    ctr65;           // 0xfd3001c0
	L9A0_CTR66_T    ctr66;           // 0xfd3001c4
	L9A0_CTR67_T    ctr67;           // 0xfd3001c8
	L9A0_CTR68_T    ctr68;           // 0xfd3001cc
	L9A0_CTR69_T    ctr69;           // 0xfd3001d0
	L9A0_CTR70_T    ctr70;           // 0xfd3001d4
	L9A0_CTR71_T    ctr71;           // 0xfd3001d8
	L9A0_CTR72_T    ctr72;           // 0xfd3001dc
	L9A0_CTR73_T    ctr73;           // 0xfd3001e0
	L9A0_CTR74_T    ctr74;           // 0xfd3001e4
	L9A0_CTR75_T    ctr75;           // 0xfd3001e8	
} CTOP_CTRL_REG_L9B0_CHG_T;

#endif
