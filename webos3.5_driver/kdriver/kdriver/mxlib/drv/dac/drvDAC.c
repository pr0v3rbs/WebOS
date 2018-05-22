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
/// file    drvDAC.c
/// @author MStar Semiconductor Inc.
/// @brief  DAC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#define  MDRV_DAC_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "drvDAC.h"
#include "halDAC.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

void MDrv_DAC_SetIOMapBase(MS_U32 u32NPMBase, MS_U32 u32PMBase)
{
    Hal_DAC_SetIOMapBase(u32NPMBase, u32PMBase);
}

void MDrv_DAC_Init(void)
{
    Hal_DAC_Power_Saving(FALSE);
    Hal_HDMITx_InitSeq();
}

void MDrv_DAC_Exit(void)
{
    Hal_DAC_Power_Saving(TRUE);
}

void MDrv_DAC_Enable(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_Enable(bEnable, bIsYPbPr);
}

void MDrv_DAC_SetClkInv(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetClkInv(bEnable, bIsYPbPr);
}

//DAC MUX select: YPbPr -> E_DAC_TO_HDGEN; CVBS/S-Video -> E_DAC_TO_VE
void MDrv_DAC_SetOutputSource(EN_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetOutputSource(enOutputType, bIsYPbPr);
}

//by default, YPbPr -> E_DAC_OUTPUT_LEVEL_LOW; CVBS/S-Video -> E_DAC_OUTPUT_LEVEL_HIGH
void MDrv_DAC_SetOutputLevel(EN_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetOutputLevel(enLevel, bIsYPbPr);
}

void MDrv_DAC_SetOutputSwapSel(EN_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetOutputSwapSel(enSwap,bIsYPbPr);
}

void MDrv_DAC_SetOutputTiming(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming)
{
    Hal_HDGEN_SetTiming(enTiming);
}

void MDrv_ClkSel_Set(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming, EN_OUTPUT_BIT_TYPE enBits)
{
    //Hal_ClkSel_Set(enTiming, enBits);
}

void MDrv_DAC_OnOffSD(EN_DAC_SD_ENABLE_CTRL enBit)
{
    Hal_DAC_OnOffSD(enBit);
}

void MDrv_DAC_OnOffHD(EN_DAC_HD_ENABLE_CTRL enBit)
{
    Hal_DAC_OnOffHD(enBit);
}

void MDrv_DAC_DumpTable(MS_U8 *pDACTable, MS_U8 u8DACtype)
{
    Hal_DAC_DumpTable(pDACTable, u8DACtype);
}


//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
void MDrv_DAC_SetIHalfOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetIHalfOutput(bEnable, bIsYPbPr);
}

//----------------------------------------------------------------
// DAC - set quart output current
// @return none
//----------------------------------------------------------------
void MDrv_DAC_SetQuartOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetQuartOutput(bEnable, bIsYPbPr);
}

//----------------------------------------------------------------
// MDrv_DAC_SetDACState - Set DAC init state
// @param: bEnable: TRUE for DAC is initialized, FALSE for not initialized
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return none
//----------------------------------------------------------------
void MDrv_DAC_SetDACState(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    Hal_DAC_SetDACState(bEnable, bIsYPbPr);
}

//----------------------------------------------------------------
// MDrv_DAC_GetDACState - Get DAC init state
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return: TRUE is DAC is initialized
//----------------------------------------------------------------
MS_BOOL MDrv_DAC_GetDACState(MS_BOOL bIsYPbPr)
{
    return Hal_DAC_GetDACState(bIsYPbPr);
}

//----------------------------------------------------------------
// MDrv_DAC_HotPlugDetect - Get DAC HotPlug state
// @param: SelDAC: DAC_DETECT_HD, DAC_DETECT_SD
// @param: DetectType: DAC_DETECT_PLUGIN, DAC_DETECT_PLUGOUT
// @param: bIsSignIn: Report signal is in/out
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL MDrv_DAC_HotPlugDetect(EN_DAC_DETECT SelDAC, EN_DAC_DETECT_TYPE DetectType, MS_BOOL *State)
{
    return Hal_DAC_HotPlugDetect(SelDAC, DetectType, State);
}
