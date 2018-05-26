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

#ifndef _REG_DLR_M14A0_h
#define _REG_DLR_M14A0_h

/*-----------------------------------------------------------------------------
	0x0300 dlr_in_img_format ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_image_format     : 3;   //  0: 2
	UINT32                     : 1;   //     3 reserved
	UINT32 color_format        : 2;   //  4: 5
	UINT32                     : 2;   //  6: 7 reserved
	UINT32 lr_in_order         : 1;   //     8
	UINT32                     : 3;   //  9:11 reserved
	UINT32 progressive         : 1;   //    12
} M14A0_DLR_IN_IMG_FORMAT_T;

/*-----------------------------------------------------------------------------
	0x0304 dlr_frame_pack_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 one_frame_height    : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 active_blank        : 12;  // 16:27
	UINT32 fp_interlace        : 1;   //    28
} M14A0_DLR_FRAME_PACK_INFO_T;

/*-----------------------------------------------------------------------------
	0x0308 dlr_status_pic_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status              : 8;   //  0: 7
} M14A0_DLR_STATUS_PIC_CNT_T;

/*-----------------------------------------------------------------------------
	0x030c dlr_status_line_cnt ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_line_counts    : 13;  //  0:12
	UINT32                     : 3;   // 13:15 reserved
	UINT32 chroma_line_counts  : 13;  // 16:28
} M14A0_DLR_STATUS_LINE_CNT_T;

typedef struct {
	M14A0_DLR_IN_IMG_FORMAT_T       dlr_in_img_format;               // 0x0300
	M14A0_DLR_FRAME_PACK_INFO_T     dlr_frame_pack_info;             // 0x0304
	M14A0_DLR_STATUS_PIC_CNT_T      dlr_status_pic_cnt;              // 0x0308
	M14A0_DLR_STATUS_LINE_CNT_T     dlr_status_line_cnt;             // 0x030c
} DE_DLR_REG_M14A0_T;

#endif
