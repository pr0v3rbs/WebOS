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
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mdrv_gflip_st.h
// @brief  GFlip KMD Driver Interface
// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MDRV_GFLIP_ST_H
#define _MDRV_GFLIP_ST_H

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Type and Structure Declaration
//=============================================================================
#define MULTIINFO_NUM 8

//IO Ctrl struct defines:
typedef enum
{
    /// Color format RGB555 and Blink.
    E_GFLIP_COLOR_RGB555_BLINK    =0,
    /// Color format RGB565.
    E_GFLIP_COLOR_RGB565          =1,
    /// Color format ARGB4444.
    E_GFLIP_COLOR_ARGB4444        =2,
    /// Color format alpha blink.
    E_GFLIP_COLOR_2266      =3,
    /// Color format I8 (256-entry palette).
    E_GFLIP_COLOR_I8              =4,
    /// Color format ARGB8888.
    E_GFLIP_COLOR_ARGB8888        =5,
    /// Color format ARGB1555.
    E_GFLIP_COLOR_ARGB1555        =6,
    /// Color format ARGB8888.  - Andriod format
    E_GFLIP_COLOR_ABGR8888        =7,
    /// Color format RGB555/YUV422.
    E_GFLIP_COLOR_RGB555YUV422    =8,
    /// Color format YUV422.
    E_GFLIP_COLOR_YUV422          =9,
    /// Color format ARGB8888.  - Andriod format
    E_GFLIP_COLOR_RGBA5551        =10,
    /// Color format ARGB8888.  - Andriod format
    E_GFLIP_COLOR_RGBA4444        =11,

    /// Invalid color format.
    E_GFLIP_COLOR_INVALID
} GFLIP_COLOR_TYPE;

typedef union
{
    MS_U8 u8DWinIntInfo;
    struct
    {
        MS_U8 bDWinIntWADR:1;
        MS_U8 bDWinIntPROG:1;
        MS_U8 bDWinIntTF:1;
        MS_U8 bDWinIntBF:1;
        MS_U8 bDWinIntVS:1;
        MS_U8 reserved:3;
    }sDwinIntInfo;
}GFLIP_DWININT_INFO, *PGFLIP_DWININT_INFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_U32 u32Addr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
}MS_GFLIP_INFO, *PMS_GFLIP_INFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_U32 u32Addr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
    MS_BOOL bTLBEnable;
    MS_U32 u32TLBAddr;
}MS_TLB_GFLIP_INFO, *PMS_TLB_GFLIP_INFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_U32 u32MainAddr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
    MS_U32 u32SubAddr; //in
}MS_GFLIP_3DINFO, *PMS_GFLIP_3DINFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_U32 u32MainAddr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
    MS_U32 u32SubAddr; //in
    MS_BOOL bTLBEnable;
    MS_U32 u32TLBAddr;
}MS_TLB_GFLIP_3DINFO, *PMS_TLB_GFLIP_3DINFO;

typedef struct
{
    MS_U8 u8GopInfoCnt;
    MS_GFLIP_3DINFO astGopInfo[MULTIINFO_NUM];
}MS_GFLIP_MULTIINFO, *PMS_GFLIP_MULTIINFO;

typedef struct
{
    MS_U8 u8GopInfoCnt;
    MS_TLB_GFLIP_3DINFO astTLBGopInfo[MULTIINFO_NUM];
}MS_TLB_GFLIP_MULTIINFO, *PMS_TLB_GFLIP_MULTIINFO;

typedef struct
{
    GFLIP_DWININT_INFO gflipDWinIntInfo;
    MS_BOOL bResetDWinIntInfo;

}MS_GFLIP_DWININT_INFO, *PMS_GFLIP_DWININT_INFO;

typedef struct
{
    GFLIP_DWININT_INFO gflipDWinIntInfo;
    MS_U32  u32Timeout;
    MS_BOOL bResetDWinIntInfo;
}MS_GFLIP_DWININT_INFO2, *PMS_GFLIP_DWININT_INFO2;

typedef struct
{
    MS_BOOL bEnable; //InOut
    MS_U8   u8FrameCount; //Out, value range: 0~3
    MS_U8   u8Result; //Out
}MS_GFLIP_VECAPTURESTATE, *PMS_GFLIP_VECAPTURESTATE;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_U32 u32Result; //Out
}MS_GFLIP_GOPGWINIDX,*PMS_GFLIP_GOPGWINIDX;

typedef struct
{
    MS_U32 u32Addr; 
    MS_U16 u16X; 
    MS_U16 u16Y; 
    MS_U16 u16W; 
    MS_U16 u16H;
    MS_U8  u8GopIdx;
    MS_U8  u8GwinIdx;
    GFLIP_COLOR_TYPE clrType;
}MS_GWIN_INFO;

typedef struct
{    
    MS_BOOL bCurveChange;  // Curve Change
    MS_U8  u8LumaCurve[16];  // Default luma curve from Supernova DLC.ini
    MS_U8  u8LumaCurve2_a[16];  // Default luma curve 2a  from Supernova DLC.ini
    MS_U8  u8LumaCurve2_b[16];  // Default luma curve 2b  from Supernova DLC.ini
    MS_U8  u8Result; //Out
}MS_DLC_INFO;

typedef struct
{
    MS_BOOL bBLEPointChange;  // BLE Point Change
    MS_U16  u16BLESlopPoint[6];  // Default BLE Slop Point from Supernova DLC.ini
    MS_U8  u8Result; //Out
}MS_BLE_INFO;


typedef struct
{
    MS_BOOL bInitInfoChange;  // Init Info Change

    // Default luma curve
    MS_U8 ucLumaCurve[16];
    MS_U8 ucLumaCurve2_a[16];
    MS_U8 ucLumaCurve2_b[16];
    MS_U8 ucDlcHistogramLimitCurve[17];

    MS_U8 ucDlcPureImageMode; // Compare difference of max and min bright
    MS_U8 ucDlcLevelLimit; // n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    MS_U8 ucDlcAvgDelta; // n = 0 ~ 50, default value: 12
    MS_U8 ucDlcAvgDeltaStill; // n = 0 ~ 15 => 0: disable still curve, 1 ~ 15 => 0.1 ~ 1.5 enable still curve
    MS_U8 ucDlcFastAlphaBlending; // min 17 ~ max 32
    MS_U8 ucDlcYAvgThresholdL; // default value: 0
    MS_U8 ucDlcYAvgThresholdH; // default value: 128

    MS_U8 ucDlcBLEPoint; // n = 24 ~ 64, default value: 48
    MS_U8 ucDlcWLEPoint; // n = 24 ~ 64, default value: 48
    MS_U8 bEnableBLE : 1; // 1: enable; 0: disable
    MS_U8 bEnableWLE : 1; // 1: enable; 0: disable
    MS_U8 ucDlcYAvgThresholdM;
    MS_U8 ucDlcCurveMode;
    MS_U8 ucDlcCurveModeMixAlpha;
    MS_U8 ucDlcAlgorithmMode;
    MS_U8 ucDlcSepPointH;
    MS_U8 ucDlcSepPointL;
    MS_U16 uwDlcBleStartPointTH;
    MS_U16 uwDlcBleEndPointTH;
    MS_U8 ucDlcCurveDiff_L_TH;
    MS_U8 ucDlcCurveDiff_H_TH;
    MS_U16 uwDlcBLESlopPoint_1;
    MS_U16 uwDlcBLESlopPoint_2;
    MS_U16 uwDlcBLESlopPoint_3;
    MS_U16 uwDlcBLESlopPoint_4;
    MS_U16 uwDlcBLESlopPoint_5;
    MS_U16 uwDlcDark_BLE_Slop_Min;
    MS_U8 ucDlcCurveDiffCoringTH;
    MS_U8 ucDlcAlphaBlendingMin;
    MS_U8 ucDlcAlphaBlendingMax;
    MS_U8 ucDlcFlicker_alpha;
    MS_U8 ucDlcYAVG_L_TH;
    MS_U8 ucDlcYAVG_H_TH;
    MS_U8 ucDlcDiffBase_L;
    MS_U8 ucDlcDiffBase_M;
    MS_U8 ucDlcDiffBase_H;
}MS_DLC_INIT_INFO;


#endif //_MDRV_GFLIP_ST_H
