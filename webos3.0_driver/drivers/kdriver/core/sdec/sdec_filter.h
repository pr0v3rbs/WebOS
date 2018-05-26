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


#ifndef _SDEC_FILTER_H_
#define _SDEC_FILTER_H_

/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sdec_impl.h"
#include "sdec_core.h"


#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


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
/// PID filter
int SDEC_IO_PIDFilterAlloc(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_ALLOC_T *stpLXSdecPIDFltAlloc);
int SDEC_IO_PIDFilterFree(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_FREE_T *stpLXSdecPIDFltFree);
int SDEC_IO_PIDFilterSetPID(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_SET_PID_T *stpLXSdecPIDFltSetPID);
int SDEC_IO_PIDFilterMapSelect(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_SELSECFLT_T *stpLXSdecPIDFltSelect);
int SDEC_IO_PIDFilterEnable(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_ENABLE_T *stpLXSdecPIDFltEnable);
int SDEC_IO_PIDFilterCRCEnable(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_ENABLE_T *stpLXSdecPIDFltPESCRCEnable);
int SDEC_IO_PIDFilterGetState(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_STATE_T *stpLXSdecPIDFltState);
int SDEC_IO_PIDFilterEnableSCMBCHK(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_ENABLE_T *stpLXSdecPIDFltEnable);
int SDEC_IO_PIDFilterEnableDownload(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_ENABLE_T *stpLXSdecPIDFltEnableDownload);

/// Section filter
int SDEC_IO_SectionFilterAlloc(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_ALLOC_T *stpLXSdecSecFltAlloc);
int SDEC_IO_SectionFilterFree(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_FREE_T *stpLXSecPIDFltFree);
int SDEC_IO_PESFilterFree(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_FREE_T *stpLXSecPIDFltFree);
int SDEC_IO_SectionFilterPattern(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_PATTERN_T *stpLXSecFltPattern);
int SDEC_IO_SectionFilterBufferReset(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_BUFFER_RESET *stpLXSdecSecfltBufferReset);
int SDEC_IO_SectionFilterBufferSet(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_BUFFER_SET_T *stpLXSdecSecfltBufferSet);
int SDEC_IO_SectionFilterDummyBufferSet(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_BUFFER_SET_T *stpLXSdecSecfltBufferSet);
int SDEC_IO_SectionFilterGetInfo(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_BUFFER_GET_INFO_T *stpLXSdecSecfltBufferGetInfo);
int SDEC_IO_SectionFilterSetReadPtr(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_READPTR_SET_T *stpLXSdecSecfltReadPtrSet);
int SDEC_IO_SectionFilterGetState(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_STATE_T *stpLXSdecSecfltState);
int SDEC_IO_SectionFilterGetAvailableNumber(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_GET_AVAIL_NUMBER_T *stpLXSdecSecFltAvailNum);

int SDEC_Pidf_Clear(S_SDEC_PARAM_T *stpSdecParam,  UINT8 ui8ch,UINT32 ui8PidfIdx);
int SDEC_Secf_Clear(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8ch, UINT32 ui8PidfIdx);
int SDEC_PIDIdxCheck(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8ch,UINT8 *ui8pPidIdx, LX_SDEC_PIDFLT_MODE_T ePidFltMode, UINT16 ui16PidValue);
int SDEC_SecIdxCheck(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8ch,UINT8 *ui8pSecIdx);

int SDEC_SetPidfData(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch,	UINT8 ui8PIDIdx, UINT32 val);
UINT32 SDEC_GetPidfData(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch, UINT8 ui8PIDIdx);
int SDEC_SetPidf_Enable(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch, UINT8 ui8PIDIdx, UINT8 en);
int SDEC_SetPidf_TPI_IEN_Enable(S_SDEC_PARAM_T *stpSdecParam,	UINT8 ui8Ch, UINT8 ui8PIDIdx, UINT8 en);
int SDEC_SetPidf_Disc_Enable(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch,	UINT8 ui8PIDIdx, UINT8 en);
int SDEC_SetPidf_DN_Enable(S_SDEC_PARAM_T *stpSdecParam, UINT8 ui8Ch, UINT8 ui8PIDIdx, UINT8 en);


int SDEC_IO_SectionFilterSetWritePtr(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_WRITEPTR_SET_T *stpLXSdecSecfltWritePtrSet);
int SDEC_IO_SectionFilterGetReadPtr(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_GET_READPTR_T *stpLXSdecSecfltReadPtrGet);
int SDEC_IO_SectionFilterGetWritePtr(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SECFLT_GET_WRITEPTR_T *stpLXSdecSecfltWritePtrGet);



/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SDEC_FILTER_H_ */

/** @} */

