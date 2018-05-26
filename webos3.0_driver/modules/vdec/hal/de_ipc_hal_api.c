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
 *  DE IPC HAL.
 *  (Register Access Hardware Abstraction Layer )
 *
 *  author	youngki.lyu@lge.com
 *  version	1.0
 *  date		2011-05-03
 * @addtogroup h13_vdec
 * @{
 */

/*------------------------------------------------------------------------------
 *   Control Constants
 *----------------------------------------------------------------------------*/
#define LOG_NAME	vdec_hal_deipc

/*------------------------------------------------------------------------------
 *   File Inclusions
 *----------------------------------------------------------------------------*/
#include "hal/vdec_base.h"

#include "hal/de_ipc_hal_api.h"
#include "deipc_reg.h"
#include "hal/de_vdo_hal_api.h"

#ifdef	__linux__
#include <linux/kernel.h>
#include <linux/io.h>
#else
#include "../../platform/hal/include/mcu_hal_api.h"
#endif

#include "log.h"

/*------------------------------------------------------------------------------
 *   Constant Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Macro Definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Type Definitions
 *----------------------------------------------------------------------------*/
typedef enum
{
	CHIP_D13 = 0,
	CHIP_D14,
	CHIP_H13,
	CHIP_H14,
	CHIP_M14,
	CHIP_M14_PLUS,
	CHIP_H15,
	CHIP_VERSION_NONE
}CHIP_VERSION_E;
/*------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   External Variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   global Functions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   global Variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Static Variables
 *----------------------------------------------------------------------------*/
static volatile DEIPC_REG_T		*stpDE_IPC_Reg[DE_NUM_OF_CHANNEL];
static BOOLEAN		gbVdecVdo;		// TRUE: VDO on VDEC	FALSE: VDO on DE
static CHIP_VERSION_E eChipVersion = CHIP_VERSION_NONE;
static UINT8	hevc_data_bit = 0;
#ifdef SIMPLE_DE
static UINT32 _gu32YBase[DE_NUM_OF_CHANNEL];
static UINT32 _gu32CBase[DE_NUM_OF_CHANNEL];
static UINT32 _gu32Stride[DE_NUM_OF_CHANNEL];
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
void _deipc_reset(volatile DEIPC_REG_T *deipc_reg)
{
	deipc_reg->frame_idx = 0xFF;
#if !defined (CHIP_NAME_h15) && !defined (CHIP_NAME_m16)
	deipc_reg->y_frame_offset = 0;
	deipc_reg->c_frame_offset = 0;
#endif
	deipc_reg->picture_size = 0;
}

void DE_IPC_HAL_Init(void)
{
	UINT8		ui8DeIpcCh;
	UINT32		chip, rev;
	UINT32		u32RegBasePh;

	chip = vdec_chip_rev();
	rev = chip & 0xFF;

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

		if ( chip == VDEC_CHIP_REV(M14, C0) )
			eChipVersion = CHIP_M14_PLUS;
		if ( (chip == VDEC_CHIP_REV(H15, A0)) || (chip == VDEC_CHIP_REV(H15, A1)) || (chip == VDEC_CHIP_REV(H15, B0)) || (chip == VDEC_CHIP_REV(M16, A0)))
			eChipVersion = CHIP_H15;
	}

	for (ui8DeIpcCh = 0; ui8DeIpcCh < DE_NUM_OF_CHANNEL; ui8DeIpcCh++)
	{
		VDEC_GET_DE_IF_BASE(rev, ui8DeIpcCh, u32RegBasePh);
#ifdef __linux__
		stpDE_IPC_Reg[ui8DeIpcCh] = (volatile DEIPC_REG_T *)ioremap(
				u32RegBasePh, 0x40);
#else
		stpDE_IPC_Reg[ui8DeIpcCh] = (volatile DEIPC_REG_T *)MCU_HAL_TranslateAddrforMCU(
					u32RegBasePh, 0x40);
#endif
		_deipc_reset(stpDE_IPC_Reg[ui8DeIpcCh]);
	}
}

void DE_IPC_HAL_Resume(void)
{
	UINT8		ui8DeIpcCh;

	for (ui8DeIpcCh = 0; ui8DeIpcCh < DE_NUM_OF_CHANNEL; ui8DeIpcCh++)
		_deipc_reset(stpDE_IPC_Reg[ui8DeIpcCh]);
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
/*
#if defined (CHIP_NAME_h15)
inline BOOLEAN DE_IPC_HAL_SetNewFrame(UINT8 ui8DeIpcCh, S_DE_IPC_T *psDeIpc)
{
	volatile DEIPC_REG_T *pstDeIpcReg;
	const UINT32	ui32DownScaler = 0x10;
	UINT32 			ui16FrameRateRes;
	UINT32 			ui16FrameRateDiv;
	UINT16			ui16Width, ui16Height;
	UINT32			ui32H_Offset, ui32V_Offset;
	UINT32			u32FrameBaseY;
	UINT32			u32FrameBaseC;
	UINT32			u32FrameInfo;
	UINT32			u32DpbMapType;
	UINT32			u32SmartBalOffset;
	UINT8			u8Cnt;

	log_trace ("%d:%x %07x %07x %d %x %x %x %x %x %x(%dx%d)%x %x %x %d "
			"%x(%d/%d)%x %x %x %x\n",
			ui8DeIpcCh,
			psDeIpc->ui32Tiled_FrameBaseAddr,
			psDeIpc->ui32Y_FrameBaseAddr,
			psDeIpc->ui32C_FrameBaseAddr,
			psDeIpc->ui32Stride,
			psDeIpc->ui32FrameIdx&0xFF,
			psDeIpc->ui32ScalerFreeze,
			psDeIpc->ui32DisplayMode,
			psDeIpc->ui32FieldRepeat,
			psDeIpc->ui32VdecPause,
			psDeIpc->ui32AspectRatio,
			psDeIpc->ui16PicWidth,
			psDeIpc->ui16PicHeight,
			psDeIpc->ui32H_Offset,
			psDeIpc->ui32V_Offset,
			psDeIpc->ui32UpdateInfo,
			psDeIpc->ui32FramePackArrange,
			psDeIpc->ui32LR_Order,
			psDeIpc->ui16FrameRateRes,
			psDeIpc->ui16FrameRateDiv,
			psDeIpc->ui16ParW,
			psDeIpc->ui16ParH,
			psDeIpc->ui32PTS,
			psDeIpc->ui32DpbMapType
				);

	if (ui8DeIpcCh >= DE_NUM_OF_CHANNEL)
	{
		log_error ("[DE_IPC][Err] Channel Number(%d)\n", ui8DeIpcCh);
		return FALSE;
	}

	if(psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight > 1920*1088)
	{
		for(u8Cnt = ui8DeIpcCh;u8Cnt < (ui8DeIpcCh+2);u8Cnt++)
		{
			pstDeIpcReg = stpDE_IPC_Reg[u8Cnt];

			if (psDeIpc->ui32Tiled_FrameBaseAddr != 0)		// tiled map
			{
				// address translation
				u32FrameBaseY = psDeIpc->ui32Tiled_FrameBaseAddr +
					((psDeIpc->ui32Y_FrameBaseAddr & 0xFFFF) << 15);
				u32FrameBaseC = psDeIpc->ui32Tiled_FrameBaseAddr +
					((psDeIpc->ui32C_FrameBaseAddr & 0xFFFF) << 15);
			}
			else
			{
				u32FrameBaseY = psDeIpc->ui32Y_FrameBaseAddr;
				u32FrameBaseC = psDeIpc->ui32C_FrameBaseAddr;
			}

			u32FrameInfo = psDeIpc->ui32FrameIdx & 0xFF;
			if (!gbVdecVdo)
			{
#if defined (CHIP_NAME_h15)
				switch(psDeIpc->ui32DpbMapType)
				{
					case 0:
					case 5:
					case 6:
						u32DpbMapType = psDeIpc->ui32DpbMapType;
						break;
					case 1:
						//progressive Boda
						if (psDeIpc->ui32Stride > 1024)
							u32DpbMapType = 9;	//FHD
						else
							u32DpbMapType = 10;	//SD
						break;
					case 2:
						//interaced Boda
						if (psDeIpc->ui32Stride > 1024)
							u32DpbMapType = 7;	//FHD
						else
							u32DpbMapType = 8;	//SD
						break;
					case 3:
						//HEVC 8bit UD or FHD
						if (psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight > 1920*1088)
							u32DpbMapType = 1;	//UD
						else
							u32DpbMapType = 2;	//FHD
						break;
					case 4:
						//HEVC 10bit UD or FHD
						if (psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight > 1920*1088)
							u32DpbMapType = 1;	//UD
						else
							u32DpbMapType = 2;	//FHD

						break;
					case 0xFF:
						log_trace ("deipc:%d, no DPB map type\n", ui8DeIpcCh);
						u32DpbMapType = 0xFF;
						break;
					default:
						log_warning ("deipc:%d, unknown DPB map type %d\n",
								ui8DeIpcCh, psDeIpc->ui32DpbMapType);
						u32DpbMapType = 0xFF;
				}
				u32FrameInfo |= ((u32DpbMapType << 10) & 0x3C00);
				//smart balancing info register setting
				u32FrameInfo |= ((psDeIpc->bSmartBalancing << 15) & 0x8000);
				u32FrameInfo |= ((psDeIpc->ui8SmartBalancingMode << 16) & 0x30000);
				u32SmartBalOffset = ((psDeIpc->ui16SmBalanOffsetM1 << 16)
						|(psDeIpc->ui16SmBalanOffsetM0 & 0xFFFF));
				pstDeIpcReg->smart_bal_offset = u32SmartBalOffset; //0x028
#else
				u32FrameInfo |= (psDeIpc->ui32DpbMapType << 10) & 0xC00;
#endif
			}
			else
			{
				if (psDeIpc->ui32Tiled_FrameBaseAddr)
					u32FrameInfo |= 0xC00;
				else
					u32FrameInfo |= 0x800;
			}
			pstDeIpcReg->frame_idx = u32FrameInfo;

			ui16FrameRateRes = psDeIpc->ui16FrameRateRes;
			ui16FrameRateDiv = psDeIpc->ui16FrameRateDiv;
			while ((ui16FrameRateRes > 0xFFFF) || (ui16FrameRateDiv > 0xFFFF))
			{
				ui16FrameRateRes /= ui32DownScaler;
				ui16FrameRateDiv /= ui32DownScaler;
			}

			ui16Width	= psDeIpc->ui16PicWidth;
			ui16Height	= psDeIpc->ui16PicHeight;
			ui32H_Offset= psDeIpc->ui32H_Offset;
			ui32V_Offset= psDeIpc->ui32V_Offset;

			// Crop (Requested by DE)
			if (ui32H_Offset & 0xFFFF)		// right crop
			{
				ui16Width = psDeIpc->ui16PicWidth - (ui32H_Offset & 0xFFFF);
				ui32H_Offset &= ~0xFFFF;
			}

			if (ui32V_Offset & 0xFFFF)		// bottom crop
			{
				ui16Height = psDeIpc->ui16PicHeight - (ui32V_Offset & 0xFFFF);
				ui32V_Offset &= ~0xFFFF;
			}

#ifndef CHIP_NAME_m14
			if (psDeIpc->ui16PicHeight == 1088)		// Max height
				ui16Height = 1080;
#endif

			pstDeIpcReg->y_frame_base_address = u32FrameBaseY;
			pstDeIpcReg->c_frame_base_address = u32FrameBaseC;
#if defined (CHIP_NAME_h15)
			pstDeIpcReg->stride = ((psDeIpc->ui32TopBottomGap << 16)
					|(psDeIpc->ui32Stride & 0xFFFF));
#else
			pstDeIpcReg->stride = psDeIpc->ui32Stride;
#endif
			pstDeIpcReg->frame_rate =
				((UINT32)ui16FrameRateDiv << 16) | ((UINT32)ui16FrameRateRes & 0xFFFF);
			pstDeIpcReg->aspect_ratio = psDeIpc->ui32AspectRatio;
			pstDeIpcReg->picture_size = (ui16Width << 16) | ui16Height;
			pstDeIpcReg->h_offset = ui32H_Offset;
			pstDeIpcReg->v_offset = ui32V_Offset;
			pstDeIpcReg->frame_pack_arrange =
				(psDeIpc->ui32FramePackArrange & 0xFF) | ((psDeIpc->ui32LR_Order & 0x3) << 8);
			pstDeIpcReg->pixel_aspect_ratio =
				((UINT32)(psDeIpc->ui16ParW) << 16) | ((UINT32)(psDeIpc->ui16ParH) & 0xFFFF);
			pstDeIpcReg->pts_info = psDeIpc->ui32PTS;

		}
	}
	else
	{
		pstDeIpcReg = stpDE_IPC_Reg[ui8DeIpcCh];

		if (psDeIpc->ui32Tiled_FrameBaseAddr != 0)		// tiled map
		{
			// address translation
			u32FrameBaseY = psDeIpc->ui32Tiled_FrameBaseAddr +
				((psDeIpc->ui32Y_FrameBaseAddr & 0xFFFF) << 15);
			u32FrameBaseC = psDeIpc->ui32Tiled_FrameBaseAddr +
				((psDeIpc->ui32C_FrameBaseAddr & 0xFFFF) << 15);
		}
		else
		{
			u32FrameBaseY = psDeIpc->ui32Y_FrameBaseAddr;
			u32FrameBaseC = psDeIpc->ui32C_FrameBaseAddr;
		}

		u32FrameInfo = psDeIpc->ui32FrameIdx & 0xFF;
		if (!gbVdecVdo)
		{
#if defined (CHIP_NAME_h15)
			switch(psDeIpc->ui32DpbMapType)
			{
				case 0:
				case 5:
				case 6:
					u32DpbMapType = psDeIpc->ui32DpbMapType;
					break;
				case 1:
					//progressive Boda
					if (psDeIpc->ui32Stride > 1024)
						u32DpbMapType = 9;	//FHD
					else
						u32DpbMapType = 10;	//SD
					break;
				case 2:
					//interaced Boda
					if (psDeIpc->ui32Stride > 1024)
						u32DpbMapType = 7;	//FHD
					else
						u32DpbMapType = 8;	//SD
					break;
				case 3:
					//HEVC 8bit UD or FHD
					if (psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight > 1920*1088)
						u32DpbMapType = 1;	//UD
					else
						u32DpbMapType = 2;	//FHD
					break;
				case 4:
					//HEVC 10bit UD or FHD
					if (psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight > 1920*1088)
						u32DpbMapType = 1;	//UD
					else
						u32DpbMapType = 2;	//FHD

					break;
				case 0xFF:
					log_trace ("deipc:%d, no DPB map type\n", ui8DeIpcCh);
					u32DpbMapType = 0xFF;
					break;
				default:
					log_warning ("deipc:%d, unknown DPB map type %d\n",
							ui8DeIpcCh, psDeIpc->ui32DpbMapType);
					u32DpbMapType = 0xFF;
			}
			u32FrameInfo |= ((u32DpbMapType << 10) & 0x3C00);
			//smart balancing info register setting
			u32FrameInfo |= ((psDeIpc->bSmartBalancing << 15) & 0x8000);
			u32FrameInfo |= ((psDeIpc->ui8SmartBalancingMode << 16) & 0x30000);
			u32SmartBalOffset = ((psDeIpc->ui16SmBalanOffsetM1 << 16)
					|(psDeIpc->ui16SmBalanOffsetM0 & 0xFFFF));
			pstDeIpcReg->smart_bal_offset = u32SmartBalOffset; //0x028
#else
			u32FrameInfo |= (psDeIpc->ui32DpbMapType << 10) & 0xC00;
#endif
		}
		else
		{
			if (psDeIpc->ui32Tiled_FrameBaseAddr)
				u32FrameInfo |= 0xC00;
			else
				u32FrameInfo |= 0x800;
		}
		pstDeIpcReg->frame_idx = u32FrameInfo;

		ui16FrameRateRes = psDeIpc->ui16FrameRateRes;
		ui16FrameRateDiv = psDeIpc->ui16FrameRateDiv;
		while ((ui16FrameRateRes > 0xFFFF) || (ui16FrameRateDiv > 0xFFFF))
		{
			ui16FrameRateRes /= ui32DownScaler;
			ui16FrameRateDiv /= ui32DownScaler;
		}

		ui16Width	= psDeIpc->ui16PicWidth;
		ui16Height	= psDeIpc->ui16PicHeight;
		ui32H_Offset= psDeIpc->ui32H_Offset;
		ui32V_Offset= psDeIpc->ui32V_Offset;

		// Crop (Requested by DE)
		if (ui32H_Offset & 0xFFFF)		// right crop
		{
			ui16Width = psDeIpc->ui16PicWidth - (ui32H_Offset & 0xFFFF);
			ui32H_Offset &= ~0xFFFF;
		}

		if (ui32V_Offset & 0xFFFF)		// bottom crop
		{
			ui16Height = psDeIpc->ui16PicHeight - (ui32V_Offset & 0xFFFF);
			ui32V_Offset &= ~0xFFFF;
		}

#ifndef CHIP_NAME_m14
		if (psDeIpc->ui16PicHeight == 1088)		// Max height
			ui16Height = 1080;
#endif

		pstDeIpcReg->y_frame_base_address = u32FrameBaseY;
		pstDeIpcReg->c_frame_base_address = u32FrameBaseC;
#if defined (CHIP_NAME_h15)
		pstDeIpcReg->stride = ((psDeIpc->ui32TopBottomGap << 16)
				|(psDeIpc->ui32Stride & 0xFFFF));
#else
		pstDeIpcReg->stride = psDeIpc->ui32Stride;
#endif
		pstDeIpcReg->frame_rate =
			((UINT32)ui16FrameRateDiv << 16) | ((UINT32)ui16FrameRateRes & 0xFFFF);
		pstDeIpcReg->aspect_ratio = psDeIpc->ui32AspectRatio;
		pstDeIpcReg->picture_size = (ui16Width << 16) | ui16Height;
		pstDeIpcReg->h_offset = ui32H_Offset;
		pstDeIpcReg->v_offset = ui32V_Offset;
		pstDeIpcReg->frame_pack_arrange =
			(psDeIpc->ui32FramePackArrange & 0xFF) | ((psDeIpc->ui32LR_Order & 0x3) << 8);
		pstDeIpcReg->pixel_aspect_ratio =
			((UINT32)(psDeIpc->ui16ParW) << 16) | ((UINT32)(psDeIpc->ui16ParH) & 0xFFFF);
		pstDeIpcReg->pts_info = psDeIpc->ui32PTS;
	}
	return TRUE;
}

inline BOOLEAN DE_IPC_HAL_Update(UINT8 ui8DeIpcCh, S_DE_IPC_T *psDeIpc)
{
	volatile		DEIPC_REG_T *pstDeIpcReg;
	UINT32			ui32DisplayMode;
	UINT32			u32FrameBaseY;
	UINT32			u32FrameBaseC;
	UINT8			u8Cnt;

	log_user1 ("%d:%x %07x %07x %d %x %x %x %x %x %x(%dx%d)%x %x %x %d "
			"%x(%d/%d)%x %x %x\n",
			ui8DeIpcCh,
			psDeIpc->ui32Tiled_FrameBaseAddr,
			psDeIpc->ui32Y_FrameBaseAddr,
			psDeIpc->ui32C_FrameBaseAddr,
			psDeIpc->ui32Stride,
			psDeIpc->ui32FrameIdx&0xFF,
			psDeIpc->ui32ScalerFreeze,
			psDeIpc->ui32DisplayMode,
			psDeIpc->ui32FieldRepeat,
			psDeIpc->ui32VdecPause,
			psDeIpc->ui32AspectRatio,
			psDeIpc->ui16PicWidth,
			psDeIpc->ui16PicHeight,
			psDeIpc->ui32H_Offset,
			psDeIpc->ui32V_Offset,
			psDeIpc->ui32UpdateInfo,
			psDeIpc->ui32FramePackArrange,
			psDeIpc->ui32LR_Order,
			psDeIpc->ui16FrameRateRes,
			psDeIpc->ui16FrameRateDiv,
			psDeIpc->ui16ParW,
			psDeIpc->ui16ParH,
			psDeIpc->ui32PTS);

	if (ui8DeIpcCh >= DE_NUM_OF_CHANNEL)
	{
		log_error ("[DE_IPC][Err] Channel Number(%d)\n", ui8DeIpcCh);
		return FALSE;
	}

	if(psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight > 1920*1088)
	{
		for(u8Cnt = ui8DeIpcCh; u8Cnt<(ui8DeIpcCh + 2); u8Cnt++)
		{


			pstDeIpcReg = stpDE_IPC_Reg[u8Cnt];
			ui32DisplayMode = pstDeIpcReg->display_info & 0x3;

			switch (psDeIpc->ui32DisplayMode)
			{
				case DE_IPC_TOP_FIELD:
				case DE_IPC_BOTTOM_FIELD:
					if (ui32DisplayMode == psDeIpc->ui32DisplayMode && !psDeIpc->ui32VdecPause)
					{
						log_noti("deipc:%d, field repeat. frame index %d->%d, display info %d->%d\n",
								ui8DeIpcCh,
								pstDeIpcReg->frame_idx, psDeIpc->ui32FrameIdx,
								pstDeIpcReg->display_info, psDeIpc->ui32DisplayMode);
					}

					ui32DisplayMode = psDeIpc->ui32DisplayMode;
					break;

				default:
					log_warning ("deipc:%d, unknown display mode %d\n",
							ui8DeIpcCh, psDeIpc->ui32DisplayMode);
				case DE_IPC_FRAME:
					ui32DisplayMode = 3;
					break;
			}
#if defined (CHIP_NAME_h15)
			// g1 frame baseAddr recalculation
			if(psDeIpc->ui32DpbMapType == 5 && ((psDeIpc->ui32DisplayMode == DE_IPC_TOP_FIELD)
						||(psDeIpc->ui32DisplayMode == DE_IPC_BOTTOM_FIELD)))
			{
				if (psDeIpc->ui32DisplayMode == DE_IPC_TOP_FIELD)
				{
					u32FrameBaseY = psDeIpc->ui32Y_FrameBaseAddr;
					u32FrameBaseC = psDeIpc->ui32C_FrameBaseAddr +
						(psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight);
				}
				else if(psDeIpc->ui32DisplayMode == DE_IPC_BOTTOM_FIELD)
				{
					u32FrameBaseY = psDeIpc->ui32Y_FrameBaseAddr +
						(psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight*2);
					u32FrameBaseC = psDeIpc->ui32C_FrameBaseAddr +
						(psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight*3);
				}
				pstDeIpcReg->y_frame_base_address = psDeIpc->ui32Tiled_FrameBaseAddr +
					((u32FrameBaseY & 0xFFFF) << 15);
				pstDeIpcReg->c_frame_base_address = psDeIpc->ui32Tiled_FrameBaseAddr +
					((u32FrameBaseC & 0xFFFF) << 15);
			}
#endif
			pstDeIpcReg->display_info =
				((psDeIpc->ui32VdecPause & 0x1) << 3) |
				((psDeIpc->ui32ColorFormat & 0x3) << 4) |
				ui32DisplayMode;

			if (!psDeIpc->ui32VdecPause)
				pstDeIpcReg->update = psDeIpc->ui32UpdateInfo | (1<<1);
			else
				pstDeIpcReg->update = psDeIpc->ui32UpdateInfo;

		}
	}
	else
	{

		pstDeIpcReg = stpDE_IPC_Reg[ui8DeIpcCh];
		ui32DisplayMode = pstDeIpcReg->display_info & 0x3;

		switch (psDeIpc->ui32DisplayMode)
		{
			case DE_IPC_TOP_FIELD:
			case DE_IPC_BOTTOM_FIELD:
				if (ui32DisplayMode == psDeIpc->ui32DisplayMode && !psDeIpc->ui32VdecPause)
				{
					log_noti("deipc:%d, field repeat. frame index %d->%d, display info %d->%d\n",
							ui8DeIpcCh,
							pstDeIpcReg->frame_idx, psDeIpc->ui32FrameIdx,
							pstDeIpcReg->display_info, psDeIpc->ui32DisplayMode);
				}

				ui32DisplayMode = psDeIpc->ui32DisplayMode;
				break;

			default:
				log_warning ("deipc:%d, unknown display mode %d\n",
						ui8DeIpcCh, psDeIpc->ui32DisplayMode);
			case DE_IPC_FRAME:
				ui32DisplayMode = 3;
				break;
		}
#if defined (CHIP_NAME_h15)
		// g1 frame baseAddr recalculation
		if(psDeIpc->ui32DpbMapType == 5 && ((psDeIpc->ui32DisplayMode == DE_IPC_TOP_FIELD)
					||(psDeIpc->ui32DisplayMode == DE_IPC_BOTTOM_FIELD)))
		{
			if (psDeIpc->ui32DisplayMode == DE_IPC_TOP_FIELD)
			{
				u32FrameBaseY = psDeIpc->ui32Y_FrameBaseAddr;
				u32FrameBaseC = psDeIpc->ui32C_FrameBaseAddr +
					(psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight);
			}
			else if(psDeIpc->ui32DisplayMode == DE_IPC_BOTTOM_FIELD)
			{
				u32FrameBaseY = psDeIpc->ui32Y_FrameBaseAddr +
					(psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight*2);
				u32FrameBaseC = psDeIpc->ui32C_FrameBaseAddr +
					(psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight*3);
			}
			pstDeIpcReg->y_frame_base_address = psDeIpc->ui32Tiled_FrameBaseAddr +
				((u32FrameBaseY & 0xFFFF) << 15);
			pstDeIpcReg->c_frame_base_address = psDeIpc->ui32Tiled_FrameBaseAddr +
				((u32FrameBaseC & 0xFFFF) << 15);
		}
#endif
		pstDeIpcReg->display_info =
			((psDeIpc->ui32VdecPause & 0x1) << 3) |
			((psDeIpc->ui32ColorFormat & 0x3) << 4) |
			ui32DisplayMode;

		if (!psDeIpc->ui32VdecPause)
			pstDeIpcReg->update = psDeIpc->ui32UpdateInfo | (1<<1);
		else
			pstDeIpcReg->update = psDeIpc->ui32UpdateInfo;
	}
	return TRUE;
}
#else
*/
inline BOOLEAN DE_IPC_HAL_SetNewFrame(UINT8 ui8DeIpcCh, S_DE_IPC_T *psDeIpc)
{
	volatile DEIPC_REG_T *pstDeIpcReg;
	const UINT32	ui32DownScaler = 0x10;
	UINT32 			ui16FrameRateRes;
	UINT32 			ui16FrameRateDiv;
	UINT16			ui16Width, ui16Height;
	UINT32			ui32H_Offset, ui32V_Offset;
	UINT32			u32FrameBaseY;
	UINT32			u32FrameBaseC;
	UINT32			u32FrameInfo;
	UINT32			u32DpbMapType;
	UINT32			u32SmartBalOffset;
	UINT32 			u32Residual;
	UINT32			u32CropInfo;

	log_trace ("%d:%x %07x %07x %d %x %x %x %x %x %x(%dx%d)%x %x %x %d "
			"%x(%d/%d)%x %x %x %x\n",
			ui8DeIpcCh,
			psDeIpc->ui32Tiled_FrameBaseAddr,
			psDeIpc->ui32Y_FrameBaseAddr,
			psDeIpc->ui32C_FrameBaseAddr,
			psDeIpc->ui32Stride,
			psDeIpc->ui32FrameIdx&0xFF,
			psDeIpc->ui32ScalerFreeze,
			psDeIpc->ui32DisplayMode,
			psDeIpc->ui32FieldRepeat,
			psDeIpc->ui32VdecPause,
			psDeIpc->ui32AspectRatio,
			psDeIpc->ui16PicWidth,
			psDeIpc->ui16PicHeight,
			psDeIpc->ui32H_Offset,
			psDeIpc->ui32V_Offset,
			psDeIpc->ui32UpdateInfo,
			psDeIpc->ui32FramePackArrange,
			psDeIpc->ui32LR_Order,
			psDeIpc->ui16FrameRateRes,
			psDeIpc->ui16FrameRateDiv,
			psDeIpc->ui16ParW,
			psDeIpc->ui16ParH,
			psDeIpc->ui32PTS,
			psDeIpc->ui32DpbMapType
				);

	if (ui8DeIpcCh >= DE_NUM_OF_CHANNEL)
	{
		log_error ("[DE_IPC][Err] Channel Number(%d)\n", ui8DeIpcCh);
		return FALSE;
	}

	pstDeIpcReg = stpDE_IPC_Reg[ui8DeIpcCh];

	if (psDeIpc->ui32Tiled_FrameBaseAddr != 0)		// tiled map
	{
		// address translation
		u32FrameBaseY = psDeIpc->ui32Tiled_FrameBaseAddr +
			((psDeIpc->ui32Y_FrameBaseAddr & 0xFFFF) << 15);
		u32FrameBaseC = psDeIpc->ui32Tiled_FrameBaseAddr +
			((psDeIpc->ui32C_FrameBaseAddr & 0xFFFF) << 15);
	}
	else
	{
		u32FrameBaseY = psDeIpc->ui32Y_FrameBaseAddr;
		u32FrameBaseC = psDeIpc->ui32C_FrameBaseAddr;
	}

	u32FrameInfo = psDeIpc->ui32FrameIdx & 0xFF;
	if (!gbVdecVdo)
	{
		if (eChipVersion == CHIP_H15 || eChipVersion == CHIP_M14_PLUS)
		{
			switch(psDeIpc->ui32DpbMapType)
			{
				case 0:
				case 5:
				case 6:
					u32DpbMapType = psDeIpc->ui32DpbMapType;
					break;
				case 1:
					//progressive Boda
					if (psDeIpc->ui32Stride > 1024)
						u32DpbMapType = 9;	//FHD
					else
						u32DpbMapType = 10;	//SD
					break;
				case 2:
					//interaced Boda
					if (psDeIpc->ui32Stride > 1024)
						u32DpbMapType = 7;	//FHD
					else
						u32DpbMapType = 8;	//SD
					break;
				case 3:
					//HEVC 8bit UD or FHD
					if (psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight > 1920*1088)
						u32DpbMapType = 1;	//UD
					else
						u32DpbMapType = 2;	//FHD
					hevc_data_bit = 0;
					break;
				case 4:
					//HEVC 10bit UD or FHD
					if (psDeIpc->ui16PicWidth*psDeIpc->ui16PicHeight > 1920*1088)
						u32DpbMapType = 1;	//UD
					else
						u32DpbMapType = 2;	//FHD
					hevc_data_bit = 1;
					break;
				case 0xFF:
					log_trace ("deipc:%d, no DPB map type\n", ui8DeIpcCh);
					u32DpbMapType = 0xFF;
					break;
				default:
					log_warning ("deipc:%d, unknown DPB map type %d\n",
							ui8DeIpcCh, psDeIpc->ui32DpbMapType);
					u32DpbMapType = 0xFF;
			}
			u32FrameInfo |= ((u32DpbMapType << 10) & 0x3C00);
			//smart balancing info register setting
#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
			u32FrameInfo |= ((psDeIpc->bSmartBalancing << 15) & 0x8000);
			u32FrameInfo |= ((psDeIpc->ui8SmartBalancingMode << 16) & 0x30000);
			u32SmartBalOffset = ((psDeIpc->ui16SmBalanOffsetM1 << 16)
					|(psDeIpc->ui16SmBalanOffsetM0 & 0xFFFF));
			pstDeIpcReg->smart_bal_offset = u32SmartBalOffset; //0x028
#endif
		}
		else
			u32FrameInfo |= (psDeIpc->ui32DpbMapType << 10) & 0xC00;
	}
	else
	{
		if (psDeIpc->ui32Tiled_FrameBaseAddr)
			u32FrameInfo |= 0xC00;
		else
			u32FrameInfo |= 0x800;
	}
	pstDeIpcReg->frame_idx = u32FrameInfo;

	ui16FrameRateRes = psDeIpc->ui16FrameRateRes;
	ui16FrameRateDiv = psDeIpc->ui16FrameRateDiv;
	while ((ui16FrameRateRes > 0xFFFF) || (ui16FrameRateDiv > 0xFFFF))
	{
		ui16FrameRateRes /= ui32DownScaler;
		ui16FrameRateDiv /= ui32DownScaler;
	}

	ui16Width	= psDeIpc->ui16PicWidth;
	ui16Height	= psDeIpc->ui16PicHeight;
	ui32H_Offset= psDeIpc->ui32H_Offset;
	ui32V_Offset= psDeIpc->ui32V_Offset;

	// Crop (Requested by DE)
	if (ui32H_Offset & 0xFFFF)		// right crop
	{
		ui16Width = psDeIpc->ui16PicWidth - (ui32H_Offset & 0xFFFF);
		ui32H_Offset &= ~0xFFFF;
	}

	if (ui32V_Offset & 0xFFFF)		// bottom crop
	{
		ui16Height = psDeIpc->ui16PicHeight - (ui32V_Offset & 0xFFFF);
		ui32V_Offset &= ~0xFFFF;
	}

#if defined (CHIP_NAME_h13)
	u32Residual = ui16Width%4;
	if (u32Residual)
	{
		u32CropInfo = 4-u32Residual;
		ui16Width += u32CropInfo;
		ui32H_Offset |= (u32CropInfo&0xFFFF);
	}
#endif

#ifndef CHIP_NAME_m14
	if (psDeIpc->ui16PicHeight == 1088)		// Max height
		ui16Height = 1080;
#endif

	pstDeIpcReg->y_frame_base_address = u32FrameBaseY;
	pstDeIpcReg->c_frame_base_address = u32FrameBaseC;
#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
	pstDeIpcReg->stride = ((psDeIpc->ui32TopBottomGap << 16)
			|(psDeIpc->ui32Stride & 0xFFFF));
#else
	pstDeIpcReg->stride = psDeIpc->ui32Stride;
#endif
	pstDeIpcReg->frame_rate =
		((UINT32)ui16FrameRateDiv << 16) | ((UINT32)ui16FrameRateRes & 0xFFFF);
	pstDeIpcReg->aspect_ratio = psDeIpc->ui32AspectRatio;
	pstDeIpcReg->picture_size = (ui16Width << 16) | ui16Height;
	pstDeIpcReg->h_offset = ui32H_Offset;
	pstDeIpcReg->v_offset = ui32V_Offset;
	pstDeIpcReg->frame_pack_arrange =
		(psDeIpc->ui32FramePackArrange & 0xFF) | ((psDeIpc->ui32LR_Order & 0x3) << 8);
	pstDeIpcReg->pixel_aspect_ratio =
		((UINT32)(psDeIpc->ui16ParW) << 16) | ((UINT32)(psDeIpc->ui16ParH) & 0xFFFF);
	pstDeIpcReg->pts_info = psDeIpc->ui32PTS;
	return TRUE;
}

inline BOOLEAN DE_IPC_HAL_Update(UINT8 ui8DeIpcCh, S_DE_IPC_T *psDeIpc)
{
	volatile		DEIPC_REG_T *pstDeIpcReg;
	UINT32			ui32DisplayMode;
	UINT32			u32FrameBaseY;
	UINT32			u32FrameBaseC;

	log_user1 ("%d:%x %07x %07x %d %x %x %x %x %x %x(%dx%d)%x %x %x %d "
			"%x(%d/%d)%x %x %x\n",
			ui8DeIpcCh,
			psDeIpc->ui32Tiled_FrameBaseAddr,
			psDeIpc->ui32Y_FrameBaseAddr,
			psDeIpc->ui32C_FrameBaseAddr,
			psDeIpc->ui32Stride,
			psDeIpc->ui32FrameIdx&0xFF,
			psDeIpc->ui32ScalerFreeze,
			psDeIpc->ui32DisplayMode,
			psDeIpc->ui32FieldRepeat,
			psDeIpc->ui32VdecPause,
			psDeIpc->ui32AspectRatio,
			psDeIpc->ui16PicWidth,
			psDeIpc->ui16PicHeight,
			psDeIpc->ui32H_Offset,
			psDeIpc->ui32V_Offset,
			psDeIpc->ui32UpdateInfo,
			psDeIpc->ui32FramePackArrange,
			psDeIpc->ui32LR_Order,
			psDeIpc->ui16FrameRateRes,
			psDeIpc->ui16FrameRateDiv,
			psDeIpc->ui16ParW,
			psDeIpc->ui16ParH,
			psDeIpc->ui32PTS);

	if (ui8DeIpcCh >= DE_NUM_OF_CHANNEL)
	{
		log_error ("[DE_IPC][Err] Channel Number(%d)\n", ui8DeIpcCh);
		return FALSE;
	}

	pstDeIpcReg = stpDE_IPC_Reg[ui8DeIpcCh];
	ui32DisplayMode = pstDeIpcReg->display_info & 0x3;

	switch (psDeIpc->ui32DisplayMode)
	{
		case DE_IPC_TOP_FIELD:
		case DE_IPC_BOTTOM_FIELD:
			if (ui32DisplayMode == psDeIpc->ui32DisplayMode && !psDeIpc->ui32VdecPause)
			{
				log_noti("deipc:%d, field repeat. frame index %d->%d, display info %d->%d\n",
						ui8DeIpcCh,
						pstDeIpcReg->frame_idx, psDeIpc->ui32FrameIdx,
						pstDeIpcReg->display_info, psDeIpc->ui32DisplayMode);
			}

			ui32DisplayMode = psDeIpc->ui32DisplayMode;
			break;

		default:
			log_warning ("deipc:%d, unknown display mode %d\n",
					ui8DeIpcCh, psDeIpc->ui32DisplayMode);
			// fallthrough
		case DE_IPC_FRAME:
			ui32DisplayMode = 3;
			break;
	}
#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
	// g1 frame baseAddr recalculation
	if(psDeIpc->ui32DpbMapType == 5 && ((psDeIpc->ui32DisplayMode == DE_IPC_TOP_FIELD)
				||(psDeIpc->ui32DisplayMode == DE_IPC_BOTTOM_FIELD)))
	{
		if (psDeIpc->ui32DisplayMode == DE_IPC_TOP_FIELD)
		{
			u32FrameBaseY = psDeIpc->ui32Y_FrameBaseAddr;
			u32FrameBaseC = psDeIpc->ui32C_FrameBaseAddr;
		}
		else if(psDeIpc->ui32DisplayMode == DE_IPC_BOTTOM_FIELD)
		{
			u32FrameBaseY = psDeIpc->ui32Y_FrameBaseAddr +
				psDeIpc->ui16PicWidth*(psDeIpc->ui16PicHeight/2);
			u32FrameBaseC = psDeIpc->ui32C_FrameBaseAddr +
				psDeIpc->ui16PicWidth*(psDeIpc->ui16PicHeight/4);
		}
		pstDeIpcReg->y_frame_base_address = u32FrameBaseY;
		pstDeIpcReg->c_frame_base_address = u32FrameBaseC;
	}
#endif
	if (eChipVersion == CHIP_M14_PLUS || eChipVersion == CHIP_H15)
	{
		pstDeIpcReg->display_info =
			((hevc_data_bit & 0x3) << 6) |
			((psDeIpc->ui32VdecPause & 0x1) << 3) |
			((psDeIpc->ui32ColorFormat & 0x3) << 4) |
			ui32DisplayMode;
	}
	else
	{
		pstDeIpcReg->display_info =
			((psDeIpc->ui32VdecPause & 0x1) << 3) |
			((psDeIpc->ui32ColorFormat & 0x3) << 4) |
			ui32DisplayMode;
	}
	if (!psDeIpc->ui32VdecPause)
		pstDeIpcReg->update = psDeIpc->ui32UpdateInfo | (1<<1);
	else
		pstDeIpcReg->update = psDeIpc->ui32UpdateInfo;
	return TRUE;
}
//#endif
/** @} */

