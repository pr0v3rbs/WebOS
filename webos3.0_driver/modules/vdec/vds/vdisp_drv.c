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
 *
 * author     youngki.lyu@lge.com
 * version    1.0
 * date       2011.06.14
 * note       Additional information.
 *
 * @addtogroup lg115x_vdec
 * @{
 */


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/
#define LOG_NAME	vdec_vdisp
#define LOG_LEVEL	log_level_error

//#define VDISP_DBG_LIPSYNC
/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "vdisp_drv.h"
#include "vsync_drv.h"
#include "vdec_rate.h"

#ifdef VDEC_USE_MCU
#include "mmcu_drv.h"
#endif
#include <linux/kernel.h>
#include <asm/string.h>	// memset
#include <linux/spinlock.h>
#include <linux/seq_file.h>
//#include <linux/kthread.h>

#include "include/vdec_kapi.h"
#include "vdisp_cfg.h"

#if defined(CHIP_NAME_d13)
#include "hal/d13/vd3_top_hal.h"
#endif
#include "hal/top_hal_api.h"
#include "hal/pdec_hal_api.h"
#include "hal/vdec_base.h"
#include "log.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define		VDISP_NUM_OF_CMD_Q				0x20

#if defined (CHIP_NAME_h15) || defined(CHIP_NAME_m16)
#define		VDISP_DUAL_CH_NUM			6
#else
#define		VDISP_DUAL_CH_NUM			4
#endif

#define		VDISP_DUAL_SYNC_THOLD				(270000)
#define		VDISP_DUAL_SYNC_THOLD_FILEPLAY		(900000)
#define		VDISP_DUAL_EMPTY_THOLD				(20)
#define		VDISP_DUAL_EMPTY_THOLD_FILEPLAY		(200)
#define		VDISP_UHD_FRM_DELAY			2
//#define 	AMAZON_TEST	1
/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
//#define __M14B0__
#define	VDISP_PTS_GET_PTS_OFFSET( _prev, _curr )	\
	(((_curr) < (_prev)) ? ((_curr) + VDEC_PTS_MASK - (_prev)) : 		\
	 (_curr) - (_prev))

#define VDISP_PTS_GET_OFFSET(pts, base, offset)				\
	do {												\
		(offset) = (pts)-(base);				\
		if (((SINT32)(offset)) > (SINT32)(VDEC_PTS_MASK>>1))	\
			(offset) -= (VDEC_PTS_MASK);				\
		else if (((SINT32)(offset)) < -(SINT32)(VDEC_PTS_MASK>>1))	\
			(offset) += (VDEC_PTS_MASK);					\
	} while(0)

#define	VDISP_PTS_CHECK_PTS_WRAPAROUND( _pts)	((_pts) & VDEC_PTS_MASK)
#define VDS_ABS(i)		((i)<0?-(i):(i))
/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
//typedef UINT32 (*fpGetStc)(void[], UINT8);

typedef enum
{
	VDISP_CMD_NONE = 0,
	VDISP_CMD_PLAY,
	VDISP_CMD_STEP,
	VDISP_CMD_PAUSE,
	VDISP_CMD_RESET,
	VDISP_CMD_FLUSH,
	VDISP_CMD_REPEAT,
	VDISP_CMD_SET_OUT,
	VDISP_CMD_SET_CLKSRC,
	VDISP_CMD_UINT32 = 0x20110915
} VDISP_CMD_TYPE_E;

typedef struct
{
	VDISP_CMD_TYPE_E	eCmdType;
	UINT32				u32Param;
} VDISP_CMD_T;

typedef enum
{
	VDISP_NULL = 0,
	VDISP_READY = 10,
	VDISP_PLAY = 20,
	VDISP_PAUSE,
	VDISP_STATE_UINT32 = 0x20110716
} E_VDISP_STATE_T;

struct _VDISP_CH_T;
typedef struct _VDISP_CH_T VDISP_CH_T;

typedef struct
{
	VDISP_CH_T	*pstVdispCh[2];
	UINT8			u8EmptyCnt[2];
	UINT8			u8nPluged;
	UINT8			u8VsyncCh;
	UINT8			u8State;
	spinlock_t stLock;
} DualCh_T;

typedef struct
{
	enum {
		SYNC_FRM_INVALID,
		SYNC_FRM_INACTIVE,
		SYNC_FRM_WAITPAIR,
		SYNC_FRM_READY,
		SYNC_FRM_ACTIVE
	} eState;

	UINT32			u32PTS;
	UINT32			u32FrameRateRes;
	UINT32			u32FrameRateDiv;
	S_DISPQ_BUF_T 	stDqFrame;
	S_DISPQ_BUF_T 	stPairFrame;
	BOOLEAN			bValidPairFrame;
} SYNC_FRAME_T;

struct _VDISP_CH_T
{
	E_VDISP_STATE_T	State;
	UINT8			u8VdispCh;
	VDISP_CLK_SRC	eClkSrc;
	UINT32			u32PrePts;
	UINT32			u32PreOrgPts;
	UINT32			u32PreDispPeriod;
	S_DISPQ_BUF_T	stNextFrame;
	S_DISPQ_BUF_T	stCurFrame;
	S_DISPQ_BUF_T	stNextPair;
	BOOLEAN			bValidCurFrame;
	BOOLEAN			bValidNextFrame;
	BOOLEAN			bValidNextPair;
	SYNC_FRAME_T	stSyncFrame;

	BOOLEAN			bForcedPush;

	BOOLEAN			bIsDualCh;
	VDISP_CH_T		*pstPairCh;
	DualCh_T		*pstDualCh;

	BOOLEAN			bLock4Pair;

	UINT32			n_diff_sum;
	SINT32			diff_sum;
	SINT32			diff_var;

	UINT32			u32PhaseErrorCnt;
	SINT32			i32PhaseShift;
	SINT32			i32VsyncDiff;
	BOOLEAN			is_film_mode;
	BOOLEAN			film_mode_cnt;

	UINT32			lt_rate;
	SINT32			lt_rate_cnt;
	UINT32			lt_rate_strat;

	UINT32			dq_enq_cnt;

	SYNC_T			*pstSync;

	struct
	{
		DE_IF_DST_E		eDeIfDstCh;
		UINT8 			u8VSyncCh;
	} Config;
	struct
	{
		VDISP_CMD_T			astCmd[VDISP_NUM_OF_CMD_Q];
		volatile UINT32		u32WrIndex;
		volatile UINT32		u32RdIndex;
	} CmdQ;
	struct
	{
		BOOLEAN			bStepping;
		volatile BOOLEAN		bPlaying;
		volatile BOOLEAN		bClosing;
		BOOLEAN			bFreeze;
		VDISP_STATUS_CB	fpStatus;
	} Status;

	struct
	{
		UINT32		u32LogTick_UpdateBuf;
		UINT32		u32nInt;
		UINT32		u32nPopFrame;
		UINT32		u32nDispFrame;
	} Debug;

	spinlock_t stLock;
};



/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
extern int VDISP_proc_make_instance(void);
/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/
VDISP_CH_T* VDISP_GetHandler(UINT8 u8VdispCh);
/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/
#define DEFAULT_FRAME_RATE_RES 24
#define DEFAULT_FRAME_RATE_DIV 1
static int gDbgDefaultRateRes = DEFAULT_FRAME_RATE_RES;
module_param (gDbgDefaultRateRes, int, 0644);
static int gDbgDefaultRateDiv = DEFAULT_FRAME_RATE_DIV;
module_param (gDbgDefaultRateDiv, int, 0644);
#ifdef VDISP_DBG_LIPSYNC
static volatile UINT32 *pDeInfo, *pDePixel;
static struct task_struct *gpstVdispTask = NULL;
#endif
static UINT8 ui8Seamless = 0;
static BOOLEAN bVdispInit = FALSE;
static UINT32 gMatchModeDbg = 0xFF;
static int gDbgVsyncShift = 0;
module_param (gMatchModeDbg, uint, 0644);
module_param (gDbgVsyncShift, int, 0644);
#if defined (CHIP_NAME_d14)
static UINT8 delayoffsetflag = 0;
static UINT8 uhd_ex_delay = 0;
static UINT8 ui8FrameCnt = 0;
static UINT32 pre_uhd_out_mode = 0;
static UINT8 uhd_chg_cnt = 0;
static BOOLEAN bFirstFrmInfoSet = FALSE;
#endif
//For BBC & Amazon
static SINT32 i32VsiInfoTimming = 2;
static UINT32 ui32PicW = 0;
static UINT32 ui32PicH = 0;
static UINT32 ui32VOff = 0;
static UINT32 ui32HOff = 0;
//For pre phase shift of first frame
static BOOLEAN bSetBaseTime[VDISP_NUM_OF_CH];
static BOOLEAN bFirstFrameAfterSetBase[VDISP_NUM_OF_CH];
static BOOLEAN bLiveChannelOpen[VDISP_NUM_OF_CH];
static BOOLEAN bLivePrephaseShift[VDISP_NUM_OF_CH];
static UINT32  ui32DisplayOffset_90K;
static UINT8 ui8PrePhaseshiftCnt[VDISP_NUM_OF_CH];
static UINT8 ui8PrePhaseshiftCntLive[VDISP_NUM_OF_CH];
static BOOLEAN bInterlaced2 = FALSE;
/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/
static void VDISP_VsyncCallback(UINT8 u8VsyncCh, UINT8 u8nVdisp, UINT8 au8VdispCh[], UINT8 u8FieldInfo);
void VDISP_ClearFrame(VDISP_CH_T *pstVdispCh, DISPQ_FRM_T *buffer);
void VDISP_FlushFrames(VDISP_CH_T *pstVdispCh);
#if defined (CHIP_NAME_d14)
SINT32 _VDISP_SetSeamlessDisplayOffset(UINT8 u8VdispCh, S_DISPQ_BUF_T *pstDqFrame);
extern void DIF_SetFrameInfoDxIpc(UINT32 ui32Resolution, UINT32 ui32DualOutMode, UINT32 ui32FrameRate, UINT32 ui32V_Offset);
extern UINT32 DIF_GetClockSrc(void);
#endif
extern void	DE_VDO_HAL_SetVsiInfo(UINT8 ui8VdoCh, UINT32 ui32PicWidth, UINT32 ui32PicHeight, UINT32 ui32H_Offset, UINT32 ui32V_Offset);
extern void DIF_SetOutChannelMode(UINT32 ch);
static UINT32 _vdisp_calculation_duration(UINT32 ui32FrameRateRes, UINT32 ui32FrameRateDiv);
static void _vdisp_prephaseshift_calculation(VDISP_CH_T *pstVdispCh,
								UINT32 ui32Pts,UINT32 ui32FrameRes,UINT32 ui32FrameDiv, UINT32 ui32Stc);
/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
static CHAR gazClkSrcStr[VDISP_CLK_MAX][10] =
{
	[VDISP_CLK_NONE]	= "CLK_NONE",
	[VDISP_CLK_PDEC0]	= "CLK_PDEC0",
	[VDISP_CLK_PDEC1]	= "CLK_PDEC1",
	[VDISP_CLK_PDEC2]	= "CLK_PDEC2",
	[VDISP_CLK_PDEC3]	= "CLK_PDEC3",
	[VDISP_CLK_GSTC]	= "CLK_GSTC"
};

VDISP_CH_T gsVdisp[VDISP_NUM_OF_CH];
DualCh_T gstDualCh[VDISP_DUAL_CH_NUM];



/*----------------------------------------------------------------------------------------
 *   Function Define
 *---------------------------------------------------------------------------------------*/
static BOOLEAN _VDISP_CheckBelongTo90kTick(UINT32 u32STC_Start, UINT32 u32STC_End, UINT32 u32PTS)
{
	if( u32STC_Start <= u32STC_End )
	{
		if( (u32PTS > u32STC_Start) &&
			(u32PTS <= u32STC_End) )
			return TRUE;
	}
	else
	{
		if( (u32PTS > u32STC_Start) ||
			(u32PTS <= u32STC_End) )
			return TRUE;
	}

	return FALSE;
}

#ifdef VDISP_DBG_LIPSYNC
static UINT32 u32VdecSendTime;
static int VDISP_MonTask(void *data)
{
	UINT32 preData=0;
	UINT32 preDataW=0;
	UINT32 addr, addrW, pixelData=0, pixelDataW=0;
	UINT32 u32Stc;
	UINT32 uFrameAddr[12] = {
			0x98000000,
			0x98438000,
			0x98870000,
			0x98ca8000,
			0x990e0000,
			0x99518000,
			0x99950000,
			0x99d88000,
			0x9a1c0000,
			0x9a5f8000,
			0x9aa30000,
			0x9ae68000 };
	UINT32 nFrame, nFrame2, nFramePre=0, i;

	OS_MsecSleep(3000);

	while(1)
	{

		if( gsVdisp[0].State != VDISP_PLAY )
		{
			OS_MsecSleep(100);
			continue;
		}

		addr = *pDeInfo;
		addr = (addr >> 19)<<15;
		addrW = *(pDeInfo+2);
		addrW = (addrW >> 19)<<15;

		if( addr < 0x800000 && addrW < 0x800000 )
		{
			pixelData = *(volatile UINT32*)((UINT32)pDePixel+addr);
			pixelDataW = *(volatile UINT32*)((UINT32)pDePixel+addrW);
		}

		u32Stc = VDISP_GetSTC(0);

		nFrame = 0;
		nFrame2 = 0;

		for(i=0;i<12;i++)
		{
			pixelData = *(volatile UINT32*)(pDePixel+(uFrameAddr[i]-uFrameAddr[0]+((2048*500)+900)*2 )/4);
			log_debug("  + %d: pixel %X nfrm %d addr %X\n",i, pixelData, nFrame,
					pDePixel+(uFrameAddr[i]-uFrameAddr[0]+((2048*500)+900)*2 )/4
					);
			//if( pixelData > 0xA0000000 )
			if( pixelData < 0x20000000 )
				nFrame2++;
			else if( pixelData > 0x40000000 )
				nFrame++;
		}

		if( nFramePre == 0 && nFrame != 0 && nFrame2 > 5 )
			log_noti("= Write %d dif %d %X =\n", u32Stc, u32Stc-u32VdecSendTime, *pDePixel);

		nFramePre = nFrame;

			/*
		if( preData < 0x70000000 && pixelData > 0x70000000 )
			log_noti("= Read %d dif %d %X =\n", u32Stc, u32Stc-u32VdecSendTime, pixelData);
		if( preDataW < 0x70000000 && pixelDataW > 0x70000000 )
			log_noti("= Write %d dif %d %X =\n", u32Stc, u32Stc-u32VdecSendTime, pixelData);
*/
		preData = pixelData;
		preDataW = pixelDataW;

		// check stop condition when device is closed
		if(kthread_should_stop())
		{
			break;
		}

		OS_MsecSleep(5);
	}
	return 0;
}
#endif

#if defined (CHIP_NAME_d14)
void _VDISP_SetFirstFrmInfo(S_DISPQ_BUF_T *pstDqFrm)
{
	UINT32 ui32CropW;
	UINT32 ui32CropH;
	UINT32 ui32Resol = 0;
	UINT32 ui32DualOutputMode = 0;
	UINT32 ui32FrmRate = 30;
	UINT32 u32Residual;
	UINT32 ui32ClkSrc;
	UINT32 ui32V_Offset;

	// 1. Resolution information
	if((pstDqFrm->ui32DpbMapMode == 3) || (pstDqFrm->ui32DpbMapMode == 4)) //HEVC crop(left, right, bottom)
	{//HEVC
		ui32CropW = (pstDqFrm->ui32PicWidth -( (pstDqFrm->ui32H_Offset >>16)&0xFFFF) ) - (pstDqFrm->ui32H_Offset&0xFFFF);
		// top and bottom crop set to DX_IPC
		ui32CropH = (pstDqFrm->ui32PicHeight & 0xFFFF) - ((pstDqFrm->ui32V_Offset >>16) & 0xFFFF) - (pstDqFrm->ui32V_Offset & 0xFFFF);
		ui32Resol = (ui32CropW << 16) | (ui32CropH & 0xFFFF);
	}
	else //H.264 crop(Right, bottom)
	{
		ui32CropW = pstDqFrm->ui32PicWidth - (pstDqFrm->ui32H_Offset&0xFFFF);
		ui32CropH = (pstDqFrm->ui32PicHeight & 0xFFFF) - (pstDqFrm->ui32V_Offset & 0xFFFF);
		ui32Resol = (ui32CropW << 16) | (ui32CropH & 0xFFFF);
	}

	// 2. FrameRate information
	if (pstDqFrm->ui32FrameRateDiv != 0)
	{
		ui32FrmRate = pstDqFrm->ui32FrameRateRes/pstDqFrm->ui32FrameRateDiv;
		ui32FrmRate *= 1000;
		u32Residual = pstDqFrm->ui32FrameRateRes%pstDqFrm->ui32FrameRateDiv;
		ui32FrmRate += (u32Residual*1000)/pstDqFrm->ui32FrameRateDiv;

		ui32ClkSrc = DIF_GetClockSrc();
		if(ui32ClkSrc == 1)//live
		{

			if( ui32FrmRate <= 23976 )
			{
				ui32FrmRate = 24;			// 23.976 Hz
			}
			else if( ui32FrmRate <= 24000 )
			{
				ui32FrmRate = 24;			// 24 Hz
			}
			else if( ui32FrmRate <= 25000 )
			{
				ui32FrmRate = 25;	// 25 Hz
			}
			else if( ui32FrmRate <= 29970 )
			{
				ui32FrmRate = 30;	// 29.97 Hz
			}
			else if( ui32FrmRate <= 30000 )
			{
				ui32FrmRate = 30;	// 30 Hz
			}
			else if( ui32FrmRate <= 50000 )
			{
				ui32FrmRate = 50;	// 50 Hz
			}
			else if( ui32FrmRate <= 59940 )
			{
				ui32FrmRate = 60;	// 59.94 Hz
			}
			else if( ui32FrmRate <= 60000 )
			{
				ui32FrmRate = 60;	// 60 Hz
			}
			else
			{
				log_error("not suported frame rate for VCP, Res %d, Div%d\n",
						pstDqFrm->ui32FrameRateRes, pstDqFrm->ui32FrameRateDiv);
				ui32FrmRate = 30;
			}
		}
		else if(ui32ClkSrc == 0) //not live
		{

			if( ui32FrmRate <= 23988 )
			{
				ui32FrmRate = 23;			// 23.976 Hz
			}
			else if( ui32FrmRate <= 24500 )
			{
				ui32FrmRate = 24;			// 24 Hz
			}
			else if( ui32FrmRate <= 27485 )
			{
				ui32FrmRate = 25;	// 25 Hz
			}
			else if( ui32FrmRate <= 29985 )
			{
				ui32FrmRate = 29;	// 29.97 Hz
			}
			else if( ui32FrmRate <= 40000 )
			{
				ui32FrmRate = 30;	// 30 Hz
			}
			else if( ui32FrmRate <= 54970 )
			{
				ui32FrmRate = 50;	// 50 Hz
			}
			else if( ui32FrmRate <= 59970 )
			{
				ui32FrmRate = 59;	// 59.94 Hz
			}
			else if( ui32FrmRate <= 60000 )
			{
				ui32FrmRate = 60;	// 60 Hz
			}
			else
			{
				log_error("not suported frame rate for VCP, Res %d, Div%d\n",
						pstDqFrm->ui32FrameRateRes, pstDqFrm->ui32FrameRateDiv);
				ui32FrmRate = 30;
			}
		}
	}
	// 3. Dual output mode information
	if((pstDqFrm->ui32DpbMapMode == 1) || (pstDqFrm->ui32DpbMapMode == 2) || (pstDqFrm->ui32DpbMapMode == 3))
	{
		//8bit
		ui32DualOutputMode = (0);
	}
	else if(pstDqFrm->ui32DpbMapMode == 4)
	{
		//10bit
		ui32DualOutputMode = (1);
	}
	//4. Vertical Crop information
	ui32V_Offset = pstDqFrm->ui32V_Offset;

 	DIF_SetFrameInfoDxIpc(ui32Resol, ui32DualOutputMode, ui32FrmRate, ui32V_Offset);
}
#endif

BOOLEAN _VDISP_Q_Pop(int ch, S_DISPQ_BUF_T *pstDqFrame)
{
	UINT8		u8VdispCh = ch;
	UINT32		u32Temp;
	BOOLEAN		ret;
	BOOLEAN		ret2;
#if defined (CHIP_NAME_d14)
	S_DISPQ_BUF_T pstRefFrame;
#endif

	ret = DISP_Q_Pop(ch, pstDqFrame);

	if (ret)
	{
		// for extract debugging info
		u32Temp = pstDqFrame->ui32DisplayPeriod;
		pstDqFrame->ui32DisplayPeriod = u32Temp & ((1<<3) -1);
		pstDqFrame->ui32FrameIdx = u32Temp >> 3;

		//For phase shift,check state of the setbasetime and first frame  
		if (bFirstFrameAfterSetBase[ch] == FALSE && bSetBaseTime[ch] == TRUE)
		{
			bFirstFrameAfterSetBase[ch] = TRUE;
			VSync_SetPrePhaseShift(ch,bFirstFrameAfterSetBase[ch]);
			log_user3("[Vdisp%d]State of SetBaseTime(%s) and First frame after SetBaseTime(%s)\n"
										, u8VdispCh, (bSetBaseTime[ch] == TRUE)?"TRUE":"FALSE"
										, (bFirstFrameAfterSetBase[ch] == TRUE)?"TRUE":"FALSE");
		}
		else
		{
			bFirstFrameAfterSetBase[ch] = FALSE;
			bSetBaseTime[ch] = FALSE;
			VSync_SetPrePhaseShift(ch,bFirstFrameAfterSetBase[ch]);
			log_user3("[Vdisp%d]State of SetBaseTime(%s) and First frame after SetBaseTime(%s)\n"
										, u8VdispCh, (bSetBaseTime[ch] == TRUE)?"TRUE":"FALSE"
										, (bFirstFrameAfterSetBase[ch] == TRUE)?"TRUE":"FALSE");

		}
		if (bLiveChannelOpen[u8VdispCh])
		{
			bLivePrephaseShift[u8VdispCh] = TRUE;
			VSync_SetPrePhaseShift(u8VdispCh,bLivePrephaseShift[u8VdispCh]);
			log_user3("[Vdisp%d]Set Live Tv prephaseshift condition : TRUE Cnt: %d\n"
											,ch, ui8PrePhaseshiftCntLive[ch]);
		}
		else
			log_user3("[Vdisp%d]Set Live Tv prephaseshift condition : FALSE Cnt: %d\n"
											,ch, ui8PrePhaseshiftCntLive[ch]);
#if defined (CHIP_NAME_d14)
		if(bFirstFrmInfoSet == FALSE)
		{
			_VDISP_SetFirstFrmInfo(pstDqFrame);
			bFirstFrmInfoSet = TRUE;
		}
		
		//For BBC & Amazon
		if(ui8Seamless == 2/* || ui8Seamless == 3*/)
		{
			if(i32VsiInfoTimming == 1) //1-frame pre setting for VSI info
			{
				ui32PicW = pstDqFrame->ui32PicWidth;
				ui32PicH = pstDqFrame->ui32PicHeight;
				ui32VOff = pstDqFrame->ui32V_Offset;
				ui32HOff = pstDqFrame->ui32H_Offset;
			}
			else if(i32VsiInfoTimming == 2) //2-frame pre setting for VSI info
			{
				ret2 = DISP_Q_Ref(ch, &pstRefFrame);
				if (ret2)
				{
					ui32PicW = pstRefFrame.ui32PicWidth;
					ui32PicH = pstRefFrame.ui32PicHeight;
					ui32VOff = pstRefFrame.ui32V_Offset;
					ui32HOff = pstRefFrame.ui32H_Offset;
				}
				else//if Qlen is 0, no ref frame info
				{
					ui32PicW = pstDqFrame->ui32PicWidth;
					ui32PicH = pstDqFrame->ui32PicHeight;
					ui32VOff = pstDqFrame->ui32V_Offset;
					ui32HOff = pstDqFrame->ui32H_Offset;
				}
			}
		}
#endif
		log_user1("[Vdisp%d] Pop Idx %2d  Rate %d/%d PTS %5u%s STC %5u Qlen %d "
				"Dispi %d/%d %c MT%d bufT %d\n",
				u8VdispCh, //pstDqFrame->ui32Y_FrameBaseAddr,
				pstDqFrame->ui32FrameIdx,
				pstDqFrame->ui32FrameRateRes, pstDqFrame->ui32FrameRateDiv,
				pstDqFrame->ui32PTS,
				pstDqFrame->bDiscont?" Discont.":"",
				VDISP_GetSTC(u8VdispCh),
				DISP_Q_NumActive(u8VdispCh),
				pstDqFrame->ui32DisplayInfo,
				pstDqFrame->ui32DisplayPeriod,
				pstDqFrame->ui32LR_Order == DISPQ_3D_FRAME_LEFT ? 'L':
				(pstDqFrame->ui32LR_Order == DISPQ_3D_FRAME_RIGHT ? 'R' : ' '),
				pstDqFrame->ui32DpbMapMode,
				TOP_HAL_GetGSTCC() - pstDqFrame->enq_gstc
				);
	}

	return ret;
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
static BOOLEAN _VDISP_GetCmdQ(UINT8 u8VdispCh, VDISP_CMD_T *pstCmd)
{
	UINT32		u32WrIndex;
	UINT32		u32RdIndex, u32RdIndex_Next;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] %s\n", u8VdispCh, __FUNCTION__ );
		return FALSE;
	}

	u32WrIndex = gsVdisp[u8VdispCh].CmdQ.u32WrIndex;
	u32RdIndex = gsVdisp[u8VdispCh].CmdQ.u32RdIndex;

	if( u32WrIndex == u32RdIndex )
		return FALSE;

	*pstCmd = gsVdisp[u8VdispCh].CmdQ.astCmd[u32RdIndex];

	log_noti("[Vdisp%d-%d][DBG] Cmd: %d, %s\n", u8VdispCh,
			gsVdisp[u8VdispCh].State, pstCmd->eCmdType, __FUNCTION__ );

	u32RdIndex_Next = u32RdIndex + 1;
	if( u32RdIndex_Next == VDISP_NUM_OF_CMD_Q )
		u32RdIndex_Next = 0;

	gsVdisp[u8VdispCh].CmdQ.u32RdIndex = u32RdIndex_Next;

	return TRUE;
}

UINT32 VDISP_GetSTC(UINT8 u8VdispCh)
{
	VDISP_CH_T	*pstVdispCh;
	UINT32				u32STC = 0xFFFFFFFF;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] %s", u8VdispCh, __FUNCTION__ );
		return 0xFFFFFFFF;
	}

	pstVdispCh = &gsVdisp[u8VdispCh];

	if( pstVdispCh->eClkSrc == VDISP_CLK_GSTC )
		u32STC = TOP_HAL_GetGSTCC() & VDEC_PTS_MASK;
	else
		u32STC = PDEC_HAL_GetStc(pstVdispCh->eClkSrc - VDISP_CLK_PDEC0) & VDEC_PTS_MASK;

	return u32STC;
}

static void VDISP_InitDualCh(void)
{
	UINT8 i;

	for(i=0; i<VDISP_DUAL_CH_NUM; i++)
	{
		gstDualCh[i].u8nPluged = 0;
		gstDualCh[i].pstVdispCh[0] = NULL;
		gstDualCh[i].pstVdispCh[1] = NULL;
		gstDualCh[i].u8EmptyCnt[0] = 0;
		gstDualCh[i].u8EmptyCnt[1] = 0;
		gstDualCh[i].stLock = __SPIN_LOCK_UNLOCKED(gstDualCh[i].stLock);
	}

	return;
}

static DualCh_T* VDISP_GetFreeDualCh(void)
{
	UINT8 i;
	DualCh_T*	pstDualCh = NULL;

	for(i=0; i<VDISP_DUAL_CH_NUM; i++)
		if( gstDualCh[i].u8nPluged < 2)
			break;

	if( i< VDISP_DUAL_CH_NUM )
		pstDualCh = &gstDualCh[i];

	return pstDualCh;
}

static DualCh_T* VDISP_SetDualCh(VDISP_CH_T *pstVdispCh)
{
	DualCh_T*	pstDualCh = NULL;
	UINT32		u8VdispIdx;

	pstDualCh = VDISP_GetFreeDualCh();

	if( pstDualCh == NULL )
	{
		log_error("[Vdisp][Err] insufficient dual channel\n");
		return NULL;
	}

	for(u8VdispIdx=0; u8VdispIdx<2; u8VdispIdx++)
		if( pstDualCh->pstVdispCh[u8VdispIdx] == NULL )
			break;

	if( u8VdispIdx == 2 )
	{
		log_error("Wrong Dual Channel Vdisp Idx %d\n", u8VdispIdx);
		return NULL;
	}

	pstDualCh->pstVdispCh[u8VdispIdx] = pstVdispCh;
	pstDualCh->u8nPluged++;
	pstVdispCh->bIsDualCh = TRUE;
	pstVdispCh->pstDualCh = pstDualCh;

	if(pstDualCh->u8nPluged == 1)
	{
		// init
		pstDualCh->u8EmptyCnt[0] = 0;
		pstDualCh->u8EmptyCnt[1] = 0;
	}
	else if(pstDualCh->u8nPluged == 2)
	{
		pstDualCh->pstVdispCh[0]->pstPairCh = pstDualCh->pstVdispCh[1];
		pstDualCh->pstVdispCh[1]->pstPairCh = pstDualCh->pstVdispCh[0];
	}

	log_noti("DualCh %p Plug Vdisp%d  n:%d\n",
			pstDualCh, pstVdispCh->u8VdispCh, pstDualCh->u8nPluged);

	return pstDualCh;
}

static void VDISP_FreeDualCh(VDISP_CH_T *pstVdispCh)
{
	DualCh_T*	pstDualCh = pstVdispCh->pstDualCh;
	UINT32		u8VdispIdx;

	if( pstDualCh == NULL )
	{
		log_error("not belong to dual ch\n");
		return;
	}

	for(u8VdispIdx=0; u8VdispIdx<2; u8VdispIdx++)
		if( pstDualCh->pstVdispCh[u8VdispIdx] == pstVdispCh )
			break;

	if( u8VdispIdx == 2 )
	{
		log_error("Wrong Dual Channel Vdisp Idx %d\n", u8VdispIdx);
		return;
	}

	if(pstDualCh->u8nPluged == 2)
	{
		pstDualCh->pstVdispCh[0]->pstPairCh = NULL;
		pstDualCh->pstVdispCh[1]->pstPairCh = NULL;
	}

	pstDualCh->u8nPluged--;
	pstDualCh->pstVdispCh[u8VdispIdx] = NULL;
	pstVdispCh->bIsDualCh = FALSE;
	pstVdispCh->pstDualCh = NULL;


	log_noti("DualCh %p: Free Vdisp%d  n:%d\n",
			pstDualCh, pstVdispCh->u8VdispCh, pstDualCh->u8nPluged);

	return;
}

VDISP_CH_T* VDISP_GetHandler(UINT8 u8VdispCh)
{
	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n", u8VdispCh, __FUNCTION__ );
		return NULL;
	}

	return &gsVdisp[u8VdispCh];
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
static BOOLEAN _VDISP_PutCmdQ(UINT8 u8VdispCh, VDISP_CMD_T *pstCmd)
{
	UINT32		u32WrIndex, u32WrIndex_Next;
	UINT32		u32RdIndex;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] %s\n", u8VdispCh, __FUNCTION__ );
		return FALSE;
	}

	u32WrIndex = gsVdisp[u8VdispCh].CmdQ.u32WrIndex;
	u32RdIndex = gsVdisp[u8VdispCh].CmdQ.u32RdIndex;

	u32WrIndex_Next = u32WrIndex + 1;
	if( u32WrIndex_Next == VDISP_NUM_OF_CMD_Q )
		u32WrIndex_Next = 0;

	if( u32WrIndex_Next == u32RdIndex )
	{
		log_error("[Vdisp%d][Err] Cmd Q Overflow\n", u8VdispCh );
		return FALSE;
	}

	log_noti("[Vdisp%d-%d][DBG] Cmd: %d, %s\n", u8VdispCh,
			gsVdisp[u8VdispCh].State, pstCmd->eCmdType, __FUNCTION__ );

	gsVdisp[u8VdispCh].CmdQ.astCmd[u32WrIndex] = *pstCmd;
	gsVdisp[u8VdispCh].CmdQ.u32WrIndex = u32WrIndex_Next;

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
void VDISP_Init(void)
{
	UINT8	u8VdispCh;

	if( bVdispInit == TRUE )
	{
		log_warning("VDISP is already initialized\n");
		return;
	}

	bVdispInit = TRUE;

	VDISP_InitDualCh();

	for( u8VdispCh = 0; u8VdispCh < VDISP_NUM_OF_CH; u8VdispCh++ )
	{
		gsVdisp[u8VdispCh].stLock = __SPIN_LOCK_UNLOCKED(gsVdisp[u8VdispCh].stLock);
		gsVdisp[u8VdispCh].State = VDISP_NULL;
		gsVdisp[u8VdispCh].u8VdispCh = u8VdispCh;
		gsVdisp[u8VdispCh].u32PrePts = 0xFFFFFFFF;
		gsVdisp[u8VdispCh].bIsDualCh = FALSE;
		gsVdisp[u8VdispCh].pstDualCh = NULL;
		gsVdisp[u8VdispCh].pstPairCh = NULL;

		gsVdisp[u8VdispCh].Config.eDeIfDstCh = 0xFF;
		gsVdisp[u8VdispCh].Config.u8VSyncCh = 0xFF;

		gsVdisp[u8VdispCh].CmdQ.u32WrIndex = 0;
		gsVdisp[u8VdispCh].CmdQ.u32RdIndex = 0;

		gsVdisp[u8VdispCh].Status.bStepping = FALSE;
		gsVdisp[u8VdispCh].Status.bPlaying = FALSE;
		gsVdisp[u8VdispCh].Status.bClosing = FALSE;
		gsVdisp[u8VdispCh].Status.bFreeze = FALSE;

		gsVdisp[u8VdispCh].Debug.u32LogTick_UpdateBuf = 0;

		gsVdisp[u8VdispCh].pstSync = PTS_GetHandler(u8VdispCh);
		bFirstFrameAfterSetBase[u8VdispCh] = FALSE;
		bLiveChannelOpen[u8VdispCh] = FALSE;
		bLivePrephaseShift[u8VdispCh] = FALSE;
		bSetBaseTime[u8VdispCh] = FALSE;
		ui8PrePhaseshiftCnt[u8VdispCh] = 0;
		ui8PrePhaseshiftCntLive[u8VdispCh] = 0;
	}

	PTS_Init();
	VSync_Init(VDISP_VsyncCallback);
	DE_IF_Init();

	VDISP_proc_make_instance();

#ifdef VDISP_DBG_LIPSYNC
	pDeInfo		= (volatile UINT32 *)VDEC_TranselateVirualAddr(0xC0014B40, 0x20);
	//pDePixel	= (volatile UINT32 *)VDEC_TranselateVirualAddr(0x90000000, 0x800000);
	pDePixel	= (volatile UINT32 *)VDEC_TranselateVirualAddr(0x98000000, 0x32A0000);
	gpstVdispTask = kthread_run(VDISP_MonTask, (void *)NULL, "vdisp_mon_thread");
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

VDISP_CH_T* VDISP_AllocChannel(E_VDISP_DST_T eOutPort)
{
	VDISP_CH_T		*pstVdispCh;
	UINT8			u8VdispCh = (UINT8)eOutPort;
	UINT32			i;

	// if possible, use eOutput No. as VDISP channel No. for convenience
	if (u8VdispCh<VDISP_NUM_OF_CH)
	{
		pstVdispCh = VDISP_GetHandler(u8VdispCh);
		if (pstVdispCh->State == VDISP_NULL)
			return pstVdispCh;
	}

	//for (i=VDISP_NUM_OF_CH-1;i>=0;i--)
	for (i=0;i<VDISP_NUM_OF_CH;i++)
	{
		pstVdispCh = VDISP_GetHandler(i);
		if (pstVdispCh->State == VDISP_NULL)
			return pstVdispCh;
	}

	return NULL;
}

void vdisp_reset_handler(VDISP_CH_T *pstVdispCh)
{
	//pstVdispCh->State = VDISP_NULL;
	pstVdispCh->stSyncFrame.eState = SYNC_FRM_INVALID;
	pstVdispCh->stSyncFrame.bValidPairFrame = FALSE;
	pstVdispCh->u32PrePts = 0xFFFFFFFF;
	pstVdispCh->u32PreOrgPts = 0xFFFFFFFF;
	pstVdispCh->u32PreDispPeriod = 0;

	pstVdispCh->bValidCurFrame = FALSE;
	pstVdispCh->bValidNextFrame = FALSE;
	pstVdispCh->bValidNextPair = FALSE;
	pstVdispCh->bForcedPush = TRUE;
	pstVdispCh->bLock4Pair = TRUE;
	pstVdispCh->u32PhaseErrorCnt = 0;
	pstVdispCh->i32PhaseShift = 0;
	pstVdispCh->i32VsyncDiff = 0;
	pstVdispCh->dq_enq_cnt = 0;
	pstVdispCh->is_film_mode = FALSE;
	pstVdispCh->film_mode_cnt = 0;

	pstVdispCh->n_diff_sum = 0;
	pstVdispCh->diff_sum = 0;
	pstVdispCh->diff_var = 0;

	pstVdispCh->Status.bStepping = FALSE;
	pstVdispCh->Status.bPlaying = FALSE;
	pstVdispCh->Status.bClosing = FALSE;
	pstVdispCh->Status.bFreeze = FALSE;

	pstVdispCh->lt_rate = 0;
	pstVdispCh->lt_rate_cnt = -5;
	pstVdispCh->lt_rate_strat = 0;

	return;
}

UINT8 VDISP_Open(E_VDISP_DST_T eOutPort, VDISP_CLK_SRC eClkSrc,
		UINT32 u32DisplayOffset_ms, UINT32 u32NumOfDq,
		BOOLEAN bIsDualDecoding, BOOLEAN bFixedVSync,
		VDISP_STATUS_CB fpStatusCb, VDISP_3D_DUAL_TYPE_E eDual3dtype)
{
	VDISP_CH_T *pstVdispCh;
	UINT8	u8VdispCh = 0xFF;
	UINT8	u8VSyncCh = 0xFF;

	pstVdispCh = VDISP_AllocChannel(eOutPort);
	if( pstVdispCh == NULL )
	{
		log_error("[Vdisp%d][Err] Fail to alloc Vdisp handler\n", u8VdispCh);
		return 0xFF;
	}
	u8VdispCh = pstVdispCh->u8VdispCh;

	log_noti("[VDISP%d] OPEN - Outport %d, CLK %d, DispOffset %dms %s %s\n",
			u8VdispCh, eOutPort, eClkSrc, u32DisplayOffset_ms,
			bIsDualDecoding?"Dual":"", bFixedVSync?"Fixed":"");

	if( eClkSrc >= VDISP_CLK_MAX || eClkSrc == VDISP_CLK_NONE )
	{
		log_error("[Vdisp%d][Err] Wrong Clock Src %d\n", u8VdispCh, eClkSrc);
		return 0xFF;
	}

	if( DISP_Q_Open(u8VdispCh, u32NumOfDq) == FALSE )
	{
		log_error("[Vdisp%d-%d][Err] Failed to Alloc Display Queue Memory, %s\n",
				u8VdispCh, pstVdispCh->State, __FUNCTION__ );
		return 0xFF;
	}

	log_noti("[Vdisp%d-%d][DBG] Display Offset: %d, ClkSrc: %s\n",
			u8VdispCh, pstVdispCh->State, u32DisplayOffset_ms,
			gazClkSrcStr[eClkSrc] );

	if( DE_IF_Open(u8VdispCh, eOutPort) == FALSE )
	{
		DISP_Q_Close(u8VdispCh);
		return 0xFF;
	}

	DE_IF_SetPrivateData(u8VdispCh, pstVdispCh);

	pstVdispCh->Status.fpStatus = fpStatusCb;

	pstVdispCh->pstSync = PTS_Open(u8VdispCh, eClkSrc==VDISP_CLK_GSTC,
			u32DisplayOffset_ms, bIsDualDecoding);
	pstVdispCh->eClkSrc = eClkSrc;

	if (bIsDualDecoding) VDISP_SetDualCh(pstVdispCh);

	u8VSyncCh = VSync_Open(u8VdispCh, bIsDualDecoding, gDbgDefaultRateRes, gDbgDefaultRateDiv, FALSE, (UINT8)eDual3dtype);

	// set VDEC/DE vsync connection
	DE_IF_SelVsync((DE_IF_DST_E)eOutPort, u8VSyncCh);

	vdisp_reset_handler(pstVdispCh);
	pstVdispCh->Config.eDeIfDstCh = (DE_IF_DST_E)u8VdispCh;
	pstVdispCh->Config.u8VSyncCh = u8VSyncCh;
	pstVdispCh->State = VDISP_READY;
#if defined (CHIP_NAME_d14)
	delayoffsetflag = 0;
	uhd_ex_delay = 0;
	ui8FrameCnt = 0;
	bFirstFrmInfoSet = FALSE;
	if (pstVdispCh->Config.eDeIfDstCh == 0)
	{
		pre_uhd_out_mode = 0;
		uhd_chg_cnt = 0;
		DIF_SetOutChannelMode(0);
	}
#endif
	bFirstFrameAfterSetBase[u8VdispCh] = FALSE;
	bLiveChannelOpen[u8VdispCh] = FALSE;
	bLivePrephaseShift[u8VdispCh] = FALSE;
	bSetBaseTime[u8VdispCh] = FALSE;
	ui8PrePhaseshiftCnt[u8VdispCh] = 0;
	ui8PrePhaseshiftCntLive[u8VdispCh] = 0;

	//For live play, prephase shift condition is setted
	if (pstVdispCh->eClkSrc != VDISP_CLK_GSTC)
	{
		bLiveChannelOpen[u8VdispCh] = TRUE;
		ui8PrePhaseshiftCntLive[u8VdispCh] = 0;
	}
	log_noti("	VDISP%d Open Success - VSYNC:%d\n",	u8VdispCh, u8VSyncCh);

	return u8VdispCh;
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
void VDISP_Close(UINT8 u8VdispCh)
{
	VDISP_CH_T *pstVdispCh;
	VDISP_CMD_T	stCmd;
	unsigned long flags, flags2=0;
	DualCh_T		*pstDualCh = NULL;

	pstVdispCh = VDISP_GetHandler(u8VdispCh);
	if( pstVdispCh == NULL )
	{
		log_error("[Vdisp%d][Err] Fail to get Vdisp handler\n", u8VdispCh);
		return;
	}

	spin_lock_irqsave (&pstVdispCh->stLock, flags);

	if( pstVdispCh->pstDualCh != NULL &&
			pstVdispCh->pstDualCh->pstVdispCh[1] == pstVdispCh )
	{
		pstDualCh = pstVdispCh->pstDualCh;
		spin_lock_irqsave(&pstDualCh->stLock, flags2);
	}


	log_noti("[Vdisp%d-%d] vdisp channel close\n", u8VdispCh, pstVdispCh->State);

	if( pstVdispCh->State == VDISP_NULL )
	{
		log_error("[Vdisp%d-%d] State Error\n", u8VdispCh, pstVdispCh->State);
		goto exit_close;
	}

	// from _VDISP_Close
	VSync_Close(pstVdispCh->Config.u8VSyncCh, u8VdispCh);
	DE_IF_Close(pstVdispCh->Config.eDeIfDstCh);

	if( pstVdispCh->pstDualCh != NULL )
		VDISP_FreeDualCh(&gsVdisp[u8VdispCh]);

	PTS_Close(pstVdispCh->pstSync);

	pstVdispCh->State = VDISP_NULL;
	pstVdispCh->Config.eDeIfDstCh = 0xFF;
	pstVdispCh->Config.u8VSyncCh = 0xFF;

	pstVdispCh->Status.bClosing = FALSE;
	////////////////////////////////

	while( _VDISP_GetCmdQ( u8VdispCh, &stCmd ) == TRUE );

	DISP_Q_Close(u8VdispCh);

exit_close:

	if (pstDualCh != NULL)
		spin_unlock_irqrestore(&pstDualCh->stLock, flags2);

	spin_unlock_irqrestore (&pstVdispCh->stLock, flags);
}

void VDISP_Suspend(void)
{
	VSYNC_Suspend();
	DEIF_Suspend();
}

void VDISP_Resume(void)
{
	DEIF_Resume();
	VSYNC_Resume();
}

E_VDISP_DST_T VDISP_SetOutPort(UINT8 u8VdispCh, E_VDISP_DST_T eOutPort)
{
	VDISP_CH_T 			*pstVdispCh;
	VDISP_CMD_T			stCmd;
	E_VDISP_DST_T		ePreOutPort;

	pstVdispCh = VDISP_GetHandler(u8VdispCh);

	if( pstVdispCh == NULL )
	{
		log_error("[Vdisp%d][Err] Fail to get Vdisp handler\n", u8VdispCh);
		return 0xFF;
	}

	ePreOutPort = DE_IF_GetTargetDe(pstVdispCh->Config.eDeIfDstCh);

	log_noti("[Vdisp%d-%d][DBG] Set Outport: %d->%d\n", u8VdispCh,
			gsVdisp[u8VdispCh].State, ePreOutPort, eOutPort);

	stCmd.eCmdType = VDISP_CMD_SET_OUT;
	stCmd.u32Param = eOutPort;
	_VDISP_PutCmdQ( u8VdispCh, &stCmd );

	return ePreOutPort;
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
void VDISP_SetDisplayOffset(UINT8 u8VdispCh, UINT32 u32DisplayOffset_ms)
{
	VDISP_CH_T 			*pstVdispCh;

	pstVdispCh = VDISP_GetHandler(u8VdispCh);

	if( pstVdispCh == NULL )
	{
		log_error("[Vdisp%d][Err] Fail to get Vdisp handler\n", u8VdispCh);
		return;
	}

#if defined(CHIP_NAME_d14)
	if (ui8Seamless == 1)
	{
		log_noti("[Vdisp%d] add seamless delay: org %d ms\n",
				u8VdispCh, u32DisplayOffset_ms);

		u32DisplayOffset_ms -= (83+60);

		if (pstVdispCh->Config.eDeIfDstCh == 1 && delayoffsetflag)
		{
			log_noti("[Vdisp%d] add ud delay: org %d ms, ex %d ms\n",
					u8VdispCh, u32DisplayOffset_ms, uhd_ex_delay);
			u32DisplayOffset_ms += uhd_ex_delay;
		}
	}
#endif

	log_noti("[Vdisp%d][DBG] DisplayOffset: %dms, %s\n",
			u8VdispCh, u32DisplayOffset_ms, __FUNCTION__);
	ui32DisplayOffset_90K = u32DisplayOffset_ms*90;
	PTS_SetDisplayOffset(pstVdispCh->pstSync, u32DisplayOffset_ms);

}


void VDISP_SetFrameRate(UINT8 u8VdispCh, UINT32 u32Res, UINT32 u32Div)
{
	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n",
				u8VdispCh, __FUNCTION__ );
		return;
	}

	log_noti("[Vdisp%d] Config Rate %d/%d\n", u8VdispCh, u32Res, u32Div);

	VRATE_UpdateConfigRate (u8VdispCh, u32Res, u32Div);
}

UINT32 VDISP_GetRateInfo(UINT8 u8VdispCh, UINT32 *pui32FrameRateRes,
		UINT32 *pui32FrameRateDiv)
{
	UINT32 u32Res;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n",
				u8VdispCh, __FUNCTION__ );
		return 0;
	}

	u32Res = VRATE_GetResDiv(u8VdispCh, pui32FrameRateRes, pui32FrameRateDiv);

	log_user3("[Vdisp%d] Get Rate Info %d/%d\n", u8VdispCh, *pui32FrameRateRes,
			*pui32FrameRateDiv);

	return u32Res;
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
void VDISP_Reset(UINT8 u8VdispCh)
{
	VDISP_CMD_T stCmd;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] %s\n", u8VdispCh, __FUNCTION__ );
		return;
	}

	log_noti("[Vdisp%d-%d][DBG] %s \n", u8VdispCh, gsVdisp[u8VdispCh].State, __FUNCTION__ );

	stCmd.eCmdType = VDISP_CMD_RESET;
	stCmd.u32Param = 0;
	_VDISP_PutCmdQ( u8VdispCh, &stCmd );
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
void VDISP_Start(UINT8 u8VdispCh, BOOLEAN bStep)
{
	VDISP_CMD_T stCmd;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n",
				u8VdispCh, __FUNCTION__ );
		return;
	}

	log_noti("[Vdisp%d-%d][DBG] Step:%d, %s \n", u8VdispCh,
			gsVdisp[u8VdispCh].State, bStep, __FUNCTION__ );

	gsVdisp[u8VdispCh].Status.bPlaying = TRUE;

	if( bStep )
		stCmd.eCmdType = VDISP_CMD_STEP;
	else
		stCmd.eCmdType = VDISP_CMD_PLAY;

	stCmd.u32Param = 0;
	_VDISP_PutCmdQ( u8VdispCh, &stCmd );
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
void VDISP_Stop(UINT8 u8VdispCh)
{
	VDISP_CMD_T stCmd;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n", u8VdispCh, __FUNCTION__ );
		return;
	}

	log_noti("[Vdisp%d-%d][DBG] %s \n", u8VdispCh,
			gsVdisp[u8VdispCh].State, __FUNCTION__ );

	stCmd.eCmdType = VDISP_CMD_PAUSE;
	stCmd.u32Param = 0;
	_VDISP_PutCmdQ( u8VdispCh, &stCmd );
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
void VDISP_Flush(UINT8 u8VdispCh)
{
	VDISP_CMD_T stCmd;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n", u8VdispCh, __FUNCTION__ );
		return;
	}

	log_noti("[Vdisp%d-%d][DBG] %s \n", u8VdispCh, gsVdisp[u8VdispCh].State, __FUNCTION__ );

	stCmd.eCmdType = VDISP_CMD_FLUSH;
	stCmd.u32Param = 0;
	_VDISP_PutCmdQ( u8VdispCh, &stCmd );
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
void VDISP_Freeze( UINT8 u8VdispCh, BOOLEAN bOnOff )
{
	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n", u8VdispCh, __FUNCTION__ );
		return;
	}

	log_noti("[Vdisp%d-%d][DBG] Freeze:%d\n",
			u8VdispCh, gsVdisp[u8VdispCh].State, bOnOff);


	DE_IF_Freeze( gsVdisp[u8VdispCh].Config.eDeIfDstCh, bOnOff );
	gsVdisp[u8VdispCh].Status.bFreeze = bOnOff;
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

void VDISP_RepeatFrame(UINT8 u8DeCh, UINT8 u8nRepeat)
{
	VDISP_CMD_T stCmd;
	UINT8 u8VdispCh, i;
	VDISP_CH_T *pstVdispCh;

	for(i=0; i<VDISP_NUM_OF_CH ; i++)
	{
		pstVdispCh = VDISP_GetHandler(i);
		if(pstVdispCh!= NULL && pstVdispCh->State != VDISP_NULL &&
				pstVdispCh->Config.eDeIfDstCh == u8DeCh)
			break;
	}

	if( i<VDISP_NUM_OF_CH )
	{
		u8VdispCh = i;
		log_noti("[Vdisp%d-%d] repeat %d\n", u8VdispCh, gsVdisp[u8VdispCh].State, u8nRepeat);
		stCmd.eCmdType = VDISP_CMD_REPEAT;
		stCmd.u32Param = (UINT32)u8nRepeat;
		_VDISP_PutCmdQ( u8VdispCh, &stCmd );
	}
	else
	{
		log_noti("cant find open de channel %d\n", u8DeCh);
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
void VDISP_SetStcDiscont(UINT8 u8VdispCh, UINT32 u32AuiId)
{
	log_noti("[Vdisp%d] Dicontinuity AU ID %u\n", u8VdispCh, u32AuiId);
	//
	//PTS_SetStcDiscont(gsVdisp[u8VdispCh].pstSync);
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
void VDISP_SetMatchMode(UINT8 u8VdispCh, PTS_MATCH_MODE_E eMatchMode)
{
	VDISP_CH_T	*pstVdispCh;
	VDISP_CH_T	*pstPairCh;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n", u8VdispCh, __FUNCTION__ );
		return;
	}

	if (gMatchModeDbg != 0xFF)
		eMatchMode = (PTS_MATCH_MODE_E)gMatchModeDbg;

	log_noti("[Vdisp%d][DBG] Set Match Mode %d\n", u8VdispCh, eMatchMode);

	pstVdispCh = VDISP_GetHandler(u8VdispCh);

	pstPairCh = pstVdispCh->pstPairCh;
	if( pstVdispCh->pstDualCh != NULL && pstPairCh != NULL )
		PTS_SetMatchMode(pstPairCh->pstSync, eMatchMode);

	PTS_SetMatchMode(pstVdispCh->pstSync, eMatchMode);
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
void VDISP_SetMaxQlen(UINT8 u8VdispCh, UINT8 u8MaxQlen)
{
	VDISP_CH_T	*pstVdispCh;
	VDISP_CH_T	*pstPairCh;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n",
				u8VdispCh, __FUNCTION__);
		return;
	}

	log_noti("[VDISP%d] Set max qlen %d\n", u8VdispCh, u8MaxQlen);

	pstVdispCh = VDISP_GetHandler(u8VdispCh);

	pstPairCh = pstVdispCh->pstPairCh;
	if( pstVdispCh->pstDualCh != NULL && pstPairCh != NULL )
		PTS_SetMaxQlen(pstPairCh->pstSync, u8MaxQlen);

	PTS_SetMaxQlen(pstVdispCh->pstSync, u8MaxQlen);

	return;
}

UINT32 VDISP_GetMaxQlen(UINT8 u8VdispCh)
{
	VDISP_CH_T	*pstVdispCh;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n",
				u8VdispCh, __FUNCTION__);
		return 0;
	}

	pstVdispCh = VDISP_GetHandler(u8VdispCh);

	return PTS_GetMaxQlen(pstVdispCh->pstSync);
}

void VDISP_SetSpeed( UINT8 u8VdispCh, SINT32 i32Speed )
{
	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d] Channel Number, %s\n", u8VdispCh, __FUNCTION__);
		return;
	}

	log_noti("[Vdisp%d-%d] Speed: %d\n", u8VdispCh,
			gsVdisp[u8VdispCh].State, i32Speed);

	VRATE_SetSpeed(u8VdispCh, i32Speed);
}

void VDISP_BBCVsiSetTime( UINT8 u8VdispCh, SINT32 i32VSISetTimming )
{
	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d] Channel Number, %s\n", u8VdispCh, __FUNCTION__);
		return;
	}
	i32VsiInfoTimming = i32VSISetTimming;
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
SINT32 VDISP_GetSpeed( UINT8 u8VdispCh )
{
	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n", u8VdispCh, __FUNCTION__ );
		return 0;
	}

	return VRATE_GetSpeed(u8VdispCh);
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
void VDISP_SetBaseTime(UINT8 u8VdispCh, UINT32 u32BaseTime, UINT32 u32BasePTS)
{
	VDISP_CH_T	*pstVdispCh;
	VDISP_CH_T	*pstPairCh;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d] Channel Number, %s\n", u8VdispCh, __FUNCTION__ );
		return;
	}

#if defined(CHIP_NAME_d14)
	{
		UINT32		cur_gstc;
		cur_gstc = VDISP_GetSTC(u8VdispCh);

		log_noti("[VDISP%d] Basetime transform curSTC %u STCb %u PTSb %u\n",
				u8VdispCh, cur_gstc, u32BaseTime, u32BasePTS);

		u32BasePTS = u32BasePTS + (cur_gstc - u32BaseTime);
		u32BasePTS &= VDEC_PTS_MASK;
		u32BaseTime = cur_gstc;
	}
#endif
	if (!(u32BaseTime == 0 && u32BasePTS == 0))
	{
		bSetBaseTime[u8VdispCh] = TRUE;
		ui8PrePhaseshiftCnt[u8VdispCh] = 0;
	}
	log_noti("[VDISP%d]    Set new -> BaseTime:%u, PTS:%u\n", u8VdispCh,
			u32BaseTime, u32BasePTS);

	pstVdispCh = VDISP_GetHandler(u8VdispCh);

	pstPairCh = pstVdispCh->pstPairCh;
	if( pstVdispCh->pstDualCh != NULL && pstPairCh != NULL )
		PTS_Set_BaseTime(pstPairCh->pstSync, u32BaseTime, u32BasePTS);

	PTS_Set_BaseTime(pstVdispCh->pstSync, u32BaseTime, u32BasePTS);
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
void VDISP_GetBaseTime(UINT8 u8VdispCh, UINT32 *pui32BaseTime, UINT32 *pui32BasePTS)
{
	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] Channel Number, %s\n", u8VdispCh, __FUNCTION__ );
		return;
	}

	PTS_Get_BaseTime(gsVdisp[u8VdispCh].pstSync, pui32BaseTime, pui32BasePTS);
}

void VDISP_GetCurrentInfo(UINT8 u8DeCh, S_VDISP_DISP_INFO_T *info)
{
	VDISP_CH_T	*pstVdispCh;
	UINT8 i;

	info->width = 0;
	info->height = 0;

	for(i=0; i<VDISP_NUM_OF_CH ; i++)
	{
		pstVdispCh = VDISP_GetHandler(i);
		if(pstVdispCh!= NULL && pstVdispCh->State != VDISP_NULL &&
				pstVdispCh->Config.eDeIfDstCh == u8DeCh)
			break;
	}

	if( i<VDISP_NUM_OF_CH )
	{
		if(pstVdispCh->bValidCurFrame)
		{
			info->width = pstVdispCh->stCurFrame.ui32PicWidth;
			info->width -= pstVdispCh->stCurFrame.ui32H_Offset & 0xFFFF;
			info->width -= (pstVdispCh->stCurFrame.ui32H_Offset>>16) & 0xFFFF;

			info->height = pstVdispCh->stCurFrame.ui32PicHeight;
			info->height -= pstVdispCh->stCurFrame.ui32V_Offset & 0xFFFF;
			info->height -= (pstVdispCh->stCurFrame.ui32V_Offset>>16) & 0xFFFF;

			info->fr_num = pstVdispCh->stCurFrame.ui32FrameRateRes;
			info->fr_den = pstVdispCh->stCurFrame.ui32FrameRateDiv;
		}
	}
}

BOOLEAN VDISP_SetDualOutput(E_VDISP_DST_T src, BOOLEAN bOn)
{
	DE_IF_DST_E		ePreDst = DE_IF_DST_NONE;
	UINT32			chip;
	BOOLEAN			bNotSupport = FALSE;

	chip = vdec_chip_rev();

	if (!VDEC_COMP_CHIP(chip,VDEC_CHIP_M14))	// if M14
	{
		if (chip > VDEC_CHIP_REV(M14, B0))
			bNotSupport = TRUE;
	}
	else
	{
		if (chip > VDEC_CHIP_REV(H14, FF))
			bNotSupport = TRUE;
	}

	//if (bNotSupport)
	//	log_warning("warn! VDEC dual output is not supported\n");

	log_noti("VDISP: Set dual output: src %u onoff %u\n",
			(UINT32)src, (UINT32)bOn);

	if (bOn)
	{
		if (src == DE_IF_DST_DE_IF0)
			ePreDst = DE_IF_SetDualOutput(DE_IF_DST_DE_IF0, DE_IF_DST_DE_IF1);
		else if (src == DE_IF_DST_DE_IF1)
			ePreDst = DE_IF_SetDualOutput(DE_IF_DST_DE_IF1, DE_IF_DST_DE_IF0);
	}
	else
		ePreDst = DE_IF_SetDualOutput(DE_IF_DST_NONE, DE_IF_DST_NONE);

	return (ePreDst!=DE_IF_DST_NONE);
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

inline static void vdisp_start_cmd(VDISP_CH_T *pstVdispCh)
{
	UINT8 u8VdispCh = pstVdispCh->u8VdispCh;

	pstVdispCh->Status.bPlaying = FALSE;

	switch( pstVdispCh->State )
	{
		case VDISP_READY :
			log_noti("[Vdisp%d-%d][DBG] READY --> PLAY\n", u8VdispCh, pstVdispCh->State );
			pstVdispCh->State = VDISP_PLAY;
			break;
		case VDISP_PAUSE :
			log_noti("[Vdisp%d-%d][DBG] PAUSE --> PLAY\n", u8VdispCh, pstVdispCh->State );
			pstVdispCh->State = VDISP_PLAY;
			break;
		case VDISP_PLAY :
			log_noti("[Vdisp%d-%d][Warning] Already Started\n", u8VdispCh, pstVdispCh->State );
			break;
		case VDISP_NULL :
		default :
			log_error("[Vdisp%d-%d][Err] State, Cmd:START, Step? %d\n",
					u8VdispCh, pstVdispCh->State, pstVdispCh->Status.bStepping);
			break;
	}

	return;
}

inline static void vdisp_pause_cmd(VDISP_CH_T *pstVdispCh)
{
	UINT8 u8VdispCh = pstVdispCh->u8VdispCh;

	switch( pstVdispCh->State )
	{
		case VDISP_PLAY :
			log_noti("[Vdisp%d-%d][DBG] PLAY --> PAUSE\n", u8VdispCh, pstVdispCh->State );
			pstVdispCh->State = VDISP_PAUSE;
			break;
		case VDISP_PAUSE :
			log_noti("[Vdisp%d-%d][Warning] Already Paused\n", u8VdispCh, pstVdispCh->State );
			break;
		case VDISP_READY :
			log_noti("[Vdisp%d-%d][DBG] READY --> PAUSE\n", u8VdispCh, pstVdispCh->State );
			break;
		case VDISP_NULL :
		default :
			log_error("[Vdisp%d-%d][Err] State, Cmd:PAUSE\n", u8VdispCh, pstVdispCh->State);
			break;
	}

	return;
}

inline static void vdisp_reset_cmd(VDISP_CH_T *pstVdispCh)
{
	UINT8 u8VdispCh = pstVdispCh->u8VdispCh;

	pstVdispCh->u32PrePts = 0xFFFFFFFF;
	pstVdispCh->u32PreOrgPts = 0xFFFFFFFF;
	switch( pstVdispCh->State )
	{
		case VDISP_READY :
			log_noti("[Vdisp%d-%d][DBG] Reset in READY State\n", u8VdispCh, pstVdispCh->State );
			break;
		case VDISP_PLAY :
			log_noti("[Vdisp%d-%d][DBG] Reset in PLAY State\n", u8VdispCh, pstVdispCh->State );
			PTS_Flush(pstVdispCh->pstSync);
			VDISP_FlushFrames(pstVdispCh);
			break;
		case VDISP_PAUSE :
			log_noti("[Vdisp%d-%d][DBG] Reset in PAUSE State\n", u8VdispCh, pstVdispCh->State );
			PTS_Flush(pstVdispCh->pstSync);
			VDISP_FlushFrames(pstVdispCh);
			break;
		case VDISP_NULL :
		default :
			log_error("[Vdisp%d-%d][Err] State, Cmd:RESET\n", u8VdispCh, pstVdispCh->State );
			break;
	}

	return;
}

inline static void vdisp_flush_cmd(VDISP_CH_T *pstVdispCh)
{
	UINT8 u8VdispCh = pstVdispCh->u8VdispCh;

	pstVdispCh->u32PrePts = 0xFFFFFFFF;
	pstVdispCh->u32PreOrgPts = 0xFFFFFFFF;
	switch( pstVdispCh->State )
	{
		case VDISP_READY :
			log_noti("[Vdisp%d-%d] Flush in Not Play State\n", u8VdispCh, pstVdispCh->State );
			break;
		case VDISP_PLAY :
		case VDISP_PAUSE :
			log_noti("[Vdisp%d-%d] Flush in %s State\n", u8VdispCh, pstVdispCh->State,
					pstVdispCh->State==VDISP_PLAY?"PLAY":"PAUSE");

			if( pstVdispCh->bValidNextFrame && pstVdispCh->bLock4Pair == FALSE )
			{
				log_noti("[Vdisp%d] Clear next frame for flush\n", u8VdispCh);
				pstVdispCh->bValidNextFrame = FALSE;
				VDISP_ClearFrame(pstVdispCh, &pstVdispCh->stNextFrame);

				if (pstVdispCh->bValidNextPair)
				{
					VDISP_ClearFrame(pstVdispCh, &pstVdispCh->stNextPair);
					log_debug("[Vdisp%d] Clear next pair for flush\n", u8VdispCh);
				}
			}

			if( pstVdispCh->stSyncFrame.eState != SYNC_FRM_INVALID )
			{
				log_noti("[Vdisp%d] Clear sync cal frame for flush\n", u8VdispCh);
				VDISP_ClearFrame(pstVdispCh, &pstVdispCh->stSyncFrame.stDqFrame);

				if( pstVdispCh->stSyncFrame.bValidPairFrame )
				{
					VDISP_ClearFrame(pstVdispCh,
							&pstVdispCh->stSyncFrame.stPairFrame);
					log_debug("[Vdisp%d] Clear pair frame for flush\n", u8VdispCh);
				}
				pstVdispCh->stSyncFrame.eState = SYNC_FRM_INVALID;
			}

			PTS_Flush(pstVdispCh->pstSync);
			VDISP_FlushFrames(pstVdispCh);
			break;
		case VDISP_NULL :
		default :
			log_error("[Vdisp%d-%d][Err] State, Cmd:FLUSH\n", u8VdispCh, pstVdispCh->State );
			break;
	}

	return;
}

inline static void vdisp_repeat_cmd(VDISP_CH_T *pstVdispCh, UINT8 u8nTimes)
{
	UINT8 u8VdispCh = pstVdispCh->u8VdispCh;

	log_noti("[Vdisp%d] repeat frame\n", u8VdispCh);

	switch( pstVdispCh->State )
	{
		case VDISP_READY :
		case VDISP_PLAY :
		case VDISP_PAUSE :
			DE_IF_RepeatFrame(pstVdispCh->Config.eDeIfDstCh, u8nTimes);
			break;
		case VDISP_NULL :
		default :
			log_error("[Vdisp%d-%d][Err] State, Cmd:REPEAT\n", u8VdispCh, pstVdispCh->State );
			break;
	}

	return;
}

UINT8 VSync_SetVsyncSrc(UINT8 vsync_ch, UINT8 vsync_src);
inline static void vdisp_set_outport_cmd(VDISP_CH_T *pstVdispCh, E_VDISP_DST_T eTargetDe)
{
	UINT8		u8VdispCh = pstVdispCh->u8VdispCh;
	UINT8		vsync_src_ch;

	log_noti("[Vdisp%d] set out port %d\n", u8VdispCh, eTargetDe);

	switch( pstVdispCh->State )
	{
		case VDISP_READY :
		case VDISP_PLAY :
		case VDISP_PAUSE :
		case VDISP_NULL :
			DE_IF_SetTargetDe(pstVdispCh->Config.eDeIfDstCh, eTargetDe);
			// TODO: Need to change vsync if the channel is for dual stream
			//DE_IF_SelVsync(pstVdispCh->Config.eDeIfDstCh,
			//		pstVdispCh->Config.u8VSyncCh );

			if (eTargetDe == DE_IF_DST_NONE)
				vsync_src_ch = VSYNC_SWSYNC_CH;
			else
				vsync_src_ch = (UINT8)eTargetDe;

			VSync_SetVsyncSrc(u8VdispCh, vsync_src_ch);
			break;
		default :
			log_error("[Vdisp%d-%d][Err] State, Cmd:SET_OUTPORT\n",
					u8VdispCh, pstVdispCh->State );
			break;
	}

	return;
}

inline static void vdisp_set_clksrc_cmd(VDISP_CH_T *pstVdispCh, VDISP_CLK_SRC eClockSrc)
{
	UINT8 u8VdispCh = pstVdispCh->u8VdispCh;
	if (pstVdispCh->eClkSrc != eClockSrc)
	{	
		log_noti("[Vdisp%d]Change Clock Source : %d -> %d\n",
										u8VdispCh, pstVdispCh->eClkSrc, eClockSrc);
		pstVdispCh->eClkSrc = eClockSrc;
	}
	PTS_SetUseGstc(u8VdispCh, eClockSrc == VDISP_CLK_GSTC);
}

void VDISP_RunCmd(UINT8 u8VdispCh)
{
	VDISP_CMD_T	stCmd;
	VDISP_CH_T	*pstVdispCh;
	VDISP_CH_T	*pstPairCh;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] %s\n", u8VdispCh, __FUNCTION__ );
		return;
	}

	pstVdispCh = &gsVdisp[u8VdispCh];
	while( _VDISP_GetCmdQ( u8VdispCh, &stCmd ) == TRUE )
	{
		switch( stCmd.eCmdType )
		{
		case VDISP_CMD_STEP :
		case VDISP_CMD_PLAY :
			if( stCmd.eCmdType == VDISP_CMD_STEP )
				pstVdispCh->Status.bStepping = TRUE;
			else
				pstVdispCh->Status.bStepping = FALSE;

			vdisp_start_cmd(pstVdispCh);
			break;

		case VDISP_CMD_PAUSE :
			vdisp_pause_cmd(pstVdispCh);
			break;

		case VDISP_CMD_RESET :
			vdisp_reset_cmd(pstVdispCh);
			break;
		case VDISP_CMD_FLUSH :
			pstPairCh = pstVdispCh->pstPairCh;
			if( pstVdispCh->pstDualCh != NULL && pstPairCh != NULL )
				vdisp_flush_cmd(pstPairCh);
			vdisp_flush_cmd(pstVdispCh);
			break;
		case VDISP_CMD_REPEAT:
			vdisp_repeat_cmd(pstVdispCh, stCmd.u32Param);
			break;
		case VDISP_CMD_SET_OUT:
			vdisp_set_outport_cmd(pstVdispCh, stCmd.u32Param);
			break;
		case VDISP_CMD_SET_CLKSRC:
			vdisp_set_clksrc_cmd(pstVdispCh, stCmd.u32Param);
			break;
		case VDISP_CMD_NONE :
		default :
			log_error("[Vdisp%d-%d][Err] Cmd: %d, (%d)\n", u8VdispCh,
					pstVdispCh->State, stCmd.eCmdType, __LINE__ );
			break;
		}
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

BOOLEAN _VDISP_IsInterlacedFrame(S_DISPQ_BUF_T *pstDqFrame)
{
	BOOLEAN bRet;

	switch( pstDqFrame->ui32DisplayInfo )
	{
		case DISPQ_SCAN_PROG :
			bRet = FALSE;
			break;
		case DISPQ_SCAN_TFF :
		case DISPQ_SCAN_BFF :
			//if( pstDqFrame->ui32DisplayPeriod == 1 || pstDqFrame->ui32DisplayPeriod >= 4 )
			//	log_error("[Vdisp][Err] Disp Period: %d of Field Pic\n",
			//			pstDqFrame->ui32DisplayPeriod );

			bRet = TRUE;
			break;
		default :
			log_error("[Vdisp][Err] Display Info: %d\n",
					pstDqFrame->ui32DisplayInfo );

			bRet = TRUE;
			break;
	}

	return bRet;
}

BOOLEAN VDISP_CheckFrame(VDISP_CH_T *pstVdispCh, S_DISPQ_BUF_T *pstDqFrame)
{
	SYNC_FRAME_T *pstSyncFrame = &pstVdispCh->stSyncFrame;
	UINT8	u8VdispCh = pstVdispCh->u8VdispCh;
	BOOLEAN bInterlaced;
	UINT32 u32FrameRateLimit = 30;

	pstSyncFrame->stDqFrame.bDispResult = FALSE;

	if (pstDqFrame->ui32DisplayPeriod == 3 && !(pstVdispCh->film_mode_cnt&1))
		pstVdispCh->film_mode_cnt++;
	else if ((pstVdispCh->film_mode_cnt&1) && pstDqFrame->ui32DisplayPeriod==2)
		pstVdispCh->film_mode_cnt++;
	else
		pstVdispCh->film_mode_cnt = 0;
	//In case of film mode, Phase shift is turned off for H13
	if (!pstVdispCh->is_film_mode && pstVdispCh->film_mode_cnt>4)
	{
		pstVdispCh->is_film_mode = TRUE;
#if defined (CHIP_NAME_h13) || (CHIP_NAME_h15)
		VSync_SetMaxShift(pstVdispCh->Config.u8VSyncCh, 0);
#else
		VSync_SetMaxShift(pstVdispCh->Config.u8VSyncCh, 1350);
#endif
		log_noti("vdisp%d: film mode detected\n", u8VdispCh);
	}

	if (PTS_GetMatchMode(pstVdispCh->pstSync) == PTS_MATCH_FREERUN_IGNORE_SYNC)
	{
		// long term rate estimate
		if (pstVdispCh->lt_rate_cnt == 30*10)
		{
			pstVdispCh->lt_rate = pstDqFrame->enq_gstc - pstVdispCh->lt_rate_strat;
			pstVdispCh->lt_rate &= VDEC_PTS_MASK;
			pstVdispCh->lt_rate_cnt=0;

			VSync_SetDcoFrameRate(pstVdispCh->Config.u8VSyncCh,
					30*10*90000, pstVdispCh->lt_rate);

			log_noti("long term rate %d\n",
					VSync_CalFrameDuration(pstVdispCh->lt_rate,30*10*100000));
		}

		if (pstVdispCh->lt_rate_cnt==0)
			pstVdispCh->lt_rate_strat = pstDqFrame->enq_gstc;

		pstVdispCh->lt_rate_cnt++;
	}

	// Check Frame Sanity
	if( pstDqFrame->ui32FrameIdx >= 32 )	// invalid idx
	{
		log_warning("ch:%d wrong idx %d", u8VdispCh, pstDqFrame->ui32FrameIdx );
		return FALSE;
	}

	//if( pstDqFrame->ui32DisplayPeriod )

	PTS_CheckFrame(pstVdispCh->pstSync, pstDqFrame);

	bInterlaced = _VDISP_IsInterlacedFrame( pstDqFrame );

	if( pstDqFrame->ui32PTS != VDEC_UNKNOWN_PTS &&
			pstVdispCh->u32PreOrgPts != pstDqFrame->ui32PTS )
		pstSyncFrame->u32PTS = pstDqFrame->ui32PTS;
	else
	{
		UINT32 u32Dura;
		u32Dura = VRATE_GetDuration(u8VdispCh);
		if(bInterlaced)
			u32Dura /= 2;
		u32Dura *= pstVdispCh->u32PreDispPeriod;
		pstSyncFrame->u32PTS = (pstVdispCh->u32PrePts + u32Dura) & VDEC_PTS_MASK;

		log_debug("ch%d same pts %d -> %d\n", u8VdispCh, pstDqFrame->ui32PTS,
				pstSyncFrame->u32PTS );
	}
#ifdef CHIP_NAME_d14
	if ((ui8Seamless == 0) || (ui8Seamless == 2))
		u32FrameRateLimit = 60;
	else if((ui8Seamless == 1) || (ui8Seamless == 3))
		u32FrameRateLimit = 30;
#else
	if( pstDqFrame->ui32DisplayInfo == DISPQ_SCAN_PROG )
	{
#if !defined(CHIP_NAME_h15) && !defined(CHIP_NAME_m16)
		if( pstDqFrame->ui32PicWidth * pstDqFrame->ui32PicHeight < 1920*1080 ||
				pstDqFrame->ui32LR_Order != DISPQ_3D_FRAME_NONE )
#endif
			u32FrameRateLimit = 60;
	}
#endif

	VRATE_SetRateLimit(u8VdispCh, u32FrameRateLimit);

	VRATE_UpdateCodecRate(u8VdispCh, pstDqFrame->ui32FrameRateRes,
			pstDqFrame->ui32FrameRateDiv, bInterlaced);

	return TRUE;
}

UINT32 VDISP_GetDispQLen(UINT8 u8VdispCh)
{
	return DISP_Q_NumActive(u8VdispCh);
}

wait_queue_head_t *VDISP_GetDispQWait (int ch)
{
	return DISP_Q_GetWait(ch);
}

void VDISP_SendNewFrame(UINT8 u8VdispCh, S_DISPQ_BUF_T *data,
		void (*displayed_callback) (void *arg, S_DISPQ_BUF_T *buffer),
		void *displayed_callback_arg)
{
	VDISP_CH_T	*pstVdispCh;
	UINT32		res, div;
	UINT32		stable;
	unsigned long flags;
	VDEC_MAP_INFO_T *pstMapInfo;

	pstVdispCh = VDISP_GetHandler(u8VdispCh);
	if( pstVdispCh == NULL )
	{
		log_error("[Vdisp%d] %s: Fail to get Vdisp handler\n",
				u8VdispCh, __FUNCTION__);
		return;
	}

	/* read frame private map information */
	pstMapInfo = (VDEC_MAP_INFO_T*)data->pBufferPriv;

	if ( pstMapInfo != NULL )
	{
		data->ui32Tiled_FrameBaseAddr = pstMapInfo->ui32TiledBase;
#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
		data->ui32TopBottomGap = 0;

		if ( pstMapInfo->eBalanceMode == VDEC_MAP_BALANCE_OFF) {
			data->bSmartBalancing = 0;
		} else {
			data->bSmartBalancing = 1;
			data->ui16SmBalanOffsetM0 = pstMapInfo->ui16M0Offset;
			data->ui16SmBalanOffsetM1 = pstMapInfo->ui16M1Offset;

			switch (pstMapInfo->eBalanceMode)
			{
			case VDEC_MAP_BALANCE_1_1: data->ui8SmartBalancingMode = 0; break;
			case VDEC_MAP_BALANCE_1_3: data->ui8SmartBalancingMode = 2; break;
			case VDEC_MAP_BALANCE_3_1: data->ui8SmartBalancingMode = 3; break;
			default: data->bSmartBalancing = 0; break;
			}
		}

		switch (pstMapInfo->eBalanceMode)
		{
		default: // FALL-THROUGH
		case VDEC_MAP_BALANCE_OFF: data->bSmartBalancing = 0; break;
		case VDEC_MAP_BALANCE_1_1: data->ui8SmartBalancingMode = 0; // FALL-THROUGH
		case VDEC_MAP_BALANCE_1_3: data->ui8SmartBalancingMode = 2; // FALL-THROUGH
		case VDEC_MAP_BALANCE_3_1: data->ui8SmartBalancingMode = 3;
			data->bSmartBalancing = 1;
			data->ui16SmBalanOffsetM0 = pstMapInfo->ui16M0Offset;
			data->ui16SmBalanOffsetM1 = pstMapInfo->ui16M1Offset;
			break;
		}
#endif
		switch (pstMapInfo->eFrameMapType)
		{
		default:
		case VDEC_MAP_LINEAR:
			data->ui32DpbMapMode = DISPQ_LINEAR_MAP_DPB;
			break;
		case VDEC_MAP_TILED_MIXED_V:
			data->ui32DpbMapMode = DISPQ_MIXED_MAP_DPB;
			break;
		case VDEC_MAP_TILED_FRAME_V:
			data->ui32DpbMapMode = DISPQ_FRAME_MAP_DPB;
			break;
		case VDEC_MAP_TILED_HEVC:
			data->ui32DpbMapMode = DISPQ_HEVC_MAP_DPB;
			break;
		case VDEC_MAP_TILED_HEVC10:
			data->ui32DpbMapMode = DISPQ_HEVC10_MAP_DPB;
			break;
		case VDEC_MAP_TILED_8X4:
			data->ui32DpbMapMode = DISPQ_G1_MAP;
			break;
		case VDEC_MAP_TILED_4X4:
			data->ui32DpbMapMode = DISPQ_G2_MAP;
			break;
		}
	}
	else
	{
		/* planer type memory */
		data->ui32Tiled_FrameBaseAddr = 0;
#if defined (CHIP_NAME_h15) || defined (CHIP_NAME_m16)
		data->ui32TopBottomGap = 0;
		data->bSmartBalancing = 0;
		data->ui8SmartBalancingMode = 0;
		data->ui16SmBalanOffsetM0 = 0;
		data->ui16SmBalanOffsetM1 = 0;
#endif
		data->ui32DpbMapMode = DISPQ_LINEAR_MAP_DPB;
	}

	spin_lock_irqsave (&pstVdispCh->stLock, flags);

	// preliminary frame rate setting
	if (pstVdispCh->dq_enq_cnt == 0)
	{
		BOOLEAN		bInterlaced;

		bInterlaced = _VDISP_IsInterlacedFrame(data);
		VRATE_UpdateCodecRate(u8VdispCh, data->ui32FrameRateRes,
				data->ui32FrameRateDiv, bInterlaced);
	}

	stable = VRATE_UpdateByDqPTS(u8VdispCh, data->ui32PTS, &res, &div);

	if (stable && pstVdispCh->eClkSrc != VDISP_CLK_GSTC &&
			pstVdispCh->dq_enq_cnt<8)
		VRATE_UseEnqRate(u8VdispCh, TRUE, data->ui32FrameRateRes,
				data->ui32FrameRateDiv);

	if (pstVdispCh->dq_enq_cnt<0xFFFFFFFF)
		pstVdispCh->dq_enq_cnt++;

	spin_unlock_irqrestore (&pstVdispCh->stLock, flags);

	DISP_Q_Push_callback(u8VdispCh, data,
			displayed_callback, displayed_callback_arg);

	return;
}

static BOOLEAN _VDISP_SyncDualCh(VDISP_CH_T* pstVdispCh)
{
	DualCh_T	*pstDualCh;
	UINT32		u32BchMkPts, u32Bound, u32AcceptThr0, u32AcceptThr1;
	S_DISPQ_BUF_T	*pstDqFrame0, *pstDqFrame1, *pstAdjFrame;
	VDISP_CH_T	*pstAdjVdispCh;
	UINT32		u32Duration90K, u32SyncThold, u32EmptyThold;
	SYNC_FRAME_T *pstSyncFrame;
	static UINT32 u32DbgCnt;

	pstDualCh = pstVdispCh->pstDualCh;

	// if it's not the main channel
	if( pstDualCh->pstVdispCh[0] != pstVdispCh )
		return pstDualCh->u8State;

	if( DE_IF_IsNeedNewFrame(pstDualCh->pstVdispCh[1]->Config.eDeIfDstCh) == FALSE )
	{
		log_debug("dual strm: wait another ch's new frame request\n");
		pstDualCh->u8State = FALSE;
		return FALSE;
	}

	if( pstVdispCh->eClkSrc == VDISP_CLK_GSTC)
	{
		u32EmptyThold = VDISP_DUAL_EMPTY_THOLD_FILEPLAY;
		u32SyncThold = VDISP_DUAL_SYNC_THOLD_FILEPLAY;
	}
	else
	{
		u32EmptyThold = VDISP_DUAL_EMPTY_THOLD;
		u32SyncThold = VDISP_DUAL_SYNC_THOLD;
	}

	pstSyncFrame = &pstDualCh->pstVdispCh[0]->stSyncFrame;
	if( pstSyncFrame->eState == SYNC_FRM_INVALID )
	{
		if( _VDISP_Q_Pop(pstDualCh->pstVdispCh[0]->u8VdispCh, &pstSyncFrame->stDqFrame) )
		{
			VDISP_CheckFrame(pstDualCh->pstVdispCh[0], &pstSyncFrame->stDqFrame);

			pstSyncFrame->stDqFrame.bDispResult = FALSE;
			pstSyncFrame->eState = SYNC_FRM_READY;

			pstDualCh->pstVdispCh[0]->Debug.u32nPopFrame++;
		}
	}

	if( pstSyncFrame->eState == SYNC_FRM_READY )
	{
		pstDqFrame0 = &pstSyncFrame->stDqFrame;
		pstDualCh->u8EmptyCnt[0] = 0;
	}
	else
	{
		log_debug("no main ch frame, cnt %d\n", pstDualCh->u8EmptyCnt[0]);

		pstDualCh->u8EmptyCnt[0]++;
		pstSyncFrame = &pstDualCh->pstVdispCh[1]->stSyncFrame;
		if(pstDualCh->u8EmptyCnt[0]>20 && pstSyncFrame->eState != SYNC_FRM_INVALID )
		{
			VDISP_ClearFrame(pstDualCh->pstVdispCh[1], &pstSyncFrame->stDqFrame);
			pstSyncFrame->eState = SYNC_FRM_INVALID;
		}

		pstDualCh->u8State = FALSE;
		return FALSE;
	}

	pstSyncFrame = &pstDualCh->pstVdispCh[1]->stSyncFrame;
	if( pstSyncFrame->eState == SYNC_FRM_INVALID )
	{
		if( _VDISP_Q_Pop(pstDualCh->pstVdispCh[1]->u8VdispCh, &pstSyncFrame->stDqFrame) )
		{
			VDISP_CheckFrame(pstDualCh->pstVdispCh[1], &pstSyncFrame->stDqFrame);

			pstSyncFrame->stDqFrame.bDispResult = FALSE;
			pstSyncFrame->eState = SYNC_FRM_READY;

			pstDualCh->pstVdispCh[1]->Debug.u32nPopFrame++;
		}
	}

	if( pstSyncFrame->eState == SYNC_FRM_READY )
	{
		pstDqFrame1 = &pstSyncFrame->stDqFrame;
		pstDualCh->u8EmptyCnt[1] = 0;
	}
	else
	{
		log_debug("no sub ch frame, cnt %d\n", pstDualCh->u8EmptyCnt[1]);

		pstDualCh->u8EmptyCnt[1]++;
		pstSyncFrame = &pstDualCh->pstVdispCh[0]->stSyncFrame;
		if(pstDualCh->u8EmptyCnt[1]>20 && pstSyncFrame->eState != SYNC_FRM_INVALID )
		{
			VDISP_ClearFrame(pstDualCh->pstVdispCh[0], &pstSyncFrame->stDqFrame);
			pstSyncFrame->eState = SYNC_FRM_INVALID;
		}

		pstDualCh->u8State = FALSE;
		return FALSE;
	}

	u32Bound = (pstDqFrame0->ui32PTS + (VDEC_PTS_MASK>>1)) & VDEC_PTS_MASK;
	if( _VDISP_CheckBelongTo90kTick(pstDqFrame0->ui32PTS, u32Bound,
				pstDqFrame1->ui32PTS) == TRUE )
	{
		u32BchMkPts = pstDualCh->pstVdispCh[1]->stSyncFrame.u32PTS;
		pstAdjVdispCh = pstDualCh->pstVdispCh[0];
		pstAdjFrame = pstDqFrame0;
		pstSyncFrame = &pstDualCh->pstVdispCh[0]->stSyncFrame;
	}
	else
	{
		u32BchMkPts = pstDualCh->pstVdispCh[0]->stSyncFrame.u32PTS;
		pstAdjVdispCh = pstDualCh->pstVdispCh[1];
		pstAdjFrame = pstDqFrame1;
		pstSyncFrame = &pstDualCh->pstVdispCh[1]->stSyncFrame;
	}

	u32Duration90K = VRATE_GetDuration(pstDualCh->pstVdispCh[0]->u8VdispCh);
	u32Duration90K /= 2;

	u32AcceptThr0 = (u32BchMkPts-u32Duration90K) & VDEC_PTS_MASK;
	u32AcceptThr1 = (u32BchMkPts+u32Duration90K) & VDEC_PTS_MASK;
	do
	{
		log_trace("AdjustCh %d, %d %d Dual Diff %d\n",
				pstAdjVdispCh->u8VdispCh, u32BchMkPts,
				pstSyncFrame->u32PTS, pstSyncFrame->u32PTS - u32BchMkPts);

		if( PTS_IsDiscont(pstDualCh->pstVdispCh[0]->pstSync) == TRUE ||
				PTS_IsDiscont(pstDualCh->pstVdispCh[1]->pstSync) == TRUE )
			break;

		// if meet pts with benchmark pts, stop
		if( _VDISP_CheckBelongTo90kTick(u32AcceptThr0, u32AcceptThr1, pstSyncFrame->u32PTS) )
			break;

		// check the diff
		if (((u32BchMkPts - pstSyncFrame->u32PTS) & VDEC_PTS_MASK ) > u32SyncThold)
		{
			log_trace("give up dual sync!! diff %d\n",
					(u32BchMkPts - pstSyncFrame->u32PTS) & VDEC_PTS_MASK );
			break;
		}

		log_noti("[Vdisp%d] frame drop for 3d sync\n", pstAdjVdispCh->u8VdispCh);

		VDISP_ClearFrame(pstAdjVdispCh, pstAdjFrame);
		pstSyncFrame->eState = SYNC_FRM_INVALID;

		if( _VDISP_Q_Pop(pstAdjVdispCh->u8VdispCh, &pstSyncFrame->stDqFrame) )
		{
			VDISP_CheckFrame(pstAdjVdispCh, &pstSyncFrame->stDqFrame);

			pstSyncFrame->stDqFrame.bDispResult = FALSE;
			pstSyncFrame->eState = SYNC_FRM_READY;

			pstAdjVdispCh->Debug.u32nPopFrame++;
		}
		else
		{
			log_debug("no adjust ch(%d) frame\n", pstAdjVdispCh->u8VdispCh);
		}

		if( pstSyncFrame->eState == SYNC_FRM_READY )
			pstAdjFrame = &pstSyncFrame->stDqFrame;
		else
		{
			pstDualCh->u8State = FALSE;
			return FALSE;
		}

	} while(1);

	if( !((u32DbgCnt++) % 0x100) && pstAdjFrame != NULL )
		log_noti("Dual Diff %d\n", pstSyncFrame->u32PTS - u32BchMkPts);

	pstDualCh->u8State = TRUE;
	return TRUE;
}


BOOLEAN VDISP_CheckDualStream(VDISP_CH_T *pstVdispCh)
{
	unsigned long flags;
	BOOLEAN bRet = TRUE;

	// 3D dual stream pre-adjust
	if( pstVdispCh->pstDualCh == NULL )
		return TRUE;

	spin_lock_irqsave(&pstVdispCh->pstDualCh->stLock, flags);

	if( pstVdispCh->pstPairCh == NULL )
	{
		log_debug("dual strm but no pair ch\n");
		bRet = FALSE;
	}
	else if( //VRATE_GetSpeed(pstVdispCh->u8VdispCh) == 1000 &&
			_VDISP_SyncDualCh(pstVdispCh) == FALSE )
	{
		log_debug("dual strm: wait another ch's PTS timing\n");
		bRet = FALSE;
	}

	spin_unlock_irqrestore(&pstVdispCh->pstDualCh->stLock, flags);

	return bRet;
}

static UINT32 _vdisp_calculation_duration(UINT32 ui32FrameRateRes, UINT32 ui32FrameRateDiv)
{
	//use do_div() function
	UINT64 		ui64FrameRateDiv90K_Scaled;

	if( ui32FrameRateRes == 0 )
		return 0;

	ui64FrameRateDiv90K_Scaled = (UINT64)ui32FrameRateDiv * 90000;
	do_div( ui64FrameRateDiv90K_Scaled, ui32FrameRateRes ); //do_div(x,y) -> x = x/y
	log_user3("Calculation frame duration Res:%d, Div:%d, Dur:%d(90k)\n",ui32FrameRateRes,
									ui32FrameRateDiv, ui64FrameRateDiv90K_Scaled);
	return ui64FrameRateDiv90K_Scaled;
}

static void _vdisp_prephaseshift_calculation(VDISP_CH_T *pstVdispCh,
								UINT32 ui32Pts,UINT32 ui32FrameRes,UINT32 ui32FrameDiv,UINT32 ui32Stc)
{
	SINT32	i32PtsStcDiff;
	UINT32	ui32Duration;
	UINT32  ui32BaseTime;
	UINT32  ui32BasePTS;
	UINT8   ui8VdispCh;
	SINT32  i32PTS_Offset;
	UINT32  ui32StreamPTS;
	UINT32  ui32CompenPTS;
	SINT32  i32StcPtsDiff;
	SINT32  i32Cnt;
	SINT32  i32PhaseShift;

	log_user3("Start pre phase shift calculation!! pts:0x%X, framerate:(%d/%d), stc:0x%X\n",
									ui32Pts, ui32FrameRes, ui32FrameDiv, ui32Stc);
	ui8VdispCh = pstVdispCh->u8VdispCh;
	ui32StreamPTS = ui32Pts & VDEC_PTS_MASK;
	ui32Stc &= VDEC_PTS_MASK;

	ui32Duration = _vdisp_calculation_duration(ui32FrameRes,ui32FrameDiv);
	VDISP_GetBaseTime(ui8VdispCh, &ui32BaseTime, &ui32BasePTS);
	VDISP_PTS_GET_OFFSET(ui32StreamPTS, ui32BasePTS, i32PTS_Offset);
	if (pstVdispCh->eClkSrc == VDISP_CLK_GSTC)//File play
	{
		ui32CompenPTS = VDISP_PTS_CHECK_PTS_WRAPAROUND(ui32BaseTime +
									i32PTS_Offset + ui32DisplayOffset_90K);
	}
	else//Live
		ui32CompenPTS = VDISP_PTS_CHECK_PTS_WRAPAROUND(ui32StreamPTS + ui32DisplayOffset_90K);

	log_user3("BaseTime: %d, BasePTS: %d, PTS: %x, Stc: %x, PTS_BasePTS_Diff: %d, dispOffset: %d\n",
									ui32BaseTime, ui32BasePTS, ui32StreamPTS, ui32Stc,
									i32PTS_Offset, ui32DisplayOffset_90K);
	//STC & PTS diff calculation
	if ( (ui32Stc ^ ui32CompenPTS) & (~(VDEC_PTS_MASK>>2)) )
	{
		if (ui32Stc > ui32CompenPTS)
			i32StcPtsDiff = VDISP_PTS_GET_PTS_OFFSET(ui32Stc, ui32CompenPTS);
		else
			i32StcPtsDiff = VDISP_PTS_GET_PTS_OFFSET(ui32CompenPTS, ui32Stc);
	}
	else
	{
		i32StcPtsDiff = ui32Stc - ui32CompenPTS;
		log_user3("Nomal Case stc: %d, CompenPTS: %d, diff: %d\n", ui32Stc
										, ui32CompenPTS, i32StcPtsDiff);
	}
	// In case of interaced stream, we don't calculate pre phase shift
	if( VSync_IntField(ui8VdispCh) && bInterlaced2)
	{
		if (i32StcPtsDiff < 0)
			i32StcPtsDiff += (ui32Duration/2);
		else if (i32StcPtsDiff > 0)
			i32StcPtsDiff -= (ui32Duration/2);
	}

	if ( (i32StcPtsDiff <= ui32Duration/2) || (i32StcPtsDiff >= -(ui32Duration/2)) )
	{// 1. In match boundary
		if ((i32StcPtsDiff >= 270 && i32StcPtsDiff <ui32Duration/2+100)
				|| (i32StcPtsDiff < -270 && i32StcPtsDiff >-(ui32Duration/2)-100))
		{
				pstVdispCh->i32PhaseShift = -i32StcPtsDiff;
		}
		else
				pstVdispCh->i32PhaseShift = 0;
		log_user3("first frame is in match boundary ptsStcDiff:%d, frameduration/2:%d\n"
										,i32StcPtsDiff,ui32Duration/2);
	}
	else
	{// 2. Not in match boundary, calculation phase shift diff value
		log_user3("first frame is not in match boundary[Before devide] i32StcPtsDiff: %d\n"
										,i32StcPtsDiff);
		i32StcPtsDiff = i32StcPtsDiff % (SINT32)ui32Duration;
		log_user3("first frame is not in match boundary[After devide] i32StcPtsDiff: %d\n"
										,i32StcPtsDiff);
		if (VDS_ABS(i32StcPtsDiff) <= ui32Duration/2)
		{
			if ((i32StcPtsDiff >= 270 && i32StcPtsDiff <ui32Duration/2+100)
				|| (i32StcPtsDiff < -270 && i32StcPtsDiff >-(ui32Duration/2)-100))
			{
					pstVdispCh->i32PhaseShift = -i32StcPtsDiff;
					log_user3("first frame is not in match boundary phaseshift: %d\n"
										,pstVdispCh->i32PhaseShift);
			}
		}
		else
		{
			if (i32StcPtsDiff >= 0)
				i32PhaseShift = ui32Duration - i32StcPtsDiff;
			else
				i32PhaseShift = -(ui32Duration + i32StcPtsDiff);

			if ((i32PhaseShift >= 270 && i32PhaseShift <ui32Duration/2+100)
					|| (i32PhaseShift < -270 && i32PhaseShift >-(ui32Duration/2)-100))
			{
				pstVdispCh->i32PhaseShift = i32PhaseShift;
			}
			else
				pstVdispCh->i32PhaseShift = 0;
		}
	}
	log_user3("End pre phase shift calculation!! Phase Shift : %d\n"
									,pstVdispCh->i32PhaseShift);

}

static void _vdisp_vsync_adjust(VDISP_CH_T *pstVdispCh, UINT8 u8VdispCh,
		UINT32 u32Pts)
{
	SINT32		i32Diff;
	SINT32		diff_avrg;
	UINT32		ui32FrameDuration;
	BOOLEAN		bNeedShift = FALSE;
	PTS_MATCH_MODE_E	eMatch;

	i32Diff = PTS_GetDiff(pstVdispCh->pstSync);
	ui32FrameDuration = VRATE_GetDuration(u8VdispCh);
	eMatch = PTS_GetMatchMode(pstVdispCh->pstSync);

	if (eMatch == PTS_MATCH_ENABLE)
	{
#if !defined(CHIP_NAME_d13) && !defined(CHIP_NAME_d14)
		if ((i32Diff >= 270 && i32Diff <ui32FrameDuration/2+100)
				|| (i32Diff < -270 && i32Diff >-(ui32FrameDuration/2)-100))
		{
			if (pstVdispCh->i32VsyncDiff == 0)
			{
				pstVdispCh->i32VsyncDiff = i32Diff;
				bNeedShift = TRUE;
			}
			else if (VDS_ABS(i32Diff-pstVdispCh->i32VsyncDiff)<270)
			{
				bNeedShift = TRUE;
			}
		}
#else
		// for D14
		if ((i32Diff >= ui32FrameDuration/4 && i32Diff <ui32FrameDuration*3/4)
				|| (i32Diff < -(ui32FrameDuration/4) && i32Diff >-(ui32FrameDuration*3/4)) )
		{
			if (pstVdispCh->i32VsyncDiff == 0)
			{
				pstVdispCh->i32VsyncDiff = i32Diff;
				bNeedShift = TRUE;
			}
			else
			{
				SINT32 diff_diff;

				diff_diff = i32Diff-pstVdispCh->i32VsyncDiff;
				if (diff_diff > (SINT32)ui32FrameDuration/2)
						diff_diff -= ui32FrameDuration;
				else if (diff_diff < -(SINT32)(ui32FrameDuration/2))
						diff_diff += ui32FrameDuration;

				if (VDS_ABS(diff_diff)<270)
						bNeedShift = TRUE;
			}
		}
#endif

		if (bNeedShift == TRUE)
		{
			pstVdispCh->u32PhaseErrorCnt++;
			if (pstVdispCh->u32PhaseErrorCnt > 10)
				pstVdispCh->i32PhaseShift = -i32Diff;
		}
		else
		{
			pstVdispCh->i32VsyncDiff = 0;
			pstVdispCh->u32PhaseErrorCnt = 0;
			pstVdispCh->i32PhaseShift = 0;
		}
	}
	else if (eMatch == PTS_MATCH_FREERUN_IGNORE_SYNC)
	{
		pstVdispCh->n_diff_sum++;
		pstVdispCh->diff_sum += i32Diff;

		if (pstVdispCh->n_diff_sum >= 30)
		{
			diff_avrg = pstVdispCh->diff_sum/pstVdispCh->n_diff_sum;
			log_user2("diff avrg : %d\n", diff_avrg);

			if (diff_avrg<3000+ui32FrameDuration/2-600)
			{
				pstVdispCh->i32PhaseShift = 3000+ui32FrameDuration/2-300-diff_avrg;
				log_user2("make vsync slower -> shift : %d\n",
						pstVdispCh->i32PhaseShift);
			}
			else if (diff_avrg>3000+ui32FrameDuration/2)
			{
				pstVdispCh->i32PhaseShift = 3000+ui32FrameDuration/2-300-diff_avrg;
				log_user2("make vsync faster -> shift : %d\n",
						pstVdispCh->i32PhaseShift);
			}

			pstVdispCh->n_diff_sum=0;
			pstVdispCh->diff_sum=0;
			pstVdispCh->diff_var=0;
		}
	}
	else
		pstVdispCh->i32PhaseShift = 0;

}

BOOLEAN VDISP_GetNewFrame(VDISP_CH_T *pstVdispCh, S_DISPQ_BUF_T *pstNextFrame, UINT32 u32STC)
{
	BOOLEAN bRet;
	SINT32		i32speedFrameSeq;
	static UINT32 u32EmptyCnt=0;
	PTS_MATCH_E			eMatch = STC_PTS_NONE;
	BOOLEAN 			bInterlaced, bGotNewFrame = FALSE;
	BOOLEAN 			bValFrame2Clear = FALSE, bValFrame2Clear2 = FALSE;
	S_DISPQ_BUF_T 		stDqFrame2Clear, stDqFrame2Clear2;
	UINT8				u8VdispCh;
	SYNC_FRAME_T		*pstSyncFrame;
	S_DISPQ_BUF_T		*pstNextPair;
	UINT32 ui32FrameRes = 24;
	UINT32 ui32FrameDiv = 1;
	SINT32 i32Speed;

	if( pstVdispCh == NULL )
		return FALSE;

	u8VdispCh = pstVdispCh->u8VdispCh;

	if( pstVdispCh->State != VDISP_PLAY )
	{
		static UINT32 u32DbgCnt=0;
		if( !((u32DbgCnt++)%100) )
			log_info("[Vdisp%d-%d][Err] Current State\n", u8VdispCh, pstVdispCh->State );
		return FALSE;
	}

	if( !VDISP_CheckDualStream(pstVdispCh) )
		return bGotNewFrame;

	pstSyncFrame = &pstVdispCh->stSyncFrame;
	pstNextPair = &pstVdispCh->stNextPair;

re_pop:
	if( pstSyncFrame->eState == SYNC_FRM_INVALID )
	{
		bRet = _VDISP_Q_Pop(u8VdispCh, &pstSyncFrame->stDqFrame);
		if( bRet )
		{
			UINT32 u32QLen = DISP_Q_NumActive(u8VdispCh);

			pstVdispCh->Debug.u32nPopFrame++;
			u32EmptyCnt = 0;

			PTS_NotiCurQlen(pstVdispCh->pstSync, u32QLen+1);

			if( VDISP_CheckFrame(pstVdispCh, &pstSyncFrame->stDqFrame) == FALSE )
				goto re_pop;

			pstSyncFrame->stDqFrame.bDispResult = FALSE;
			pstSyncFrame->eState = SYNC_FRM_READY;

			if (pstSyncFrame->stDqFrame.ui32DisplayInfo == DISPQ_SCAN_PROG &&
					VRATE_GetDuration(u8VdispCh) <= 1502 &&
					VRATE_IsModifiedRate(u8VdispCh) )
			{
				DE_IF_RepeatFrame(pstVdispCh->Config.eDeIfDstCh, 10);
			}

			// Frame Sequential Pairing
			if( pstSyncFrame->stDqFrame.ui32LR_Order == DISPQ_3D_FRAME_LEFT )
			{
				i32speedFrameSeq = VDISP_GetSpeed(u8VdispCh);
				if ( i32speedFrameSeq == 1000 )//normal play
				{
					pstSyncFrame->bValidPairFrame = FALSE;
					pstSyncFrame->eState = SYNC_FRM_WAITPAIR;
					log_debug("Vdisp%d: Wait pair frame for frame sequential\n",
							u8VdispCh);
				}
				else //trick play
				{
					pstSyncFrame->bValidPairFrame = FALSE;
					pstSyncFrame->eState = SYNC_FRM_READY;
					log_debug("Vdisp%d: NO Wait pair frame for trick play of frame sequential\n",
							u8VdispCh);
				}
			}
		}
		else
		{
			PTS_NotiCurQlen(pstVdispCh->pstSync, 0);
			if( !((u32EmptyCnt++)%60) )
				log_noti("[Vdisp%d] DispQ empty\n", u8VdispCh);

			if( pstVdispCh->Status.fpStatus )
				pstVdispCh->Status.fpStatus(u8VdispCh, VDISP_ST_EMPTY, 0);
		}
	}
	i32Speed = VDISP_GetSpeed(u8VdispCh);
	// Pre_phase shift calculation of first frame after Setbase time or Live TV
	if ((i32Speed == 1000) && (bFirstFrameAfterSetBase[u8VdispCh] == TRUE || bLivePrephaseShift[u8VdispCh] == TRUE))
	{
		if (bLivePrephaseShift[u8VdispCh])
			ui8PrePhaseshiftCntLive[u8VdispCh]++;
		else if (bFirstFrameAfterSetBase[u8VdispCh])
			ui8PrePhaseshiftCnt[u8VdispCh]++;

		if (ui8PrePhaseshiftCnt[u8VdispCh] == 1 || ui8PrePhaseshiftCntLive[u8VdispCh] == 1)
		{
			//Get Decided frame rate
			VRATE_GetResDiv(u8VdispCh, &ui32FrameRes,&ui32FrameDiv);
			bInterlaced2 = _VDISP_IsInterlacedFrame(&pstSyncFrame->stDqFrame );
			//Pre-phase-shift
			_vdisp_prephaseshift_calculation(pstVdispCh,pstSyncFrame->u32PTS
											,ui32FrameRes,ui32FrameDiv, u32STC);
			//vsync interval update according to frame rate
			VSync_SetNextVsyncField(pstVdispCh->Config.u8VSyncCh
											,ui32FrameRes,ui32FrameDiv,FALSE);
			VSync_SetPhaseShift(pstVdispCh->Config.u8VSyncCh,pstVdispCh->i32PhaseShift);
			pstVdispCh->i32PhaseShift = 0;
		}
		if (bLivePrephaseShift[u8VdispCh] && ui8PrePhaseshiftCntLive[u8VdispCh] == 3)
		{
			bLiveChannelOpen[u8VdispCh] = FALSE;
			bLivePrephaseShift[u8VdispCh] = FALSE;
			VSync_SetPrePhaseShift(u8VdispCh,FALSE);
		}
	}

	if( pstSyncFrame->eState == SYNC_FRM_WAITPAIR )
	{
		if (!pstSyncFrame->bValidPairFrame)
		{
			bRet = _VDISP_Q_Pop(u8VdispCh, &pstSyncFrame->stPairFrame);
			if( bRet )
			{
				UINT32 res, div;

				pstSyncFrame->stPairFrame.bDispResult = FALSE;

				VRATE_UpdateByPTS(u8VdispCh, pstSyncFrame->stPairFrame.ui32PTS,
						&res, &div);

				//if( RIGHT);
				pstSyncFrame->bValidPairFrame = TRUE;
				log_debug("Vdisp%d: Get new pair\n", u8VdispCh);
			}
			else
				log_warning("Vdisp%d: fail to get pair frame\n", u8VdispCh);
		}

		if (pstSyncFrame->bValidPairFrame)
		{
			pstSyncFrame->eState = SYNC_FRM_READY;
		}
	}

	if( pstSyncFrame->eState == SYNC_FRM_READY )
	{
		eMatch = PTS_MatchCheck(pstVdispCh->pstSync, pstSyncFrame->u32PTS, u32STC,
				pstVdispCh->u32PrePts);

		if( pstVdispCh->Status.fpStatus )
			pstVdispCh->Status.fpStatus(u8VdispCh, VDISP_ST_SYNC_DIFF,
					(UINT32)(PTS_GetDiff(pstVdispCh->pstSync)/90));

		if( eMatch == STC_PTS_MATCHED || eMatch == STC_PTS_FREERUN )
		{
			// Display matched frame
			pstSyncFrame->eState = SYNC_FRM_INVALID;
			pstVdispCh->u32PrePts = pstSyncFrame->u32PTS;
			pstVdispCh->u32PreOrgPts = pstSyncFrame->stDqFrame.ui32PTS;
			pstVdispCh->u32PreDispPeriod = pstSyncFrame->stDqFrame.ui32DisplayPeriod;

			*pstNextFrame = pstSyncFrame->stDqFrame;
			pstNextFrame->ui32PTS = pstSyncFrame->u32PTS;
			bGotNewFrame = TRUE;

			if (pstSyncFrame->bValidPairFrame)
			{
				pstSyncFrame->bValidPairFrame = FALSE;
				*pstNextPair = pstSyncFrame->stPairFrame;
				pstVdispCh->bValidNextPair = TRUE;
			}
		}
		else if( eMatch == STC_PTS_DISCARD )
		{
			if( bValFrame2Clear )
			{
				// discard DqFrame2clear previously set
				log_noti("[Vdisp%d] Discard -> clear frame #%d\n",
						u8VdispCh, stDqFrame2Clear.ui32FrameIdx);
				VDISP_ClearFrame(pstVdispCh, &stDqFrame2Clear);
			}
			if( bValFrame2Clear2 )
			{
				// discard DqFrame2clear previously set
				log_noti("[Vdisp%d] Discard -> clear frame2 #%d\n",
						u8VdispCh, stDqFrame2Clear2.ui32FrameIdx);
				VDISP_ClearFrame(pstVdispCh, &stDqFrame2Clear2);
				bValFrame2Clear2 = FALSE;
			}

			// Don't discard right now...
			pstSyncFrame->eState = SYNC_FRM_INVALID;
			pstVdispCh->u32PrePts = pstSyncFrame->u32PTS;
			pstVdispCh->u32PreOrgPts = pstSyncFrame->stDqFrame.ui32PTS;
			pstVdispCh->u32PreDispPeriod = pstSyncFrame->stDqFrame.ui32DisplayPeriod;

			stDqFrame2Clear = pstSyncFrame->stDqFrame;
			stDqFrame2Clear.ui32PTS = pstSyncFrame->u32PTS;
			bValFrame2Clear = TRUE;

			if (pstSyncFrame->bValidPairFrame)
			{
				pstSyncFrame->bValidPairFrame = FALSE;
				stDqFrame2Clear2 = pstSyncFrame->stPairFrame;
				stDqFrame2Clear2.ui32PTS = pstSyncFrame->u32PTS;
				bValFrame2Clear2 = TRUE;
			}

			goto re_pop;
		}
	}

	// Clear frame if need
	if( bValFrame2Clear )
	{
		if( bGotNewFrame )
		{
			// discard
			log_noti("[Vdisp%d] Discard -> clear frame #%d\n",
					u8VdispCh, stDqFrame2Clear.ui32FrameIdx);
			VDISP_ClearFrame(pstVdispCh, &stDqFrame2Clear);
			if( bValFrame2Clear2 )
			{
				log_noti("[Vdisp%d] Discard -> clear frame2 #%d\n",
						u8VdispCh, stDqFrame2Clear2.ui32FrameIdx);
				VDISP_ClearFrame(pstVdispCh, &stDqFrame2Clear2);
				bValFrame2Clear2 = FALSE;
			}
		}
		else
		{
			// revival pre-discarded frame
			bGotNewFrame = TRUE;
			*pstNextFrame = stDqFrame2Clear;
			if( bValFrame2Clear2 )
			{
				*pstNextPair = stDqFrame2Clear2;
				pstVdispCh->bValidNextPair = TRUE;
			}

			bValFrame2Clear = FALSE;
			bValFrame2Clear2 = FALSE;
		}
	}

	// when new frame turned to be activated
	if( bGotNewFrame )
	{
		log_user2("[Vdisp%d] Pop Idx %2d  bValClr %d  Match %d\n", u8VdispCh,
				pstNextFrame->ui32FrameIdx, bValFrame2Clear, eMatch);

		if( pstVdispCh->Status.bStepping == TRUE )
		{
			pstVdispCh->Status.bStepping = FALSE;
			pstVdispCh->State = VDISP_PAUSE;
		}

		bInterlaced = _VDISP_IsInterlacedFrame( pstNextFrame );

		VRATE_GetResDiv(u8VdispCh, &(pstNextFrame->ui32FrameRateRes),
				&(pstNextFrame->ui32FrameRateDiv));

//#if !defined(CHIP_NAME_d13) && !defined(CHIP_NAME_d14)
		_vdisp_vsync_adjust(pstVdispCh, u8VdispCh, pstSyncFrame->u32PTS);
//#endif

		VSync_SetNextVsyncField(pstVdispCh->Config.u8VSyncCh,
				pstNextFrame->ui32FrameRateRes, pstNextFrame->ui32FrameRateDiv,
				bInterlaced);
	}

	return bGotNewFrame;
}
#if defined (CHIP_NAME_d14)
SINT32 _VDISP_SetSeamlessDisplayOffset(UINT8 u8VdispCh, S_DISPQ_BUF_T *pstDqFrame)
{
	UINT32				u32FrameRate;
	UINT32 				u32Residual;
	SINT32				DisplayOffset;

	if (pstDqFrame->ui32FrameRateDiv == 0)
		return -1;

	u32FrameRate = pstDqFrame->ui32FrameRateRes/pstDqFrame->ui32FrameRateDiv;
	u32FrameRate *= 1000;
	u32Residual = pstDqFrame->ui32FrameRateRes%pstDqFrame->ui32FrameRateDiv;
	u32FrameRate += (u32Residual*1000)/pstDqFrame->ui32FrameRateDiv;

	if( u32FrameRate <= 23976 )
	{
		u32FrameRate = 24;			// 23.976 Hz
	}
	else if( u32FrameRate <= 24000 )
	{
		u32FrameRate = 24;			// 24 Hz
	}
	else if( u32FrameRate <= 25000 )
	{
		u32FrameRate = 25;	// 25 Hz
	}
	else if( u32FrameRate <= 29970 )
	{
		u32FrameRate = 30;	// 29.97 Hz
	}
	else if( u32FrameRate <= 30000 )
	{
		u32FrameRate = 30;	// 30 Hz
	}
	else if( u32FrameRate <= 50000 )
	{
		u32FrameRate = 50;	// 50 Hz
	}
	else if( u32FrameRate <= 59940 )
	{
		u32FrameRate = 60;	// 59.94 Hz
	}
	else if( u32FrameRate <= 60000 )
	{
		u32FrameRate = 60;	// 60 Hz
	}
	else
	{
		u32FrameRate = 30;
	}

	DisplayOffset = PTS_GetDisplayOffset(gsVdisp[u8VdispCh].pstSync);
	log_noti("[Vdisp%d] Before Extra delay for UHD seamless: %d fr:%d\n",
			u8VdispCh, DisplayOffset,u32FrameRate);
/*
	switch(u32FrameRate)
	{
		case 60:
			DisplayOffset += 46;
			break;
		case 50:
			DisplayOffset += 54;
			break;
		case 30:
			DisplayOffset += 79;
			break;
		case 25:
			DisplayOffset += 94;
			break;
		case 24:
			DisplayOffset += 98;
			break;
		default:
			DisplayOffset += 0;
			break;
	}
*/
	uhd_ex_delay = VDISP_UHD_FRM_DELAY * 1000 / u32FrameRate;
	DisplayOffset += uhd_ex_delay;

	PTS_SetDisplayOffset(gsVdisp[u8VdispCh].pstSync, DisplayOffset);
	return DisplayOffset;
}
#endif

//extern void DIF_SetOutChannelMode(UINT32 ch);
BOOLEAN VDISP_SetNewFrame(VDISP_CH_T *pstVdispCh, S_DISPQ_BUF_T *pstDqFrame,
		UINT32 u32SyncField)
{
	UINT8	u8VdispCh;
	BOOLEAN bSet;
#if defined (CHIP_NAME_d14)
	SINT32				DisplayOffset;
#endif

	if( pstVdispCh->State == VDISP_PLAY || pstVdispCh->State == VDISP_PAUSE )
	{
	}
	else
		return FALSE;

	u8VdispCh = pstVdispCh->u8VdispCh;

	if(pstVdispCh->Status.bFreeze)
	{
		VDISP_ClearFrame(pstVdispCh, pstDqFrame);
		return TRUE;
	}

#if defined (CHIP_NAME_d14)
	if((ui8Seamless == 1) && pstVdispCh->Config.eDeIfDstCh==1 && delayoffsetflag == 0)
	{
		ui8FrameCnt++;
		if(ui8FrameCnt == 3)
		{
			ui8FrameCnt = 0;
			delayoffsetflag = 1;
			DisplayOffset = _VDISP_SetSeamlessDisplayOffset(u8VdispCh, pstDqFrame);
			if(DisplayOffset >= 0)
			{
				log_noti("[Vdisp%d] After Extra delay for UHD seamless: %d\n",
						u8VdispCh, DisplayOffset);
			}
			else
			{
				log_error("[Vdisp%d][Err] Fail to set Extra delay offset for UHD seamless %s\n",
						u8VdispCh,__FUNCTION__ );
			}
		}
	}
#endif


	log_user3("Frame2Disp STC %d Vdisp%d Idx %2d  Rate %d/%d DTS %5u PTS %5u Dispi %d\n",
			VDISP_GetSTC(u8VdispCh),
			u8VdispCh, pstDqFrame->ui32FrameIdx,
			pstDqFrame->ui32FrameRateRes, pstDqFrame->ui32FrameRateDiv,
			pstDqFrame->ui32DTS, pstDqFrame->ui32PTS,
			pstDqFrame->ui32DisplayInfo);

	// use sequential 3D L/R option only for normal play speed
	if( VRATE_GetSpeed(u8VdispCh) != 1000 &&
			pstDqFrame->ui32LR_Order != DISPQ_3D_FRAME_NONE )
		pstDqFrame->ui32LR_Order |= (0x1 << 1);

#ifdef CHIP_NAME_d14
	if (ui8Seamless == 1)
	{
		BOOLEAN		is_uhd_frm;
		UINT32		uhd_out_mode;

		if (pstDqFrame->ui32PicWidth*pstDqFrame->ui32PicHeight > 1920*1088)
		{
			uhd_out_mode = 2;
			is_uhd_frm = TRUE;
		}
		else
		{
			uhd_out_mode = 1;
			is_uhd_frm = FALSE;
		}

		if (pstVdispCh->Config.eDeIfDstCh==0 &&
				pre_uhd_out_mode != uhd_out_mode)
		{
			if (uhd_chg_cnt >= VDISP_UHD_FRM_DELAY)
			{
				// set UHD info frame
				DIF_SetOutChannelMode(uhd_out_mode);

				pre_uhd_out_mode = uhd_out_mode;
				uhd_chg_cnt = 0;
			}
			else
				uhd_chg_cnt++;
		}

		if ( (pstVdispCh->Config.eDeIfDstCh==0 && is_uhd_frm ) ||
				(pstVdispCh->Config.eDeIfDstCh==1 && !is_uhd_frm))
		{
			VDISP_ClearFrame(pstVdispCh, pstDqFrame);
			return TRUE;
		}
	}
#endif

	bSet = DE_IF_SetNewFrame(pstVdispCh->Config.eDeIfDstCh, pstDqFrame, u32SyncField);
	if( bSet )
	{
		pstDqFrame->bDispResult = TRUE;
		pstVdispCh->Debug.u32nDispFrame++;
		pstVdispCh->stCurFrame = *pstDqFrame;
		pstVdispCh->bValidCurFrame = TRUE;

		if (gDbgVsyncShift)
		{
			pstVdispCh->i32PhaseShift = gDbgVsyncShift;
			gDbgVsyncShift = 0;
		}

		if (pstVdispCh->i32PhaseShift)
		{
#if defined (CHIP_NAME_d14)
			PTS_SetPhaseShift(pstVdispCh->pstSync, -(pstVdispCh->i32PhaseShift));
#else
			VSync_SetPhaseShift(pstVdispCh->Config.u8VSyncCh, pstVdispCh->i32PhaseShift);
#endif
			pstVdispCh->u32PhaseErrorCnt = 0;
			pstVdispCh->i32PhaseShift = 0;
		}
	}

	return bSet;
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
void VDISP_DE_IF_UpdateDisplay(UINT8 u8VdispCh, DE_IF_DST_E eDeIfDstCh,
		UINT32 u32SyncField)
{
	VDISP_CH_T		*pstVdispCh;
	pstVdispCh = VDISP_GetHandler(u8VdispCh);

	if( pstVdispCh == NULL )
	{
		log_error("[Vdisp%d][Err] Fail get handler %s\n",
				u8VdispCh,__FUNCTION__ );
		return;
	}

	switch( pstVdispCh->State )
	{
	case VDISP_PAUSE :
	case VDISP_PLAY :
	  	DE_IF_UpdateDisplay(eDeIfDstCh, u32SyncField);
		break;
	case VDISP_NULL :
	case VDISP_READY :
	default :
		if( ((pstVdispCh->Debug.u32LogTick_UpdateBuf++) % 23) == 0 )
			log_noti("[Vdisp%d-%d][Warning] Current State, %s(%d)\n",
					pstVdispCh->u8VdispCh, pstVdispCh->State, __FUNCTION__, __LINE__ );
		break;
	}

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

void vdisp_debug_print_status(VDISP_CH_T *pstVdispCh, UINT32 u32Stc)
{
	static UINT32 u32PreStc[VDISP_NUM_OF_CH] = {0,};

	if( u32Stc-u32PreStc[pstVdispCh->u8VdispCh] >= 90000*2 )
	{
		UINT32 u32ElapseTime;
		static UINT8 i;

		u32ElapseTime = u32Stc - u32PreStc[pstVdispCh->u8VdispCh];

		if( i == 0 && u32ElapseTime )
		{
			log_noti("[Vdisp%d] Rate100:  Disp %d  Pop %d\n",
					pstVdispCh->u8VdispCh,
					pstVdispCh->Debug.u32nDispFrame*100*90000/u32ElapseTime,
					pstVdispCh->Debug.u32nPopFrame*100*90000/u32ElapseTime);
		}

		if( i == 1 )
		{
			//log_noti("[Vdisp%d] Match,", pstVdispCh->u8VdispCh);
		}


		if( (++i)==2 )
			i=0;

		u32PreStc[pstVdispCh->u8VdispCh] = u32Stc;
		pstVdispCh->Debug.u32nDispFrame = 0;
		pstVdispCh->Debug.u32nPopFrame = 0;
	}
}


#ifdef VDISP_DBG_LIPSYNC
static void vdisp_debug_search_frame(VDISP_CH_T *pstVdispCh)
{
	#define VDISP_DBG_N_FRAME 3
	UINT32 i;
	UINT32 u32Pts[VDISP_DBG_N_FRAME] =
	{
//0x709C2	//0x123234	// DivX A31 8 Hz
//0x6FAFE	//0x123234	// DivX A32 12 Hz
//0x98562	// DivX A08 15 Hz
//0x70C92	//0x1237D4	// DivX A34 15 Hz
//0x6FAFE	//0x123234	// DivX A12 24 Hz
//0x972C0	// DivX A22 25 Hz
//0x6F9F0	// DivX A50 25 Hz
//0x6F6C6	// DivX A27 29.97 Hz
//0x98562 //0x14BC98	// DivX A11 30 Hz
//0x14BC98	// DivX A20 50 Hz
//0x14B5EA	// DivX A21 59.94 Hz
//0x14B69E	// DivX A10 60 Hz
		//0x33E92C4,
		//0x3457E11,
		//0x34C695E

		//1080
		0xBD17ADD-3003,
		0xBD8662A,
		0xBDF5177


	};

	for(i=0; i<VDISP_DBG_N_FRAME; i++)
	{
		if(	pstVdispCh->stNextFrame.u32PTS > u32Pts[i] - 100 &&
				pstVdispCh->stNextFrame.u32PTS < u32Pts[i] + 100 )
		{
			UINT32 u32Stc = VDISP_GetSTC(pstVdispCh->u8VdispCh);
			u32VdecSendTime = u32Stc;
			log_noti("%d %d\n", u32Stc, pstVdispCh->stNextFrame.u32PTS);
			printk("%d %d Idx %d\n", u32Stc, pstVdispCh->stNextFrame.u32PTS,
					pstVdispCh->stNextFrame.u32FrameIdx);

			while(1);
			break;
		}
	}

	return;
}
#else
#define vdisp_debug_search_frame(pstVdispCh)
#endif

void VDISP_ClearFrame(VDISP_CH_T *pstVdispCh, DISPQ_FRM_T *buffer)
{
	if (pstVdispCh->State == VDISP_NULL)
	{
		log_warning("VDISP %d: Clear fail, state is NULL\n",
				pstVdispCh->u8VdispCh);
		return;
	}

	VRATE_GetResDiv(pstVdispCh->u8VdispCh, &(buffer->ui32FrameRateRes),
				&(buffer->ui32FrameRateDiv));

	log_user2("Vdisp:%d Push ClrQ %d FrameRate(%d/%d)\n", pstVdispCh->u8VdispCh,
			buffer->ui32FrameIdx, buffer->ui32FrameRateRes, buffer->ui32FrameRateDiv);

	if (buffer->displayed_callback)
		buffer->displayed_callback (buffer->displayed_callback_arg, buffer);

	return;
}

void VDISP_FlushFrames(VDISP_CH_T *pstVdispCh)
{
	S_DISPQ_BUF_T sPopDisBuf;
	UINT8 u8VdispCh = pstVdispCh->u8VdispCh;

	log_noti("[VDISP%d] Flush: Occupancy: %d\n", u8VdispCh,
			DISP_Q_NumActive(u8VdispCh) );

	while (_VDISP_Q_Pop(u8VdispCh, &sPopDisBuf ) == TRUE)
	{
		sPopDisBuf.ui32FrameIdx = sPopDisBuf.ui32DisplayPeriod >> 3;
		sPopDisBuf.bDispResult = FALSE;
		VDISP_ClearFrame(pstVdispCh, &sPopDisBuf);
	}
}

void vdisp_disp_done(void *priv, S_DISPQ_BUF_T *pstClrFrm)
{
	VDISP_CH_T		*pstVdispCh = (VDISP_CH_T*)priv;
	DISPQ_FRM_T		stFrm;

	// becareful race condition with close syscall

	if (pstClrFrm == NULL)
	{
		pstClrFrm = &stFrm;
		//bRes = DEIF_GetDisplayedInfo(pstVdispCh->Config.eDeIfDstCh, &stFrm);
	}

	if (pstClrFrm != NULL)
		VDISP_ClearFrame(pstVdispCh, pstClrFrm);
}

void VDISP_VVsyncCallback(UINT8 u8VsyncCh, UINT8 u8nVdisp, UINT8 au8VdispCh[], UINT8 u8FieldInfo)
{
	UINT32			i, u32Stc = 0;
	VDISP_CH_T		*pstVdispCh;
	VDISP_CH_T		*pstPairCh;
	BOOLEAN			bSet, bReqNewFrame;
	unsigned long flags, pair_flags;

	for( i = 0; i < u8nVdisp; i++ )
	{
		pstVdispCh = VDISP_GetHandler(au8VdispCh[i]);
		if( pstVdispCh == NULL )
		{
			log_error("[Vdisp%d][Err] Fail get handler\n", au8VdispCh[i]);
			continue;
		}

		spin_lock_irqsave (&pstVdispCh->stLock, flags);

		VDISP_RunCmd(pstVdispCh->u8VdispCh);

		pstPairCh = pstVdispCh->pstPairCh;
		if (pstVdispCh->pstDualCh != NULL && pstPairCh != NULL)
		{
			spin_lock_irqsave (&pstPairCh->stLock, pair_flags);
			VDISP_RunCmd(pstPairCh->u8VdispCh);
			spin_unlock_irqrestore (&pstPairCh->stLock, pair_flags);
		}

		bReqNewFrame = DE_IF_IsNeedNewFrame(pstVdispCh->Config.eDeIfDstCh);

		if (i==0)
			u32Stc = VDISP_GetSTC(pstVdispCh->u8VdispCh);

		PTS_WraparoundCheck(pstVdispCh->u8VdispCh, u32Stc);

		if( bReqNewFrame == TRUE )
		{
			if( !pstVdispCh->bValidNextFrame )
				pstVdispCh->bValidNextFrame = VDISP_GetNewFrame(pstVdispCh,
						&pstVdispCh->stNextFrame, u32Stc);

			if( pstVdispCh->bValidNextFrame )
			{
				bSet = VDISP_SetNewFrame(pstVdispCh, &pstVdispCh->stNextFrame, u8FieldInfo);

				if( bSet )
				{
					pstVdispCh->bValidNextFrame = FALSE;

					if (pstVdispCh->bValidNextPair)
					{
						SYNC_FRAME_T *pstSyncFrame = &pstVdispCh->stSyncFrame;

						pstVdispCh->bLock4Pair = TRUE;

						pstVdispCh->stNextFrame = pstVdispCh->stNextPair;
						pstVdispCh->bValidNextPair = FALSE;
						pstVdispCh->bValidNextFrame = TRUE;

						VDISP_CheckFrame(pstVdispCh, &pstVdispCh->stNextFrame);
						pstSyncFrame->stDqFrame.bDispResult = FALSE;

						log_debug("Vdisp%d : Pair Lock\n", pstVdispCh->u8VdispCh);
					}
					else if( pstVdispCh->bLock4Pair == TRUE )
					{
						pstVdispCh->bLock4Pair = FALSE;
						log_debug("Vdisp%d : Pair Unlock\n", pstVdispCh->u8VdispCh);
					}

					vdisp_debug_search_frame(pstVdispCh);
				}
			}
		}
		//For BBC & Amazon VSI info setting
		if(ui8Seamless == 2/* || ui8Seamless == 3*/)
		{
			if(i32VsiInfoTimming == 1 || i32VsiInfoTimming == 2)
			{
				DE_VDO_HAL_SetVsiInfo(pstVdispCh->u8VdispCh,ui32PicW, ui32PicH, ui32HOff, ui32VOff);
			}
			else
				log_user1("No VSI information setting\n");
		}
		
		vdisp_debug_print_status(pstVdispCh, u32Stc);

		spin_unlock_irqrestore (&pstVdispCh->stLock, flags);
	}
}

void VDISP_VsyncCallback(UINT8 u8VsyncCh,
		UINT8 u8nVdisp,
		UINT8 au8VdispCh[],
		UINT8 u8FieldInfo)
{
	UINT32			i;
	VDISP_CH_T		*pstVdispCh;
	unsigned long flags;

	for( i = 0; i < u8nVdisp; i++ )
	{
		pstVdispCh = VDISP_GetHandler(au8VdispCh[i]);
		if( pstVdispCh == NULL )
		{
			log_error("[Vdisp%d][Err] Fail get handler\n", au8VdispCh[i]);
			continue;
		}

		spin_lock_irqsave (&pstVdispCh->stLock, flags);

		VDISP_DE_IF_UpdateDisplay(pstVdispCh->u8VdispCh,
				pstVdispCh->Config.eDeIfDstCh,
				(UINT32)u8FieldInfo);

		spin_unlock_irqrestore (&pstVdispCh->stLock, flags);
	}
}

#if defined(CHIP_NAME_d14)
void VDISP_SeamlessModeNoti(UINT8 ui8SeamlessMode)
{
#if defined(AMAZON_TEST)
	ui8Seamless = 3;

	log_noti("[Vdisp] Set seamless mode %u\n", ui8Seamless);
	DE_IF_SeamlessModeNoti(ui8Seamless);
#else
	ui8Seamless = ui8SeamlessMode;

	log_noti("[Vdisp] Set seamless mode %u\n", ui8SeamlessMode);
	DE_IF_SeamlessModeNoti(ui8SeamlessMode);
#endif
}
#endif

BOOLEAN VDISP_SetClkSrc(UINT8 u8VdispCh, VDISP_CLK_SRC eClockSrc)
{
	VDISP_CMD_T stCmd;

	if( u8VdispCh >= VDISP_NUM_OF_CH )
	{
		log_error("[Vdisp%d][Err] %s\n", u8VdispCh, __FUNCTION__ );
		return FALSE;
	}
	
	if( eClockSrc >= VDISP_CLK_MAX || eClockSrc == VDISP_CLK_NONE )
	{
		log_error("[Vdisp%d][Err] Wrong Clock Src %d\n", u8VdispCh, eClockSrc);
		return FALSE;
	}
	stCmd.eCmdType = VDISP_CMD_SET_CLKSRC;
	stCmd.u32Param = eClockSrc;
	_VDISP_PutCmdQ( u8VdispCh, &stCmd );
}

void _dq_print_entry(struct seq_file *m, S_DISPQ_BUF_T *pstBuf);
void _vdisp_print_channel(struct seq_file *m, VDISP_CH_T *pstVdispCh)
{
#define VDS_DPRINT(frm, args...)		seq_printf(m, frm, ##args )
	VDS_DPRINT("Vdisp #%d, [%X]\n", pstVdispCh->u8VdispCh, (UINT32)pstVdispCh);

	VDS_DPRINT(" State            : %u\n", (UINT32)pstVdispCh->State);
	VDS_DPRINT(" u8VdispCh        : %u\n", (UINT32)pstVdispCh->u8VdispCh);
	VDS_DPRINT(" eClkSrc          : %u\n", (UINT32)pstVdispCh->eClkSrc);
	VDS_DPRINT(" u32PrePts        : %u\n", (UINT32)pstVdispCh->u32PrePts);
	VDS_DPRINT(" u32PreOrgPts     : %u\n", (UINT32)pstVdispCh->u32PreOrgPts);
	VDS_DPRINT(" u32PreDispPeriod : %u\n", (UINT32)pstVdispCh->u32PreDispPeriod);
	VDS_DPRINT(" stNextFrame      : %u\n", (UINT32)&pstVdispCh->stNextFrame);
	VDS_DPRINT(" stCurFrame       : %u\n", (UINT32)&pstVdispCh->stCurFrame);
	VDS_DPRINT("   -->\n");
	_dq_print_entry(m, &pstVdispCh->stCurFrame);

	VDS_DPRINT(" bValidCurFrame   : %u\n", (UINT32)pstVdispCh->bValidCurFrame);
	VDS_DPRINT(" bValidNextFrame  : %u\n", (UINT32)pstVdispCh->bValidNextFrame);
	VDS_DPRINT(" stSyncFrame      : %u\n", (UINT32)&pstVdispCh->stSyncFrame);
//	VDS_DPRINT(" bForcedPush      : %u\n", (UINT32)pstVdispCh->bForcedPush);
	VDS_DPRINT(" bIsDualCh        : %u\n", (UINT32)pstVdispCh->bIsDualCh);
	VDS_DPRINT(" pstPairCh        : %u\n", (UINT32)pstVdispCh->pstPairCh);
	VDS_DPRINT(" pstDualCh        : %u\n", (UINT32)pstVdispCh->pstDualCh);
	VDS_DPRINT(" bLock4Pair       : %u\n", (UINT32)pstVdispCh->bLock4Pair);
	VDS_DPRINT(" u32PhaseErrorCnt : %u\n", (UINT32)pstVdispCh->u32PhaseErrorCnt);
	VDS_DPRINT(" i32PhaseShift    : %d\n", (UINT32)pstVdispCh->i32PhaseShift);
	VDS_DPRINT(" film mode        : %d\n", (UINT32)pstVdispCh->is_film_mode);
	VDS_DPRINT(" pstSync          : %u\n", (UINT32)pstVdispCh->pstSync);

	VDS_DPRINT(" eDeIfDstCh       : %u\n", (UINT32)pstVdispCh->Config.eDeIfDstCh);
	VDS_DPRINT(" u8VSyncCh        : %u\n", (UINT32)pstVdispCh->Config.u8VSyncCh);

	VDS_DPRINT(" bStepping        : %u\n", (UINT32)pstVdispCh->Status.bStepping);
	VDS_DPRINT(" bPlaying         : %u\n", (UINT32)pstVdispCh->Status.bPlaying);
	VDS_DPRINT(" bClosing         : %u\n", (UINT32)pstVdispCh->Status.bClosing);
	VDS_DPRINT(" bFreeze          : %u\n", (UINT32)pstVdispCh->Status.bFreeze);
	VDS_DPRINT(" fpStatus         : %u\n", (UINT32)pstVdispCh->Status.fpStatus);

	VDS_DPRINT(" u32LogTick_UpdateBuf: %u\n", (UINT32)pstVdispCh->Debug.u32LogTick_UpdateBuf);
	VDS_DPRINT(" u32nInt             : %u\n", (UINT32)pstVdispCh->Debug.u32nInt             );
	VDS_DPRINT(" u32nPopFrame        : %u\n", (UINT32)pstVdispCh->Debug.u32nPopFrame        );
	VDS_DPRINT(" u32nDispFrame       : %u\n", (UINT32)pstVdispCh->Debug.u32nDispFrame       );

	VDS_DPRINT(" Qlen             : %u\n", DISP_Q_NumActive(pstVdispCh->u8VdispCh));

	VDS_DPRINT("\n");
}

void VDISP_PrintDebug(struct seq_file *m)
{
	VDISP_CH_T *pstVdispCh;
	UINT32 i;

	VDS_DPRINT("seamless_mode   : %u\n\n", ui8Seamless);

	for (i=0;i<VDISP_NUM_OF_CH;i++)
	{

		pstVdispCh = VDISP_GetHandler(i);
		_vdisp_print_channel(m, pstVdispCh);
	}
#undef VDS_DPRINT
}

void VDISP_PrintDebugCmd(struct seq_file *m)
{
#define VDS_DPRINT(frm, args...)		seq_printf(m, frm, ##args )
	VDS_DPRINT("0: Set output port\n");
	VDS_DPRINT("	echo 0 [ch] [out port]\n");
	VDS_DPRINT("\n  out port\n");
	VDS_DPRINT("    0~ : out port #0 ~\n");
	VDS_DPRINT("    6 : background port\n");

	VDS_DPRINT("\n1: Set lipsync mode\n");
	VDS_DPRINT("	echo 1 [ch] [type]\n");
	VDS_DPRINT("\n  type\n");
	VDS_DPRINT("    0 : ENABLE\n");
	VDS_DPRINT("    1 : FREERUN_BASED_SYNC\n");
	VDS_DPRINT("    2 : FREERUN_IGNORE_SYNC\n");
	VDS_DPRINT("    3 : ADAPTIVE_FREERUN\n");

	VDS_DPRINT("\n2: Set display offset\n");
	VDS_DPRINT("	echo 2 [ch] [offset]\n");

	VDS_DPRINT("\n3: Set dual disp\n");
	VDS_DPRINT("	echo 3 [src ch] [dst ch]\n");

	VDS_DPRINT("\n4: Set discard level for async freerun\n");
	VDS_DPRINT("	echo 4 [lv]\n");

	VDS_DPRINT("\n5: Set debug rate\n");
	VDS_DPRINT("	echo 5 [ch] [res] [div]\n");
	VDS_DPRINT("	for disable\n");
	VDS_DPRINT("	echo 5 [ch] 0 0\n");

	VDS_DPRINT("\n6: Set adptive queue len\n");
	VDS_DPRINT("	echo 6 [ch] [qlen]\n");

	VDS_DPRINT("\n7: Set play speed(normal speed=1000)\n");
	VDS_DPRINT("	echo 7 [ch] [speed]\n");
	
	VDS_DPRINT("\n8: Set BBC VSI info setting timming\n");
	VDS_DPRINT("	echo 8 [ch] [timing(1 or 2]\n");

	VDS_DPRINT("\n9: Start vdisp\n");
	VDS_DPRINT("	echo 9 [ch] [stepmode, default:0]\n");

	VDS_DPRINT("\n10: Stop vdisp\n");
	VDS_DPRINT("	echo 10 [ch]\n");
#if defined(CHIP_NAME_d14)
	VDS_DPRINT("\n11: Set Mode(Normal(0)/Netflix(1)/BBC(2)/Amazon(3))\n");
	VDS_DPRINT("	echo 11 [Mode]\n");
#endif
}

void VDISP_DebugCmd(UINT32 u32nParam, CHAR *apParams[])
{
	UINT32		u32CmdType;
	UINT8		u8VdispCh;
	UINT32		au32Params[10];
	UINT32		i;

	for(i=0;i<10 && i<u32nParam;i++)
		au32Params[i] = simple_strtoul(apParams[i], NULL, 0);

	u32CmdType = au32Params[0];
	u8VdispCh = au32Params[1];

	switch (u32CmdType)
	{
	case 0:
		VDISP_SetOutPort(u8VdispCh, au32Params[2]);
		break;
	case 1:	// set lipsync mode
		VDISP_SetMatchMode(u8VdispCh, au32Params[2]);
		break;
	case 2: // set display offset
		VDISP_SetDisplayOffset(u8VdispCh, au32Params[2]);
		break;
	case 3:
		VDISP_SetDualOutput(u8VdispCh, au32Params[2]);
		break;
	case 4:
		PTS_SetDiscardLv(au32Params[1]);
		break;
	case 5:
		VRATE_UpdateDebugRate(u8VdispCh, au32Params[2], au32Params[3]);
		break;
	case 6:
		VDISP_SetMaxQlen(u8VdispCh, au32Params[2]);
		break;
	case 7:
		VDISP_SetSpeed(u8VdispCh, au32Params[2]);
		break;
	case 8:
		VDISP_BBCVsiSetTime(u8VdispCh, au32Params[2]);
		break;
	case 9:
		{
			BOOLEAN		step = FALSE;

			if (u32nParam > 2)
				step = au32Params[2];

			VDISP_Start(u8VdispCh, step);
		}
		break;
	case 10:
		VDISP_Stop(u8VdispCh);
		break;
#if defined(CHIP_NAME_d14)
	case 11:
		VDISP_SeamlessModeNoti(au32Params[1]);
		break;
#endif
	case 13:
		{
			E_VDISP_DST_T ePre;
			ePre = VDISP_SetOutPort(0, 2);

			if (ePre == DE_IF_DST_DE_IF0)
			{
				VDISP_SetOutPort(1, 0);
				VDISP_SetOutPort(0, 1);
			}
			else
			{
				VDISP_SetOutPort(1, 1);
				VDISP_SetOutPort(0, 0);
			}
		}
		break;
	case 14:
		VDISP_RepeatFrame(au32Params[2], au32Params[3]);
		break;
	}
	return;
}

