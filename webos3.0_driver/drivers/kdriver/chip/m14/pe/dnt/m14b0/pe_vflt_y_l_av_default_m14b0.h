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

const PE_REG_PARAM_T vflt_y_l_av_default_m14b0[] = 
{
	/* pe0d v filter, 0x3A80~0x3A88 */
	{0x3A80, 0x3EFF3000}, // VFILTER_CTRL_0
	{0x3A84, 0x01003091}, // VFILTER_CTRL_1
	{0x3A88, 0x042000FF}  // VFILTER_CTRL_2
};
