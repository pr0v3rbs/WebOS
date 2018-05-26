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
 * de device will teach you how to make device driver with new platform.
 *
 * author     youngki.lyu@lge.com
 * version    1.0
 * date       2011.04.09
 * note       Additional information.
 *
 * @addtogroup lg115x_vdec
 * @{
 */


/*------------------------------------------------------------------------------
 *   Control Constants
 *----------------------------------------------------------------------------*/
#define LOG_NAME	vdec_de_if
#define LOG_LEVEL	log_level_error

/*------------------------------------------------------------------------------
 *   File Inclusions
 *----------------------------------------------------------------------------*/
#include "de_if_drv.h"
#include "vsync_drv.h"

#ifdef __linux__
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/seq_file.h>
#include <asm/string.h>	// memset
#else
#include <string.h>
#endif

#include "../hal/dif_hal.h"
#include "../hal/top_hal_api.h"
#include "../hal/vdec_base.h"
#include "mcu_if.h"
#include "vdisp_cfg.h"

#include "log.h"

/*------------------------------------------------------------------------------
 *   Constant Definitions
 *----------------------------------------------------------------------------*/
#define	DE_IF_NUM_OF_CMD_Q					0x10
#define DE_Q_FRAME_NUM						(2+3)
/*------------------------------------------------------------------------------
 *   Macro Definitions
 *----------------------------------------------------------------------------*/
#ifndef	__linux__
#define DISP_Q_NumActive(u8DeIfCh)	0
#endif	//__linux__
/*------------------------------------------------------------------------------
 *   Type Definitions
 *----------------------------------------------------------------------------*/
typedef struct
{
	DE_IF_DST_E		eTargetDe;

	DIF_FRM_T		*cur_frm;
	DIF_FRM_T		*next_frm;
	DIF_FRM_T		*clr_frm;

	DIF_FRM_T		frames[3];

	UINT8			u8DeIfCh;
	UINT8			u8VsyncCh;
	enum
	{
		DE_IF_CLOSE,
		DE_IF_OPEN
	} eState;

	BOOLEAN			bFreezing;

#ifdef CHIP_NAME_h13
	UINT32			u8PauseCnt;
	UINT32			u8PrevPauseCnt;
	UINT32			u8PauseSeekStatRepeat;
#endif

	UINT32			ui32FieldCount;

	BOOLEAN			bForcedPush;
	UINT8			u8NoneForcedPushCnt;
	SINT8			repeat_cnt;

	void            *priv;

	// for Debug
	UINT32			ui32LogTick;
	UINT32			ui32UpdateFrameCount;
#ifdef	__linux__
	spinlock_t		stLock;
#endif	//__linux__
} S_DE_IF_DB_T;

typedef S_DE_IF_DB_T	DE_IF_T;

/*------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *----------------------------------------------------------------------------*/
// TODO : add callback fn registeration & get rid of reverse reference!!
extern void vdisp_disp_done(void *priv, DISPQ_FRM_T *pstClrFrm);
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
static DE_IF_T		gstDeIf[DEIF_NUMOF_CH];
static DE_IF_DST_E		geMirrorPort, geMirrorSrc;
static int			gDbgNDisp=-1;

module_param_named (n_frm_disp, gDbgNDisp, int, 0644);
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
void _DE_IF_Reset(UINT8 u8DeIfCh)
{
	DE_IF_T			*pstDeIf;
	DISPQ_FRM_T		*frm_info;

	pstDeIf = &gstDeIf[u8DeIfCh];

	pstDeIf->u8DeIfCh = u8DeIfCh;
	pstDeIf->u8VsyncCh = 0xFF;

	pstDeIf->bFreezing = FALSE;
	pstDeIf->bForcedPush = TRUE;
	pstDeIf->repeat_cnt = 0;
	pstDeIf->u8NoneForcedPushCnt = 0;
#ifdef CHIP_NAME_h13
	pstDeIf->u8PauseCnt = 0;
	pstDeIf->u8PrevPauseCnt = 0;
	pstDeIf->u8PauseSeekStatRepeat = 0;
#endif
	pstDeIf->ui32FieldCount = 0xFFFFFFFF;
	pstDeIf->ui32LogTick = 0xFFFFFFF0;
	pstDeIf->ui32UpdateFrameCount = 0;

	pstDeIf->cur_frm = &pstDeIf->frames[0];
	pstDeIf->next_frm = &pstDeIf->frames[1];
	pstDeIf->clr_frm = &pstDeIf->frames[2];

	pstDeIf->next_frm->valid = FALSE;
	pstDeIf->cur_frm->valid = FALSE;
	pstDeIf->clr_frm->valid = FALSE;

	memset((void *)pstDeIf->cur_frm, 0x0, sizeof(DIF_FRM_T));
	pstDeIf->cur_frm->scan_type = 0x3;
	frm_info = &pstDeIf->cur_frm->frm_info;
	frm_info->ui32FrameIdx = 0xFF;
	frm_info->ui32LR_Order = 2;	// None
	frm_info->ui32DpbMapMode = 0xFF;
	frm_info->i32FramePackArrange = (UINT32)-2;

	DIF_HAL_Reset(u8DeIfCh);
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

void _deif_set_disp_state(DE_IF_T *pstDeIf, MCUIF_DISP_STATE *pstDispStt)
{
	log_user3("DEIF :disp stt %d/%d\n", pstDispStt->u32FieldCnt,
			pstDispStt->u32DispPeriod);

	pstDeIf->next_frm->valid = pstDispStt->bNext;
	pstDeIf->cur_frm->valid = pstDispStt->bCur;
	pstDeIf->cur_frm->frm_info.ui32DisplayPeriod = pstDispStt->u32DispPeriod;
	pstDeIf->ui32FieldCount = pstDispStt->u32FieldCnt;
}

#ifdef	__linux__
void _deif_callback(UINT8 u8DeIfCh, MMCU_PIPE_CMD eType, UINT8* pu8Data)
{
	DEIF_CB_TYPE_E		eCbType;

	switch (eType)
	{
	case MPIPE_CLR_FRAME:
		vdisp_disp_done(gstDeIf[u8DeIfCh].priv, (DISPQ_FRM_T*)pu8Data);
		break;
	case MPIPE_DISP_STATE:
		_deif_set_disp_state(&gstDeIf[u8DeIfCh], (MCUIF_DISP_STATE*)pu8Data);
		break;
	case MPIPE_VSYNC_STATE:
	{
		//VSYNC_INFO_T	*pstVsyncStt = (VSYNC_INFO_T*)pu8Data;
		//eCbType = DEIF_CB_REQ_NEXT;
		//VSYNC_SetPreVsync();
		//__deif_prevsync_cb((UINT8)pstVsyncStt->u32VsyncCh, 0);
		//VVSYNC_ISR();
		//VDISP_VVsyncCallback(u8VsyncCh, u8nVdisp, au8VdispCh, u8FieldInfo);
		break;
	}
	default:
		eCbType = DEIF_CB_NONE;
		break;
	}

	return;
}
#endif	//__linux__

void DE_IF_Init(void)
{
	UINT8	u8DeIfCh;

	log_noti("[DE_IF][DBG] DE I/F Init: 0x%X, %s\n",
			(UINT32)gstDeIf, __FUNCTION__ );

	DIF_HAL_Init();

#ifdef ARM_USING_MMCU
	MCU_IF_Init();
#endif

	for (u8DeIfCh = 0; u8DeIfCh < DEIF_NUMOF_CH; u8DeIfCh++)
	{
		gstDeIf[u8DeIfCh].eState = DE_IF_CLOSE;
		gstDeIf[u8DeIfCh].eTargetDe = DE_IF_DST_NONE;
#ifdef	__linux__
		gstDeIf[u8DeIfCh].stLock = __SPIN_LOCK_UNLOCKED(gstDeIf[u8DeIfCh].stLock);
#endif	//__linux__

		_DE_IF_Reset(u8DeIfCh);
	}
}

void DE_IF_SetPrivateData(UINT8 u8DeIfCh, void *priv)
{
	gstDeIf[u8DeIfCh].priv = priv;

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
BOOLEAN DE_IF_Open(UINT8 u8DeIfCh, DE_IF_DST_E eOutPort)
{
	log_noti("[DE_IF%d][DBG] %s \n", u8DeIfCh, __FUNCTION__ );

	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("[DE_IF%d] wrong channel! %s\n", u8DeIfCh, __FUNCTION__);
		return FALSE;
	}

	if (gstDeIf[u8DeIfCh].eState == DE_IF_OPEN)
		log_warning("[DE_IF%d] Already opened channel, %s\n",
				u8DeIfCh, __FUNCTION__ );

	gstDeIf[u8DeIfCh].eState = DE_IF_OPEN;

	_DE_IF_Reset(u8DeIfCh);

#ifdef ARM_USING_MMCU
	_mpipe_send_init_ch(u8DeIfCh);
#endif

	DE_IF_SetTargetDe(u8DeIfCh, (UINT8)eOutPort);

	return TRUE;
}

void DE_IF_SelVsync(UINT8 u8DeIfCh, UINT32 ui32VSyncSrc)
{
	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("[DE_IF%d][Err] wrong channel! %s\n", u8DeIfCh, __FUNCTION__ );
		return;
	}

#ifdef ARM_USING_MMCU
	_mpipe_sel_vsync(u8DeIfCh, ui32VSyncSrc);
#endif

	if (gstDeIf[u8DeIfCh].eTargetDe != DE_IF_DST_NONE)
		DIF_HAL_SelVsync((UINT8)gstDeIf[u8DeIfCh].eTargetDe, ui32VSyncSrc);
}

DE_IF_DST_E DE_IF_GetTargetDe(UINT8 u8DeIfCh)
{
	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("[DE_IF%d][Err] wrong channel! %s\n", u8DeIfCh, __FUNCTION__ );
		return 0xFF;
	}

	return gstDeIf[u8DeIfCh].eTargetDe;
}

DE_IF_DST_E DE_IF_SetTargetDe(UINT8 u8DeIfCh, DE_IF_DST_E eTargetDe)
{
	DE_IF_DST_E		ePreTargetDe;

	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("[DE_IF%d][Err] wrong channel! %s\n", u8DeIfCh, __FUNCTION__ );
		return 0xFF;
	}

	if (eTargetDe >= DE_IF_DST_MAX && eTargetDe != DE_IF_DST_NONE)
	{
		log_error("[DEIF %u] inval out port %u (>=%u)\n",
				u8DeIfCh, eTargetDe, DE_IF_DST_MAX);
		return 0xFF;
	}

	log_noti("DEIF %d : Set target de %d\n",u8DeIfCh, (UINT32)eTargetDe);

	ePreTargetDe = gstDeIf[u8DeIfCh].eTargetDe;

	if (ePreTargetDe == eTargetDe)
		return ePreTargetDe;

	gstDeIf[u8DeIfCh].eTargetDe =  eTargetDe;

	if (ePreTargetDe != DE_IF_DST_NONE)
		DIF_HAL_Close(ePreTargetDe);
	if (eTargetDe != DE_IF_DST_NONE)
		DIF_HAL_Open(eTargetDe);

#ifdef	ARM_USING_MMCU
	_mpipe_send_target(u8DeIfCh, (UINT8)eTargetDe);
#endif

	// TODO : add some MCU support
	return ePreTargetDe;
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
void DE_IF_Close(UINT8 u8DeIfCh)
{
#ifdef	__linux__
	unsigned long flags;
#endif

	log_noti("[DE_IF%d][DBG] %s \n", u8DeIfCh, __FUNCTION__ );

	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("[DE_IF%d][Err] wrong channel! %s\n", u8DeIfCh, __FUNCTION__ );
		return;
	}
#ifdef	__linux__
	spin_lock_irqsave(&gstDeIf[u8DeIfCh].stLock, flags);
#endif	//__linux__

#ifdef ARM_USING_MMCU
	_mpipe_send_del_ch(u8DeIfCh);
#endif

	gstDeIf[u8DeIfCh].u8VsyncCh = 0xFF;

	DE_IF_SetTargetDe(u8DeIfCh, DE_IF_DST_NONE);

	gstDeIf[u8DeIfCh].eState = DE_IF_CLOSE;
	_DE_IF_Reset(u8DeIfCh);

#ifdef	__linux__
	spin_unlock_irqrestore(&gstDeIf[u8DeIfCh].stLock, flags);
#endif	//__linux__
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
void DE_IF_Freeze(UINT8 u8DeIfCh, BOOLEAN bOnOff)
{
	if (u8DeIfCh < DEIF_NUMOF_CH)
	{
		gstDeIf[u8DeIfCh].bFreezing = bOnOff;
		log_noti("[DE_IF%d][DBG] Freeze:%d\n", u8DeIfCh, bOnOff);
	}
	else
	{
		log_error("[DE_IF] wrong ch number %d,%s", u8DeIfCh, __FUNCTION__ );
	}
}

DE_IF_DST_E DE_IF_SetDualOutput(DE_IF_DST_E eMirrorSrc, DE_IF_DST_E eMirrorPort)
{
	DE_IF_DST_E		ePreConf;
	UINT8			src, out;

	ePreConf = geMirrorPort;
	geMirrorSrc = eMirrorSrc;
	geMirrorPort = eMirrorPort;

	log_noti("Set Dual Out: src %d out %d\n", eMirrorSrc, eMirrorPort);


	if (eMirrorSrc != DE_IF_DST_NONE)
		src = (UINT8)eMirrorSrc - (UINT8)DE_IF_DST_DE_IF0;
	else
		src = 0xFF;

	if (eMirrorPort != DE_IF_DST_NONE)
		out = (UINT8)eMirrorPort - (UINT8)DE_IF_DST_DE_IF0;
	else
		out = 0xFF;

	DIF_HAL_SetMirrorOut(src, out);

#ifdef	ARM_USING_MMCU
	_mpipe_send_set_dualout((UINT8)eMirrorSrc, (UINT8)eMirrorPort);
#endif

	return ePreConf;
}

void DE_IF_RepeatFrame(UINT8 u8DeIfCh, UINT8 u8nTimes)
{
	DE_IF_T	*pstDeIf = NULL;

	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("wrong DE IF channel %d\n", u8DeIfCh);
		return;
	}
	pstDeIf = &gstDeIf[u8DeIfCh];

	if (u8nTimes == 0)
	{
		pstDeIf->bForcedPush = FALSE;
		log_noti("ch %d forced push off\n", u8DeIfCh);
	}
	else
	{
		pstDeIf->repeat_cnt = -(u8nTimes);
		log_noti("ch %d repeat %d frames\n", u8DeIfCh, u8nTimes);
	}

#ifdef ARM_USING_MMCU
	_mpipe_send_set_repeat(u8DeIfCh, u8nTimes);
#endif
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
UINT32 DE_IF_GetRunningFrameIndex(UINT8 u8DeIfCh)
{
	UINT32	ui32FrameIdx = 0xFF;

	if (u8DeIfCh < DEIF_NUMOF_CH)
	{
		ui32FrameIdx = gstDeIf[u8DeIfCh].cur_frm->frm_info.ui32FrameIdx;
		//log_noti("[DE_IF%d][DBG] Running Frame Index:0x%X\n",
		//	u8DeIfCh, ui32FrameIdx );
	}
	else
	{
		log_error("[DE_IF] wrong ch number %d,%s", u8DeIfCh, __FUNCTION__ );
	}

	return ui32FrameIdx;
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

static UINT32 _get_nth_field(UINT32 u32PicScanType, UINT32 u32nThField)
{
	UINT32 u32FieldType;

	switch (u32PicScanType)
	{
		case DISPQ_SCAN_PROG :
			u32FieldType = DE_IPC_FRAME;
			break;

		case DISPQ_SCAN_TFF :
			if (u32nThField == 0)
				u32FieldType = DE_IPC_TOP_FIELD;
			else
				u32FieldType = DE_IPC_BOTTOM_FIELD;
			break;

		case DISPQ_SCAN_BFF :
			if (u32nThField == 0)
				u32FieldType = DE_IPC_BOTTOM_FIELD;
			else
				u32FieldType = DE_IPC_TOP_FIELD;
			break;

		default :
			u32FieldType = DE_IPC_FRAME;
			log_error("[DE_IF][Err] Picture Scan Type:%d, Frame Idx:%d\n",
					u32PicScanType, u32nThField);
			break;
	}

	return u32FieldType;
}

inline BOOLEAN DE_IF_IsNeedNewFrame(UINT8 u8DeIfCh)
{
	DE_IF_T	*pstDeIf = NULL;
	BOOLEAN			bRet = TRUE;

	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("[DE_IF%d] wrong ch num\n", u8DeIfCh);
		return FALSE;
	}

	pstDeIf = &gstDeIf[u8DeIfCh];

	if (pstDeIf->eTargetDe == DE_IF_DST_NONE)
		return TRUE;

	if (pstDeIf->next_frm->valid)
		bRet = FALSE;
	else if (pstDeIf->cur_frm->valid &&
			pstDeIf->ui32FieldCount < pstDeIf->cur_frm->frm_info.ui32DisplayPeriod)
		bRet = FALSE;

	log_user2("DE_IF:%d: NeedNew? %d, nxt %d cur %d, %d >= %d ?\n",
			u8DeIfCh, bRet,pstDeIf->next_frm->valid,
			pstDeIf->cur_frm->valid,
			pstDeIf->ui32FieldCount,
			pstDeIf->cur_frm->frm_info.ui32DisplayPeriod);

	if (bRet && gDbgNDisp > -1)
	{
		if (gDbgNDisp == 0)
			bRet = FALSE;
		else
			gDbgNDisp--;
	}

	return bRet;
}

/**
 *******************************************************************************
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

inline static void _check_new_frame(UINT8 u8DeIfCh, DIF_FRM_T *cur_frm,
		DISPQ_FRM_T *new_info)
{
	DISPQ_FRM_T		*cur_info = &cur_frm->frm_info;

	if (cur_frm->valid == FALSE)
	{
		log_noti("[DE_IF%d] Start Transfer\n", u8DeIfCh);
		log_noti("[DE_IF%d] Stride: 0x%X, Base_Y: 0x%X, Base_C: 0x%X\n",
				u8DeIfCh, new_info->ui32Stride,
				new_info->ui32Y_FrameBaseAddr, new_info->ui32C_FrameBaseAddr);

		log_noti("[DE_IF%d] FrameIdx: 0x%X, RateRes: %d, eRateDiv: %d, "
				"AspectRatio: %d\n",
				u8DeIfCh, new_info->ui32FrameIdx, new_info->ui32FrameRateRes,
				new_info->ui32FrameRateDiv, new_info->ui32AspectRatio);

		log_noti("[DE_IF%d] H_Offset: %d, V_Offset: %d, PicWidth: %d, "
				"PicHeight: %d\n",
				u8DeIfCh, new_info->ui32H_Offset, new_info->ui32V_Offset,
				new_info->ui32PicWidth, new_info->ui32PicHeight);

		log_noti("[DE_IF%d] FramePackArrange: %d, LR_Order: %d\n", u8DeIfCh,
				new_info->i32FramePackArrange, new_info->ui32LR_Order);
	}
	else if (gstDeIf[u8DeIfCh].ui32FieldCount != cur_info->ui32DisplayPeriod)
	{
		log_noti("[DE_IF%d] Field Count: %d / Display Period: %d\n",
				u8DeIfCh, gstDeIf[u8DeIfCh].ui32FieldCount,
				cur_info->ui32DisplayPeriod);
	}

	if ((cur_info->ui32FrameRateRes != new_info->ui32FrameRateRes) ||
			(cur_info->ui32FrameRateDiv != new_info->ui32FrameRateDiv) ||
			(cur_info->ui32AspectRatio != new_info->ui32AspectRatio))
		log_noti("[DE_IF%d] FrameRate: %d/%d -> %d/%d, AspectRatio: %d -> %d\n",
				u8DeIfCh, cur_info->ui32FrameRateRes,
				cur_info->ui32FrameRateDiv,
				new_info->ui32FrameRateRes, new_info->ui32FrameRateDiv,
				cur_info->ui32AspectRatio, new_info->ui32AspectRatio);

	if ((cur_info->ui32PicWidth != new_info->ui32PicWidth) ||
			(cur_info->ui32PicHeight != new_info->ui32PicHeight))
		log_noti("[DE_IF%d] Width * Height: %d * %d -> %d * %d\n",
				u8DeIfCh, cur_info->ui32PicWidth, cur_info->ui32PicHeight,
				new_info->ui32PicWidth, new_info->ui32PicHeight);

	return;
}
/*
inline static void _de_if_debug_log(UINT8 u8DeIfCh, DE_IF_T *pstDeIf,
		S_DE_IPC_T *psDeIpc)
{
	if (pstDeIf->ui32FieldCount >= pstDeIf->ui32DisplayPeriod)
	{
		if ((pstDeIf->ui32FieldCount % 0x100) == 0xFF && !pstDeIf->bFreezing)
		{
			log_noti("[DE_IF%d] Too Big FieldCnt %d >= Period %d "
					"DispInfo:%d/%d\n",
					u8DeIfCh, pstDeIf->ui32FieldCount,
					pstDeIf->ui32DisplayPeriod,
					pstDeIf->ui32DisplayInfo,
					psDeIpc->ui32DisplayMode);
		}
	}

	if ((pstDeIf->ui32UpdateFrameCount == 1) &&
			(pstDeIf->ui32FieldCount == 1))
	{
		log_noti("[DE_IF%d][DBG] Start Updating\n", u8DeIfCh );
	}

	// log for debug
	pstDeIf->ui32LogTick++;
	if (pstDeIf->ui32LogTick >= 0x100)
	{
		log_noti("[DE_IF%d][DBG] Updated Frame Count: %d, DispInfo: %d\n",
				u8DeIfCh,
				pstDeIf->ui32UpdateFrameCount,
				pstDeIf->ui32DisplayInfo);

		pstDeIf->ui32LogTick = 0;
	}
}
*/

void DEIF_GetDispState(UINT8 u8DeIfCh, UINT32 *pu32DispPeriod,
		UINT32 *pu32FieldCnt, BOOLEAN *bValidCurFrm, BOOLEAN *bValidNextFrm)
{
	DE_IF_T	*pstDeIf = NULL;

	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("[DE_IF%d][Err] %s\n", u8DeIfCh, __FUNCTION__ );
		return;
	}

	pstDeIf = &gstDeIf[u8DeIfCh];

	*pu32DispPeriod = pstDeIf->cur_frm->frm_info.ui32DisplayPeriod;
	*pu32FieldCnt = pstDeIf->ui32FieldCount;
	*bValidCurFrm = pstDeIf->cur_frm->valid;
	*bValidNextFrm = pstDeIf->next_frm->valid;

	return;
}

/*
void _check_and_set_new_frame()
{
	UINT32 expected_scan;

	if (next->ui32DisplayInfo == DISPQ_SCAN_PROG || pre_scan != next->scan_type)
		goto set_new;

	expected_scan = _get_nth_field(dif->ui32DisplayInfo,
			dif->ui32DisplayPeriod);

	if (expected_scan == next->scan_type)
		goto skip_set;

	if (next->ui32DisplayPeriod >= 3)
	{
		next->ui32DisplayPeriod--;
		goto skip_set;
	}

	if (cur->ui32DisplayPeriod >= 3)
	{
		change;
		goto set_new;
	}

	goto skip_set;

set_new:
	DIF_HAL_SetNewFrame(pstDeIf->eTargetDe, psDeIpc);
	//_swap_frame(deif->pstNextFrame, deif->pstCurFrame);
	deif->stCurFrame = deif->stNextFrame;
	deif->bValidClrFrame = TRUE;
skip_set:

	return;
}
*/


#define _swap_frame(a,b)		\
	do {DIF_FRM_T	*t; t = a; a = b; b = t; } while(0)
inline BOOLEAN DE_IF_UpdateDisplay(UINT8 u8DeIfCh, UINT32 ui32SyncField)
{
	DE_IF_T			*pstDeIf = NULL;
	UINT8			u8PauseThold=0;
	DIF_FRM_T		*frame;
	DISPQ_FRM_T		*frm_info;

	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("[DE_IF%d][Err] %s\n", u8DeIfCh, __FUNCTION__ );
		return FALSE;
	}

	pstDeIf = &gstDeIf[u8DeIfCh];

	if (pstDeIf->eTargetDe == DE_IF_DST_NONE)
		return FALSE;

	if (pstDeIf->next_frm->valid)
	{
		if (pstDeIf->cur_frm->valid)
			_swap_frame(pstDeIf->cur_frm, pstDeIf->clr_frm);

		_swap_frame(pstDeIf->cur_frm, pstDeIf->next_frm);
		pstDeIf->next_frm->valid = FALSE;

		DIF_HAL_SetNewFrame(pstDeIf->eTargetDe, &pstDeIf->cur_frm->frm_info);
		pstDeIf->ui32FieldCount = 0;
	}

	frame = pstDeIf->cur_frm;
	frm_info = &frame->frm_info;

	// detect whether still picture or not
	if( pstDeIf->bForcedPush )
	{
		if( pstDeIf->ui32FieldCount == 0 )
			pstDeIf->u8NoneForcedPushCnt++;
		else if( pstDeIf->ui32FieldCount >= frm_info->ui32DisplayPeriod*8 )
			pstDeIf->u8NoneForcedPushCnt = 0;

		if( pstDeIf->u8NoneForcedPushCnt >=3 )
		{
			log_noti("[DE_IF%d] : forced push off\n",u8DeIfCh);
			pstDeIf->bForcedPush = FALSE;
			pstDeIf->u8NoneForcedPushCnt = 0;
		}
	}

	if (frame->valid == FALSE)
	{
		log_user1("DE_IF: mute idx\n");
		goto exit_de_if_up;
	}

	if (pstDeIf->eTargetDe >= DEIF_NUMOF_HWPORT)
	{
		if (pstDeIf->eTargetDe != DE_IF_DST_NONE)
			log_error("[DE_IF%d][Err] wrong target DE ch %d\n",
					u8DeIfCh, (UINT32)pstDeIf->eTargetDe);

		goto no_update;
	}

	u8PauseThold = frm_info->ui32DisplayPeriod;

#ifdef CHIP_NAME_h13
	//repeat 6 field for h13 when pause->seek state
	if ((pstDeIf->u8PrevPauseCnt - pstDeIf->u8PauseSeekStatRepeat < 6) 
			&& pstDeIf->u8PrevPauseCnt >= 20)
	{
		u8PauseThold *= DE_Q_FRAME_NUM+1;
		pstDeIf->u8PauseSeekStatRepeat--;
	}
#endif

	if (pstDeIf->bForcedPush)
		u8PauseThold *= DE_Q_FRAME_NUM+1;

	if (pstDeIf->ui32FieldCount >= u8PauseThold)
		frame->freeze = 0x1;
	else
		frame->freeze = 0x0;

	if (pstDeIf->repeat_cnt!=0)
	{
		if (pstDeIf->repeat_cnt<0 && !(pstDeIf->ui32FieldCount&1))
			pstDeIf->repeat_cnt = -(pstDeIf->repeat_cnt);

		if (pstDeIf->repeat_cnt>0)
		{
			if (frm_info->ui32DisplayInfo==DISPQ_SCAN_PROG)
				pstDeIf->repeat_cnt--;
			else if (pstDeIf->ui32FieldCount&1)
				pstDeIf->repeat_cnt--;

			frame->freeze = 0;
		}
	}

#ifdef CHIP_NAME_h13
	//counting continuous pause state
	if (frame->freeze == 0x1)
	{
		pstDeIf->u8PauseCnt++;
		pstDeIf->u8PrevPauseCnt = pstDeIf->u8PauseCnt;
		pstDeIf->u8PauseSeekStatRepeat = pstDeIf->u8PrevPauseCnt;
	}
	else if (frame->freeze == 0x0)
	{
		pstDeIf->u8PauseCnt = 0;
	}
#endif

	if (unlikely(frame->freeze))
	{
		frame->scan_type = _get_nth_field(frm_info->ui32DisplayInfo,
				(frm_info->ui32DisplayPeriod-1) % 2);
	}
	else
	{
		frame->scan_type = _get_nth_field(frm_info->ui32DisplayInfo,
				pstDeIf->ui32FieldCount % 2);
	}

	DIF_HAL_UpdateDisplay(pstDeIf->eTargetDe, frame);

no_update:
	if (pstDeIf->clr_frm->valid)
	{
		log_user1("clear %d !!\n", pstDeIf->clr_frm->frm_info.ui32FrameIdx);
		pstDeIf->clr_frm->frm_info.bDispResult = TRUE;
		vdisp_disp_done(gstDeIf[u8DeIfCh].priv, &pstDeIf->clr_frm->frm_info);
		pstDeIf->clr_frm->valid = FALSE;
	}

	pstDeIf->ui32FieldCount++;

	//_de_if_debug_log(u8DeIfCh, pstDeIf, psDeIpc);

	log_user1("DE%d Idx %2d Map %d Field %d FldCnt %d/%d(%d) "
			"Pause %d PicTy %d Qlen %d "
#ifdef CHIP_NAME_h13
			"pauseCnt %d PrevPauseCnt %d PauSeekStatRepeat %d "
#endif
			"ForcPush %d\n"
			,u8DeIfCh, frm_info->ui32FrameIdx, frm_info->ui32DpbMapMode
			,ui32SyncField, pstDeIf->ui32FieldCount,u8PauseThold
			,frm_info->ui32DisplayPeriod
			,frame->freeze
			,frame->scan_type, DISP_Q_NumActive(u8DeIfCh)
#ifdef CHIP_NAME_h13
			,pstDeIf->u8PauseCnt
			,pstDeIf->u8PrevPauseCnt
			,(pstDeIf->u8PrevPauseCnt - pstDeIf->u8PauseSeekStatRepeat)
#endif
			,pstDeIf->bForcedPush
			);

exit_de_if_up:

	return TRUE;
}

inline BOOLEAN DE_IF_SetNewFrame(UINT8 u8DeIfCh, DISPQ_FRM_T *psDisBuf,
		UINT32 ui32SyncField)
{
	DE_IF_T			*pstDeIf = NULL;
	UINT32			expect_scan, next_scan;

	if (u8DeIfCh >= DEIF_NUMOF_CH)
	{
		log_error("[DE_IF%d][Err] %s\n", u8DeIfCh, __FUNCTION__ );
		return FALSE;
	}

	pstDeIf = &gstDeIf[u8DeIfCh];

	if (pstDeIf->bFreezing == TRUE)
		return FALSE;

	log_trace("[DEIF%d] Set New Frame #%d\n",
			u8DeIfCh, psDisBuf->ui32FrameIdx);

	if (pstDeIf->eTargetDe == DE_IF_DST_NONE)
	{
		log_user1("[DEIF%d] background disp->clear %d !!\n",
				u8DeIfCh, psDisBuf->ui32FrameIdx);
		vdisp_disp_done(gstDeIf[u8DeIfCh].priv, psDisBuf);

		return TRUE;
	}

	_check_new_frame(u8DeIfCh, pstDeIf->cur_frm, psDisBuf);

#ifdef ARM_USING_MMCU
	pstDeIf->next_frm->frm_info = *psDisBuf;
	pstDeIf->next_frm->valid = TRUE;

	_mpipe_send_frame(u8DeIfCh, psDisBuf);

	return TRUE;
#endif

	if (pstDeIf->cur_frm->valid)
	{
		expect_scan = _get_nth_field(pstDeIf->cur_frm->frm_info.ui32DisplayInfo,
				pstDeIf->cur_frm->frm_info.ui32DisplayPeriod%2);
		next_scan = _get_nth_field(psDisBuf->ui32DisplayInfo, 0);

		if (pstDeIf->bForcedPush == FALSE && psDisBuf->ui32DisplayPeriod != 1 &&
				expect_scan != next_scan &&
				expect_scan != DE_IPC_FRAME &&
				next_scan != DE_IPC_FRAME)
		{
			if (psDisBuf->ui32DisplayPeriod >2)
			{
				log_noti("drop one field : DispPeriod %d\n",
						psDisBuf->ui32DisplayPeriod);
				psDisBuf->ui32DisplayPeriod--;
			}
			else
			{
				log_noti("stuff one field : DispPeriod %d\n",
						psDisBuf->ui32DisplayPeriod);
				psDisBuf->ui32DisplayPeriod++;
			}

			if (psDisBuf->ui32DisplayInfo == DISPQ_SCAN_TFF)
				psDisBuf->ui32DisplayInfo = DISPQ_SCAN_BFF;
			else
				psDisBuf->ui32DisplayInfo = DISPQ_SCAN_TFF;
		}
	}

	pstDeIf->next_frm->frm_info = *psDisBuf;
	pstDeIf->next_frm->valid = TRUE;

	pstDeIf->ui32UpdateFrameCount++;

	return TRUE;
}

void DEIF_Suspend(void)
{
	DIF_HAL_Suspend();
}

void DEIF_Resume(void)
{
	DIF_HAL_Resume();
}

#ifdef __linux__
void _deif_print_channel(struct seq_file *m, DE_IF_T *deif)
{
#define VDS_DPRINT(v,f)		\
	seq_printf(m, #v" = "f"\n", deif->v)
#define VDS_DPRINT_PTR(v)		\
	seq_printf(m, #v" = 0x%X\n", (UINT32)&deif->v)

	VDS_DPRINT(eTargetDe, "%u");

	VDS_DPRINT(u8DeIfCh, "%u");
	VDS_DPRINT(u8VsyncCh, "%u");
	VDS_DPRINT(eState, "%u");

	VDS_DPRINT(bFreezing, "%u");

	VDS_DPRINT(ui32FieldCount, "%u");

	VDS_DPRINT(bForcedPush, "%u");
	VDS_DPRINT(u8NoneForcedPushCnt, "%u");
	VDS_DPRINT(repeat_cnt, "%u");

	VDS_DPRINT(priv, "%p");

	VDS_DPRINT(ui32LogTick, "%u");
	VDS_DPRINT(ui32UpdateFrameCount, "%u");

#undef VDS_DPRINT
#undef VDS_DPRINT_PTR

	seq_printf(m,"\n");
}

void DEIF_PrintDebug(struct seq_file *m)
{
	DE_IF_T		*deif;
	UINT32		i;

	seq_printf(m, "Mirror: Src %u, Port %u\n\n",
			(UINT32)geMirrorSrc, (UINT32)geMirrorPort);

	seq_printf(m,"\n");

	for (i=0;i<DEIF_NUMOF_CH;i++)
	{
		deif = &gstDeIf[i];
		seq_printf(m, "====  DE I/F #%d [%X]  ====\n", i, (UINT32)deif);
		_deif_print_channel(m, deif);
	}
}
#endif

#if defined (CHIP_NAME_d14)
void DE_IF_SeamlessModeNoti(UINT8 ui8SeamlessMode)
{
	DIF_HAL_SetSeamless(ui8SeamlessMode);
}
#endif


