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
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
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

#define HAL_SAR_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsTypes.h"

// Internal Definition
#include "regSAR.h"
#include "halSAR.h"

#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
#include "halSAR_DVFS.h"
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define BIT0    BIT(0)
#define BIT1    BIT(1)
#define BIT2    BIT(2)
#define BIT3    BIT(3)
#define BIT4    BIT(4)
#define BIT5    BIT(5)
#define BIT6    BIT(6)
#define BIT7    BIT(7)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT _gMIO_MapBase = 0;
static MS_VIRT _gMIO_MapBase_NPM = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define HAL_SAR_ERR(x, args...)        //{printf(x, ##args);}
#define HAL_SAR_INFO(x, args...)        //{printf(x, ##args);}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U8 HAL_SAR_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Read2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U16 HAL_SAR_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[(u32RegAddr)];
}

///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByteMask
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8ValIn, MS_U8 u8Msk)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte(u32RegAddr);
    u8Val = (u8Val & ~(u8Msk)) | ((u8ValIn) & (u8Msk));
    HAL_SAR_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_ReadRegBit
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_ReadRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte(u32RegAddr);
    return (u8Val & u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_ReadByte_NPM
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U8 HAL_SAR_ReadByte_NPM(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase_NPM))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByte_NPM
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByte_NPM(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase_NPM))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteByteMask_NPM
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteByteMask_NPM(MS_U32 u32RegAddr, MS_U8 u8ValIn, MS_U8 u8Msk)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte_NPM(u32RegAddr);
    u8Val = (u8Val & ~(u8Msk)) | ((u8ValIn) & (u8Msk));
    HAL_SAR_WriteByte_NPM(u32RegAddr, u8Val);
    return TRUE;
}

#if 0//Temporarily marked out for compiler warning free
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Read2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U16 HAL_SAR_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U32 HAL_SAR_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_SAR_Read2Byte(u32RegAddr) | HAL_SAR_Read2Byte(u32RegAddr+2) << 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_SAR_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_SAR_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_WriteRegBit
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_SAR_WriteRegBit(MS_U32 u32RegAddr, MS_BOOL bEnable, MS_U8 u8Mask)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        HAL_SAR_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_SAR_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_SAR_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetIOMapBase
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b u32Base : io map base address
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SetIOMapBase(MS_VIRT virtBase)
{
    _gMIO_MapBase = virtBase;
    HAL_SAR_INFO("SAR IOMap base:%8lx Reg offset:%4x\n", virtBase, SAR_REG_BASE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetIOMapBase
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32 : io map base address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_SAR_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgChannelBound
/// @brief \b Function  \b Description: Configure sar channel upper/lower bound
/// @param <IN>         \b u8Channel : sar channel (0~7), psarBndCfg: sar bound info
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_CfgChannelBound(MS_U8 u8Channel ,HAL_SAR_BndCfg *psarBndCfg)
{
    MS_U16  wChannelAdcValue = 0;

    if(u8Channel >= HAL_SAR_CH_MAXID)
        return FALSE;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH1_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH1_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH1_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH1_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH1_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH1_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH2:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH2_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH2_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH2_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH2_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH2_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH2_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH3:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH3_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH3_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH3_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH3_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH3_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH3_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH4:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH4_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH4_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH4_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH4_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH4_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH4_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH5:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH5_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH5_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH5_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH5_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH5_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH5_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH6:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH6_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH6_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH6_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH6_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH6_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH6_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH7:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH7_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH7_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH7_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH7_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH7_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH7_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

        case HAL_SAR_CH8:
        {
            wChannelAdcValue = psarBndCfg->u8UpBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH8_UPD + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH8_UPD, (MS_U8) wChannelAdcValue, _SAR_CHN_UPB_MSK);

            wChannelAdcValue = psarBndCfg->u8LoBnd;
            wChannelAdcValue <<= 2;
            wChannelAdcValue |= 0x3;
            HAL_SAR_WriteByteMask((REG_SAR_CH8_LOB + 1), (MS_U8) (wChannelAdcValue >> 8), 0x03);
            HAL_SAR_WriteByteMask(REG_SAR_CH8_LOB, (MS_U8) wChannelAdcValue, _SAR_CHN_LOB_MSK);

            //HAL_SAR_WriteByteMask(REG_SAR_CH8_UPD,psarBndCfg->u8UpBnd,_SAR_CHN_UPB_MSK);
            //HAL_SAR_WriteByteMask(REG_SAR_CH8_LOB,psarBndCfg->u8LoBnd,_SAR_CHN_LOB_MSK);
        }
        break;

    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetChannelADC
/// @brief \b Function  \b Description: Get sar channel ADC value
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8: sar ADC value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_SAR_GetChannelADC(MS_U8 u8Channel)
{
    MS_U32  u32Reg = REG_SAR_ADC_CH1_DATA;
    MS_U16  wChannelAdcValue = 0;

    if(u8Channel >= HAL_SAR_CH_MAXID)
        return HAL_SAR_ADC_DATA_MAX;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u32Reg = REG_SAR_ADC_CH1_DATA;
            break;
        case HAL_SAR_CH2:
            u32Reg = REG_SAR_ADC_CH2_DATA;
            break;
        case HAL_SAR_CH3:
            u32Reg = REG_SAR_ADC_CH3_DATA;
            break;
        case HAL_SAR_CH4:
            u32Reg = REG_SAR_ADC_CH4_DATA;
            break;
        case HAL_SAR_CH5:
            u32Reg = REG_SAR_ADC_CH5_DATA;
            break;
        case HAL_SAR_CH6:
            u32Reg = REG_SAR_ADC_CH6_DATA;
            break;
        case HAL_SAR_CH7:
            u32Reg = REG_SAR_ADC_CH7_DATA;
            break;
        case HAL_SAR_CH8:
            u32Reg = REG_SAR_ADC_CH8_DATA;
            break;
    }

    HAL_SAR_WriteByte(REG_SAR_CTRL1 ,HAL_SAR_ReadByte(REG_SAR_CTRL1)|_SAR_LOAD_EN);
    wChannelAdcValue = (HAL_SAR_Read2Byte(u32Reg) & _SAR_ADC_OUT_10BITMSK);
    wChannelAdcValue = ((wChannelAdcValue >> 2) & _SAR_ADC_OUT_8BITMSK);
    /* because of interface can't be change , we return [9:2] to drv layer */

#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
    SysDvfsProc();
#endif

    return (MS_U8) wChannelAdcValue;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgSingleChannel
/// @brief \b Function  \b Description: Configure channel for single channel mode
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgSingleChannel(MS_U8 u8Channel)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, u8Channel, _SAR_SINGLE_CH_MSK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgTriggerMode
/// @brief \b Function  \b Description: Configure keypad level(trigger) mode
/// @param <IN>         \b bMode : 0: edge trigger mode, 1: level trigger mode
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgTriggerMode(MS_U8 bMode)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bMode)? _SAR_LEVEL_TRIGGER : 0), _SAR_LEVEL_TRIGGER);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgSingleChannelEn
/// @brief \b Function  \b Description: enable single channel mode
/// @param <IN>         \b bEnable : 0: disable, 1: enable
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgSingleChannelEn(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bEnable)? _SAR_SINGLE_CH_EN : 0), _SAR_SINGLE_CH_EN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgDigitOperMode
/// @brief \b Function  \b Description: Configure sar digital operation mode
/// @param <IN>         \b u8Mode : 0: one-shot, 1: freerun
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgDigitOperMode(MS_U8 u8Mode)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((u8Mode)? _SAR_MODE : 0), _SAR_MODE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgDigitPowerdown
/// @brief \b Function  \b Description: Configure sar digital power down
/// @param <IN>         \b bEnable : 0: power up, 1: power down
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgDigitPowerdown(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bEnable)? _SAR_PD : 0), _SAR_PD);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgStart
/// @brief \b Function  \b Description: Configure sar to trigger start signal
/// @param <IN>         \b bEnable : 0: stop, 1: trigger to start
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgStart(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL0, ((bEnable)? _SAR_START : 0), _SAR_START);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgAtopPowerdown
/// @brief \b Function  \b Description: Configure sar atop power down
/// @param <IN>         \b bEnable : 0: power up, 1: power down
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgAtopPowerdown(MS_U8 bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, ((bEnable)? _SAR_ADC_PD : 0), _SAR_ADC_PD);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgAtopFreeRun
/// @brief \b Function  \b Description: Configure sar atop freerun mode
/// @param <IN>         \b bEnable : 0: controlled by digital, 1: freerun
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgAtopFreeRun(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, ((bEnable)? _SAR_FREERUN : 0), _SAR_FREERUN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgSelection
/// @brief \b Function  \b Description: Configure sar selection
/// @param <IN>         \b bEnable : 0: disable, 1: enable
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgSelection(MS_BOOL bEnable)
{
    MS_U8 u8CtrlData,u8Mask=_SAR_SEL;

    u8CtrlData = (bEnable)? u8Mask : 0;
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgHighChannel
/// @brief \b Function  \b Description: Configure to use high sar channel ( 4~7, or 4~5)
/// @param <IN>         \b bEnable : 0: disable, 1: enable
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgHighChannel(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_CTRL1, ((bEnable)? _SAR_NCH_EN : 0), _SAR_NCH_EN);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgClockSamplePeriod
/// @brief \b Function  \b Description: Configure sample period
/// @param <IN>         \b u8ClkSmpPrd :
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgClockSamplePeriod(MS_U8 u8ClkSmpPrd)
{
    HAL_SAR_WriteByteMask(REG_SAR_CKSAMP_PRD, u8ClkSmpPrd, _SAR_CKSAMP_PRD_MSK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgAanlogInputSelect
/// @brief \b Function  \b Description: Configure Analog Input/GPIO
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bEnable : 0: GPIO, 1: Analog Input
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgAanlogInputSelect(MS_U8 u8Channel, MS_BOOL bEnable)
{
    MS_U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH1 : (~_SAR_AISEL_CH1);
            u8Mask=_SAR_AISEL_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH2 : (~_SAR_AISEL_CH2);
            u8Mask=_SAR_AISEL_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH3 : (~_SAR_AISEL_CH3);
            u8Mask=_SAR_AISEL_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH4 : (~_SAR_AISEL_CH4);
            u8Mask=_SAR_AISEL_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bEnable)? _SAR_AISEL_CH5 : (~_SAR_AISEL_CH5);
            u8Mask=_SAR_AISEL_CH5;
            break;
        default: return;
    }
    HAL_SAR_WriteByteMask(REG_SAR_AISEL, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgOutputEnable
/// @brief \b Function  \b Description: Configure output enable for sar channel set as GPIO
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bEnable: 0: output, 1: input
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgOutputEnable(MS_U8 u8Channel, MS_BOOL bEnable)
{
    MS_U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH1 : _SAR_OEN_GPIO_CH1;
            u8Mask = _SAR_OEN_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH2 : _SAR_OEN_GPIO_CH2;
            u8Mask = _SAR_OEN_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH3 : _SAR_OEN_GPIO_CH3;
            u8Mask = _SAR_OEN_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH4 : _SAR_OEN_GPIO_CH4;
            u8Mask = _SAR_OEN_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bEnable)? ~_SAR_OEN_GPIO_CH5 : _SAR_OEN_GPIO_CH5;
            u8Mask = _SAR_OEN_GPIO_CH5;
            break;
        default: return;
    }
    HAL_SAR_WriteByteMask(REG_SAR_OEN_GPIO, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetOutput
/// @brief \b Function  \b Description: Set GPIO output value(high/low) for sar channel
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bHighLow  0: low, 1: high
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SetOutput(MS_U8 u8Channel, MS_BOOL bHighLow)
{
    MS_U8 u8CtrlData,u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH1 : ~_SAR_I_GPIO_CH1;
            u8Mask = _SAR_I_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH2 : ~_SAR_I_GPIO_CH2;
            u8Mask = _SAR_I_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH3 : ~_SAR_I_GPIO_CH3;
            u8Mask = _SAR_I_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH4 : ~_SAR_I_GPIO_CH4;
            u8Mask = _SAR_I_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8CtrlData = (bHighLow)? _SAR_I_GPIO_CH5 : ~_SAR_I_GPIO_CH5;
            u8Mask = _SAR_I_GPIO_CH5;
            break;
        default: return;
    }
    HAL_SAR_WriteByteMask(REG_SAR_I_GPIO, u8CtrlData, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetInput
/// @brief \b Function  \b Description: Get GPIO input value(high/low) for sar channel
/// @param <IN>         \b u8Channel : sar channel (0~7)
/// @param <IN>         \b bHighLow  0: low, 1: high
/// @param <RET>        \b TRUE: High FALSE: Low
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_GetInput(MS_U8 u8Channel)
{
    MS_U8 u8Mask;

    switch(u8Channel)
    {
        case HAL_SAR_CH1:
            u8Mask = _SAR_C_GPIO_CH1;
            break;
        case HAL_SAR_CH2:
            u8Mask = _SAR_C_GPIO_CH2;
            break;
        case HAL_SAR_CH3:
            u8Mask = _SAR_C_GPIO_CH3;
            break;
        case HAL_SAR_CH4:
            u8Mask = _SAR_C_GPIO_CH4;
            break;
        case HAL_SAR_CH5:
            u8Mask = _SAR_C_GPIO_CH5;
            break;
        default: return FALSE;
    }
    return HAL_SAR_ReadRegBit(REG_SAR_C_GPIO, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgIntMask
/// @brief \b Function  \b Description: Interrupt mask for sar int
/// @param <IN>         \b bEnable : 0: enable int, 1: disable int
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgIntMask(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_STATUS0, ((bEnable)? _SAR_INT_MASK : 0), _SAR_INT_MASK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgIntClear
/// @brief \b Function  \b Description: Interrupt clear for sar int
/// @param <IN>         \b bEnable : 0: not clear, 1: clear
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgIntClear(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_STATUS0, ((bEnable)? _SAR_INT_CLR : 0), _SAR_INT_CLR);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CfgIntForce
/// @brief \b Function  \b Description: Force interrupt for sar int
/// @param <IN>         \b bEnable : 0: not force, 1: force
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CfgIntForce(MS_BOOL bEnable)
{
    HAL_SAR_WriteByteMask(REG_SAR_STATUS0, ((bEnable)? _SAR_INT_FORCE : 0), _SAR_INT_FORCE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetIntStatus
/// @brief \b Function  \b Description: get sar interrupt status
/// @param <IN>         \b None : 0: not force, 1: force
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: interrupt comes, FALSE: no interrupt
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_GetIntStatus(void)
{
    MS_U8 u8IntStatus;

    u8IntStatus = HAL_SAR_ReadByte(REG_SAR_STATUS0) & _SAR_INT_STATUS;
    return (u8IntStatus)? ENABLE : DISABLE;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetIOMapBase_NPM
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b u32Base : io map base address
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SetIOMapBase_NPM(MS_VIRT virtBase)
{
    _gMIO_MapBase_NPM = virtBase;
    HAL_SAR_INFO("SAR NPM IOMap base:%8lx Reg offset:%4x\n", u32Base);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_GetIOMapBase_NPM
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32 : io map base address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_SAR_GetIOMapBase_NPM(void)
{
    return _gMIO_MapBase_NPM;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SetAdcHSyncChannel
/// @brief \b Function  \b Description: Switch HSync signal to SAR High channel
/// @param <IN>         \b u8Channel : ADC HSync channel (0~2)
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_SetAdcHSyncChannel(MS_U8 u8Channel)
{
    MS_U8 u8HSyncCh;

    switch(u8Channel)
    {
        case HAL_SAR_ADC_HSYNC_CH0:
            u8HSyncCh = _SAR2_HSYNC_CH0;
            break;
        case HAL_SAR_ADC_HSYNC_CH1:
            u8HSyncCh = _SAR2_HSYNC_CH1;
            break;
        case HAL_SAR_ADC_HSYNC_CH2:
            u8HSyncCh = _SAR2_HSYNC_CH2;
            break;
        case HAL_SAR_ADC_HSYNC_CH3:
            u8HSyncCh = _SAR2_HSYNC_CH3;
            break;
        default: return FALSE;
    }
    return HAL_SAR_WriteByteMask_NPM(REG_ADC_ATOP_SAR2, u8HSyncCh, _SAR2_HSYNC_MSK);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Interrupt_EN
/// @brief \b Function  \b Description: Switch SAR Interrupt Enable/Disable
/// @param <IN>         \b u8Channel : ADC HSync channel (0~3)
/// @param <IN>         \b bEnablel : True: enable interrupt; False: disable interrupt
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_Interrupt_EN(MS_U8 u8Channel, MS_BOOL bEnable)
{
    return HAL_SAR_WriteByteMask(REG_SAR_INT, ((bEnable)? 1<<u8Channel : 0), 1<<u8Channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_Wakeup_EN
/// @brief \b Function  \b Description: Switch SAR Interrupt  Wakeup Enable/Disable
/// @param <IN>         \b bEnablel : True: enable interrrupt wakeup; False: disable interrupt wakeup
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_Wakeup_EN(MS_BOOL bEnable)
{
    return HAL_SAR_WriteByteMask(REG_WK_IRQ, ((bEnable)? _W_SAR : 0), _W_SAR);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_CLR_INT
/// @brief \b Function  \b Description: Switch SAR Clear Interrupt Status
/// @param <IN>         \b u8Channel : ADC HSync channel (0~3)
/// @param <OUT>        \b None :
/// @param <RET>        \b  None
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_CLR_INT(MS_U8 u8Channel)
{
    HAL_SAR_WriteByteMask(REG_SAR_CLR_INT, 1<<u8Channel, 1<<u8Channel);
    HAL_SAR_WriteByteMask(REG_SAR_CLR_INT, 0, 1<<u8Channel);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_INT_Status
/// @brief \b Function  \b Description: get sar interrupt status
/// @param <IN>         \b u8Channel : ADC HSync channel (0~3)
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: interrupt comes, FALSE: no interrupt
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SAR_INT_Status(MS_U8 u8Channel)
{
    return (HAL_SAR_ReadByte(REG_SAR_INT_STATUS) & (1<<u8Channel)) > 0? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SAR_SET_LEVEL
/// @brief \b Function  \b Description: set sar level is 2.0V or 3.3V
/// @param <IN>         \b bLevel : 1:3.3V , 0:2.0V
/// @param <OUT>        \b None :
/// @param <RET>        \b Nonw:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_SAR_SET_LEVEL(MS_BOOL bLevel)
{
    HAL_SAR_WriteByteMask(REG_SAR_TEST0, (bLevel? 1<<6:0) , 1<<6);
}

//=================================================================================================
#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
static MS_S32 _s32SAR_Dvfs_Mutex;
static MSTAR_DVFS_INFO hMstarDvfsInfo =
{
    .bDvfsInitOk = 0,
    .dwVidSetting = 0,
};

#if (CONFIG_DVFS_CPU_POWER_I2C_ENABLE | CONFIG_DVFS_CORE_POWER_I2C_ENABLE)
extern MS_BOOL MApi_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
extern MS_BOOL MApi_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsProc
/// @brief \b Function  \b Description: Read T-Sensor to Handle DVFS Flow
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////

static pthread_t   hDvfsThread;
static void *SysDvfsThreadProc(void *pThreadInfo)
{
    int                     hFileHandle;
    int                     dwDataLength = 0;
    char                    sFileName[48] = "/proc/on_demand_ctl\0";
    MSTAR_DVFS_READ_INFO    hMstarDvfsReadInfo;
    MSTAR_DVFS_WRITE_INFO   hMstarDvfsWriteInfo;

    hFileHandle = open(sFileName, (O_RDWR | O_EXCL));
    if(hFileHandle == -1)
    {
        DVFS_DEBUG("\033[35m[ERROR] Cannot Open File: %s\033[m\n", sFileName);
    }
    else
    {
        while(1)
        {
            dwDataLength = read(hFileHandle, (void *) &hMstarDvfsReadInfo, sizeof(hMstarDvfsReadInfo));
            DVFS_DEBUG("\033[35m[INFO] Read Data Length: %d\033[m\n", dwDataLength);
            DVFS_DEBUG("\033[35m[INFO] CPU Clock: %d\033[m\n", hMstarDvfsReadInfo.dwCpuClock);
            DVFS_DEBUG("\033[35m[INFO] Voltage: %d\033[m\n", hMstarDvfsReadInfo.dwVoltage);
            DVFS_DEBUG("\033[35m[INFO] Voltage Type: %d\033[m\n", hMstarDvfsReadInfo.dwVoltageType);
            DVFS_DEBUG("\033[35m[INFO] Data Exchange Count (Read): %d\033[m\n", hMstarDvfsReadInfo.dwDataExchangeCount);

            if(hMstarDvfsReadInfo.dwVoltageType == CONFIG_DVFS_CPU_POWER)
            {
                SysDvfsCpuPowerAdjustment(hMstarDvfsReadInfo.dwVoltage);
                sleep(CONFIG_DVFS_DELAY_US);
            }
            else if(hMstarDvfsReadInfo.dwVoltageType == CONFIG_DVFS_CORE_POWER)
            {
                SysDvfsCorePowerAdjustment(hMstarDvfsReadInfo.dwVoltage);
                sleep(CONFIG_DVFS_DELAY_US);
            }

            hMstarDvfsWriteInfo.dwDataExchangeCount = hMstarDvfsReadInfo.dwDataExchangeCount;
            hMstarDvfsWriteInfo.dwDataExchangeResult = 0;
            dwDataLength = write(hFileHandle, (void *)&hMstarDvfsWriteInfo, sizeof(hMstarDvfsWriteInfo));
            DVFS_DEBUG("\033[35m[INFO] Write Data Length: %d\033[m\n", dwDataLength);
            DVFS_DEBUG("\033[35m[INFO] Data Exchange Count (Write): %d\033[m\n", hMstarDvfsWriteInfo.dwDataExchangeCount);
            DVFS_DEBUG("\033[35m[INFO] Data Exchange Result: %d\033[m\n", hMstarDvfsWriteInfo.dwDataExchangeResult);

        }

        close(hFileHandle);
    }

    return NULL;
}

void SysDvfsProc(void)
{
    if(*(volatile MS_U16 *)(_gMIO_MapBase + (0x100500 << 1)) == CONFIG_DVFS_ENABLE_PATTERN)
    {
        if(hMstarDvfsInfo.bDvfsInitOk == 0)
        {
            int dwErrorCode = 0;
            char sDvfsThreadName[20] = "MstarDvfsThread\0";
    
            _s32SAR_Dvfs_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex SAR_DVFS", MSOS_PROCESS_SHARED);
            MS_ASSERT(_s32SAR_Dvfs_Mutex >= 0);
    
            if (FALSE == MsOS_ObtainMutex(_s32SAR_Dvfs_Mutex, CONFIG_DVFS_MUTEX_WAIT_TIME))
            {
                DVFS_INFO("\033[37m[DVFS] Mutex Lock Fail\033[m\n");
                MsOS_ReleaseMutex(_s32SAR_Dvfs_Mutex);
                return;
            }
    
            SysDvfsInit();
    
            dwErrorCode = pthread_create(
                                &hDvfsThread,
                                NULL,
                                SysDvfsThreadProc,
                                (void *) sDvfsThreadName);
    
            if(dwErrorCode)
            {
                DVFS_DEBUG("\033[33m[ERROR] Create DVFS Thread Failed\033[m\n");
                MsOS_ReleaseMutex(_s32SAR_Dvfs_Mutex);
                return;
            }
    
            dwErrorCode = pthread_join(hDvfsThread, NULL);
    
            MsOS_ReleaseMutex(_s32SAR_Dvfs_Mutex);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsInit
/// @brief \b Function  \b Description: Read T-Sensor to Handle DVFS Flow
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsInit(void)
{
//    MS_U32  dwRegisterValue = 0;

    SysDvfsCpuPowerInit();
    SysDvfsCorePowerInit();

//    hMstarDvfsInfo.dwVidSetting = *(volatile MS_U16 *)(_gMIO_MapBase + (0x10050c << 1));
//
//    dwRegisterValue = (hMstarDvfsInfo.dwVidSetting & 0x00FF);
//    if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CPU_POWER_DEFAULT))
//    {
//        SysDvfsCpuPowerAdjustment(dwRegisterValue);
//    }
//    else
    {
        SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
    }

//    dwRegisterValue = ((hMstarDvfsInfo.dwVidSetting & 0xFF00) >> 8);
//    if((dwRegisterValue > 0) && (dwRegisterValue <= CONFIG_DVFS_CORE_POWER_DEFAULT))
//    {
//        SysDvfsCorePowerAdjustment(dwRegisterValue);
//    }
//    else
    {
        SysDvfsCorePowerAdjustment(CONFIG_DVFS_CORE_POWER_DEFAULT);
    }

    hMstarDvfsInfo.bDvfsInitOk = 1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCpuPowerAdjustment
/// @brief \b Function  \b Description: Update Output Voltage Level in External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuPowerAdjustment(MS_U32 dwCpuPowerVoltage)
{
#if CONFIG_DVFS_CPU_POWER_I2C_ENABLE
    MS_U32  dwRegisterValue = 0;
    MS_U32  dwPowerShiftValue = CONFIG_DVFS_CPU_POWER_SHIFT;
    MS_U32  dwCpuPowerShiftValue = 0;

    MS_U8   byTargetRegAddress[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };
    MS_U8   byTargetData[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };

    dwCpuPowerShiftValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x10050a << 1));
    dwCpuPowerShiftValue >>= 12;
    dwCpuPowerShiftValue &= 0x0F;
    if(dwCpuPowerShiftValue >= 0x08)
    {
        dwPowerShiftValue -= (dwCpuPowerShiftValue & 0x07);
    }
    else
    {
        dwPowerShiftValue += (dwCpuPowerShiftValue & 0x07);
    }

    byTargetRegAddress[0] = 0x10;
    byTargetRegAddress[1] = (0x06 << 1);
    byTargetRegAddress[2] = 0x00;

    dwRegisterValue = (dwCpuPowerVoltage - dwPowerShiftValue);
    dwRegisterValue = (((dwRegisterValue * 1000000) / 1.333333) / 1000000);
    byTargetRegAddress[3] = dwRegisterValue;

    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 4, byTargetRegAddress, 0, byTargetData) == TRUE)
    {
        DVFS_INFO("\033[37m[INFO] Change to Voltage: %d mv (0x%X)\033[m\n", (unsigned int) dwCpuPowerVoltage, (unsigned int) dwRegisterValue);
    }
    else
    {
        DVFS_INFO("\033[37m[ERROR] Software I2C Write Failed\033[m\n");
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCorePowerAdjustment
/// @brief \b Function  \b Description: Update Output Voltage Level in External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCorePowerAdjustment(MS_U32 dwCorePowerVoltage)
{
#if CONFIG_DVFS_CORE_POWER_I2C_ENABLE
    #error "No Support Core Power Adjustment by I2C in Monaco Platform"
#endif
#if CONFIG_DVFS_CORE_POWER_GPIO_ENABLE
    MS_U32  dwRegisterValue = 0;

    dwRegisterValue = *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1));

    if(dwCorePowerVoltage <= CONFIG_DVFS_CORE_POWER_LEVEL_00)
    {
        //VID = 2'b00 = Core Power is 0.95V
        dwRegisterValue &= ~(0x03 << 8);
    }
    else if(dwCorePowerVoltage <= CONFIG_DVFS_CORE_POWER_LEVEL_01)
    {
        //VID = 2'b01 = Core Power is 1.00V
        dwRegisterValue &= ~(0x03 << 8);
        dwRegisterValue |= (0x01 << 8);
    }
    else
    {
       //VID = 2'b01 = Core Power is 1.05V
        dwRegisterValue |= (0x03 << 8);
    }

    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1)) = dwRegisterValue;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x100512 << 1)) = CONFIG_DVFS_DYNAMIC_POWER_ADJUST_INIT;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCpuPowerInit
/// @brief \b Function  \b Description: The Init Flow of  External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuPowerInit(void)
{
#if CONFIG_DVFS_CPU_POWER_I2C_ENABLE
    MS_U8   byTargetRegAddress[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };
    MS_U8   byTargetData[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };

    byTargetRegAddress[0] = 0x53;
    byTargetRegAddress[1] = 0x45;
    byTargetRegAddress[2] = 0x52;
    byTargetRegAddress[3] = 0x44;
    byTargetRegAddress[4] = 0x42;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 5, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_Enter_I2C Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x7F;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_USE_CFG Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x7D;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_OUT_NO_DELAY Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x50;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_AD_BYTE_EN0 Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x55;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_DA_BYTE_EN1 Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x35;
    if(MApi_SWI2C_WriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_USE Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x10;
    byTargetRegAddress[1] = 0xc0;
    if(MApi_SWI2C_ReadBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 2, byTargetRegAddress, 2, byTargetData) == TRUE)
    {
        DVFS_DEBUG("\033[33m[INFO] MStar Power IC Chip ID: %x%x\033[m\n", (unsigned int) byTargetData[0], (unsigned int) byTargetData[1]);
    }

    *(volatile MS_U16 *)(_gMIO_MapBase + (0x100510 << 1)) = CONFIG_DVFS_DYNAMIC_POWER_ADJUST_INIT;
#endif
#if CONFIG_DVFS_CPU_POWER_GPIO_ENABLE
#error "No Support CPU Power Adjustment by GPIO in Muji Platform"
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCorePowerInit
/// @brief \b Function  \b Description: The Init Flow of  External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCorePowerInit(void)
{
#if CONFIG_DVFS_CORE_POWER_I2C_ENABLE
    #error "No Support Core Power Init by I2C in Muji Platform"
#endif
#if CONFIG_DVFS_CORE_POWER_GPIO_ENABLE
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x110cb2 << 1)) = 0x1310;
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x000e38 << 1)) &= ~(0x01 << 10);
    *(volatile MS_U16 *)(_gMIO_MapBase + (0x100512 << 1)) = CONFIG_DVFS_DYNAMIC_POWER_ADJUST_INIT;
#endif
}

#endif

#undef HAL_SAR_C

