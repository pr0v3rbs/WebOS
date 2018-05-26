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

/** @file pe_spd.h
 *
 *  suspend mode util header for pe device. ( used only within kdriver )
 *	
 *	@author		
 *	@version	0.1
 *	@note		
 *	@date		2011.06.11
 *	@see		
 */

#ifndef	_PE_SPD_H_
#define	_PE_SPD_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
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
#define PE_COPY_FROM_USER(indx, to, from, n) PE_SPD_copy_from_user(g_pe_suspended, indx, to, from, n)
#define PE_BACK_FROM_USER(indx, to, from, n) PE_SPD_copy_from_user(1, indx, to, from, n)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	unsigned int bOn;
	unsigned short indx;
	unsigned short size;
	unsigned int functype;
	int (*susF)(unsigned int *data, unsigned int functype, unsigned int datasize);
	char *pData;
} PE_SUSPEND_STORE_T;

typedef enum {
	PE_SUSPEND_CMN_SETCONTRAST 		, 
	PE_SUSPEND_CMN_SETBRIGHTNESS		,
	PE_SUSPEND_CMN_SETSATURATION		,
	PE_SUSPEND_CMN_SETHUE				,
	PE_SUSPEND_CMN_SETDEFAULT		,
	PE_SUSPEND_CCM_GAMMALUT			,
	PE_SUSPEND_CCM_PIXEL_REPLACE	,
	PE_SUSPEND_CCM_WB				,
	PE_SUSPEND_CCM_SETDEFAULT		,
	PE_SUSPEND_DCM_DCE_CONF			,
	PE_SUSPEND_DCM_DCE_LUT			,
	PE_SUSPEND_DCM_DSE_LUT			,
	PE_SUSPEND_DCM_SETDEFAULT		,
	PE_SUSPEND_CSC_XVYCC			,
	PE_SUSPEND_CSC_GAMUT			,
	PE_SUSPEND_CSC_SETDEFAULT		,
	PE_SUSPEND_CMG_REGION_ENABLE	,
	PE_SUSPEND_CMG_REGION			,
	PE_SUSPEND_CMG_REGION_CTRL		,
	PE_SUSPEND_CMG_GLOBAL_CTRL		,
	PE_SUSPEND_CMG_SETDEFAULT		,
	PE_SUSPEND_NRD_SETDEFAULT		,
	PE_SUSPEND_DNT_FILMMODE			,
	PE_SUSPEND_DNT_SETDEFAULT		,
	PE_SUSPEND_SHP_SCLFILTER		,
	PE_SUSPEND_SHP_SETDEFAULT		,
	PE_SUSPEND_WIN_SETDEFAULT		,
	PE_SUSPEND_D3D_SETDEFAULT		,	
	PE_SUSPEND_MAX 			
} PE_SUSPEND_INDX_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern int PE_SPD_copy_from_user(unsigned short bSuspended, unsigned short indx, void * to, const void __user * from, unsigned int n);
extern int PE_SPD_suspend(void);
extern int PE_SPD_probe(void);
extern int PE_SPD_remove(void);
extern int PE_SPD_resume(void);
extern int PE_SPD_release(void);
extern int PE_SPD_CheckInitResume(unsigned int *data, unsigned int datasize);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern int 	g_pe_suspended;


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_SPD_H_ */
