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

/** @file pe_spd.c
 *
 *  suspend mode util functions for pe device. ( used only within kdriver )
 *	
 *	@author		
 *	@version	0.1
 *	@note		
 *	@date		2011.06.11
 *	@see		
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "pe_kapi.h"
#include "pe_cfg.h"
#include "pe_def.h"
#include "pe_spd.h"

#include "pe_cmn.h"
#include "pe_nrd.h"
#include "pe_ccm.h"
#include "pe_dcm.h"
#include "pe_csc.h"
#include "pe_cmg.h"
#include "pe_nrd.h"
#include "pe_dnt.h"
#include "pe_shp.h"
#include "pe_win.h"
#include "pe_etc.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_SPD_CHECK_CODE(_checker,_action,fmt,args...)		\
	if (_checker)\
	{\
		PE_PRINT_ERROR(fmt,##args);\
		_action;\
	}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
PE_SUSPEND_STORE_T pe_suspend_store[] = {
	{FALSE  ,PE_SUSPEND_CMN_SETCONTRAST		,sizeof(LX_PE_CMN_CONTRAST_T)		,PE_ITEM_PKTFUNC(LX_PE_CMN_CONTRAST_T), PE_CMN_SetFunction 	   ,NULL},
	{FALSE  ,PE_SUSPEND_CMN_SETBRIGHTNESS	,sizeof(LX_PE_CMN_BRIGHTNESS_T)		,PE_ITEM_PKTFUNC(LX_PE_CMN_BRIGHTNESS_T), PE_CMN_SetFunction	   ,NULL},
	{FALSE  ,PE_SUSPEND_CMN_SETSATURATION	,sizeof(LX_PE_CMN_SATURATION_T)		,PE_ITEM_PKTFUNC(LX_PE_CMN_SATURATION_T), PE_CMN_SetFunction	   ,NULL},
	{FALSE  ,PE_SUSPEND_CMN_SETHUE			,sizeof(LX_PE_CMN_HUE_T)			,PE_ITEM_PKTFUNC(LX_PE_CMN_HUE_T), PE_CMN_SetFunction			   ,NULL},
	{FALSE  ,PE_SUSPEND_CMN_SETDEFAULT		,sizeof(LX_PE_DEFAULT_SETTINGS_T)	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_CMN_SetFunction	   ,NULL},
	{FALSE	,PE_SUSPEND_CCM_GAMMALUT		,sizeof(LX_PE_CCM_GAMMALUT_T)		,PE_ITEM_PKTFUNC(LX_PE_CCM_GAMMALUT_T), PE_CCM_SetFunction	   ,NULL},
	{FALSE	,PE_SUSPEND_CCM_PIXEL_REPLACE	,sizeof(LX_PE_CCM_PIXEL_REPLACE_T)	,PE_ITEM_PKTFUNC(LX_PE_CCM_PIXEL_REPLACE_T), PE_CCM_SetFunction	   ,NULL},
	{FALSE	,PE_SUSPEND_CCM_WB				,sizeof(LX_PE_CCM_WB_T)				,PE_ITEM_PKTFUNC(LX_PE_CCM_WB_T), PE_CCM_SetFunction    ,NULL},
	{FALSE	,PE_SUSPEND_CCM_SETDEFAULT		,sizeof(LX_PE_DEFAULT_SETTINGS_T) 	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_CCM_SetFunction	,NULL},
	{FALSE	,PE_SUSPEND_DCM_DCE_CONF		,sizeof(LX_PE_DCM_DCE_CONF_T)		,PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_CONF_T), PE_DCM_SetFunction	,NULL},
	{FALSE	,PE_SUSPEND_DCM_DCE_LUT			,sizeof(LX_PE_DCM_DCE_LUT_T)		,PE_ITEM_PKTFUNC(LX_PE_DCM_DCE_LUT_T), PE_DCM_SetFunction	,NULL},
	{FALSE	,PE_SUSPEND_DCM_DSE_LUT			,sizeof(LX_PE_DCM_DSE_LUT_T)		,PE_ITEM_PKTFUNC(LX_PE_DCM_DSE_LUT_T), PE_DCM_SetFunction	,NULL},
	{FALSE	,PE_SUSPEND_DCM_SETDEFAULT 		,sizeof(LX_PE_DEFAULT_SETTINGS_T)	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_DCM_SetFunction	,NULL},
	{FALSE	,PE_SUSPEND_CSC_XVYCC			,sizeof(LX_PE_CSC_GAMUT_T)			,PE_ITEM_PKTFUNC(LX_PE_CSC_GAMUT_T), PE_CSC_SetFunction	,NULL},
	{FALSE	,PE_SUSPEND_CSC_GAMUT			,sizeof(LX_PE_CSC_XVYCC_T)			,PE_ITEM_PKTFUNC(LX_PE_CSC_XVYCC_T), PE_CSC_SetFunction	,NULL},
	{FALSE	,PE_SUSPEND_CSC_SETDEFAULT		,sizeof(LX_PE_DEFAULT_SETTINGS_T)	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_CSC_SetFunction ,NULL},
	{FALSE	,PE_SUSPEND_CMG_REGION_ENABLE	,sizeof(LX_PE_CMG_REGION_ENABLE_T)	,PE_ITEM_PKTFUNC(LX_PE_CMG_REGION_ENABLE_T), PE_CMG_SetFunction ,NULL},
	{FALSE	,PE_SUSPEND_CMG_REGION			,sizeof(LX_PE_CMG_REGION_T)			,PE_ITEM_PKTFUNC(LX_PE_CMG_REGION_T), PE_CMG_SetFunction ,NULL},
	{FALSE	,PE_SUSPEND_CMG_REGION_CTRL		,sizeof(LX_PE_CMG_REGION_CTRL_T) 	,PE_ITEM_PKTFUNC(LX_PE_CMG_REGION_CTRL_T), PE_CMG_SetFunction ,NULL},
	{FALSE	,PE_SUSPEND_CMG_GLOBAL_CTRL 	,sizeof(LX_PE_CMG_GLOBAL_CTRL_T)	,PE_ITEM_PKTFUNC(LX_PE_CMG_GLOBAL_CTRL_T), PE_CMG_SetFunction ,NULL},
	{FALSE	,PE_SUSPEND_CMG_SETDEFAULT 		,sizeof(LX_PE_DEFAULT_SETTINGS_T)	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_CMG_SetFunction ,NULL},		
	{FALSE	,PE_SUSPEND_NRD_SETDEFAULT 		,sizeof(LX_PE_DEFAULT_SETTINGS_T)	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_NRD_SetFunction ,NULL},		
	{FALSE	,PE_SUSPEND_DNT_FILMMODE		,sizeof(LX_PE_DNT_FILMMODE_T)		,PE_ITEM_PKTFUNC(LX_PE_DNT_FILMMODE_T), PE_DNT_SetFunction ,NULL},		
	{FALSE	,PE_SUSPEND_DNT_SETDEFAULT		,sizeof(LX_PE_DEFAULT_SETTINGS_T)	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_DNT_SetFunction ,NULL},		
	{FALSE	,PE_SUSPEND_SHP_SCLFILTER		,sizeof(LX_PE_SHP_SCLFILTER_T)		,PE_ITEM_PKTFUNC(LX_PE_SHP_SCLFILTER_T), PE_SHP_SetFunction ,NULL},		
	{FALSE	,PE_SUSPEND_SHP_SETDEFAULT		,sizeof(LX_PE_DEFAULT_SETTINGS_T)	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_SHP_SetFunction ,NULL},		
	{FALSE	,PE_SUSPEND_WIN_SETDEFAULT		,sizeof(LX_PE_DEFAULT_SETTINGS_T)	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_WIN_SetFunction ,NULL},			
	{FALSE	,PE_SUSPEND_D3D_SETDEFAULT		,sizeof(LX_PE_DEFAULT_SETTINGS_T)	,PE_ITEM_PKTFUNC(LX_PE_DEFAULT_SETTINGS_T), PE_ETC_SetFunction ,NULL}, 		
};

int     g_pe_suspended = FALSE;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

int PE_SPD_copy_from_user(unsigned short bSuspended, unsigned short indx, void * to, const void __user * from, unsigned int n)
{
	int ret = RET_OK;
	PE_SUSPEND_STORE_T *pStore = NULL;

	do {
		if (!bSuspended) {
			if (!memcpy(to, from, n)) BREAK_WRONG(n);
		}
		if (!n) break;
		if ( !(indx < ARRAY_SIZE(pe_suspend_store)) ) BREAK_WRONG(indx);
		pStore = &pe_suspend_store[indx];
		CHECK_KNULL(pStore);
		CHECK_KNULL(pStore->pData);
		if (pStore->indx != indx) BREAK_WRONG(indx);
		if (!bSuspended) {
			if (!memcpy(pStore->pData, to, n)) BREAK_WRONG(n);
			pStore->size = n;
			pStore->bOn = TRUE;
		} else {
			if (!pStore->size) BREAK_SETV(ret, 1);
			if (pStore->size != n) BREAK_WRONG(n);
			if (!memcpy(to, pStore->pData, n)) BREAK_WRONG(n);
			if (!memcpy(to, pStore->pData, n)) BREAK_WRONG(n);
		}
	} while (0);

	return ret;
}

int PE_SPD_suspend(void)
{
	int ret = RET_OK;

    g_pe_suspended = 1;

	return ret;
}

int PE_SPD_probe(void)
{
	int ret = RET_OK;
	PE_SUSPEND_STORE_T *pStore = NULL;
	int indx;

	for (indx=0;indx<ARRAY_SIZE(pe_suspend_store);indx++) {
		pStore = &pe_suspend_store[indx];
		CHECK_KNULL(pStore);
		pStore->pData = OS_Malloc(pStore->size);
		CHECK_KNULL(pStore->pData);
		pStore->size = 0;
	}

    g_pe_suspended = 0;

	PE_PRINT_NOTI("%s[%d] : probe\n", __F__, __L__);

	return ret;
}

int PE_SPD_remove(void)
{
	int ret = RET_OK;
	PE_SUSPEND_STORE_T *pStore = NULL;
	int indx;

	for (indx=0;indx < ARRAY_SIZE(pe_suspend_store);indx++) {
		pStore = &pe_suspend_store[indx];
		CHECK_KNULL(pStore);
		OS_Free(pStore->pData);
	}
	PE_PRINT_NOTI("%s[%d] : remove\n", __F__, __L__);

	return ret;
}

int PE_SPD_CheckInitResume(unsigned int *data, unsigned int datasize)
{
	int check_resume = 0;
	LX_PE_INIT_SETTINS_T *pRcvData;

	do{
		/* data null check */
		PE_SPD_CHECK_CODE(!data,break,"[%s,%d] data is null.\n",__F__,__L__);
		/* data size check */
		PE_SPD_CHECK_CODE(datasize!=sizeof(LX_PE_INIT_SETTINS_T),break,"[%s,%d] size mismatch.\n",__F__,__L__);

		pRcvData = (LX_PE_INIT_SETTINS_T *)data;
		check_resume = (pRcvData->suspend_mode)? 1:0;
		PE_PRINT_DEBUG("check_resume:%d\n",check_resume);
	}while(0);

	return check_resume;
}

int PE_SPD_resume(void)
{
	int ret = RET_OK;
	PE_SUSPEND_STORE_T *pStore = NULL;
	int indx;

	for (indx=0;indx<ARRAY_SIZE(pe_suspend_store);indx++) {
		pStore = &pe_suspend_store[indx];
		CHECK_KNULL(pStore);
		if (!pStore->bOn) continue;
		ret = pStore->susF((unsigned int *)pStore->pData, pStore->functype, pStore->size);
		if (ret > 0) continue;
		if (ret) break;
	}

    g_pe_suspended = 0;

	PE_PRINT_NOTI("%s[%d] : resume\n", __F__, __L__);

	return ret;
}

int PE_SPD_release(void)
{
	int ret = RET_OK;

	return ret;
}

