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


static const CVD_REG_PARAM_T cvd_rf_ntsc_m_black_level_low[] = 
{
//    {0x43DC, 0x0F0007FE},   //0x0F7B9042 OADJ NTSC-J
//    {0x43E0, 0x00000EF1},	//0x00000F48
//    {0x43E4, 0x00600203},	
//    {0x43E8, 0x10001000},
//    {0x43EC, 0x06002030}    
    {0x43DC, 0x0F0007FC},   // OADJ NTSC-M
    {0x43E0, 0x00000EF5},	
    {0x43E4, 0x00600203},	
    {0x43E8, 0x10001000},
    {0x43EC, 0x06002030}    
};
