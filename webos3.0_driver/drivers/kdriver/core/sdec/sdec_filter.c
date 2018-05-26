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
#include "sdec_drv.h"
#include "sdec_io.h"
#include "sdec_pes.h"
#include "sdec_hal.h"
#include "sdec_swparser.h"
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
 *	 pid filter clear.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	  ui8PidfIdx :pid idx
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_Pidf_Clear
	 (S_SDEC_PARAM_T *stpSdecParam,
	 UINT8 ui8ch,
	 UINT32 ui8PidfIdx)
 {
	 int	 eRet = RET_ERROR;
	 UINT8			 ui8Pidf_idx = 0x0;
	 UINT8			 ui8PidfNum = 0x0;
	 LX_SDEC_CFG_T*  pSdecConf = NULL;
	 UINT8			 core = 0, org_ch = 0;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	 SDEC_PRINT_TRACE( "<--SDEC_Pidf_Clear");

	 /* get chip configuation */
	 pSdecConf = SDEC_CFG_GetConfig();

	 /* get pid filter number from channel info structure */
	 ui8PidfNum  = pSdecConf->chInfo[ui8ch].num_pidf;

	 /* if channel doesn't have pid filter, return error */
	 if( ui8PidfNum == 0 )
	 {
		 SDEC_PRINT_ERROR("this channel [%d] doesn't have pid filter !!!!", ui8ch);
		 eRet = RET_OK;
		 goto exit;
	 }

	 org_ch = ui8ch;
	 SDEC_CONVERT_CORE_CH(core, ui8ch);

	 if( pSdecConf->chInfo[org_ch].capa_lev == 0 )
	 {
		 if(ui8PidfIdx == CLEAR_ALL_MODE)
		 {
			 //clear all pid filter
			 for (ui8Pidf_idx = 0x0; ui8Pidf_idx < ui8PidfNum; ui8Pidf_idx++)
			 {
				 SDEC_HAL_CDIC2PIDFSetPidfData(core, ui8Pidf_idx, 0x1FFF);
			 }
		 }
		 else if (ui8PidfIdx < ui8PidfNum )
		 {
			 SDEC_HAL_CDIC2PIDFSetPidfData(core, ui8PidfIdx, 0x1FFF);
		 }

		 eRet = RET_OK;
		 goto exit;
	 }

	 if(ui8PidfIdx == CLEAR_ALL_MODE)
	 {
		 //clear all pid filter
		 for (ui8Pidf_idx = 0x0; ui8Pidf_idx < ui8PidfNum; ui8Pidf_idx++)
		 {
			 SDEC_SetPidfData(stpSdecParam, org_ch, ui8Pidf_idx, 0x1FFF0000);
			 SDEC_HAL_SECFSetMap(org_ch, ui8Pidf_idx * 2, 0);
			 SDEC_HAL_SECFSetMap(org_ch, ui8Pidf_idx * 2 + 1, 0);
		 }
		 SDEC_HAL_CCCheckEnableSet(org_ch, 0, 0);
		 SDEC_HAL_CCCheckEnableSet(org_ch, 1, 0);
	 }
	 else if (ui8PidfIdx < ui8PidfNum )
	 {
	 	UINT32 val, idx, bit;

		SDEC_SetPidfData(stpSdecParam, org_ch, ui8PidfIdx, 0x1FFF0000);
		SDEC_HAL_SECFSetMap(org_ch, ui8PidfIdx * 2, 0);
		SDEC_HAL_SECFSetMap(org_ch, ui8PidfIdx * 2 + 1, 0);

		idx = (ui8PidfIdx < 32) ? 0 : 1;
		bit = 1 << (ui8PidfIdx % 32);

		val = SDEC_HAL_CCCheckEnableGet(org_ch, idx);
		val &= ~bit;
		SDEC_HAL_CCCheckEnableSet(org_ch, idx, val);
	 }

	 SDEC_PRINT_TRACE( "-->SDEC_Pidf_Clear");

	 eRet = RET_OK;

 exit:

	 return (eRet);
 }

 /**
 ********************************************************************************
 * @brief
 *	 section filter clear.
 * @remarks
 *	DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	 ui8PidfIdx :pid idx
 *	 stpSdecParam :SDEC parameter
 * @return
 *	int
 ********************************************************************************
 */
 int SDEC_Secf_Clear
	 (S_SDEC_PARAM_T *stpSdecParam,
	 UINT8 ui8ch,
	 UINT32 ui8SecfIdx)
 {
	 int	 eRet = RET_ERROR;
	 UINT8			 ui8Secf_idx = 0x0, ui8Word_idx = 0x0;
	 UINT8			 ui8SecfNum  = 0x0;
	 LX_SDEC_CFG_T*  pSdecConf		 = NULL;

	 LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	 SDEC_PRINT_TRACE( "<--SDEC_Secf_Clear");

	 /* get chip configuation */
	 pSdecConf = SDEC_CFG_GetConfig();

	 /* get section filter number from channel info structure */
	 ui8SecfNum  = pSdecConf->chInfo[ui8ch].num_secf;

	 /* if channel doesn't have secction filter, return error */
	 if( ui8SecfNum == 0 )
	 {
		 SDEC_PRINT_ERROR("this channel doesn't have section filter !!!!");
		 eRet = RET_OK;
		 goto exit;
	 }

	 /* if pid filter is just simple filter, set ts2pes filter only */
	 if( pSdecConf->chInfo[ui8ch].capa_lev == 0 )
	 {
		 eRet = RET_OK;
		 goto exit;
	 }

	 //auto increment disable
	 //SDEC_Enable_AutoIncr(stpSdecParam, 0);
 //  SDEC_HAL_EnableAutoIncr(SDEC_HAL_DISABLE);

	 if(ui8SecfIdx == CLEAR_ALL_MODE)
	 {
		 //clear all section filter
		 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secf_en[0].secf_en = 0x0;
		 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secf_en[1].secf_en = 0x0;
		 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secfb_valid[0].secfb_valid = 0x0;
		 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secfb_valid[1].secfb_valid = 0x0;
		 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secf_mtype[0].secf_mtype = 0x0;
		 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secf_mtype[1].secf_mtype = 0x0;
		 SDEC_HAL_SECFSetEnable(ui8ch, 0, 0);
		 SDEC_HAL_SECFSetEnable(ui8ch, 1, 0);
		 SDEC_HAL_SECFSetBufValid(ui8ch, 0, 0);
		 SDEC_HAL_SECFSetBufValid(ui8ch, 1, 0);
		 SDEC_HAL_SECFSetMapType(ui8ch, 0, 0);
		 SDEC_HAL_SECFSetMapType(ui8ch, 1, 0);

		 for (ui8Secf_idx = 0x0; ui8Secf_idx < ui8SecfNum; ui8Secf_idx++)
		 {

			 for (ui8Word_idx = 0x0; ui8Word_idx < 7; ui8Word_idx++)
			 {
				 //secf_addr.secf_idx = ui8Secf_idx;
				 //secf_addr.word_idx = ui8Word_idx;
				 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secf_addr = secf_addr;
				 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secf_data.secf_data = 0xAA000000;
				 SDEC_HAL_SECFSetSecfData(ui8ch, ui8Secf_idx, ui8Word_idx, 0xAA000000);
			 }
		 }

	 /* For PES H/W bug workaound. See @LX_SDEC_USE_KTHREAD_PES */
	#if ( LX_SDEC_USE_KTHREAD_PES == 1)
		 if(pSdecConf->noPesBug == 0)
		 {
			 /* if there is pes h/w buf, do it */
			 SDEC_PES_AllClearPESFlt(ui8ch);
		 }
	#endif

	 }
	 else if ( ui8SecfIdx < ui8SecfNum )
	 {
		 //clear only one section filter

		 //clear__bit(stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secf_en[ui8IsHigh].secf_en, (ui8SecfIdx%32));
		 //clear__bit(stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secfb_valid[ui8IsHigh].secfb_valid, (ui8SecfIdx % 32));
		 SDEC_HAL_SECFClearEnableBit(ui8ch, ui8SecfIdx);
		 SDEC_HAL_SECFClearBufValidBit(ui8ch, ui8SecfIdx);
		 for (ui8Word_idx = 0x0; ui8Word_idx < 7; ui8Word_idx++)
		 {
			 //secf_addr.secf_idx = ui8SecfIdx;
			 //secf_addr.word_idx = ui8Word_idx;
			 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secf_addr = secf_addr;
			 //stpSdecParam->stSDEC_MPG_Reg[ui8ch]->secf_data.secf_data = 0xAA000000;
			 SDEC_HAL_SECFSetSecfData(ui8ch, ui8SecfIdx, ui8Word_idx, 0xAA000000);
		 }
	 /* For PES H/W bug workaound. See @LX_SDEC_USE_KTHREAD_PES */
#if ( LX_SDEC_USE_KTHREAD_PES == 1)
		 if(pSdecConf->noPesBug == 0)
		 {
			 /* if there is pes h/w buf, do it */
			 SDEC_PES_ClearPESFlt(ui8ch, ui8SecfIdx);
		 }
#endif
	 }

	 //auto increment enable
	 //SDEC_Enable_AutoIncr(stpSdecParam, 1);
 //  SDEC_HAL_EnableAutoIncr(SDEC_HAL_ENABLE);

	 SDEC_PRINT_TRACE( "-->SDEC_Secf_Clear");

	 eRet = RET_OK;

 exit:
	 return (eRet);
}


/**
********************************************************************************
* @brief
*   sdec TPI_IEN in PIDF_DATA register set
* @remarks
*   DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   ui8Ch :channel
*   ui8PIDIdx : PID filter index
*   val : PIDF_DATA value
*   work_struct
* @return
*  int
********************************************************************************
*/

int SDEC_SetPidfData
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8Ch,
	UINT8 ui8PIDIdx,
	UINT32 val
	)
{
	int ret = RET_ERROR;
	unsigned long 		flags = 0;

	spin_lock_irqsave(&stpSdecParam->stSdecPidfSpinlock, flags);

	SDEC_PRINT_NORMAL("Set PIDF_DATA ch[%d]idx[%d][0x%x]", ui8Ch, ui8PIDIdx, val);
	ret = SDEC_HAL_PIDFSetPidfData(ui8Ch, ui8PIDIdx, val);

	spin_unlock_irqrestore(&stpSdecParam->stSdecPidfSpinlock, flags);

	return ret;

}


/**
********************************************************************************
* @brief
*   sdec TPI_IEN in PIDF_DATA register get
* @remarks
*   DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   ui8Ch :channel
*   ui8PIDIdx : PID filter index
*   work_struct
* @return
*  PIDF_DATA value
********************************************************************************
*/

UINT32 SDEC_GetPidfData
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8Ch,
	UINT8 ui8PIDIdx
	)
{
	UINT32 ret_value = 0;
	unsigned long 		flags = 0;

	spin_lock_irqsave(&stpSdecParam->stSdecPidfSpinlock, flags);

	ret_value = SDEC_HAL_PIDFGetPidfData(ui8Ch, ui8PIDIdx);
	SDEC_PRINT_NORMAL("Get PIDF_DATA ch[%d]idx[%d][0x%x]", ui8Ch, ui8PIDIdx, ret_value);

	spin_unlock_irqrestore(&stpSdecParam->stSdecPidfSpinlock, flags);

	return ret_value;
}


/**
********************************************************************************
* @brief
*   sdec PIDF enable
* @remarks
*   DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   ui8Ch :channel
*   ui8PIDIdx : PID filter index
*   en : enable/disable
*   work_struct
* @return
*  int
********************************************************************************
*/

int SDEC_SetPidf_Enable
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8Ch,
	UINT8 ui8PIDIdx,
	UINT8 en
	)
{
	int ret = RET_ERROR;
	unsigned long 		flags = 0;

	spin_lock_irqsave(&stpSdecParam->stSdecPidfSpinlock, flags);

	SDEC_PRINT_NORMAL("PIDF_EN ch[%d]idx[%d][%d]", ui8Ch, ui8PIDIdx, en);
	ret = SDEC_HAL_PIDFEnable(ui8Ch, ui8PIDIdx, en);

	spin_unlock_irqrestore(&stpSdecParam->stSdecPidfSpinlock, flags);

	return ret;
}


/**
********************************************************************************
* @brief
*   sdec TPI_IEN in PIDF_DATA register set
* @remarks
*   DETAIL INFORMATION
*   Added by jinhwan.bae, 2012.04.26, No Audio at long time aging test with US/Cable
*   US/Cable Check Scramble Status periodically, Scramble Check is used pusi interrupt -> check tsc
*   Process Context : start scramble check -> write TPI_IEN in Audio PIDF_DATA
*   Interrupt Context : Video pusi interrupt -> check tsc -> disable scramble check (since H/W Bug) -> write TPI_IEN in Video PIDF_DATA
*   If interrupt context write TPI_IEN during process context stopped just after writing PIDF_INDEX,
*   PIDF_DATA in process context after interrupt context operation is changed to Video PIDF_DATA,
*   so Video PIDF_DATA | TPI_IEN will be writen to Audio PIDF_DATA
*   all added pidf spin lock codes are related to this
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   ui8Ch :channel
*   ui8PIDIdx : PID filter index
*   en : Enable or Disable
*   work_struct
* @return
*  int
********************************************************************************
*/

int SDEC_SetPidf_TPI_IEN_Enable
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8Ch,
	UINT8 ui8PIDIdx,
	UINT8 en
	)
{
	int ret = RET_ERROR;
	unsigned long 		flags = 0;

	spin_lock_irqsave(&stpSdecParam->stSdecPidfSpinlock, flags);

	SDEC_PRINT_NORMAL("TPI_IEN idx[%d][%d][%d]", ui8Ch, ui8PIDIdx, en);
	ret = SDEC_HAL_PIDFScrambleCheck(ui8Ch, ui8PIDIdx, en);

	spin_unlock_irqrestore(&stpSdecParam->stSdecPidfSpinlock, flags);

	return ret;
}




/**
********************************************************************************
* @brief
*   sdec PIDF Descrambler enable
* @remarks
*   DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   ui8Ch :channel
*   ui8PIDIdx : PID filter index
*   en : enable/disable
*   work_struct
* @return
*  int
********************************************************************************
*/

int SDEC_SetPidf_Disc_Enable
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8Ch,
	UINT8 ui8PIDIdx,
	UINT8 en
	)
{
	int ret = RET_ERROR;
	unsigned long 		flags = 0;

	spin_lock_irqsave(&stpSdecParam->stSdecPidfSpinlock, flags);

	SDEC_PRINT_NORMAL("PIDF_DESC_EN ch[%d]idx[%d][%d]", ui8Ch, ui8PIDIdx, en);
	ret = SDEC_HAL_PIDFDescEnable(ui8Ch, ui8PIDIdx, en);

	spin_unlock_irqrestore(&stpSdecParam->stSdecPidfSpinlock, flags);

	return ret;
}


/**
********************************************************************************
* @brief
*   sdec PIDF Download enable
* @remarks
*   DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   ui8Ch :channel
*   ui8PIDIdx : PID filter index
*   en : enable/disable
*   work_struct
* @return
*  int
********************************************************************************
*/

int SDEC_SetPidf_DN_Enable
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8Ch,
	UINT8 ui8PIDIdx,
	UINT8 en
	)
{
	int ret = RET_ERROR;
	unsigned long 		flags = 0;

	spin_lock_irqsave(&stpSdecParam->stSdecPidfSpinlock, flags);

	SDEC_PRINT_NORMAL("PIDF_DN_EN ch[%d]idx[%d][%d]", ui8Ch, ui8PIDIdx, en);
	ret = SDEC_HAL_PIDFDownloadEnable(ui8Ch, ui8PIDIdx, en);

	spin_unlock_irqrestore(&stpSdecParam->stSdecPidfSpinlock, flags);

	return ret;
}



/**
********************************************************************************
* @brief
*   SDEC core input port setting.
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
int SDEC_IO_PIDFilterAlloc
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_ALLOC_T *stpLXSdecPIDFltAlloc)
{
	int eRet = RET_ERROR, eResult = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT8 ui8PidIdx = 0x0;
	UINT16 ui16Pid = 0x0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltAlloc == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PIDFilterAlloc");

	ui8Ch 	= stpLXSdecPIDFltAlloc->eCh;
	ui16Pid	= stpLXSdecPIDFltAlloc->uiPidValue;

	eResult = SDEC_PIDIdxCheck(stpSdecParam, ui8Ch, &ui8PidIdx, stpLXSdecPIDFltAlloc->ePidFltMode, ui16Pid);
	LX_SDEC_CHECK_CODE( LX_IS_ERR(eResult), goto exit, "SDEC_SelPidFilterIdx failed:[%d]", eResult);

	SDEC_PRINT_PIDSEC( "Ch[%d] returned PID Idx:[%d]", ui8Ch, ui8PidIdx);

	stpLXSdecPIDFltAlloc->uiPidFltId = ui8PidIdx;

	SDEC_PRINT_TRACE( "-->SDEC_IO_PIDFilterAlloc");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   SDEC PID idx check.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui8pPidIdx :return pid idx
*   stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_PIDIdxCheck
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8ch,
	UINT8 *ui8pPidIdx,
	LX_SDEC_PIDFLT_MODE_T ePidFltMode,
	UINT16 ui16PidValue )
{
	int 	eRet 			= RET_ERROR;
	UINT8 			ui8Count 		= 0x0;
	UINT8 			ui8PidfNum		= 0x0;
	UINT8 			ui8FltMode 	= 0x0;
	BOOLEAN 		bFind		 	= FALSE;
	UINT32			ui32PidfData	= 0x0;
	LX_SDEC_CFG_T* 	pSdecConf 		= NULL;
	UINT8			core = 0, org_ch = ui8ch;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_PIDIdxCheck");

	SDEC_CONVERT_CORE_CH(core, ui8ch);

	/* get chip configuation */
	pSdecConf = SDEC_CFG_GetConfig();

	ui8FltMode = ePidFltMode;

	if( org_ch >= pSdecConf->nChannel )
	{
		SDEC_PRINT_ERROR("over channel range %d", org_ch);
		goto exit;
	}

	/* get pid filter number from channel info structure */
	ui8PidfNum = pSdecConf->chInfo[org_ch].num_pidf;

	/* Check CH_C */
	if( pSdecConf->chInfo[org_ch].capa_lev == 0)
	{
		for(ui8Count = 0; ui8Count < ui8PidfNum; ui8Count++)
		{

			/* Check if there is same pid filter */
			ui32PidfData = SDEC_HAL_CDIC2PIDFGetPidfData(core, ui8Count);
			if( ui16PidValue == ( ui32PidfData & 0x1FFF ) )
			{
				SDEC_PRINT_ERROR( "Same PID is exist!!!! pidval[%04x] idx[%2d]", ui16PidValue, ui8Count );

				/* if there is same pid filter, clear!! */
				SDEC_Pidf_Clear(stpSdecParam, org_ch, ui8Count);
				stpSdecParam->stPIDMap[org_ch][ui8Count].used = 0x0;
				stpSdecParam->stPIDMap[org_ch][ui8Count].flag = FALSE;
				stpSdecParam->stPIDMap[org_ch][ui8Count].mode = 0x0;
				stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_FREE = 0x0;
				stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_ENABLE = 0x0;
				stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_OVERFLOW = 0x0;
				stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_ALLOC = 0x0;
			}

			if( (stpSdecParam->stPIDMap[org_ch][ui8Count].used) == 0x0)
			{
				stpSdecParam->stPIDMap[org_ch][ui8Count].used = 0x1;
				stpSdecParam->stPIDMap[org_ch][ui8Count].mode = ui8FltMode;
				stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_ALLOC = 0x1;
				*ui8pPidIdx = ui8Count;
				bFind = TRUE;

				break;
			}
		}

		goto check_find;
	}

	for(ui8Count = 0; ui8Count < ui8PidfNum; ui8Count++)
	{

		ui32PidfData = SDEC_GetPidfData(stpSdecParam, org_ch, ui8Count);

		if( ui16PidValue == ( ( ui32PidfData >> 16 ) & 0x1FFF) )
		{
			SDEC_PRINT_ERROR( "Same PID is exist!!!! pidval[%04x] idx[%2d]", ui16PidValue, ui8Count );

			/* if there is same pid filter, clear!! */
			SDEC_Pidf_Clear(stpSdecParam, org_ch, ui8Count);
			stpSdecParam->stPIDMap[org_ch][ui8Count].used = 0x0;
			stpSdecParam->stPIDMap[org_ch][ui8Count].flag = FALSE;
			stpSdecParam->stPIDMap[org_ch][ui8Count].mode = 0x0;
			stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_FREE = 0x0;
			stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_ENABLE = 0x0;
			stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_OVERFLOW = 0x0;
			stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_ALLOC = 0x0;
		}

		if( (stpSdecParam->stPIDMap[org_ch][ui8Count].used) == 0x0)
		{
			stpSdecParam->stPIDMap[org_ch][ui8Count].used = 0x1;
			stpSdecParam->stPIDMap[org_ch][ui8Count].mode = ui8FltMode;
			stpSdecParam->stPIDMap[org_ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_ALLOC = 0x1;
			*ui8pPidIdx = ui8Count;
			bFind = TRUE;

			break;
		}
	}

check_find:
	if(bFind == FALSE )
	{
		SDEC_PRINT_ERROR( RED_COLOR"Ch[%d] PID filter alloc failed"NORMAL_COLOR, org_ch);
		goto exit;
	}

	SDEC_PRINT_TRACE( "-->SDEC_PIDIdxCheck");

	eRet = RET_OK;

exit:
	return (eRet);
}

/**
********************************************************************************
* @brief
*   SDEC Sec idx check.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui8pPidIdx :return Sec idx
*   stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_SecIdxCheck
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8ch,
	UINT8 *ui8pSecIdx)
{
	int 	eRet = RET_ERROR;

	UINT8 			ui8SecfNum		= 0x0;
	UINT8 			ui8Count 		= 0x0;
	BOOLEAN 		bBufFullFlag 	= FALSE;
	LX_SDEC_CFG_T* 	pSdecConf 		= NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_SecIdxCheck");

	/* get chip configuation */
	pSdecConf = SDEC_CFG_GetConfig();

	if( ui8ch >= pSdecConf->nChannel )
	{
		SDEC_PRINT_ERROR("over channel range %d", ui8ch);
		goto exit;
	}

	/* get section filter number from channel info structure */
	ui8SecfNum = pSdecConf->chInfo[ui8ch].num_secf;

	if( ui8ch < LX_SDEC_CH_C )
	{
		/* gaius.lee 2011.06.08
		 * set this because of problem in L8 & L9 A0
		 * there is h/w issue in GPB0, so we shall not use GPB0
		 */
		//if (lx_chip_rev() <= LX_CHIP_REV(L9, A1)) 	ui8Count = 1;
		//else											ui8Count = 0;
		/* gaius.lee 2012.01.05
		 * upper issue is still in L9 B0
		 */
		ui8Count = 1;

		//sec filter map check
		for(; ui8Count < ui8SecfNum; ui8Count++)
		{
			if((stpSdecParam->stSecMap[ui8ch][ui8Count].used) == 0x0)
			{
				stpSdecParam->stSecMap[ui8ch][ui8Count].used = 0x1;
				//empty GPB idx set
				//stpSdecParam->stSecMap[ui8ch][ui8Count].flag = TRUE;
				stpSdecParam->stSecMap[ui8ch][ui8Count].stStatusInfo.f.SDEC_FLTSTATE_ALLOC = 0x1;

				*ui8pSecIdx = ui8Count;

				bBufFullFlag = TRUE;

				break;
			}
		}
	}
	else
	{
		stpSdecParam->stSecMap[ui8ch][0].used = 0x1;
		stpSdecParam->stSecMap[ui8ch][0].stStatusInfo.f.SDEC_FLTSTATE_ALLOC = 0x1;
		*ui8pSecIdx = 0;
		bBufFullFlag = TRUE;
	}

	//allock fail, buffer full
	if(bBufFullFlag  != TRUE)
	{
		SDEC_PRINT_ERROR( RED_COLOR"Ch[%d] Section  filter allock failed"NORMAL_COLOR, ui8ch);
		goto exit;
	}

	SDEC_PRINT_TRACE( "-->SDEC_SecIdxCheck");

	eRet = RET_OK;

exit:
	return (eRet);
}


/**
********************************************************************************
* @brief
*   SDEC Check Available Section Filter Number
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui8pPidIdx :return Sec idx
*   stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_IO_SectionFilterGetAvailableNumber
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_GET_AVAIL_NUMBER_T *stpLXSdecSecFltAvailNum)
{
	int 	eRet = RET_ERROR;
	UINT8 			ui8SecfNum		= 0x0;
	UINT8 			ui8Count 		= 0x0;
	LX_SDEC_CFG_T* 	pSdecConf 		= NULL;
	UINT8 			ui8Ch = 0x0;
	UINT32 			uiAvailFilter = 0x0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecFltAvailNum == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterGetAvailableNumber");

	ui8Ch = (UINT8)stpLXSdecSecFltAvailNum->eCh;

	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);

	/* get section filter number from channel info structure */
	ui8SecfNum = pSdecConf->chInfo[ui8Ch].num_secf;

	if( SDEC_IS_NORMAL_CHANNEL((LX_SDEC_CH_T)ui8Ch) )
	{
		/* gaius.lee 2011.06.08
		 * set this because of problem in L8 & L9 A0
		 * there is h/w issue in GPB0, so we shall not use GPB0
		 */
		ui8Count = 1;

		/* check section filter map */
		for(; ui8Count < ui8SecfNum; ui8Count++)
		{
			if((stpSdecParam->stSecMap[ui8Ch][ui8Count].used) == 0x0)
			{
				uiAvailFilter++;
			}
		}
	}
	else if( ( (LX_SDEC_CH_T)ui8Ch == LX_SDEC_CH_C) || ((LX_SDEC_CH_T)ui8Ch == LX_SDEC_CH_G) )
	{
		uiAvailFilter = 1;
	}
	else
	{
		/* CH_D, CH_H case */
		uiAvailFilter = 0;
	}

	/* set return value */
	stpLXSdecSecFltAvailNum->uiAvailFilter = uiAvailFilter;

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterGetAvailableNumber");
	eRet = RET_OK;

exit:
	return (eRet);
}


/**
********************************************************************************
* @brief
*   SDEC PID filter free.
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
int SDEC_IO_PIDFilterFree
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_FREE_T *stpLXSdecPIDFltFree)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidFltId = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltFree == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PIDFilterFree");

	ui8Ch = stpLXSdecPIDFltFree->eCh;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);

	ui32PidFltId = stpLXSdecPIDFltFree->uiPidFltId;
	LX_SDEC_CHECK_CODE( ui32PidFltId >= pSdecConf->chInfo[ui8Ch].num_pidf, goto exit, "over pid filter range %d", ui32PidFltId);

	SDEC_PRINT_PIDSEC( "Ch[%d] PidFltId[%d] Free", ui8Ch, ui32PidFltId);

	SDEC_Pidf_Clear(stpSdecParam, ui8Ch, ui32PidFltId);

	//PID Map init
	stpSdecParam->stPIDMap[ui8Ch][ui32PidFltId].used = 0x0;
	stpSdecParam->stPIDMap[ui8Ch][ui32PidFltId].flag = FALSE;
	stpSdecParam->stPIDMap[ui8Ch][ui32PidFltId].mode = 0x0;
	stpSdecParam->stPIDMap[ui8Ch][ui32PidFltId].stStatusInfo.w = 0x0;
	stpSdecParam->stPIDMap[ui8Ch][ui32PidFltId].stStatusInfo.f.SDEC_FLTSTATE_ALLOC = 0x0;

	SDEC_PRINT_TRACE( "-->SDEC_IO_PIDFilterFree");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   Decide Packet is Clear or Not
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui32PidValue : PID
* @return
*   TRUE or FALSE
********************************************************************************
*/
static BOOLEAN _SDEC_Is_JPClearPacket(UINT32 ui32PidValue)
{
	/* If PAT and DVB-SI Table, it's clear in JP - get the confirmation from Tokyo Lab. */
	if( (ui32PidValue == 0x00) || (ui32PidValue == 0x01) || (ui32PidValue == 0x02) || (ui32PidValue == 0x10)
		|| (ui32PidValue == 0x11) || (ui32PidValue == 0x12) || (ui32PidValue == 0x13) || (ui32PidValue == 0x14)
		|| (ui32PidValue == 0x16) || (ui32PidValue == 0x1E) || (ui32PidValue == 0x1F) )
	{
		return TRUE;
	}

	return FALSE;
}


/**
********************************************************************************
* @brief
*   SDEC PID filter set.
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
int SDEC_IO_PIDFilterSetPID
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_SET_PID_T *stpLXSdecPIDFltSetPID)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidFltId = 0x0, ui32PidValue = 0x0, ui32PidfData = 0x0, ui32PidfDest = DEST_RESERVED, ui32dest= 0x0;
	LX_SDEC_CFG_T* pSdecConf = NULL;
	BOOLEAN	bSection = FALSE, bEnable = TRUE;
	BOOLEAN bClearPacket = FALSE; // JP WorkAround of MCU WorkAround
	UINT8 core = 0, org_ch = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltSetPID == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PIDFilterSetPID");

	ui8Ch = stpLXSdecPIDFltSetPID->eCh;
	ui32PidFltId = stpLXSdecPIDFltSetPID->uiPidFltId;
	ui32PidValue = stpLXSdecPIDFltSetPID->uiPidValue;
	ui32dest = stpLXSdecPIDFltSetPID->uiDest;
	bSection = stpLXSdecPIDFltSetPID->bSection;
	bEnable = stpLXSdecPIDFltSetPID->bEnable;

	org_ch = ui8Ch;
	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	SDEC_PRINT_PIDSEC( "filter type:[%08x]", ui32dest);

	/* check channel number */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( org_ch >= pSdecConf->nChannel, goto exit, "over channel range %d", org_ch);
	LX_SDEC_CHECK_CODE( ui32PidFltId >= pSdecConf->chInfo[ui8Ch].num_pidf, goto exit, "PID Filter Index Error [%d]", ui32PidFltId);

	/* from H13 A0, CDIC2 has 4 pid filters */
	if( pSdecConf->chInfo[org_ch].capa_lev == 0 )
	{
		SDEC_HAL_CDIC2PIDFSetPidfData(core, ui32PidFltId, ui32PidValue);
		SDEC_HAL_CDIC2PIDFEnablePidFilter(core, ui32PidFltId, SDEC_HAL_ENABLE);	/* seperate enable as L9 correction */
		SDEC_SWP_SetPID(ui32PidFltId, ui32PidValue);
		goto exit_without_error;
	}

	/* setting PID */
	ui32PidfData = PID(ui32PidValue);

	if(ui32dest & LX_SDEC_PIDFLT_DEST_VDEC0)
	{
		ui32PidfData |= PAYLOAD_PES;
		ui32PidfDest = VID_DEV0;
		SDEC_HAL_STCCSetVideoAssign(core, 0, ui8Ch);
		if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		{
			SDEC_HAL_SetVideoOut(0, core);
		}
		SDEC_PRINT_PIDSEC( "Ch[%d] VID0 PidFltId[%d] PidValue[0x%08x]", org_ch, ui32PidFltId, ui32PidValue);
	}
	else if(ui32dest & LX_SDEC_PIDFLT_DEST_VDEC1)
	{
		ui32PidfData |= PAYLOAD_PES;
		ui32PidfDest = VID_DEV1;
		SDEC_HAL_STCCSetVideoAssign(core, 1, ui8Ch);
		if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		{
			SDEC_HAL_SetVideoOut(1, core);
		}
		SDEC_PRINT_PIDSEC( "Ch[%d] VID1 PidFltId[%d] PidValue[0x%08x]", org_ch, ui32PidFltId, ui32PidValue);
	}
	else if(ui32dest & LX_SDEC_PIDFLT_DEST_ADEC0)
	{
		ui32PidfData |= PAYLOAD_PES;
		ui32PidfDest = AUD_DEV0;
		SDEC_HAL_STCCSetAudioAssign(core, 0, ui8Ch);
		if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		{
			SDEC_HAL_SetAudioOut(0, core);
		}
		SDEC_PRINT_PIDSEC( "Ch[%d] AUD0 PidFltId[%d] PidValue[0x%08x]", org_ch, ui32PidFltId, ui32PidValue);
	}
	else if(ui32dest & LX_SDEC_PIDFLT_DEST_ADEC1)
	{
		ui32PidfData |= PAYLOAD_PES;
		ui32PidfDest = AUD_DEV1;
		SDEC_HAL_STCCSetAudioAssign(core, 1, ui8Ch);
		if(lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		{
			SDEC_HAL_SetAudioOut(1, core);
		}
		SDEC_PRINT_PIDSEC( "Ch[%d] AUD1 PidFltId[%d] PidValue[0x%08x]", org_ch, ui32PidFltId, ui32PidValue);
	}
	else if(ui32dest & LX_SDEC_PIDFLT_DEST_GPB)
	{
		ui32PidfDest = DEST_RESERVED;
		SDEC_PRINT_PIDSEC( "Ch[%d] GPBPidFltId[%d] PidValue[0x%08x]", org_ch, ui32PidFltId, ui32PidValue);
	}
	else if(ui32dest & LX_SDEC_PIDFLT_DEST_RAWTS)
	{
		//for saving raw ts
		ui32PidfData |= TS_DN;
		ui32PidfDest = DEST_RESERVED;
		SDEC_PRINT_PIDSEC( "Ch[%d] GPBPidFltId[%d] PidValue[0x%08x]", org_ch, ui32PidFltId, ui32PidValue);
	}
	else if(ui32dest & LX_SDEC_PIDFLT_DEST_OTHER)
	{
		ui32PidfDest = DEST_RESERVED;
		SDEC_PRINT_PIDSEC( "Ch[%d] GPBPidFltId[%d] PidValue[0x%08x]", org_ch, ui32PidFltId, ui32PidValue);
	}

	if(ui32dest & LX_SDEC_PIDFLT_DEST_TSO)
	{
		ui32PidfData |= TSO_EN;
		SDEC_PRINT_PIDSEC( "Ch[%d] TSO PidFltId[%d] PidValue[0x%08x]", org_ch, ui32PidFltId, ui32PidValue);
	}

	if( ui32dest & LX_SDEC_PIDFLT_DEST_DVR )
	{
		/* To delete PAYLOAD_PES for Section Download - for JP 2013.02.04 jinhwan.bae */
		//ui32PidfData |= PAYLOAD_PES;
		/* jinhwan.bae 2013. 02.17
		    Purpose : Support Section Download and Parsing at the same time, for netcast4.0 JP
		    Reason  : To support this, remove PLOAD_PES at download set pid.
		                  But Video Data is inserted to Section GPB
		    WorkAround : To divide set value as Section/PES type.
		                       If PES, set PLOAD_PES at download set pid. so need flag bSection */
		if(bSection == TRUE)
		{
			/* Not Insert PLOAD_PES at Download */
		}
		else
		{
			/* Insert PLOAD_PES at Download */
			ui32PidfData |= PAYLOAD_PES;
		}

		ui32PidfData |= DL_EN;

		SDEC_PRINT_PIDSEC( "Ch[%d] DVR PidFltId[%d] PidValue[0x%08x]", org_ch, ui32PidFltId, ui32PidValue);
	}

	SDEC_PRINT_PIDSEC( "Ch[%d] ui32PidfData[0x%08x] ui32PidfDest[0x%08x]", org_ch, ui32PidfData, ui32PidfDest);

	if(bEnable == TRUE)
	{
		// if( (stpSdecParam->ui8McuDescramblerCtrlMode == 1) && (ui8Ch == 1) && (ui32PidValue == 0x0))  /* in JP , PAT, CH_B */
		// jinhwan.bae for MCU Test H13 JP WorkAround
		// Channel 1 's triggering : PSI Section, so TPI_IEN is needed to send MCU the timimg to set key . cf. CH0 -> PCR
		/* 20140213 jinhwan.bae webOS adaptation
		    TPI_IEN is needed only at the download channel.
		    There are lots of way, but simplicity, only check DL_EN */
		if(stpSdecParam->ui8McuDescramblerCtrlMode == 1) // JP Mode
		{
			bClearPacket = _SDEC_Is_JPClearPacket(ui32PidValue);
#if 0
			if((org_ch == 1) && (bClearPacket == TRUE))
#else
			if((ui32PidfData & DL_EN) && (bClearPacket == TRUE))
#endif
			{
				SDEC_PRINT_PIDSEC( "Set DN Channel TPI_IEN for MCU Descrambler Ch[%d] ui32PidFltId[%d] ui32PidfData[0x%08x]", org_ch, ui32PidFltId, ui32PidfData);
				SDEC_SetPidfData(stpSdecParam, org_ch, ui32PidFltId, DEC_EN | TPI_IEN | ui32PidfData | ui32PidfDest );
			}
			else
			{
				SDEC_SetPidfData(stpSdecParam, org_ch, ui32PidFltId, DEC_EN | ui32PidfData | ui32PidfDest );
			}
		}
		else
		{
			/* 20140324 tvct problem */
			if( (stpSdecParam->ui8ClearTVCTGathering == 1) && (ui32PidValue == TVCT_PID) )
			{
				SDEC_SetPidfData(stpSdecParam, org_ch, ui32PidFltId, DEC_EN | NO_DSC | ui32PidfData | ui32PidfDest );
			}
			else
			{
				// same as previous version
				SDEC_SetPidfData(stpSdecParam, org_ch, ui32PidFltId, DEC_EN | ui32PidfData | ui32PidfDest );
			}
		}
	}
	else
	{
		// if( (stpSdecParam->ui8McuDescramblerCtrlMode == 1) && (ui8Ch == 1) && (ui32PidValue == 0x0))  /* in JP , PAT, CH_B */
		// jinhwan.bae for MCU Test H13 JP WorkAround
		// Channel 1 's triggering : PSI Section, so TPI_IEN is needed to send MCU the timimg to set key . cf. CH0 -> PCR
		/* 20140213 jinhwan.bae webOS adaptation
		    TPI_IEN is needed only at the download channel.
		    There are lots of way, but simplicity, only check DL_EN */
		if(stpSdecParam->ui8McuDescramblerCtrlMode == 1) // JP Mode
		{
			bClearPacket = _SDEC_Is_JPClearPacket(ui32PidValue);
#if 0
			if((org_ch == 1) && (bClearPacket == TRUE))
#else
			if((ui32PidfData & DL_EN) && (bClearPacket == TRUE))
#endif
			{
				SDEC_PRINT_PIDSEC( "Set DN Channel TPI_IEN for MCU Descrambler Ch[%d] ui32PidFltId[%d] ui32PidfData[0x%08x]", org_ch, ui32PidFltId, ui32PidfData);
				SDEC_SetPidfData(stpSdecParam, org_ch, ui32PidFltId, TPI_IEN | ui32PidfData | ui32PidfDest );
			}
			else
			{
				SDEC_SetPidfData(stpSdecParam, org_ch, ui32PidFltId, ui32PidfData | ui32PidfDest );
			}
		}
		else
		{
			/* 20140324 tvct problem */
			if( (stpSdecParam->ui8ClearTVCTGathering == 1) && (ui32PidValue == TVCT_PID) )
			{
				SDEC_SetPidfData(stpSdecParam, org_ch, ui32PidFltId, NO_DSC | ui32PidfData | ui32PidfDest );
			}
			else
			{
				// same as previous version
				SDEC_SetPidfData(stpSdecParam, org_ch, ui32PidFltId, ui32PidfData | ui32PidfDest );
			}
		}
	}

exit_without_error:
	SDEC_PRINT_TRACE( "-->SDEC_IO_PIDFilterSetPID");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);

}


/**
********************************************************************************
* @brief
*   SDEC PID filter map select.
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
int SDEC_IO_PIDFilterMapSelect
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_SELSECFLT_T *stpLXSdecPIDFltSelect)
{
	int eRet = RET_ERROR;
	LX_SDEC_CFG_T* 	pSdecConf = NULL;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidFltId = 0x0, ui32SecFltId = 0x0, pidf_data = 0x0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltSelect == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PIDFilterMapSelect");

	ui8Ch = stpLXSdecPIDFltSelect->eCh;
	ui32PidFltId = stpLXSdecPIDFltSelect->uiPidFltId;
	ui32SecFltId = stpLXSdecPIDFltSelect->uiSecFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32PidFltId >= pSdecConf->chInfo[ui8Ch].num_pidf, goto exit, "PID Filter Index Error Ch[%d]Pidf[%d]Total[%d]",
							ui8Ch, ui32PidFltId, pSdecConf->chInfo[ui8Ch].num_pidf);

	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	pidf_data = SDEC_GetPidfData(stpSdecParam, ui8Ch, ui32PidFltId);

	/* If PES mode, section filter is not used, But for simple structure, use section filter */
	if(	stpLXSdecPIDFltSelect->eGpbMode == LX_SDEC_PIDFLT_GPB_MODE_PES )
	{
		/* Disconnect section filter connection and remove destination */
		pidf_data &= ~ ( SF_MAN_EN | DEST );

		/* Enable PES_Paylod and link gpd index */
		pidf_data |=  PAYLOAD_PES | ui32SecFltId;

		/* For PES H/W bug workaound. See @LX_SDEC_USE_KTHREAD_PES */
#if	( LX_SDEC_USE_KTHREAD_PES == 1)
		if(pSdecConf->noPesBug == 0 )
		{
			/* if there is pes h/w buf, do it */
			/* added TS_DN. Gather TS packet and parse manually */
			pidf_data |=  PAYLOAD_PES | TS_DN | ui32SecFltId;

			SDEC_PES_SetPESFlt(ui8Ch, ui32SecFltId);
			SDEC_PES_SetDstBuf(stpSdecParam, ui8Ch, ui32SecFltId);
		}
#endif
	}
	else if ( stpLXSdecPIDFltSelect->eGpbMode == LX_SDEC_PIDFLT_GPB_MODE_RAWTS )
	{
		/* Disconnect section filter connection and remove destination */
		pidf_data &= ~ ( SF_MAN_EN | DEST );

		/* Enable PES_Paylod and link gpd index */
		pidf_data |=  PAYLOAD_PES | GPB_IRQ_CONF | TS_DN | ui32SecFltId;
	}
	else
	{
		/* connect section filter connection */
		pidf_data |= SF_MAN_EN;

		SDEC_HAL_SECFSetMapBit(ui8Ch, ui32PidFltId, ui32SecFltId);
	}

	SDEC_SetPidfData(stpSdecParam, ui8Ch, ui32PidFltId, pidf_data);

	stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui8PidFltIdx = ui32PidFltId;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].flag = true;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_ENABLE = 0x1;

	SDEC_PRINT_PIDSEC( "Ch[%d] SecMap true flag idx[%d]", ui8Ch, ui32SecFltId);
	SDEC_PRINT_TRACE( "-->SDEC_IO_PIDFilterMapSelect");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   SDEC PID filter enable or disable.
* @remarks
*  Enable is made by jinhwan.bae for fixing L9 Issue at google TV
*  There are some abnormal stream, some EIT have same PID as PAT,
*  So, IF the pid filter were enabled before section filter condition setting,
*  PAT callback is called with EIT payload.
*  the solution is to divide pid filter set and pid filter enable, as I remember at 2013.8.
* @par requirements:
*
* @param
*   ui32Arg :arguments from userspace
*   stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_IO_PIDFilterEnable
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_ENABLE_T *stpLXSdecPIDFltEnable)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidFltId = 0x0;
	BOOLEAN bPidFltEnable = 0x0;
	UINT8 core = 0, org_ch = 0;
	LX_SDEC_CFG_T* 	pSdecConf 		= NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltEnable == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PIDFilterEnable");

	ui8Ch = stpLXSdecPIDFltEnable->eCh;
	ui32PidFltId = stpLXSdecPIDFltEnable->uiPidFltId;
	bPidFltEnable = stpLXSdecPIDFltEnable->bPidFltEnable;

	org_ch = ui8Ch;
	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	SDEC_PRINT_PIDSEC( "Ch[%d] FltId[%d] flag[0x%08x]", org_ch, ui32PidFltId, bPidFltEnable);

	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( org_ch >= pSdecConf->nChannel, goto exit, "over channel range %d", org_ch);
	LX_SDEC_CHECK_CODE( ui8Ch == LX_SDEC_CH_D, goto exit, "Channel is Invalid (%d)",ui8Ch);
	LX_SDEC_CHECK_CODE( ui32PidFltId >= pSdecConf->chInfo[org_ch].num_pidf, goto exit, "CH[%d] PID Filter ID is Invalid (%d) Capa (%d)",
									org_ch, ui32PidFltId, pSdecConf->chInfo[org_ch].num_pidf);

	switch (bPidFltEnable)
	{
		case TRUE:

			if( pSdecConf->chInfo[org_ch].capa_lev == 0 )
			{
				SDEC_HAL_CDIC2PIDFEnablePidFilter(core, ui32PidFltId, SDEC_HAL_ENABLE);
			}
			else
            {
				SDEC_SetPidf_Enable(stpSdecParam, org_ch, ui32PidFltId, SDEC_HAL_ENABLE);
            }

			stpSdecParam->stPIDMap[org_ch][ui32PidFltId].stStatusInfo.f.SDEC_FLTSTATE_ENABLE = 0x1;
			stpSdecParam->stPIDMap[org_ch][ui32PidFltId].flag = TRUE;

		break;

		case FALSE:

			if( pSdecConf->chInfo[org_ch].capa_lev == 0 )
			{
				SDEC_HAL_CDIC2PIDFEnablePidFilter(core, ui32PidFltId, SDEC_HAL_DISABLE);
			}
			else
			{
				SDEC_SetPidf_Enable(stpSdecParam, org_ch, ui32PidFltId, SDEC_HAL_DISABLE);
			}

			stpSdecParam->stPIDMap[org_ch][ui32PidFltId].stStatusInfo.f.SDEC_FLTSTATE_ENABLE = 0x0;
			stpSdecParam->stPIDMap[org_ch][ui32PidFltId].flag = FALSE;
		break;

		default:
			SDEC_PRINT_ERROR("Invalid parameter:[%d]", bPidFltEnable);
			goto exit;
		break;
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_PIDFilterEnable");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   CRC check scheme of SDEC PID filter enable or disable.
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
int SDEC_IO_PIDFilterCRCEnable
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_ENABLE_T *stpLXSdecPIDFltPESCRCEnable)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidFltId = 0x0;
	BOOLEAN bPidFltEnable = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltPESCRCEnable == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PIDFilterCRCEnable");

	ui8Ch = stpLXSdecPIDFltPESCRCEnable->eCh;
	ui32PidFltId = stpLXSdecPIDFltPESCRCEnable->uiPidFltId;
	bPidFltEnable = stpLXSdecPIDFltPESCRCEnable->bPidFltEnable;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32PidFltId >= pSdecConf->chInfo[ui8Ch].num_pidf, goto exit, "PID Filter Index Error Ch[%d]Pidf[%d]Total[%d]",
							ui8Ch, ui32PidFltId, pSdecConf->chInfo[ui8Ch].num_pidf);

	SDEC_PRINT_PIDSEC( "Ch[%d] FltId[%d] flag[0x%08x]", ui8Ch, ui32PidFltId, bPidFltEnable);

	if( bPidFltEnable)
		SDEC_HAL_PIDFSetCRCBit(ui8Ch, ui32PidFltId);
	else
		SDEC_HAL_PIDFClearCRCBit(ui8Ch, ui32PidFltId);

	SDEC_PRINT_TRACE( "-->SDEC_IO_PIDFilterCRCEnable");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   SDEC PID filter get state.
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
int SDEC_IO_PIDFilterGetState
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_STATE_T *stpLXSdecPIDFltState)
{
	int eRet = RET_ERROR;
	UINT32	pidfData = 0x0;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidFltId = 0x0;
	LX_SDEC_CFG_T* pSdecConf = NULL;
	UINT8 core = 0, org_ch = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltState == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PIDFilterGetState");

	ui8Ch = stpLXSdecPIDFltState->eCh;
	ui32PidFltId = stpLXSdecPIDFltState->uiPidFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32PidFltId >= pSdecConf->chInfo[ui8Ch].num_pidf, goto exit, "PID Filter Index Error Ch[%d]Pidf[%d]Total[%d]",
							ui8Ch, ui32PidFltId, pSdecConf->chInfo[ui8Ch].num_pidf);

	org_ch = ui8Ch;
	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	/* from H13 A0, CDIC2 has 4 pid filters */
	if( pSdecConf->chInfo[org_ch].capa_lev == 0 )
	{
		stpLXSdecPIDFltState->uiPidValue = SDEC_HAL_CDIC2PIDFGetPidfData(core, stpLXSdecPIDFltState->uiPidFltId);
		stpLXSdecPIDFltState->bDec_en	= SDEC_HAL_CDIC2GetPIDFEnable(core, stpLXSdecPIDFltState->uiPidFltId);

		/* jinhwan.bae more information need? */
		goto exit_without_error;
	}

	pidfData = SDEC_GetPidfData(stpSdecParam, org_ch, stpLXSdecPIDFltState->uiPidFltId);

	/* output */
	stpLXSdecPIDFltState->uiPidValue = extract_bits(pidfData, 0x1FFF, 16);
	stpLXSdecPIDFltState->bDec_en	= extract_bits(pidfData, 0x1, 31);
	stpLXSdecPIDFltState->bDl_en	= extract_bits(pidfData, 0x1, 30);
	stpLXSdecPIDFltState->bPload_pes= extract_bits(pidfData, 0x1, 12);
	stpLXSdecPIDFltState->bSf_map_en= extract_bits(pidfData, 0x1, 11);
	stpLXSdecPIDFltState->uiDest	= extract_bits(pidfData, 0x7f, 0);
	stpLXSdecPIDFltState->uiSecf_map[0] = SDEC_HAL_SECFGetMap(org_ch, ui32PidFltId*2);
	stpLXSdecPIDFltState->uiSecf_map[1] = SDEC_HAL_SECFGetMap(org_ch, ui32PidFltId*2 + 1);
	stpLXSdecPIDFltState->uiFltState = stpSdecParam->stPIDMap[org_ch][ui32PidFltId].stStatusInfo.w;
	stpLXSdecPIDFltState->uiRegValue = pidfData;

	SDEC_PRINT_PIDSEC( "Ch[%d] pididx[%d] value[0x%08x]", org_ch, ui32PidFltId, stpLXSdecPIDFltState->uiPidValue);

exit_without_error:
	SDEC_PRINT_TRACE( "-->SDEC_IO_PIDFilterGetState");
	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);

}

/**
********************************************************************************
* @brief
*   SDEC PID filter enable or disable.
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
int SDEC_IO_PIDFilterEnableSCMBCHK
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_ENABLE_T *stpLXSdecPIDFltEnable)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidFltId = 0x0;
	BOOLEAN bPidFltEnable = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltEnable == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PIDFilterEnable");

	ui8Ch = stpLXSdecPIDFltEnable->eCh;
	ui32PidFltId = stpLXSdecPIDFltEnable->uiPidFltId;
	bPidFltEnable = stpLXSdecPIDFltEnable->bPidFltEnable;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32PidFltId >= pSdecConf->chInfo[ui8Ch].num_pidf, goto exit, "PID Filter Index Error Ch[%d]Pidf[%d]Total[%d]",
							ui8Ch, ui32PidFltId, pSdecConf->chInfo[ui8Ch].num_pidf);

	SDEC_PRINT_PIDSEC( "Ch[%d] FltId[%d] flag[0x%08x]", ui8Ch, ui32PidFltId, bPidFltEnable);

	// jinhwan.bae for No Audio Issue, 2012.04.26 SDEC_HAL_PIDFScrambleCheck(ui8Ch, ui32PidFltId, bPidFltEnable);
	// replaced following spin lock I/F
	SDEC_SetPidf_TPI_IEN_Enable(stpSdecParam, ui8Ch, ui32PidFltId, bPidFltEnable);

	/* enable tp interrupt */
	if(bPidFltEnable)
		SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(ui8Ch, SDEC_HAL_ENABLE);
	else
		SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(ui8Ch, SDEC_HAL_DISABLE);

	SDEC_PRINT_TRACE( "-->SDEC_IO_PIDFilterEnable");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   Enable download as SDEC PID filter
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
int SDEC_IO_PIDFilterEnableDownload
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_ENABLE_T *stpLXSdecPIDFltEnableDownload)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidFltId = 0x0;
	UINT8  bEnable = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;
	UINT32 ui32PidValue = 0x0, pidfData = 0x0;
	BOOLEAN bClearPacket = FALSE; // JP WorkAround of MCU WorkAround

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltEnableDownload == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PIDFilterEnableDownload");

	ui8Ch = stpLXSdecPIDFltEnableDownload->eCh;
	ui32PidFltId = stpLXSdecPIDFltEnableDownload->uiPidFltId;
	bEnable = stpLXSdecPIDFltEnableDownload->bPidFltEnable;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32PidFltId >= pSdecConf->chInfo[ui8Ch].num_pidf, goto exit, "PID Filter Index Error Ch[%d]Pidf[%d]Total[%d]",
							ui8Ch, ui32PidFltId, pSdecConf->chInfo[ui8Ch].num_pidf);

	SDEC_SetPidf_DN_Enable(stpSdecParam, ui8Ch, ui32PidFltId, bEnable);

	/* 20140503 jinhwan.bae for H13 JP Mode
	 * MCU TPI Interrupt is not used for key setting yet,
	 * because call sequence is changed
	 * from DN_EN -> Request, to Request -> DN_EN
	 * so add second case defensive code without any touching to Main PCR */
	if(stpSdecParam->ui8McuDescramblerCtrlMode == 1) // H13 JP Mode
	{
		pidfData = SDEC_GetPidfData(stpSdecParam, ui8Ch, ui32PidFltId);
		ui32PidValue = extract_bits(pidfData, 0x1FFF, 16);
		bClearPacket = _SDEC_Is_JPClearPacket(ui32PidValue);
		if(bClearPacket == TRUE)
		{
			SDEC_PRINT_PIDSEC("Set TPI_IEN in DN_EN for MCU Descrambler Ch[%d] ui32PidFilId[%d]", ui8Ch, ui32PidFltId);
			SDEC_SetPidf_TPI_IEN_Enable(stpSdecParam, ui8Ch, ui32PidFltId, bEnable);
		}
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_PIDFilterEnableDownload");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


//from here for section filter

/**
********************************************************************************
* @brief
*   SDEC Section filter alloc.
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
int SDEC_IO_SectionFilterAlloc
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_ALLOC_T *stpLXSdecSecFltAlloc)
{
	int eRet = RET_ERROR, eResult = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT8 ui8SecIdx = 0x0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecFltAlloc == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterAlloc");

	ui8Ch = stpLXSdecSecFltAlloc->eCh;

	eResult = SDEC_SecIdxCheck(stpSdecParam, ui8Ch, &ui8SecIdx);
	LX_SDEC_CHECK_CODE( LX_IS_ERR(eResult), goto exit, "SDEC_SelSecFilterIdx failed:[%d]", eResult);

	SDEC_PRINT_PIDSEC( "Ch[%d] returned Section PID Idx:[%d]", ui8Ch, ui8SecIdx);

	stpLXSdecSecFltAlloc->uiSecFltId = ui8SecIdx;

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterAlloc");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/* Work Around for si11~ stream, PMT updated stream but all client request/cancel independantly
    so, MHEG DSMCC and Subtitle with same PID are enabled, and Subtitle Canceled
    But PES_PLOAD is not removed in PID filter, so, abnormal situation happened. */
/**
********************************************************************************
* @brief
*   SDEC PES filter free.
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
int SDEC_IO_PESFilterFree
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_FREE_T *stpLXSecPIDFltFree)
{
	int eRet = RET_ERROR;
	LX_SDEC_CFG_T* pSdecConf = NULL;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0;
	UINT32 ui8PidFltIdx = 0x0;

	unsigned long flags = 0;

	UINT32 pidf_data = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSecPIDFltFree == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_PESFilterFree");

	ui8Ch = stpLXSecPIDFltFree->eCh;
	ui32SecFltId = stpLXSecPIDFltFree->uiSecFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	ui8PidFltIdx = stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui8PidFltIdx;

	pidf_data = SDEC_GetPidfData(stpSdecParam, ui8Ch, ui8PidFltIdx);

	/* Clear PAYLOAD_PES Fields */
	pidf_data &= ~ ( PAYLOAD_PES | DEST );
	pidf_data |= DEST_RESERVED; /* 4F */

	SDEC_SetPidfData(stpSdecParam, ui8Ch, ui8PidFltIdx, pidf_data);

	if(SDEC_IS_NORMAL_CHANNEL(ui8Ch))
	{
		SDEC_HAL_SECFClearMapBit(ui8Ch, ui8PidFltIdx, ui32SecFltId);
	}

	SDEC_PRINT_PIDSEC( "Ch[%d] PesFltId[%d] Free", ui8Ch, ui32SecFltId);

	SDEC_GPB_LOCK(stpSdecParam, ui8Ch, ui32SecFltId);

	/* delete msge in notify queue */
	SDEC_DeleteInNotify(stpSdecParam, ui8Ch, ui32SecFltId);

	/* Section Filter  Map init */
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].used = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].flag = FALSE;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].mode = 0x0;

	SDEC_Secf_Clear(stpSdecParam, ui8Ch, ui32SecFltId);

	/* 2012.02.06 gaius.lee
	 * Bug exist in L9 HW.
	 * While CPU access Read/Write/Bound Register, SDEC HW accesses write register, write pointer goes to read/write/bound regitser which CPU access.
	 * So, we make GPB map as static. That's why we disable this section. */
	if((pSdecConf->staticGPB == 0) || (ui8Ch == LX_SDEC_CH_C))
	{
		stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui32Baseptr 	= 0x00000000;
		stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui32Endptr	= 0x00000000;
		stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui32Readptr 	= 0x00000000;
		stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui32UsrReadptr 	= 0x00000000;
	}
	stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui8PidFltIdx	= 0x0;

	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.w = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_OVERFLOW = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_DATAREADY = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_SCRAMBLED = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_ENABLE = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_ALLOC = 0x0;

	/* clear full buffer interrupt */
	if(SDEC_IS_NORMAL_CHANNEL(ui8Ch))
	{
		SDEC_HAL_GPBClearFullIntr(ui8Ch, ui32SecFltId);
	}

	SDEC_GPB_UNLOCK(stpSdecParam, ui8Ch, ui32SecFltId);

	SDEC_PRINT_TRACE( "-->SDEC_IO_PESFilterFree");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   SDEC Section filter free.
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
int SDEC_IO_SectionFilterFree
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_FREE_T *stpLXSecPIDFltFree)
{
	int eRet = RET_ERROR;
	LX_SDEC_CFG_T* pSdecConf = NULL;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0;
	UINT32 ui8PidFltIdx = 0x0;
	unsigned long flags = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSecPIDFltFree == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterFree");

	ui8Ch = stpLXSecPIDFltFree->eCh;
	ui32SecFltId = stpLXSecPIDFltFree->uiSecFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	ui8PidFltIdx = stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui8PidFltIdx;

	if(SDEC_IS_NORMAL_CHANNEL(ui8Ch))
	{
		SDEC_HAL_SECFClearMapBit(ui8Ch, ui8PidFltIdx, ui32SecFltId);
	}

	SDEC_PRINT_PIDSEC( "Ch[%d] SecFltId[%d] Free", ui8Ch, ui32SecFltId);

	SDEC_GPB_LOCK(stpSdecParam, ui8Ch, ui32SecFltId);

	/* delete msge in notify queue */
	SDEC_DeleteInNotify(stpSdecParam, ui8Ch, ui32SecFltId);

	/* Section Filter  Map init */
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].used = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].flag = FALSE;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].mode = 0x0;

	SDEC_Secf_Clear(stpSdecParam, ui8Ch, ui32SecFltId);

	/* 2012.02.06 gaius.lee
	 * Bug exist in L9 HW.
	 * While CPU access Read/Write/Bound Register, SDEC HW accesses write register, write pointer goes to read/write/bound regitser which CPU access.
	 * So, we make GPB map as static. That's why we disable this section. */
	if((pSdecConf->staticGPB == 0) || (ui8Ch == LX_SDEC_CH_C))
	{
		stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui32Baseptr 	= 0x00000000;
		stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui32Endptr	= 0x00000000;
		stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui32Readptr 	= 0x00000000;
		stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui32UsrReadptr 	= 0x00000000;
	}
	stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui8PidFltIdx	= 0x0;

	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.w = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_OVERFLOW = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_DATAREADY = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_SCRAMBLED = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_ENABLE = 0x0;
	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.f.SDEC_FLTSTATE_ALLOC = 0x0;

	/* clear full buffer interrupt */
	if(SDEC_IS_NORMAL_CHANNEL(ui8Ch))
	{
		SDEC_HAL_GPBClearFullIntr(ui8Ch,	ui32SecFltId);
	}

	SDEC_GPB_UNLOCK(stpSdecParam, ui8Ch, ui32SecFltId);

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterFree");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   SDEC Section filter pattern.
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
int SDEC_IO_SectionFilterPattern
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_PATTERN_T *stpLXSecFltPattern)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0, bHigh = 0x0, ui8Mode = 0x0;
	UINT32 ui32SecFltId = 0x0;
	UINT32 *ui32pPattern;
	UINT32 *ui32pMask;
	UINT32 *ui32pNeq;
	UINT32 ui32Pattern_L = 0x0, ui32Pattern_H = 0x0;
	UINT32 ui32Mask_L = 0x0, ui32Mask_H = 0x0;
	UINT32 ui32Neq_L = 0x0, ui32Neq_H = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSecFltPattern == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterPattern");

	ui8Ch = stpLXSecFltPattern->eCh;
	ui32SecFltId = stpLXSecFltPattern->uiSecFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	/* if section filter index is higher than 32, sometimes use other register */
	if(ui32SecFltId > 32)	bHigh = 0x1;
	else					bHigh = 0x0;

 	ui8Mode = (UINT8)stpLXSecFltPattern->uiSecFltMode;

	if((ui8Ch == LX_SDEC_CH_C) || (ui8Ch == LX_SDEC_CH_G))
	{
		SDEC_SWP_SetSectionFilterPattern(stpLXSecFltPattern);
		stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].mode = ui8Mode;

		SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterPattern");
		OS_UnlockMutex(&stpSdecParam->stSdecMutex);
		return RET_OK;
	}

	ui32pPattern = (UINT32 *)stpLXSecFltPattern->pucPattern;
	ui32pMask = (UINT32 *)stpLXSecFltPattern->pucMask;
	ui32pNeq = (UINT32 *)stpLXSecFltPattern->pucNotEqual;

	ui32Pattern_H	= swab32(*ui32pPattern); 	ui32pPattern++;
	ui32Pattern_L	= swab32(*ui32pPattern);
	ui32Mask_H		= swab32(*ui32pMask);		ui32pMask++;
	ui32Mask_L 		= swab32(*ui32pMask);
	ui32Neq_H 		= swab32(*ui32pNeq);		ui32pNeq++;
	ui32Neq_L 		= swab32(*ui32pNeq);

	stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].mode = ui8Mode;

	SDEC_PRINT_PIDSEC( "Ch[%d] Section Filter Mode:[%d]", ui8Ch, stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].mode);

	SDEC_HAL_SECFSetBufValidBit(ui8Ch, ui32SecFltId);

	if( ui8Mode & LX_SDEC_FLTMODE_NOCRCCHK )
		SDEC_HAL_SECFClearCRCBit(ui8Ch, ui32SecFltId);
	else
		SDEC_HAL_SECFSetCRCBit(ui8Ch, ui32SecFltId);

	SDEC_HAL_SECFSetSecfData(ui8Ch, ui32SecFltId, 0, ui32Pattern_H);
	SDEC_HAL_SECFSetSecfData(ui8Ch, ui32SecFltId, 1, ui32Pattern_L);
	SDEC_HAL_SECFSetSecfData(ui8Ch, ui32SecFltId, 2, ui32Mask_H);
	SDEC_HAL_SECFSetSecfData(ui8Ch, ui32SecFltId, 3, ui32Mask_L);
	SDEC_HAL_SECFSetSecfData(ui8Ch, ui32SecFltId, 4, ui32Neq_H);
	SDEC_HAL_SECFSetSecfData(ui8Ch, ui32SecFltId, 5, ui32Neq_L);
	SDEC_HAL_SECFSetSecfData(ui8Ch, ui32SecFltId, 6, ui32SecFltId);

	SDEC_HAL_SECFSetEnableBit(ui8Ch, ui32SecFltId);

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterPattern");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   SDEC Section filter buffer reset.
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
int SDEC_IO_SectionFilterBufferReset
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_BUFFER_RESET *stpLXSdecSecfltBufferReset)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32GpbIdx = 0x0, ui32GpbBaseAddr = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltBufferReset == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterBufferReset");

	ui8Ch 		= stpLXSdecSecfltBufferReset->eCh;
	ui32GpbIdx 	= stpLXSdecSecfltBufferReset->uiSecFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32GpbIdx >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "GPB Index Error Ch[%d]GPB[%d]Total[%d]",
							ui8Ch, ui32GpbIdx, pSdecConf->chInfo[ui8Ch].num_secf);

	ui32GpbBaseAddr = stpSdecParam->stSdecMeminfo[ui8Ch][ui32GpbIdx].ui32Baseptr;

	SDEC_PRINT_ERROR( "SDEC_IO_SectionFilterBufferReset CH%c gpb[%d]", (ui8Ch)?'B':'A', ui32GpbIdx);

	SDEC_HAL_GPBSetReadPtr(ui8Ch, 	ui32GpbIdx, ui32GpbBaseAddr);
	SDEC_HAL_GPBSetWritePtr(ui8Ch, 	ui32GpbIdx, ui32GpbBaseAddr);

	/* initialize base & end pointer */
	stpSdecParam->stSdecMeminfo[ui8Ch][ui32GpbIdx].ui32Baseptr 	= ui32GpbBaseAddr;
	stpSdecParam->stSdecMeminfo[ui8Ch][ui32GpbIdx].ui32Readptr 	= ui32GpbBaseAddr;
	stpSdecParam->stSdecMeminfo[ui8Ch][ui32GpbIdx].ui32UsrReadptr 	= ui32GpbBaseAddr;

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterBufferReset");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   SDEC Section filter  buffer set.
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
int SDEC_IO_SectionFilterBufferSet
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_BUFFER_SET_T *stpLXSdecSecfltBufferSet)
{
	int eRet = RET_ERROR, eResult = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0, ui32BufAddress = 0x0;
	LX_SDEC_GPB_SIZE_T eBufferSize = LX_SDEC_GPB_SIZE_4K;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltBufferSet == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	ui8Ch = stpLXSdecSecfltBufferSet->eCh;
	ui32SecFltId = stpLXSdecSecfltBufferSet->uiSecFltId;
	ui32BufAddress = stpLXSdecSecfltBufferSet->uiBufAddress;
	eBufferSize = stpLXSdecSecfltBufferSet->eBufferSize;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterBufferSet [%d][%d]",ui8Ch, ui32SecFltId);

	/* GPB Set */
	eResult = SDEC_GpbSet(stpSdecParam,	ui8Ch, eBufferSize, ui32BufAddress, ui32SecFltId);
	LX_SDEC_CHECK_CODE( LX_IS_ERR(eResult), goto exit, "SDEC_GpbSet failed");

	SDEC_PRINT_PIDSEC( "Ch[%d] GPB Base address:[0x%08x]", ui8Ch, stpLXSdecSecfltBufferSet->uiBufAddress);
	SDEC_PRINT_PIDSEC( "Ch[%d] GPB size:[%d]", ui8Ch, stpLXSdecSecfltBufferSet->eBufferSize);
	SDEC_PRINT_PIDSEC( "Ch[%d] ui32PidFltId:[%d]", ui8Ch, ui32SecFltId);

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterBufferSet");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);

}

/**
********************************************************************************
* @brief
*   SDEC Section filter dummy buffer set.
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
int SDEC_IO_SectionFilterDummyBufferSet
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_BUFFER_SET_T *stpLXSdecSecfltBufferSet)
{
	int eRet = RET_ERROR, eResult = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0, ui32BufAddress = 0x0;
	LX_SDEC_GPB_SIZE_T eBufferSize = LX_SDEC_GPB_SIZE_4K;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltBufferSet == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	ui8Ch = stpLXSdecSecfltBufferSet->eCh;
	ui32SecFltId = stpLXSdecSecfltBufferSet->uiSecFltId;
	ui32BufAddress = stpLXSdecSecfltBufferSet->uiBufAddress;
	eBufferSize = stpLXSdecSecfltBufferSet->eBufferSize;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= 64, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]",
							ui8Ch, ui32SecFltId);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterDummyBufferSet [%d][%d]",ui8Ch, ui32SecFltId);

	/* GPB Set */
	eResult = SDEC_DummyGpbSet(stpSdecParam,	ui8Ch, eBufferSize, ui32BufAddress, ui32SecFltId);
	LX_SDEC_CHECK_CODE( LX_IS_ERR(eResult), goto exit, "SDEC_DummyGpbSet failed");

	SDEC_PRINT_PIDSEC( "Ch[%d] Dummy GPB Base address:[0x%08x]", ui8Ch, stpLXSdecSecfltBufferSet->uiBufAddress);
	SDEC_PRINT_PIDSEC( "Ch[%d] Dummy GPB size:[%d]", ui8Ch, stpLXSdecSecfltBufferSet->eBufferSize);
	SDEC_PRINT_PIDSEC( "Ch[%d] Dummy ui32PidFltId:[%d]", ui8Ch, ui32SecFltId);

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterDummyBufferSet");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);

}

/**
********************************************************************************
* @brief
*   SDEC Section filter  get infot.
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
int SDEC_IO_SectionFilterGetInfo
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_BUFFER_GET_INFO_T *stpLXSdecSecfltBufferGetInfo)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltBufferGetInfo == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterGetInfo");

	ui8Ch = stpLXSdecSecfltBufferGetInfo->eCh;
	ui32SecFltId = stpLXSdecSecfltBufferGetInfo->uiSecFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	LX_SDEC_CHECK_CODE( !SDEC_IS_NORMAL_CHANNEL(ui8Ch), goto exit, "ch[%d] is Invalid parameters", ui8Ch );

	stpLXSdecSecfltBufferGetInfo->uiReadPtr 	= SDEC_HAL_GPBGetReadPtr(ui8Ch, ui32SecFltId);
	stpLXSdecSecfltBufferGetInfo->uiWritePtr 	= SDEC_HAL_GPBGetWritePtr(ui8Ch, ui32SecFltId);

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterGetInfo");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   SDEC Section filter  set read pointer.
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
int SDEC_IO_SectionFilterSetReadPtr
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_READPTR_SET_T *stpLXSdecSecfltReadPtrSet)
{
	int eRet = RET_ERROR;
	LX_SDEC_CFG_T* pSdecConf = NULL;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0;
	UINT32 ui32ReadPtr = 0x0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltReadPtrSet == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE_EX( "<--SDEC_IO_SectionFilterSetReadPtr");

	ui8Ch 			= stpLXSdecSecfltReadPtrSet->eCh;
	ui32SecFltId 	= stpLXSdecSecfltReadPtrSet->uiSecFltId;
	ui32ReadPtr 	= stpLXSdecSecfltReadPtrSet->uiReadPtr;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	/* 2012.02.06 gaius.lee
	 * Bug exist in L9 HW.
	 * While CPU access Read/Write/Bound Register, SDEC HW accesses write register, write pointer goes to read/write/bound regitser which CPU access.
	 * So, remove access to read register. That's why we disable this line. */
	if(pSdecConf->staticGPB == 0)
	{
		SDEC_HAL_GPBSetReadPtr(ui8Ch, ui32SecFltId, ui32ReadPtr);
	}

	/* save user read pointer */
	stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui32UsrReadptr = ui32ReadPtr;

	SDEC_PRINT_TRACE_EX( "-->SDEC_IO_SectionFilterSetReadPtr");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   SDEC Section filter  buffer get state.
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
int SDEC_IO_SectionFilterGetState
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_STATE_T *stpLXSdecSecfltState)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0;
	UINT8 ui8wordIdx = 0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltState == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterGetState");

	ui8Ch = stpLXSdecSecfltState->eCh;
	ui32SecFltId = stpLXSdecSecfltState->uiSecFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	/* Get Status of Section Filter @see LX_SDEC_FLTSTATE_T */
	stpLXSdecSecfltState->uiFltState = stpSdecParam->stSecMap[ui8Ch][ui32SecFltId].stStatusInfo.w;

	/* Get Map Type */
	stpLXSdecSecfltState->ucSecf_mtype = SDEC_HAL_SECFGetMapTypeBit(ui8Ch, ui32SecFltId);

	/* Get Linked PID Filter */
	stpLXSdecSecfltState->uiPidFltId = stpSdecParam->stSdecMeminfo[ui8Ch][ui32SecFltId].ui8PidFltIdx;

	for( ui8wordIdx = 0 ; ui8wordIdx < 7 ; ui8wordIdx++ )
	{
		/* jinhwan.bae , static analysis, protect overrun */
		if( ui8wordIdx < 2 ) stpLXSdecSecfltState->uiPattern[ui8wordIdx] = SDEC_HAL_SECFGetSecfData(ui8Ch, ui32SecFltId, ui8wordIdx);
		else if( ui8wordIdx < 4 ) stpLXSdecSecfltState->uiMask[ui8wordIdx - 2] = SDEC_HAL_SECFGetSecfData(ui8Ch, ui32SecFltId, ui8wordIdx);
		else if( ui8wordIdx < 6 ) stpLXSdecSecfltState->uiNotEqual[ui8wordIdx - 4] = SDEC_HAL_SECFGetSecfData(ui8Ch, ui32SecFltId, ui8wordIdx);
		else if( ui8wordIdx == 6 ) stpLXSdecSecfltState->uiGpbIdx = SDEC_HAL_SECFGetSecfData(ui8Ch, ui32SecFltId, ui8wordIdx);
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterGetState");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   SDEC Section filter  set read pointer.
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
int SDEC_IO_SectionFilterGetReadPtr
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_GET_READPTR_T *stpLXSdecSecfltReadPtrGet)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0;
	UINT32 ui32ReadPtr = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltReadPtrGet == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterGetReadPtr");

	ui8Ch 			= stpLXSdecSecfltReadPtrGet->eCh;
	ui32SecFltId 	= stpLXSdecSecfltReadPtrGet->uiSecFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	ui32ReadPtr = SDEC_HAL_GPBGetReadPtr(ui8Ch, ui32SecFltId);

	stpLXSdecSecfltReadPtrGet->uiValue = ui32ReadPtr;

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterGetReadPtr (%x)", stpLXSdecSecfltReadPtrGet->uiValue);

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   SDEC Section filter  get write pointer.
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
int SDEC_IO_SectionFilterGetWritePtr
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_GET_WRITEPTR_T *stpLXSdecSecfltWritePtrGet)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0;
	UINT32 ui32ReadPtr = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltWritePtrGet == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterGetReadPtr");

	ui8Ch 			= stpLXSdecSecfltWritePtrGet->eCh;
	ui32SecFltId 	= stpLXSdecSecfltWritePtrGet->uiSecFltId;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
       LX_SDEC_CHECK_CODE( ( ( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf ) || ( pSdecConf->chInfo[ui8Ch].capa_lev == 0 ) ),
                                                                                                                goto exit,
                                                                                                                "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]capa_lev[0]",
                                                                                                                ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);
#if 0
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);
#endif

	ui32ReadPtr = SDEC_HAL_GPBGetWritePtr(ui8Ch, ui32SecFltId);

	stpLXSdecSecfltWritePtrGet->uiValue = ui32ReadPtr;

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterGetWritePtr (%x)", stpLXSdecSecfltWritePtrGet->uiValue);

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}



/**
********************************************************************************
* @brief
*   SDEC Section filter  set read pointer.
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
int SDEC_IO_SectionFilterSetWritePtr
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SECFLT_WRITEPTR_SET_T *stpLXSdecSecfltWritePtrSet)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32SecFltId = 0x0;
	UINT32 ui32WritePtr = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSecfltWritePtrSet == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SectionFilterSetWritePtr");

	ui8Ch 			= stpLXSdecSecfltWritePtrSet->eCh;
	ui32SecFltId 	= stpLXSdecSecfltWritePtrSet->uiSecFltId;
	ui32WritePtr 	= stpLXSdecSecfltWritePtrSet->uiWritePtr;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE( ui32SecFltId >= pSdecConf->chInfo[ui8Ch].num_secf, goto exit, "Section Filter Index Error Ch[%d]Secf[%d]Total[%d]",
							ui8Ch, ui32SecFltId, pSdecConf->chInfo[ui8Ch].num_secf);

	SDEC_HAL_GPBSetWritePtr(ui8Ch, ui32SecFltId, ui32WritePtr);

	SDEC_PRINT_TRACE( "-->SDEC_IO_SectionFilterSetWritePtr");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


