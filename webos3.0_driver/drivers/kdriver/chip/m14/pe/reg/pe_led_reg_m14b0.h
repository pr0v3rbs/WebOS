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

#ifndef _PE_LED_REG_M14B0_H_
#define _PE_LED_REG_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0020540L led_rp_hv_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 rp_v_pos                           : 12;  //  0:11
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 rp_h_pos                           : 12;  // 27:16
} PE_M14B0_LED_RP_HV_POS;

/*-----------------------------------------------------------------------------
		0xc0020544 led_rp_make_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mark_en                            : 1 ;  //     0
} PE_M14B0_LED_RP_MAKE_EN;

/*-----------------------------------------------------------------------------
		0xc0020548 led_rp_r_value ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 rd_pixel_r                         : 14;  // 13: 0
} PE_M14B0_LED_RP_R_VALUE;

/*-----------------------------------------------------------------------------
		0xc002054c led_rp_g_value ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 rd_pixel_g                         : 14;  // 13: 0
} PE_M14B0_LED_RP_G_VALUE;

/*-----------------------------------------------------------------------------
		0xc0020550 led_rp_b_value ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 rd_pixel_b                         : 14;  // 13: 0
} PE_M14B0_LED_RP_B_VALUE;

typedef struct {
	PE_M14B0_LED_RP_HV_POS                  led_rp_hv_pos;                //0xc0020540L
	PE_M14B0_LED_RP_MAKE_EN                 led_rp_make_en;               //0xc0020544L
	PE_M14B0_LED_RP_R_VALUE                 led_rp_r_value;               //0xc0020548L
	PE_M14B0_LED_RP_G_VALUE                 led_rp_g_value;               //0xc002054cL
	PE_M14B0_LED_RP_B_VALUE                 led_rp_b_value;               //0xc0020550L
}PE_LED_REG_M14B0_T;

#endif
