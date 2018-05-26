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
 * date       2014.06.02
 * note       Additional information.
 *
 * @addtogroup BE
 * @{
 */

#ifndef  _IPC_DEF_H15_H_
#define  _IPC_DEF_H15_H_

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define BE_IPC_FROM_CPU_H15A0_BASE
#define BE_IPC_FROM_MCU_H15A0_BASE

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
	BE_TOP_B0_INIT					= 0xA0000000,
	BE_TOP_B0_SET_3D_OUT_FORMAT,
	BE_TOP_B0_SET_BLOCK_BYPASS,
	BE_TOP_B0_SET_DEBUG_DISPLAY,
	BE_TOP_B0_GET_SYSTEM_INFO,
	BE_TOP_B0_TOGGLE_3D_LR,
	BE_TOP_B0_SET_3D_CONVERGENCE,
	BE_TOP_B0_SET_BLACK_BAR,
	BE_TOP_B0_SET_REAL_CINEMA,

	BE_H3D_B0_SET_MODE				= 0xA1000000,
	BE_H3D_B0_T3C_DEPTH_LEVEL,

	BE_LED_B0_LOAD_LUT				= 0xA3000000,
	BE_LED_B0_SET_MODE,
	BE_DRC_B0_LOAD_LUT,
	BE_DRC_B0_SET_MODE,
	BE_WCG_B0_LOAD_LUT,
	BE_WCG_B0_SET_MODE,
} BE_IPC_B0_DEF_T;

//   BE_IPC_TOP    ////////////////////////////////////////////////

typedef struct
{
	UINT32	bInit;      			// 0: HDP All Block Normal, 1: HDP All Block Init
	UINT32	rsvd[3];
} BE_IPC_B0_INIT_T;

typedef struct
{
	UINT32 	uTDPGMode;			// 0 : Normal PG, 1 : FPR
	UINT32	uTDSingleOn;		// 0 : Normal, 1 : Single
	UINT32	rsvd[2];
} BE_IPC_B0_3D_OUT_FORMAT_T;

typedef struct
{
	UINT32	uFRCBypass;
	UINT32	uH3DBypass;
	UINT32	uWCGBypass;
	UINT32	rsvd;
} BE_IPC_B0_BLOCK_BYPASS_T;

typedef struct
{
	UINT32  	uH3DDebugdisplay;
	UINT32  	uPEDebugDisplay;
	UINT32  	uLEDDebugDisplay;
} BE_IPC_B0_DEBUG_DISPLAY_T;

typedef struct
{

} BE_IPC_B0_SYSTEM_INFO_T;

typedef struct
{
	UINT32	bOutLR;				 // 0: Left/Top First, 1: Right/Bottom First
	UINT32	rsvd[3];
} BE_IPC_B0_3D_LR_T;

typedef struct
{
	UINT32	uTDConvVal;
	UINT32	bTDConvEnable;
	UINT32	rsvd[2];
} BE_IPC_B0_3D_CONVERGENCE_T;

typedef struct
{
	UINT32	uTDBlackBarEnable;
	UINT32	uTDBlackBarVal;
	UINT32	rsvd[2];
} BE_IPC_B0_BLACK_BAR_T;

typedef struct
{
	UINT32	uRealCinemaEnable;
	UINT32	rsvd[3];
} BE_IPC_B0_REAL_CINEMA_T;

//   BE_IPC_H3D    ////////////////////////////////////////////////

typedef struct
{
	UINT32  	uT3CMode;
	UINT32  	rsvd[3];
} BE_IPC_B0_H3D_MODE_T;

typedef struct
{
	UINT32  	uT3CDepth;
	UINT32  	rsvd[3];
} BE_IPC_B0_H3D_T3C_DEPTH_T;

////   BE_IPC_PE	///////////////////////////////////////////////////////////
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
	UINT32 	uLEDLutLoad;
	UINT32 	rsvd[3];
} BE_IPC_B0_LED_LOAD_LUT_T;

typedef struct
{
	UINT32	uLEDEn;
	UINT32 	uLEDMode;
	UINT32 	rsvd[2];
}  BE_IPC_B0_LED_MODE_T;

typedef struct
{
	UINT32 	bDRCLutLoad;
	UINT32 	rsvd[3];
} BE_IPC_B0_DRC_LOAD_LUT_T;

typedef struct
{
	UINT32	uDRCEn;
	UINT32 	uDRCMode;
	UINT32 	rsvd[2];
}  BE_IPC_B0_DRC_MODE_T;

typedef struct
{
	UINT32 	bWCGLutLoad;
	UINT32 	rsvd[3];
} BE_IPC_B0_WCG_LOAD_LUT_T;

typedef struct
{
	UINT32	uWCGEn;
	UINT32 	uWCGMode;	// 0:BT709, 1:SMPTE, 2:EBU, 3:Color Extension, 4:BT2020
	UINT32 	rsvd[2];
}  BE_IPC_B0_WCG_MODE_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef _IPC_DEF_H15_H_  ----- */
/**  @} */

