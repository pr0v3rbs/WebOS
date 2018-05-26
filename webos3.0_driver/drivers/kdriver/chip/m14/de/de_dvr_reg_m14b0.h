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

#ifndef _REG_DVR_M14B0_h
#define _REG_DVR_M14B0_h

/*-----------------------------------------------------------------------------
	0x5ca0 dvr_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_status           : 3;   //  0: 2
	UINT32                     : 1;   //     3 reserved
	UINT32 ctrl_mode           : 1;   //     4
	UINT32                     : 11;  //  5:15 reserved
	UINT32 rd_indx             : 2;   // 16:17
	UINT32 de_info             : 14;  // 18:31
} M14B0_DVR_STATUS_T;

/*-----------------------------------------------------------------------------
	0x5ca4 dvr_dsize ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsize               : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 hsize               : 12;  // 16:27
} M14B0_DVR_DSIZE_T;

/*-----------------------------------------------------------------------------
	0x5ca8 dvr_decsize ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsize               : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 hsize               : 12;  // 16:27
} M14B0_DVR_DECSIZE_T;

/*-----------------------------------------------------------------------------
	0x5cac dvr_hoffset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 roffset             : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 loffset             : 12;  // 16:27
} M14B0_DVR_HOFFSET_T;

/*-----------------------------------------------------------------------------
	0x5cb0 dvr_voffset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 boffset             : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 toffset             : 12;  // 16:27
} M14B0_DVR_VOFFSET_T;

/*-----------------------------------------------------------------------------
	0x5cb4 dvr_seqinfo ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aspect_ratio        : 5;   //  0: 4
	UINT32                     : 11;  //  5:15 reserved
	UINT32 prog_seq            : 1;   //    16
	UINT32                     : 11;  // 17:27 reserved
	UINT32 frame_rate          : 4;   // 28:31
} M14B0_DVR_SEQINFO_T;

/*-----------------------------------------------------------------------------
	0x5cb8 dvr_picinfo ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 prog_frame          : 1;   //     0
	UINT32                     : 3;   //  1: 3 reserved
	UINT32 topfield_first      : 1;   //     4
	UINT32 repeat_firstfield   : 1;   //     5
	UINT32 frame_struct        : 2;   //  6: 7
	UINT32 frame_ptr           : 8;   //  8:15
	UINT32 clr_info            : 16;  // 16:31
} M14B0_DVR_PICINFO_T;

/*-----------------------------------------------------------------------------
	0x5cbc dvr_ptsinfo ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pts_info            : 32;  //  0:31
} M14B0_DVR_PTSINFO_T;

/*-----------------------------------------------------------------------------
	0x5cc0 dvr_y_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_addr              : 32;  //  0:31
} M14B0_DVR_Y_ADDR_T;

/*-----------------------------------------------------------------------------
	0x5cc4 dvr_y_stride ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_stride            : 12;  //  0:11
} M14B0_DVR_Y_STRIDE_T;

/*-----------------------------------------------------------------------------
	0x5cc8 dvr_c_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_addr              : 32;  //  0:31
} M14B0_DVR_C_ADDR_T;

/*-----------------------------------------------------------------------------
	0x5ccc dvr_c_stride ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_stride            : 12;  //  0:11
} M14B0_DVR_C_STRIDE_T;

typedef struct {
	M14B0_DVR_STATUS_T      dvr_status;              // 0x5ca0
	M14B0_DVR_DSIZE_T       dvr_dsize;               // 0x5ca4
	M14B0_DVR_DECSIZE_T     dvr_decsize;             // 0x5ca8
	M14B0_DVR_HOFFSET_T     dvr_hoffset;             // 0x5cac
	M14B0_DVR_VOFFSET_T     dvr_voffset;             // 0x5cb0
	M14B0_DVR_SEQINFO_T     dvr_seqinfo;             // 0x5cb4
	M14B0_DVR_PICINFO_T     dvr_picinfo;             // 0x5cb8
	M14B0_DVR_PTSINFO_T     dvr_ptsinfo;             // 0x5cbc
	M14B0_DVR_Y_ADDR_T      dvr_y_addr;              // 0x5cc0
	M14B0_DVR_Y_STRIDE_T    dvr_y_stride;            // 0x5cc4
	M14B0_DVR_C_ADDR_T      dvr_c_addr;              // 0x5cc8
	M14B0_DVR_C_STRIDE_T    dvr_c_stride;            // 0x5ccc
} DE_DVR_REG_M14B0_T;

#endif
