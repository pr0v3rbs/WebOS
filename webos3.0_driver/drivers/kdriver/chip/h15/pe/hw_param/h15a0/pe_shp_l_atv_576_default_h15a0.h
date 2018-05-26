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

const PE_REG_PARAM_T shp_l_atv_576_default_h15a0[] =
{
	{0xD4B0, 0x0A030119},   //PE1_DERH_CTRL_00
	{0xD4B4, 0x02084040},   //PE1_DERH_CTRL_01
	{0xD4B8, 0x0801F808},   //PE1_DERH_CTRL_02
	{0xD4BC, 0xC80C0000},   //PE1_DERH_CTRL_03
	{0xD4C0, 0x0000FF01},   //PE1_DERH_CTRL_04
	{0xD4C4, 0x1E100001},   //PE1_DERH_CTRL_05
	{0xD4C8, 0xC1040401},   //PE1_DERH_CTRL_06
	{0xD4CC, 0x60000200},   //PE1_DERH_CTRL_07
	{0xD4D0, 0x30FF1010},   //PE1_DERH_CTRL_08
	{0xD4D4, 0x00004020},   //PE1_DERH_CTRL_09
	{0xD4D8, 0x06101400},   //PE1_DERH_CTRL_0A
	{0xD4DC, 0x07036020},   //PE1_DERH_CTRL_0B
	{0xD4E0, 0x00002030},   //PE1_DERH_CTRL_0C
	{0xD4E4, 0xFFFFC0C0},   //PE1_CORING_CTRL_10
	{0xD4F0, 0x0018200A},   //PE1_DERV_CTRL_0
	{0xD4F4, 0x00000008},   //PE1_DERV_CTRL_1
	{0xD4F8, 0x44280000},   //PE1_DERV_CTRL_2
	{0xD4FC, 0x00002828},   //PE1_DERV_CTRL_3
	{0xD510, 0x00000000},   //PE1_DP_CTRL_00
	{0xD514, 0xC0C02028},   //PE1_DP_CTRL_01
	{0xD518, 0xB0B00C10},   //PE1_DP_CTRL_02
	{0xD51C, 0x80800000},   //PE1_DP_CTRL_03
	{0xD530, 0x07000001},   //PE1_SP_CTRL_00
	{0xD534, 0x08202828},   //PE1_SP_CTRL_01
	{0xD538, 0x2001A020},   //PE1_SP_CTRL_02
	{0xD53C, 0x00000000},   //PE1_SP_CTRL_03
	{0xD540, 0x01401000},   //PE1_SP_CTRL_04
	{0xD544, 0x00000000},   //PE1_SP_CTRL_05
	{0xD548, 0x00001013},   //PE1_SP_CTRL_06
	{0xD54C, 0x00020101},   //PE1_SP_CTRL_07
	{0xD550, 0x00C00C08},   //PE1_SP_CTRL_08
	{0xD554, 0xC0806040},   //PE1_SP_CTRL_09
	{0xD558, 0x40E08040},   //PE1_SP_CTRL_0A
	{0xD55C, 0x40C08060},   //PE1_SP_CTRL_0B
	{0xD560, 0x0000E080},   //PE1_SP_CTRL_0C
	{0xD570, 0x07000001},   //PE1_MP_CTRL_00
	{0xD574, 0x10202020},   //PE1_MP_CTRL_01
	{0xD578, 0x2001A040},   //PE1_MP_CTRL_02
	{0xD57C, 0x00000000},   //PE1_MP_CTRL_03
	{0xD580, 0x01401000},   //PE1_MP_CTRL_04
	{0xD584, 0x00000000},   //PE1_MP_CTRL_05
	{0xD588, 0x00000022},   //PE1_MP_CTRL_06
	{0xD58C, 0x00020101},   //PE1_MP_CTRL_07
	{0xD590, 0x00C00C08},   //PE1_MP_CTRL_08
	{0xD594, 0xC0A06040},   //PE1_MP_CTRL_09
	{0xD598, 0x40E0C080},   //PE1_MP_CTRL_0A
	{0xD59C, 0x80C0A060},   //PE1_MP_CTRL_0B
	{0xD5A0, 0x0000E0C0},   //PE1_MP_CTRL_0C
	{0xD5B0, 0x0303FF1C},   //PE1_CORING_CTRL_00
	{0xD5B4, 0x0A101D10},   //PE1_CORING_CTRL_01
	{0xD5B8, 0x70FF1000},   //PE1_CORING_CTRL_02
	{0xD5BC, 0x28FF0800},   //PE1_CORING_CTRL_03
	{0xD5C0, 0xA0FF0800},   //PE1_CORING_CTRL_04
	{0xD5C4, 0x30FF1000},   //PE1_CORING_CTRL_05
	{0xD5C8, 0x80C00008},   //PE1_CORING_CTRL_06
	{0xD5CC, 0x60400008},   //PE1_CORING_CTRL_07
	{0xD5D0, 0x200801FF},   //PE1_CORING_CTRL_08
	{0xD5D4, 0xC0C00C10},   //PE1_CORING_CTRL_09
	{0xD600, 0x20200000},   //PE1_PTI_CTRL_00
	{0xD604, 0x50200000},   //PE1_PTI_CTRL_01
	{0xD608, 0x00000000},   //PE1_STI_CTRL_00
	{0xD60C, 0x00000000},   //PE1_STI_CTRL_01
	{0xD620, 0xB0500165},   //PE1_DJ_CTRL_00
	{0xD624, 0x02802000},   //PE1_DJ_CTRL_01
	{0xD628, 0x00001408},   //PE1_DJ_CTRL_02
	{0xD62C, 0x10080828},   //PE1_DJ_CTRL_03
	{0xD630, 0x00004051},   //PE1_CTI_CTRL_00
	{0xD634, 0x00011810},   //PE1_CTI_CTRL_01
	{0xD638, 0x00880812},   //PE1_CTI_CTRL_02
	{0xD650, 0x00000000},   //PE1_DEBUG_CTRL_00
	{0xD654, 0x00000000},   //PE1_DEBUG_CTRL_01
	{0xD658, 0x00000000},   //PE1_DEBUG_CTRL_02
	{0xD6A0, 0x00000000},   //PE1_TGEN_CTRL_00
	{0xD6A4, 0x03400603},   //PE1_TGEN_CTRL_01
	{0xD6A8, 0x0000043F},   //PE1_TGEN_CTRL_02
	{0xD6AC, 0x48AB6D1E},   //PE1_TGEN_CTRL_03
	{0xD6B0, 0x01010A01},   //PE1_TGEN_CTRL_04
	{0xD6B4, 0x1F000050},   //PE1_TGEN_CTRL_05
	{0xD6B8, 0x1200100A},   //PE1_TGEN_CTRL_06
	{0xD6BC, 0x1200100A},   //PE1_TGEN_CTRL_07
	{0xD6C0, 0x3F401F1F},   //PE1_TGEN_CTRL_08
	{0xD6C4, 0xFFFFFFFF}    //PE1_TGEN_CTRL_09
};