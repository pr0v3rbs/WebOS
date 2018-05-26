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

const PE_REG_PARAM_T dnr_sd_i_r_default_h13a0[] = 
{
	{0x25D8, 0x8000001F},	//[5:0]	: 1F = ALL Enable , 00 = ALL Disable
	{0x25AC, 0xFF00FF43},	//[15:8] : DC_BNR Master Gain (FF = 1.0)
	{0x25C4, 0x07804010},	//[23:16] : AC_BNR H Gain (80 = 1.0)
	{0x25C8, 0x00804010},	//[23:16] : AC_BNR V Gain (80 = 1.0)
	{0x2588, 0x0000FF35},	//[15:8] : MNR Master Gain (FF = 1.0)
	{0x25BC, 0x0103FFFD}
};
