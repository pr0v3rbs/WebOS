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


/** @file
 *
 *  osd header file for APR device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.07.06
 *  note		Additional information.
 *
 *  @addtogroup APR
 *	@{
 */

#ifndef _APR_OSD_H_
#define _APR_OSD_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"

#ifdef  __cplusplus
extern "C"
{
#endif /* __cplusplus */
	
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define STRING_MAX_SIZE 64

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	APR_OSD_NONE,
	APR_OSD_TP,
	APR_OSD_2D,
	APR_OSD_SS,
	APR_OSD_TB,
	APR_OSD_CB,
	APR_OSD_CC,
	APR_OSD_LL,
	APR_OSD_MAX
} 
APR_OSD_PATTERN;

typedef enum {
	APR_OSD_SET_BASE,
	APR_OSD_SET_FLIP
}
APR_OSD_FLIP_CMD;

typedef enum {
	APR_OSD_PLANE_0,
	APR_OSD_PLANE_1,
	APR_OSD_PLANE_MAX
}
APR_OSD_PLANE_INFO;

typedef struct {
	BOOLEAN valid;
	volatile UINT32 *pOsdReg;
	volatile UINT32 *pOsdBase;
	UINT32 *osd_base_virt[2];
	UINT32  osd_base_phys[2];
	UINT32 max_x;
	UINT32 max_y;
	UINT32 stride;
	APR_OSD_PLANE_INFO plane;
} 
OSD_INFO_T;

typedef struct {
	BOOLEAN active;
	UINT32  x0;
	UINT32  y0;
	UINT32  x1;
	UINT32  y1;
	UINT32  color;
}
RECT_INFO_T;

typedef struct {
	char upper;
	char lower;
	int  *ch_array;
}
CHAR_MAP_T;

typedef struct {
	BOOLEAN use;
	UINT32 x_ofs;
	UINT32 x_num;
}
ROW_USE_INFO_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int APR_OSD_Init(void);
int APR_OSD_Uninit(void);
int APR_OSD_SetControl(BOOLEAN enable);
int APR_OSD_SetFlip(APR_OSD_FLIP_CMD cmd);
int APR_OSD_VsyncHandler(void);
int APR_OSD_ClearAllLayer(void);
int APR_OSD_DrawPattern(APR_OSD_PATTERN pattern);
int APR_OSD_DrawString(char *str, UINT32 x, UINT32 y);
int APR_OSD_PrintParam(char *buf0,    \
						UINT32 data0, \
						char *buf1,   \
						UINT32 data1, \
						char *buf2,   \
						UINT32 data2, \
						char *buf3,   \
						UINT32 data3, \
						UINT32 x,     \
						UINT32 y);
int APR_OSD_PrintParamHex(char *buf0, \
						UINT32 data0, \
						char *buf1,   \
						UINT32 data1, \
						char *buf2,   \
						UINT32 data2, \
						char *buf3,   \
						UINT32 data3, \
						UINT32 x,     \
						UINT32 y);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* _APR_OSD_H_ */

/** @} */





