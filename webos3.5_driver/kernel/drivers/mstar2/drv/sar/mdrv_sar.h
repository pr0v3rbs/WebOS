////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 MStar Semiconductor, Inc.
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
/// @file   mdrv_sar.h
/// @brief  SAR Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_SAR_H_
#define _MDRV_SAR_H_

#include "mdrv_sar_st.h"
#include "mdrv_sar_io.h"
#include "mdrv_types.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define KEYPAD_STABLE_NUM 30
#define KEYPAD_STABLE_NUM_MIN 29

#define WAFER_TYPE_11Y	  1
#define WAFER_TYPE_12Y	  0


//sar max supported channels (cabability)
#define CFG_SAR_ADC_CHANNELS            4 //### do not modify
//sar max available channels
#define CFG_SAR_ADC_AVAILABLES          2 //### must be <= CFG_SAR_ADC_CHANNELS
//sar adc max levels of all channels
#define CFG_SAR_ADC_LEVEL               4 //### max level for available channels (Maximum level <= 8)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void MDrv_SAR_Init(void);
U8 MDrv_SAR_SetChInfo(SAR_CFG_t *sarChInfo);
U8 MDrv_SAR_CHGetKey(U8 u8Index, U8 u8Channel, U8 *u8Key , U8 *u8Repstatus);
void MDrv_SAR_SetTouchScanMode(U8 bonoff);
U8 MDrv_SAR_GetTouchKeyScanData(void);
void MDrv_SAR_ReadTouchKeyScanData(void);
U8 MDrv_SAR_GetADC_Value(U8 u8Channel);		// GP2R_20101026_jy.song : Touch Key HW Revison ReadÀ§ÇÑ ADC Port Control
U8 MDrv_SAR_GetKeyCode(U8 *u8Key, U8 *u8Repstatus);
void MDrv_SAR_SetPDPErrorPort(void);

#endif // _MDRV_SAR_H_
