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

const PE_REG_PARAM_T vflt_y_l_hd_default_m14b0[] = 
{
	/* pe0d v filter, 0x3A80~0x3A88 */
	{0x3A80, 0x3EFF2001}, // VFILTER_CTRL_0, 20140224, ath:0x10->0x20, protect dtv vflter on,off
	{0x3A84, 0x0100501D}, // VFILTER_CTRL_1, 20140224, f_num_th:0x4->0x7, protect dtv vflter on,off
	{0x3A88, 0x041000FD}  // VFILTER_CTRL_2
};
