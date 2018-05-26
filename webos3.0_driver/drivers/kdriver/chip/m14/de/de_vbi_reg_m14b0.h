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

#ifndef _REG_VBI_M14B0_H_
#define _REG_VBI_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0021d00L ctrl_cgms0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cgms_enable                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 cgms_mode                          : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cgms_event_line                    : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 cgms_data_bit_width                : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cgms_line_offset                   : 4 ;  // 27:24     
	UINT32 cgms_field_mode                    : 1 ;  //    28     
} M14B0_CTRL_CGMS0_T;

/*-----------------------------------------------------------------------------
		0xc0021d04L ctrl_cgms1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cgms_white_peak_level              : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cgms_blank_level                   : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cgms_data_bit_count                : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 cgms_detect_offset                 : 7 ;  // 30:24     
} M14B0_CTRL_CGMS1_T;

/*-----------------------------------------------------------------------------
		0xc0021d08L data_cgms0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cgms_even_data                     : 32;  // 31: 0     
} M14B0_DATA_CGMS0_T;

/*-----------------------------------------------------------------------------
		0xc0021d0cL data_cgms1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cgms_odd_data                      : 32;  // 31: 0     
} M14B0_DATA_CGMS1_T;

/*-----------------------------------------------------------------------------
		0xc0021d10L ctrl_wss0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wss_enable                         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 wss_mode                           : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 wss_event_line                     : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 wss_data_bit_width                 : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 wss_line_offset                    : 4 ;  // 27:24     
	UINT32 wss_field_mode                     : 1 ;  //    28     
} M14B0_CTRL_WSS0_T;

/*-----------------------------------------------------------------------------
		0xc0021d14L ctrl_wss1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wss_white_peak_level               : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 wss_blank_level                    : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 wss_data_bit_count                 : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 wss_detect_offset                  : 7 ;  // 30:24     
} M14B0_CTRL_WSS1_T;

/*-----------------------------------------------------------------------------
		0xc0021d18L ctrl_wss2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 runin_data                         : 12;  // 11: 0     
	UINT32 wss_st_detect_offset               : 4 ;  // 15:12     
	UINT32 wss_st_data_bit_width              : 7 ;  // 22:16     
} M14B0_CTRL_WSS2_T;

/*-----------------------------------------------------------------------------
		0xc0021d1cL data_wss0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 raw_wss_data                       : 32;  // 31: 0     
} M14B0_DATA_WSS0_T;

/*-----------------------------------------------------------------------------
		0xc0021d20L data_wss1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wss_data                           : 16;  // 15: 0     
} M14B0_DATA_WSS1_T;

typedef struct {
	M14B0_CTRL_CGMS0_T                 	ctrl_cgms0;                   //0xc0021d00L
	M14B0_CTRL_CGMS1_T                 	ctrl_cgms1;                   //0xc0021d04L
	M14B0_DATA_CGMS0_T                 	data_cgms0;                   //0xc0021d08L
	M14B0_DATA_CGMS1_T                 	data_cgms1;                   //0xc0021d0cL
	M14B0_CTRL_WSS0_T                  	ctrl_wss0;                    //0xc0021d10L
	M14B0_CTRL_WSS1_T                  	ctrl_wss1;                    //0xc0021d14L
	M14B0_CTRL_WSS2_T                  	ctrl_wss2;                    //0xc0021d18L
	M14B0_DATA_WSS0_T                  	data_wss0;                    //0xc0021d1cL
	M14B0_DATA_WSS1_T                  	data_wss1;                    //0xc0021d20L
} DE_VBI_REG_M14B0_T;

#endif