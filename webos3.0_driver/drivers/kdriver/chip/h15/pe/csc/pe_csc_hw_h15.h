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

/** @file pe_csc_hw_h15.h
 *
 *  driver header for picture enhance csc functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.03.15
 *	@see		
 */

#ifndef	_PE_CSC_HW_H15_H_
#define	_PE_CSC_HW_H15_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe csc module settings type
 */
typedef struct
{
	UINT32 pre_post_type;			///< pre post type
	UINT32 cur_post_type;			///< cur post type
	LX_PE_CSC_INPUT_T pre_in_csc;	///< pre input csc
}
PE_CSC_HW_H15_SETTINGS_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_CSC_HW_H15_Init(PE_CFG_CTRL_T *pstParams);
int PE_CSC_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams);
int PE_CSC_HW_H15_SetColorGamut(LX_PE_CSC_GAMUT_T *pstParams);
int PE_CSC_HW_H15_GetColorGamut(LX_PE_CSC_GAMUT_T *pstParams);
int PE_CSC_HW_H15_SetPostCsc(LX_PE_CSC_POST_T *pstParams);
int PE_CSC_HW_H15_GetPostCsc(LX_PE_CSC_POST_T *pstParams);
int PE_CSC_HW_H15_SetInputCsc(LX_PE_CSC_INPUT_T *pstParams);
int PE_CSC_HW_H15_GetInputCsc(LX_PE_CSC_INPUT_T *pstParams);
int PE_CSC_HW_H15_SetxvYCC(LX_PE_CSC_XVYCC_T *pstParams);
int PE_CSC_HW_H15_GetxvYCC(LX_PE_CSC_XVYCC_T *pstParams);
int PE_CSC_HW_H15_SetCscMode(PE_TSK_H15_CFG_T *pCfg);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_CSC_HW_H15_H_ */
