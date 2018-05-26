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

const PE_REG_PARAM_T clc_l_init_h15a0[] = 
{
	/* nd dnr1 clc, 0x4260~0x4278, 0x436C*/
	{0x4260, 0x301F10F0},	//CLC_CTRL_00
	{0x4264, 0xE0120525},	//CLC_CTRL_01
	{0x4268, 0x19000985},	//CLC_CTRL_02
	{0x426C, 0xC030F030},	//CLC_CTRL_03
	{0x4270, 0x00000020},	//CLC_CTRL_04
	{0x4274, 0x00008000},	//CLC_CTRL_05
	{0x4278, 0xC000BC00},	//CLC_CTRL_06
	{0x436C, 0x00004589}	//CLC_CTRL_07
};
