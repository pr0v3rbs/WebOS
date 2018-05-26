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
 *	GFX hw control
 *
 *  author      sinchul.jung (sinchul.jung@lge.com)
 *  version     1.0
 *  date
 *  note        Additional information.
 *
 *  @addtogroup lg1150_gfx
 *  @{
 */

#ifndef	_GFX_HW_H_
#define	_GFX_HW_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
//#define __ARM__

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
#endif

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	GFX_REG_MEMCPY(dst,src,cnt)								\
{																\
	register int i;												\
	volatile UINT32* dst_ptr = (volatile UINT32*)dst;			\
			 UINT32* src_ptr = (UINT32*)src;					\
	for ( i=cnt; i>0; i-- )	 *dst_ptr++ = *src_ptr++;			\
}

/*----------------------------------------------------------------------------------------
    Type Definitions (enum)
----------------------------------------------------------------------------------------*/
typedef enum
{
    GFX_INT_BATCH = 0,
    GFX_INT_SINGLECMD,
    GFX_INT_SPECIFIEDCMD,
}
GFX_INT_MODE_T;

typedef enum
{
	eWriteOnly = 0,
	eReadOnePort,
	eReadTwoPorts,
	eReadThreePorts,
	eReadTwoPortsYC,
	eReadThreePortsYC
}
GFX_ENUM_INPUT_T;

typedef enum
{
	eNONE = 0,
	ePort0,
	ePort1,
	ePort2
}
GFX_ENUM_PORT_T;

typedef enum
{
	eINDEX_0	= 0,
	eINDEX_1 = 0x1,
	eINDEX_2 = 0x2,
	eINDEX_4 = 0x3,
	eINDEX_8 = 0x4,
	eALPHA_8 = 0x5,
	eYCbCr444 = 0x6,
	eCb8Cr8422 = 0x7,
	eCb8Cr8420 = 0x8,
	eYCbCr655 = 0x9,
	eAYCbCr2644 = 0xa,
	eAYCbCr4633 = 0xb,
	eAYCbCr6433 = 0xc,
	eCbCr420 = 0xd,
	eAYCbCr8888 = 0xe,
	eY0Cb0Y1Cr422 = 0xf,
	eRGB565 = 0x19,
	eARGB1555 = 0x1a,
	eARGB4444 = 0x1b,
	eARGB6343 = 0x1c,
	eCbCr422 = 0x1d,
	eARGB8888 = 0x1e,
}
GFX_ENUM_PIXEL_FORMAT_T;

/*	raxis.lim (2010/05/31)	-- bug fix
 *	-- 	swap endian value
 *		according to GFX manual, little endian is 0 and big endian is 1
 *		( system default is little endian )
 */
typedef enum
{
	eLITTLE = 0,
	eBIG 	= 1,
}
GFX_ENUM_ENDIAN_T;

typedef enum
{
	ePORT0	= 0,
	ePORT1,
	ePORT2,
	eCONSTANT,
	eBLEND,
	eRASTER,
	eYC_INTERLEAVING
}
GFX_ENUM_OUT_T;

typedef enum
{
	eCOMP_LINEAR = 0,
	eCOMP_MIN,
	eCOMP_MAX
}
GFX_ENUM_BLEND_COMP_T;

typedef enum
{
	eZERO	= 0,
	eSRC_AND_DST,
	eSRC_AND_NOT_DST,
	eSRC,
	eNOT_SRC_AND_DST,
	eDST,
	eSRC_XOR_DST,
	eSRC_OR_DST,
	eNOT_SRC_AND_NOT_DST,
	eNOT_SRC_XOR_DST,
	eNOT_DST,
	eSRC_OR_NOT_DST,
	eNOT_SRC,
	eNOT_SRC_OR_DST,
	eNOT_SRC_OR_NOT_DST,
	eONE
}
GFX_ENUM_RASTER_T;

typedef enum
{
	eREPEAT =0,
	eTWO_TAP_AVG,
	eOTHERS
}
GFX_ENUM_CH_FILTER_MODE_T;

typedef enum
{
	eYC_TO_RGB0 =0,
	eYC_TO_RGB1,
	eRGB_TO_YC0,
	eRGB_TO_YC1,
}
GFX_ENUM_CSC_COEF_SEL_T;

/*----------------------------------------------------------------------------------------
    Type Definitions (struct)
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT8 	bStatus;
	UINT8 	bFull;
	UINT8 	bBatchStatus;
	UINT8	bCmdQStatus;

	UINT16 	usLine;
	UINT16 	usRemainSpace;
	UINT16 	usRemainParam;

	UINT32	status[3];
}
GFX_CMD_QUEUE_CTRL_T;

typedef struct
{
	GFX_ENUM_INPUT_T sPort;	// pixel format
	
	UINT8 bBurstModeEnable;		// burst mode enable
	UINT8 bUpdateCLUT;
	UINT8 bChromaFilterEnable;
	GFX_ENUM_CH_FILTER_MODE_T sFmode;
	
	UINT16 usCoef[4];
/*
	UINT8 bRun;
	UINT8 bStart;
	UINT8 bPause;
	UINT8 bHalt;
*/	
}
GFX_CMD_OP_T;

typedef struct
{
	GFX_ENUM_PORT_T 		sPort;
	
	UINT32 					ulAddr;			// base address
	UINT16 					usStride;		// stride
	GFX_ENUM_ENDIAN_T 		sEndian;		// endian
	GFX_ENUM_PIXEL_FORMAT_T sFmt;			// pixel format
	UINT32 					uiGlobalAlpha;
	
	UINT32 					ulDataCLUT;
	UINT16 					usAddrCLUT;
#if 1
	UINT16 					bRW_CLUT:1,
							bAuto_Inc_CLUT:1,
	
							bColorKeyEnable:1,
							bBitMaskEnable:1,
							bCOCEnable:1,
							bCSCEnable:1,
							bColorKeyMode:1,
							xxx:9;
#else
	UINT8 					bRW_CLUT;
	UINT8 					bAuto_Inc_CLUT;
	
	UINT8 					bColorKeyEnable;
	UINT8 					bBitMaskEnable;
	UINT8 					bCOCEnable;
	UINT8					bCSCEnable;
	UINT8 					bColorKeyMode;
#endif
	
	GFX_ENUM_CSC_COEF_SEL_T sCSCSel;
	
	UINT32 					ulKeyLow;
	UINT32 					ulKeyHigh;
	UINT32 					ulReplaceColor;
	UINT32 					ulBitMask;
	UINT32 					ulCOCCtrl;
} 
GFX_PORT_CONFIGURE_T;

typedef struct
{
	GFX_ENUM_OUT_T 			sOut;
	GFX_ENUM_BLEND_COMP_T 	sAlphaBlend;
	GFX_ENUM_BLEND_COMP_T 	sRBlend;
	GFX_ENUM_BLEND_COMP_T 	sGBlend;
	GFX_ENUM_BLEND_COMP_T 	sBBlend;
	
	UINT16 					usAlpha_m0;
	UINT16 					usC_m0;
	
#if 1
	UINT32 					bDIVEnable:1,
							bXOR0Enable:1,
							bXOR1Enable:1,
							bPMA0Enable:1,
							bPMA1Enable:1,
							xxx:27;
#else
	UINT8 					bDIVEnable;
	UINT8 					bXOR0Enable;
	UINT8 					bXOR1Enable;
	UINT8 					bPMA0Enable;
	UINT8 					bPMA1Enable;
#endif
	
	UINT16 					usA0_Alpha;
	UINT16 					usB0_Alpha;
	UINT16 					usA1_R;
	UINT16 					usB1_R;
	UINT16 					usA2_G;
	UINT16 					usB2_G;
	UINT16 					usA3_B;
	UINT16 					usB3_B;
	
	UINT32 					ulBlendConstant;
	GFX_ENUM_RASTER_T 		sRaster;
}
GFX_BLEND_CONFIGURE_T;

typedef struct
{
	UINT32 ulAddr;		// out base address
	UINT16 usStride;	// out stride. bytes per line
	
	GFX_ENUM_ENDIAN_T sEndian;	// endian
	GFX_ENUM_PIXEL_FORMAT_T sFmt;	// pixel format	
	
	UINT16 usVSize;
	UINT16 usHSize;
	
	UINT32 ulGAlpha;
	
	UINT8 bCSCEnable;
	UINT8 bCOCEnable;
	
	UINT32 ulCOCCtrl;
	
	UINT32 uiCSCCoef[8];
}
GFX_OUT_CONFIGURE_T;

#if 0
typedef struct
{
	UINT32      phase_offset;
	UINT32      boundary_mode;
	UINT32      sampling_mode;
	UINT32      numerator;
	UINT32      denomidator;
	UINT32      filter_bank;
}
GFX_SCALER_FILTER_DATA_T;
#endif

typedef struct
{
#define GFX_SCALER_CMD_BYPASS			0		/** bypass scaler block */
#define	GFX_SCALER_CMD_SOFT_RESET		1		/** reset scaler block */
#define	GFX_SCALER_CMD_START			2		/** start scaler block */
	UINT8			cmd;

#define	GFX_SCALER_FILTER_BILINEAR		0		/** bilinear filter  (GFX default filter) */
#define	GFX_SCALER_FILTER_CUSTOM		1		/** polyphase filter (custom filter) */
#define GFX_SCALER_FILTER_MAX			2
	UINT8			filter;

	LX_DIMENSION_T	in_dim;					/** scaler input dimension */
	LX_DIMENSION_T	out_dim;				/** scaler output dimension */
}
GFX_SCALER_CONFIGURE_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
void		GFX_initialize(void);

int			GFX_InitHW(void);
int			GFX_ShutdownHW(void);

int			GFX_RunFlushCommand(void);
int			GFX_RunStartCommand(void);
int			GFX_PauseCommand(void);
int			GFX_ResumeCommand(void);
int			GFX_StopCommand(void);
int			GFX_SwResetCommand(void);
void		GFX_GetComQueueStatus(GFX_CMD_QUEUE_CTRL_T *queue);
void		GFX_GetOperationStatus(GFX_CMD_OP_T *operate);
void		GFX_SetOperationStatus(GFX_CMD_OP_T *operate);
void		GFX_GetInputConfigure(int iPort, GFX_PORT_CONFIGURE_T *port);
void		GFX_SetInputConfigure( GFX_PORT_CONFIGURE_T *port);
void		GFX_SetInputBasicConfigure( GFX_PORT_CONFIGURE_T *port);
void		GFX_GetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend);
void		GFX_SetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend);
void		GFX_SetBlendingOut(GFX_ENUM_OUT_T type);
void		GFX_GetOutputConfigure(GFX_OUT_CONFIGURE_T *port);
void		GFX_SetOutputConfigure(GFX_OUT_CONFIGURE_T *port);
void		GFX_SetOutputBasicConfigure(GFX_OUT_CONFIGURE_T *port);
void		GFX_SetScalerConfigure(GFX_SCALER_CONFIGURE_T* scaler);
int			GFX_SetCLUT(int port , int size , UINT32 *data);
void		GFX_SetInterruptMode(UINT32);
void		GFX_SetAckMode(UINT32 *);
void		GFX_GetAckMode(UINT32 *);
void		GFX_Stop(void);

void		GFX_SetCommandDelayReg(UINT16 delay);
UINT16		GFX_GetCommandDelayReg(void);

void		GFX_SetRegister(UINT32 addr, UINT32 val);
UINT32		GFX_GetRegister(UINT32 addr);
void		GFX_DumpRegister(void);

BOOLEAN		GFX_IsGfxIdle(void);

int			GFX_RunSuspend(void);
int			GFX_RunResume(void);

BOOLEAN		GFX_ClkCheck			(void);
int			GFX_RunClkOnCommand		(void);
int			GFX_RunClkOffCommand	(void);


/*
#define MALI_DEBUG_PRINT(nr, X )  do { if ( nr<=MALI_DEBUG_LEVEL ) { MALI_PRINTF X ; MALI_PRINT_FLUSH; } } while (0)
*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GFX_HW_H_ */

/** @} */

