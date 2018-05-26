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

const PE_REG_PARAM_T pblur_dnr_l_comp_sd_default_h15a0[] =
{
	/* pre blur dnr, 0x719C, 0x7288 */
	{0x719C, 0x00020406},	//REG_420C_CTRL
	{0x7288, 0x00000000}	//C_PRE_BLUR_CTRL_0, [1]median:0, [0]121blur:0
};
