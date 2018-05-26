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

const PE_REG_PARAM_T vflt_y_l_pc_default_h15a0[] =
{
	/* dnr v filter, 0x7180~0x7188 */
	{0x7180, 0x3EFF3000}, // VFILTER_CTRL_0
	{0x7184, 0x01003091}, // VFILTER_CTRL_1
	{0x7188, 0x041000FE}  // VFILTER_CTRL_2
};