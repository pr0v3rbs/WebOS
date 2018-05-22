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
// Copyright (c) 2008-2012 MStar Semiconductor, Inc.
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
/// @file   apiXC_ex.h
/// @brief  XC Interface for extension
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_XC_EX_H_
#define _API_XC_EX_H_

#include "apiXC.h"
#ifdef __cplusplus
extern "C"
{
#endif

//-----------------------------
// 3D macros
//-----------------------------
#define DOUBLEHD_1080X2P_PRESCALE_GARBAGE_VSIZE (23)
#define DOUBLEHD_1080X2P_PRESCALE_VSIZE (DOUBLEHD_PRESCALE_VONEHDSIZE*2+DOUBLEHD_1080X2P_PRESCALE_GARBAGE_VSIZE)

#define DOUBLEHD_720X2P_PRESCALE_GARBAGE_VSIZE (24)
#define DOUBLEHD_720X2P_PRESCALE_VSIZE (DOUBLEHD_PRESCALE_VONEHDSIZE*2+DOUBLEHD_720X2P_PRESCALE_GARBAGE_VSIZE)

#define DOUBLEHD_1080X2P_MVC_FRAME_SIZE  (2176)

#define KOREA_3D_VSIZE (2160)
#define XC_NEW_FILL_EVENT_STABLE_MOTHOD                           1//ldl121117


//---------------------------------------
// Refrigerator macros
//---------------------------------------
#define DUMMY_REFRIGLATOR REG_SC_BK00_34_L
#define KERNEL_INIT_NUMBER 0x5
#define KERNEL_FREEZE_BIT BIT(3)
#define KERNEL_YCBCR_BIT BIT(4)

#define INITID  //MApi_XC_W2BYTEMSK(DUMMY_REFRIGLATOR, KERNEL_INIT_NUMBER, KERNEL_INIT_NUMBER);
#define ClearKernelFreezeID  MApi_XC_W2BYTEMSK(DUMMY_REFRIGLATOR, 0, KERNEL_FREEZE_BIT);
#define ClearForceRW     MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(11));\
    MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(10));\
    MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(9));\
    MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(8));\


//mdrv_mhl_stl.h
#define MHL_MSC_SCRATCHPAD_SIZE
#define MHL_CBUS_DATA_SIZE              (MHL_MSC_SCRATCHPAD_SIZE +2) // offset + maxi 16 packets
typedef enum
{
	CBUS_REQ_IDLE       = 0,
	CBUS_REQ_PENDING,           // Request is waiting to be sent
	CBUS_REQ_SENT,              // Request has been sent
	CBUS_REQ_RECEIVED,          // Request data has been received
} CbusReqState_T;
typedef struct
{
	MS_U8 msgData[MHL_CBUS_DATA_SIZE +1];    // Cbus message data
	MS_U8 cmd;    // MSC command
	MS_U8 len;    // message length
	MS_U8 rcState;    // received state
	CbusReqState_T reqStatus;    // Cbus request status
} CbusReq_S;


////////////////////////////////////////////////////
//         Specific enums
////////////////////////////////////////////////////
typedef enum
{
    E_XC_3D_EX_OUTPUT_1920X2160,
    E_XC_3D_EX_OUTPUT_KOREA_SIDE_BY_SIDE,
    #ifdef KR3DINTERLACE
    E_XC_3D_EX_OUTPUT_1920X2160_KR3D,
    #endif
    E_XC_3D_EX_OUTPUT_1280X1440,
    E_XC_3D_EX_OUTPUT_OSD_1920X2160,
    E_XC_3D_EX_OUTPUT_MAX,
} E_XC_3D_EX_OUTPUT_SPECIFIC_TIMING;
#if 0
typedef enum
{
    E_DSLOAD_UNSUPPORTED                = 0, // DS is not supported by the driver or hardware
    E_DSLOAD_DISABLED                   = 1, // DS mode disabled
    E_DSLOAD_ENABLED_SWMODE_LOADING     = 2, // DS Software mode enabled, and it is in loading-command-process (Loading and not-Loading process is triggered by driver)
    E_DSLOAD_ENABLED_SWMODE_NOT_LOADING = 3, // DS Software mode enabled, but it is not in loading-command-process  (Loading and not-Loading process is triggered by driver)
    E_DSLOAD_ENABLED_HWMODE             = 4, // DS Hardware mode
} DSLOAD_TYPE;
#endif
// Custom 3D Enum
typedef enum
{
    MAIN_SOURCE_CHANNEL,
    PIP_SOURCE_CHANNEL,
    MAX_CHANNEL,
} SOURCE_CHANNEL_TYPE;

typedef enum
{
    FREQUENCY_48HZ,
    FREQUENCY_50HZ,
    FREQUENCY_60HZ,
    FREQUENCY_MAX,
} FREQUENCY_TO_PANEL;

typedef enum
{
    MAIN_PIP_OUTPUT_SYNC,
    MAIN_PIP_OUTPUT_ASYNC,
    MAIN_PIP_OUTPUT_MAX,
} MAIN_PIP_OUTPUT_TYPE;

typedef enum
{
    E_CUS3D_NO,
    E_CUS3D_NORMAL,
    E_CUS3D_OUTPUTASYNC,
    E_CUS3D_DEINTERLACESIGNAL,
    E_CUS3D_DEINTERLACESIGNAL_VER,
    E_CUS3D_MAX,
} CUS3DMODE;

typedef enum
{
        MSD_EXTIN0,
        MSD_EXTIN1,
        MSD_EXTIN2,
        MSD_EXTIN3,
        MSD_EXTIN4,
        MSD_EXTIN5,
        MSD_EXTIN_HDMI0,
        MSD_EXTIN_HDMI1,
        MSD_EXTIN_MAX,
} MSdExtInID_k;



//***************************************************
//             GSTREAMER SCALER RELATED
//***************************************************

//-------------------------------------------------------------------------------------------------
/// Initialize the XC MISC EXT
/// @param  pXC_Init_Misc                   \b IN: the Initialized XC MISC
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_Init_MISC_EXT(XC_INITMISC *pXC_Init_Misc, MS_U32 u32InitMiscDataLen);


//***************************************************
//             SCALER RELATED
//***************************************************

E_APIXC_ReturnValue MApi_SC_ForceInterlaceInPMode(MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL MApi_XC_FPiIn_FPOut(SCALER_WIN eWindow);

MS_BOOL MApi_XC_SetIOMapBase_i(void);


void MApi_XC_SetIPMUX_TestPattern(MS_BOOL bEnable, MS_U16 u16R_Cr, MS_U16 u16G_Y, MS_U16 u16B_Cb);
void MApi_XC_SetIPMUX_TestPattern_U2(void *pInstance, MS_BOOL bEnable, MS_U16 u16R_Cr, MS_U16 u16G_Y, MS_U16 u16B_Cb);

//-------------------------------------------------------------------------------------------------
/// Set Line buffer mode of sub window
/// @return void
/// @param ENABLE for ring buffer mode, DISABLE for line buffer mode
//-------------------------------------------------------------------------------------------------
void MApi_SC_SetDisplay_LineBuffer_Mode(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Get Constant Alpha Value of Video
/// @return void
/// @param ENABLE to start at faling edge of Vsync_init or\n
///        "Disable" for start at advance 1 display line
//-------------------------------------------------------------------------------------------------
void MApi_SC_SetDisplay_Start_Mode(MS_BOOL bEnable);

MS_U32 MApi_XC_GetFBSize( SCALER_WIN eWindow );
#if 1
MS_U32 MApi_XC2_GetFBSize( SCALER_WIN eWindow );
#endif
void    MApi_XC_3D_Set_SpecialOutputTiming(E_XC_3D_EX_OUTPUT_SPECIFIC_TIMING SpecificTiming, MS_BOOL bEnable);
MS_BOOL MApi_XC_3D_Get_SpecialOutputTiming(E_XC_3D_EX_OUTPUT_SPECIFIC_TIMING SpecificTiming);

// Custom 3D API
CUS3DMODE MApi_XC_GetCus3DMode(SCALER_WIN eChannelType);
MS_BOOL MApi_XC_Crop_MemoryWindow(MS_U16 u16Upper,
                                  MS_U16 u16Bottom,
                                  MS_U16 u16Left,
                                  MS_U16 u16Right,
                                  SOURCE_CHANNEL_TYPE eChannelType);
MS_BOOL MApi_XC_SetSize_DisplayWindow(MS_U16 u16XPos,
                                      MS_U16 u16YPos,
                                      MS_U16 u16Width,
                                      MS_U16 u16Height,
                                      SOURCE_CHANNEL_TYPE eChannelType);
MS_BOOL MApi_XC_Deinterlace_MainPipSignal(MS_BOOL bMainFirst);
MS_BOOL MApi_XC_VerDeinterlace_MainPipSignal(MS_BOOL bMainFirst);
MS_BOOL MApi_XC_SetFrequency_DisplayWindow(FREQUENCY_TO_PANEL eFreq,
        MAIN_PIP_OUTPUT_TYPE eSync);
MS_BOOL MApi_XC_DisableCus3D(void);

MS_U16 MApi_XC_Read2Byte(MS_U32 u32Reg);
MS_U32 MApi_XC_Read4Byte(MS_U32 u32Reg);
//MS_BOOL MDrv_XC_SetIOMapBase_i(void);

//-------------------------------------------------------------------------------------------------
/// @param  eWindow, MAIN_WINDOW or SUB_WINDOW               \b IN: the format of memory
/// @param  bIsSkipWaitVsyn, ENABLE or DISABLE
/// to skip Vsync wait
/// @return void
//-------------------------------------------------------------------------------------------------
//void MDrv_SC_SetSkipWaitVsync( MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn);

//-------------------------------------------------------------------------------------------------
/// @param  eWindow, MAIN_WINDOW or SUB_WINDOW               \b IN: the format of memory
/// @param  bIsSkipWaitVsyn, ENABLE or DISABLE
/// to skip Vsync wait
/// @return TRUE or FALSE
//-------------------------------------------------------------------------------------------------
//MS_BOOL MDrv_SC_GetSkipWaitVsync( MS_BOOL eWindow);

void _XC_Restore_FD_Mask(void);
//***************************************************
//             PQ RELATED
//***************************************************
void MApi_XC_SetGameMode(SCALER_WIN eWindow, MS_BOOL bNeedSetWindow );
void MApi_XC_ForceSet_OPWriteOffEnable(MS_BOOL bEnable ,SCALER_WIN eWindow);
void MApi_SC_SetNRDemoEnable_ForMLoad(MS_BOOL bEnable);
MS_BOOL MApi_SC_getNRDemoEnable_ForMLoad(void);
//void MApi_XC_ReLoad_OPWriteOff(void);
void MApi_SetSourceYCbCr(MS_BOOL b);
//***************************************************
//             DS RELATED
//***************************************************
//MS_BOOL MApi_XC_GetDynamicScalingStatus(void);
DSLOAD_TYPE MApi_XC_DSLoad_GetStatus(void);
MS_BOOL MApi_XC_DS_IP_Write2ByteMask(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL MApi_XC_DS_OP_Write2ByteMask(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL MApi_XC_DS_OP_Write4Byte(MS_U32 u32Addr, MS_U32 u32Data);
MS_BOOL MApi_XC_DS_IP_Write4Byte(MS_U32 u32Addr, MS_U32 u32Data);
MS_BOOL MApi_XC_DS_SetSoftwareMode(MS_BOOL bEnable);
void MApi_XC_DS_SetDSCallBackMode(MS_BOOL bEnable);
MS_BOOL MApi_XC_DS_GetDSCallBackMode(void);
void MApi_XC_DS_SetDSEnable(MS_BOOL bEnable);
MS_BOOL MApi_XC_DS_IsDSEnabled(void);
//***************************************************
//             REFRIGERATOR ISSUE RELATED
//***************************************************
void MApi_XC_UnFreezeByTimer(void);
MS_BOOL IsFreezeByKernel(void);

//void    MApi_XC_SetFIRDownSampleDivider(MS_U8 u8Enable , MS_U16 u16OverSamplingCount , SCALER_WIN eWindow );
void    MApi_XC_GetDEWidthHeightInDEByPassMode(MS_U16* u16Width,MS_U16* u16Height ,SCALER_WIN eWindow);
MS_BOOL MApi_XC_EnableHVTotalValidation(MS_BOOL bEnable, XC_PANEL_INFO_EX *pPanelInfoEx, MS_U32 u32SizeofPanelInfoEx);
void    MApi_XC_SetCaptureWindowVsize(MS_U16 u16Vsize , SCALER_WIN eWindow);
void MApi_XC_Set_SVP_Status(MS_BOOL bIsSecure);
MS_BOOL MApi_XC_Get_SVP_Status(void);
void MApi_XC_Set_VTBlock_Status(MS_BOOL bIsSecure);
MS_BOOL MApi_XC_Get_VTBlock_Status(void);
void MApi_XC_Set_SmartZoom(MS_BOOL bEnable);
MS_BOOL MApi_XC_Get_SmartZoom(void);

//apiWble.h
MS_BOOL MApi_WBLE_Init(void);
void MApi_WBLE_EnableBLE(MS_BOOL bEnable);
void MApi_WBLE_Set_SlopeValue(MS_U8 u8Slope);

////////////////

//***************************************************
//             PANEL RELATED
//***************************************************
//void MApi_Init_PNLAdvInfo_beforePNLinit(void* pInitInfo);
//-------------------------------------------------------------------------------------------------
/// Skip Get Wait Vsync
/// @param eWindow              \b IN: Enable
/// @param Skip wait Vsync      \b IN: Disable wait Vsync
//-------------------------------------------------------------------------------------------------

MS_BOOL MApi_XC_Get_SkipWaitVsync( MS_BOOL eWindow);
//***************************************************
//             MISC RELATED
//***************************************************
void MApi_SetFrameCount(SCALER_WIN eWindow,MS_U16 fcount);
void MApi_XC2_SetFrameCount(SCALER_WIN eWindow,MS_U16 fcount);

void MApi_XC_SetSkipScalingSetting(MS_BOOL bEnable);
MS_BOOL MApi_XC_GetSkipScalingSetting(void);


#if 1 //NEED_IMPLEMENT_PQL_FUNCTIONS
#if 1
#define GM_GBD_MAX_SIZE                     21

// packet Colorimetry
typedef enum
{
    HDMI_COLORIMETRY_NONE,
    HDMI_COLORIMETRY_ITU601,
    HDMI_COLORIMETRY_ITU709,
    HDMI_COLORIMETRY_EXTEND,
} SC_HDMI_COLORIMETRY_FORMAT_e;

// packet Extended Colorimetry
typedef enum
{
    HDMI_EXTENDEDCOLORIMETRY_XVYCC601,
    HDMI_EXTENDEDCOLORIMETRY_XVYCC709,
    HDMI_EXTENDEDCOLORIMETRY_RESERVED,
} SC_HDMI_EXTENDEDCOLORIMETRY_FORMAT_e;

// IOCTL_SC_GET_HDMI_XVYCC
typedef struct
{
    SCALER_WIN srcIdx;
    SC_HDMI_COLORIMETRY_FORMAT_e eColorimetry;
    SC_HDMI_EXTENDEDCOLORIMETRY_FORMAT_e eExtColorimetry;
    MS_U8 GM_GBD[(GM_GBD_MAX_SIZE+0x1) & ~0x1];        // Currently, support up to 21 bytes GBD

} SC_GET_HDMI_XVYCC_t;
#endif
void MApi_XC_SetScaleFilter(MS_U8 u8VIDEO3D);
void MApi_XC_DLCInit(MS_U8 u8WinIdx, MS_U16 u16Histogram_Vstart, MS_U16 u16Histogram_Vend);
void MApi_XC_SetPieceWiseEnable(MS_U8 u8WinIdx, MS_U8 u8OnOff, MS_U8 *pPieceWiseXPosition);
void MApi_XC_SetColorAdaptiveRange(MS_U8 u8WinIdx, MS_U8 CbUp, MS_U8 CbDown, MS_U8 CrUp, MS_U8 CrDown);
void MApi_XC_SetHistogramReqEnable(MS_U8 u8WinIdx, MS_BOOL bEnable);
void MApi_XC_SetICCSaturationEnable(MS_U8 u8WinIdx, MS_BOOL bEnable);
void MApi_XC_SetIBCYEnable(MS_U8 u8WinIdx, MS_BOOL bEnable);
void MApi_XC_SetIHCHueEnable(MS_U8 u8WinIdx, MS_BOOL bEnable);
void MApi_XC_SetCSC_OffsetEnable(MS_U8 u8WinIdx, MS_BOOL isMinus16);
void MApi_XC_SetICCRegion(MS_U16 *region_table);
void MApi_XC_SetIHCRegion(MS_U16 *region_table);
void MApi_XC_SetLumaCurveEnable(MS_U8 u8WinIdx, MS_BOOL bEnable);
void MApi_XC_SetAdaptiveCGainEnable(MS_U8 u8WinIdx, MS_U8 u8OnOff);
void MApi_XC_SetLumaCurve(MS_U8 u8WinIdx, MS_U16* pu16LumaCurve);
void MApi_XC_SetICCDecreaseGainAdj(MS_U8 u8Gain);
void MApi_XC_SetIHCSkinHueAdj(MS_U8 u8WinIdx, MS_S8 s8Hue, MS_U8 u8SkinYIndex, MS_U8 u8SkinYLevel);
void MApi_XC_SetAutoNREnable(MS_U8 u8WinIdx, MS_U8 bOnOff);
void MApi_XC_SetCDNREnable(MS_U8 u8WinIdx, MS_U8 bOnOff);
void MApi_XC_SetCDNRIndex(MS_U8 u8WinIdx, MS_U8 *pIndex);
void MApi_XC_SetCDNRGain(MS_U8 u8WinIdx, MS_U8 *pDnrGain, MS_U8 *pPreSnrGain);
void MApi_XC_SetFrameTo48Hz(MS_BOOL bEnable);
MS_U16 MApi_XC_GetFrameRate(MS_U8 u8WinIdx);
void MApi_XC_SetCSCEnable(MS_U8 u8WinIdx, MS_BOOL bEnable);
void MApi_XC_SelectCSC(MS_U8 u8WinIdx, MS_U8 u8sel);
void MApi_XC_SetBlackLevel(MS_U8 u8WinIdx, MS_U8 index);
void MApi_XC_SetGammaTable(MS_U8 u8WinIdx, MS_U8* pu8Gamma_R, MS_U8* pu8Gamma_G, MS_U8* pu8Gamma_B);
void MApi_XC_SetGammaTableEx(MS_U8 u8WinIdx, MS_U8* pu8Gamma_R, MS_U8* pu8Gamma_G, MS_U8* pu8Gamma_B);
void MApi_XC_GetHDMIxvYCC(MS_U8 u8WinIdx, SC_GET_HDMI_XVYCC_t *pxvYCC);
void MApi_XC_SetICCYModelEnable(MS_U8 u8WinIdx, MS_BOOL bEnable);
void MApi_XC_SetIHCYModelEnable(MS_U8 u8WinIdx, MS_BOOL bEnable);
void MApi_XC_SetBlueStretchEnable(MS_U8 u8WinIdx, MS_BOOL bEnable);
void MApi_XC_SetICCSaturationAdj(MS_U8 u8WinIdx, MS_S8 s8Saturaton, MS_U8 u8colorType);
void MApi_XC_SetIHCHueAdj(MS_U8 u8WinIdx, MS_S8 s8Hue, MS_U8 u8colorType);
void MApi_XC_SetIBCYAdj(MS_U8 u8WinIdx, MS_U8 u8Y, MS_U8 u8colorType);
void MApi_XC_GetHistogramInfo(MS_U8 u8WinIdx, MS_U16* pu16Histogram, MS_U8* pu8Max, MS_U8* pu8Mix, MS_U8* pu8Avg, MS_BOOL* pbDataReady, MS_U16* pu16ColorCount);
MS_BOOL MApi_XC_Get_Record_Flag(void);
void MApi_XC_Set_Record_Flag(MS_BOOL bRecord_Flag);


#endif


#ifdef __cplusplus
}
#endif
#endif

