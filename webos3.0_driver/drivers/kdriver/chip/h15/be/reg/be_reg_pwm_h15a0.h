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

#ifndef _REG_PWM_H15A0_h
#define _REG_PWM_H15A0_h

/*
	PE_PWM
*/

/*-----------------------------------------------------------------------------
	0x0000 pe_pwm_intr_status_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_tx_intr                    : 1,	//     0
	pwm1_tx_intr                    : 1,	//     1
	pwm2_tx_intr                    : 1,	//     2
	pwm3_rx_intr0                   : 1,	//     3
	pwm3_rx_intr1                   : 1;	//     4
} H15A0_PE_PWM_INTR_STATUS_EN;

/*-----------------------------------------------------------------------------
	0x0004 pe_pwm_intr_clear ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_tx_intr                    : 1,	//     0
	pwm1_tx_intr                    : 1,	//     1
	pwm2_tx_intr                    : 1,	//     2
	pwm3_rx_intr0                   : 1,	//     3
	pwm3_rx_intr1                   : 1;	//     4
} H15A0_PE_PWM_INTR_CLEAR;

/*-----------------------------------------------------------------------------
	0x0008 pe_pwm_intr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_tx_intr                    : 1,	//     0
	pwm1_tx_intr                    : 1,	//     1
	pwm2_tx_intr                    : 1,	//     2
	pwm3_rx_intr0                   : 1,	//     3
	pwm3_rx_intr1                   : 1;	//     4
} H15A0_PE_PWM_INTR_STATUS;

/*-----------------------------------------------------------------------------
	0x000c pe_pwm_intr_mask ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_tx_intr                    : 1,	//     0
	pwm1_tx_intr                    : 1,	//     1
	pwm2_tx_intr                    : 1,	//     2
	pwm3_rx_intr0                   : 1,	//     3
	pwm3_rx_intr1                   : 1;	//     4
} H15A0_PE_PWM_INTR_MASK;

/*-----------------------------------------------------------------------------
	0x0010 pe_pwm0_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_en                         : 1,	//     0
	pwm0_freq_mode                  : 3,	//  1: 3
	pwm0_resolution                 : 2,	//  4: 5
	pwm0_inv                        : 1,	//     6
	pwm0_sel                        : 1,	//     7
	pwm0_width_falling_pos          :24;	//  8:31
} H15A0_PE_PWM0_CTRL0;

/*-----------------------------------------------------------------------------
	0x0014 pe_pwm0_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_free_width                 :24,	//  0:23
	_rsvd                                : 4,	// 24:27 reserved
	pwm0_end_ctrl0                  : 1,	//    28
	pwm0_end_ctrl1                  : 1;	//    29
} H15A0_PE_PWM0_CTRL1;

/*-----------------------------------------------------------------------------
	0x0018 pe_pwm0_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_intr_mask                  :16,	//  0:15
	pwm0_method                     : 1,	//    16
	_rsvd_00                                : 3,	// 17:19 reserved
	pwm0_mux                        : 3,	// 20:22
	_rsvd_01                                : 1,	//    23 reserved
	pwm0_phase_mode                 : 2,	// 24:25
	pwm0_phase_index                : 3,	// 26:28
	_rsvd_02                                : 1,	//    29 reserved
	pwm0_fc_h_disp                  : 1,	//    30
	pwm0_fc_l_disp                  : 1;	//    31
} H15A0_PE_PWM0_CTRL2;

/*-----------------------------------------------------------------------------
	0x001c pe_pwm1_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_en                         : 1,	//     0
	pwm1_freq_mode                  : 3,	//  1: 3
	pwm1_resolution                 : 2,	//  4: 5
	pwm1_inv                        : 1,	//     6
	pwm1_sel                        : 1,	//     7
	pwm1_width_falling_pos          :24;	//  8:31
} H15A0_PE_PWM1_CTRL0;

/*-----------------------------------------------------------------------------
	0x0020 pe_pwm1_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_free_width                 :24,	//  0:23
	_rsvd                                : 4,	// 24:27 reserved
	pwm1_end_ctrl0                  : 1,	//    28
	pwm1_end_ctrl1                  : 1;	//    29
} H15A0_PE_PWM1_CTRL1;

/*-----------------------------------------------------------------------------
	0x0024 pe_pwm1_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_intr_mask                  :16,	//  0:15
	pwm1_method                     : 1,	//    16
	_rsvd_00                                : 3,	// 17:19 reserved
	pwm1_mux                        : 3,	// 20:22
	_rsvd_01                                : 1,	//    23 reserved
	pwm1_phase_mode                 : 2,	// 24:25
	pwm1_phase_index                : 3,	// 26:28
	_rsvd_02                                : 1,	//    29 reserved
	pwm1_fc_h_disp                  : 1,	//    30
	pwm1_fc_l_disp                  : 1;	//    31
} H15A0_PE_PWM1_CTRL2;

/*-----------------------------------------------------------------------------
	0x0028 pe_pwm2_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_en                         : 1,	//     0
	pwm2_freq_mode                  : 3,	//  1: 3
	pwm2_resolution                 : 2,	//  4: 5
	pwm2_inv                        : 1,	//     6
	pwm2_sel                        : 1,	//     7
	pwm2_width_falling_pos          :24;	//  8:31
} H15A0_PE_PWM2_CTRL0;

/*-----------------------------------------------------------------------------
	0x002c pe_pwm2_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_free_width                 :24,	//  0:23
	_rsvd                                : 4,	// 24:27 reserved
	pwm2_end_ctrl0                  : 1,	//    28
	pwm2_end_ctrl1                  : 1;	//    29
} H15A0_PE_PWM2_CTRL1;

/*-----------------------------------------------------------------------------
	0x0030 pe_pwm2_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_intr_mask                  :16,	//  0:15
	pwm2_method                     : 1,	//    16
	_rsvd_00                                : 3,	// 17:19 reserved
	pwm2_mux                        : 3,	// 20:22
	_rsvd_01                                : 1,	//    23 reserved
	pwm2_phase_mode                 : 2,	// 24:25
	pwm2_phase_index                : 3,	// 26:28
	_rsvd_02                                : 1,	//    29 reserved
	pwm2_fc_h_disp                  : 1,	//    30
	pwm2_fc_l_disp                  : 1;	//    31
} H15A0_PE_PWM2_CTRL2;

/*-----------------------------------------------------------------------------
	0x0034 pe_pwm3_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_en                         : 1,	//     0
	pwm3_freq_mode                  : 3,	//  1: 3
	_rsvd                                : 4,	//  4: 7 reserved
	pwm3_threshold                  :24;	//  8:31
} H15A0_PE_PWM3_CTRL0;

/*-----------------------------------------------------------------------------
	0x0038 pe_pwm3_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_intr_mask1                 : 8,	//  0: 7
	_rsvd_00                                : 8,	//  8:15 reserved
	pwm3_intr_mask0                 : 1,	//    16
	_rsvd_01                                : 3,	// 17:19 reserved
	pwm3_in_mux                     : 2;	// 20:21
} H15A0_PE_PWM3_CTRL1;

/*-----------------------------------------------------------------------------
	0x003c pe_pwm3_0_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_0_low                      :24;	//  0:23
} H15A0_PE_PWM3_0_LOW;

/*-----------------------------------------------------------------------------
	0x0040 pe_pwm3_0_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_0_high                     :24;	//  0:23
} H15A0_PE_PWM3_0_HIGH;

/*-----------------------------------------------------------------------------
	0x0044 pe_pwm3_1_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_1_low                      :24;	//  0:23
} H15A0_PE_PWM3_1_LOW;

/*-----------------------------------------------------------------------------
	0x0048 pe_pwm3_1_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_1_high                     :24;	//  0:23
} H15A0_PE_PWM3_1_HIGH;

/*-----------------------------------------------------------------------------
	0x004c pe_pwm3_2_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_2_low                      :24;	//  0:23
} H15A0_PE_PWM3_2_LOW;

/*-----------------------------------------------------------------------------
	0x0050 pe_pwm3_2_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_2_high                     :24;	//  0:23
} H15A0_PE_PWM3_2_HIGH;

/*-----------------------------------------------------------------------------
	0x0054 pe_pwm3_3_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_3_low                      :24;	//  0:23
} H15A0_PE_PWM3_3_LOW;

/*-----------------------------------------------------------------------------
	0x0058 pe_pwm3_3_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_3_high                     :24;	//  0:23
} H15A0_PE_PWM3_3_HIGH;

/*-----------------------------------------------------------------------------
	0x005c pe_pwm3_4_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_4_low                      :24;	//  0:23
} H15A0_PE_PWM3_4_LOW;

/*-----------------------------------------------------------------------------
	0x0060 pe_pwm3_4_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_4_high                     :24;	//  0:23
} H15A0_PE_PWM3_4_HIGH;

/*-----------------------------------------------------------------------------
	0x0064 pe_pwm3_5_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_5_low                      :24;	//  0:23
} H15A0_PE_PWM3_5_LOW;

/*-----------------------------------------------------------------------------
	0x0068 pe_pwm3_5_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_5_high                     :24;	//  0:23
} H15A0_PE_PWM3_5_HIGH;

/*-----------------------------------------------------------------------------
	0x006c pe_pwm3_6_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_6_low                      :24;	//  0:23
} H15A0_PE_PWM3_6_LOW;

/*-----------------------------------------------------------------------------
	0x0070 pe_pwm3_6_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_6_high                     :24;	//  0:23
} H15A0_PE_PWM3_6_HIGH;

/*-----------------------------------------------------------------------------
	0x0074 pe_pwm3_7_low ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_7_low                      :24;	//  0:23
} H15A0_PE_PWM3_7_LOW;

/*-----------------------------------------------------------------------------
	0x0078 pe_pwm3_7_high ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_7_high                     :24;	//  0:23
} H15A0_PE_PWM3_7_HIGH;

/*-----------------------------------------------------------------------------
	0x007c pe_pwm_v_load_write ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_we                       : 1,	//     0
	pwm1_v_we                       : 1,	//     1
	pwm2_v_we                       : 1,	//     2
	pwm0_v_sub_we                   : 1,	//     3
	pwm1_v_sub_we                   : 1,	//     4
	pwm2_v_sub_we                   : 1,	//     5
	_rsvd                                : 2,	//  6: 7 reserved
	pwm0_v_sub_f_we                 : 1,	//     8
	pwm1_v_sub_f_we                 : 1,	//     9
	pwm2_v_sub_f_we                 : 1;	//    10
} H15A0_PE_PWM_V_LOAD_WRITE;

/*-----------------------------------------------------------------------------
	0x0080 pe_pwm0_v_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_r                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm0_v_r_id                     : 4;	// 28:31
} H15A0_PE_PWM0_V_R;

/*-----------------------------------------------------------------------------
	0x0084 pe_pwm0_v_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_f                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm0_v_f_id                     : 4;	// 28:31
} H15A0_PE_PWM0_V_F;

/*-----------------------------------------------------------------------------
	0x0088 pe_pwm1_v_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_v_r                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm1_v_r_id                     : 4;	// 28:31
} H15A0_PE_PWM1_V_R;

/*-----------------------------------------------------------------------------
	0x008c pe_pwm1_v_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_v_f                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm1_v_f_id                     : 4;	// 28:31
} H15A0_PE_PWM1_V_F;

/*-----------------------------------------------------------------------------
	0x0090 pe_pwm2_v_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_v_r                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm2_v_r_id                     : 4;	// 28:31
} H15A0_PE_PWM2_V_R;

/*-----------------------------------------------------------------------------
	0x0094 pe_pwm2_v_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_v_f                        :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm2_v_f_id                     : 4;	// 28:31
} H15A0_PE_PWM2_V_F;

/*-----------------------------------------------------------------------------
	0x0098 pe_pwm0_v_sub_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_sub                      :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm0_v_sub_id                   : 4;	// 28:31
} H15A0_PE_PWM0_V_SUB_R;

/*-----------------------------------------------------------------------------
	0x009c pe_pwm1_v_sub_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_v_sub                      :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm1_v_sub_id                   : 4;	// 28:31
} H15A0_PE_PWM1_V_SUB_R;

/*-----------------------------------------------------------------------------
	0x00a0 pe_pwm2_v_sub_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_v_sub                      :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm2_v_sub_id                   : 4;	// 28:31
} H15A0_PE_PWM2_V_SUB_R;

/*-----------------------------------------------------------------------------
	0x00a4 pe_pwm0_lock_pulse_xy ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_lock_pulse_x               :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	pwm0_lock_pulse_y               :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	pwm0_lock_pulse_half_en         : 1;	//    31
} H15A0_PE_PWM0_LOCK_PULSE_XY;

/*-----------------------------------------------------------------------------
	0x00a8 pe_pwm1_lock_pulse_xy ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_lock_pulse_x               :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	pwm1_lock_pulse_y               :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	pwm1_lock_pulse_half_en         : 1;	//    31
} H15A0_PE_PWM1_LOCK_PULSE_XY;

/*-----------------------------------------------------------------------------
	0x00ac pe_pwm2_lock_pulse_xy ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_lock_pulse_x               :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	pwm2_lock_pulse_y               :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	pwm2_lock_pulse_half_en         : 1;	//    31
} H15A0_PE_PWM2_LOCK_PULSE_XY;

/*-----------------------------------------------------------------------------
	0x00b0 pe_pwm3_lock_pulse_xy ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm3_lock_pulse_x               :13,	//  0:12
	_rsvd_00                                : 3,	// 13:15 reserved
	pwm3_lock_pulse_y               :13,	// 16:28
	_rsvd_01                                : 2,	// 29:30 reserved
	pwm3_lock_pulse_half_en         : 1;	//    31
} H15A0_PE_PWM3_LOCK_PULSE_XY;

/*-----------------------------------------------------------------------------
	0x00b4 pe_pwm0_width ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00b8 pe_pwm0_offset ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00bc pe_pwm0_offset_sum ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00c0 pe_pwm1_width ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00c4 pe_pwm1_offset ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00c8 pe_pwm1_offset_sum ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00cc pe_pwm2_width ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00d0 pe_pwm2_offset ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00d4 pe_pwm2_offset_sum ''
------------------------------------------------------------------------------*/
/*	no field */

/*-----------------------------------------------------------------------------
	0x00d8 pe_pwm_master ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	_rsvd_00                                :16,	//  0:15 reserved
	pwm_master_mode                 : 3,	// 16:18
	_rsvd_01                                : 1,	//    19 reserved
	pwm2_ld_pwm                     : 1,	//    20
	_rsvd_02                                : 2,	// 21:22 reserved
	pwm2_ld_pwm_tx_mode             : 1,	//    23
	pwm1_ld_pwm                     : 1,	//    24
	_rsvd_03                                : 2,	// 25:26 reserved
	pwm1_ld_pwm_tx_mode             : 1,	//    27
	pwm0_ld_pwm                     : 1,	//    28
	_rsvd_04                                : 2,	// 29:30 reserved
	pwm0_ld_pwm_tx_mode             : 1;	//    31
} H15A0_PE_PWM_MASTER;

/*-----------------------------------------------------------------------------
	0x00dc pe_pwm0_v_sub_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm0_v_sub_f                    :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm0_v_sub_f_id                 : 4;	// 28:31
} H15A0_PE_PWM0_V_SUB_F;

/*-----------------------------------------------------------------------------
	0x00e0 pe_pwm1_v_sub_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm1_v_sub_f                    :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm1_v_sub_f_id                 : 4;	// 28:31
} H15A0_PE_PWM1_V_SUB_F;

/*-----------------------------------------------------------------------------
	0x00e4 pe_pwm2_v_sub_f ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pwm2_v_sub_f                    :16,	//  0:15
	_rsvd                                :12,	// 16:27 reserved
	pwm2_v_sub_f_id                 : 4;	// 28:31
} H15A0_PE_PWM2_V_SUB_F;

/*-----------------------------------------------------------------------------
	0x00ec pe_pwm_sw_reset_lock ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sw_reset_lock					: 1;  //     0
} H15A0_PE_PWM_SW_RESET_LOCK_T;

typedef struct {
	H15A0_PE_PWM_INTR_STATUS_EN      	pe_pwm_intr_status_en              ;	// 0x0000 : ''
	H15A0_PE_PWM_INTR_CLEAR              pe_pwm_intr_clear                  ;	// 0x0004 : ''
	H15A0_PE_PWM_INTR_STATUS           	pe_pwm_intr_status                 ;	// 0x0008 : ''
	H15A0_PE_PWM_INTR_MASK               	pe_pwm_intr_mask                   ;	// 0x000c : ''
	H15A0_PE_PWM0_CTRL0                     	pe_pwm0_ctrl0                      ;	// 0x0010 : ''
	H15A0_PE_PWM0_CTRL1                     	pe_pwm0_ctrl1                      ;	// 0x0014 : ''
	H15A0_PE_PWM0_CTRL2                     	pe_pwm0_ctrl2                      ;	// 0x0018 : ''
	H15A0_PE_PWM1_CTRL0                    	pe_pwm1_ctrl0                      ;	// 0x001c : ''
	H15A0_PE_PWM1_CTRL1                    	pe_pwm1_ctrl1                      ;	// 0x0020 : ''
	H15A0_PE_PWM1_CTRL2                    	pe_pwm1_ctrl2                      ;	// 0x0024 : ''
	H15A0_PE_PWM2_CTRL0                    	pe_pwm2_ctrl0                      ;	// 0x0028 : ''
	H15A0_PE_PWM2_CTRL1                    	pe_pwm2_ctrl1                      ;	// 0x002c : ''
	H15A0_PE_PWM2_CTRL2                     	pe_pwm2_ctrl2                      ;	// 0x0030 : ''
	H15A0_PE_PWM3_CTRL0                     	pe_pwm3_ctrl0                      ;	// 0x0034 : ''
	H15A0_PE_PWM3_CTRL1                    	pe_pwm3_ctrl1                      ;	// 0x0038 : ''
	H15A0_PE_PWM3_0_LOW                   	pe_pwm3_0_low                      ;	// 0x003c : ''
	H15A0_PE_PWM3_0_HIGH                   	pe_pwm3_0_high                     ;	// 0x0040 : ''
	H15A0_PE_PWM3_1_LOW                    	pe_pwm3_1_low                      ;	// 0x0044 : ''
	H15A0_PE_PWM3_1_HIGH                   	pe_pwm3_1_high                     ;	// 0x0048 : ''
	H15A0_PE_PWM3_2_LOW                   	pe_pwm3_2_low                      ;	// 0x004c : ''
	H15A0_PE_PWM3_2_HIGH                   	pe_pwm3_2_high                     ;	// 0x0050 : ''
	H15A0_PE_PWM3_3_LOW                    	pe_pwm3_3_low                      ;	// 0x0054 : ''
	H15A0_PE_PWM3_3_HIGH                  	pe_pwm3_3_high                     ;	// 0x0058 : ''
	H15A0_PE_PWM3_4_LOW                    	pe_pwm3_4_low                      ;	// 0x005c : ''
	H15A0_PE_PWM3_4_HIGH                  	pe_pwm3_4_high                     ;	// 0x0060 : ''
	H15A0_PE_PWM3_5_LOW                    	pe_pwm3_5_low                      ;	// 0x0064 : ''
	H15A0_PE_PWM3_5_HIGH                   	pe_pwm3_5_high                     ;	// 0x0068 : ''
	H15A0_PE_PWM3_6_LOW                    	pe_pwm3_6_low                      ;	// 0x006c : ''
	H15A0_PE_PWM3_6_HIGH                   	pe_pwm3_6_high                     ;	// 0x0070 : ''
	H15A0_PE_PWM3_7_LOW                    	pe_pwm3_7_low                      ;	// 0x0074 : ''
	H15A0_PE_PWM3_7_HIGH                   	pe_pwm3_7_high                     ;	// 0x0078 : ''
	H15A0_PE_PWM_V_LOAD_WRITE       	pe_pwm_v_load_write                ;	// 0x007c : ''
	H15A0_PE_PWM0_V_R                       	pe_pwm0_v_r                        ;	// 0x0080 : ''
	H15A0_PE_PWM0_V_F                        	pe_pwm0_v_f                        ;	// 0x0084 : ''
	H15A0_PE_PWM1_V_R                      	pe_pwm1_v_r                        ;	// 0x0088 : ''
	H15A0_PE_PWM1_V_F                        	pe_pwm1_v_f                        ;	// 0x008c : ''
	H15A0_PE_PWM2_V_R                        	pe_pwm2_v_r                        ;	// 0x0090 : ''
	H15A0_PE_PWM2_V_F                        	pe_pwm2_v_f                        ;	// 0x0094 : ''
	H15A0_PE_PWM0_V_SUB_R                	pe_pwm0_v_sub_r                    ;	// 0x0098 : ''
	H15A0_PE_PWM1_V_SUB_R               	pe_pwm1_v_sub_r                    ;	// 0x009c : ''
	H15A0_PE_PWM2_V_SUB_R              	pe_pwm2_v_sub_r                    ;	// 0x00a0 : ''
	H15A0_PE_PWM0_LOCK_PULSE_XY    	pe_pwm0_lock_pulse_xy              ;	// 0x00a4 : ''
	H15A0_PE_PWM1_LOCK_PULSE_XY     	pe_pwm1_lock_pulse_xy              ;	// 0x00a8 : ''
	H15A0_PE_PWM2_LOCK_PULSE_XY     	pe_pwm2_lock_pulse_xy              ;	// 0x00ac : ''
	H15A0_PE_PWM3_LOCK_PULSE_XY    	pe_pwm3_lock_pulse_xy              ;	// 0x00b0 : ''
	UINT32                          		pe_pwm0_width                      ;	// 0x00b4 : ''
	UINT32                          		pe_pwm0_offset                     ;	// 0x00b8 : ''
	UINT32                          		pe_pwm0_offset_sum                 ;	// 0x00bc : ''
	UINT32                          		pe_pwm1_width                      ;	// 0x00c0 : ''
	UINT32                          		pe_pwm1_offset                     ;	// 0x00c4 : ''
	UINT32                          		pe_pwm1_offset_sum                 ;	// 0x00c8 : ''
	UINT32                          		pe_pwm2_width                      ;	// 0x00cc : ''
	UINT32                          		pe_pwm2_offset                     ;	// 0x00d0 : ''
	UINT32                          		pe_pwm2_offset_sum                 ;	// 0x00d4 : ''
	H15A0_PE_PWM_MASTER                    	pe_pwm_master                      ;	// 0x00d8 : ''
	H15A0_PE_PWM0_V_SUB_F                 	pe_pwm0_v_sub_f                    ;	// 0x00dc : ''
	H15A0_PE_PWM1_V_SUB_F                	pe_pwm1_v_sub_f                    ;	// 0x00e0 : ''
	H15A0_PE_PWM2_V_SUB_F                	pe_pwm2_v_sub_f                    ;	// 0x00e4 : ''
	UINT32                          	    _rsvd_00[   1]					   ;	// 0x00e8
	H15A0_PE_PWM_SW_RESET_LOCK_T            pe_pwm_sw_reset_lock               ;	// 0x00ec : ''
} PE_PWM_REG_H15A0_T;

#endif //#ifndef _REG_PWM_H15A0_h