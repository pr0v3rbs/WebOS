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

/*------------------------------------------------------------------------------
	Control Constants
------------------------------------------------------------------------------*/
#define LOG_NAME	vdec_hal_vdo
#define LOG_LEVEL	log_level_error

/*------------------------------------------------------------------------------
	File Inclusions
------------------------------------------------------------------------------*/
#ifdef __linux__
#include <linux/io.h>
#else	//__linux__
#include "../../platform/hal/include/mcu_hal_api.h"
#include "../../mmcu_lock.h"
#endif	//__linux__
#include "hal/vdec_base.h"

#include "hal/de_vdo_hal_api.h"
#include "hal/top_hal_api.h"

#include "log.h"

#include "hal/vdo_reg.h"
#if defined(CHIP_NAME_d13)
#include "hal/d13/vdogdi_reg.h"
#include "hal/d13/vdo_tilemap.h"
#else
#include "hal/vdogdi_reg.h"
#include "hal/vdo_tilemap.h"
#endif

/*------------------------------------------------------------------------------
	Constant Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Macro Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Type Definitions
------------------------------------------------------------------------------*/
typedef struct
{
	BOOLEAN			bAutoVsync;
	UINT32			ui32CurDpbMapType;
	UINT32			ui32CurDpbTiledBase;
	UINT32			ui32CurDpbStride;
} VDO_CH_T;

/*------------------------------------------------------------------------------
	External Function Prototype Declarations
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	External Variables
------------------------------------------------------------------------------*/
#ifndef	__linux__
extern volatile MMCU_LOCK_T *pstLockVdo;
#endif	//__linux__

/*------------------------------------------------------------------------------
	global Functions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	global Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	Static Function Prototypes Declarations
------------------------------------------------------------------------------*/
static void _SetTiledMapType(UINT8 ui8VdoCh, UINT32 u32FrameBufStartAddr, TILED_MAP_T mapType, UINT32 ui32Stride);

/*------------------------------------------------------------------------------
	Static Variables
------------------------------------------------------------------------------*/
static volatile UINT32 stpVdoReg = 0;//NULL;
static volatile UINT32 stpVdoGdiReg = 0;//NULL;
static VDO_CH_T	stVdoCh[DE_VDO_NUM_OF_CHANNEL];
static BOOLEAN	gbVdecVdo = FALSE;		// TRUE: VDO on VDEC	FALSE: VDO on DE
static BOOLEAN	gbApbSet = TRUE;		// TRUE: APB Set Need


/*========================================================================================
	Implementation Group
========================================================================================*/

#ifndef __linux__
UINT32 TOP_HAL_SetApbVDO(UINT8 ui8GdiSelection, UINT8 ui8Offset)
{
	UINT32 u32RegVal;
	UINT32 ret;

	MLOCK_Lock(pstLockVdo);

	u32RegVal = *((volatile UINT32*)0xF0001408);
	ret = u32RegVal >> 29;

	u32RegVal = u32RegVal & (~(0x7<<29));
	u32RegVal = u32RegVal | (ui8Offset<<29);

	*((volatile UINT32*)0xF0001408) = u32RegVal;

	MLOCK_Unlock(pstLockVdo);
	return ret;
}
#endif
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

	if (ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL)
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
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
	if (ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL)
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
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
	if (ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL)
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
		return;
	}

	//VDO_CH_CHROMA_FORMAT(ui8VdoCh) = ;
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

	if (ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2))
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
		return;
	}
	if ((ui32ScanModeType > 3) || (ui32ScanModeType == 0))
	{
		log_error("[DE_VDO%d][Err] Scan Mode: %d, %s\n", ui8VdoCh, ui32ScanModeType, __FUNCTION__);
		return;
	}

	ui32ScanMode = VDO_CH_SCAN_MODE(ui8VdoCh);

/*
	scan mode (4th bit)
	0: interlaced scan mode
	1: progressive scan mode
*/

	if (ui32ScanModeType == 3) // progressive
	{
		ui32ScanMode |= (1 << 4);
	}
	else if (ui8VdoCh < 2) // interlaced
	{
		ui32ScanMode &= ~(1 << 4);
		if (ui32ScanModeType == 1) // Top Field First
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
void DE_VDO_HAL_EnableMifWaitReqb(UINT8 ui8VdoCh, UINT8 ui32ReqbWaitCycle)
{
	VDO_CH_WAIT_REQB(ui8VdoCh) = ((1 << 28) | (ui32ReqbWaitCycle & 0xFF));
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
void DE_VDO_HAL_DisableMifWaitReqb(UINT8 ui8VdoCh)
{
	VDO_CH_WAIT_REQB(ui8VdoCh) &= ~(1 << 28);
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
void DE_VDO_HAL_SetLineWait(UINT8 ui8VdoCh, UINT32 ui32LineWaitCycle)
{
	UINT32 ui32VdoConf;
	if (ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL)
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
		return;
	}

	ui32VdoConf = VDO_CH_VDO_CONF(ui8VdoCh) & 0xFFFF;
	ui32VdoConf |= ((ui32LineWaitCycle & 0xFFFF) << 16);

	VDO_CH_VDO_CONF(ui8VdoCh) = ui32VdoConf;
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
	UINT32		ui32SyncCmd;
	
	if (!gbVdecVdo)
		return;

	if (ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2))
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
		return;
	}

	log_noti("[VDO%d] sel vsync %d\n", ui8VdoCh, ui32VSyncSrc);

/*
	vsync source sel (2bits)
	It should be set to one of the four sync source in VDEC.
*/
	ui32SyncCmd = VDO_CH_VSYNC_CMD(ui8VdoCh);

	ui32SyncCmd &= ~(0x3 << 16);
	ui32SyncCmd |= ((ui32VSyncSrc & 0x3) << 16);

	VDO_CH_VSYNC_CMD(ui8VdoCh) = ui32SyncCmd;
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
	if (ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2))
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
		return;
	}

/*
	vsync mode (3~0 bits)
	0: MCU generation using VSYNC_GEN register
	1: Automatic generation using VBP_NUM and VSYNC_NUM registers
*/
	if (bAutoGen == TRUE)
		VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 8);
	else
		VDO_CH_VSYNC_CMD(ui8VdoCh) &= ~(1 << 8);
}

void DE_VDO_HAL_SetVActvTerm(UINT8 ui8VdoCh, UINT32 u32Term)
{
	if (ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2))
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
		return;
	}

	VDO_CH_VSYNC_VACT_TERM(ui8VdoCh) = u32Term;
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
	if (ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2))
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
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
	if (bAutoGen == TRUE)
	{
		VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 9);
		VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 10);
	}
	else
	{
		VDO_CH_VSYNC_CMD(ui8VdoCh) &= ~(1 << 9);
		VDO_CH_VSYNC_CMD(ui8VdoCh) &= ~(1 << 10);
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
void DE_VDO_HAL_SetVSyncGen(UINT8 ui8VdoCh)
{
/*
	vsync generation
	It is automatically cleared when set to '1'
*/
	VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 0);
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
void DE_VDO_HAL_SetVActvGen(UINT8 ui8VdoCh)
{
/*
	vsync generation
	It is automatically cleared when set to '1'
*/
	VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 4);
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
	UINT32 i;
	UINT32		chip, rev;

	chip = vdec_chip_rev();
	rev = chip & 0xFF;

#if defined(CHIP_NAME_d13)
	gbVdecVdo = TRUE;
	gbApbSet = FALSE;
#else
	if (!VDEC_COMP_CHIP(chip,VDEC_CHIP_H14))	// if H14
		gbVdecVdo = TRUE;
	else
	{
		if (vdec_chip() == VDEC_CHIP_M14 &&
				chip >= VDEC_CHIP_REV(M14, B0))
			gbVdecVdo = FALSE;
		else if (vdec_chip() >= VDEC_CHIP_H15)
			gbVdecVdo = FALSE;
		else
			gbVdecVdo = TRUE;
	}
#endif

#ifdef __linux__
	stpVdoReg		= (UINT32)ioremap(VDEC_VDO_BASE, 0x100);
	stpVdoGdiReg	= (UINT32)ioremap(VDEC_VDO_GDI_BASE, 0x200);
#else
	stpVdoReg		= (UINT32)MCU_HAL_TranslateAddrforMCU(VDEC_VDO_BASE, 0x100);
	stpVdoGdiReg	= (UINT32)MCU_HAL_TranslateAddrforMCU(VDEC_VDO_GDI_BASE, 0x200);
#endif

	log_noti("VDO Init\n");

	if (!gbVdecVdo)
		return;

	// Vsync/Vactive Setting
	for(i=0;i<4;i++)
	{
		VDO_CH_VSYNC_CMD(i) &= ~((0x7<<8) | (0x3<<16));
#if defined(CHIP_NAME_d13)
		VDO_CH_VSYNC_CMD(i) |= ((0x7<<8) | ((i%2)<<16) );
#else
		VDO_CH_VSYNC_CMD(i) |= ((0x5<<8) | ((i%2)<<16));
#endif
		//VDO_CH_VSYNC_VACT_TERM(i&1) |= 1<<31;
		VDO_CH_VSYNC_NUM(i) = 0x00000898;//0x00000780;

		if (i<2)
		{
#if !defined(CHIP_NAME_d13)
			VDO_CH_WAIT_REQB(i)  = 0x10000010;
			VDO_CH_FLUSH_CONF(i) = 0x30000040;
			VDO_CH_VDO_CONF(i) = 0x00409000;
#else
			VDO_CH_VDO_CONF(i) = 0x00408000;
#endif
		}
	}

	stVdoCh[0].bAutoVsync = TRUE;
	stVdoCh[1].bAutoVsync = TRUE;

	stVdoCh[0].ui32CurDpbMapType = 0xFF;
	stVdoCh[1].ui32CurDpbMapType = 0xFF;
	stVdoCh[0].ui32CurDpbTiledBase = 0xFFFFFFFF;
	stVdoCh[1].ui32CurDpbTiledBase = 0xFFFFFFFF;
	stVdoCh[0].ui32CurDpbStride = 0xFFFFFFFF;
	stVdoCh[1].ui32CurDpbStride = 0xFFFFFFFF;

	//VDO_CH_VBP_NUM(ui8VdoCh)        = 0x00000780;
	//VDO_CH_VFP_NUM(ui8VdoCh)        = 0x00000780;
	//VDO_CH_VSYNC_NUM(ui8VdoCh)      = 0x00000898;//0x00000780;
}

void DE_VDO_HAL_Suspend(void)
{
	log_warning ("FIXME: not implemented\n");
}

void DE_VDO_HAL_Resume(void)
{
	UINT32 i;

	if (!gbVdecVdo)
		return;

	log_warning ("FIXME: not implemented\n");

	// Vsync/Vactive Setting
	for(i=0;i<4;i++)
	{
		VDO_CH_VSYNC_CMD(i) &= ~((0x7<<8) | (0x3<<16));
		VDO_CH_VSYNC_CMD(i) |= ((0x5<<8) | ((i%2)<<16));
		//VDO_CH_VSYNC_VACT_TERM(i&1) |= 1<<31;
		VDO_CH_VSYNC_NUM(i) = 0x00000898;//0x00000780;
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
void	DE_VDO_HAL_InitFrameBuffer(UINT8 ui8VdoCh)
{
	if (ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL)
	{
		log_error("[VDEC][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
		return;
	}

	// TILED MAP
	//if (pFrameBuf->bInterlaced)
	//	_SetTiledMapType(ui8VdoCh, VDEC_TILED_BUF_BASE, TILED_MAP_MIXED_V);
	//else
	//	_SetTiledMapType(ui8VdoCh, VDEC_TILED_BUF_BASE, TILED_MAP_FRAME_V);
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
void DE_VDO_HAL_Update(UINT8 ui8VdoCh, S_DE_IPC_T *pDisBuf, UINT32 ui32SyncField)
{
	UINT32 ui32Size;
	UINT32 ui32CropWidth;
	UINT32 ui32CropHeight;
	UINT32 ui32Residual;
	UINT32 ui32Crop;
	UINT32 ui32PreAPBVal = 0;
	UINT32 u32Cnt=0;
	UINT32 u32Temp;

	if (!gbVdecVdo)
		return;

	log_user1("VdoCh %d: Y 0x%X  C 0x%X BY 0x%X  BC 0x%X DispMode %d, Field %d\n",
			ui8VdoCh, pDisBuf->ui32Y_FrameBaseAddr, pDisBuf->ui32C_FrameBaseAddr,
			//pDisBuf->ui32Y_FrameBaseAddrBot, pDisBuf->ui32C_FrameBaseAddrBot,
			pDisBuf->ui32Y_FrameBaseAddr, pDisBuf->ui32C_FrameBaseAddr,
			pDisBuf->ui32DisplayMode, ui32SyncField);

	if (ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL)
	{
		log_error("[VDEC][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__);
		return;
	}

	if (pDisBuf->ui32DpbMapType==0)		// ==Linear Map?
	{
		VDO_CH_VDO_CMD(ui8VdoCh) = VDO_CMD_PIC_INIT;
		return;
	}

	// Init for first received frame
	if ((stVdoCh[ui8VdoCh].ui32CurDpbMapType != pDisBuf->ui32DpbMapType) ||
		(stVdoCh[ui8VdoCh].ui32CurDpbTiledBase != pDisBuf->ui32Tiled_FrameBaseAddr) ||
		(stVdoCh[ui8VdoCh].ui32CurDpbStride != pDisBuf->ui32Stride))
	{
		log_noti("VDO:%d: set dpb map type %d->%d stride %d\n", ui8VdoCh,
				stVdoCh[ui8VdoCh].ui32CurDpbMapType, pDisBuf->ui32DpbMapType,
				pDisBuf->ui32Stride);

		stVdoCh[ui8VdoCh].ui32CurDpbMapType = pDisBuf->ui32DpbMapType;
		stVdoCh[ui8VdoCh].ui32CurDpbTiledBase = pDisBuf->ui32Tiled_FrameBaseAddr;
		stVdoCh[ui8VdoCh].ui32CurDpbStride = pDisBuf->ui32Stride;

		switch (pDisBuf->ui32DpbMapType)
		{
			case 1: // Frame Mmap
				_SetTiledMapType(ui8VdoCh, pDisBuf->ui32Tiled_FrameBaseAddr,
						TILED_MAP_FRAME_V, pDisBuf->ui32Stride);
				break;
			case 2: // Mixed Map
				_SetTiledMapType(ui8VdoCh, pDisBuf->ui32Tiled_FrameBaseAddr,
						TILED_MAP_MIXED_V, pDisBuf->ui32Stride);
				break;
#if defined(CHIP_NAME_d13)
			case 3: // HEVC Map
				_SetTiledMapType(ui8VdoCh, pDisBuf->ui32Tiled_FrameBaseAddr,
						TILED_MAP_FRAME_V_HEVC, pDisBuf->ui32Stride);
				break;
#endif
			case 0:	// Linear
				break;

			default :
				break;
		}
	}

	if (VDO_CH_MIF_STATUS(ui8VdoCh) || VDO_CH_RDC_STATUS(ui8VdoCh))
	{
		static UINT32 u32DbgCnt=0;
		if (!((u32DbgCnt++) % 100 ))
		{
			log_noti("[DE_VDO%d][Err] Status : MIF(0x%08X/0x%08X), "
					"RDC(0x%08X/0x%08X)\n",
					ui8VdoCh, VDO_CH_MIF_STATUS(ui8VdoCh),
					VDO_CH_MIF_LINE_CNT(ui8VdoCh),
					VDO_CH_RDC_STATUS(ui8VdoCh), VDO_CH_RDC_LINE_CNT(ui8VdoCh));
			log_noti("[DE_VDO #%d][DBG]   StallCnt Luma %d Croma %d\n",
					ui8VdoCh, VDO_CH_LUMA_STALL_CNT(ui8VdoCh),
					VDO_CH_CHMA_STALL_CNT(ui8VdoCh));
		}
	}

	//RC0_VDO_RST_COUNT = 0x00400040			//??
	VDO_CH_VDO_CMD(ui8VdoCh) = VDO_CMD_PIC_INIT;
#if !defined(CHIP_NAME_d13)
	VDO_CH_VDO_CMD(ui8VdoCh) = VDO_CMD_FLUSH_RUN;
#endif

	while ((VDO_CH_MIF_STATUS(ui8VdoCh) & 0x1f) != 0x0)
	{
		if ((++u32Cnt) > 0x100000)
		{
			log_error("vdo #%d infinite loop: MIF Status 0x%X\n",
					ui8VdoCh, VDO_CH_MIF_STATUS(ui8VdoCh));
			goto GOTO_END;
		}
	}

	// Crop (Requested by DE)
	ui32CropWidth = (pDisBuf->ui16PicWidth - (pDisBuf->ui32H_Offset & 0xFFFF));
	ui32CropHeight = (pDisBuf->ui16PicHeight - (pDisBuf->ui32V_Offset & 0xFFFF));

#if defined (CHIP_NAME_h13)
	//if crop Width is not 4-multiple
	ui32Residual = ui32CropWidth%4;
	if (ui32Residual != 0)
	{
		ui32Crop = 4-ui32Residual;
		ui32CropWidth += ui32Crop;
	} 
#endif
	ui32Size = ((ui32CropWidth&0xFFFF) << 16) | (ui32CropHeight & 0xFFFF);

#ifndef CHIP_NAME_m14
	if (pDisBuf->ui16PicHeight == 1088)
		ui32Size = (ui32Size & 0xFFFF0000) | 1080;
#endif

	VDO_CH_PIC_SIZE(ui8VdoCh)       = ui32Size;
	VDO_CH_BUF_IDX(ui8VdoCh)        = ui8VdoCh;
#if !defined(CHIP_NAME_d13)
	VDO_CH_CHROMA_FORMAT(ui8VdoCh)  = VDO_CHROMA_420;
#else
	VDO_CH_CHROMA_FORMAT(ui8VdoCh)  = 0x00040004;
#endif

	if (pDisBuf->ui32DisplayMode == 3)
	{
		/* Progressive*/
		u32Temp = VDO_SCAN_PROG;
	}
	else
	{
		/* Interaced */
		u32Temp = VDO_SCAN_INTRLC;

		if (pDisBuf->ui32DisplayMode == 2)
			u32Temp	|= VDO_SCAN_BOT;
	}
	VDO_CH_SCAN_MODE(ui8VdoCh) = u32Temp;

	VDO_GDI_PIC_INIT_HOST  = 0x1;
	VDO_GDI_PIC_INIT_HOST  = 0x0;

	if (gbApbSet)
		ui32PreAPBVal = TOP_HAL_SetApbVDO(1, 2);

#if !defined(CHIP_NAME_d13)
	u32Temp = VDO_GDI_MAP_MIXED;
#else
	switch (pDisBuf->ui32DpbMapType)
	{
		case TILED_MAP_LINEAR:
			u32Temp = 0x00000000;
			break;
		case TILED_MAP_FRAME_V_HEVC:
		case TILED_MAP_FRAME_V:
			u32Temp = 0x00100000;
			break;
		case TILED_MAP_MIXED_V:
			u32Temp = 0x00400000;
			break;
		default:
			u32Temp = 0x00100000;
			log_error("invalid map type %d\n", pDisBuf->ui32DpbMapType);
			break;
	}
#endif

	VDO_GDI_INFO_CONTROL(ui8VdoCh)  =
		u32Temp |
		VDO_GDI_COLOR_MODE_420 |
		VDO_GDI_CBCR_INTLV |
		pDisBuf->ui32Stride;

	VDO_GDI_INFO_PIC_SIZE(ui8VdoCh) = ui32Size;
	VDO_GDI_INFO_BASE_Y(ui8VdoCh)   = pDisBuf->ui32Y_FrameBaseAddr;
	VDO_GDI_INFO_BASE_CB(ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddr;
	VDO_GDI_INFO_BASE_CR(ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddr;

#if defined(CHIP_NAME_d13)
	//VDO_GDI_INFO_BASE_Y_BOT(ui8VdoCh)   = pDisBuf->ui32Y_FrameBaseAddrBot;
	//VDO_GDI_INFO_BASE_CB_BOT(ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddrBot;
	//VDO_GDI_INFO_BASE_CR_BOT(ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddrBot;
	VDO_GDI_INFO_BASE_Y_BOT(ui8VdoCh)   = pDisBuf->ui32Y_FrameBaseAddr;
	VDO_GDI_INFO_BASE_CB_BOT(ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddr;
	VDO_GDI_INFO_BASE_CR_BOT(ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddr;
#endif

	if (gbApbSet)
		TOP_HAL_SetApbVDO(1, ui32PreAPBVal);

	// VSYNC manual generation
	if (!stVdoCh[ui8VdoCh].bAutoVsync)
		VDO_CH_VSYNC_CMD(ui8VdoCh) |= 0x01;

	// Run Command
	VDO_CH_VDO_CMD(ui8VdoCh) = VDO_CMD_PIC_RUN;
GOTO_END:
	return;
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
}

void DE_VDO_HAL_Register(UINT8 u8VdoCh, UINT8 u8DeIfCh, BOOLEAN bTiled)
{
	if (!gbVdecVdo)
		return;

	if (u8VdoCh >= DE_VDO_NUM_OF_CHANNEL)
	{
		log_error("wrong vdo channel num %d\n", u8VdoCh);
		return;
	}

	log_noti("VDO Register %d deif %d tiled %d\n", u8VdoCh, u8DeIfCh, bTiled);
}

void	DE_VDO_HAL_SetInterlaced(UINT8 ui8VdoCh, BOOLEAN bInterlaced)
{
	if (!gbVdecVdo)
		return;

	if (ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL)
	{
		log_error("wrong vdo channel num %d\n", ui8VdoCh);
		return;
	}

	if (bInterlaced == TRUE)
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

static void _SetTiledMapType(UINT8 ui8VdoCh, UINT32 u32FrameBufStartAddr,
		TILED_MAP_T mapType, UINT32 ui32Stride)
{
	const TILED_MAP_CONFIG_T*	pstMapCfg;
	volatile UINT32 *pVdoGdiReg;
	UINT32 			ui32PreAPBVal = 0;
	UINT32	i;

	if (mapType >= TILED_MAP_MAX)
	{
		log_error("VDO:%d wrong tiled map type %d\n", ui8VdoCh, mapType);
		goto GOTO_END;
	}

	if (ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL)
	{
		log_error("wrong vdo channel %d\n", ui8VdoCh);
		goto GOTO_END;
	}

	if (gbApbSet)
		ui32PreAPBVal = TOP_HAL_SetApbVDO(1, 4+ui8VdoCh);

#if defined(CHIP_NAME_d13)
	pstMapCfg = &_gastTiledMapConfig[(UINT32)mapType][(ui32Stride > 2048)? 0 :
													  (ui32Stride > 1024)? 1 :
													  (ui32Stride > 512)? 2 : 3];
	pVdoGdiReg = &(VDO_GDI_CH0_XY2_AXI_LUM_00);

	for (i=0; i<0x40; i++, pVdoGdiReg++)
		*pVdoGdiReg = (UINT32)pstMapCfg->u8Data[i];

	VDO_GDI_CH0_XY2_AXI_CONFIG = pstMapCfg->u32Config;
#else
	pstMapCfg = &_gastTiledMapConfig[(UINT32)mapType][(ui32Stride <= 1024)? 0 : 1];

	// VDO GDI Setting
	pVdoGdiReg = &(VDO_GDI_XY2_CAS_0);
	for (i=0; i<36; i++, pVdoGdiReg++)
	{
		*pVdoGdiReg = pstMapCfg->u32Data[i];
	}
	//xy2rbc_config
	VDO_GDI_XY2_RBC_CONFIG = pstMapCfg->u32Config;

	pVdoGdiReg = &(VDO_GDI_RBC2_AXI_0);
	for (i=0; i<32; i++, pVdoGdiReg++)
	{
		*pVdoGdiReg = pstMapCfg->u32Data2[i];
	}
	VDO_GDI_TILEDBUF_BASE = (u32FrameBufStartAddr);
#endif

	if (gbApbSet)
		TOP_HAL_SetApbVDO(1, ui32PreAPBVal);

GOTO_END:
	return;
}

