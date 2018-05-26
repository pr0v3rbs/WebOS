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

const PE_REG_PARAM_T cti_p0d_l_pc_default_m14b0[] =
{
	/* pe0d cti, 0x3A8C~0x3A90 */
	{0x3A8C, 0x00008000},   // CTI_CTRL_0
                        	// [    0]=0x0(0) : cti_en
                        	// [    1]=0x0(0) : cti_clipping_mode
                        	// [ 3: 2]=0x0(0) : cti_pre_flt_mode
                        	// [    4]=0x0(0) : cti_tap
                        	// [15: 8]=0x80(128) : cti_gain
                        	// [18:16]=0x0(0) : cti_debug_mode
	{0x3A90, 0x00232000}   	// CTI_CTRL_1
                        	// [ 7: 0]=0x0(0) : cti_coring_th0
                        	// [15: 8]=0x20(32) : cti_coring_th1
                        	// [18:16]=0x3(3) : cti_coring_smooth
                        	// [21:20]=0x2(2) : reg_cti_tap2
};
