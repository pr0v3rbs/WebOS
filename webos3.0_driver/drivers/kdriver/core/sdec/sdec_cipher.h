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


#ifndef _SDEC_CIPHER_H_
#define _SDEC_CIPHER_H_

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
int SDEC_CIPHER_Init(S_SDEC_PARAM_T *stpSdecParam);

int SDEC_IO_SetCipherEnable(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_CIPHER_ENABLE_T *stLXSdecCipherEnable);
int SDEC_IO_SetCipherMode(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_CIPHER_MODE_T *stpLXSdecCipherMode);
int SDEC_IO_SetCipherKey(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_CIPHERKEY_T *stpLXSdecCipherKey);
int SDEC_IO_GetCipherKey(S_SDEC_PARAM_T *stpSdecParam, LX_SDEC_GET_CIPHERKEY_T *stpLXSdecCipherKey);


/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SDEC_CIPHER_H_ */

/** @} */
