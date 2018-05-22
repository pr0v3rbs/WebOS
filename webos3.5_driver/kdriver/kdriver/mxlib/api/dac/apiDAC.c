//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiDAC.c
/// @author MStar Semiconductor,Inc.
/// @brief  DAC Api
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "drvDAC.h"
#include "drvMMIO.h"
#include "apiDAC.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"
#include "apiDAC_private.h"
#include "apiDAC_v2.h"

static MSIF_Version _api_dac_version = {
    .DDI = { DAC_API_VERSION },
};

// for Utopia 2.0 to Utopia 1.0 compatibility.
void* g_pDACInst;
void* g_pDACRes;

MS_U32 MDrv_DAC_Get_Semaphore(void)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }

    void* pModule = NULL;
    UtopiaInstanceGetModule(g_pDACInst, &pModule);
    if(UtopiaResourceObtain(pModule, E_DAC_POOL_ID_INTERNAL_VARIABLE, &g_pDACRes) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    u32Return = UTOPIA_STATUS_SUCCESS;
    return u32Return;
}

MS_U32 MDrv_DAC_Release_Semaphore(void)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    u32Return = UtopiaResourceRelease(g_pDACRes);
    return u32Return;
}

APIDAC_Result MApi_DAC_GetLibVer_U2(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_APIDAC_FAIL;
    }

    *ppVersion = &_api_dac_version;
    return E_APIDAC_OK;
}

APIDAC_Result MApi_DAC_GetLibVer(const MSIF_Version **ppVersion)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIDAC_FAIL;
    }

    stDAC_GET_LIBVER DACArgs;
    DACArgs.ppVersion = ppVersion;
    DACArgs.eReturnValue = E_APIDAC_FAIL;

    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_LIBVER, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.eReturnValue;
}

const DAC_ApiInfo * MApi_DAC_GetInfo_U2(void)          ///< Get info from driver
{
    MDrv_DAC_Get_Semaphore();
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    MDrv_DAC_Release_Semaphore();
    return &pDACResourcePrivate->stapiDAC._cstDac_ApiInfo;
}

const DAC_ApiInfo * MApi_DAC_GetInfo(void)          ///< Get info from driver
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return NULL;
    }

    stDAC_GET_INFO DACArgs;
    DACArgs.stReturnValue = NULL;

    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_INFO, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.stReturnValue;
}

MS_BOOL MApi_DAC_GetStatus_U2(DAC_ApiStatus *pDacStatus)
{
    MDrv_DAC_Get_Semaphore();
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    pDacStatus->bDac_Initialized    = pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_Initialized;
    pDacStatus->bDac_VE_enabled     = pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_VE_enabled;
    pDacStatus->bDac_YPbPr_enabled  = pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_YPbPr_enabled;
    MDrv_DAC_Release_Semaphore();
    return TRUE;
}

MS_BOOL MApi_DAC_GetStatus(DAC_ApiStatus *pDacStatus)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_GET_STATUS DACArgs;
    DACArgs.pDacStatus = pDacStatus;
    DACArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_STATUS, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}

MS_BOOL MApi_DAC_SetDbgLevel_U2(MS_U16 u16DbgSwitch)
{
    MDrv_DAC_Get_Semaphore();
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    pDACResourcePrivate->stapiDAC._u16DbgSwitch = u16DbgSwitch;
    MDrv_DAC_Release_Semaphore();
    return TRUE;
}

MS_BOOL MApi_DAC_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_SET_DBG_LEVEL DACArgs;
    DACArgs.u16DbgSwitch = u16DbgSwitch;
    DACArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_DBG_LEVEL, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}


MS_BOOL MApi_DAC_Init_U2(void)
{
    MS_U32 u32PMBaseAddr=0, u32PMBaseSize=0;
    if(!MDrv_MMIO_GetBASE( &u32PMBaseAddr, &u32PMBaseSize, MS_MODULE_PM ))
    {
        printf("[MAPI_DAC]MApi_DAC_Init Get PM BASE failure!\n");
        return FALSE;
    }
    MDrv_DAC_Get_Semaphore();
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    MDrv_DAC_SetIOMapBase( u32PMBaseAddr, u32PMBaseAddr);
    MDrv_DAC_Init();
    pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_Initialized = TRUE;
    MDrv_DAC_Release_Semaphore();
    return TRUE;
}

MS_BOOL MApi_DAC_Init(void)
{
    if (g_pDACInst == NULL)
    {
        if(UtopiaOpen(MODULE_DAC, &g_pDACInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DAC failed\n");
    		return FALSE;
        }
    }

    stDAC_INIT DACArgs;
    DACArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(g_pDACInst, E_DAC_INIT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}


void MApi_DAC_Enable_U2(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    MDrv_DAC_Get_Semaphore();
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    MDrv_DAC_Enable(bEnable, bIsYPbPr);
    /*
     * DAC enable/disable should not do MDrv_DAC_SetOutputSource or
     * MDrv_DAC_SetOutputLevel. In order to make this API downward compatible,
     * MDrv_DAC_SetOutputSource and MDrv_DAC_SetOutputLevel are called
     * only when the first time that MApi_DAC_Enable is called. (HD and SD)
     */
    if (bIsYPbPr && !MDrv_DAC_GetDACState(TRUE)) {
        MDrv_DAC_SetOutputSource(DAC_TO_HDGEN, bIsYPbPr);
        MDrv_DAC_SetOutputLevel(DAC_OUTPUT_LEVEL_LOW, bIsYPbPr);
        MApi_DAC_SetOutputSwapSel(E_DAC_R_B_G, bIsYPbPr);
        MDrv_DAC_SetDACState(TRUE, TRUE);
    }
    if (!bIsYPbPr && !MDrv_DAC_GetDACState(FALSE)) {
        MDrv_DAC_SetOutputSource(DAC_TO_VE, bIsYPbPr);
        MDrv_DAC_SetOutputLevel(DAC_OUTPUT_LEVEL_HIGH, bIsYPbPr);
        MApi_DAC_SetOutputSwapSel(E_DAC_R_G_B, bIsYPbPr);
        MDrv_DAC_SetDACState(TRUE, FALSE);
    }

    if (bIsYPbPr)
    {
        pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_YPbPr_enabled = bEnable;
    }
    else
    {
        pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_VE_enabled = bEnable;
    }
    MDrv_DAC_Release_Semaphore();
}

void MApi_DAC_Enable(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_ENABLE DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_ENABLE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_SetOutputSource_U2(E_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetOutputSource((EN_DAC_OUTPUT_TYPE)enOutputType,bIsYPbPr);
    return;
}

void MApi_DAC_SetOutputSource(E_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_OUTPUT_SOURCE DACArgs;
    DACArgs.enOutputType = enOutputType;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_OUTPUT_SOURCE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_SetOutputLevel_U2(E_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetOutputLevel((EN_DAC_MAX_OUTPUT_LEVEL)enLevel,bIsYPbPr);
}

void MApi_DAC_SetOutputLevel(E_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OUTPUT_LEVEL DACArgs;
    DACArgs.enLevel = enLevel;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_OUTPUT_LEVEL, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_SetOutputSwapSel_U2(E_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetOutputSwapSel((EN_DAC_SWAP_SEL)enSwap, bIsYPbPr);
}

void MApi_DAC_SetOutputSwapSel(E_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_OUTPUT_SWAPSEL DACArgs;
    DACArgs.enSwap = enSwap;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_OUTPUT_SWAPSEL, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_SetClkInv_U2(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetClkInv(bEnable, bIsYPbPr);
}

void MApi_DAC_SetClkInv(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_CLKINV DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_CLKINV, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_OnOffSD_U2(E_DAC_SD_ENABLE_CTRL enBit)
{
    MDrv_DAC_OnOffSD((EN_DAC_SD_ENABLE_CTRL)enBit);
}

void MApi_DAC_OnOffSD(E_DAC_SD_ENABLE_CTRL enBit)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_ONOFF_SD DACArgs;
    DACArgs.enBit = enBit;

    if(UtopiaIoctl(g_pDACInst, E_DAC_ONOFF_SD, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_OnOffHD_U2(E_DAC_HD_ENABLE_CTRL enBit)
{
    MDrv_DAC_OnOffHD((EN_DAC_HD_ENABLE_CTRL)enBit);
}

void MApi_DAC_OnOffHD(E_DAC_HD_ENABLE_CTRL enBit)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_ONOFF_HD DACArgs;
    DACArgs.enBit = enBit;

    if(UtopiaIoctl(g_pDACInst, E_DAC_ONOFF_HD, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

//set output timing for YPbPr (hdgen)
void MApi_DAC_SetYPbPrOutputTiming_U2(E_OUTPUT_VIDEO_TIMING_TYPE eTiming)
{
    EN_OUTPUT_VIDEO_TIMING_TYPE enTimingType;

    if (eTiming == E_RES_720x480I_60Hz)
    {
        enTimingType = RES_720x480I_60Hz;
    }
    else if (eTiming == E_RES_720x480P_60Hz)
    {
        enTimingType = RES_720x480P_60Hz;
    }
    else if (eTiming == E_RES_720x576I_50Hz)
    {
        enTimingType = RES_720x576I_50Hz;
    }
    else if (eTiming == E_RES_720x576P_50Hz)
    {
        enTimingType = RES_720x576P_50Hz;
    }
    else if (eTiming == E_RES_1280x720P_50Hz)
    {
        enTimingType = RES_1280x720P_50Hz;
    }
    else if (eTiming == E_RES_1280x720P_60Hz)
    {
        enTimingType = RES_1280x720P_60Hz;
    }
    else if (eTiming == E_RES_1920x1080I_50Hz)
    {
        enTimingType = RES_1920x1080I_50Hz;
    }
    else if (eTiming == E_RES_1920x1080I_60Hz)
    {
        enTimingType = RES_1920x1080I_60Hz;
    }
    else if (eTiming == E_RES_1920x1080P_24Hz)
    {
        enTimingType = RES_1920x1080P_24Hz;
    }
    else if (eTiming == E_RES_1920x1080P_25Hz)
    {
        enTimingType = RES_1920x1080P_25Hz;
    }
    else if (eTiming == E_RES_1920x1080P_30Hz)
    {
        enTimingType = RES_1920x1080P_30Hz;
    }
    else if (eTiming == E_RES_1920x1080P_50Hz)
    {
        enTimingType = RES_1920x1080P_50Hz;
    }
    else if (eTiming == E_RES_1920x1080P_60Hz)
    {
        enTimingType = RES_1920x1080P_60Hz;
    }
    else if (eTiming == E_RES_VGA640x480P_60Hz)
    {
        enTimingType = RES_VGA640x480P_60Hz;
    }
    else if (eTiming == E_RES_TTL480X272P_60Hz)
    {
        enTimingType = RES_TTL480X272P_60Hz;
    }
    else
    {
        printf("[MAPI_DAC]Error timing type %d!\n", eTiming);
        return;
    }

    MDrv_DAC_SetOutputTiming(enTimingType);
}

void MApi_DAC_SetYPbPrOutputTiming(E_OUTPUT_VIDEO_TIMING_TYPE eTiming)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_YPBPR_OUTPUTTIMING DACArgs;
    DACArgs.eTiming = eTiming;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_YPBPR_OUTPUTTIMING, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}


void MApi_DAC_ClkSel_U2(E_OUTPUT_VIDEO_TIMING_TYPE eTiming, E_OUTPUT_BIT_TYPE ebits)
{
    EN_OUTPUT_VIDEO_TIMING_TYPE enTimingType;
    EN_OUTPUT_BIT_TYPE enBits;

    if (eTiming == E_RES_720x480I_60Hz)
    {
        enTimingType = RES_720x480I_60Hz;
    }
    else if (eTiming == E_RES_720x480P_60Hz)
    {
        enTimingType = RES_720x480P_60Hz;
    }
    else if (eTiming == E_RES_720x576I_50Hz)
    {
        enTimingType = RES_720x576I_50Hz;
    }
    else if (eTiming == E_RES_720x576P_50Hz)
    {
        enTimingType = RES_720x576P_50Hz;
    }
    else if (eTiming == E_RES_1280x720P_50Hz)
    {
        enTimingType = RES_1280x720P_50Hz;
    }
    else if (eTiming == E_RES_1280x720P_60Hz)
    {
        enTimingType = RES_1280x720P_60Hz;
    }
    else if (eTiming == E_RES_1920x1080I_50Hz)
    {
        enTimingType = RES_1920x1080I_50Hz;
    }
    else if (eTiming == E_RES_1920x1080I_60Hz)
    {
        enTimingType = RES_1920x1080I_60Hz;
    }
    else if (eTiming == E_RES_1920x1080P_24Hz)
    {
        enTimingType = RES_1920x1080P_24Hz;
    }
    else if (eTiming == E_RES_1920x1080P_25Hz)
    {
        enTimingType = RES_1920x1080P_25Hz;
    }
    else if (eTiming == E_RES_1920x1080P_30Hz)
    {
        enTimingType = RES_1920x1080P_30Hz;
    }
    else if (eTiming == E_RES_1920x1080P_50Hz)
    {
        enTimingType = RES_1920x1080P_50Hz;
    }
    else if (eTiming == E_RES_1920x1080P_60Hz)
    {
        enTimingType = RES_1920x1080P_60Hz;
    }
    else if (eTiming == E_RES_640x480P_60Hz)
    {
        enTimingType = RES_VGA640x480P_60Hz;
    }
    else if (eTiming == E_RES_VGA640x480P_60Hz)
    {
        enTimingType = RES_VGA640x480P_60Hz;
    }
    else if (eTiming == E_RES_TTL480X272P_60Hz)
    {
        enTimingType = RES_TTL480X272P_60Hz;
    }
    else
    {
        printf("[MAPI_CLKSEL]Error timing type %d!\n", eTiming);
        return;
    }
    switch(ebits)
    {
        case E_COLOR_8_BIT:
            enBits = COLOR_8_BIT;
            break;
        case E_COLOR_10_BIT:
            enBits = COLOR_10_BIT;
            break;
        case E_COLOR_12_BIT:
            enBits = COLOR_12_BIT;
            break;
        default:
            printf("[MAPI_CLKSEL]Error bit type %d!\n", ebits);
            return;
    }
    MDrv_ClkSel_Set(enTimingType, enBits);
}

void MApi_DAC_ClkSel(E_OUTPUT_VIDEO_TIMING_TYPE eTiming, E_OUTPUT_BIT_TYPE ebits)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_CLKSEL DACArgs;
    DACArgs.eTiming = eTiming;
    DACArgs.ebits = ebits;

    if(UtopiaIoctl(g_pDACInst, E_DAC_CLKSEL, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;

}

void MApi_DAC_Exit_U2(void)
{
    MDrv_DAC_Exit();
    UtopiaClose(g_pDACInst);
    g_pDACRes = NULL;
    g_pDACInst = NULL;
}

void MApi_DAC_Exit(void)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(g_pDACInst, E_DAC_EXIT, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_DumpTable_U2(MS_U8 *pDACTable, MS_U8 u8DACtype)
{
    MDrv_DAC_DumpTable(pDACTable, u8DACtype);
}

void MApi_DAC_DumpTable(MS_U8 *pDACTable, MS_U8 u8DACtype)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_DUMP_TABLE DACArgs;
    DACArgs.pDACTable = pDACTable;
    DACArgs.u8DACtype = u8DACtype;

    if(UtopiaIoctl(g_pDACInst, E_DAC_DUMP_TABLE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
void MApi_DAC_SetIHalfOutput_U2(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetIHalfOutput(bEnable, bIsYPbPr);
}

void MApi_DAC_SetIHalfOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_IHALF_OUTPUT DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_IHALF_OUTPUT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

//----------------------------------------------------------------
// DAC - set quart output current
// @return none
//----------------------------------------------------------------
void MApi_DAC_SetQuartOutput_U2(MS_BOOL bEnable,MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetQuartOutput(bEnable, bIsYPbPr);
}

void MApi_DAC_SetQuartOutput(MS_BOOL bEnable,MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_QUART_OUTPUT DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_QUART_OUTPUT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

//----------------------------------------------------------------
// DAC - SetDacState
// When DAC is already enabled in the MBoot, we can use this function to
// mark the DAC init state as "enabled".
// @bInited: TRUE: enabled, FALSE: not enabled
// @bIsYPbPr: TRUE: HD, FALSE: SD
// @return none
//----------------------------------------------------------------
void MApi_DAC_SetDacState_U2(MS_BOOL bEnabled, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetDACState(bEnabled, bIsYPbPr);
}

void MApi_DAC_SetDacState(MS_BOOL bEnabled, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_DAC_STATE DACArgs;
    DACArgs.bEnabled = bEnabled;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_DAC_STATE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

//----------------------------------------------------------------
// MApi_DAC_HotPlugDetect - Get DAC HotPlug state
// @param: SelDAC: DAC_DETECT_HD, DAC_DETECT_SD
// @param: DetectType: DAC_DETECT_PLUGIN, DAC_DETECT_PLUGOUT
// @param: bIsSignIn: Report signal is in/out
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL MApi_DAC_HotPlugDetect_U2(E_DAC_DETECT SelDAC,E_DAC_DETECT_TYPE DetectType, MS_BOOL *State)
{
    return MDrv_DAC_HotPlugDetect((EN_DAC_DETECT) SelDAC, (EN_DAC_DETECT_TYPE)DetectType, State);
}

MS_BOOL MApi_DAC_HotPlugDetect(E_DAC_DETECT SelDAC,E_DAC_DETECT_TYPE DetectType, MS_BOOL *State)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_HOTPLUG_DETECT DACArgs;
    DACArgs.SelDAC = SelDAC;
    DACArgs.DetectType = DetectType;
    DACArgs.State = State;

    if(UtopiaIoctl(g_pDACInst, E_DAC_HOTPLUG_DETECT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}


