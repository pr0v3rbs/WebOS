/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/**
 * @file
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     youngki.lyu@lge.com
 * version    0.1
 * date       2012.04.14
 * note       Additional information.
 *
 */

#ifndef _VDEC_DISP_Q_H_
#define _VDEC_DISP_Q_H_

#ifdef	__linux__
#include <linux/wait.h>
#endif	//__linux__

#include "vdec_type_defs.h"
#include "disp_clear_q.h"


typedef struct S_DISPQ_BUF
{
	ULONG ui32Tiled_FrameBaseAddr;
	UINT32 ui32Y_FrameBaseAddr;
	UINT32 ui32C_FrameBaseAddr;
	UINT32 ui32Y_FrameBaseAddrBot;
	UINT32 ui32C_FrameBaseAddrBot;
	UINT32 ui32Stride;
#if defined (CHIP_NAME_h15) || defined(CHIP_NAME_m16)
	//For H15
	UINT32 ui32TopBottomGap;
	BOOLEAN bSmartBalancing;
	UINT8 ui8SmartBalancingMode;
	UINT16 ui16SmBalanOffsetM0;
	UINT16 ui16SmBalanOffsetM1;
#endif
	enum
	{
		DISPQ_LINEAR_MAP_DPB = 0,
		DISPQ_FRAME_MAP_DPB = 1,		// BODA Progressive
		DISPQ_MIXED_MAP_DPB = 2,		// BODA Interlaced
		DISPQ_HEVC_MAP_DPB = 3,			// 8 bit
		DISPQ_HEVC10_MAP_DPB = 4,		// 10 bit
		DISPQ_G1_MAP = 5,
		DISPQ_G2_MAP = 6,
	} ui32DpbMapMode;

	UINT32 ui32FrameRateRes;
	UINT32 ui32FrameRateDiv;
	BOOLEAN	bVariableFrameRate;

	UINT32 ui32FrameIdx;
	UINT32 ui32AspectRatio;
	UINT32 ui32PicWidth;
	UINT32 ui32PicHeight;
	UINT32 ui32H_Offset;
	UINT32 ui32V_Offset;
	enum
	{
		DISPQ_SCAN_NULL = 0,
		DISPQ_SCAN_TFF = 1,		// Top Field First
		DISPQ_SCAN_BFF = 2,		// Bottom Field First
		DISPQ_SCAN_PROG = 3,	// Progressive Frame
		DISPQ_SCAN_32bits = 0x20110531,
	} ui32DisplayInfo;

	enum DISPQ_FRAME_FORMAT
	{
		DISPQ_FRAME_FORMAT_420,
		DISPQ_FRAME_FORMAT_422,
	} FrameFormat;

	BOOLEAN bDiscont;
	UINT32 ui32DTS;
	UINT32 ui32PTS;
	UINT32 enq_gstc;
	UINT64 ui64TimeStamp;
	UINT32 ui32UId;
	UINT32 ui32DisplayPeriod;
	UINT8 ui8ActiveFormatDescription;

	enum
	{
		/*
		 * SEI(Supplemental enhancement information) FPA (Frame Packing
		 * Arrangement) message on MPEG4.
		 *
		 * see http://en.wikipedia.org/wiki/H.264/MPEG-4_AVC
		 *
		 * 0: checkerboard - pixels are alternatively from L and R
		 * 1: column alternation - L and R are interlaced by column
		 * 2: row alternation - L and R are interlaced by row
		 * 3: side by side - L is on the left, R on the right
		 * 4: top bottom - L is on top, R on bottom
		 * 5: frame alternation - one view per frame
		 */

		DISPQ_FPA_CHECKERBOARD = 0,
		DISPQ_FPA_COLUMN_ALTERNATION = 1,
		DISPQ_FPA_ROW_ALTERNATION = 2,
		DISPQ_FPA_SIDE_BY_SIDE = 3,
		DISPQ_FPA_TOP_BOTTOM = 4,
		DISPQ_FPA_FRAME_ALTERNATION = 5,

		DISPQ_FPA_NONE = -2,
	} i32FramePackArrange;		// 3D SEI
	enum
	{
		DISPQ_3D_FRAME_RIGHT = 0,
		DISPQ_3D_FRAME_LEFT = 1,
		DISPQ_3D_FRAME_NONE = 2,
		DISPQ_3D_FRAME_32bits = 0x20120114,
	} ui32LR_Order;
	UINT16 ui16ParW;
	UINT16 ui16ParH;

	/* callback function when display completed */
	void (*displayed_callback) (void *arg, struct S_DISPQ_BUF *buffer);
	void *displayed_callback_arg;
	BOOLEAN bDispResult;	// 1: Display Successed  0: Fail(Discarded)

	void *pBufferPriv;
}S_DISPQ_BUF_T;

typedef S_DISPQ_BUF_T	DISPQ_FRM_T;

void DISP_Q_Init(void);
void DISP_Q_Reset(int ch);
BOOLEAN DISP_Q_Open(int ch, UINT32 ui32NumOfDq);
void DISP_Q_Close(int ch);
BOOLEAN DISP_Q_Push_callback(int ch, S_DISPQ_BUF_T *psDisBuf,
		void (*displayed_callback) (void *arg, S_DISPQ_BUF_T *buffer),
		void *displayed_callback_arg);
BOOLEAN DISP_Q_Push_clearq(int ch, S_DISPQ_BUF_T *psDisBuf, clear_q_t *clear_q);
BOOLEAN DISP_Q_Pop(int ch, S_DISPQ_BUF_T *psDisBuf);
S_DISPQ_BUF_T* DISP_Q_Peep(int ch);
UINT32 DISP_Q_NumActive(int ch);
UINT32 DISP_Q_NumFree(int ch);
BOOLEAN DISP_Q_Ref(int ch, S_DISPQ_BUF_T *buf);

#ifdef	__linux__
wait_queue_head_t *DISP_Q_GetWait (int ch);
#endif	//__linux__

#endif /* _VDEC_DISP_Q_H_ */

