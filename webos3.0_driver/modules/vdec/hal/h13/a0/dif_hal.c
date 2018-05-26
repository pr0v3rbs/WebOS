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
 * main driver implementation for de device.
 *
 * author     @lge.com
 * version    1.0
 * date       20.04.09
 * note       Additional information.
 *
 * @addtogroup lx_vdec
 * @{
 */

/*------------------------------------------------------------------------------
 *   Control Constants
 *----------------------------------------------------------------------------*/
#define LOG_NAME	dif_hal
#define LOG_LEVEL	log_level_error

/*------------------------------------------------------------------------------
 *   File Inclusions
 *----------------------------------------------------------------------------*/
#ifndef __linux
#include <string.h>
#endif

#include "hal/de_ipc_hal_api.h"
#include "hal/de_vdo_hal_api.h"
#include "hal/vsync_hal_api.h"
#include "hal/dif_hal.h"
#include "log.h"

#if defined(CHIP_NAME_d13)
// VCP
#include "../dxipc/dxipc_drv.h"
#include "../dxipc/uvd_reg.h"
#endif

/*------------------------------------------------------------------------------
 *   Constant Definitions
 *----------------------------------------------------------------------------*/
#if defined(CHIP_NAME_h15) || defined(CHIP_NAME_m16)
#define DIF_NUMOF_CH			6
#else
#define DIF_NUMOF_CH			4
#endif

#define DEIF_NUMOF_VDO_CH		2
#define DE_Q_FRAME_NUM			2


/*------------------------------------------------------------------------------
 *   Macro Definitions
 *----------------------------------------------------------------------------*/
//#define assert(e)
#define assert(e)		\
	do{ if (!e) printk("assert! %s:%d\n",__FILE__, __LINE__); } while(0)

/*------------------------------------------------------------------------------
 *   Type Definitions
 *----------------------------------------------------------------------------*/
typedef enum
{
	DEIF_HAL_IDLE_CH,
	DEIF_HAL_ACTIVE_CH,
	DEIF_HAL_MIRROR_CH
} DEIF_CH_STATE_E;

typedef struct
{
	BOOLEAN			valid;
	S_DE_IPC_T		ipc;
	UINT16			scan_type;
	UINT16			disp_period;
} DEIF_FRM_T;

typedef struct
{
	S_DE_IPC_T		sDeIpc;
	DEIF_FRM_T		frm[2];
	DEIF_FRM_T		*cur_frm;
	DEIF_FRM_T		*next_frm;

	UINT8			ch;
	UINT8			u8VsyncCh;
	enum
	{
		DE_IF_CLOSE,
		DE_IF_OPEN
	} eState;

	UINT32			ui32FieldCount;

	BOOLEAN			bForcedPush;
	UINT8			u8NoneForcedPushCnt;
	UINT8			u8nFrameRepeat;

#if defined(CHIP_NAME_d13)
	UINT32			u32PicWidth;
	UINT32			u32PicHeight;

	UINT32			u32FrameRateRes;
	UINT32			u32FrameRateDiv;
#endif
} DEIF_HAL_T;

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
static UINT8		au8PortUse[DIF_NUMOF_CH];
static UINT8		g_mirror_out = 0xFF;
static UINT8		g_mirror_src = 0xFF;
static DEIF_HAL_T	deifs[DIF_NUMOF_CH];

/******************************************************************************
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
*******************************************************************************
*/
UINT32 DIF_HAL_GetChannelNum(void)
{
	return DIF_NUMOF_CH;
}

void DIF_HAL_Reset(UINT8 ch)
{
	S_DE_IPC_T		*psDeIpc = NULL;
	DEIF_HAL_T		*deif;

	deif = &deifs[ch];

	deif->cur_frm = &deif->frm[0];
	deif->next_frm = &deif->frm[1];

	deif->cur_frm->valid = FALSE;
	deif->next_frm->valid = FALSE;

	psDeIpc = (S_DE_IPC_T *)&deif->sDeIpc;

	memset((void *)psDeIpc, 0x0, sizeof(S_DE_IPC_T));
	psDeIpc->ui32FrameIdx = 0xFF;
	psDeIpc->ui32DisplayMode = 0x3;
	psDeIpc->ui32LR_Order = 2;	// None
	psDeIpc->ui32DpbMapType = 0xFF;
	psDeIpc->ui32FramePackArrange = (UINT32)-2;

	deif->ch = ch;

	deif->bForcedPush = TRUE;
	deif->u8NoneForcedPushCnt = 0;

	deif->ui32FieldCount = 0xFFFFFFFF;

#if defined(CHIP_NAME_d13)
	deif->u32PicWidth = 0;
	deif->u32PicHeight = 0;

	deif->u32FrameRateRes = 0;
	deif->u32FrameRateDiv = 0;
#endif

#if !defined(CHIP_NAME_d13)
	if (ch < DE_IPC_NUM_OF_CHANNEL)
	{
		DE_IPC_HAL_SetNewFrame(ch, psDeIpc);
		DE_IPC_HAL_Update(ch, psDeIpc);
	}
#endif
}

void DIF_HAL_Init(void)
{
	UINT32	i;

	DE_IPC_HAL_Init();
	DE_VDO_HAL_Init();

	for (i=0; i<DIF_NUMOF_CH; i++)
	{
		au8PortUse[i] = DEIF_HAL_IDLE_CH;
		DIF_HAL_Reset(i);
	}
}

#if defined(CHIP_NAME_d13)
inline void DE_IF_SetVcp(UINT32 ch, DISPQ_FRM_T *frm_info)
{
	UINT32			u32Temp;
	BOOLEAN			bChanged = FALSE;
	DEIF_HAL_T		*dif;

	dif = &deifs[ch];

	if (dif->u32PicWidth != frm_info->ui32PicWidth ||
			dif->u32PicHeight != frm_info->ui32PicHeight)
	{
		log_noti("Change resol %d x %x -> %d x %d\n",
			dif->u32PicWidth, dif->u32PicHeight,
			frm_info->ui32PicWidth, frm_info->ui32PicHeight);

		dif->u32PicWidth = frm_info->ui32PicWidth;
		dif->u32PicHeight = frm_info->ui32PicHeight;

		bChanged = TRUE;
	}

	if (dif->u32FrameRateRes != frm_info->ui32FrameRateRes ||
			dif->u32FrameRateDiv != frm_info->ui32FrameRateDiv)
	{
		log_noti("Change rate %d/%d -> %d/%d\n",
				dif->u32FrameRateRes, dif->u32FrameRateDiv,
				frm_info->ui32FrameRateRes, frm_info->ui32FrameRateDiv);

		dif->u32FrameRateRes = frm_info->ui32FrameRateRes;
		dif->u32FrameRateDiv = frm_info->ui32FrameRateDiv;

		bChanged = TRUE;
	}

	if (bChanged)
	{
		UINT32 u32Residual;

		u32Temp = (frm_info->ui32PicWidth << 16) |
			(frm_info->ui32PicHeight & 0xFFFF);
		DX_IPC_HAL_SetReg(DX_REG_RESOLUTION, u32Temp);

		u32Temp = frm_info->ui32FrameRateRes/frm_info->ui32FrameRateDiv;
		u32Temp *= 1000;
		u32Residual = frm_info->ui32FrameRateRes%frm_info->ui32FrameRateDiv;
		u32Temp += (u32Residual*1000)/frm_info->ui32FrameRateDiv;

		if (u32Temp <= 23976 ) u32Temp = 1;			// 23.976 Hz
		else if (u32Temp <= 24000 ) u32Temp = 2;	// 24 Hz
		else if (u32Temp <= 25000 ) u32Temp = 3;	// 25 Hz
		else if (u32Temp <= 29970 ) u32Temp = 4;	// 29.97 Hz
		else if (u32Temp <= 30000 ) u32Temp = 5;	// 30 Hz
		else if (u32Temp <= 50000 ) u32Temp = 6;	// 50 Hz
		else if (u32Temp <= 59940 ) u32Temp = 7;	// 59.94 Hz
		else if (u32Temp <= 60000 ) u32Temp = 8;	// 60 Hz
		else
		{
			log_error("not suported frame rate for VCP, Res %d, Div%d\n",
				frm_info->ui32FrameRateRes, frm_info->ui32FrameRateDiv);
			u32Temp = 5;
		}

		DX_IPC_HAL_SetReg(DX_REG_DECODER_STATUS, 1);	//frame decoding done
		DX_IPC_HAL_SetReg(DX_REG_FRAME_RATE, u32Temp);
		DX_IPC_Send_to_MCU0(DX_REG_FRAME_RATE);
	}
}
#endif

void DIF_HAL_SetMirrorOut(UINT8 src, UINT8 mirror_out)
{
	log_noti("DIF_HAL %d: set mirror port %d - > %d\n",
			src, g_mirror_out, mirror_out);

	if (g_mirror_out == mirror_out)
		return;

	if (src >= DIF_NUMOF_CH && src != 0xFF)
	{
		log_error("dif %d: wrong src ch num\n", src);
		return;
	}

	if (mirror_out >= DIF_NUMOF_CH && mirror_out != 0xFF)
	{
		log_error("dif %d: wrong out ch num\n", mirror_out);
		return;
	}

	if (g_mirror_out != 0xFF)
	{
		// already have mirror out port
		if (au8PortUse[g_mirror_out] == DEIF_HAL_MIRROR_CH)
			au8PortUse[g_mirror_out] = DEIF_HAL_IDLE_CH;
	}

	g_mirror_out = mirror_out;
	g_mirror_src = src;

	if (g_mirror_out != 0xFF && au8PortUse[g_mirror_out] == DEIF_HAL_IDLE_CH)
	{
		au8PortUse[g_mirror_out] = DEIF_HAL_MIRROR_CH;
		if (g_mirror_src != 0xFF)
			DE_VDO_HAL_SetVSyncSel(g_mirror_out, g_mirror_src);
	}
}

void DIF_HAL_SelVsync(UINT8 ch, UINT32 ui32VSyncSrc)
{
	/*
	if (ch >= DE_IF_NUM)
	{
		log_error("[DE_IF%d][Err] wrong channel! %s\n", u8DeIfCh, __FUNCTION__ );
		return;
	}*/

#ifdef ARM_USING_MMCU
	_mpipe_sel_vsync(u8DeIfCh, ui32VSyncSrc);
#endif

#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
	VSync_HAL_SelVsync(ui32VSyncSrc, ch);
#else
	DE_VDO_HAL_SetVSyncSel(ch, ui32VSyncSrc);
#endif
}

void DIF_HAL_Open(UINT8 ch)
{
	if (au8PortUse[ch] == DEIF_HAL_ACTIVE_CH)
		log_warning("DEIF_HAL %d: already used channel\n", ch);

	au8PortUse[ch] = DEIF_HAL_ACTIVE_CH;
	//DE_VDO_HAL_SetVSyncSel(ch, ch);
}

void DIF_HAL_Close(UINT8 ch)
{
	if (ch == g_mirror_out)
	{
		au8PortUse[ch] = DEIF_HAL_MIRROR_CH;
		DE_VDO_HAL_SetVSyncSel(g_mirror_out, g_mirror_src);
	}
	else
		au8PortUse[ch] = DEIF_HAL_IDLE_CH;

 	DIF_HAL_Reset(ch);
}

void DIF_HAL_SetNewFrame(UINT8 ch, DISPQ_FRM_T *frm_info)
{
	S_DE_IPC_T		*ipc;
	
	ipc = &deifs[ch].sDeIpc;

	ipc->ui32Tiled_FrameBaseAddr = frm_info->ui32Tiled_FrameBaseAddr;
	ipc->ui32DpbMapType = frm_info->ui32DpbMapMode;
	ipc->ui32Y_FrameBaseAddr = frm_info->ui32Y_FrameBaseAddr;
	ipc->ui32C_FrameBaseAddr = frm_info->ui32C_FrameBaseAddr;
	ipc->ui32Stride = frm_info->ui32Stride;
	ipc->ui32ColorFormat = frm_info->FrameFormat;

	ipc->ui32FrameIdx = frm_info->ui32FrameIdx;
	ipc->ui16FrameRateRes = frm_info->ui32FrameRateRes;
	ipc->ui16FrameRateDiv = frm_info->ui32FrameRateDiv;
	ipc->ui32AspectRatio = frm_info->ui32AspectRatio;
	ipc->ui32H_Offset = frm_info->ui32H_Offset;
	ipc->ui32V_Offset = frm_info->ui32V_Offset;
	ipc->ui16PicWidth = frm_info->ui32PicWidth;
	ipc->ui16PicHeight = frm_info->ui32PicHeight;
	ipc->ui32FramePackArrange = frm_info->i32FramePackArrange;
	ipc->ui32LR_Order = frm_info->ui32LR_Order;
	ipc->ui16ParW = frm_info->ui16ParW;
	ipc->ui16ParH = frm_info->ui16ParH;
	ipc->ui32PTS = frm_info->ui32PTS;
	ipc->ui32UpdateInfo = 0x1;
#if defined (CHIP_NAME_h15) || defined(CHIP_NAME_m16)
	//For H15
	ipc->ui32TopBottomGap = frm_info->ui32TopBottomGap;
	ipc->bSmartBalancing = frm_info->bSmartBalancing;
	ipc->ui8SmartBalancingMode = frm_info->ui8SmartBalancingMode;
	ipc->ui16SmBalanOffsetM0 = frm_info->ui16SmBalanOffsetM0;
	ipc->ui16SmBalanOffsetM1 = frm_info->ui16SmBalanOffsetM1;
#endif

#if defined(CHIP_NAME_d13)
	DE_IF_SetVcp(ch, frm_info);
#else
	DE_IPC_HAL_SetNewFrame(ch, ipc);
	if (ch == g_mirror_src && g_mirror_out != 0xFF &&
			au8PortUse[g_mirror_out] == DEIF_HAL_MIRROR_CH)
		DE_IPC_HAL_SetNewFrame(g_mirror_out, ipc);
#endif

}

BOOLEAN DIF_HAL_UpdateDisplay(UINT8 ch, DIF_FRM_T *frame)
{
	S_DE_IPC_T		*ipc;

	ipc = &deifs[ch].sDeIpc;

	ipc->ui32UpdateInfo = frame->update_info;
	ipc->ui32VdecPause = frame->freeze;
	ipc->ui32DisplayMode = frame->scan_type;

#if !defined(CHIP_NAME_d13)
	DE_IPC_HAL_Update(ch, ipc);
	if (ch == g_mirror_src && g_mirror_out != 0xFF &&
			au8PortUse[g_mirror_out] == DEIF_HAL_MIRROR_CH)
		DE_IPC_HAL_Update(g_mirror_out, ipc);
#endif

	if (ch<DEIF_NUMOF_VDO_CH)
	{
		DE_VDO_HAL_Update(ch, ipc, 0);
		if (ch == g_mirror_src && g_mirror_out != 0xFF &&
				au8PortUse[g_mirror_out] == DEIF_HAL_MIRROR_CH)
			DE_VDO_HAL_Update(g_mirror_out, ipc, 0);
	}

	ipc->ui32UpdateInfo = 0;

	//deif->ui32FieldCount++;

	return TRUE;
}

void DIF_HAL_Suspend(void)
{
	DE_VDO_HAL_Suspend();
	//DE_IPC_HAL_Suspend();
}

void DIF_HAL_Resume(void)
{
	DE_IPC_HAL_Resume();
	DE_VDO_HAL_Resume();
}
