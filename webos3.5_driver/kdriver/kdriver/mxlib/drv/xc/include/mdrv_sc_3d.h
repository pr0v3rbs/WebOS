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
/// file    Mdrv_sc_3d.h
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_SC_3D_H_
#define _MDRV_SC_3D_H_
#undef INTERFACE
#ifdef DRV_SC_3D_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#ifndef DISABLE_3D_FUNCTION

#if HW_2DTO3D_SUPPORT
    #define NBLK_H_MAX 120
    #define NBLK_V_MAX 68
    #define YMAX            0
    #define YMIN            1
    #define YCTRS           2
    #define BLUE            3
    #define YAVG            4
    #define HEDGE           5
    #define VEDGE           6
    #define GREEN           7
    #define BUILDING        8
    #define YAVGEDGE        9
    #define CBAVG           10
    #define CRVG            11
    #define OBJIBNDRY       12
    #define OBJIDENT        13
    #define YMAXBUFINDEX    9
    #define YMINBUFINDEX    12
    #define YAVGBUFINDEX    15
    #define YHEDGEBUFINDEX  28
    #define YVEDGEBUFINDEX  29
    #define HBLOCKNUM       60
    #define VBLOCKNUM       34
    #define MAXINDEX        256
    #define MAXARTGAIN      15
    #define ELEMENTNUM      14
    #define DDBANKOFFSET    0x00010000; //64K
#endif

//Frame-Packing timing spec
#define InputSize1920x2415CapHSize 1920 
#define InputSize1920x2205CapHSize 1920 
#define InputSize1280x1470CapHSize 1280
#define InputSize1920x2228CapHSize 1920
#define InputSize1440x1028CapHSize 1440 // 1440x480i@60FP

#define InputSize1920x2415CapVSize 2415 // 1920x1080i@50FP
#define InputSize1920x2228CapVSize 2228 	//FP imode
#define InputSize1920x2205CapVSize 2205
#define InputSize1440x1028CapVSize 1028 // 1440x480i@60FP
#define InputSize1280x1470CapVSize 1470
#define InputSize1440x1226CapVSize 1226 // 1440x576i@60FP
#define InputSize1440x502CapVSize 502 	// 1440x240p
#define InputSize_x503CapVSize 503 	// 1440x240p
#define InputSize_x1201CapVSize 1201 	// 720x576p
#define InputSize_x600CapVSize 600 	// 1440x288p
#define InputSize_x1005CapVSize 1005 	// 480p


#define InputSize1920x2205VSize 1080
#define InputSize1280x1470VSize 720
#define InputSize1920x2228VSize 540// 1920x1080i@60FP
#define InputSize1920x2415VSize 540// 1920x1080i@50FP
#define InputSize1440x1028VSize 240// 1440x480i@60FP
#define InputSize1440x1226VSize 288// 1440x576i@60FP

//-------------- FP p mode GAP --------------
#define InputSize1920x2405_GAP    85
#define InputSize1920x2205_GAP    45
#define InputSize1280x1470_GAP    30
#define InputSize640x480_GAP    45
#define InputSize1440x502_GAP    22//1440x240p
#define InputSize_x503_GAP    23
#define InputSize_x1201_GAP    49 // 720x576p
#define InputSize_x600_GAP    24// 1440x288p

//-------------- FP i mode GAP --------------
#define InputSize1920x2415_GAP0_2    85//1920x1080i@50FP
#define InputSize1920x2415_GAP1    85

#define InputSize1920x2228_GAP0_2    23//
#define InputSize1920x2228_GAP1    22

#define InputSize1440x1028_GAP0_2    23// 1440x480i@60FP
#define InputSize1440x1028_GAP1    22

#define InputSize1440x1226_GAP0_2    25// 1440x576i@50FP
#define InputSize1440x1226_GAP1    24


#define FramePacking1920x1080pVTotal 2250
#define FramePacking1280x720pVTotal  1500
#define FramePacking720x480pVTotal   1050
#define FramePacking720x576pVTotal   1250
#define FramePacking640x480pVTotal   1050
#define FramePacking1440x576iVTotal   1250


#define Input1920x1080pVSize 1080
#define Input1280x720pVSize 720
#define Input720x480pVSize  480
#define Input720x576pVSize  576
#define Input640x480pVSize  480
#define Input1440x576iVSize   576
#define Input1440x240pVSize 240 // 1440x240p
#define Input1440x288pVSize 288 //1440x288p


#define FramePacking1920x1080pCapVSize (FramePacking1920x1080pVTotal -(FramePacking1920x1080pVTotal  /2 -Input1920x1080pVSize))
#define FramePacking1280x720pCapVSize  (FramePacking1280x720pVTotal  -(FramePacking1280x720pVTotal   /2 -Input1280x720pVSize))
#define FramePacking720x480pCapVSize   (FramePacking720x480pVTotal   -(FramePacking720x480pVTotal    /2 -Input720x480pVSize))
#define FramePacking720x576pCapVSize   (FramePacking720x576pVTotal   -(FramePacking720x576pVTotal    /2 -Input720x576pVSize))
#define FramePacking640x480pCapVSize   (FramePacking640x480pVTotal   -(FramePacking640x480pVTotal    /2 -Input640x480pVSize))
#define FramePacking1440x576iCapVSize   (FramePacking1440x576iVTotal   -(FramePacking1440x576iVTotal    /2 -Input1440x576iVSize))

//Field Alternative
#define PreVscalDown_1 (1)
#define FA_1920x1080iCapVSize 1103
#define FA_1920x1080iVSize 540
#define FA_1920x1080i_GAP    23

#define FA_1440x480iCapVSize 503
#define FA_1440x480iVSize 240
#define FA_1440x480i_GAP    23

#define FA_1440x576iCapVSize 601
#define FA_1440x576iVSize 288
#define FA_1440x576i_GAP    25

typedef struct _PIXEL_DATA
{
    MS_U16 R;
    MS_U16 G;
    MS_U16 B;
}PIXEL;

// 3d internal internal interface
INTERFACE MS_BOOL MDrv_SC_3D_IsOutputVfreq48hz(void *pInstance, SCALER_WIN eWindow, MS_U16 u16InputVFreq);
INTERFACE MS_BOOL MDrv_SC_3D_AdjustPreVerSrcForFramepacking(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_IsIPMFetchNoLimitCase(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Variable_Init(void *pInstance);
INTERFACE void MDrv_SC_3D_Adjust_FillLBMode(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_PreVLength(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                            XC_InternalStatus *pSrcInfo,
                                            SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_PreHorDstSize(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                               XC_InternalStatus *pSrcInfo,
                                               SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_SubLineBufferOffset(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                                     XC_InternalStatus *pSrcInfo,
                                                     SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_SC_3D_Adjust_PreVerDstSize(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                                 XC_InternalStatus *pSrcInfo,
                                                 MS_U16 u16ScaleDst,
                                                 SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_SC_3D_Adjust_PostHorDstSize(void *pInstance, XC_InternalStatus *pSrcInfo,
                                                  MS_U16 u16ScaleDst,
                                                  SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_SC_3D_Adjust_PostHorSrcSize(void *pInstance, XC_InternalStatus *pSrcInfo,
                                                  MS_U16 u16ScaleSrc,
                                                  SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_SC_3D_Adjust_PostVerSrcSize(void *pInstance, XC_InternalStatus *pSrcInfo,
                                                  MS_U16 u16ScaleSrc,
                                                  SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_PostVLength(void *pInstance, XC_InternalStatus *pSrcInfo,
                                             SCALER_WIN eWindow);
INTERFACE XC_FRAME_STORE_NUMBER MDrv_SC_3D_Adjust_FrameNum(void *pInstance, XC_FRAME_STORE_NUMBER u8FBNumType,
                                                 SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_DNRBase(void *pInstance, MS_PHY *pu32DNRBase0,
                                         MS_PHY *pu32DNRBase1,
                                         SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_OPMBase(void *pInstance, MS_PHY *pu32OPMBase0,
                                         MS_PHY *pu32OPMBase1,
                                         MS_PHY *pu32OPMBase2,
                                         MS_PHY u32Offset,
                                         SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_FetchOffset(void *pInstance, XC_InternalStatus *pSrcInfo,
                                             SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_FormatValidation(void *pInstance, XC_InternalStatus *pSrcInfo,
                                              SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_SetWriteMemory(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_MirrorDNROffset(void *pInstance, XC_InternalStatus *pSrcInfo,
                                                 MS_U16 *pu16V_SizeAfterPreScaling,
                                                 MS_U16 *pu16DNROffset,
                                                 MS_U16 *pu16Ratio,
                                                 SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_AdjustMirrorCrop(void *pInstance, XC_InternalStatus *pSrcInfo,SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_DNROffset(void *pInstance, MS_U16 *pu16DNROffsetTemp,MS_U16 *pu16DNROffset, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_CropWinWidth(void *pInstance, MS_U16 *pu16CropWinWidth,SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_PixelOffset(void *pInstance, XC_InternalStatus *pSrcInfo,MS_PHY *pu32FRCM_RPixelOffset,MS_U16 *pu16FRCM_ROffset,MS_U16 *pu16FRCM_ROffset3D,SCALER_WIN eWindow);

INTERFACE void MDrv_SC_3D_GetFP_Info(void *pInstance, XC_InternalStatus *pSrcInfo,
                                     MS_U16 *pu16VactVideo,
                                     MS_U16 *pu16VactSpace,
                                     MS_BOOL bPreVscalingEna);
INTERFACE void MDrv_SC_3D_Disable_Crop(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_3D_DynamicScaling_LoadReg(void *pInstance, E_XC_3D_INPUT_MODE eInputMode, E_XC_3D_OUTPUT_MODE eOutputMode, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_3D_LoadReg(void *pInstance, E_XC_3D_INPUT_MODE eInputMode, E_XC_3D_OUTPUT_MODE eOutputMode, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_Is2Dto3DCase(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode,
                                          E_XC_3D_OUTPUT_MODE e3dOutputMode);
INTERFACE MS_BOOL MDrv_SC_3D_IsUseSameBuffer(void *pInstance);
INTERFACE void MDrv_SC_3D_CloneMainSettings(void *pInstance, XC_InternalStatus *pSrcInfo,
                                            SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_Is_LR_Sbs2Line(void *pInstance);
INTERFACE void MDrv_SC_3D_AdjustHShift(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_3D_Adjust_BytesPer2Pixel(void *pInstance, MS_U8 *u8BytesPer2PixelTemp,SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_Is2TapModeSupportedFormat(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_Set2TapMode(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_PostPQSetting(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_3D_IsDoubleAlign(void *pInstance);
INTERFACE MS_BOOL MDrv_SC_3D_IsSupportFBL3D(void); //NO_NEED    for get chip cap
INTERFACE MS_BOOL MDrv_XC_Enable_3D_LR_Sbs2Line(void *pInstance, MS_BOOL bEnable);
INTERFACE MS_U16 MDrv_XC_Get_3D_HShift(void *pInstance);
INTERFACE MS_BOOL MDrv_XC_Is3DFormatSupported(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode, E_XC_3D_OUTPUT_MODE e3dOutputMode);
INTERFACE E_XC_3D_PANEL_TYPE MDrv_XC_Get_3D_Panel_Type(void *pInstance);
INTERFACE E_XC_3D_OUTPUT_MODE MDrv_XC_Get_3D_Output_Mode(void *pInstance);
INTERFACE E_XC_3D_INPUT_MODE MDrv_XC_Get_3D_Input_Mode(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_XC_Get_3D_HW_Version(void); //NO_NEED
INTERFACE MS_BOOL MDrv_XC_3D_IsHWDepthAdjSupported(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode,
                                                   E_XC_3D_OUTPUT_MODE e3dOutputMode,
                                                   SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_Get_3D_IsSupportedHW2DTo3D(void); //NO_NEED
INTERFACE MS_BOOL MDrv_XC_EnableT3D(void *pInstance, MS_BOOL bEnable);
INTERFACE void MDrv_XC_Set_3D_KR3DMode(void *pInstance, MS_BOOL bOnOff);
INTERFACE MS_BOOL MDrv_XC_Get_3D_KR3DMode(void *pInstance);

#undef INTERFACE
#endif
#endif

