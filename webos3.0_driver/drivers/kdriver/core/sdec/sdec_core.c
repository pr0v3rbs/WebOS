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
#include "sdec_hal.h"
#include "sdec_pcr.h"
#include "sdec_core.h"
#include "sdec_pes.h"
#include "sdec_filter.h"
#include "sdec_cipher.h"
#include "sdec_swparser.h"
#include "ciplus/sdec_ciplus.h"

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
 *	 initialize Mutex.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_MutexInitialize
	 (S_SDEC_PARAM_T *stpSdecParam)
 {
	 int eRet = RET_ERROR;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	 SDEC_PRINT_TRACE( "<--SDEC_MutexInitialize");

	 //mutex_init(&stpSdecParam->stSdecMutex);
	 OS_InitMutex(&stpSdecParam->stSdecMutex, OS_SEM_ATTR_DEFAULT);

	 SDEC_PRINT_TRACE( "-->SDEC_MutexInitialize");

	 eRet = RET_OK;

	 return (eRet);
 }

 /**
 ********************************************************************************
 * @brief
 *	 Deastory Mutex.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_MutexDestroy
	 (S_SDEC_PARAM_T *stpSdecParam)
 {
	 int eRet = RET_ERROR;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	 SDEC_PRINT_TRACE( "<--SDEC_MutexInitialize");

	 //mutex_init(&stpSdecParam->stSdecMutex);
	 OS_InitMutex(&stpSdecParam->stSdecMutex, OS_SEM_ATTR_DEFAULT);

	 SDEC_PRINT_TRACE( "-->SDEC_MutexInitialize");

	 eRet = RET_OK;

	 return (eRet);
 }


 /**
 ********************************************************************************
 * @brief
 *	 initialize spinlock.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 ui32Arg :arguments from userspace
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_SpinLockInitialize
	 (S_SDEC_PARAM_T *stpSdecParam)
 {
	 int eRet = RET_ERROR;
	 int ch_idx = 0, flt_idx = 0;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	 SDEC_PRINT_TRACE( "<--SDEC_SpinLockInitialize");

	 spin_lock_init(&stpSdecParam->stSdecNotiSpinlock);
	 spin_lock_init(&stpSdecParam->stSdecPesSpinlock);
	 spin_lock_init(&stpSdecParam->stSdecSecSpinlock);
	 spin_lock_init(&stpSdecParam->stSdecResetSpinlock);
	 spin_lock_init(&stpSdecParam->stSdecPidfSpinlock);

	 for ( ch_idx = 0; ch_idx < SDEC_IO_CH_NUM ; ch_idx++ )
	 {
		 for ( flt_idx = 0; flt_idx < SDEC_IO_FLT_NUM ; flt_idx++ )
		 {
			 spin_lock_init(&stpSdecParam->stSdecGpbSpinlock[ch_idx][flt_idx]);
		 }
	 }

	 SDEC_PRINT_TRACE( "-->SDEC_SpinLockInitialize");

	 eRet = RET_OK;

	 return (eRet);
 }


 /**
 ********************************************************************************
 * @brief
 *	 initialize Workqueue.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_WorkQueueInitialize
	 (S_SDEC_PARAM_T *stpSdecParam)
 {
	 int eRet = RET_ERROR;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	 SDEC_PRINT_TRACE( "<--SDEC_WorkQueueInitialize");

	 //Sdec work queue init and fnc reg
	 stpSdecParam->workqueue = create_workqueue("SDEC");

	 if(!stpSdecParam->workqueue){
		 SDEC_PRINT_ERROR("create work queue failed");
		 goto exit;
	 }

	 INIT_WORK(&stpSdecParam->Notify,		 SDEC_Notify);
	 INIT_WORK(&stpSdecParam->PcrRecovery,	 SDEC_PCRRecovery);
	 INIT_WORK(&stpSdecParam->PesProc,		 SDEC_PES_Proc);
	 INIT_WORK(&stpSdecParam->TPIIntr,		 SDEC_TPIIntr);

	 eRet = RET_OK;

	 SDEC_PRINT_TRACE( "-->SDEC_WorkQueueInitialize");

 exit:
	 return (eRet);
 }

 /**
 ********************************************************************************
 * @brief
 *	 initialize Workqueue destory.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_WorkQueueDestroy
	 (S_SDEC_PARAM_T *stpSdecParam)
 {
	 int eRet = RET_ERROR;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	 SDEC_PRINT_TRACE( "<--SDEC_WorkQueueDestroy");

	 flush_workqueue(stpSdecParam->workqueue);

	 destroy_workqueue(stpSdecParam->workqueue);

	 SDEC_PRINT_TRACE( "-->SDEC_WorkQueueDestroy");

	 eRet = RET_OK;

	 return (eRet);
 }

 /**
 ********************************************************************************
 * @brief
 *	 initialize Watequeue destory.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_WaitQueueInitialize
	 (S_SDEC_PARAM_T *stpSdecParam)
 {
	 int eRet = RET_ERROR;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	 SDEC_PRINT_TRACE( "<--SDEC_WaitQueueInitialize");

	 init_waitqueue_head(&stpSdecParam->wq);
	 stpSdecParam->wq_condition = 0;

	 /* SW Parser Event Init */
	 OS_InitEvent( &stpSdecParam->stSdecSWPEvent);

	 /* LIVE_HEVC */
	 OS_InitEvent( &stpSdecParam->stSdecRFOUTEvent);

	 SDEC_PRINT_TRACE( "-->SDEC_WaitQueueInitialize");

	 eRet = RET_OK;

	 return (eRet);
}


/**
********************************************************************************
* @brief
*   initialize sdec module.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam : SDEC parameter
*   stpLXSdecCap : ioctrl arguments from userspace
* @return
*  int
********************************************************************************
*/
int SDEC_IO_InitialaizeModule
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_CAP_T*	stpLXSdecCap)
{
	int 	eRet = RET_ERROR;
	int 	eResult = RET_ERROR;
	UINT8 			ui8ch;
	LX_SDEC_CFG_T* 	pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecCap == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_InitialaizeModule");

	ui8ch = stpLXSdecCap->eCh;


	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8ch);


	/* pidf init */
	if(pSdecConf->chInfo[ui8ch].num_pidf != 0)
	{
		eResult = SDEC_Pidf_Clear(stpSdecParam, ui8ch, CLEAR_ALL_MODE);
		LX_SDEC_CHECK_CODE( LX_IS_ERR(eResult), goto exit, "SDEC_Pidf_Clear failed:[%d]", eResult);
	}

	/* secf init */
	if(pSdecConf->chInfo[ui8ch].num_secf != 0)
	{
		eResult = SDEC_Secf_Clear(stpSdecParam, ui8ch, CLEAR_ALL_MODE);
		LX_SDEC_CHECK_CODE( LX_IS_ERR(eResult), goto exit, "SDEC_Secf_Clear failed:[%d]", eResult);
	}

	/* get filter number & depth from channel info structure */
	stpLXSdecCap->ucPidFltNr = pSdecConf->chInfo[ui8ch].num_pidf;
	stpLXSdecCap->ucSecFltNr = pSdecConf->chInfo[ui8ch].num_secf;
	stpLXSdecCap->ucFltDepth = pSdecConf->chInfo[ui8ch].flt_dept;

	SDEC_PRINT_TRACE( "-->SDEC_IO_InitialaizeModule");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   Set MCU Descrambler Setting Mode in H13, M14A0
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam : SDEC parameter
*   stpLXSdecSetMcumode : ioctrl arguments from userspace
* @return
*  int
********************************************************************************
*/
int SDEC_IO_SetMCUDescramblerCtrlMode
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SET_MCUMODE_T *stpLXSdecSetMcumode)
{
	/* H13 Only */
	if (lx_chip_rev() < LX_CHIP_REV(M14, A0))
	{
		stpSdecParam->ui8McuDescramblerCtrlMode = stpLXSdecSetMcumode->ui32param;
	}

	return RET_OK;
}

/**
********************************************************************************
* @brief
*   Set MCU Descrambler Setting Mode in H13, M14A0
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam : SDEC parameter
*   stpLXSdecSetMcumode : ioctrl arguments from userspace
* @return
*  int
********************************************************************************
*/
int SDEC_IO_SetTVCTMode
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SET_TVCTMODE_T *stpLXSdecClearTVCTGathering)
{
	stpSdecParam->ui8ClearTVCTGathering = stpLXSdecClearTVCTGathering->ui32param;
	return RET_OK;
}

/**
********************************************************************************
* @brief
*   Set MCU Descrambler Setting Mode in H13, M14A0
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam : SDEC parameter
*   stpLXSdecSetMcumode : ioctrl arguments from userspace
* @return
*  int
********************************************************************************
*/
int SDEC_IO_UseOrgExtDemodCLKinSerialInput
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SET_SERIALCLKMODE_T *stpLXSdecSetSerialClkMode)
{

	stpSdecParam->ui8UseOrgExtDemodClk = stpLXSdecSetSerialClkMode->ui32param;

	return RET_OK;
}



/**
********************************************************************************
* @brief
*   sdec param init.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
static int SDEC_ParamInit(S_SDEC_PARAM_T *stpSdecParam)
{
	int eRet = RET_ERROR;

	UINT8 ui8Count = 0x0, i = 0;
	UINT8 ui8Countch = 0x0;

	LX_SDEC_CFG_T* 	cfg 		= NULL;
	UINT8			ui8ChNum		= 0;
	UINT8			ui8PidfNum		= 0;
	UINT8			ui8SecfNum		= 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_ParamInit");

	/* get chip configuation */
	cfg = SDEC_CFG_GetConfig();
	stpSdecParam->cfg = cfg;

	/* get informations from channel info structure */
	ui8ChNum	= cfg->nChannel;

	for(ui8Countch = 0; ui8Countch < ui8ChNum; ui8Countch++)
	{
		/* get pid filter number from channel info structure */
		ui8PidfNum	= cfg->chInfo[ui8Countch].num_pidf;

		/* pid filter map init */
		for(ui8Count = 0; ui8Count < ui8PidfNum; ui8Count++)
		{
			stpSdecParam->stPIDMap[ui8Countch][ui8Count].used = 0x0;
			stpSdecParam->stPIDMap[ui8Countch][ui8Count].flag = FALSE;
			stpSdecParam->stPIDMap[ui8Countch][ui8Count].mode = 0x0;
			stpSdecParam->stPIDMap[ui8Countch][ui8Count].stStatusInfo.w = 0x0;
		}

		/* get pid filter number from channel info structure */
		ui8SecfNum	= cfg->chInfo[ui8Countch].num_secf;
		/* section filter map init */
		for(ui8Count = 0; ui8Count < ui8SecfNum; ui8Count++)
		{
			stpSdecParam->stSecMap[ui8Countch][ui8Count].used = 0x0;
			stpSdecParam->stSecMap[ui8Countch][ui8Count].flag = FALSE;
			stpSdecParam->stSecMap[ui8Countch][ui8Count].mode = 0x0;
			stpSdecParam->stSecMap[ui8Countch][ui8Count].stStatusInfo.w = 0x0;

			stpSdecParam->stSdecMeminfo[ui8Countch][ui8Count].ui32Baseptr = 0x0;
			stpSdecParam->stSdecMeminfo[ui8Countch][ui8Count].ui32Endptr = 0x0;
			stpSdecParam->stSdecMeminfo[ui8Countch][ui8Count].ui32Readptr = 0x0;
			stpSdecParam->stSdecMeminfo[ui8Countch][ui8Count].ui32UsrReadptr = 0x0;
			stpSdecParam->stSdecMeminfo[ui8Countch][ui8Count].ui8PidFltIdx = 0x0;
		}
	}

	//gpbdata structure init
	memset(stpSdecParam->stGPBInfo, 0x0, sizeof(LX_SDEC_NOTIFY_PARAM_T) * LX_SDEC_MAX_GPB_DATA);

	//valuable init
	stpSdecParam->ui8CurrentCh = 0x0;
	stpSdecParam->ui8CurrentMode = 0x0;
	stpSdecParam->bPcrRecoveryFlag[0] = 0x0;
	stpSdecParam->bPcrRecoveryFlag[1] = 0x0;
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		for(i=0;i<SDEC_IO_CH_NUM;i++)
		{
			stpSdecParam->bPcrRecoveryFlag[i] = 0x0;
		}
	}

	stpSdecParam->ui8GpbInfoWIdx = 0x0;
	stpSdecParam->ui8GpbInfoRIdx = 0x0;

	/* set reset mode */
	stpSdecParam->ui8CDICResetMode		= E_SDEC_RESET_MODE_ENABLE;
	stpSdecParam->ui8SDMWCResetMode	= E_SDEC_RESET_MODE_ONETIME;

	/* set reset number as 0 */
	stpSdecParam->ui8CDICResetNum		= 0;
	stpSdecParam->ui8SDMWCResetNum		= 0;


	/* for JP MCU Descrambler Mode, initial value = 0, for all market , and JP set this to 1 in SDEC_IO_Init */
	stpSdecParam->ui8McuDescramblerCtrlMode	= 0;

	/* for using original external demod clock in serial input, ex> Columbia, set 0 for all market, and Columbia set this to 1 in SDEC_IO_Init */
	stpSdecParam->ui8UseOrgExtDemodClk = 1;		// use ext demod clock as default - 20140630 ks.hyun

	/* for UHD bypass channel */
	stpSdecParam->ui8RFBypassChannel = 0;

	/* for UHD SDT channel in H13 */
	stpSdecParam->ui8SDTChannel = 0;

	/* for ATSC TVCT mode */
	stpSdecParam->ui8ClearTVCTGathering = 0;

	SDEC_PRINT_TRACE( "-->SDEC_ParamInit");

	eRet = RET_OK;

	return (eRet);
}


/**
********************************************************************************
* @brief
*   GPB init.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui8GpbSize :gpb size
*   stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
static int SDEC_Gpb_Init(void)
{
	UINT8 ui8Ch, ui8Idx;
	UINT32 gpbBaseAddr, gpbBaseAddr_H, gpbBaseAddr_L;
	LX_MEMCFG_T* memCfg;
	LX_SDEC_CFG_T* cfg;
	int core;
	UINT8 ch;

	SDEC_PRINT_TRACE( "<--SDEC_Gpb_Init");

	cfg = SDEC_CFG_GetConfig();
	memCfg = SDEC_CFG_GetMemConfig();

	gpbBaseAddr = memCfg->base;
	gpbBaseAddr &= ~GPB_BASE_ADDR_MASK;
	gpbBaseAddr_H = ( gpbBaseAddr >> 16 ) & 0xFFFF;
	gpbBaseAddr_L = ( gpbBaseAddr >> 0  ) & 0xFFFF;

	/* initialize BND & Write & Read pointer Register as base address */
	for(core=0; core<cfg->nCore; core++)
	{
		for ( ui8Ch = 0 ; ui8Ch < 2 ; ui8Ch++ )
		{
			ch = ui8Ch + core*SDEC_CORE_CH_NUM;
			for ( ui8Idx = 0 ; ui8Idx < 64 ; ui8Idx++ )
			{
				SDEC_HAL_GPBSetBnd(ch, ui8Idx, gpbBaseAddr_H, gpbBaseAddr_L);
				SDEC_HAL_GPBSetReadPtr(ch, ui8Idx, gpbBaseAddr);
				SDEC_HAL_GPBSetWritePtr(ch, ui8Idx, gpbBaseAddr);
			}
		}
	}

	for(core=0; core<cfg->nCore; core++)
	{
		SDEC_HAL_SetGPBBaseAddr(core, (memCfg->base & GPB_BASE_ADDR_MASK) >> 28);
	}

	SDEC_PRINT_TRACE( "-->SDEC_Gpb_Init");

	return RET_OK;
}

int SDEC_GpbSet
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8ch,
	LX_SDEC_GPB_SIZE_T eGpbSize,
	UINT32 ui32GpbBaseAddr,
	UINT32 ui32GpbIdx)
{
	int eRet = RET_ERROR;

	GPB_BND stGpbBnd;
	UINT32 ui32Lower_Bound = 0x0, ui32Upper_Bound = 0x0, ui32GpbSize = 0x0;
	LX_SDEC_CFG_T* 	pSdecConf 		= NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_GpbSet");

	/* get chip configuation */
	pSdecConf = SDEC_CFG_GetConfig();

	if( ui8ch >= pSdecConf->nChannel )
	{
		SDEC_PRINT_ERROR("over channel range %d", ui8ch);
		goto exit;
	}

	if( ui32GpbIdx >= pSdecConf->chInfo[ui8ch].num_secf )
	{
		SDEC_PRINT_ERROR("over GPB range %d", ui32GpbIdx);
		goto exit;
	}

	ui32GpbSize = eGpbSize * 0x1000;

	ui32Lower_Bound = ui32GpbBaseAddr & 0x0FFFF000;
	ui32Upper_Bound = ui32Lower_Bound + ui32GpbSize;

	/* set GPB boudary register */
	stGpbBnd.l_bnd = (ui32Lower_Bound >> 12) & 0x0000FFFF;
	stGpbBnd.u_bnd = (ui32Upper_Bound >> 12) & 0x0000FFFF;

	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		if((ui8ch < LX_SDEC_CH_G) && ( (ui8ch != LX_SDEC_CH_C) || (ui8ch != LX_SDEC_CH_D) ) )
		{
			//stpSdecParam->stSDEC_MPG_Reg[ui8ch]->gpb_bnd[ui32GpbIdx]= stGpbBnd;
			SDEC_HAL_GPBSetBnd(ui8ch, ui32GpbIdx, stGpbBnd.l_bnd, stGpbBnd.u_bnd);

			/* set GPB read & write pointer register */
			//stpSdecParam->stSDEC_MPG_Reg[ui8ch]->gpb_w_ptr[ui32GpbIdx].gpb_w_ptr =	ui32Lower_Bound;
			//stpSdecParam->stSDEC_MPG_Reg[ui8ch]->gpb_r_ptr[ui32GpbIdx].gpb_r_ptr =	ui32Lower_Bound;
			SDEC_HAL_GPBSetReadPtr(ui8ch, 	ui32GpbIdx, ui32Lower_Bound);
			SDEC_HAL_GPBSetWritePtr(ui8ch, 	ui32GpbIdx, ui32Lower_Bound);

			/* enable full intr */
			SDEC_HAL_GPBSetFullIntr(ui8ch, 	ui32GpbIdx);
		}
	}
	else
	{
		if(ui8ch < LX_SDEC_CH_C)
		{
			//stpSdecParam->stSDEC_MPG_Reg[ui8ch]->gpb_bnd[ui32GpbIdx]= stGpbBnd;
			SDEC_HAL_GPBSetBnd(ui8ch, ui32GpbIdx, stGpbBnd.l_bnd, stGpbBnd.u_bnd);

			/* set GPB read & write pointer register */
			//stpSdecParam->stSDEC_MPG_Reg[ui8ch]->gpb_w_ptr[ui32GpbIdx].gpb_w_ptr =	ui32Lower_Bound;
			//stpSdecParam->stSDEC_MPG_Reg[ui8ch]->gpb_r_ptr[ui32GpbIdx].gpb_r_ptr =	ui32Lower_Bound;
			SDEC_HAL_GPBSetReadPtr(ui8ch, 	ui32GpbIdx, ui32Lower_Bound);
			SDEC_HAL_GPBSetWritePtr(ui8ch, 	ui32GpbIdx, ui32Lower_Bound);

			/* enable full intr */
			SDEC_HAL_GPBSetFullIntr(ui8ch, 	ui32GpbIdx);
		}
	}

	/* initialize base & end pointer */
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].ui32Baseptr 		= ui32GpbBaseAddr;
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].ui32Endptr 		= ui32GpbBaseAddr + ui32GpbSize;
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].ui32Readptr 		= ui32GpbBaseAddr;
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].ui32UsrReadptr	= ui32GpbBaseAddr;
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].stGpbBnd 			= stGpbBnd;

	/* set to channel C section buffer */
	if(ui8ch == LX_SDEC_CH_C)
	{
		SDEC_SWP_SetSectionBuffer(stpSdecParam);
	}

	SDEC_PRINT_NORMAL( "l_bnd:[0x%08x]", ui32Lower_Bound);
	SDEC_PRINT_NORMAL( "u_bnd:[0x%08x]", ui32Upper_Bound);
	SDEC_PRINT_NORMAL( "gpb_w_ptr:[0x%08x]", ui32Lower_Bound);
	SDEC_PRINT_NORMAL( "gpb_r_ptr:[0x%08x]", ui32Lower_Bound);

	SDEC_PRINT_TRACE( "-->SDEC_GpbSet");

	eRet = RET_OK;

exit:
	return (eRet);
}

int SDEC_DummyGpbSet
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8ch,
	LX_SDEC_GPB_SIZE_T eGpbSize,
	UINT32 ui32GpbBaseAddr,
	UINT32 ui32GpbIdx)
{
	int eRet = RET_ERROR;

	GPB_BND stGpbBnd;
	UINT32 ui32Lower_Bound = 0x0, ui32Upper_Bound = 0x0, ui32GpbSize = 0x0;
	LX_SDEC_CFG_T* 	pSdecConf 		= NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_DummyGpbSet");

	/* get chip configuation */
	pSdecConf = SDEC_CFG_GetConfig();

	if( ui8ch >= pSdecConf->nChannel )
	{
		SDEC_PRINT_ERROR("over channel range %d", ui8ch);
		goto exit;
	}

	if( ui32GpbIdx >= 64 )
	{
		SDEC_PRINT_ERROR("over GPB range %d", ui32GpbIdx);
		goto exit;
	}

	ui32GpbSize = eGpbSize * 0x1000;

	ui32Lower_Bound = ui32GpbBaseAddr & 0x0FFFF000;
	ui32Upper_Bound = ui32Lower_Bound + ui32GpbSize;

	/* set GPB boudary register */
	stGpbBnd.l_bnd = (ui32Lower_Bound >> 12) & 0x0000FFFF;
	stGpbBnd.u_bnd = (ui32Upper_Bound >> 12) & 0x0000FFFF;

	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		if((ui8ch < LX_SDEC_CH_G) && ( (ui8ch != LX_SDEC_CH_C) && (ui8ch != LX_SDEC_CH_D) ) ) //srini
		{
			SDEC_HAL_GPBSetBnd(ui8ch, ui32GpbIdx, stGpbBnd.l_bnd, stGpbBnd.u_bnd);

			/* set GPB read & write pointer register */
			SDEC_HAL_GPBSetReadPtr(ui8ch, 	ui32GpbIdx, ui32Lower_Bound);
			SDEC_HAL_GPBSetWritePtr(ui8ch, 	ui32GpbIdx, ui32Lower_Bound);
		}
		else
		{
			goto exit;
		}
	}
	else
	{
		if(ui8ch < LX_SDEC_CH_C)
		{
			SDEC_HAL_GPBSetBnd(ui8ch, ui32GpbIdx, stGpbBnd.l_bnd, stGpbBnd.u_bnd);

			/* set GPB read & write pointer register */
			SDEC_HAL_GPBSetReadPtr(ui8ch, 	ui32GpbIdx, ui32Lower_Bound);
			SDEC_HAL_GPBSetWritePtr(ui8ch, 	ui32GpbIdx, ui32Lower_Bound);
		}
	}

	/* initialize base & end pointer */
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].ui32Baseptr 		= ui32GpbBaseAddr;
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].ui32Endptr 		= ui32GpbBaseAddr + ui32GpbSize;
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].ui32Readptr 		= ui32GpbBaseAddr;
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].ui32UsrReadptr	= ui32GpbBaseAddr;
	stpSdecParam->stSdecMeminfo[ui8ch][ui32GpbIdx].stGpbBnd 			= stGpbBnd;

	SDEC_PRINT_NORMAL( "l_bnd:[0x%08x]", ui32Lower_Bound);
	SDEC_PRINT_NORMAL( "u_bnd:[0x%08x]", ui32Upper_Bound);
	SDEC_PRINT_NORMAL( "gpb_w_ptr:[0x%08x]", ui32Lower_Bound);
	SDEC_PRINT_NORMAL( "gpb_r_ptr:[0x%08x]", ui32Lower_Bound);

	SDEC_PRINT_TRACE( "-->SDEC_DummyGpbSet");

	eRet = RET_OK;

exit:
	return (eRet);
}


int SDEC_InitHW(S_SDEC_PARAM_T *stpSdecParam)
{
	LX_SDEC_CFG_T* conf;
	int core;
	int baseCh;

	SDEC_PRINT_NOTI("SDEC_InitHW");

	conf = SDEC_CFG_GetConfig();

	SDEC_Gpb_Init();

	/* Use fixed 27Mhz clock */
	for(core=0; core<conf->nCore; core++)
	{
		SDEC_HAL_SetGSTCClkSource(core, 0, 1);
		SDEC_HAL_SetGSTCClkSource(core, 1, 1);
		SDEC_HAL_SetTimeStampClkSource(core, 1);
	}

	/* pwm reset */
	SDEC_PWM_Init(stpSdecParam);

	SDEC_PRINT_NOTI("PWM Init Done");

	//intr src set
	for(core=0; core<conf->nCore; core++)
	{
		SDEC_HAL_EnableInterrupt( core, PCR,				SDEC_HAL_ENABLE );
		SDEC_HAL_EnableInterrupt( core, GPB_DATA_CHA_GPL,	SDEC_HAL_ENABLE );
		SDEC_HAL_EnableInterrupt( core, GPB_DATA_CHA_GPH,	SDEC_HAL_ENABLE );
		SDEC_HAL_EnableInterrupt( core, GPB_DATA_CHB_GPL,	SDEC_HAL_ENABLE );
		SDEC_HAL_EnableInterrupt( core, GPB_DATA_CHB_GPH,	SDEC_HAL_ENABLE );

		/* 2012.02.06 gaius.lee
		 * Bug exist in L9 HW.
		 * While CPU access Read/Write/Bound Register, SDEC HW accesses write register, write pointer goes to read/write/bound regitser which CPU access.
		 * So, remove access to read register. That's why we disable full interrupt */
		if(conf->staticGPB == 0)
		{
			SDEC_HAL_EnableInterrupt( core, GPB_FULL_CHA_GPL,	SDEC_HAL_ENABLE );
			SDEC_HAL_EnableInterrupt( core, GPB_FULL_CHA_GPH,	SDEC_HAL_ENABLE );
			SDEC_HAL_EnableInterrupt( core, GPB_FULL_CHB_GPL,	SDEC_HAL_ENABLE );
			SDEC_HAL_EnableInterrupt( core, GPB_FULL_CHB_GPH,	SDEC_HAL_ENABLE );
		}
#if 1 // jinhwan.bae for MCU test
		SDEC_HAL_EnableInterrupt( core, TP_INFO_CHA,		SDEC_HAL_ENABLE );
		SDEC_HAL_EnableInterrupt( core, TP_INFO_CHB,		SDEC_HAL_ENABLE );
#else
		SDEC_HAL_EnableInterrupt( core, TP_INFO_CHA,		SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt( core, TP_INFO_CHB,		SDEC_HAL_DISABLE );
#endif
		SDEC_HAL_EnableInterrupt( core, ERR_RPT,			SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt( core, TB_DCOUNT, 		SDEC_HAL_ENABLE );

#if 1 // jinhwan.bae for BDRC Serial Out Buf Level Interrupt
		SDEC_HAL_EnableInterrupt( core, BDRC_3,			SDEC_HAL_ENABLE );
#endif

		/* jinhwan.bae 20140320 tvct problem */
		SDEC_HAL_EnableInterrupt( core, SEC_ERR_CHA,		SDEC_HAL_ENABLE );
		SDEC_HAL_EnableInterrupt( core, SEC_ERR_CHB,		SDEC_HAL_ENABLE );
	}

	for(core=0; core<conf->nCore; core++)
	{
		SDEC_HAL_EnableErrorInterrupt(core, MPG_SD,		0x3);
		SDEC_HAL_EnableErrorInterrupt(core, MPG_CC,		0x3);
		SDEC_HAL_EnableErrorInterrupt(core, MPG_DUP,	0x3);
		SDEC_HAL_EnableErrorInterrupt(core, MPG_TS,		0x3);
		SDEC_HAL_EnableErrorInterrupt(core, MPG_PD,		0x3);
		SDEC_HAL_EnableErrorInterrupt(core, STCC_DCONT,	0x3);
		SDEC_HAL_EnableErrorInterrupt(core, CDIF_RPAGE,	0x3);
		SDEC_HAL_EnableErrorInterrupt(core, CDIF_WPAGE,	0x3);
		SDEC_HAL_EnableErrorInterrupt(core, CDIF_OVFLOW,0x3);
		SDEC_HAL_EnableErrorInterrupt(core, SB_DROPPED,	0x3);
		SDEC_HAL_EnableErrorInterrupt(core, SYNC_LOST,	0x3);
		SDEC_HAL_EnableErrorInterrupt(core, TEST_DCONT,	0x1);
	}

	for(core=0; core<conf->nCore; core++)
	{
		baseCh = core*SDEC_CORE_CH_NUM;

		SDEC_HAL_SDMWCLastBValidMode(core, 0);

		/* Auto Increment Disable - jinhwan.bae from M14A0 2013. 06. 17 */
		SDEC_HAL_EnableAutoIncr(core, SDEC_HAL_DISABLE);

		SDEC_HAL_EnableVideoReady(core, 0, SDEC_HAL_DISABLE);
		SDEC_HAL_EnableVideoReady(core, 1, SDEC_HAL_DISABLE);
		SDEC_HAL_EnableAudioReady(core, 0, SDEC_HAL_DISABLE);
		SDEC_HAL_EnableAudioReady(core, 1, SDEC_HAL_DISABLE);

		/* Set gpb full level as 256 byte */
		SDEC_HAL_ExtConfGPBFullLevel(0 + baseCh, 1);
		SDEC_HAL_ExtConfGPBFullLevel(1 + baseCh, 1);

		/* M14_TBD, H14_TBD, pes ready is not working at M14B0, H14 */
		//SDEC_HAL_ConfSetPESReadyCheck(0 + baseCh, 0, SDEC_HAL_ENABLE);
		//SDEC_HAL_ConfSetPESReadyCheck(0 + baseCh, 1, SDEC_HAL_ENABLE);
		SDEC_HAL_ConfSetPESReadyCheck(1 + baseCh, 0, SDEC_HAL_ENABLE);
		SDEC_HAL_ConfSetPESReadyCheck(1 + baseCh, 1, SDEC_HAL_ENABLE);


		SDEC_HAL_ExtConfSECIDcont(0 + baseCh, SDEC_HAL_DISABLE);
		SDEC_HAL_ExtConfSECIDcont(1 + baseCh, SDEC_HAL_DISABLE);

		SDEC_HAL_ExtConfSECICCError(0 + baseCh, SDEC_HAL_DISABLE);
		SDEC_HAL_ExtConfSECICCError(1 + baseCh, SDEC_HAL_DISABLE);

		SDEC_HAL_ExtConfVideoDupPacket(0 + baseCh, SDEC_HAL_DISABLE);
		SDEC_HAL_ExtConfVideoDupPacket(1 + baseCh, SDEC_HAL_DISABLE);

#if 0
		/* 2013.02.07 Set from enable to disable. All Decoders should process Discontinuity Counter by theirselves
		   SarnOff ATSC_02.ts have discontinuity indicator in the first TS packet including PES header of I frame,
		   1st part of GOP is decoded unexpectedly (color tone is yellow , expected blue)
		   it's requested by VDEC */
		SDEC_HAL_ExtConfDcontDupPacket(0 + baseCh, SDEC_HAL_ENABLE);
		SDEC_HAL_ExtConfDcontDupPacket(1 + baseCh, SDEC_HAL_ENABLE);
#else
		SDEC_HAL_ExtConfDcontDupPacket(0 + baseCh, SDEC_HAL_DISABLE);
		SDEC_HAL_ExtConfDcontDupPacket(1 + baseCh, SDEC_HAL_DISABLE);
#endif
		SDEC_HAL_DSCEnablePESCramblingCtrl(core, 0, SDEC_HAL_DISABLE);
		SDEC_HAL_DSCEnablePESCramblingCtrl(core, 1, SDEC_HAL_DISABLE);
	}


	/* Set Bus Gate On in H14 only */
	// PVR Download is not working at H14 , bus gate blocked default! 2013. 07. 08
	// C0006060 : SDEC Core 0, C00006068 : PVR, C0006070 : SDEC Core 1
	// CHECK H15 more!!!!
	if (!LX_COMP_CHIP(lx_chip_rev(), LX_CHIP_H14) )
	{
		UINT32 vaddr = (UINT32)ioremap(0xC0006060, 0x20);
	    if (vaddr)
	    {
	    	SDEC_WRITE32(vaddr+0x08, 0x1);
			iounmap( (void*)vaddr );
	    }
		else
		{
			SDEC_PRINT_ERROR("ioremap error for bug gating");
		}
	}

	SDEC_PRINT_NOTI("HW Init Done");

	return 0;
}


int SDEC_CleanupHW(S_SDEC_PARAM_T *stpSdecParam)
{
	LX_SDEC_CFG_T* conf;
	int core;

	conf = SDEC_CFG_GetConfig();
	for(core=0; core<conf->nCore; core++)
	{
		//disable intr
		SDEC_HAL_EnableInterrupt(core, PCR, 				SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, GPB_DATA_CHA_GPL, 	SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, GPB_DATA_CHA_GPH, 	SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, GPB_DATA_CHB_GPL, 	SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, GPB_DATA_CHB_GPH, 	SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, GPB_FULL_CHA_GPL, 	SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, GPB_FULL_CHA_GPH, 	SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, GPB_FULL_CHB_GPL, 	SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, GPB_FULL_CHB_GPH, 	SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, TP_INFO_CHA, 		SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, TP_INFO_CHB, 		SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, ERR_RPT, 			SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, TB_DCOUNT, 			SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, BDRC_3, 				SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, SEC_ERR_CHA,			SDEC_HAL_DISABLE );
		SDEC_HAL_EnableInterrupt(core, SEC_ERR_CHB,			SDEC_HAL_DISABLE );
	}


	for(core=0; core<conf->nCore; core++)
	{
		SDEC_HAL_EnableErrorInterrupt(core, MPG_SD, 		0x0);
		SDEC_HAL_EnableErrorInterrupt(core, MPG_CC, 		0x0);
		SDEC_HAL_EnableErrorInterrupt(core, MPG_DUP, 		0x0);
		SDEC_HAL_EnableErrorInterrupt(core, MPG_TS, 		0x0);
		SDEC_HAL_EnableErrorInterrupt(core, MPG_PD, 		0x0);
		SDEC_HAL_EnableErrorInterrupt(core, STCC_DCONT, 	0x0);
		SDEC_HAL_EnableErrorInterrupt(core, CDIF_RPAGE, 	0x0);
		SDEC_HAL_EnableErrorInterrupt(core, CDIF_WPAGE, 	0x0);
		SDEC_HAL_EnableErrorInterrupt(core, CDIF_OVFLOW, 	0x0);
		SDEC_HAL_EnableErrorInterrupt(core, SB_DROPPED, 	0x0);
		SDEC_HAL_EnableErrorInterrupt(core, SYNC_LOST,		0x0);
		SDEC_HAL_EnableErrorInterrupt(core, TEST_DCONT, 	0x0);
	}

	return 0;
}


/**
********************************************************************************
* @brief
*   parameter init.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_Initialize(S_SDEC_PARAM_T *stpSdecParam)
{
	int eRet = RET_ERROR;
	int eResult = RET_ERROR;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_Initialize");

	/* initialize HAL api */
	SDEC_HAL_TOPInit();
	SDEC_HAL_IOInit();
	SDEC_HAL_MPGInit();

	SDEC_ParamInit(stpSdecParam);

	//mutex init
	eResult = SDEC_MutexInitialize(stpSdecParam);
	if(LX_IS_ERR(eResult))
	{
		SDEC_PRINT_ERROR("SDEC_MutexInitialize failed:[%d]", eResult);
		goto exit;
	}

	//work queue init
	eResult = SDEC_WorkQueueInitialize(stpSdecParam);
	if(LX_IS_ERR(eResult))
	{
		SDEC_PRINT_ERROR("SDEC_WorkQueueInitialize failed:[%d]", eResult);
		goto exit;
	}

	SDEC_PRINT_NORMAL( "SDEC_WorkQueueInitialize success");

	//spin lock init
	eResult = SDEC_SpinLockInitialize(stpSdecParam);
	if(LX_IS_ERR(eResult))
	{
		SDEC_PRINT_ERROR("SDEC_SpinLockInitialize failed:[%d]", eResult);
		goto exit;
	}

	SDEC_PRINT_NORMAL( "SDEC_SpinLockInitialize success");

	//wait queue init
	eResult = SDEC_WaitQueueInitialize(stpSdecParam);

	if(LX_IS_ERR(eResult))
	{
		SDEC_PRINT_ERROR("SDEC_WaitQueueInitialize failed:[%d]", eResult);
		goto exit;
	}

	SDEC_PRINT_NORMAL( "SDEC_WaitQueueInitialize success");

	SDEC_InitHW(stpSdecParam);

	SDEC_IO_Init(stpSdecParam);
	SDEC_CIPHER_Init(stpSdecParam);

//	SDEC_CIPLUS_Init();

	/* SDEC SW Parser Init */
	SDEC_SWP_Init(stpSdecParam);

	/* LIVE_HEVC */
	SDEC_RFOUT_Init(stpSdecParam);


	SDEC_PRINT_TRACE( "-->SDEC_Initialize");

	eRet = RET_OK;

exit:
	return (eRet);
}


/**
********************************************************************************
* @brief
*   sdec notify
* @remarks
*  wake up interrupt for signaling to user
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   work_struct
* @return
*  int
********************************************************************************
*/
void SDEC_Notify
	(struct work_struct *work)
{
	S_SDEC_PARAM_T *stpSdecParam;

	LX_SDEC_CHECK_CODE( work == NULL, return, "Invalid parameters" );

	stpSdecParam =  container_of (work, S_SDEC_PARAM_T, Notify);

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return, "Invalid parameters" );

	SDEC_PRINT_TRACE_EX( "<--SDEC_Notify");

	stpSdecParam->wq_condition = 1;
	wake_up_interruptible(&stpSdecParam->wq);

	SDEC_PRINT_TRACE_EX( "-->SDEC_Notify");
}

/**
********************************************************************************
* @brief
*   sdec notify
* @remarks
*  wake up interrupt for signaling to user
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   work_struct
* @return
*  int
********************************************************************************
*/
BOOLEAN SDEC_SetNotifyParam(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_NOTIFY_PARAM_T notiParam)
{
	BOOLEAN isFound = FALSE;
	unsigned long flags = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return isFound, "Invalid parameters" );

	SDEC_PRINT_TRACE_EX( "<--SDEC_SetNotifyParam");

	spin_lock_irqsave(&stpSdecParam->stSdecNotiSpinlock, flags);

	/*
	 * overflow check
	 *
	 * case 1 : one more packet, then overflow will happen.
	 *	  ui8GpbInfoWIdx
	 *			V
	 * |---------------------------------------------------------|
	 *			 ^
	 *	  ui8GpbInfoRIdx
	 *
	 * case 2 : read pointer is 0, write pointer is ( LX_SDEC_MAX_GPB_DATA - 1 ).
	 *		  if there is one more packet, write pointer is wrapped around and same with read pointer.
	 *												   ui8GpbInfoWIdx
	 *															 V
	 * |---------------------------------------------------------|
	 * ^
	 * ui8GpbInfoRIdx
	 */

	/* stored buf info	*/
	stpSdecParam->stGPBInfo[stpSdecParam->ui8GpbInfoWIdx] = notiParam;

	/* increase write pointer */
	stpSdecParam->ui8GpbInfoWIdx = (stpSdecParam->ui8GpbInfoWIdx + 1) % LX_SDEC_MAX_GPB_DATA;

	if( ( stpSdecParam->ui8GpbInfoRIdx - stpSdecParam->ui8GpbInfoWIdx) != 1 &&							// case 1.
		( stpSdecParam->ui8GpbInfoWIdx - stpSdecParam->ui8GpbInfoRIdx) != ( LX_SDEC_MAX_GPB_DATA - 1 )	// case 2.
		)
	{
		/* buffer is not full */

		/* set found flag */
		isFound = TRUE;
	}
	else
	{
		/* buffer is full, reset to 0 */
		stpSdecParam->ui8GpbInfoRIdx = 0;
		stpSdecParam->ui8GpbInfoWIdx = 0;
		SDEC_PRINT_ERROR( "Krdv->User Msg Queue is full");
	}

	spin_unlock_irqrestore(&stpSdecParam->stSdecNotiSpinlock, flags);

	SDEC_PRINT_TRACE_EX( "-->SDEC_SetNotifyParam");

	return isFound;
}

/**
********************************************************************************
* @brief
*   Check overflowed msg in msg queue from kdrv -> user level
* @remarks
*  check if this section or pes data is overflowed by overflow, and if, remove.
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   work_struct
* @return
*  int
********************************************************************************
*/
#if ( LX_SDEC_USE_GPB_OW == 1 )
void SDEC_CheckNotifyOvf(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_NOTIFY_PARAM_T *pNotiParam)
{
	unsigned long 	flags = 0;
	UINT8			ui8GpbInfoRIdx = 0, ui8GpbInfoWIdx = 0, ui8Ch = 0, ui8Idx = 0;
	UINT8			ui8DeleteMargin = 0xff;
	UINT32			rdPtr = 0, wrPtr = 0, bfStr = 0, bfEnd = 0;
	UINT32			urPtr = 0;				/* user read pointer */
	UINT32			cwPtr = 0;				/* current write pointer */
	SINT32			si32DataSize = 0;		/* size to delete */
	SINT32			si32SectionSize = 0;	/* size of a section */

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return , "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_CheckNotifyOvf");

	spin_lock_irqsave(&stpSdecParam->stSdecNotiSpinlock, flags);

	ui8Ch			= pNotiParam->channel;
	ui8Idx			= pNotiParam->index;

	ui8GpbInfoRIdx 	= stpSdecParam->ui8GpbInfoRIdx;
	ui8GpbInfoWIdx 	= stpSdecParam->ui8GpbInfoWIdx;

	bfStr			= stpSdecParam->stSdecMeminfo[ui8Ch][ui8Idx].ui32Baseptr;
	bfEnd			= stpSdecParam->stSdecMeminfo[ui8Ch][ui8Idx].ui32Endptr;

	SDEC_PRINT_PIDSEC( "SDEC_CheckNotifyOvf : CH%d[%d] e[%08x]\n",
				pNotiParam->channel,
				ui8Idx,
				pNotiParam->writePtr);

	/* while loop */
	while( ui8GpbInfoRIdx != ui8GpbInfoWIdx )
	{
		if( stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].channel == ui8Ch &&
			stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].index == ui8Idx &&
			stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].status & LX_SDEC_FLTSTATE_DATAREADY)
		{
			if(ui8Ch == LX_SDEC_CH_C)
			{
				stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].status = LX_SDEC_FLTSTATE_DELETED;

				/* increase user read pointer to section write pointer */
				stpSdecParam->stSdecMeminfo[ui8Ch][ui8Idx].ui32UsrReadptr = wrPtr;

				/* increase read pointer */
				ui8GpbInfoRIdx = (ui8GpbInfoRIdx + 1) % LX_SDEC_MAX_GPB_DATA;
				continue;
			}

			/* get section read pointer */
			rdPtr = stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].readPtr;
			/* get section write pointer */
			wrPtr = stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].writePtr;
			/* get user read pointer. this means real read pointer of user */
			//urPtr = SDEC_HAL_GPBGetReadPtr(ui8Ch, ui8Idx) 	| (bfStr & GPB_BASE_ADDR_MASK);
			urPtr = stpSdecParam->stSdecMeminfo[ui8Ch][ui8Idx].ui32UsrReadptr | (bfStr & GPB_BASE_ADDR_MASK);
			/* get current real-time write pointer */
			cwPtr = SDEC_HAL_GPBGetWritePtr(ui8Ch, ui8Idx) 	| (bfStr & GPB_BASE_ADDR_MASK);

			/* 1. calculate size to delete */
			if( ui8DeleteMargin == 0xff )
			{
				stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].status = LX_SDEC_FLTSTATE_DELETED;

				if( rdPtr <= cwPtr && cwPtr < wrPtr )
				{

					/* case 1. normal 1 ( probably in SDEC_Section_Proc(). But occasually in SDEC_Full_Proc() )
					 *
					 *             rdPtr      wrPtr
					 *              V          V
					 * |---------------------------------------------------------|
					 *                  ^
					 *	              cwPtr
					 */
					/* 정상적으로 current write pointer가 중간에 있다. */
					LX_MEMSIZE(si32DataSize, 		rdPtr, cwPtr, bfStr, bfEnd);
					LX_MEMSIZE(si32SectionSize, 	cwPtr, wrPtr, bfStr, bfEnd);
					ui8DeleteMargin = 2;
				}
				else if( rdPtr < cwPtr && wrPtr < cwPtr )
				{

					/* case 2. normal 2 ( probably in SDEC_Full_Proc() )
					 *
					 *             rdPtr      wrPtr
					 *              V          V
					 * |---------------------------------------------------------|
					 *                           ^
					 *	                       cwPtr
					 */
					/* current write pointer가 이 Table을 지나 있다. ( 많이 빠르므로 3개 정도 앞서서 삭제 )*/
					LX_MEMSIZE(si32DataSize, 		rdPtr, cwPtr, bfStr, bfEnd);
					LX_MEMSIZE(si32SectionSize, 	rdPtr, wrPtr, bfStr, bfEnd);
					ui8DeleteMargin = 3;
				}
				else
				{
					/* case 3. sometimes ( probably in SDEC_Full_Proc() )
					 *
					 *             rdPtr      wrPtr
					 *              V          V
					 * |---------------------------------------------------------|
					 *             ^
					 *	         cwPtr
					 */
					/* GPB_F_INTR 에서 가끔 이렇게 먼저 뜰 때가 있다. 이럴경우 그냥 앞에 있는 애들만 지워도 됨. */
					LX_MEMSIZE(si32DataSize, 		rdPtr, wrPtr, bfStr, bfEnd);
					LX_MEMSIZE(si32SectionSize, 	rdPtr, wrPtr, bfStr, bfEnd);
					ui8DeleteMargin = 1;
				}

				SDEC_PRINT_PIDSEC( "DS[%d] SS[%d] rp[%08x] wp[%08x] cw[%08x] ur[%08x] m[%d]\n",
					si32DataSize,
					si32SectionSize,
					rdPtr,
					wrPtr,
					pNotiParam->writePtr,
					urPtr,
					ui8DeleteMargin
					);

				/* decrease data size as section size */
				si32DataSize -= si32SectionSize;
				/* increase user read pointer to section write pointer */
				//SDEC_HAL_GPBSetReadPtr(ui8Ch, ui8Idx, wrPtr);
				stpSdecParam->stSdecMeminfo[ui8Ch][ui8Idx].ui32UsrReadptr = wrPtr;
			}
			/* 2. Delete while size is not under zero, or margin is not zero. */
			else if(si32DataSize > 0 || ui8DeleteMargin)
			{
				SDEC_PRINT_PIDSEC("[%d] [%d]\n", si32DataSize, ui8DeleteMargin);

				stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].status = LX_SDEC_FLTSTATE_DELETED;

				/* increase user read pointer to section write pointer */
				//SDEC_HAL_GPBSetReadPtr(ui8Ch, ui8Idx, wrPtr);
				stpSdecParam->stSdecMeminfo[ui8Ch][ui8Idx].ui32UsrReadptr = wrPtr;

				/* calculate section size */
				LX_MEMSIZE(si32SectionSize, rdPtr, wrPtr, bfStr, bfEnd);

				/* decrease data size as section size */
				si32DataSize -= si32SectionSize;

				/* if data size is under zero, delete more packet as margin. Sometimes data speed is too fast. */
				if(si32DataSize < 0) ui8DeleteMargin--;
			}
			else
			{
				break;
			}
		}

		/* increase read pointer */
		ui8GpbInfoRIdx = (ui8GpbInfoRIdx + 1) % LX_SDEC_MAX_GPB_DATA;
	}

	spin_unlock_irqrestore(&stpSdecParam->stSdecNotiSpinlock, flags);

	SDEC_PRINT_TRACE( "-->SDEC_CheckNotifyOvf");
}
#endif

/**
********************************************************************************
* @brief
*   sdec notify
* @remarks
*  wake up interrupt for signaling to user
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   work_struct
* @return
*  int
********************************************************************************
*/
void SDEC_DeleteInNotify(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch, UINT8 ui32SecFltId)
{
	UINT8	ui8GpbInfoRIdx = 0, ui8GpbInfoWIdx;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return , "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_CheckNotify");

	spin_lock(&stpSdecParam->stSdecNotiSpinlock);

	ui8GpbInfoRIdx 	= stpSdecParam->ui8GpbInfoRIdx;
	ui8GpbInfoWIdx 	= stpSdecParam->ui8GpbInfoWIdx;

	//printk("%s : CH%d[%d]\n", __FUNCTION__, ui8Ch, ui32SecFltId );

	/* while loop */
	while( ui8GpbInfoRIdx != ui8GpbInfoWIdx )
	{
		if( stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].channel == ui8Ch &&
			stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].index == ui32SecFltId &&
			stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].status & LX_SDEC_FLTSTATE_DATAREADY)
		{
			stpSdecParam->stGPBInfo[ui8GpbInfoRIdx].status = LX_SDEC_FLTSTATE_DELETED;

			printk("CH%d[%d] DELETE R[%d] W[%d]\n", ui8Ch, ui32SecFltId, ui8GpbInfoRIdx, ui8GpbInfoWIdx );
		}

		/* increase read pointer */
		ui8GpbInfoRIdx = (ui8GpbInfoRIdx + 1) % LX_SDEC_MAX_GPB_DATA;
	}

	//printk("%s : END\n", __FUNCTION__ );

	spin_unlock(&stpSdecParam->stSdecNotiSpinlock);

	SDEC_PRINT_TRACE( "-->SDEC_CheckNotify");
}


/**
********************************************************************************
* @brief
*   get Chip Configuration
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
int SDEC_IO_GetChipCfg
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_CHIP_CFG_T *stpLXSdecGetChipCfg)
{
	int eRet = RET_ERROR;
	LX_SDEC_CFG_T* pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecGetChipCfg == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetChipCfg");

	/* get config */
	pSdecConf = SDEC_CFG_GetConfig();

	stpLXSdecGetChipCfg->nCore			= pSdecConf->nCore;
	stpLXSdecGetChipCfg->nChannel 		= pSdecConf->nChannel;
	stpLXSdecGetChipCfg->nVdecOutPort 	= pSdecConf->nVdecOutPort;
	stpLXSdecGetChipCfg->noPesBug 		= pSdecConf->noPesBug;
	stpLXSdecGetChipCfg->staticGPB 		= pSdecConf->staticGPB;
	stpLXSdecGetChipCfg->inTsSelector	= pSdecConf->inTsSelector;
	stpLXSdecGetChipCfg->ciPlus			= pSdecConf->ciPlus;

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetChipCfg");

	eRet = RET_OK;

	return (eRet);
}


/**
********************************************************************************
* @brief
*   enable log.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui32Arg :arguments from userspace
*   stpVdecParam :VDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_IO_GetGPBBaseOffset
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GPB_BASE_OFFSET_T *stpLXSdecGPBBaseOffset)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT8 core = 0x0;
	UINT32 gpb_base_offset = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecGPBBaseOffset == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetGPBBaseOffset");

	ui8Ch = (UINT8)stpLXSdecGPBBaseOffset->eCh;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);

	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	gpb_base_offset = SDEC_HAL_GetGPBBaseAddr(core);
	stpLXSdecGPBBaseOffset->uiGPBBaseOffset = (gpb_base_offset & 0xF0000000);

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetGPBBaseOffset");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   enable log.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui32Arg :arguments from userspace
*   stpVdecParam :VDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_IO_EnableLog(S_SDEC_PARAM_T *stpSdecParam, UINT32 *pulArg)
{
	int eRet = RET_ERROR;
	int idx = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( pulArg == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_EnableLog");

	stpSdecParam->ui32MsgMask = *pulArg;

	for (idx = 0; idx < LX_MAX_MODULE_DEBUG_NUM; idx++)
	{
		if ( *pulArg & (1 << idx) ) OS_DEBUG_EnableModuleByIndex ( g_sdec_debug_fd, idx, DBG_COLOR_NONE );
		else						OS_DEBUG_DisableModuleByIndex( g_sdec_debug_fd, idx);
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_EnableLog");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


