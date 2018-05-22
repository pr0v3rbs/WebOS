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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVWDT_PRIV_H_
#define _DRVWDT_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvWDT.h"
 
typedef WDT_Result (*IOCTL_WDT_INIT)(WDT_DbgLv);
typedef WDT_Result (*IOCTL_WDT_STOP)(WDT_DbgLv);
typedef WDT_Result (*IOCTL_WDT_CLEAR)(void);
typedef WDT_Result (*IOCTL_WDT_CLEAR_RST_FLAG)(void);
typedef WDT_Result (*IOCTL_WDT_IS_RESET)(void);
typedef WDT_Result (*IOCTL_WDT_IS_ENABLE)(void);
typedef WDT_Result (*IOCTL_WDT_SETTIMER)(WDT_DbgLv, MS_U16);
typedef WDT_Result (*IOCTL_WDT_SETTIMER_MS)(WDT_DbgLv, MS_U16);
typedef WDT_Result (*IOCTL_WDT_SETTIMER_US)(WDT_DbgLv, MS_U16);
typedef WDT_Result (*IOCTL_WDT_SETINTTIMER)(WDT_DbgLv, MS_U16);
typedef void (*IOCTL_TIMER_COUNT)(E_PIU_Timer, MS_BOOL);
typedef void (*IOCTL_TIMER_INT)(E_PIU_Timer, MS_BOOL);
typedef void (*IOCTL_TIMER_RST)(E_PIU_Timer);
typedef void (*IOCTL_TIMER_SETMAXMATCH)(E_PIU_Timer, MS_U32);
typedef MS_BOOL (*IOCTL_TIMER_HITMAXMATCH)(E_PIU_Timer);
typedef MS_U32 (*IOCTL_TIMER_GETMAXMATCH)(E_PIU_Timer);
typedef MS_U32 (*IOCTL_TIMER_GETCOUNTER)(E_PIU_Timer);
typedef MS_U32 (*IOCTL_TIMER_GETSECOND)(E_PIU_Timer);
typedef MS_U32 (*IOCTL_TIMER_GETMS)(E_PIU_Timer);
typedef void (*IOCTL_TIMER_DELAY)(E_PIU_Timer, MS_U32);
typedef void (*IOCTL_TIMER_DELAYMS)(E_PIU_Timer, MS_U32);

//WDT_Private
typedef struct _WDT_RESOURCE_PRIVATE
{
    MS_U32 WDT_Dummy;
}WDT_RESOURCE_PRIVATE;

typedef struct _WDT_RESOURCE_SHARED
{
    void* pWDTResource;
}WDT_RESOURCE_SHARED;

typedef struct _WDT_INSTANT_PRIVATE
{
    IOCTL_WDT_INIT             fpWDTInit;
    IOCTL_WDT_STOP             fpWDTStop;
    IOCTL_WDT_CLEAR            fpWDTClear;
    IOCTL_WDT_CLEAR_RST_FLAG   fpWDTClearRstFlag;
    IOCTL_WDT_IS_RESET         fpWDTIsReset;
    IOCTL_WDT_IS_ENABLE        fpWDTIsEnable;
    IOCTL_WDT_SETTIMER         fpWDTSetTimer;
    IOCTL_WDT_SETTIMER_MS      fpWDTSetTimer_ms;
    IOCTL_WDT_SETTIMER_US      fpWDTSetTimer_us;
    IOCTL_WDT_SETINTTIMER      fpWDTSetIntTimer;
     
    IOCTL_TIMER_COUNT          fpTIMERCount;
    IOCTL_TIMER_INT            fpTIMERInt;
    IOCTL_TIMER_RST            fpTIMERRst;
    IOCTL_TIMER_SETMAXMATCH    fpTIMERSetMaxMatch;
    IOCTL_TIMER_HITMAXMATCH    fpTIMERHitMaxMatch;
    IOCTL_TIMER_GETMAXMATCH    fpTIMERGetMaxMatch;
    IOCTL_TIMER_GETCOUNTER     fpTIMERGetCounter;
    IOCTL_TIMER_GETSECOND      fpTIMERGetSecond;
    IOCTL_TIMER_GETMS          fpTIMERGetMs;
    IOCTL_TIMER_DELAY          fpTIMERDelay;
    IOCTL_TIMER_DELAYMS        fpTIMERDelayMs;
    
}WDT_INSTANT_PRIVATE;

void WDTRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 WDTOpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 WDTClose(void* pInstance);
MS_U32 WDTIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);
  
#ifdef __cplusplus
}
#endif
#endif // _DRVWDT_PRIV_H_
