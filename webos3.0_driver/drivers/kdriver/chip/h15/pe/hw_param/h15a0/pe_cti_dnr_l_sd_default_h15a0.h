/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it adnr/or 
	modify it udnrer the terms of the GNU General Public License
	version 2 as published by the Free Software Foudnration.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 

const PE_REG_PARAM_T cti_dnr_l_sd_default_h15a0[] =
{
	/* cti dnr, 0x718C~0x7190 */
	{0x718C, 0x0000FF05},   // CTI_CTRL_0,1114,cti_pre_flt_mode:0->1
	{0x7190, 0x00332000}  	// CTI_CTRL_1
};
