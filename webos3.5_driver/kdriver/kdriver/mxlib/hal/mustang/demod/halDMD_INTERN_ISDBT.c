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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "MsCommon.h"
#include "MsTypes.h"
#include "MsOS.h"
#include "drvBDMA.h"
#include "drvDMD_VD_MBX.h"
//#include "drvDMD_ISDBT.h"
#include "halDMD_INTERN_ISDBT.h"
//#include "drvPWS.h"
#include "drvDMD_common.h"
#include "halDMD_INTERN_common.h"
#ifdef SUPPORT_AP_BIN_IN_FLASH_2
#include "InfoBlock.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define SHOW_DEBUG_INFORMATION 0
#define REG_MCU_RST     0x103480UL   
#define REG_TOP_SOFT_RST     0x2000   
#define REG_MCU_FSM_EN  0x20C0

//#define REG_DEMOD_SW_RST 0x0B20
#define REG_ISDBT_LOCK_STATUS 0x36F5 //(0x3600+(0x7A)*2+1)

#define ISDBT_FE_REG_BASE     0x2700
#define ISDBT_TDP_REG_BASE    0x3700
#define ISDBT_FDP_REG_BASE    0x3800
#define ISDBT_FDPEXT_REG_BASE    0x3900
#define ISDBT_OUTER_REG_BASE  0x3A00
#define ISDBT_DMDTOP_REG_BASE 0x2000


#define SRAM_BASE       0x8000
#define LOAD_CODE_I2C_BLOCK_NUM  0x80



extern s_ISDBT_HAL_FUNCTION_TABLE    sISDBT_HAL_FUNCTION_TABLE;
s_MDRV_DMD_INTERFACE_FUNCTION  sMdrvDmdInterfaceFunc;

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HAL_INTERN_ISDBT_DBINFO(y)   y
#define INTERN_ISDBT_LOAD_FW_FROM_CODE_MEMORY

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
const MS_U8 INTERN_ISDBT_table[] = {
    #include "DMD_INTERN_ISDBT.dat"
};

typedef struct
{
    float    power_db;
    MS_U8    agc_val;
}S_IFAGC_SSI;

static S_IFAGC_SSI IfagcSsi[] =
{
    {-47,    0x8A},
    {-48,    0x8B},
    {-49,    0x8C},
    {-50,    0x8D},
    {-51,    0x8F},
    {-52,    0x90},
    {-53,    0x91},
    {-54,    0x92},
    {-55,    0x93},
    {-56,    0x95},
    {-57,    0x96},
    {-58,    0x97},
    {-59,    0x98},
    {-60,    0x9A},
    {-61,    0x9B},
    {-62,    0x9C},
    {-63,    0x9E},
    {-64,    0x9F},
    {-65,    0xA0},
    {-66,    0xA1},
    {-67,    0xA2},
    {-68,    0xA3},
    {-69,    0xA4},
    {-70,    0xA4},
    {-71,    0xA5},
    {-72,    0xA6},
    {-73,    0xA7},
    {-74,    0xA7},
    {-75,    0xA8},
    {-76,    0xA9},
    {-77,    0xAA},
    {-78,    0xAB},
    {-79,    0xAC},
    {-80,    0xAE},
    {-81,    0xAF},
    {-82,    0xB0},
    {-83,    0xB2},
    {-84,    0xB3},
    {-85,    0xB5},
    {-86,    0xB6},
    {-87,    0xB8},
    {-88,    0xBA},
    {-89,    0xBC},
    {-90,    0xC1},
    {-99,    0xFF},
};

extern MS_U16  u16DMD_ISDBT_TUNER_IF_KHZ;
extern MS_U8    u8DMD_ISDBT_TS_CONFIG;
extern MS_BOOL bDMD_ISDBT_TUNER_IS_IQ_SWAP;
extern MS_U16  u16DMD_ISDBT_AGC_REF_VALUE; 
extern MS_U32  u32DMD_ISDBT_TDI_START_ADDR;
static MS_BOOL downloaded = FALSE;
static MS_BOOL _HAL_INTERN_ISDBT_InitClk(void);
static MS_BOOL _HAL_INTERN_ISDBT_Exit(void);
//static void _HAL_INTERN_ISDBT_InitClk(MS_BOOL bRFAGCTristateEnable);
static MS_BOOL _HAL_INTERN_ISDBT_Download(void);
static MS_BOOL _HAL_INTERN_ISDBT_SoftReset(void);
static MS_BOOL _HAL_INTERN_ISDBT_SetModeClean(void);
static MS_BOOL _HAL_INTERN_ISDBT_Active(void);
static MS_BOOL _HAL_INTERN_ISDBT_Check_FEC_Lock(void);
static MS_BOOL _HAL_INTERN_ISDBT_Check_FSA_TRACK_Lock(void);
static MS_BOOL _HAL_INTERN_ISDBT_Check_PSYNC_Lock(void);
static MS_BOOL _HAL_INTERN_ISDBT_Check_ICFO_CH_EXIST_Lock(void);
static MS_U16 _HAL_INTERN_ISDBT_Read_PKT_ERR(MS_U8 LayerIndex, MS_U16 *u16PacketErr);
static MS_U16 _HAL_INTERN_ISDBT_FWVERSION(void);
static MS_BOOL _HAL_INTERN_ISDBT_GetSignalCodeRate(EN_ISDBT_Layer LayerIndex, EN_ISDBT_CODE_RATE  *IsdbtCodeRate);
static MS_BOOL _HAL_INTERN_ISDBT_GetSignalGuardInterval(EN_ISDBT_GUARD_INTERVAL *IsdbtGI);
static MS_BOOL _HAL_INTERN_ISDBT_GetSignalTimeInterleaving(EN_ISDBT_Layer LayerIndex, EN_ISDBT_TIME_INTERLEAVING *IsdbtTDI);
static MS_BOOL _HAL_INTERN_ISDBT_GetSignalFFTValue(EN_ISDBT_FFT_VAL *IsdbtFFT);
static MS_BOOL _HAL_INTERN_ISDBT_GetSignalModulation(EN_ISDBT_Layer LayerIndex, EN_ISDBT_CONSTEL_TYPE *IsdbtConstellation);
static MS_U16 _HAL_INTERN_ISDBT_GetSignalStrength(void);

static MS_U16 _HAL_ISDBT_CALCULATE_SQI(float fber);
static MS_U16 _HAL_INTERN_ISDBT_GetSignalQuality(void);
static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerA(void);
static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerB(void);
static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerC(void);
static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerCombine(void);
static MS_BOOL _HAL_INTERN_ISDBT_IIC_Bypass_Mode(MS_BOOL bEnable);

static MS_BOOL _HAL_INTERN_ISDBT_SWITCH_SSPI_GPIO(MS_BOOL bEnable);

static MS_BOOL _HAL_INTERN_ISDBT_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel);

static MS_BOOL _HAL_INTERN_ISDBT_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel);

static MS_BOOL _HAL_INTERN_ISDBT_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut);

static MS_BOOL _HAL_INTERN_ISDBT_GetPostViterbiBer(EN_ISDBT_Layer LayerIndex, float *ber);

static MS_BOOL _HAL_INTERN_ISDBT_GetPreViterbiBer(EN_ISDBT_Layer LayerIndex, float *ber);

static MS_BOOL _HAL_INTERN_ISDBT_GetFreqOffset(float *pFreqOff);
static MS_BOOL _HAL_INTERN_ISDBT_GetSNR(float *pf_snr);
static MS_BOOL _HAL_INTERN_ISDBT_TS_INTERFACE_CONFIG(MS_U8 u8TsConfigData);
static MS_BOOL _HAL_INTERN_ISDBT_WRITE_MBX(MS_U16 u16Addr, MS_U8 *pu8Data);
static MS_BOOL _HAL_INTERN_ISDBT_READ_MBX(MS_U16 u16Addr, MS_U8 *pu8Data);


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//void _HAL_INTERN_ISDBT_InitClk(MS_BOOL bRFAGCTristateEnable)
//{    
//    return;
//}

static MS_BOOL _HAL_INTERN_ISDBT_InitClk(void) /* Ok */
{    
    MS_BOOL bRet = TRUE;
    //MS_U8   u8Val = 0x00;
    //MS_U8   u8MiuSel = 0;
    //MS_U32  u32StartOffset = 0;

    HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    // Init by HKMCU
    #if 0 // Audio MIU Bridge removed from demod    
    u8Val = HAL_DMD_RIU_ReadByte(0x11208E);    //dan add to clear bit 0
    u8Val &= ~0x01; 
    HAL_DMD_RIU_WriteByte(0x11208E, u8Val);
    #endif

    HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    HAL_DMD_RIU_WriteByte(0x103302, 0x00);

    HAL_DMD_RIU_WriteByte(0x111f29, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f28, 0x10); 
    HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f45, 0x44);
    HAL_DMD_RIU_WriteByte(0x111f44, 0x44);
    HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    HAL_DMD_RIU_WriteByte(0x111f4d, 0x00); //inner clock
    HAL_DMD_RIU_WriteByte(0x111f4c, 0x40);    
    HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 
    HAL_DMD_RIU_WriteByte(0x111f71, 0x14);  
    HAL_DMD_RIU_WriteByte(0x111f70, 0x41);     
    HAL_DMD_RIU_WriteByte(0x111f77, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f76, 0x00);   
    HAL_DMD_RIU_WriteByte(0x111f4f, 0x01);
    HAL_DMD_RIU_WriteByte(0x111f4e, 0x00);    
/*
    _phy_to_miu_offset(u8MiuSel, u32StartOffset, (u32DMD_ISDBT_TDI_START_ADDR<<4));
    MDrv_MIU_Init();
    switch (u8MiuSel)
    {    
    case E_CHIP_MIU_0:
        MDrv_MIU_SelMIU(MIU_CLIENT_DEMOD_W, MIU_SELTYPE_MIU0);
        MDrv_MIU_SelMIU(MIU_CLIENT_DEMOD_R, MIU_SELTYPE_MIU0);
        break;  
    case E_CHIP_MIU_1:
        MDrv_MIU_SelMIU(MIU_CLIENT_DEMOD_W, MIU_SELTYPE_MIU1);
        MDrv_MIU_SelMIU(MIU_CLIENT_DEMOD_R, MIU_SELTYPE_MIU1);
        break; 
    case E_CHIP_MIU_2:
        MDrv_MIU_SelMIU(MIU_CLIENT_DEMOD_W, MIU_SELTYPE_MIU2);
        MDrv_MIU_SelMIU(MIU_CLIENT_DEMOD_R, MIU_SELTYPE_MIU2);
        break; 
    case E_CHIP_MIU_3:
        MDrv_MIU_SelMIU(MIU_CLIENT_DEMOD_W, MIU_SELTYPE_MIU3);
        MDrv_MIU_SelMIU(MIU_CLIENT_DEMOD_R, MIU_SELTYPE_MIU3);
        break; 
    default:
        break;
    }    
 */   
    HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
    
    return bRet;
}


static MS_BOOL _HAL_INTERN_ISDBT_Download(void)
{
    MS_U8 udata = 0x00;
    MS_U16 i=0;
    MS_U16 fail_cnt=0;
    MS_U8  u8TmpData;
    MS_U16  u16AddressOffset;    
    //static MS_BOOL downloaded = FALSE;
    #ifndef INTERN_ISDBT_LOAD_FW_FROM_CODE_MEMORY
    BININFO BinInfo;
    MS_BOOL bResult;
    MS_U32 u32GEAddr;
    MS_U8 Data;
    MS_S8 op;
    MS_U32 srcaddr;
    MS_U32 len;
    MS_U32 SizeBy4K;
    MS_U16 u16Counter=0;
    MS_U8 *pU8Data;
    #endif

    if (downloaded)
    {
        HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  0x01); // reset VD_MCU
        HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  0x00);
        MsOS_DelayTask(20);
        return TRUE;
    }
    
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00,  0x01);        // reset VD_MCU
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01,  0x00);        // disable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00,  0x00);        // release MCU, madison patch
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x50);        // enable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x51);        // enable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  0x00);        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  0x00);        // sram address high byte

    ////  Load code thru VDMCU_IF ////
    HAL_INTERN_ISDBT_DBINFO(printf(">Load Code...\n"));

    #ifdef INTERN_ISDBT_LOAD_FW_FROM_CODE_MEMORY
    for ( i = 0; i < sizeof(INTERN_ISDBT_table); i++)
    {
        HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, INTERN_ISDBT_table[i]); // write data to VD MCU 51 code sram
    }
    if(sizeof(INTERN_ISDBT_table) >= 0x6B80)
    {
        HAL_INTERN_ISDBT_DBINFO(printf("Firmware code size over 0x6B80!!!\n"));
        return FALSE;    
    }
    /*
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  0x00);        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  0x04);        // sram address high byte  
    udata = HAL_DMD_RIU_ReadByte(DMDMcuBase+0x10);
    u16AddressOffset = udata << 8;
    udata = HAL_DMD_RIU_ReadByte(DMDMcuBase+0x10);
    u16AddressOffset |= udata;
    printf("u16AddressOffset = 0x%x\n", u16AddressOffset);
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  (u16AddressOffset & 0xFF));        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  (u16AddressOffset >>8));        // sram address high byte  
    u8TmpData = (MS_U8)u16DMD_ISDBT_TUNER_IF_KHZ;
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u16DMD_ISDBT_TUNER_IF_KHZ >> 8);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(bDMD_ISDBT_TUNER_IS_IQ_SWAP);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u16DMD_ISDBT_AGC_REF_VALUE);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u16DMD_ISDBT_AGC_REF_VALUE >> 8);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u32DMD_ISDBT_TDI_START_ADDR);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram 
    u8TmpData = (MS_U8)(u32DMD_ISDBT_TDI_START_ADDR >> 8);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u32DMD_ISDBT_TDI_START_ADDR >> 16);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u32DMD_ISDBT_TDI_START_ADDR >> 24);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram    
    */
    #else
    BinInfo.B_ID = BIN_ID_INTERN_ISDBT_DEMOD;
    msAPI_MIU_Get_BinInfo(&BinInfo, &bResult);
    if ( bResult != PASS )
    {
        HAL_INTERN_ISDBT_DBINFO(printf("Get BIN info Error!\r\n"));
        return FALSE;
    }    

    #ifdef SUPPORT_AP_BIN_IN_FLASH_2
    InfoBlock_Flash_2_Checking_Start(&BinInfo);
    #endif
    MApi_BDMA_CopyFromResource(BinInfo.B_FAddr, _PA2VA((DEMOD_MEM_MEMORY_TYPE & MIU1) ?
                               (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)), MemAlign(BinInfo.B_Len, 8));
    #ifdef SUPPORT_AP_BIN_IN_FLASH_2
    InfoBlock_Flash_2_Checking_End(&BinInfo);
    #endif

    SizeBy4K = BinInfo.B_Len/0x1000;

    u32GEAddr = _PA2VA(((DEMOD_MEM_MEMORY_TYPE & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)));

    for (i = 0; i <= SizeBy4K; i++)
    {
        if (i == SizeBy4K) 
            len = BinInfo.B_Len%0x1000;
        else
            len = 0x1000;

        srcaddr = u32GEAddr+(0x1000*i);   
        op = 1;
        u16Counter = 0 ;
        while(len--)
        {
            u16Counter++ ;

            Data = *pU8Data;

            HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, Data); // write data to VD MCU 51 code sram

            srcaddr += op;
        }
    }
    #endif 

    ////  Content verification ////
    HAL_INTERN_ISDBT_DBINFO(printf(">Verify Code...\n"));

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, 0x00); // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x00); // sram address high byte

    #ifdef INTERN_ISDBT_LOAD_FW_FROM_CODE_MEMORY
    for ( i = 0; i < sizeof(INTERN_ISDBT_table); i++)
    {
        udata = HAL_DMD_RIU_ReadByte(DMDMcuBase+0x10); // read sram data

        if (udata != INTERN_ISDBT_table[i])
        {
            HAL_INTERN_ISDBT_DBINFO(printf(">fail add = 0x%x\n", i));
            HAL_INTERN_ISDBT_DBINFO(printf(">code = 0x%x\n", INTERN_ISDBT_table[i]));
            HAL_INTERN_ISDBT_DBINFO(printf(">data = 0x%x\n", udata));

            if (fail_cnt++ > 10)
            {
                HAL_INTERN_ISDBT_DBINFO(printf(">DSP Loadcode fail!"));
                return FALSE;
            }
        }
    }
    
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  0x00);        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  0x04);        // sram address high byte  
    udata = HAL_DMD_RIU_ReadByte(DMDMcuBase+0x10);
    u16AddressOffset = udata << 8;
    udata = HAL_DMD_RIU_ReadByte(DMDMcuBase+0x10);
    u16AddressOffset |= udata;

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  (u16AddressOffset & 0xFF));        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  (u16AddressOffset >>8));        // sram address high byte  
    u8TmpData = (MS_U8)u16DMD_ISDBT_TUNER_IF_KHZ;
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u16DMD_ISDBT_TUNER_IF_KHZ >> 8);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(bDMD_ISDBT_TUNER_IS_IQ_SWAP);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u16DMD_ISDBT_AGC_REF_VALUE);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u16DMD_ISDBT_AGC_REF_VALUE >> 8);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u32DMD_ISDBT_TDI_START_ADDR);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram 
    u8TmpData = (MS_U8)(u32DMD_ISDBT_TDI_START_ADDR >> 8);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u32DMD_ISDBT_TDI_START_ADDR >> 16);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(u32DMD_ISDBT_TDI_START_ADDR >> 24);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram    
    
    
    #else
    for (i = 0; i <= SizeBy4K; i++)
    {
        if (i == SizeBy4K) 
			len = BinInfo.B_Len%0x1000;
        else
			len = 0x1000;

        srcaddr = u32GEAddr+(0x1000*i);

        op = 1;
        u16Counter = 0 ;
        while(len--)
        {
            u16Counter++ ;

            Data  = *pU8Data;

            udata = HAL_DMD_RIU_ReadByte(DMDMcuBase+0x10); // read sram data

            if (udata != Data)
            {
                HAL_INTERN_ISDBT_DBINFO(printf(">fail add = 0x%lx\n", (U32)((i*0x1000)+(0x1000-len))));
                HAL_INTERN_ISDBT_DBINFO(printf(">code = 0x%x\n", Data));
                HAL_INTERN_ISDBT_DBINFO(printf(">data = 0x%x\n", udata));

                if (fail_cnt++ > 10)
                {
                    HAL_INTERN_ISDBT_DBINFO(printf(">DSP Loadcode fail!"));
                    return FALSE;
                }
            }
        }
    }
    #endif
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x50);     // diable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x00);     // disable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x01);     // reset MCU, madison patch
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01, 0x01);     // enable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x00);     // release VD_MCU
    downloaded = TRUE;
    HAL_INTERN_ISDBT_DBINFO(printf(">DSP Loadcode done."));

    return TRUE;
}

static MS_BOOL _HAL_INTERN_ISDBT_SoftReset(void) /* OK */
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(REG_MCU_FSM_EN, &u8Data);
    u8Data = u8Data & ~0x04;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(REG_MCU_FSM_EN, u8Data);   
    MsOS_DelayTask(1);
    
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(REG_TOP_SOFT_RST, &u8Data);
    sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(REG_TOP_SOFT_RST, (u8Data|0x10));
    MsOS_DelayTask(1);
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(REG_TOP_SOFT_RST, (u8Data&(~0x10)));
    return bRet;
        
}

static MS_BOOL _HAL_INTERN_ISDBT_SetModeClean(void) /* OK */
{    
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(REG_MCU_FSM_EN, &u8Data);
    u8Data = u8Data & ~0x04;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(REG_MCU_FSM_EN, u8Data);

    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_Active(void) /* OK */
{    
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(REG_MCU_FSM_EN, &u8Data);
    u8Data = u8Data | 0x04;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(REG_MCU_FSM_EN, u8Data);
    return bRet;
}


static MS_BOOL _HAL_INTERN_ISDBT_Check_FEC_Lock(void) /* OK */
{
    MS_BOOL   bCheckPass = FALSE;
    MS_U8     u8Data;       
    // ISDBT_LOCK_STATUS
    // [0] F_FSATRACK_LOCKED
    // [1] F_FEC_LOCKED
    // [2] F_PSYNC_LOCKED
    // [7] F_ICFO_CH_EXIST_FLAG
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(REG_ISDBT_LOCK_STATUS, &u8Data);
    if((u8Data & 0x02) != 0x00)       // Check FEC Lock Flag
        bCheckPass = TRUE;

    return bCheckPass;
}

static MS_BOOL _HAL_INTERN_ISDBT_Check_FSA_TRACK_Lock(void) /* OK */
{
    MS_BOOL   bCheckPass = FALSE;
    MS_U8     u8Data;       
    // ISDBT_LOCK_STATUS
    // [0] F_FSATRACK_LOCKED
    // [1] F_FEC_LOCKED
    // [2] F_PSYNC_LOCKED
    // [7] F_ICFO_CH_EXIST_FLAG
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(REG_ISDBT_LOCK_STATUS, &u8Data);
    if((u8Data & 0x01) != 0x00)       // Check FSA Track Lock Flag
        bCheckPass = TRUE;

    return bCheckPass;
}

static MS_BOOL _HAL_INTERN_ISDBT_Check_PSYNC_Lock(void) /* OK */
{
    MS_BOOL   bCheckPass = FALSE;
    MS_U8     u8Data;       
    // ISDBT_LOCK_STATUS
    // [0] F_FSATRACK_LOCKED
    // [1] F_FEC_LOCKED
    // [2] F_PSYNC_LOCKED
    // [7] F_ICFO_CH_EXIST_FLAG
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(REG_ISDBT_LOCK_STATUS, &u8Data);
    if((u8Data & 0x04) != 0x00)       // Check Psync Lock Flag
        bCheckPass = TRUE;

    return bCheckPass;
}

static MS_BOOL _HAL_INTERN_ISDBT_Check_ICFO_CH_EXIST_Lock(void) /* OK */
{
    MS_BOOL   bCheckPass = FALSE;
    MS_U8     u8Data;    
    // ISDBT_LOCK_STATUS
    // [0] F_FSATRACK_LOCKED
    // [1] F_FEC_LOCKED
    // [2] F_PSYNC_LOCKED
    // [7] F_ICFO_CH_EXIST_FLAG
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(REG_ISDBT_LOCK_STATUS, &u8Data);
    if((u8Data & 0x80) != 0x00)       // Check Psync Lock Flag
        bCheckPass = TRUE;

    return bCheckPass;
}


static MS_U16 _HAL_INTERN_ISDBT_Read_PKT_ERR(MS_U8 LayerIndex, MS_U16 *u16PacketErr) /* OK */
{
    MS_U8 bRet = true;
    MS_U8 u8Data, u8FrzData;
    MS_U16 u16PacketErrA = 0xFFFF, u16PacketErrB = 0xFFFF, u16PacketErrC = 0xFFFF;

    // Read packet errors of three layers
    // OUTER_FUNCTION_ENABLE
    // [8] reg_biterr_num_pcktprd_freeze
    // Freeze Packet error
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE+0x01*2+1, &u8FrzData);
    u8Data = u8FrzData | 0x01;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE+0x01*2+1, u8Data);

    switch(LayerIndex)
    {
        case E_ISDBT_Layer_A:
            // [15:0] OUTER_UNCRT_PKT_NUM_PCKTPRD_A
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE+0x08*2+1, &u8Data);
            u16PacketErrA = u8Data << 8;
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE+0x08*2, &u8Data);
            u16PacketErrA = u16PacketErrA | u8Data;
            *u16PacketErr = u16PacketErrA;
            break;
        case E_ISDBT_Layer_B:
            // [15:0] OUTER_UNCRT_PKT_NUM_PCKTPRD_B
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE+0x21*2+1, &u8Data);
            u16PacketErrB = u8Data << 8;
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE+0x21*2, &u8Data);
            u16PacketErrB = u16PacketErrB | u8Data;
            *u16PacketErr = u16PacketErrB;
            break;
        case E_ISDBT_Layer_C:
            // [15:0] OUTER_UNCRT_PKT_NUM_PCKTPRD_C
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE+0x42*2+1, &u8Data);
            u16PacketErrC = u8Data << 8;
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE+0x42*2, &u8Data);
            u16PacketErrC = u16PacketErrC | u8Data;
            *u16PacketErr = u16PacketErrC;
            break;
        default:
            *u16PacketErr = 0xFFFF;
            break;
    }

    // Unfreeze Packet error
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE+0x01*2+1, u8FrzData);


    return bRet;  
}



static MS_U16 _HAL_INTERN_ISDBT_FWVERSION(void) /* OK */
{
    MS_U8 u8Data;
    MS_U16 u16FwVer;
    
    printf("Separate hal .so file\n");
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_DMDTOP_REG_BASE+0x62*2+0, &u8Data);
    printf(">>>Intern-ISDB-T FW ver: %x\n", u8Data);//HAL_INTERN_ISDBT_DBINFO(printf(">>>MSB140x FW ver: %x\n", u8Data));
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_DMDTOP_REG_BASE+0x62*2+1, &u8Data);
    printf(">>>Intern-ISDB-T FW ver: %x\n", u8Data);//HAL_INTERN_ISDBT_DBINFO(printf(">>>MSB140x FW ver: %x\n", u8Data));
    u16FwVer = u8Data << 8;
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_DMDTOP_REG_BASE+0x63*2+0, &u8Data);
    printf(">>>Intern-ISDB-T FW ver: %x\n", u8Data);//HAL_INTERN_ISDBT_DBINFO(printf(">>>MSB140x FW ver: %x\n", u8Data));
    u16FwVer |= u8Data;
    
    return u16FwVer; 
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSignalCodeRate(EN_ISDBT_Layer LayerIndex, EN_ISDBT_CODE_RATE  *IsdbtCodeRate) /* OK */
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data, u8CodeRate;
    

    switch(LayerIndex)
    {
        case E_ISDBT_Layer_A:
            // [10:8] reg_tmcc_cur_convolution_code_rate_a
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE+0x04*2+1, &u8Data);
            u8CodeRate = u8Data & 0x07;
            break;
        case E_ISDBT_Layer_B:
            // [10:8] reg_tmcc_cur_convolution_code_rate_b
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE+0x05*2+1, &u8Data);
            u8CodeRate = u8Data & 0x07;
            break;
       case E_ISDBT_Layer_C:
            // [10:8] reg_tmcc_cur_convolution_code_rate_c
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE+0x06*2+1, &u8Data);
            u8CodeRate = u8Data & 0x07;
            break;
       default:
            u8CodeRate = 15;
            break;
    }

    switch(u8CodeRate)
    {
        case 0:
            *IsdbtCodeRate = E_ISDBT_CODERATE_1_2;
            break;
        case 1:
            *IsdbtCodeRate = E_ISDBT_CODERATE_2_3;
            break;
        case 2:
            *IsdbtCodeRate = E_ISDBT_CODERATE_3_4;
            break;
        case 3:
            *IsdbtCodeRate = E_ISDBT_CODERATE_5_6;
            break;
        case 4:
            *IsdbtCodeRate = E_ISDBT_CODERATE_7_8;
            break;
        default:
            *IsdbtCodeRate = E_ISDBT_CODERATE_INVALID;
            break;
    }

    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSignalGuardInterval(EN_ISDBT_GUARD_INTERVAL *IsdbtGI) /* OK */
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data, u8CP;

    // [7:6] reg_mcd_out_cp
    // output cp -> 00: 1/4
    //                    01: 1/8
    //                    10: 1/16
    //                    11: 1/32
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE+0x34*2, &u8Data);
    u8CP  = (u8Data >> 6) & 0x03;
    switch(u8CP)
    {
        case 0:
            *IsdbtGI = E_ISDBT_GUARD_INTERVAL_1_4;
            break;
        case 1:
            *IsdbtGI = E_ISDBT_GUARD_INTERVAL_1_8;
            break;
        case 2:
            *IsdbtGI = E_ISDBT_GUARD_INTERVAL_1_16;
            break;
        case 3:
            *IsdbtGI = E_ISDBT_GUARD_INTERVAL_1_32;
            break;
        default:
            *IsdbtGI = E_ISDBT_GUARD_INTERVAL_INVALID;
            break;
    }

    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSignalTimeInterleaving(EN_ISDBT_Layer LayerIndex, EN_ISDBT_TIME_INTERLEAVING *IsdbtTDI)/* OK */
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data, u8Mode, u8Tdi;
    

    // [5:4] reg_mcd_out_mode
    // output mode  -> 00: 2k
    //                         01: 4k
    //                         10: 8k
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE+0x34*2, &u8Data);
    u8Mode  = (u8Data >> 4) & 0x03;

    switch(LayerIndex)
    {
        case E_ISDBT_Layer_A:
            // [14:12] reg_tmcc_cur_interleaving_length_a
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE+0x04*2+1, &u8Data);
            u8Tdi = (u8Data >> 4) & 0x07;
            break;
        case E_ISDBT_Layer_B:
            // [14:12] reg_tmcc_cur_interleaving_length_b
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE+0x05*2+1, &u8Data);
            u8Tdi = (u8Data >> 4) & 0x07;
            break;
        case E_ISDBT_Layer_C:
            // [14:12] reg_tmcc_cur_interleaving_length_c
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE+0x06*2+1, &u8Data);
            u8Tdi = (u8Data >> 4) & 0x07;
            break;
       default:
            u8Tdi = 15;
            break;
    }

    // u8Tdi+u8Mode*4
    // => 0~3: 2K
    // => 4~7: 4K
    // => 8~11:8K
    switch(u8Tdi+u8Mode*4)
    {
        case 0:
            *IsdbtTDI = E_ISDBT_2K_TDI_0;
            break;
        case 1:
            *IsdbtTDI = E_ISDBT_2K_TDI_4;
            break;
        case 2:
            *IsdbtTDI = E_ISDBT_2K_TDI_8;
            break;
        case 3:
            *IsdbtTDI = E_ISDBT_2K_TDI_16;
            break;
        case 4:
            *IsdbtTDI = E_ISDBT_4K_TDI_0;
            break;
        case 5:
            *IsdbtTDI = E_ISDBT_4K_TDI_2;
            break;
        case 6:
            *IsdbtTDI = E_ISDBT_4K_TDI_4;
            break;
        case 7:
            *IsdbtTDI = E_ISDBT_4K_TDI_8;
            break;
        case 8:
            *IsdbtTDI = E_ISDBT_8K_TDI_0;
            break;
        case 9:
            *IsdbtTDI = E_ISDBT_8K_TDI_1;
            break;
        case 10:
            *IsdbtTDI = E_ISDBT_8K_TDI_2;
            break;
        case 11:
            *IsdbtTDI = E_ISDBT_8K_TDI_4;
            break;
        default:
            *IsdbtTDI = E_ISDBT_TDI_INVALID;
            break;
    }

    return bRet;
}
static MS_BOOL _HAL_INTERN_ISDBT_GetSignalFFTValue(EN_ISDBT_FFT_VAL *IsdbtFFT) /* OK */
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data, u8Mode;
    // [5:4]  reg_mcd_out_mode
    // output mode  -> 00: 2k
    //                         01: 4k
    //                         10: 8k
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE+0x34*2, &u8Data);
    u8Mode  = (u8Data >> 4) & 0x03;

    switch(u8Mode)
    {
        case 0:
            *IsdbtFFT = E_ISDBT_FFT_2K;
            break;
        case 1:
            *IsdbtFFT = E_ISDBT_FFT_4K;
            break;
        case 2:
            *IsdbtFFT = E_ISDBT_FFT_8K;
            break;
        default:
            *IsdbtFFT = E_ISDBT_FFT_INVALID;
            break;
    }

    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSignalModulation(EN_ISDBT_Layer LayerIndex, EN_ISDBT_CONSTEL_TYPE *IsdbtConstellation) /* OK */
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data, u8QAM;

    switch(LayerIndex)
    {
        case E_ISDBT_Layer_A:
            // [6:4] reg_tmcc_cur_carrier_modulation_a
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE+0x04*2, &u8Data);
            u8QAM = (u8Data >> 4) & 0x07;
            break;
        case E_ISDBT_Layer_B:
            // [6:4] reg_tmcc_cur_carrier_modulation_b
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE+0x05*2, &u8Data);
            u8QAM = (u8Data >> 4) & 0x07;
            break;
        case E_ISDBT_Layer_C:
            // [6:4] reg_tmcc_cur_carrier_modulation_c
            bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE+0x06*2, &u8Data);
            u8QAM = (u8Data >> 4) & 0x07;
            break;
        default:
            u8QAM = 15;
            break;
    }

    switch(u8QAM)
    {
        case 0:
            *IsdbtConstellation = E_ISDBT_DQPSK;
            break;
        case 1:
            *IsdbtConstellation = E_ISDBT_QPSK;
            break;
        case 2:
            *IsdbtConstellation = E_ISDBT_16QAM;
            break;
        case 3:
            *IsdbtConstellation = E_ISDBT_64QAM;
            break;
        default:
            *IsdbtConstellation = E_ISDBT_QAM_INVALID;
            break;
    }

    return bRet;
}

// If you want to use this api, you need to add one talbe(IfagcSsi[]) which is depending on tuner.
static float _HAL_INTERN_ISDBT_GetSignalLevelFromDemodIF(void) /* OK */
{
    MS_BOOL  bRet = true;
    MS_U8  u8Data, i;
//   MAPI_U16 u16IfAgc, if_agc_val;
    MS_U8  u8IfAgc, if_agc_val;
    float    ch_power_if = 0.0, ch_power_dbm = 0.0;

    ////////// IF-AGC Gain Out /////////////
    //debug select
    //select agc gain2, dagc0 gain, dagc1 gain, dagc2 gain
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FE_REG_BASE+0x0B*2, 0x03);

    //set freeze & dump [Write]
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FE_REG_BASE+0x01*2+1, 0x80);

    // Read High Byte of IF AGC
    bRet = sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FE_REG_BASE+0x0C*2+1, &u8Data);
    u8IfAgc = u8Data;
    if_agc_val = u8IfAgc;

    //Unfreeze & dump [Write]
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FE_REG_BASE+0x01*2+1, 0x00);

    for(i = 0; i < sizeof(IfagcSsi)/sizeof(S_IFAGC_SSI); i++)
    {
        if (if_agc_val <= IfagcSsi[i].agc_val)
        {
            ch_power_if = IfagcSsi[i].power_db;
            break;
        }
    }

    ch_power_dbm = ch_power_if;

    return ch_power_dbm;
}
static MS_U16 _HAL_INTERN_ISDBT_GetSignalStrength(void) /* OK */
{
    float        ch_power_dbm;
    MS_U16     u16Strength;
    MS_BOOL      bRet = true;

    ch_power_dbm = _HAL_INTERN_ISDBT_GetSignalLevelFromDemodIF();

    if(_HAL_INTERN_ISDBT_Check_FEC_Lock() == FALSE)
    { 
        return 0; 
    }
    else
    {
        // Part 2: transfer signal level to SSI values.
        if (ch_power_dbm < -90.0)
            u16Strength = 0;
        else if (ch_power_dbm < -70.0)
            u16Strength = (MS_U16)((ch_power_dbm + 90.0)*(50.0/20.0)+0.0);
        else if (ch_power_dbm < -60.0)
            u16Strength = (MS_U16)((ch_power_dbm + 70.0)*(30.0/10.0)+ 50.0);
        else if (ch_power_dbm < -50.0)
            u16Strength = (MS_U16)((ch_power_dbm + 60.0)*(20.0/10.0) + 80.0);
        else
            u16Strength = 100;

        if (bRet == true)
            return u16Strength;
        else
            return 0;
    }

}

static MS_U16 _HAL_ISDBT_CALCULATE_SQI(float fber)
{
    float log_ber;
    MS_U16 SQI;
    
    #ifdef MSOS_TYPE_LINUX
    log_ber = (float)log10((double)fber);
    #else
    if(fber != 0.0)
      log_ber = (float)(-1.0*sMdrvDmdInterfaceFunc.Log10Approx((double)(1.0 / fber))); 
    else
      log_ber = -8.0;//when fber=0 means SQI=100    
    #endif
    
    //printf("dan fber = %f\n", fber);
    //printf("dan log_ber = %f\n", log_ber);
    // Part 2: transfer ber value to SQI value.
    if (log_ber <= ( - 7.0))
    {
       SQI = 100;    //*quality = 100;
    }
    else if(log_ber < -6.0)
    {
       SQI = (90+((( - 6.0) - log_ber) / (( - 6.0) - ( - 7.0))*(100-90)));
    }
    else if(log_ber < -5.5)
    {
       SQI = (80+((( - 5.5) - log_ber) / (( - 5.5) - ( - 6.0))*(90-80)));
    }
    else if(log_ber < -5.0)
    {
       SQI = (70+((( - 5.0) - log_ber) / (( - 5.0) - ( - 5.5))*(80-70)));
    }
    else if(log_ber < -4.5)
    {
       SQI = (60+((( - 4.5) - log_ber) / (( -4.5) - ( - 5.0))*(70-50)));
    }
    else if(log_ber < -4.0)
    {
       SQI = (50+((( - 4.0) - log_ber) / (( - 4.0) - ( - 45))*(60-50)));
    }
    else if(log_ber < -3.5)
    {
       SQI = (40+((( - 3.5) - log_ber) / (( - 3.5) - ( - 4.0))*(50-40)));
    }
    else if(log_ber < -3.0)
    {
       SQI = (30+((( - 3.0) - log_ber) / (( - 3.0) - ( - 3.5))*(40-30)));
    }
    else if(log_ber < -2.5)
    {
       SQI = (20+((( - 2.5) - log_ber) / (( - 2.5) - ( -3.0))*(30-20)));
}
    else if(log_ber < -2.0)
    {
       SQI = (0+((( - 2.0) - log_ber) / (( - 2.0) - ( - 2.5))*(20-0)));
    }
    else
    {
      SQI = 0;
    }
    return SQI;

}
static MS_U16 _HAL_INTERN_ISDBT_GetSignalQuality(void) /* OK */
{
    float fber;
    //float log_ber;
    MS_BOOL bRet = TRUE;
    EN_ISDBT_Layer LayerIndex;
    MS_U16 SQI;

    // Tmp solution
    LayerIndex = E_ISDBT_Layer_A;

    if(_HAL_INTERN_ISDBT_Check_FEC_Lock() == FALSE)
    {
        //printf("Dan Demod unlock!!!\n");
        SQI = 0; 
    }
    else
    {
        // Part 1: get ber value from demod.
        bRet &= _HAL_INTERN_ISDBT_GetPostViterbiBer(LayerIndex, &fber);
    
        SQI = _HAL_ISDBT_CALCULATE_SQI(fber);
    }
    #if 0
        log_ber = (float)log10((double)fber);
    //printf("dan fber = %f\n", fber);
    //printf("dan log_ber = %f\n", log_ber);
    // Part 2: transfer ber value to SQI value.
    if (log_ber <= ( - 7.0))
    {
        SQI = 100;    //*quality = 100;
    }
    else if(log_ber < -6.0)
    {
         SQI = (90+((( - 6.0) - log_ber) / (( - 6.0) - ( - 7.0))*(100-90)));
    }
    else if(log_ber < -5.5)
    {
         SQI = (80+((( - 5.5) - log_ber) / (( - 5.5) - ( - 6.0))*(90-80)));
    }
    else if(log_ber < -5.0)
    {
         SQI = (70+((( - 5.0) - log_ber) / (( - 5.0) - ( - 5.5))*(80-70)));
    }
    else if(log_ber < -4.5)
    {
         SQI = (60+((( - 4.5) - log_ber) / (( -4.5) - ( - 5.0))*(70-50)));
    }
    else if(log_ber < -4.0)
    {
         SQI = (50+((( - 4.0) - log_ber) / (( - 4.0) - ( - 45))*(60-50)));
    }
    else if(log_ber < -3.5)
    {
         SQI = (40+((( - 3.5) - log_ber) / (( - 3.5) - ( - 4.0))*(50-40)));
    }
    else if(log_ber < -3.0)
    {
         SQI = (30+((( - 3.0) - log_ber) / (( - 3.0) - ( - 3.5))*(40-30)));
    }
    else if(log_ber < -2.5)
    {
         SQI = (20+((( - 2.5) - log_ber) / (( - 2.5) - ( -3.0))*(30-20)));
    }
    else if(log_ber < -2.0)
    {
         SQI = (0+((( - 2.0) - log_ber) / (( - 2.0) - ( - 2.5))*(20-0)));
    }
        else
    {
        SQI = 0;
    }
    #endif
    //printf("dan SQI = %d\n", SQI);
    return SQI;
}

static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerA(void) /* OK */
{
    float fber;
    //float log_ber;
    MS_BOOL bRet = TRUE;
    EN_ISDBT_Layer LayerIndex;
    MS_U16 SQI;

    // Tmp solution
    LayerIndex = E_ISDBT_Layer_A;

    if(_HAL_INTERN_ISDBT_Check_FEC_Lock() == FALSE)
    {
        //printf("Dan Demod unlock!!!\n");
        SQI = 0;   
    }
    else
    {
        // Part 1: get ber value from demod.
        bRet &= _HAL_INTERN_ISDBT_GetPostViterbiBer(LayerIndex, &fber);
    
        SQI = _HAL_ISDBT_CALCULATE_SQI(fber);
    }
    #if 0
    log_ber = (float)log10((double)fber);
    //printf("dan fber = %f\n", fber);
    //printf("dan log_ber = %f\n", log_ber);
    // Part 2: transfer ber value to SQI value.
    if (log_ber <= ( - 7.0))
    {
        SQI = 100;    //*quality = 100;
    }
    else if(log_ber < -6.0)
    {
         SQI = (90+((( - 6.0) - log_ber) / (( - 6.0) - ( - 7.0))*(100-90)));
    }
    else if(log_ber < -5.5)
    {
         SQI = (80+((( - 5.5) - log_ber) / (( - 5.5) - ( - 6.0))*(90-80)));
    }
    else if(log_ber < -5.0)
    {
         SQI = (70+((( - 5.0) - log_ber) / (( - 5.0) - ( - 5.5))*(80-70)));
    }
    else if(log_ber < -4.5)
    {
         SQI = (60+((( - 4.5) - log_ber) / (( -4.5) - ( - 5.0))*(70-50)));
    }
    else if(log_ber < -4.0)
    {
         SQI = (50+((( - 4.0) - log_ber) / (( - 4.0) - ( - 45))*(60-50)));
    }
    else if(log_ber < -3.5)
    {
         SQI = (40+((( - 3.5) - log_ber) / (( - 3.5) - ( - 4.0))*(50-40)));
    }
    else if(log_ber < -3.0)
    {
         SQI = (30+((( - 3.0) - log_ber) / (( - 3.0) - ( - 3.5))*(40-30)));
    }
    else if(log_ber < -2.5)
    {
         SQI = (20+((( - 2.5) - log_ber) / (( - 2.5) - ( -3.0))*(30-20)));
    }
    else if(log_ber < -2.0)
    {
         SQI = (0+((( - 2.0) - log_ber) / (( - 2.0) - ( - 2.5))*(20-0)));
    }
    else
    {
        SQI = 0;
    }
        #endif
    //printf("dan SQI = %d\n", SQI);
    return SQI;
}

static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerB(void) /* OK */
{
    float fber;
    //float log_ber;
    MS_BOOL bRet = TRUE;
    EN_ISDBT_Layer LayerIndex;
    MS_U16 SQI;

    // Tmp solution
    LayerIndex = E_ISDBT_Layer_B;

    if(_HAL_INTERN_ISDBT_Check_FEC_Lock() == FALSE)
    {
        //printf("Dan Demod unlock!!!\n");
        SQI = 0;   
    }
    else
    {
        // Part 1: get ber value from demod.
        bRet &= _HAL_INTERN_ISDBT_GetPostViterbiBer(LayerIndex, &fber);
            
        SQI = _HAL_ISDBT_CALCULATE_SQI(fber);
    }
    #if 0
    log_ber = (float)log10((double)fber);
        //printf("dan fber = %f\n", fber);
        //printf("dan log_ber = %f\n", log_ber);
        // Part 2: transfer ber value to SQI value.
        if (log_ber <= ( - 7.0))
        {
            SQI = 100;    //*quality = 100;
        }
        else if(log_ber < -6.0)
        {
            SQI = (90+((( - 6.0) - log_ber) / (( - 6.0) - ( - 7.0))*(100-90)));
        }
        else if(log_ber < -5.5)
        {
            SQI = (80+((( - 5.5) - log_ber) / (( - 5.5) - ( - 6.0))*(90-80)));
        }
        else if(log_ber < -5.0)
        {
            SQI = (70+((( - 5.0) - log_ber) / (( - 5.0) - ( - 5.5))*(80-70)));
        }
        else if(log_ber < -4.5)
        {
            SQI = (60+((( - 4.5) - log_ber) / (( -4.5) - ( - 5.0))*(70-50)));
        }
        else if(log_ber < -4.0)
        {
            SQI = (50+((( - 4.0) - log_ber) / (( - 4.0) - ( - 45))*(60-50)));
        }
        else if(log_ber < -3.5)
        {
            SQI = (40+((( - 3.5) - log_ber) / (( - 3.5) - ( - 4.0))*(50-40)));
        }
        else if(log_ber < -3.0)
        {
            SQI = (30+((( - 3.0) - log_ber) / (( - 3.0) - ( - 3.5))*(40-30)));
        }
        else if(log_ber < -2.5)
        {
            SQI = (20+((( - 2.5) - log_ber) / (( - 2.5) - ( -3.0))*(30-20)));
        }
        else if(log_ber < -2.0)
        {
            SQI = (0+((( - 2.0) - log_ber) / (( - 2.0) - ( - 2.5))*(20-0)));
        }
        else
        {
            SQI = 0;
        }
    #endif
    //printf("dan SQI = %d\n", SQI);
    return SQI;
}

static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerC(void) /* OK */
{
    float fber;
    //float log_ber;
    MS_BOOL bRet = TRUE;
    EN_ISDBT_Layer LayerIndex;
    MS_U16 SQI;

    // Tmp solution
    LayerIndex = E_ISDBT_Layer_C;

    if(_HAL_INTERN_ISDBT_Check_FEC_Lock() == FALSE)
    {
        //printf("Dan Demod unlock!!!\n");
        SQI = 0; 
    }
    else
    {
        // Part 1: get ber value from demod.
        bRet &= _HAL_INTERN_ISDBT_GetPostViterbiBer(LayerIndex, &fber);
        
        SQI = _HAL_ISDBT_CALCULATE_SQI(fber);
    }
    #if 0
    log_ber = (float)log10((double)fber);
    //printf("dan fber = %f\n", fber);
    //printf("dan log_ber = %f\n", log_ber);
    // Part 2: transfer ber value to SQI value.
    if (log_ber <= ( - 7.0))
    {
        SQI = 100;    //*quality = 100;
    }
    else if(log_ber < -6.0)
    {
         SQI = (90+((( - 6.0) - log_ber) / (( - 6.0) - ( - 7.0))*(100-90)));
    }
    else if(log_ber < -5.5)
    {
         SQI = (80+((( - 5.5) - log_ber) / (( - 5.5) - ( - 6.0))*(90-80)));
    }
    else if(log_ber < -5.0)
    {
         SQI = (70+((( - 5.0) - log_ber) / (( - 5.0) - ( - 5.5))*(80-70)));
    }
    else if(log_ber < -4.5)
    {
         SQI = (60+((( - 4.5) - log_ber) / (( -4.5) - ( - 5.0))*(70-50)));
    }
    else if(log_ber < -4.0)
    {
         SQI = (50+((( - 4.0) - log_ber) / (( - 4.0) - ( - 45))*(60-50)));
    }
    else if(log_ber < -3.5)
    {
         SQI = (40+((( - 3.5) - log_ber) / (( - 3.5) - ( - 4.0))*(50-40)));
    }
    else if(log_ber < -3.0)
    {
         SQI = (30+((( - 3.0) - log_ber) / (( - 3.0) - ( - 3.5))*(40-30)));
    }
    else if(log_ber < -2.5)
    {
         SQI = (20+((( - 2.5) - log_ber) / (( - 2.5) - ( -3.0))*(30-20)));
    }
    else if(log_ber < -2.0)
    {
         SQI = (0+((( - 2.0) - log_ber) / (( - 2.0) - ( - 2.5))*(20-0)));
    }
    else
    {
        SQI = 0;
    }
    #endif
    //printf("dan SQI = %d\n", SQI);
    return SQI;
}


static MS_BOOL _HAL_INTERN_ISDBT_IIC_Bypass_Mode(MS_BOOL bEnable) /* OK */
{
    /* This function is not available for internal demod */
    return FALSE;
}

static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerCombine(void)
{
    MS_S8  s8LayerAValue = 0, s8LayerBValue = 0, s8LayerCValue = 0;
    MS_U16 SQI;
    EN_ISDBT_Layer eLayerIndex; 
    EN_ISDBT_CONSTEL_TYPE eIsdbtConstellationA, eIsdbtConstellationB, eIsdbtConstellationC;
    //Get modulation of each layer
    eLayerIndex = E_ISDBT_Layer_A;
    _HAL_INTERN_ISDBT_GetSignalModulation(eLayerIndex, &eIsdbtConstellationA);	
    eLayerIndex = E_ISDBT_Layer_B;
    _HAL_INTERN_ISDBT_GetSignalModulation(eLayerIndex, &eIsdbtConstellationB);
    eLayerIndex = E_ISDBT_Layer_C;
    _HAL_INTERN_ISDBT_GetSignalModulation(eLayerIndex, &eIsdbtConstellationC);
    
    if(eIsdbtConstellationA != E_ISDBT_QAM_INVALID)
        s8LayerAValue = (MS_S8)eIsdbtConstellationA;
    else
        s8LayerAValue = -1;
    	       
    if(eIsdbtConstellationB != E_ISDBT_QAM_INVALID)
        s8LayerBValue = (MS_S8)eIsdbtConstellationB;
    else
        s8LayerBValue = -1;
        
    if(eIsdbtConstellationC != E_ISDBT_QAM_INVALID)
        s8LayerCValue = (MS_S8)eIsdbtConstellationC;
    else
        s8LayerCValue = -1;
    	   
    //printf("Layer info A:%d, B:%d, C:%d\n", s8LayerAValue, s8LayerBValue, s8LayerCValue);	   
    if(s8LayerAValue >= s8LayerBValue)
    {
        if(s8LayerCValue >= s8LayerAValue)
        {
            //Get Layer C SQI	
            SQI = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerC();
            //printf("dan SQI Layer C1: %d\n", SQI);
        }	
        else  //A>C
        {
            //Get Layer A SQI
            SQI = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerA();
            //printf("dan SQI Layer A: %d\n", SQI);		
        }		
    }
    else  // B >= A
    {
        if(s8LayerCValue >= s8LayerBValue)
        {
            //Get Layer C SQI	
            SQI = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerC();
            //printf("dan SQI Layer C2: %d\n", SQI);
        }	
        else  //B>C
        {
            //Get Layer B SQI
            SQI = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerB();
            //printf("dan SQI Layer B: %d\n", SQI);
        }			
    }
    
    return SQI;
}

static MS_BOOL _HAL_INTERN_ISDBT_Exit(void) /* OK */
{

    MS_U8 u8CheckCount = 0;
    //Notify firmware before reset by mailbox
    
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x1C, 0x01);
    
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);
    
    // assert interrupt to VD MCU51                           
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02));
    
    while ((HAL_DMD_RIU_ReadByte(MBRegBase + 0x1C)&0x02) != 0x02)
    {
        MsOS_DelayTaskUs(10);

       if (u8CheckCount++ == 0xFF)
        {
            printf(">> ISDBT Exit Fail!\n");
            return FALSE;
        }
    }
    printf(">> ISDBT Exit Ok!\n");
 
    HAL_DMD_RIU_WriteByte(0x111f0b, 0x01);  // reg_ckg_atsc_adcd_sync
    HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f0d, 0x01);  // reg_ckg_dvbtc_inner2x
    HAL_DMD_RIU_WriteByte(0x111f0c, 0x01);  // reg_ckg_dvbtc_inner1x
    HAL_DMD_RIU_WriteByte(0x111f0f, 0x01);  // reg_ckg_dvbtc_inner4x
    HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f11, 0x01);  // reg_ckg_dvbtc_outer2x
    HAL_DMD_RIU_WriteByte(0x111f10, 0x01);  // reg_ckg_dvbtc_outer1x
    HAL_DMD_RIU_WriteByte(0x111f13, 0x01);  // reg_ckg_dvbtc_outer2x_c
    HAL_DMD_RIU_WriteByte(0x111f12, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f19, 0x51);  // 
    HAL_DMD_RIU_WriteByte(0x111f18, 0x11);  // reg_ckg_dvbtc_sram0 ~4
    HAL_DMD_RIU_WriteByte(0x111f43, 0x01);  // 
    HAL_DMD_RIU_WriteByte(0x111f42, 0x11);  // reg_ckg_isdbt_inner1x ~4x
    HAL_DMD_RIU_WriteByte(0x111f45, 0x11);  // 
    HAL_DMD_RIU_WriteByte(0x111f44, 0x11);  // reg_ckg_isdbt_outer1x ~12x
    HAL_DMD_RIU_WriteByte(0x111f49, 0x11);  // reg_ckg_adc1x_eq1x 0p5x
    HAL_DMD_RIU_WriteByte(0x111f48, 0x11);  // reg_ckg_dvbtc_sram4_isdbt_inner4x 6x
    HAL_DMD_RIU_WriteByte(0x111f4b, 0x11);  // reg_ckg_isdbt_outer6x_dvbt_outer2x 2xc
    HAL_DMD_RIU_WriteByte(0x111f4a, 0x11);  // reg_ckg_isdbt_outer6x_dvbt_inner1x 2x
    HAL_DMD_RIU_WriteByte(0x111f4c, 0x11);  // reg_ckg_isdbt_inner2x_dvbt_inner2x reg_ckg_isdbt_outer_rs
    HAL_DMD_RIU_WriteByte(0x111f23, 0x11);  // reg_ckg_acifir reg_ckg_frontend_d2
    HAL_DMD_RIU_WriteByte(0x111f22, 0x11);  // reg_ckg_frontend reg_ckg_tr

    HAL_DMD_RIU_WriteByte(0x103314, 0x01);   // reg_ckg_dvbtc_adc
    HAL_DMD_RIU_WriteByte(0x103315, 0x01);   // 
    HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // reg_ckg_atsc_ts
    HAL_DMD_RIU_WriteByte(0x103309, 0x01);   // reg_ckg_atsc_ts

    return TRUE;  
}

static MS_BOOL _HAL_INTERN_ISDBT_SWITCH_SSPI_GPIO(MS_BOOL bEnable) /* OK */
{
    /* This function is not available for internal demod */
    return FALSE;
}
static MS_BOOL _HAL_INTERN_ISDBT_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel) /* OK */
{
    /* This function is not available for internal demod */   
    return FALSE;
}

static MS_BOOL _HAL_INTERN_ISDBT_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel) /* OK */
{
    /* This function is not available for internal demod */   
    return FALSE;
}

static MS_BOOL _HAL_INTERN_ISDBT_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut) /* OK */
{
    /* This function is not available for internal demod */   
    return FALSE;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetPostViterbiBer(EN_ISDBT_Layer LayerIndex, float *ber) /* OK */
{
    MS_BOOL bRet = TRUE;
    MS_U8     u8Data, u8FrzData;
    MS_U32    u32BerValue = 0;
    MS_U16    u16BerPeriod = 0;
    // reg_rd_freezeber
    //bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x02, &u8Data);
    //bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE + 0x02, u8Data|0x01);
    
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE+0x01*2+1, &u8FrzData);
    u8Data = u8FrzData | 0x01;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE+0x01*2+1, u8Data);
    
    if(LayerIndex == E_ISDBT_Layer_A)
    {
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x14, &u8Data);  //0x0A * 2
        u32BerValue = u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x15, &u8Data);  //0x0A * 2+1
        u32BerValue |= u8Data << 8;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x16, &u8Data);  //0x0B * 2
        u32BerValue |= u8Data << 16;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x17, &u8Data);  //0x0B * 2+1
        u32BerValue |= u8Data << 24;

        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x0A, &u8Data);  //0x05 * 2
        u16BerPeriod = u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x0B, &u8Data);  //0x05 * 2+1
        u16BerPeriod |= u8Data << 8;

    }
    else if(LayerIndex == E_ISDBT_Layer_B)
    {
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x46, &u8Data);  //0x23 * 2
        u32BerValue = u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x47, &u8Data);  //0x23 * 2+1
        u32BerValue |= u8Data << 8;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x48, &u8Data);  //0x24 * 2
        u32BerValue |= u8Data << 16;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x49, &u8Data);  //0x24 * 2+1
        u32BerValue |= u8Data << 24;

        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x3A, &u8Data);  //0x1d * 2
        u16BerPeriod = u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x3B, &u8Data);  //0x1d * 2+1
        u16BerPeriod |= u8Data << 8;
    }
    else if(LayerIndex == E_ISDBT_Layer_C)
    {
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x88, &u8Data);  //0x44 * 2
        u32BerValue = u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x89, &u8Data);  //0x44 * 2+1
        u32BerValue |= u8Data << 8;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x8A, &u8Data);  //0x45 * 2
        u32BerValue |= u8Data << 16;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x8B, &u8Data);  //0x45 * 2+1
        u32BerValue |= u8Data << 24;

        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x3E, &u8Data);  //0x1f * 2
        u16BerPeriod = u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x3F, &u8Data);  //0x1d * 2+1
        u16BerPeriod |= u8Data << 8;
    }
    else
    {
        HAL_INTERN_ISDBT_DBINFO(printf("Please select correct Layer\n"));
        bRet = FALSE;
    }

    // reg_rd_freezeber
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE+0x01*2+1, u8FrzData);

    if(u16BerPeriod == 0) u16BerPeriod = 1;
        *ber = (float)u32BerValue/u16BerPeriod/(128.0*188.0*8.0);
        
    HAL_INTERN_ISDBT_DBINFO(printf("Layer: 0x%x, Post-Ber = %e\n", LayerIndex, *ber));

    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetPreViterbiBer(EN_ISDBT_Layer LayerIndex, float *ber) /* OK */
{
    MS_BOOL bRet = TRUE;
    MS_U8     u8Data;
    MS_U16   u16BerValue = 0;
    MS_U32   u32BerPeriod = 0;
    // reg_rd_freezeber
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x60, &u8Data);
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE + 0x60, u8Data|0x08);

    if(LayerIndex == E_ISDBT_Layer_A)
    {
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x90, &u8Data);  //0x48 * 2
        u16BerValue=u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x91, &u8Data);  //0x48 * 2+1
        u16BerValue |= (u8Data << 8);
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x76, &u8Data); //0x3b * 2
        u32BerPeriod = (u8Data&0x3F);
        u32BerPeriod <<= 16;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x70, &u8Data); //0x38 * 2
        u32BerPeriod |= u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x70, &u8Data); //0x38 * 2 +1
        u32BerPeriod |= (u8Data << 8);
    }
    else if(LayerIndex == E_ISDBT_Layer_B)
    {
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x92, &u8Data);  //0x49 * 2
        u16BerValue=u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x93, &u8Data);  //0x49 * 2+1
        u16BerValue |= (u8Data << 8);
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x77, &u8Data); //0x3b * 2 + 1
        u32BerPeriod = (u8Data&0x3F);
        u32BerPeriod <<= 16;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x72, &u8Data); //0x39 * 2
        u32BerPeriod |= u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x73, &u8Data); //0x39 * 2 +1
        u32BerPeriod |= (u8Data << 8);
    }
    else if(LayerIndex == E_ISDBT_Layer_C)
    {
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x94, &u8Data);  //0x4A * 2
        u16BerValue=u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x95, &u8Data);  //0x4A * 2+1
        u16BerValue |= (u8Data << 8);
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x78, &u8Data); //0x3C
        u32BerPeriod = (u8Data&0x003F);
        u32BerPeriod <<= 16;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x74, &u8Data); //0x3A * 2
        u32BerPeriod |= u8Data;
        bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x75, &u8Data); //0x3A * 2 +1
        u32BerPeriod |= (u8Data << 8);
    }
    else
    {
        HAL_INTERN_ISDBT_DBINFO(printf("Please select correct Layer\n"));
        bRet = FALSE;
    }

    // reg_rd_freezeber
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE + 0x60, &u8Data);
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE + 0x60, (u8Data&~0x08));

    u32BerPeriod <<= 8; // *256
    if(u32BerPeriod == 0) u32BerPeriod = 1;
        *ber = (float)u16BerValue/u32BerPeriod;

    HAL_INTERN_ISDBT_DBINFO(printf("Layer: 0x%x, Pre-Ber = %e\n", LayerIndex, *ber));
    return bRet;

}

static MS_BOOL _HAL_INTERN_ISDBT_GetFreqOffset(float *pFreqOff) /* OK */
{
    MS_BOOL bRet = TRUE;
    MS_U8     u8Data;
    MS_S32   s32TdCfoRegValue = 0;
    float        fTdCfoFreq = 0.0;
    MS_S32   s32FdCfoRegValue = 0;
    float        fFdCfoFreq = 0.0;
    MS_S16   s16IcfoRegValue = 0.0;
    float        fICfoFreq = 0.0;
    //Get TD CFO
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE + 0x04, &u8Data);   //0x02 * 2
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_TDP_REG_BASE + 0x04, (u8Data|0x01));

    //read td_freq_error
    //Read <29,38>
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE + 0x8A, &u8Data);   //0x45 * 2
    s32TdCfoRegValue = u8Data;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE + 0x8B, &u8Data);   //0x45 * 2 + 1
    s32TdCfoRegValue |= u8Data << 8;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE + 0x8C, &u8Data);   //0x46 * 2
    s32TdCfoRegValue = u8Data << 16;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE + 0x8D, &u8Data);   //0x46 * 2 + 1
    s32TdCfoRegValue |= u8Data << 24;

    if(u8Data >= 0x10)
        s32TdCfoRegValue = 0xE0000000 | s32TdCfoRegValue;
    s32TdCfoRegValue >>=4;
    //TD_cfo_Hz = RegCfoTd * fb
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE + 0x04, &u8Data);   //0x02 * 2
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_TDP_REG_BASE + 0x04, (u8Data&~0x01));

    fTdCfoFreq = ((float)s32TdCfoRegValue) / 17179869184.0; //<25,34>
    fTdCfoFreq = fTdCfoFreq * 8126980.0;
    
    //Get FD CFO
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0xFE, &u8Data);   //0x7f * 2
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FDP_REG_BASE + 0xFE, (u8Data|0x01));
    //load
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0xFF, &u8Data);   //0x7f * 2 + 1
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FDP_REG_BASE + 0xFF, (u8Data|0x01));

    //read CFO_KI
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0x5E, &u8Data);   //0x2F * 2
    s32FdCfoRegValue = u8Data;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0x5F, &u8Data);   //0x2F * 2 + 1
    s32FdCfoRegValue |= u8Data << 8;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0x60, &u8Data);   //0x30 * 2
    s32FdCfoRegValue |= u8Data << 16;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0x61, &u8Data);   //0x30 * 2
    s32FdCfoRegValue |= u8Data << 24;
    if(u8Data >= 0x01)
        s32FdCfoRegValue = 0xFE000000 | s32FdCfoRegValue;
    
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0xFE, &u8Data);   //0x7f * 2
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FDP_REG_BASE + 0xFE, (u8Data&~0x01));
    //load
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0xFF, &u8Data);   //0x7f * 2 + 1
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FDP_REG_BASE + 0xFF, (u8Data|0x01));

    fFdCfoFreq = ((float)s32FdCfoRegValue) / 17179869184.0;
    fFdCfoFreq = fFdCfoFreq * 8126980.0;

    //Get ICFO
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0x5C, &u8Data);   //0x2E * 2
    s16IcfoRegValue = u8Data;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0x5D, &u8Data);   //0x2E * 2 + 1
    s16IcfoRegValue |= u8Data << 8;
    s16IcfoRegValue = (s16IcfoRegValue >> 4) & 0x07FF;
    if(s16IcfoRegValue >= 0x400)
        s16IcfoRegValue = s16IcfoRegValue | 0xFFFFF800;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_TDP_REG_BASE + 0x68, &u8Data);   //0x34 * 2

    if((u8Data & 0x30) == 0x0000) // 2k
        fICfoFreq = (float)s16IcfoRegValue*250000.0/63.0;
    else if((u8Data & 0x0030) == 0x0010)	// 4k
        fICfoFreq = (float)s16IcfoRegValue*125000.0/63.0;
    else //if(u16data & 0x0030 == 0x0020) // 8k
        fICfoFreq = (float)s16IcfoRegValue*125000.0/126.0;

    
    *pFreqOff = fTdCfoFreq + fFdCfoFreq + fICfoFreq;

    HAL_INTERN_ISDBT_DBINFO(printf("Total CFO value = %f\n", *pFreqOff));
    
    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSNR(float *pf_snr) /* OK */
{
    MS_U8 bRet = true;
    MS_U8 u8Data;
    MS_U32 u32RegSNR = 0;
    MS_U16 u16RegSnrObsNum = 0;
    float SNRAvg = 0.0;

    //set freeze
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0xFE, &u8Data);   //0x7f * 2
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FDP_REG_BASE + 0xFE, (u8Data|0x01));
    //load
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0xFF, &u8Data);   //0x7f * 2 + 1
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FDP_REG_BASE + 0xFF, (u8Data|0x01));

    // ==============Average SNR===============//
    // [26:0] reg_snr_accu
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2d*2+1, &u8Data);
    u32RegSNR = u8Data&0x07;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2d*2, &u8Data);
    u32RegSNR = (u32RegSNR<<8) | u8Data;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2c*2+1, &u8Data);
    u32RegSNR = (u32RegSNR<<8) | u8Data;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2c*2, &u8Data);
    u32RegSNR = (u32RegSNR<<8) | u8Data;

    // [12:0] reg_snr_observe_sum_num
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2a*2+1, &u8Data);
    u16RegSnrObsNum = u8Data&0x1f;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2a*2, &u8Data);
    u16RegSnrObsNum = (u16RegSnrObsNum<<8) | u8Data;

    //release freeze
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0xFE, &u8Data);   //0x7f * 2
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FDP_REG_BASE + 0xFE, (u8Data&~0x01));
    //load
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_FDP_REG_BASE + 0xFF, &u8Data);   //0x7f * 2 + 1
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_FDP_REG_BASE + 0xFF, (u8Data|0x01));

    if (u16RegSnrObsNum == 0)
        u16RegSnrObsNum = 1;

    SNRAvg = (float)u32RegSNR/u16RegSnrObsNum;

    if (SNRAvg == 0)                 //protect value 0
      SNRAvg=0.01;

    
#ifdef MSOS_TYPE_LINUX
    *pf_snr =10*(float)log10f((double)SNRAvg/2);
#else
    *pf_snr =10*(float)sMdrvDmdInterfaceFunc.Log10Approx((double)SNRAvg/2);
#endif

    HAL_INTERN_ISDBT_DBINFO(printf("SNR value = %f\n", *pf_snr));
    //printf("SNR value = %f\n", *pf_snr);
    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_TURN_ON_DOWNLOAD_FW_FLOW(void)
{
    downloaded = FALSE;
    return TRUE;  
}

static MS_BOOL _HAL_INTERN_ISDBT_CHECK_FW_STATUS(void)
{
	MS_BOOL bIs_dtvmode;
	MS_U8 u8data, u8val;
	
	u8val = HAL_DMD_RIU_ReadByte(0x101e39);
	HAL_DMD_RIU_WriteByte(0x101e39, (u8val & 0xFC));
    u8data = HAL_DMD_RIU_ReadByte(0x112000);
    HAL_DMD_RIU_WriteByte(0x101e39, u8val);
	
	if((u8data & 0x10) == 0x10)
	{
        printf("\n VT: (Check Suspend in Hal:0x%x) It is non DTV Mode Now!\n",u8data);
		bIs_dtvmode=0; 		
	}	
	else
	{	 
		bIs_dtvmode=1;
		printf("\n VT: (Check Suspend in Hal:0x%x) It is DTV Mode Now!\n",u8data);		
	}	
	
  return bIs_dtvmode;

}
static MS_BOOL _HAL_INTERN_ISDBT_TS_INTERFACE_CONFIG(MS_U8 u8TsConfigData) /* OK */
{

    MS_BOOL  bResult = TRUE;
    MS_U8      u8TmpData, u8ReadData;
    MS_U16      u16TSConfigDataAddr;
    //Set Data swap
    u8TmpData = (u8TsConfigData & 0x02) >> 1;
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE +0xA0, &u8ReadData);   
    if(u8TmpData == 0x01)  //Serial mode
        u8ReadData = u8ReadData | 0x10;
    else
        u8ReadData = u8ReadData & ~0x10;
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE+0xA0, u8ReadData);

    //Set Serial mode
    u8TmpData = u8TsConfigData & 0x01;
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(ISDBT_OUTER_REG_BASE +0xA0, &u8ReadData);   
    if(u8TmpData == 0x01)  //Serial mode
    {
        u8ReadData = u8ReadData | 0x02;
        bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE+0xA0, u8ReadData);
        HAL_DMD_RIU_WriteByte(0x103300+0x08, 0x01); // TS clock setting
        HAL_DMD_RIU_WriteByte(0x103300+0x09, 0x04);
        HAL_DMD_RIU_WriteByte(0x103300+0x00, 0x00); // TS div num
        u8ReadData = HAL_DMD_RIU_ReadByte(0x103300+0x01);
        u8ReadData |= 0x04;
        HAL_DMD_RIU_WriteByte(0x103300+0x01, u8ReadData);
        
    }
    else
    {
        u8ReadData = u8ReadData & ~0x02;
        bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(ISDBT_OUTER_REG_BASE+0xA0, u8ReadData);
        HAL_DMD_RIU_WriteByte(0x103300+0x08, 0x01); // TS clock setting
        HAL_DMD_RIU_WriteByte(0x103300+0x09, 0x00);
        HAL_DMD_RIU_WriteByte(0x103300+0x00, u8TmpData); // TS div num
        u8ReadData = HAL_DMD_RIU_ReadByte(0x103300+0x01);
        u8ReadData |= 0x07;
        HAL_DMD_RIU_WriteByte(0x103300+0x01, u8ReadData);
    }
    
    //Set TS config data to Xdata
    bResult = _HAL_INTERN_ISDBT_READ_MBX(0x0080, &u8ReadData);
    u16TSConfigDataAddr = u8ReadData << 8;
    bResult = _HAL_INTERN_ISDBT_READ_MBX(0x0081, &u8ReadData);
    u16TSConfigDataAddr = u8ReadData;
    
    bResult &= _HAL_INTERN_ISDBT_WRITE_MBX(u16TSConfigDataAddr, &u8TsConfigData);

    return bResult;
}

static MS_BOOL _HAL_INTERN_ISDBT_WRITE_MBX(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bResult = TRUE;

/*  MS_U8     u8ReadData, u8Cnt;
   
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(0x0B02, (MS_U8)u16Addr);        //Reg
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(0x0B03, (MS_U8)(u16Addr >> 8));        //Reg
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(0x0B01, 0x01);           //write Length
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(0x0B50, *(pu8Data));
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(0x0B00, 0x01);          //Trigger write operation
    u8ReadData = 0x00;
    u8Cnt = 0;
    while( (sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(0x0B00, &u8ReadData)== TRUE ) && ((u8ReadData&0x01)==0x01))
    {
        if (u8Cnt>100)
        {
            printf("_HAL_INTERN_ISDBT_WRITE_MBX FAIL!!!\n");
            return FALSE;
        }
        u8Cnt++;
        MsOS_DelayTask(1);
    }

*/
    return bResult;
}

static MS_BOOL _HAL_INTERN_ISDBT_READ_MBX(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bResult = TRUE;

/*    MS_U8     u8ReadData, u8Cnt;

    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(0x0B02, (MS_U8)u16Addr);        //Reg
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(0x0B03, (MS_U8)(u16Addr >> 8));        //Reg
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(0x0B01, 0x01);           //write Length
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg(0x0B00, 0x02);          //Trigger read operation
    u8ReadData = 0x00;
    u8Cnt = 0;
    while( (sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(0x0B00, &u8ReadData)== TRUE ) && ((u8ReadData&0x02)==0x02))
    {
        if (u8Cnt>100)
        {
            printf("_HAL_INTERN_ISDBT_READ_MBX FAIL!!!\n");
            return false;
        }
        u8Cnt++;
        MsOS_DelayTask(1);
    }
    
    u8ReadData = 0x00;
    bResult &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(0x0B50, &u8ReadData);
    *pu8Data = u8ReadData;
*/
    return bResult;

}

MS_BOOL HAL_INTERN_ISDBT_IOCTL_CMD(e_ISDBT_HAL_COMMAND eCmd, void *pHandle)
{
    MS_BOOL  bResult = TRUE;

    switch(eCmd)
    {
        case CMD_ISDBT_Exit:
            bResult = _HAL_INTERN_ISDBT_Exit();
            break;
        case CMD_ISDBT_InitClk:
            bResult = _HAL_INTERN_ISDBT_InitClk();
            break;
        case CMD_ISDBT_Download:
            bResult = _HAL_INTERN_ISDBT_Download();
            break;
        case CMD_ISDBT_SoftReset:
            bResult = _HAL_INTERN_ISDBT_SoftReset();
            break;
        case CMD_ISDBT_SetModeClean:
            bResult = _HAL_INTERN_ISDBT_SetModeClean();
            break;
        case CMD_ISDBT_Active:
            bResult = _HAL_INTERN_ISDBT_Active();
            break;
        case CMD_ISDBT_Check_FEC_Lock:
            bResult = _HAL_INTERN_ISDBT_Check_FEC_Lock();
            break;
        case CMD_ISDBT_Check_FSA_TRACK_Lock:
            bResult = _HAL_INTERN_ISDBT_Check_FSA_TRACK_Lock();
            break;
        case CMD_ISDBT_Check_PSYNC_Lock:
            bResult = _HAL_INTERN_ISDBT_Check_PSYNC_Lock();
            break;
        case CMD_ISDBT_Check_ICFO_CH_EXIST_Lock:
            bResult = _HAL_INTERN_ISDBT_Check_ICFO_CH_EXIST_Lock();
            break;
        case CMD_ISDBT_Read_PKT_ERR:
            {
                s_ISDBT_GET_PKT_ERR *psIsdbtGetPktErr = (s_ISDBT_GET_PKT_ERR *)pHandle;
                bResult = _HAL_INTERN_ISDBT_Read_PKT_ERR(psIsdbtGetPktErr->eIsdbtLayer, &psIsdbtGetPktErr->u16PacketErr);
            }
            break;
        case CMD_ISDBT_FWVERSION:
            *((MS_U16*)pHandle) = _HAL_INTERN_ISDBT_FWVERSION();
            break;
        case CMD_ISDBT_GetSignalCodeRate:
            {
                s_ISDBT_GET_CodeRate *psIsdbtGetCodeRate = (s_ISDBT_GET_CodeRate *)pHandle;
                bResult = _HAL_INTERN_ISDBT_GetSignalCodeRate(psIsdbtGetCodeRate->eIsdbtLayer, &psIsdbtGetCodeRate->eCodeRate);
            }
            break;
        case CMD_ISDBT_GetSignalGuardInterval:
            bResult = _HAL_INTERN_ISDBT_GetSignalGuardInterval((EN_ISDBT_GUARD_INTERVAL *)pHandle);
            break;
        case CMD_ISDBT_GetSignalTimeInterleaving:
            {
                s_ISDBT_GET_TimeInterleaving *psIsdbtGetTimeInterleaving = (s_ISDBT_GET_TimeInterleaving *)pHandle;
                _HAL_INTERN_ISDBT_GetSignalTimeInterleaving(psIsdbtGetTimeInterleaving->eIsdbtLayer, &psIsdbtGetTimeInterleaving->eTimeInterleaving);
            }
            break;
        case CMD_ISDBT_GetSignalFFTValue:
            bResult = _HAL_INTERN_ISDBT_GetSignalFFTValue((EN_ISDBT_FFT_VAL *)pHandle);
            break;
        case CMD_ISDBT_GetSignalModulation:
            {
                s_ISDBT_GET_MODULATION  *psIsdbtGetModulation = (s_ISDBT_GET_MODULATION*)pHandle;
                _HAL_INTERN_ISDBT_GetSignalModulation(psIsdbtGetModulation->eIsdbtLayer, &psIsdbtGetModulation->eConstellation);
            }
            break;
        case CMD_ISDBT_GetSignalStrength:
            *((MS_U16*)pHandle) = _HAL_INTERN_ISDBT_GetSignalStrength();
            break;
        case CMD_ISDBT_GetSignalQuality:
            *((MS_U16*)pHandle) = _HAL_INTERN_ISDBT_GetSignalQuality();
            break;
        case CMD_ISDBT_GetSignalQualityOfLayerA:
            *((MS_U16*)pHandle) = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerA();
            break;    
        case CMD_ISDBT_GetSignalQualityOfLayerB:
            *((MS_U16*)pHandle) = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerB();
            break;
        case CMD_ISDBT_GetSignalQualityOfLayerC:
            *((MS_U16*)pHandle) = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerC();
            break;  
        case CMD_ISDBT_GetSignalQualityCombine:
            *((MS_U16*)pHandle) = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerCombine();
            break;            
        case CMD_ISDBT_IIC_Bypass_Mode:
            {
                MS_BOOL *pEnable = (MS_BOOL *)pHandle;
                bResult = _HAL_INTERN_ISDBT_IIC_Bypass_Mode(*pEnable);  
            }
            break;
        case CMD_ISDBT_SWITCH_SSPI_GPIO:
            {
                MS_BOOL *pEnable = (MS_BOOL *)pHandle;
                bResult = _HAL_INTERN_ISDBT_SWITCH_SSPI_GPIO(*pEnable);  
            }
            break;
        case CMD_ISDBT_GPIO_GET_LEVEL:
            {
                s_ISDBT_GPIO_INTERFACE *psIsdbtGpioInterface = (s_ISDBT_GPIO_INTERFACE *)pHandle;
                bResult = _HAL_INTERN_ISDBT_GPIO_GET_LEVEL(psIsdbtGpioInterface->u8Pin, &psIsdbtGpioInterface->bLevel);
            }
            break;
        case CMD_ISDBT_GPIO_SET_LEVEL:
            {
                s_ISDBT_GPIO_INTERFACE *psIsdbtGpioInterface = (s_ISDBT_GPIO_INTERFACE *)pHandle;
                bResult = _HAL_INTERN_ISDBT_GPIO_SET_LEVEL(psIsdbtGpioInterface->u8Pin, psIsdbtGpioInterface->bLevel);
            }
            break;
        case CMD_ISDBT_GPIO_OUT_ENABLE:
            {
                s_ISDBT_GPIO_INTERFACE *psIsdbtGpioInterface = (s_ISDBT_GPIO_INTERFACE *)pHandle;
                bResult = _HAL_INTERN_ISDBT_GPIO_OUT_ENABLE(psIsdbtGpioInterface->u8Pin, psIsdbtGpioInterface->bLevel);
            }
            break;
        case CMD_ISDBT_GetPostViterbiBer:
            {
                s_ISDBT_GET_BER_VALUE  *psIsdbtGetBerValue = (s_ISDBT_GET_BER_VALUE *)pHandle; 
                bResult = _HAL_INTERN_ISDBT_GetPostViterbiBer(psIsdbtGetBerValue->eIsdbtLayer, &psIsdbtGetBerValue->fBerValue);
            }
            break;
        case CMD_ISDBT_GetPreViterbiBer:
            {
                s_ISDBT_GET_BER_VALUE  *psIsdbtGetBerValue = (s_ISDBT_GET_BER_VALUE *)pHandle; 
                bResult = _HAL_INTERN_ISDBT_GetPreViterbiBer(psIsdbtGetBerValue->eIsdbtLayer, &psIsdbtGetBerValue->fBerValue);
            }
            break;
        case CMD_ISDBT_GetFreqOffset:
            bResult = _HAL_INTERN_ISDBT_GetFreqOffset((float *)pHandle);
            break;
        case CMD_ISDBT_GetSNR:
            bResult = _HAL_INTERN_ISDBT_GetSNR((float *)pHandle);  
            break;
        case CMD_ISDBT_TS_INTERFACE_CONFIG:
            {
                MS_U8 *pTsConfigData = (MS_U8 *)pHandle;
                bResult = _HAL_INTERN_ISDBT_TS_INTERFACE_CONFIG(*pTsConfigData);  
            }
            break;
        case CMD_ISDBT_TURN_ON_DOWNLOAD_FW_FLOW:
            bResult = _HAL_INTERN_ISDBT_TURN_ON_DOWNLOAD_FW_FLOW();
            break;    
        case CMD_ISDBT_CHECK_FW_STATUS:
        	bResult = _HAL_INTERN_ISDBT_CHECK_FW_STATUS();
        	break;
        default:
            break;
    }

    return bResult;
}

MS_BOOL MDrv_DMD_ISDBT_Initial_Hal_Interface(void)
{
    sISDBT_HAL_FUNCTION_TABLE.HAL_DMD_ISDBT_IOCTL_CMD = HAL_INTERN_ISDBT_IOCTL_CMD;

    sMdrvDmdInterfaceFunc.MDrv_DMD_I2C_Channel_Change  = MDrv_DMD_I2C_Channel_Change;
    sMdrvDmdInterfaceFunc.MDrv_DMD_I2C_Channel_Set        = MDrv_DMD_I2C_Channel_Set;
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg                     = MDrv_DMD_ReadReg;
    sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg                     = MDrv_DMD_WriteReg;
    sMdrvDmdInterfaceFunc.MDrv_DMD_WriteRegs                   = MDrv_DMD_WriteRegs; 
    sMdrvDmdInterfaceFunc.Log10Approx                                 = Log10Approx;

    return TRUE;
}

MS_BOOL MDrv_INTERNAL_DMD_ISDBT_Link_Func(void)
{
    return  MDrv_DMD_ISDBT_Initial_Hal_Interface();
}

   #if SHOW_DEBUG_INFORMATION
static MS_BOOL _HAL_INTERN_ISDBT_GetTimingOffset(float *pFreqOff)
{

    return TRUE;
}

static MS_BOOL _HAL_INTERN_ISDBT_ChannelProfile(MS_U8 *pu8FdIndex)
{
    MS_BOOL   bRet = TRUE;
    MS_U8       u8Data;
    bRet &= sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg(0x3600 + 0xF2, &u8Data);  //0x79 * 2
    *pu8FdIndex = u8Data & 0x1F;
    
    return bRet;
}
static MS_BOOL _HAL_INTERN_ISDBT_DEBUG_INFORMATION(MS_BOOL bFecLock)
{
    
    MS_BOOL bIcfoLock;
    
    float         fBer;
    float         fSnr;
    MS_U16    u16PacketErr;
    float         fOffsetValue;
    MS_U8     u8FdIndex;
    EN_ISDBT_GUARD_INTERVAL   eGIValue;
    EN_ISDBT_FFT_VAL                  eTransmissionMode;
    EN_ISDBT_CODE_RATE            eCodeRate;
    EN_ISDBT_TIME_INTERLEAVING  eTI;
    EN_ISDBT_CONSTEL_TYPE          eConstellation;
    
    bIcfoLock = _HAL_INTERN_ISDBT_Check_ICFO_CH_EXIST_Lock();

    
    printf("########\n");

    if((bIcfoLock == TRUE) && (bFecLock == TRUE))
    {
        printf("ISDBT in Lock state\n");


        //Get CFO
        _HAL_INTERN_ISDBT_GetFreqOffset(&fOffsetValue);
        printf("ISDBT Freq offset = [%f]\n", fOffsetValue);
        printf("ISDBT Freq offset = [%ld]\n", (MS_S32)fOffsetValue);
        //Get SFO
        _HAL_INTERN_ISDBT_GetTimingOffset(&fOffsetValue);
        printf("ISDBT Timing offset = [%f]\n", fOffsetValue);
        printf("ISDBT Timing offset = [%ld]\n", (MS_S32)fOffsetValue);

        //Get mode CP information
        _HAL_INTERN_ISDBT_GetSignalGuardInterval(&eGIValue);
        _HAL_INTERN_ISDBT_GetSignalFFTValue(&eTransmissionMode);
        printf("ISDBT Mode = [%d] and CP = [%d]\n", eTransmissionMode, eGIValue);

        //Get TMCC information
        _HAL_INTERN_ISDBT_GetSignalCodeRate(E_ISDBT_Layer_A, &eCodeRate);
        _HAL_INTERN_ISDBT_GetSignalTimeInterleaving(E_ISDBT_Layer_A, &eTI);
        _HAL_INTERN_ISDBT_GetSignalModulation(E_ISDBT_Layer_A, &eConstellation);
        printf("ISDBT TMCC Layer A, Modulation = [%d], CR = [%d] and TI = [%d]\n", eConstellation, eCodeRate, eTI);
        _HAL_INTERN_ISDBT_GetSignalCodeRate(E_ISDBT_Layer_B, &eCodeRate);
        _HAL_INTERN_ISDBT_GetSignalTimeInterleaving(E_ISDBT_Layer_B, &eTI);
        _HAL_INTERN_ISDBT_GetSignalModulation(E_ISDBT_Layer_B, &eConstellation);
        printf("ISDBT TMCC Layer B, Modulation = [%d], CR = [%d] and TI = [%d]\n", eConstellation, eCodeRate, eTI);
        _HAL_INTERN_ISDBT_GetSignalCodeRate(E_ISDBT_Layer_C, &eCodeRate);
        _HAL_INTERN_ISDBT_GetSignalTimeInterleaving(E_ISDBT_Layer_C, &eTI);
        _HAL_INTERN_ISDBT_GetSignalModulation(E_ISDBT_Layer_C, &eConstellation);
        printf("ISDBT TMCC Layer C, Modulation = [%d], CR = [%d] and TI = [%d]\n", eConstellation, eCodeRate, eTI);

        //Get Snr
        _HAL_INTERN_ISDBT_GetSNR(&fSnr);
        printf("ISDBT SNR = [%f]\n", fSnr);
        printf("ISDBT SNR = [%d]\n", (MS_U8)fSnr);

       //Get Channel Length
       _HAL_INTERN_ISDBT_ChannelProfile(&u8FdIndex);
       printf("ISDBT FD INDEX = [%d]\n", u8FdIndex);

        //Get FEC data
        _HAL_INTERN_ISDBT_GetPostViterbiBer(E_ISDBT_Layer_A, &fBer);
        _HAL_INTERN_ISDBT_Read_PKT_ERR(E_ISDBT_Layer_A, &u16PacketErr);
        printf("ISDBT Layer A PostBer = [%f] and PktErr = [%d]\n", fBer, u16PacketErr);
        _HAL_INTERN_ISDBT_GetPostViterbiBer(E_ISDBT_Layer_B, &fBer);
        _HAL_INTERN_ISDBT_Read_PKT_ERR(E_ISDBT_Layer_B, &u16PacketErr);
        printf("ISDBT Layer B PostBer = [%f] and PktErr = [%d]\n", fBer, u16PacketErr);
        _HAL_INTERN_ISDBT_GetPostViterbiBer(E_ISDBT_Layer_C, &fBer);
        _HAL_INTERN_ISDBT_Read_PKT_ERR(E_ISDBT_Layer_C, &u16PacketErr);
        printf("ISDBT Layer C PostBer = [%f] and PktErr = [%d]\n", fBer, u16PacketErr);

    }
    else if((bIcfoLock == TRUE) && (bFecLock == FALSE))
    {
        printf("ISDBT in Icfo Lock state\n");

        //Get CFO
        _HAL_INTERN_ISDBT_GetFreqOffset(&fOffsetValue);
        printf("ISDBT Freq offset = [%f]\n", fOffsetValue);

        //Get SFO
        _HAL_INTERN_ISDBT_GetTimingOffset(&fOffsetValue);
        printf("ISDBT Timing offset = [%f]\n", fOffsetValue);

        //Get mode CP information
        _HAL_INTERN_ISDBT_GetSignalGuardInterval(&eGIValue);
        _HAL_INTERN_ISDBT_GetSignalFFTValue(&eTransmissionMode);
        printf("ISDBT Mode = [%d] and CP = [%d]\n", eTransmissionMode, eGIValue);

    }

    //Get Agc value

    return TRUE;
}



   #endif

