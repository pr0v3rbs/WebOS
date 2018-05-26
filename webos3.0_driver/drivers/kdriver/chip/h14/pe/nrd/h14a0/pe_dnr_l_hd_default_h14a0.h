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

const PE_REG_PARAM_T dnr_hd_l_default_h14a0[] = 
{
	/* dnr 0x0588~0x05A0, 0x05AC~0x05E0, 0x0688~0x068C, 0x069C*/
	{0x0588, 0x0000FF35},
	{0x058C, 0x44480024},	// MNR Second Ripel Ratio B0 -> 48
	{0x0590, 0x80D08884},	// MNR V Gain 80 -> D0
	{0x0594, 0x00FF2838},
	{0x0598, 0x30102D21},
	{0x059C, 0x00FF2030},
	
	// bnr                                                      
	{0x05A0, 0x0F0F0A50},	// 	BNR_DETAIL_CTRL                 
	{0x05AC, 0xFF200005},   //	BNR_DC_CTRL_00                  
	{0x05B0, 0x5251010D},   //  BNR_DC_CTRL_01                  
	{0x05B4, 0x80FF8040},   //  BNR_AC_CTRL_07                  
	{0x05B8, 0x000AC668},   //  BNR_AC_CTRL_08                  
	{0x05BC, 0x00FFC0C0},   //  BNR_AC_CTRL_06                  
	{0x05C0, 0x4D0B0B4F},   //  BNR_AC_CTRL_00                  
	{0x05C4, 0x0F19370A},   //  BNR_AC_CTRL_01                  
	{0x05C8, 0x0A0A0A0A},   //  BNR_AC_CTRL_02                  
	{0x05CC, 0x14145AFA},   //  BNR_AC_CTRL_03                  
	{0x05D0, 0x0A2050D8},   //  BNR_AC_CTRL_04                  
	{0x05D4, 0xFA963201},   //  BNR_AC_CTRL_05                  
	{0x05D8, 0x00180000},   //  MAX_CTRL (ac_bnr,dc_bnr,mnr off)
	{0x0688, 0x0AFF80AD},   //  BNR_DC_CTRL_02                  
	{0x068C, 0x15254080},   //  BNR_DC_CTRL_03                  
	{0x069C, 0x43218071}   	//  BNR_AC_CTRL_09                  
};