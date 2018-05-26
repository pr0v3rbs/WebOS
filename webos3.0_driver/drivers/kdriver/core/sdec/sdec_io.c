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

#include "sys_regs.h"

#include "sdec_swparser.h"
#include "sdec_pcr.h"
#include "sdec_core.h"
#include "sdec_filter.h"

#include "pvr_cfg.h"

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
 * Initialize SDEC IO
 *
 * @param	stpSdecParam	[in	] sdec param
 * @return	if succeeded - RET_OK, else - RET_ERROR.
 * @see
 */
int SDEC_IO_Init(S_SDEC_PARAM_T *stpSdecParam)
{
	return RET_OK;
}



/**
********************************************************************************
* @brief
*   sdec TPI register set
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui8Ch :channel
*   stpSdecParam :SDEC parameter
*   work_struct
* @return
*  int
********************************************************************************
*/

int SDEC_TPI_Set
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8Ch)
{
	int eRet = RET_ERROR;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_TPI_Set");

	//TPI_ICONF Reg set
	// gaius.lee 2010.10.26
	// Setting interrupt as payload unit start indicator bit is 1.
	// This means we want to check every PES packet for scramble status checking. This is for speed.
#if 1 //	jinhwan.bae mcu test
	SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(ui8Ch, SDEC_HAL_ENABLE);
	SDEC_HAL_TPISetIntrAutoScCheck(ui8Ch, SDEC_HAL_DISABLE);
#else
//	jinhwan.bae mcu test SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(ui8Ch, SDEC_HAL_ENABLE);
//	SDEC_HAL_TPISetIntrAutoScCheck(ui8Ch, SDEC_HAL_DISABLE);
//	jinhwan.bae mcu test SDEC_HAL_TPISetIntrAutoScCheck(ui8Ch, SDEC_HAL_ENABLE);
#endif

	SDEC_PRINT_TRACE( "-->SDEC_TPI_Set");

	eRet = RET_OK;

	return (eRet);
}




/**
********************************************************************************
* @brief
*   sdec Err interrrupt register set
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   ui8Ch :channel
*   stpSdecParam :SDEC parameter
*   work_struct
* @return
*  int
********************************************************************************
*/

int SDEC_ERR_Intr_Set
	(S_SDEC_PARAM_T *stpSdecParam,
	UINT8 ui8Ch)
{
	int eRet = RET_ERROR;

	// TODO: 초기화시 한번만 설정하면 되지 않나 ? 20140423 ks.hyun
#if 0
	UINT8 core;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_ERR_Intr_Set");

	core = (ui8Ch < SDEC_CORE_CH_NUM) ? 0 : 1;

	SDEC_HAL_EnableErrorInterrupt(core, MPG_SD, 	0x3);
	SDEC_HAL_EnableErrorInterrupt(core, MPG_CC, 	0x3);
	SDEC_HAL_EnableErrorInterrupt(core, MPG_DUP,	0x3);
	SDEC_HAL_EnableErrorInterrupt(core, MPG_TS, 	0x3);
	SDEC_HAL_EnableErrorInterrupt(core, MPG_PD, 	0x3);
	SDEC_HAL_EnableErrorInterrupt(core, STCC_DCONT, 0x3);
	SDEC_HAL_EnableErrorInterrupt(core, CDIF_RPAGE, 0x3);
	SDEC_HAL_EnableErrorInterrupt(core, CDIF_WPAGE, 0x3);
	SDEC_HAL_EnableErrorInterrupt(core, CDIF_OVFLOW,0x3);
	SDEC_HAL_EnableErrorInterrupt(core, SB_DROPPED, 0x3);
	SDEC_HAL_EnableErrorInterrupt(core, SYNC_LOST,	0x3);
	SDEC_HAL_EnableErrorInterrupt(core, TEST_DCONT, 0x1);

	SDEC_PRINT_TRACE( "-->SDEC_ERR_Intr_Set");
#endif
	eRet = RET_OK;

	return (eRet);
}



/**
********************************************************************************
* @brief
*   get the current STC ASG value.
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
int SDEC_IO_GetSTCCStatus
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GET_STCC_STATUS_T *stpLXSdecGetSTCCStatus)
{
	int eRet = RET_ERROR;
	LX_SDEC_STCC_STATUS_T *p = NULL;
	UINT8 	ui8Ch = 0x0;
	UINT32 	uiSTCCRegValue_32b = 0;
	UINT8	core = 0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecGetSTCCStatus == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetSTCCStatus");

	ui8Ch = stpLXSdecGetSTCCStatus->eCh;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);

	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	uiSTCCRegValue_32b = SDEC_HAL_STCCGetReg(core, ui8Ch);

	p = &(stpLXSdecGetSTCCStatus->stcc_status);
	p->bEn 			= ( (uiSTCCRegValue_32b & 0x80000000) >> 31 );
	p->ui8Chan 		= ( (uiSTCCRegValue_32b & 0x20000000) >> 29 );
	p->ui16Pcr_Pid 	= ( (uiSTCCRegValue_32b & 0x1fff0000) >> 16 );
	p->bRd_Mode		= ( (uiSTCCRegValue_32b & 0x00000100) >> 8 );
	p->bRate_Ctrl 	= ( (uiSTCCRegValue_32b & 0x00000010) >> 4 );
	p->bCopy_En 	= ( (uiSTCCRegValue_32b & 0x00000002) >> 1 );
	p->bLatch_En 	= ( (uiSTCCRegValue_32b & 0x00000001) );

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetSTCCStatus");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   get the current STC ASG value.
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
int SDEC_IO_GetSTCCASGStatus
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GET_STCCASG_T *stpLXSdecGetSTCCASG)
{
	int eRet = RET_ERROR;
	UINT32 ui32ASGRegValue = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecGetSTCCASG == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetSTCCASGStatus");

	ui32ASGRegValue = SDEC_HAL_STCCGetASG(_SDEC_CORE_0); /* M14_TBD , H14_TBD */

	stpLXSdecGetSTCCASG->ui8Main = (ui32ASGRegValue & 0x01);
	stpLXSdecGetSTCCASG->ui8Aud1 = ((ui32ASGRegValue & 0x80000) >> 19);
	stpLXSdecGetSTCCASG->ui8Aud0 = ((ui32ASGRegValue & 0x40000) >> 18);
	stpLXSdecGetSTCCASG->ui8Vid1 = ((ui32ASGRegValue & 0x20000) >> 17);
	stpLXSdecGetSTCCASG->ui8Vid0 = ((ui32ASGRegValue & 0x10000) >> 16);

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetSTCCASGStatus");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   set the current STC ASG value.
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
int SDEC_IO_SetSTCCASGStatus
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GET_STCCASG_T *stpLXSdecSetSTCCASG)
{
	int eRet = RET_ERROR;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSetSTCCASG == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SetSTCCASGStatus");

	/* At this time, only implemented to assign main PCR Channel , Later all fields will be added */
	SDEC_HAL_STCCSetMain(_SDEC_CORE_0, stpLXSdecSetSTCCASG->ui8Main); /* M14_TBD, H14_TBD */

	SDEC_PRINT_TRACE( "-->SDEC_IO_SetSTCCASGStatus");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   get the current STC value.
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
int SDEC_IO_GetCurrentSTCPCR
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GET_STC_PCR_T *stpLXSdecGetSTCPCR)
{
	int eRet = RET_ERROR;
	LX_SDEC_CFG_T *pSdecConf = SDEC_CFG_GetConfig();
	UINT8 ui8Ch = 0x0;
	UINT8 core = 0;
	UINT32 val[2];

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecGetSTCPCR == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	ui8Ch = stpLXSdecGetSTCPCR->eCh;
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, return RET_INVALID_PARAMS, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE(!SDEC_IS_NORMAL_CHANNEL(ui8Ch), return RET_INVALID_PARAMS, "Not normal channel %d", ui8Ch);

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetCurrentSTC");

	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	SDEC_HAL_STCCGetLatchedSTC(core, ui8Ch, &val[0], &val[1]);
	stpLXSdecGetSTCPCR->uiStc_hi = val[0];
	stpLXSdecGetSTCPCR->uiStc_lo = val[1];

	SDEC_HAL_STCCGetPCR(core, ui8Ch, &val[0], &val[1]);
	stpLXSdecGetSTCPCR->uiPcr_hi = val[0];
	stpLXSdecGetSTCPCR->uiPcr_lo = val[1];

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetCurrentSTC");

	eRet = RET_OK;
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   get the current Live STC value.
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
int SDEC_IO_GetLiveSTC(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_STC_T *stc)
{
	int eRet = RET_ERROR;
	LX_SDEC_CFG_T *cfg = SDEC_CFG_GetConfig();
	UINT8 core, ch;
	UINT32 val[2];

	LX_SDEC_CHECK_CODE(stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE(stc == NULL, return RET_INVALID_PARAMS, "Invalid argument" );
	LX_SDEC_CHECK_CODE(stc->lxch >= cfg->nChannel, return RET_INVALID_PARAMS, "over channel range %d", stc->lxch);
	LX_SDEC_CHECK_CODE(!SDEC_IS_NORMAL_CHANNEL(stc->lxch), return RET_INVALID_PARAMS, "Not normal channel %d", stc->lxch);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetLiveSTC");

	SDEC_LXCH_TO_CORE_CH(stc->lxch, core, ch);

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_HAL_STCCGetLiveSTC(core, ch, &val[0], &val[1]);
	stc->stc_41_10	= val[0];
	stc->stc_9_0	= val[1];

	eRet = RET_OK;
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	SDEC_PRINT_TRACE( "-->SDEC_IO_GetLiveSTC");

	return (eRet);
}




/**
********************************************************************************
* @brief
*   get the current GSTC value.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   	unsigned long *pcr
*	unsigned long *stc
* @return
*  int
********************************************************************************
*/
int SDEC_IO_GetCurrentGSTC
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GET_GSTC_T *stpLXSdecGetGSTC)
{
	int eRet = RET_ERROR;
	UINT32 gstc_sdec[2] = { 0, 0 };

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecGetGSTC == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetCurrentGSTC");

	SDEC_HAL_GSTC(_SDEC_CORE_0, &gstc_sdec[0], &gstc_sdec[1]);

	stpLXSdecGetGSTC->gstc_32_1 	= gstc_sdec[0];
	stpLXSdecGetGSTC->gstc_0 		= gstc_sdec[1];

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetCurrentGSTC");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   get the current GSTC value. (New Version)
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   	unsigned long *pcr
*	unsigned long *stc
* @return
*  int
********************************************************************************
*/
int SDEC_IO_GetCurrentGSTC32
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GET_GSTC32_T *stpLXSdecGetGSTC)
{
	int eRet = RET_ERROR;
	UINT32 gstc_sdec[2] = { 0, 0 };
	UINT8 core = 0, index = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecGetGSTC == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	core = stpLXSdecGetGSTC->core;
	index = stpLXSdecGetGSTC->index;

	/* argument check */
	LX_SDEC_CHECK_CODE( core > _SDEC_CORE_1, goto exit, "over core range %d", core);
	LX_SDEC_CHECK_CODE( index > 1 /* MAX GSTC Num */, goto exit, "over GSTC index range %d", index);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetCurrentGSTC32");

	if(index == 0)	SDEC_HAL_GSTC(core, &gstc_sdec[0], &gstc_sdec[1]);
	else			SDEC_HAL_GSTC1(core, &gstc_sdec[0], &gstc_sdec[1]);

	stpLXSdecGetGSTC->gstc_32_1 	= gstc_sdec[0];
	stpLXSdecGetGSTC->gstc_0 		= gstc_sdec[1];

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetCurrentGSTC32");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   set the current GSTC value.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
* @return
*  int
********************************************************************************
*/
int SDEC_IO_SetCurrentGSTC
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SET_GSTC_T *stpLXSdecSetGSTC)
{
	int eRet = RET_ERROR;
	int ret = -1;
	UINT8 index = 0;
	UINT8 core = 0;
	UINT32 gstc_base = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSetGSTC == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	core = stpLXSdecSetGSTC->core;
	index = stpLXSdecSetGSTC->index;
	gstc_base = stpLXSdecSetGSTC->gstc_base;

	/* argument check */
	LX_SDEC_CHECK_CODE( core > _SDEC_CORE_1, goto exit, "over core range %d", core);
	LX_SDEC_CHECK_CODE( index > 1 /* MAX GSTC Num */, goto exit, "over GSTC index range %d", index);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SetCurrentGSTC [%d][%d][0x%x]", core, index, gstc_base);

	ret = SDEC_HAL_GSTCSetValue(core, index, gstc_base);
	LX_SDEC_CHECK_CODE( ret != RET_OK, goto exit, "SDEC_HAL_GSTCSetValue error [%d][%d]", core, index);

	SDEC_PRINT_TRACE( "-->SDEC_IO_SetCurrentGSTC");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}



/**
********************************************************************************
* @brief
*   select input port.
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
int SDEC_IO_SelInputPort
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SEL_INPUT_T *stpLXSdecSelPort)
{
	int eRet = RET_ERROR, eResult = RET_ERROR;
	UINT8 ui8Ch = 0x0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSelPort == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	SDEC_PRINT_TRACE( "<--SDEC_IO_SelInputPort");

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	ui8Ch = stpLXSdecSelPort->eCh;

	/* argument check */
	LX_SDEC_CHECK_CODE( ui8Ch >= stpSdecParam->cfg->nChannel, goto exit, "over channel range %d", ui8Ch);

	/* CDIC Set Src and Reset */
	eResult = SDEC_InputSet(stpSdecParam, stpLXSdecSelPort);
	LX_SDEC_CHECK_CODE( LX_IS_ERR(eResult), goto exit, "SDEC_InputSet failed:[%d]", eResult);

	/* Error Interupt Setting */
	eResult = SDEC_ERR_Intr_Set(stpSdecParam, ui8Ch);
	LX_SDEC_CHECK_CODE( LX_IS_ERR(eResult), goto exit, "SDEC_ERR_Intr_Set failed:[%d]", eResult);

	/* TPI Interrupt Setting, Only for Normal Channel no regiseter in C,D,G,H */
	if(SDEC_IS_NORMAL_CHANNEL(ui8Ch))
	{
		/**	caution
		*	should set after input port set. Because if u set this register before input port set, and that register all bit clear
		*/
		eResult = SDEC_TPI_Set(stpSdecParam, ui8Ch);
		LX_SDEC_CHECK_CODE( LX_IS_ERR(eResult), goto exit, "SDEC_TPI_Set failed:[%d]", eResult);
	}
	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	SDEC_PRINT_TRACE( "-->SDEC_IO_SelInputPort");

	return (eRet);
}

/**
********************************************************************************
* @brief
*   select input port.
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
int SDEC_IO_CfgInputPort
	(S_SDEC_PARAM_T *sparam,
	LX_SDEC_CFG_INPUT_PARAM_T *stpLXSdecCfgPortParam)
{
	int eRet = RET_ERROR;
	LX_SDEC_CFG_INPUT_T *cfgInput;
	UINT8 lxch, core, ch;
	UINT8 newCDIP;

	LX_SDEC_CHECK_CODE( sparam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecCfgPortParam == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&sparam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_CfgInputPort");

	lxch = stpLXSdecCfgPortParam->ui8Ch;
	SDEC_CHECK_CHANNEL(lxch, goto exit);

	SDEC_LXCH_TO_CORE_CH(lxch, core, ch);
	cfgInput = &(stpLXSdecCfgPortParam->input_param);

	newCDIP = sparam->cfg->newCDIP;

	switch (cfgInput->eInputPort)
	{
		case LX_SDEC_INPUT_PARALLEL0:
		case LX_SDEC_INPUT_PARALLEL4:
			SDEC_PRINT_IO( "PARALLEL0");
			if(newCDIP)
			{
				SDEC_HAL_CDIP(core, 0, cfgInput);
			}
			else
			{
				SDEC_HAL_CDIP(core, 0, cfgInput);
				SDEC_HAL_CDIP(core, 1, NULL);
				SDEC_HAL_CDIP(core, 2, NULL);
			}
			SDEC_HAL_CDIPn_2ND(core, 0, NULL);
			break;

		case LX_SDEC_INPUT_PARALLEL1:
		case LX_SDEC_INPUT_PARALLEL5:
			SDEC_PRINT_IO( "PARALLEL1");
			if(newCDIP)
			{
				SDEC_HAL_CDIP(core, 1, cfgInput);
			}
			else
			{
				SDEC_HAL_CDIPA(core, 0, cfgInput);
				SDEC_HAL_CDIPA(core, 1, NULL);
				SDEC_HAL_CDIPA(core, 2, NULL);
			}
			SDEC_HAL_CDIPn_2ND(core, 1, NULL);
			break;

		case LX_SDEC_INPUT_SERIAL0:
		case LX_SDEC_INPUT_SERIAL4:
			SDEC_PRINT_IO( "SERIAL0");
			if(newCDIP)
			{
				SDEC_HAL_CDIP(core, 0, cfgInput);
			}
			else
			{
				SDEC_HAL_CDIP(core, 0, cfgInput);
				SDEC_HAL_CDIP(core, 1, NULL);
				SDEC_HAL_CDIP(core, 2, NULL);
			}

			SDEC_HAL_CDIPn_2ND(core, 0, cfgInput);
			break;

		case LX_SDEC_INPUT_SERIAL1:
		case LX_SDEC_INPUT_SERIAL5:
			SDEC_PRINT_IO( "SERIAL1");
			if(newCDIP)
			{
				SDEC_HAL_CDIP(core, 1, cfgInput);
			}
			else
			{
				SDEC_HAL_CDIPA(core, 0, cfgInput);
				SDEC_HAL_CDIPA(core, 1, NULL);
				SDEC_HAL_CDIPA(core, 2, NULL);
			}

			SDEC_HAL_CDIPn_2ND(core, 1, cfgInput);
			break;

		case LX_SDEC_INPUT_SERIAL2:
		case LX_SDEC_INPUT_SERIAL6:
			if(newCDIP)
			{
				SDEC_HAL_CDIP(core, 2, cfgInput);
			}
			else
			{
				/* CDIN3 characteristic is set to CDIP[3] */
				SDEC_HAL_CDIP(core, 3, cfgInput);
			}

			SDEC_HAL_CDIPn_2ND(core, 2, cfgInput);
			break;

		case LX_SDEC_INPUT_SERIAL3:
		case LX_SDEC_INPUT_SERIAL7:
			if(newCDIP)
			{
				SDEC_HAL_CDIP(core, 3, cfgInput);
			}
			else
			{
				/* CDIN4 characteristic is set to CDIOP[0] */
				SDEC_HAL_CDIOP(core, 0, cfgInput);
			}

			SDEC_HAL_CDIPn_2ND(core, 3, cfgInput);
			break;

		default:
			SDEC_PRINT_ERROR("Invalid channel:[%d]", cfgInput->eInputPort);
			goto exit;
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_CfgInputPort");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&sparam->stSdecMutex);
	return (eRet);
}

int SDEC_IO_SelInputSource(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SEL_INPUT_SRC_T *inputSrc)
{
	int	eRet		= RET_ERROR;
	LX_SDEC_CFG_T	*cfg = SDEC_CFG_GetConfig();

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( inputSrc == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SelInputSource. inTsSelector:%d, input:%d, source:%d",
		cfg->inTsSelector, inputSrc->input, inputSrc->source);

	if(cfg->inTsSelector)
	{
		UINT8 core, port, src_sel;

		switch(inputSrc->input)
		{
			case LX_SDEC_INPUT_SERIAL0:
			case LX_SDEC_INPUT_SERIAL1:
			case LX_SDEC_INPUT_SERIAL2:
			case LX_SDEC_INPUT_SERIAL3:
				core = 0;
				port = inputSrc->input - LX_SDEC_INPUT_SERIAL0;
				break;

			case LX_SDEC_INPUT_PARALLEL0:
			case LX_SDEC_INPUT_PARALLEL1:
				core = 0;
				port = inputSrc->input - LX_SDEC_INPUT_PARALLEL0;
				break;

			case LX_SDEC_INPUT_SERIAL4:
			case LX_SDEC_INPUT_SERIAL5:
			case LX_SDEC_INPUT_SERIAL6:
			case LX_SDEC_INPUT_SERIAL7:
				core = 1;
				port = inputSrc->input - LX_SDEC_INPUT_SERIAL4;
				break;

			case LX_SDEC_INPUT_PARALLEL4:
			case LX_SDEC_INPUT_PARALLEL5:
				core = 1;
				port = inputSrc->input - LX_SDEC_INPUT_PARALLEL4;
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Input Port:[%d]", inputSrc->input);
				goto exit;
		}

		switch (inputSrc->source)
		{
			case LX_SDEC_SRC_IN_DEMOD0:
				SDEC_PRINT_IO( "LX_SDEC_SRC_IN_DEMOD0 [P%d]", port);
				src_sel = SDEC_HAL_IN_EXT_DEMOD;
				break;

			case LX_SDEC_SRC_EXT_INPUT0:
				SDEC_PRINT_IO( "LX_SDEC_SRC_EXT_INPUT0 [P%d]", port);
				src_sel = SDEC_HAL_IN_EXT_PARALLEL0;
				break;

			case LX_SDEC_SRC_EXT_INPUT1:
				SDEC_PRINT_IO( "LX_SDEC_SRC_EXT_INPUT1 [P%d]", port);
				src_sel = SDEC_HAL_IN_EXT_PARALLEL1;
				break;

			case LX_SDEC_SRC_EXT_INPUT2:
				SDEC_PRINT_IO( "LX_SDEC_SRC_EXT_INPUT2 [P%d]", port);
				src_sel = SDEC_HAL_IN_EXT_SERIAL;
				break;

			case LX_SDEC_SRC_CI_INPUT:
				SDEC_PRINT_IO( "LX_SDEC_SRC_CI_INPUT [P%d]", port);
				src_sel = SDEC_HAL_IN_EXT_CI_INPUT;
				break;

			case LX_SDEC_SRC_CI_OUTPUT:
				SDEC_PRINT_IO( "LX_SDEC_SRC_CI_OUTPUT [P%d]", port);
				src_sel = SDEC_HAL_IN_EXT_CI_OUTPUT;
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Source:[%d]", inputSrc->source);
				goto exit;
			break;
		}
		SDEC_HAL_SetTsSelector(core, port, SDEC_HAL_TS_INPUT_EXTERNAL, src_sel);
	}
	else goto exit;

	SDEC_PRINT_TRACE( "-->SDEC_IO_SelParInput");
	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);

}


int SDEC_IO_GetInputSource(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SEL_INPUT_SRC_T *inputSrc)
{
	int eRet 		= RET_ERROR;
	LX_SDEC_CFG_T	*cfg = SDEC_CFG_GetConfig();

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( inputSrc == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetInputSource");

	if(cfg->inTsSelector)
	{
		UINT8 core, port, main_src, sub_src;

		switch(inputSrc->input)
		{
			case LX_SDEC_INPUT_SERIAL0:
			case LX_SDEC_INPUT_SERIAL1:
			case LX_SDEC_INPUT_SERIAL2:
			case LX_SDEC_INPUT_SERIAL3:
				core = 0;
				port = inputSrc->input - LX_SDEC_INPUT_SERIAL0;
				break;

			case LX_SDEC_INPUT_PARALLEL0:
			case LX_SDEC_INPUT_PARALLEL1:
				core = 0;
				port = inputSrc->input - LX_SDEC_INPUT_PARALLEL0;
				break;

			case LX_SDEC_INPUT_SERIAL4:
			case LX_SDEC_INPUT_SERIAL5:
			case LX_SDEC_INPUT_SERIAL6:
			case LX_SDEC_INPUT_SERIAL7:
				core = 1;
				port = inputSrc->input - LX_SDEC_INPUT_SERIAL4;
				break;

			case LX_SDEC_INPUT_PARALLEL4:
			case LX_SDEC_INPUT_PARALLEL5:
				core = 1;
				port = inputSrc->input - LX_SDEC_INPUT_PARALLEL4;
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Input Port:[%d]", inputSrc->input);
				goto exit;
		}

		SDEC_HAL_GetTsSelector(core, port, &main_src, &sub_src);

		SDEC_PRINT_IO( "CORE[%d],PORT[%d], MAIN[%d], SUB[%d]", core, port, main_src, sub_src);
		switch(sub_src)
		{
			case SDEC_HAL_IN_EXT_DEMOD:
				inputSrc->source = LX_SDEC_SRC_IN_DEMOD0;
				break;

			case SDEC_HAL_IN_EXT_PARALLEL0:
				inputSrc->source = LX_SDEC_SRC_EXT_INPUT0;
				break;

			case SDEC_HAL_IN_EXT_PARALLEL1:
				inputSrc->source = LX_SDEC_SRC_EXT_INPUT1;
				break;

			case SDEC_HAL_IN_EXT_SERIAL:
				inputSrc->source = LX_SDEC_SRC_EXT_INPUT2;
				break;

			case SDEC_HAL_IN_EXT_CI_INPUT:
				inputSrc->source = LX_SDEC_SRC_CI_INPUT;
				break;

			case SDEC_HAL_IN_EXT_CI_OUTPUT:
				inputSrc->source = LX_SDEC_SRC_CI_OUTPUT;
				break;

			default:
				inputSrc->source = LX_SDEC_SRC_NONE;
				break;
		}
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetInputSource");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   select input port.
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
int SDEC_IO_SelParInput
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SEL_PAR_INPUT_T *stpLXSdecParInput)
{
	int 	eRet 		= RET_ERROR;
	LX_SDEC_INPUT_T	eInputPort;
	UINT8			ui4ts_sel	= 0x0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecParInput == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SelParInput");

	eInputPort = stpLXSdecParInput->eInputPort;

	if(0){}
	else if(lx_chip() == LX_CHIP_H15)
	{
		// TODO: will remove
		UINT8 port, src_sel;

		if(eInputPort >= LX_SDEC_INPUT_PARALLEL0 &&
			eInputPort <= LX_SDEC_INPUT_PARALLEL1)
		{
			port = eInputPort - LX_SDEC_INPUT_PARALLEL0;
		}
		else
		{
			SDEC_PRINT_ERROR("Invalid Input Port:[%d]", eInputPort);
			goto exit;
		}

		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD [P%d]", port);
				src_sel = SDEC_HAL_IN_EXT_DEMOD;
				break;

			case LX_SDEC_EXTERNAL_DEMOD0:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD [P%d]", port);
				src_sel = SDEC_HAL_IN_EXT_PARALLEL0;
				break;

			case LX_SDEC_EXTERNAL_DEMOD1:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD [P%d]", port);
				src_sel = SDEC_HAL_IN_EXT_PARALLEL1;
				break;

			case LX_SDEC_CI_INPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_INPUT [P%d]", port);
				CTOP_CTRL_H15A0_RdFL(VIP, ctr36);
				CTOP_CTRL_H15A0_Wr01(VIP, ctr36, tpio_sel_ctrl, 1);	/* Set Parallel IO as Input */
				CTOP_CTRL_H15A0_WrFL(VIP, ctr36);
				src_sel = SDEC_HAL_IN_EXT_CI_INPUT;
				break;

			case LX_SDEC_CI_OUTPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_OUTPUT [P%d]", port);
				CTOP_CTRL_H15A0_RdFL(VIP, ctr36);
				CTOP_CTRL_H15A0_Wr01(VIP, ctr36, tpio_sel_ctrl, 0); /* Set Parallel IO as Output */
				CTOP_CTRL_H15A0_WrFL(VIP, ctr36);
				src_sel = SDEC_HAL_IN_EXT_CI_OUTPUT;
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
		SDEC_HAL_SetTsSelector(0, port, SDEC_HAL_TS_INPUT_EXTERNAL, src_sel);
		SDEC_HAL_SetTsSelector(1, port, SDEC_HAL_TS_INPUT_EXTERNAL, src_sel);	// TODO: how to set core1

	}
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		/*--------------------------------------------------------
			--> ORIGINAL Design Concept but not real
				(Please Check After H13A0!!, they might be changed to original)
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [5:4])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [7:6])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In

			--> H13 A0 Ctr58 Bits are Crossed Abnormally as follows
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [7:6])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [5:4])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In
		   --------------------------------------------------------*/
		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD [P%d]", eInputPort - 2);

				CTOP_CTRL_H14A0_RdFL(ctr58);
				CTOP_CTRL_H14A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC; /* XX00 */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3; /*00XX */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_H14A0_WrFL(ctr58);
				break;
			case LX_SDEC_EXTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD [P%d]", eInputPort - 2);

				CTOP_CTRL_H14A0_RdFL(ctr58);
				CTOP_CTRL_H14A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x1;	/* XX01 */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x4;	/* 01XX */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_H14A0_WrFL(ctr58);
				break;
			case LX_SDEC_CI_INPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_INPUT [P%d]", eInputPort - 2);

				CTOP_CTRL_H14A0_RdFL(ctr58);
				CTOP_CTRL_H14A0_Wr01(ctr58, tpio_sel_ctrl, 1);	/* Set Parallel IO as Input ?? jinhwan.bae -> yes, so if you need ci input signal, select external one instead of CI_INPUT */
				// CTOP_CTRL_H13A0_Wr01(ctr58, tpio_sel_ctrl, 0); /* Always Output !We want to get output data ??  */
				CTOP_CTRL_H14A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x3;	/* XX11 */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x8;	/* 10XX */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_H14A0_WrFL(ctr58);
				break;
			case LX_SDEC_CI_OUTPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_OUTPUT [P%d]", eInputPort - 2);

				CTOP_CTRL_H14A0_RdFL(ctr58);
				CTOP_CTRL_H14A0_Wr01(ctr58, tpio_sel_ctrl, 0); /* CI_INPUT port changed to Always Output !We want to get output data ??  */
				CTOP_CTRL_H14A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x2;	/* XX10 */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0xC;	/* 11XX */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_H14A0_WrFL(ctr58);
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		/*--------------------------------------------------------
			--> ORIGINAL Design Concept but not real
				(Please Check After H13A0!!, they might be changed to original)
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [5:4])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [7:6])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In

			--> H13 A0 Ctr58 Bits are Crossed Abnormally as follows
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [7:6])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [5:4])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In
		   --------------------------------------------------------*/
		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD [P%d]", eInputPort - 2);

				CTOP_CTRL_M14B0_RdFL(TOP, ctr19);
				CTOP_CTRL_M14B0_Rd01(TOP, ctr19, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC; /* XX00 */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3; /*00XX */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_M14B0_WrFL(TOP, ctr19);
				break;
			case LX_SDEC_EXTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD [P%d]", eInputPort - 2);

				CTOP_CTRL_M14B0_RdFL(TOP, ctr19);
				CTOP_CTRL_M14B0_Rd01(TOP, ctr19, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x1;	/* XX01 */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x4;	/* 01XX */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_M14B0_WrFL(TOP, ctr19);
				break;
			case LX_SDEC_CI_INPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_INPUT [P%d]", eInputPort - 2);

				CTOP_CTRL_M14B0_RdFL(TOP, ctr08);
				CTOP_CTRL_M14B0_Wr01(TOP, ctr08, tpio_sel_ctrl, 1);	/* Set Parallel IO as Input ?? jinhwan.bae -> yes, so if you need ci input signal, select external one instead of CI_INPUT */
				CTOP_CTRL_M14B0_WrFL(TOP, ctr08);

				CTOP_CTRL_M14B0_RdFL(TOP, ctr19);
				CTOP_CTRL_M14B0_Rd01(TOP, ctr19, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x3;	/* XX11 */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x8;	/* 10XX */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_M14B0_WrFL(TOP, ctr19);
				break;
			case LX_SDEC_CI_OUTPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_OUTPUT [P%d]", eInputPort - 2);

				CTOP_CTRL_M14B0_RdFL(TOP, ctr08);
				CTOP_CTRL_M14B0_Wr01(TOP, ctr08, tpio_sel_ctrl, 0);	/* CI_INPUT port changed to Always Output !We want to get output data ??  */
				CTOP_CTRL_M14B0_WrFL(TOP, ctr08);

				CTOP_CTRL_M14B0_RdFL(TOP, ctr19);
				CTOP_CTRL_M14B0_Rd01(TOP, ctr19, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x2;	/* XX10 */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0xC;	/* 11XX */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_M14B0_WrFL(TOP, ctr19);
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		/*--------------------------------------------------------
			--> ORIGINAL Design Concept but not real
				(Please Check After H13A0!!, they might be changed to original)
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [5:4])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [7:6])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In

			--> H13 A0 Ctr58 Bits are Crossed Abnormally as follows
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [7:6])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [5:4])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In
		   --------------------------------------------------------*/
		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD [P%d]", eInputPort - 2);

				CTOP_CTRL_M14A0_RdFL(ctr58);
				CTOP_CTRL_M14A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC; /* XX00 */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3; /*00XX */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_M14A0_WrFL(ctr58);
				break;
			case LX_SDEC_EXTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD [P%d]", eInputPort - 2);

				CTOP_CTRL_M14A0_RdFL(ctr58);
				CTOP_CTRL_M14A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x1;	/* XX01 */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x4;	/* 01XX */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_M14A0_WrFL(ctr58);
				break;
			case LX_SDEC_CI_INPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_INPUT [P%d]", eInputPort - 2);

				CTOP_CTRL_M14A0_RdFL(ctr58);
				CTOP_CTRL_M14A0_Wr01(ctr58, tpio_sel_ctrl, 1);	/* Set Parallel IO as Input ?? jinhwan.bae -> yes, so if you need ci input signal, select external one instead of CI_INPUT */
				// CTOP_CTRL_H13A0_Wr01(ctr58, tpio_sel_ctrl, 0); /* Always Output !We want to get output data ??  */
				CTOP_CTRL_M14A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x3;	/* XX11 */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x8;	/* 10XX */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_M14A0_WrFL(ctr58);
				break;
			case LX_SDEC_CI_OUTPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_OUTPUT [P%d]", eInputPort - 2);

				CTOP_CTRL_M14A0_RdFL(ctr58);
				CTOP_CTRL_M14A0_Wr01(ctr58, tpio_sel_ctrl, 0); /* CI_INPUT port changed to Always Output !We want to get output data ??  */
				CTOP_CTRL_M14A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x2;	/* XX10 */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0xC;	/* 11XX */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_M14A0_WrFL(ctr58);
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		/*--------------------------------------------------------
			--> ORIGINAL Design Concept but not real
				(Please Check After H13A0!!, they might be changed to original)
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [5:4])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [7:6])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In

			--> H13 A0 Ctr58 Bits are Crossed Abnormally as follows
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [7:6])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [5:4])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In
		   --------------------------------------------------------*/
		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD [P%d]", eInputPort - 2);

				CTOP_CTRL_H13A0_RdFL(ctr58);
				CTOP_CTRL_H13A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC; /* XX00 */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3; /*00XX */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_H13A0_WrFL(ctr58);
				break;
			case LX_SDEC_EXTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD [P%d]", eInputPort - 2);

				CTOP_CTRL_H13A0_RdFL(ctr58);
				CTOP_CTRL_H13A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x1;	/* XX01 */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x4;	/* 01XX */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_H13A0_WrFL(ctr58);
				break;
			case LX_SDEC_CI_INPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_INPUT [P%d]", eInputPort - 2);

				CTOP_CTRL_H13A0_RdFL(ctr58);
				CTOP_CTRL_H13A0_Wr01(ctr58, tpio_sel_ctrl, 1);	/* Set Parallel IO as Input ?? jinhwan.bae -> yes, so if you need ci input signal, select external one instead of CI_INPUT */
				// CTOP_CTRL_H13A0_Wr01(ctr58, tpio_sel_ctrl, 0); /* Always Output !We want to get output data ??  */
				CTOP_CTRL_H13A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x3;	/* XX11 */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x8;	/* 10XX */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_H13A0_WrFL(ctr58);
				break;
			case LX_SDEC_CI_OUTPUT:
				SDEC_PRINT_IO( "LX_SDEC_CI_OUTPUT [P%d]", eInputPort - 2);

				CTOP_CTRL_H13A0_RdFL(ctr58);
				CTOP_CTRL_H13A0_Wr01(ctr58, tpio_sel_ctrl, 0); /* CI_INPUT port changed to Always Output !We want to get output data ??  */
				CTOP_CTRL_H13A0_Rd01(ctr58, ts_sel, ui4ts_sel);

				if( eInputPort == LX_SDEC_INPUT_PARALLEL1 )
				{
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x2;	/* XX10 */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}
				else if ( eInputPort == LX_SDEC_INPUT_PARALLEL0 )
				{
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0xC;	/* 11XX */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
				}

				CTOP_CTRL_H13A0_WrFL(ctr58);
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
	}
#endif

	SDEC_PRINT_TRACE( "-->SDEC_IO_SelParInput");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   select input port.
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
int SDEC_IO_SelCiInput
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SEL_CI_INPUT_T *stpLXSdecParInput)
{
	int 	eRet = RET_ERROR;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecParInput == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SelCiInput");
	SDEC_PRINT_IO( "%s : stpLXSdecParInput->eParInput = %d", __FUNCTION__, stpLXSdecParInput->eParInput);

	if(0){}
	else if(lx_chip() == LX_CHIP_H15)
	{
		CTOP_CTRL_H15A0_RdFL(VIP, ctr36);
		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD");
				CTOP_CTRL_H15A0_Wr01(VIP, ctr36, tpo_sel_ctrl, 0);
				break;

			case LX_SDEC_EXTERNAL_DEMOD0:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD0");
				CTOP_CTRL_H15A0_Wr01(VIP, ctr36, tpo_sel_ctrl, 1);
				break;

			case LX_SDEC_EXTERNAL_DEMOD1:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD1");
				CTOP_CTRL_H15A0_Wr01(VIP, ctr36, tpo_sel_ctrl, 2);
				break;

			case LX_SDEC_INTERNAL_TE:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_TE");
				CTOP_CTRL_H15A0_Wr01(VIP, ctr36, tpo_sel_ctrl, 3);
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
		CTOP_CTRL_H15A0_WrFL(VIP, ctr36);

	}
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD");

				CTOP_CTRL_H14A0_RdFL(ctr58);
				CTOP_CTRL_H14A0_Wr01(ctr58, tpo_sel_ctrl, 0);
				CTOP_CTRL_H14A0_WrFL(ctr58);

				break;
			case LX_SDEC_EXTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD");

				CTOP_CTRL_H14A0_RdFL(ctr58);
				CTOP_CTRL_H14A0_Wr01(ctr58, tpo_sel_ctrl, 1);
				CTOP_CTRL_H14A0_WrFL(ctr58);

				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CTOP_CTRL_M14B0_RdFL(TOP, ctr08);
		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD");
				CTOP_CTRL_M14B0_Wr01(TOP, ctr08, tpo_sel_ctrl, 0);
				break;
			case LX_SDEC_EXTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD");
				CTOP_CTRL_M14B0_Wr01(TOP, ctr08, tpo_sel_ctrl, 1);
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
		CTOP_CTRL_M14B0_WrFL(TOP, ctr08);
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD");

				CTOP_CTRL_M14A0_RdFL(ctr58);
				CTOP_CTRL_M14A0_Wr01(ctr58, tpo_sel_ctrl, 0);
				CTOP_CTRL_M14A0_WrFL(ctr58);

				break;
			case LX_SDEC_EXTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD");

				CTOP_CTRL_M14A0_RdFL(ctr58);
				CTOP_CTRL_M14A0_Wr01(ctr58, tpo_sel_ctrl, 1);
				CTOP_CTRL_M14A0_WrFL(ctr58);

				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		switch (stpLXSdecParInput->eParInput)
		{
			case LX_SDEC_INTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_INTERNAL_DEMOD");

				CTOP_CTRL_H13A0_RdFL(ctr58);
				CTOP_CTRL_H13A0_Wr01(ctr58, tpo_sel_ctrl, 0);
				CTOP_CTRL_H13A0_WrFL(ctr58);

				break;
			case LX_SDEC_EXTERNAL_DEMOD:
				SDEC_PRINT_IO( "LX_SDEC_EXTERNAL_DEMOD");

				CTOP_CTRL_H13A0_RdFL(ctr58);
				CTOP_CTRL_H13A0_Wr01(ctr58, tpo_sel_ctrl, 1);
				CTOP_CTRL_H13A0_WrFL(ctr58);

				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", stpLXSdecParInput->eParInput);
				goto exit;
			break;
		}
	}
#endif

	SDEC_PRINT_TRACE( "-->SDEC_IO_SelCiInput");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   select input port.
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
int SDEC_IO_GetParInput
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GET_PAR_INPUT_T *stpLXSdecParInput)
{
	int 	eRet 		= RET_ERROR;
	LX_SDEC_INPUT_T	eInputPort;
	UINT8			ui4ts_sel	= 0x0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecParInput == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetParInput");

	eInputPort = stpLXSdecParInput->eInputPort;

	if(0){}
	else if(lx_chip() == LX_CHIP_H15)
	{
		UINT8 port, main_src, sub_src;

		if(eInputPort >= LX_SDEC_INPUT_PARALLEL0 &&
			eInputPort <= LX_SDEC_INPUT_PARALLEL1)
		{
			port = eInputPort - LX_SDEC_INPUT_PARALLEL0;
		}
		else
		{
			SDEC_PRINT_ERROR("Invalid Input Port:[%d]", eInputPort);
			goto exit;
		}
		SDEC_HAL_GetTsSelector(0, port, &main_src, &sub_src);

		switch(sub_src)
		{
			case SDEC_HAL_IN_EXT_DEMOD:
				stpLXSdecParInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
				break;
			case SDEC_HAL_IN_EXT_PARALLEL0:
				stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD0;
				break;
			case SDEC_HAL_IN_EXT_PARALLEL1:
				stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD1;
				break;
			case SDEC_HAL_IN_EXT_CI_INPUT:
				stpLXSdecParInput->eParInput = LX_SDEC_CI_INPUT;
				break;
			case SDEC_HAL_IN_EXT_CI_OUTPUT:
				stpLXSdecParInput->eParInput = LX_SDEC_CI_OUTPUT;
				break;
			default:
				break;
		}
	}
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		/*--------------------------------------------------------
			--> ORIGINAL Design Concept but not real
				(Please Check After H13A0!!, they might be changed to original)
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [5:4])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [7:6])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In

			--> H13 A0 Ctr58 Bits are Crossed Abnormally as follows
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [7:6])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [5:4])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In
		   --------------------------------------------------------*/
		switch (eInputPort)
		{
			case LX_SDEC_INPUT_PARALLEL0:
				CTOP_CTRL_H14A0_RdFL(ctr58);
				CTOP_CTRL_H14A0_Rd01(ctr58, ts_sel, ui4ts_sel);
				ui4ts_sel = ( (ui4ts_sel & 0xC) >> 2 ); /*VVxx */
				if(ui4ts_sel == 0) stpLXSdecParInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
				else if(ui4ts_sel == 1) stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
				else if(ui4ts_sel == 2) stpLXSdecParInput->eParInput = LX_SDEC_CI_INPUT;
				else if(ui4ts_sel == 3) stpLXSdecParInput->eParInput = LX_SDEC_CI_OUTPUT;
				break;

			case LX_SDEC_INPUT_PARALLEL1:
				CTOP_CTRL_H14A0_RdFL(ctr58);
				CTOP_CTRL_H14A0_Rd01(ctr58, ts_sel, ui4ts_sel);
				ui4ts_sel = (ui4ts_sel & 0x3); /*xxVV */
				if(ui4ts_sel == 0) stpLXSdecParInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
				else if(ui4ts_sel == 1) stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
				else if(ui4ts_sel == 2) stpLXSdecParInput->eParInput = LX_SDEC_CI_OUTPUT;
				else if(ui4ts_sel == 3) stpLXSdecParInput->eParInput = LX_SDEC_CI_INPUT;
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", eInputPort);
				break;
		}
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		/*--------------------------------------------------------
			--> ORIGINAL Design Concept but not real
				(Please Check After H13A0!!, they might be changed to original)
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [5:4])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [7:6])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In

			--> H13 A0 Ctr58 Bits are Crossed Abnormally as follows
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [7:6])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [5:4])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In
		   --------------------------------------------------------*/
		switch (eInputPort)
		{
			case LX_SDEC_INPUT_PARALLEL0:
				CTOP_CTRL_M14B0_RdFL(TOP, ctr19);
				CTOP_CTRL_M14B0_Rd01(TOP, ctr19, ts_sel, ui4ts_sel);
				ui4ts_sel = ( (ui4ts_sel & 0xC) >> 2 ); /*VVxx */
				if(ui4ts_sel == 0) stpLXSdecParInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
				else if(ui4ts_sel == 1) stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
				else if(ui4ts_sel == 2) stpLXSdecParInput->eParInput = LX_SDEC_CI_INPUT;
				else if(ui4ts_sel == 3) stpLXSdecParInput->eParInput = LX_SDEC_CI_OUTPUT;
				break;

			case LX_SDEC_INPUT_PARALLEL1:
				CTOP_CTRL_M14B0_RdFL(TOP, ctr19);
				CTOP_CTRL_M14B0_Rd01(TOP, ctr19, ts_sel, ui4ts_sel);
				ui4ts_sel = (ui4ts_sel & 0x3); /*xxVV */
				if(ui4ts_sel == 0) stpLXSdecParInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
				else if(ui4ts_sel == 1) stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
				else if(ui4ts_sel == 2) stpLXSdecParInput->eParInput = LX_SDEC_CI_OUTPUT;
				else if(ui4ts_sel == 3) stpLXSdecParInput->eParInput = LX_SDEC_CI_INPUT;
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", eInputPort);
				break;
		}
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		/*--------------------------------------------------------
			--> ORIGINAL Design Concept but not real
				(Please Check After H13A0!!, they might be changed to original)
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [5:4])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [7:6])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In

			--> H13 A0 Ctr58 Bits are Crossed Abnormally as follows
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [7:6])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [5:4])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In
		   --------------------------------------------------------*/
		switch (eInputPort)
		{
			case LX_SDEC_INPUT_PARALLEL0:
				CTOP_CTRL_M14A0_RdFL(ctr58);
				CTOP_CTRL_M14A0_Rd01(ctr58, ts_sel, ui4ts_sel);
				ui4ts_sel = ( (ui4ts_sel & 0xC) >> 2 ); /*VVxx */
				if(ui4ts_sel == 0) stpLXSdecParInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
				else if(ui4ts_sel == 1) stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
				else if(ui4ts_sel == 2) stpLXSdecParInput->eParInput = LX_SDEC_CI_INPUT;
				else if(ui4ts_sel == 3) stpLXSdecParInput->eParInput = LX_SDEC_CI_OUTPUT;
				break;

			case LX_SDEC_INPUT_PARALLEL1:
				CTOP_CTRL_M14A0_RdFL(ctr58);
				CTOP_CTRL_M14A0_Rd01(ctr58, ts_sel, ui4ts_sel);
				ui4ts_sel = (ui4ts_sel & 0x3); /*xxVV */
				if(ui4ts_sel == 0) stpLXSdecParInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
				else if(ui4ts_sel == 1) stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
				else if(ui4ts_sel == 2) stpLXSdecParInput->eParInput = LX_SDEC_CI_OUTPUT;
				else if(ui4ts_sel == 3) stpLXSdecParInput->eParInput = LX_SDEC_CI_INPUT;
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", eInputPort);
				break;
		}
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		/*--------------------------------------------------------
			--> ORIGINAL Design Concept but not real
				(Please Check After H13A0!!, they might be changed to original)
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [5:4])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [7:6])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In

			--> H13 A0 Ctr58 Bits are Crossed Abnormally as follows
			LX_SDEC_INPUT_PARALLEL0 (CDIN0 Selection) (Ctr58 [7:6])
			00b : Internal Demod 	01b : External Demod
			10b : tpo,bidir,CI In	11b : tpi, CI Out
			LX_SDEC_INPUT_PARALLEL1 (CDINA Selection) (Ctr58 [5:4])
			00b : Internal Demod	01b : External Demod
			10b : tpi, CI Out		11b : tpo,bidir,CI In
		   --------------------------------------------------------*/
		switch (eInputPort)
		{
			case LX_SDEC_INPUT_PARALLEL0:
				CTOP_CTRL_H13A0_RdFL(ctr58);
				CTOP_CTRL_H13A0_Rd01(ctr58, ts_sel, ui4ts_sel);
				ui4ts_sel = ( (ui4ts_sel & 0xC) >> 2 ); /*VVxx */
				if(ui4ts_sel == 0) stpLXSdecParInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
				else if(ui4ts_sel == 1) stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
				else if(ui4ts_sel == 2) stpLXSdecParInput->eParInput = LX_SDEC_CI_INPUT;
				else if(ui4ts_sel == 3) stpLXSdecParInput->eParInput = LX_SDEC_CI_OUTPUT;
				break;

			case LX_SDEC_INPUT_PARALLEL1:
				CTOP_CTRL_H13A0_RdFL(ctr58);
				CTOP_CTRL_H13A0_Rd01(ctr58, ts_sel, ui4ts_sel);
				ui4ts_sel = (ui4ts_sel & 0x3); /*xxVV */
				if(ui4ts_sel == 0) stpLXSdecParInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
				else if(ui4ts_sel == 1) stpLXSdecParInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
				else if(ui4ts_sel == 2) stpLXSdecParInput->eParInput = LX_SDEC_CI_OUTPUT;
				else if(ui4ts_sel == 3) stpLXSdecParInput->eParInput = LX_SDEC_CI_INPUT;
				break;

			default:
				SDEC_PRINT_ERROR("Invalid Port:[%d]", eInputPort);
				break;
		}
	}
#endif

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetParInput");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   select input port.
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
int SDEC_IO_GetCiInput
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GET_CI_INPUT_T *stpLXSdecCiInput)
{
	int 	eRet 		= RET_ERROR;
	UINT8 tpo_sel_ctrl = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecCiInput == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetCiInput");

	if(0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if(lx_chip() == LX_CHIP_H15)
	{
		CTOP_CTRL_H15A0_RdFL(VIP, ctr36);
		CTOP_CTRL_H15A0_Rd01(VIP, ctr36, tpo_sel_ctrl, tpo_sel_ctrl);

		if(tpo_sel_ctrl == 0)		stpLXSdecCiInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
		else if(tpo_sel_ctrl == 1)	stpLXSdecCiInput->eParInput = LX_SDEC_EXTERNAL_DEMOD0;
		else if(tpo_sel_ctrl == 2)	stpLXSdecCiInput->eParInput = LX_SDEC_EXTERNAL_DEMOD1;
		else if(tpo_sel_ctrl == 3)	stpLXSdecCiInput->eParInput = LX_SDEC_INTERNAL_TE;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CTOP_CTRL_H14A0_RdFL(ctr58);
		CTOP_CTRL_H14A0_Rd01(ctr58, tpo_sel_ctrl, tpo_sel_ctrl);

		if(tpo_sel_ctrl == 0) stpLXSdecCiInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
		else if(tpo_sel_ctrl == 1) stpLXSdecCiInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CTOP_CTRL_M14B0_RdFL(TOP, ctr08);
		CTOP_CTRL_M14B0_Rd01(TOP, ctr08, tpo_sel_ctrl, tpo_sel_ctrl);

		if(tpo_sel_ctrl == 0) stpLXSdecCiInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
		else if(tpo_sel_ctrl == 1) stpLXSdecCiInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CTOP_CTRL_M14A0_RdFL(ctr58);
		CTOP_CTRL_M14A0_Rd01(ctr58, tpo_sel_ctrl, tpo_sel_ctrl);

		if(tpo_sel_ctrl == 0) stpLXSdecCiInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
		else if(tpo_sel_ctrl == 1) stpLXSdecCiInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CTOP_CTRL_H13A0_RdFL(ctr58);
		CTOP_CTRL_H13A0_Rd01(ctr58, tpo_sel_ctrl, tpo_sel_ctrl);

		if(tpo_sel_ctrl == 0) stpLXSdecCiInput->eParInput = LX_SDEC_INTERNAL_DEMOD;
		else if(tpo_sel_ctrl == 1) stpLXSdecCiInput->eParInput = LX_SDEC_EXTERNAL_DEMOD;
	}
#endif
	else
	{
		SDEC_PRINT_ERROR("Invalid Chip : 0x%x\n", lx_chip_rev());
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetCiInput");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   set esa mode.
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
int SDEC_IO_SetCipherEnable
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_CIPHER_ENABLE_T *stLXSdecCipherEnable)
{
	int eRet = RET_ERROR;
	int ret = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidFltId = 0x0;
	BOOLEAN bEnable = 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stLXSdecCipherEnable == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SetCipherEnable");

	ui8Ch = stLXSdecCipherEnable->eCh;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);

	ui32PidFltId  = stLXSdecCipherEnable->uiPidFltId;
	LX_SDEC_CHECK_CODE( ui32PidFltId >= pSdecConf->chInfo[ui8Ch].num_pidf, goto exit, "over pid filter range %d", ui32PidFltId);

	bEnable	= stLXSdecCipherEnable->bEnable;

	ret = SDEC_SetPidf_Disc_Enable(stpSdecParam, ui8Ch, ui32PidFltId, bEnable);
	LX_SDEC_CHECK_CODE( ret != RET_OK, goto exit, "SDEC_SetPidf_Disc_Enable Error" );

	eRet = RET_OK;

exit:

	SDEC_PRINT_DESC( "SDEC_IO_SetCipherEnable : Ch[%d] FilterId[%d] bEnable[%d]", ui8Ch, ui32PidFltId, bEnable);
	SDEC_PRINT_TRACE( "-->SDEC_IO_SetCipherEnable ret[%d]", (UINT32)eRet);

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}



/**
********************************************************************************
* @brief
*   get information of input port setting
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   stpLXSdecGetInputPort : ioctrl arguments from userspace
* @return
*  int
********************************************************************************
*/
int SDEC_IO_GetInputPort
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_GET_INPUT_T *stpLXSdecGetInputPort)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	CDIC cdic;
	UINT8 core = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecGetInputPort == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_GetInputPort");

	ui8Ch = stpLXSdecGetInputPort->eCh;
	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	SDEC_HAL_CIDCGetStatus(core, ui8Ch, &cdic);

	stpLXSdecGetInputPort->uiSrc			= cdic.src;
	stpLXSdecGetInputPort->uiNo_wdata		= cdic.no_wdata;
	stpLXSdecGetInputPort->uiSync_lost		= cdic.sync_lost;
	stpLXSdecGetInputPort->uiSb_dropped 	= cdic.sb_dropped;
	stpLXSdecGetInputPort->uiCdif_empty 	= cdic.cdif_empty;
	stpLXSdecGetInputPort->uiCdif_full		= cdic.cdif_full;
	stpLXSdecGetInputPort->uiPd_wait		= cdic.pd_wait;
	stpLXSdecGetInputPort->uiCdif_ovflow	= cdic.cdif_ovflow;
	stpLXSdecGetInputPort->uiCdif_wpage 	= cdic.cdif_wpage;
	stpLXSdecGetInputPort->uiCdif_rpage 	= cdic.cdif_rpage;

	SDEC_PRINT_TRACE( "-->SDEC_IO_GetInputPort");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   set video output port
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
int SDEC_IO_SetVidOutport
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SET_VDEC_PORT_T *stpLXSdecSetVidOutort)
{
	int eRet = RET_ERROR;
	LX_SDEC_CFG_T* pSdecConf = NULL;

	UINT8 ui8Ch = 0x0;
	UINT8 ui8sel = 0x0;
	int ret = RET_ERROR;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSetVidOutort == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SetVidOutport");

	/* M14_TBD, channel converting is not needed? */
	ui8Ch 	= (UINT8)stpLXSdecSetVidOutort->ucPort;
	ui8sel	= (UINT8)stpLXSdecSetVidOutort->eFrom;

	/* get config */
	pSdecConf = SDEC_CFG_GetConfig();

	/* if port number is out of range, return errror */
	LX_SDEC_CHECK_CODE( ui8Ch > pSdecConf->nVdecOutPort, goto exit, "Invalid Channel [%d][%d]",ui8Ch, pSdecConf->nVdecOutPort );

	ret = SDEC_HAL_SetVideoOut(ui8Ch, ui8sel);
	LX_SDEC_CHECK_CODE( ret != RET_OK, goto exit, "SDEC_HAL_SetVideoOut Error [%d][%d]",ui8Ch, ui8sel );

	SDEC_PRINT_TRACE( "-->SDEC_IO_SetVidOutport");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   set  SDEC input port enable and disable select
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
int SDEC_InputPortEnable(UINT8 core, LX_SDEC_INPUT_T eInputPath, UINT8 en)
{
	int ret = RET_ERROR;
	UINT8 newCDIP;
	LX_SDEC_CFG_T* cfg = SDEC_CFG_GetConfig();

	newCDIP = cfg->newCDIP;

	switch (eInputPath)
	{
		case LX_SDEC_INPUT_PARALLEL0:
		case LX_SDEC_INPUT_PARALLEL4:
			SDEC_HAL_CDIPEnable(core, 0, en);
			break;
		case LX_SDEC_INPUT_PARALLEL1:
		case LX_SDEC_INPUT_PARALLEL5:
			if(newCDIP)
			{
				SDEC_HAL_CDIPEnable(core, 1, en);
			}
			else
			{
				SDEC_HAL_CDIPAEnable(core, 0, en);
			}
			break;

		case LX_SDEC_INPUT_SERIAL0:
		case LX_SDEC_INPUT_SERIAL4:
			if( en == SDEC_HAL_ENABLE)
			{
				// TODO: why do we set the input ?
				if(0){}
				else if(cfg->inTsSelector)
				{
					// nothing
				}
#ifdef INCLUDE_H14_CHIP_KDRV
				else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
				{
					UINT8			ui4ts_sel	= 0x0;

					CTOP_CTRL_H14A0_RdFL(ctr58);
					CTOP_CTRL_H14A0_Rd01(ctr58, ts_sel, ui4ts_sel);
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x4;	/* 01XX */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
					CTOP_CTRL_H14A0_WrFL(ctr58);
				}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
				else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
				{
					UINT8			ui4ts_sel	= 0x0;

					CTOP_CTRL_M14B0_RdFL(TOP, ctr19);
					CTOP_CTRL_M14B0_Rd01(TOP, ctr19, ts_sel, ui4ts_sel);
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x4;	/* 01XX */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
					CTOP_CTRL_M14B0_WrFL(TOP, ctr19);
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
				{
					UINT8			ui4ts_sel	= 0x0;

					CTOP_CTRL_M14A0_RdFL(ctr58);
					CTOP_CTRL_M14A0_Rd01(ctr58, ts_sel, ui4ts_sel);
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x4;	/* 01XX */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
					CTOP_CTRL_M14A0_WrFL(ctr58);
				}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
				else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
				{
					UINT8			ui4ts_sel	= 0x0;

					CTOP_CTRL_H13A0_RdFL(ctr58);
					CTOP_CTRL_H13A0_Rd01(ctr58, ts_sel, ui4ts_sel);
					ui4ts_sel &= 0x3;
					ui4ts_sel |= 0x4;	/* 01XX */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
					CTOP_CTRL_H13A0_WrFL(ctr58);
				}
#endif
				else
				{
					SDEC_PRINT_ERROR("Invalid Chip : 0x%x\n", lx_chip_rev());
				}
			}

			SDEC_HAL_CDIPEnable(core, 0, en);

			break;

		case LX_SDEC_INPUT_SERIAL1:
		case LX_SDEC_INPUT_SERIAL5:
			if( en == SDEC_HAL_ENABLE)
			{
				if(0){}
				else if(cfg->inTsSelector)
				{
					// nothing
				}
#ifdef INCLUDE_H14_CHIP_KDRV
				else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
				{
					UINT8			ui4ts_sel	= 0x0;

					CTOP_CTRL_H14A0_RdFL(ctr58);
					CTOP_CTRL_H14A0_Rd01(ctr58, ts_sel, ui4ts_sel);
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x1;	/* XX01 */
					CTOP_CTRL_H14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
					CTOP_CTRL_H14A0_WrFL(ctr58);
				}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
				else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
				{
					UINT8			ui4ts_sel	= 0x0;

					CTOP_CTRL_M14B0_RdFL(TOP, ctr19);
					CTOP_CTRL_M14B0_Rd01(TOP, ctr19, ts_sel, ui4ts_sel);
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x1;	/* XX01 */
					CTOP_CTRL_M14B0_Wr01(TOP, ctr19, ts_sel, ui4ts_sel);
					CTOP_CTRL_M14B0_WrFL(TOP, ctr19);
				}
				else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
				{
					UINT8			ui4ts_sel	= 0x0;

					CTOP_CTRL_M14A0_RdFL(ctr58);
					CTOP_CTRL_M14A0_Rd01(ctr58, ts_sel, ui4ts_sel);
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x1;	/* XX01 */
					CTOP_CTRL_M14A0_Wr01(ctr58, ts_sel, ui4ts_sel);
					CTOP_CTRL_M14A0_WrFL(ctr58);
				}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
				else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
				{
					UINT8			ui4ts_sel	= 0x0;

					CTOP_CTRL_H13A0_RdFL(ctr58);
					CTOP_CTRL_H13A0_Rd01(ctr58, ts_sel, ui4ts_sel);
					ui4ts_sel &= 0xC;
					ui4ts_sel |= 0x1;	/* XX01 */
					CTOP_CTRL_H13A0_Wr01(ctr58, ts_sel, ui4ts_sel);
					CTOP_CTRL_H13A0_WrFL(ctr58);
				}
#endif
				else
				{
					SDEC_PRINT_ERROR("Invalid Chip : 0x%x\n", lx_chip_rev());
				}
			}

			if(newCDIP)
			{
				SDEC_HAL_CDIPEnable(core, 1, en);
			}
			else
			{
				SDEC_HAL_CDIPAEnable(core, 0, en);
			}
			break;

		case LX_SDEC_INPUT_SERIAL2:
		case LX_SDEC_INPUT_SERIAL6:
			if(newCDIP)
			{
				SDEC_HAL_CDIPEnable(core, 2, en);
			}
			else
			{
				SDEC_HAL_CDIPEnable(core, 3, en);
			}
			break;

		case LX_SDEC_INPUT_SERIAL3:
		case LX_SDEC_INPUT_SERIAL7:
			if(newCDIP)
			{
				SDEC_HAL_CDIPEnable(core, 3, en);
			}
			else
			{
				SDEC_HAL_CDIOPEnable(core, 0, en);
			}
			break;

		default:
			goto exit;
	}

	ret = RET_OK;

exit:
	return ret;
}


/**
********************************************************************************
* @brief
*   set  SDEC input port enable and disable select
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
int SDEC_IO_InputPortEnable
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_ENABLE_INPUT_T *stpLXSdecEnableInput)
{
	int eRet = RET_ERROR;
	LX_SDEC_CH_T eCh, org_eCh = LX_SDEC_CH_A;
	LX_SDEC_INPUT_T eInputPath;
	LX_SDEC_CFG_T	*pSdecConf = NULL;
	UINT8 ui8En = 0, ui8cdif_full = 0, ui8cdif_ovflow = 0;
	CDIC cdic;
	unsigned long flags = 0;
	UINT8 core = 0;
	int ret = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecEnableInput == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_InputPortEnable");

	eCh 		= stpLXSdecEnableInput->eCh;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( eCh >= (LX_SDEC_CH_T)(pSdecConf->nChannel), goto exit, "over channel range %d", eCh);

	org_eCh		= eCh;
	eInputPath 	= stpLXSdecEnableInput->eInputPath;
	ui8En		= stpLXSdecEnableInput->bEnable ? 0x1 : 0x0;

	SDEC_CONVERT_CORE_CH(core, eCh);

	ret = SDEC_HAL_CIDCGetStatus(core, eCh, &cdic);
	LX_SDEC_CHECK_CODE( ret != RET_OK, goto exit, "SDEC_HAL_CIDCGetStatus error" );

	ui8cdif_full	= cdic.cdif_full;
	ui8cdif_ovflow	= cdic.cdif_ovflow;

	/* reset CDIC before enable CDIP */
	if(stpLXSdecEnableInput->bEnable)
	{
		if( ui8cdif_full || ui8cdif_ovflow )
		{
			spin_lock_irqsave(&stpSdecParam->stSdecResetSpinlock, flags);

			SDEC_PRINT_RESET( "CDIC       [0x%08x]", SDEC_HAL_CIDCGet(core, eCh));	/* SDIO-CDIC */
			SDEC_PRINT_RESET( "SDMWC      [0x%08x]", SDEC_HAL_SDMWCGet(core));		/* SDIO-SDMWC */
			SDEC_PRINT_RESET( "SDMWC_STAT [0x%08x]", SDEC_HAL_SDMWCGetStatus(core));		/* SDIO-SDMWC_STAT */
			SDEC_PRINT_RESET( "CHAN_STAT  [0x%08x]", SDEC_HAL_GetChannelStatus2(org_eCh));	/* SDCORE-CHAN_STAT */
			SDEC_PRINT_RESET( "CC_CHK_EN  [0x%08x%08x]", SDEC_HAL_CCCheckEnableGet(org_eCh, 0), SDEC_HAL_CCCheckEnableGet(org_eCh, 1));	/* SDCORE-CC_CHK_EN */

			/* disable channel input and wait 0.01 ms */
			SDEC_InputPortEnable(core, stpSdecParam->eInputPath[org_eCh], SDEC_HAL_DISABLE);

			OS_UsecDelay(10);

			SDEC_InputPortReset(org_eCh);

			OS_UsecDelay(10);

			spin_unlock_irqrestore(&stpSdecParam->stSdecResetSpinlock, flags);
		}

		/* CLEAR CC_CHK_EN Register */
		SDEC_HAL_CCCheckEnableSet(org_eCh, 0, 0);
		SDEC_HAL_CCCheckEnableSet(org_eCh, 1, 0);
	}

	SDEC_InputPortEnable(core, eInputPath,	ui8En);

	/* save input mode for polling check SDEC stuck */
	if(SDEC_IS_NORMAL_CHANNEL(eCh))	stpSdecParam->eInputPath[org_eCh] = eInputPath;

	SDEC_PRINT_TRACE( "-->SDEC_IO_InputPortEnable");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}

/**
********************************************************************************
* @brief
*   set  SDEC input port enable and disable select
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
int SDEC_IO_SelectPVRSource
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_DL_SEL_T *stpLXSdecDlSel)
{
	int eRet = RET_ERROR;
	UINT8 lxch, core, ch;
	int ret = RET_ERROR;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecDlSel == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	lxch = stpLXSdecDlSel->eCh;
	SDEC_CHECK_CHANNEL(lxch, goto exit);

	SDEC_LXCH_TO_CORE_CH(lxch, core, ch);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SelectPVRSource [%d][%d]", lxch, (UINT32)(stpLXSdecDlSel->eSrc));

	/* For CDIC2 Download for SDT set */
	/* H14_TBD, M14_TBD, set source from sdec0,1 should be inserted, jinhwan.bae 2013. 06. 15 */
	if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		if( stpLXSdecDlSel->eSrc == LX_SDEC_FROM_CDIC2 )
		{
			/* SDT S/W Parser Case, CDIC2 -> DL */
			/* set TOP Reg, PVR source as SDEC (PVR DN CH, SDEC0 or 1 or SENC) */
			if(core == 0)		ret = SDEC_HAL_SetPVRSrc(ch, E_SDEC_DN_OUT_FROM_CORE_0);
			else if(core == 1)	ret = SDEC_HAL_SetPVRSrc(ch, E_SDEC_DN_OUT_FROM_CORE_1);

			/* set PVR SDEC source as CDIC3 (SDEC(A,B) / CDIC3) */
			ret = SDEC_HAL_CIDC3DlConf(core, ch, 1);
			/* set PVR SDEC CDIC source as CDIC2 (CDIC3 / CDIC2) */
			ret = SDEC_HAL_CDIC2DlExtConf(core, ch, 1);
		}
		else if( stpLXSdecDlSel->eSrc == LX_SDEC_FROM_CDIC )
		{
			/* ALL DN Case, CDIC3 -> DL */
			/* set TOP Reg, PVR source as SDEC (PVR DN CH, SDEC0 or 1 or SENC) */
			if(core == 0)		ret = SDEC_HAL_SetPVRSrc(ch, E_SDEC_DN_OUT_FROM_CORE_0);
			else if(core == 1)	ret = SDEC_HAL_SetPVRSrc(ch, E_SDEC_DN_OUT_FROM_CORE_1);

			/* set PVR source as CDIC3 (SDEC(A,B) / CDIC3) */
			ret = SDEC_HAL_CIDC3DlConf(core, ch, 1);
			/* set PVR SDEC CDIC source as CDIC3 (CDIC3 / CDIC2) */
			ret = SDEC_HAL_CDIC2DlExtConf(core, ch, 0);
		}
		else
		{
			/* set PVR source as SDEC Core , Normal PVR Case  */
			/* set TOP Reg, PVR source as SDEC (PVR DN CH, SDEC0 or 1 or SENC) */
			if(core == 0) 		ret = SDEC_HAL_SetPVRSrc(ch, E_SDEC_DN_OUT_FROM_CORE_0);
			else if(core == 1)	ret = SDEC_HAL_SetPVRSrc(ch, E_SDEC_DN_OUT_FROM_CORE_1);

			ret = SDEC_HAL_CIDC3DlConf(core, ch, 0);
		}
	}
	else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		if( stpLXSdecDlSel->eSrc == LX_SDEC_FROM_CDIC2 )
		{
			/* set PVR source as SDEC (SDEC / SENC) */
			ret = SDEC_HAL_SetPVRSrc(ch, LX_SDEC_FROM_SDEC);
			/* set PVR SDEC source as CDIC3 (SDEC(A,B) / CDIC3) */
			ret = SDEC_HAL_CIDC3DlConf(_SDEC_CORE_0, ch, 1);
			/* set PVR SDEC CDIC source as CDIC2 (CDIC3 / CDIC2) */
			ret = SDEC_HAL_CDIC2DlExtConf(_SDEC_CORE_0, ch, 1);
		}
		else if( stpLXSdecDlSel->eSrc == LX_SDEC_FROM_CDIC )
		{
			/* set PVR source as SDEC (SDEC / SENC) */
			ret = SDEC_HAL_SetPVRSrc(ch, LX_SDEC_FROM_SDEC);
			/* set PVR source as CDIC3 (SDEC(A,B) / CDIC3) */
			ret = SDEC_HAL_CIDC3DlConf(_SDEC_CORE_0, ch, 1);
			/* set PVR SDEC CDIC source as CDIC3 (CDIC3 / CDIC2) */
			ret = SDEC_HAL_CDIC2DlExtConf(_SDEC_CORE_0, ch, 0);
		}
		else
		{
			/* set PVR source as SDEC Core */
			ret = SDEC_HAL_CIDC3DlConf(_SDEC_CORE_0, ch, 0);
			ret = SDEC_HAL_SetPVRSrc(ch, stpLXSdecDlSel->eSrc);
		}
	}

	LX_SDEC_CHECK_CODE( ret != RET_OK, goto exit, "error in HAL" );

	SDEC_PRINT_TRACE( "-->SDEC_IO_SelectPVRSource");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   set input port.
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
int SDEC_InputSet
	(S_SDEC_PARAM_T *param,
	LX_SDEC_SEL_INPUT_T *stpLXSdecSelPort)
{
	int eRet = RET_ERROR;
	LX_SDEC_CH_T lxch;
	UINT8 core, ch;
	LX_PVR_CFG_T* pvr_cfg;
	E_SDEC_CDIC_SRC_T cdic_src;

	LX_SDEC_CHECK_CODE( param == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSelPort == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	SDEC_PRINT_TRACE( "<--SDEC_InputSet");

	lxch = stpLXSdecSelPort->eCh;
	/* argument check */
	LX_SDEC_CHECK_CODE(lxch >= param->cfg->nChannel, goto exit, "Over channel[%d] range", lxch);

	SDEC_LXCH_TO_CORE_CH(lxch, core, ch);


	SDEC_HAL_CIDCMinSyncByteDetection(core, ch, 3);
	SDEC_HAL_CIDCMaxSyncByteDrop(core, ch, 3);

	switch (stpLXSdecSelPort->eInputPath)
	{
		case LX_SDEC_INPUT_SERIAL0:
		case LX_SDEC_INPUT_SERIAL4:
			SDEC_PRINT_IO( "Ch[%d] SERIAL0", lxch);
			SDEC_HAL_CIDCSetSrc(core, ch, E_SDEC_CDIC_SRC_CDIN0);
			break;
		case LX_SDEC_INPUT_SERIAL1:
		case LX_SDEC_INPUT_SERIAL5:
			SDEC_PRINT_IO( "Ch[%d] SERIAL1", lxch);
			SDEC_HAL_CIDCSetSrc(core, ch, E_SDEC_CDIC_SRC_CDINA);
			break;
		case LX_SDEC_INPUT_SERIAL2:
		case LX_SDEC_INPUT_SERIAL6:
			SDEC_PRINT_IO( "Ch[%d] SERIAL2", lxch);
			SDEC_HAL_CIDCSetSrc(core, ch, E_SDEC_CDIC_SRC_CDIN3);
			break;
		case LX_SDEC_INPUT_SERIAL3:
		case LX_SDEC_INPUT_SERIAL7:
			SDEC_PRINT_IO( "Ch[%d] SERIAL3", lxch);
			SDEC_HAL_CIDCSetSrc(core, ch, E_SDEC_CDIC_SRC_CDIN4);
			break;
		case LX_SDEC_INPUT_PARALLEL0:
		case LX_SDEC_INPUT_PARALLEL4:
			SDEC_PRINT_IO( "Ch[%d] PARALLEL0", lxch);
			SDEC_HAL_CIDCSetSrc(core, ch, E_SDEC_CDIC_SRC_CDIN0);
			break;
		case LX_SDEC_INPUT_PARALLEL1:
		case LX_SDEC_INPUT_PARALLEL5:
			SDEC_PRINT_IO( "Ch[%d] PARALLEL1", lxch);
			SDEC_HAL_CIDCSetSrc(core, ch, E_SDEC_CDIC_SRC_CDINA);
			break;
		case LX_SDEC_INPUT_DVR0:
		case LX_SDEC_INPUT_DVR1:
			SDEC_PRINT_IO( "Ch[%d] DVR", lxch);

			pvr_cfg = PVR_CFG_GetConfig();
			if(pvr_cfg->num_updev == 1)
			{
				cdic_src = E_SDEC_CDIC_SRC_UPLOAD0;
			}
			else
			{
				if(ch == LX_SDEC_CH_D)
				{
					cdic_src = (stpLXSdecSelPort->eInputPath == LX_SDEC_INPUT_DVR0) ?
								E_SDEC_CDIC_SRC_UPLOAD0 : E_SDEC_CDIC_SRC_UPLOAD1;
				}
				else
				{
					/* For backward compatibility */
					if(ch == LX_SDEC_CH_B)
						cdic_src = E_SDEC_CDIC_SRC_UPLOAD1;
					else
						cdic_src = E_SDEC_CDIC_SRC_UPLOAD0;
				}
			}
			SDEC_HAL_CIDCSetSrc(core, ch, cdic_src);

			// jinhwan.bae for bypass all upload data without sync check packet drop. 20130726, found by JP model
			// upload packet started with Video PES header, 3 TP dropped, causes 1 GOP drop.
			// fixed without sync checking, 0 packet used to detect sync
			SDEC_HAL_CIDCMinSyncByteDetection(core, ch, 0);

			break;
		case LX_SDEC_INPUT_BDRC:
			SDEC_PRINT_IO( "Ch[%d] DVR", lxch);
			SDEC_HAL_CIDCSetSrc(core, ch, E_SDEC_CDIC_SRC_GPB);
			break;
		default:
			SDEC_PRINT_ERROR("Invalid input path:[%d]", stpLXSdecSelPort->eInputPath);
			goto exit;
			break;
	}

	SDEC_PRINT_IO( "Reset");

	if(stpLXSdecSelPort->bPortReset == TRUE)
	{
		/* disable sdec input port */
		SDEC_InputPortEnable(core, stpLXSdecSelPort->eInputPath,	SDEC_HAL_DISABLE);
		OS_UsecDelay(10);
		SDEC_InputPortReset(lxch);
		OS_UsecDelay(10);
		/* enable sdec input port */
		SDEC_InputPortEnable(core, stpLXSdecSelPort->eInputPath,	SDEC_HAL_ENABLE);
	}

	/* jinhwan.bae added 2013. 02. 19 work around for CDIF_FULL, MWF_OVF, in US mode reset was done with SERIAL (default value)
	    if SetInputConfig(ENABLE) is not called, no input is defined param->eInputPath[ch], default serial is set  */
	/* save input mode for polling check SDEC stuck */
	if( stpLXSdecSelPort->eCh < LX_SDEC_CH_C )	param->eInputPath[stpLXSdecSelPort->eCh] = stpLXSdecSelPort->eInputPath;

	SDEC_PRINT_IO( "Done");
	SDEC_PRINT_TRACE( "-->SDEC_InputSet");

	eRet = RET_OK;

exit:
	return (eRet);
}

UINT32* SDEC_IO_GetRegBaseAddr(LX_SDEC_BLOCK_T block)
{
	UINT32 *base_addr;

	switch(block)
	{
		case LX_SDEC_BLOCK_TOP:
			base_addr = (UINT32*)g_sdec_sdtop_reg_ctrl[0].phys.addr;
			break;

		case LX_SDEC_BLOCK_IO:
			base_addr = (UINT32*)g_sdec_sdio_reg_ctrl[0].phys.addr;
			break;

		case LX_SDEC_BLOCK_CORE_A:
			base_addr = (UINT32*)g_sdec_mpg_reg_ctrl[0].phys[0].addr;
			break;

		case LX_SDEC_BLOCK_CORE_B:
			base_addr = (UINT32*)g_sdec_mpg_reg_ctrl[0].phys[1].addr;
			break;

		case LX_SDEC_BLOCK_CORE1_IO:
			base_addr = (UINT32*)g_sdec_sdio_reg_ctrl[1].phys.addr;
			break;

		case LX_SDEC_BLOCK_CORE1_CORE_A:
			base_addr = (UINT32*)g_sdec_mpg_reg_ctrl[1].phys[0].addr;
			break;

		case LX_SDEC_BLOCK_CORE1_CORE_B:
			base_addr = (UINT32*)g_sdec_mpg_reg_ctrl[1].phys[1].addr;
			break;

		default :
			base_addr = NULL;
			break;
	}

	return base_addr;
}

UINT32 SDEC_IO_GetRegSize(LX_SDEC_BLOCK_T block)
{
	UINT32 size;
	switch(block)
	{
		case LX_SDEC_BLOCK_TOP:
			size = g_sdec_sdtop_reg_ctrl[0].size;
			break;

		case LX_SDEC_BLOCK_IO:
			size = g_sdec_sdio_reg_ctrl[0].size;
			break;

		case LX_SDEC_BLOCK_CORE_A:
		case LX_SDEC_BLOCK_CORE_B:
			size = g_sdec_mpg_reg_ctrl[0].size;
			break;

		case LX_SDEC_BLOCK_CORE1_IO:
			size = g_sdec_sdio_reg_ctrl[1].size;
			break;

		case LX_SDEC_BLOCK_CORE1_CORE_A:
		case LX_SDEC_BLOCK_CORE1_CORE_B:
			size = g_sdec_mpg_reg_ctrl[1].size;
			break;

		default :
			size = 0;
			break;
	}
	return size;
}


/**
********************************************************************************
* @brief
*   Read Register value
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
int SDEC_IO_GetRegister(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_REG_T *stpLXSdecReadRegisters)
{
	int eRet = RET_ERROR;
	LX_SDEC_BLOCK_T eSdecBlock;
	UINT32 ui32Offset;
	UINT32* reg_ptr;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecReadRegisters == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	eSdecBlock	= stpLXSdecReadRegisters->eSdecBlock;
	ui32Offset 	= stpLXSdecReadRegisters->uiOffset;

	LX_SDEC_CHECK_CODE((ui32Offset%4) != 0, return RET_INVALID_PARAMS, "Not aligned offset");

	reg_ptr = SDEC_IO_GetRegBaseAddr(eSdecBlock);
	if(reg_ptr != NULL)
	{
		OS_LockMutex(&stpSdecParam->stSdecMutex);
		stpLXSdecReadRegisters->uiValue = SDEC_READ32(reg_ptr + (ui32Offset/4));
		OS_UnlockMutex(&stpSdecParam->stSdecMutex);
		eRet = RET_OK;
	}
	return (eRet);
}

/**
********************************************************************************
* @brief
*   Write Register value
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
int SDEC_IO_SetRegister(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_REG_T *stpLXSdecWriteRegisters)
{
	int eRet = RET_ERROR;
	LX_SDEC_BLOCK_T eSdecBlock;
	UINT32 ui32Offset;
	UINT32* reg_ptr;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecWriteRegisters == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	eSdecBlock	= stpLXSdecWriteRegisters->eSdecBlock;
	ui32Offset 	= stpLXSdecWriteRegisters->uiOffset;
	LX_SDEC_CHECK_CODE((ui32Offset%4) != 0, return RET_INVALID_PARAMS, "Not aligned offset");

	reg_ptr = SDEC_IO_GetRegBaseAddr(eSdecBlock);
	if(reg_ptr != NULL)
	{
		OS_LockMutex(&stpSdecParam->stSdecMutex);
		SDEC_WRITE32(reg_ptr + (ui32Offset/4), stpLXSdecWriteRegisters->uiValue);
		OS_UnlockMutex(&stpSdecParam->stSdecMutex);
		eRet = RET_OK;
	}
	return (eRet);
}


/**
********************************************************************************
* @brief
*   Reset  SDEC input port
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
int SDEC_InputPortReset(UINT8 ui8Ch)
{
	int 			ret = RET_ERROR;
	UINT32			sdmwc_stat = 0;
	UINT8			chk_cnt = 0;
	UINT8			core = 0;

	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	/* wait until SDMWC is idle */
	sdmwc_stat = ( SDEC_HAL_SDMWCGetStatus(core) & 0x00000333 );

	while(sdmwc_stat != 0 && chk_cnt++ < 3)
	{
		sdmwc_stat = ( SDEC_HAL_SDMWCGetStatus(core) & 0x00000333 );
		OS_UsecDelay(1000);
	}

	SDEC_PRINT_RESET( "chk_cnt[%d] ", chk_cnt );

	/* if not idle, reset SDMWC */
	if(chk_cnt >= 3)
	{
		SDEC_PRINT_RESET( "NOT IDLE!");

		/* 0xC000B264(SDIO-SDMWC_STAT)*/
		SDEC_PRINT_RESET( "SDMWC_STAT [0x%08x]", SDEC_HAL_SDMWCGetStatus(core));

		/* This block is added from L9 B0 */
		/* Reset */
		SDEC_HAL_SDMWCReset(core, 0x30000000);
		/* wait 10 us */
		OS_UsecDelay(10);
		/* Clear */
		SDEC_HAL_SDMWCReset(core, 0x00000000);
		/* wait 10 us */
		OS_UsecDelay(10);
	}


	/* reset channel */
	//stpSdecParam->stSDEC_IO_Reg->cdic[ui8Ch].rst = 1;
	SDEC_HAL_CIDCReset(core, ui8Ch);

	/* wait 10 us */
	OS_UsecDelay(10);

	ret = RET_OK;

	return ret;
}


/**
********************************************************************************
* @brief
*   Reset  SDEC Memory write Controller
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
int SDEC_SDMWCReset(UINT8 ui8Ch)
{
	int 			ret = RET_ERROR;
	UINT8			core = 0;

	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	/* This block is added from L9 B0 */
	/* Reset */
	SDEC_HAL_SDMWCReset(core, 0x30000000);

	/* wait 10 us */
	OS_UsecDelay(10);

	/* Clear */
	SDEC_HAL_SDMWCReset(core, 0x00000000);

	/* wait 1 ms */
	OS_UsecDelay(1000);

	ret = RET_OK;

	return ret;
}






static UINT32* _SDEC_GetRegIOBackupOffset(void)
{
	static UINT32 reg_io_backup_offset[] =
	{
		[E_SDEC_REGBACKUP_IO_CDIP0] 		= 0x00,
		[E_SDEC_REGBACKUP_IO_CDIP1] 		= 0x04,
		[E_SDEC_REGBACKUP_IO_CDIP2] 		= 0x08,
		[E_SDEC_REGBACKUP_IO_CDIP3] 		= 0x0C,
		[E_SDEC_REGBACKUP_IO_CDIOP0]		= 0x10,
		[E_SDEC_REGBACKUP_IO_CDIOP1]		= 0x14,
		[E_SDEC_REGBACKUP_IO_CDIC_DSC0] 	= 0x28,
		[E_SDEC_REGBACKUP_IO_CDIC_DSC1] 	= 0x2C,
		[E_SDEC_REGBACKUP_IO_INTR_EN]		= 0x100,
		[E_SDEC_REGBACKUP_IO_ERR_INTR_EN]	= 0x110,
		[E_SDEC_REGBACKUP_IO_GPB_BASE]		= 0x120,
		[E_SDEC_REGBACKUP_IO_CDIC2] 		= 0x12C,
	};

	return reg_io_backup_offset;
}

static UINT32* _SDEC_GetRegCoreBackupOffset(void)
{
	static UINT32 reg_core_backup_offset[] =
	{
		[E_SDEC_REGBACKUP_CORE_EXT_CONF]		= 0x10,
		[E_SDEC_REGBACKUP_CORE_TPI_ICONF]		= 0x40,
		[E_SDEC_REGBACKUP_CORE_KMEM_ADDR]		= 0x98,
		[E_SDEC_REGBACKUP_CORE_KMEM_DATA]		= 0x9C,
		[E_SDEC_REGBACKUP_CORE_SECF_EN_L]		= 0xB0,
		[E_SDEC_REGBACKUP_CORE_SECF_EN_H]		= 0xB4,
		[E_SDEC_REGBACKUP_CORE_SECF_MTYPE_L]	= 0xB8,
		[E_SDEC_REGBACKUP_CORE_SECF_MTYPE_H]	= 0xBC,
		[E_SDEC_REGBACKUP_CORE_SECFB_VALID_L]	= 0xC0,
		[E_SDEC_REGBACKUP_CORE_SECFB_VALID_H]	= 0xC4,
	};

	return reg_core_backup_offset;
}

/**
********************************************************************************
* @brief
*   Reset  SDEC H/W block. Using Chip TOP.
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
static UINT32* _SDEC_RegBackup(void)
{
	UINT8			i, offset;
	LX_SDEC_CFG_T	*cfg;
	UINT32			*reg_io_offset;
	UINT32			*reg_core_offset;
	int				core, ch;
	UINT32			*reg_store;

	cfg = SDEC_CFG_GetConfig();

	reg_io_offset = _SDEC_GetRegIOBackupOffset();
	reg_core_offset = _SDEC_GetRegCoreBackupOffset();

	reg_store = (UINT32*)OS_KMalloc(sizeof(UINT32) * (E_SDEC_REGBACKUP_IO_NUM + E_SDEC_REGBACKUP_CORE_NUM*2)*cfg->nCore);
	SDEC_CHECK_ERROR(reg_store == NULL, return NULL, "kmalloc fail");

	offset = 0;
	for(core=0; core<cfg->nCore; core++)
	{
		/* SD IO Bakcup */
		for ( i = 0 ; i < E_SDEC_REGBACKUP_IO_NUM ; i++, offset++ )
		{
			reg_store[offset] = SDEC_READ32(cfg->sdioRegBase[core] + reg_io_offset[i]);
			SDEC_PRINT_RESET( "BACKUP  IO    OFFSET[0x%03x] VAL[0x%08x]", reg_io_offset[i], reg_store[offset]);
		}

		/* SD CORE Bakcup */
		for(ch=0; ch<2; ch++)
		{
			for (i=0 ; i<E_SDEC_REGBACKUP_CORE_NUM; i++, offset++)
			{
				reg_store[offset] = SDEC_READ32(cfg->mpgRegBase[core][ch] + reg_core_offset[i]);
				SDEC_PRINT_RESET( "BACKUP  CORE0 OFFSET[0x%03x] VAL[0x%08x]", reg_core_offset[i], reg_store[offset]);
			}
		}
	}

	return reg_store;
}

/**
********************************************************************************
* @brief
*   Reset  SDEC H/W block. Using Chip TOP.
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
static int _SDEC_RegRestore(UINT32 *reg_store)
{
	int 			ret = RET_ERROR;
	UINT8			i, offset;
	LX_SDEC_CFG_T	*cfg;
	UINT32			*reg_io_offset;
	UINT32			*reg_core_offset;
	int				core, ch;

	SDEC_CHECK_ERROR(reg_store == NULL, goto exit, "invalid param");

	cfg = SDEC_CFG_GetConfig();

	reg_io_offset = _SDEC_GetRegIOBackupOffset();
	reg_core_offset = _SDEC_GetRegCoreBackupOffset();

	offset = 0;
	for(core=0; core<cfg->nCore; core++)
	{
		SDEC_PRINT_RESET("RESTORE CORE[%d]", core);
		/* SD IO Restore */
		for ( i = 0 ; i < E_SDEC_REGBACKUP_IO_NUM ; i++, offset++ )
		{
			SDEC_PRINT_RESET( "RESTORE IO    OFFSET[0x%03x] VAL[0x%08x]", reg_io_offset[i], reg_store[offset]);
			SDEC_WRITE32(cfg->sdioRegBase[core] + reg_io_offset[i], reg_store[i]);
		}

		/* SD CORE Restore */
		for(ch=0; ch<2; ch++)
		{
			for (i=0 ; i<E_SDEC_REGBACKUP_CORE_NUM; i++, offset++)
			{
				SDEC_PRINT_RESET( "RESTORE CH[%d] OFFSET[0x%03x] VAL[0x%08x]", ch, reg_core_offset[i], reg_store[offset]);
				SDEC_WRITE32(cfg->mpgRegBase[core][ch] + reg_core_offset[i], reg_store[offset]);
			}
		}
	}
	OS_KFree(reg_store);
	ret = RET_OK;

exit:
	return ret;
}



/**
********************************************************************************
* @brief
*   Reset  SDEC H/W block. Using Chip TOP.
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
int SDEC_TE_Reset(UINT8 core)
{
	int 		ret = RET_ERROR;
	UINT32		*reg_store;


	/* Register Backup */
	reg_store = _SDEC_RegBackup();
	SDEC_CHECK_ERROR(reg_store == NULL, return RET_ERROR, "can't backup register");

	/* Port Disable */
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL0, 		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL1, 		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL0,		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL1,		SDEC_HAL_DISABLE);

	/* Port Enable */
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL0, 		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL1, 		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL0,		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL1,		SDEC_HAL_ENABLE);

	/* Register Restore */
	_SDEC_RegRestore(reg_store);

	ret = RET_OK;

	return ret;
}

/**
********************************************************************************
* @brief
*   Reset  SDEC H/W block. Using Chip TOP.
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
int SDEC_TEA_Reset(UINT8 core)
{
	int 			ret = RET_ERROR;
	UINT32		*reg_store;

	/* Register Backup */
	reg_store = _SDEC_RegBackup();
	SDEC_CHECK_ERROR(reg_store == NULL, return RET_ERROR, "can't backup register");

	/* Port Disable */
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL0, 		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL1, 		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL0,		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL1,		SDEC_HAL_DISABLE);

	/* Port Enable */
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL0, 		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL1, 		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL0,		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL1,		SDEC_HAL_ENABLE);

	/* Register Restore */
	_SDEC_RegRestore(reg_store);

	ret = RET_OK;

	return ret;
}


/**
********************************************************************************
* @brief
*   Reset  SDEC H/W block. Using Chip TOP.
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
int SDEC_TEAH_Reset(UINT8 core)
{
	int 		ret = RET_ERROR;
	UINT32		*reg_store;

	/* Register Backup */
	reg_store = _SDEC_RegBackup();
	SDEC_CHECK_ERROR(reg_store == NULL, return RET_ERROR, "can't backup register");

	/* Port Disable */
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL0, 		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL1, 		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL0,		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL1,		SDEC_HAL_DISABLE);

	/* Port Enable */
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL0, 		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL1, 		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL0,		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL1,		SDEC_HAL_ENABLE);

	/* Register Restore */
	_SDEC_RegRestore(reg_store);

	ret = RET_OK;

	return ret;
}

/**
********************************************************************************
* @brief
*   Reset  SDEC H/W block. Using Chip TOP.
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
int SDEC_TEAHFlush_Reset(UINT8 core)
{
	int 		ret = RET_ERROR;
	UINT32		*reg_store;

	/* Register Backup */
	reg_store = _SDEC_RegBackup();
	SDEC_CHECK_ERROR(reg_store == NULL, return RET_ERROR, "can't backup register");

	/* Port Disable */
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL0, 		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL1, 		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL0,		SDEC_HAL_DISABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL1,		SDEC_HAL_DISABLE);

	/* Port Enable */
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL0, 		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_SERIAL1, 		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL0,		SDEC_HAL_ENABLE);
	SDEC_InputPortEnable(core, LX_SDEC_INPUT_PARALLEL1,		SDEC_HAL_ENABLE);

	/* Register Restore */
	_SDEC_RegRestore(reg_store);

	ret = RET_OK;

	return ret;
}


/**
********************************************************************************
* @brief
*   Send Debug Command to Kdrv.
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
int SDEC_IO_DebugCommand
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_DBG_CMD_T 	*stpLXSdecDbgCmd)
{
	int 		eRet = RET_ERROR;
	UINT8				ui8Ch = 0, core = 0, org_ch = 0;
	unsigned long 		flags = 0;
	LX_SDEC_CFG_T 		*pSdecConf = NULL;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecDbgCmd == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_DebugCommand");

	/* get channel */
	ui8Ch = (UINT8)stpLXSdecDbgCmd->inParam;

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, goto exit, "over channel range %d", ui8Ch);

	org_ch = ui8Ch;
	SDEC_CONVERT_CORE_CH(core, ui8Ch);

	switch(stpLXSdecDbgCmd->eCmd)
	{
		case LX_SDEC_CMD_RESET_CDIC:
			spin_lock_irqsave(&stpSdecParam->stSdecResetSpinlock, flags);
			/* disable channel input and wait 0.01 ms */
			SDEC_InputPortEnable(core, stpSdecParam->eInputPath[org_ch], 		SDEC_HAL_DISABLE);
			OS_UsecDelay(10);
			SDEC_InputPortReset(org_ch);
			OS_UsecDelay(10);
			/* enable channel input */
			SDEC_InputPortEnable(core, stpSdecParam->eInputPath[org_ch], 		SDEC_HAL_ENABLE);
			spin_unlock_irqrestore(&stpSdecParam->stSdecResetSpinlock, flags);
			break;

		case LX_SDEC_CMD_RESET_SDMWC:
			spin_lock_irqsave(&stpSdecParam->stSdecResetSpinlock, flags);
			/* disable channel input and wait 0.01 ms */
			SDEC_InputPortEnable(core, stpSdecParam->eInputPath[org_ch], SDEC_HAL_DISABLE);
			OS_UsecDelay(10);
			SDEC_SDMWCReset(org_ch);
			OS_UsecDelay(10);
			/* enable channel input */
			SDEC_InputPortEnable(core, stpSdecParam->eInputPath[org_ch], SDEC_HAL_ENABLE);
			spin_unlock_irqrestore(&stpSdecParam->stSdecResetSpinlock, flags);
			break;

		case LX_SDEC_CMD_RESET_TE:
			spin_lock_irqsave(&stpSdecParam->stSdecResetSpinlock, flags);
			SDEC_TE_Reset(core);
			spin_unlock_irqrestore(&stpSdecParam->stSdecResetSpinlock, flags);
			break;

		case LX_SDEC_CMD_RESET_TEA:
			spin_lock_irqsave(&stpSdecParam->stSdecResetSpinlock, flags);
			SDEC_TEA_Reset(core);
			spin_unlock_irqrestore(&stpSdecParam->stSdecResetSpinlock, flags);
			break;

		case LX_SDEC_CMD_RESET_TEAH:
			spin_lock_irqsave(&stpSdecParam->stSdecResetSpinlock, flags);
			SDEC_TEAH_Reset(core);
			spin_unlock_irqrestore(&stpSdecParam->stSdecResetSpinlock, flags);
			break;

		case LX_SDEC_CMD_RESET_TEAH_FLUSH:
			spin_lock_irqsave(&stpSdecParam->stSdecResetSpinlock, flags);
			SDEC_TEAHFlush_Reset(core);
			spin_unlock_irqrestore(&stpSdecParam->stSdecResetSpinlock, flags);
			break;

		case LX_SDEC_CMD_EN_CIDC :
			if(stpLXSdecDbgCmd->inParam == E_SDEC_RESET_MODE_ONETIME)	stpSdecParam->ui8CDICResetNum = 0;
			stpSdecParam->ui8CDICResetMode = (UINT8)stpLXSdecDbgCmd->inParam;
			break;

		case LX_SDEC_CMD_EN_SDMWC:
			if(stpLXSdecDbgCmd->inParam == E_SDEC_RESET_MODE_ONETIME)	stpSdecParam->ui8SDMWCResetNum = 0;
			stpSdecParam->ui8SDMWCResetMode = (UINT8)stpLXSdecDbgCmd->inParam;
			break;

		case LX_SDEC_CMD_SET_FCW:
			SDEC_IO_SettingFCW(stpSdecParam , stpLXSdecDbgCmd->inParam);
			break;

		default :
			break;
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_DebugCommand");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/* Serial TS OUT Stub */


/**
********************************************************************************
* @brief
*	Serial TS Out
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*	ui32Arg :arguments from userspace
*	stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_IO_SerialTSOUT_SetSrc
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SERIALTSOUT_SET_SOURCE_T *stpLXSdecSTSOutSetSrc)
{
	int eRet = RET_ERROR;
	LX_SDEC_SERIALTSOUT_SRC_T src = LX_SDEC_SO_NONE;
	UINT8 tso_src;
	UINT8 core = 0;
	UINT8 ui8Ch= 0x0;
	LX_SDEC_CFG_T *pSdecConf = NULL;


	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSTSOutSetSrc == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	/* argument check */
	pSdecConf = SDEC_CFG_GetConfig();

	ui8Ch = stpLXSdecSTSOutSetSrc->eCh;
	LX_SDEC_CHECK_CODE( ui8Ch >= pSdecConf->nChannel, return RET_INVALID_PARAMS, "over channel range %d", ui8Ch);
	LX_SDEC_CHECK_CODE(!SDEC_IS_NORMAL_CHANNEL(ui8Ch), return RET_INVALID_PARAMS, "Not normal channel %d", ui8Ch);

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	src = stpLXSdecSTSOutSetSrc->src;
	tso_src = stpLXSdecSTSOutSetSrc->eCh;

	SDEC_CONVERT_CORE_CH(core, tso_src);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SerialTSOUT_SetSrc [%d]", (UINT8)src);

	SDEC_HAL_CDOC_SetSrc(core, 1 /* CDOC 1 only available */, (UINT8)src);
	SDEC_HAL_CDOC_SetTsoSrc(core, 1, tso_src);

	/* reset with set src */
	SDEC_HAL_CDOC_Reset(core, 1 /* CDOC 1 only available */);

	SDEC_PRINT_TRACE( "-->SDEC_IO_SerialTSOUT_SetSrc");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);

}



/**
********************************************************************************
* @brief
*	Serial TS Out
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*	ui32Arg :arguments from userspace
*	stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_IO_BDRC_SetData
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_BDRC_T *stpLXSdecBDRCSetData)
{
	int eRet = RET_ERROR;
	UINT8	bdrc_idx = 0, en = 0, rst = 0, wptr_auto = 0, gpb_chan = 0, gpb_idx = 0, q_len = 0, dtype = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecBDRCSetData == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	bdrc_idx = stpLXSdecBDRCSetData->bdrc_idx;
	en = stpLXSdecBDRCSetData->en;
	rst = stpLXSdecBDRCSetData->rst;
	wptr_auto = stpLXSdecBDRCSetData->wptr_auto;
	gpb_chan = stpLXSdecBDRCSetData->gpb_chan;
	gpb_idx = stpLXSdecBDRCSetData->gpb_idx;
	q_len = stpLXSdecBDRCSetData->q_len;
	dtype = stpLXSdecBDRCSetData->dtype;

	SDEC_PRINT_TRACE( "<--SDEC_IO_BDRC_SetData [%d][%d][%d][%d][%d][%d][%d][%d]",bdrc_idx,en,rst,wptr_auto,gpb_chan,gpb_idx,q_len,dtype);

	/* BDRC 3 is only available for TS out, BDRC 1 is used for SDEC input */
	SDEC_HAL_BDRC_Reset( _SDEC_CORE_0, bdrc_idx );
	SDEC_HAL_BDRC_EnableWptrAutoUpdate( _SDEC_CORE_0, bdrc_idx, wptr_auto );
	SDEC_HAL_BDRC_SetGPBChannel( _SDEC_CORE_0, bdrc_idx, gpb_chan );
	SDEC_HAL_BDRC_SetGPBIndex( _SDEC_CORE_0, bdrc_idx, gpb_idx );
	SDEC_HAL_BDRC_SetQlen( _SDEC_CORE_0, bdrc_idx, q_len );
	SDEC_HAL_BDRC_SetDtype( _SDEC_CORE_0, bdrc_idx, dtype );
	SDEC_HAL_BDRC_Enable( _SDEC_CORE_0, bdrc_idx, en );

	SDEC_PRINT_TRACE( "-->SDEC_IO_BDRC_SetData");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*	Serial TS Out
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*	ui32Arg :arguments from userspace
*	stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_IO_BDRC_Enable
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_BDRC_ENABLE_T *stpLXSdecBDRCEnable)
{
	int eRet = RET_ERROR;
	UINT8	bdrc_idx = 0, en = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecBDRCEnable == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	bdrc_idx = stpLXSdecBDRCEnable->bdrc_idx;
	en = stpLXSdecBDRCEnable->en;

	SDEC_PRINT_TRACE( "<--SDEC_IO_BDRC_Enable [%d][%d]",bdrc_idx,en);

	SDEC_HAL_BDRC_Enable( _SDEC_CORE_0, bdrc_idx, en );

	SDEC_PRINT_TRACE( "-->SDEC_IO_BDRC_SetData");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);

}


/**
********************************************************************************
* @brief
*	Serial TS Out
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*	ui32Arg :arguments from userspace
*	stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_IO_SerialTSOUT_SetBufELevel
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SERIALTSOUT_SET_BUFELEV_T *stpLXSdecSTSOutSetBufELev)
{
	int eRet = RET_ERROR;
	UINT32 buf_e_lev = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSTSOutSetBufELev == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	buf_e_lev = stpLXSdecSTSOutSetBufELev->buf_e_lev;

	SDEC_PRINT_TRACE( "<--SDEC_IO_SerialTSOUT_SetBufELevel [%d]",buf_e_lev);

	/* BDRC 3 is only available for TS out */
	SDEC_HAL_BDRC_SetBufferEmptyLevel( _SDEC_CORE_0, 3, buf_e_lev );

	SDEC_PRINT_TRACE( "-->SDEC_IO_SerialTSOUT_SetBufELevel");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}


/**
********************************************************************************
* @brief
*   select input port.
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
int SDEC_IO_CfgOutputPort
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_CFG_OUTPUT_T *stpLXSdecCfgPort)
{
	int eRet = RET_ERROR;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecCfgPort == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_CfgOutputPort");

	SDEC_HAL_CDIOP_SetOut(_SDEC_CORE_0, 1, stpLXSdecCfgPort);

	SDEC_PRINT_TRACE( "-->SDEC_IO_CfgOutputPort");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}




/**
********************************************************************************
* @brief
*	Serial TS Out
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*	ui32Arg :arguments from userspace
*	stpSdecParam :SDEC parameter
* @return
*  int
********************************************************************************
*/
int SDEC_IO_BDRC_SetWritePtrUpdated
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_BDRC_WPTRUPD_T *stpLXSdecBDRCSetWptrUpd)
{
	int eRet = RET_ERROR;
	UINT8 idx = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecBDRCSetWptrUpd == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	idx = stpLXSdecBDRCSetWptrUpd->idx;

	SDEC_PRINT_TRACE( "<--SDEC_IO_BDRC_SetWritePtrUpdated [%d]",idx);

	/* BDRC 3 is only available for TS out */
	SDEC_HAL_BDRC_SetWptrUpdate(_SDEC_CORE_0, idx );

	SDEC_PRINT_TRACE( "-->SDEC_IO_BDRC_SetWritePtrUpdated");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}



/**
********************************************************************************
* @brief
*   select input port.
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
int SDEC_IO_CfgIOPort
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_CFG_IO_T *stpLXSdecCfgIOPort)
{
	int 		eRet 		= RET_ERROR;
	UINT8				reg_val		= 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecCfgIOPort == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_CfgIOPort");

	if(stpLXSdecCfgIOPort->ePort == LX_SDEC_SERIAL_IO_0)
	{
		/* stpio_en_ctrl, 1 is set by output, 0 is set by input */
		if(stpLXSdecCfgIOPort->en_out != 0) reg_val = 1;

		if(0){}
#ifdef INCLUDE_H15_CHIP_KDRV
		else if(lx_chip() == LX_CHIP_H15)
		{
			CTOP_CTRL_H15A0_RdFL(CVI, ctr36);
			CTOP_CTRL_H15A0_Wr01(CVI, ctr36, stpio_en_ctrl, reg_val);
			CTOP_CTRL_H15A0_WrFL(CVI, ctr36);
		}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
		else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		{
			CTOP_CTRL_H14A0_RdFL(ctr58);
			CTOP_CTRL_H14A0_Wr01(ctr58, stpio_en_ctrl, reg_val);
			CTOP_CTRL_H14A0_WrFL(ctr58);
		}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
		else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		{
			CTOP_CTRL_M14B0_RdFL(TOP, ctr08);
			CTOP_CTRL_M14B0_Wr01(TOP, ctr08, stpio_en_ctrl, reg_val);
			CTOP_CTRL_M14B0_WrFL(TOP, ctr08);
		}
		else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
		{
			CTOP_CTRL_M14A0_RdFL(ctr58);
			CTOP_CTRL_M14A0_Wr01(ctr58, stpio_en_ctrl, reg_val);
			CTOP_CTRL_M14A0_WrFL(ctr58);
		}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
		else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
		{
			CTOP_CTRL_H13A0_RdFL(ctr58);
			CTOP_CTRL_H13A0_Wr01(ctr58, stpio_en_ctrl, reg_val);
			CTOP_CTRL_H13A0_WrFL(ctr58);
		}
#endif
		else
		{
			SDEC_PRINT_ERROR("Invalid Chip : 0x%x\n", lx_chip_rev());
		}
	}
	else if(stpLXSdecCfgIOPort->ePort == LX_SDEC_PARALLEL_IO_0)
	{
		/* tpio_sel_ctrl, 1 is set by input, 0 is set by output */
		if(stpLXSdecCfgIOPort->en_out == 0) reg_val = 1;
		else 								reg_val = 0;


		if(0){}
#ifdef INCLUDE_H15_CHIP_KDRV
		else if(lx_chip() == LX_CHIP_H15)
		{
			CTOP_CTRL_H15A0_RdFL(VIP, ctr36);
			CTOP_CTRL_H15A0_Wr01(VIP, ctr36, tpio_sel_ctrl, reg_val); /* Set Parallel IO as Output */
			CTOP_CTRL_H15A0_WrFL(VIP, ctr36);
		}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
		else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
		{
			CTOP_CTRL_H14A0_RdFL(ctr58);
			CTOP_CTRL_H14A0_Wr01(ctr58, tpio_sel_ctrl, reg_val);
			CTOP_CTRL_H14A0_WrFL(ctr58);
		}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
		else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
		{
			CTOP_CTRL_M14B0_RdFL(TOP, ctr08);
			CTOP_CTRL_M14B0_Wr01(TOP, ctr08, tpio_sel_ctrl, reg_val);
			CTOP_CTRL_M14B0_WrFL(TOP, ctr08);
		}
		else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
		{
			CTOP_CTRL_M14A0_RdFL(ctr58);
			CTOP_CTRL_M14A0_Wr01(ctr58, tpio_sel_ctrl, reg_val);
			CTOP_CTRL_M14A0_WrFL(ctr58);
		}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
		else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
		{
			CTOP_CTRL_H13A0_RdFL(ctr58);
			CTOP_CTRL_H13A0_Wr01(ctr58, tpio_sel_ctrl, reg_val);
			CTOP_CTRL_H13A0_WrFL(ctr58);
		}
#endif
		else
		{
			SDEC_PRINT_ERROR("Invalid Chip : 0x%x\n", lx_chip_rev());
		}
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_CfgIOPort");

	eRet = RET_OK;

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}
/* End Of Serial TS OUT Stub */


void SDEC_TPIIntr(struct work_struct *work)
{
	S_SDEC_PARAM_T 	*stpSdecParam;
	UINT8 	ui8Ch 		= 0x0;
	UINT8	ui8PIDIdx	= 0x0;

	stpSdecParam =	container_of (work, S_SDEC_PARAM_T, TPIIntr);

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return, "Invalid parameters" );

	ui8Ch			= stpSdecParam->stTPI_Intr.ui8Channel;
	ui8PIDIdx 		= stpSdecParam->stTPI_Intr.ui8PIDIdx;

	SDEC_SetPidf_TPI_IEN_Enable(stpSdecParam, ui8Ch, ui8PIDIdx, SDEC_HAL_DISABLE);
	SDEC_HAL_TPISetIntrPayloadUnitStartIndicator(ui8Ch, SDEC_HAL_DISABLE);
}


/**
********************************************************************************
* @brief
*   copy all registers to mem(shadow)
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*  none
* @return
*  int
********************************************************************************
*/
int SDEC_IO_Suspend(S_SDEC_PARAM_T *param)
{
	/* Register Offsets */
	UINT32 ui32RegOfs;
	int core, ch;
	LX_SDEC_CFG_T* cfg = SDEC_CFG_GetConfig();

	SDEC_PRINT_TRACE( "<--SDEC_IO_Suspend");

	if(cfg->instantBoot)
	{
		param->reg_backup = (SDEC_REG_BACKUP_T*)OS_KMalloc(sizeof(SDEC_REG_BACKUP_T));
		SDEC_CHECK_ERROR(param->reg_backup == NULL, return RET_ERROR, "kmalloc fail");

		/* SDEC Top Reg */
		/* TODO: Prevent to use hard coding (0x0c0) */
		for(ui32RegOfs=0; ui32RegOfs<sizeof(SDTOP_REG_T); ui32RegOfs+=4)
		{
			param->reg_backup->top[ui32RegOfs/4] = SDTOP_RdPHYofs(ui32RegOfs);
			//SDTOP_RdFLOfs(ui32RegOfs);
		}

		/* SDEC IO Reg */
		for(ui32RegOfs=0; ui32RegOfs<sizeof(SDIO_REG_T); ui32RegOfs+=4)
		{
			for(core=0; core<2; core++)
			{
				param->reg_backup->io[core][ui32RegOfs/4] = SDIO_RdPHYofs(core,ui32RegOfs);
				//SDIO_RdFLOfs(core, ui32RegOfs);
			}
		}

		/* SDEC Core Reg */
		for(ui32RegOfs=0; ui32RegOfs<sizeof(MPG_REG_T); ui32RegOfs+=4)
		{
			for(core=0; core<2; core++)
			{
				for(ch=0; ch<2; ch++)
				{
					param->reg_backup->mpg[core][ch][ui32RegOfs/4] = MPG_RdPHYofs(core,ch,ui32RegOfs);
					//MPG_RdFLOfs(core, ch, ui32RegOfs); /* Core 1, Ch B */
				}
			}
		}
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_Suspend");
	return 0;
}

/* Restore shadow Mem to registers */
int SDEC_IO_Resume(S_SDEC_PARAM_T *param)
{
	/* Register Offsets */
	UINT32 ui32RegOfs, ui32Value;
	int core, ch;
	int idx, word, ret;
	LX_SDEC_CFG_T* cfg = SDEC_CFG_GetConfig();

	SDEC_PRINT_TRACE( "<--SDEC_IO_Resume");

	SDEC_InitHW(param);

	if(cfg->instantBoot)
	{

		/* SDEC Top Reg */
		for(ui32RegOfs=0; ui32RegOfs<sizeof(SDTOP_REG_T); ui32RegOfs+=4)
		{
			SDTOP_WrPHYofs(ui32RegOfs, param->reg_backup->top[ui32RegOfs/4]);
			//SDTOP_WrFLOfs(ui32RegOfs);
		}


		/* SDEC IO Reg */
		for(ui32RegOfs=0; ui32RegOfs<sizeof(SDIO_REG_T); ui32RegOfs+=4)
		{
			for(core=0; core<2; core++)
			{
				SDIO_WrPHYofs(core, ui32RegOfs, param->reg_backup->io[core][ui32RegOfs/4]);
				//SDIO_RdFLOfs(core, ui32RegOfs);
			}
		}

		/* SDEC Core Reg */
		for(ui32RegOfs=0; ui32RegOfs<sizeof(MPG_REG_T); ui32RegOfs+=4)
		{
			for(core=0; core<2; core++)
			{
				for(ch=0; ch<2; ch++)
				{
					MPG_WrPHYofs(core, ch, ui32RegOfs, param->reg_backup->mpg[core][ch][ui32RegOfs/4]);
					//MPG_RdFLOfs(core, ch, ui32RegOfs);
				}
			}
		}

		/* GetpidFilter */
		for(idx=0; idx<64; idx++)
		{
			for(ch=0; ch<2; ch++){
				ui32Value=SDEC_HAL_PIDFGetPidfData(ch, idx);
				ret = SDEC_HAL_PIDFSetPidfData(ch, idx, ui32Value);
			}
			for(ch=4; ch<6; ch++){
				ui32Value=SDEC_HAL_PIDFGetPidfData(ch, idx);
				ret = SDEC_HAL_PIDFSetPidfData(ch, idx, ui32Value);
			}
		}

		/* Section Filter */
		for(idx=0; idx<64; idx++)
		{
			for(word=0; word<7; word++)
			{
				for(ch=0; ch<2; ch++){
					ui32Value=SDEC_HAL_SECFGetSecfData(ch, idx, word);
					ret = SDEC_HAL_SECFSetSecfData(ch, idx, word, ui32Value);
				}
				for(ch=4; ch<6; ch++){
					ui32Value=SDEC_HAL_SECFGetSecfData(ch, idx, word);
					ret = SDEC_HAL_SECFSetSecfData(ch, idx, word, ui32Value);
				}
			}
		}

		OS_KFree(param->reg_backup);
	}
	SDEC_PRINT_TRACE( "-->SDEC_IO_Resume");
	return 0;
}

