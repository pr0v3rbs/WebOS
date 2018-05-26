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

/** @file
 *
 * Video decoding driver header for vdec device.
 * VDEC device will teach you how to make device driver with lg115x platform.
 *
 * author     Junghyun son(junghyun.son@lge.com)
 * version    1.0
 * date       2012.05.10
 * note       Additional information.
 *
 */

#ifndef _VDC_DRV_H_
#define _VDC_DRV_H_


/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/wait.h>
#include "../vds/disp_clear_q.h"
#include "../decoded_q.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
	VDC Codec Type.
		decoding formats supported by the VDC.
*/
typedef UINT32 VDC_HANDLE_T;

typedef void (*VDC_STATE_CHANGED) (UINT8 ch);

#define FSTR(s0,s1,s2,s3)	((s0<<0)|(s1<<8)|(s2<<16)|(s3<<24))
typedef enum
{
	FEED_DATA = 0,
	FEED_CMD_FLUSH		= FSTR('F','L','S','H'),
	FEED_CMD_SCAN_ALL	= FSTR('S','C','N','A'),
	FEED_CMD_SCAN_I		= FSTR('S','C','N','I'),
	FEED_CMD_SKIP_B		= FSTR('S','K','P','B'),
	FEED_CMD_SKIP_DISH	= FSTR('S','K','P','D'),
	FEED_CMD_OPEN		= FSTR('O','P','E','N'),
	FEED_CMD_CLOSE		= FSTR('C','L','O','S'),
	FEED_TIMESTAMP      	= FSTR('T','I','M','E'),
	FEED_CMD_NONE = 0x20130511
} FEEDDUMP_CMD_T;

typedef enum
{
	feeddump_feed,
	feeddump_decdone,
	feeddump_header,
} feeddump_mode_t;

typedef enum
{
	VDC_RET_ERROR			= 0,
	VDC_RET_OK			= 1,

	VDC_RET_MIN			= VDC_RET_ERROR,
	VDC_RET_MAX			= VDC_RET_OK,
	VDC_RET_INVALID
} VDC_RET_T;

typedef enum
{
	VDC_CODEC_H264_HP		=  0,
	VDC_CODEC_VC1_RCV_V1		=  1,
	VDC_CODEC_VC1_RCV_V2		=  2,
	VDC_CODEC_VC1_ES		=  3,
	VDC_CODEC_MPEG2_HP		=  4,
	VDC_CODEC_MPEG4_ASP		=  5,
	VDC_CODEC_XVID			=  6,
	VDC_CODEC_DIVX3			=  7,
	VDC_CODEC_DIVX4			=  8,
	VDC_CODEC_DIVX5			=  9,
	VDC_CODEC_RVX			= 10,
	VDC_CODEC_AVS			= 11,
	VDC_CODEC_VP8			= 12,
	VDC_CODEC_H264_MVC		= 13,
	VDC_CODEC_THEORA		= 14,
	VDC_CODEC_HEVC			= 15,
	VDC_CODEC_H263_SORENSON		= 16,
    VDC_CODEC_VP9           = 17,

	VDC_CODEC_MIN			= VDC_CODEC_H264_HP,
	VDC_CODEC_MAX			= VDC_CODEC_VP9,
	VDC_CODEC_INVALID
} VDC_CODEC_T;

typedef enum
{
	VDC_CODEC_STATE_NULL = 0,
	VDC_CODEC_STATE_READY,
	VDC_CODEC_STATE_BUSY
} VDC_CODEC_STATE_T;

/**
	VDC Skip Mode & Type.
		picture skip mode and type supported by the VDC.
*/
typedef enum
{
	VDC_SKIP_NONE			= 0,			///< decode IPB frame.
	VDC_SKIP_B			= 1,			///< IP picture only (B skip only)
	VDC_SKIP_PB			= 2,			///< I picture only (PB skip)
	VDC_SKIP_DISH			= 3,			///< dish-trick work-around (To display npf)
	VDC_SKIP_MIN			= VDC_SKIP_NONE,
	VDC_SKIP_MAX			= VDC_SKIP_B,
	VDC_SKIP_INVALID
} VDC_SKIP_T;

/* Will be removed */
typedef enum
{
	VDC_SRC_SDEC0	= 0,
	VDC_SRC_SDEC1	= 1,
	VDC_SRC_SDEC2	= 2,
	VDC_SRC_BUFF0	= 3,
	VDC_SRC_BUFF1	= 4,
	VDC_SRC_BUFFTVP	= 5,
	VDC_SRC_FRMBUF0	= 6,
	VDC_SRC_FRMBUF1	= 7
} VDC_SRC_T;

typedef enum
{
	VDC_MEMORY_FORMAT_TILED = 0,
	VDC_MEMORY_FORMAT_RASTER
} VDC_MEMORY_FORMAT_T;

typedef enum
{
	VDC_LINEAR_MODE_NONE = 0,
	VDC_LINEAR_MODE_STATIC,
	VDC_LINEAR_MODE_DYNAMIC
} VDC_LINEAR_MODE_T;

typedef enum
{
	VDC_NO_DELAY		    = (1<<0),
	VDC_SINGLE_FRAME_DECODE = (1<<1),
	VDC_ULTRA_HD		    = (1<<2),
	VDC_LIVE_CHANNEL		= (1<<3),
} VDC_OPEN_CONF_E;

typedef enum
{
    VDC_DEC_BODA,
    VDC_DEC_G1,
    VDC_DEC_G2,
    VDC_DEC_HEVC,
} VDC_OPEN_DECODER_E;

typedef struct
{
	UINT32			ui32Stride;
	UINT32			ui32Height;
	UINT32			ui32BufAddr;
} VDC_LINEAR_FRAME_BUF_T;

/**
	VDC Open Parameter Structure.
*/
typedef struct
{
	UINT8			ui8VesCh;
	void			*priv;
	VDC_SRC_T		ui8Src;
	VDC_CODEC_T		ui8Vcodec;
	UINT32			ui32CpbBufAddr;
	UINT32			ui32CpbBufSize;
	clear_q_t		*clear_q;
	decoded_q_t		*decoded_q;
	VDC_MEMORY_FORMAT_T	output_memory_format;
	VDC_LINEAR_MODE_T	extern_linear_buf_mode;
	VDC_OPEN_CONF_E eOpenConf;
    VDC_OPEN_DECODER_E eOpenDec;
	VDC_STATE_CHANGED state_changed;
} VDC_OPEN_PARAM_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Generic Usage Functions
----------------------------------------------------------------------------------------*/
void		VDC_Init(void);
void		VDC_Cleanup(void);
VDC_HANDLE_T	VDC_Open(VDC_OPEN_PARAM_T* pstOpenParam);
VDC_RET_T	VDC_Close(VDC_HANDLE_T hVdcCh);
VDC_RET_T	VDC_Reset(VDC_HANDLE_T hVdcCh);
VDC_RET_T	VDC_Start(VDC_HANDLE_T hVdcCh);
VDC_RET_T	VDC_Update(VDC_HANDLE_T hVdcCh);
VDC_RET_T	VDC_Flush(VDC_HANDLE_T hVdcCh);
VDC_RET_T	VDC_Stop(VDC_HANDLE_T hVdcCh);
VDC_CODEC_STATE_T VDC_GetCoreState(VDC_HANDLE_T hVdcCh, wait_queue_head_t **wait);
VDC_RET_T	VDC_SetSkipMode(VDC_HANDLE_T hVdcCh, VDC_SKIP_T eSkipMode);
VDC_RET_T	VDC_SetUserDataOutput(VDC_HANDLE_T hVdcCh, BOOLEAN bEnable);
VDC_RET_T	VDC_GetFrame(VDC_HANDLE_T hVdcCh, UINT8 ui8FrameNum, void* vpFrameBuf);
VDC_RET_T	VDC_Suspend(void);
VDC_RET_T	VDC_Resume(void);
VDC_RET_T	VDC_SetExternDpb( VDC_HANDLE_T hVdcCh, VDC_LINEAR_FRAME_BUF_T stFrameBufInfo );

#ifdef __cplusplus
}
#endif
#endif /* _VDC_DRV_H_ */

