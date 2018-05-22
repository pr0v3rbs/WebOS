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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvPWM.c
/// @brief  Pulse Width Modulation Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Include Files
////////////////////////////////////////////////////////////////////////////////
// Common Definition
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "MsDevice.h"
#include "MsOS.h"
#include "mdrv_ld_pwm.h"
#include "mhal_ld_pwm.h"

#define DEBUG_PWM(x)		//x
#define CRYSTAL_CLOCK	12000000ul//24000000ul

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

MS_U16 AlignTo2expo(MS_U16 u16Input)
{
    MS_U16 u16Checkbit = BIT(0);

    while(u16Input > u16Checkbit)   //Increase return value until >= input
    {
        u16Checkbit<<=1;
    }

    return u16Checkbit;
}

//==========================================================
//Description:
//u8PWMNum(PWM0 ~PWM5)      : 0~5
//ucFrequency     : PWM frequency
//bVsyncEn         : PWM synchronize with Vsync
//Period             : 18bit
//ucDuty             : Duty=  0~100
//shift              : 0~360
//==========================================================

void MDrv_MFC_PWM_waveform_maker(DRV_PWM_CH u8PWMNum, MS_U16 u16FrequencyX10, MS_U16 u16DutyInPercentX100,
                                 MS_U16 u16ShiftIn360DegreeX100, MS_U16 u16Div, MS_U8 SyncRstCnt,
                                 MS_U8 u8DoubleBufferSelect, EnuPwmRstCtrl ePwmRstCtrl, MS_BOOL bPolarity
                                 )
{
    MS_U32 u32Duty, u32Shift, u32PWM_LocalClk;
    MS_U32 u32temp;
    MS_U32 u32Period0,u32Period1;
    MS_BOOL bReg_RESET_EN = 0;
    MS_BOOL bReg_RST_MUX = 0;
    MS_BOOL bReg_NVS_RST_EN = 0;
    MS_BOOL bReg_NVS_align_en = 0;
    MS_BOOL bReg_LR_flag_rising_pulse = 0;
    MS_U8 u8HSyncResetCount = 0;

    DEBUG_PWM( printf("MDrv_MFC_PWM_waveform_maker(u8PWMNum=%u)\n", u8PWMNum); );
    DEBUG_PWM( printf(" (u16FrequencyX10=%u)\n", u16FrequencyX10););
    DEBUG_PWM( printf(" (SyncRstCnt=%u)\n", SyncRstCnt); );


    //PWM waveform could be reset by display VSYNC or HSYNC to keep the synchronous relation.
    //(1) When RESET_EN = 1 and RST_MUX = 0,
    //      PWM waveform will be reset by incoming VSYNC. As the diagram1 shown below.
    //      => Reset every Vsync
    //(2) When RESET_EN = 1 and RST_MUX = 1 and NVS_RST_EN = 0,
    //      PWM waveform will be reset by incoming HSYNC.  If you need reset every 2 HSYNC, set HS_RST_CNT = 1(start from 0). As the diagram2 shown below.
    //(3) When RESET_EN = 1 and RST_MUX = 1 and NVS_RST_EN = 1,
    //      PWM waveform will be reset by incoming VSYNC. If you need reset every 2 VSYNC, set HS_RST_CNT = 1

    if( ePwmRstCtrl == PWM_RST_CTRL__LRFLAG )
    {
        bReg_RESET_EN = TRUE;
        bReg_RST_MUX = 1; // for NVS
        bReg_NVS_RST_EN = 1;
        bReg_LR_flag_rising_pulse = 1;
    }
    else if( ePwmRstCtrl == PWM_RST_CTRL__VSYNC )
    {
        bReg_RESET_EN = TRUE;

        
        //bReg_RST_MUX=0 : each Vsync reset
        //bReg_RST_MUX=1 : multiple Vsync reset

        if (SyncRstCnt>0)
        {
            bReg_NVS_RST_EN = 1;
            bReg_RST_MUX = 1; // for NVS
        }
        else
        {
            bReg_RST_MUX = 0; // for V
        }

        u8HSyncResetCount = SyncRstCnt;
    }
    else if( ePwmRstCtrl == PWM_RST_CTRL__HSYNC )// HSync_Rst
    {
        bReg_RESET_EN = TRUE;
        bReg_RST_MUX = 1; // for H sync
        u8HSyncResetCount = SyncRstCnt;
    }
    else // PWM_RST_NONE
    {
        bReg_RESET_EN = FALSE;
        bReg_RST_MUX = 0;
        bReg_LR_flag_rising_pulse = 0;
    }

    UNUSED(bReg_RESET_EN);
    
    u32PWM_LocalClk = (CRYSTAL_CLOCK + u16Div/2)/ u16Div;
    u32Period0 =( u32PWM_LocalClk *10 + (MS_U32)(u16FrequencyX10)/2) / (MS_U32)(u16FrequencyX10);

#if 0 //( PANEL_TYPE_SEL == PNL_LTA750HQ01_VX1_FHD_240HZ )
    if( ePwmRstCtrl == PWM_RST_CTRL__NONE )
    {
        u32Period1 = u32Period0;
    }
    else if( (u8PWMNum == LD_SEC75_VSYNC_PWM_CH_SEL) &&  (u16FrequencyX10<1300) )
    {
        u32Period1 = u32Period0 + 0x40;
    }
    else
    {
        u32Period1 = u32Period0*5/4 ;//add 1/4 period for timing change (240->200Hz)
    }    
#else
    if(( ePwmRstCtrl != PWM_RST_CTRL__NONE ) /*&&(MDrv_PNL_Get_CurOutputVFreqX10()<u16FrequencyX10) */)
    {
        u32Period1 = u32Period0*5/4 ;//add 1/4 period for timing change (240->200Hz)
    }
    else
    {
        u32Period1 = u32Period0;
    }
#endif

    if (u32Period1 > 0x3FFFF)    //Period overflow protection
    {
        printf("Warning: Period is over 0x3FFFF\n");
        u32temp = u32Period1 / 0x3FFFF;
        u32temp = AlignTo2expo(u32temp+1);        //Divider should be exponential of 2
        u32Period1 = u32Period1 / (u32temp);    //Period = 1.x * 0x3FFFF => Period/2, Divider*2
        u32Period0 = u32Period0 / (u32temp);    //Period = 1.x * 0x3FFFF => Period/2, Divider*2
        u16Div = u16Div * (u32temp);
    }
    DEBUG_PWM( printf(" u32Period=0x%X\n", u32Period1); );

    u32Shift = (u32Period0 * (MS_U32)u16ShiftIn360DegreeX100 + 36000/2) / 36000;
    u32Duty  = (u32Period0 * (MS_U32)u16DutyInPercentX100 + 10000/2) / 10000 + u32Shift;

    DEBUG_PWM( printf(" u32Shift=0x%X\n", u32Shift); );
    DEBUG_PWM( printf(" u32Duty=0x%X\n", u32Duty); );

    MHal_PWM_SWVdbEn(u8PWMNum, 0);

    MHal_PWM_HWDoubleBuffer(u8PWMNum, u8DoubleBufferSelect);

    MHal_PWM_ResetMuxSel(u8PWMNum, bReg_RST_MUX);
    MHal_PWM_NVS_RST_EN(u8PWMNum, bReg_NVS_RST_EN);
    MHal_PWM_NVS_align_en(u8PWMNum, bReg_NVS_align_en);
    MHal_PWM_Set_LR_flag_rising_pulse(u8PWMNum,bReg_LR_flag_rising_pulse); // reg_pwm0_lr_sync_rp

    MHal_PWM_HsyncResetCnt(u8PWMNum, u8HSyncResetCount);
    MHal_PWM_SetPeriod(u8PWMNum, u32Period1-1); //"+4" for longer than one frame no Vsyn rst issue
    //MHal_PWM_SetPeriod(u8PWMNum, u32Period-1 + 0x40); //"+4" for longer than one frame no Vsyn rst issue
    MHal_PWM_SetDuty(u8PWMNum, u32Duty-1);
    MHal_PWM_SetShift(u8PWMNum, u32Shift);
    MHal_PWM_SetDivider(u8PWMNum, u16Div-1);
    MHal_PWM_SetPolarity(u8PWMNum, bPolarity);

    MHal_PWM_SWVdbEn(u8PWMNum, 1);
}

