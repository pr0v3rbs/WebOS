////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 MStar Semiconductor, Inc.
//
// You can redistribute it and/or modify it under the terms of the GNU General Public
// License version 2 as published by the Free Foundation. This program is distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_VIF_C_
#define _MDRV_VIF_C_
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrv_vif.c
/// @brief  VIF Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
//#include "MsCommon.h"
#include <linux/autoconf.h>
//#include <linux/undefconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <asm/io.h>

#include "mst_devid.h"

#include "mdrv_vif.h"
#include "mhal_vif_reg.h"
#include "mhal_vif.h"
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define MDRV_VIF_GetSystemTime()    ((unsigned long)((jiffies)*(1000/HZ)))

#define DRVVIFDBG(x)       //  x

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern FrequencyBand g_FreqBand;
extern IfFrequencyType g_FreqType;
//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

//static MSIF_Version _drv_vif_version = {
//    .DDI = { VIF_DRV_VERSION },
//};

VIFInitialIn VIFInitialIn_inst;
VIFUserFilter sVIFUserFilter;
VIFNotchA1A2 sVIFNotchA1A2;
VIFSOS1112 sVIFSOS1112;
DWORD dwVIFPara;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

void MDRV_VIF_WriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask)
{
    MHAL_VIF_WriteByteMask(u32Reg, u8Val, u8Mask);
}

void MDRV_VIF_WriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask)
{
    MHAL_VIF_WriteBit(u32Reg, bEnable, u8Mask);
}

void MDRV_VIF_WriteByte(U32 u32Reg, U8 u8Val )
{
    MHAL_VIF_WriteByte(u32Reg, u8Val);
}

U8 MDRV_VIF_ReadByte(U32 u32Reg )
{
    return MHAL_VIF_ReadByte(u32Reg);
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#if 0
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
#endif

/***********************************************************************************
  Subject:    read register
  Function:   MDrv_1210_Read_CR_FOE
  Parmeter:
  Return:
  Remark:
************************************************************************************/
U8 MDRV_VIF_Read_CR_FOE(void)
{
    DRVVIFDBG(printk("\r\nDRV_VIF_Read_CR_FOE()"));
    return msVifReadCRFOE();
}
/***********************************************************************************
  Subject:    read register
  Function:   MDrv_1210_Read_CR_LOCK_STATUS
  Parmeter:
  Return:
  Remark:
************************************************************************************/
U8 MDRV_VIF_Read_CR_LOCK_STATUS(void)
{
    DRVVIFDBG(printk("\r\nDRV_VIF_Read_CR_LOCK_STATUS()"));
    return msVifReadLockStatus();
}

void MDRV_VIF_SetClock(BOOL bEnable)
{
    DRVVIFDBG(printk("\r\nDRV_VIF_SetClock %d",bEnable));
    MHAL_VIF_SetClock(bEnable);
}

void MDRV_VIF_Init(VIFInitialIn * pVIF_InitData, DWORD u32InitDataLen)
{
    U32 u32StartTime=MDRV_VIF_GetSystemTime();

    DRVVIFDBG(printk("\r\nDRV_VIF_Init"));

    while (MDRV_VIF_GetSystemTime()-u32StartTime < 3);

    MHAL_VIF_RegInit(); // !! any register access should be after this function

    if ( sizeof(VIFInitialIn_inst) == u32InitDataLen)
    {
        memcpy(&VIFInitialIn_inst, pVIF_InitData, u32InitDataLen);
        VIFInitialIn_inst.VifFreqBand = (BYTE)g_FreqBand;
    }
    else
    {
        printk("\r\nERROR  DRV_VIF init data size FAILE!!");
        return;
    }

/*
    printk("\r\n------------Start of VIF lib para-----------",0);
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifTop);// = VIF_TOP;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifIfBaseFreq);// = VIF_IF_BASE_FREQ;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifTunerStepSize);// = VIF_TUNER_STEP_SIZE;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSawArch);//=VIF_SAW_ARCH;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifVgaMaximum);// = VIF_VGA_MAXIMUM;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifVgaMinimum);// = VIF_VGA_MINIMUM;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.GainDistributionThr);// = GAIN_DISTRIBUTION_THR;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcVgaBase);//=VIF_AGC_VGA_BASE;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcVgaOffs);//=VIF_AGC_VGA_OFFS;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcRefNegative);// = VIF_AGC_REF_NEGATIVE;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcRefPositive);// = VIF_AGC_REF_POSITIVE;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc1Ref);// = VIF_DAGC1_REF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc2Ref);// = VIF_DAGC2_REF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc1GainOv);//=VIF_DAGC1_GAIN_OV;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc2GainOv);//=VIF_DAGC2_GAIN_OV;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKf1);//=VIF_CR_KF1;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKp1);//=VIF_CR_KP1;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKi1);//=VIF_CR_KI1;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKp2);//=VIF_CR_KP2;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKi2);//=VIF_CR_KI2;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKp);// = VIF_CR_KP;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKi);// = VIF_CR_KI;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrLockThr);// = VIF_CR_LOCK_THR;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrThr);//= VIF_CR_THR;
    printk("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrLockNum>>16)&0x0000FFFF));//=VIF_CR_LOCK_NUM;
    printk("%x",(WORD) (VIFInitialIn_inst.VifCrLockNum&0x0000FFFF));//=VIF_CR_LOCK_NUM;
    printk("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrUnlockNum>>16)&0x0000FFFF));//=VIF_CR_UNLOCK_NUM;
    printk("%x",(WORD) (VIFInitialIn_inst.VifCrUnlockNum&0x0000FFFF));
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrPdErrMax);//=VIF_CR_PD_ERR_MAX;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrLockLeakySel);//=VIF_CR_LOCK_LEAKY_SEL;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrPdX2);//=VIF_CR_PD_X2;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrLpfSel);//=VIF_CR_LPF_SEL;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrPdModeSel);//=VIF_CR_PD_MODE_SEL;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjust);//=VIF_CR_KP_KI_ADJUST;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustGear);//=VIF_CR_KP_KI_ADJUST_GEAR;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustThr1);//=VIF_CR_KP_KI_ADJUST_THR1;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustThr2);//=VIF_CR_KP_KI_ADJUST_THR2;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustThr3);//=VIF_CR_KP_KI_ADJUST_THR3;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifDynamicTopAdjust);//=VIF_DYNAMIC_TOP_ADJUST;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifDynamicTopMin);//=VIF_DYNAMIC_TOP_MIN;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifAmHumDetection);//=VIF_AM_HUM_DETECTION;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampSel);//=VIF_CLAMPGAIN_CLAMP_SEL;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainSyncbottRef);//=VIF_CLAMPGAIN_SYNCBOTT_REF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainSyncheightRef);//=VIF_CLAMPGAIN_SYNCHEIGHT_REF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainKc);//=VIF_CLAMPGAIN_KC;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainKg);//=VIF_CLAMPGAIN_KG;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampOren);//=VIF_CLAMPGAIN_CLAMP_OREN;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainOren);//=VIF_CLAMPGAIN_GAIN_OREN;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampOvNegative);//=VIF_CLAMPGAIN_CLAMP_OV_NEGATIVE;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainOvNegative);//=VIF_CLAMPGAIN_GAIN_OV_NEGATIVE;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampOvPositive);//=VIF_CLAMPGAIN_CLAMP_OV_POSITIVE;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainOvPositive);//=VIF_CLAMPGAIN_GAIN_OV_POSITIVE;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampMin);//=VIF_CLAMPGAIN_CLAMP_MIN;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampMax);//=VIF_CLAMPGAIN_CLAMP_MAX;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainMin);//=VIF_CLAMPGAIN_GAIN_MIN;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainMax);//=VIF_CLAMPGAIN_GAIN_MAX;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainPorchCnt);//=VIF_CLAMPGAIN_PORCH_CNT;

    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterB_VHF_L);//=VIF_PEAKING_FILTER_B_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterB_VHF_L);//=VIF_YC_DELAY_FILTER_B_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterB_VHF_L);//=VIF_GROUP_DELAY_FILTER_B_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterGH_VHF_L);//=VIF_PEAKING_FILTER_GH_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterGH_VHF_L);//=VIF_YC_DELAY_FILTER_GH_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_L);//=VIF_GROUP_DELAY_FILTER_GH_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterDK_VHF_L);//=VIF_PEAKING_FILTER_DK_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterDK_VHF_L);//=VIF_YC_DELAY_FILTER_DK_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_L);//=VIF_GROUP_DELAY_FILTER_DK_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterI_VHF_L);//=VIF_PEAKING_FILTER_I_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterI_VHF_L);//=VIF_YC_DELAY_FILTER_I_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterI_VHF_L);//=VIF_GROUP_DELAY_FILTER_I_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterL_VHF_L);//=VIF_PEAKING_FILTER_L_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterL_VHF_L);//=VIF_YC_DELAY_FILTER_L_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterL_VHF_L);//=VIF_GROUP_DELAY_FILTER_L_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterLL_VHF_L);//=VIF_PEAKING_FILTER_LL_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterLL_VHF_L);//=VIF_YC_DELAY_FILTER_LL_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_L);//=VIF_GROUP_DELAY_FILTER_LL_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterMN_VHF_L);//=VIF_PEAKING_FILTER_MN_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterMN_VHF_L);//=VIF_YC_DELAY_FILTER_MN_VHF_L;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_L);//=VIF_GROUP_DELAY_FILTER_MN_VHF_L;

    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterB_VHF_H);//=VIF_PEAKING_FILTER_B_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterB_VHF_H);//=VIF_YC_DELAY_FILTER_B_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterB_VHF_H);//=VIF_GROUP_DELAY_FILTER_B_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterGH_VHF_H);//=VIF_PEAKING_FILTER_GH_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterGH_VHF_H);//=VIF_YC_DELAY_FILTER_GH_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_H);//=VIF_GROUP_DELAY_FILTER_GH_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterDK_VHF_H);//=VIF_PEAKING_FILTER_DK_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterDK_VHF_H);//=VIF_YC_DELAY_FILTER_DK_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_H);//=VIF_GROUP_DELAY_FILTER_DK_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterI_VHF_H);//=VIF_PEAKING_FILTER_I_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterI_VHF_H);//=VIF_YC_DELAY_FILTER_I_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterI_VHF_H);//=VIF_GROUP_DELAY_FILTER_I_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterL_VHF_H);//=VIF_PEAKING_FILTER_L_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterL_VHF_H);//=VIF_YC_DELAY_FILTER_L_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterL_VHF_H);//=VIF_GROUP_DELAY_FILTER_L_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterLL_VHF_H);//=VIF_PEAKING_FILTER_LL_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterLL_VHF_H);//=VIF_YC_DELAY_FILTER_LL_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_H);//=VIF_GROUP_DELAY_FILTER_LL_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterMN_VHF_H);//=VIF_PEAKING_FILTER_MN_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterMN_VHF_H);//=VIF_YC_DELAY_FILTER_MN_VHF_H;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_H);//=VIF_GROUP_DELAY_FILTER_MN_VHF_H;

    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterB_UHF);//=VIF_PEAKING_FILTER_B_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterB_UHF);//=VIF_YC_DELAY_FILTER_B_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterB_UHF);//=VIF_GROUP_DELAY_FILTER_B_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterGH_UHF);//=VIF_PEAKING_FILTER_GH_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterGH_UHF);//=VIF_YC_DELAY_FILTER_GH_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterGH_UHF);//=VIF_GROUP_DELAY_FILTER_GH_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterDK_UHF);//=VIF_PEAKING_FILTER_DK_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterDK_UHF);//=VIF_YC_DELAY_FILTER_DK_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterDK_UHF);//=VIF_GROUP_DELAY_FILTER_DK_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterI_UHF);//=VIF_PEAKING_FILTER_I_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterI_UHF);//=VIF_YC_DELAY_FILTER_I_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterI_UHF);//=VIF_GROUP_DELAY_FILTER_I_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterL_UHF);//=VIF_PEAKING_FILTER_L_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterL_UHF);//=VIF_YC_DELAY_FILTER_L_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterL_UHF);//=VIF_GROUP_DELAY_FILTER_L_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterLL_UHF);//=VIF_PEAKING_FILTER_LL_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterLL_UHF);//=VIF_YC_DELAY_FILTER_LL_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterLL_UHF);//=VIF_GROUP_DELAY_FILTER_LL_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterMN_UHF);//=VIF_PEAKING_FILTER_MN_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterMN_UHF);//=VIF_YC_DELAY_FILTER_MN_UHF;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterMN_UHF);//=VIF_GROUP_DELAY_FILTER_MN_UHF;

    printk("\r\n %x",(WORD)VIFInitialIn_inst.ChinaDescramblerBox);// = CHINA_DESCRAMBLER_BOX;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifDelayReduce);//= VIF_DELAY_REDUCE;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifOverModulation);//=VIF_OVER_MODULATION;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifFreqBand);// = FREQ_VHF_L; // should be changed when setting tuner freq
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC0);//= VIF_SOS21_FILTER_C0;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC1);//= VIF_SOS21_FILTER_C1;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC2);//= VIF_SOS21_FILTER_C2;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC3);//= VIF_SOS21_FILTER_C3;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC4);//= VIF_SOS21_FILTER_C4;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC0);//= VIF_SOS22_FILTER_C0;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC1);//= VIF_SOS22_FILTER_C1;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC2);//= VIF_SOS22_FILTER_C2;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC3);//= VIF_SOS22_FILTER_C3;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC4);//= VIF_SOS22_FILTER_C4;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC0);//= VIF_SOS31_FILTER_C0;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC1);//= VIF_SOS31_FILTER_C1;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC2);//= VIF_SOS31_FILTER_C2;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC3);//= VIF_SOS31_FILTER_C3;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC4);//= VIF_SOS31_FILTER_C4;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC0);//= VIF_SOS32_FILTER_C0;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC1);//= VIF_SOS32_FILTER_C1;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC2);//= VIF_SOS32_FILTER_C2;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC3);//= VIF_SOS32_FILTER_C3;
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC4);//= VIF_SOS32_FILTER_C4;

    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifTunerType);
    printk("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_B>>16)&0x0000FFFF));
    printk("%x",(WORD)(VIFInitialIn_inst.VifCrRate_B&0x0000FFFF));
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_B);
    printk("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_GH>>16)&0x0000FFFF));
    printk("%x",(WORD)(VIFInitialIn_inst.VifCrRate_GH&0x0000FFFF));
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_GH);
    printk("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_DK>>16)&0x0000FFFF));
    printk("%x",(WORD)(VIFInitialIn_inst.VifCrRate_DK&0x0000FFFF));
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_DK);
    printk("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_I>>16)&0x0000FFFF));
    printk("%x",(WORD)(VIFInitialIn_inst.VifCrRate_I&0x0000FFFF));
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_I);
    printk("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_L>>16)&0x0000FFFF));
    printk("%x",(WORD)(VIFInitialIn_inst.VifCrRate_L&0x0000FFFF));
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_L);
    printk("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_LL>>16)&0x0000FFFF));
    printk("%x",(WORD)(VIFInitialIn_inst.VifCrRate_LL&0x0000FFFF));
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_LL);
    printk("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_MN>>16)&0x0000FFFF));
    printk("%x",(WORD)(VIFInitialIn_inst.VifCrRate_MN&0x0000FFFF));
    printk("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_MN);
    printk("\r\n------------End of VIF lib para-----------",0);
*/

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

void MDRV_VIF_Reset(void)
{
    msVifInitial();
}

void MDRV_VIF_Handler(BOOL bAutoScan)
{
    msVifHandler(bAutoScan);
}

void MDRV_VIF_SetSoundSystem(VIFSoundSystem ucSoundSystem)
{
    DRVVIFDBG(printk("MDRV_VIF_SetSoundSystem ucSoundSystem=%d\n",ucSoundSystem));
    msVifSetSoundSystem(ucSoundSystem);
}

void MDRV_VIF_SetIfFreq(IfFrequencyType u16IfFreq)
{
    DRVVIFDBG(printk("DRV_VIF_SetIfFreq() u16IfFreq=%d\n",u16IfFreq));
    msVifSetIfFreq(u16IfFreq);
}

void MDRV_VIF_BypassDBBAudioFilter(BOOL bEnable)
{
    DRVVIFDBG(printk("DRV_VIF_BypassDBBAudioFilter() bEnableq=%d\n",bEnable));
    MHAL_VIF_BypassDBBAudioFilter(bEnable);
}

void MDRV_VIF_SetFreqBand(FrequencyBand u8FreqBand)
{
    DRVVIFDBG(printk("DRV_VIF_SetFreqBand() u8FreqBand=%d\n",u8FreqBand));
    VIFInitialIn_inst.VifFreqBand=u8FreqBand;
    g_FreqBand = u8FreqBand;
}

BOOL MDRV_VIF_GetInputLevelIndicator(void)
{
    DRVVIFDBG(printk("DRV_VIF_GetInputLevelIndicator() \n"));
    return MHAL_VIF_GetInputLevelIndicator();
}

void MDRV_VIF_CopyFilter(BYTE i)
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
    }
}

BOOL MDRV_VIF_SetParameter(VIF_PARA_GROUP paraGroup, void * pVIF_Para, DWORD u32DataLen)
{
    DRVVIFDBG(printk("DRV_VIF_SetParameter() \n"));

    switch (paraGroup)
    {
        case VIF_PARA_USER_FILTER:
            if ( sizeof(VIFUserFilter) == u32DataLen)
            {
                memcpy(&sVIFUserFilter, (VIFUserFilter *) pVIF_Para, u32DataLen);

                // check if need to update user define peaking filter
                if (sVIFUserFilter.VifUserPeakingFilterSelect>=PK_START &&
                    sVIFUserFilter.VifUserPeakingFilterSelect<=PK_END)
                {
                    MDRV_VIF_CopyFilter(1);

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
                if (sVIFUserFilter.VifUserYcDelayFilterSelect>=YC_START &&
                    sVIFUserFilter.VifUserYcDelayFilterSelect<=YC_END)
                {
                    MDRV_VIF_CopyFilter(2);

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
                if (sVIFUserFilter.VifUserGroupDelayFilterSelect>=GP_START &&
                    sVIFUserFilter.VifUserGroupDelayFilterSelect<=GP_END)
                {
                    MDRV_VIF_CopyFilter(3);

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
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_USER_FILTER");
            break;

        case VIF_PARA_TOP:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                VIFInitialIn_inst.VifTop = (U8) dwVIFPara;
                msVifTopAdjust();
            }
            else
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_TOP");
            break;

        case VIF_PARA_LEAKY_SEL:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                VIFInitialIn_inst.VifCrLockLeakySel = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                MDRV_VIF_WriteBit(LOCK_LEAKY_FF_SEL, VIFInitialIn_inst.VifCrLockLeakySel, _BIT0);
            }
            else
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_LEAKY_SEL");
            break;

        case VIF_PARA_K_SEL:
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL K_select;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                K_select = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                MDRV_VIF_WriteBit(CR_K_SEL, K_select, _BIT6);
            }
            else
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_K_SEL");
            break;
#if 0
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
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_VERSION");
            break;
#endif
        case VIF_PARA_SET_BYPASS_PEAKING_SOS21:
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL bBypassSOS21;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                bBypassSOS21 = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                MDRV_VIF_WriteBit(BYPASS_SOS21, bBypassSOS21, _BIT2);
            }
            else
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_BYPASS_FILTER");
            break;

        case VIF_PARA_GET_PGA2_LEVEL:
            if ( sizeof(BYTE) == u32DataLen)
            {
                BYTE u8Pga2;
                u8Pga2= MDRV_VIF_ReadByte(AGC_PGA2C) & 0x1F;
                memcpy(pVIF_Para, &u8Pga2, u32DataLen);
            }
            else
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_PGA2_LEVEL");
            break;

	case VIF_PARA_GET_IF:
            if ( sizeof(WORD) == u32DataLen)
		{
		 U16 u16FreqType = g_FreqType;
		 memcpy(pVIF_Para, &u16FreqType, u32DataLen);
		}
		else
                 printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_IF");
		break;

	 case VIF_PARA_GET_K_SEL:
             if ( sizeof(BYTE) == u32DataLen)
            {
                 U8 K_select;

                 if ((MDRV_VIF_ReadByte(CR_K_SEL) & _BIT6)!=0)
		       K_select=1;
                 else
		       K_select=0;

                 memcpy(pVIF_Para, &K_select,1);
             }
             else
                 printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_K_SEL");
             break;

        case VIF_PARA_SET_HW_KPKI:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                MDRV_VIF_WriteByte(CR_KP1_HW, (U8) dwVIFPara);
            }
            else
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_HW_KPKI");
            break;

        case VIF_PARA_SET_TAGC_ODMODE:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                if ((U8) dwVIFPara==0)
                {
                    MDRV_VIF_WriteBit(TAGC_ODMODE,  FALSE,  _BIT1);
                    MDRV_VIF_WriteBit(TAGC_ODMODE,  FALSE,  _BIT5);
                }
                else if ((U8) dwVIFPara==1) // RF AGC OD MODE
                {
                    MDRV_VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT1);
                }
                else if ((U8) dwVIFPara==2) // IF AGC OD MODE
                {
                    MDRV_VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT5);
                }
                else if ((U8) dwVIFPara==3)
                {
                    MDRV_VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT1);
                    MDRV_VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT5);
                }
            }
            else
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_TAGC_ODMODE");
            break;

		case VIF_PARA_SET_NOTCH_A1A2:
            if ( sizeof(VIFNotchA1A2) == u32DataLen)
            {
                memcpy(&sVIFNotchA1A2, (VIFNotchA1A2 *) pVIF_Para, u32DataLen);

                MDRV_VIF_WriteByte(N_A1_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C0 & 0x00FF));
                MDRV_VIF_WriteByte(N_A1_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C0>>8) & 0x00FF));
                MDRV_VIF_WriteByte(N_A1_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C1 & 0x00FF));
                MDRV_VIF_WriteByte(N_A1_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C1>>8) & 0x00FF));
                MDRV_VIF_WriteByte(N_A1_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C2 & 0x00FF));
                MDRV_VIF_WriteByte(N_A1_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C2>>8) & 0x00FF));
                MDRV_VIF_WriteByte(N_A2_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C0 & 0x00FF));
                MDRV_VIF_WriteByte(N_A2_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C0>>8) & 0x00FF));
                MDRV_VIF_WriteByte(N_A2_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C1 & 0x00FF));
                MDRV_VIF_WriteByte(N_A2_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C1>>8) & 0x00FF));
                MDRV_VIF_WriteByte(N_A2_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C2 & 0x00FF));
                MDRV_VIF_WriteByte(N_A2_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C2>>8) & 0x00FF));
            }
            else
                printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_NOTCH_A1A2");
	     break;

	    case VIF_PARA_SET_SOS_1112:
		    if( sizeof(VIFSOS1112) == u32DataLen)
		    {
		   memcpy(&sVIFSOS1112, (VIFSOS1112 *) pVIF_Para, u32DataLen);

		   MDRV_VIF_WriteByte(SOS11_C0_L, (U8) (sVIFSOS1112.Vif_SOS_11_C0 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS11_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C0>>8) & 0x00FF));
		   MDRV_VIF_WriteByte(SOS11_C1_L, (U8) (sVIFSOS1112.Vif_SOS_11_C1 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS11_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C1>>8) & 0x00FF));
		   MDRV_VIF_WriteByte(SOS11_C2_L, (U8) (sVIFSOS1112.Vif_SOS_11_C2 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS11_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C2>>8) & 0x00FF));
		   MDRV_VIF_WriteByte(SOS11_C3_L, (U8) (sVIFSOS1112.Vif_SOS_11_C3 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS11_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C3>>8) & 0x00FF));
		   MDRV_VIF_WriteByte(SOS11_C4_L, (U8) (sVIFSOS1112.Vif_SOS_11_C4 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS11_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C4>>8) & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C0_L, (U8) (sVIFSOS1112.Vif_SOS_12_C0 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C0>>8) & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C1_L, (U8) (sVIFSOS1112.Vif_SOS_12_C1 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C1>>8) & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C2_L, (U8) (sVIFSOS1112.Vif_SOS_12_C2 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C2>>8) & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C3_L, (U8) (sVIFSOS1112.Vif_SOS_12_C3 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C3>>8) & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C4_L, (U8) (sVIFSOS1112.Vif_SOS_12_C4 & 0x00FF));
		   MDRV_VIF_WriteByte(SOS12_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C4>>8) & 0x00FF));
		}
		    else
		        printk("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_SOS_1112");
		    break;

        default:
            break;
    }
    return TRUE;
}

void MDRV_VIF_ShiftClk(BYTE VifShiftClk)
{
    DRVVIFDBG(printk("DRV_VIF_ShiftClk() VifShiftClk=%d\n",VifShiftClk));

    msVifShiftClk(VifShiftClk);
}

#endif //_MDRV_VIF_C_
