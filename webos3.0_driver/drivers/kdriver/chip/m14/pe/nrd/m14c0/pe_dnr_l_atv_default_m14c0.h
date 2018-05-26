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

const PE_REG_PARAM_T dnr_l_atv_default_m14c0[] =
{
	/* pe0d dnr, 0x3A94~0x3ADC, 0x3AE4~0x3AE8, 0x3AF0~0x3AF8, 0x3B00~0x3B10 */
	// dnr max
	{0x3A94, 0x00F80003}, // DNR_MAX_CTRL
	{0x3A98, 0x00000000}, // DNR_DBAR_CTRL
	{0x3A9C, 0x00000000}, // REG_420C_CTRL
	// mnr
	{0x3AA0, 0x00FFFF31}, // MNR_CTRL_0
	{0x3AA4, 0x44480024}, // MNR_CTRL_1
	{0x3AA8, 0xC0C08884}, // MNR_CTRL_2
	{0x3AAC, 0x00FF2838}, // MNR_CTRL_3
	{0x3AB0, 0x30100000}, // MNR_CTRL_4
	{0x3AB4, 0x00FF2030}, // MNR_CTRL_5
	// detail
	{0x3AB8, 0x04040A40}, // DETAIL_CTRL
	// dc bnr
	{0x3ABC, 0xFFFF0005}, // DC_BNR_CTRL_0
	{0x3AC0, 0x5251010D}, // DC_BNR_CTRL_1
	{0x3AC4, 0x0A2080AD}, // DC_BNR_CTRL_2
	{0x3AC8, 0x15254080}, // DC_BNR_CTRL_3
	{0x3ACC, 0x00FFFF00}, // DC_BNR_CTRL_4
	{0x3AD0, 0x004080FF}, // DC_BNR_CTRL_5
	// ac bnr
	{0x3AD4, 0xB30B0B4F}, // AC_BNR_CTRL_0
	{0x3AD8, 0x0F19370B}, // AC_BNR_CTRL_1
	{0x3ADC, 0x0A0A0A0A}, // AC_BNR_CTRL_2
	{0x3AE4, 0x14145AFA}, // AC_BNR_CTRL_3
	{0x3AE8, 0x0A2050D8}, // AC_BNR_CTRL_4
	{0x3AF0, 0xFA960000}, // AC_BNR_CTRL_5
	{0x3AF4, 0x00FF8040}, // AC_BNR_CTRL_6
	{0x3AF8, 0x00808040}, // AC_BNR_CTRL_7
	{0x3B00, 0x00000F20}, // AC_BNR_CTRL_8
	{0x3B04, 0x00008000}, // AC_BNR_CTRL_9
	{0x3B08, 0xFF00FF00}, // AC_BNR_CTRL_10
	{0x3B0C, 0xFF00FF00}, // AC_BNR_CTRL_11
	{0x3B10, 0x0000FF00}, // AC_BNR_CTRL_12
};