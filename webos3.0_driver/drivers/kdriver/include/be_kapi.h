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


/** @file be_kapi.h
 *
 *	@author	    won.hur (won.hur@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.04.25
 *	@see
 */

#ifndef	_BE_KAPI_H_
#define	_BE_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	BE_IOC_MAGIC		'D'

/* histo bin num [00]~[31]:bin00~bin31 */
#define	BE_FW_HST_BIN	32

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
/* common */
#define BE_IOW_UPDATE_FIRMWARE     	_IOW(BE_IOC_MAGIC, 0, BE_FW_DWLD_T)
#define BE_IOW_RESET_SW            		_IO(BE_IOC_MAGIC, 1)

#define BE_IOW_TOP_SET_BYPASS			_IOW(BE_IOC_MAGIC, 2, LX_BE_BLOCK_BYPASS_T)
#define BE_IOR_TOP_GET_BYPASS			_IOR(BE_IOC_MAGIC, 3, LX_BE_BLOCK_BYPASS_T)
#define BE_IOW_TOP_TOGGLE_3D_LR			_IOW(BE_IOC_MAGIC, 4, LX_BE_3D_LR_T)
#define BE_IOW_TOP_SET_3D_IN_FORMAT 	_IOW(BE_IOC_MAGIC, 5, LX_BE_TOP_3D_IN_FORMAT_T)
#define BE_IOW_TOP_GET_3D_IN_FORMAT 	_IOW(BE_IOC_MAGIC, 6, LX_BE_TOP_3D_IN_FORMAT_T)
#define BE_IOW_TOP_SET_3D_OUT_FORMAT	_IOW(BE_IOC_MAGIC, 7, LX_BE_3D_OUT_FORMAT_T)
#define BE_IOW_TOP_SET_3D_CONVERGENCE	_IOW(BE_IOC_MAGIC, 8,	LX_BE_3D_CONVERGENCE_T)
#define BE_IOW_TOP_SET_BLACK_BAR		_IOW(BE_IOC_MAGIC, 9, LX_BE_BLACK_BAR_T)
#define BE_IOW_TOP_SET_VIDEO_MUTE		_IOW(BE_IOC_MAGIC, 10, LX_BE_VIDEO_MUTE_T)
#define BE_IOW_TOP_SET_DISPLAY_TYPE		_IOW(BE_IOC_MAGIC, 11, LX_BE_DISPLAY_TYPE_T)
//#define BE_IOW_TOP_SET_SPREAD_SPECT		// BE TOP에서 ARM에서 구현 필요
#define BE_IOW_TOP_SET_DEBUG_DISPLAY	_IOW(BE_IOC_MAGIC, 12, LX_BE_DEBUG_DISPLAY_T)
#define BE_IOW_SET_VID_OPERATION		_IOW(BE_IOC_MAGIC, 13, LX_BE_VID_OPERATION_T)
#define BE_IOW_SET_INPUT_SRC_TYPE		_IOW(BE_IOC_MAGIC, 14, LX_BE_SRC_TYPE_T)
#define BE_IOW_SET_ACTIVE_WINDOW_START	_IOW(BE_IOC_MAGIC, 15, LX_BE_ACTIVE_WINDOW_START_T)
#define BE_IOW_SET_ACTIVE_WINDOW_SIZE	_IOW(BE_IOC_MAGIC, 16, LX_BE_ACTIVE_WINDOW_SIZE_T)
#define BE_IOW_SET_REAL_CINEMA			_IOW(BE_IOC_MAGIC, 17, LX_BE_REAL_CINEMA_T)
#define BE_IOW_TOP_FIRMWARE_DEBUG		_IOW(BE_IOC_MAGIC, 18, BE_FIRMWARE_DEBUG_T)
#define BE_IOW_SET_UART_FOR_MCU			_IOW(BE_IOC_MAGIC, 19, BOOLEAN )
#define BE_IORW_REG_ACCESS				_IOWR(BE_IOC_MAGIC, 20, BE_REG_RW_T)
#define BE_IOW_ENABLE_DEBUG_PRINT		_IOW(BE_IOC_MAGIC, 21, BE_DEBUG_PRINT_T )
#define BE_IOR_GET_FIRMWARE_INFO_ADDR	_IOR(BE_IOC_MAGIC, 22, BE_FIRMWARE_INFO_T )
#define BE_IOR_GET_GIT_DATE_INFO        _IO(BE_IOC_MAGIC, 23)
#define BE_IOR_GET_BE_KERNEL_DRV_INFO	_IOR(BE_IOC_MAGIC, 24, LX_BE_KDRV_INFO_T)

#define BE_IOC_TOP_MAX					(_IOC_NR(BE_IOR_GET_BE_KERNEL_DRV_INFO) +1 )

/* H3D */
#define BE_IOR_H3D_GET_VERSION			_IOR(BE_IOC_MAGIC, BE_IOC_TOP_MAX, LX_BE_H3D_VERSION_T)
#define BE_IOW_H3D_RESET_BLOCK			_IOR(BE_IOC_MAGIC, BE_IOC_TOP_MAX+1, LX_BE_H3D_RESET_BLOCK_T)
#define BE_IOW_H3D_BYPASS_ALL				_IOR(BE_IOC_MAGIC, BE_IOC_TOP_MAX+2, LX_BE_H3D_BYPASS_ALL_T)
#define BE_IOW_H3D_SET_MODE				_IOW(BE_IOC_MAGIC, BE_IOC_TOP_MAX+3, LX_BE_H3D_MODE_T)
#define BE_IOW_H3D_GET_MODE				_IOW(BE_IOC_MAGIC, BE_IOC_TOP_MAX+4, LX_BE_H3D_MODE_T)
#define BE_IOW_H3D_IN_FORMAT				_IOW(BE_IOC_MAGIC, BE_IOC_TOP_MAX+5, LX_BE_H3D_IN_FORMAT_T)
#define BE_IOW_H3D_T3C_DEPTH_LEVEL		_IOW(BE_IOC_MAGIC, BE_IOC_TOP_MAX+6, LX_BE_H3D_T3C_DEPTH_LEVEL_T)
#define BE_IOW_H3D_A3C_DEPTH_LEVEL		_IOW(BE_IOC_MAGIC, BE_IOC_TOP_MAX+7, LX_BE_H3D_A3C_DEPTH_LEVEL_T)
#define BE_IOW_H3D_A3C_AUTOCONVERGENCE	_IOW(BE_IOC_MAGIC, BE_IOC_TOP_MAX+8, LX_BE_H3D_A3C_AUTOCONVERGENCE_T)
#define BE_IOW_H3D_SET_TRIDFMT                   _IOW(BE_IOC_MAGIC, BE_IOC_TOP_MAX+9, LX_BE_TRIDTV_FMT_CTRL_T)

#define BE_IOC_H3D_MAX					(_IOC_NR(BE_IOW_H3D_SET_TRIDFMT) +1 )

/* FRC */
#define BE_IOW_FRC_CONTROL_MEMC        		_IOW(BE_IOC_MAGIC, BE_IOC_H3D_MAX, LX_BE_FRC_MEMC_MODE_T)
//#define BE_IOW_FRC_SET_TRUEMOTION_DEMO 	_IOW(BE_IOC_MAGIC, BE_IOC_H3D_MAX+1, BOOLEAN)	//구현되는 것인가? 확인 필요. 기존 L9에서 가져온것임.
#define BE_IOW_FRC_SET_DEMO_MODE		_IOW(BE_IOC_MAGIC, BE_IOC_H3D_MAX+1, LX_BE_FRC_DEMO_MODE_T)
#define BE_IOW_FRC_SET_DEBUG_BOX			_IOW(BE_IOC_MAGIC, BE_IOC_H3D_MAX+2, LX_BE_FRC_ENABLE_DEBUG_BOX_T)
#define BE_IOW_FRC_SET_BYPASS_MEMC_REG	_IOW(BE_IOC_MAGIC, BE_IOC_H3D_MAX+3, LX_BE_FRC_MEMC_BYPASS_REGION_T)
#define BE_IOW_FRC_SET_YUV_444_MODE		_IOW(BE_IOC_MAGIC, BE_IOC_H3D_MAX+4, LX_BE_FRC_COLOR_YUV444_T)
#define BE_IOW_FRC_SET_LOW_DELAY			_IOW(BE_IOC_MAGIC, BE_IOC_H3D_MAX+5, LX_BE_FRC_LOW_DELAY_T)
#define BE_IOR_FRC_GET_FIRMWARE_VER		_IOR(BE_IOC_MAGIC, BE_IOC_H3D_MAX+6, LX_BE_FRC_VERSION_T)
#define BE_IOW_FRC_FORCE_FRAME_RATE		_IOW(BE_IOC_MAGIC, BE_IOC_H3D_MAX+7, LX_BE_FRC_FRAME_RATE_T)
#define BE_IOW_FRC_SET_FREEZE				_IOW(BE_IOC_MAGIC, BE_IOC_H3D_MAX+8, LX_BE_FRC_FREEZE_T)
#define BE_IOR_FRC_UPDATE_FRAME_DELAY		_IOR(BE_IOC_MAGIC, BE_IOC_H3D_MAX+9, LX_BE_LOW_DELAY_T)
#define BE_IOC_FRC_MAX					(_IOC_NR(BE_IOR_FRC_UPDATE_FRAME_DELAY) +1 )

/* SRE */
#define BE_IOW_SRE_SET_COMMAND			_IOW(BE_IOC_MAGIC, BE_IOC_FRC_MAX, LX_BE_SRE_CMN_T)
#define BE_IOW_SRE_SET_PARAMS				_IOW(BE_IOC_MAGIC, BE_IOC_FRC_MAX+1, LX_BE_SRE_PRAM_T)
#define BE_IOR_SRE_GET_VERSION				_IOR(BE_IOC_MAGIC, BE_IOC_FRC_MAX+2, LX_BE_SRE_VERSION_T)
#define BE_IOW_SRE_SET_INPUT_FORMAT		_IOW(BE_IOC_MAGIC, BE_IOC_FRC_MAX+3, LX_BE_SRE_INPUT_MODE_T)

#define BE_IOC_SRE_MAX					(_IOC_NR(BE_IOW_SRE_SET_INPUT_FORMAT) +1 )

/* LED */
#define BE_IOR_LED_GET_VERSION				_IOR(BE_IOC_MAGIC, BE_IOC_SRE_MAX, LX_BE_LED_VERSION_T)
#define BE_IOW_LED_SET_MODE				_IOW(BE_IOC_MAGIC, BE_IOC_SRE_MAX+1, LX_BE_LED_MODE_T)
#define BE_IOW_LED_CONTROL				_IOW(BE_IOC_MAGIC, BE_IOC_SRE_MAX+2, LX_BE_LED_CONTROL_T)
#define BE_IOW_LED_GET_APL				_IOR(BE_IOC_MAGIC, BE_IOC_SRE_MAX+3, LX_BE_LED_APL_T)
#define BE_IOW_LED_SET_CLIPPING_VALUE	_IOW(BE_IOC_MAGIC, BE_IOC_SRE_MAX+4, UINT32)
#define BE_IOW_LED_GET_REG_INFO			_IOW(BE_IOC_MAGIC, BE_IOC_SRE_MAX+5, LX_BE_LED_REG_INFO_T)

#define BE_IOR_HDR_GET_VERSION			_IOR(BE_IOC_MAGIC, BE_IOC_SRE_MAX+6, LX_BE_HDR_VERSION_T)
#define BE_IOW_HDR_SET_MODE				_IOW(BE_IOC_MAGIC, BE_IOC_SRE_MAX+7, LX_BE_HDR_MODE_T)
#define BE_IOW_HDR_CONTROL				_IOW(BE_IOC_MAGIC, BE_IOC_SRE_MAX+8, LX_BE_HDR_CONTROL_T)

#define BE_IOR_WCG_GET_VERSION			_IOW(BE_IOC_MAGIC, BE_IOC_SRE_MAX+9, LX_BE_WCG_VERSION_T)
#define BE_IOW_WCG_SET_MODE				_IOW(BE_IOC_MAGIC, BE_IOC_SRE_MAX+10, LX_BE_WCG_MODE_T)
#define BE_IOW_WCG_CONTROL				_IOW(BE_IOC_MAGIC, BE_IOC_SRE_MAX+11, LX_BE_WCG_CONTROL_T)

#define BE_IOC_LED_MAX					(_IOC_NR(BE_IOW_WCG_CONTROL) +1 )

/* PWM */
#define BE_IOW_PWM_SET_CONTROL				_IOW(BE_IOC_MAGIC, BE_IOC_LED_MAX, BE_PWM_CTRL_T)
#define BE_IOW_PWM_SET_ADAPT_FREQ_CONTROL	_IOW(BE_IOC_MAGIC, BE_IOC_LED_MAX+1, BE_PWM_ADAPT_FREQ_DB_T)
#define BE_IOW_PWM_SET_FREQUENCY			_IOW(BE_IOC_MAGIC, BE_IOC_LED_MAX+2, BE_PWM_FREQ_T)
#define BE_IOW_PWM_SET_DUTY_CYCLE			_IOW(BE_IOC_MAGIC, BE_IOC_LED_MAX+3, BE_PWM_DUTY_T)
#define BE_IOR_PWM_GET_EXTERNAL_PWM			_IOR(BE_IOC_MAGIC, BE_IOC_LED_MAX+4, BE_EXTERNAL_PWM_T)

#define BE_IOC_PWM_MAX						(_IOC_NR(BE_IOR_PWM_GET_EXTERNAL_PWM) +1 )

#define BE_IOW_STATUS_KEEPER_DBG			_IOW(BE_IOC_MAGIC, BE_IOC_PWM_MAX, BE_SKEEPER_DBG_T)

#define BE_IOC_STATUS_KEEPER_MAX			(_IOC_NR(BE_IOW_STATUS_KEEPER_DBG) +1 )

#define BE_IOC_MAXNR						BE_IOC_STATUS_KEEPER_MAX
/* OIF */
/*
#define BE_IOW_OIF_SET_VESA_JEIDA	_IOW(BE_IOC_MAGIC, 190, BOOLEAN)
#define BE_IOW_OIF_SET_BIT_DEPTH
#define BE_IOW_OIF_SET_SELECT_OUTCH
#define BE_IOW_OIF_SET_LVDS_OUTPUT
#define BE_IOW_OIF_SET_LVDS_POWER
#define BE_IOW_OIF_SET_DARK_SCREEN
#define BE_IOW_OIF_SET_LVDS_OUT_CTRL
#define BE_IOW_OIF_SET_LVDS_PORT_SWAP
*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions (Common)
----------------------------------------------------------------------------------------*/
typedef struct {
	UINT32	dbgMask;	// bit 0 : PRINT, bit 1 : ERROR, bit 2 : DEBUG, bit 3: TRACE
	UINT32	funcSupportDbgLevel; // 0 : error, 1 : warning, 2 : ignore
} BE_DEBUG_PRINT_T;

typedef enum {
	BE_FRC_MC_TYPE_OFF,
	BE_FRC_MC_TYPE_LOW,
	BE_FRC_MC_TYPE_MID,
	BE_FRC_MC_TYPE_HIGH,
	BE_FRC_MC_TYPE_USER,
	BE_FRC_MC_TYPE_55_PULLDOWN
} BE_FRC_MC_TYPE_T;

typedef enum {
	LX_BE_FRC_MEMC_OFF,LX_BE_FRC_MEMC_DISABLE = 0,	/* FW MEMC value (0:off, 1:low, 2:medium,  3:high, 4:user, 5: 55pull down)*/
	LX_BE_FRC_MEMC_LOW,
	LX_BE_FRC_MEMC_MIDIUM,
	LX_BE_FRC_MEMC_HIGH,
	LX_BE_FRC_MEMC_USER,
	LX_BE_FRC_MEMC_PULLDOWN,

	LX_BE_FRC_MEMC_ENABLE = 10,							/* DISABLE MEMC (0:off) BNABLE (Call Recently stored MEMC value) */
	LX_BE_FRC_OPEN,
	LX_BE_FRC_CLOSED,
	LX_BE_FRC_MEMC_MAX
} LX_BE_FRC_CONT_T; //130531 kyungryun.lee

typedef enum {
	LX_BE_HDR_DCR_BYPASS_OFF =0,
	LX_BE_HDR_DCR_BYPASS_ON,
	LX_BE_HDR_DCR_BYPASS_INTERNAL_OFF,	//HDR DCR bypass just for DC Internal bypass
	LX_BE_HDR_DCR_BYPASS_INTERNAL_ON,	//HDR DCR bypass just for DC Internal bypass
} LX_BE_LED_CONT_T;  //130531 kyungryun.lee

typedef enum {
	BE_REG_RW_TYPE_NONE = 0,
	BE_REG_RW_TYPE_DPPB_TOP,
	BE_REG_RW_TYPE_DPPB_TXM,
	BE_REG_RW_TYPE_DPPB_LED,
	BE_REG_RW_TYPE_FRC_LRX,
	BE_REG_RW_TYPE_FRC_P3D,
	BE_REG_RW_TYPE_FRC_DVI,

	BE_REG_RW_TYPE_MAXNUM
} BE_REG_RW_TYPE_T;

typedef struct
{
	unsigned int addr;
	unsigned int size;
} BE_FIRMWARE_INFO_T;

/**
 *	be fw histo data info
 *	- apl_status :
 *		[3:0] valid read buf num : 0~2
 *		[7:4] fw writing buf num : 0~2
 *	- histo_status :
 *		[3:0] valid read buf num : 0~2
 *		[7:4] fw writing buf num : 0~2
 *		[31:24] sat_status : 0~255
 */
typedef struct
{
	UINT32 apl_buf0_ysv;					///< apl y buffer 0, [9:0]y, [19:10]s, [29:20]v
	UINT32 apl_buf1_ysv;					///< apl y buffer 1, [9:0]y, [19:10]s, [29:20]v
	UINT32 apl_buf2_ysv;					///< apl y buffer 2, [9:0]y, [19:10]s, [29:20]v
	UINT32 apl_status;						///< apl_status
	UINT32 histo_buf0_y[BE_FW_HST_BIN+1];	///< histo y buffer 0, [32]:valid mask
	UINT32 histo_buf1_y[BE_FW_HST_BIN+1];	///< histo y buffer 1, [32]:valid mask
	UINT32 histo_buf2_y[BE_FW_HST_BIN+1];	///< histo y buffer 2, [32]:valid mask
	UINT32 histo_buf0_h[BE_FW_HST_BIN+1];	///< histo h buffer 0, [32]:valid mask
	UINT32 histo_buf1_h[BE_FW_HST_BIN+1];	///< histo h buffer 1, [32]:valid mask
	UINT32 histo_buf2_h[BE_FW_HST_BIN+1];	///< histo h buffer 2, [32]:valid mask
	UINT32 histo_buf0_s[BE_FW_HST_BIN+1];	///< histo s buffer 0, [32]:valid mask
	UINT32 histo_buf1_s[BE_FW_HST_BIN+1];	///< histo s buffer 1, [32]:valid mask
	UINT32 histo_buf2_s[BE_FW_HST_BIN+1];	///< histo s buffer 2, [32]:valid mask
	UINT32 histo_status;					///< histo_status
} BE_FW_HISTO_INFO_T;

/* When adding meta data to this structure,
 * please add your data size to fit in 32-bit align
 * so to avoid memory fragmentation */

typedef struct
{
	UINT32 	dviTodvoRealDelay;
	UINT32	dviToDvoTargetDelay;
	BE_FW_HISTO_INFO_T histoInfo;
} BE_FW_INFO_T;


typedef struct
{
	BOOLEAN	isRead;
	BE_REG_RW_TYPE_T type;
	UINT32	addr;
	UINT32	data;
} BE_REG_RW_T;

typedef enum {
	BE_FW_DWLD_TYPE_FRC = 0,
	BE_FW_DWLD_TYPE_SRE,
	BE_FW_DWLD_TYPE_WCG,
	BE_FW_DWLD_TYPE_LED,
	BE_FW_DWLD_TYPE_HDR,
	BE_FW_DWLD_TYPE_MAXNUM,

	BE_FW_DWLD_TYPE_ALL = BE_FW_DWLD_TYPE_MAXNUM
} BE_FW_DWLD_TYPE_T;

typedef struct {
	UINT32	size;	// 0 : header or file download
	CHAR	*pData;	// NULL : header or file download
	BE_FW_DWLD_TYPE_T	type;
} BE_FW_DWLD_T;

typedef enum {
	BE_PWM0 = 0, ///< pwm port0
	BE_PWM1,		///< pwm port1
	BE_PWM2,		///< pwm port2
	BE_PWM3,		///< pwm port3
	BE_PWM_MAX
} BE_PWM_ID_T;

typedef enum {
	BE_PWM_LOW	= 0,		///< PWM Output Signal Cotrol - Low
	BE_PWM_NORMAL,			///< PWM Output Signal Cotrol - Normal
	BE_PWM_HIGH,				///< PWM Output Signal Cotrol - High
	BE_PWM_INVERSION,		///< PWM Output Signal Cotrol - Inversion
} BE_PWM_OUT_CTRL_T;

typedef struct
{
	BE_PWM_ID_T			port;		///< PWM number: PWM0~2.
	BOOLEAN					enable;
	BOOLEAN					pwmMode;	///< freerun(0) / locking(1) mode
	BE_PWM_OUT_CTRL_T		pwmOutput;	///< PWM Output signal control
} BE_PWM_CTRL_T;

typedef struct
{
	UINT32			freq_48nHz;	///< PWM Output Frequency on 48xN Hz Vsync
	UINT32			freq_50nHz; ///< PWM Output Frequency on 50xN Hz Vsync
	UINT32			freq_60nHz; ///< PWM Output Frequency on 60xN Hz Vsync
} BE_PWM_ADAPT_FREQ_DB_T;

typedef struct
{
	BE_PWM_ID_T				port;	///< PWM number: PWM0~2.
	BOOLEAN						enable;
	BE_PWM_ADAPT_FREQ_DB_T	freqDb;	///< PWM Output Frequency DB
} BE_PWM_ADAPT_FREQ_CTRL_T;

typedef struct
{
	BE_PWM_ID_T			port;		///< PWM number: PWM0~2.
	UINT32					frequency;	///< PWM Output Frequency
} BE_PWM_FREQ_T;

typedef struct
{
	BE_PWM_ID_T	port;		///< PWM number: PWM0~2.
	UINT16			duty;		///< Set duty of PWM (0 ~ 255).
	UINT16			offset;		///< PWM Start Position (0 ~ 255).
} BE_PWM_DUTY_T;

/**
 * BE PWM parameter.
 * get PWM Duty value for external port(FRC) .
 *
*/
typedef struct
{
	UINT16			frequency;		///< current frequency (locking mode frequncy)
    	UINT16			duty;		    	///< current duty of PWM (0 ~ 255).
} BE_EXTERNAL_PWM_T;

typedef struct
{
	UINT16  	u16ConvergenceVal;
	UINT16 	u16ConvergenceEnable;	// 0 : Convergence auto disable,
									// 1 : Convergence auto enable with 3D depth info
} LX_BE_3D_CONVERGENCE_T;

typedef struct
{
	UINT16	bOutLR; 	// 0: Left/Top First, 1: Right/Bottom First
	UINT16  	reserved;
} LX_BE_3D_LR_T;

typedef struct
{
	UINT8 	u8BE3DInFormat;	// 0 : 2D, 1 : S/S, 2 : T/B, 3: Dual
	UINT8	u8BE3DInType;	// 0 : 3D Normal, 0 : Dual Stream
	UINT8	u8BE3DInSize ;	// 0 : 1920x1080, 1 : 960x1080, 2 : 1920x540, 3 : UD
	UINT8	_rsvd;
}LX_BE_TOP_3D_IN_FORMAT_T;

typedef struct
{
	UINT8 	u8OutFormat;	//	0 : PG,	1 : SG,	2 : FS,
	UINT8	u8PGMode; 		//	0 : Normal PG, 1 : FPR
	UINT8	u8GSyncEnable;	// 	0 : Glass Sync disable, 1 : Glass Sync Enable for SG
	UINT8	u8Single;		// 	0 : Dual View, 1 : Single View
} LX_BE_3D_OUT_FORMAT_T;

typedef struct
{
	UINT8 	u8BarOn;
	UINT8	u8BarVal;
	UINT8	u8BarStep;
	UINT8	reserved;
} LX_BE_BLACK_BAR_T;

typedef struct
{
	UINT32 	u32RealCinemaEnable;
} LX_BE_REAL_CINEMA_T;

typedef struct
{
	BOOLEAN		bVideoMuteOn;		// 0 : Video Mute off,	1 : Video Mute On
	UINT32		u32DEInRate;		// DE Video input frame rate (0:15, 1:20, 2:24, 3:25, 4:30, 5:48, 6:50, 7:60, 0xF:N/A)
	UINT32		u32DEOutRate;		// DE Video output frame rate (0:15, 1:20, 2:24, 3:25, 4:30, 5:48, 6:50, 7:60, 0xF:N/A)
} LX_BE_VIDEO_MUTE_T;

typedef struct
{
	BOOLEAN		bH3DBypass;	//	0: H3D Normal, 1: H3D(2D-3D,Depth Block) Bypass
	BOOLEAN		bFRCBypass;		//	0: FRC Normal, 1: FRC Bypass
	BOOLEAN		bSREBypass;		//	0: SRE Normal, 1: SRE Bypass
	BOOLEAN		bPEBypass;		//	0: PE Normal, 1: PE Bypass
	BOOLEAN		bLEDBypass;		//	0: LED Normal, 1: LED Bypass
	BOOLEAN		bHDRBypass;	//	0: HDR Normal, 1: HDR Bypass
	BOOLEAN		bWCGBypass;	//	0: WCG Normal, 1: WCG Bypass
} LX_BE_BLOCK_BYPASS_T;

typedef struct
{
	UINT32		u32BEPanelType;	// 0 : FHD,	 1 : UD,
	UINT32		u32BEPanelRate;		// 0 : 60Hz, 1 : 120Hz
	UINT32		u32BEPanelReverse;	// 0 : Normal,	1 : Reverse
} LX_BE_DISPLAY_TYPE_T;

typedef struct
{
	UINT8	u8H3DDebugDisplay;
	UINT8	u8FRCDebugDisplay;
	UINT8	u8PEDebugDisplay;
	UINT8	u8LEDDebugDisplay;
} LX_BE_DEBUG_DISPLAY_T;

typedef struct
{
	UINT32	u32Mode;			// 0 : 2D,	1 : PIP, 2 : V_ENC On
} LX_BE_VID_OPERATION_T;

typedef struct
{
	UINT32	u32SrcInputType;	// 0 : HDMI,	1 : Component, 2 : Composite, 3 : ETC
	UINT32	u32SrcInputSize;	// 1080, 720, 480
} LX_BE_SRC_TYPE_T;

typedef struct
{
	UINT16	u16ActiveWindowStartX;
	UINT16 	u16ActiveWindowStartY;
} LX_BE_ACTIVE_WINDOW_START_T;

typedef struct
{
	UINT16	u16ActiveWindowWidth;
	UINT16	u16ActiveWindowHeight;
}LX_BE_ACTIVE_WINDOW_SIZE_T;

typedef struct
{
	UINT8	u8Read;
	UINT32	u32Cmd;
	UINT32	u32Param;
} BE_FIRMWARE_DEBUG_T;

typedef struct
{
	UINT16	u16A3CVersion;
	UINT16	u16T3CVersion;

} LX_BE_H3D_VERSION_T;

typedef struct
{
	UINT16	bInitA3C;
	UINT16	bInitT3C;
} BE_H3D_INIT_BLOCK_T;  // 0624 kim back to original lx naming to be naming

typedef struct
{
	UINT32	bReset;
} LX_BE_H3D_RESET_BLOCK_T;

typedef struct
{
	UINT32	bBypass;
} LX_BE_H3D_BYPASS_ALL_T;

typedef struct
{
	UINT16	bA3CMode; // 0 : off, 1: on
	UINT16	bT3CMode; // 0 : off, 1: on
} LX_BE_H3D_MODE_T;

typedef struct
{
	UINT32	u32H3DInFormat;	// 0: 1920x1080(FHD), 1:960x1080(SS), 2:1920x540(TB)
} LX_BE_H3D_IN_FORMAT_T;

typedef struct
{
	UINT32	u32T3CDepthLevel;			// Depth level : 0 ~ 20

} LX_BE_H3D_T3C_DEPTH_LEVEL_T;

typedef struct
{
	UINT32	u32A3CDepthLevel;	// Depth level : 0 ~ 20
} LX_BE_H3D_A3C_DEPTH_LEVEL_T;

typedef struct
{
	UINT32	bA3CAutoConvergence; // 0 : Auto Convergence off, 1 : Auto Convergence on
} LX_BE_H3D_A3C_AUTOCONVERGENCE_T;

typedef enum
{
	LX_INPUT_LR_SEQ = 0,
	LX_INPUT_RL_SEQ,
	LX_INPUT_LR_MAX,
} LX_TRIDTV_INPUT_LR_SEQ_T;

typedef enum
{
	LX_TRIDTV_3DMODE_OFF = 0,
	LX_TRIDTV_3DMODE_ON,
	LX_TRIDTV_3DMODE_2DTO3D,
	LX_TRIDTV_3DMODE_3DTO2D,
	LX_TRIDTV_3DMODE_MAX,
} LX_TRIDTV_3DMODE_FLAG_T;

typedef enum
{
	LX_SC_OUT_SINGLE_2D = 0,
	LX_SC_OUT_SINGLE_TB,
	LX_SC_OUT_SINGLE_SS,
	LX_SC_OUT_SINGLE_FS,
	LX_SC_OUT_DUAL_FHD,
	LX_SC_OUT_DUAL_TB,
	LX_SC_OUT_DUAL_SS,
	LX_SC_OUT_SINGLE_T,
	LX_SC_OUT_MAX,
} LX_TRIDTV_SC_OUTPUT_TYPE_T;

typedef struct{
	LX_TRIDTV_3DMODE_FLAG_T lx_mode;
	LX_TRIDTV_SC_OUTPUT_TYPE_T lx_in_fmt;
	LX_TRIDTV_INPUT_LR_SEQ_T	lx_lr;
	UINT8	depthCtrl;
	SINT8	convCtrl;
	BOOLEAN	bAutoConvMode;
} LX_BE_TRIDTV_FMT_CTRL_T;

typedef struct
{
    	UINT16	u16Version;
    	UINT16	u16Subversion;
} LX_BE_FRC_VERSION_T;

typedef struct
{
	UINT8	u8Blur;							// 0~9 : Blur level
    	UINT8	u8Judder;						// 0~9 : Judder level
    	UINT8	u8MemcType;					// 0 : OFF, 1: LOW, 2:MID, 3:HIGH, 4:USER, 5: PULLDOWN55 : @ref FRC_MC_TYPE_T
    	UINT8	_rsvd;							//reservved
} LX_BE_FRC_MEMC_MODE_T;

typedef struct
{
    	UINT32	u32Lowdelay;					// 0: NORMAL_DELAY, 1:LOW_DELAY
} LX_BE_FRC_LOW_DELAY_T;

typedef struct
{
    	UINT16 	u16DemoMode;     				// 0: DEMO_OFF, 1:DEMO_ON
    	UINT16	u16DemoSide;					// 0: DEMO_SIDE_LEFT, 1:DEMO_SIDE_RIGHT
} LX_BE_FRC_DEMO_MODE_T;

typedef struct
{
    	UINT8	u8BypassRegion0;				// Region 0 Bypass 0 : Off, 1 : On
    	UINT8	u8BypassRegion1;				// Region 1 Bypass 0 : Off, 1 : On
    	UINT8	u8BypassRegion2;				// Region 2 Bypass 0 : Off, 1 : On
    	UINT8	u8BypassRegion3;				// Region 3 Bypass 0 : Off, 1 : On
} LX_BE_FRC_MEMC_BYPASS_ENABLE_T;

typedef struct
{
   	UINT16	u16MemcBypassX; 				// Memc Bypass region Start X position
    	UINT16	u16MemcBypassY;				// Memc Bypass region Start Y position
} LX_BE_FRC_MEMC_BYPASS_XY_T;

typedef struct
{
   	UINT16	u16MemcBypassW; 				// Memc Bypass region Width
    	UINT16	u16MemcBypassH;				// Memc Bypass region Height
} LX_BE_FRC_MEMC_BYPASS_WH_T;

typedef struct
{
	LX_BE_FRC_MEMC_BYPASS_ENABLE_T		u32BypassEn;	// Memc Bypass On/Off
	LX_BE_FRC_MEMC_BYPASS_XY_T			u32BypassXY;	// Memc Bypass region Start position X,Y
	LX_BE_FRC_MEMC_BYPASS_WH_T		u32BypassWH;	// Memc Bypass region Start position W,H
} LX_BE_FRC_MEMC_BYPASS_REGION_T;

typedef struct
{
    	UINT32	u32FrameRate; 					// 0 : FREE_RUN, 1: RATE_120HZ, 2: RATE_100HZ, 3: RATE_60HZ, 4: RATE_50HZ
} LX_BE_FRC_FRAME_RATE_T;

typedef struct
{
    	UINT32	u32Freeze;	 					// 0 : FREEZE OFF, 1: FREEZE
} LX_BE_FRC_FREEZE_T;

typedef struct
{
	UINT8 	u8ColorYuv;						// 0: YUV422, 1: YUV444
	UINT8	u8ColorDepth; 					// 0: 10bit, 1:8bit
	UINT8	u8CompMode;					// 0: Compression mode Off, 1:Compression mode On
	UINT8	_rsvd;							//reservved
} LX_BE_FRC_COLOR_YUV444_T;

typedef struct
{
	UINT16	u16FRCDebugBoxOn; 			// 0 : Debug Box Off, 1 : Debug Box On
	UINT16	u16FRCDebugBoxMode;			// FRC Debug Box mode
} LX_BE_FRC_ENABLE_DEBUG_BOX_T;

typedef struct
{
	UINT8	u8ResolutionType;		/// 0 : FHD,		1 : SD,		2 : UD
	UINT8	u8SrControl;				///< cotrol 0 : OFF, 1 :low , 2: midium, 3:high
	UINT8	u8SrAGlobalScale;		///<PP_global scale
	UINT8	u8SrBGlobalScale;		///<PP_local scale
} LX_BE_SRE_CMN_T;

typedef struct
{
	UINT8	u8ResolutionType;		/// 0 : FHD,		1 : SD,		2 : UD
	UINT8	u8SrControl;				///< enable
	UINT8	u8DbGrade; 				///< DB_grade
	UINT8	u8ModeLocalScale;		///<Mode_ls
	UINT8	u8SrALocalScale; 		///<SR_a local scale
	UINT8	u8SrBLocalScale ;		///<SR_b local scale
	UINT8	u8PP_XFilter ;			///<PP_X filter coefficient
	UINT8	u8PP_YFilter ; 			///<PP_Y filter coefficient
	UINT8	u8PP_GlobalScale;		///<PP_global scale
	UINT8	u8PP_LocalScale;		///<PP_local scale
	UINT16	_rsvd;					// reserved
} LX_BE_SRE_PRAM_T;

typedef struct
{
	UINT16	u16Version;
	UINT16	u16SubVersion;
} LX_BE_SRE_VERSION_T;

typedef struct
{
	UINT32	u32PictureMode;			// 0: Cinema, 1:Standard, 2:Vivid, 3:Game, 4:Expert
} LX_BE_SRE_MODE_T;

typedef struct
{
	UINT16	u16InputResolution;		// 0 : FHD,		1 : SD,		2 : UD(detour)
	UINT16	u16Input3DMode;		// 0 : 2D,3D SG,	1 : 3D PG
	// 3D, HD, SD
} LX_BE_SRE_INPUT_MODE_T;

typedef struct
{
	UINT8	u8LEDLutVersion;
	UINT8	u8LEDSubVersion	;
	UINT8	u8PanelSize;
	UINT8	u8PanelType;
} LX_BE_LED_VERSION_T;

typedef struct
{
	UINT32	u32PictureMode;				// 0: Lowest, 1:Low, 2:Medium, 3:High
} LX_BE_LED_MODE_T;

typedef struct
{
	UINT16	u16DimminOn;				// 0 : OFF(smooth),	1 : ON(smooth), 2 : OFF(instant), 3 : ON(instant)
	UINT16	u16StoreMode;				// 0 : OFF,	1 : ON
} LX_BE_LED_CONTROL_T;

typedef struct
{
	UINT16	u16Min;
	UINT16	u16Max;
} LX_BE_LED_APL_T;

typedef struct
{
	UINT32 AD_MEM_ADDR_CLR;
	UINT32 AD_MEM_DL_SBI_HCOEFF;
	UINT32 AD_MEM_DL_SBI_VCOEFF;
	UINT32 AD_FC_TAB1;
	UINT32 AD_FC_TAB5;
	UINT32 AD_UPPER_DC_LUT;
	UINT32 AD_LOWER_DC_LUT;
	UINT32 AD_MEM_DL_PIXEL_COMP_A;
	UINT32 AD_MEM_DL_PIXEL_COMP_B;
	UINT32 AD_MEM_DL_PIXEL_COMP_C;
	UINT32 AD_MEM_DL_PIXEL_COMP_D;
	UINT32 AD_UMASK_WR_COEFF;
} LX_BE_LED_REG_OFFSET;

typedef struct
{
	UINT32	u32CpuBaseAddr;
	UINT32	u32McuBaseAddr;
	LX_BE_LED_REG_OFFSET regOffset;
} LX_BE_LED_REG_INFO_T;

typedef struct
{
	UINT32	u32HDRLutVersion;
} LX_BE_HDR_VERSION_T;

typedef struct
{
	UINT8	u8DECMode;
	UINT8	u8DCMode;
	UINT8	u8DCBypass;
	UINT8	u8DCsc;
	UINT8	u8OSD;
	UINT8	u8Store;
	UINT8	u8DRC_shp;
	UINT8	_rsvd;					//reservved
} LX_BE_HDR_MODE_T;

typedef struct
{
	UINT16	u16DECEnable;
	UINT16	u16DRCEnable;
} LX_BE_HDR_CONTROL_T;

typedef struct
{
	UINT32	u32WCGLutVersion;
} LX_BE_WCG_VERSION_T;

typedef struct
{
	UINT32	u32PictureMode;			// 0:BT709, 1:SMPTE, 2:EBU, 3:Color Extension, 4:BT2020
} LX_BE_WCG_MODE_T;

typedef struct
{
	UINT32	u32Enable;;
} LX_BE_WCG_CONTROL_T;

typedef struct
{
	UINT32 isFrcLowDelayMode;
} LX_BE_KDRV_INFO_T;


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
LX_BE_LOW_DELAY_T;



typedef enum
{
	BE_SKEEPER_DBG_CMD_INIT = 0,
	BE_SKEEPER_DBG_CMD_UNINIT,
	BE_SKEEPER_DBG_CMD_INIT_HISTORY,
	BE_SKEEPER_DBG_CMD_UNINIT_HISTORY,
	BE_SKEEPER_DBG_CMD_CHECK_STATUS,
	BE_SKEEPER_DBG_CMD_PRINT_MODULE_LIST,
	BE_SKEEPER_DBG_CMD_PRINT_STATUSINDEX,
	BE_SKEEPER_DBG_CMD_PRINTLOG,
	BE_SKEEPER_DBG_CMD_CHANGE_HISTORY_TIME_UNIT,
	BE_SKEEPER_DBG_CMD_CHANGE_HISTORY_LOG_STATUS,
	BE_SKEEPER_DBG_CMD_CHANGE_MODULE_STATUS,
	BE_SKEEPER_DBG_CMD_ENABLE_DBG_PRINT,
	BE_SKEEPER_DBG_CMD_MAXNUM
} BE_SKEEPER_DBG_CMD_TYPE_T;

typedef struct
{
	UINT32	cmd_type	:8,		// Cmd type
			module_type	:8,		// Module Type
			history_type :8,		// History Type
			etcData		:8;		// data field
} BE_SKEEPER_DBG_T;

typedef enum
{
	BE_SKEEPER_FRC_MEMC_MODE = 0,
	BE_SKEEPER_FRC_LOW_DELAY_MODE,
	BE_SKEEPER_FRC_RGB_YUV_MODE,
	BE_SKEEPER_FRC_FREEZE,
	BE_SKEEPER_H3D_3D_MODE,
	BE_SKEEPER_H3D_AUTO_CONVERGENCE,
	BE_SKEEPER_H3D_2D_TO_3D_DEPTH,
	BE_SKEEPER_H3D_3D_DEPTH,
	BE_SKEEPER_H3D_CONVERGENCE,
	BE_SKEEPER_H3D_LR_TOGGLE,
	BE_SKEEPER_STATUS_MAXNUM
} BE_SKEEPER_STATUS_TYPE_T;
/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _BE_KAPI_H_ */

/** @} */

