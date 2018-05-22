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

#ifndef _HAL_SAR_H_
#define _HAL_SAR_H_

#include "mdrv_types.h"
#include "mdrv_sar_st.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void MHal_SAR_Init(void);
U8 MHal_SAR_GetChannelMaxId(void);
B16 MHal_SAR_Config_ChannelBound(U8 u8Channel, SAR_BND_t *psarBndCfg);
void MHal_SAR_Config_SingleChannel(U8 u8Channel);
void MHal_SAR_Config_TriggerMode(B16 bMode);
void MHal_SAR_Config_SingleChannelEn(B16 bEnable);
void MHal_SAR_Config_ShotMode(U8 u8Mode);
void MHal_SAR_Config_Powerdown(B16 bEnable);
void MHal_SAR_Config_Start(B16 bEnable);
void MHal_SAR_Config_ADCPowerdown(B16 bEnable);
void MHal_SAR_Config_FreeRun(B16 bEnable);
void MHal_SAR_Config_Selection(B16 bEnable);
void MHal_SAR_Config_8Channel(B16 bEnable);
void MHal_SAR_Config_ClockSamplePeriod(U8 u8ClkSmpPrd);
void MHal_SAR_Config_IntMask(B16 bEnable);
void MHal_SAR_Config_IntClear(B16 bEnable);
void MHal_SAR_Config_IntForce(B16 bEnable);
B16 MHal_SAR_GetIntStatus(void);
U8 MHal_SAR_GetChannelADC(U8 u8Channel);
void HAL_SAR_Config_AanlogInputSelect(U8 u8Channel, BOOL bEnable);
void HAL_SAR_Config_OptputEnable(U8 u8Channel, BOOL bEnable);

void MHal_SAR_SetInterrupt(void);
void MHal_SAR_SwPackEnable(BOOL bEnable);


#endif // _HAL_SAR_H_

