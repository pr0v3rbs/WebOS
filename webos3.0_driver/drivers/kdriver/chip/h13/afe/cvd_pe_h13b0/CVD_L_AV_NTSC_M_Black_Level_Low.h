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


static const CVD_REG_PARAM_T cvd_av_ntsc_m_black_level_low[] = 
{
    //{0x43DC, 0x0F7C003F},   // NTSC-J OADJ
    //{0x43E0, 0x00000EEC},
    //{0x43E4, 0x00600202},
    //{0x43E8, 0x10001000},
    //{0x43EC, 0x06002020}
    {0x43DC, 0x0F7C003C},   // NTSC-M OADJ
    {0x43E0, 0x00000EF8},
    {0x43E4, 0x00600202},
    {0x43E8, 0x10001000},
    {0x43EC, 0x06002020}
};
