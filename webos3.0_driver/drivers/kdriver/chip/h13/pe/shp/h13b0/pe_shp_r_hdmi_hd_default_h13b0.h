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

const PE_REG_PARAM_T shp_r_hdmi_hd_default_h13b0[] = 
{
	/* scaler : min-max clipping(V)*/
	{0x3180, 0x00000281},
	/* cvb cti */
	{0x0284, 0x30000030},
	/* p0l cti */
	{0x2584, 0x00232000},
	{0x25E4, 0x00280001}
};

