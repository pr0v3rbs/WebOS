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
 * date       2011.06.14
 * note       Additional information.
 *
 * @addtogroup lg115x_vdec
 * @{
 */


/*-----------------------------------------------------------------------------
 *   Control Constants
 *----------------------------------------------------------------------------*/
#define	SYNC_WAIT_THOLD						(256)
#define	SYNC_WAIT_THOLD_LIVE				(256/2)	//kibeom.kim reduce wait count from 256 to (256/2) for Spain_Test_Stream_1_19905874.TRP
#define	SYNC_SMTH_WAIT_THOLD				(4)
#define	SYNC_DCARD_THOLD					(2048)	// 1min =~ 60*1000/33
#define	SYNC_DCARD_THOLD_LIVE				(300/2)	//kibeom.kim reduce discard count from 300 to (300/2) for Spain_Test_Stream_1_19905874.TRP
#define	SYNC_SMTH_DCARD_THOLD				(64)
#define	PTS_CONTINUITY_DURATION				(64)
#define	PTS_CONTINUITY_COUNT				(16)
#define	PTS_WAITING_SYNC_COUNT				(4)
#define	PTS_DISCARD_SYNC_COUNT				(2)

#define	VDEC_FAST_CHCH_FRAME			3
#define	VDEC_FAST_CHCH_FRAME_DEVDO		1

#define LOG_NAME	vdec_pts
#define LOG_LEVEL	log_level_error
/*-----------------------------------------------------------------------------
 *   File Inclusions
 *----------------------------------------------------------------------------*/
#include <linux/seq_file.h>

#include "pts_drv.h"
#include "disp_q.h"
#include "../hal/vdec_base.h"
#include "vdec_rate.h"
#include "../hal/top_hal_api.h"

#include "log.h"


static int dbg_disp_offset;
static unsigned int vdec_adpq_len = 7;
module_param (dbg_disp_offset, int, 0644);
module_param (vdec_adpq_len, uint, 0644);

/*-----------------------------------------------------------------------------
 *   Constant Definitions
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *   Macro Definitions
 *----------------------------------------------------------------------------*/
#define	PTS_GET_PTS_OFFSET( _prev, _curr )	\
	(((_curr) < (_prev)) ? ((_curr) + VDEC_PTS_MASK - (_prev)) : 		\
	 (_curr) - (_prev))

#define PTS_GET_OFFSET(pts, base, offset)				\
	do {												\
		(offset) = (pts)-(base);				\
		if (((SINT32)(offset)) > (SINT32)(VDEC_PTS_MASK>>1))	\
			(offset) -= (VDEC_PTS_MASK);				\
		else if (((SINT32)(offset)) < -(SINT32)(VDEC_PTS_MASK>>1))	\
			(offset) += (VDEC_PTS_MASK);					\
	} while(0)

#define	PTS_CHECK_PTS_WRAPAROUND( _pts)	((_pts) & VDEC_PTS_MASK)

/*-----------------------------------------------------------------------------
 *   Type Definitions
 *----------------------------------------------------------------------------*/
typedef enum
{
	PTS_CONTINUITY_NONE = 0,
	PTS_CONTINUITY_NORMAL,
	PTS_CONTINUITY_SAME,
	PTS_CONTINUITY_JITTER,
	PTS_CONTINUITY_WRAPAROUND,
	PTS_CONTINUITY_INVALID_STC,
	PTS_CONTINUITY_MAX,
	PTS_CONTINUITY_MAX_STR_LEN = 15,
	PTS_CONTINUITY_UINT32 = 0x20110809
} PTS_CONTINUITY_E;

typedef enum
{
	PTS_DQ_NONE = 0,
	PTS_DQ_UNDERRUN,
	PTS_DQ_NORMAL,
	PTS_DQ_OVERFLOW,
	PTS_DQ_UINT32 = 0x20110716
} E_PTS_FLOW_T;

struct _SYNC_T
{
	UINT32		ui8SyncCh;

	UINT32		u32nFrame;
	BOOLEAN		bSmoothEn;
	UINT32		u32SmWaitCnt;
	UINT32		u32SmDiscardCnt;

	UINT32		ui32PTS_Prev;
	UINT32		ui32STC_Prev;
	PTS_CONTINUITY_E		ePtsCont;
	UINT32		u32PtsContnCnt;
	UINT32		u32ContnFrmCnt;
	SINT32		i32SyncDiff;

	UINT32		u32EmptyCnt10;
	UINT32		u32AdpMaxQLen;
	UINT32		u32AdpOverCnt;
	UINT32		u32QLen;

	UINT8		u8DiscontCnt;
	UINT8		u8DiscardLv;

	UINT16		short_qlen_thold;
	UINT16		short_qlen_cnt;
	UINT16		long_qlen_cnt;

	struct
	{
		BOOLEAN		bUseGstc;
		PTS_MATCH_MODE_E		eMatchMode;
		UINT32 		u32DispOffsetMs;
		UINT8		bIsDualDecoding;
	} Config;
	struct
	{
		UINT32		ui32PtsRateRes;
		UINT32		ui32PtsRateDiv;

		UINT32		ui32BaseTime;
		UINT32		ui32BasePTS;

		BOOLEAN		bInterlaced;
	} Rate;
	struct
	{
		UINT32		ui32LogTick_GetNew;
		UINT32		ui32LogTick_GetNewNo;
		UINT32		ui32LogTick_Noti;
		UINT32		ui32LogTick_Match;
		UINT32		ui32LogTick_Pause;

		UINT32		ui32MatchedFrameCount;
		UINT32		ui32MatchingFrameDuration;
		UINT32		ui32TotalFrameCount;
	} Debug;
};

/*-----------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *   External Variables
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *   global Functions
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *   global Variables
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *   Static Variables
 *----------------------------------------------------------------------------*/
static SYNC_T gsPts[VDISP_NUM_OF_CHANNEL];
static UINT32 u32PreStc[VDISP_NUM_OF_CHANNEL];
static UINT32 u32PrePts[VDISP_NUM_OF_CHANNEL];
static BOOLEAN bWraparoundCheck[VDISP_NUM_OF_CHANNEL];
static BOOLEAN bStcDiscon[VDISP_NUM_OF_CHANNEL];
static BOOLEAN bNoPCR[VDISP_NUM_OF_CHANNEL];
static BOOLEAN bClockSourceChanged[VDISP_NUM_OF_CHANNEL];
static UINT32 u32StcDisconCnt[VDISP_NUM_OF_CHANNEL];
static UINT32 gu32nFastChChFrame;
static UINT8	gu8DiscardLv = 2;
static SINT32	phase_shift = 0;
#if defined(CHIP_NAME_h13) && (CHIP_REV<0xb0)
#define bDualVsyncForProgFrame	TRUE
#else
#define bDualVsyncForProgFrame	FALSE
#endif

static char conti_string[PTS_CONTINUITY_MAX][PTS_CONTINUITY_MAX_STR_LEN] =
{
	[PTS_CONTINUITY_NONE]		= "NONE",
	[PTS_CONTINUITY_NORMAL]		= "NORMAL",
	[PTS_CONTINUITY_SAME]		= "SAME",
	[PTS_CONTINUITY_JITTER]		= "JITTER",
	[PTS_CONTINUITY_WRAPAROUND]	= "WRAPAROUND",
	[PTS_CONTINUITY_INVALID_STC] = "INVAL STC"
};

char azMatchType[STC_PTS_MAX][STC_PTS_MAX_STR_LEN] =
{
	[STC_PTS_NONE]  			= "NONE",
	[STC_PTS_DISCARD]           = "DISCARD",
	[STC_PTS_SMOOTH_DISCARD]    = "SMOOTH DISCARD",
	[STC_PTS_MATCHED]           = "MATCHED",
	[STC_PTS_SMOOTH_WAITING]    = "SMOOTH WAITING",
	[STC_PTS_WAITING]           = "WAITING",
	[STC_PTS_FREERUN]           = "FREERUN"
};

char azMatchMode[PTS_MATCH_MODE_MAX][PTS_MATCH_MODE_STR_LEN] =
{
	[PTS_MATCH_ENABLE				] = "ENABLE",
	[PTS_MATCH_FREERUN_BASED_SYNC	] = "FREERUN_BASED_SYNC",
	[PTS_MATCH_FREERUN_IGNORE_SYNC	] = "FREERUN_IGNORE_SYNC",
	[PTS_MATCH_ADAPTIVE_FREERUN		] = "ADAPTIVE_FREERUN"
};

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
#if 0
static void _PTS_Set_AVLipSyncBase(UINT8 ui8SyncCh, UINT32 ui32BaseTime,
		UINT32 ui32BasePTS)
{
	UINT8	bUseGstc = gsPts[ui8SyncCh].Config.bUseGstc;

	if (bUseGstc == FALSE)
	{
		log_error("[PTS%d][Err] Not File Play Channel, %s\n",
				ui8SyncCh, __FUNCTION__ );
		return;
	}

	if ( ((ui32BaseTime != 0xFFFFFFFF) && (ui32BasePTS == 0xFFFFFFFF)) &&
		((ui32BaseTime == 0xFFFFFFFF) && (ui32BasePTS != 0xFFFFFFFF)) )
	{
		log_error("[PTS%d][ERR] Set Base Time:0x%08X, PTS:0x%08X, %s\n",
				ui8SyncCh, ui32BaseTime, ui32BasePTS, __FUNCTION__);
		return;
	}

	log_noti ("pts%d, set basetime %08x, basePTS %08x\n", ui8SyncCh,
			ui32BaseTime, ui32BasePTS);

	gsPts[ui8SyncCh].Rate.ui32BaseTime = ui32BaseTime;
	gsPts[ui8SyncCh].Rate.ui32BasePTS = ui32BasePTS;
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
void PTS_Reset(SYNC_T *pstSync)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS][Err] null pointer %x %s\n",
				(UINT32)pstSync, __FUNCTION__ );
		return;
	}

	log_noti("[PTS%d][DBG] %s\n", pstSync->ui8SyncCh, __FUNCTION__ );

	pstSync->u32nFrame = 0;

	// Pic
	pstSync->ui32PTS_Prev = 0xFFFFFFFF;
	pstSync->ui32STC_Prev = 0xFFFFFFFF;
	pstSync->ePtsCont = PTS_CONTINUITY_NORMAL;
	pstSync->u8DiscontCnt = 0;
	pstSync->u8DiscardLv = gu8DiscardLv;

	pstSync->bSmoothEn = FALSE;
	pstSync->u32SmWaitCnt = 0;
	pstSync->u32SmDiscardCnt = 0;

	pstSync->u32PtsContnCnt = 0;
	pstSync->u32ContnFrmCnt = 0;
	pstSync->i32SyncDiff = 0;

	pstSync->u32EmptyCnt10 = 0;
	pstSync->u32AdpMaxQLen = vdec_adpq_len;
	pstSync->u32AdpOverCnt = 0;
	pstSync->u32QLen = 0;

	pstSync->short_qlen_thold = 100;
	pstSync->short_qlen_cnt = 101;
	pstSync->long_qlen_cnt = 0;

	pstSync->Rate.ui32PtsRateRes = 0;
	pstSync->Rate.ui32PtsRateDiv = 1;
	pstSync->Rate.ui32BaseTime = 0xFFFFFFFF;
	pstSync->Rate.ui32BasePTS = 0xFFFFFFFF;
	pstSync->Rate.bInterlaced = TRUE;

	pstSync->Debug.ui32LogTick_GetNew = 0;
	pstSync->Debug.ui32LogTick_GetNewNo = 0;
	pstSync->Debug.ui32LogTick_Noti = 0x40;
	pstSync->Debug.ui32LogTick_Match = 0x80;
	pstSync->Debug.ui32LogTick_Pause = 0;

	pstSync->Debug.ui32MatchedFrameCount = 0;
	pstSync->Debug.ui32MatchingFrameDuration = 0;
	pstSync->Debug.ui32TotalFrameCount = 0;
}

SYNC_T* PTS_GetHandler(UINT8 ui8SyncCh)
{
	if ( ui8SyncCh >= VDISP_NUM_OF_CHANNEL )
	{
		log_error("[PTS] wrong ch num %d\n", ui8SyncCh);
		return NULL;
	}

	gsPts[ui8SyncCh].ui8SyncCh = ui8SyncCh;

	return &gsPts[ui8SyncCh];
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
void PTS_Init(void)
{
	SYNC_T		*pstSync;
	UINT8		ui8SyncCh;
	UINT32		chip;
	BOOLEAN		bVdecVdo;

	VRATE_Init();

	chip = vdec_chip_rev();

	if (!VDEC_COMP_CHIP(chip,VDEC_CHIP_H14))	// if H14
		bVdecVdo = TRUE;
	else
	{
		if (chip >= VDEC_CHIP_REV(M14, B0))
			bVdecVdo = FALSE;
		else
			bVdecVdo = TRUE;
	}

	if (bVdecVdo)
		gu32nFastChChFrame = VDEC_FAST_CHCH_FRAME;
	else
		gu32nFastChChFrame = VDEC_FAST_CHCH_FRAME_DEVDO;

	log_noti("[PTS][DBG] PTS init: 0x%X, %s\n", (UINT32)gsPts, __FUNCTION__ );

	for ( ui8SyncCh = 0; ui8SyncCh < VDISP_NUM_OF_CHANNEL; ui8SyncCh++ )
	{
		pstSync = PTS_GetHandler(ui8SyncCh);
		if (pstSync == NULL)
		{
			log_error("[PTS%d] fail to get handler\n", ui8SyncCh);
			break;
		}

		pstSync->ui8SyncCh = ui8SyncCh;
		pstSync->Config.bUseGstc = FALSE;
		pstSync->Config.eMatchMode = PTS_MATCH_ENABLE;
		pstSync->Config.u32DispOffsetMs = 0x0;
		pstSync->Config.bIsDualDecoding = FALSE;

		PTS_Reset(pstSync);
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
BOOLEAN PTS_SetUseGstc(UINT8 ui8SyncCh, BOOLEAN bUseGstc)
{
	SYNC_T *pstSync;

	pstSync = PTS_GetHandler(ui8SyncCh);
	if ( pstSync == NULL )
	{
		log_error("[PTS%d] fail to get handler!!\n", ui8SyncCh);
		return FALSE;
	}

	if (pstSync->Config.bUseGstc != bUseGstc)
	{
		bClockSourceChanged[ui8SyncCh] = TRUE;
		pstSync->Config.bUseGstc = bUseGstc;
	}
	log_noti("[PTS%d] bUse Gstc : %s\n",ui8SyncCh, (bUseGstc == TRUE)? "true":"false");
	return TRUE;
}

SYNC_T* PTS_Open(UINT8 ui8SyncCh, BOOLEAN bUseGstc, UINT32 u32DispOffsetMs,
		BOOLEAN bIsDualDecoding)
{
	SYNC_T *pstSync;

	log_noti("[PTS%d][DBG] Display Offset: %d Dual? %d\n",
			ui8SyncCh, u32DispOffsetMs, bIsDualDecoding);

	pstSync = PTS_GetHandler(ui8SyncCh);
	if ( pstSync == NULL )
	{
		log_error("[PTS%d] fail to get handler!!\n", ui8SyncCh);
		return NULL;
	}

	pstSync->Config.bUseGstc = bUseGstc;
	pstSync->Config.u32DispOffsetMs = u32DispOffsetMs;
	pstSync->Config.bIsDualDecoding = bIsDualDecoding;
	pstSync->Config.eMatchMode = PTS_MATCH_ENABLE;

	u32PreStc[ui8SyncCh] = 0xffffffff;
	u32PrePts[ui8SyncCh] = 0xffffffff;
	bWraparoundCheck[ui8SyncCh] = FALSE;
	bStcDiscon[ui8SyncCh] = FALSE;
	bNoPCR[ui8SyncCh] = FALSE;
	bClockSourceChanged[ui8SyncCh] = FALSE;
	phase_shift = 0;

	PTS_Reset(pstSync);

	VRATE_Reset(ui8SyncCh);
	VRATE_SetUseGSTC(ui8SyncCh, bUseGstc);

	return pstSync;
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
void PTS_Close(SYNC_T *pstSync)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS][Err] NULL handler %s\n", __FUNCTION__ );
		return;
	}

	if (pstSync->ui8SyncCh >= VDISP_NUM_OF_CHANNEL )
	{
		log_error("[PTS%d][Err] %s\n", pstSync->ui8SyncCh, __FUNCTION__ );
		return;
	}

	log_noti("[PTS%d][DBG] %s\n", pstSync->ui8SyncCh, __FUNCTION__ );

	PTS_Reset(pstSync);

	pstSync->Config.eMatchMode = PTS_MATCH_ENABLE;
	pstSync->Config.u32DispOffsetMs = 0x0;
	pstSync->Config.bIsDualDecoding = FALSE;
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
void PTS_SetDisplayOffset(SYNC_T *pstSync, UINT32 u32DispOffsetMs)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS] null pointer, %s\n", __FUNCTION__ );
		return;
	}

	log_noti("[PTS%d][DBG] DisplayOffset: %dms, %s\n", pstSync->ui8SyncCh,
			u32DispOffsetMs, __FUNCTION__);

	pstSync->Config.u32DispOffsetMs = u32DispOffsetMs;
}
#if defined (CHIP_NAME_d14)
SINT32 PTS_GetDisplayOffset(SYNC_T *pstSync)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS] null pointer, %s\n", __FUNCTION__ );
		return 0;
	}

	return pstSync->Config.u32DispOffsetMs;
}

void PTS_SetPhaseShift(SYNC_T *pstSync, SINT32 shift)
{
	UINT32		frame_intv;

	frame_intv = VRATE_GetDuration(pstSync->ui8SyncCh);

	phase_shift += shift;

	log_noti("[PTS%d] add phase shift %d, => %d\n",
		pstSync->ui8SyncCh, shift, phase_shift);

	// range check
	if (phase_shift > (SINT32)(frame_intv/2))
		phase_shift -= frame_intv;
	else if (phase_shift < -((SINT32)(frame_intv/2)))
		phase_shift += frame_intv;
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
void PTS_SetStcDiscont(SYNC_T *pstSync)
{
/*
	if ( pstSync->u8DiscontCnt > 32 )
		log_error("Too big discontinuity count %d\n", pstSync->u8DiscontCnt);

	pstSync->u8DiscontCnt++;
	log_debug("discontinuity count %d\n", pstSync->u8DiscontCnt);
*/
}

BOOLEAN PTS_IsDiscont(SYNC_T *pstSync)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS] wrong handler %X, %s\n",
				(UINT32)pstSync, __FUNCTION__);
		return FALSE;
	}

	return bStcDiscon[pstSync->ui8SyncCh];
}

void PTS_SetMatchMode(SYNC_T *pstSync, PTS_MATCH_MODE_E eMatchMode)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS] wrong handler %X, %s\n",
				(UINT32)pstSync, __FUNCTION__);
		return;
	}

	log_noti("[PTS%d] Set Match Mode %d\n", pstSync->ui8SyncCh, eMatchMode);

	switch ( eMatchMode )
	{
	case PTS_MATCH_ENABLE :
	case PTS_MATCH_FREERUN_BASED_SYNC :
	case PTS_MATCH_FREERUN_IGNORE_SYNC :
	case PTS_MATCH_ADAPTIVE_FREERUN :
		log_noti("[PTS%d] Match Mode %d(%s) -> %d(%s)\n",
				pstSync->ui8SyncCh,
				pstSync->Config.eMatchMode,
				azMatchMode[pstSync->Config.eMatchMode],
				eMatchMode, azMatchMode[eMatchMode]
				);

		pstSync->Config.eMatchMode = eMatchMode;
		break;
	default :
		log_error("[PTS%d][Err] Match Mode:%d, %s\n", pstSync->ui8SyncCh,
				eMatchMode, __FUNCTION__ );
		break;
	}
}

PTS_MATCH_MODE_E PTS_GetMatchMode(SYNC_T *pstSync)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS] wrong handler %X, %s\n",
				(UINT32)pstSync, __FUNCTION__);
		return 0;
	}

	log_user3("[PTS%d] Get Match Mode %d\n", pstSync->ui8SyncCh,
			pstSync->Config.eMatchMode);

	return pstSync->Config.eMatchMode;
}

void PTS_SetMaxQlen(SYNC_T *pstSync, UINT8 max_qlen)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS] wrong handler %X, %s\n",
				(UINT32)pstSync, __FUNCTION__);
		return;
	}

	pstSync->u32AdpMaxQLen = max_qlen;
}

UINT32 PTS_GetMaxQlen(SYNC_T *pstSync)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS] wrong handler %X, %s\n",
				(UINT32)pstSync, __FUNCTION__);
		return 0;
	}

	return pstSync->u32AdpMaxQLen;
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
void PTS_Set_BaseTime(SYNC_T *pstSync, UINT32 ui32BaseTime, UINT32 ui32BasePTS)
{
	if ( pstSync == NULL )
	{
		log_error("[PTS] wrong handler %X, %s\n",
				(UINT32)pstSync, __FUNCTION__);
		return;
	}

	if ( ui32BasePTS == VDEC_UNKNOWN_PTS )
	{
		log_noti("[PTS%d][DBG] Unknwon PTS, %s\n",
				pstSync->ui8SyncCh, __FUNCTION__ );
		return;
	}

	ui32BaseTime &= VDEC_PTS_MASK;
	ui32BasePTS &= VDEC_PTS_MASK;
	phase_shift = 0;

	if ( pstSync->Config.bUseGstc )
	{
		log_noti("[PTS%d] Pre BaseTime:0x%08X, PTS:0x%08X\n",
				pstSync->ui8SyncCh,	pstSync->Rate.ui32BaseTime,
				pstSync->Rate.ui32BasePTS);
		log_noti("[PTS%d]    Set new -> BaseTime:0x%08X, PTS:0x%08X\n",
				pstSync->ui8SyncCh, ui32BaseTime, ui32BasePTS);

		//_PTS_Set_AVLipSyncBase( pstSync->ui8SyncCh, ui32BaseTime, ui32BasePTS);
		pstSync->Rate.ui32BaseTime = ui32BaseTime;
		pstSync->Rate.ui32BasePTS = ui32BasePTS;
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
void PTS_Get_BaseTime(SYNC_T *pstSync, UINT32 *pui32BaseTime,
		UINT32 *pui32BasePTS )
{
	*pui32BaseTime = pstSync->Rate.ui32BaseTime;
	*pui32BasePTS = pstSync->Rate.ui32BasePTS;
}

SINT32 PTS_GetDiff(SYNC_T *pstSync)
{
	return pstSync->i32SyncDiff;
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
void PTS_Flush(SYNC_T *pstSync)
{
	UINT8 ui8SyncCh = pstSync->ui8SyncCh;

	log_noti("[PTS%d] Flush: Occupancy: %d\n", ui8SyncCh,
			DISP_Q_NumActive(ui8SyncCh) );

	VRATE_Flush(ui8SyncCh);

	// Pic
	pstSync->bSmoothEn = FALSE;
	pstSync->ui32PTS_Prev = 0xFFFFFFFF;
	pstSync->ui32STC_Prev = 0xFFFFFFFF;
	pstSync->ePtsCont = PTS_CONTINUITY_NORMAL;
	pstSync->u8DiscontCnt = 0;

	pstSync->u32PtsContnCnt = 0;
	pstSync->u32ContnFrmCnt = 0;
	pstSync->i32SyncDiff = 0;
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
static BOOLEAN _PTS_WithinRange(UINT32 ui32STC_Start, UINT32 ui32STC_End,
		UINT32 ui32PTS)
{
	if ( ui32STC_Start <= ui32STC_End )
	{
		if ( (ui32PTS > ui32STC_Start) &&
			(ui32PTS <= ui32STC_End) )
			return TRUE;
	}
	else
	{
		if ( (ui32PTS > ui32STC_Start) ||
			(ui32PTS <= ui32STC_End) )
			return TRUE;
	}

	return FALSE;
}

void PTS_WraparoundCheck(UINT8 ui8SyncCh, UINT32 ui32STC)
{
	UINT32 u32Start, u32End;
	static UINT32 u32SameStcCnt[VDISP_NUM_OF_CHANNEL];

	// No PCR check, No STC change check
	if ( u32PreStc[ui8SyncCh] == ui32STC )
	{
		if (u32SameStcCnt[ui8SyncCh] > 10)
		{
			static UINT32 u32DbgCnt;

			if ( !(u32DbgCnt++ % 900) )
					log_noti("PTS%d: wrong stc(pre:%d cur:%d)\n", ui8SyncCh,
							u32PreStc[ui8SyncCh], ui32STC);

			bNoPCR[ui8SyncCh] = TRUE;
			return;
		}
		else
			u32SameStcCnt[ui8SyncCh]++;
	}
	else
	{
		bNoPCR[ui8SyncCh] = FALSE;
		u32SameStcCnt[ui8SyncCh] = 0;
	}

	if ( !bWraparoundCheck[ui8SyncCh] )
		return;

	if ( u32PreStc[ui8SyncCh] == 0xffffffff )
		goto inval_wa_check_exit;

	if ( bClockSourceChanged[ui8SyncCh] )
	{
		bClockSourceChanged[ui8SyncCh] = FALSE;
		goto inval_wa_check_exit;
	}

	// stc discontinuity is regarded as start of warp-around duration
	if ( bStcDiscon[ui8SyncCh] == FALSE )
	{
		u32Start = (u32PreStc[ui8SyncCh]-18000) & VDEC_PTS_MASK;
		u32End = (u32PreStc[ui8SyncCh]+45000) & VDEC_PTS_MASK;

		if ( _PTS_WithinRange(u32Start, u32End, ui32STC) != TRUE )
		{
			log_noti("[pts%d] start wrap around %d->%d %d %d\n",
					ui8SyncCh, u32PreStc[ui8SyncCh], ui32STC, u32Start, u32End);
			bStcDiscon[ui8SyncCh] = TRUE;
			u32StcDisconCnt[ui8SyncCh] = 0;
			phase_shift = 0;
		}
	}

inval_wa_check_exit:

	u32PreStc[ui8SyncCh] = ui32STC;
}

static BOOLEAN pts_invalid_stc_check(UINT8 ui8SyncCh, UINT32 ui32PTS,
		UINT32 ui32STC)
{
	UINT32 u32Start, u32End;

	if ( bNoPCR[ui8SyncCh] )
		return TRUE;

	// Handling Wrap Around
	if ( u32PrePts[ui8SyncCh] == 0xffffffff )
		goto inval_stc_check_exit;

	// pts discontinuity is regarded as end of wrap-around duration.
	// we expect PTS will increase linearly after this point.
	if ( bStcDiscon[ui8SyncCh] )
	{
		u32Start = (u32PrePts[ui8SyncCh]-9000) & VDEC_PTS_MASK;
		u32End = (u32PrePts[ui8SyncCh]+27000) & VDEC_PTS_MASK;

		if ( _PTS_WithinRange(u32Start, u32End, ui32PTS) != TRUE ||
				_PTS_WithinRange((ui32STC-180000)&VDEC_PTS_MASK,
					(ui32STC+180000)&VDEC_PTS_MASK, ui32PTS) 	)
		{
			log_noti("[pts%d] end wrap around\n", ui8SyncCh);
			bStcDiscon[ui8SyncCh] = FALSE;
		}

		u32StcDisconCnt[ui8SyncCh]++;
		if ( u32StcDisconCnt[ui8SyncCh] > 60 )
		{
			log_noti("[pts%d] end wrap around forcedly %d\n", ui8SyncCh,
					u32StcDisconCnt[ui8SyncCh]);
			bStcDiscon[ui8SyncCh] = FALSE;
		}
	}

inval_stc_check_exit:

	u32PrePts[ui8SyncCh] = ui32PTS;

	return bStcDiscon[ui8SyncCh];

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
static PTS_CONTINUITY_E _PTS_CheckPtsDiscont(SYNC_T *pstSync,
		UINT32 ui32PTS_Prev, UINT32 ui32PTS_Curr, UINT32 ui32STC)
{
	UINT32		ui32JitterDuration;
	UINT32		ui32JitterBoundary;
	UINT32		ui32NormalDuration;
	UINT32		ui32NormalBoundary;
	UINT32		ui32FrameDuration;
	SINT32 		i32Speed;
	static UINT32 u32LogCnt = 0;
	UINT8 		ui8SyncCh = pstSync->ui8SyncCh;
	PTS_CONTINUITY_E	ePtsCont = PTS_CONTINUITY_NONE;
	static PTS_CONTINUITY_E	ePreCont[VDISP_NUM_OF_CHANNEL];

	// check STC continuity
	if ( pts_invalid_stc_check(ui8SyncCh, ui32PTS_Curr, ui32STC) )
		return PTS_CONTINUITY_INVALID_STC;

	// for the first frame
	if ( ui32PTS_Prev == 0xFFFFFFFF )
		return PTS_CONTINUITY_NORMAL;

	ui32FrameDuration = VRATE_GetDuration(ui8SyncCh);
	if ( ui32FrameDuration == 0 )
		return PTS_CONTINUITY_NORMAL;

	ui32PTS_Prev &= VDEC_PTS_MASK;
	ui32PTS_Curr &= VDEC_PTS_MASK;

	i32Speed = VRATE_GetSpeed(ui8SyncCh);

	if ( i32Speed == 1000 )
		ui32JitterDuration = ui32FrameDuration * PTS_CONTINUITY_DURATION;
	else
	{
		if ( i32Speed < 0 )
			return PTS_CONTINUITY_NORMAL;

		ui32JitterDuration = ui32FrameDuration * PTS_CONTINUITY_DURATION
			* i32Speed / 1000;
	}

	if ( ui32PTS_Prev>= ui32JitterDuration )
		ui32JitterBoundary = ui32PTS_Prev - ui32JitterDuration;
	else
		ui32JitterBoundary = ui32PTS_Prev + VDEC_PTS_MASK - ui32JitterDuration;

	ui32NormalDuration = ui32FrameDuration * PTS_CONTINUITY_DURATION;
	if ( i32Speed > 1000 || i32Speed < -1000)
		ui32NormalDuration = ui32NormalDuration * i32Speed / 1000;
	//else if (i32Speed < 0)
	//		ui32NormalDuration = -ui32NormalDuration;
	ui32NormalBoundary = ui32PTS_Prev + ui32NormalDuration;
	ui32NormalBoundary &= VDEC_PTS_MASK;

	if ( _PTS_WithinRange(ui32JitterBoundary, ui32PTS_Prev,
				ui32PTS_Curr) == TRUE )
	{
		if ( ui32PTS_Prev == ui32PTS_Curr )
		{
			ePtsCont = PTS_CONTINUITY_SAME;
			if ( !((u32LogCnt++) % 30) )
				log_noti("[PTS%d][Warn:0x%X] PTS Discontin - Same PTS(0x%X), "
						"Occupancy: %d, FrameDuration:0x%X\n",
						ui8SyncCh, ui32STC, ui32PTS_Curr,
						DISP_Q_NumActive(ui8SyncCh), ui32FrameDuration );
		}
		else
		{
			ePtsCont = PTS_CONTINUITY_JITTER;

			if ( !((u32LogCnt++) % 30) )
				log_noti("[PTS%d][Warn:0x%X] PTS Discontinuity"
						"- Jitter PTS(Prev:0x%X, Curr:0x%X), Occupancy: %d, "
						"FrameDuration:0x%X\n",
						ui8SyncCh, ui32STC, ui32PTS_Prev, ui32PTS_Curr,
						DISP_Q_NumActive(ui8SyncCh), ui32FrameDuration );
		}
	}
	else if ( _PTS_WithinRange(ui32PTS_Prev, ui32NormalBoundary,
				ui32PTS_Curr) == TRUE )
	{
		ePtsCont = PTS_CONTINUITY_NORMAL;
		u32LogCnt = 0;
	}
	else
	{
		ePtsCont = PTS_CONTINUITY_WRAPAROUND;
		log_noti("[PTS%d][Warn:0x%X] PTS Discontinuity - Wrap Around PTS"
				"(Prev:0x%X, Curr:0x%X), Occupancy: %d, FrameDuration:0x%X\n",
				ui8SyncCh, ui32STC, ui32PTS_Prev, ui32PTS_Curr,
				DISP_Q_NumActive(ui8SyncCh), ui32FrameDuration );
	}

	if (ePtsCont != ePreCont[ui8SyncCh])
		log_noti("pts conti %s => %s\n",conti_string[ePreCont[ui8SyncCh]],
				conti_string[ePtsCont]);

	ePreCont[ui8SyncCh] = ePtsCont;

	return ePtsCont;
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
static PTS_MATCH_E _PTS_CheckStcPtsMatch(SYNC_T *pstSync, UINT32 ui32STC,
		UINT32 ui32PTS, UINT32 u32DispOffsetMs, BOOLEAN bSmoothing)
{
	UINT32		ui32DiscardDuration;
	UINT32		ui32MatchedDuration;
	UINT32		ui32WaitingDuration;
	UINT32		ui32SmthWaitingDuration;
	UINT32		ui32SmthDiscardDuration;

	UINT32		u32DiscardBndary;
	UINT32		u32MatchBndary;
	UINT32		u32MatchBndaryEnd;
	UINT32		u32WaitBndary;
	UINT32		u32SmthWaitBndary;
	UINT32		u32SmthDcardBndr;

	UINT32		ui32StreamTime;
	UINT32		u32CompenPts;
	PTS_MATCH_E	eStcPtsMatched = STC_PTS_NONE;
	UINT32		ui32DisplayOffset_90kTick;
	SINT32 		i32Speed;
	UINT32		ui32FrameDuration;
	UINT8		ui8SyncCh = pstSync->ui8SyncCh;
	SINT32		i32PTS_Offset;
	UINT64		u64Temp;

	bWraparoundCheck[ui8SyncCh] = TRUE;

	i32Speed = VRATE_GetSpeed(ui8SyncCh);
	if ( i32Speed == 0 )
	{
		if ( (pstSync->Debug.ui32LogTick_Pause % 10) == 0 )
			log_noti("[PTS%d][DBG:0x%X] STC Matched - Speed: %d, PTS:0x%X, "
					"STC:0x%X, Occupancy: %d\n", ui8SyncCh, ui32STC, i32Speed,
					ui32PTS, ui32STC, DISP_Q_NumActive(ui8SyncCh));
		pstSync->Debug.ui32LogTick_Pause++;

		return STC_PTS_WAITING;
	}
	else
	{
		pstSync->Debug.ui32LogTick_Pause = 0;
	}

	ui32FrameDuration = VRATE_GetDuration(ui8SyncCh);
	if ( ui32FrameDuration == 0 )
	{
		log_error("[PTS%d][Err:0x%X] No Frame Rate\n", ui8SyncCh, ui32STC);
		return STC_PTS_FREERUN;
	}

	ui32StreamTime = ui32PTS & VDEC_PTS_MASK;

	// For debug : overriding the offset if dbg_disp_offset is set
	if (dbg_disp_offset)
		ui32DisplayOffset_90kTick = dbg_disp_offset * 90;
	else	// = Xms * 90,000 / 1,000
		ui32DisplayOffset_90kTick = u32DispOffsetMs * 90;

	if ( pstSync->Config.bUseGstc )
	{
		// Case for file play mode
		UINT32		ui32BaseTime;
		UINT32		ui32BasePTS;

		// if there was no basetime setting
		if ( (pstSync->Rate.ui32BaseTime == 0xFFFFFFFF) ||
				(pstSync->Rate.ui32BasePTS == 0xFFFFFFFF) )
		{
			log_noti("pts%d, cut basetime %08x, basePTS %08x\n", ui8SyncCh,
					ui32STC, ui32PTS);
			PTS_Set_BaseTime(pstSync, ui32STC, ui32PTS);
		}

		ui32BaseTime = pstSync->Rate.ui32BaseTime;
		ui32BasePTS = pstSync->Rate.ui32BasePTS;

		PTS_GET_OFFSET(ui32StreamTime, ui32BasePTS, i32PTS_Offset);

		if ( i32Speed == 1000 )
		{
			// case for normal play speed
			u32CompenPts = PTS_CHECK_PTS_WRAPAROUND(ui32BaseTime +
					i32PTS_Offset +	ui32DisplayOffset_90kTick +
					phase_shift);
			ui32DiscardDuration = ui32FrameDuration * SYNC_DCARD_THOLD;
			ui32SmthDiscardDuration = ui32FrameDuration * SYNC_SMTH_DCARD_THOLD;
			ui32MatchedDuration = ui32FrameDuration/2;
			ui32SmthWaitingDuration = ui32FrameDuration	* SYNC_SMTH_WAIT_THOLD;
			ui32WaitingDuration = ui32FrameDuration * SYNC_WAIT_THOLD;
		}
		else
		{
			UINT32 u32Speed;

			if ( i32Speed > 0 )
				u32Speed = i32Speed;
			else
				u32Speed = -i32Speed;

			if (i32PTS_Offset > 0)
				u64Temp = (UINT64)i32PTS_Offset;
			else
				u64Temp = (UINT64)(-i32PTS_Offset);

			u64Temp = u64Temp * 1000;
			do_div(u64Temp, u32Speed);
			if ((i32Speed > 0 && i32PTS_Offset > 0) ||
					(i32Speed < 0 && i32PTS_Offset < 0))
				i32PTS_Offset = (SINT32)u64Temp;
			else
				i32PTS_Offset = -((SINT32)u64Temp);

			u32CompenPts = PTS_CHECK_PTS_WRAPAROUND(ui32BaseTime + i32PTS_Offset
				+ ui32DisplayOffset_90kTick + phase_shift );
			ui32DiscardDuration = ui32FrameDuration	* SYNC_DCARD_THOLD
				/ 100 * u32Speed / 10;
			ui32SmthDiscardDuration = ui32FrameDuration	* SYNC_SMTH_DCARD_THOLD
				* u32Speed / 1000;
			ui32MatchedDuration = ui32FrameDuration * u32Speed / 1000 /2;
			ui32SmthWaitingDuration = ui32FrameDuration * SYNC_SMTH_WAIT_THOLD
				* u32Speed / 1000;
			ui32WaitingDuration = ui32FrameDuration	* SYNC_WAIT_THOLD
				/ 100 * u32Speed / 10;

			log_debug("[PTS%d] STC Match - Speed %d, BaseTime 0x%X"
					", PTS:0x%X(0x%X), STC:0x%X "
					"Intv:%u pbase %u poff %d str %u\n",
					ui8SyncCh, i32Speed, ui32BaseTime,
					u32CompenPts, ui32PTS, ui32STC,	ui32FrameDuration,
					ui32BasePTS, i32PTS_Offset, ui32StreamTime);
		}
	}
	else
	{
		u32CompenPts = PTS_CHECK_PTS_WRAPAROUND(
				ui32StreamTime + ui32DisplayOffset_90kTick + phase_shift);
		ui32DiscardDuration = ui32FrameDuration	* SYNC_DCARD_THOLD_LIVE;
		ui32SmthDiscardDuration = ui32FrameDuration	* SYNC_SMTH_DCARD_THOLD;
		ui32MatchedDuration = ui32FrameDuration/2;
		ui32SmthWaitingDuration = ui32FrameDuration	* SYNC_SMTH_WAIT_THOLD;
		ui32WaitingDuration = ui32FrameDuration	* SYNC_WAIT_THOLD_LIVE;
	}

	// For the first frame
	if ( pstSync->u32nFrame == 1 && pstSync->Config.bUseGstc )
		ui32WaitingDuration *= 4;

	u32DiscardBndary = (ui32STC - ui32DiscardDuration) & VDEC_PTS_MASK;
	u32SmthDcardBndr = (ui32STC - ui32SmthDiscardDuration) & VDEC_PTS_MASK;
	u32MatchBndary = (ui32STC - ui32MatchedDuration) & VDEC_PTS_MASK;
	u32MatchBndaryEnd = (ui32STC + ui32MatchedDuration) & VDEC_PTS_MASK;
	u32SmthWaitBndary = (ui32STC + ui32SmthWaitingDuration) & VDEC_PTS_MASK;
	u32WaitBndary = (ui32STC + ui32WaitingDuration) & VDEC_PTS_MASK;


	if ( _PTS_WithinRange(u32MatchBndary, u32MatchBndaryEnd, u32CompenPts) == TRUE )
		eStcPtsMatched = STC_PTS_MATCHED;
	else if (_PTS_WithinRange(u32DiscardBndary, u32SmthDcardBndr, u32CompenPts) == TRUE)
		eStcPtsMatched = STC_PTS_DISCARD;
	else if ( _PTS_WithinRange(u32SmthDcardBndr, u32MatchBndary, u32CompenPts) == TRUE )
		eStcPtsMatched = STC_PTS_SMOOTH_DISCARD;
	else if ( _PTS_WithinRange(u32MatchBndaryEnd, u32SmthWaitBndary, u32CompenPts) == TRUE )
		eStcPtsMatched = STC_PTS_SMOOTH_WAITING;
	else if ( _PTS_WithinRange(u32SmthWaitBndary, u32WaitBndary, u32CompenPts) == TRUE )
		eStcPtsMatched = STC_PTS_WAITING;
	else
		eStcPtsMatched = STC_PTS_FREERUN;

	if ( bSmoothing == FALSE )
	{
		if ( eStcPtsMatched == STC_PTS_SMOOTH_WAITING )
			eStcPtsMatched = STC_PTS_WAITING;
		else if ( eStcPtsMatched == STC_PTS_SMOOTH_DISCARD )
			eStcPtsMatched = STC_PTS_DISCARD;
	}

	// for handling carry over
	// eg. STC:0x0FFFFFFF PTS:0x00003000
	// Diff:0x0FFFCFFF (x)	-0x3001 (o)
	if ( (ui32STC ^ u32CompenPts) & (~(VDEC_PTS_MASK>>2)) )
	{
		if ( ui32STC > u32CompenPts )
			pstSync->i32SyncDiff = PTS_GET_PTS_OFFSET( ui32STC, u32CompenPts );
		else
			pstSync->i32SyncDiff = PTS_GET_PTS_OFFSET( u32CompenPts, ui32STC );
	}
	else
		pstSync->i32SyncDiff = ui32STC - u32CompenPts;

	log_user1("[PTS%d, %4d] %s SMTH%d PTS:%u(+%u), STC:%u dif:%d "
			"Qlen %d, Duration:0x%X\n",
			ui8SyncCh, pstSync->u32nFrame,
			azMatchType[eStcPtsMatched], bSmoothing, ui32PTS,
			u32CompenPts, ui32STC, pstSync->i32SyncDiff,
			DISP_Q_NumActive(ui8SyncCh), ui32FrameDuration );

	pstSync->Debug.ui32LogTick_Match++;
	if ( pstSync->Debug.ui32LogTick_Match == 0x100 )
	{
		log_noti(
				"[PTS%d][DBG:0x%X] speed %d Base(%d %d) (%d/%d) Offset90K:%d "
				"PTS:0x%X(0x%X) STC:0x%X Diff:%dms\n",
				ui8SyncCh, ui32STC,
				i32Speed,
				pstSync->Rate.ui32BaseTime,
				pstSync->Rate.ui32BasePTS,
				pstSync->Debug.ui32MatchedFrameCount,
				pstSync->Debug.ui32MatchingFrameDuration,
				ui32DisplayOffset_90kTick,
				ui32PTS, u32CompenPts, ui32STC,
				((SINT32)ui32STC - (SINT32)u32CompenPts) / 90);

		pstSync->Debug.ui32LogTick_Match = 0;
		pstSync->Debug.ui32MatchedFrameCount = 0;
		pstSync->Debug.ui32MatchingFrameDuration = 0;
	}

	return eStcPtsMatched;
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
static PTS_MATCH_E _PTS_DecideFrameMatch(SYNC_T *pstSync,
		PTS_CONTINUITY_E ePtsCont, UINT32 ui32STC, UINT32 ui32PTS_Prev,
		UINT32 ui32PTS, UINT32 u32DispOffsetMs, BOOLEAN bSmoothing)
{
	PTS_MATCH_E	eStcPtsMatched = STC_PTS_NONE;
	static PTS_MATCH_E ePreStcMatch[VDISP_NUM_OF_CHANNEL];
	UINT8 ui8SyncCh = pstSync->ui8SyncCh;

	switch ( ePtsCont )
	{
	case PTS_CONTINUITY_SAME :
	case PTS_CONTINUITY_NORMAL :
		eStcPtsMatched = _PTS_CheckStcPtsMatch(pstSync, ui32STC, ui32PTS,
				u32DispOffsetMs, bSmoothing);
		break;
	case PTS_CONTINUITY_JITTER :
		pstSync->u32PtsContnCnt = 0;
		break;
	case PTS_CONTINUITY_INVALID_STC :
	case PTS_CONTINUITY_WRAPAROUND :
		log_debug("[PTS%d][Warning:0x%X] PTS Continuity(%s) (Running:%d) - "
				"Prev:0x%X, Curr:0x%X, STC:0x%X, Occupancy: %d\n",
				ui8SyncCh, ui32STC, conti_string[(UINT32)ePtsCont],
				pstSync->ePtsCont, ui32PTS_Prev, ui32PTS, ui32STC,
				DISP_Q_NumActive(ui8SyncCh) );

		eStcPtsMatched = STC_PTS_FREERUN;
		break;

	default :
		log_error("[PTS%d][Err:0x%X] PTS eMatch %d, %s\n",
				ui8SyncCh, ui32STC, eStcPtsMatched, __FUNCTION__ );
	}

	if ( ePtsCont == PTS_CONTINUITY_NORMAL )
		pstSync->u32PtsContnCnt++;
	else
		pstSync->u32PtsContnCnt = 0;

	// freerun for jitter continuity mode
	if ( pstSync->ePtsCont == PTS_CONTINUITY_JITTER )
	{
		// turn to normal continuity mode from jitter continuity mode
		// after several freerun
		if ( pstSync->u32PtsContnCnt >= PTS_CONTINUITY_COUNT )
		{
			log_noti("[PTS%d] Restore Normal State from Jitter PTS, "
					"STC:0x%X, DispQ len: %d\n",
					ui8SyncCh, ui32STC, DISP_Q_NumActive(ui8SyncCh) );
			pstSync->ePtsCont = PTS_CONTINUITY_NORMAL;
		}
		else
		{
			log_debug("[PTS%d] Jitter freerun, cnt%d\n", ui8SyncCh,
					pstSync->u32PtsContnCnt);
			pstSync->ePtsCont = PTS_CONTINUITY_JITTER;
			eStcPtsMatched = STC_PTS_FREERUN;
		}

		// keep frames from displaying by freerun
		// in video pause status
		if (VRATE_GetSpeed(ui8SyncCh) == 0)
		{
			if ( (pstSync->Debug.ui32LogTick_Pause % 10) == 0 )
				log_noti("[PTS%d][STC:0x%X] STC Speed 0, PTS:%u Qlen %d\n",
						ui8SyncCh, ui32STC,	ui32PTS,
						DISP_Q_NumActive(ui8SyncCh));
			pstSync->Debug.ui32LogTick_Pause++;

			eStcPtsMatched = STC_PTS_WAITING;
		}
	}
	else
	{
		pstSync->ePtsCont = ePtsCont;
	}

	// log for transition of frame stc match mode
	if ( eStcPtsMatched != ePreStcMatch[ui8SyncCh] )
		log_noti("[PTS%d][DBG] ContiState %s, Stc Match %s -> %s\n", ui8SyncCh,
				conti_string[(UINT32)ePtsCont],
				azMatchType[ePreStcMatch[ui8SyncCh]],
				azMatchType[eStcPtsMatched] );

	ePreStcMatch[ui8SyncCh] = eStcPtsMatched;

	return eStcPtsMatched;
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
UINT32 PTS_CheckFrame(SYNC_T *pstSync, S_DISPQ_BUF_T *pstDqFrame)
{
	UINT32		u32Ret = 0;
	UINT32		ui32PTS_Curr;
	UINT8 ui8SyncCh = pstSync->ui8SyncCh;

	// switch pts to enqueued gstc for calculation of elapsed time in queue
	if (pstSync->Config.eMatchMode == PTS_MATCH_FREERUN_IGNORE_SYNC)
		pstDqFrame->ui32PTS = pstDqFrame->enq_gstc;

	// work-around for H13A0
	if ( bDualVsyncForProgFrame	&&
			DISPQ_SCAN_PROG == pstDqFrame->ui32DisplayInfo )
		pstDqFrame->ui32DisplayPeriod = 2;

	// PTS check
	if ( pstDqFrame->ui32PTS == VDEC_UNKNOWN_PTS )
		ui32PTS_Curr = pstSync->ui32PTS_Prev;
	else
	{
		pstDqFrame->ui32PTS &= VDEC_PTS_MASK;
		ui32PTS_Curr = pstDqFrame->ui32PTS;
	}

	// for calculation of PTS rate
	if (pstDqFrame->ui32LR_Order != DISPQ_3D_FRAME_RIGHT)
		VRATE_UpdateByPTS(ui8SyncCh, ui32PTS_Curr,
			(UINT32 *)&pstSync->Rate.ui32PtsRateRes,
			(UINT32 *)&pstSync->Rate.ui32PtsRateDiv);

	pstSync->ui32PTS_Prev = ui32PTS_Curr;
	pstSync->Rate.bInterlaced =
		pstDqFrame->ui32DisplayInfo == DISPQ_SCAN_PROG?FALSE:TRUE;

	if ( pstSync->u32nFrame < 0xFFFFFFFF )
		pstSync->u32nFrame++;

	return u32Ret;
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
UINT32	PTS_GetDisplayOffsetMs(SYNC_T *pstSync)
{
	UINT32 ui32OffsetMs;

	ui32OffsetMs = pstSync->Config.u32DispOffsetMs;

	// Live for Channel Change  // Not 3D Channel
	if ( pstSync->Config.bUseGstc == FALSE &&
			pstSync->Config.bIsDualDecoding == FALSE )
	{
		// First & Second Frame
		if (	pstSync->u32nFrame <= gu32nFastChChFrame &&
				pstSync->Config.u32DispOffsetMs >= 100 )
		{
			ui32OffsetMs -= 100;

			log_noti("[PTS%d] DispOffset: %d --> %d for Fast Channel Change\n",
					pstSync->ui8SyncCh, pstSync->Config.u32DispOffsetMs,
					ui32OffsetMs );
		}
	}

	return ui32OffsetMs;
}

void PTS_SetDiscardLv(UINT8 u8Lv)
{
	gu8DiscardLv = u8Lv;
}

void PTS_NotiCurQlen(SYNC_T *pstSync, SINT32 i32QLen)
{
	pstSync->u32QLen = (UINT32)i32QLen;

	if (pstSync->Config.eMatchMode != PTS_MATCH_ADAPTIVE_FREERUN)
		return;
}

PTS_MATCH_E _sync_buf_control(SYNC_T *pstSync)
{
	PTS_MATCH_E		eMatch = STC_PTS_FREERUN;
	UINT32			qlen, adp_qlen;

	qlen = pstSync->u32QLen;
	adp_qlen = pstSync->u32AdpMaxQLen;

	if (qlen < adp_qlen)
		pstSync->short_qlen_cnt++;
	else
		pstSync->short_qlen_cnt=0;

	if (qlen > adp_qlen)
		pstSync->long_qlen_cnt++;
	else
		pstSync->long_qlen_cnt=0;

	if (qlen == adp_qlen)
	{
		pstSync->short_qlen_thold = 300;
	}
	else if (qlen < adp_qlen)
	{
		if (pstSync->short_qlen_cnt > pstSync->short_qlen_thold)
		{
			eMatch = STC_PTS_WAITING;
			pstSync->short_qlen_cnt = 0;
		}
	}
	else if (!(pstSync->long_qlen_cnt&3))
	{
		if (qlen == adp_qlen+1)
		{
			if (pstSync->long_qlen_cnt > 300)
			{
				eMatch = STC_PTS_DISCARD;
				pstSync->long_qlen_cnt = 0;
			}
		}
		else
			eMatch = STC_PTS_DISCARD;
	}

	log_user1("pts%d: adp sync -> %s, qlen%d adp_lv%d cnt S%d L%d\n",
			pstSync->ui8SyncCh, azMatchType[eMatch],
			pstSync->u32QLen, pstSync->u32AdpMaxQLen,
			pstSync->short_qlen_cnt, pstSync->long_qlen_cnt);

	return eMatch;
}

PTS_MATCH_E PTS_MatchCheck(SYNC_T *pstSync, UINT32 ui32PTS,	UINT32 ui32STC,
		UINT32 ui32PTS_Prev)
{
	PTS_MATCH_E			eMatch = STC_PTS_NONE;
	PTS_CONTINUITY_E	ePtsCont = PTS_CONTINUITY_NONE;
	UINT32				u32DispOffsetMs = 0;

	SINT32 				i32Speed;
	BOOLEAN				bSmoothing = TRUE;
	UINT8 				ui8SyncCh;

	if ( pstSync == NULL )
	{
		log_error("[PTS] null pointer\n");
		return FALSE;
	}

	ui8SyncCh = pstSync->ui8SyncCh;

	i32Speed = VRATE_GetSpeed(ui8SyncCh);

	// compare main frame rate with PTS rate.
	// if they are different we don't use smooth mode because frames will be
	// not distributed evenly on time domain
	if (VRATE_IsModifiedRate(ui8SyncCh) == TRUE ||
			pstSync->bSmoothEn == FALSE )
		bSmoothing = FALSE;

	switch (pstSync->Config.eMatchMode)
	{
		case PTS_MATCH_ENABLE :
		if (ui32PTS == VDEC_UNKNOWN_PTS)
		{
			eMatch = STC_PTS_FREERUN;
		}
		else
		{
			// Check PTS discontinuity
			ePtsCont = _PTS_CheckPtsDiscont(pstSync, ui32PTS_Prev,
					ui32PTS, ui32STC);

			u32DispOffsetMs = PTS_GetDisplayOffsetMs(pstSync);

			// use smooth mode only for 1x speed, because frame must be
			// discarded exact rate in the case over 1x speed.
			if ( (pstSync->Config.bIsDualDecoding == TRUE) || (i32Speed != 1000))
				bSmoothing = FALSE;

			// decide how to handle one frame
			eMatch = _PTS_DecideFrameMatch(pstSync,	ePtsCont, ui32STC,
					ui32PTS_Prev, ui32PTS, u32DispOffsetMs, bSmoothing);
		}
		break;
		case PTS_MATCH_FREERUN_IGNORE_SYNC :
			// for camera.
			// compute sync diff, this will be used for
			// vsync interval control for buffering level
			pstSync->i32SyncDiff = TOP_HAL_GetGSTCC() - ui32PTS;
			// TODO: handling bit underflow
			//pstSync->i32SyncDiff |= PTS_BIT_MASK;

			// check buffering level.
			// if buffering level is too high we will give up buffering level
			// control by vsync control and we will discard frames to make the
			// buffering level stay low
			if (pstSync->u32QLen <= pstSync->u8DiscardLv)
				eMatch = STC_PTS_FREERUN;
			else
				eMatch = STC_PTS_DISCARD;

			//eMatch = STC_PTS_DISCARD;
			log_user1 ("pts%d, freerun ignore sync, mch %d len %d\n",
					ui8SyncCh, eMatch, pstSync->u32QLen);
			break;
		case PTS_MATCH_ADAPTIVE_FREERUN :
			// for widi, miracast, cloud game.
			// buffer underrun make max buffering level be enlarge so that
			// buffering underrun doesn't occur again.
			// but if there are enough buffering we will shrink max buffering
			// level so that display delay become low
			eMatch = _sync_buf_control(pstSync);
			break;
		case PTS_MATCH_FREERUN_BASED_SYNC :
			eMatch = STC_PTS_FREERUN;
			break;
		default :
			log_error("[PTS%d][Err] Match Mode:%d, %s\n", ui8SyncCh,
					pstSync->Config.eMatchMode, __FUNCTION__ );
	}

	if ( eMatch == STC_PTS_MATCHED )
	{
		// do not turn on smooth mode until
		// finish the fast channel change
		if ( pstSync->u32nFrame > gu32nFastChChFrame)
		{
			pstSync->bSmoothEn = TRUE;
			pstSync->u32SmWaitCnt = 0;
			pstSync->u32SmDiscardCnt = 0;
		}
	}
	else if ( eMatch == STC_PTS_SMOOTH_DISCARD )
	{
		// for smooth discard
		// we will give the frame some chance that not to be discarded.
		// but if there are consecutive smooth discard
		// the frame will be discarded
		pstSync->u32SmDiscardCnt++;
		eMatch = STC_PTS_DISCARD;

		if ( pstSync->u32SmDiscardCnt < 4 )
			eMatch = STC_PTS_MATCHED;
		else if ( pstSync->u32SmDiscardCnt < 20 )
		{
			if ( pstSync->u32SmDiscardCnt&3 )
				eMatch = STC_PTS_MATCHED;
		}
		else if ( pstSync->u32SmDiscardCnt < 40 )
		{
			if ( pstSync->u32SmDiscardCnt&1 )
				eMatch = STC_PTS_MATCHED;
		}
		else if ( pstSync->u32SmDiscardCnt < 80 )
		{
			if ( !(pstSync->u32SmDiscardCnt&3) )
				eMatch = STC_PTS_MATCHED;
		}
	}
	else if ( eMatch == STC_PTS_SMOOTH_WAITING )
	{
		// similar with smooth discard,
		// consecutive smooth waiting make the frame wait and
		// not be displayed until being matched
		pstSync->u32SmWaitCnt++;
		eMatch = STC_PTS_WAITING;

		if ( pstSync->u32SmWaitCnt < 4 )
			eMatch = STC_PTS_MATCHED;
		else if ( pstSync->u32SmWaitCnt < 20 )
		{
			if ( pstSync->u32SmWaitCnt&3 )
				eMatch = STC_PTS_MATCHED;
		}
		else if ( pstSync->u32SmWaitCnt < 40 )
		{
			if ( pstSync->u32SmWaitCnt&1 )
				eMatch = STC_PTS_MATCHED;
		}
		else if ( pstSync->u32SmWaitCnt < 80 )
		{
			if ( !(pstSync->u32SmWaitCnt&3) )
				eMatch = STC_PTS_MATCHED;
		}
	}

	pstSync->u32ContnFrmCnt++;
	pstSync->Debug.ui32LogTick_GetNewNo = 0;
/*
	// log for debug
	pstSync->Debug.ui32LogTick_GetNew++;
	if ( pstSync->Debug.ui32LogTick_GetNew == 0x100 )
	{
		log_noti("[PTS%d][DBG:0x%X] DQlen:%d, Match Mode:%d, "
			"Offset(Conf %d Cur %d)\n",
			ui8SyncCh, ui32STC,
			DISP_Q_NumActive(ui8SyncCh),
			pstSync->Config.eMatchMode,
			pstSync->Config.u32DispOffsetMs,
			PTS_GetDisplayOffsetMs(pstSync)
			);

		pstSync->Debug.ui32LogTick_GetNew = 0;
	}
*/
	pstSync->ui32STC_Prev = ui32STC;

	return eMatch;
}

void _sync_print_channel(struct seq_file *m, SYNC_T *pstSync)
{
	seq_printf(m,"===  SYNC #%d [0x%X]  ===\n",
			pstSync->ui8SyncCh, (UINT32)pstSync);

#define VDS_DPRINT(v,f)		\
	seq_printf(m, #v" = "f"\n", pstSync->v)

	VDS_DPRINT(ui8SyncCh, "%u");
	VDS_DPRINT(u32nFrame, "%u");
	VDS_DPRINT(bSmoothEn, "%u");
	VDS_DPRINT(u32SmWaitCnt, "%u");
	VDS_DPRINT(u32SmDiscardCnt, "%u");
	VDS_DPRINT(ui32PTS_Prev, "%u");
	VDS_DPRINT(ui32STC_Prev, "%u");
	VDS_DPRINT(ePtsCont, "%u");
	VDS_DPRINT(u32PtsContnCnt, "%u");
	VDS_DPRINT(u32ContnFrmCnt, "%u");
	VDS_DPRINT(i32SyncDiff, "%d");
	VDS_DPRINT(u32EmptyCnt10, "%u");
	VDS_DPRINT(u32AdpMaxQLen, "%u");
	VDS_DPRINT(u32AdpOverCnt, "%u");
	VDS_DPRINT(u32QLen, "%u");
	VDS_DPRINT(u8DiscontCnt, "%u");
#undef VDS_DPRINT

#define VDS_DPRINT(v,f)		\
	seq_printf(m, #v" = "f"\n", pstSync->Config.v)
#define VDS_DPRINT2(f,v)		\
	seq_printf(m, f"\n", v)

	VDS_DPRINT(bUseGstc, "%u");
	VDS_DPRINT2("eMatchMode = %s", azMatchMode[pstSync->Config.eMatchMode]);
	VDS_DPRINT(u32DispOffsetMs, "%u");
	VDS_DPRINT(bIsDualDecoding, "%u");
#undef VDS_DPRINT

#define VDS_DPRINT(v,f)		\
	seq_printf(m, #v" = "f"\n", pstSync->Rate.v)
	VDS_DPRINT(ui32PtsRateRes, "%u");
	VDS_DPRINT(ui32PtsRateDiv, "%u");
	VDS_DPRINT(ui32BaseTime, "%u");
	VDS_DPRINT(ui32BasePTS, "%u");
	VDS_DPRINT(bInterlaced, "%u");
#undef VDS_DPRINT

	seq_printf(m,"\n");
}

void SYNC_PrintDebug(struct seq_file *m)
{
	SYNC_T *pstSync;
	UINT32 i;

	seq_printf(m," phase_shift : %d\n\n", phase_shift);

	for (i=0;i<VDISP_NUM_OF_CHANNEL;i++)
	{
		pstSync = PTS_GetHandler(i);
		_sync_print_channel(m, pstSync);
	}
}
