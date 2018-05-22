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
#ifndef _DRVVIF_C_
#define _DRVVIF_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsVersion.h"
#include "halVIF.h"
#include "halVIF_Customer.h"
#include "regVIF.h"
#include "drvVIF.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//****************************************************************************
// VIF lib Changelist
//****************************************************************************
#define MSIF_VIF_LIB_CODE     {'V','I','F','_'}
#define MSIF_VIF_LIBVER       {'1','8'}
#define MSIF_VIF_BUILDNUM     {'1','1'}
#define MSIF_VIF_CHANGELIST   {'0','0','8','5','8','4','8','3'}
#define MSIF_VIF_DASH          {'_'}
#define MSIF_VIF_DATE          {'1','4','0','5','2','0'}    // yymmdd

#define VIF_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_VIF_LIB_CODE,                  /* IP__                                             */  \
    MSIF_VIF_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_VIF_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_VIF_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

const unsigned char VIF_Version1[] = {'V','I','F', '_', '_'};
const unsigned char VIF_Version2[] = {CHIP_TYPE};
const unsigned char VIF_Version3[] = MSIF_VIF_DASH;
const unsigned char VIF_Version4[] = MSIF_VIF_LIBVER;
const unsigned char VIF_Version5[] = MSIF_VIF_DASH;
const unsigned char VIF_Version6[] = MSIF_VIF_BUILDNUM;
const unsigned char VIF_Version7[] = MSIF_VIF_DASH;
const unsigned char VIF_Version8[] = MSIF_VIF_DATE;
const unsigned char VIF_Version9[] = MSIF_VIF_DASH;
const unsigned char VIF_VersionA[] = MSIF_VIF_CHANGELIST;

#define DRVVIFDBG(x)          //x

//-------------------------------------------------------------------------------------------------
//  extern function
//-------------------------------------------------------------------------------------------------
#define DRV_VIF_GetSystemTime()                     MsOS_GetSystemTime()

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_vif_version = {
    .DDI = { VIF_DRV_VERSION },
};

VIFInitialIn VIFInitialIn_inst, VIF_InitData;
VIFUserFilter sVIFUserFilter;
VIFNotchA1A2 sVIFNotchA1A2;
VIFSOS1112 sVIFSOS1112;
VIFSOS33 sVIFSOS33;
DWORD dwVIFPara;

BOOL bEnableUsrSteadyAgcK;
U8 u8UsrSteadyAgcK;
BOOL bEnableUsrNonSteadyAgcK;
U8 u8UsrNonSteadyAgcK;

void DRV_VIF_WriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask)
{
    HAL_VIF_WriteByteMask(u32Reg, u8Val, u8Mask);
}

void DRV_VIF_WriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask)
{
    HAL_VIF_WriteBit(u32Reg, bEnable, u8Mask);
}

void DRV_VIF_WriteByte(U32 u32Reg, U8 u8Val )
{
    HAL_VIF_WriteByte(u32Reg, u8Val);
}

U8 DRV_VIF_ReadByte(U32 u32Reg )
{
    return HAL_VIF_ReadByte(u32Reg);
}

/***********************************************************************************
  Subject:    printf version number
  Function:   DRV_VIF_Version
  Parmeter:
  Return:
  Remark:
************************************************************************************/
void DRV_VIF_MSIFVersion(MSIF_Version** mVersion)
{
    *mVersion = &_drv_vif_version;
}

void DRV_VIF_Version(void)
{
    U8 sVersion[30]={0x00};
    // VIF version
#ifdef _2_DIGIT_CHIP_TYPE_
    memcpy(sVersion, VIF_Version1, 5);
    memcpy(sVersion+5, VIF_Version2, 2);
    memcpy(sVersion+7, VIF_Version3, 1);
    memcpy(sVersion+8, VIF_Version4, 2);
    memcpy(sVersion+10, VIF_Version5, 1);
    memcpy(sVersion+11, VIF_Version6, 2);
    memcpy(sVersion+13, VIF_Version7, 1);
    memcpy(sVersion+14, VIF_Version8, 6);
    memcpy(sVersion+20, VIF_Version9, 1);
    memcpy(sVersion+21, VIF_VersionA, 8);
#else
    memcpy(sVersion, VIF_Version1, 5);
    memcpy(sVersion+5, VIF_Version2, 1);
    memcpy(sVersion+6, VIF_Version3, 1);
    memcpy(sVersion+7, VIF_Version4, 2);
    memcpy(sVersion+9, VIF_Version5, 1);
    memcpy(sVersion+10, VIF_Version6, 2);
    memcpy(sVersion+12, VIF_Version7, 1);
    memcpy(sVersion+13, VIF_Version8, 6);
    memcpy(sVersion+19, VIF_Version9, 1);
    memcpy(sVersion+20, VIF_VersionA, 8);
#endif

    printf("\r\n[VIF Version]: ");
    printf("%s\r\n",sVersion);
}

/***********************************************************************************
  Subject:    read register
  Function:   MDrv_1210_Read_CR_FOE
  Parmeter:
  Return:
  Remark:
************************************************************************************/
U8 DRV_VIF_Read_CR_FOE(void)
{
    //DRVVIFDBG(printf("\r\nDRV_VIF_Read_CR_FOE()"));
    return msVifReadCRFOE();
}
/***********************************************************************************
  Subject:    read register
  Function:   MDrv_1210_Read_CR_LOCK_STATUS
  Parmeter:
  Return:
  Remark:
************************************************************************************/
U8 DRV_VIF_Read_CR_LOCK_STATUS(void)
{
    //DRVVIFDBG(printf("\r\nDRV_VIF_Read_CR_LOCK_STATUS()"));
    return msVifReadLockStatus();
}

void DRV_VIF_SetClock(BOOL bEnable)
{
    DRVVIFDBG(printf("\r\nDRV_VIF_SetClock %d",bEnable));
    HAL_VIF_SetClock(bEnable);
}

void DRV_VIF_Init(VIFInitialIn * pVIF_InitData, DWORD u32InitDataLen)
{
    U32 u32StartTime=DRV_VIF_GetSystemTime();

    DRVVIFDBG(printf("\r\nDRV_VIF_Init"));

    while (DRV_VIF_GetSystemTime()-u32StartTime < 3)
    {
        // do nothing;
    }

    HAL_VIF_RegInit(); // !! any register access should be after this function

    if ( sizeof(VIFInitialIn_inst) == u32InitDataLen)
    {
        memcpy(&VIFInitialIn_inst, pVIF_InitData, u32InitDataLen);
        VIFInitialIn_inst.VifFreqBand = (BYTE)g_FreqBand;

        bEnableUsrSteadyAgcK = FALSE;
        u8UsrSteadyAgcK = 0;
        bEnableUsrNonSteadyAgcK = FALSE;
        u8UsrNonSteadyAgcK = 0;
    }
    else
    {
        printf("\r\nERROR  DRV_VIF init data size FAILE!!");
        return;
    }

    if (VIFInitialIn_inst.VifReserve & _BIT2)
    {
        printf("\r\n------------Start of VIF lib para-----------");
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifTop);// = VIF_TOP;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifIfBaseFreq);// = VIF_IF_BASE_FREQ;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifTunerStepSize);// = VIF_TUNER_STEP_SIZE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSawArch);//=VIF_SAW_ARCH;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifVgaMaximum);// = VIF_VGA_MAXIMUM;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifVgaMinimum);// = VIF_VGA_MINIMUM;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.GainDistributionThr);// = GAIN_DISTRIBUTION_THR;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcVgaBase);//=VIF_AGC_VGA_BASE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcVgaOffs);//=VIF_AGC_VGA_OFFS;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcRefNegative);// = VIF_AGC_REF_NEGATIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcRefPositive);// = VIF_AGC_REF_POSITIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc1Ref);// = VIF_DAGC1_REF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc2Ref);// = VIF_DAGC2_REF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc1GainOv);//=VIF_DAGC1_GAIN_OV;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc2GainOv);//=VIF_DAGC2_GAIN_OV;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKf1);//=VIF_CR_KF1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKp1);//=VIF_CR_KP1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKi1);//=VIF_CR_KI1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKp2);//=VIF_CR_KP2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKi2);//=VIF_CR_KI2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKp);// = VIF_CR_KP;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKi);// = VIF_CR_KI;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrLockThr);// = VIF_CR_LOCK_THR;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrThr);//= VIF_CR_THR;
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrLockNum>>16)&0x0000FFFF));//=VIF_CR_LOCK_NUM;
        printf("%x",(WORD) (VIFInitialIn_inst.VifCrLockNum&0x0000FFFF));//=VIF_CR_LOCK_NUM;
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrUnlockNum>>16)&0x0000FFFF));//=VIF_CR_UNLOCK_NUM;
        printf("%x",(WORD) (VIFInitialIn_inst.VifCrUnlockNum&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrPdErrMax);//=VIF_CR_PD_ERR_MAX;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrLockLeakySel);//=VIF_CR_LOCK_LEAKY_SEL;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrPdX2);//=VIF_CR_PD_X2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrLpfSel);//=VIF_CR_LPF_SEL;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrPdModeSel);//=VIF_CR_PD_MODE_SEL;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjust);//=VIF_CR_KP_KI_ADJUST;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustGear);//=VIF_CR_KP_KI_ADJUST_GEAR;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustThr1);//=VIF_CR_KP_KI_ADJUST_THR1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustThr2);//=VIF_CR_KP_KI_ADJUST_THR2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustThr3);//=VIF_CR_KP_KI_ADJUST_THR3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDynamicTopAdjust);//=VIF_DYNAMIC_TOP_ADJUST;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDynamicTopMin);//=VIF_DYNAMIC_TOP_MIN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAmHumDetection);//=VIF_AM_HUM_DETECTION;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampSel);//=VIF_CLAMPGAIN_CLAMP_SEL;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainSyncbottRef);//=VIF_CLAMPGAIN_SYNCBOTT_REF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainSyncheightRef);//=VIF_CLAMPGAIN_SYNCHEIGHT_REF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainKc);//=VIF_CLAMPGAIN_KC;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainKg);//=VIF_CLAMPGAIN_KG;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampOren);//=VIF_CLAMPGAIN_CLAMP_OREN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainOren);//=VIF_CLAMPGAIN_GAIN_OREN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampOvNegative);//=VIF_CLAMPGAIN_CLAMP_OV_NEGATIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainOvNegative);//=VIF_CLAMPGAIN_GAIN_OV_NEGATIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampOvPositive);//=VIF_CLAMPGAIN_CLAMP_OV_POSITIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainOvPositive);//=VIF_CLAMPGAIN_GAIN_OV_POSITIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampMin);//=VIF_CLAMPGAIN_CLAMP_MIN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampMax);//=VIF_CLAMPGAIN_CLAMP_MAX;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainMin);//=VIF_CLAMPGAIN_GAIN_MIN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainMax);//=VIF_CLAMPGAIN_GAIN_MAX;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainPorchCnt);//=VIF_CLAMPGAIN_PORCH_CNT;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterB_VHF_L);//=VIF_PEAKING_FILTER_B_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterB_VHF_L);//=VIF_YC_DELAY_FILTER_B_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterB_VHF_L);//=VIF_GROUP_DELAY_FILTER_B_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterGH_VHF_L);//=VIF_PEAKING_FILTER_GH_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterGH_VHF_L);//=VIF_YC_DELAY_FILTER_GH_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_L);//=VIF_GROUP_DELAY_FILTER_GH_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterDK_VHF_L);//=VIF_PEAKING_FILTER_DK_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterDK_VHF_L);//=VIF_YC_DELAY_FILTER_DK_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_L);//=VIF_GROUP_DELAY_FILTER_DK_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterI_VHF_L);//=VIF_PEAKING_FILTER_I_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterI_VHF_L);//=VIF_YC_DELAY_FILTER_I_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterI_VHF_L);//=VIF_GROUP_DELAY_FILTER_I_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterL_VHF_L);//=VIF_PEAKING_FILTER_L_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterL_VHF_L);//=VIF_YC_DELAY_FILTER_L_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterL_VHF_L);//=VIF_GROUP_DELAY_FILTER_L_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterLL_VHF_L);//=VIF_PEAKING_FILTER_LL_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterLL_VHF_L);//=VIF_YC_DELAY_FILTER_LL_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_L);//=VIF_GROUP_DELAY_FILTER_LL_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterMN_VHF_L);//=VIF_PEAKING_FILTER_MN_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterMN_VHF_L);//=VIF_YC_DELAY_FILTER_MN_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_L);//=VIF_GROUP_DELAY_FILTER_MN_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterB_VHF_H);//=VIF_PEAKING_FILTER_B_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterB_VHF_H);//=VIF_YC_DELAY_FILTER_B_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterB_VHF_H);//=VIF_GROUP_DELAY_FILTER_B_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterGH_VHF_H);//=VIF_PEAKING_FILTER_GH_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterGH_VHF_H);//=VIF_YC_DELAY_FILTER_GH_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_H);//=VIF_GROUP_DELAY_FILTER_GH_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterDK_VHF_H);//=VIF_PEAKING_FILTER_DK_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterDK_VHF_H);//=VIF_YC_DELAY_FILTER_DK_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_H);//=VIF_GROUP_DELAY_FILTER_DK_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterI_VHF_H);//=VIF_PEAKING_FILTER_I_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterI_VHF_H);//=VIF_YC_DELAY_FILTER_I_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterI_VHF_H);//=VIF_GROUP_DELAY_FILTER_I_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterL_VHF_H);//=VIF_PEAKING_FILTER_L_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterL_VHF_H);//=VIF_YC_DELAY_FILTER_L_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterL_VHF_H);//=VIF_GROUP_DELAY_FILTER_L_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterLL_VHF_H);//=VIF_PEAKING_FILTER_LL_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterLL_VHF_H);//=VIF_YC_DELAY_FILTER_LL_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_H);//=VIF_GROUP_DELAY_FILTER_LL_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterMN_VHF_H);//=VIF_PEAKING_FILTER_MN_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterMN_VHF_H);//=VIF_YC_DELAY_FILTER_MN_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_H);//=VIF_GROUP_DELAY_FILTER_MN_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterB_UHF);//=VIF_PEAKING_FILTER_B_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterB_UHF);//=VIF_YC_DELAY_FILTER_B_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterB_UHF);//=VIF_GROUP_DELAY_FILTER_B_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterGH_UHF);//=VIF_PEAKING_FILTER_GH_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterGH_UHF);//=VIF_YC_DELAY_FILTER_GH_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterGH_UHF);//=VIF_GROUP_DELAY_FILTER_GH_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterDK_UHF);//=VIF_PEAKING_FILTER_DK_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterDK_UHF);//=VIF_YC_DELAY_FILTER_DK_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterDK_UHF);//=VIF_GROUP_DELAY_FILTER_DK_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterI_UHF);//=VIF_PEAKING_FILTER_I_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterI_UHF);//=VIF_YC_DELAY_FILTER_I_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterI_UHF);//=VIF_GROUP_DELAY_FILTER_I_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterL_UHF);//=VIF_PEAKING_FILTER_L_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterL_UHF);//=VIF_YC_DELAY_FILTER_L_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterL_UHF);//=VIF_GROUP_DELAY_FILTER_L_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterLL_UHF);//=VIF_PEAKING_FILTER_LL_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterLL_UHF);//=VIF_YC_DELAY_FILTER_LL_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterLL_UHF);//=VIF_GROUP_DELAY_FILTER_LL_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterMN_UHF);//=VIF_PEAKING_FILTER_MN_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterMN_UHF);//=VIF_YC_DELAY_FILTER_MN_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterMN_UHF);//=VIF_GROUP_DELAY_FILTER_MN_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.ChinaDescramblerBox);// = CHINA_DESCRAMBLER_BOX;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDelayReduce);//= VIF_DELAY_REDUCE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifOverModulation);//=VIF_OVER_MODULATION;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifOverModulationDetect);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetect);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIAGCREF);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifADCOverflowAGCREF);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifChanelScanAGCREF);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetTHR1);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetTHR2);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetTHR3);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetTHR4);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifFreqBand);// = FREQ_VHF_L; // should be changed when setting tuner freq
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC0);//= VIF_SOS21_FILTER_C0;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC1);//= VIF_SOS21_FILTER_C1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC2);//= VIF_SOS21_FILTER_C2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC3);//= VIF_SOS21_FILTER_C3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC4);//= VIF_SOS21_FILTER_C4;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC0);//= VIF_SOS22_FILTER_C0;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC1);//= VIF_SOS22_FILTER_C1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC2);//= VIF_SOS22_FILTER_C2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC3);//= VIF_SOS22_FILTER_C3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC4);//= VIF_SOS22_FILTER_C4;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC0);//= VIF_SOS31_FILTER_C0;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC1);//= VIF_SOS31_FILTER_C1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC2);//= VIF_SOS31_FILTER_C2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC3);//= VIF_SOS31_FILTER_C3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC4);//= VIF_SOS31_FILTER_C4;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC0);//= VIF_SOS32_FILTER_C0;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC1);//= VIF_SOS32_FILTER_C1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC2);//= VIF_SOS32_FILTER_C2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC3);//= VIF_SOS32_FILTER_C3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC4);//= VIF_SOS32_FILTER_C4;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifTunerType);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_B>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_B&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_B);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_GH>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_GH&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_GH);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_DK>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_DK&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_DK);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_I>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_I&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_I);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_L>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_L&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_L);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_LL>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_LL&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_LL);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_MN>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_MN&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_MN);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifReserve);
        printf("\r\n------------End of VIF lib para-----------");
    }

    msVifAdcInitial();

    if (VIFInitialIn_inst.VifIfBaseFreq == IF_FREQ_45_75)
        msVifSetIfFreq(IF_FREQ_4575);
    else if (VIFInitialIn_inst.VifIfBaseFreq == IF_FREQ_38_00)
        msVifSetIfFreq(IF_FREQ_3800);
    else
        msVifSetIfFreq(IF_FREQ_3890);

    msVifSetSoundSystem(VIF_SOUND_DK2);//initial
    msVifTopAdjust();
    msVifInitial();
}

void DRV_VIF_Reset(void)
{
    msVifInitial();
}

void DRV_VIF_Handler(BOOL bAutoScan)
{
    msVifHandler(bAutoScan);
}

void DRV_VIF_SetSoundSystem(VIFSoundSystem ucSoundSystem)
{
    DRVVIFDBG(printf("DRV_VIF_SetSoundSystem ucSoundSystem=%d\n",ucSoundSystem));
    msVifSetSoundSystem(ucSoundSystem);
}

void DRV_VIF_SetIfFreq(IfFrequencyType u16IfFreq)
{
    DRVVIFDBG(printf("DRV_VIF_SetIfFreq() u16IfFreq=%d\n",u16IfFreq));
    msVifSetIfFreq(u16IfFreq);
}

void DRV_VIF_BypassDBBAudioFilter(BOOL bEnable)
{
    DRVVIFDBG(printf("DRV_VIF_BypassDBBAudioFilter() bEnableq=%d\n",bEnable));
    HAL_VIF_BypassDBBAudioFilter(bEnable);
}

void DRV_VIF_SetFreqBand(FrequencyBand u8FreqBand)
{
    DRVVIFDBG(printf("DRV_VIF_SetFreqBand() u8FreqBand=%d\n",u8FreqBand));
    VIFInitialIn_inst.VifFreqBand=u8FreqBand;
    g_FreqBand = u8FreqBand;
}

BOOL DRV_VIF_GetInputLevelIndicator(void)
{
    DRVVIFDBG(printf("DRV_VIF_GetInputLevelIndicator() \n"));
    return HAL_VIF_GetInputLevelIndicator();
}

void DRV_VIF_CopyFilter(BYTE i)
{
    if (i==1)       // set peaking
    {
        VIFInitialIn_inst.VifSos21FilterC0=sVIFUserFilter.VifSos21FilterC0;
        VIFInitialIn_inst.VifSos21FilterC1=sVIFUserFilter.VifSos21FilterC1;
        VIFInitialIn_inst.VifSos21FilterC2=sVIFUserFilter.VifSos21FilterC2;
        VIFInitialIn_inst.VifSos21FilterC3=sVIFUserFilter.VifSos21FilterC3;
        VIFInitialIn_inst.VifSos21FilterC4=sVIFUserFilter.VifSos21FilterC4;
    }
    else if (i==2)    // set YC
    {
        VIFInitialIn_inst.VifSos22FilterC0=sVIFUserFilter.VifSos22FilterC0;
        VIFInitialIn_inst.VifSos22FilterC1=sVIFUserFilter.VifSos22FilterC1;
        VIFInitialIn_inst.VifSos22FilterC2=sVIFUserFilter.VifSos22FilterC2;
        VIFInitialIn_inst.VifSos22FilterC3=sVIFUserFilter.VifSos22FilterC3;
        VIFInitialIn_inst.VifSos22FilterC4=sVIFUserFilter.VifSos22FilterC4;
    }
    else if (i==3)    // set group delay
    {
        VIFInitialIn_inst.VifSos31FilterC0=sVIFUserFilter.VifSos31FilterC0;
        VIFInitialIn_inst.VifSos31FilterC1=sVIFUserFilter.VifSos31FilterC1;
        VIFInitialIn_inst.VifSos31FilterC2=sVIFUserFilter.VifSos31FilterC2;
        VIFInitialIn_inst.VifSos31FilterC3=sVIFUserFilter.VifSos31FilterC3;
        VIFInitialIn_inst.VifSos31FilterC4=sVIFUserFilter.VifSos31FilterC4;
        VIFInitialIn_inst.VifSos32FilterC0=sVIFUserFilter.VifSos32FilterC0;
        VIFInitialIn_inst.VifSos32FilterC1=sVIFUserFilter.VifSos32FilterC1;
        VIFInitialIn_inst.VifSos32FilterC2=sVIFUserFilter.VifSos32FilterC2;
        VIFInitialIn_inst.VifSos32FilterC3=sVIFUserFilter.VifSos32FilterC3;
        VIFInitialIn_inst.VifSos32FilterC4=sVIFUserFilter.VifSos32FilterC4;
    /*    
        sVIFSOS33.Vif_SOS_33_C0=sVIFUserFilter.VifSos33FilterC0;
        sVIFSOS33.Vif_SOS_33_C1=sVIFUserFilter.VifSos33FilterC1;
        sVIFSOS33.Vif_SOS_33_C2=sVIFUserFilter.VifSos33FilterC2;
        sVIFSOS33.Vif_SOS_33_C3=sVIFUserFilter.VifSos33FilterC3;
        sVIFSOS33.Vif_SOS_33_C4=sVIFUserFilter.VifSos33FilterC4;
    */
    }
}

BOOL DRV_VIF_SetParameter(VIF_PARA_GROUP paraGroup, void * pVIF_Para, DWORD u32DataLen)
{
    DRVVIFDBG(printf("DRV_VIF_SetParameter() \n"));

    switch (paraGroup)
    {
        case VIF_PARA_USER_FILTER:
            if ( sizeof(VIFUserFilter) == u32DataLen)
            {
                memcpy(&sVIFUserFilter, (VIFUserFilter *) pVIF_Para, u32DataLen);

                // check if need to update user define peaking filter
                if ((sVIFUserFilter.VifUserPeakingFilterSelect>=PK_START) &&
                    (sVIFUserFilter.VifUserPeakingFilterSelect<=PK_END))
                {
                    DRV_VIF_CopyFilter(1);

                    if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_B_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterB_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_GH_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterGH_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_DK_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterDK_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_I_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterI_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_L_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_LL_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterLL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_MN_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterMN_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_B_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterB_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_GH_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterGH_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_DK_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterDK_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_I_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterI_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_L_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_LL_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterLL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_MN_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterMN_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_B_UHF)
                        VIFInitialIn_inst.VifPeakingFilterB_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_GH_UHF)
                        VIFInitialIn_inst.VifPeakingFilterGH_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_DK_UHF)
                        VIFInitialIn_inst.VifPeakingFilterDK_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_I_UHF)
                        VIFInitialIn_inst.VifPeakingFilterI_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_L_UHF)
                        VIFInitialIn_inst.VifPeakingFilterL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_LL_UHF)
                        VIFInitialIn_inst.VifPeakingFilterLL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_MN_UHF)
                        VIFInitialIn_inst.VifPeakingFilterMN_UHF = 0x80;
                }

                // check if need to update user define YC filter
                if ((sVIFUserFilter.VifUserYcDelayFilterSelect>=YC_START) &&
                    (sVIFUserFilter.VifUserYcDelayFilterSelect<=YC_END))
                {
                    DRV_VIF_CopyFilter(2);

                    if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_B_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterB_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_GH_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterGH_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_DK_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterDK_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_I_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterI_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_L_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_LL_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterLL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_MN_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterMN_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_B_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterB_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_GH_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterGH_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_DK_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterDK_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_I_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterI_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_L_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_LL_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterLL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_MN_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterMN_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_B_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterB_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_GH_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterGH_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_DK_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterDK_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_I_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterI_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_L_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_LL_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterLL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_MN_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterMN_UHF = 0x80;
                }

                // check if need to update user define Group delay filter
                if ((sVIFUserFilter.VifUserGroupDelayFilterSelect>=GP_START) &&
                    (sVIFUserFilter.VifUserGroupDelayFilterSelect<=GP_END))
                {
                    DRV_VIF_CopyFilter(3);

                    if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_B_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterB_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_GH_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_DK_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_I_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterI_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_L_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_LL_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_MN_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_B_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterB_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_GH_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_DK_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_I_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterI_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_L_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_LL_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_MN_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_B_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterB_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_GH_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterGH_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_DK_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterDK_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_I_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterI_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_L_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_LL_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterLL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_MN_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterMN_UHF = 0x80;
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_USER_FILTER");
            break;

        case VIF_PARA_TOP:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                VIFInitialIn_inst.VifTop = (U8) dwVIFPara;
                msVifTopAdjust();
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_TOP");
            break;

        case VIF_PARA_LEAKY_SEL:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                VIFInitialIn_inst.VifCrLockLeakySel = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                DRV_VIF_WriteBit(LOCK_LEAKY_FF_SEL, VIFInitialIn_inst.VifCrLockLeakySel, _BIT0);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_LEAKY_SEL");
            break;

        case VIF_PARA_K_SEL:
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL K_select;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                K_select = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                DRV_VIF_WriteBit(CR_K_SEL, K_select, _BIT6);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_K_SEL");
            break;

        case VIF_PARA_GET_VERSION:
            if ( 30 == u32DataLen)
            {
                U8 sVersion[30]={0x00};
                // VIF version
#ifdef _2_DIGIT_CHIP_TYPE_
                memcpy(sVersion, VIF_Version1, 5);
                memcpy(sVersion+5, VIF_Version2, 2);
                memcpy(sVersion+7, VIF_Version3, 1);
                memcpy(sVersion+8, VIF_Version4, 2);
                memcpy(sVersion+10, VIF_Version5, 1);
                memcpy(sVersion+11, VIF_Version6, 2);
                memcpy(sVersion+13, VIF_Version7, 1);
                memcpy(sVersion+14, VIF_Version8, 6);
                memcpy(sVersion+20, VIF_Version9, 1);
                memcpy(sVersion+21, VIF_VersionA, 8);
#else
                memcpy(sVersion, VIF_Version1, 5);
                memcpy(sVersion+5, VIF_Version2, 1);
                memcpy(sVersion+6, VIF_Version3, 1);
                memcpy(sVersion+7, VIF_Version4, 2);
                memcpy(sVersion+9, VIF_Version5, 1);
                memcpy(sVersion+10, VIF_Version6, 2);
                memcpy(sVersion+12, VIF_Version7, 1);
                memcpy(sVersion+13, VIF_Version8, 6);
                memcpy(sVersion+19, VIF_Version9, 1);
                memcpy(sVersion+20, VIF_VersionA, 8);
#endif
                memcpy(pVIF_Para ,sVersion,30);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_VERSION");
            break;

        case VIF_PARA_SET_BYPASS_PEAKING_SOS21:
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL bBypassSOS21;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                bBypassSOS21 = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                DRV_VIF_WriteBit(BYPASS_SOS21, bBypassSOS21, _BIT2);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_BYPASS_FILTER");
            break;

        case VIF_PARA_GET_PGA2_LEVEL:
            if ( sizeof(BYTE) == u32DataLen)
            {
                BYTE u8Pga2;
                u8Pga2= DRV_VIF_ReadByte(AGC_PGA2C) & 0x1F;
                memcpy(pVIF_Para, &u8Pga2, u32DataLen);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_PGA2_LEVEL");
            break;

	case VIF_PARA_GET_IF:
            if ( sizeof(WORD) == u32DataLen)
	 	{
	 	 U16 u16FreqType = g_FreqType;
		 memcpy(pVIF_Para, &u16FreqType, u32DataLen);
	 	}
		else
                 printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_IF");
		break;

	 case VIF_PARA_GET_K_SEL:
             if ( sizeof(BYTE) == u32DataLen)
            {
                 U8 K_select;

                 if ((DRV_VIF_ReadByte(CR_K_SEL) & _BIT6)!=0)
		       K_select=1;
                 else
		       K_select=0;

                 memcpy(pVIF_Para, &K_select,1);
             }
             else
                 printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_K_SEL");
             break;

        case VIF_PARA_SET_HW_KPKI:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                DRV_VIF_WriteByte(CR_KP1_HW, (U8) dwVIFPara);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_HW_KPKI");
            break;

        case VIF_PARA_SET_TAGC_ODMODE:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                if ((U8) dwVIFPara==0)
                {
                    DRV_VIF_WriteBit(TAGC_ODMODE,  FALSE,  _BIT1);
                    DRV_VIF_WriteBit(TAGC_ODMODE,  FALSE,  _BIT5);
                }
                else if ((U8) dwVIFPara==1) // RF AGC OD MODE
                {
                    DRV_VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT1);
                }
                else if ((U8) dwVIFPara==2) // IF AGC OD MODE
                {
                    DRV_VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT5);
                }
                else if ((U8) dwVIFPara==3)
                {
                    DRV_VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT1);
                    DRV_VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT5);
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_TAGC_ODMODE");
            break;

        case VIF_PARA_SET_NOTCH_A1A2:
            if ( sizeof(VIFNotchA1A2) == u32DataLen)
            {
                memcpy(&sVIFNotchA1A2, (VIFNotchA1A2 *) pVIF_Para, u32DataLen);
                
                if(strcmp(CHIP_TYPE, EDEN) < 0) 
                {
                   DRV_VIF_WriteByte(N_A1_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C0 & 0x00FF));
                   DRV_VIF_WriteByte(N_A1_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C0>>8) & 0x00FF));
                   DRV_VIF_WriteByte(N_A1_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C1 & 0x00FF));
                   DRV_VIF_WriteByte(N_A1_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C1>>8) & 0x00FF));
                   DRV_VIF_WriteByte(N_A1_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C2 & 0x00FF));
                   DRV_VIF_WriteByte(N_A1_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C2>>8) & 0x00FF));
                   DRV_VIF_WriteByte(N_A2_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C0 & 0x00FF));
                   DRV_VIF_WriteByte(N_A2_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C0>>8) & 0x00FF));
                   DRV_VIF_WriteByte(N_A2_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C1 & 0x00FF));
                   DRV_VIF_WriteByte(N_A2_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C1>>8) & 0x00FF));
                   DRV_VIF_WriteByte(N_A2_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C2 & 0x00FF));
                   DRV_VIF_WriteByte(N_A2_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C2>>8) & 0x00FF));
                }
                else
                {
                   if((VIFInitialIn_inst.ChinaDescramblerBox >= 2)&&(VIFInitialIn_inst.ChinaDescramblerBox != 7))
                   {
                      DRV_VIF_WriteByte(N_A1_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C0 & 0x00FF));
                      DRV_VIF_WriteByte(N_A1_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C0>>8) & 0x00FF));
                      DRV_VIF_WriteByte(N_A1_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C1 & 0x00FF));
                      DRV_VIF_WriteByte(N_A1_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C1>>8) & 0x00FF));
                      DRV_VIF_WriteByte(N_A1_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C2 & 0x00FF));
                      DRV_VIF_WriteByte(N_A1_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C2>>8) & 0x00FF));
                      DRV_VIF_WriteByte(N_A2_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C0 & 0x00FF));
                      DRV_VIF_WriteByte(N_A2_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C0>>8) & 0x00FF));
                      DRV_VIF_WriteByte(N_A2_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C1 & 0x00FF));
                      DRV_VIF_WriteByte(N_A2_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C1>>8) & 0x00FF));
                      DRV_VIF_WriteByte(N_A2_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C2 & 0x00FF));
                      DRV_VIF_WriteByte(N_A2_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C2>>8) & 0x00FF));
                   }
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_NOTCH_A1A2");
	     break;

	   case VIF_PARA_SET_SOS_1112:
	 	if( sizeof(VIFSOS1112) == u32DataLen)
	 	{
	 	   memcpy(&sVIFSOS1112, (VIFSOS1112 *) pVIF_Para, u32DataLen);

		   DRV_VIF_WriteByte(SOS11_C0_L, (U8) (sVIFSOS1112.Vif_SOS_11_C0 & 0x00FF));
		   DRV_VIF_WriteByte(SOS11_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C0>>8) & 0x00FF));
		   DRV_VIF_WriteByte(SOS11_C1_L, (U8) (sVIFSOS1112.Vif_SOS_11_C1 & 0x00FF));
		   DRV_VIF_WriteByte(SOS11_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C1>>8) & 0x00FF));
		   DRV_VIF_WriteByte(SOS11_C2_L, (U8) (sVIFSOS1112.Vif_SOS_11_C2 & 0x00FF));
		   DRV_VIF_WriteByte(SOS11_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C2>>8) & 0x00FF));
		   DRV_VIF_WriteByte(SOS11_C3_L, (U8) (sVIFSOS1112.Vif_SOS_11_C3 & 0x00FF));
		   DRV_VIF_WriteByte(SOS11_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C3>>8) & 0x00FF));
		   DRV_VIF_WriteByte(SOS11_C4_L, (U8) (sVIFSOS1112.Vif_SOS_11_C4 & 0x00FF));
		   DRV_VIF_WriteByte(SOS11_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C4>>8) & 0x00FF));
                     
                 if(strcmp(CHIP_TYPE, EDEN) < 0) 
                 {
		      DRV_VIF_WriteByte(SOS12_C0_L, (U8) (sVIFSOS1112.Vif_SOS_12_C0 & 0x00FF));
		      DRV_VIF_WriteByte(SOS12_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C0>>8) & 0x00FF));
		      DRV_VIF_WriteByte(SOS12_C1_L, (U8) (sVIFSOS1112.Vif_SOS_12_C1 & 0x00FF));
		      DRV_VIF_WriteByte(SOS12_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C1>>8) & 0x00FF));
		      DRV_VIF_WriteByte(SOS12_C2_L, (U8) (sVIFSOS1112.Vif_SOS_12_C2 & 0x00FF));
		      DRV_VIF_WriteByte(SOS12_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C2>>8) & 0x00FF));
		      DRV_VIF_WriteByte(SOS12_C3_L, (U8) (sVIFSOS1112.Vif_SOS_12_C3 & 0x00FF));
		      DRV_VIF_WriteByte(SOS12_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C3>>8) & 0x00FF));
		      DRV_VIF_WriteByte(SOS12_C4_L, (U8) (sVIFSOS1112.Vif_SOS_12_C4 & 0x00FF));
		      DRV_VIF_WriteByte(SOS12_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C4>>8) & 0x00FF));
                 }
                 else
                 {
                    if((VIFInitialIn_inst.ChinaDescramblerBox >= 2)&&(VIFInitialIn_inst.ChinaDescramblerBox != 7))
                    {
                        DRV_VIF_WriteByte(SOS12_C0_L, (U8) (sVIFSOS1112.Vif_SOS_12_C0 & 0x00FF));
		          DRV_VIF_WriteByte(SOS12_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C0>>8) & 0x00FF));
		          DRV_VIF_WriteByte(SOS12_C1_L, (U8) (sVIFSOS1112.Vif_SOS_12_C1 & 0x00FF));
		          DRV_VIF_WriteByte(SOS12_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C1>>8) & 0x00FF));
		          DRV_VIF_WriteByte(SOS12_C2_L, (U8) (sVIFSOS1112.Vif_SOS_12_C2 & 0x00FF));
		          DRV_VIF_WriteByte(SOS12_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C2>>8) & 0x00FF));
		          DRV_VIF_WriteByte(SOS12_C3_L, (U8) (sVIFSOS1112.Vif_SOS_12_C3 & 0x00FF));
		          DRV_VIF_WriteByte(SOS12_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C3>>8) & 0x00FF));
		          DRV_VIF_WriteByte(SOS12_C4_L, (U8) (sVIFSOS1112.Vif_SOS_12_C4 & 0x00FF));
		          DRV_VIF_WriteByte(SOS12_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C4>>8) & 0x00FF));
                    }
                    else if(VIFInitialIn_inst.ChinaDescramblerBox == 0)
                    {
                        if(strcmp(CHIP_TYPE, MADISON) >= 0)
                        {
                             DRV_VIF_WriteByte(N_A1_C0_L, (U8) (sVIFSOS1112.Vif_SOS_11_C0 & 0x00FF));
                             DRV_VIF_WriteByte(N_A1_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C0 >>8) & 0x00FF));
                             DRV_VIF_WriteByte(N_A1_C1_L, (U8) (sVIFSOS1112.Vif_SOS_11_C1 & 0x00FF));
                             DRV_VIF_WriteByte(N_A1_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C1 >>8) & 0x00FF));
                             DRV_VIF_WriteByte(N_A1_C2_L, (U8) (sVIFSOS1112.Vif_SOS_11_C3 & 0x00FF));
                             DRV_VIF_WriteByte(N_A1_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C3 >>8) & 0x00FF));
                        
                             DRV_VIF_WriteByte(N_A2_C0_L, (U8) (sVIFSOS1112.Vif_SOS_11_C0 & 0x00FF));
                             DRV_VIF_WriteByte(N_A2_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C0 >>8) & 0x00FF));
                             DRV_VIF_WriteByte(N_A2_C1_L, (U8) (sVIFSOS1112.Vif_SOS_11_C1 & 0x00FF));
                             DRV_VIF_WriteByte(N_A2_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C1 >>8) & 0x00FF));
                             DRV_VIF_WriteByte(N_A2_C2_L, (U8) (sVIFSOS1112.Vif_SOS_11_C3 & 0x00FF));
                             DRV_VIF_WriteByte(N_A2_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C3 >>8) & 0x00FF));
                    
                             DRV_VIF_WriteByte(SOS12_C0_L, (U8) (sVIFSOS1112.Vif_SOS_11_C0 & 0x00FF));
                             DRV_VIF_WriteByte(SOS12_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C0>>8) & 0x00FF));
                             DRV_VIF_WriteByte(SOS12_C1_L, (U8) (sVIFSOS1112.Vif_SOS_11_C1 & 0x00FF));
                             DRV_VIF_WriteByte(SOS12_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C1>>8) & 0x00FF));
                             DRV_VIF_WriteByte(SOS12_C2_L, (U8) (sVIFSOS1112.Vif_SOS_11_C2 & 0x00FF));
                             DRV_VIF_WriteByte(SOS12_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C2>>8) & 0x00FF));
                             DRV_VIF_WriteByte(SOS12_C3_L, (U8) (sVIFSOS1112.Vif_SOS_11_C3 & 0x00FF));
                             DRV_VIF_WriteByte(SOS12_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C3>>8) & 0x00FF));
                             DRV_VIF_WriteByte(SOS12_C4_L, (U8) (sVIFSOS1112.Vif_SOS_11_C4 & 0x00FF));
                             DRV_VIF_WriteByte(SOS12_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C4>>8) & 0x00FF));
                        }
                    }
                 }
	 	}
		else
		   printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_SOS_1112");
		break;

        case VIF_PARA_GET_PD_INV:
             if ( sizeof(BYTE) == u32DataLen)
            {
                 U8 pd_inv;

                 pd_inv=HAL_VIF_GetCrPDInverse();

                 memcpy(pVIF_Para, &pd_inv,1);
             }
             else
                 printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_PD_INV");
             break;

        case VIF_PARA_SET_PD_INV:
            if ( sizeof(DWORD) == u32DataLen)
            {
                 // if in overmodulation, do NOT diable inverse Qpart
                 if ((DRV_VIF_ReadByte(ZERO_DETECT) & _BIT4)==0)
                {
                    memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                    if ((U8) dwVIFPara==1)
                    {
                        HAL_VIF_SetCrPDInverse(TRUE);
                    }
                    else
                    {
                        HAL_VIF_SetCrPDInverse(FALSE);
                    }
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_PD_INV");
            break;

        case VIF_PARA_GET_VGA:
            if ( sizeof(WORD) == u32DataLen)
            {
                U16 u16Vga;
                u16Vga= (U16)(DRV_VIF_ReadByte(AGC_VGA))+ (U16)(DRV_VIF_ReadByte(AGC_VGA+1)<<8) ;
                memcpy(pVIF_Para, &u16Vga, u32DataLen);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_VGA");
            break;

        case VIF_PARA_SET_CLAMPGAIN_GAIN_OV_POS:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                VIFInitialIn_inst.VifClampgainGainOvPositive = (U16)dwVIFPara;
                DRV_VIF_WriteByte(CLAMPGAIN_GAIN_OVERWRITE, dwVIFPara&0x000000FF);    // clampgain gain overwrite value
                DRV_VIF_WriteByteMask(CLAMPGAIN_GAIN_OVERWRITE+1, dwVIFPara>>8, 0x07);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_CLAMPGAIN_GAIN_OV_POS");
            break;

        case VIF_PARA_SET_CLAMPGAIN_GAIN_OV_NEG:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                VIFInitialIn_inst.VifClampgainGainOvNegative = (U16)dwVIFPara;
                DRV_VIF_WriteByte(CLAMPGAIN_GAIN_OVERWRITE, dwVIFPara&0x000000FF);    // clampgain gain overwrite value
                DRV_VIF_WriteByteMask(CLAMPGAIN_GAIN_OVERWRITE+1, dwVIFPara>>8, 0x07);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_CLAMPGAIN_GAIN_OV_NEG");
            break;

        case VIF_PARA_SET_AGC_K:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                if (((dwVIFPara>>24)&0x00000001)==1)
                {
                    bEnableUsrSteadyAgcK = TRUE;
                    u8UsrSteadyAgcK = (dwVIFPara>>16)&0x00000007;
                }
                else
                {
                    bEnableUsrSteadyAgcK = FALSE;
                    u8UsrSteadyAgcK = 0;
                }

                if (((dwVIFPara>>8)&0x00000001)==1)
                {
                    bEnableUsrNonSteadyAgcK = TRUE;
                    u8UsrNonSteadyAgcK = dwVIFPara&0x00000007;
                }
                else
                {
                    bEnableUsrNonSteadyAgcK = FALSE;
                    u8UsrNonSteadyAgcK = 0;
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_AGC_K");
            break;
        case VIF_PARA_SET_AGC_REF:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                VIFInitialIn_inst.VifAgcRefNegative = (U16)dwVIFPara;
                VIFInitialIn_inst.VifAgcRefPositive = (U16)dwVIFPara;
                DRV_VIF_WriteByte(AGC_REF, dwVIFPara&0x000000FF);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_AGC_REF");
            break;            

        case VIF_PARA_SET_IF_RATE_FM:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                DRV_VIF_WriteByte(IF_RATE, (BYTE)(dwVIFPara & 0x000000FF));
                DRV_VIF_WriteByte(IF_RATE+1 , (BYTE)((dwVIFPara >> 8) & 0x000000FF));
                DRV_VIF_WriteByteMask(IF_RATE+2, (BYTE)((dwVIFPara >> 16) & 0x000000FF), 0x3F);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_IF_RATE_FM");
            break;            

        case VIF_PARA_SET_CR_RATE_FM:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                DRV_VIF_WriteByte(CR_RATE, (BYTE)(dwVIFPara & 0x000000FF));
                DRV_VIF_WriteByte(CR_RATE+1 , (BYTE)((dwVIFPara >> 8) & 0x000000FF));
                DRV_VIF_WriteByteMask(CR_RATE+2, (BYTE)((dwVIFPara >> 16) & 0x000000FF), 0x1F);
                DRV_VIF_WriteBit(CR_RATE_INV, 0, _BIT0);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_CR_RATE_FM");
            break;   

       case VIF_PARA_SET_DESCRAMBLERBOX_DELAY:
            if( sizeof(DWORD) == u32DataLen)
            {
                 DWORD VifDelayNumTemp;
                 memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                 if((strcmp(CHIP_TYPE, EULER) == 0) ||(strcmp(CHIP_TYPE, NIKE) == 0) ||(strcmp(CHIP_TYPE, EIFFEL) == 0))
                 {
                    if(VIFInitialIn_inst.ChinaDescramblerBox == 7)
                    {
                         if( g_VifShiftClk == 1)
                         {
                             VifDelayNumTemp = dwVIFPara;
                             VifDelayNumTemp = (VifDelayNumTemp * 1992) >> 11;
                             DRV_VIF_WriteByte(0x123F4, (BYTE)(VifDelayNumTemp & 0x000000FF)); // DAGC1_DLY_NUM
                             DRV_VIF_WriteByteMask(0x123F5, (BYTE)((VifDelayNumTemp >> 8) & 0x000000FF), 0x1F);
                         }
                         else if(g_VifShiftClk == 2)
                         {
                             VifDelayNumTemp = dwVIFPara;
                             VifDelayNumTemp = (VifDelayNumTemp * 2105) >> 11;
                             DRV_VIF_WriteByte(0x123F4, (BYTE)(VifDelayNumTemp & 0x000000FF)); // DAGC1_DLY_NUM
                             DRV_VIF_WriteByteMask(0x123F5, (BYTE)((VifDelayNumTemp >> 8) & 0x000000FF), 0x1F);
                         }
                         else
                         {
                             DRV_VIF_WriteByte(0x123F4, (BYTE)(dwVIFPara & 0x000000FF)); // DAGC1_DLY_NUM
                             DRV_VIF_WriteByteMask(0x123F5, (BYTE)((dwVIFPara >> 8) & 0x000000FF), 0x1F);
                         }
                        
                         DRV_VIF_WriteBit(0x123F5, 1, _BIT7);                                 // DAGC1_DLY_EN    
                         DRV_VIF_WriteByteMask(VIFDAC_OUT_SEL, 0x04, 0x07);    // 0: cvbs output; 4: debug bus
                         DRV_VIF_WriteBit(DEBUG2_EN, 1, _BIT7);                           // select debug2 data
                         DRV_VIF_WriteByteMask(DEBUG_MODULE, 0x00, 0x0F);      // select filter debug bus
                         DRV_VIF_WriteByte(DEBUG_PORT, 0x8D);                           // selsect CVBS output after DAGC1 filter

                         if(strcmp(CHIP_TYPE, NIKE) == 0)
                              DRV_VIF_WriteBit(DAGC2_DL_BYPASS, 1, _BIT3);
                    }
                 }
                 else if(strcmp(CHIP_TYPE, EINSTEIN) >= 0)
                 {
                    if(VIFInitialIn_inst.ChinaDescramblerBox == 0)
                    {
                         if( g_VifShiftClk == 1)
                         {
                             VifDelayNumTemp = dwVIFPara;
                             VifDelayNumTemp = (VifDelayNumTemp * 1992) >> 11;
                             VifDelayNumTemp = VifDelayNumTemp + 0x00000751; // offset = 44.6us
                             DRV_VIF_WriteByte(0x12124, (BYTE)(VifDelayNumTemp & 0x000000FF)); // DAC_DLY_NUM
                             DRV_VIF_WriteByteMask(0x12125, (BYTE)((VifDelayNumTemp >> 8) & 0x000000FF), 0x0F);
                         }
                         else if(g_VifShiftClk == 2)
                         {
                             VifDelayNumTemp = dwVIFPara; 
                             VifDelayNumTemp = (VifDelayNumTemp * 2105) >> 11;
                             VifDelayNumTemp = VifDelayNumTemp + 0x00000330; // offset = 18.4us
                             DRV_VIF_WriteByte(0x12124, (BYTE)(VifDelayNumTemp & 0x000000FF)); // DAC_DLY_NUM
                             DRV_VIF_WriteByteMask(0x12125, (BYTE)((VifDelayNumTemp >> 8) & 0x000000FF), 0x0F);
                         }
                         else
                         {
                             DRV_VIF_WriteByte(0x12124, (BYTE)(dwVIFPara & 0x000000FF)); // DAC_DLY_NUM
                             DRV_VIF_WriteByteMask(0x12125, (BYTE)((dwVIFPara >> 8) & 0x000000FF), 0x0F);
                         }
                         
                         DRV_VIF_WriteBit(0x12125, 0, _BIT7);                                  //DAC_DLY not bypass
                         DRV_VIF_WriteByteMask(VIFDAC_OUT_SEL, 0x00, 0x07);    // 0: cvbs output; 4: debug bus
                         DRV_VIF_WriteBit(DEBUG2_EN, 0, _BIT7);                           // not select debug2 data
                    }
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_DESCRAMBLERBOX_DELAY");
            break;

        case VIF_PARA_SET_SSC_ENABLE:
            if( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                
                if((dwVIFPara&0x00000001) != 0)
                   DRV_VIF_WriteBit(0x120A1, 1, _BIT0);
                else
                   DRV_VIF_WriteBit(0x120A1, 0, _BIT0);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_SSC_ENABLE");
            break;
            
        case VIF_PARA_SET_DISABLE_ADAGC:
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL bDisableAdagc;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                bDisableAdagc = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                DRV_VIF_WriteBit(ADAGC_BYPASS, bDisableAdagc, _BIT1);
                DRV_VIF_WriteBit(ADAGC_ENABLE, !bDisableAdagc, _BIT0);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_DISABLE_ADAGC");
            break;
           
        case VIF_PARA_SET_ENABLE_SOS33:  
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL bEnableSOS33;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                bEnableSOS33 = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                DRV_VIF_WriteBit(0x1220A, !bEnableSOS33, _BIT6);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_ENABLE_SOS33");
            break;
     
        default:
            break;
    }
    return TRUE;
}

void DRV_VIF_ShiftClk(BYTE VifShiftClk)
{
    DRVVIFDBG(printf("DRV_VIF_ShiftClk() VifShiftClk=%d\n",VifShiftClk));
    #ifdef _2_DIGIT_CHIP_TYPE_
    msVifShiftClk(VifShiftClk);
    #endif
}

void DRV_VIF_LoadVIFParameter(void)
{
     //printf("\n^^^^^^^^^^     MDrv_VIF_Init >>>>>>>>>>>>>>>>\r\n");
    VIF_InitData.VifTop = 3;
    VIF_InitData.VifIfBaseFreq = IF_FREQ_38_00;
    VIF_InitData.VifTunerStepSize = FREQ_STEP_50KHz;
    VIF_InitData.VifSawArch= 3;
    VIF_InitData.VifVgaMaximum = 0x7000;
    VIF_InitData.VifVgaMinimum = 0x8000;
    VIF_InitData.GainDistributionThr = 0x6A00;
    VIF_InitData.VifAgcVgaBase= 0xD4;
    VIF_InitData.VifAgcVgaOffs= 0x40;
    VIF_InitData.VifAgcRefNegative = 0x60;
    VIF_InitData.VifAgcRefPositive = 0x19;
    VIF_InitData.VifDagc1Ref = 0x26;
    VIF_InitData.VifDagc2Ref = 0x26;
    VIF_InitData.VifDagc1GainOv= 0x1800;
    VIF_InitData.VifDagc2GainOv= 0x1800;
    VIF_InitData.VifCrKf1= 0x03;
    VIF_InitData.VifCrKp1= 0x03;
    VIF_InitData.VifCrKi1= 0x03;
    VIF_InitData.VifCrKp2= 0x06;
    VIF_InitData.VifCrKi2= 0x05;
    VIF_InitData.VifCrKp = 0x06;
    VIF_InitData.VifCrKi = 0x09;
    VIF_InitData.VifCrLockThr = 0x20;
    VIF_InitData.VifCrThr= 0x500;
    VIF_InitData.VifCrLockNum= 0x8000;
    VIF_InitData.VifCrUnlockNum= 0x4000;
    VIF_InitData.VifCrPdErrMax= 0x3FFF;
    VIF_InitData.VifCrLockLeakySel= 1;
    VIF_InitData.VifCrPdX2= 1;
    VIF_InitData.VifCrLpfSel= 1;
    VIF_InitData.VifCrPdModeSel= 0;
    VIF_InitData.VifCrKpKiAdjust= 1;
    VIF_InitData.VifCrKpKiAdjustGear= 2;
    VIF_InitData.VifCrKpKiAdjustThr1= 0x05;
    VIF_InitData.VifCrKpKiAdjustThr2= 0x10;
    VIF_InitData.VifCrKpKiAdjustThr3= 0x03;
    VIF_InitData.VifDynamicTopAdjust= 0;
    VIF_InitData.VifDynamicTopMin= 0;
    VIF_InitData.VifAmHumDetection= 0;
    VIF_InitData.VifClampgainClampSel= 1;
    VIF_InitData.VifClampgainSyncbottRef= 0x78;
    VIF_InitData.VifClampgainSyncheightRef= 0x68;
    VIF_InitData.VifClampgainKc= 0x07;
    VIF_InitData.VifClampgainKg= 0x07;
    VIF_InitData.VifClampgainClampOren= 1;
    VIF_InitData.VifClampgainGainOren= 1;
    VIF_InitData.VifClampgainClampOvNegative= 0x750;
    VIF_InitData.VifClampgainGainOvNegative= 0x5B0;
    VIF_InitData.VifClampgainClampOvPositive= 0x5F0;
    VIF_InitData.VifClampgainGainOvPositive= 0x650;
    VIF_InitData.VifClampgainClampMin= 0x80;
    VIF_InitData.VifClampgainClampMax= 0x00;
    VIF_InitData.VifClampgainGainMin= 0x40;
    VIF_InitData.VifClampgainGainMax= 0xFF;
    VIF_InitData.VifClampgainPorchCnt= 0xC8;

    VIF_InitData.VifPeakingFilterB_VHF_L= 0;
    VIF_InitData.VifYcDelayFilterB_VHF_L= 1;
    VIF_InitData.VifGroupDelayFilterB_VHF_L= 2;
    VIF_InitData.VifPeakingFilterGH_VHF_L= 0;
    VIF_InitData.VifYcDelayFilterGH_VHF_L= 1;
    VIF_InitData.VifGroupDelayFilterGH_VHF_L= 2;
    VIF_InitData.VifPeakingFilterDK_VHF_L= 5;
    VIF_InitData.VifYcDelayFilterDK_VHF_L= 0;
    VIF_InitData.VifGroupDelayFilterDK_VHF_L= 0;
    VIF_InitData.VifPeakingFilterI_VHF_L= 5;
    VIF_InitData.VifYcDelayFilterI_VHF_L= 0;
    VIF_InitData.VifGroupDelayFilterI_VHF_L= 0;
    VIF_InitData.VifPeakingFilterL_VHF_L= 0;
    VIF_InitData.VifYcDelayFilterL_VHF_L= 0;
    VIF_InitData.VifGroupDelayFilterL_VHF_L= 0;
    VIF_InitData.VifPeakingFilterLL_VHF_L= 0;
    VIF_InitData.VifYcDelayFilterLL_VHF_L= 0;
    VIF_InitData.VifGroupDelayFilterLL_VHF_L= 0;
    VIF_InitData.VifPeakingFilterMN_VHF_L= 3;
    VIF_InitData.VifYcDelayFilterMN_VHF_L= 0;
    VIF_InitData.VifGroupDelayFilterMN_VHF_L= 6;

    VIF_InitData.VifPeakingFilterB_VHF_H= 0;
    VIF_InitData.VifYcDelayFilterB_VHF_H= 1;
    VIF_InitData.VifGroupDelayFilterB_VHF_H= 2;
    VIF_InitData.VifPeakingFilterGH_VHF_H= 0;
    VIF_InitData.VifYcDelayFilterGH_VHF_H= 1;
    VIF_InitData.VifGroupDelayFilterGH_VHF_H= 2;
    VIF_InitData.VifPeakingFilterDK_VHF_H= 5;
    VIF_InitData.VifYcDelayFilterDK_VHF_H= 0;
    VIF_InitData.VifGroupDelayFilterDK_VHF_H= 0;
    VIF_InitData.VifPeakingFilterI_VHF_H= 5;
    VIF_InitData.VifYcDelayFilterI_VHF_H= 0;
    VIF_InitData.VifGroupDelayFilterI_VHF_H= 0;
    VIF_InitData.VifPeakingFilterL_VHF_H= 0;
    VIF_InitData.VifYcDelayFilterL_VHF_H= 0;
    VIF_InitData.VifGroupDelayFilterL_VHF_H= 0;
    VIF_InitData.VifPeakingFilterLL_VHF_H= 0;
    VIF_InitData.VifYcDelayFilterLL_VHF_H= 0;
    VIF_InitData.VifGroupDelayFilterLL_VHF_H= 0;
    VIF_InitData.VifPeakingFilterMN_VHF_H= 3;
    VIF_InitData.VifYcDelayFilterMN_VHF_H= 0;
    VIF_InitData.VifGroupDelayFilterMN_VHF_H= 6;

    VIF_InitData.VifPeakingFilterB_UHF= 6;
    VIF_InitData.VifYcDelayFilterB_UHF= 1;
    VIF_InitData.VifGroupDelayFilterB_UHF= 2;
    VIF_InitData.VifPeakingFilterGH_UHF= 6;
    VIF_InitData.VifYcDelayFilterGH_UHF= 1;
    VIF_InitData.VifGroupDelayFilterGH_UHF= 2;
    VIF_InitData.VifPeakingFilterDK_UHF= 5;
    VIF_InitData.VifYcDelayFilterDK_UHF= 0;
    VIF_InitData.VifGroupDelayFilterDK_UHF= 0;
    VIF_InitData.VifPeakingFilterI_UHF= 5;
    VIF_InitData.VifYcDelayFilterI_UHF= 0;
    VIF_InitData.VifGroupDelayFilterI_UHF= 0;
    VIF_InitData.VifPeakingFilterL_UHF= 0;
    VIF_InitData.VifYcDelayFilterL_UHF= 0;
    VIF_InitData.VifGroupDelayFilterL_UHF= 0;
    VIF_InitData.VifPeakingFilterLL_UHF= 0;
    VIF_InitData.VifYcDelayFilterLL_UHF= 0;
    VIF_InitData.VifGroupDelayFilterLL_UHF=0;
    VIF_InitData.VifPeakingFilterMN_UHF= 3;
    VIF_InitData.VifYcDelayFilterMN_UHF= 0;
    VIF_InitData.VifGroupDelayFilterMN_UHF= 6;
    VIF_InitData.ChinaDescramblerBox = 0;
    VIF_InitData.VifDelayReduce= 0x00;
    VIF_InitData.VifOverModulation= 0;
    VIF_InitData.VifOverModulationDetect= 0;

    VIF_InitData.VifACIDetect= 0;
    VIF_InitData.VifACIAGCREF= 0x30;
    VIF_InitData.VifADCOverflowAGCREF= 0x20;
    VIF_InitData.VifChanelScanAGCREF= 0x20;
    VIF_InitData.VifSeriousACIDetect= 0;
    VIF_InitData.VifACIDetTHR1= 0x16;
    VIF_InitData.VifACIDetTHR2= 0x0A;
    VIF_InitData.VifACIDetTHR3= 0x08;
    VIF_InitData.VifACIDetTHR4= 0x03;

    VIF_InitData.VifFreqBand = FREQ_VHF_L; // should be changed when setting tuner freq
    VIF_InitData.VifSos21FilterC0= 0x0000;
    VIF_InitData.VifSos21FilterC1= 0x0000;
    VIF_InitData.VifSos21FilterC2= 0x0000;
    VIF_InitData.VifSos21FilterC3= 0x0000;
    VIF_InitData.VifSos21FilterC4= 0x0200;
    VIF_InitData.VifSos22FilterC0= 0x0000;
    VIF_InitData.VifSos22FilterC1= 0x0000;
    VIF_InitData.VifSos22FilterC2= 0x0000;
    VIF_InitData.VifSos22FilterC3= 0x0000;
    VIF_InitData.VifSos22FilterC4= 0x0200;
    VIF_InitData.VifSos31FilterC0= 0x0000;
    VIF_InitData.VifSos31FilterC1= 0x0000;
    VIF_InitData.VifSos31FilterC2= 0x0000;
    VIF_InitData.VifSos31FilterC3= 0x0000;
    VIF_InitData.VifSos31FilterC4= 0x0200;
    VIF_InitData.VifSos32FilterC0= 0x0000;
    VIF_InitData.VifSos32FilterC1= 0x0000;
    VIF_InitData.VifSos32FilterC2= 0x0000;
    VIF_InitData.VifSos32FilterC3= 0x0000;
    VIF_InitData.VifSos32FilterC4= 0x0200;

    VIF_InitData.VifTunerType= 0;
    VIF_InitData.VifCrRate_B= 0x00097B42;
    VIF_InitData.VifCrInvert_B= 0;
    VIF_InitData.VifCrRate_GH= 0x000A0000;
    VIF_InitData.VifCrInvert_GH= 0;
    VIF_InitData.VifCrRate_DK= 0x000A25ED;
    VIF_InitData.VifCrInvert_DK= 0;
    VIF_InitData.VifCrRate_I= 0x000ABDA1;
    VIF_InitData.VifCrInvert_I= 0;
    VIF_InitData.VifCrRate_L= 0x000A0000;
    VIF_InitData.VifCrInvert_L= 0;
    VIF_InitData.VifCrRate_LL= 0x0001DA12;
    VIF_InitData.VifCrInvert_LL= 1;
    VIF_InitData.VifCrRate_MN= 0x00080000;
    VIF_InitData.VifCrInvert_MN= 0;
   
}

MS_U32 MDrv_VIF_SetPowerState(EN_POWER_MODE u16PowerState)
{
    if(u16PowerState == 2)   // RESUME
    {
       DRV_VIF_LoadVIFParameter();
       DRV_VIF_Init(&VIF_InitData, sizeof(VIF_InitData));

       return TRUE;
    }
    else
       return FALSE;
}

#endif //_DRVVIF_C_

