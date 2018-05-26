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

const PE_REG_PARAM_T dnr_l_sd_default_h15a0[] =
{
	/* dnr, 0x7194~0x7198, 0x71A0~0x71DC, 0x71E4~0x71E8, 0x71F0~0x71F8, 0x7200~0x7210, 0x726C~0x728C */
	// dnr max
	{0x7194, 0x00380001}, // DNR_MAX_CTRL
	{0x7198, 0x00000000}, // DNR_DBAR_CTRL
	// mnr
	{0x71A0, 0x00FFFF21}, // MNR_CTRL_0, revised by smhong
	{0x71A4, 0x44480024}, // MNR_CTRL_1
	{0x71A8, 0xC0C08884}, // MNR_CTRL_2
	{0x71AC, 0x00FF2838}, // MNR_CTRL_3
	{0x71B0, 0x30100000}, // MNR_CTRL_4
	{0x71B4, 0x00FF2030}, // MNR_CTRL_5
	// detail
	{0x71B8, 0x04040A40}, // DETAIL_CTRL
	// dc bnr
	{0x71BC, 0xFFFF0005}, // DC_BNR_CTRL_0
	{0x71C0, 0x5251010D}, // DC_BNR_CTRL_1
	{0x71C4, 0x0A2080AD}, // DC_BNR_CTRL_2
	{0x71C8, 0x15254080}, // DC_BNR_CTRL_3
	{0x71CC, 0x00A0FF00},   //DC_BNR_CTRL_4	// 14.10.28
	{0x71D0, 0x004080FF}, // DC_BNR_CTRL_5
	// ac bnr
	{0x71D4, 0xB30B0B4F}, // AC_BNR_CTRL_0
	{0x71D8, 0x0F10300B}, // AC_BNR_CTRL_1, revised by smhong
	{0x71DC, 0x0A103050}, // AC_BNR_CTRL_2, revised by smhong
	{0x71E4, 0x0060A0FF}, // AC_BNR_CTRL_3, revised by smhong
	{0x71E8, 0x103050D8}, // AC_BNR_CTRL_4, revised by smhong
	{0x71F0, 0xFA960000}, // AC_BNR_CTRL_5
	{0x71F4, 0x00FF8040}, // AC_BNR_CTRL_6
	{0x71F8, 0x00808040}, // AC_BNR_CTRL_7
	{0x7200, 0x00000F20}, // AC_BNR_CTRL_8
	{0x7204, 0x00098000}, // AC_BNR_CTRL_9, revised by smhong
	{0x7208, 0xFF00FF00}, // AC_BNR_CTRL_10
	{0x720C, 0xFF00FF00}, // AC_BNR_CTRL_11
	{0x7210, 0x0000FFFF}, // AC_BNR_CTRL_12, revised by smhong
	// sc bnr
	{0x726C, 0x00000000}, // SC_BNR_CTRL_0
	{0x7270, 0x00000000}, // SC_BNR_CTRL_1
	{0x7274, 0x00000000}, // SC_BNR_CTRL_2
	{0x7278, 0x00000000}, // SC_BNR_CTRL_3
	{0x727C, 0x00000000}, // SC_BNR_CTRL_4
	{0x7280, 0x00000000}, // SC_BNR_CTRL_5
	{0x7284, 0x00000000}, // SC_BNR_CTRL_6
	// texture
	{0x728C, 0x10000400}, // TEXTURE_CTRL_0, revised by smhong
};