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
 * chip specific definition for BE device.
 *
 * author     dj911.kim
 * version    1.0
 * date       2013.04.03
 * note       Additional information.
 *
 * @addtogroup BE
 * @{
 */

#ifndef  _IPC_DEF_H14_H_
#define  _IPC_DEF_H14_H_

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define BE_IPC_FROM_CPU_H14A0_BASE       (DPPB_TOP_REG_H14_BASE + DPPB_TOP_IPC_H14A0_OFFSET)
#define BE_IPC_FROM_MCU_H14A0_BASE       (BE_IPC_FROM_CPU_H14A0_BASE + IPC_FROM_CPU_SIZE)

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "base_types.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define IPC_SET_DEFAULT_PARAM(structure, name)  \
	structure name;\
	do{\
		memset(&(name), 0xFF, sizeof(structure));\
	}while(0)

#define IPC_SET_GET_PARAM(structure, name)	IPC_SET_DEFAULT_PARAM(structure, name)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef enum {
	BE_TOP_INIT					= 0x00000000,
	BE_TOP_STATUS,
	BE_TOP_TOGGLE_3D_LR,
	BE_TOP_SET_3D_IN_FORMAT,
	BE_TOP_SET_3D_OUT_FORMAT,	
	BE_TOP_SET_3D_SYNC_DELAY,		
	BE_TOP_SET_3D_CONVERGENCE,		
	BE_TOP_SET_BLACK_BAR,	
	BE_TOP_SET_VIDEO_MUTE,			
	BE_TOP_SET_MUTE_COLOR,			
	BE_TOP_SET_BLOCK_BYPASS,		
	BE_TOP_SET_DISPLAY_TYPE,		
	BE_TOP_SET_DEBUG_DISPLAY,		
	BE_TOP_SET_VID_OPERATION,
	BE_TOP_SET_SRC_TYPE,
	BE_TOP_SET_ACTIVE_WINDOW_START,
	BE_TOP_SET_ACTIVE_WINDOW_SIZE,

	BE_H3D_GET_VERSION		= 0x10000000,
	BE_H3D_GET_STATUS,
	BE_H3D_INIT_BLOCK,
	BE_H3D_RESET_BLOCK,
	BE_H3D_BYPASS_ALL,
	BE_H3D_SET_MODE,
	BE_H3D_IN_FORMAT,
	BE_H3D_T3C_DEPTH_LEVEL,
	BE_H3D_A3C_DEPTH_LEVEL,	
	BE_H3D_A3C_AUTOCONVERGENCE,
	BE_H3D_GET_A3C_DMINMAX,
	
	BE_FRC_GET_FIRMWARE_VERSION	= 0x20000000,
	BE_FRC_GET_STATUS,
	BE_FRC_INIT_BLOCK,
	BE_FRC_SET_MEMC_MODE,
	BE_FRC_SET_LOW_DELAY,
	BE_FRC_SET_DEMO_MODE,
	BE_FRC_BYPASS_REGION,
	BE_FRC_BYPASS_REGION0_XY,
	BE_FRC_BYPASS_REGION0_WH,
	BE_FRC_BYPASS_REGION1_XY,
	BE_FRC_BYPASS_REGION1_WH,
	BE_FRC_BYPASS_REGION2_XY,
	BE_FRC_BYPASS_REGION2_WH,
	BE_FRC_BYPASS_REGION3_XY,
	BE_FRC_BYPASS_REGION3_WH,	
	BE_FRC_FORCE_FRAME_RATE,
	BE_FRC_SET_YUV444,
	BE_FRC_SET_FAST_MV_TH,
	BE_FRC_SET_STILL_DET_TH,
	BE_FRC_SET_SAME_DET_TH,
	BE_FRC_SET_DIFF_DET_TH,
	BE_FRC_SET_VMODE_ERR_CNT,
	BE_FRC_SET_JUDDER_SLOPE,
	BE_FRC_SET_MIN_ERR_TH,
	BE_FRC_SET_ABNORMAL_SYNC_CNT,
	BE_FRC_INIT_DB_ADDR,
	BE_FRC_ENABLE_DEBUG_BOX,

	BE_SRE_GET_VERSION		= 0x30000000, // SRE
	BE_SRE_GET_STATUS,
	BE_SRE_INIT_BLOCK,
	BE_SRE_BYPASS_ALL,
	BE_SRE_SET_LUT_ADDR,
	BE_SRE_LOAD_LUT,
	BE_SRE_SET_MODE,
	BE_SRE_CONTROL,
	BE_SRE_SET_INPUT_MODE,
	BE_SRE_SET_DB_GRADE,
	BE_SRE_SR_STRENGTH_LEVEL,
	BE_SRE_UPDATE_ALL,
	BE_SRE_WRITE_ADDRESS,
	BE_SRE_WRITE_DATA,
	BE_SRE_READ_ADDRESS,
	BE_SRE_GET_DATA,

	//SRE IPC Command for H13B0
	BE_SRE_GLOBAL_SCALE	= 0x30000016,
	BE_SRE_LOCAL_SCALE,
	BE_SRE_PREPROCESSING,
	BE_SRE_NOISE_CONTROL,
	BE_SRE_LOCAL_BYPASS,
	BE_SRE_CLIP_CONTROL,
	BE_SRE_WEIGHT_CONTROL,
	BE_SRE_ETC_CONTROL,
	
	BE_PE_GET_STATUS		= 0x40000000,	// DPPA
	BE_PE_INIT_BLOCK,
	BE_PE_SET_CEN_GAIN0,
	BE_PE_SET_CEN_GAIN1,
	BE_PE_GET_HISTO,
	
	BE_LED_GET_VERSION		= 0x50000000,	// DPPB
	BE_LED_GET_STATUS,
	BE_LED_INIT_BLOCK,
	BE_LED_SET_LUT_ADDR,
	BE_LED_LOAD_LUT,
	BE_LED_SET_MODE,
	BE_LED_CONTROL,		// store, dimming, pix_comp
	BE_HDR_GET_VERSION,
	BE_HDR_GET_STATUS,
	BE_HDR_INIT_BLOCK,
	BE_HDR_SET_LUT_ADDR,
	BE_HDR_LOAD_LUT,
	BE_HDR_SET_MODE,
	BE_HDR_CONTROL,
	BE_WCG_GET_VERSION,
	BE_WCG_GET_STATUS,
	BE_WCG_INIT_BLOCK,
	BE_WCG_SET_LUT_ADDR,
	BE_WCG_LOAD_LUT,
	BE_WCG_SET_MODE,
	BE_WCG_CONTROL,

	BE_PWM_SET_TX_CH0		= 0x60000000,	// control, duty, freq
	BE_PWM_SET_TX_CH1,
	BE_PWM_SET_TX_CH2,
	BE_PWM_GET_RX_CH0,
	BE_PWM_GET_RX_CH1
} BE_IPC_DEF_T;

// prefix definition
// 4bit  : boolean			= b		-> boolean 용도지만 ignore value를 위해 4bit 할당, ignore value : 0xF
// 4bit  : nibble			= u4 	-> ignore value : 0xF
// 8bit  : byte    			= u8 	-> ignore value : 0xFF
// 16bit : unsigned int 	= u16		-> ignore value : 0xFFFF
// 32bit : unsigned int 	= u32		-> ignore value : 0xFFFFFFFF

//   BE_IPC_TOP    ////////////////////////////////////////////////

typedef struct
{
	UINT32	bInit      				:	4,	// 0: BE All Block Normal, 1: BE All Block Init
			reserved				:	28;
} BE_IPC_INIT_T;

typedef struct{
	UINT32	bOutLR					:	4,	// 0: Left/Top First, 1: Right/Bottom First
			reserved				:	28;
}BE_IPC_3D_LR_T;

typedef struct{
	UINT32 	u4BE3DInFormat			:	4,	// 0 : 2D, 1 : S/S, 2 : T/B, 3: Dual
			u4BE3DInType 			:	4,	// 0 : 3D Normal, 0 : Dual Stream
			u4BE3DInSize 			:	4,	// 0 : 1920x1080, 1 : 960x1080, 2 : 1920x540, 3 : UD
			reserved				:	20;
}BE_IPC_TOP_3D_IN_FORMAT_T;

typedef struct{
	UINT32 	u4OutFormat 			:	4,	//	0 : PG,	1 : SG,	2 : FS,	
			u4PGMode 				:	4,	//	0 : Normal PG, 1 : FPR 
			u4GSyncEnable			:	4,	// 	0 : Glass Sync disable, 1 : Glass Sync Enable for SG
			u4Single				:	4,	// 	0 : Dual View, 1 : Single View
			reserved				:	16;
}BE_IPC_3D_OUT_FORMAT_T;

typedef struct{
	UINT32  	u16ConvergenceVal 		:	16,
			u16ConvergenceEnable	:	16;	// 0 : Convergence auto disable, 
											// 1 : Convergence auto enable with 3D depth info
}BE_IPC_3D_CONVERGENCE_T;

typedef struct{
	UINT32 	u8BarOn					:	8,
			u8BarVal				:	8,
			u8BarStep				:	8,
			reserved				:	8;
}BE_IPC_BE_BLACK_BAR_T;

typedef struct
{
	UINT32	bVideoMuteOn			:	4,	// 0 : Video Mute off,	1 : Video Mute On	
			u4DEInRate				:	4,	// DE Video input frame rate (0:15, 1:20, 2:24, 3:25, 4:30, 5:48, 6:50, 7:60, 0xF:N/A)
			u4DEOutRate			:	4,	// DE Video output frame rate (0:15, 1:20, 2:24, 3:25, 4:30, 5:48, 6:50, 7:60, 0xF:N/A)
			reserved				:	20;
} BE_IPC_VIDEO_MUTE_T;

typedef struct
{
	UINT32	bH3DBypass				:	4,	//	0: H3D Normal, 1: H3D(2D-3D,Depth Block) Bypass
			bFRCBypass				:	4,	//	0: FRC Normal, 1: FRC Bypass
			bSREBypass				:	4,	//	0: SRE Normal, 1: SRE Bypass
			bPEBypass				:	4,	//	0: PE Normal, 1: PE Bypass
			bLEDBypass				:	4,	//	0: LED Normal, 1: LED Bypass
			bHDRBypass				:	4,	//	0: HDR Normal, 1: HDR Bypass
			bWCGBypass				:	4,	//	0: WCG Normal, 1: WCG Bypass
			reserved				:	4;	
} BE_IPC_BLOCK_BYPASS_T;

typedef struct
{
	UINT32	u4BEPanelType			:	4,	// 0 : FHD,	 1 : UD,
			u4BEPanelRate			:	4,	// 0 : 60Hz, 1 : 120Hz
			u4BEPanelReverse		:	4,	// 0 : Normal,	1 : Reverse
			reserved				:	20;	
} BE_IPC_DISPLAY_TYPE_T;

typedef struct
{
	UINT32	u8H3DDebugDisplay		:	8,	
			u8FRCDebugDisplay		:	8,
			u8PEDebugDisplay		:	8,
			u8LEDDebugDisplay		:	8;
} BE_IPC_DEBUG_DISPLAY_T;

typedef struct
{
	UINT32	u4Mode					:	4,	// 0 : 2D,	1 : PIP, 2 : V_ENC On
			reserved				:	28;
} BE_IPC_VID_OPERATION_T;

typedef struct
{
	UINT32	u16SrcInputType		:	16,	// 0 : HDMI,	1 : Component, 2 : Composite, 3 : ETC
			u16SrcInputSize		:	16;	// 1080, 720, 480
} BE_IPC_SRC_TYPE_T;

typedef struct 
{ 
	UINT32  u16ActiveWindowStartX   :          16,
			u16ActiveWindowStartY   :          16;
} BE_IPC_ACTIVE_WINDOW_START_T;
 
typedef struct 
{ 
	UINT32  u16ActiveWindowWidth   :          16,
			u16ActiveWindowHeight  :          16;
} BE_IPC_ACTIVE_WINDOW_SIZE_T;

//   BE_IPC_H3D    ////////////////////////////////////////////////

typedef struct{
	UINT32	u8A3CVersion		: 	8,
			u8T3CVersion			:	8,
			reserved			:	16;
}BE_IPC_H3D_VERSION_T;

typedef struct{
	UINT32	bReset					:	4,
			reserved				:	28;
}BE_IPC_H3D_RESET_BLOCK_T;

typedef struct{
	UINT32	bBypass					:	 4,
			reserved				:	28;
}BE_IPC_H3D_BYPASS_ALL_T;

typedef struct{
	UINT32	bA3CMode				:	4, // 0 : off, 1: on
			bT3CMode				:	4, // 0 : off, 1: on
			reserved  				: 	24;
}BE_IPC_H3D_MODE_T;

typedef struct{
	UINT32	u4H3DInFormat			:	4,	// 0: 1920x1080(FHD), 1:960x1080(SS), 2:1920x540(TB)
			reserved			:	28;
}BE_IPC_H3D_IN_FORMAT_T;

typedef struct{
	UINT32	u8T3CDepthLevel		:	8,	// Depth level : 0 ~ 20
			reserved		:	24;
}BE_IPC_H3D_T3C_DEPTH_LEVEL_T;

typedef struct{
	UINT32	u8A3CDepthLevel		:	8,	// Depth level : 0 ~ 20
			reserved		:	24;
}BE_IPC_H3D_A3C_DEPTH_LEVEL_T;

typedef struct{
	UINT32	bA3CAutoConvergence	:	4,	// 0 : Auto Convergence off, 1 : Auto Convergence on
			reserved		:	28;
}BE_IPC_H3D_A3C_AUTOCONVERGENCE_T;

//   BE_IPC_FRC    ////////////////////////////////////////////////

typedef struct
{
    UINT32	u16Version      		:	16,
            		u16Subversion   		:	16;
} BE_IPC_FRC_VERSION_T;

typedef struct
{
	UINT32	bInit          			:	4,		// 0: NOT_READY, 1:INIT, 2:NORMAL
            		reserved      			:	28;
} BE_IPC_FRC_INIT_T;

typedef struct
{
    UINT32	u8Blur       				:	8,		// 0~9 : Blur level
            		u8Judder      			:	8,		// 0~9 : Judder level
			u8MemcType     			:	8,		// 0 : OFF, 1: LOW, 2:MID, 3:HIGH, 4:USER, 5: PULLDOWN55 : @ref FRC_MC_TYPE_T
			reserved				:	8;
} BE_IPC_FRC_MEMC_MODE_T;

typedef struct
{
    UINT32	bLowdelay      			:	4,		// 0: NORMAL_DELAY, 1:LOW_DELAY
            		reserved      				:	28;
} BE_IPC_FRC_LOW_DELAY_T;

typedef struct
{
    UINT32 	bDemoMode     			:	4,		// 0: DEMO_OFF, 1:DEMO_ON
			bDemoSide				:	4,		// 0: DEMO_SIDE_LEFT, 1:DEMO_SIDE_RIGHT
            		reserved   				:	24;
} BE_IPC_FRC_DEMO_MODE_T;

typedef struct
{
    UINT32	bBypassRegion0			:	4,		// Region 0 Bypass 0 : Off, 1 : On
			bBypassRegion1			:	4,		// Region 1 Bypass 0 : Off, 1 : On
			bBypassRegion2			:	4,		// Region 2 Bypass 0 : Off, 1 : On
			bBypassRegion3			:	4,		// Region 3 Bypass 0 : Off, 1 : On
			reserved   				:	16;
} BE_IPC_FRC_MEMC_BYPASS_ENABLE_T;

typedef struct
{
    UINT32	u16MemcBypassX 			:	16,	// Memc Bypass region Start X position
			u16MemcBypassY			:	16;	// Memc Bypass region Start Y position
} BE_IPC_FRC_MEMC_BYPASS_XY_T;

typedef struct
{
    UINT32	u16MemcBypassW 			:	16,	// Memc Bypass region Width
			u16MemcBypassH			:	16;	// Memc Bypass region Height
} BE_IPC_FRC_MEMC_BYPASS_WH_T;

typedef struct
{
	BE_IPC_FRC_MEMC_BYPASS_ENABLE_T	u32BypassEn;	// Memc Bypass On/Off
	BE_IPC_FRC_MEMC_BYPASS_XY_T	u32BypassXY;		// Memc Bypass region Start position X,Y
	BE_IPC_FRC_MEMC_BYPASS_WH_T	u32BypassWH;		// Memc Bypass region Start position W,H
} BE_IPC_FRC_MEMC_BYPASS_REGION_T;

typedef struct
{
    UINT32	u8FrameRate 			:	8,		// 0 : FREE_RUN, 1: RATE_120HZ, 2: RATE_100HZ, 3: RATE_60HZ, 4: RATE_50HZ
			reserved				:	24;
} BE_IPC_FRC_FRAME_RATE_T;

typedef struct
{
	UINT32 	u4ColorYuv 				: 	4,		// 0: YUV422, 1: YUV444
			u4ColorDepth 			:	4,		// 0: 10bit, 1:8bit
			u4CompMode 			:	4,		// 0: Compression mode Off, 1:Compression mode On
			reserved 				:	20;
} BE_IPC_FRC_COLOR_YUV444_T;

typedef struct
{
	UINT32	u32FrcInitDDRAddr;				// FRC Initialization DB DDR Address
} BE_IPC_FRC_INIT_DB_ADDR_T;

typedef struct
{
	UINT32	u4FRCDebugBoxOn 		: 	4,		// 0 : Debug Box Off, 1 : Debug Box On
			u4FRCDebugBoxMode		: 	4,		// FRC Debug Box mode 
			reserved				:	24;
} BE_IPC_FRC_ENABLE_DEBUG_BOX_T;

////   BE_IPC_SRE    ////////////////////////////////////////////////

typedef struct
{
	UINT32	u16Version				:	16,
			u16SubVersion			:	16;
} BE_IPC_SRE_VERSION_T;

typedef struct
{
	UINT32	bInit					:	4,
			reserved				:	28;
} BE_IPC_SRE_INIT_BLOCK_T;

typedef struct
{
	UINT32	u32SreLutDDRAddr;
} BE_IPC_SRE_LUT_ADDR_T;


typedef struct
{
	UINT32	bLoad					:	4,
			reserved				:	28;
} BE_IPC_SRE_LOAD_LUT_T;

typedef struct
{
	UINT32	u4PictureMode			:	4,		// 0: Cinema, 1:Standard, 2:Vivid, 3:Game, 4:Expert
			reserved				:	28;
} BE_IPC_SRE_MODE_T;

typedef struct
{
	UINT32	u4Control				:	4,		//0:SRE bypass, 1:SRE LOW, 2:SRE MED, 3:SRE HIGH
			reserved				:	28;
} BE_IPC_SRE_CONTROL_T;

typedef struct
{
	UINT32	u4InputResolution		:	4,		// 0 : FHD,		1 : SD,		2 : UD(detour)
			u4Input3DMode			:	4,		// 0 : 2D,3D SG,	1 : 3D PG
			reserved				:	24;
	// 3D, HD, SD
} BE_IPC_SRE_INPUT_MODE_T;

typedef struct
{
	UINT32	u8DBGrade				:	8,
			reserved				:	24;
} BE_IPC_SRE_DB_GRADE_T;

typedef struct
{
	UINT32	bUpdate					:	4,
			reserved				:	28;
} BE_IPC_SRE_UPDATE_ALL_T;

typedef struct
{
	UINT32	u8SRAGlobalScale		:	8,
			u8SRBGlobalScale		:	8,
			reserved				:	16;
} BE_IPC_SRE_GLOBAL_SCALE_T;

typedef struct
{
	UINT32	u4ModeLocalScale		:	4,		// 0:standard deviation 1:gradient 2:std+grd
			u4SRALocalScale			:	4,
			u4SRBLocalScale			:	4,
			reserved				:	20;
} BE_IPC_SRE_LOCAL_SCALE_T;

typedef struct
{
	UINT32	u8XFilter				:	8,		// 0~15 : Horizontal PP filter idx
			u8YFilter				:	8,		// 0~15 : Vertical PP filter idx
			u8GlobalScale			:	8,		// 0~127 : Global scale for Pre-processing
			u8LocalScale				:	8;		// 0~15 : Local scale idx for Pre-processing
} BE_IPC_SRE_PREPROCESSING_T;

typedef struct
{
	UINT32	bInit					:	4,
			reserved				:	28;
} BE_IPC_PE_INIT_BLOCK_T;

typedef struct
{
	UINT32	u8regnNum				:	8,	//	region number 0~15
			u8deltHue				:	8,	//	delta hue, -127 ~ 127
			u8deltSat				:	8,	//	delta saturation, -127 ~ 127
			u8deltVal				:	8;	//	delta value, -127 ~ 127
} BE_IPC_PE_CEN_CTRL0_T;

typedef struct
{
	UINT32	u8deltGrn				:	8,	//	delta green, -127 ~ 127
			u8deltBlu				:	8,	//	delta blue, -127 ~ 127
			u8deltRed				:	8,	//	delta red, -127 ~ 127
			u8gainMst				:	8;	//	master gain, 0 ~ 255
} BE_IPC_PE_CEN_CTRL1_T;

////   BE_IPC_LED HDR WCG    ////////////////////////////////////////////////

typedef struct
{
	UINT32	u8LEDLutVersion			:	8,
			u8LEDSubVersion			:	8,
			u4PanelSize				:	8,
			u4PanelType				:	8;
}  BE_IPC_LED_VERSION_T;

typedef struct
{
	UINT32	bInit					:	4,
			reserved				:	28;
} BE_IPC_LED_INIT_BLOCK_T;

typedef struct
{
	UINT32	u32LedLutDDRAddr;
} BE_IPC_LED_LUT_ADDR_T;

typedef struct
{
	UINT32	bLoad					:	4,
			reserved				:	28;
} BE_IPC_LED_LOAD_LUT_T;

typedef struct
{
	UINT32	u4PictureMode			:	4,		// 0: Lowest, 1:Low, 2:Medium, 3:High
			reserved				:	28;
}  BE_IPC_LED_MODE_T;

typedef struct
{
	UINT32	u4DimminOn				:	4,		// 0 : OFF(smooth),	1 : ON(smooth), 2 : OFF(instant), 3 : ON(instant)
			bStoreMode				:	4,		// 0 : OFF,	1 : ON
			reserved				:	24;
}  BE_IPC_LED_CONTROL_T;

typedef struct
{
	UINT32	u32HDRLutVersion;
}  BE_IPC_HDR_VERSION_T;

typedef struct
{
	UINT32	bInit					:	4,
			reserved				:	28;
} BE_IPC_HDR_INIT_BLOCK_T;

typedef struct
{
	UINT32	u32HdrLutDDRAddr;
} BE_IPC_HDR_LUT_ADDR_T;

typedef struct
{
	UINT32	bLoad					:	4,
			reserved				:	28;	
} BE_IPC_HDR_LOAD_LUT_T;

typedef struct
{
	UINT32	u4DECMode			:	4,
			u4DCMode			:	4,
			bDCBypass			:	4,
			bDCsc				:	4,
			bOSD				:	4,
			bStore				: 	4,
			bDRC_shp			:	4,
			reserved			:	4;
}  BE_IPC_HDR_MODE_T;

typedef struct
{
	UINT32	bDECEnable			:	4,
			bDRCEnable			:	4,
			reserved			:	24;
}  BE_IPC_HDR_CONTROL_T;

typedef struct
{
	UINT32	u32WCGLutVersion;
}  BE_IPC_WCG_VERSION_T;

typedef struct
{
	UINT32	bInit				:	4,
			reserved			:	28;
} BE_IPC_WCG_INIT_BLOCK_T;

typedef struct
{
	UINT32	u32WcgLutDDRAddr;
	
} BE_IPC_WCG_LUT_ADDR_T;

typedef struct
{
	UINT32	bLoad				:	4,
			reserved			:	28;
} BE_IPC_WCG_LOAD_LUT_T;

typedef struct
{
	UINT32	u4PictureMode		:	4,		// 0:BT709, 1:SMPTE, 2:EBU, 3:Color Extension
			reserved			:	28;
}  BE_IPC_WCG_MODE_T;

typedef struct
{
	UINT32	bEnable				:	4,
			reserved			:	28;
}  BE_IPC_WCG_CONTROL_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef _IPC_DEF_H14_H_  ----- */
/**  @} */

