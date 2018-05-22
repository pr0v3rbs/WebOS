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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
/// file    apiXC_PCMonitor_ex.c
/// @brief  specific Scaler API layer Interface for current project
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _API_XC_PC_MONITOR_EX_C_
#define _API_XC_PC_MONITOR_EX_C_

// Common
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
// Driver
#include "drvXC_IOPort.h"
#include "mhal_xc_chip_config.h"
#include "xc_hwreg_utility2.h"
#include "apiXC.h"
#include "drvXC_HDMI_Internal.h"
#include "drvXC_HDMI_if.h"
#include "mhal_hdmi.h"
#include "mhal_ip.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "mhal_xc_chip_config.h"
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
#include "apiXC_v2.h"
#include "drv_sc_scaling.h"
#include "apiXC_PCMonitor_v2.h"
#include "apiXC_EX.h"


#ifdef __cplusplus
extern "C"
{
#endif
////////////////////////////////////////////////////////////
//             LOCAL VARIABLE
///////////////////////////////////////////////////////////
MS_BOOL FastMute = FALSE;
MS_U32 u32tempTime = 0;
static MS_BOOL bIsYcbCr = FALSE;
MS_BOOL b_isKR = false;
#define DbgPrint_apiXC_PCMonitor(x) x

extern MS_BOOL MDrv_XC_PCMonitor_InvalidTimingDetect_YPbPr(MS_BOOL *bSyncChanged, MS_BOOL bPollingOnly, SCALER_WIN eWindow);


MS_U16 MApi_XC_PCMonitor_InvalidTimingDetect_YPbPr( MS_BOOL bPollingOnly, SCALER_WIN eWindow)
{

return 0;
#if 0
    MS_BOOL bReturn = FALSE;
    MS_BOOL bSyncChanged = FALSE;

    _XC_SEMAPHORE_ENTRY(E_XC_POOL_ID_INTERNAL_VARIABLE);
    _XC_ENTRY();
    bReturn = MDrv_XC_PCMonitor_InvalidTimingDetect_YPbPr(&bSyncChanged, bPollingOnly, eWindow);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(E_XC_POOL_ID_INTERNAL_VARIABLE);
    return (((MS_U16)bSyncChanged << 8) | bReturn); // stable sync
 #endif
}

MS_BOOL IsFreezeByKernel(void)
{
    return (MApi_XC_R2BYTEMSK(DUMMY_REFRIGLATOR,KERNEL_FREEZE_BIT))?TRUE:FALSE;
}

void MApi_XC_UnFreezeByTimer()
{
 //no need

#if 0
    if(IsFreezeByKernel())
    {
        FastMute = false;
        ClearKernelFreezeID
        ClearForceRW

        //by Reginal's advise
        MApi_XC_W2BYTEMSK(REG_SC_BK12_04_L, 0, BIT(15));

        //MApi_SC_FastSetFreeze(0,MAIN_WINDOW);
        //Hal_SC_set_freezeimg(0, MAIN_WINDOW);
        MApi_XC_FreezeImg(FALSE, MAIN_WINDOW);

        DbgPrint_apiXC_PCMonitor(printf("\n\n [user ]unfreeze\n"));

    }
#endif
}

void MApi_SetSourceYCbCr(MS_BOOL b)
{
    if(b)
        MApi_XC_W2BYTEMSK(DUMMY_REFRIGLATOR, KERNEL_YCBCR_BIT, KERNEL_YCBCR_BIT);
    else
        MApi_XC_W2BYTEMSK(DUMMY_REFRIGLATOR, 0, KERNEL_YCBCR_BIT);

    bIsYcbCr = b;
}

#ifdef __cplusplus
}
#endif

#undef _API_XC_PC_MONITOR_EX_C_
#endif

