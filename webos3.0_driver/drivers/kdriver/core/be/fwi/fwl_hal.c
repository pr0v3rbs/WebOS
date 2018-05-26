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


/** @file
 *
 *  fw loader hal driver file for BE device
 *
 *  author		dj911.kim
 *  version		1.0
 *  date		2014.06.02
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_def.h"

#include "h13/be/fwi/be_fwl_hw_h13.h"
#include "h14/be/fwi/be_fwl_hw_h14.h"
#include "m14/be/fwi/be_fwl_hw_m14.h"
#include "h15/be/fwi/be_fwl_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	int (*pfnInitializeFWL)(void);
	int (*pfnCloseFWL)(void);
	BE_FWL_HAL_FW_FILE_T * (*pfnFwBinInfoFWL)(UINT8 ucIndex);
	BE_FWL_HAL_FW_T * (*pfnFwHeaderInfoFWL)(UINT8 ucIndex);
	UINT32 (*pfnFwDownloadFWL)(UINT8 ucIndex, UINT8 *pucBuff, UINT32 ulSize, BOOLEAN bIsMcuFw);
	BOOLEAN (*pfnMcuIsAliveFWL)(void);
	UINT32 (*pfnGetFwMemBaseSizeFWL)(UINT8 ucIndex);
	int (*pfnFrcFwInitFWL)(void);
	int (*pfnSreFwInitFWL)(UINT32 u32FwBaseAddr);
	int (*pfnWcgFwInitFWL)(UINT32 u32FwBaseAddr);
	int (*pfnLedFwInitFWL)(UINT32 u32FwBaseAddr);
	int (*pfnHdrFwInitFWL)(UINT32 u32FwBaseAddr);
} BE_FWL_HW_FUNCTION_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static BE_FWL_HW_FUNCTION_T gstBeFwlHwFunction;

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_FWL_HAL_Init(void)
{
	int ret = RET_OK;

	if(0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		BE_PRINT("BE FWL chip revision is set to H15 A0\n");
		gstBeFwlHwFunction.pfnInitializeFWL				= NULL;
		gstBeFwlHwFunction.pfnCloseFWL				= NULL;
		gstBeFwlHwFunction.pfnFwBinInfoFWL			= NULL;
		gstBeFwlHwFunction.pfnFwHeaderInfoFWL		= BE_FWL_HW_H15_GetFwHeaderInfo;
		gstBeFwlHwFunction.pfnFwDownloadFWL			= BE_FWL_HW_H15_FW_Download;
		gstBeFwlHwFunction.pfnMcuIsAliveFWL			= BE_FWL_HW_H15_MCUisAlive;
		gstBeFwlHwFunction.pfnGetFwMemBaseSizeFWL	= BE_FWL_HW_H15_GetFwMemBaseSize;
		gstBeFwlHwFunction.pfnFrcFwInitFWL			= NULL;
		gstBeFwlHwFunction.pfnSreFwInitFWL			= NULL;
		gstBeFwlHwFunction.pfnWcgFwInitFWL			= BE_FWL_HW_H15_WcgFwInit;
		gstBeFwlHwFunction.pfnLedFwInitFWL			= NULL;
		gstBeFwlHwFunction.pfnHdrFwInitFWL			= NULL;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		BE_PRINT("BE FWL chip revision is set to H14 A0\n");
		gstBeFwlHwFunction.pfnInitializeFWL				= NULL;
		gstBeFwlHwFunction.pfnCloseFWL				= NULL;
		gstBeFwlHwFunction.pfnFwBinInfoFWL			= NULL;
		gstBeFwlHwFunction.pfnFwHeaderInfoFWL		= BE_FWL_HW_H14_GetFwHeaderInfo;
		gstBeFwlHwFunction.pfnFwDownloadFWL			= BE_FWL_HW_H14_FW_Download;
		gstBeFwlHwFunction.pfnMcuIsAliveFWL			= BE_FWL_HW_H14_MCUisAlive;
		gstBeFwlHwFunction.pfnGetFwMemBaseSizeFWL	= BE_FWL_HW_H14_GetFwMemBaseSize;
		gstBeFwlHwFunction.pfnFrcFwInitFWL			= BE_FWL_HW_H14_FrcFwInit;
		gstBeFwlHwFunction.pfnSreFwInitFWL			= BE_FWL_HW_H14_SreFwInit;
		gstBeFwlHwFunction.pfnWcgFwInitFWL			= BE_FWL_HW_H14_WcgFwInit;
		gstBeFwlHwFunction.pfnLedFwInitFWL			= BE_FWL_HW_H14_LedFwInit;
		gstBeFwlHwFunction.pfnHdrFwInitFWL			= BE_FWL_HW_H14_HdrFwInit;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		BE_PRINT("BE FWL chip revision is set to M14 A0\n");
		gstBeFwlHwFunction.pfnInitializeFWL				= NULL;
		gstBeFwlHwFunction.pfnCloseFWL				= NULL;
		gstBeFwlHwFunction.pfnFwBinInfoFWL			= NULL;
		gstBeFwlHwFunction.pfnFwHeaderInfoFWL		= BE_FWL_HW_M14_GetFwHeaderInfo;
		gstBeFwlHwFunction.pfnFwDownloadFWL			= BE_FWL_HW_M14_FW_Download;
		gstBeFwlHwFunction.pfnMcuIsAliveFWL			= BE_FWL_HW_M14_MCUisAlive;
		gstBeFwlHwFunction.pfnGetFwMemBaseSizeFWL	= BE_FWL_HW_M14_GetFwMemBaseSize;
		gstBeFwlHwFunction.pfnFrcFwInitFWL			= BE_FWL_HW_M14_FrcFwInit;
		gstBeFwlHwFunction.pfnSreFwInitFWL			= BE_FWL_HW_M14_SreFwInit;
		gstBeFwlHwFunction.pfnWcgFwInitFWL			= BE_FWL_HW_M14_WcgFwInit;
		gstBeFwlHwFunction.pfnLedFwInitFWL			= BE_FWL_HW_M14_LedFwInit;
		gstBeFwlHwFunction.pfnHdrFwInitFWL			= BE_FWL_HW_M14_HdrFwInit;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, A0) )
	{
		BE_PRINT("BE FWL chip revision is set to H13 A0\n");
		gstBeFwlHwFunction.pfnInitializeFWL				= NULL;
		gstBeFwlHwFunction.pfnCloseFWL				= NULL;
		gstBeFwlHwFunction.pfnFwBinInfoFWL			= BE_FWL_HW_H13_GetFwBinInfo;
		gstBeFwlHwFunction.pfnFwHeaderInfoFWL		= BE_FWL_HW_H13_GetFwHeaderInfo;
		gstBeFwlHwFunction.pfnFwDownloadFWL			= BE_FWL_HW_H13_FW_Download;
		gstBeFwlHwFunction.pfnMcuIsAliveFWL			= BE_FWL_HW_H13_MCUisAlive;
		gstBeFwlHwFunction.pfnGetFwMemBaseSizeFWL	= BE_FWL_HW_H13_GetFwMemBaseSize;
		gstBeFwlHwFunction.pfnFrcFwInitFWL			= BE_FWL_HW_H13_FrcFwInit;
		gstBeFwlHwFunction.pfnSreFwInitFWL			= BE_FWL_HW_H13_SreFwInit;
		gstBeFwlHwFunction.pfnWcgFwInitFWL			= BE_FWL_HW_H13_WcgFwInit;
		gstBeFwlHwFunction.pfnLedFwInitFWL			= BE_FWL_HW_H13_LedFwInit;
		gstBeFwlHwFunction.pfnHdrFwInitFWL			= BE_FWL_HW_H13_HdrFwInit;
	}
#endif
	else
	{
		BE_ERROR("BE FWL ERROR! Unknown chip revision at BE FWL module\n");
		gstBeFwlHwFunction.pfnInitializeFWL				= NULL;
		gstBeFwlHwFunction.pfnCloseFWL				= NULL;
		gstBeFwlHwFunction.pfnFwBinInfoFWL			= NULL;
		gstBeFwlHwFunction.pfnFwHeaderInfoFWL		= NULL;
		gstBeFwlHwFunction.pfnFwDownloadFWL			= NULL;
		gstBeFwlHwFunction.pfnMcuIsAliveFWL			= NULL;
		gstBeFwlHwFunction.pfnGetFwMemBaseSizeFWL	= NULL;
		gstBeFwlHwFunction.pfnFrcFwInitFWL			= NULL;
		gstBeFwlHwFunction.pfnSreFwInitFWL			= NULL;
		gstBeFwlHwFunction.pfnWcgFwInitFWL			= NULL;
		gstBeFwlHwFunction.pfnLedFwInitFWL			= NULL;
		gstBeFwlHwFunction.pfnHdrFwInitFWL			= NULL;
		ret = RET_ERROR;
	}

	if(gstBeFwlHwFunction.pfnInitializeFWL)
	{
		ret = gstBeFwlHwFunction.pfnInitializeFWL();
	}

	return ret;
}

int BE_FWL_HAL_Close(void)
{
	int ret = RET_OK;

	if(gstBeFwlHwFunction.pfnCloseFWL)
	{
		ret = gstBeFwlHwFunction.pfnCloseFWL();
	}

	gstBeFwlHwFunction.pfnInitializeFWL				= NULL;
	gstBeFwlHwFunction.pfnCloseFWL				= NULL;
	gstBeFwlHwFunction.pfnFwBinInfoFWL			= NULL;
	gstBeFwlHwFunction.pfnFwHeaderInfoFWL		= NULL;
	gstBeFwlHwFunction.pfnFwDownloadFWL			= NULL;
	gstBeFwlHwFunction.pfnMcuIsAliveFWL			= NULL;
	gstBeFwlHwFunction.pfnGetFwMemBaseSizeFWL	= NULL;
	gstBeFwlHwFunction.pfnFrcFwInitFWL			= NULL;
	gstBeFwlHwFunction.pfnSreFwInitFWL			= NULL;
	gstBeFwlHwFunction.pfnWcgFwInitFWL			= NULL;
	gstBeFwlHwFunction.pfnLedFwInitFWL			= NULL;
	gstBeFwlHwFunction.pfnHdrFwInitFWL			= NULL;

	return ret;
}

BE_FWL_HAL_FW_FILE_T *BE_FWL_HAL_FwBinInfo(UINT8 ucIndex)
{
	if(gstBeFwlHwFunction.pfnFwBinInfoFWL)
	{
		return gstBeFwlHwFunction.pfnFwBinInfoFWL(ucIndex);
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		return NULL;
	}
}

BE_FWL_HAL_FW_T *BE_FWL_HAL_GetFwHeaderInfo(UINT8 ucIndex)
{
	if(gstBeFwlHwFunction.pfnFwHeaderInfoFWL)
	{
		return gstBeFwlHwFunction.pfnFwHeaderInfoFWL(ucIndex);
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		return NULL;
	}
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief download BE firmware in hal layer
 *
 * @param pstParams [IN] pointer, size information for new firmware
 *
 * @return FW base address if success, NULL for otherwise
 */
UINT32 BE_FWL_HAL_FwDownload(BE_FWL_HAL_FW_T* pstParams)
{
	if(gstBeFwlHwFunction.pfnFwDownloadFWL)
	{
		return gstBeFwlHwFunction.pfnFwDownloadFWL(pstParams->index, pstParams->pBuffer, pstParams->size, pstParams->isMcuFw);
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		return NULL;
	}
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief check mcu is alive in hal layer
 *
 * @param void
 *
 * @return TRUE if mcu run, FALSE for otherwise
 */
BOOLEAN BE_FWL_HAL_McuIsAlive(void)
{
	if(gstBeFwlHwFunction.pfnMcuIsAliveFWL)
	{
		return gstBeFwlHwFunction.pfnMcuIsAliveFWL();
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		return FALSE;
	}
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief return firmware memory base size in hal layer
 *
 * @param ucIndex [IN] index information for the firmware
 *
 * @return firmware memory base size if success, 0 for otherwise
 */
UINT32 BE_FWL_HAL_GetFwMemBaseSize(UINT8 ucIndex)
{
	if(gstBeFwlHwFunction.pfnGetFwMemBaseSizeFWL)
	{
		return gstBeFwlHwFunction.pfnGetFwMemBaseSizeFWL(ucIndex);
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		return 0;
	}
}

int BE_FWL_HAL_FrcFwInit(void)
{
	int ret = RET_OK;

	if(gstBeFwlHwFunction.pfnFrcFwInitFWL)
	{
		ret = gstBeFwlHwFunction.pfnFrcFwInitFWL();
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int BE_FWL_HAL_SreFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;

	if(gstBeFwlHwFunction.pfnSreFwInitFWL)
	{
		ret = gstBeFwlHwFunction.pfnSreFwInitFWL(u32FwBaseAddr);
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int BE_FWL_HAL_WcgFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;

	if(gstBeFwlHwFunction.pfnWcgFwInitFWL)
	{
		ret = gstBeFwlHwFunction.pfnWcgFwInitFWL(u32FwBaseAddr);
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int BE_FWL_HAL_LedFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;

	if(gstBeFwlHwFunction.pfnLedFwInitFWL)
	{
		ret = gstBeFwlHwFunction.pfnLedFwInitFWL(u32FwBaseAddr);
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int BE_FWL_HAL_HdrFwInit(UINT32 u32FwBaseAddr)
{
	int ret = RET_OK;

	if(gstBeFwlHwFunction.pfnHdrFwInitFWL)
	{
		ret = gstBeFwlHwFunction.pfnHdrFwInitFWL(u32FwBaseAddr);
	}
	else
	{
		BE_ERROR("BE_FWL_HAL  ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

