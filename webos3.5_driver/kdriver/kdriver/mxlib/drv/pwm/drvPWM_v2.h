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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvPWM_v2.h
/// @brief  MIU Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_PWM_V2_H_
#define _DRV_PWM_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MDrv_CMD_PWM_Oen,
    MDrv_CMD_PWM_Period,
    MDrv_CMD_PWM_DutyCycle,
    MDrv_CMD_PWM_UnitDiv,
    MDrv_CMD_PWM_Div,
    MDrv_CMD_PWM_Polarity,
    MDrv_CMD_PWM_Vdben,
    MDrv_CMD_PWM_ResetEn,
    MDrv_CMD_PWM_Dben,
    MDrv_CMD_PWM_Shift,
    MDrv_CMD_PWM_Nvsync,
} ePwmIoctlOpt;

//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------


typedef struct _PWM_PARAM_Oen
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bOenPWM;
}PWM_PARAM_Oen, *PPWM_PARAM_Oen; 

typedef struct _PWM_PARAM_Period
{
    PWM_ChNum u8IndexPWM;
    MS_U32 u32PeriodPWM;
}PWM_PARAM_Period, *PPWM_PARAM_Period; 

typedef struct _PWM_PARAM_DutyCycle
{
    PWM_ChNum u8IndexPWM;
    MS_U32 u32DutyPWM;
}PWM_PARAM_DutyCycle, *PPWM_PARAM_DutyCycle; 

typedef struct _PWM_PARAM_UnitDiv
{
    MS_U16 u16UnitDivPWM;
}PWM_PARAM_UnitDiv, *PPWM_PARAM_UnitDiv; 

typedef struct _PWM_PARAM_Div
{
    PWM_ChNum u8IndexPWM;
    MS_U16 u16DivPWM;
}PWM_PARAM_Div, *PPWM_PARAM_Div; 

typedef struct _PWM_PARAM_Polarity
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bPolPWM;
}PWM_PARAM_Polarity, *PPWM_PARAM_Polarity; 

typedef struct _PWM_PARAM_Vdben
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bVdbenPWM;
}PWM_PARAM_Vdben, *PPWM_PARAM_Vdben; 

typedef struct _PWM_PARAM_ResetEn
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bRstPWM;
}PWM_PARAM_ResetEn, *PPWM_PARAM_ResetEn; 

typedef struct _PWM_PARAM_Dben
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bDbenPWM;
}PWM_PARAM_Dben, *PPWM_PARAM_Dben; 

typedef struct _PWM_PARAM_Shift
{
    PWM_ChNum u8IndexPWM;
    MS_U32 u32DutyPWM;
}PWM_PARAM_Shift, *PPWM_PARAM_Shift; 

typedef struct _PWM_PARAM_Nvsync
{
    PWM_ChNum u8IndexPWM;
    MS_BOOL bNvsPWM;
}PWM_PARAM_Nvsync, *PPWM_PARAM_Nvsync; 

typedef union _PWM_PRIVATE_PARAM
{
    PWM_PARAM_Oen privateOen;
    PWM_PARAM_Period privatePeriod;
    PWM_PARAM_DutyCycle privateDutyCycle;
    PWM_PARAM_UnitDiv privateUnitDiv;
    PWM_PARAM_Div privateDiv;    
    PWM_PARAM_Polarity privatePolarity;
    PWM_PARAM_Vdben privateVdben;
    PWM_PARAM_ResetEn privateResetEn;
    PWM_PARAM_Dben privateDben;
    PWM_PARAM_Shift privateShift;
    PWM_PARAM_Nvsync privateNvsync;
  
}PWM_PRIVATE_PARAM;


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

#endif // _DRV_PWM_V2_H_

