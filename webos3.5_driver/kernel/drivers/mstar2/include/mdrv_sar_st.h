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
/// @file   mdrv_sar_st.h
/// @brief  SAR Driver Interface
/// @author MStar Semiconductor Inc.
///
/// Data structure definition
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_SAR_ST_H_
#define _MDRV_SAR_ST_H_

#include "mdrv_types.h"

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef struct
{
    U8 u8UpBnd;   //upper bound
    U8 u8LoBnd;  //low bound
} SAR_BND_t;


typedef struct
{
    U8 u8SARChID;
    SAR_BND_t SARChBnd;
    U8 u8KeyLevelNum;
    U8 u8KeyThreshold[8];
    U8 u8KeyCode[8];
    U8 u8RetVal;
} SAR_CFG_t;

typedef struct
{
    U8 u8Index;
    U8 u8Channel;
    U8 u8Key;
    U8 u8Rep;
    U8 u8RetVal;
} SAR_Key_t;

enum KEYPAD_ADC_CHANNEL
{
    KEYPAD_ADC_CHANNEL_1 = 0,
    KEYPAD_ADC_CHANNEL_2,
    KEYPAD_ADC_CHANNEL_3,
    KEYPAD_ADC_CHANNEL_4,
    KEYPAD_ADC_CHANNEL_5,
    KEYPAD_ADC_CHANNEL_6,
    KEYPAD_ADC_CHANNEL_7,
    KEYPAD_ADC_CHANNEL_8,
};

#endif // _MDRV_SAR_ST_H_
