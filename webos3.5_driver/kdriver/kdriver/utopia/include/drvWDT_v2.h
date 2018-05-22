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

#ifndef _DRV_WDT_V2_H_
#define _DRV_WDT_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure


typedef enum {
    MDrv_CMD_WDT_INIT,
    MDrv_CMD_WDT_STOP,
    MDrv_CMD_WDT_CLEAR,
    MDrv_CMD_WDT_CLEAR_RST_FLAG,
    MDrv_CMD_WDT_IS_RESET,
    MDrv_CMD_WDT_IS_ENABLE,
    MDrv_CMD_WDT_SETTIMER,
    MDrv_CMD_WDT_SETTIMER_MS,
    MDrv_CMD_WDT_SETTIMER_US,
    MDrv_CMD_WDT_SETINTTIMER,
    
    MDrv_CMD_TIMER_COUNT,
    MDrv_CMD_TIMER_INT,
    MDrv_CMD_TIMER_RST,
    MDrv_CMD_TIMER_SETMAXMATCH,
    MDrv_CMD_TIMER_HITMAXMATCH,
    MDrv_CMD_TIMER_GETMAXMATCH,
    MDrv_CMD_TIMER_GETCOUNTER,
    MDrv_CMD_TIMER_GETSECOND,
    MDrv_CMD_TIMER_GETMS,
    MDrv_CMD_TIMER_DELAY,
    MDrv_CMD_TIMER_DELAYMS,
    
} eWdtIoctlOpt;
   
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
 
//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
 
typedef struct _WDT_PRIVATE_PARAM_Init
{
    WDT_DbgLv eLevel;
} WDT_PRIVATE_PARAM_Init, *PWDT_PRIVATE_PARAM_Init ;

typedef struct _WDT_PRIVATE_PARAM_Stop
{
    WDT_DbgLv eLevel;
}WDT_PRIVATE_PARAM_Stop, *PWDT_PRIVATE_PARAM_Stop;

typedef struct _WDT_PRIVATE_PARAM_SetTimer
{
    WDT_DbgLv eLevel;
    MS_U16 sec;
}WDT_PRIVATE_PARAM_SetTimer, *PWDT_PRIVATE_PARAM_SetTimer;

typedef struct _WDT_PRIVATE_PARAM_SetTimer_ms
{
    WDT_DbgLv eLevel;
    MS_U16 msec;
}WDT_PRIVATE_PARAM_SetTimer_ms, *PWDT_PRIVATE_PARAM_SetTimer_ms;

typedef struct _WDT_PRIVATE_PARAM_SetTimer_us
{
    WDT_DbgLv eLevel;
    MS_U16 usec;
}WDT_PRIVATE_PARAM_SetTimer_us, *PWDT_PRIVATE_PARAM_SetTimer_us;

typedef struct _WDT_PRIVATE_PARAM_SetIntTimer
{
    WDT_DbgLv eLevel;
    MS_U16 sec;
}WDT_PRIVATE_PARAM_SetIntTimer, *PWDT_PRIVATE_PARAM_SetIntTimer;

typedef struct _TIMER_PRIVATE_PARAM_Count
{
    E_PIU_Timer eTimer;
    MS_BOOL bEnable;
}TIMER_PRIVATE_PARAM_Count, *PTIMER_PRIVATE_PARAM_Count;

typedef struct _TIMER_PRIVATE_PARAM_INT
{
    E_PIU_Timer eTimer;
    MS_BOOL bEnable;
}TIMER_PRIVATE_PARAM_INT, *PTIMER_PRIVATE_PARAM_INT;

typedef struct _TIMER_PRIVATE_PARAM_Rst
{
    E_PIU_Timer eTimer;
}TIMER_PRIVATE_PARAM_Rst, *PTIMER_PRIVATE_PARAM_Rst;

typedef struct _TIMER_PRIVATE_PARAM_SetMaxMatch
{
    E_PIU_Timer eTimer;
    MS_U32 u32MaxTimer;
}TIMER_PRIVATE_PARAM_SetMaxMatch, *PTIMER_PRIVATE_PARAM_SetMaxMatch;

typedef struct _TIMER_PRIVATE_PARAM_HitMaxMatch
{
    E_PIU_Timer eTimer;
}TIMER_PRIVATE_PARAM_HitMaxMatch, *PTIMER_PRIVATE_PARAM_HitMaxMatch;

typedef struct _TIMER_PRIVATE_PARAM_GetMaxMatch
{
    E_PIU_Timer eTimer;
}TIMER_PRIVATE_PARAM_GetMaxMatch, *PTIMER_PRIVATE_PARAM_GetMaxMatch;

typedef struct _TIMER_PRIVATE_PARAM_GetCounter
{
    E_PIU_Timer eTimer;
}TIMER_PRIVATE_PARAM_GetCounter, *PTIMER_PRIVATE_PARAM_GetCounter;

typedef struct _TIMER_PRIVATE_PARAM_GetSecond
{
    E_PIU_Timer eTimer;
}TIMER_PRIVATE_PARAM_GetSecond, *PTIMER_PRIVATE_PARAM_GetSecond;

typedef struct _TIMER_PRIVATE_PARAM_GetMs
{
    E_PIU_Timer eTimer;
}TIMER_PRIVATE_PARAM_GetMs, *PTIMER_PRIVATE_PARAM_GetMs;

typedef struct _TIMER_PRIVATE_PARAM_Delay
{
    E_PIU_Timer eTimer;
    MS_U32 u32Second;
}TIMER_PRIVATE_PARAM_Delay, *PTIMER_PRIVATE_PARAM_Delay;

typedef struct _TIMER_PRIVATE_PARAM_DelayMs
{
    E_PIU_Timer eTimer;
    MS_U32 u32MS;
}TIMER_PRIVATE_PARAM_DelayMs, *PTIMER_PRIVATE_PARAM_DelayMs;

typedef union _WDT_PRIVATE_PARAM
{
    WDT_PRIVATE_PARAM_Init            privateInit;
    WDT_PRIVATE_PARAM_Stop            privateStop;
    //WDT_PRIVATE_PARAM_Clear           privateClear;
    //WDT_PRIVATE_PARAM_ClearRstFlag    privateClearRstFlag;
    //WDT_PRIVATE_PARAM_IsReset         privateIsReset;
    
    //WDT_PRIVATE_PARAM_IsEnable        privateIsEnable;
    WDT_PRIVATE_PARAM_SetTimer        privateSetTimer;
    WDT_PRIVATE_PARAM_SetTimer_ms     privateSetTimer_ms;
    WDT_PRIVATE_PARAM_SetTimer_us     privateSetTimer_us;
    WDT_PRIVATE_PARAM_SetIntTimer     privateSetIntTimer;
    
    TIMER_PRIVATE_PARAM_Count         privateCount;
    TIMER_PRIVATE_PARAM_INT           privateINT;
    TIMER_PRIVATE_PARAM_Rst           privateRst;
    TIMER_PRIVATE_PARAM_SetMaxMatch   privateSetMaxMatch;
    TIMER_PRIVATE_PARAM_HitMaxMatch   privateHitMaxMatch;
    
    TIMER_PRIVATE_PARAM_GetMaxMatch   privateGetMaxMatch;
    TIMER_PRIVATE_PARAM_GetCounter    privateGetCounter;
    TIMER_PRIVATE_PARAM_GetSecond     privateGetSecond;
    TIMER_PRIVATE_PARAM_GetMs         privateGetMs;
    TIMER_PRIVATE_PARAM_Delay         privateDelay;
     
    TIMER_PRIVATE_PARAM_DelayMs       privateDelayMs;
    
}WDT_PRIVATE_PARAM;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------
 
//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_WDT_V2_H_

