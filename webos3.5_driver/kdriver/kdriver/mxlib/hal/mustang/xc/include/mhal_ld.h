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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
/// file    Mdrv_ld.c
/// @brief  local dimming Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MHAL_LD_H_
#define _MHAL_LD_H_


#ifdef _MHAL_LD_H_
    #define _MHAL_LD_H_DEC_
#else
    #define _MHAL_LD_H_DEC_  extern
#endif

#include "MsCommon.h"

#if 1//( LD_ENABLE )
//#include "Board.h"
//#include "MsCommon.h"
//#include "msReg.h"
//#include "drvGlobal.h"
//#include "leddevice.h"

///////////////////////////////////////////////
// CustomerType
///////////////////////////////////////////////
//Mst
#define CustomerType_Mst_Urania             0x01 //
#define CustomerType_SZ_Unicorn             0x14///CustomerType_SZ_Urania              0x14
#define CustomerType_SZ_TXX_Unicorn         0x15///For SZ TXX
#define CustomerType_LG_Urania              0x35
#define CustomerType_LG_Unicorn              0x36
#define CustomerType_CMI_Urania             0x73
#define CustomerType_AUO_Urania             0x82
#define CustomerType_TSB_Unicorn             0x83
#define CustomerType_AMT_Unicorn             0x84
#define CustomerType_TPV_Unicorn            0x85
#define CustomerType_FUNAI_Unicorn          0x86
#define CustomerType_SZ_Hisense_Unicorn     0x87
#define CustomerType_BEKO_Unicorn           0x91
#define CustomerType_Converter_Urania       0x92
#define CustomerType_SHARP_Unicorn          0x93

#define CustomerType                        CustomerType_Mst_Urania


#define psDrvLdInfo sCusPtrMap.p_sDrvLdInfo
#define psDrvLdPQInfo sCusPtrMap.p_sDrvLdPQInfo

//#define MDrv_LD_MIUReadByte(_BUF_ID, _offset)           (*(volatile unsigned char *) (0x80000000 + _BUF_ID + _offset))
//#define MDrv_LD_MIURead2Bytes(_BUF_ID, _offset)         (*(volatile MS_U16 *) (0x80000000 + _BUF_ID + _offset))
//#define MDrv_LD_MIUWriteByte(_BUF_ID, _offset, _val)    (*((volatile MS_U8*)(0x80000000 + _BUF_ID + _offset))) = (MS_U8)(_val)
//#define MDrv_LD_MIUWrite2Bytes(_BUF_ID, _offset, _val)  (*((volatile MS_U16*)(0x80000000 + _BUF_ID + _offset))) = (MS_U16)(_val)

#define PANEL_WIDTH_NORMAL	1920
#define PANEL_HEIGHT_NORMAL	1080
#define LOCAL_DIMMING_ADR		0
#define MSPI_PORT_NUM				0
//#define PWM_NUMS							6

#define SUPPORT_LD				0
#define SUPPORT_LD_SPI				1
#define ENABLE_TWO_SPI				0
#define LED_MULTI_SUPPORT_ENABLE 0
//#define MDrv_LD_PackLength  (32)
#define MDrv_LD_PackLength  (16)
#define LDM_MAX_SPI_CMD_NUM     10//ursa7 , 10
//#define MDrv_LD_RegAddr2PhyAddr(reg_addr) ((reg_addr) << 5)
//#define MDrv_LD_PhyAddr2RegAddr(reg_addr) ((reg_addr) >> 5)
#define LDF_MAX_PATCH			1

typedef enum
{
    LD_SPI_STAGE = 0,
    LD_PQ_STAGE = 1
} EnuLD_DEVELOPE_STAGE;

typedef enum
{
    HAL_LD_EDGE_TB_TYPE = 0,
    HAL_LD_EDGE_LR_TYPE = 1,
    HAL_LD_DIRECT_TYPE  = 2
} EnuLD_LED_TYPE;

typedef enum
{
    MSPI_CLK_SPD__DIV_2=0, // mspi_internal_clk/2
    MSPI_CLK_SPD__DIV_4, // mspi_internal_clk/4
    MSPI_CLK_SPD__DIV_8, // mspi_internal_clk/8
    MSPI_CLK_SPD__DIV_16, // mspi_internal_clk/16
    MSPI_CLK_SPD__DIV_32, // mspi_internal_clk/32
    MSPI_CLK_SPD__DIV_64, // mspi_internal_clk/64
    MSPI_CLK_SPD__DIV_128, // mspi_internal_clk/128
    MSPI_CLK_SPD__DIV_256, // mspi_internal_clk/256
}EnuMSPI_CLOCK_DIV;

typedef enum
{
    MSPI_CLK_SPD_24M=0,// cystal 24Mhz
    MSPI_CLK_SPD_27M=0x9,//27 MHz
    MSPI_CLK_SPD_36M=0xA,
    MSPI_CLK_SPD_43M=0xB,//43.2 MHz
    MSPI_CLK_SPD_54M=0xC,
    MSPI_CLK_SPD_86M=0xE,//86Mhz
}EnuMSPI_CLOCK_GEN; // 1E98[2:5] ; bit5=0 is crystal,bit5=1 decide by bit2~4

typedef struct
{
    EnuLD_DEVELOPE_STAGE eStage;
    
    // PANEL
    MS_U16 u16PanelWidth, u16PanelHeight; // Panel width & height
    MS_U8 u8LEDNumW, u8LEDNumH; 
    MS_U8 u8DMAWidth, u8DMAHeight; // DMA width & height (number of SPI data)
    MS_U32 u32BaseAddr; // DRAM base address for LD
    
    // SPI (LD)
    MS_BOOL bBacklightGlobeMode; // DMA output data non-8bit enable
    MS_U8 u8BacklightGlobeGain;  // DMA output data gain
    MS_U8 u8BacklightGlobeAlign; // DMA output data 0:16b  1:12b  2:4b  3:10b
    
    // SPI (LDM/MSPI)
    EnuMSPI_CLOCK_GEN eMSPIClockGen;
    EnuMSPI_CLOCK_DIV eMSPIClockDiv;
    MS_U8 u8ClockPolarity; // 0 or 1
    MS_U8 u8ClockPhase;    // 0 or 1
    MS_U8 u8Time_FromTriggerToFirstClock;  // The time from "reg_mspi_trigger" to first SPI clock.     0 => Delay 1 MSPI clock
    MS_U8 u8Time_LastClockToDone;    // The time from last SPI clock to "reg_mspi_done_flag"     0 => Delay 1 MSPI clock
    MS_U8 u8Time_BetweenByteToByte;  // The time between byte to byte transfer       0 => Delay 0 MSPI clock
    MS_U8 u8Time_FromWriteToRead;    // The time between last write and first read   0 => Delay 0 MSPI clock
    MS_U8 u8SpiTime_VSyncWidth; // Unit is us
    MS_U8 u8SpiTime_VSync_To_FirstClock; // Unit is us
    MS_U8 u8LdDmaSpiPort;
    MS_U8 u8SpiCmdLen;
    MS_U16 au16SpiCmd[LDM_MAX_SPI_CMD_NUM];
    MS_U8 u8CheckSumMode;
    MS_U8 u8LdmDmaTransferBits;
    
} StuDrvLdInfo;
    
typedef struct
{
    EnuLD_LED_TYPE eLEDType;
    
    // PANEL
    MS_U8 u8LDFWidth, u8LDFHeight; // Statistic width & height  MAX=32*32
    MS_U8 u8LEDWidth, u8LEDHeight; // LED width & height  MAX=W*H=64 if Edge2d enable, MAX=128*72 otherwise
    MS_U8 u8LSFWidth, u8LSFHeight; // LSF width & height  MAX=128*72
    MS_BOOL bCompensationEn; // Compensation enable
    const MS_U8* pu8CompTable; // Compensation table  SIZE=256x2
    MS_BOOL bEdge2DEn; // Edge2D enable
    const MS_U8* pu8Edge2DTable; // Edge2D table  SIZE=(u8LEDWidth*u8LEDHeight)*(u8LDFWidth*u8LDFHeight)
    
    // PQ HW
    MS_U32 u32LsfInitialPhase_HL, u32LsfInitialPhase_HR, u32LsfInitialPhase_V;
    MS_U8 u8DCMaxAlpha; // statistics : higher -> prefer max, lower -> prefer average
    MS_U8 u8CompensationBlend; // pixel compensation tuner : higher -> prefer compensated pixel, lower -> prefer original pixel
    
    // PQ SW
    MS_U8 u8GDStrength; // global dimming strength
    MS_U8 u8LDStrength; // local dimming strength
    MS_U8 u8TFStrengthUp; // temporal filter strength up
    MS_U8 u8TFStrengthDn; // temporal filter strength down
    MS_U8 u8TFLowThreshold; // temporal filter low threshold
    MS_U8 u8TFHightThreshold; // temporal filter high threshold
    MS_U8 u8SFStrength; // spatial filter strength
    MS_U8 u8SFStrength2; // spatial filter strength 2
    MS_U8 u8SFStrength3; // spatial filter strength 3
    MS_U8 u8InGamma; // gamma at SW begin
    MS_U8 u8OutGamma; // gamma at SW end
    MS_U8 u8BLMinValue; // min clamp value
    MS_U8 u8NRStrength; // de-noise strength
    
} StuDrvLdPQInfo;

typedef void (*tLED_CONFIG_OUTPUT)(MS_U16,MS_BOOL);
typedef MS_U16 (*tLED_GET_VFREQ)(MS_U8);
typedef MS_U16 (*tLED_GET_SYNCRSTCNT)(MS_U8);
typedef void (*tLED_SET_OUTPUTSPI)(void*, const MS_U16*);

typedef struct
{
    StuDrvLdInfo            * p_sDrvLdInfo;
    StuDrvLdPQInfo          * p_sDrvLdPQInfo;
    tLED_CONFIG_OUTPUT      LED_DEVICE_Config_OutputSignal_To_LED;
    tLED_GET_VFREQ          LED_DEVICE_Get_OutputVFreq;
    tLED_GET_SYNCRSTCNT     LED_DEVICE_Get_SyncRstCnt;
    tLED_SET_OUTPUTSPI        LED_DEVICE_SET_OutputSPI;

}t_sDrvLdCusPtrMap;

// h01[0] reg_local_dimming_en
void MHal_LD_Set_LocalDimmingEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LocalDimmingEn(void *pInstance);
// h01[15] reg_ld_rst
void MHal_LD_Set_SoftReset(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_SoftReset(void *pInstance);
// h19[19:0] reg_ratio_h_in
void MHal_LD_Set_InputHScalingDownRatio(void *pInstance, MS_U32 u32Ratio);
MS_U32  MHal_LD_Get_InputHScalingDownRatio(void *pInstance);
// h1B[19:0] reg_ratio_v_in
void MHal_LD_Set_InputVScalingDownRatio(void *pInstance, MS_U32 u32Ratio);
MS_U32  MHal_LD_Get_InputVScalingDownRatio(void *pInstance);
// h03[4:0] reg_bl_width
void MHal_LD_Set_LDF_BacklightWidth(void *pInstance, MS_U8 u8Width);
MS_U8   MHal_LD_Get_LDF_BacklightWidth(void *pInstance);
// h03[9:5] reg_bl_height
void MHal_LD_Set_LDF_BacklightHeight(void *pInstance, MS_U8 u8Height);
MS_U8   MHal_LD_Get_LDF_BacklightHeight(void *pInstance);
// h04[25:0] reg_baseaddr0_l
void MHal_LD_Set_LDF_FrameBufBaseAddr_L0(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDF_FrameBufBaseAddr_L0(void *pInstance);
// h06[25:0] reg_baseaddr1_l
void MHal_LD_Set_LDF_FrameBufBaseAddr_L1(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDF_FrameBufBaseAddr_L1(void *pInstance);
// h08[25:0] reg_baseaddr0_r
void MHal_LD_Set_LDF_FrameBufBaseAddr_R0(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDF_FrameBufBaseAddr_R0(void *pInstance);
// h0a[25:0] reg_baseaddr1_r
void MHal_LD_Set_LDF_FrameBufBaseAddr_R1(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDF_FrameBufBaseAddr_R1(void *pInstance);
// h14[25:0] reg_max_adr_ld
void MHal_LD_Set_LDF_MinWritableAddr(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDF_MinWritableAddr(void *pInstance);
// h16[25:0] reg_min_adr_ld
void MHal_LD_Set_LDF_MaxWritableAddr(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDF_MaxWritableAddr(void *pInstance);
// h18[0] reg_miu_adr_limit_en
void MHal_LD_Set_LDF_AddrLimitEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LDF_AddrLimitEn(void *pInstance);
// h1d[7:0] reg_pel_hlpf_thrd
void MHal_LD_Set_LDF_HLPFThreshold(void *pInstance, MS_U8 u8Th);
MS_U8   MHal_LD_Get_LDF_HLPFThreshold(void *pInstance);
// h1d[12] reg_pel_hlpf_en
void MHal_LD_Set_LDF_HLPFEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LDF_HLPFEn(void *pInstance);
// h1d[13] reg_pel_hlpf_step
void MHal_LD_Set_LDF_HLPFStep(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LDF_HLPFStep(void *pInstance);
// h1f[3:0] reg_alpha
void MHal_LD_Set_LDF_DCMaxAlpha(void *pInstance, MS_U8 u8Alpha);
MS_U8   MHal_LD_Get_LDF_DCMaxAlpha(void *pInstance);
// h1f[7] reg_pel_white_mod_en
void MHal_LD_Set_LDF_PixelWhiteModeEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LDF_PixelWhiteModeEn(void *pInstance);
// h1f[8] reg_write_dc_max_en
void MHal_LD_Set_LDF_WriteDCMaxEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LDF_WriteDCMaxEn(void *pInstance);
// h1f[9] reg_write_dc_max_of_en
void MHal_LD_Set_LDF_WriteDCMaxOFEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LDF_WriteDCMaxOFEn(void *pInstance);
// h1f[15] reg_dc_cbmode
void MHal_LD_Set_LDF_MeanPatchEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LDF_MeanPatchEn(void *pInstance);
// h22[15:0] reg_dummy0
void MHal_LD_Set_SW_PulseId(void *pInstance, MS_U16 u16Id);
MS_U16  MHal_LD_Get_SW_PulseId(void *pInstance);
// h22[11:0] reg_dummy0
void MHal_LD_Set_SW_PulseMode(void *pInstance, MS_U8 u8Mode);
MS_U8   MHal_LD_Get_SW_PulseMode(void *pInstance);
// h23[7:0] reg_dummy1
void MHal_LD_Set_SW_PulseLEDIntensity(void *pInstance, MS_U8 u8Intensity);
MS_U8   MHal_LD_Get_SW_PulseLEDIntensity(void *pInstance);
// h23[15:8] reg_dummy1
void MHal_LD_Set_SW_PulseLDBIntensity(void *pInstance, MS_U8 u8Intensity);
MS_U8   MHal_LD_Get_SW_PulseLDBIntensity(void *pInstance);
// h24[7:0] reg_dummy2
void MHal_LD_Set_SW_SpatialFilterStrength(void *pInstance, MS_U8 u8Str);
MS_U8   MHal_LD_Get_SW_SpatialFilterStrength(void *pInstance);
// h24[15:8] reg_dummy2
void MHal_LD_Set_SW_TemporalFilterStrengthDn(void *pInstance, MS_U8 u8Str);
MS_U8   MHal_LD_Get_SW_TemporalFilterStrengthDn(void *pInstance);
// h25[4:0] reg_dummy3
void MHal_LD_Set_SW_LocalDimmingStrength(void *pInstance, MS_U8 u8Str);
MS_U8   MHal_LD_Get_SW_LocalDimmingStrength(void *pInstance);
// h25[15:8] reg_dummy3
void MHal_LD_Set_SW_GlobalDimmingStrength(void *pInstance, MS_U8 u8Str);
MS_U8   MHal_LD_Get_SW_GlobalDimmingStrength(void *pInstance);
// h30[19:0] reg_ratio_h_out
void MHal_LD_Set_OutputHScalingUpRatio(void *pInstance, MS_U32 u32Ratio);
MS_U32  MHal_LD_Get_OutputHScalingUpRatio(void *pInstance);
// h32[19:0] reg_ratio_v_out
void MHal_LD_Set_OutputVScalingUpRatio(void *pInstance, MS_U32 u32Ratio);
MS_U32  MHal_LD_Get_OutputVScalingUpRatio(void *pInstance);
// h37[0] reg_edge2d_gain_en
void MHal_LD_Set_Edge2DGainEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_Edge2DGainEn(void *pInstance);
// h37[1] reg_edge2d_en
void MHal_LD_Set_Edge2DEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_Edge2DEn(void *pInstance);
// h37[2] reg_edge2d_direct_type_en
void MHal_LD_Set_Edge2DDirectTypeEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_Edge2DDirectTypeEn(void *pInstance);
// h37[4] reg_edge2d_pack16_en
void MHal_LD_Set_Edge2DPack16En(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_Edge2DPack16En(void *pInstance);
// h37[5] reg_bl_depack16b_mod
void MHal_LD_Set_SpiDepack16En(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_SpiDepack16En(void *pInstance);
// h38[19:0] reg_lsf_h_init
void MHal_LD_Set_LSF_H_InitialPhase_L(void *pInstance, MS_U32 u32Phase);
MS_U32 MHal_LD_Get_LSF_H_InitialPhase_L(void *pInstance);
// h55[19:0] reg_lsf_h_init_r
void MHal_LD_Set_LSF_H_InitialPhase_R(void *pInstance, MS_U32 u32Phase);
MS_U32 MHal_LD_Get_LSF_H_InitialPhase_R(void *pInstance);
// h3a[19:0] reg_lsf_v_init
void MHal_LD_Set_LSF_V_InitialPhase(void *pInstance, MS_U32 u32Phase);
MS_U32 MHal_LD_Get_LSF_V_InitialPhase(void *pInstance);
// h3c[0] reg_lsf_h_shift
void MHal_LD_Set_LSF_H_Boundary_L(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LSF_H_Boundary_L(void *pInstance);
// h3c[15] reg_lsf_h_shift_r
void MHal_LD_Set_LSF_H_Boundary_R(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LSF_H_Boundary_R(void *pInstance);
// h3c[1] reg_lsf_v_shift
void MHal_LD_Set_LSF_V_Boundary(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_LSF_V_Boundary(void *pInstance);
// h3c[3:2] reg_edge_level_shift
void MHal_LD_Set_EdgeLevelShift(void *pInstance, MS_U8 u8Mode);
MS_U8   MHal_LD_Get_EdgeLevelShift(void *pInstance);
// h3c[8:4] reg_cmp_blend
void MHal_LD_Set_CompensationBlend(void *pInstance, MS_U8 u8Height);
MS_U8   MHal_LD_Get_CompensationBlend(void *pInstance);
// h3c[10] reg_bld_dither_en
void MHal_LD_Set_BlendingDitherEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_BlendingDitherEn(void *pInstance);
// h3c[13] reg_cmp_dither_en
void MHal_LD_Set_CompensationDitherEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_CompensationDitherEn(void *pInstance);
// h3d[11:0] reg_comp_lut
void MHal_LD_Write_CompensationLUT(void *pInstance, MS_U16 u16Lut);
// h3e[11:0] reg_r_comp_lut (readonly);
MS_U16  MHal_LD_Read_CompensationLUT(void *pInstance);
// h3f[7:0] reg_comp_addr
void MHal_LD_Set_CompensationLUTAddress(void *pInstance, MS_U8 u8Addr);
MS_U8   MHal_LD_Get_CompensationLUTAddress(void *pInstance);
// h3f[10] reg_comp_wd
void MHal_LD_Set_CompensationWriteEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_CompensationWriteEn(void *pInstance);
// h3f[11] reg_comp_rd
void MHal_LD_Set_CompensationReadEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_CompensationReadEn(void *pInstance);
// h3f[12] reg_comp_mod, 1:incremental
void MHal_LD_Set_CompensationMode(void *pInstance, MS_BOOL bMode);
MS_BOOL MHal_LD_Get_CompensationMode(void *pInstance);
// h40[4] reg_read_addr_idx (readonly);
MS_BOOL MHal_LD_Get_CurFrameIdx(void *pInstance);
// h40[14] reg_hw_trig_mod
// h40[15] reg_vsync_select
// h41[7:0] reg_bl_width_led
void MHal_LD_Set_LED_BacklightWidth(void *pInstance, MS_U8 u8Width);
MS_U8   MHal_LD_Get_LED_BacklightWidth(void *pInstance);
// h41[15:8] reg_bl_height_led
void MHal_LD_Set_LED_BacklightHeight(void *pInstance, MS_U8 u8Height);
MS_U8   MHal_LD_Get_LED_BacklightHeight(void *pInstance);
// h43[15:8] reg_dummy
void MHal_LD_Set_SW_TemporalFilterStrengthUp(void *pInstance, MS_U8 u8Str);
MS_U8   MHal_LD_Get_SW_TemporalFilterStrengthUp(void *pInstance);
// h44[3] reg_dummy10
void MHal_LD_Set_SW_SpatialFilterModeEn(void *pInstance, MS_BOOL bEn);          
MS_BOOL MHal_LD_Get_SW_SpatialFilterModeEn(void *pInstance);   

void MHal_LD_Set_SW_WriteSPIEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_SW_WriteSPIEn(void *pInstance);
// h44[7] reg_dummy10
void MHal_LD_Set_SW_WriteLDBEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_SW_WriteLDBEn(void *pInstance);
// h44[12] reg_dummy10
void MHal_LD_Set_SW_AlgorithmEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_SW_AlgorithmEn(void *pInstance);
// h44[13] reg_dummy10
void MHal_LD_Set_SW_ReadLDFEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_SW_ReadLDFEn(void *pInstance);
// h44[15:14] reg_dummy10
void MHal_LD_Set_LEDType(void *pInstance, MS_U8 u8Type);
MS_U8   MHal_LD_Get_LEDType(void *pInstance);
// h46[1] reg_background_w
// h46[5:2] reg_rpt_time
// h46[6] reg_comp_en
void MHal_LD_Set_CompensationEn(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_CompensationEn(void *pInstance);
// h46[15] reg_2chip_mode_en
// h4b[7:0] reg_bl_globe
void MHal_LD_Set_BacklightGlobeGain(void *pInstance, MS_U8 u8Gain);
MS_U8   MHal_LD_Get_BacklightGlobeGain(void *pInstance);
// h4b[8] reg_bl_globe_mod
void MHal_LD_Set_BacklightGlobeMode(void *pInstance, MS_BOOL bMode);
MS_BOOL MHal_LD_Get_BacklightGlobeMode(void *pInstance);
// h4b[10:9] reg_bl_data_alig
void MHal_LD_Set_BacklightDataAlign(void *pInstance, MS_U8 u8Align);
MS_U8   MHal_LD_Get_BacklightDataAlign(void *pInstance);
// h4c[6:0] reg_bl_width
void MHal_LD_Set_LSF_BacklightWidth(void *pInstance, MS_U8 u8Width);
MS_U8   MHal_LD_Get_LSF_BacklightWidth(void *pInstance);
// h4c[14:8] reg_bl_height
void MHal_LD_Set_LSF_BacklightHeight(void *pInstance, MS_U8 u8Height);
MS_U8   MHal_LD_Get_LSF_BacklightHeight(void *pInstance);
// h4D[25:0] reg_baseaddr0_l
void MHal_LD_Set_LDB_FrameBufBaseAddr_L0(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDB_FrameBufBaseAddr_L0(void *pInstance);
// h4F[25:0] reg_baseaddr1_l
void MHal_LD_Set_LDB_FrameBufBaseAddr_L1(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDB_FrameBufBaseAddr_L1(void *pInstance);
// h51[25:0] reg_baseaddr0_r
void MHal_LD_Set_LDB_FrameBufBaseAddr_R0(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDB_FrameBufBaseAddr_R0(void *pInstance);
// h53[25:0] reg_baseaddr1_r
void MHal_LD_Set_LDB_FrameBufBaseAddr_R1(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_LDB_FrameBufBaseAddr_R1(void *pInstance);
// h57[7:0] reg_dummy5
void MHal_LD_Set_SW_TemporalFilterLowTh(void *pInstance, MS_U8 u8Th);
MS_U8   MHal_LD_Get_SW_TemporalFilterLowTh(void *pInstance);
// h57[15:8] reg_dummy5
void MHal_LD_Set_SW_TemporalFilterHighTh(void *pInstance, MS_U8 u8Th);
MS_U8   MHal_LD_Get_SW_TemporalFilterHighTh(void *pInstance);
// h58[7:0] reg_dummy6
void MHal_LD_Set_SW_SpatialFilterStrength2(void *pInstance, MS_U8 u8Dist);
MS_U8   MHal_LD_Get_SW_SpatialFilterStrength2(void *pInstance);
// h58[15:8] reg_dummy6
void MHal_LD_Set_SW_SpatialFilterStrength3(void *pInstance, MS_U8 u8Dist);
MS_U8   MHal_LD_Get_SW_SpatialFilterStrength3(void *pInstance);
void MHal_LD_Set_SW_SpatialFilterStrength4(void *pInstance, MS_U8 u8Dist);
MS_U8   MHal_LD_Get_SW_SpatialFilterStrength4(void *pInstance);
void MHal_LD_Set_SW_SpatialFilterStrength5(void *pInstance, MS_U8 u8Dist);
MS_U8   MHal_LD_Get_SW_SpatialFilterStrength5(void *pInstance);

// h60[5:0] reg_bl_width_dma
void MHal_LD_Set_DMA_BacklightWidth(void *pInstance, MS_U8 u8Width);
MS_U8   MHal_LD_Get_DMA_BacklightWidth(void *pInstance);
// h60[13:8] reg_bl_height_dma
void MHal_LD_Set_DMA_BacklightHeight(void *pInstance, MS_U8 u8Height);
MS_U8   MHal_LD_Get_DMA_BacklightHeight(void *pInstance);
// h63[4:0] reg_bl_width_led
void MHal_LD_Set_LDB_BacklightWidth(void *pInstance, MS_U8 u8Width);
MS_U8   MHal_LD_Get_LDB_BacklightWidth(void *pInstance);
// h63[9:5] reg_bl_height_led
void MHal_LD_Set_LDB_BacklightHeight(void *pInstance, MS_U8 u8Height);
MS_U8   MHal_LD_Get_LDB_BacklightHeight(void *pInstance);
// h64[3:0] reg_miu_pack_offset_0
void MHal_LD_Set_DMA0_PackOffset(void *pInstance, MS_U8 u8Offset);
// h65[3:0] reg_miu_pack_length_0
void MHal_LD_Set_DMA0_PackLength(void *pInstance, MS_U8 u8Len);
// h65[8] reg_dma0_en
void MHal_LD_Set_DMA0_En(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_DMA0_En(void *pInstance);
// h66[3:0] reg_miu_pack_offset_1
void MHal_LD_Set_DMA1_PackOffset(void *pInstance, MS_U8 u8Offset);
// h67[3:0] reg_miu_pack_length_1
void MHal_LD_Set_DMA1_PackLength(void *pInstance, MS_U8 u8Len);
// h67[8] reg_dma1_en
void MHal_LD_Set_DMA1_En(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_DMA1_En(void *pInstance);
// h68[3:0] reg_miu_pack_offset_2
void MHal_LD_Set_DMA2_PackOffset(void *pInstance, MS_U8 u8Offset);
// h69[3:0] reg_miu_pack_length_2
void MHal_LD_Set_DMA2_PackLength(void *pInstance, MS_U8 u8Len);
// h69[8] reg_dma2_en
void MHal_LD_Set_DMA2_En(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_DMA2_En(void *pInstance);
// h6a[3:0] reg_miu_pack_offset_3
void MHal_LD_Set_DMA3_PackOffset(void *pInstance, MS_U8 u8Offset);
// h6b[3:0] reg_miu_pack_length_3
void MHal_LD_Set_DMA3_PackLength(void *pInstance, MS_U8 u8Len);
// h6b[8] reg_dma3_en
void MHal_LD_Set_DMA3_En(void *pInstance, MS_BOOL bEn);
MS_BOOL MHal_LD_Get_DMA3_En(void *pInstance);
// h6c[5:0] reg_dma_yoff_end_0
void MHal_LD_Set_DMA0_YOffEnd(void *pInstance, MS_U8 u8End);
// h6d[5:0] reg_dma_yoff_end_1
void MHal_LD_Set_DMA1_YOffEnd(void *pInstance, MS_U8 u8End);
// h6e[5:0] reg_dma_yoff_end_2
void MHal_LD_Set_DMA2_YOffEnd(void *pInstance, MS_U8 u8End);
// h6f[5:0] reg_dma_yoff_end_3
void MHal_LD_Set_DMA3_YOffEnd(void *pInstance, MS_U8 u8End);
// h70[12:0] reg_frm_width
void MHal_LD_Set_FrameWidth(void *pInstance, MS_U16 u16Width);
MS_U16  MHal_LD_Get_FrameWidth(void *pInstance);
// h71[11:0] reg_frm_height
void MHal_LD_Set_FrameHeight(void *pInstance, MS_U16 u16Height);
MS_U16  MHal_LD_Get_FrameHeight(void *pInstance);
// h72[0] reg_debug_mod
void MHal_LD_Set_DebugMode(void *pInstance, MS_BOOL bMode);
MS_BOOL MHal_LD_Get_DebugMode(void *pInstance);
// h72[3:1] reg_lsf_out_mod
void MHal_LD_Set_LSFOutMode(void *pInstance, MS_U8 u8Mode);
MS_U8   MHal_LD_Get_LSFOutMode(void *pInstance);
// h72[5:4] reg_comp_out_mod
void MHal_LD_Set_CompensationOutMode(void *pInstance, MS_U8 u8Mode);
MS_U8   MHal_LD_Get_CompensationOutMode(void *pInstance);
// h76[7:0] reg_dummy0
void MHal_LD_Set_SW_MinClampValue(void *pInstance, MS_U8 u8Val);
MS_U8   MHal_LD_Get_SW_MinClampValue(void *pInstance);
// h76[11:8] reg_dummy0
void MHal_LD_Set_SW_BacklightInGamma(void *pInstance, MS_U8 u8Type);
MS_U8   MHal_LD_Get_SW_BacklightInGamma(void *pInstance);
// h76[15:12] reg_dummy0
void MHal_LD_Set_SW_BacklightOutGamma(void *pInstance, MS_U8 u8Type);
MS_U8   MHal_LD_Get_SW_BacklightOutGamma(void *pInstance);
// h77[25:0] reg_baseaddr_offset_spi_2
void MHal_LD_Set_SPI_BufBaseAddr_2(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_SPI_BufBaseAddr_2(void *pInstance);
// h78[15] reg_nxt_spi_access_en
void MHal_LD_Set_NextSpiAccessEn(void *pInstance, MS_BOOL bEn);         
MS_BOOL MHal_LD_Get_NextSpiAccessEn(void *pInstance);
// h79[25:0] reg_edge_2d_baseaddr
void MHal_LD_Set_Edge2D_BufBaseAddr(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_Edge2D_BufBaseAddr(void *pInstance);
// h7E[25:0] reg_baseaddr_offset
void MHal_LD_Set_SPI_BufBaseAddr(void *pInstance, MS_U32 u32Addr);
MS_U32  MHal_LD_Get_SPI_BufBaseAddr(void *pInstance);
void MHal_LD_Set_SPI_VsyncSel(void *pInstance, MS_BOOL bsrc);

#if ENABLE_TWO_SPI
void MHal_LD_Set_DmaMiuPack_Offset_Length(void *pInstance, MS_U8 offset1, MS_U8 offset2, MS_U8 len);
#else
void MHal_LD_Set_DmaMiuPack_Offset_Length(void *pInstance, MS_U8 offset, MS_U8 len);
#endif

void MHal_LD_Set_DmaEnable(void *pInstance, MS_BOOL bEn);
void MHal_LD_Set_DmaAccessYEnd(void *pInstance, MS_U8 ylen);
void MHal_LD_InitU02(void *pInstance);

void MHal_LD_SetMiuSelect(void *pInstance, MS_U8 u8Select);
MS_U8 MHal_LD_GetMiuSelect(void *pInstance);

void MHal_LD_SetSPIMirror(void *pInstance, MS_BOOL bEnable);             
MS_BOOL MHal_LD_GetSPIMirror(void *pInstance);
void MHal_LD_SetDummyReg(void *pInstance, MS_U16 u16Addr, MS_U16 u16Val);
MS_U16 MHal_LD_GetDummyReg(void *pInstance, MS_U16 u16Addr);

void MHal_LD_Set_MaxSpeedLimit(void *pInstance, MS_U8 u8Speed);   
MS_U8   MHal_LD_Get_MaxSpeedLimit(void *pInstance);
void MHal_LD_Set_SPI_AvgData(void *pInstance, MS_U8 u8Value);            
MS_U8 MHal_LD_Get_SPI_AvgData(void *pInstance);
void MHal_LD_EnableLedOutput(void *pInstance, MS_BOOL bEnable);
MS_BOOL MHal_LD_IsEnableLedOutput(void *pInstance);

#endif // LD_ENABLE

#endif // _MHAL_LD_H_
