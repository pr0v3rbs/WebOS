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
 * date       2011.02.21
 * note       Additional information.
 *
 * @addtogroup lg115x_vdec
 * @{
 */

/*-----------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------*/
#define LOG_NAME	vdec_rate

/*-----------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/seq_file.h>
#include "asm/div64.h"

#include "vdec_rate.h"
#include "vsync_drv.h"
#include "pts_drv.h"

#include "../hal/ipc_reg_api.h"
#include "../hal/vdec_base.h"

#include "log.h"

/*-----------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------*/
#define	VDEC_RATE_NUM_OF_CHANNEL				VDISP_NUM_OF_CHANNEL
#define	VRATE_N_HISTORY_BUF		0x100
#define	VDEC_RATE_MAX_DIFF_HISTORY_NUM			\
							(0xFFFFFFFF / VRATE_N_HISTORY_BUF)
/*-----------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------*/
#define	VDEC_DIFF(_a, _b)		(((_a) >= (_b)) ? (_a) - (_b) : (_b) - (_a))

/*-----------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------*/

typedef struct
{
	UINT32		ui32_Prev;
	UINT32		u32DiffHistory[VRATE_N_HISTORY_BUF];
	BOOLEAN		bChanged;
	UINT32		u32BufIdx;
	UINT32		u32DiffSum;
	UINT32		u32HistoryCnt;
	UINT32		u32Cnt;
	UINT32		stc_diff;
	BOOLEAN		stable;
	SINT32		stable_cnt;
	UINT32		ui32FrameRateRes;
	UINT32		ui32FrameRateDiv;
	UINT32		u32Duration;
} VRATE_AVERAGE_T;

typedef struct
{
	UINT32		u32Id;
	SINT32		i32Speed;		// 1x: 1000, 2x: 2000, ...
	BOOLEAN		bInterlaced;
	UINT32		ui32FrameRateLimit;
	BOOLEAN		bUpdateNeed;
	BOOLEAN		fract_speed;
	BOOLEAN		bUseGSTC;
	struct
	{
		BOOLEAN	bChanged;
		UINT32	u32RateResOrg;
		UINT32	u32RateDivOrg;
		UINT32	u32Duration90kOrg;
		UINT32	u32RateRes;
		UINT32	u32RateDiv;
		UINT32	u32Duration90k;
	} Config;
	struct
	{
		BOOLEAN	bChanged;
		UINT32	u32RateResOrg;
		UINT32	u32RateDivOrg;
		UINT32	u32Duration90kOrg;
		UINT32	u32RateRes;
		UINT32	u32RateDiv;
		UINT32	u32Duration90k;
	} Codec;
	struct
	{
		BOOLEAN	bUse;
		UINT32	u32RateRes;
		UINT32	u32RateDiv;
		UINT32	u32Duration90k;
	} Debug;
	VRATE_AVERAGE_T DQ_Pop;
	VRATE_AVERAGE_T PTS;
	VRATE_AVERAGE_T dq_pts;
	struct
	{
		RATE_SRC_E	eRateSrc;
		UINT32	u32RateRes;
		UINT32	u32RateDiv;
		// frame duration @ 90KHz clock tick.
		//  valid only after sequence header found.
		UINT32	u32Duration90k;
	} Status;
} RATE_T;

/*------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------*/
static RATE_T gsVdecRate[VDEC_RATE_NUM_OF_CHANNEL];
static char rate_src_str[RATE_SRC_MAX][10] =
{
	[RATE_SRC_NONE  	] = "NONE",
	[RATE_SRC_PTS     	] = "PTS",
	[RATE_SRC_CONF    	] = "CONF",
	[RATE_SRC_CODEC   	] = "CODEC",
	[RATE_SRC_CODEC_DBL	] = "CODEC_DBL",
	[RATE_SRC_DEBUG		] = "DEBUG"
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
void VRATE_Init(void)
{
	RATE_T			*pstRate;
	UINT32			i;

	log_noti("[RATE][DBG] Init Vdec Rate Drv, %s\n", __FUNCTION__);

	for (i=0;i<VDEC_RATE_NUM_OF_CHANNEL;i++)
	{
		pstRate = &gsVdecRate[i];
		pstRate->Debug.bUse = FALSE;
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
static void _VRATE_ResetMovingAverage(VRATE_AVERAGE_T *pMovingAverage)
{
	pMovingAverage->ui32_Prev = 0;
	pMovingAverage->u32BufIdx = 0;
	pMovingAverage->u32DiffSum = 0;
	pMovingAverage->u32HistoryCnt = 0xFFFFFFFF;
	pMovingAverage->u32Cnt = 0;
	pMovingAverage->stc_diff = 0xFFFFFFFF;
	pMovingAverage->stable = FALSE;
	pMovingAverage->stable_cnt = 0;
	pMovingAverage->ui32FrameRateRes = 0;
	pMovingAverage->ui32FrameRateDiv = 0xFFFFFFFF;
	pMovingAverage->u32Duration = 0xFFFFFFFF;
	pMovingAverage->bChanged = FALSE;
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

void VRATE_SetUseGSTC(UINT8 ui8SyncCh, BOOLEAN bUseGstc)
{
	RATE_T		*pstRate;

	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number %s\n", ui8SyncCh, __FUNCTION__);
		return;
	}

	pstRate = &gsVdecRate[ui8SyncCh];
	
	pstRate->bUseGSTC = bUseGstc;

}

void VRATE_Reset(UINT8 ui8SyncCh)
{
	RATE_T		*pstRate;

	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number %s\n", ui8SyncCh, __FUNCTION__);
		return;
	}

	pstRate = &gsVdecRate[ui8SyncCh];

	log_noti("[RATE%d][DBG] %s\n", ui8SyncCh, __FUNCTION__);

	pstRate->u32Id = ui8SyncCh;
	pstRate->i32Speed = 1000;
	pstRate->bInterlaced = TRUE;
	pstRate->ui32FrameRateLimit = VDEC_FRAMERATE_MAX;
	pstRate->bUpdateNeed = FALSE;
	pstRate->fract_speed = FALSE;

	pstRate->Config.bChanged = FALSE;
	pstRate->Config.u32RateResOrg = 0x0;
	pstRate->Config.u32RateDivOrg = 0xFFFFFFFF;
	pstRate->Config.u32Duration90kOrg = 0;
	pstRate->Config.u32RateRes = 0x0;
	pstRate->Config.u32RateDiv = 0xFFFFFFFF;
	pstRate->Config.u32Duration90k = 0;

	pstRate->Codec.bChanged = FALSE;
	pstRate->Codec.u32RateResOrg = 0x0;
	pstRate->Codec.u32RateDivOrg = 0xFFFFFFFF;
	pstRate->Codec.u32Duration90kOrg = 0;
	pstRate->Codec.u32RateRes = 0x0;
	pstRate->Codec.u32RateDiv = 0xFFFFFFFF;
	pstRate->Codec.u32Duration90k = 0;

	_VRATE_ResetMovingAverage(&pstRate->DQ_Pop);
	_VRATE_ResetMovingAverage(&pstRate->PTS);
	_VRATE_ResetMovingAverage(&pstRate->dq_pts);

	pstRate->Status.eRateSrc = RATE_SRC_NONE;
	pstRate->Status.u32RateRes = VDEC_DEFAULT_FRAME_RATE_RES;
	pstRate->Status.u32RateDiv = VDEC_DEFAULT_FRAME_RATE_DIV;
	pstRate->Status.u32Duration90k =
		VSync_CalFrameDuration(VDEC_DEFAULT_FRAME_RATE_RES,
			VDEC_DEFAULT_FRAME_RATE_DIV);
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
void VRATE_Flush(UINT8 ui8SyncCh)
{
	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return;
	}

	log_noti("[RATE%d][DBG] %s\n", ui8SyncCh, __FUNCTION__);

	_VRATE_ResetMovingAverage(&gsVdecRate[ui8SyncCh].DQ_Pop);
	_VRATE_ResetMovingAverage(&gsVdecRate[ui8SyncCh].PTS);
	_VRATE_ResetMovingAverage(&gsVdecRate[ui8SyncCh].dq_pts);
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
BOOLEAN VRATE_SetSpeed(UINT8 ui8SyncCh, SINT32 i32Speed)
{
	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return FALSE;
	}

	log_noti("[RATE%d][DBG] Speed: %d --> %d\n",
			ui8SyncCh, gsVdecRate[ui8SyncCh].i32Speed, i32Speed);

#if defined(CHIP_NAME_d14)
	if (gsVdecRate[ui8SyncCh].bUseGSTC == TRUE)
	{
		if (gsVdecRate[ui8SyncCh].fract_speed)
		{
			log_trace("[RATE%d] fraction rate: change speed to x1.001", ui8SyncCh);
			i32Speed = i32Speed * 1001 / 1000;
		}
	}
#endif

	if (i32Speed == gsVdecRate[ui8SyncCh].i32Speed)
	{
		return FALSE;
	}

	gsVdecRate[ui8SyncCh].i32Speed = i32Speed;

	return TRUE;
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
SINT32 VRATE_GetSpeed(UINT8 ui8SyncCh)
{
	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return 0;
	}

	return gsVdecRate[ui8SyncCh].i32Speed;
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
static UINT32 _VRATE_GetAvgIntv90K(RATE_T *rate, BOOLEAN bConfig,
		BOOLEAN bCodec, BOOLEAN bTimeStamp)
{
	UINT32		u32CfgRate = 0;
	UINT32		u32CodecRate = 0;
	UINT32		u32PtsRate = 0;
	UINT32		u32AvrgDuration = 0;
	UINT32		u32NumOfSrc = 0;
	SINT32		i32Speed;

	i32Speed = rate->i32Speed;

	if (bConfig == TRUE && rate->Config.u32RateRes && rate->Config.u32RateDiv)
		u32CfgRate = rate->Config.u32RateRes / rate->Config.u32RateDiv;
	if (bCodec == TRUE && rate->Codec.u32RateRes && rate->Codec.u32RateDiv)
		u32CodecRate = rate->Codec.u32RateRes / rate->Codec.u32RateDiv;

	if (u32CfgRate >= VDEC_FRAMERATE_MIN && u32CfgRate <= VDEC_FRAMERATE_MAX)
	{
		u32AvrgDuration += VSync_CalFrameDuration(
				rate->Config.u32RateRes, rate->Config.u32RateDiv);
		u32NumOfSrc++;
	}

	if (u32CodecRate >= VDEC_FRAMERATE_MIN &&
			u32CodecRate <= VDEC_FRAMERATE_MAX)
	{
		u32AvrgDuration += VSync_CalFrameDuration(
				rate->Codec.u32RateRes,	rate->Codec.u32RateDiv);
		u32NumOfSrc++;
	}

	if (bTimeStamp == TRUE && rate->PTS.ui32FrameRateRes
			&& rate->PTS.ui32FrameRateDiv)
	{
		u32PtsRate = rate->PTS.ui32FrameRateRes / rate->PTS.ui32FrameRateDiv;
		u32PtsRate = u32PtsRate * i32Speed / 1000;
	}

	if (u32PtsRate >= VDEC_FRAMERATE_MIN &&
			u32PtsRate <= VDEC_FRAMERATE_MAX)
	{
		u32AvrgDuration += VSync_CalFrameDuration(
				rate->PTS.ui32FrameRateRes, rate->PTS.ui32FrameRateDiv);
		u32NumOfSrc++;
	}

	log_user2("PTS Intv %d/%d	AVG Intv %d/%d\n",
			rate->PTS.ui32FrameRateRes, rate->PTS.ui32FrameRateDiv,
			u32AvrgDuration, u32NumOfSrc);

	if (u32NumOfSrc)
		u32AvrgDuration /= u32NumOfSrc;

	return u32AvrgDuration;
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

static RATE_SRC_E _VRATE_SelConfDecFrameRate(RATE_T *rate)
{
	RATE_SRC_E	eRateSrc, eIntvSmall, eIntvLarge;
	UINT32		u32FrmIntvAvg = 0xFFFFFFFF;	// Frame Interval Average
	UINT32		u32FrmIntvConf = 0;	// Frame Interval from Config
	UINT32		u32FrmIntvCodec = 0;// Frmae Interval from Codec
	UINT32		u32FrmIntvLarge;
	UINT32		u32FrmIntvSmall;
	UINT32		u32FrmIntvThold;

	if (rate->Debug.bUse)
		return RATE_SRC_DEBUG;

	// Config: values from container(upper layer)
	// Codec: values from vpu(BODA, etc.)
	if (rate->Config.u32RateRes && rate->Config.u32RateDiv != 0xFFFFFFFF)
		u32FrmIntvConf = rate->Config.u32Duration90k;
	if (rate->Codec.u32RateRes && rate->Codec.u32RateDiv != 0xFFFFFFFF)
		u32FrmIntvCodec = rate->Codec.u32Duration90k;

	////////////////////////////////////////////////////////////////////
	// 		Decide More Close Frame Rate from AVG Frame Rate
	//		Between Config FR and Codec FR
	////////////////////////////////////////////////////////////////////
	if (u32FrmIntvConf && u32FrmIntvCodec)
	{
		if (u32FrmIntvConf == u32FrmIntvCodec)
		{
			eRateSrc = RATE_SRC_CONF;
		}
		else
		{
			if (u32FrmIntvConf > u32FrmIntvCodec)
			{
				u32FrmIntvLarge = u32FrmIntvConf;
				u32FrmIntvSmall = u32FrmIntvCodec;
				eIntvSmall = RATE_SRC_CODEC;
				eIntvLarge = RATE_SRC_CONF;
			}
			else
			{
				u32FrmIntvLarge = u32FrmIntvCodec;
				u32FrmIntvSmall = u32FrmIntvConf;
				eIntvSmall = RATE_SRC_CONF;
				eIntvLarge = RATE_SRC_CODEC;
			}

			if (u32FrmIntvLarge > (u32FrmIntvSmall * 12/10))
			{
				if (rate->Status.eRateSrc == eIntvSmall)
				{
					u32FrmIntvThold = u32FrmIntvSmall +
						(u32FrmIntvLarge - u32FrmIntvSmall) * 9/10;
				}
				if (rate->Status.eRateSrc == eIntvLarge)
				{
					u32FrmIntvThold = u32FrmIntvSmall +
						(u32FrmIntvLarge - u32FrmIntvSmall) * 1/10;
				}
				else
				{
					u32FrmIntvThold = u32FrmIntvSmall +
						(u32FrmIntvLarge - u32FrmIntvSmall) * 5/10;
				}

				u32FrmIntvAvg = _VRATE_GetAvgIntv90K(rate, FALSE, FALSE, TRUE);

				if (u32FrmIntvAvg == 0)
					eRateSrc = RATE_SRC_CONF;
				else if (u32FrmIntvAvg < u32FrmIntvThold)
					eRateSrc = eIntvSmall;
				else
					eRateSrc = eIntvLarge;

				log_noti("[RATE%d] Sel:%s, Small %d(%s) Large %d(%s) "
						"Thold %d AVRG %d\n",
						rate->u32Id, rate_src_str[eRateSrc],
						u32FrmIntvSmall, rate_src_str[eIntvSmall],
						u32FrmIntvLarge, rate_src_str[eIntvLarge],
						u32FrmIntvThold, u32FrmIntvAvg);
			}
			else
			{
				// if small diffrence
				eRateSrc = eIntvSmall;
			}
		}
	}
	else if (u32FrmIntvConf)
	{
		eRateSrc = RATE_SRC_CONF;
	}
	else if (u32FrmIntvCodec)
	{
		u32FrmIntvAvg = _VRATE_GetAvgIntv90K(rate, FALSE, FALSE, TRUE);
		//codec frame rate < 15.5Hz  && codec frame rate < avg(codec,config,pts)9/5
		if (u32FrmIntvCodec > 6000-200 && u32FrmIntvCodec > u32FrmIntvAvg*9/5)
		{
			static UINT32 u32DbgCnt;
			// wrong codec frame rate suspected
			if (!((u32DbgCnt++)%100))
				log_noti("wrong frame rate suspected: frm interval codec %d "
						"pts %d\n",	u32FrmIntvCodec, u32FrmIntvAvg);

			eRateSrc = RATE_SRC_CODEC_DBL;
		}
		else
			eRateSrc = RATE_SRC_CODEC;
	}
	else
	{
		eRateSrc = RATE_SRC_PTS;
		log_debug("[RATE%d] No Rate Info, Config:%d, Codec:%d\n",
			rate->u32Id, u32FrmIntvConf, u32FrmIntvCodec);
	}

	log_debug("[RATE%d] Sel Src:%s, PTS_AVG %d  Conf:%d, Codec:%d\n",
			rate->u32Id, rate_src_str[eRateSrc],
			_VRATE_GetAvgIntv90K(rate, FALSE, FALSE, TRUE),
			u32FrmIntvConf, u32FrmIntvCodec);

	return eRateSrc;
}

void VRATE_SetRateLimit(UINT8 ui8SyncCh, UINT32 ui32RateLimit)
{
	if (gsVdecRate[ui8SyncCh].ui32FrameRateLimit != ui32RateLimit)
	{
		log_noti("[rate %d] rate limit changed %d -> %d\n", ui8SyncCh,
				gsVdecRate[ui8SyncCh].ui32FrameRateLimit, ui32RateLimit);

		gsVdecRate[ui8SyncCh].ui32FrameRateLimit = ui32RateLimit;
		gsVdecRate[ui8SyncCh].bUpdateNeed = TRUE;
		gsVdecRate[ui8SyncCh].Config.bChanged = TRUE;
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
static BOOLEAN _VRATE_CorrectCodecRate(RATE_T *rate,
		UINT32 *pui32FrameRateRes, UINT32 *pui32FrameRateDiv)
{
	BOOLEAN		bCorrect = FALSE;

	UINT32		u32PreDecDura = 0;
	UINT32		u32CurDecDura = 0;
	UINT32		u32PtsDura;
	UINT32		u32DecDuraDiff = 0;
	UINT32		ui32FrameRateRes;
	UINT32		ui32FrameRateDiv;
	UINT32		ui32FrameRate_PTS;

	ui32FrameRateRes = *pui32FrameRateRes;
	ui32FrameRateDiv = *pui32FrameRateDiv;

	u32PreDecDura = VSync_CalFrameDuration(
			rate->Codec.u32RateRes, rate->Codec.u32RateDiv);
	u32CurDecDura = VSync_CalFrameDuration(ui32FrameRateRes, ui32FrameRateDiv);
	u32DecDuraDiff = VDEC_DIFF(u32PreDecDura, u32CurDecDura);

	ui32FrameRate_PTS = VRATE_GetPTSRate(rate->u32Id, NULL, NULL);

	if (u32DecDuraDiff >= 1 && u32DecDuraDiff <= 3)
	{
		log_noti("[RATE%d] Compensate FrameRate:%d/%d --> %d/%d, %s\n",
				rate->u32Id, ui32FrameRateRes, ui32FrameRateDiv,
				rate->Codec.u32RateRes,
				rate->Codec.u32RateDiv,
				__FUNCTION__);

		ui32FrameRateRes = rate->Codec.u32RateRes;
		ui32FrameRateDiv = rate->Codec.u32RateDiv;
		bCorrect = TRUE;
	}
	else if ((ui32FrameRate_PTS >= VDEC_FRAMERATE_MIN) &&
			(ui32FrameRate_PTS <= VDEC_FRAMERATE_MAX))
	{
		u32PtsDura = VSync_CalFrameDuration(ui32FrameRateRes, ui32FrameRateDiv);

		if (((u32CurDecDura / 2) < (u32PtsDura * 11/10)) &&
				((u32CurDecDura / 2) >= (u32PtsDura * 9 / 10)))
		{
			ui32FrameRateRes *= 2;
			bCorrect = TRUE;
			log_debug("[RATE%d] Compensate FrameRate P Res:%d, Div:%d\n",
					rate->u32Id, ui32FrameRateRes, ui32FrameRateDiv);
		}
	}

	if (bCorrect == TRUE)
	{
		*pui32FrameRateRes = ui32FrameRateRes;
		*pui32FrameRateDiv = ui32FrameRateDiv;
	}

	return bCorrect;
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
static UINT32 _VRATE_GetApproximateRate(UINT32 u32FrameIntv)
{
    UINT32 u32Ret;

    if (u32FrameIntv>=(3750+3600)/2)
    {
        // 24 Hz
        u32Ret = 24;
    }
    else if (u32FrameIntv>=(3600+3000)/2)
    {
        // 25 Hz
        u32Ret = 25;
    }
    else/* if (u32FrameIntv>=(3000+1800)/2)*/
    {
        // 30 Hz
        u32Ret = 30;
    }/*
    else if (u32FrameIntv>=(1800+1500)/2)
    {
        // 50 Hz
        u32Ret = 50;
    }
    else 
    {
        // 60 Hz
        u32Ret = 60;
    }
*/
    log_user2("Intv %d  Rate %d\n", u32FrameIntv, u32Ret);
    return u32Ret;
}

static void _VRATE_AssumeRate(RATE_T *rate, UINT32 *pu32RateRes,
        UINT32 *pu32RateDiv, UINT32 *pu32FrmIntv)
{
    UINT32 u32FrmIntvAvg;
    UINT32 u32CurRate;
    UINT32 u32NewRate;
    BOOLEAN bRateChange=FALSE;

    u32FrmIntvAvg = _VRATE_GetAvgIntv90K(rate, FALSE, FALSE, TRUE);

    if (u32FrmIntvAvg)
	{
		u32CurRate = _VRATE_GetApproximateRate(rate->Status.u32Duration90k);
		u32NewRate = _VRATE_GetApproximateRate(u32FrmIntvAvg);

		log_user1("Rate Cur %d, New %d\n", u32CurRate, u32NewRate);

		if (u32CurRate != u32NewRate)
			bRateChange = TRUE;
	}

	if (bRateChange)
	{
		*pu32RateRes = u32NewRate;
		*pu32RateDiv = 1;
		*pu32FrmIntv = 90000/u32NewRate;

		log_info("change assumed frame rate %d->%d\n", u32CurRate, u32NewRate);
	}
	else
	{
		*pu32RateRes = rate->Status.u32RateRes;
		*pu32RateDiv = rate->Status.u32RateDiv;
		*pu32FrmIntv = rate->Status.u32Duration90k;
	}

    return;
}

#if defined(CHIP_NAME_d13)
static void _vrate_convert_rate_for_hdmi(UINT32 *pu32FrameRateRes,
		UINT32 *pu32FrameRateDiv)
{
	UINT32 u32Temp;
	UINT32 u32Residual;

	// convert for D13 rate
	u32Temp = *pu32FrameRateRes/ *pu32FrameRateDiv;
	u32Temp *= 1000;
	u32Residual = *pu32FrameRateRes % *pu32FrameRateDiv;
	u32Temp += (u32Residual*1000)/ *pu32FrameRateDiv;
	/*
	   if (u32Temp <= 23976)			// 23.976 Hz
	   {
	   *pu32FrameRateRes = 24000;
	   *pu32FrameRateDiv = 1001;
	   }
	   else */
	if (u32Temp <= 24000) 	// 24 Hz
	{
		*pu32FrameRateRes = 24000;
		*pu32FrameRateDiv = 1000;
	}
	else if (u32Temp <= 25000) 	// 25 Hz
	{
		*pu32FrameRateRes = 25000;
		*pu32FrameRateDiv = 1000;
	}
	/*
	   else if (u32Temp <= 29970) 	// 29.97 Hz
	   {
	   *pu32FrameRateRes = 30000;
	   *pu32FrameRateDiv = 1001;
	   }*/
	else if (u32Temp <= 30000)		// 30 Hz
	{
		*pu32FrameRateRes = 30000;
		*pu32FrameRateDiv = 1000;
	}
	else if (u32Temp <= 50000)		// 50 Hz
	{
		*pu32FrameRateRes = 50000;
		*pu32FrameRateDiv = 1000;
	}
	else if (u32Temp <= 59940)		// 59.94 Hz
	{
		*pu32FrameRateRes = 60000;
		*pu32FrameRateDiv = 1001;
	}
	else if (u32Temp <= 60000)		// 60 Hz
	{
		*pu32FrameRateRes = 60000;
		*pu32FrameRateDiv = 1000;
	}
	else
	{
		log_error("not suported frame rate for VCP, Res %d, Div%d\n",
				*pu32FrameRateRes, *pu32FrameRateDiv);

		*pu32FrameRateRes = 30000;
		*pu32FrameRateDiv = 1000;
	}
}
#endif

void VRATE_UpdateDebugRate(UINT8 ui8SyncCh, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv)
{
	RATE_T		*pstRate;

	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_noti("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return;
	}

	pstRate = &gsVdecRate[ui8SyncCh];

	if ((ui32FrameRateRes == 0) || (ui32FrameRateDiv == 0))
	{
		pstRate->Debug.bUse = FALSE;
		return;
	}
	else
	{
		pstRate->Debug.bUse = TRUE;
		pstRate->Debug.u32RateRes = ui32FrameRateRes;
		pstRate->Debug.u32RateDiv = ui32FrameRateDiv;
		pstRate->Debug.u32Duration90k = VSync_CalFrameDuration(ui32FrameRateRes,
				ui32FrameRateDiv);
	}

	pstRate->bUpdateNeed = TRUE;

	return;
}

#if defined(CHIP_NAME_d14)
static BOOLEAN _is_fract_rate(UINT32 frame_intv)
{
	BOOLEAN ret = FALSE;

	if (frame_intv >= 90000*1000/23988)
	{
		// 23.976
		ret = TRUE;
	}
	else if (frame_intv >= 90000*1000/29985 && frame_intv < 90000*1000/27485)
	{
		// 29.97
		ret = TRUE;
	}
	else if (frame_intv > 90000*1000/59970 && frame_intv < 90000*1000/54970)
	{
		// 59.94
		ret = TRUE;
	}

	log_trace("[VRATE] %u is fraction rate? %d\n", frame_intv, ret);

	return ret;
}
#endif

static BOOLEAN _VRATE_Update(UINT8 ui8SyncCh)
{
	RATE_T		*pstRate;
	BOOLEAN		bUpdated = FALSE;
	RATE_SRC_E	eRateSrc;
	UINT32 ui32FrameRateRes, ui32FrameRateDiv, ui32FrameDuration90K =0;
	static UINT32	log_cnt;

	pstRate = &gsVdecRate[ui8SyncCh];

	// check if there is changes
	if (!pstRate->bUpdateNeed)
	{
		log_debug("RATE %d: no rate change (cur: %s)\n",
				ui8SyncCh, rate_src_str[pstRate->Status.eRateSrc]);
		return FALSE;
	}
	else
	{
		log_info("one of rate src changed: Dec %d Conf %d Pts%d\n",
				pstRate->Codec.bChanged,
				pstRate->Config.bChanged,
				pstRate->PTS.bChanged);
	}

	pstRate->bUpdateNeed = FALSE;
	pstRate->Codec.bChanged = FALSE;
	pstRate->Config.bChanged = FALSE;
	pstRate->PTS.bChanged = FALSE;

	eRateSrc = _VRATE_SelConfDecFrameRate(pstRate);

	if (eRateSrc == RATE_SRC_CONF)
	{
		ui32FrameRateRes = pstRate->Config.u32RateRes;
		ui32FrameRateDiv = pstRate->Config.u32RateDiv;
		ui32FrameDuration90K = pstRate->Config.u32Duration90k;
	}
	else if (eRateSrc == RATE_SRC_CODEC || eRateSrc == RATE_SRC_CODEC_DBL)
	{
		ui32FrameRateRes = pstRate->Codec.u32RateRes;
		ui32FrameRateDiv = pstRate->Codec.u32RateDiv;
		ui32FrameDuration90K = pstRate->Codec.u32Duration90k;

		if (eRateSrc == RATE_SRC_CODEC_DBL)
		{
			ui32FrameRateRes *= 2;
			ui32FrameDuration90K /= 2;
		}
	}
	else if (eRateSrc == RATE_SRC_NONE || eRateSrc == RATE_SRC_PTS)
	{
		_VRATE_AssumeRate(pstRate, &ui32FrameRateRes,
				&ui32FrameRateDiv, &ui32FrameDuration90K);
	}
	else if (eRateSrc == RATE_SRC_DEBUG)
	{
		ui32FrameRateRes = pstRate->Debug.u32RateRes;
		ui32FrameRateDiv = pstRate->Debug.u32RateDiv;
		ui32FrameDuration90K = pstRate->Debug.u32Duration90k;
	}
	else
	{
		if(!(log_cnt++)%100)
			log_error("Rate%d : wrong rate src %d\n", ui8SyncCh, eRateSrc);

		_VRATE_AssumeRate(pstRate, &ui32FrameRateRes,
				&ui32FrameRateDiv, &ui32FrameDuration90K);
	}

	if (ui32FrameRateRes && ui32FrameRateDiv)
	{
		UINT32 ui32FrameRate;

#if defined(CHIP_NAME_d13)
		_vrate_convert_rate_for_hdmi(&ui32FrameRateRes, &ui32FrameRateDiv);
#endif
		// Limit check
		ui32FrameRate = ui32FrameRateRes / ui32FrameRateDiv;
		if (ui32FrameRate > pstRate->ui32FrameRateLimit)
		{
			UINT64 u64Temp;

			if (ui32FrameRate >= 50 && ui32FrameRate <= 60)
				ui32FrameRateDiv *= 2;
			else
				ui32FrameRateRes = ui32FrameRateDiv *
					pstRate->ui32FrameRateLimit;

			u64Temp = (UINT64)ui32FrameRateDiv * 90000;
			do_div(u64Temp, ui32FrameRateRes);
			ui32FrameDuration90K = (UINT32)u64Temp;

			log_user2("[RATE%d] by rate limit %d, %d -> %d\n", ui8SyncCh,
					pstRate->ui32FrameRateLimit, ui32FrameRate,
					ui32FrameRateRes/ui32FrameRateDiv);
		}

		// ignore change into little lower rate
		if (pstRate->Status.u32Duration90k * 25/20 < ui32FrameDuration90K ||
				pstRate->Status.u32Duration90k > ui32FrameDuration90K ||
				pstRate->Status.eRateSrc != eRateSrc
		   )
		{
			log_noti("[RATE%d] FrameDuration90k: %d(%s)--> %d(%s)\n",
					ui8SyncCh,
					pstRate->Status.u32Duration90k,
					rate_src_str[pstRate->Status.eRateSrc],
					ui32FrameDuration90K,
					rate_src_str[eRateSrc]
					);

			pstRate->Status.eRateSrc = eRateSrc;
			pstRate->Status.u32RateRes = ui32FrameRateRes;
			pstRate->Status.u32RateDiv = ui32FrameRateDiv;
			pstRate->Status.u32Duration90k = ui32FrameDuration90K;
			bUpdated = TRUE;

			// change speed x1.001 for fraction frame rate
#if defined(CHIP_NAME_d14)
			if(pstRate->bUseGSTC == TRUE)
			{
				if (_is_fract_rate(ui32FrameDuration90K))
				{
					if (!pstRate->fract_speed)
					{
						pstRate->i32Speed = pstRate->i32Speed * 1001 / 1000;
						pstRate->fract_speed = TRUE;
					}
				}
				else
				{
					if (pstRate->fract_speed)
					{
						pstRate->i32Speed = pstRate->i32Speed * 1000 / 1001;
						pstRate->fract_speed = FALSE;
					}
				}
			}
#endif
		}
	}
	else
	{
		log_noti("[RATE%d][DBG] FrameRateRes: %d, FrameRateDiv: %d\n",
				ui8SyncCh, ui32FrameRateRes, ui32FrameRateDiv);
	}

	return bUpdated;
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
void VRATE_UpdateConfigRate(UINT8 ui8SyncCh, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv)
{
	UINT32		ui32FrameRate;

	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_noti("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return;
	}

	if ((ui32FrameRateRes == 0) || (ui32FrameRateDiv == 0))
	{
		//log_debug("[RATE%d] ui32FrameRateRes:%d, ui32FrameRateDiv:%d, %s\n",
		//	ui8SyncCh, ui32FrameRateRes, ui32FrameRateDiv, __FUNCTION__);
		return;
	}

	if ((ui32FrameRateRes != gsVdecRate[ui8SyncCh].Config.u32RateResOrg) ||
		(ui32FrameRateDiv != gsVdecRate[ui8SyncCh].Config.u32RateDivOrg))
	{
		log_noti("[RATE%d][DBG] Config FrameRate_Org: %d/%d --> %d/%d\n",
				ui8SyncCh,
				gsVdecRate[ui8SyncCh].Config.u32RateResOrg,
				gsVdecRate[ui8SyncCh].Config.u32RateDivOrg,
				ui32FrameRateRes, ui32FrameRateDiv);

		gsVdecRate[ui8SyncCh].Config.u32RateResOrg = ui32FrameRateRes;
		gsVdecRate[ui8SyncCh].Config.u32RateDivOrg = ui32FrameRateDiv;
		gsVdecRate[ui8SyncCh].Config.u32Duration90kOrg =
			VSync_CalFrameDuration(ui32FrameRateRes, ui32FrameRateDiv);
	}
	else
	{
		// no change!!
		return;
	}

	// check rate
	ui32FrameRate = ui32FrameRateRes / ui32FrameRateDiv;
	if ((ui32FrameRate < VDEC_FRAMERATE_MIN) ||
			(ui32FrameRate > VDEC_FRAMERATE_MAX))
	{
		static UINT32 cnt=0;

		// invalid
		if (!(cnt++ % 100))
			log_noti("[RATE%d] Over Range Config FrameRate: %d/%d\n",
					ui8SyncCh, ui32FrameRateRes, ui32FrameRateDiv);

		if (ui32FrameRate < VDEC_FRAMERATE_MIN)
		{
		}
		else if (ui32FrameRate > VDEC_FRAMERATE_MAX)
		{
		}

		return;
	}

	if ((ui32FrameRateRes != gsVdecRate[ui8SyncCh].Config.u32RateRes) ||
		(ui32FrameRateDiv != gsVdecRate[ui8SyncCh].Config.u32RateDiv))
	{
		log_noti("[RATE%d][DBG] Config FrameRate: %d/%d --> %d/%d\n",
				ui8SyncCh,
				gsVdecRate[ui8SyncCh].Config.u32RateRes,
				gsVdecRate[ui8SyncCh].Config.u32RateDiv,
				ui32FrameRateRes, ui32FrameRateDiv);

		gsVdecRate[ui8SyncCh].Config.u32RateRes = ui32FrameRateRes;
		gsVdecRate[ui8SyncCh].Config.u32RateDiv = ui32FrameRateDiv;
		gsVdecRate[ui8SyncCh].Config.u32Duration90k =
			VSync_CalFrameDuration(ui32FrameRateRes, ui32FrameRateDiv);
		gsVdecRate[ui8SyncCh].bUpdateNeed = TRUE;
		gsVdecRate[ui8SyncCh].Config.bChanged = TRUE;

		_VRATE_Update(ui8SyncCh);
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
void VRATE_UpdateCodecRate(UINT8 ui8SyncCh, UINT32 ui32FrameRateRes,
		UINT32 ui32FrameRateDiv, BOOLEAN bInterlaced)
{
	RATE_T		*rate;
	BOOLEAN		bChanged = FALSE;

	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return;
	}

	rate = &gsVdecRate[ui8SyncCh];

	if ((ui32FrameRateRes == 0) || (ui32FrameRateDiv == 0) ||
		(ui32FrameRateRes > 0x3FFFFFFF) || (ui32FrameRateDiv > 0x3FFFFFFF))
	{
		log_user3("[RATE%d] ui32FrameRateRes:%d, ui32FrameRateDiv:%d, %s\n",
			ui8SyncCh, ui32FrameRateRes, ui32FrameRateDiv, __FUNCTION__);

		ui32FrameRateRes = 0x0;
		ui32FrameRateDiv = 0xFFFFFFFF;

		goto exit_no_chg;
	}

	if ((ui32FrameRateRes != rate->Codec.u32RateResOrg) ||
		(ui32FrameRateDiv != rate->Codec.u32RateDivOrg) ||
		rate->bInterlaced != bInterlaced
		)
	{
		log_noti("[RATE%d] Codec FrameRate_Org: %d/%d %s --> %d/%d %s\n",
				ui8SyncCh,
				rate->Codec.u32RateResOrg,
				rate->Codec.u32RateDivOrg,
				rate->bInterlaced?"Intl":"Prog",
				ui32FrameRateRes, ui32FrameRateDiv,
				bInterlaced?"Intl":"Prog");

		rate->bInterlaced = bInterlaced;
		rate->Codec.u32RateResOrg = ui32FrameRateRes;
		rate->Codec.u32RateDivOrg = ui32FrameRateDiv;
		rate->Codec.u32Duration90kOrg =
			VSync_CalFrameDuration(ui32FrameRateRes, ui32FrameRateDiv);

		bChanged = TRUE;
	}

	// Frame Rate Error Correction
	if (bChanged)
	{
		_VRATE_CorrectCodecRate(rate, &ui32FrameRateRes, &ui32FrameRateDiv);

		if ((ui32FrameRateRes != rate->Codec.u32RateRes) ||
				(ui32FrameRateDiv != rate->Codec.u32RateDiv))
		{
			log_noti("[RATE%d] Codec FrameRate: %d/%d --> %d/%d\n",
					ui8SyncCh, rate->Codec.u32RateRes,
					rate->Codec.u32RateDiv,
					ui32FrameRateRes, ui32FrameRateDiv);

			rate->Codec.u32RateRes = ui32FrameRateRes;
			rate->Codec.u32RateDiv = ui32FrameRateDiv;
			rate->Codec.u32Duration90k =
				VSync_CalFrameDuration(ui32FrameRateRes, ui32FrameRateDiv);
			rate->bUpdateNeed = TRUE;
			rate->Codec.bChanged = TRUE;
		}
	}

exit_no_chg:
	// Frame rate could be changed even if codec rate is not changed
	//  , because AVG frame rate could be changed
	_VRATE_Update(ui8SyncCh);
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
static UINT32 _VRATE_UpdateByXXX(UINT32 ui32STC,
		UINT32 *pui32FrameRateRes, UINT32 *pui32FrameRateDiv,
		VRATE_AVERAGE_T *pstAvrg)
{
	UINT32		ui32STCDiff = 0;
	UINT32		u32Duration;
	SINT32		diff_diff;

	ui32STC &= VDEC_PTS_MASK;

	if (pstAvrg->u32HistoryCnt == 0xFFFFFFFF)
	{
		pstAvrg->u32HistoryCnt = 0;

		pstAvrg->ui32_Prev = ui32STC;

		*pui32FrameRateRes = 0;
		*pui32FrameRateDiv = 1;
		//In case of Unkown PTS
		if (ui32STC == 0xFFFFFFFF)
		{
			pstAvrg->u32HistoryCnt = 0xFFFFFFFF;
			log_user2 ("In case of unkown PTS STC : %d, Cnt : 0x%X\n", ui32STC, pstAvrg->u32HistoryCnt);
		}
		log_user2("In the first time condition STC:%d\n",ui32STC);

	}
	else
	{
		if (ui32STC >= pstAvrg->ui32_Prev)
			ui32STCDiff = ui32STC - pstAvrg->ui32_Prev;
		else
			ui32STCDiff = VDEC_PTS_MASK - pstAvrg->ui32_Prev + ui32STC;

		if (ui32STCDiff > VDEC_RATE_MAX_DIFF_HISTORY_NUM)
			ui32STCDiff = VDEC_RATE_MAX_DIFF_HISTORY_NUM;

		if (pstAvrg->u32HistoryCnt >= VRATE_N_HISTORY_BUF)
			pstAvrg->u32DiffSum -= pstAvrg->u32DiffHistory[pstAvrg->u32BufIdx];
		else
			pstAvrg->u32HistoryCnt++;

		if (pstAvrg->u32Cnt < 0xFFFFFFFF)
			pstAvrg->u32Cnt++;

		pstAvrg->u32DiffHistory[pstAvrg->u32BufIdx] = ui32STCDiff;
		pstAvrg->u32BufIdx++;

		if (pstAvrg->u32BufIdx >= VRATE_N_HISTORY_BUF)
			pstAvrg->u32BufIdx = 0;

		pstAvrg->u32DiffSum += ui32STCDiff;
		pstAvrg->ui32_Prev = ui32STC;

		if (pstAvrg->stc_diff == 0xFFFFFFFF)
			pstAvrg->stc_diff = ui32STCDiff;
		else
		{
			diff_diff = ui32STCDiff - pstAvrg->stc_diff;
			if (diff_diff<0) diff_diff = -diff_diff;

			if (diff_diff<4)
				pstAvrg->stable_cnt++;
			else
			{
				pstAvrg->stable = FALSE;
				pstAvrg->stable_cnt=-30;
				pstAvrg->stc_diff=0xFFFFFFFF;
			}

			if (pstAvrg->stable_cnt>=4)
			{
				pstAvrg->stable = TRUE;
				pstAvrg->stable_cnt = -10;
				pstAvrg->stc_diff = ui32STCDiff;
			}
		}

		log_user2("History sum %d cnt %d, avg %d, add %d stb %d\n",
				pstAvrg->u32DiffSum, pstAvrg->u32HistoryCnt,
				pstAvrg->u32DiffSum/ pstAvrg->u32HistoryCnt,
				ui32STCDiff, pstAvrg->stable);


		if (pstAvrg->u32HistoryCnt >= 2)
		{
			*pui32FrameRateRes = 90000 * pstAvrg->u32HistoryCnt;
			*pui32FrameRateDiv = pstAvrg->u32DiffSum;
			u32Duration = pstAvrg->u32DiffSum/pstAvrg->u32HistoryCnt;

			if (*pui32FrameRateDiv == 0)
			{ // All Same PTS
				*pui32FrameRateRes = 0;
				*pui32FrameRateDiv = 0xFFFFFFFF;
				u32Duration = 0xFFFFFFFF;
			}
		}
		else
		{
			*pui32FrameRateRes = 0;
			*pui32FrameRateDiv = 0xFFFFFFFF;
			u32Duration = 0xFFFFFFFF;
		}

		pstAvrg->ui32FrameRateRes = *pui32FrameRateRes;
		pstAvrg->ui32FrameRateDiv = *pui32FrameRateDiv;

		if (pstAvrg->u32Duration != u32Duration)
		{
			pstAvrg->u32Duration = u32Duration;
			pstAvrg->bChanged = TRUE;
		}
	}

	//log_debug("[RATE] STC:%d, Diff:%d, Cnt:%u, Sum:%u, Res:%d, Div:%d\n",
	//	ui32STC, ui32STCDiff, pstAvrg->u32HistoryCnt,
	//	pstAvrg->u32DiffSum, *pui32FrameRateRes, *pui32FrameRateDiv);

	return pstAvrg->bChanged;
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
UINT32 VRATE_UpdateByDqPop(UINT8 ui8SyncCh, UINT32 ui32GSTC,
		UINT32 *pui32FrameRateRes, UINT32 *pui32FrameRateDiv)
{
	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return 0;
	}

	return _VRATE_UpdateByXXX(ui32GSTC, pui32FrameRateRes,
			pui32FrameRateDiv, &gsVdecRate[ui8SyncCh].DQ_Pop);
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
UINT32 VRATE_UpdateByPTS(UINT8 ui8SyncCh, UINT32 ui32PTS,
		UINT32 *pui32FrameRateRes, UINT32 *pui32FrameRateDiv)
{
	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return 0;
	}
	log_user2 ("Call _VRATE_UpdateByXXX\n");
	_VRATE_UpdateByXXX(ui32PTS, pui32FrameRateRes,
			pui32FrameRateDiv, &gsVdecRate[ui8SyncCh].PTS);

	if (gsVdecRate[ui8SyncCh].PTS.bChanged &&
			gsVdecRate[ui8SyncCh].PTS.u32Cnt < 30*5)
		gsVdecRate[ui8SyncCh].bUpdateNeed = TRUE;

	return gsVdecRate[ui8SyncCh].PTS.bChanged;
}

UINT32 VRATE_UpdateByDqPTS(UINT8 ui8SyncCh, UINT32 ui32PTS,
		UINT32 *pui32FrameRateRes, UINT32 *pui32FrameRateDiv)
{
	BOOLEAN	pre_stable;
	UINT32	ret=0;

	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return 0;
	}

	pre_stable = gsVdecRate[ui8SyncCh].dq_pts.stable;

	_VRATE_UpdateByXXX(ui32PTS, pui32FrameRateRes,
			pui32FrameRateDiv, &gsVdecRate[ui8SyncCh].dq_pts);

	if (pre_stable == FALSE && gsVdecRate[ui8SyncCh].dq_pts.stable == TRUE)
		ret = 1;

	return ret;
}

BOOLEAN VRATE_UseEnqRate(UINT8 u8SyncCh, BOOLEAN on, UINT32 res, UINT32 div)
{
	UINT64	u64Temp;
	UINT32	frame_intv_codec;

	if (u8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", u8SyncCh, __FUNCTION__);
		return 0;
	}

	if (on)
	{
		if ( res == 0 ) {
			//log_error("Error rate %d %d !!!!!!!!!!!!!!!!!!\n", res, div);
			res = 1;
			div = 0;
		}

		// only use for 25hz -> 29.97hz compensation
		// (because not verified for all cases)
		u64Temp = (UINT64)div * 90000;
		do_div(u64Temp, res);
		frame_intv_codec = (UINT32)u64Temp;

		if (frame_intv_codec == 3600)
		{
			if (gsVdecRate[u8SyncCh].dq_pts.u32Duration == 3003)
			{
				log_noti("VRATE:%d Use DqEnq Rate\n", u8SyncCh);

				VRATE_UpdateConfigRate(u8SyncCh,
						gsVdecRate[u8SyncCh].dq_pts.ui32FrameRateRes,
						gsVdecRate[u8SyncCh].dq_pts.ui32FrameRateDiv
						);
			}
		}
	}

	return 0;
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
UINT32 VRATE_GetPTSRate(UINT8 ui8SyncCh, UINT32 *pui32FrameRateRes,
		UINT32 *pui32FrameRateDiv)
{
	UINT32		ui32FrameRateRes;
	UINT32		ui32FrameRateDiv;
	SINT32		i32Speed;

	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return 0;
	}

	i32Speed = gsVdecRate[ui8SyncCh].i32Speed;

	if (i32Speed == 0)
		return 0;

	ui32FrameRateRes = gsVdecRate[ui8SyncCh].PTS.ui32FrameRateRes;
	ui32FrameRateDiv = gsVdecRate[ui8SyncCh].PTS.ui32FrameRateDiv;

	if (i32Speed != 1000)
	{
		ui32FrameRateRes /= 1000;
		ui32FrameRateDiv /= i32Speed;
	}

	if (pui32FrameRateRes != NULL && pui32FrameRateDiv != NULL)
	{
		*pui32FrameRateRes = ui32FrameRateRes;
		*pui32FrameRateDiv = ui32FrameRateDiv;
	}

	if (ui32FrameRateDiv == 0)
		return 0;

	return ui32FrameRateRes / ui32FrameRateDiv;
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
UINT32 VRATE_GetDuration(UINT8 ui8SyncCh)
{
	UINT32		ui32FrameDuration90K = 0;

	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return 0;
	}

	if (gsVdecRate[ui8SyncCh].Status.u32Duration90k != 0)
	{
		ui32FrameDuration90K = gsVdecRate[ui8SyncCh].Status.u32Duration90k;
	}

	return ui32FrameDuration90K;
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

inline RATE_SRC_E VRATE_GetRateSource(UINT8 ui8SyncCh)
{
	return gsVdecRate[ui8SyncCh].Status.eRateSrc;
}

UINT32 VRATE_GetResDiv(UINT8 ui8SyncCh, UINT32 *pui32FrameRateRes,
		UINT32 *pui32FrameRateDiv)
{
	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return 0;
	}

	if ((gsVdecRate[ui8SyncCh].Status.u32RateRes != 0)
			&& (gsVdecRate[ui8SyncCh].Status.u32RateDiv != 0))
	{
		*pui32FrameRateRes = gsVdecRate[ui8SyncCh].Status.u32RateRes;
		*pui32FrameRateDiv = gsVdecRate[ui8SyncCh].Status.u32RateDiv;
	}
#if 0
	else if (VRATE_GetPTSRate(ui8SyncCh, pui32FrameRateRes,
				pui32FrameRateDiv) >= VDEC_FRAMERATE_MIN)
	{
		VDEC_RATE_CategorizeFrameRateResDiv(pui32FrameRateRes, pui32FrameRateDiv);
	}
#endif
	else
	{
		*pui32FrameRateRes = 0;
		*pui32FrameRateDiv = 1;
	}

	if (*pui32FrameRateDiv == 0)
		return 0;

	return *pui32FrameRateRes / *pui32FrameRateDiv;
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
BOOLEAN VRATE_IsModifiedRate(UINT8 ui8SyncCh)
{
	SINT32		diff;
	UINT32		u32FrmIntvAvg;

	if (ui8SyncCh >= VDEC_RATE_NUM_OF_CHANNEL)
	{
		log_error("[RATE%d] Channel Number, %s\n", ui8SyncCh, __FUNCTION__);
		return FALSE;
	}

	if (gsVdecRate[ui8SyncCh].Status.u32Duration90k == 0)
		return FALSE;

	u32FrmIntvAvg = _VRATE_GetAvgIntv90K(&gsVdecRate[ui8SyncCh],
			FALSE, FALSE, TRUE);

	if (u32FrmIntvAvg == 0)
		return FALSE;

	diff = u32FrmIntvAvg - gsVdecRate[ui8SyncCh].Status.u32Duration90k;

	if (diff<0)
		diff = -diff;

	diff = diff*100/u32FrmIntvAvg;	// convert % ratio
	if (diff>10)
		return TRUE;
	else
		return FALSE;
}

void _rate_print_channel(struct seq_file *m, RATE_T *pstRate)
{
#define VDS_DPRINT(frm, args...)		seq_printf(m, frm, ##args)
	VDS_DPRINT("Speed            : %u\n", (UINT32)pstRate->i32Speed);
	VDS_DPRINT("bInterlaced      : %u\n", (UINT32)pstRate->bInterlaced);
	VDS_DPRINT("FrameRateLimit   : %u\n", (UINT32)pstRate->ui32FrameRateLimit);
	VDS_DPRINT("\n- Config -\n");
	VDS_DPRINT("FrameRateRes_Org : %u\n", (UINT32)pstRate->Config.u32RateResOrg);
	VDS_DPRINT("FrameRateDiv_Org : %u\n", (UINT32)pstRate->Config.u32RateDivOrg);
	VDS_DPRINT("FrameDuration_Org: %u\n", (UINT32)pstRate->Config.u32Duration90kOrg);
	VDS_DPRINT("FrameRateRes     : %u\n", (UINT32)pstRate->Config.u32RateRes);
	VDS_DPRINT("FrameRateDiv     : %u\n", (UINT32)pstRate->Config.u32RateDiv);
	VDS_DPRINT("FrameDuration90K : %u\n", (UINT32)pstRate->Config.u32Duration90k);
	VDS_DPRINT("\n- Codec -\n");
	VDS_DPRINT("FrameRateRes_Org : %u\n", (UINT32)pstRate->Codec.u32RateResOrg);
	VDS_DPRINT("FrameRateDiv_Org : %u\n", (UINT32)pstRate->Codec.u32RateDivOrg);
	VDS_DPRINT("FrameDuration_Org: %u\n", (UINT32)pstRate->Codec.u32Duration90kOrg);
	VDS_DPRINT("FrameRateRes     : %u\n", (UINT32)pstRate->Codec.u32RateRes);
	VDS_DPRINT("FrameRateDiv     : %u\n", (UINT32)pstRate->Codec.u32RateDiv);
	VDS_DPRINT("FrameDuration90K : %u\n", (UINT32)pstRate->Codec.u32Duration90k);
	VDS_DPRINT("\n- PTS -\n");
	VDS_DPRINT("Prev             : %u\n", (UINT32)pstRate->PTS.ui32_Prev);
	VDS_DPRINT("bChanged         : %u\n", (UINT32)pstRate->PTS.bChanged);
	VDS_DPRINT("BufIdx           : %u\n", (UINT32)pstRate->PTS.u32BufIdx);
	VDS_DPRINT("DiffSum          : %u\n", (UINT32)pstRate->PTS.u32DiffSum);
	VDS_DPRINT("HistoryCnt       : %u\n", (UINT32)pstRate->PTS.u32HistoryCnt);
	VDS_DPRINT("2FrameRateRes    : %u\n", (UINT32)pstRate->PTS.ui32FrameRateRes);
	VDS_DPRINT("2FrameRateDiv    : %u\n", (UINT32)pstRate->PTS.ui32FrameRateDiv);
	VDS_DPRINT("Duration         : %u\n", (UINT32)pstRate->PTS.u32Duration);
	VDS_DPRINT("Stable           : %u\n", (UINT32)pstRate->PTS.stable);

	VDS_DPRINT("\n- DispQ PTS -\n");
	VDS_DPRINT("Prev             : %u\n", (UINT32)pstRate->dq_pts.ui32_Prev);
	VDS_DPRINT("bChanged         : %u\n", (UINT32)pstRate->dq_pts.bChanged);
	VDS_DPRINT("BufIdx           : %u\n", (UINT32)pstRate->dq_pts.u32BufIdx);
	VDS_DPRINT("DiffSum          : %u\n", (UINT32)pstRate->dq_pts.u32DiffSum);
	VDS_DPRINT("HistoryCnt       : %u\n", (UINT32)pstRate->dq_pts.u32HistoryCnt);
	VDS_DPRINT("2FrameRateRes    : %u\n", (UINT32)pstRate->dq_pts.ui32FrameRateRes);
	VDS_DPRINT("2FrameRateDiv    : %u\n", (UINT32)pstRate->dq_pts.ui32FrameRateDiv);
	VDS_DPRINT("Duration         : %u\n", (UINT32)pstRate->dq_pts.u32Duration);
	VDS_DPRINT("Stable           : %u\n", (UINT32)pstRate->dq_pts.stable);

	VDS_DPRINT("\n- Debug -\n");
	VDS_DPRINT("bUse             : %u\n", (UINT32)pstRate->Debug.bUse);
	VDS_DPRINT("FrameRateRes     : %u\n", (UINT32)pstRate->Debug.u32RateRes);
	VDS_DPRINT("FrameRateDiv     : %u\n", (UINT32)pstRate->Debug.u32RateDiv);
	VDS_DPRINT("FrameDuration90K : %u\n", (UINT32)pstRate->Debug.u32Duration90k);

	VDS_DPRINT("\n- Status -\n");
	VDS_DPRINT("eRateSrc         : %s\n", rate_src_str[pstRate->Status.eRateSrc]);
	VDS_DPRINT("FrameRateRes     : %u\n", (UINT32)pstRate->Status.u32RateRes);
	VDS_DPRINT("FrameRateDiv     : %u\n", (UINT32)pstRate->Status.u32RateDiv);
	VDS_DPRINT("FrameDuration90K : %u\n", (UINT32)pstRate->Status.u32Duration90k);
	VDS_DPRINT("\n\n\n");
}

void RATE_PrintDebug(struct seq_file *m)
{
	RATE_T			*pstRate;
	UINT32			i;

	for (i=0;i<VDEC_RATE_NUM_OF_CHANNEL;i++)
	{
		pstRate = &gsVdecRate[i];
		VDS_DPRINT("====  Rate %d, %X  ====\n", i, (UINT32)pstRate);
		_rate_print_channel(m, pstRate);
	}

#undef VDS_DPRINT
}
/** @} */
