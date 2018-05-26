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

const PE_REG_PARAM_T tnr_l_crg_init_ia_h13b0[] =
{
	{0x04F0, 0x00001000},
	// region 0: skin color
	{0x04F4, 0x00577D10},	//0x00578210->0x00577D10, changed by sy.sohn, 20130609
	{0x04F4, 0x00618760},	//0x00618C60->0x00618760, changed by sy.sohn, 20130609
	{0x04F4, 0x0078A4B4},	//0x00799470->0x0078A4B4, changed by sy.sohn, 20130609
	{0x04F4, 0x0093AEC8},	//0x00839EB5->0x0093AEC8, changed by sy.sohn, 20130609
	// region 1: green (leaf, grass)
	{0x04F4, 0x0001101A},
	{0x04F4, 0x00304038},
	{0x04F4, 0x006080A0},
	{0x04F4, 0x0080A0BE},
	// region 2: all region
	{0x04F4, 0x00010101},
	{0x04F4, 0x00020202},
	{0x04F4, 0x00030303},
	{0x04F4, 0x00040404},
	// region 3: all region
	{0x04F4, 0x00010101},
	{0x04F4, 0x00020202},
	{0x04F4, 0x00030303},
	{0x04F4, 0x00040404},
	// color region gain
	{0x04AC, 0x20202020}
};
