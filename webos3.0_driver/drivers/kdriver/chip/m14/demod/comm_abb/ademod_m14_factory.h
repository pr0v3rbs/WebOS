/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 


/*****************************************************************************
**
**  Name:ademod_m14_factory.h
**
**  Description:   ABB M14 factory settings.
**
**  Functions
**
**  References:    
**
**  Dependencies:   ademod_common.h   - for basic FM types,
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/

#ifndef _ADEMOD_M14_FACTORY_H_
#define _ADEMOD_M14_FACTORY_H_

#include "ademod_common.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

#define LX_ADEMOD_MAX_PARAM_SIZE (16)

typedef INT64 LX_ADEMOD_PARAM[LX_ADEMOD_MAX_PARAM_SIZE];
typedef LX_ADEMOD_PARAM LX_ADEMOD_FACTORY_SETTINGS[];

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Factory_Table_Get
**
**  Description:    Returns Factory settings table.
**
**  Parameters:     pLX_ADEMOD_Demod_Instance  - pointer to context,
**					Table		  - Factory table.
**					Size		  - Table size.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Factory_Table_Get(pLX_ADEMOD_Demod_Instance pContext, const LX_ADEMOD_FACTORY_SETTINGS **Table, UINT32 *Size);

#if defined( __cplusplus )
}
#endif

#endif //_ADEMOD_M14_FACTORY_H_
