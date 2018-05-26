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
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sdec_impl.h"

#include "sdec_io.h"
#include "sdec_isr.h"
#include "sdec_pes.h"
#include "sdec_hal.h"
#include "sdec_filter.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/

/**
********************************************************************************
* @brief
*   initialize Mutex.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui32Arg :arguments from userspace
*   stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/

BOOLEAN SDEC_Section_Proc(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch, UINT8 ui8IsHigh)
{
	UINT32 ui32GpbDataIdx = 0x0;
	UINT8 ui8PidFltIdx = 0x0;
	UINT8 ui8SecFltIdx = 0x0;
	UINT8 ui8Count = 0x0;
	UINT32 ui32GpbWrPtr = 0x0;
	UINT32 ui32GpbRdPtr = 0x0;
	UINT32 ui32UsrRdPtr = 0x0;
	UINT32 ui32GpbBase = 0x0;
	UINT32 ui32GpbEnd = 0x0;
	BOOLEAN isFound = FALSE;
	unsigned long flags = 0;

	LX_SDEC_NOTIFY_PARAM_T notiParam;

	ui32GpbDataIdx = SDEC_HAL_GPBGetDataIntrStat(ui8Ch, ui8IsHigh);

	if(ui32GpbDataIdx)
	{
		SDEC_PRINT_ISR( "GPB_DATA_CH[%d]_GPB%c:[0x%08x]", ui8Ch, (ui8IsHigh)?'H':'L', ui32GpbDataIdx);

		/* 2011.03.18 - gaius.lee
		 * 32 bit를 모두 도는 것보다 필요한 bit만 돌고, 없으면 바로 끝나도록 수정.
		 */
		for (ui8Count = 0; ui8Count < 32 && ui32GpbDataIdx; ui8Count++)
		{
			if(ui8IsHigh)	ui8SecFltIdx = ui8Count + 32;
			else			ui8SecFltIdx = ui8Count;

			if ( check_one_bit(ui32GpbDataIdx, ui8Count) && (stpSdecParam->stSecMap[ui8Ch][ui8SecFltIdx].flag == TRUE))
			{
				SDEC_GPB_LOCK(stpSdecParam, ui8Ch, ui8SecFltIdx);
				ui32GpbWrPtr	= SDEC_HAL_GPBGetWritePtr(ui8Ch, ui8SecFltIdx);
				/* use s/w read pointer to implement h/w overflow */
				ui32GpbRdPtr	= stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].ui32Readptr;
				/* get user readpointer from h/w */
				ui32UsrRdPtr	= stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].ui32UsrReadptr;
				ui32GpbBase		= stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].ui32Baseptr;
				ui32GpbEnd		= stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].ui32Endptr;

				if( !ui32GpbBase || !ui32GpbEnd || !ui32GpbWrPtr || !ui32GpbRdPtr )
				{
					SDEC_PRINT_ERROR("in ISR, address is not valid : B[%08x] E[%08x] W[%08x] R[%08x]", ui32GpbBase, ui32GpbEnd, ui32GpbWrPtr, ui32GpbRdPtr);
					SDEC_GPB_UNLOCK(stpSdecParam, ui8Ch, ui8SecFltIdx);
					continue;
				}
				// 20110331 kibeom.kim : add the mask to apply the gpb_base_addr (logical address in L9) value
				// ui32GpbRdPtr & ui32GpbWrPtr is restricted to 28 bit.
				ui32GpbWrPtr	|= (ui32GpbBase & GPB_BASE_ADDR_MASK);
				ui32GpbRdPtr 	|= (ui32GpbBase & GPB_BASE_ADDR_MASK);
				ui32UsrRdPtr 	|= (ui32GpbBase & GPB_BASE_ADDR_MASK);

				/* in some case write pointer jump to wrong place, so resetting write pointer as base address */
				if( ui32GpbWrPtr < ui32GpbBase || ui32GpbEnd < ui32GpbWrPtr )
				{
					SDEC_PRINT_ERROR( "CH[%d] GPB_Data_%c GPB BND ERROR Idx[%d] b[%08x] w[%08x] e[%08x]",
										ui8Ch, (ui8IsHigh)?'H':'L', ui8SecFltIdx ,ui32GpbBase, ui32GpbWrPtr, ui32GpbEnd );

					/* disable section filter */
					SDEC_HAL_SECFClearEnableBit(ui8Ch, ui8SecFltIdx);
					SDEC_HAL_SECFClearBufValidBit(ui8Ch, ui8SecFltIdx);

					/* resetting write pointer and bound register */
					SDEC_HAL_GPBSetWritePtr(ui8Ch, ui8SecFltIdx, ui32GpbRdPtr);
					SDEC_HAL_GPBSetBnd(ui8Ch, ui8SecFltIdx, stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].stGpbBnd.l_bnd, stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].stGpbBnd.u_bnd);

					/* enable section filter */
					SDEC_HAL_SECFSetBufValidBit(ui8Ch, ui8SecFltIdx);
					SDEC_HAL_SECFSetEnableBit(ui8Ch, ui8SecFltIdx);

					/* continue to next */
					SDEC_GPB_UNLOCK(stpSdecParam, ui8Ch, ui8SecFltIdx);
					continue;
				}
				stpSdecParam->stSecMap[ui8Ch][ui8SecFltIdx].stStatusInfo.f.SDEC_FLTSTATE_DATAREADY = 0x1;

				SDEC_PRINT_ISR( "CH[%d] GPB_Data_%c GPB Idx[%d] rp[%d] wp[%d]",
					ui8Ch, (ui8IsHigh)?'H':'L', ui8SecFltIdx ,stpSdecParam->ui8GpbInfoRIdx, stpSdecParam->ui8GpbInfoWIdx);

				notiParam.channel	= ui8Ch;
				notiParam.index 	= ui8SecFltIdx;
				notiParam.readPtr	= ui32GpbRdPtr;
				notiParam.writePtr	= ui32GpbWrPtr;
				notiParam.status	= LX_SDEC_FLTSTATE_DATAREADY;
				notiParam.msec		= OS_GetMsecTicks();

#if ( LX_SDEC_USE_GPB_OW == 1 )
				/* check gpb overflow */
				if( ui32GpbRdPtr < ui32GpbWrPtr )
				{
					/* case 1.
					 *          ui32GpbRdPtr   ui32GpbWrPtr
					 *              V               V
					 * |---------------------------------------------------------|
					 *                  ^
					 *	           ui32UsrRdPtr
					 */
					if( ui32GpbRdPtr < ui32UsrRdPtr &&
						ui32UsrRdPtr < ui32GpbWrPtr )
					{
						SDEC_PRINT_ERROR( "1 CH[%d] GPB_Data_%c GPB Idx[%d] rp[%08x] wp[%08x] OVERFLOW",
											ui8Ch, (ui8IsHigh)?'H':'L', ui8SecFltIdx ,ui32GpbRdPtr, ui32GpbWrPtr);
						/* check previous queue */
						SDEC_CheckNotifyOvf(stpSdecParam, &notiParam);
					}
				}
				else
				{
					/* case 2.
					 *      ui32GpbWrPtr                             ui32GpbRdPtr
					 *          V                                          V
					 * |---------------------------------------------------------|
					 *                                                         ^
					 *	                                                 ui32UsrRdPtr
					 *
					 *     ui32GpbWrPtr                              ui32GpbRdPtr
					 *          V                                          V
					 * |---------------------------------------------------------|
					 *      ^
					 *  ui32UsrRdPtr
					 */
					if( ui32GpbRdPtr < ui32UsrRdPtr ||
						ui32UsrRdPtr < ui32GpbWrPtr )
					{
						SDEC_PRINT_ERROR( "2 CH[%d] GPB_Data_%c GPB Idx[%d] rp[%08x] wp[%08x] OVERFLOW",
											ui8Ch, (ui8IsHigh)?'H':'L', ui8SecFltIdx ,ui32GpbRdPtr, ui32GpbWrPtr);
						/* check previous queue */
						SDEC_CheckNotifyOvf(stpSdecParam, &notiParam);
					}
				}
#endif
				isFound = SDEC_SetNotifyParam( stpSdecParam, notiParam);

				SDEC_PRINT_ISR( "[%d] rp[0x%08x] wp[0x%08x] b[0x%08x]] e[0x%08x]"
							, ui8SecFltIdx, ui32GpbRdPtr , ui32GpbWrPtr ,ui32GpbBase, ui32GpbEnd);

				/* if write pointer arrive end, turn around to base */
				if( ui32GpbWrPtr == ui32GpbEnd )	ui32GpbWrPtr = ui32GpbBase;

				/* use s/w read pointer to implement h/w overflow */
				stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].ui32Readptr = ui32GpbWrPtr;

				/* if oneshot mode is true */
				if (check_one_bit(stpSdecParam->stSecMap[ui8Ch][ui8SecFltIdx].mode, 1))
				{
					SDEC_PRINT_ISR( "CH[%d] GPB_Data_%c GPB Idx[%d] one shot  mode", ui8Ch, (ui8IsHigh)?'H':'L', ui8SecFltIdx);

					/* Get PID Filter index */
					ui8PidFltIdx = stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].ui8PidFltIdx;

					/* Clear secion filter map register */
					SDEC_HAL_SECFClearMapBit(ui8Ch, ui8PidFltIdx, ui8SecFltIdx);

					/* disable pid filter if there is no section filter which is linked this pidfilter */
					if(SDEC_HAL_SECFGetLinkedMap(ui8Ch, ui8PidFltIdx) == 0)
					{
						SDEC_PRINT_ISR( "CH[%d] GPB_Data_%c GPB Idx[%d] Disable pid filter %d", ui8Ch, (ui8IsHigh)?'H':'L', ui8SecFltIdx, ui8PidFltIdx);
#ifdef NOT_USE_PIDF_SPINLOCK
						SDEC_HAL_PIDFEnable(ui8Ch, ui8PidFltIdx, SDEC_HAL_DISABLE);
#else
						SDEC_SetPidf_Enable(stpSdecParam, ui8Ch, ui8PidFltIdx, SDEC_HAL_DISABLE);
#endif
					}
				}

				SDEC_GPB_UNLOCK(stpSdecParam, ui8Ch, ui8SecFltIdx);

				/* clear bit for reduce useless iteration */
				clear__bit(ui32GpbDataIdx, ui8Count);
			}
		}
	}

	return isFound;
}

BOOLEAN SDEC_Full_Proc(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch, UINT8 ui8IsHigh)
{
	UINT32 ui32GpbDataIdx = 0x0;
	UINT8 ui8SecFltIdx = 0x0;
	UINT8 ui8Count = 0x0;
	UINT32 ui32GpbWrPtr = 0x0;
	UINT32 ui32GpbRdPtr = 0x0;
	UINT32 ui32GpbBase = 0x0;
	BOOLEAN isFound = FALSE;

	LX_SDEC_NOTIFY_PARAM_T notiParam;

	ui32GpbDataIdx = SDEC_HAL_GPBGetFullIntrStat(ui8Ch, ui8IsHigh);

	if(ui32GpbDataIdx)
	{
		/* 2011.03.18 - gaius.lee
		 * 32 bit를 모두 도는 것보다 필요한 bit만 돌고, 없으면 바로 끝나도록 수정.
		 */
		for (ui8Count = 0; ui8Count < 32 && ui32GpbDataIdx; ui8Count++)
		{
			if(ui8IsHigh)	ui8SecFltIdx = ui8Count + 32;
			else			ui8SecFltIdx = ui8Count;

			if ( check_one_bit(ui32GpbDataIdx, ui8Count) )
			{
				ui32GpbWrPtr	= SDEC_HAL_GPBGetWritePtr(ui8Ch, ui8SecFltIdx);
				/* use s/w read pointer to implement h/w overflow */
				ui32GpbRdPtr	= stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].ui32Readptr;
				ui32GpbBase		= stpSdecParam->stSdecMeminfo[ui8Ch][ui8SecFltIdx].ui32Baseptr;

				// 20110331 kibeom.kim : add the mask to apply the gpb_base_addr (logical address in L9) value
				// ui32GpbRdPtr & ui32GpbWrPtr is restricted to 28 bit.
				ui32GpbWrPtr	|= (ui32GpbBase & GPB_BASE_ADDR_MASK);
				ui32GpbRdPtr 	|= (ui32GpbBase & GPB_BASE_ADDR_MASK);

				SDEC_PRINT_ERROR( "GPB_FULL_CH[%d]_GP%c gpb[%d], wptr: [0x%08x], rptr: [0x%08x]",
					ui8Ch, (ui8IsHigh)?'H':'L',	ui8SecFltIdx, ui32GpbWrPtr, ui32GpbRdPtr);

				/*
				 * overflow check
				 * see above in SDEC_Section_Proc()
				 */
				/* stored buf info	*/
				notiParam.channel	= ui8Ch;
				notiParam.index 	= ui8SecFltIdx;
				notiParam.readPtr	= ui32GpbRdPtr;
				notiParam.writePtr	= ui32GpbWrPtr;
				notiParam.status	= LX_SDEC_FLTSTATE_OVERFLOW;
				notiParam.msec		= OS_GetMsecTicks();

			#if ( LX_SDEC_USE_GPB_OW == 1 )
				/* check previous queue */
				SDEC_CheckNotifyOvf(stpSdecParam, &notiParam);
			#endif

				isFound = SDEC_SetNotifyParam( stpSdecParam, notiParam);
			}
		}

		/* clear bit for reduce useless iteration */
		clear__bit(ui32GpbDataIdx, ui8Count);
	}

	return isFound;
}


#define SDEC_ERR_INTR_PRINT(field, str) \
{\
	if(field & 0x3) {\
		SDEC_PRINT_ISR( str " : %x", field); \
		if(field & 0x1) SDEC_PRINT_ISR( RED_COLOR "Ch[A]: " str NORMAL_COLOR); \
		if(field & 0x2) SDEC_PRINT_ISR( RED_COLOR "Ch[B]: " str NORMAL_COLOR); \
	}\
}

#define ERR_INTR_TEST_DCOUNT	0x1
#define ERR_INTR_SYNC_LOST		0x30
#define ERR_INTR_SB_DROPPED		0xC0
#define ERR_INTR_CDIF_OVFLOW	0x300
#define ERR_INTR_CDIF_WPAGE		0xC00
#define ERR_INTR_CDIF_RPAGE		0x3000
#define ERR_INTR_STCC_DCOUNT	0xC000
#define ERR_INTR_MPG_PD			0x30000
#define ERR_INTR_MPG_TS			0xC0000
#define ERR_INTR_MPG_DUP		0x300000
#define ERR_INTR_MPG_CC			0xC00000
#define ERR_INTR_MPG_SD			0x3000000

int SDEC_ErrIntrCheck(UINT8 core, S_SDEC_PARAM_T *stpSdecParam)
{
	UINT32 stat;

	stat =	SDEC_HAL_GetErrorInterruptStat(core);

//	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_TEST_DCOUNT) 	>> 0  ), "TEST_DCOUNT");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_SYNC_LOST) 	>> 4  ), "SYNC_LOST");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_SB_DROPPED) 	>> 6  ), "SB_DROPPED");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_CDIF_OVFLOW) 	>> 8  ), "CDIF_OVFLOW");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_CDIF_WPAGE) 	>> 10 ), "CDIF_WPAGE");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_CDIF_RPAGE) 	>> 12 ), "CDIF_RPAGE");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_STCC_DCOUNT) 	>> 14 ), "STCC_DCOUNT");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_MPG_PD) 		>> 16 ), "MPG_PD");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_MPG_TS) 		>> 18 ), "MPG_TS");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_MPG_DUP) 		>> 20 ), "MPG_DUP");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_MPG_CC) 		>> 22 ), "MPG_CC");
	SDEC_ERR_INTR_PRINT(((stat & ERR_INTR_MPG_SD) 		>> 24 ), "MPG_SD");

	return RET_OK;
}


int SDEC_TPInfoIntrCheck(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch)
{
	int eRet = RET_OK;
	TPI_ISTAT stTPIInfo, stTPIInfo_remain, *stpTPIInfo;

	UINT8 ui8PIDIdx = 0x0;
	UINT8 i = 0;

	if (stpSdecParam == NULL)
	{
		SDEC_PRINT_ERROR("Invalid parameters");
		eRet = RET_ERROR;

		goto error;
	}

	stTPIInfo = SDEC_HAL_TPIGetIntrStat(ui8Ch);
	stpTPIInfo = &stTPIInfo;
	for(i=0;i<TPI_ITR_BUF_SIZE-1;i++)
	{
		// to be empty of TPI Interrupt Buffer, process last one only
		stTPIInfo_remain = SDEC_HAL_TPIGetIntrStat(ui8Ch);
		if( 0 != stTPIInfo_remain.pusi)
		{
			SDEC_PRINT_ISR("RTPI[%d][%d]",stTPIInfo_remain.pidf_idx, stTPIInfo_remain.pusi);
			stpTPIInfo = &stTPIInfo_remain;
		}
	}

	ui8PIDIdx  = stpTPIInfo->pidf_idx;

	if(stpTPIInfo->afef == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:afef", ui8PIDIdx);
	}
	if(stpTPIInfo->tpdf == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:tpdf", ui8PIDIdx);
	}
	if(stpTPIInfo->spf == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:SPF", ui8PIDIdx);
	}
	if(stpTPIInfo->espi == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:ESPI", ui8PIDIdx);
	}
	if(stpTPIInfo->rai == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:RAI", ui8PIDIdx);
	}
	if(stpTPIInfo->di == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:DI", ui8PIDIdx);
	}
	if(stpTPIInfo->tpri == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:TPRI", ui8PIDIdx);
	}
	if(stpTPIInfo->pusi == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:PUSI", ui8PIDIdx);

		stpSdecParam->stPIDMap[ui8Ch][ui8PIDIdx].stStatusInfo.f.SDEC_FLTSTATE_SCRMREADY = 0x1;

		if(stpTPIInfo->tsc == 0x2 || stpTPIInfo->tsc == 0x3)
		{
			stpSdecParam->stPIDMap[ui8Ch][ui8PIDIdx].stStatusInfo.f.SDEC_FLTSTATE_SCRAMBLED = 0x1;
		}

#if 0
		/* check once at a time */
		// jinhwan.bae for No Audio Issue, 2012.04.26
		// replaced following spin lock I/F
#if 0 // gaius original
		SDEC_HAL_PIDFScrambleCheck(ui8Ch, ui8PIDIdx, SDEC_HAL_DISABLE);
		SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(ui8Ch, SDEC_HAL_DISABLE);
#else
		SDEC_SetPidf_TPI_IEN_Enable(stpSdecParam, ui8Ch, ui8PIDIdx, SDEC_HAL_DISABLE);
		SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(ui8Ch, SDEC_HAL_DISABLE);
#endif
#else
		if(stpSdecParam->ui8McuDescramblerCtrlMode == 1)	/* H13 JP */
		{
			stpSdecParam->stTPI_Intr.ui8Channel		= ui8Ch;
			stpSdecParam->stTPI_Intr.ui8PIDIdx		= ui8PIDIdx;
			queue_work(stpSdecParam->workqueue, &stpSdecParam->TPIIntr);
		}
		else
		{
			SDEC_SetPidf_TPI_IEN_Enable(stpSdecParam, ui8Ch, ui8PIDIdx, SDEC_HAL_DISABLE);
			SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(ui8Ch, SDEC_HAL_DISABLE);
		}
#endif
	}
	if(stpTPIInfo->tei == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:TEI", ui8PIDIdx);
	}
	if(stpTPIInfo->tsc_chg == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:TSC_CHG", ui8PIDIdx);
	}
	if(stpTPIInfo->auto_sc_chk == 0x1)
	{
		SDEC_PRINT_ISR("idx[%d]:AUTO_SC_CHK", ui8PIDIdx);
	}

	return (eRet);

error:
	return (eRet);
}

static void SDEC_ISR_Handler(UINT8 core, S_SDEC_PARAM_T *stpSdecParam)
{
	SE_ISTAT	stSeIntrStat;

	UINT32 ui32IntrStat = 0x0;
	UINT8 ui8MainStc = 0x0;
	BOOLEAN isFound = FALSE;
	UINT8 baseCh;
	UINT32 val[2];

	int i = 0;


	ui32IntrStat = SDEC_HAL_IntrStatReadAndClear(core);

	baseCh = SDEC_CORE_CH_NUM*core;

	/* PCR Arrival */
	if((ui32IntrStat & PCR) == PCR)
	{
		ui8MainStc = SDEC_HAL_STCCGetMain(core);
		stpSdecParam->stPCR_STC.ui8Channel		= ui8MainStc + baseCh;	/* io channel is also total 8 */

		SDEC_HAL_STCCGetLatchedSTC(core, ui8MainStc, &val[0], &val[1]);
		stpSdecParam->stPCR_STC.STC_hi_value = val[0];
		stpSdecParam->stPCR_STC.PCR_lo_value = val[1];

		SDEC_HAL_STCCGetPCR(core, ui8MainStc, &val[0], &val[1]);
		stpSdecParam->stPCR_STC.PCR_hi_value = val[0];
		stpSdecParam->stPCR_STC.PCR_lo_value = val[1];

		SDEC_HAL_STCCEnableLatch(core, 0, SDEC_HAL_ENABLE);
		SDEC_HAL_STCCEnableLatch(core, 1, SDEC_HAL_ENABLE);

		queue_work(stpSdecParam->workqueue, &stpSdecParam->PcrRecovery);
	}

	if((ui32IntrStat & TB_DCOUNT) == TB_DCOUNT) /* Time Base Discontinuity at TS Packet */
	{
		SDEC_PRINT_ISR( "CORE[%d] TB_DCOUNT:[0x%08x]", core, ui32IntrStat);
	}

	/* BDRC Interrupt */
	if((ui32IntrStat & BDRC_0) == BDRC_0) SDEC_PRINT_ISR( "CORE[%d] BDRC_0:[0x%08x]", core, ui32IntrStat);
	if((ui32IntrStat & BDRC_1) == BDRC_1) SDEC_PRINT_ISR( "CORE[%d] BDRC_1:[0x%08x]", core, ui32IntrStat);
	if((ui32IntrStat & BDRC_2) == BDRC_2) SDEC_PRINT_ISR( "CORE[%d] BDRC_2:[0x%08x]", core, ui32IntrStat);
	if((ui32IntrStat & BDRC_3) == BDRC_3) SDEC_PRINT_ISR( "CORE[%d] BDRC_3:[0x%08x]", core, ui32IntrStat);

	/* Error Report Interrupt set by ERR_INTR_EN */
	if((ui32IntrStat & ERR_RPT) == ERR_RPT)
	{
		SDEC_PRINT_ISR( "CORE[%d] ERR_RPT:[0x%08x]", core, ui32IntrStat);
		SDEC_ErrIntrCheck(core, stpSdecParam);
	}

	/* GPB FULL */
	if((ui32IntrStat & GPB_FULL_CHA_GPL) == GPB_FULL_CHA_GPL)	SDEC_Full_Proc(stpSdecParam, 0 + baseCh, 0); /* CH_A FULL Low */
	if((ui32IntrStat & GPB_FULL_CHA_GPH) == GPB_FULL_CHA_GPH)	SDEC_Full_Proc(stpSdecParam, 0 + baseCh, 1); /* CH_A FULL High */
	if((ui32IntrStat & GPB_FULL_CHB_GPL) == GPB_FULL_CHB_GPL)	SDEC_Full_Proc(stpSdecParam, 1 + baseCh, 0); /* CH_B FULL Low */
	if((ui32IntrStat & GPB_FULL_CHB_GPH) == GPB_FULL_CHB_GPH)	SDEC_Full_Proc(stpSdecParam, 1 + baseCh, 1); /* CH_B FULL High */

	/* GPB Data Arrived !*/
	if((ui32IntrStat & GPB_DATA_CHA_GPL) == GPB_DATA_CHA_GPL)	isFound = SDEC_Section_Proc(stpSdecParam, 0 + baseCh, 0); /* CH_A Low */
	if((ui32IntrStat & GPB_DATA_CHA_GPH) == GPB_DATA_CHA_GPH)	isFound = SDEC_Section_Proc(stpSdecParam, 0 + baseCh, 1); /* CH_A High */
	if((ui32IntrStat & GPB_DATA_CHB_GPL) == GPB_DATA_CHB_GPL)	isFound = SDEC_Section_Proc(stpSdecParam, 1 + baseCh, 0); /* CH_B Low */
	if((ui32IntrStat & GPB_DATA_CHB_GPH) == GPB_DATA_CHB_GPH)	isFound = SDEC_Section_Proc(stpSdecParam, 1 + baseCh, 1); /* CH_B High */

	/* if found notifying section data, notify to appl layer */
	if(isFound) queue_work(stpSdecParam->workqueue, &stpSdecParam->Notify);

#if 1	// jinhwan.bae for MCU test
	/* TP Info Interrupt */
	if((ui32IntrStat & TP_INFO_CHA) == TP_INFO_CHA)
	{
		SDEC_PRINT_ISR( "CORE[%d] TP_INFO_CHA:[0x%08x]", core, ui32IntrStat);
		SDEC_TPInfoIntrCheck(stpSdecParam, 0 + baseCh);
	}
	if(stpSdecParam->ui8McuDescramblerCtrlMode != 1) // in JP, disable CH_B TPI Interrupt Processing for MCU
	{
		if((ui32IntrStat & TP_INFO_CHB) == TP_INFO_CHB)
		{
			SDEC_PRINT_ISR( "CORE[%d] TP_INFO_CHB:[0x%08x]", core, ui32IntrStat);
			SDEC_TPInfoIntrCheck(stpSdecParam, 1 + baseCh);
		}
	}
#endif

	/* Section Error */
	if((ui32IntrStat & SEC_ERR_CHA) == SEC_ERR_CHA)
	{
		/* jinhwan.bae 20140320 tvct problem - clean up buffer */
		for(i=0;i<SEC_ERR_ITR_BUF_SIZE;i++)
		{
			stSeIntrStat = SDEC_HAL_SEGetIntrStat(0 + baseCh);
			SDEC_PRINT_ISR_SEC_ERR( "CORE[%d] SEC_ERR_CHA:[0x%08x]", core, ui32IntrStat);
			SDEC_PRINT_ISR_SEC_ERR( "CORE[%d] CHA valid:[0x%08x], err_type:[0x%08x], filter_idx:[0x%08x]",
									core, stSeIntrStat.valid, stSeIntrStat.err_type, stSeIntrStat.filter_idx);
		}
	}
	if((ui32IntrStat & SEC_ERR_CHB) == SEC_ERR_CHB)
	{
		/* jinhwan.bae 20140320 tvct problem - clean up buffer */
		for(i=0;i<SEC_ERR_ITR_BUF_SIZE;i++)
		{
			stSeIntrStat = SDEC_HAL_SEGetIntrStat(1 + baseCh); /* direct access , so 2,3 is used */
			SDEC_PRINT_ISR_SEC_ERR( "CORE[%d] SEC_ERR_CHB:[0x%08x]", core, ui32IntrStat);
			SDEC_PRINT_ISR_SEC_ERR( "CORE[%d] CHB valid:[0x%08x], err_type:[0x%08x], filter_idx:[0x%08x]",
									core, stSeIntrStat.valid, stSeIntrStat.err_type, stSeIntrStat.filter_idx);
		}
	}
}


void SDEC_PollThread(void *param)
{
	S_SDEC_PARAM_T 	*stpSdecParam 	= (S_SDEC_PARAM_T*) param;
	UINT8			ui8Ch = 0, ui8cdif_full = 0, ui8cdif_ovf = 0;
	unsigned long 	flags = 0;
	UINT8			org_ch = 0, core = 0;

	/* polling SDEC stuck , check only A/V Channel */
	for ( org_ch = 0 ; org_ch < 6 ; org_ch++ )
	{
		if (lx_chip_rev() < LX_CHIP_REV(M14, B0))
		{
			if(org_ch >= 2) return;
		}

		if((org_ch == 2) || (org_ch == 3)) continue;

		ui8Ch = org_ch;
		SDEC_CONVERT_CORE_CH(core, ui8Ch);

		/* check input fifo overflow and full bit */
		ui8cdif_full	= SDEC_HAL_CIDCGetCdif_Full(core, ui8Ch);
		ui8cdif_ovf		= SDEC_HAL_CIDCGetCdif_OverFlow(core, ui8Ch);

		/* this means something bad happen */
		if(ui8cdif_full || ui8cdif_ovf)
		{
			if ( stpSdecParam->ui8CDICResetMode == E_SDEC_RESET_MODE_ENABLE ||	/* Reseting SDMWC is enabled */
				/* Reseting SDMWC is permitted only 1 time and this is first time. */
				 ( stpSdecParam->ui8CDICResetMode == E_SDEC_RESET_MODE_ONETIME	&& stpSdecParam->ui8CDICResetNum == 0))
			{
				/* increase SDWMC Reset Number */
				if (stpSdecParam->ui8CDICResetMode == E_SDEC_RESET_MODE_ONETIME)
					  stpSdecParam->ui8CDICResetNum++;

				spin_lock_irqsave(&stpSdecParam->stSdecResetSpinlock, flags);

				SDEC_PRINT_RESET(  RED_COLOR "SDIO RESET CH[%d]" NORMAL_COLOR "", org_ch );
				SDEC_PRINT_RESET( "CDIC       [0x%08x]", SDEC_HAL_CIDCGet(core, ui8Ch));
				SDEC_PRINT_RESET( "SDMWC      [0x%08x]", SDEC_HAL_SDMWCGet(core));
				SDEC_PRINT_RESET( "SDMWC_STAT [0x%08x]", SDEC_HAL_SDMWCGetStatus(core));
				SDEC_PRINT_RESET( "CHAN_STAT  [0x%08x]", SDEC_HAL_GetChannelStatus2(org_ch));
				SDEC_PRINT_RESET( "CC_CHK_EN  [0x%08x%08x]", SDEC_HAL_CCCheckEnableGet(org_ch, 0), SDEC_HAL_CCCheckEnableGet(org_ch, 1));
				SDEC_InputPortEnable(core, stpSdecParam->eInputPath[org_ch], SDEC_HAL_DISABLE);
				OS_UsecDelay(10);
				/* Reset Input port */
				SDEC_InputPortReset(org_ch);
				OS_UsecDelay(10);
				/* CLEAR CC_CHK_EN Register */
				SDEC_HAL_CCCheckEnableSet(org_ch, 0, 0);
				SDEC_HAL_CCCheckEnableSet(org_ch, 1, 0);
				/* enable channel input */
				SDEC_InputPortEnable(core, stpSdecParam->eInputPath[org_ch], SDEC_HAL_ENABLE);

				spin_unlock_irqrestore(&stpSdecParam->stSdecResetSpinlock, flags);
			}
			else
			{
				switch(stpSdecParam->ui8CDICResetMode)
				{
					case E_SDEC_RESET_MODE_DISABLE :
						SDEC_PRINT_RESET( "SDIO RESET is disabled\n");
						break;
					case E_SDEC_RESET_MODE_ONETIME :
						SDEC_PRINT_RESET( "SDIO RESET is one time mode and number is %d.\n", stpSdecParam->ui8CDICResetNum);
						break;
					default:
						SDEC_PRINT_RESET( "??? SDIO RESET MODE is %d\n", stpSdecParam->ui8CDICResetMode);
						break;
				}
			}
		}
	}
}


static irqreturn_t sdec_isr0(int irq, void *dev_id)
{
	S_SDEC_PARAM_T *stpSdecParam;

	stpSdecParam = (S_SDEC_PARAM_T *)dev_id;

	SDEC_ISR_Handler(_SDEC_CORE_0, stpSdecParam);

	return IRQ_HANDLED;
}

static irqreturn_t sdec_isr1(int irq, void *dev_id)
{
	S_SDEC_PARAM_T *stpSdecParam;

	stpSdecParam = (S_SDEC_PARAM_T *)dev_id;

	SDEC_ISR_Handler(_SDEC_CORE_1, stpSdecParam);

	return IRQ_HANDLED;
}

int SDEC_ISR_Init(S_SDEC_PARAM_T *stpSdecParam)
{
	int core;
	struct {
		const char *name;
		irqreturn_t (*handler)(int irq, void *dev_id);
	} sdec_irqs[2] = {
		{"sdec0", sdec_isr0},
		{"sdec1", sdec_isr1},
	};
	LX_SDEC_CFG_T *cfg = SDEC_CFG_GetConfig();

	for(core=0; core<cfg->nCore; core++)
	{
		if(request_irq(cfg->irqNum[core], sdec_irqs[core].handler, 0, sdec_irqs[core].name, stpSdecParam))
		{
			SDEC_PRINT_ERROR("%s request_irq failed[%d]", sdec_irqs[core].name, cfg->irqNum[core]);
			return -EIO;
		}
	}

	return 0;
}


void SDEC_ISR_Cleanup(S_SDEC_PARAM_T *stpSdecParam)
{
	int core;
	LX_SDEC_CFG_T *cfg = SDEC_CFG_GetConfig();

	for(core=0; core<cfg->nCore; core++)
	{
		free_irq(cfg->irqNum[core], stpSdecParam);
	}
}

