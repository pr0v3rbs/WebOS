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
 *  register interface implementation for png device of h13. ( used only within kdriver )
 *	png device will teach you how to make device driver with new platform.
 *
 *  @author		Youngwoo Jin (youngwoo.jin@lge.com)
 *  @version	1.0
 *  @date		2011.04.21
 *
 *  @addtogroup h13
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define TILED_BUFFER_BASE		(0x70000000)
#define LOG_NAME	vdec_hal_vdo
#define LOG_LEVEL	log_level_error

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "vdec_type_defs.h"
#include "../lg1154_vdec_base.h"

#include "vdo_reg.h"
#include "../de_vdo_hal_api.h"
#include "../top_hal_api.h"

#include "log.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define EM_RAS   		(13)
#define EM_BANK  		(3)
#define EM_CAS   		(10)
#define EM_WIDTH 		(2)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define XY2CONFIG(A,B,C,D,E,F,G,H,I) ((A)<<20 | (B)<<19 | (C)<<18 | (D)<<17 | (E)<<16 | (F)<<12 | (G)<<8 | (H)<<4 | (I))
#define XY2(A,B,C,D)                 ((A)<<12 | (B)<<8 | (C)<<4 | (D))
#define XY2BANK(A,B,C,D,E,F)         ((A)<<13 | (B)<<12 | (C)<<8 | (D)<<5 | (E)<<4 | (F))
#define RBC(A,B,C,D)                 ((A)<<10 | (B)<< 6 | (C)<<4 | (D))
#define RBC_SAME(A,B)                ((A)<<10 | (B)<< 6 | (A)<<4 | (B))
#define X_SEL 0
#define Y_SEL 1
#define CA_SEL 0
#define BA_SEL 1
#define RA_SEL 2
#define Z_SEL 3

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	LINEAR_FRAME_MAP  = 0,
	TILED_FRAME_V_MAP = 1,
	TILED_FRAME_H_MAP = 2,
	TILED_FIELD_V_MAP = 3,
	TILED_MIXED_V_MAP = 4,
	TILED_FRAME_MB_RASTER_MAP = 5,
	TILED_FIELD_MB_RASTER_MAP = 6,
	TILED_USER_MAP,
	TILED_MAP_TYPE_MAX
} TiledMapType;

typedef struct {
	SINT32  xy2caMap[16];
	SINT32  xy2baMap[16];
	SINT32  xy2raMap[16];
	SINT32  rbc2axiMap[32];
	SINT32  mapType;
	SINT32  xy2rbcConfig;

	SINT32  tiledBaseAddr;
	SINT32  tbSeparateMap;
	SINT32  topBotSplit;
	SINT32  tiledMap;
	SINT32  caIncHor;
	SINT32  convLinear;
} TiledMapConfig;

typedef struct
{
	BOOLEAN			bAutoVsync;
} VDO_CH_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static UINT32	_ChangeBufBase(UINT32 ui32Offset, UINT32 ui32OriginBase, UINT32 ui32TargetBase);
static int 		_SetTiledMapType(UINT32 u32FrameBufStartAddr, TiledMapType mapType);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static volatile UINT32 stpVdoReg = 0;//NULL;
static VDO_CH_T	stVdoCh[DE_VDO_NUM_OF_CHANNEL];
/*
 * status
 *  0: nothing. or RunCmd() called
 *  1: Update() called
 */
static int status;
static unsigned char update_channel[DE_VDO_NUM_OF_CHANNEL];
static unsigned int display_info[DE_VDO_NUM_OF_CHANNEL];
static unsigned int tiled[DE_VDO_NUM_OF_CHANNEL];
#if defined(CHIP_NAME_h13) && (CHIP_REV<0xb0)
#define bTiedChannel	TRUE
#else
#define bTiedChannel	FALSE
#endif

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetPicSize(UINT8 ui8VdoCh, UINT32 ui32PicHeight, UINT32 ui32PicWidth)
{
	UINT32		ui32PicSize = 0;

	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

	ui32PicSize = ((ui32PicWidth & 0xFFF) << 16) | (ui32PicHeight & 0xFFF);
	VDO_CH_PIC_SIZE(ui8VdoCh) = ui32PicSize;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetBufIdx(UINT8 ui8VdoCh, UINT32 ui32BufIdx)
{
	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

	VDO_CH_BUF_IDX(ui8VdoCh) = (ui32BufIdx & 0x1F);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetChromaFormat(UINT8 ui8VdoCh)
{
	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

	// To do
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetScanMode(UINT8 ui8VdoCh, UINT32 ui32ScanModeType)
{
	UINT32		ui32ScanMode = 0;

	if( ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2) )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}
	if( (ui32ScanModeType > 3) || (ui32ScanModeType == 0) )
	{
		log_error("[DE_VDO%d][Err] Scan Mode: %d, %s\n", ui8VdoCh, ui32ScanModeType, __FUNCTION__ );
		return;
	}

	ui32ScanMode = VDO_CH_SCAN_MODE(ui8VdoCh);

/*
	scan mode (4th bit)
	0: interlaced scan mode
	1: progressive scan mode
*/

	if( ui32ScanModeType == 3 ) // progressive
	{
		ui32ScanMode |= (1 << 4);
	}
	else if( ui8VdoCh < 2 ) // interlaced
	{
		ui32ScanMode &= ~(1 << 4);
		if( ui32ScanModeType == 1 ) // Top Field First
			ui32ScanMode &= ~(1 << 0);
		else // Bottom Field First
			ui32ScanMode |= (1 << 0);
	}

	VDO_CH_SCAN_MODE(ui8VdoCh) = ui32ScanMode;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_EnableMifWaitReqb(UINT32 ui32ReqbWaitCycle)
{
	VDO_LINE_WAIT = ((1 << 28) | (ui32ReqbWaitCycle & 0xFFFF));
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_DisableMifWaitReqb(void)
{
	VDO_LINE_WAIT &= ~(1 << 28);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetLineWait(UINT32 ui32LineWaitCycle)
{
	VDO_LINE_WAIT = (ui32LineWaitCycle & 0xFF);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVSyncSel(UINT8 ui8VdoCh, UINT32 ui32VSyncSrc)
{
	UINT32		ui32VsyncSel;

	if( ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2) )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

/*
	vsync source sel (2bits)
	It should be set to one of the four sync source in VDEC.
*/
	ui32VsyncSel = VDO_VSYNC_SEL;

	ui32VsyncSel &= ~(0x3 << (ui8VdoCh * 2));
	ui32VsyncSel |= ((ui32VSyncSrc & 0x3) << (ui8VdoCh * 2));

	VDO_VSYNC_SEL = ui32VsyncSel;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVSyncMode(UINT8 ui8VdoCh, BOOLEAN bAutoGen)
{
	if( ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2) )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

/*
	vsync mode (3~0 bits)
	0: MCU generation using VSYNC_GEN register
	1: Automatic generation using VBP_NUM and VSYNC_NUM registers
*/
	if( bAutoGen == TRUE )
		VDO_VSYNC_MODE |= (1 << ui8VdoCh);
	else
		VDO_VSYNC_MODE &= ~(1 << ui8VdoCh);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVActvMode(UINT8 ui8VdoCh, BOOLEAN bAutoGen)
{
	if( ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2) )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

/*
	vactive mode (7~4 bits)
	0: MCU generation using VACTV_GEN register
	1: Automatic generation using VBP_NUM and VSYNC_NUM and VFP registers

	vactive automatic generation when chx_vactv_mode set to '0' (11~8 bits)
	0: MCU should set VACTV_GEN register after ch_vsync_rdy interrupt occur
	1: Automatic generation of vactive after receiption of ch_vsync_rdy from DE
*/
	if( bAutoGen == TRUE )
	{
		VDO_VSYNC_MODE |= (1 << (ui8VdoCh + 4));
		VDO_VSYNC_MODE |= (1 << (ui8VdoCh + 8));
	}
	else
	{
		VDO_VSYNC_MODE &= ~(1 << (ui8VdoCh + 4));
		VDO_VSYNC_MODE &= ~(1 << (ui8VdoCh + 8));
	}
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVSyncGen(BOOLEAN bVSyncGen[])
{
	UINT32		ui32VSyncGen = 0;
	UINT32		ui8VdoCh;

/*
	vsync generation
	It is automatically cleared when set to '1'
*/
	for( ui8VdoCh = 0; ui8VdoCh < (DE_VDO_NUM_OF_CHANNEL * 2); ui8VdoCh++ )
	{
		if( bVSyncGen[ui8VdoCh] == TRUE )
			ui32VSyncGen |= (1 << ui8VdoCh);
	}

	VDO_VSYNC_GEN = ui32VSyncGen;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVActvGen(BOOLEAN bVActvGen[])
{
	UINT32		ui32VActvGen = 0;
	UINT32		ui8VdoCh;

/*
	vsync generation
	It is automatically cleared when set to '1'
*/
	for( ui8VdoCh = 0; ui8VdoCh < (DE_VDO_NUM_OF_CHANNEL * 2); ui8VdoCh++ )
	{
		if( bVActvGen[ui8VdoCh] == TRUE )
			ui32VActvGen |= (1 << ui8VdoCh);
	}

	VDO_VACTV_GEN = ui32VActvGen;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_Init(void)
{
	UINT32	i, ui32PreAPBVal;
	stpVdoReg		= (UINT32)VDEC_TranselateVirualAddr(VDEC_VDO_BASE, 0x100);

	log_noti("VDO Init\n");

	for(i=0; i<DE_VDO_NUM_OF_CHANNEL; i++)
	{
		update_channel[i] = 0xFF;
		tiled[i] = FALSE;
	}

	// Set DE vsync, vactive timing
	ui32PreAPBVal = TOP_HAL_SetApbVDO(0, 1);
	//VDO_CH_VBP_NUM(ui8VdoCh)        = 0x00000780;
	//VDO_CH_VFP_NUM(ui8VdoCh)        = 0x00000780;
	VDO_CH_VSYNC_NUM(0)      = 0x00000898;//0x00000780;
	VDO_CH_VSYNC_NUM(1)      = 0x00000898;//0x00000780;
	TOP_HAL_SetApbConfig(ui32PreAPBVal);

	// V Active Count Enable for Debugging
	VDO_CH_VSYNC_VACTIV_CNT(0) |= 1<<31;
	VDO_CH_VSYNC_VACTIV_CNT(1) |= 1<<31;

	VDO_VSYNC_MODE = 0xFFF;

	/* VReady wait setting */
	VDO_VSYNC_MODE &= ~(1<<(0+4));
	VDO_VSYNC_MODE &= ~(1<<(1+4));
	//VDO_VSYNC_MODE &= ~(1<<(1+4));

	/* Set all channel to VSYNC Ch0 */
	VDO_VSYNC_SEL = 0;

	stVdoCh[0].bAutoVsync = TRUE;
	stVdoCh[1].bAutoVsync = TRUE;

	// TILED MAP
	_SetTiledMapType(TILED_BUFFER_BASE, TILED_MIXED_V_MAP);
}



/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_Update(UINT8 ui8VdoCh, S_DE_IPC_T *pDeIpc, UINT32 ui32SyncField)
{
	UINT32 ui32Size;
	UINT32 ui32PreAPBVal;
	UINT16 ui16Width, ui16Height;
	UINT32 ui32AddrY, ui32AddrC;

	log_user1("VdoCh %d: Y 0x%X  C 0x%X  DispMode %d, Field %d\n",
			ui8VdoCh, pDeIpc->ui32Y_FrameBaseAddr,
			pDeIpc->ui32C_FrameBaseAddr, pDeIpc->ui32DisplayMode, ui32SyncField);

	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("[VDEC][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

	if (status == 0)
	{
		DE_VDO_HAL_InitCmd ();
		status = 1;
	}

	if (pDeIpc->ui32Tiled_FrameBaseAddr == 0)
	{
		log_trace ("vdo:%d, no tiled base\n", ui8VdoCh);
		return;
	}

	ui32AddrY = _ChangeBufBase(pDeIpc->ui32Y_FrameBaseAddr, pDeIpc->ui32Tiled_FrameBaseAddr, TILED_BUFFER_BASE);
	ui32AddrC = _ChangeBufBase(pDeIpc->ui32C_FrameBaseAddr, pDeIpc->ui32Tiled_FrameBaseAddr, TILED_BUFFER_BASE);

	display_info[ui8VdoCh] = pDeIpc->ui32DisplayMode;

	switch( ui8VdoCh )
	{
		case 0 : VDO_CONF &= ~(0x10); break;
		case 1 : VDO_CONF |=  (0x10); break;
		default :
			log_error("[VDEC][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
			return;
	}

	ui16Width	= pDeIpc->ui16PicWidth;
	ui16Height	= pDeIpc->ui16PicHeight;

	// Crop (Requested by DE)
	if( pDeIpc->ui32H_Offset & 0xFFFF )		// right crop
	{
		ui16Width = pDeIpc->ui16PicWidth - (pDeIpc->ui32H_Offset & 0xFFFF);
	}

	if( pDeIpc->ui32V_Offset & 0xFFFF )		// bottom crop
	{
		ui16Height = pDeIpc->ui16PicHeight - (pDeIpc->ui32V_Offset & 0xFFFF);
	}

	if( pDeIpc->ui16PicHeight > 1080 )		// Max height
		ui16Height = 1080;

	ui32Size = (ui16Width << 16) | ui16Height;

	ui32PreAPBVal = TOP_HAL_SetApbVDO(1, 4);
	VDO_GDI_INFO_CONTROL(ui8VdoCh)  = 0x00210000 | pDeIpc->ui32Stride; // [21:20]TILED_MIXED_MAP, [19:17]color_format(4:2:0),[16]CbCr_Interleaved,[15:0]Stride_Size
	VDO_GDI_INFO_PIC_SIZE(ui8VdoCh) = ui32Size;
	{
		VDO_GDI_INFO_BASE_Y(ui8VdoCh)   = ui32AddrY;
		VDO_GDI_INFO_BASE_CB(ui8VdoCh)  = ui32AddrC;
		VDO_GDI_INFO_BASE_CR(ui8VdoCh)  = ui32AddrC;
	}

	TOP_HAL_SetApbConfig(ui32PreAPBVal);

	VDO_CH_PIC_SIZE(ui8VdoCh)       = ui32Size;//(pDeIpc->ui16PicWidth << 16) | (pDeIpc->ui16PicHeight & 0xFFFF);
	VDO_CH_BUF_IDX(ui8VdoCh)        = ui8VdoCh;//pDeIpc->ui32FrameIdx + (ui8VdoCh*10); /* Fixed 11 total frame each channel */
	VDO_CH_CHROMA_FORMAT(ui8VdoCh)  = 0x00040004;

	if( pDeIpc->ui32DisplayMode == 3 )
	{
		/* Progressive*/
		VDO_CH_SCAN_MODE(ui8VdoCh)  |= 0x00000010;

		if( ui32SyncField & 0x1 )
			VDO_CH_SCAN_MODE(ui8VdoCh)	|=  (0x1);
		else
			VDO_CH_SCAN_MODE(ui8VdoCh)	&= ~(0x1);
	}
	else
	{
		/* Interaced */
		VDO_CH_SCAN_MODE(ui8VdoCh)  &= ~(0x00000010);

		if( pDeIpc->ui32DisplayMode == 2 )
			VDO_CH_SCAN_MODE(ui8VdoCh)	|=  (0x1);
		else if( pDeIpc->ui32DisplayMode == 1 )
			VDO_CH_SCAN_MODE(ui8VdoCh)	&= ~(0x1);
	}

	if( bTiedChannel )
		tiled[ui8VdoCh] = TRUE;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_InitCmd(void)
{
	UINT32 ui32PreAPBVal;

	if( VDO_MIF_STATUS || VDO_RDC_STATUS )
	{
		static UINT32 u32DbgCnt=0;
		if( !((u32DbgCnt++) % 100 ) )
		{
			log_warning("[DE_VDO][Err] Status : MIF(0x%08X/0x%08X), RDC(0x%08X/0x%08X), %s\n",
					VDO_MIF_STATUS, VDO_MIF_LINE_CNT,
					VDO_RDC_STATUS, VDO_RDC_LINE_CNT, __FUNCTION__ );
			log_warning("[DE_VDO #0][DBG]   ActiveCnt %d  StallCnt Luma %d Croma %d\n",
					VDO_CH_VSYNC_VACTIV_CNT(0) & 0xFFFFFF,
					VDO_CH0_LUMA_STALL_CNT, VDO_CH0_CHMA_STALL_CNT );
			log_warning("[DE_VDO #1][DBG]   ActiveCnt %d  StallCnt Luma %d Croma %d\n",
					VDO_CH_VSYNC_VACTIV_CNT(1) & 0xFFFFFF,
					VDO_CH1_LUMA_STALL_CNT, VDO_CH1_CHMA_STALL_CNT );
		}
	}

	ui32PreAPBVal = TOP_HAL_SetApbVDO(1, 0);
	VDO_GDI_PIC_INIT_HOST = 0x01;
	VDO_GDI_PIC_INIT_HOST = 0x00;
	TOP_HAL_SetApbConfig(ui32PreAPBVal);

	// VDO flush
	VDO_CMD  = 0x00000010;        // pic_init
	VDO_CMD  = 0x00000100;        // flush_run

	// Wait for end of flush
	while((VDO_MIF_STATUS & 0x1f) != 0x0)
	{
		UINT32 u32Cnt=0;
		u32Cnt++;
		if( u32Cnt > 0x100000 )
		{
			log_error("vdo infinite loop: MIF Status 0x%X\n", VDO_MIF_STATUS);
			break;
		}
	}

	VDO_CMD  = 0x00000010;        // pic_init
	VDO_CONF = 0x08081000;        // TODO : should be fixed dual_mode, chan_sel, vsync_sel, endian setting

	log_trace("vdo init cmd\n");
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_RunCmd(UINT8 ui8DeIfCh[], UINT32 aui32DisplayInfo[], UINT32 ui32SyncField)
{
	UINT8		ui8UseCh;
	UINT8		ui8UpdatedCh = 0;
	UINT32		ui32SyncGen = 0;

	if (status == 0)
	{
		log_trace ("no tiled picture\n");
		return;
	}

	status = 0;

	if (ui8DeIfCh == NULL)
		ui8DeIfCh = update_channel;
	if (aui32DisplayInfo == NULL)
		aui32DisplayInfo = display_info;

	if( bTiedChannel && tiled[0] == FALSE && tiled[1] == TRUE )
		VDO_CH_SCAN_MODE(0) = VDO_CH_SCAN_MODE(1);

	for( ui8UseCh = 0 ; ui8UseCh < DE_VDO_NUM_OF_CHANNEL ; ui8UseCh++ )
	{
		if( ui8DeIfCh[ui8UseCh] == 0xFF )
			continue;

		if( tiled[ui8UseCh] == TRUE )
			ui8UpdatedCh++;

		if( bTiedChannel )
			tiled[ui8UseCh] = FALSE;

		if( !stVdoCh[ui8UseCh].bAutoVsync )
		{
			if( ui32SyncField == 0 )
				ui32SyncGen |= (0x01 << ui8DeIfCh[ui8UseCh]);
			else if( ( aui32DisplayInfo[ui8UseCh] == 1 ) ||
					( aui32DisplayInfo[ui8UseCh] == 2 ) )
				ui32SyncGen |= (0x01 << ui8DeIfCh[ui8UseCh]);
		}
	}

	if( ui8UpdatedCh )
	{
		/* Dual mode selection */
		if( ui8UpdatedCh == DE_VDO_NUM_OF_CHANNEL )
			VDO_CONF |=  (0x00000001);	// Dual mode
		else
			VDO_CONF &= ~(0x00000001);	// Single mode

		VDO_CMD = 0x00000001;
	}

	if( ui32SyncGen != 0 )
	{
		VDO_VSYNC_GEN = ui32SyncGen;
	}

	log_trace("vdo runCmd sgen %x nCh %d\n", ui32SyncGen, ui8UpdatedCh);
}

void DE_VDO_HAL_Register(UINT8 u8VdoCh, UINT8 u8DeIfCh, BOOLEAN bTiled)
{
	if( u8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("wrong vdo channel num %d\n", u8VdoCh);
		return;
	}

	log_noti("VDO Register %d deif %d tiled %d\n", u8VdoCh, u8DeIfCh, bTiled);

	update_channel[u8VdoCh] = u8DeIfCh;

	if( !bTiedChannel )
		tiled[u8VdoCh] = bTiled;
}

void	DE_VDO_HAL_SetInterlaced(UINT8 ui8VdoCh, BOOLEAN bInterlaced)
{
	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("wrong vdo channel num %d\n", ui8VdoCh);
		return;
	}

	if( bInterlaced == TRUE )
	{
		/* Interlaced */
		VDO_CH_SCAN_MODE(ui8VdoCh)  &= ~0x00000010;
	}
	else
	{
		/* Progressive */
		VDO_CH_SCAN_MODE(ui8VdoCh)  |= 0x00000010;
	}
}

/*========================================================================================
	Static Implementation Group
========================================================================================*/
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static UINT32	_ChangeBufBase(UINT32 ui32Offset, UINT32 ui32OriginBase, UINT32 ui32TargetBase)
{
	UINT32 ui32Top, ui32Bottom;

	ui32Top = ui32Offset & 0xFFFF;
	ui32Bottom = (ui32Offset >> 16) & 0xFFFF;

	ui32Top = ui32Top << (EM_BANK + EM_CAS + EM_WIDTH);
	ui32Bottom = ui32Bottom << (EM_BANK + EM_CAS + EM_WIDTH);

	ui32Top += ui32OriginBase;
	ui32Bottom += ui32OriginBase;

	ui32Top -= ui32TargetBase;
	ui32Bottom -= ui32TargetBase;

	ui32Top = ui32Top >> (EM_BANK + EM_CAS + EM_WIDTH);
	ui32Bottom = ui32Bottom >> (EM_BANK + EM_CAS + EM_WIDTH);

	return (((ui32Bottom & 0xFFFF) << 16) | (ui32Top & 0xFFFF));
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static int _SetTiledMapType(UINT32 u32FrameBufStartAddr, TiledMapType mapType)
{
	TiledMapConfig stMapCfg = { .xy2rbcConfig = 0 };
	TiledMapConfig *pMapCfg = &stMapCfg;
	UINT32 ui32PreAPBVal;

	int luma_map;
	int chro_map;
	int i;

	pMapCfg->mapType = mapType;
	//         inv = 1'b0, zero = 1'b1 , tbxor = 1'b0, xy = 1'b0, bit = 4'd0
	luma_map = 64;
	chro_map = 64;

	for (i=0; i<16 ; i=i+1) {
		pMapCfg->xy2caMap[i] = luma_map << 8 | chro_map;
	}

	for (i=0; i<4;  i=i+1) {
		pMapCfg->xy2baMap[i] = luma_map << 8 | chro_map;
	}

	for (i=0; i<16; i=i+1) {
		pMapCfg->xy2raMap[i] = luma_map << 8 | chro_map;
	}

	switch(mapType)
	{
		case LINEAR_FRAME_MAP:
			pMapCfg->xy2rbcConfig = 0;
			break;
		case TILED_FRAME_V_MAP:
			//cas
			pMapCfg->xy2caMap[0] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2caMap[1] = XY2(Y_SEL, 0, Y_SEL, 0);
			pMapCfg->xy2caMap[2] = XY2(Y_SEL, 1, Y_SEL, 1);
			pMapCfg->xy2caMap[3] = XY2(Y_SEL, 2, Y_SEL, 2);
			pMapCfg->xy2caMap[4] = XY2(Y_SEL, 3, Y_SEL, 3);
			pMapCfg->xy2caMap[5] = XY2(X_SEL, 3, X_SEL, 3);
			pMapCfg->xy2caMap[6] = XY2(X_SEL, 4, X_SEL, 4);
			pMapCfg->xy2caMap[7] = XY2(X_SEL, 5, X_SEL, 5);
			pMapCfg->xy2caMap[8] = XY2(X_SEL, 6, X_SEL, 6);
			pMapCfg->xy2caMap[9] = XY2(Y_SEL, 4, Y_SEL, 4);

			//bank
			pMapCfg->xy2baMap[0] = XY2BANK(0,X_SEL, 7, 4, X_SEL, 7);
			pMapCfg->xy2baMap[1] = XY2BANK(0,X_SEL, 8, 4, X_SEL, 8);
			pMapCfg->xy2baMap[2] = XY2BANK(0,Y_SEL, 5, 4, Y_SEL, 5);

			//ras
			pMapCfg->xy2raMap[ 0] = XY2(X_SEL, 9, X_SEL, 9);
			pMapCfg->xy2raMap[ 1] = XY2(X_SEL,10, X_SEL,10);
			pMapCfg->xy2raMap[ 2] = XY2(Y_SEL, 6, Y_SEL, 6);
			pMapCfg->xy2raMap[ 3] = XY2(Y_SEL, 7, Y_SEL, 7);
			pMapCfg->xy2raMap[ 4] = XY2(Y_SEL, 8, Y_SEL, 8);
			pMapCfg->xy2raMap[ 5] = XY2(Y_SEL, 9, Y_SEL, 9);
			pMapCfg->xy2raMap[ 6] = XY2(Y_SEL,10, Y_SEL,10);
			pMapCfg->xy2raMap[ 7] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[ 8] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[ 9] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[10] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[11] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[12] = XY2(Z_SEL, 0, Z_SEL, 0);

			//xy2rbc_config
			pMapCfg->xy2rbcConfig = XY2CONFIG(0,0,0,1,1,15,0,15,0);
			break;
		case TILED_FRAME_H_MAP:
			//cas
			pMapCfg->xy2caMap[0] = XY2(X_SEL, 3, X_SEL, 3);
			pMapCfg->xy2caMap[1] = XY2(X_SEL, 4, X_SEL, 4);
			pMapCfg->xy2caMap[2] = XY2(X_SEL, 5, X_SEL, 5);
			pMapCfg->xy2caMap[3] = XY2(X_SEL, 6, X_SEL, 6);
			pMapCfg->xy2caMap[4] = XY2(Y_SEL, 0, Y_SEL, 0);
			pMapCfg->xy2caMap[5] = XY2(Y_SEL, 1, Y_SEL, 1);
			pMapCfg->xy2caMap[6] = XY2(Y_SEL, 2, Y_SEL, 2);
			pMapCfg->xy2caMap[7] = XY2(Y_SEL, 3, Y_SEL, 3);
			pMapCfg->xy2caMap[8] = XY2(Y_SEL, 4, Y_SEL, 5);

			//bank
			pMapCfg->xy2baMap[0] = XY2BANK(0,X_SEL, 7, 4,X_SEL, 7);
			pMapCfg->xy2baMap[1] = XY2BANK(0,Y_SEL, 5, 4,Y_SEL, 4);

			//ras
			pMapCfg->xy2raMap[ 0] = XY2(X_SEL, 8, X_SEL, 8);
			pMapCfg->xy2raMap[ 1] = XY2(X_SEL, 9, X_SEL, 9);
			pMapCfg->xy2raMap[ 2] = XY2(X_SEL,10, X_SEL,10);
			pMapCfg->xy2raMap[ 3] = XY2(Y_SEL, 6, Y_SEL, 6);
			pMapCfg->xy2raMap[ 4] = XY2(Y_SEL, 7, Y_SEL, 7);
			pMapCfg->xy2raMap[ 5] = XY2(Y_SEL, 8, Y_SEL, 8);
			pMapCfg->xy2raMap[ 6] = XY2(Y_SEL, 9, Y_SEL, 9);
			pMapCfg->xy2raMap[ 7] = XY2(Y_SEL,10, Y_SEL,10);
			pMapCfg->xy2raMap[ 8] = XY2(Y_SEL,11, Y_SEL,11);
			pMapCfg->xy2raMap[ 9] = XY2(Y_SEL,12, Y_SEL,12);
			pMapCfg->xy2raMap[10] = XY2(Y_SEL,13, Y_SEL,13);
			pMapCfg->xy2raMap[11] = XY2(Y_SEL,14, Y_SEL,14);
			pMapCfg->xy2raMap[12] = XY2(Y_SEL,15, Y_SEL,15);

			//xy2rbcConfig
			pMapCfg->xy2rbcConfig = XY2CONFIG(0,0,0,1,0,15,15,15,15);
			break;
		case TILED_FIELD_V_MAP:
			//cas
			pMapCfg->xy2caMap[0] = XY2(Y_SEL, 0, Y_SEL, 0);
			pMapCfg->xy2caMap[1] = XY2(Y_SEL, 1, Y_SEL, 1);
			pMapCfg->xy2caMap[2] = XY2(Y_SEL, 2, Y_SEL, 2);
			pMapCfg->xy2caMap[3] = XY2(Y_SEL, 3, Y_SEL, 3);
			pMapCfg->xy2caMap[4] = XY2(X_SEL, 3, X_SEL, 3);
			pMapCfg->xy2caMap[5] = XY2(X_SEL, 4, X_SEL, 4);
			pMapCfg->xy2caMap[6] = XY2(X_SEL, 5, X_SEL, 5);
			pMapCfg->xy2caMap[7] = XY2(X_SEL, 6, X_SEL, 6);
			pMapCfg->xy2caMap[8] = XY2(Y_SEL, 4, Y_SEL, 5);

			//bank
			pMapCfg->xy2baMap[0] = XY2BANK(0,X_SEL, 7, 4,X_SEL, 7);
			pMapCfg->xy2baMap[1] = XY2BANK(1,Y_SEL, 5, 5,Y_SEL, 4);

			//ras
			pMapCfg->xy2raMap[ 0] = XY2(X_SEL, 8, X_SEL, 8);
			pMapCfg->xy2raMap[ 1] = XY2(X_SEL, 9, X_SEL, 9);
			pMapCfg->xy2raMap[ 2] = XY2(X_SEL,10, X_SEL,10);
			pMapCfg->xy2raMap[ 3] = XY2(Y_SEL, 6, Y_SEL, 6);
			pMapCfg->xy2raMap[ 4] = XY2(Y_SEL, 7, Y_SEL, 7);
			pMapCfg->xy2raMap[ 5] = XY2(Y_SEL, 8, Y_SEL, 8);
			pMapCfg->xy2raMap[ 6] = XY2(Y_SEL, 9, Y_SEL, 9);
			pMapCfg->xy2raMap[ 7] = XY2(Y_SEL,10, Y_SEL,10);
			pMapCfg->xy2raMap[ 8] = XY2(Y_SEL,11, Y_SEL,11);
			pMapCfg->xy2raMap[ 9] = XY2(Y_SEL,12, Y_SEL,12);
			pMapCfg->xy2raMap[10] = XY2(Y_SEL,13, Y_SEL,13);
			pMapCfg->xy2raMap[11] = XY2(Y_SEL,14, Y_SEL,14);
			pMapCfg->xy2raMap[12] = XY2(Y_SEL,15, Y_SEL,15);

			//xy2rbcConfig
			pMapCfg->xy2rbcConfig = XY2CONFIG(0,1,1,1,1,15,15,15,15);
			break;
		case TILED_MIXED_V_MAP:
			//cas
			pMapCfg->xy2caMap[0] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2caMap[1] = XY2(Y_SEL, 1, Y_SEL, 1);
			pMapCfg->xy2caMap[2] = XY2(Y_SEL, 2, Y_SEL, 2);
			pMapCfg->xy2caMap[3] = XY2(Y_SEL, 3, Y_SEL, 3);
			pMapCfg->xy2caMap[4] = XY2(Y_SEL, 0, Y_SEL, 0);
			pMapCfg->xy2caMap[5] = XY2(X_SEL, 3, X_SEL, 3);
			pMapCfg->xy2caMap[6] = XY2(X_SEL, 4, X_SEL, 4);
			pMapCfg->xy2caMap[7] = XY2(X_SEL, 5, X_SEL, 5);
			pMapCfg->xy2caMap[8] = XY2(X_SEL, 6, X_SEL, 6);
			pMapCfg->xy2caMap[9] = XY2(Y_SEL, 4, Y_SEL, 4);

			//bank
			pMapCfg->xy2baMap[0] = XY2BANK(0,X_SEL, 7, 4,X_SEL, 7);
			pMapCfg->xy2baMap[1] = XY2BANK(0,X_SEL, 8, 4,X_SEL, 8);
			pMapCfg->xy2baMap[2] = XY2BANK(0,Y_SEL, 5, 4,Y_SEL, 5);

			//ras
			pMapCfg->xy2raMap[ 0] = XY2(X_SEL, 9, X_SEL, 9);
			pMapCfg->xy2raMap[ 1] = XY2(X_SEL,10, X_SEL,10);
			pMapCfg->xy2raMap[ 2] = XY2(Y_SEL, 6, Y_SEL, 6);
			pMapCfg->xy2raMap[ 3] = XY2(Y_SEL, 7, Y_SEL, 7);
			pMapCfg->xy2raMap[ 4] = XY2(Y_SEL, 8, Y_SEL, 8);
			pMapCfg->xy2raMap[ 5] = XY2(Y_SEL, 9, Y_SEL, 9);
			pMapCfg->xy2raMap[ 6] = XY2(Y_SEL,10, Y_SEL,10);
			pMapCfg->xy2raMap[ 7] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[ 8] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[ 9] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[10] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[11] = XY2(Z_SEL, 0, Z_SEL, 0);
			pMapCfg->xy2raMap[12] = XY2(Z_SEL, 0, Z_SEL, 0);

			//xy2rbcConfig
			pMapCfg->xy2rbcConfig = XY2CONFIG(0,0,1,1,1,7,7,7,7);
			break;
		case TILED_FRAME_MB_RASTER_MAP:
			//cas
			pMapCfg->xy2caMap[0] = XY2(Y_SEL, 0, Y_SEL, 0);
			pMapCfg->xy2caMap[1] = XY2(Y_SEL, 1, Y_SEL, 1);
			pMapCfg->xy2caMap[2] = XY2(Y_SEL, 2, Y_SEL, 2);
			pMapCfg->xy2caMap[3] = XY2(Y_SEL, 3, X_SEL, 3);
			pMapCfg->xy2caMap[4] = XY2(X_SEL, 3, 4    , 0);

			//xy2rbcConfig
			pMapCfg->xy2rbcConfig = XY2CONFIG(0,0,0,1,1,15,0,7,0);
			break;
		case TILED_FIELD_MB_RASTER_MAP:
			//cas
			pMapCfg->xy2caMap[0] = XY2(Y_SEL, 0, Y_SEL, 0);
			pMapCfg->xy2caMap[1] = XY2(Y_SEL, 1, Y_SEL, 1);
			pMapCfg->xy2caMap[2] = XY2(Y_SEL, 2, X_SEL, 3);
			pMapCfg->xy2caMap[3] = XY2(X_SEL, 3, 4    , 0);

			//xy2rbcConfig
			pMapCfg->xy2rbcConfig = XY2CONFIG(0,1,1,1,1,7,7,3,3);
			break;
		case TILED_USER_MAP:
			break;
		default:
			return 0;
			break;
	}

	if (mapType == TILED_FRAME_MB_RASTER_MAP)
	{
		pMapCfg->rbc2axiMap[ 0] = RBC( Z_SEL, 0,  Z_SEL, 0);
		pMapCfg->rbc2axiMap[ 1] = RBC( Z_SEL, 0,  Z_SEL, 0);
		pMapCfg->rbc2axiMap[ 2] = RBC( Z_SEL, 0,  Z_SEL, 0);
		pMapCfg->rbc2axiMap[ 3] = RBC(CA_SEL, 0, CA_SEL, 0);
		pMapCfg->rbc2axiMap[ 4] = RBC(CA_SEL, 1, CA_SEL, 1);
		pMapCfg->rbc2axiMap[ 5] = RBC(CA_SEL, 2, CA_SEL, 2);
		pMapCfg->rbc2axiMap[ 6] = RBC(CA_SEL, 3, CA_SEL, 3);
		pMapCfg->rbc2axiMap[ 7] = RBC(CA_SEL, 4, CA_SEL, 8);
		pMapCfg->rbc2axiMap[ 8] = RBC(CA_SEL, 8, CA_SEL, 9);
		pMapCfg->rbc2axiMap[ 9] = RBC(CA_SEL, 9, CA_SEL,10);
		pMapCfg->rbc2axiMap[10] = RBC(CA_SEL,10, CA_SEL,11);
		pMapCfg->rbc2axiMap[11] = RBC(CA_SEL,11, CA_SEL,12);
		pMapCfg->rbc2axiMap[12] = RBC(CA_SEL,12, CA_SEL,13);
		pMapCfg->rbc2axiMap[13] = RBC(CA_SEL,13, CA_SEL,14);
		pMapCfg->rbc2axiMap[14] = RBC(CA_SEL,14, CA_SEL,15);
		pMapCfg->rbc2axiMap[15] = RBC(CA_SEL,15, RA_SEL, 0);
		pMapCfg->rbc2axiMap[16] = RBC(RA_SEL, 0, RA_SEL, 1);
		pMapCfg->rbc2axiMap[17] = RBC(RA_SEL, 1, RA_SEL, 2);
		pMapCfg->rbc2axiMap[18] = RBC(RA_SEL, 2, RA_SEL, 3);
		pMapCfg->rbc2axiMap[19] = RBC(RA_SEL, 3, RA_SEL, 4);
		pMapCfg->rbc2axiMap[20] = RBC(RA_SEL, 4, RA_SEL, 5);
		pMapCfg->rbc2axiMap[21] = RBC(RA_SEL, 5, RA_SEL, 6);
		pMapCfg->rbc2axiMap[22] = RBC(RA_SEL, 6, RA_SEL, 7);
		pMapCfg->rbc2axiMap[23] = RBC(RA_SEL, 7, RA_SEL, 8);
		pMapCfg->rbc2axiMap[24] = RBC(RA_SEL, 8, RA_SEL, 9);
		pMapCfg->rbc2axiMap[25] = RBC(RA_SEL, 9, RA_SEL,10);
		pMapCfg->rbc2axiMap[26] = RBC(RA_SEL,10, RA_SEL,11);
		pMapCfg->rbc2axiMap[27] = RBC(RA_SEL,11, RA_SEL,12);
		pMapCfg->rbc2axiMap[28] = RBC(RA_SEL,12, RA_SEL,13);
		pMapCfg->rbc2axiMap[29] = RBC(RA_SEL,13, RA_SEL,14);
		pMapCfg->rbc2axiMap[30] = RBC(RA_SEL,14, RA_SEL,15);
		pMapCfg->rbc2axiMap[31] = RBC(RA_SEL,15,  Z_SEL, 0);
	}
	else if (mapType == TILED_FIELD_MB_RASTER_MAP)
	{
		pMapCfg->rbc2axiMap[ 0] = RBC(Z_SEL ,0  ,Z_SEL , 0);
		pMapCfg->rbc2axiMap[ 1] = RBC(Z_SEL ,0  ,Z_SEL , 0);
		pMapCfg->rbc2axiMap[ 2] = RBC(Z_SEL ,0  ,Z_SEL , 0);
		pMapCfg->rbc2axiMap[ 3] = RBC(CA_SEL,0  ,CA_SEL, 0);
		pMapCfg->rbc2axiMap[ 4] = RBC(CA_SEL,1  ,CA_SEL, 1);
		pMapCfg->rbc2axiMap[ 5] = RBC(CA_SEL,2  ,CA_SEL, 2);
		pMapCfg->rbc2axiMap[ 6] = RBC(CA_SEL,3  ,CA_SEL, 8);
		pMapCfg->rbc2axiMap[ 7] = RBC(CA_SEL,8,  CA_SEL, 9);
		pMapCfg->rbc2axiMap[ 8] = RBC(CA_SEL,9,  CA_SEL,10);
		pMapCfg->rbc2axiMap[ 9] = RBC(CA_SEL,10 ,CA_SEL,11);
		pMapCfg->rbc2axiMap[10] = RBC(CA_SEL,11 ,CA_SEL,12);
		pMapCfg->rbc2axiMap[11] = RBC(CA_SEL,12 ,CA_SEL,13);
		pMapCfg->rbc2axiMap[12] = RBC(CA_SEL,13 ,CA_SEL,14);
		pMapCfg->rbc2axiMap[13] = RBC(CA_SEL,14 ,CA_SEL,15);
		pMapCfg->rbc2axiMap[14] = RBC(CA_SEL,15 ,RA_SEL, 0);

		pMapCfg->rbc2axiMap[15] = RBC(RA_SEL,0  ,RA_SEL, 1);
		pMapCfg->rbc2axiMap[16] = RBC(RA_SEL,1  ,RA_SEL, 2);
		pMapCfg->rbc2axiMap[17] = RBC(RA_SEL,2  ,RA_SEL, 3);
		pMapCfg->rbc2axiMap[18] = RBC(RA_SEL,3  ,RA_SEL, 4);
		pMapCfg->rbc2axiMap[19] = RBC(RA_SEL,4  ,RA_SEL, 5);
		pMapCfg->rbc2axiMap[20] = RBC(RA_SEL,5  ,RA_SEL, 6);
		pMapCfg->rbc2axiMap[21] = RBC(RA_SEL,6  ,RA_SEL, 7);
		pMapCfg->rbc2axiMap[22] = RBC(RA_SEL,7  ,RA_SEL, 8);
		pMapCfg->rbc2axiMap[23] = RBC(RA_SEL,8  ,RA_SEL, 9);
		pMapCfg->rbc2axiMap[24] = RBC(RA_SEL,9  ,RA_SEL,10);
		pMapCfg->rbc2axiMap[25] = RBC(RA_SEL,10 ,RA_SEL,11);
		pMapCfg->rbc2axiMap[26] = RBC(RA_SEL,11 ,RA_SEL,12);
		pMapCfg->rbc2axiMap[27] = RBC(RA_SEL,12 ,RA_SEL,13);
		pMapCfg->rbc2axiMap[28] = RBC(RA_SEL,13 ,RA_SEL,14);
		pMapCfg->rbc2axiMap[29] = RBC(RA_SEL,14 ,RA_SEL,15);
		pMapCfg->rbc2axiMap[30] = RBC(RA_SEL,15 , Z_SEL, 0);
		pMapCfg->rbc2axiMap[31] = RBC(Z_SEL , 0 , Z_SEL, 0);
	}
	else
	{
		pMapCfg->rbc2axiMap[ 0] = RBC(Z_SEL , 0,Z_SEL , 0);
		pMapCfg->rbc2axiMap[ 1] = RBC(Z_SEL , 0,Z_SEL , 0);
		pMapCfg->rbc2axiMap[ 2] = RBC(CA_SEL, 0,CA_SEL, 0);
		pMapCfg->rbc2axiMap[ 3] = RBC(CA_SEL, 1,CA_SEL, 1);
		pMapCfg->rbc2axiMap[ 4] = RBC(CA_SEL, 2,CA_SEL, 2);
		pMapCfg->rbc2axiMap[ 5] = RBC(CA_SEL, 3,CA_SEL, 3);
		pMapCfg->rbc2axiMap[ 6] = RBC(CA_SEL, 4,CA_SEL, 4);
		pMapCfg->rbc2axiMap[ 7] = RBC(CA_SEL, 5,CA_SEL, 5);
		pMapCfg->rbc2axiMap[ 8] = RBC(CA_SEL, 6,CA_SEL, 6);
		pMapCfg->rbc2axiMap[ 9] = RBC(CA_SEL, 7,CA_SEL, 7);
		pMapCfg->rbc2axiMap[10] = RBC(CA_SEL, 8,CA_SEL, 8);
		pMapCfg->rbc2axiMap[11] = RBC(CA_SEL, 9,CA_SEL, 9);

		pMapCfg->rbc2axiMap[12] = RBC(BA_SEL, 0,BA_SEL, 0);
		pMapCfg->rbc2axiMap[13] = RBC(BA_SEL, 1,BA_SEL, 1);
		pMapCfg->rbc2axiMap[14] = RBC(BA_SEL, 2,BA_SEL, 2);

		pMapCfg->rbc2axiMap[15] = RBC(RA_SEL, 0,RA_SEL, 0);
		pMapCfg->rbc2axiMap[16] = RBC(RA_SEL, 1,RA_SEL, 1);
		pMapCfg->rbc2axiMap[17] = RBC(RA_SEL, 2,RA_SEL, 2);
		pMapCfg->rbc2axiMap[18] = RBC(RA_SEL, 3,RA_SEL, 3);
		pMapCfg->rbc2axiMap[19] = RBC(RA_SEL, 4,RA_SEL, 4);
		pMapCfg->rbc2axiMap[20] = RBC(RA_SEL, 5,RA_SEL, 5);
		pMapCfg->rbc2axiMap[21] = RBC(RA_SEL, 6,RA_SEL, 6);
		pMapCfg->rbc2axiMap[22] = RBC(RA_SEL, 7,RA_SEL, 7);
		pMapCfg->rbc2axiMap[23] = RBC(RA_SEL, 8,RA_SEL, 8);
		pMapCfg->rbc2axiMap[24] = RBC(RA_SEL, 9,RA_SEL, 9);
		pMapCfg->rbc2axiMap[25] = RBC(RA_SEL,10,RA_SEL,10);
		pMapCfg->rbc2axiMap[26] = RBC(RA_SEL,11,RA_SEL,11);
		pMapCfg->rbc2axiMap[27] = RBC(RA_SEL,12,RA_SEL,12);

		pMapCfg->rbc2axiMap[28] = RBC(Z_SEL , 0,Z_SEL , 0);
		pMapCfg->rbc2axiMap[29] = RBC(Z_SEL , 0,Z_SEL , 0);
		pMapCfg->rbc2axiMap[30] = RBC(Z_SEL , 0,Z_SEL , 0);
		pMapCfg->rbc2axiMap[31] = RBC(Z_SEL , 0,Z_SEL , 0);
	}

	//// fast access for reading
	pMapCfg->tbSeparateMap  = (pMapCfg->xy2rbcConfig >> 19) & 0x1;
	pMapCfg->topBotSplit    = (pMapCfg->xy2rbcConfig >> 18) & 0x1;
	pMapCfg->tiledMap		= (pMapCfg->xy2rbcConfig >> 17) & 0x1;
	pMapCfg->caIncHor       = (pMapCfg->xy2rbcConfig >> 16) & 0x1;

	// VDO GDI Setting /*{{{*/
	ui32PreAPBVal = TOP_HAL_SetApbVDO(1, 8);

	VDO_GDI_XY2_CAS_0 = pMapCfg->xy2caMap[ 0];
	VDO_GDI_XY2_CAS_1 = pMapCfg->xy2caMap[ 1];
	VDO_GDI_XY2_CAS_2 = pMapCfg->xy2caMap[ 2];
	VDO_GDI_XY2_CAS_3 = pMapCfg->xy2caMap[ 3];
	VDO_GDI_XY2_CAS_4 = pMapCfg->xy2caMap[ 4];
	VDO_GDI_XY2_CAS_5 = pMapCfg->xy2caMap[ 5];
	VDO_GDI_XY2_CAS_6 = pMapCfg->xy2caMap[ 6];
	VDO_GDI_XY2_CAS_7 = pMapCfg->xy2caMap[ 7];
	VDO_GDI_XY2_CAS_8 = pMapCfg->xy2caMap[ 8];
	VDO_GDI_XY2_CAS_9 = pMapCfg->xy2caMap[ 9];
	VDO_GDI_XY2_CAS_A = pMapCfg->xy2caMap[10];
	VDO_GDI_XY2_CAS_B = pMapCfg->xy2caMap[11];
	VDO_GDI_XY2_CAS_C = pMapCfg->xy2caMap[12];
	VDO_GDI_XY2_CAS_D = pMapCfg->xy2caMap[13];
	VDO_GDI_XY2_CAS_E = pMapCfg->xy2caMap[14];
	VDO_GDI_XY2_CAS_F = pMapCfg->xy2caMap[15];

	VDO_GDI_XY2_BA_0  = pMapCfg->xy2baMap[0];
	VDO_GDI_XY2_BA_1  = pMapCfg->xy2baMap[1];
	VDO_GDI_XY2_BA_2  = pMapCfg->xy2baMap[2];
	VDO_GDI_XY2_BA_3  = pMapCfg->xy2baMap[3];

	VDO_GDI_XY2_RAS_0 = pMapCfg->xy2raMap[ 0];
	VDO_GDI_XY2_RAS_1 = pMapCfg->xy2raMap[ 1];
	VDO_GDI_XY2_RAS_2 = pMapCfg->xy2raMap[ 2];
	VDO_GDI_XY2_RAS_3 = pMapCfg->xy2raMap[ 3];
	VDO_GDI_XY2_RAS_4 = pMapCfg->xy2raMap[ 4];
	VDO_GDI_XY2_RAS_5 = pMapCfg->xy2raMap[ 5];
	VDO_GDI_XY2_RAS_6 = pMapCfg->xy2raMap[ 6];
	VDO_GDI_XY2_RAS_7 = pMapCfg->xy2raMap[ 7];
	VDO_GDI_XY2_RAS_8 = pMapCfg->xy2raMap[ 8];
	VDO_GDI_XY2_RAS_9 = pMapCfg->xy2raMap[ 9];
	VDO_GDI_XY2_RAS_A = pMapCfg->xy2raMap[10];
	VDO_GDI_XY2_RAS_B = pMapCfg->xy2raMap[11];
	VDO_GDI_XY2_RAS_C = pMapCfg->xy2raMap[12];
	VDO_GDI_XY2_RAS_D = pMapCfg->xy2raMap[13];
	VDO_GDI_XY2_RAS_E = pMapCfg->xy2raMap[14];
	VDO_GDI_XY2_RAS_F = pMapCfg->xy2raMap[15];

	//xy2rbc_config
	VDO_GDI_XY2_RBC_CONFIG = pMapCfg->xy2rbcConfig;

	VDO_GDI_RBC2_AXI_0  = pMapCfg->rbc2axiMap[ 0];
	VDO_GDI_RBC2_AXI_1  = pMapCfg->rbc2axiMap[ 1];
	VDO_GDI_RBC2_AXI_2  = pMapCfg->rbc2axiMap[ 2];
	VDO_GDI_RBC2_AXI_3  = pMapCfg->rbc2axiMap[ 3];
	VDO_GDI_RBC2_AXI_4  = pMapCfg->rbc2axiMap[ 4];
	VDO_GDI_RBC2_AXI_5  = pMapCfg->rbc2axiMap[ 5];
	VDO_GDI_RBC2_AXI_6  = pMapCfg->rbc2axiMap[ 6];
	VDO_GDI_RBC2_AXI_7  = pMapCfg->rbc2axiMap[ 7];
	VDO_GDI_RBC2_AXI_8  = pMapCfg->rbc2axiMap[ 8];
	VDO_GDI_RBC2_AXI_9  = pMapCfg->rbc2axiMap[ 9];
	VDO_GDI_RBC2_AXI_A  = pMapCfg->rbc2axiMap[10];
	VDO_GDI_RBC2_AXI_B  = pMapCfg->rbc2axiMap[11];
	VDO_GDI_RBC2_AXI_C  = pMapCfg->rbc2axiMap[12];
	VDO_GDI_RBC2_AXI_D  = pMapCfg->rbc2axiMap[13];
	VDO_GDI_RBC2_AXI_E  = pMapCfg->rbc2axiMap[14];
	VDO_GDI_RBC2_AXI_F  = pMapCfg->rbc2axiMap[15];
	VDO_GDI_RBC2_AXI_10 = pMapCfg->rbc2axiMap[16];
	VDO_GDI_RBC2_AXI_11 = pMapCfg->rbc2axiMap[17];
	VDO_GDI_RBC2_AXI_12 = pMapCfg->rbc2axiMap[18];
	VDO_GDI_RBC2_AXI_13 = pMapCfg->rbc2axiMap[19];
	VDO_GDI_RBC2_AXI_14 = pMapCfg->rbc2axiMap[20];
	VDO_GDI_RBC2_AXI_15 = pMapCfg->rbc2axiMap[21];
	VDO_GDI_RBC2_AXI_16 = pMapCfg->rbc2axiMap[22];
	VDO_GDI_RBC2_AXI_17 = pMapCfg->rbc2axiMap[23];

	TOP_HAL_SetApbVDO(1, 9);

	VDO_GDI_RBC2_AXI_18 = pMapCfg->rbc2axiMap[24];
	VDO_GDI_RBC2_AXI_19 = pMapCfg->rbc2axiMap[25];
	VDO_GDI_RBC2_AXI_1A = pMapCfg->rbc2axiMap[26];
	VDO_GDI_RBC2_AXI_1B = pMapCfg->rbc2axiMap[27];
	VDO_GDI_RBC2_AXI_1C = pMapCfg->rbc2axiMap[28];
	VDO_GDI_RBC2_AXI_1D = pMapCfg->rbc2axiMap[29];
	VDO_GDI_RBC2_AXI_1E = pMapCfg->rbc2axiMap[30];
	VDO_GDI_RBC2_AXI_1F = pMapCfg->rbc2axiMap[31];

	VDO_GDI_TILEDBUF_BASE = (u32FrameBufStartAddr);// >> (EM_RAS+EM_BANK+EM_CAS+EM_WIDTH)) << (EM_RAS + EM_BANK + EM_CAS + EM_WIDTH);
	TOP_HAL_SetApbConfig(ui32PreAPBVal);
	// VDO GDI Setting /*}}}*/

	return 1;
}
