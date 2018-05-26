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
 *  firmware loader file for BE device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.03.05
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
#include <linux/delay.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_kapi.h"
#include "be_def.h"

#include "fwl_drv.h"
#include "fwl_hal.h"

#include "be_top_hal.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define FWL_FW_INDEX_MASK_FRC		1 << BE_FW_DWLD_TYPE_FRC
#define FWL_FW_INDEX_MASK_SRE		1 << BE_FW_DWLD_TYPE_SRE
#define FWL_FW_INDEX_MASK_WCG	1 << BE_FW_DWLD_TYPE_WCG
#define FWL_FW_INDEX_MASK_LED		1 << BE_FW_DWLD_TYPE_LED
#define FWL_FW_INDEX_MASK_HDR		1 << BE_FW_DWLD_TYPE_HDR

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	BOOLEAN bMcuIsAlive;
	UINT32 ui32FwInitReqMask;
	UINT32 ui32FwBaseAddr[BE_FW_DWLD_TYPE_MAXNUM];
	UINT32 ui32FwDnloadedFlag;
} FWL_INFO_T;

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
static int FWL_FW_DownloadFromFile(int index);
static int FWL_FW_DownloadFromHead(int index);
static int FWL_FW_Download(BE_FW_DWLD_T *pstParams, BOOLEAN fromFile);
static int FWL_FW_InitializeFirmware(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static FWL_INFO_T gstFwlInfo = 
	{
		.bMcuIsAlive = FALSE, 
		.ui32FwInitReqMask = 0x0,
		.ui32FwDnloadedFlag = 0x0
	};
const static UINT32 gui32FwIndexMask[BE_FW_DWLD_TYPE_MAXNUM] = 
	{
		FWL_FW_INDEX_MASK_FRC,
		FWL_FW_INDEX_MASK_SRE,
		FWL_FW_INDEX_MASK_WCG,
		FWL_FW_INDEX_MASK_LED,
		FWL_FW_INDEX_MASK_HDR
	};

/*========================================================================================
	Implementation Group
========================================================================================*/
int BE_FW_Init(void)
{
	int ret = RET_OK;

	ret = BE_FWL_HAL_Init();

	return ret;
}

/**
 * @callgraph
 * @callergraph
 *
 * @brief download BE firmware
 *
 * @return RET_OK(0) if success, none zero for otherwise
 */
int BE_FW_Update(BE_FW_DWLD_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_NULL(pstParams);
		
		ret = FWL_FW_Download(pstParams, FALSE);	// fromFile == TRUE : for debug
		if(ret) BREAK_WRONG(ret);

		ret = FWL_FW_InitializeFirmware();
		if(ret) BREAK_WRONG(ret);
	} while(0);

	return ret;
}

int BE_FW_GetSuspendInfo(UINT32 *pu32Info)
{
	int ret = RET_OK;

	do {
		CHECK_NULL(pu32Info);
		
		*pu32Info = gstFwlInfo.ui32FwDnloadedFlag;
	} while(0);

	return ret;
}

int BE_FW_Resume(UINT32 u32Info)
{
	int ret = RET_OK;
	UINT32 idx;

	do {
		for (idx = 0 /* BE_FW_DWLD_TYPE_FRC */; idx < BE_FW_DWLD_TYPE_MAXNUM; idx++)
		{
			if (u32Info & gui32FwIndexMask[idx])
			{
				BE_FW_DWLD_T stParams;

				stParams.pData = 0;
				stParams.size = 0;
				stParams.type = idx;

				ret = FWL_FW_Download(&stParams, FALSE);	// fromFile == TRUE : for debug
				if(ret) BREAK_WRONG(ret);

				ret = FWL_FW_InitializeFirmware();
				if(ret) BREAK_WRONG(ret);
			}
		}
	} while(0);

	return ret;
}

static int FWL_FW_Download(BE_FW_DWLD_T *pstParams, BOOLEAN fromFile)
{
	int ret = RET_OK;
	UINT32 idx;
	UINT32 count = 0;
	UINT32 swRstCnt = 0;
	#define FW_RESET_TIMEOUT	100
	#define FW_DOWNLOAD_TIMEOUT	5000

	do {
		for (idx = 0 /* BE_FW_DWLD_TYPE_FRC */; idx < BE_FW_DWLD_TYPE_MAXNUM; idx++)
		{
			if(pstParams->type == idx && pstParams->size && pstParams->pData)	// case : from user buffer
			{
				BE_FWL_HAL_FW_T fwParam;
				UINT32 fwBaseAddr = (UINT32)NULL;

				if(pstParams->size > BE_FWL_HAL_GetFwMemBaseSize(pstParams->type)) BREAK_WRONG(pstParams->size);
				
				fwParam.index = pstParams->type;
				fwParam.size = pstParams->size;
				fwParam.pBuffer = NULL;
				fwParam.isMcuFw = FALSE;
				
				if(pstParams->type == BE_FW_DWLD_TYPE_FRC)
				{
					fwParam.isMcuFw = TRUE;
				}
				fwParam.pBuffer = OS_Malloc(pstParams->size);
			 	CHECK_NULL(fwParam.pBuffer);
			    	ret = copy_from_user(fwParam.pBuffer, (void __user *)pstParams->pData, pstParams->size);
			    	if (ret) 
				{
					OS_Free(fwParam.pBuffer);
					BREAK_WRONG(ret);
			     	}
				
				fwBaseAddr = BE_FWL_HAL_FwDownload(&fwParam);
				OS_Free(fwParam.pBuffer);
				CHECK_NULL(fwBaseAddr);
				
				gstFwlInfo.ui32FwInitReqMask |= gui32FwIndexMask[idx];
				gstFwlInfo.ui32FwBaseAddr[idx] = fwBaseAddr;
				break;
			}
			else if(pstParams->type == idx || (pstParams->type == BE_FW_DWLD_TYPE_ALL && (!pstParams->size || !pstParams->pData))) // case : from header or (file)
			{
				if (fromFile) // for debug only
				{
					ret = FWL_FW_DownloadFromFile(idx);
					if(ret) BREAK_WRONG(ret);
				} 
				else 
				{
					ret = FWL_FW_DownloadFromHead(idx);
					if(ret) BREAK_WRONG(ret);
				}
				
				if(pstParams->type != BE_FW_DWLD_TYPE_ALL || idx == BE_FW_DWLD_TYPE_MAXNUM-1) break;
			}
		}
		if(idx == BE_FW_DWLD_TYPE_MAXNUM)
		{
			ret = RET_INVALID_PARAMS;
			BREAK_WRONG(ret);
		}
		if(ret) break;

		while (pstParams->type == BE_FW_DWLD_TYPE_ALL || pstParams->type == BE_FW_DWLD_TYPE_FRC) {
			if (BE_FWL_HAL_McuIsAlive()) 
			{
				gstFwlInfo.bMcuIsAlive = TRUE;
				break;
			}
			msleep(1);
			count++;
			swRstCnt++;
			if(swRstCnt == FW_RESET_TIMEOUT)
			{
				BE_ERROR("BE firmware reset\n");
				BE_TOP_HAL_ResetSW();
				swRstCnt = 0;
			}
			if(count == FW_DOWNLOAD_TIMEOUT)
			{
				BE_ERROR("<error> BE firmware is not working\n");
				ret = RET_ERROR;
				BREAK_WRONG(ret);
			}
		}
	} while(0);

	return ret;
}

static int FWL_FW_DownloadFromFile(int index)
{
	int ret = RET_OK;
	int    fw_size;
	char*    fw_data;
	OS_FILE_T fw_file;
	BE_FWL_HAL_FW_T stParams;
	BE_FWL_HAL_FW_FILE_T *pFw;
	UINT32 fwBaseAddr = (UINT32)NULL;

	do {
		pFw = BE_FWL_HAL_FwBinInfo(index);
			
		CHECK_NULL(pFw);
		CHECK_NULL(pFw->fwName);
		if( RET_OK != OS_OpenFile(&fw_file, pFw->fwName, O_RDONLY | O_LARGEFILE, 0666) )
		{
			BE_ERROR("<error> file open (%s)\n", pFw->fwName);
			ret = RET_ERROR;
			break;
		}
		fw_size = OS_SizeOfFile(&fw_file);
		if(fw_size < 0)
		{
			BE_ERROR("<error> file size %d\n", fw_size);
			ret = RET_ERROR;
			break;
		}
		fw_data = OS_KMalloc(fw_size);
		OS_ReadFile(&fw_file, fw_data, fw_size);

		stParams.index = pFw->index;
		stParams.size    = fw_size;
		stParams.pBuffer = fw_data;
		stParams.isMcuFw = pFw->isMcuFw;
		fwBaseAddr = BE_FWL_HAL_FwDownload(&stParams);

		OS_CloseFile(&fw_file);
		OS_KFree(fw_data);
		CHECK_NULL(fwBaseAddr);

		gstFwlInfo.ui32FwInitReqMask |= gui32FwIndexMask[index];
		gstFwlInfo.ui32FwBaseAddr[index] = fwBaseAddr;
	} while(0);

	return ret;
}

static int FWL_FW_DownloadFromHead(int index)
{
	int ret = RET_OK;
	BE_FWL_HAL_FW_T *pFw;
	UINT32 fwBaseAddr = (UINT32)NULL;

	do {
		pFw = BE_FWL_HAL_GetFwHeaderInfo(index);
		CHECK_NULL(pFw);

		if (!(pFw->pBuffer)) break;		// pBuffer == NULL -> ignore
		
		fwBaseAddr = BE_FWL_HAL_FwDownload(pFw);
		CHECK_NULL(fwBaseAddr);

		gstFwlInfo.ui32FwInitReqMask |= gui32FwIndexMask[index];
		gstFwlInfo.ui32FwBaseAddr[index] = fwBaseAddr;

		gstFwlInfo.ui32FwDnloadedFlag |= gui32FwIndexMask[index];
	} while(0);

	return ret;
}

static int FWL_FW_InitializeFirmware(void)
{
	int ret = RET_OK;
	UINT32 idx;

	do {
		if(!gstFwlInfo.bMcuIsAlive) break;

		for(idx = 0 /* BE_FW_DWLD_TYPE_FRC */; idx < BE_FW_DWLD_TYPE_MAXNUM; idx++)
		{
			switch(gstFwlInfo.ui32FwInitReqMask & gui32FwIndexMask[idx])
			{
				case FWL_FW_INDEX_MASK_FRC :
				{
					ret = BE_FWL_HAL_FrcFwInit();
					if(ret) BREAK_WRONG(ret);
					
					gstFwlInfo.ui32FwInitReqMask &= (~gui32FwIndexMask[idx]);
				}
					break;
				case FWL_FW_INDEX_MASK_SRE :
				{
					ret = BE_FWL_HAL_SreFwInit(gstFwlInfo.ui32FwBaseAddr[idx]);
					if(ret) BREAK_WRONG(ret);

					gstFwlInfo.ui32FwInitReqMask &= (~gui32FwIndexMask[idx]);
				}
					break;
				case FWL_FW_INDEX_MASK_WCG :
				{
					ret = BE_FWL_HAL_WcgFwInit(gstFwlInfo.ui32FwBaseAddr[idx]);
					if(ret) BREAK_WRONG(ret);

					gstFwlInfo.ui32FwInitReqMask &= (~gui32FwIndexMask[idx]);
				}
					break;
				case FWL_FW_INDEX_MASK_LED :
				{
					ret = BE_FWL_HAL_LedFwInit(gstFwlInfo.ui32FwBaseAddr[idx]);
					if(ret) BREAK_WRONG(ret);

					gstFwlInfo.ui32FwInitReqMask &= (~gui32FwIndexMask[idx]);
				}
					break;
				case FWL_FW_INDEX_MASK_HDR : 
				{
					ret = BE_FWL_HAL_HdrFwInit(gstFwlInfo.ui32FwBaseAddr[idx]);
					if(ret) BREAK_WRONG(ret);

					gstFwlInfo.ui32FwInitReqMask &= (~gui32FwIndexMask[idx]);
				}
					break;
				default :
					/* Do nothing */
					break;
			}
			if(ret) break;
		}
	} while(0);

	return ret;
}
