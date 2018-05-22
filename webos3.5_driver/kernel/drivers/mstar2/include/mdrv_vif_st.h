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
/// @file   mdrv_vif_st.h
/// @brief  VIF Driver Interface
/// @author MStar Semiconductor Inc.
///
/// Data structure definition
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_VIF_ST_H_
#define _MDRV_VIF_ST_H_

//#include <asm-mips/types.h>
#include "mdrv_types.h"
//#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//Define missing type in mdrv_types.h
#if 0
#ifndef BOOL
#define BOOL MS_BOOL
#endif
#endif

/*
#ifndef BYTE
#define BYTE MS_U8
#endif
#ifndef WORD
#define WORD MS_U16
#endif
#ifndef DWORD
#define DWORD MS_U32
#endif
*/

#ifndef BYTE
#define BYTE U8
#endif
#ifndef WORD
#define WORD U16
#endif
#ifndef DWORD
#define DWORD U32
#endif


// register
typedef struct
{
    U32 u32Address;   // register index
    U8  u8Value;      // register value
} MS_VIF_REG_TYPE;

typedef enum
{
    VIF_START,
    VIF_AGC_STATUS,
    VIF_AFC_STATUS,
    VIF_AFC_STATUS2,
    VIF_STEADY_STATUS,
    VIF_NUMS
}VIFStatus;

typedef enum
{
    VCO_CAL_START,
    VCO_CAL_FINISH,
    VCO_CAL_CHECK,
    VCO_CAL_END,
    VCO_NUMS
}VCOCalStatus;

typedef enum
{
    EXTERNAL_DUAL_SAW,                   // 0
    EXTERNAL_SINGLE_SAW,               // 1
    SILICON_TUNER,                             // 2
    NO_SAW,                                          // 3
    INTERNAL_SINGLE_SAW,               // 4
    SAW_ARCH_NUMS
}VIFSawArch;



typedef enum
{
    afcBELOW_MINUS_187p5KHz = 0x07,
    afcMINUS_162p5KHz = 0x06,
    afcMINUS_137p5KHz = 0x05,
    afcMINUS_112p5KHz = 0x04,
    afcMINUS_87p5KHz = 0x03,
    afcMINUS_62p5KHz = 0x02,
    afcMINUS_37p5KHz = 0x01,
    afcMINUS_12p5KHz = 0x00,
    afcPLUS_12p5KHz = 0x0F,
    afcPLUS_37p5KHz = 0x0E,
    afcPLUS_62p5KHz = 0x0D,
    afcPLUS_87p5KHz = 0x0C,
    afcPLUS_112p5KHz = 0x0B,
    afcPLUS_137p5KHz = 0x0A,
    afcPLUS_162p5KHz = 0x09,
    afcABOVE_PLUS_187p5KHz = 0x08   //<NHK_070630_eAFC>
} eAFC;


typedef enum
{
    E_AFC_BELOW_MINUS_187p5KHz      = 0x07,
    E_AFC_MINUS_162p5KHz            = 0x06,
    E_AFC_MINUS_137p5KHz            = 0x05,
    E_AFC_MINUS_112p5KHz            = 0x04,
    E_AFC_MINUS_87p5KHz             = 0x03,
    E_AFC_MINUS_62p5KHz             = 0x02,
    E_AFC_MINUS_37p5KHz             = 0x01,
    E_AFC_MINUS_12p5KHz             = 0x00,
    E_AFC_PLUS_12p5KHz              = 0x0F,
    E_AFC_PLUS_37p5KHz              = 0x0E,
    E_AFC_PLUS_62p5KHz              = 0x0D,
    E_AFC_PLUS_87p5KHz              = 0x0C,
    E_AFC_PLUS_112p5KHz             = 0x0B,
    E_AFC_PLUS_137p5KHz             = 0x0A,
    E_AFC_PLUS_162p5KHz             = 0x09,
    E_AFC_ABOVE_PLUS_187p5KHz       = 0x08,
    E_AFC_OUT_OF_AFCWIN             = 0x10,
} AFC;

typedef enum
{
    VIF_IF_FREQ_INVALID,
    VIF_IF_FREQ_B,              // 38.90 MHz
    VIF_IF_FREQ_G,              // 38.90 MHz
    VIF_IF_FREQ_I,              // 38.90 MHz
    VIF_IF_FREQ_DK,             // 38.90 MHz
    VIF_IF_FREQ_L,              // 38.90 MHz
    VIF_IF_FREQ_L_PRIME,        // 33.90 MHz
    VIF_IF_FREQ_MN,             // 45.75 MHz
    VIF_IF_FREQ_J,              // 58.75 MHz
    VIF_IF_FREQ_PAL_38,         // 38.00 MHz
    VIF_IF_FREQ_MAX_IF_FREQ,
    VIF_IF_DIGITAL_MODE
} VIF_IF_FREQ;

typedef enum
{
    VIF_SOUND_B,
    VIF_SOUND_GH,
    VIF_SOUND_I,
    VIF_SOUND_DK1,
    VIF_SOUND_DK2,
    VIF_SOUND_DK3,
    VIF_SOUND_L,
    VIF_SOUND_LL,
    VIF_SOUND_MN,
    VIF_SOUND_NUMS
}VIFSoundSystem;

typedef enum
{
    IF_FREQ_3395, // SECAM-L'
    IF_FREQ_3800, // PAL
    IF_FREQ_3890, // PAL
    IF_FREQ_3950, // only for PAL-I
    IF_FREQ_4575, // NTSC-M/N
    IF_FREQ_5875, // NTSC-M/N
    IF_FREQ_NUMS
}IfFrequencyType;


typedef enum
{
    FREQ_VHF_L,
    FREQ_VHF_H,
    FREQ_UHF,
    FREQ_RANGE_NUMS
}FrequencyBand;

typedef struct
{
    BYTE VifTop;
    BYTE VifIfBaseFreq;
    BYTE VifTunerStepSize;
    BYTE VifSawArch;
    WORD VifVgaMaximum;
    WORD VifVgaMinimum;
    WORD GainDistributionThr;
    BYTE VifAgcVgaBase;
    BYTE VifAgcVgaOffs;
    BYTE VifAgcRefNegative;
    BYTE VifAgcRefPositive;
    BYTE VifDagc1Ref;
    BYTE VifDagc2Ref;
    WORD VifDagc1GainOv;
    WORD VifDagc2GainOv;
    BYTE VifCrKf1;
    BYTE VifCrKp1;
    BYTE VifCrKi1;
    BYTE VifCrKp2;
    BYTE VifCrKi2;
    BYTE VifCrKp;
    BYTE VifCrKi;
    WORD VifCrLockThr;
    WORD VifCrThr;
    DWORD VifCrLockNum;
    DWORD VifCrUnlockNum;
    WORD VifCrPdErrMax;
    BOOL VifCrLockLeakySel;
    BOOL VifCrPdX2;
    BOOL VifCrLpfSel;
    BOOL VifCrPdModeSel;
    BOOL VifCrKpKiAdjust;
    BYTE VifCrKpKiAdjustGear;
    BYTE VifCrKpKiAdjustThr1;
    BYTE VifCrKpKiAdjustThr2;
    BYTE VifCrKpKiAdjustThr3;
    BOOL VifDynamicTopAdjust;
    BYTE VifDynamicTopMin;
    BOOL VifAmHumDetection;
    BOOL VifClampgainClampSel;
    BYTE VifClampgainSyncbottRef;
    BYTE VifClampgainSyncheightRef;
    BYTE VifClampgainKc;
    BYTE VifClampgainKg;
    BOOL VifClampgainClampOren;
    BOOL VifClampgainGainOren;
    WORD VifClampgainClampOvNegative;
    WORD VifClampgainGainOvNegative;
    WORD VifClampgainClampOvPositive;
    WORD VifClampgainGainOvPositive;
    BYTE VifClampgainClampMin;
    BYTE VifClampgainClampMax;
    BYTE VifClampgainGainMin;
    BYTE VifClampgainGainMax;
    WORD VifClampgainPorchCnt;
    BYTE VifPeakingFilterB_VHF_L;
    BYTE VifYcDelayFilterB_VHF_L;
    BYTE VifGroupDelayFilterB_VHF_L;
    BYTE VifPeakingFilterGH_VHF_L;
    BYTE VifYcDelayFilterGH_VHF_L;
    BYTE VifGroupDelayFilterGH_VHF_L;
    BYTE VifPeakingFilterDK_VHF_L;
    BYTE VifYcDelayFilterDK_VHF_L;
    BYTE VifGroupDelayFilterDK_VHF_L;
    BYTE VifPeakingFilterI_VHF_L;
    BYTE VifYcDelayFilterI_VHF_L;
    BYTE VifGroupDelayFilterI_VHF_L;
    BYTE VifPeakingFilterL_VHF_L;
    BYTE VifYcDelayFilterL_VHF_L;
    BYTE VifGroupDelayFilterL_VHF_L;
    BYTE VifPeakingFilterLL_VHF_L;
    BYTE VifYcDelayFilterLL_VHF_L;
    BYTE VifGroupDelayFilterLL_VHF_L;
    BYTE VifPeakingFilterMN_VHF_L;
    BYTE VifYcDelayFilterMN_VHF_L;
    BYTE VifGroupDelayFilterMN_VHF_L;
    BYTE VifPeakingFilterB_VHF_H;
    BYTE VifYcDelayFilterB_VHF_H;
    BYTE VifGroupDelayFilterB_VHF_H;
    BYTE VifPeakingFilterGH_VHF_H;
    BYTE VifYcDelayFilterGH_VHF_H;
    BYTE VifGroupDelayFilterGH_VHF_H;
    BYTE VifPeakingFilterDK_VHF_H;
    BYTE VifYcDelayFilterDK_VHF_H;
    BYTE VifGroupDelayFilterDK_VHF_H;
    BYTE VifPeakingFilterI_VHF_H;
    BYTE VifYcDelayFilterI_VHF_H;
    BYTE VifGroupDelayFilterI_VHF_H;
    BYTE VifPeakingFilterL_VHF_H;
    BYTE VifYcDelayFilterL_VHF_H;
    BYTE VifGroupDelayFilterL_VHF_H;
    BYTE VifPeakingFilterLL_VHF_H;
    BYTE VifYcDelayFilterLL_VHF_H;
    BYTE VifGroupDelayFilterLL_VHF_H;
    BYTE VifPeakingFilterMN_VHF_H;
    BYTE VifYcDelayFilterMN_VHF_H;
    BYTE VifGroupDelayFilterMN_VHF_H;
    BYTE VifPeakingFilterB_UHF;
    BYTE VifYcDelayFilterB_UHF;
    BYTE VifGroupDelayFilterB_UHF;
    BYTE VifPeakingFilterGH_UHF;
    BYTE VifYcDelayFilterGH_UHF;
    BYTE VifGroupDelayFilterGH_UHF;
    BYTE VifPeakingFilterDK_UHF;
    BYTE VifYcDelayFilterDK_UHF;
    BYTE VifGroupDelayFilterDK_UHF;
    BYTE VifPeakingFilterI_UHF;
    BYTE VifYcDelayFilterI_UHF;
    BYTE VifGroupDelayFilterI_UHF;
    BYTE VifPeakingFilterL_UHF;
    BYTE VifYcDelayFilterL_UHF;
    BYTE VifGroupDelayFilterL_UHF;
    BYTE VifPeakingFilterLL_UHF;
    BYTE VifYcDelayFilterLL_UHF;
    BYTE VifGroupDelayFilterLL_UHF;
    BYTE VifPeakingFilterMN_UHF;
    BYTE VifYcDelayFilterMN_UHF;
    BYTE VifGroupDelayFilterMN_UHF;
    BYTE ChinaDescramblerBox;
    BYTE VifDelayReduce;
    BOOL VifOverModulation;
    BOOL VifOverModulationDetect;
    BOOL VifACIDetect;
    BYTE VifACIAGCREF;
    BYTE VifADCOverflowAGCREF;
    BYTE VifChanelScanAGCREF;
    BYTE VifACIDetTHR1;
    BYTE VifACIDetTHR2;
    BYTE VifACIDetTHR3;
    BYTE VifACIDetTHR4;
    BYTE VifFreqBand;
    WORD VifSos21FilterC0;
    WORD VifSos21FilterC1;
    WORD VifSos21FilterC2;
    WORD VifSos21FilterC3;
    WORD VifSos21FilterC4;
    WORD VifSos22FilterC0;
    WORD VifSos22FilterC1;
    WORD VifSos22FilterC2;
    WORD VifSos22FilterC3;
    WORD VifSos22FilterC4;
    WORD VifSos31FilterC0;
    WORD VifSos31FilterC1;
    WORD VifSos31FilterC2;
    WORD VifSos31FilterC3;
    WORD VifSos31FilterC4;
    WORD VifSos32FilterC0;
    WORD VifSos32FilterC1;
    WORD VifSos32FilterC2;
    WORD VifSos32FilterC3;
    WORD VifSos32FilterC4;
    BYTE VifTunerType;
    DWORD VifCrRate_B;
    BOOL VifCrInvert_B;
    DWORD VifCrRate_GH;
    BOOL VifCrInvert_GH;
    DWORD VifCrRate_DK;
    BOOL VifCrInvert_DK;
    DWORD VifCrRate_I;
    BOOL VifCrInvert_I;
    DWORD VifCrRate_L;
    BOOL VifCrInvert_L;
    DWORD VifCrRate_LL;
    BOOL VifCrInvert_LL;
    DWORD VifCrRate_MN;
    BOOL VifCrInvert_MN;
    BYTE VifReserve;
}VIFInitialIn;

typedef enum
{
    VIF_PARA_USER_FILTER,                                          // VIFUserFilter
    VIF_PARA_TOP,                                                         // DWORD
    VIF_PARA_LEAKY_SEL,                                             // DWORD
    VIF_PARA_K_SEL,                                                     // DWORD
    VIF_PARA_GET_VERSION,                                       // U8[30] return
    VIF_PARA_SET_BYPASS_PEAKING_SOS21,          // DWORD
    VIF_PARA_GET_PGA2_LEVEL,                                   // BYTE return
    VIF_PARA_GET_IF,			// WORD return
    VIF_PARA_GET_K_SEL,			// BYTE return
    VIF_PARA_SET_HW_KPKI,       // DWORD
    VIF_PARA_SET_TAGC_ODMODE,    // DWORD
    VIF_PARA_SET_NOTCH_A1A2,    // VIFNotchA1A2: please set user notch after setting sound system to overwrite default values
    VIF_PARA_SET_SOS_1112,        // VIFSOS1112: please set user SOS after setting sound system to overwrite default values
    VIF_PARA_GROUP_NUMS
}VIF_PARA_GROUP;


typedef enum
{
    PK_START=1,
    PK_B_VHF_L=PK_START,  //PeakingFilterB_VHF_L,
    PK_GH_VHF_L, //PeakingFilterGH_VHF_L,
    PK_DK_VHF_L, //PeakingFilterDK_VHF_L,
    PK_I_VHF_L,  //PeakingFilterI_VHF_L,
    PK_L_VHF_L,  //PeakingFilterL_VHF_L,
    PK_LL_VHF_L, //PeakingFilterLL_VHF_L,
    PK_MN_VHF_L, //PeakingFilterMN_VHF_L,
    PK_B_VHF_H,  //PeakingFilterB_VHF_H,
    PK_GH_VHF_H, //PeakingFilterGH_VHF_H,
    PK_DK_VHF_H, //PeakingFilterDK_VHF_H,
    PK_I_VHF_H,  //PeakingFilterI_VHF_H,
    PK_L_VHF_H,  //PeakingFilterL_VHF_H,
    PK_LL_VHF_H, //PeakingFilterLL_VHF_H,
    PK_MN_VHF_H, //PeakingFilterMN_VHF_H,
    PK_B_UHF,  //PeakingFilterB_UHF,
    PK_GH_UHF, //PeakingFilterGH_UHF,
    PK_DK_UHF, //PeakingFilterDK_UHF,
    PK_I_UHF,  //PeakingFilterI_UHF,
    PK_L_UHF,  //PeakingFilterL_UHF,
    PK_LL_UHF, //PeakingFilterLL_UHF,
    PK_MN_UHF, //PeakingFilterMN_UHF,
    PK_END=PK_MN_UHF,

    YC_START,
    YC_B_VHF_L=YC_START,  //YcDelayFilterB_VHF_L,
    YC_GH_VHF_L, //YcDelayFilterGH_VHF_L,
    YC_DK_VHF_L, //YcDelayFilterDK_VHF_L,
    YC_I_VHF_L,  //YcDelayFilterI_VHF_L,
    YC_L_VHF_L,  //YcDelayFilterL_VHF_L,
    YC_LL_VHF_L, //YcDelayFilterLL_VHF_L,
    YC_MN_VHF_L, //YcDelayFilterMN_VHF_L,
    YC_B_VHF_H,  //YcDelayFilterB_VHF_H,
    YC_GH_VHF_H, //YcDelayFilterGH_VHF_H,
    YC_DK_VHF_H, //YcDelayFilterDK_VHF_H,
    YC_I_VHF_H,  //YcDelayFilterI_VHF_H,
    YC_L_VHF_H,  //YcDelayFilterL_VHF_H,
    YC_LL_VHF_H, //YcDelayFilterLL_VHF_H,
    YC_MN_VHF_H, //YcDelayFilterMN_VHF_H,
    YC_B_UHF,  //YcDelayFilterB_UHF,
    YC_GH_UHF, //YcDelayFilterGH_UHF,
    YC_DK_UHF, //YcDelayFilterDK_UHF,
    YC_I_UHF,  //YcDelayFilterI_UHF,
    YC_L_UHF,  //YcDelayFilterL_UHF,
    YC_LL_UHF, //YcDelayFilterLL_UHF,
    YC_MN_UHF, //YcDelayFilterMN_UHF,
    YC_END=YC_MN_UHF,

    GP_START,
    GP_B_VHF_L=GP_START,  //GroupDelayFilterB_VHF_L,
    GP_GH_VHF_L, //GroupDelayFilterGH_VHF_L,
    GP_DK_VHF_L, //GroupDelayFilterDK_VHF_L,
    GP_I_VHF_L,  //GroupDelayFilterI_VHF_L,
    GP_L_VHF_L,  //GroupDelayFilterL_VHF_L,
    GP_LL_VHF_L, //GroupDelayFilterLL_VHF_L,
    GP_MN_VHF_L, //GroupDelayFilterMN_VHF_L,
    GP_B_VHF_H,  //GroupDelayFilterB_VHF_H,
    GP_GH_VHF_H, //GroupDelayFilterGH_VHF_H,
    GP_DK_VHF_H, //GroupDelayFilterDK_VHF_H,
    GP_I_VHF_H,  //GroupDelayFilterI_VHF_H,
    GP_L_VHF_H,  //GroupDelayFilterL_VHF_H,
    GP_LL_VHF_H, //GroupDelayFilterLL_VHF_H,
    GP_MN_VHF_H, //GroupDelayFilterMN_VHF_H,
    GP_B_UHF,  //GroupDelayFilterB_UHF,
    GP_GH_UHF, //GroupDelayFilterGH_UHF,
    GP_DK_UHF, //GroupDelayFilterDK_UHF,
    GP_I_UHF,  //GroupDelayFilterI_UHF,
    GP_L_UHF,  //GroupDelayFilterL_UHF,
    GP_LL_UHF, //GroupDelayFilterLL_UHF,
    GP_MN_UHF, //GroupDelayFilterMN_UHF,
    GP_END=GP_MN_UHF,

    VIF_USER_FILTER_SELECT_NUMS
}VIF_USER_FILTER_SELECT;

typedef struct
{
    WORD VifSos21FilterC0;
    WORD VifSos21FilterC1;
    WORD VifSos21FilterC2;
    WORD VifSos21FilterC3;
    WORD VifSos21FilterC4;
    WORD VifSos22FilterC0;
    WORD VifSos22FilterC1;
    WORD VifSos22FilterC2;
    WORD VifSos22FilterC3;
    WORD VifSos22FilterC4;
    WORD VifSos31FilterC0;
    WORD VifSos31FilterC1;
    WORD VifSos31FilterC2;
    WORD VifSos31FilterC3;
    WORD VifSos31FilterC4;
    WORD VifSos32FilterC0;
    WORD VifSos32FilterC1;
    WORD VifSos32FilterC2;
    WORD VifSos32FilterC3;
    WORD VifSos32FilterC4;
    BYTE VifUserPeakingFilterSelect;
    BYTE VifUserYcDelayFilterSelect;
    BYTE VifUserGroupDelayFilterSelect;
}VIFUserFilter;
typedef struct
{
    WORD Vif_N_A1_C0;
    WORD Vif_N_A1_C1;
    WORD Vif_N_A1_C2;
    WORD Vif_N_A2_C0;
    WORD Vif_N_A2_C1;
    WORD Vif_N_A2_C2;
}VIFNotchA1A2;

typedef struct
{
    WORD Vif_SOS_11_C0;
    WORD Vif_SOS_11_C1;
    WORD Vif_SOS_11_C2;
    WORD Vif_SOS_11_C3;
    WORD Vif_SOS_11_C4;
    WORD Vif_SOS_12_C0;
    WORD Vif_SOS_12_C1;
    WORD Vif_SOS_12_C2;
    WORD Vif_SOS_12_C3;
    WORD Vif_SOS_12_C4;
}VIFSOS1112;

/////////////////////////////////
//for ioctl structure
/////////////////////////////////

typedef struct
{
    VIFInitialIn * pVIF_InitData;
    DWORD u32InitDataLen;
} VIF_Init_Info_st;

typedef struct
{
    VIF_PARA_GROUP  paraGroup;
    void * pVIF_Para;
    DWORD u32DataLen;
    BOOL bResult;
} VIF_Para_Info_st;


#endif // _MDRV_VIF_ST_H_
