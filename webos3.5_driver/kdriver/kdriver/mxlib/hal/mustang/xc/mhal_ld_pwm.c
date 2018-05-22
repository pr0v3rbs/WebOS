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
////////////////////////////////////////////////////////////////////////////////
#ifndef _MHAL_LD_PWM_C_
#define _MHAL_LD_PWM_C_

////////////////////////////////////////////////////////////////////////////////
/// @file mhal_PWM.c
/// @author MStar Semiconductor Inc.
/// @brief Pulse Width Modulation driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "MsTypes.h"
#include "regPWM.h"
#include "mhal_ld_pwm.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "xc_hwreg_utility2.h"

#if 0
MS_U32 _gMIO_MapBase = 0;
MS_U32 _gMIO_PM_MapBase = 0;
MS_U16 _gPWM_Status = 0;

////////////////////////////////////////////////////////////////////////////////
// Define & data type
///////////////////////////////////////////////////////////////////////////////
#define WRITE_WORD_MASK(_reg, _val, _mask)  { (*((volatile MS_U16*)(_reg))) = ((*((volatile MS_U16*)(_reg))) & ~(_mask)) | ((MS_U16)(_val) & (_mask)); }

#define HAL_PWM_ReadByte(addr)		     READ_BYTE((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2))
#define HAL_PWM_Read2Byte(addr)              READ_WORD((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2))

#define HAL_PWM_WriteByte(addr, val) 	     WRITE_BYTE((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2), (val))
#define HAL_PWM_Write2Byte(addr, val)        WRITE_WORD((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2), (val))
#define HAL_PWM_WriteRegBit(addr, val, mask) WRITE_WORD_MASK((_gMIO_MapBase + REG_PWM_BASE) + ((addr)<<2), (val), (mask))

#define HAL_TOP_ReadByte(addr)		     READ_BYTE((_gMIO_MapBase + REG_TOP_BASE) + ((addr)<<2))
#define HAL_TOP_Read2Byte(addr)              READ_WORD((_gMIO_MapBase + REG_TOP_BASE) + ((addr)<<2))
#define HAL_TOP_WriteRegBit(addr, val, mask) WRITE_WORD_MASK((_gMIO_MapBase + REG_TOP_BASE) + ((addr)<<2), (val), (mask))

#define HAL_PM_WriteByte(addr, val) 	     WRITE_BYTE((_gMIO_PM_MapBase + REG_PM_BASE) + ((addr)<<2), (val))
#define HAL_PM_Write2Byte(addr, val)        WRITE_WORD((_gMIO_PM_MapBase + REG_PM_BASE) + ((addr)<<2), (val))
#define HAL_PM_WriteRegBit(addr, val, mask) WRITE_WORD_MASK((_gMIO_PM_MapBase + REG_PM_BASE) + ((addr)<<2), (val), (mask))
#else
#define HAL_PWM_ReadByte(addr)		     MDrv_ReadByte(REG_PWM_BASE + addr)
#define HAL_PWM_Read2Byte(addr)              MDrv_Read2Byte(REG_PWM_BASE + addr)

#define HAL_PWM_WriteByte(addr, val) 	     MDrv_WriteByte(REG_PWM_BASE + addr, val)
#define HAL_PWM_Write2Byte(addr, val)        MDrv_Write2Byte(REG_PWM_BASE + addr, val)
#define HAL_PWM_WriteRegBit(addr, val, mask) MDrv_Write2ByteMask(REG_PWM_BASE + addr, val, mask)

#define HAL_TOP_ReadByte(addr)		     MDrv_ReadByte(REG_TOP_BASE + addr)
#define HAL_TOP_Read2Byte(addr)              MDrv_Read2Byte(REG_TOP_BASE + addr)
#define HAL_TOP_WriteRegBit(addr, val, mask) MDrv_Write2ByteMask(REG_TOP_BASE + addr, val, mask)

#endif

#define HAL_SUBBANK0    //HAL_PWM_WriteByte(0,0)
#define HAL_SUBBANK1    //HAL_PWM_WriteByte(0,1) /* PWM sub-bank */

MS_U8  g_u8Polarity = 0;  //[0]:PWM0 Polarity, [1]:PWM1 Polarity...
MS_U16 g_u16PeriodExt = 0;
MS_U16 g_u16DutyExt = 0;

#define REG_PWM_02_L					0x02
#define REG_PWM_03_L					0x03
#define REG_PWM_04_L					0x04
#define REG_PWM_14_L					0x14
#define REG_PWM_20_L					0x20
#define REG_PWM_21_L					0x21
#define REG_PWM_22_L					0x22
#define REG_PWM_28_L					0x28
#define REG_PWM_29_L					0x29
#define REG_PWM_34_L					0x34
#define REG_PWM_35_L					0x35
#define REG_PWM_78_L					0x78

void HAL_PWM_Write2ByteMask(MS_U32 u32Addr, MS_U16 u16Val, MS_U16 u16Mask)
{
	MS_U16 u16Target = HAL_TOP_Read2Byte(u32Addr) & ~u16Mask;
	u16Target |= (u16Val & u16Mask);
	HAL_PWM_Write2Byte(u32Addr, u16Target);
}

/*  BK25_04 [8]:reg_pwm0_polarity
    BK25_07 [8]:reg_pwm1_polarity
    ...
    BK25_13 [8]:reg_pwm5_polarity  */

/*  BK25_04 [14]:reg_pwm0_vdben_sw
    BK25_07 [14]:reg_pwm1_vdben_sw
    ...
    BK25_13 [14]:reg_pwm5_vdben_sw  */
void MHal_PWM_SWVdbEn(DRV_PWM_CH u8PWMNo, MS_BOOL bSWVdbEn)
{
    MS_U8 u8PolarityStatus;
    u8PolarityStatus = (g_u8Polarity >> u8PWMNo) & 0x01;
    HAL_PWM_Write2ByteMask((REG_PWM_04_L + u8PWMNo*3), (bSWVdbEn << 6 | u8PolarityStatus) << 8, BIT(14)|BIT(8));
}

/*  BK25_04 [9]:reg_pwm0_vdben     BK25_04 [11]:reg_pwm0_dben
    BK25_07 [9]:reg_pwm0_vdben     BK25_07 [11]:reg_pwm0_dben
    ...
    BK25_13 [9]:reg_pwm0_vdben     BK25_13 [11]:reg_pwm0_dben*/
void MHal_PWM_HWDoubleBuffer(DRV_PWM_CH u8PWMNo, MS_U8 u8DoubleBufferSelect)
{
    MS_BOOL bVSyncDoubleBufferEn, bDoubleBufferEn;
    MS_U8 u8PolarityStatus;

    u8PolarityStatus = (g_u8Polarity >> u8PWMNo) & 0x01;

    switch(u8DoubleBufferSelect)
    {
        case No_DoubleBF:
            bVSyncDoubleBufferEn = 0;
            bDoubleBufferEn = 0;
        break;

        case DoubleBF:
            bVSyncDoubleBufferEn = 0;
            bDoubleBufferEn = 1;
        break;

        default:
        case Vsync_DoubleBF:
            bVSyncDoubleBufferEn = 1;
            bDoubleBufferEn = 0;
        break;
    }

    HAL_PWM_Write2ByteMask((REG_PWM_04_L + u8PWMNo*3), ((bDoubleBufferEn << 3) | (bVSyncDoubleBufferEn << 1)|(u8PolarityStatus)) << 8, BIT(11)|BIT(9)|BIT(8)); //HSyncResetCnt
}



/*  BK25_14 [15]:reg_rst_mux0   [0]:VSync Reset [1]:HSync Reset
    BK25_14 [7]:reg_rst_mux1
    ...
    BK25_16 [7]:reg_rst_mux5*/
void MHal_PWM_ResetMuxSel(DRV_PWM_CH u8PWMNo, MS_BOOL bMuxSel)
{
    if (u8PWMNo%2)
        HAL_PWM_Write2ByteMask(REG_PWM_14_L + (u8PWMNo >> 1), bMuxSel? BIT(15): 0, BIT(15));
    else
        HAL_PWM_Write2ByteMask(REG_PWM_14_L + (u8PWMNo >> 1), bMuxSel? BIT(7): 0, BIT(7));
}

void MHal_PWM_NVS_RST_EN(DRV_PWM_CH u8PWMNo, MS_BOOL bEn)
{
    HAL_PWM_Write2ByteMask(REG_PWM_34_L, bEn?BIT(u8PWMNo): 0, 0x01<<u8PWMNo);
}

void MHal_PWM_NVS_align_en(DRV_PWM_CH u8PWMNo, MS_BOOL bEn)
{
    HAL_PWM_Write2ByteMask(REG_PWM_35_L, bEn?BIT(u8PWMNo): 0, 0x01<<u8PWMNo);
}

void MHal_PWM_Set_LR_flag_rising_pulse(DRV_PWM_CH u8PWMNo, MS_BOOL bEn)
{
    HAL_PWM_Write2ByteMask(REG_PWM_78_L, bEn?BIT(u8PWMNo): 0, 0x01<<u8PWMNo); // reg_pwm0_lr_sync_rp = 1
}


/*  BK25_14 [11:8]:reg_hs_rst_cnt0
    BK25_14 [3:0]:reg_hs_rst_cnt1
    ...
    BK25_16 [3:0]:reg_hs_rst_cnt5*/
void MHal_PWM_HsyncResetCnt(DRV_PWM_CH u8PWMNo, MS_U8 u8HsyncResetCnt)
{
    if (u8PWMNo%2) // 1,3,5
    {
        //MDrv_WriteRegBit(REG_2528 + u8PWMNo-1, u8HsyncResetCnt, 0x0F);
        HAL_PWM_Write2ByteMask(REG_PWM_14_L + (u8PWMNo >> 1), (MS_U16)u8HsyncResetCnt, 0x0F);
    }
    else // 0 ,2 ,4
    {
        //MDrv_WriteRegBit(REG_2529 + u8PWMNo, u8HsyncResetCnt, 0x0F);
        HAL_PWM_Write2ByteMask(REG_PWM_14_L + (u8PWMNo >> 1), (MS_U16)u8HsyncResetCnt << 8, 0x0F00);
    }
}

/*  BK25_02 [15:0]:reg_pwm0_period
    BK25_05 [15:0]:reg_pwm1_period
    ...
    BK25_11 [15:0]:reg_pwm5_period  */
void MHal_PWM_SetPeriod(DRV_PWM_CH u8PWMNo, MS_U32 u32Period)
{
    HAL_PWM_Write2ByteMask(REG_PWM_02_L + u8PWMNo*3, (MS_U16)u32Period, 0xFF);
    g_u16PeriodExt &= ~(0x03 << (u8PWMNo*2));   //Erase
    g_u16PeriodExt |= ((u32Period >> 16) & 0x03) << (u8PWMNo*2);  //Overwrite

    HAL_PWM_Write2ByteMask(REG_PWM_20_L, g_u16PeriodExt, 0x0FFF);
}

/*  BK25_03 [15:0]:reg_pwm0_duty
    BK25_06 [15:0]:reg_pwm1_duty
    ...
    BK25_12 [15:0]:reg_pwm5_duty  */
void MHal_PWM_SetDuty(DRV_PWM_CH u8PWMNo, MS_U32 u32Duty)
{
    HAL_PWM_Write2ByteMask(REG_PWM_03_L + u8PWMNo*3, (MS_U16)u32Duty, 0xFF);
    g_u16DutyExt &= ~(0x03 << (u8PWMNo*2)); //Erase
    g_u16DutyExt |= ((u32Duty >> 16) & 0x03) << (u8PWMNo*2);  //Overwrite

    HAL_PWM_Write2ByteMask(REG_PWM_21_L, g_u16DutyExt, 0x0FFF);
}

/*  BK25_29 [1:0]|BK25_28 [15:0]:reg_pwm0_shift
    BK25_2B [1:0]|BK25_2A [15:0]:reg_pwm1_shift
    ...
    BK25_33 [1:0]|BK25_32 [15:0]:reg_pwm5_shift */
void MHal_PWM_SetShift(DRV_PWM_CH u8PWMNo, MS_U32 u32Shift)
{
    if(u32Shift==0)
       u32Shift=1;  //bug, when shift=0, we have to set shift=1 for correct waveform

    HAL_PWM_Write2ByteMask(REG_PWM_28_L + u8PWMNo*2, (MS_U16)u32Shift, 0xFFFF);
    HAL_PWM_Write2ByteMask(REG_PWM_29_L + u8PWMNo*2, (u32Shift >> 16), 0x03);
}

/*  BK25_22 [7:0]reg_pwm0_div_ext   BK25_04 [7:0]:reg_pwm0_div
    BK25_22 [15:8]reg_pwm0_div_ext  BK25_07 [7:0]:reg_pwm0_div
    ...
    BK25_24 [15:8]reg_pwm0_div_ext  BK25_13 [7:0]:reg_pwm0_div  */
void MHal_PWM_SetDivider(DRV_PWM_CH u8PWMNo, MS_U16 u16Divider)
{
	HAL_PWM_Write2ByteMask(REG_PWM_04_L + u8PWMNo*3, u16Divider, 0xFF);
	if(u8PWMNo % 2)
	{
	    HAL_PWM_Write2ByteMask(REG_PWM_22_L + (u8PWMNo >> 1), u16Divider, 0xFF00);
	}
	else
	{
	    HAL_PWM_Write2ByteMask(REG_PWM_22_L + (u8PWMNo >> 1), u16Divider >> 8, 0xFF);
	}
}

/*  Because polarity bit is in the same byte with SWVdbEn,
    and our 8051MCU access one byte at a time
    so set once together with SWVdbEn*/
void MHal_PWM_SetPolarity(DRV_PWM_CH u8PWMNo, MS_BOOL bPolarity)
{
    g_u8Polarity &= ~( (0x01) << u8PWMNo);
    g_u8Polarity |= (0x01 & bPolarity) << u8PWMNo;

    HAL_PWM_Write2ByteMask((REG_PWM_04_L + u8PWMNo*3), bPolarity? BIT(8): 0, BIT(8));
}

/*  BK25_04 [10]:reg_pwm0_reset_en
    BK25_07 [10]:reg_pwm1_reset_en
    ...
    BK25_13 [10]:reg_pwm5_reset_en*/
void MHal_PWM_WaveformResetEnable(DRV_PWM_CH u8PWMNo, MS_BOOL bResetEn)
{
    MS_U8 u8ResetEnVal = (bResetEn?BIT(2):0);
    MS_U8 u8PolarityStatus;


    u8PolarityStatus = (g_u8Polarity >> u8PWMNo) & 0x01;

    HAL_PWM_Write2ByteMask((REG_PWM_04_L + u8PWMNo*3), (u8ResetEnVal|u8PolarityStatus) << 8, BIT(10)|BIT(8));
}

#define REG_1E3F 			0x101E3F
#define REG_1E40 			0x101E40
#define REG_1E9B 			0x101E9B

void MHal_PWM_OutputEn(DRV_PWM_CH u8PWMNo, MS_BOOL bEnable)
{
    if( u8PWMNo >= PWM_NUMS )
    {
        return;
    }

    if (bEnable)
        MDrv_Write2ByteMask(REG_1E40, 0x0000, 0x01<<u8PWMNo);
    else
        MDrv_Write2ByteMask(REG_1E40, 0xFFFF, 0x01<<u8PWMNo);
}

void MDrv_PWM_Set_PadEn( DRV_PWM_CH u8PWMNum, MS_BOOL bEn )
{
    switch(u8PWMNum)
    {
        case PWM_CH0:
            MDrv_WriteRegBit(REG_1E3F, !bEn, BIT(6)); // reg_pwm0_use_as_gpio
            break;

        case PWM_CH1:
            MDrv_WriteRegBit(REG_1E3F, !bEn, BIT(7)); // reg_pwm1_use_as_gpio
            break;

        case PWM_CH5:
            MDrv_WriteRegBit(REG_1E9B, bEn, BIT(6)); // reg_vsync_like_from_pwm
            break;

        default:
            break;
    }
}

#endif // _MHAL_LD_PWM_C_
