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

/** for nt,pal av & hdd nt,pal av
*
*   if (s_status>0xc0)
*      wr 0x02100A03
*      (auto_chroma_filter_en          : 1)
*      (manual_chroma_pre_filter_en    : 0)
*      (manual_chroma_median_filter_en : 0)
*   else
*      wr 0x02100A1A(default)
*      (auto_chroma_filter_en          : 0)
*      (manual_chroma_pre_filter_en    : 1)
*      (manual_chroma_median_filter_en : 1)
*
*/
const PE_REG_PARAM_T vflt_c_l_av_default_m14b0[] = 
{
	/* msc chroma tear,   0x3878 */
	{0x3878, 0x02100A1A}  //MSC_CHROMA_TEAR_CTRL
};
