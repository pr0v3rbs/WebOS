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


/** @file de_kapi.h
 *
 *	application interface header for de device
 *
 *	@author		dongho7.park (dongho7.park@lge.com)
 *	@version		0.6a
 *	@date		2009.12.30
 *
 *	@addtogroup lg1150_de
 *	@{
 */

#ifndef _DE_KAPI_H_
#define _DE_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define _USE_VT_EXTENTION_

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#ifndef USE_XTENSA
#include "base_types.h"
#endif

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define DE_IOC_MAGIC		'a'
/**
@name DE IOCTL List
ioctl list for de device.

@{
@def DE_IOW_DE_INIT
Initialize de module.
device open and reset
set default register for clock, csc etc.
enable irq and set intr mask
load ucode

@def DE_IOR_GET_FIRMWARE_INFO
Reads de firmware information( version and update info).

@def DE_IOW_SET_BG_COLOR
set background color (non-active region color).
L8 support back_color y(8bit) cb(4bit) cr(4bit) at window compose block(OV)

@def DE_IOR_GET_OUT_FR_RATE
get display output frame rate (50 or 60).

@def DE_IOW_SET_WIN_BLANK
set window blank, to support mote control.
L8 support dark_color y(8bit) cb(4bit) cr(4bit) at window compose block(OV)

@def DE_IOR_GET_INPUT_WIN
get scaler input window size and position.
depend on source size and crop.

@def DE_IOR_GET_OUT_WIN
get scaler output window size and position.
depend on display size and picture mode.

@def DE_IOW_SET_INPUT_WIN
set scaler input window size and position.
depend on source size and crop.

@def DE_IOW_SET_OUT_WIN
set scaler output window size and position.
depend on display size and picture mode.

@def DE_IOW_SET_DIS_OUT
on/off de display output

@def DE_IOW_SET_DIS_FMT
set de display size and frame rate, etc.
depend on panel attribute.
display parameter setting (DE1A DISP_PARAM0~3)

@def DE_IOW_SET_FR_RATE
set display frame rate and mode(forced freerun or not).

@def DE_IOW_SET_IRE_PATTERN
set color and IRE levels of full pattern.
used for factory adjustment.

@def DE_IOW_MULTI_WIN_ENABLE
enable/disable multi window mode including split window mode.
used for auto demo mode on/off of 60Hz display(PDP).
also set picture quality parameters for enhanced side of window.

@def DE_IOW_SET_SPREAD
set LVDS spread spectrum for 60Hz mode (PDP).
for above 120Hz mode, FRC controls LVDS spread spectrum
also see the CTOP CTR54[31:0] control register of SSPLL

@def DE_IOW_SET_CVI_SRC_TYPE
set captured(external) video source attribute.
channel A or B, size, offset, color, scan type, aspect ratio).

@def DE_IOW_SET_CVI_CSC
send color space conversion matrix and offset for each external source information.
RGB to YCbYr.

@def DE_IOW_SET_POST_CSC
send color space conversion matrix for post processing block
YCbCr to RGB.

@def DE_IOR_GET_MVI_COLORIMETRY
get MVI source colorimetry information.

@def DE_IOW_SET_CVE
set de CVE parameter.
select CVE feedback path and timing parameters.

@def DE_IOW_SET_VCS
set de VCS parameter.
transfer information of VCS to MCU part via IPC

@def DE_IOR_GET_CAPTURE_WIN
get de captured video size and offset

@def DE_IOW_SET_CAPTURE_WIN
set de captured video size and offset

@def DE_IOW_SET_ECANVAS
set de ecanvas on/off and ecanvas source path

@def DE_IOW_FW_DOWNLOAD
downdload Firmware of DE

@def DE_IOW_SET_CVI_FIR
set FIR coefficient of captured video source.

@def DE_IOW_SET_CVI_TPG
set test pattern generator of captured video source.

@def DE_IOW_SET_CVI_CSAMPLE_MODE
set captured source color sub-sampling method.

@def DE_IOW_SET_CVI_SYNC_RESHAPE
reshape captured input sync timing for PC source.

@def DE_IOR_GET_CVI_INFO
get CVI frame rate for external inputs.

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
// system (4)
#define DE_IOW_DE_INIT					_IOW(DE_IOC_MAGIC, 0, LX_DE_PANEL_TYPE_T )
#define DE_IOR_GET_FIRMWARE_INFO		_IOR(DE_IOC_MAGIC, 1, LX_DE_FIRMWARE_INFO_T )
#define DE_IOW_FW_DOWNLOAD			_IOW(DE_IOC_MAGIC, 2, LX_DE_FW_DWLD_T)
#define DE_IOW_SET_IF_CONFIG			_IOW(DE_IOC_MAGIC, 3, LX_DE_IF_CONFIG_T)
#define DE_IOR_GET_CAPACITY			_IOR(DE_IOC_MAGIC, 4, LX_DE_CAPACITY_T)

// ======================================================================================
// source (5)
#define DE_IOW_SET_WIN_PORT_SRC			_IOW(DE_IOC_MAGIC, 10, LX_DE_INPUT_SOURCE_T)
#define DE_IOW_SET_CVI_SRC_TYPE			_IOW(DE_IOC_MAGIC, 11, LX_DE_CVI_SRC_TYPE_T)
#define DE_IOW_SET_CVI_FIR					_IOW(DE_IOC_MAGIC, 12, LX_DE_CVI_FIR_T )
#define DE_IOW_SET_CVI_CSAMPLE_MODE		_IOW(DE_IOC_MAGIC, 13, LX_DE_CSAMPLE_MODE_T )
#define DE_IOR_GET_MVI_COLORIMETRY		_IOR(DE_IOC_MAGIC, 14, LX_DE_MVI_COLORIMETRY_T)
#define DE_IOW_SET_CVI_HDMIPORT			_IOW(DE_IOC_MAGIC, 15, LX_DE_HDMIPORT_T)

// ======================================================================================
// output (10)
#define DE_IOW_SET_DIS_FMT				_IOW(DE_IOC_MAGIC, 20, LX_DE_DIS_FMT_T)
#define DE_IOW_SET_FR_RATE				_IOW(DE_IOC_MAGIC, 21, LX_DE_FR_RATE_T)
#define DE_IOR_GET_OUT_FR_RATE			_IOR(DE_IOC_MAGIC, 22, LX_DE_FR_RATE_T)
#define DE_IOW_SET_DIS_OUT				_IOW(DE_IOC_MAGIC, 23, BOOLEAN)
#define DE_IOW_SET_SUB_WIN				_IOW(DE_IOC_MAGIC, 24, LX_DE_SET_SUB_WIN_T)

// ======================================================================================
// window composer (5)
#define DE_IOR_GET_INPUT_WIN			_IOR(DE_IOC_MAGIC, 30, LX_DE_WIN_DIMENSION_T)
#define DE_IOR_GET_OUT_WIN			_IOR(DE_IOC_MAGIC, 31, LX_DE_WIN_DIMENSION_T)
#define DE_IOW_SET_INPUT_WIN			_IOW(DE_IOC_MAGIC, 32, LX_DE_WIN_DIMENSION_T)
#define DE_IOW_SET_OUT_WIN			_IOW(DE_IOC_MAGIC, 33, LX_DE_WIN_DIMENSION_T)
#define DE_IOW_SET_ZLIST				_IOW(DE_IOC_MAGIC, 34, LX_DE_ZLIST_T)
#define DE_IOW_SET_MULTI_VISION		_IOW(DE_IOC_MAGIC, 35, LX_DE_RECT_T)
#define DE_IOW_SET_WIN_BLANK			_IOW(DE_IOC_MAGIC, 36, LX_DE_SET_WIN_BLANK_T)
#define DE_IOW_SET_BG_COLOR			_IOW(DE_IOC_MAGIC, 37, LX_DE_COLOR_T)
#define DE_IOW_SET_INNER_PATTERN		_IOW(DE_IOC_MAGIC, 38, LX_DE_INNER_PATTERN_T)

// ======================================================================================
// operation (10 +?)
#define DE_IOW_SET_OUTPUT_MODE			_IOW(DE_IOC_MAGIC, 40, LX_DE_OUTPUT_MODE_T)
#define DE_IOW_SET_3D_INOUT_CTRL  		_IOW(DE_IOC_MAGIC, 41, LX_DE_3D_INOUT_CTRL_T)
#define DE_IOW_SET_UD_MODE				_IOW(DE_IOC_MAGIC, 42, BOOLEAN)
#define DE_IOW_MULTI_WIN_ENABLE			_IOW(DE_IOC_MAGIC, 43, BOOLEAN)
#define DE_IOR_GET_LOW_DELAY				_IOR(DE_IOC_MAGIC, 44, LX_DE_LOW_DELAY_T)
#define DE_IOW_SET_LOW_DELAY				_IOW(DE_IOC_MAGIC, 45, UINT32)
#define DE_IOR_GET_SCALER_INFO			_IOR(DE_IOC_MAGIC, 46, LX_DE_SCALER_INFO_T)

// MEM interface - VCR(CVE), VENC(DVR), GPU, CAPTURE
#define DE_IOW_SET_CVE					_IOW(DE_IOC_MAGIC, 47, LX_DE_CVE_PARAM_T)
#define DE_IOW_SET_DVR_FMT_CTRL		_IOW(DE_IOC_MAGIC, 48, LX_DE_DVR_DISFMT_T)
#define DE_IOW_SET_VTM					_IOW(DE_IOC_MAGIC, 49, LX_DE_VTM_FRAME_INFO_T)
#define DE_IOR_GET_VTM					_IOR(DE_IOC_MAGIC, 50, LX_DE_VTM_FRAME_INFO_T)
#define DE_IOW_SET_CAPTURE			_IOW(DE_IOC_MAGIC, 51, LX_DE_CAPTURE_CTRL_T)
#define DE_IOW_SET_SRC_MIRROR			_IOW(DE_IOC_MAGIC, 52, LX_DE_SRC_MIRROR_T)
#define DE_IOW_SET_ACT_FMT_DETECT	_IOW(DE_IOC_MAGIC, 53, LX_DE_ACT_FORMAT_T)

// pixel grab - read/write from video frame before CVI csc parameter for self-diagnosis.
#define DE_IOWR_SET_CVI_VIDEO_FRAME_BUFFER		_IOWR(DE_IOC_MAGIC, 54, LX_DE_CVI_RW_VIDEO_FRAME_T)

// freeze
#define DE_IOW_SET_WIN_FREEZE			_IOW(DE_IOC_MAGIC, 60, LX_DE_WIN_FREEZE_T)
// de freeze sub scaler frame in msc-r or subscaler path
#define DE_IOW_SET_SUB_SC_FREEZE		_IOW(DE_IOC_MAGIC, 61, BOOLEAN)
// de cvi video frame buffer freeze to read/write video frame before cvi csc parameter for self-diagnosis.
#define DE_IOW_SET_CVI_FREEZE			_IOW(DE_IOC_MAGIC, 62, LX_DE_CVI_CAPTURE_FREEZE_T)

// ======================================================================================
// for DE debug (12)
#define DE_IOR_REG_RD					_IOR(DE_IOC_MAGIC, 80, UINT32)
#define DE_IOW_REG_WR					_IOW(DE_IOC_MAGIC, 81, LX_DE_REG_WR_T)
#define DE_IOR_GET_DBG					_IOR(DE_IOC_MAGIC, 82, LX_DE_SET_DBG_T)
#define DE_IOW_SET_DBG					_IOW(DE_IOC_MAGIC, 83, LX_DE_SET_DBG_T)
#define DE_IOW_SET_UART_FOR_MCU		_IOW(DE_IOC_MAGIC, 84, BOOLEAN)
#define DE_IOW_SET_VCS					_IOW(DE_IOC_MAGIC, 85, LX_DE_VCS_IPC_T)
#define DE_IOR_GET_SYS_STATUS			_IOR(DE_IOC_MAGIC, 86, LX_DE_SYS_STATUS_T)
#define DE_IOR_GET_SRC_STATUS			_IOR(DE_IOC_MAGIC, 87, LX_DE_SRC_STATUS_T)
#define DE_IOW_SET_CVI_TPG				_IOW(DE_IOC_MAGIC, 88, LX_DE_CVI_TPG_T)

// ioctl for h13 verification
#define DE_IOW_SET_EVAL_DBG				_IOWR(DE_IOC_MAGIC, 99, LX_DE_SET_EVAL_DBG_T)
// work-around
#define DE_IOW_SET_SCART_RGB_BYPASS_MODE	_IOW(DE_IOC_MAGIC, 100, int)

// remove ======================================================================================
#define DE_IOW_SET_OPERATION			_IOW(DE_IOC_MAGIC, 110, LX_DE_OPERATION_CTRL_T)			// IOCTL 96, 32, 16 sum
#define DE_IOW_SET_SUB_OPERATION		_IOW(DE_IOC_MAGIC, 111, LX_DE_SUB_OPERATION_CTRL_T)			// IOCTL 10, 84 sum
#define DE_IOW_SET_DEINTERLACE		_IOW(DE_IOC_MAGIC, 112, BOOLEAN)

#define DE_IOC_MAXNR				200
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * de display (panel) type, full HD or HD.
 */
typedef enum {
	LX_PANEL_TYPE_1920 = 0, ///< full HD
	LX_PANEL_TYPE_1366,		///< WXGA(HD)
	LX_PANEL_TYPE_1024,		///< 1024x768
	LX_PANEL_TYPE_1365,		///< 1365x768
	LX_PANEL_TYPE_3840,		///< 3840x2160
	LX_PANEL_TYPE_1280,		///< 1280x720
	LX_PANEL_TYPE_720,		///< 720x480
	LX_PANEL_TYPE_640,		///< 640x480
	LX_PANEL_TYPE_576,		///< 720x576
	LX_PANEL_TYPE_MAX		///< max number
}
LX_DE_PANEL_TYPE_T;


/**
 * de display (panel) device type
 */
typedef enum {
	LX_PANEL_DEV_LCD, 		///< LCD/LED device
	LX_PANEL_DEV_PDP,		///< PDP device
	LX_PANEL_DEV_OLED,		///< OLED device
	LX_PANEL_DEV_BOX,		///< CP box device
	LX_PANEL_DEV_GTV_STB,		///< Set-top box device for GTV
	LX_PANEL_DEV_MAX		///< max number
}
LX_DE_PANEL_DEV_T;

/**
 * de date type for firmware version info.
 */
typedef struct {
	UINT16 year;			///< year
	UINT16 month;			///< month
	UINT16 day;				///< day
}
LX_DE_DATE_T;

/**
 * de firmware u-code information.
 */
typedef struct {
	UINT32	version;		///< firmware version
	LX_DE_DATE_T date;		///< updated date
	UINT32 ipc_count;       ///< counter for ipc loop test
}
LX_DE_FIRMWARE_INFO_T;

/**
 * de source window index.
 */
typedef enum {
	LX_DE_WIN_MAIN = 0, ///< main window
	LX_DE_WIN_0    = LX_DE_WIN_MAIN,
	LX_DE_WIN_SUB,		///< sub window
	LX_DE_WIN_1    = LX_DE_WIN_SUB,
	LX_DE_WIN_2,
	LX_DE_WIN_3,
	LX_DE_WIN_MAX,	///< max number
}
LX_DE_WIN_ID_T;

/**
 * de horizontal(vertical) start, size in pixel.
 */
typedef struct
{
	UINT16		w;	///< horizontal Size(Width)    in pixel
	UINT16		h;	///< vertical	Size(Height)   in pixel
	UINT16		x;	///< horizontal Start Position in pixel
	UINT16		y;	///< vertical	Start Position in pixel
}
LX_DE_RECT_T;

/**
 * de input window horizontal(vertical) start, size information.
 */
typedef struct {
	LX_DE_WIN_ID_T win_id;	///< window index
	LX_DE_RECT_T rect;		///< window demension(w,h,x,y)
}
LX_DE_WIN_DIMENSION_T;

/**
 * de inner pattern type
 */
typedef enum {
	LX_DE_PATTERN_FULL_GRAY = 0, 	///< full white
	LX_DE_PATTERN_WHITE_BOX, 		///< white box on black background
	LX_DE_PATTERN_MAX 				///< max number
}
LX_DE_PATTERN_TYPE_T;

/**
 * de inner pattern parameter
 */
typedef struct {
	UINT32 enable;				///< inner pattern enable, 0:off,1:on
	LX_DE_PATTERN_TYPE_T type;	///< pattern type
	UINT32 level;				///< level, 0~255
}
LX_DE_INNER_PATTERN_T;

/**
 * de display nonactive region color parameter.
 * de window color for no signal or mute time.
 */
typedef struct {
	UINT8  r;	///< red
	UINT8  g;	///< green
	UINT8  b;	///< blue
}
LX_DE_COLOR_T;

/**
 * de enable input window blank and define window color(black or blue).
 */
typedef struct {
	LX_DE_WIN_ID_T win_id;			///< window index
	BOOLEAN bEnable;				///< mute enable
	LX_DE_COLOR_T win_color;		///< mute color
	BOOLEAN	isRGB;					///< mute color domain
}
LX_DE_SET_WIN_BLANK_T;

/**
 * de input source parameter.
 */
typedef enum {
	LX_DE_SRC_CVI  ,				///< captured external video input
	LX_DE_SRC_MVI  ,				///< video from MPEG decoder A
	LX_DE_SRC_CPU  ,				///< video from system memory A
	LX_DE_SRC_NONE ,				///< No Video
	LX_DE_SRC_MAX					///< Max
}
LX_DE_INPUT_SRC_T;

/**
 * de output format type.
 */
typedef struct {
	LX_DE_PANEL_TYPE_T panel_type;		///< display size
	UINT16 fr_rate;						///< display frame rate
	UINT16 h_total;						///< pixels per line
	UINT16 v_total;						///< lines per frame
	UINT16 h_active;					///< active pixel per line
	UINT16 v_active;					///< active lines per frame
	LX_DE_PANEL_DEV_T dev_type;			///< display device type
}
LX_DE_DIS_FMT_T;

/**
 * de output frame rate mode.
 */
typedef struct {
//	LX_DE_WIN_ID_T win_id;			///< window index --> window ID is not needed
	BOOLEAN isForceFreeRun;			///< enable forced free run mode
	UINT16 fr_rate;				///< frame rate
}
LX_DE_FR_RATE_T;

/**
 * de region type of graped pixels.
 */
typedef struct {
	LX_DE_WIN_ID_T win_id;
	UINT32 pixelGrabX;		///< x position
	UINT32 pixelGrabY;		///< y position
	UINT32 pixelGrabW;		///< capture width
	UINT32 pixelGrabH;		///< capture h

	UINT32 realPixelGrabW;	///< real W in memory
	UINT32 realPixelGrabH;	///< real H in memory
	UINT8 colorSpace;		///< 0: YUV 1:RGB
	UINT8 colorDepth;		///< 0: 8bit 1: 10bit
}
LX_DE_GRAB_REGION_T;

/**
 * de color type of graped pixels.
 */
typedef struct
{
	UINT16 pixelGrabCb;		///< Cb or B
	UINT16 pixelGrabY;		///< Y or G
	UINT16 pixelGrabCr;		///< CR or R
}
LX_DE_GRAB_COLOR_T;

/**
 * de scan type parameter.
 */
typedef enum {
	LX_DE_INTERLACED,		///< interlaced scan
	LX_DE_PROGRESSIVE		///< progressive scan
}
LX_DE_SCAN_T;

/**
 * de mvi 3D Video Information.
 *
 */
typedef enum
{
    //interim format - half
	LX_DE_MVI_3D_CHECKERBOARD = 0,		///< 3D check board format
	LX_DE_MVI_3D_COLUMN_INTERLEAVING,	///< 3D column interleaving format
	LX_DE_MVI_3D_ROW_INTERLEAVING,		///< 3D row interleaving format(line alternative)
	LX_DE_MVI_3D_SIDEBYSIDE,			///< 3D Side by Side format
	LX_DE_MVI_3D_TOPNBOTTOM,			///< 3D top and bottom format
	LX_DE_MVI_3D_FRAMESEQ,				///< 3D frame sequence format

    //Full format
	LX_DE_MVI_3D_FRAMEPACK,		    ///< 3D frame packing format
	LX_DE_MVI_3D_FIELD_ALTERNATIVE,	    ///< 3D field alternative format
	LX_DE_MVI_3D_LINE_ALTERNATIVE,		///< 3D line alternative format
	LX_DE_MVI_3D_SIDEBYSIDE_FULL,		///< 3D Side by Side full format
	LX_DE_MVI_3D_DUALSTREAM,			///< 3D dual stream format
    //  From UI, 3D ON이면서 아래 Format이 넘어오면 2D to 3D 가 됨
	LX_DE_MVI_3D_2DTO3D,				///< 3D format for2D TO 3D..... INPUT_TYPE_2DTO3D
	LX_DE_MVI_2D,						///< 2D format

    LX_DE_MVI_3D_FORMAT_MAX,

} LX_DE_MVI_3D_INFO_T;

/**
 * de mvi 3D L/R order
 *
 */
 typedef enum
{
	LX_DE_MVI_3D_R_FIRST = 0,			///< right first
	LX_DE_MVI_3D_L_FIRST,			    ///< left first
	LX_DE_MVI_3D_LR_ORDER_MAX			///< max number
}
LX_DE_MVI_3D_LR_ORDER_T;
/**
 * de Debug set type.
 */
 typedef enum {
	LX_DE_DBG_PIXEL_SHIFT = 0,			///< HDMI 13.5 MHz CSC 422 Issue - Pixel Shift
	LX_DE_DBG_SYNC_POSITION = 1,		///< source sync position value for HDMI & RGB
	LX_DE_DBG_CVI_RESET = 2,			///< CVI reset information of CTOP

	LX_DE_DBG_DEVICE_STATUS,
	LX_DE_DBG_TRACE_IOCTL,
	LX_DE_DBG_MISC_DATA,
	LX_DE_DBG_IPC_TEST,
	LX_DE_DBG_VERIFY_FW,
	LX_DE_DBG_SCALER_TPG,
	LX_DE_DBG_TRACE_IPC,
	LX_DE_DBG_VTM_TEST,
	LX_DE_DBG_MUTE_TEST,
	LX_DE_DBG_RESUME_TEST,
	LX_DE_DBG_MUTE_DISABLE,
	LX_DE_DBG_UHD_SETUP,

	LX_DE_DBG_MAX						///< max number
}
LX_DE_DBG_SET_TYPE_T;


/**
 * de Debug set debug type.
 */
typedef struct {
	LX_DE_WIN_ID_T			win_id;			///< window index
	LX_DE_DBG_SET_TYPE_T	type;			///< debug set type
	BOOLEAN					bParam;			///< BOOLEAN parameter
	UINT32					u32Param;		///< UINT32 parameter 1
	UINT32					u32ParamOne;	///< UINT32 parameter 2
}
LX_DE_SET_DBG_T;

/**
 * de video timing parameter of vdec interface.
 * used for adaptaion & DDI
 */
typedef struct {
	UINT16 h_size;								///< mvi picture horizontal size
	UINT16 v_size;								///< mvi picture vertical size
	UINT16 h_start;								///< mvi picture horizontal position
	UINT16 v_start;								///< mvi picture vertical position
	LX_DE_SCAN_T isProg;						///< mvi picture scan type(0 : interlace, 1 : progressive)
	UINT16 v_freq;								///< mvi picture vertical frequency
	LX_DE_MVI_3D_INFO_T  mvi_3dfmt_info;		///< MVI 3D Format Information
	LX_DE_MVI_3D_LR_ORDER_T  mvi_3dlr_order;	///< MVI 3D LR Order Information
}
LX_DE_MPEG_TIMING_INFO_T;

/**
 * de cvi source size offset parameter.
 */
typedef struct {
	UINT16	hsize;		///< horizontal pixels
	UINT16	vsize;		///< vertical lines
	UINT16	hoffset;		///< horizontal pixel offset
	UINT16	voffset;	///< vertical line offset
}
LX_DE_CVI_DIMENSION_T;

/**
 * de cvi source color format type.
 */
typedef enum
{
	LX_DE_COLOR_601,		///< SD
	LX_DE_COLOR_709,		///< HD
	LX_DE_COLOR_RGB,		///< PC/DVI 16~235
	LX_DE_COLOR_RGBEX,		///< PC/DVI 0~255
	LX_DE_COLOR_XVYCC601,	///< HDMI deep color for SD
	LX_DE_COLOR_XVYCC709,	///< HDMI deep color for HD
	LX_DE_COLOR_601_CVD,	///< CVBS through CVD
	LX_DE_COLOR_2020_CL,	///< BT.2020 Constant Luminance
	LX_DE_COLOR_2020_NCL,	///< BT.2020 Non-constant Luminace
	LX_DE_COLOR_NOT_DEFINED
}
LX_DE_COLOR_STD_T;

/**
 * de cvi source aspect ratio type.
 */
typedef enum
{
	LX_DE_AR_4_3,		///< 4:3
	LX_DE_AR_16_9		///< 16:9
}
LX_DE_AR_T;


/**
 * de cvi channels type.
 */
typedef enum
{
	LX_DE_CVI_CH_A,		///< channel A
	LX_DE_CVI_CH_B,		///< channel B
	LX_DE_CVI_CH_C,		///< channel C
	LX_DE_CVI_CH_D		///< channel D
}
LX_DE_CVI_CHANNEL_T;

/**
 * de cvi bit resolution.
 */
typedef enum
{
	LX_DE_CVI_10BIT,	///< 10 bit
	LX_DE_CVI_8BIT		///< 8 bit
}
LX_DE_CVI_BITRES_T;

/**
 * de cvi chroma sampling.
 */
typedef enum
{
	LX_DE_CVI_422,	///< 16bit 422 chroma sampling
	LX_DE_CVI_444,	///< 24bit 444 chroma sampling
	LX_DE_CVI_420	///< 12bit 420 chroma sampling
}
LX_DE_CVI_CSAMPLE_T;

/**
 * de scaler operating chroma sampling.
 */
typedef enum
{
	LX_DE_SCALER_420,	///< YUV 420 chroma sampling
	LX_DE_SCALER_422,	///< YUV 422 chroma sampling
	LX_DE_SCALER_444,	///< YUV 444 chroma sampling
}
LX_DE_SCALER_CSAMPLE_T;

/**
 * de cvi data enable mode.
 * hv offsets must be set to zero
 */
typedef enum
{
	LX_DE_CVI_NORMAL_SYNC_MODE,		///< sync mode for CVD, ADC
	LX_DE_CVI_HDMI_SYNC_MODE		///< sync mode for HDMI
}
LX_DE_CVI_SYNC_MODE_T;

/**
 * de cvi data sampling mode.
 */
typedef enum
{
	LX_DE_CVI_NORMAL_SAMPLING,	///< sync mode for CVD, ADC
	LX_DE_CVI_DOUBLE_SAMPLING,	///< sync mode for HDMI
	LX_DE_CVI_QUAD_SAMPLING		///< sync mode for HDMI
}
LX_DE_CVI_SAMPLING_MODE_T;

/**
 * de cvi data sync stability.
 */
typedef enum
{
	LX_DE_CVI_SYNC_STD,			///< sync is stable
	LX_DE_CVI_SYNC_NONSTD		///< sync is unstable
}
LX_DE_CVI_SYNC_STABILITY_T;

/**
 * de cvi field polarity.
 */
typedef enum
{
	LX_DE_CVI_FIELD_ORIGINAL,		///< original field polarity
	LX_DE_CVI_FIELD_REVERSED		///< reversed field polarity
}
LX_DE_CVI_FIELD_POLARITY_T;

/**
 * de cvi yc delay.
 */
typedef enum
{
	LX_DE_CVI_YC_DELAY_BYPASS,		///< yc_delay_mode_r = 0, yc_delay_mode_b = 0, yc_delay_mode_g = 0
	LX_DE_CVI_YC_DELAY_ATV,			///< yc_delay_mode_r = 0, yc_delay_mode_b = 3, yc_delay_mode_g = 2
	LX_DE_CVI_YC_DELAY_OTHERS		///< yc_delay_mode_r = 0, yc_delay_mode_b = 2, yc_delay_mode_g = 1
}
LX_DE_CVI_YC_DELAY_T;

/**
 * de cvi port select. for [L9]
 */
typedef enum
{
	LX_DE_CVI_EXT_A,  ///<
	LX_DE_CVI_EXT_B,  ///<
	LX_DE_CVI_CVD,	  ///<
	LX_DE_CVI_ADC	  ///<
}
LX_DE_CVI_PORT_T;

/**
 * de cvi 3D Video Full Information. for[L9]
 *
 */
typedef enum
{
	LX_DE_CVI_NORMAL_FORMAT = 0,		///< 2D format
	LX_DE_CVI_3D_FRAMEPACK,				///< 3D frame packing format
	LX_DE_CVI_3D_SBSFULL,				///< 3D Side by Side Full format
	LX_DE_CVI_3D_FIELD_ALTERNATIVE,		///< 3D field alternative format
	LX_DE_CVI_3D_ROW_INTERLEAVING,		///< 3D row interleaving format(line alternative)
	LX_DE_CVI_3D_COLUMN_INTERLEAVING,	///< 3D column interleaving format
	LX_DE_CVI_4K_2K,						///< 4K x 2K format
} LX_DE_CVI_3D_FULL_INFO_T;

/**
 * de cvi source type Information. for[L9]
 *
 */
typedef enum
{
	// Component Analog ports
	LX_DE_CVI_SRC_VGA,
	LX_DE_CVI_SRC_YPBPR,

	// Composite Analog ports
	LX_DE_CVI_SRC_ATV,
	LX_DE_CVI_SRC_CVBS,

	// Seperate Analog Ports
	LX_DE_CVI_SRC_SVIDEO,

	// SCART port
	LX_DE_CVI_SRC_SCART,

	// HDMI ports
	LX_DE_CVI_SRC_HDMI,

	LX_DE_CVI_SRC_NUM,

} LX_DE_CVI_INPUT_SRC_T;

/**
 * This enum is used to set CVD PQ Value to selected Color System
 * @see AFE_IOW_SET_3DCOMB_VALUE
 */

typedef enum {
	LX_DE_COLOR_DEFAULT		= 0,		///< Default CVD Params
	LX_DE_COLOR_NTSC_M		= 1,		///< CVD Params for NTSC_M AV inputs
	LX_DE_COLOR_NTSC_M_RF	= 2,		///< CVD Params for NTSC_M RF input
	LX_DE_COLOR_PAL			= 3,		///< CVD Params for NTSC_M AV inputs
	LX_DE_COLOR_PAL_RF		= 4,		///< CVD Params for NTSC_M RF input
	LX_DE_COLOR_SECAM		= 5,		///< CVD Params for NTSC_M AV inputs
	LX_DE_COLOR_SECAM_RF	= 6,		///< CVD Params for NTSC_M RF input
	LX_DE_COLOR_NTSC_443	= 7,		///< CVD Params for NTSC_443 AV inputs
	LX_DE_COLOR_NTSC_443_RF = 8,		///< CVD Params for NTSC_443 RF input
	LX_DE_COLOR_PAL_60		= 9,		///< CVD Params for PAL_60 AV inputs
	LX_DE_COLOR_PAL_60_RF	= 10,		///< CVD Params for PAL_60 RF input
	LX_DE_COLOR_PAL_M		= 11,		///< CVD Params for PAL_M AV inputs
	LX_DE_COLOR_PAL_M_RF	= 12,		///< CVD Params for PAL_M RF input
	LX_DE_COLOR_PAL_NC		= 13,		///< CVD Params for PAL_NC AV inputs
	LX_DE_COLOR_PAL_NC_RF	= 14,		///< CVD Params for PAL_NC RF input
	LX_DE_COLOR_UNKNOWN   	= 15,		///< CVD Params for Color System Unkown
	LX_DE_COLOR_NUM			= 16,
} LX_DE_CVD_COLOR_STD_T;

/**
 * de cvi source attribute type.
 */
typedef struct {
	LX_DE_CVI_CHANNEL_T		   cvi_channel;		  ///< CVI channel A or B
	LX_DE_CVI_DIMENSION_T	   size_offset;		  ///< h/v size offset
	LX_DE_COLOR_STD_T		   color;			  ///< color space 601,709,PC
	LX_DE_AR_T				   aspect;			  ///< aspect ratio 4:3 or 16:9
	LX_DE_SCAN_T			   scan;			  ///< interlaced or progressive
	LX_DE_CVI_BITRES_T		   bit_res;			  ///< bit resolution 10bit(0) or 8bit(1)
	LX_DE_CVI_CSAMPLE_T		   c_sample;		  ///< chroma sampling mode 422(0) or 444(1)
	LX_DE_CVI_SYNC_MODE_T	   de_mode;			  ///< internal data enable mode generated from sync(0) or de(1)
	LX_DE_CVI_SAMPLING_MODE_T  sampling;		  ///< external input sampling: noraml(0), double(1), quadrature(2)
	LX_DE_SCALER_CSAMPLE_T	   sc_c_sample;		  ///< scaler chroma sampling mode YUV420(0) YUV422(1) YUV444(2)
	LX_DE_CVI_SYNC_STABILITY_T stability;		  ///< stable sync(0), unstable sync(1)
	LX_DE_CVI_FIELD_POLARITY_T field_polarity;	  ///< original field polarity(0), reversed field polarity(1)
	LX_DE_CVI_YC_DELAY_T	   yc_delay;
	LX_DE_CVI_PORT_T		   cvi_port;		  ///< cvi port select. for [L9]
	LX_DE_CVI_3D_FULL_INFO_T   trid_full_format;  ///< 3D Video Full Information. for [L9]
	LX_DE_CVI_INPUT_SRC_T	   cvi_input_src;	  ///< cvi source type Information. for[L9]
	LX_DE_CVD_COLOR_STD_T	   cvd_color_std;	  ///< CVD color mode for[L9]
}
LX_DE_CVI_SRC_TYPE_T;

/**
 * de captured source FIR control parameter.
 */
typedef struct {
	LX_DE_CVI_CHANNEL_T    cvi_channel; ///< CVI channel A or B
	BOOLEAN isEnable;					///< fir enable
	UINT16 fir_coef[8];					///< fir coefficient
	UINT16 fir_coef_CbCr[6];			///< CbCr fir coefficient, add from L9 B0, by sh.myoung 201108
}
LX_DE_CVI_FIR_T;

/**
 * de cvi test pattern gen. pattern type.
 */
typedef enum
{
	LX_DE_CVI_TPG_WHITE,		///< white
	LX_DE_CVI_TPG_YELLOW,		///< yellow
	LX_DE_CVI_TPG_CYAN,			///< cyan
	LX_DE_CVI_TPG_GREEN,		///< green
	LX_DE_CVI_TPG_MAGENTA,		///< magenta
	LX_DE_CVI_TPG_RED,			///< red
	LX_DE_CVI_TPG_BLUE,			///< blue
	LX_DE_CVI_TPG_BLACK,		///< black
	LX_DE_CVI_TPG_GRADATION_H,	///< horizontal gradation
	LX_DE_CVI_TPG_GRADATION_V,	///< vertical gradation
	LX_DE_CVI_TPG_GRADATION_C,	///< color gradation
	LX_DE_CVI_TPG_GRADATION_YC, ///< Y/C gradation
	LX_DE_CVI_TPG_BW			///< black and white
}
LX_DE_CVI_TPG_COLOR_T;

/**
 * de captured test pattern generator.
 */
typedef struct {
	LX_DE_CVI_CHANNEL_T    cvi_channel; ///< CVI channel A or B
	BOOLEAN isPtnOn;					///< pattern enable
	BOOLEAN isFrzOn;					///< freeze enable
	BOOLEAN isGBR;						///< pattern color space
	LX_DE_CVI_TPG_COLOR_T ptnColor;		///< pattern color
}
LX_DE_CVI_TPG_T;

/**
 * de captured source color sub-sampling parameter.(for YUV422)
 */
typedef struct {
	LX_DE_CVI_CHANNEL_T    cvi_channel; ///< CVI channel A or B
	BOOLEAN is3tap;					///< sub_sample(0), 3-tap-filter[1:2:1] (1)
}
LX_DE_CSAMPLE_MODE_T;

/**
 * de captured source sync status parameter.(for debug)
 */
typedef struct {
	LX_DE_CVI_CHANNEL_T    cvi_channel; ///< CVI channel A or B
	BOOLEAN h;					///< hsync status
	BOOLEAN v;					///< vsync status
	BOOLEAN de;					///< de status
	BOOLEAN valid;				///< source valid
	UINT16 lnum;				///< current line number
}
LX_DE_CVI_SYNC_STATUS_T;

/**
 * de colorimetry information of mpeg decoder (vdec).
 * used for kdriver
 */
typedef struct {
	LX_DE_WIN_ID_T win_id;	///< window index
	LX_DE_COLOR_STD_T colorimetry;	///< colorimetry of MPEG (601 or 709)
}
LX_DE_MVI_COLORIMETRY_T;

/**
 * de video layer type definition.
 * layer order of video,osd1,2,3 can be changed
 */

typedef enum {
	LX_SURFACE_VIDEO = 0,	///< video layer
	LX_SURFACE_OSD1 = 1,	///< osd layer 1
	LX_SURFACE_OSD2 = 2,	///< osd layer 2
	LX_SURFACE_OSD3 = 3,	///< osd layer 3
	LX_SURFACE_CURSOR = 4,	///< osd cursor layer
	LX_SURFACE_MAX			///< max number
}
LX_DE_SURFACE_T;

/**
 * de video layer(z-order) control type.
 */

typedef struct {
	BOOLEAN enable;				///< z-list on/off (off : hw default order, win0-bottom/win1-top)
	UINT32 z_list[2];			///< index : order(0->top, 1->bottom) , value : win_id
	UINT32 alpha[2];			///< index : order(0->top, 1->bottom) , value : blending alpha
}
LX_DE_ZLIST_T;

/* ############### DE CVE  ################ */

/**
 * de DENC path selection
 * with OSD: set RGB to YUV mdm csc and mdm csc enable(use default csc coef.)
 * w/o OSD(PE1): set RGB to YUV mdm csc and mdm csc enable(use default csc coef.)
 * refer DE0B_CTRL/MDM_CTRL/MDM_CSC_COEF
 * w/o OSD(OV) : mdm csc disable
 */
typedef enum {
	LX_DE_CVE_WITH_OSD = 0,		///< Denc path feedback with OSD(DE end)
	LX_DE_CVE_WITHOUT_OSD,		///< Denc path feedback w/o OSD(select OV or PE1)
	LX_DE_CVE_FROM_SRC			///< Denc path from source ( original aspect ratio)
}
LX_DE_CVE_PATH_T;

/**
 * de DENC timing standard
 * PAL and SECAM have same sync parameters
 */
typedef enum {
	LX_DE_CVE_NTSC = 0,		///< Denc type is NTSC 60Hz
	LX_DE_CVE_PAL,			///< Denc type is PAL 50Hz
	LX_DE_CVE_SECAM			///< Denc type is SECAM 50Hz
}
LX_DE_CVE_TIMING_T;

/**
 * de CVE parameter.
 *
*/
typedef struct
{
	LX_DE_CVE_PATH_T	fb;		///< CVE feedback path (0: with OSD 1: w/o OSD, 2: src)
	LX_DE_CVE_TIMING_T std;		///< CVE timing selection(0: NTSC 1: PAL 2:SECAM)
}
LX_DE_CVE_PARAM_T;

/**
 * de register value read type.
 */

typedef struct {
	UINT32 wr_addr; ///< register address
	UINT32 wr_data; ///< write data
}
LX_DE_REG_WR_T;


/**
 * IPC for VCS
 * index of IPC for VCS
 * 0  : frame_inx
 * 1  : pic_info
 * 2  : frame_rate
 * 3  : aspect_ratio
 * 4  : picture_size
 * 5  : h_offset
 * 6  : v_offset
 * 7  : frameupdated
 * 8  : y_frame_base_address
 * 9  : c_frame_base_address
 * 10 : y_frame_offset
 * 11 : c_frame_offset
 * 12 : stride
 * 13 : pixel_aspect_ratio
 * 14 : vdi_framepackarrange
 * 15 : vdi_ptsinfo
 */
typedef struct
{
	UINT32			inx;		///< index of vcs for IPC
	UINT32			data;		///< data of vcs for IPC
}
LX_DE_VCS_IPC_T;

/**
 * de FW Download.
 */
typedef struct {
	int  inx;					///< index --> 0:download all in de_fw.h, 1:led_fw, 2:de_iram, 3:de_ddr and run program
	int  size;					///< size  --> size of Firmware
	char *pData;				///< data  --> pointer carring data of Firmware
}
LX_DE_FW_DWLD_T;


/**
 * de DVR display format control. (de -> venc)
 */
typedef struct {
	BOOLEAN bOnOff;
	LX_DE_SCAN_T scan;
	UINT16 h_size;
	UINT16 v_size;
	UINT32 fr_rate;
	UINT32 baseAddr;
}
LX_DE_DVR_DISFMT_T;

typedef struct {
	UINT32 phyFrameAddrY8;
	UINT32 phyFrameAddrC8;
	UINT32 phyFrameAddrY2;
	UINT32 phyFrameAddrC2;
	UINT32 stride;
}
LX_DE_FRAME_ADDR;

/**
 * de read/write video frame before CVI csc parameter for self-diagnosis.
 */
typedef struct
{
	LX_DE_GRAB_REGION_T region; ///< grapped region
	LX_DE_GRAB_COLOR_T	*color; ///< grapped pixels color value
	BOOLEAN bReadOnOff;			/// Read(True), Write(False)
	LX_DE_FRAME_ADDR frameAddr;// phy address
}
LX_DE_CVI_RW_VIDEO_FRAME_T;

/**
 * * de read/write video frame before CVI csc parameter for self-diagnosis.
 * */
typedef struct
{
	LX_DE_WIN_ID_T win_id;
	LX_DE_FRAME_ADDR frameAddr;// phy address
	LX_DE_SCALER_CSAMPLE_T csample;
	LX_DE_RECT_T rect;
}
LX_DE_CVI_RW_PREW_FRAME_T;

/**
 * input port for multi window
 */
typedef enum {
	LX_DE_PORT_MVI,
	LX_DE_PORT_CVD,
	LX_DE_PORT_ADC,
	LX_DE_PORT_HDMI
}
LX_DE_INPUT_PORT;

/**
 * de source for multi window
 */
typedef enum {
	LX_DE_MULTI_IN_CVD_ADC,
	LX_DE_MULTI_IN_CVD_HDMI,
	LX_DE_MULTI_IN_CVD_MVI,
	LX_DE_MULTI_IN_CVD_CPU,
	LX_DE_MULTI_IN_CVD_CVD,
	LX_DE_MULTI_IN_ADC_CVD,
	LX_DE_MULTI_IN_ADC_HDMI,
	LX_DE_MULTI_IN_ADC_MVI,
	LX_DE_MULTI_IN_ADC_CPU,
	LX_DE_MULTI_IN_ADC_ADC,
	LX_DE_MULTI_IN_HDMI_CVD,
	LX_DE_MULTI_IN_HDMI_ADC,
	LX_DE_MULTI_IN_HDMI_MVI,
	LX_DE_MULTI_IN_HDMI_CPU,
	LX_DE_MULTI_IN_HDMI_HDMI,
	LX_DE_MULTI_IN_MVI_CVD,
	LX_DE_MULTI_IN_MVI_ADC,
	LX_DE_MULTI_IN_MVI_HDMI,
	LX_DE_MULTI_IN_MVI_CPU,
	LX_DE_MULTI_IN_MVI_MVI,
	LX_DE_MULTI_IN_CPU_CVD,
	LX_DE_MULTI_IN_CPU_ADC,
	LX_DE_MULTI_IN_CPU_HDMI,
	LX_DE_MULTI_IN_CPU_MVI,
	LX_DE_MULTI_IN_CPU_CPU,

	LX_DE_MULTI_IN_MVA_MVB,
	LX_DE_MULTI_IN_HDMIA_HDMIB,
	LX_DE_MULTI_IN_MAX
}
LX_DE_MULTI_SRC_T;

/**
 * de input port for multi window source mapping
 */
typedef struct {
	LX_DE_WIN_ID_T win_id;					///< window index
	LX_DE_INPUT_PORT in_port;			///< input port for multi window
}
LX_DE_MULTI_WIN_SRC_T;


/**
 * capture/freeze on or off for cvi capture
 */
typedef struct {
	BOOLEAN bCaptureOnOff;
	BOOLEAN bCviFreezeOnOff;			///< input port for multi window
}
LX_DE_CVI_CAPTURE_FREEZE_T;

/**
 * PQ Firmware Control
 */
typedef struct {
	UINT8 enable;
}
LX_DE_PQ_FIRMWARE_CTRL;

/**
 * PE1 Sharpness Coring Control_05
 */
typedef struct {
	UINT8 gt_th0;
	UINT8 gt_th1;
}
LX_DE_SHP_CORING_CTRL_05;

/**
 * PE1 Sharpness Coring Control_06
 */
typedef struct {
	UINT8 gt_th0a;
	UINT8 gt_th0b;
}
LX_DE_SHP_CORING_CTRL_06;

/**
 * PE1 Sharpness Coring Control_09
 */
typedef struct {
	UINT8 edge_gain_b;
	UINT8 edge_gain_w;
	UINT8 texture_gain_b;
    UINT8 texture_gain_w;
}
LX_DE_SHP_CORING_CTRL_09;

/**
 * PE1 Sharpness DP Control_01
 */
typedef struct {
	UINT8 edge_gain_b;
	UINT8 edge_gain_w;
	UINT8 texture_gain_b;
    UINT8 texture_gain_w;
}
LX_DE_SHP_DP_CTRL_01;


/**
 * PE1 Sharpness DERH Control_08
 */
typedef struct {
	UINT8  e_gain_th1;
	UINT8  e_gain_th2;
}
LX_DE_SHP_DERH_CTRL_0B;

/**
 * PE1 FMD Control
 */
typedef struct {
	UINT8 fmd_enable;
}
LX_DE_FMD_CTRL;

/**
 * Mute Control
 */
typedef struct {
	UINT8 mute_enable;
}
LX_DE_MUTE_CTRL;

/**
 * TNR Auto Control
 */
typedef struct {
	UINT8 auto_enable;
}
LX_DE_TNR_CTRL;

/**
 * Scaler Control
 */
typedef struct {
	UINT8 set_cnt;
}
LX_DE_SCL_CTRL;

/**
 * SRC Info
 */
typedef struct {
	UINT8 dtv_play_type;
}
LX_DE_SRC_INFO;

/**
 * PQ Firmware IPC Control
 */
typedef struct {
    LX_DE_PQ_FIRMWARE_CTRL    pq_firmware_ctrl;
    LX_DE_SHP_CORING_CTRL_05  shp_coring_ctrl_05;
	LX_DE_SHP_CORING_CTRL_06  shp_coring_ctrl_06;
    LX_DE_SHP_CORING_CTRL_09  shp_coring_ctrl_09;
    LX_DE_SHP_DP_CTRL_01      shp_dp_ctrl_01;
    LX_DE_SHP_DERH_CTRL_0B    shp_derh_ctrl_0B;
    LX_DE_FMD_CTRL            fmd_ctrl;
    LX_DE_MUTE_CTRL           mute_ctrl;
    LX_DE_TNR_CTRL            TNR_ctrl;
    LX_DE_SCL_CTRL            scl_ctrl;
    LX_DE_SRC_INFO            src_info;
}
LX_DE_PQ_IPC_CTRL;

typedef struct {
	UINT32 start : 12;
	UINT32 count : 12;
	UINT32 winId : 4;
} PQL_TABLE_HEAD_T;

/**
 * de parameter as to capture raw image used for debugging
 */
typedef struct {
	UINT32	mode;
	UINT32	*base_addr;
	UINT32	size;
}
LX_DE_PAR_CAPTURE_RAW_IMAGE_T;

/**
 * de get low delay info
 */
typedef struct {
	UINT32	wId_enable : 4;  // 3:0;	send wId to F/W ans get No_Delay enable status from F/W
	UINT32	lock   : 4;  // 7:4;
	UINT32	time   : 14; // 21:8;
	UINT32	frame  : 10; // 31:22;
	UINT32	locked_time;
}
LX_DE_LOW_DELAY_T;

/**
 *  * 3D formatter running mode parameter.
 *   * select 3D formatter operation mode. it controls 3D mode ON/OFF.
 *    */
typedef enum
{
	LX_DE_3D_RUNMODE_OFF = 0,	///< 2D mode, 3D off(bypass)
	LX_DE_3D_RUNMODE_ON,		///< 3D mode, 3D on
	LX_DE_3D_RUNMODE_3DTO2D,	///< 3D input 2D output
	LX_DE_3D_RUNMODE_2DTO3D,	///< 3D input 2D output
	LX_DE_3D_RUNMODE_MAX		///< max number
}
LX_DE_3D_RUNMODE_T;

/**
 *  * 3D formatter image format parameter.
 *   * used for input image format and output image format.
 *    */
typedef enum {
	LX_DE_3D_IMG_IN_TOPNBOTTOM = 0,		///< top and bottom
	LX_DE_3D_IMG_IN_SIDEBYSIDE,			///< side by side
	LX_DE_3D_IMG_IN_QUINCUNX,				///< quincunx
	LX_DE_3D_IMG_IN_CHECKERBOARD,			///< check board
	LX_DE_3D_IMG_IN_FRAMEPACKING,			///< frame packing
	LX_DE_3D_IMG_IN_FRAMEINTERLACE,		///< frame packing interlace
	LX_DE_3D_IMG_IN_FIELDALTERNATE,		///< field alternate
	LX_DE_3D_IMG_IN_FRAMESEQ,				///< frame sequence
	LX_DE_3D_IMG_IN_LINEALTERNATIVE,		///< line alternative full
	LX_DE_3D_IMG_IN_SIDEBYSIDE_FULL,		///< side by side full
	LX_DE_3D_IMG_IN_DUAL_HD,				///< dual HD e.g.) One is mpeg-2 ts, the other is mpeg-4 ts scenario
	LX_DE_3D_IMG_IN_COLUMNALTERNATIVE,		///< column alternate(interleaving) - mpeg
	LX_DE_3D_IMG_IN_LINEALTERNATIVE_HALF,	///< line alternative half
	LX_DE_3D_IMG_IN_MAX					///< max number
}
LX_DE_3D_IMG_FMT_IN_T;

/**
 *  * 3D formatter image format parameter.
 *   * used for input image format and output image format.
 *    */
typedef enum {
	LX_DE_3D_IMG_OUT_SINGLE_2D    = 0,	///< single nomal
	LX_DE_3D_IMG_OUT_SINGLE_TB,			///< top and bottom
	LX_DE_3D_IMG_OUT_SINGLE_SS,			///< side by side
	LX_DE_3D_IMG_OUT_SINGLE_FS,			///< Frame Sequential(Interleaving)
	LX_DE_3D_IMG_OUT_DUAL_FULL,			///< dual screen
	LX_DE_3D_IMG_OUT_DUAL_TB,			///< dual top and bottom for H13 internal type
	LX_DE_3D_IMG_OUT_DUAL_SS,			///< dual side by side for H13 internal type
	LX_DE_3D_IMG_OUT_SINGLE_LA,			///< line alternative
	LX_DE_3D_IMG_OUT_MAX				///< max number
}
LX_DE_3D_IMG_FMT_OUT_T;

/**
 *  * 3D formatter Left Roght order parameter.
 *   * used for input image and output image.
 *    */
typedef enum
{
	LX_DE_3D_LR_ORDER_R_FIRST = 0,		///< right first
	LX_DE_3D_LR_ORDER_L_FIRST,			///< left first
	LX_DE_3D_LR_ORDER_MAX				///< max number
}
LX_DE_3D_LR_ORDER_T;

/**
 *  * 3D formatter input/output control type.
 *   *  0xFF   : invalid
 *    *  Others :   valid
 *     */
typedef struct {
	LX_DE_3D_RUNMODE_T		run_mode;			///< select formatter operation mode. it controls 3D mode ON/OFF.
	LX_DE_3D_IMG_FMT_IN_T		in_img_fmt;			///< input image format(top/bottom, sidebyside, checkboard, frame, field seq, dual 2d)
	LX_DE_3D_LR_ORDER_T 		in_lr_order;		///< input left/right first order
	LX_DE_3D_IMG_FMT_OUT_T 	out_img_fmt;		///< output image format(top/bottom, sidebyside, checkboard, frame, field seq)
	LX_DE_3D_LR_ORDER_T  	out_lr_order;		///< output left/right first order
}
LX_DE_3D_INOUT_CTRL_T;

/////////////////////////H13//////////////////////////////////////////////////
typedef enum {
	LX_DE_HDMI444_NONE,
	LX_DE_HDMI444_OFF,
	LX_DE_HDMI444_ON,
	LX_DE_HDMI444_MAX
}	LX_DE_HDMI444_MODE_T;

typedef enum {
	LX_DE_DIRECT_INPUT,
	LX_DE_DIRECT_OUTPUT,
	LX_DE_DIRECT_MAX
}	LX_DE_DIRECT_T;

typedef enum
{
	LX_DE_RW_SET,
	LX_DE_RW_GET,
	LX_DE_RW_MAX
}	LX_DE_RW_T;

typedef struct {
	LX_DE_WIN_ID_T  		win_id;
	LX_DE_RECT_T 		rect;
	LX_DE_DIRECT_T		direct_type;
	LX_DE_RW_T			rw_type;
} LX_DE_WIN_CONFIG_T;


typedef enum {
	LX_DE_IN_SRC_NONE,
	LX_DE_IN_SRC_VGA,
	LX_DE_IN_SRC_YPBPR,
	LX_DE_IN_SRC_ATV,
	LX_DE_IN_SRC_CVBS,
	LX_DE_IN_SRC_SCART,
	LX_DE_IN_SRC_HDMI,
	LX_DE_IN_SRC_MVI,
	LX_DE_IN_SRC_CPU,
	LX_DE_IN_SRC_FB,
	LX_DE_IN_SRC_MAX
} LX_DE_IN_SRC_T;


typedef struct {
	LX_DE_WIN_ID_T 		win_id;			///< Display window index
	LX_DE_IN_SRC_T 		inputSrc;		///< Input source type
	UINT32				inputSrcPort;	///< Input port info (CVI_A,CVI_B, MVI_0,1,2,3)
	UINT32				inputResv;		///< Reserve parameter
} LX_DE_INPUT_CFG_T;


typedef enum
{
	LX_DE_OPER_ONE_WIN,				///< No Video
	LX_DE_OPER_TWO_WIN,
   	LX_DE_OPER_3D,
	LX_DE_OPER_UD,
	LX_DE_OPER_VENC,
	LX_DE_OPER_MAX
}	LX_DE_OPER_CONFIG_T;

typedef enum
{
	LX_DE_UD_OFF,
	LX_DE_UD_ON,		// 4k x 2k
	LX_DE_UD_HALF,		// 2k x 2k
	LX_DE_UD_MAX,		// 2k x 2k
} LX_DE_UD_CTRL_T;

typedef struct
{
	BOOLEAN bOnOff;
	LX_DE_SCAN_T scan;
	UINT16	hSize;
	UINT16 	vSize;
	UINT16 	frRate;
}	LX_DE_VENC_FMT_T;

/**
 *	DE operation.
 *
 */
typedef struct
{
	LX_DE_WIN_ID_T 		win_id;			///< De window index
	LX_DE_OPER_CONFIG_T	operation;		///< operation flag

	UINT32				multiCtrl;		// use Two window
	LX_DE_UD_CTRL_T	udCtrl;			// UD mode
	LX_DE_3D_INOUT_CTRL_T 	ctrl3D;		// use 3D mode
	LX_DE_VENC_FMT_T	vencCtrl;		// use for venc src format
}	LX_DE_OPERATION_CTRL_T;

typedef enum
{
	LX_DE_SUB_OPER_OFF,			///<
	LX_DE_SUB_OPER_CAPTURE,		///<
	LX_DE_SUB_OPER_VENC,			///<
	LX_DE_SUB_OPER_SCART_OUT,	///<
	LX_DE_SUB_OPER_MAX
}	LX_DE_SUB_OPER_CONFIG_T;

/**
 *	DE sub operation.
 *
 */
typedef struct
{
	LX_DE_WIN_ID_T 				win_id;			///< De window index
	LX_DE_SUB_OPER_CONFIG_T	operation;		///< De window index

	UINT32						capture_enable;	// for capture
	LX_DE_VENC_FMT_T			vencCtrl;		// use for venc src format
}	LX_DE_SUB_OPERATION_CTRL_T;

/*
 * de location for capture.
 */
typedef enum
{
	LX_DE_LOCATION_SOURCE,		///< DE Input Source
	LX_DE_LOCATION_DISPLAY,		///< Display Source
	LX_DE_LOCATION_MAX			///< MAX
}	LX_DE_LOCATION_T;

/*
 * de pixel format parameter.
 */
typedef enum
{
	LX_DE_PIXEL_FORMAT_RGB_10,
	LX_DE_PIXEL_FORMAT_RGB_8,
	LX_DE_PIXEL_FORMAT_YCBCR444_10,
	LX_DE_PIXEL_FORMAT_YCBCR444_8,
	LX_DE_PIXEL_FORMAT_YCBCR422_10,
	LX_DE_PIXEL_FORMAT_YCBCR422_8,
	LX_DE_PIXEL_FORMAT_YCBCR420_10,
	LX_DE_PIXEL_FORMAT_YCBCR420_8,
	LX_DE_PIXEL_FORMAT_MAX
}	LX_DE_PIXEL_FORMAT_T;

/*
 * de capture type.
 */
typedef struct
{
	LX_DE_WIN_ID_T 			win_id;			///< De window index
	LX_DE_LOCATION_T		location;		///< direction info of source for capture
	LX_DE_RECT_T 			inWin;			///< input window for capture
	UINT16					wOutSize;		///< output width size for capture
	UINT16		 			hOutSize;		///< output height size for capture
	LX_DE_PIXEL_FORMAT_T	pixelFormat;	///< pixel data format
	UINT8 					*p_y_frame;		///< y frame data start address
	UINT8 					*p_cbcr_frame;	///< cbcr frame data start address
	UINT32					stride;			///< stride size
}	LX_DE_CAPTURE_CTRL_T;

typedef enum
{
	LX_DE_CONFIG_TYPE_ALL,			///<for setting all config
	LX_DE_CONFIG_TYPE_DISPLAY_DEVICE,
	LX_DE_CONFIG_TYPE_DISPLAY_MIRROR,
	LX_DE_CONFIG_TYPE_FRC,
	LX_DE_CONFIG_TYPE_3D,
	LX_DE_CONFIG_TYPE_MAX,
}	LX_DE_CONFIG_T;

typedef enum {
	LX_DE_DIS_DEV_LCD, 	///< LCD/LED device
	LX_DE_DIS_DEV_PDP,	///< PDP device
	LX_DE_DIS_DEV_OLED,	///< OLED device
	LX_DE_DIS_DEV_CP,		///< CP box device
	LX_DE_DIS_DEV_STB,	///< Set-top box device
	LX_DE_DIS_DEV_MAX		///< max number
}	LX_DE_DISPLAY_DEVICE_T;

typedef enum {
	LX_DE_DIS_MIRROR_OFF, 	///< diaplay mirror off
	LX_DE_DIS_MIRROR_X, 		///< diaplay mirror x position
	LX_DE_DIS_MIRROR_Y, 		///< diaplay mirror y position
	LX_DE_DIS_MIRROR_X_Y, 	///< diaplay mirror x, y position
	LX_DE_DIS_MIRROR_MAX		///< max number
}	LX_DE_DISPLAY_MIRROR_T;

typedef enum
{
	LX_DE_FRC_PATH_INTERNAL,
	LX_DE_FRC_PATH_EXTERNAL,
	LX_DE_FRC_PATH_MAX,
} LX_DE_FRC_PATH_T;

typedef enum
{
	LX_DE_3D_CTRL_OFF,
	LX_DE_3D_CTRL_ON,
	LX_DE_3D_CTRL_MAX,
} LX_DE_3D_CTRL_T;

/*
 * default : DIS_DEV - LCD,  FRC Path - Internal, 3D ctrl -ON
 */
typedef	struct
{
	LX_DE_CONFIG_T	config_type;

	LX_DE_DISPLAY_DEVICE_T	display_type;
	LX_DE_DISPLAY_MIRROR_T display_mirror;
	LX_DE_FRC_PATH_T		frc_type;
	LX_DE_3D_CTRL_T			trid_type;

}	LX_DE_IF_CONFIG_T;

typedef enum
{
	LX_DE_SYS_STATUS_ALL,
	LX_DE_SYS_STATUS_DISPALY_DEVICE,
	LX_DE_SYS_STATUS_DISPALY_MIRROR,
	LX_DE_SYS_STATUS_FRC,
	LX_DE_SYS_STATUS_3D,
	LX_DE_SYS_STATUS_FC_MEM,
	LX_DE_SYS_STATUS_DISPALY_SIZE,
	LX_DE_SYS_STATUS_MAX
}	LX_DE_SYS_STATUS_CONFIG_T;

/*
 * default : DIS_DEV - LCD,  FRC Path - Internal, 3D ctrl -ON
 */
typedef	struct
{
	LX_DE_SYS_STATUS_CONFIG_T	status_type; //type

	LX_DE_DISPLAY_DEVICE_T	display_type;		///< display type
	LX_DE_DISPLAY_MIRROR_T 	display_mirror;		///< display mirror type
	LX_DE_FRC_PATH_T		frc_type;
	LX_DE_3D_CTRL_T			trid_type;
	UINT32 				 	fc_mem;		// DDR address for capture
	LX_DE_PANEL_TYPE_T 		display_size;		///< display size
}	LX_DE_SYS_STATUS_T;

typedef enum
{
	LX_DE_SRC_STATUS_ALL,
	LX_DE_SRC_STATUS_INPUT_SRC,
	LX_DE_SRC_STATUS_OPER,
	LX_DE_SRC_STATUS_SUB_OPER,
	LX_DE_SRC_STATUS_MAX
}	LX_DE_SRC_STATUS_CONFIG_T;

/*
 * default : DIS_DEV - LCD,  FRC Path - Internal, 3D ctrl -ON
 */
typedef	struct
{
	LX_DE_SRC_STATUS_CONFIG_T	status_type;

	LX_DE_IN_SRC_T 				inSrc[2];		///< Input source type
	UINT32						inSrcPort[2];	///< Input port info (CVI_A,CVI_B, MVI_0,1,2,3)

	LX_DE_OPER_CONFIG_T		operType;
	LX_DE_SUB_OPER_CONFIG_T	subOperType;
	UINT16						operCtrlFlag;
	UINT16						subOperCtrlFlag;
}	LX_DE_SRC_STATUS_T;

/*
 * de Scaler Info type.
 */
typedef struct
{
	LX_DE_WIN_ID_T 				win_id;			///< De window index
	UINT16						width;			///< stride size
	UINT16						height;			///< stride size
	UINT32						stride;			///< stride size
	LX_DE_SCALER_CSAMPLE_T		sampling;		///< data format
	UINT8 						*p_y_frame;		///< y frame data start address
	UINT8 						*p_cbcr_frame;	///< cbcr frame data start address
}	LX_DE_SCALER_INFO_T;

typedef enum
{
	LX_DE_CVI_EXT_2D_FORMAT,				///< 2D format
	LX_DE_CVI_EXT_3D_FRAMEPACK,			///< 3D frame packing format
	LX_DE_CVI_EXT_3D_FIELD_ALTERNATIVE,	///< 3D field alternative format
	LX_DE_CVI_EXT_3D_LINE_ALTERNATIVE, 	///< 3D line alternative format(row interleaving)
	LX_DE_CVI_EXT_3D_SBSFULL,				///< 3D Side by Side Full format
	LX_DE_CVI_EXT_3D_L_DEPTH,			///< 3D L + Depth format
	LX_DE_CVI_EXT_3D_L_GRAPHICS,			///< 3D L + Depth + Graphics format
	LX_DE_CVI_EXT_3D_TNB,					///< 3D Top and Bottim format
	LX_DE_CVI_EXT_3D_SBS,					///< 3D Side by Side Half format
	LX_DE_CVI_EXT_4K_2K,					///< 4K x 2K format
	LX_DE_CVI_EXT_MAX						///< Ext format Max
} LX_DE_CVI_EXT_FORMAT_INFO_T;

/**
 * de cvi source attribute type.
 */
typedef struct {
	LX_DE_CVI_CHANNEL_T	   cvi_channel;		///< CVI channel A or B
	LX_DE_CVI_INPUT_SRC_T	   input_src;			///< cvi source type Information.
	LX_DE_CVI_DIMENSION_T	   size_offset;		///< h/v size offset
	LX_DE_SCAN_T			   scan;				///< interlaced or progressive
	LX_DE_CVI_EXT_FORMAT_INFO_T   extInfo;		///< extend format Information.
	LX_DE_CVI_SYNC_MODE_T	   de_mode;			///< internal data enable mode generated from sync(0) or de(1)
	LX_DE_COLOR_STD_T		   color;				///< color space 601,709,RGB
	LX_DE_CVD_COLOR_STD_T	   cvd_color_std;		///< CVD color mode
	LX_DE_CVI_YC_DELAY_T	   yc_delay;			///< CVD yc delay
	LX_DE_CVI_SAMPLING_MODE_T  sampling_rate;	///< external input sampling: noraml(0), double(1), quadrature(2)
	LX_DE_SCALER_CSAMPLE_T	   	sc_sample;		///< scaler sampling mode YUV420(0) YUV422(1) YUV444(2)
	UINT16						fir_c_enable;	///< c fir enable
}
LX_DE_CVI_SRC_T;

/**
 * de Debug argument for evaluation.
 */
typedef enum {
	LX_DE_EVAL_CMD_DBI,
	LX_DE_EVAL_CMD_MAX
}
LX_DE_EVAL_CMD_T;

typedef struct {
	int  argc;
	char **argv;
}
LX_DE_EVAL_ARG_T;

typedef struct {
	LX_DE_EVAL_CMD_T cmd;
	LX_DE_EVAL_ARG_T arg;
	UINT32 *prm;
	UINT32 size;
}
LX_DE_SET_EVAL_DBG_T;

//////////////////////////////////////////////////////////new

/**
 * de input source path parameter.
 */
typedef struct {
	LX_DE_WIN_ID_T		srcId;		///< Source index
	LX_DE_IN_SRC_T 		srcType;	///< Input source type
	UINT32				srcPort;	///< Input source port info
	UINT32				srcAttr;	///< Input source addribute
} LX_DE_INPUT_SOURCE_T;

/**
 * de display device index.
 */
typedef enum {
	LX_DE_OUT_DEV_NONE,	///< Video To Display
	LX_DE_OUT_DEV_VTD,	///< Video To Display
	LX_DE_OUT_DEV_VTV,	///< Video To Venc
	LX_DE_OUT_DEV_VTC,	///< Video To Capture
	LX_DE_OUT_DEV_VTM,	///< Video To Memory
	LX_DE_OUT_DEV_VTS,	///< Video To Scart-Out
	LX_DE_OUT_DEV_MAX
} 	LX_DE_OUTPUT_DEVICE_T;

/**
 * de display device path parameter.
 */
typedef struct {
	LX_DE_WIN_ID_T			srcId;		///< Source index
	LX_DE_OUTPUT_DEVICE_T 	outDev;	    ///< output device type
	UINT16					devId;		///< device port info
} LX_DE_OUTPUT_MODE_T;

/**
 * de vt extention parameter which has the write-only property.
 */
typedef struct {
	LX_DE_LOCATION_T            location;   ///< capture location source(0)/display(0) [ro]
	LX_DE_RECT_T				crop_size;	///< source crop size infomation [wo]
	LX_DE_RECT_T				w_out_size; ///< window size infomation [wo]
	UINT32                      rate;       ///< frame rate [rw]
	UINT32                      frz;        ///< flag of freeze(1) / release(0) [rw]
} LX_DE_VT_EXTENTION_INFO_T;

/* New parameters for supporting VTM(Video To Memory)  */
/*
 * de Video To Memory type. - context of shared memory
 */
typedef struct
{
	LX_DE_WIN_ID_T              win_id;     ///< De window index  [rw]
	UINT32                      address;    ///< physical header address [rw]
	BOOLEAN                     enable;     ///< enable control [rw]
	UINT32                      rate;       ///< frame rate [rw]
	UINT32                      frz;        ///< flag of freeze(1) / release(0) [rw]
	UINT32                      stride;     ///< stride size [ro]
	LX_DE_SCALER_CSAMPLE_T      sampling;   ///< data format [ro]
	UINT32                      y_frame[3]; ///< y frame data start address [ro]
	UINT32                      c_frame[3]; ///< cbcr frame data start address [ro]
	UINT32                      r_idx;      ///< read frame index [ro]
	UINT32                      mute;       ///< mute status [ro]
	LX_DE_RECT_T                srcSize;    ///< source size infomation [rw]
	LX_DE_RECT_T                winSize;    ///< window size infomation [rw]
	UINT16                      mirror;     ///< mirror info.
	UINT16                      scan;       ///< scan info.	[ro]
	SINT32                      mem_fd;	    ///< file descriptor for frame memory [rw]
#ifdef _USE_VT_EXTENTION_
	LX_DE_VT_EXTENTION_INFO_T   ext_info;   ///< extention parameter [wo]
#endif
} LX_DE_VTM_FRAME_INFO_T;

/**
 * de display device index.
 */
typedef enum {
	LX_DE_FREEZE_DISP,		///< Display
	LX_DE_FREEZE_INPUT,	///< Input
	LX_DE_FREEZE_SUB,		///< sub
	LX_DE_FREEZE_MAX
} 	LX_DE_FREEZE_LOCATION_T;

/**
 * de enable image freezing in each windows.
 */
typedef struct {
	LX_DE_WIN_ID_T win_id;			///< window index
	BOOLEAN bEnable;				///< freeze enable
	LX_DE_FREEZE_LOCATION_T location;	///< freeze location
} LX_DE_WIN_FREEZE_T;

/**
 * de enable source mirror mode in each windows.
 */
typedef struct {
	UINT8 srcPort;		///< vdec source port
	UINT8 destPort;		///< mirror port
	UINT8 enable;		///< mirror enable
} LX_DE_SRC_MIRROR_T;

/**
 * de active format detection control in external inputs.
 */
typedef struct {
	UINT16 enable;		///< active format detection enable
	UINT16 chgFlag;		///< change format flag
} LX_DE_ACT_FORMAT_T;

/**
 *  tag information to firmware.
 */
typedef struct {
	UINT32 system_info;     ///< 0xDECAFE00 + 1:ASIC,2:FPGA
	UINT32 firmware_base;   ///<
	UINT32 de_frame_base;   ///<
	UINT32 de_grab_base;    ///<
	UINT32 de_prew_base;    ///<
	UINT32 be_frame_base;   ///<
	UINT32 de_shared_base;  ///<
} LX_DE_IPC_SYSTEM_T;

/**
 * de push mode parameter.
 */
typedef struct {
	LX_DE_WIN_ID_T		win_id;		///< window index
} LX_DE_PUSH_MODE_T;

/**
 * window and hdmi link .
 */
typedef struct {
	LX_DE_WIN_ID_T		win_id;		///< window index
	UINT32              port_num;
} LX_DE_HDMIPORT_T;

/**
 * de vt capacity parameters.
 */
 typedef struct {
	LX_DE_RECT_T           maxResolution;
	UINT8                  bLeftTopAlign;
	UINT8                  bSupportInputVideoDeInterlacing;
	UINT8                  bSupportDisplayVideoDeInterlacing;
	UINT8                  bSupportScaleUp;
	UINT32                 scaleUpLimitWidth;
	UINT32                 scaleUpLimitHeight;
	UINT8                  bSupportScaleDown;
	UINT32                 scaleDownLimitWidth;
	UINT32                 scaleDownLimitHeight;
	UINT32                 locationLimit;  ///< 0:source only, 1:display only, 2: dual location
} LX_DE_VT_CAPA_PARAM_T;

/**
 * get the capacity of chip .
 */
typedef struct {
	UINT32                 chip_id;
	LX_DE_WIN_ID_T         max_win;		///< max window number for visible plane
	UINT32                 max_layer;      ///< max layer of z-order
	LX_DE_RECT_T           max_in_size;    ///< max size of input window
	LX_DE_RECT_T           max_out_size;	///< max size of output window
	UINT32                 max_in_fr_rate;
	UINT32                 max_out_fr_rate;
	UINT32                 mvir_start_num;
	LX_DE_3D_IMG_FMT_OUT_T default_3d_out_fmt;
	LX_DE_VT_CAPA_PARAM_T  vtm_capa;
} LX_DE_CAPACITY_T;

/**
 * set sub window index.
 */
typedef struct {
	LX_DE_WIN_ID_T win_id;
} LX_DE_SET_SUB_WIN_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DE_DRV_H_ */

/** @} */
