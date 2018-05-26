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


#ifndef _SDEC_PCR_H_
#define _SDEC_PCR_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "os_util.h"
#include "sdec_kapi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define __NEW_PWM_RESET_COND__	1


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


int SDEC_IO_SetPCRPID(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_PIDFLT_SET_PCRPID_T *stpLXSdecPIDFltSetPID);
int SDEC_IO_SetPcrRecovery(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_SET_PCR_RECOVERY_T *stpLXSdecSetPCRRecovery);


int SDEC_IO_SetSTCMultiply(S_SDEC_PARAM_T *stpSdecParam, UINT32 *pui32Arg);

void SDEC_PCRRecovery(struct work_struct *work);
UINT32 SDEC_IO_SettingFCW(S_SDEC_PARAM_T *stpSdecParam, UINT32 new_fcw_value);
void SDEC_PWM_Init(S_SDEC_PARAM_T	*stpSdecParam );

void SDEC_IO_SetPWMResetCondition(S_SDEC_PARAM_T *stpSdecParam, UINT8 ch, BOOLEAN reset);


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SDEC_PCR_H_ */

/** @} */

