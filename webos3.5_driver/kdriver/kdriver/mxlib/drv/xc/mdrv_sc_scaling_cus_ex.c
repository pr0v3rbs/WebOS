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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#define DRV_SC_SCALING_EX_C

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_EX.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"

#include "drv_sc_ip.h"

#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "apiXC_cus_ex.h"
#include "XC_private.h"

#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"
#include "apiXC_v2.h"



static MS_BOOL _bNRDemoMode_Enable_MLoad = FALSE;

void MApi_SC_SetNRDemoEnable_ForMLoad(MS_BOOL bEnable)
{
    _bNRDemoMode_Enable_MLoad = (bEnable) ? TRUE : FALSE;
    return;
}

MS_BOOL MApi_SC_getNRDemoEnable_ForMLoad(void)
{
    return _bNRDemoMode_Enable_MLoad;
}

//for patch QC issue that Video dely and lip sync. in comp/CVBS i mode. exclude 1080i
void MApi_XC_SetGameMode_U2(void *pInstance,SCALER_WIN eWindow, MS_BOOL bNeedSetWindow )
{

//            SC_SCALING_DBG_MSG("\033[0;31m###### bNeedSetWindow(%d)   ######\033[0m\r\n", bNeedSetWindow);
//            SC_SCALING_DBG_MSG("\033[0;31m###### eWindow(%d)   ######\033[0m\r\n", eWindow);
//          SC_SCALING_DBG_MSG("\033[0;31m###### gSrcInfo[eWindow].bInterlace(%d)   ######\033[0m\r\n", gSrcInfo[eWindow].bInterlace);
//            SC_SCALING_DBG_MSG("\033[0;31m###### gSrcInfo[eWindow].u16InputVTotal(%d)   ######\033[0m\r\n", gSrcInfo[eWindow].u16InputVTotal);
//            SC_SCALING_DBG_MSG("\033[0;31m###### gSrcInfo[eWindow].enInputSourceType(%d)   ######\033[0m\r\n", gSrcInfo[eWindow].enInputSourceType);
//            SC_SCALING_DBG_MSG("\033[0;31m###### gSrcInfo[eWindow].bRWBankAuto(%d)   ######\033[0m\r\n", gSrcInfo[eWindow].bRWBankAuto);
//            SC_SCALING_DBG_MSG("\033[0;31m###### gSrcInfo[eWindow].Status2.eFrameStoreNumber(%d)   ######\033[0m\r\n", gSrcInfo[eWindow].Status2.eFrameStoreNumber);
    //_bNeedSetWindow = bNeedSetWindow ;
#if 1
    //printf("[AVsync]._gbGameModeOn[eWindow]=%u.\n",_gbGameModeOn[eWindow]);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    if( bNeedSetWindow
            && gSrcInfo[eWindow].bInterlace
            && (gSrcInfo[eWindow].u16InputVTotal <= 750 )
            && (  ( INPUT_SOURCE_CVBS   <= gSrcInfo[eWindow].enInputSourceType && gSrcInfo[eWindow].enInputSourceType < INPUT_SOURCE_CVBS_MAX )
                  || (INPUT_SOURCE_SVIDEO <= gSrcInfo[eWindow].enInputSourceType && gSrcInfo[eWindow].enInputSourceType < INPUT_SOURCE_SVIDEO_MAX )
                  || (INPUT_SOURCE_YPBPR  <= gSrcInfo[eWindow].enInputSourceType && gSrcInfo[eWindow].enInputSourceType < INPUT_SOURCE_YPBPR_MAX ) ) )
    {
//            SC_SCALING_DBG_MSG("\033[0;31m###### MApi_XC_SetGameMode: [%s], Hal_SC_set_wr_bank_mapping(0x01, %d);  ######\033[0m\r\n", eWindow == PQ_MAIN_WINDOW? "MAIN":"SUB", eWindow);
        Hal_SC_set_wr_bank_mapping(pInstance,0x01, eWindow);
    }
    else
    {
        // avoid tearing problem when we can not do frame lock
        // FIXME: Duplicate code with those in " MDrv_SC_set_fetch_number_limit ".
        if(gSrcInfo[eWindow].bRWBankAuto)
        {
            // Set read/write bank as larger than 0x01 if frame count >= 3
            if ( gSrcInfo[eWindow].Status2.eFrameStoreNumber >= IMAGE_STORE_3_FRAMES &&
                    gSrcInfo[eWindow].Status2.eFrameStoreNumber <= IMAGE_STORE_10_FRAMES)
            {
//                SC_SCALING_DBG_MSG("\033[0;31m###### Hal_SC_set_wr_bank_mapping(0x01, %d);  ######\033[0m\r\n", eWindow);
                Hal_SC_set_wr_bank_mapping(pInstance,0x01, eWindow);
            }
            else // Interlace
            {
                // framelock at 48hz and 50hz but sub is 60hz.
                // For sub window, if output frame rate < input framerate, set wr bank mapping to 1.
                if ( eWindow == SUB_WINDOW && ( MDrv_SC_GetOutputVFreqX100(pInstance) / 10 ) < 510 &&
                        gSrcInfo[eWindow].u16InputVFreq > 580 && gSrcInfo[eWindow].bInterlace /* && g_XC_InitData.bMirror*/)
                {
                    Hal_SC_set_wr_bank_mapping(pInstance,0x01, eWindow);
                }
                else
                {
//                    SC_SCALING_DBG_MSG("\033[0;31m###### Hal_SC_set_wr_bank_mapping(0x00, %d);  ######\033[0m\r\n", eWindow);
                    Hal_SC_set_wr_bank_mapping(pInstance,0x00, eWindow);
                }
            }
        }

    }
    #endif
}

void MApi_XC_SetGameMode(SCALER_WIN eWindow, MS_BOOL bNeedSetWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_GAMEMODE XCArgs;

    XCArgs.eWindow = eWindow;
    XCArgs.bNeedSetWindow = bNeedSetWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_GAMEMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_SetFrameCount(SCALER_WIN eWindow,MS_U16 fcount)
{
	if (eWindow == MAIN_WINDOW)
		MApi_XC_W2BYTEMSK(REG_SC_BK12_19_L, (MS_U8)fcount , 0x001F );
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK12_59_L, (MS_U8)fcount , 0x001F );
}
#if 1
void MApi_XC2_SetFrameCount(SCALER_WIN eWindow,MS_U16 fcount)
{
	if (eWindow == MAIN_WINDOW)
		MApi_XC2_W2BYTEMSK(REG_SC_BK12_19_L, (MS_U8)fcount , 0x001F );
	else
		MApi_XC2_W2BYTEMSK(REG_SC_BK12_59_L, (MS_U8)fcount , 0x001F );
}
#endif





