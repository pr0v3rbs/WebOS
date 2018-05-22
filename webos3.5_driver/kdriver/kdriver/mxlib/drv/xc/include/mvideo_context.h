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
//==============================================================================
// [mvideo_context.h]
// Date: 20081203
// Descriptions: structure pool
//==============================================================================
#ifndef MVIDEO_CONTEXT_H
#define MVIDEO_CONTEXT_H

#include "MsCommon.h"
#include "drvCMAPool.h"

//==============================================================================
//==============================================================================
#ifdef MVIDEO_CONTEXT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define XC_CONTEXT_ENABLE   1

#if 0//(defined ANDROID) //add for dbg convenience
#include <cutils/log.h>
#define printf LOGD
#endif

#define MS_DEBUG 1

#ifdef MS_DEBUG
#define XC_LOG_TRACE(_dbgSwitch_,  _fmt, _args...)                  \
    {                                                               \
        if( (_dbgSwitch_ & _u16XCDbgSwitch_InternalUseOnly) != 0   \
            && _eXCDbgSwitch_MessageType == E_XC_MESSAGE_TYPE_USER)\
        {                                                           \
            MDrv_XC_ShowTrace_Header(_dbgSwitch_);                  \
            printf("[%s,%5d] ",__FUNCTION__,__LINE__);                  \
            printf(_fmt, ##_args);                                  \
        }                                                           \
        else if( (_dbgSwitch_ & _u16XCDbgSwitch_InternalUseOnly) != 0   \
            && _eXCDbgSwitch_MessageType == E_XC_MESSAGE_TYPE_KERNEL)\
        {                                                           \
            MDrv_XC_ShowTrace_Header(_dbgSwitch_);                  \
            MDrv_XC_Printk("[%s,%5d] ",__FUNCTION__,__LINE__);                  \
            MDrv_XC_Printk(_fmt, ##_args);                                  \
        }\
        else if( (_dbgSwitch_ & _u16XCDbgSwitch_InternalUseOnly) != 0   \
            && _eXCDbgSwitch_MessageType == E_XC_MESSAGE_TYPE_FILE)\
        {                                                           \
            MDrv_XC_ShowTrace_Header(_dbgSwitch_);                  \
            MDrv_XC_PrintFile("[%s,%5d] ",__FUNCTION__,__LINE__);                  \
            MDrv_XC_PrintFile(_fmt, ##_args);                                  \
        }\
    }

#else
#define XC_LOG_TRACE(_dbgSwitch_,  _fmt, _args...)
#endif

///< number of panel_info_ex supported by current XC
#define PANEL_INFO_EX_NUM            3

// default value for SC_IP pixel repetition (gsrcinfo.status2.u8IP_PixelRep)
#define INVALID_IP_PIXELREPTITION   0xFF

// default timeout value for wait output vsync
#define DEFAULT_OUTPUT_VSYNC_TIMEOUT       25
#define MAX_DIP_BUFF_CNT 8

// for semaphore POOL
typedef enum
{
    E_XC_POOL_ID_INTERNAL_REGISTER = 0,          // for XC write register
    E_XC_POOL_ID_INTERNAL_VARIABLE = 1,          // for XC global variable
    E_XC_POOL_ID_INTERNAL_MENULOAD = 2,          // for menuload write register


    E_XC_POOL_ID_NUM,
    E_XC_POOL_ID_INTERNAL_REGISTER_1 = E_XC_POOL_ID_NUM,          // for XC write register
    E_XC_POOL_ID_INTERNAL_VARIABLE_1,          // for XC global variable
    E_XC_POOL_ID_INTERNAL_MENULOAD_1,          // for menuload write register

    E_XC_POOL_ID_MAX,
} E_XC_POOL_ID;

#if SC2_ENABLE

typedef enum
{
    E_XC_ID_REG = 0,          // for XC write register
    E_XC_ID_VAR = 1,          // for XC global variable
    E_XC_ID_MLD = 2,          // for menuload write register
} E_XC_POOL_ID_TYPE;


#endif //#if SC2_ENABLE



/// frame/field number stored in frame buffer
typedef enum
{
    IMAGE_STORE_UNDEFINE = 0,
    IMAGE_STORE_2_FIELDS = 1,
    IMAGE_STORE_4_FIELDS,
    IMAGE_STORE_6_FIELDS,
    IMAGE_STORE_8_FIELDS,
    IMAGE_STORE_10_FIELDS,
    IMAGE_STORE_12_FIELDS,
    IMAGE_STORE_14_FIELDS,
    IMAGE_STORE_16_FIELDS,
    IMAGE_STORE_18_FIELDS,
    IMAGE_STORE_20_FIELDS,

    IMAGE_STORE_2_FRAMES = 100,
    IMAGE_STORE_3_FRAMES,
    IMAGE_STORE_4_FRAMES,
    IMAGE_STORE_5_FRAMES,
    IMAGE_STORE_6_FRAMES,
    IMAGE_STORE_7_FRAMES,
    IMAGE_STORE_8_FRAMES,
    IMAGE_STORE_10_FRAMES,
    IMAGE_STORE_12_FRAMES
} XC_FRAME_STORE_NUMBER;

// for ipm connect opm or frcm
typedef enum
{
    E_XC_IPM_CONNETC_SUB_FRCM = 0,
    E_XC_IPM_CONNETC_MAIN_FRCM,
    E_XC_IPM_CONNETC_SUB_OPM,
    E_XC_IPM_CONNETC_MAIN_OPM
} E_XC_IPM_CONNECT_ID;

// for frcm connect scm or frcm
typedef enum
{
    E_XC_FRCMW_CONNETC_SUB_SCM = 0,
    E_XC_FRCMW_CONNETC_MAIN_SCM,
    E_XC_FRCMW_CONNETC_SUB_FRCM,
    E_XC_FRCMW_CONNETC_MAIN_FRCM
} E_XC_FRCMW_CONNECT_ID;

// for memory scm->frcm or direct frcm
typedef enum
{
    E_XC_SCM_TO_FRCM = 0,
    E_XC_DIRECT_FRCM,
    E_XC_STREAM_FLOW_MAX
} E_XC_STREAM_FLOW;

////===========================>>SW DB Buffer
typedef struct
{
    MS_U16 u16H_CapStart;
    MS_U16 u16H_CapSize;
    MS_U16 u16V_CapStart;
    MS_U16 u16V_CapSize;

    MS_U16 u16H_DisStart;
    MS_U16 u16H_DisEnd;
    MS_U16 u16V_DisStart;
    MS_U16 u16V_DisEnd;

    MS_U32 u32H_PreScalingRatio;
    MS_U32 u32V_PreScalingRatio;
    MS_U32 u32H_PostScalingRatio;
    MS_U32 u32V_PostScalingRatio;

    MS_U16 u16VLen;
    MS_U16 u16VWritelimit;

    MS_U16 u16DNROffset;
    MS_U16 u16DNRFetch;
    MS_U16 u16OPMOffset;
    MS_U16 u16OPMFetch;
    MS_U16 u16LBOffset;
    MS_U16 u16DispOffset;
    MS_U16 u16AlignWidth;

    MS_PHY u32WritelimitBase;
    MS_PHY u32DNRBase0;
    MS_PHY u32DNRBase1;
    MS_PHY u32DNRBase2;

    MS_PHY u32OPMBase0;
    MS_PHY u32OPMBase1;
    MS_PHY u32OPMBase2;
    MS_U8  u8Peakingmode;
    // add FRCM memory control
    MS_PHY u32FRCM_WBase0;
    MS_PHY u32FRCM_WBase1;
    MS_PHY u32FRCM_WBase2;
    MS_U16 u16FRCM_WOffset;
    MS_U16 u16FRCM_WFetch;

    MS_PHY u32FRCM_RBase0;
    MS_PHY u32FRCM_RBase1;
    MS_PHY u32FRCM_RBase2;
    MS_U16 u16FRCM_ROffset;
    MS_U16 u16FRCM_RFetch;

    MS_PHY u32FRCMWritelimitBase;
    MS_U16 u16VFRCMWritelimit;
    MS_U16 u16FRCMVLen;
    MS_BOOL bopm_lcnt_inv;
    MS_BOOL bIsConnectOPM;
    MS_BOOL b3Dstate;
    MS_BOOL bInterlace;	
    MirrorMode_t eMirrorMode;
}SC_SWDB_INFO, *P_SC_SWDB_INFO;

typedef struct
{
    // XC need below information do to frame lock
    MS_U16 u16HStart;           ///<DE H start
    MS_U16 u16VStart;           ///<DE V start
    MS_U16 u16Width;            ///<DE H width
    MS_U16 u16Height;           ///< DE V height
    MS_U16 u16HTotal;           ///<H total
    MS_U16 u16VTotal;           ///<V total
    MS_U16 u16VFreq;            ///<Panel output Vfreq
}XC_PANEL_TIMING, *PXC_PANEL_TIMING;

// OSDC TIMING GEN
typedef struct
{
    MS_U16 u16OC_Tgen_HTotal;
    MS_U16 u16OC_Tgen_VTotal;

    MS_U16 u16OC_Tgen_Hsync_Start;
    MS_U16 u16OC_Tgen_Hsync_End;
    MS_U16 u16OC_Tgen_HframDE_Start;
    MS_U16 u16OC_Tgen_HframDE_End;

    MS_U16 u16OC_Tgen_Vsync_Start;
    MS_U16 u16OC_Tgen_Vsync_End;
    MS_U16 u16OC_Tgen_VframDE_Start;
    MS_U16 u16OC_Tgen_VframDE_End;

    MS_BOOL bOC_ClK_En;
    MS_BOOL bOC_Mixer_Bypass_En;
    MS_BOOL bOC_Mixer_InvAlpha_En;
    MS_BOOL bOC_Mixer_Hsync_Vfde_Out;    // 1:hs_out = hs&vfde
    MS_BOOL bOC_Mixer_Hfde_Vfde_Out;     // 1:hs_out = de(hfde)&vfde

}XC_OSDC_TIMING, *PXC_OSDC_TIMING;

// OSDC MISC Control
typedef struct
{
    MS_BOOL bOC_ClK_En;
    MS_BOOL bOC_Mixer_Bypass_En;
    MS_BOOL bOC_Mixer_InvAlpha_En;
    MS_BOOL bOC_Mixer_Hsync_Vfde_Out;    // 1:hs_out = hs&vfde
    MS_BOOL bOC_Mixer_Hfde_Vfde_Out;     // 1:hs_out = de(hfde)&vfde
    MS_U16  u16OC_Lpll_type;
	MS_U8   u8OC_OutputFormat;           // output bit order
}XC_OSDC_MSIC_CTRL, *PXC_OSDC_MSIC_CTRL;

typedef struct
{
    //-------------------------
    // customized pre scaling
    //-------------------------
    MS_BOOL bPreHCusScaling;            ///<assign pre H customized scaling instead of using XC scaling
    MS_U16  u16PreHCusScalingSrc;       ///<pre H customized scaling src width
    MS_U16  u16PreHCusScalingDst;       ///<pre H customized scaling dst width
    MS_BOOL bPreVCusScaling;            ///<assign pre V manuel scaling instead of using XC scaling
    MS_U16  u16PreVCusScalingSrc;       ///<pre V customized scaling src height
    MS_U16  u16PreVCusScalingDst;       ///<pre V customized scaling dst height

    MS_BOOL bAutoBestPreScaling;

    //-------------------------
    // memory format
    //-------------------------
    XC_FRAME_STORE_NUMBER eFrameStoreNumber;    ///< the number of frame/field stored in frame bufffer
    XC_FRAME_STORE_NUMBER eFRCMFrameStoreNumber;    ///< the number of frame/field stored in frcm frame bufffer

    MS_U16 u16LBOffset;                 ///<line buffer offset
    MS_U16 u16OutputVFreqAfterFRC;
    E_XC_SOURCE_TO_VE eSourceToVE;
    XC_PANEL_TIMING stXCPanelDefaultTiming;
    MS_BOOL bIPMemWriteRequestDisabled;
    MirrorMode_t eMirrorMode;          ///<For API to get current XC mirror type
    XC_PANEL_INFO_EX stPanelInfoEx[PANEL_INFO_EX_NUM];
    MS_U8  u8IP_PixelRep;              ///<For SC_ip set fir down sample divider
    MS_BOOL bForceInterlaceInPMode;    ///<Force InterlaceMode in IP1 when source is Progessive.
    MS_BOOL bInFRCMode;
    MS_BOOL bIsInitOSDC;
    XC_OSDC_TIMING stXCOSDCTIMING;
    XC_OSDC_MSIC_CTRL stXCOSDC_CTRL;
    MS_U16  u16OC_PreLplltype;
    MS_U16 	u16OC_Tgen_HTotal;
    MS_U16 	u16OC_Tgen_VTotal;

    MS_U16 u16OP1_OutputVFreqAfterFRC;  /// ***When FRC inside chip turn on the FRC featur, it will have XC_OP and FRC_OP
                                        /// So, assign u16OP1_OutputVFreqAfterFRC for XC_OP
                                        /// assign u16OutputVFreqAfterFRC for FRC_OP
                                        /// ***But, non-FRC inside chip, this value will return u16OutputVFreqAfterFRC
	//--------------------------
	// dynamic scaling parameter
	//--------------------------
	MS_WINDOW_TYPE stCodedWin;
    //-------------------------
    // SW DS
    //-------------------------
    MS_U16 u16VirtualBox_Width;
    MS_U16 u16VirtualBox_Height;
    MS_U32  u32PreCMAMemDualSCMSize;
} XC_InternalStatus2, *P_XC_InternalStatus2;

/// XC internal status
typedef struct
{
    //----------------------
    // Customer setting
    //----------------------
    INPUT_SOURCE_TYPE_t enInputSourceType;      ///< Input source type

    //----------------------
    // Window
    //----------------------
    MS_WINDOW_TYPE stCapWin;       ///<Capture window
    MS_WINDOW_TYPE stDispWin;      ///<Display window
    MS_WINDOW_TYPE stCropWin;      ///<Crop window

    //----------------------
    // Timing
    //----------------------
    MS_BOOL bInterlace;             ///<Interlaced or Progressive
    MS_BOOL bHDuplicate;            ///<flag for vop horizontal duplicate, for MVD, YPbPr, indicate input double sampled or not
    MS_U16  u16InputVFreq;          ///<Input V Frequency, VFreqx10, for calculate output panel timing
    MS_U16  u16InputVTotal;         ///<Input Vertical total, for calculate output panel timing
    MS_U16  u16DefaultHtotal;       ///<Default Htotal for VGA/YPbPr input
    MS_U16   u16DefaultPhase;         ///<Default Phase for VGA/YPbPr input

    //----------------------
    // customized post scaling
    //----------------------
    MS_BOOL bHCusScaling;           ///<assign H customized scaling instead of using XC scaling
    MS_U16  u16HCusScalingSrc;      ///<H customized scaling src width
    MS_U16  u16HCusScalingDst;      ///<H customized scaling dst width
    MS_BOOL bVCusScaling;           ///<assign V manuel scaling instead of using XC scaling
    MS_U16  u16VCusScalingSrc;      ///<V customized scaling src height
    MS_U16  u16VCusScalingDst;      ///<V customized scaling dst height

    //--------------
    // 9 lattice
    //--------------
    MS_BOOL bDisplayNineLattice;    ///<used to indicate where to display in panel and where to put in frame buffer

    //----------------------
    // XC internal setting
    //----------------------

    /* scaling ratio */
    MS_U16 u16H_SizeAfterPreScaling;         ///<Horizontal size after prescaling
    MS_U16 u16V_SizeAfterPreScaling;         ///<Vertical size after prescaling
    MS_BOOL bPreV_ScalingDown;      ///<define whether it's pre-Vertical scaling down
    MS_BOOL bPreScalingRatioChanged;        ///<True: PreScaling Ratio is different from with previous setWindow. False: PreScaling Ratio is same as previous one.

    /* real crop win in memory */
    MS_WINDOW_TYPE ScaledCropWin;

    /* others */
    MS_U32 u32Op2DclkSet;           ///<OP to Dot clock set

    /* Video screen status */
    MS_BOOL bBlackscreenEnabled;      ///<Black screen status
    MS_BOOL bBluescreenEnabled;       ///<Blue screen status
    MS_BOOL bAutoNoSignalEnabled;     ///< Auto No signal enabled
    MS_U16 u16VideoDark;            ///<Video dark

    MS_U16 u16V_Length;             ///<for MDrv_Scaler_SetFetchNumberLimit(opm v lenth)
    MS_U16 u16FRCMV_Length;         ///<for MDrv_Scaler_SetFetchNumberLimit(FRCM v length)
    MS_U16 u16BytePerWord;          ///<BytePerWord in Scaler
    MS_U16 u16OffsetPixelAlignment; ///<Pixel alignment of Offset (including IPM/OPM)
    MS_U8 u8BitPerPixel;            ///<Bits number Per Pixel
    MS_DEINTERLACE_MODE eDeInterlaceMode;       ///<De-interlace mode
    MS_U8 u8DelayLines;             ///<delay lines
    MS_BOOL bMemFmt422;             ///<whether memory format is 422, for easy usage
    MS_IMAGE_STORE_FMT eMemory_FMT; ///<memory format
    MS_BOOL bForceNRoff;            ///<Force NR off
    MS_BOOL bEn3DNR;                ///<whether it's 3DNR enabled
    MS_BOOL bUseYUVSpace;           ///< color format before 3x3 matrix
    MS_BOOL bMemYUVFmt;             ///< memroy color format
    MS_BOOL bForceRGBin;            ///<Force set format in memory as RGB (only for RGB input source)
    MS_BOOL bLinearMode;            ///<Is current memory format LinearMode?
    MS_BOOL bRWBankAuto;            ///<accounding to framebuffer ,Auto adjust read/write bank mapping mode

    // frame lock related
    // only main can select FB/FBL because panel output timing depends on main window, sub will always use FB
    MS_BOOL bFBL;                   ///<whether it's framebufferless case
    MS_BOOL bR_FBL;                   ///<whether it's request framebufferless case
    MS_BOOL bFastFrameLock;         ///<whether framelock is enabled or not
    MS_BOOL bDoneFPLL;              ///<whether the FPLL is done
    MS_BOOL bEnableFPLL;            ///<enable FPLL or not
    MS_BOOL bFPLL_LOCK;             ///<whether FPLL is locked (in specific threshold.)

    // Capture_Memory
    MS_U32 u32IPMBase0;             ///<IPM base 0
    MS_U32 u32IPMBase1;             ///<IPM base 1
    MS_U32 u32IPMBase2;             ///<IPM base 2
    MS_U16 u16IPMOffset;            ///<IPM offset
    MS_U16 u16IPMFetch;             ///<IPM fetch

    E_XC_STREAM_FLOW  enPreStreamFlow; ///<memory scm->frcm or direct frcm
    //-------------------------------------------------------
    // To keep interface compatibility, please don't add any
    // new members to XC_InternalStatus.
    // Please add to XC_InternalStatus2.
    //-------------------------------------------------------
    XC_InternalStatus2 Status2;
    MS_BOOL b3DMenuload;            /// for 14:9, zoom mode feature
    MS_U16  u16BK23_0B;             /// temp solution for 3D menuload
    MS_BOOL bIsConnectOPM;
    MS_BOOL bpqmap;
    MS_BOOL btimingchg;
    MS_BOOL bxcoutput;
    MS_BOOL bmemfmt;
    MS_BOOL bcropwindowchg;
    MS_U32  u32PreCMAMemSCMSize[CMA_XC_MEM_NUM];
    MS_U32  u32PreCMAMemFRCMSize[CMA_XC_MEM_NUM];
}XC_InternalStatus, *P_XC_InternalStatus;

typedef struct
{
    MS_U8               u8DIP_BufCnt;
    MS_PHY              u64DIP_YBuf[MAX_DIP_BUFF_CNT];
    MS_PHY              u64DIP_CBuf[MAX_DIP_BUFF_CNT];
} XC_DIP_BUFFER_INFO;

//SC
typedef struct
{
	//----------------------
	// Customer setting
	//----------------------
	INPUT_SOURCE_TYPE_t enInputSourceType; 	 ///< Input source type
	//----------------------
	// Source type for DIP
	//----------------------
    SCALER_DIP_SOURCE_TYPE  enDIPSourceType;   ///<DIP input source type
	//----------------------
	// Window
	//----------------------
	MS_WINDOW_TYPE stCapWin;		///<Capture window
	//----------------------
	// Timing
	//----------------------
	MS_BOOL bInterlace;			 ///<Interlaced or Progressive
	MS_BOOL bHDuplicate;			 ///<flag for vop horizontal duplicate, for MVD, YPbPr, indicate input double sampled or not
	//----------------------
	// customized post scaling
	//----------------------
	MS_BOOL bHCusScaling;			 ///<assign H customized scaling instead of using XC scaling
	MS_U16  u16HCusScalingSrc; 	 ///<H customized scaling src width
	MS_U16  u16HCusScalingDst; 	 ///<H customized scaling dst width
	MS_BOOL bVCusScaling;			 ///<assign V manuel scaling instead of using XC scaling
	MS_U16  u16VCusScalingSrc; 	 ///<V customized scaling src height
	MS_U16  u16VCusScalingDst; 	 ///<V customized scaling dst height
    //-------------------------
    // customized pre scaling
    //-------------------------
    MS_BOOL bPreHCusScaling;            ///<assign pre H customized scaling instead of using XC scaling
    MS_U16  u16PreHCusScalingSrc;       ///<pre H customized scaling src width
    MS_U16  u16PreHCusScalingDst;       ///<pre H customized scaling dst width
    MS_BOOL bPreVCusScaling;            ///<assign pre V manuel scaling instead of using XC scaling
    MS_U16  u16PreVCusScalingSrc;       ///<pre V customized scaling src height
    MS_U16  u16PreVCusScalingDst;       ///<pre V customized scaling dst height
	//----------------------
	// XC internal setting
	//----------------------
	/* scaling ratio */
	MS_U16 u16H_SizeAfterPreScaling;		  ///<Horizontal size after prescaling
	MS_U16 u16V_SizeAfterPreScaling;		  ///<Vertical size after prescaling

	MS_U8 u8DelayLines;			 ///<delay lines
	MS_BOOL bMemFmt422;			 ///<whether memory format is 422, for easy usage
	MS_BOOL bMemYUVFmt;			 ///< memroy color format

	XC_DIP_BUFFER_INFO stBufInfo;
}XC_DIP_InternalStatus, *P_XC_DIP_InternalStatus;

typedef struct
{
    MS_S32 s32MutexCnt;
    MS_S32 s32CurThreadId;
    MS_S32 s32ComMutex;
}XC_Mutex;

typedef struct
{
    XC_InternalStatus StatusInfo[MAX_WINDOW];
#ifndef UTOPIA_V2
	XC_DIP_InternalStatus DIP_StatusInfo[3];
#endif
}XC_Context_Shared;

typedef struct
{
    XC_Mutex Mutex;
    XC_Context_Shared *pCommInfo;

    // FPLL related
    MS_BOOL bEnableFPLLManualDebug;     ///< if enabled, no set to phase limit, limitD5D6D7RK
}XC_Context;

typedef struct
{
    MS_BOOL bTop;
    MS_BOOL bBottom;
    MS_BOOL bLeft;
    MS_BOOL bRight;
    MS_U8 u8TopBold; //0~3
    MS_U8 u8BottomBold; //0~3
    MS_U8 u8LeftBold; //0~3
    MS_U8 u8RightBold; //0~3
} XC_OP1_PatGen_Border;

// Path
typedef struct s_Data_Path
{

    //////////////////////////////
    // Path Type
    //////////////////////////////
    E_PATH_TYPE e_PathType;

    //////////////////////////////
    // Path Status
    //////////////////////////////

    /*
        Make the signal go through the path. If this flag is not enabled,
        synchronous_event_handler and source_monitor are useless.
     */
    #define DATA_PATH_ENABLE BIT(0)

    /*
        Used to identify a path is created or not. ( 1: path created )
     */
    #define DATA_PATH_USING BIT(1)

    /*
        Used to identify the priodic handler enabled or disabled
     */
    #define ENABLE_PERIODIC_HANDLER BIT(2)

    MS_U8 u8PathStatus;

    //////////////////////////////
    // A path has a format like Src -> Dest
    //////////////////////////////
    INPUT_SOURCE_TYPE_t src;
    E_DEST_TYPE dest;

    //////////////////////////////
    // thread for source.
    //////////////////////////////
    void (*path_thread)(INPUT_SOURCE_TYPE_t src, MS_BOOL bRealTimeMonitorOnly);

    ///////////////////////////////////////
    // Customize function.
    // This will be executed automatically when synchronous event triggered.
    ///////////////////////////////////////
    void (* synchronous_event_handler )(INPUT_SOURCE_TYPE_t src, void* para);

    ///////////////////////////////////////
    // Customize function.
    // This will be executed automatically when destination on/off event triggered.
    ///////////////////////////////////////
    void (* dest_on_off_event_handler )(INPUT_SOURCE_TYPE_t src, void* para);

    ///////////////////////////////////////
    // Customize function.
    // This will be executed periodically after enabled destinatio period handler
    ///////////////////////////////////////
    void (* dest_periodic_handler )(INPUT_SOURCE_TYPE_t src, MS_BOOL bRealTimeMonitorOnly);
}MUX_DATA_PATH;


// Path Collection
typedef struct s_Mux_InputSource_Attr
{
    // The number of pathes which is used right now.
    MS_U8  u8PathCount;

    void (*input_source_to_input_port)(INPUT_SOURCE_TYPE_t src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count );
    // Pathes.
    // Path array in MAPP_MUX is constructed [Sync path, Sync path, ..., Async path, Async path]
    MUX_DATA_PATH Mux_DataPaths[MAX_DATA_PATH_SUPPORTED];

    // indicate how many times the src created
    MS_U16 u16SrcCreateTime[INPUT_SOURCE_NUM];
}InputSource_Mux_Attr;

#if (XC_CONTEXT_ENABLE)
    #define gSrcInfo    (pXCResourcePrivate->stdrvXC_MVideo._SCShared.StatusInfo)
    #ifndef UTOPIA_V2
        #define g_DIPSrcInfo (pXCResourcePrivate->stdrvXC_MVideo._SCShared.DIP_StatusInfo)
    #endif
#else
    INTERFACE XC_InternalStatus  gSrcInfo[2];
    #ifndef UTOPIA_V2
        INTERFACE XC_DIP_InternalStatus  g_DIPSrcInfo[3];
    #endif
#endif

#define IsVMirrorMode(eWindow) ((MIRROR_V_ONLY == gSrcInfo[(eWindow)].Status2.eMirrorMode) || (MIRROR_HV == gSrcInfo[(eWindow)].Status2.eMirrorMode))
#define IsHMirrorMode(eWindow) ((MIRROR_H_ONLY == gSrcInfo[(eWindow)].Status2.eMirrorMode) || (MIRROR_HV == gSrcInfo[(eWindow)].Status2.eMirrorMode))

// for Fast Boot
#define MID_KEY_LENGTH 64
#define HDCP_KEY_LENGTH 289
#define EDID_KEY_LENGTH 256

INTERFACE MS_U16 _u16XCDbgSwitch_InternalUseOnly;
INTERFACE E_XC_MESSAGE_TYPE _eXCDbgSwitch_MessageType;
// for Utopia 2.0 to Utopia 1.0 compatibility.
INTERFACE void* pu32XCInst;
INTERFACE void* pu32XCInst_private;
//K3_U2 move to EX later
INTERFACE void* pu32XCInst_1;
INTERFACE void* g_pXCResource[E_XC_POOL_ID_MAX];
//EX_ADD
INTERFACE MS_BOOL g_bIsIMMESWITCH;
INTERFACE MS_BOOL g_bDVI_AUTO_EQ;
INTERFACE MS_BOOL g_bIMMESWITCH_DVI_POWERSAVING;

INTERFACE XC_CMA_CONFIG stXC_CMA_Config[CMA_XC_MEM_NUM];

INTERFACE struct CMA_Pool_Init_Param CMA_Pool_Init_PARAM[CMA_XC_MEM_NUM];


INTERFACE void MDrv_XC_ShowTrace_Header(MS_U16 u16LogSwitch); //NO_NEED
INTERFACE void MDrv_XC_Printk(char *fmt, ...);
INTERFACE void MDrv_XC_PrintFile(char *fmt, ...);

INTERFACE void mvideo_sc_variable_init(void *pInstance, MS_BOOL bFirstInstance, XC_INITDATA *pXC_InitData);
INTERFACE void mvideo_sc_getcropwin( void *pInstance, MS_WINDOW_TYPE *pCropWin, SCALER_WIN eWindow );
INTERFACE MS_U8  mvideo_pnl_is_dualport(void *pInstance);
INTERFACE void mvideo_pnl_set_dclkmode(void *pInstance, E_XC_PNL_OUT_TIMING_MODE enPnl_Out_Timing_Mode);

INTERFACE MS_BOOL mvideo_sc_is_enable_3dnr(void *pInstance, SCALER_WIN eWindow);
INTERFACE void mvideo_sc_set_3dnr(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);

INTERFACE void MDrv_XC_GetInfo(void *pInstance, XC_ApiInfo *pXC_Info);
INTERFACE void MDrv_XC_SetHdmiSyncMode(void *pInstance, E_HDMI_SYNC_TYPE eSynctype, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_Switch_DE_HV_Mode_By_Timing(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_Set_DE_Lock_Mode(void *pInstance, SCALER_WIN eWindow);

//DLC
INTERFACE void MDrv_DLC_Handler(MS_BOOL bWindow);
INTERFACE MS_BOOL MDrv_DLC_GetHistogram(MS_BOOL bWindow);
INTERFACE MS_U8 MDrv_DLC_GetAverageValue(void);
INTERFACE MS_U16 MDrv_XC_PCMonitor_Get_Vtotal(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_XC_PCMonitor_Get_Htotal(void *pInstance,SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_PCMonitor_GetDEWidthHeightInDEByPassMode(void* pInstance, MS_U16* pu16Width, MS_U16* pu16Height, SCALER_WIN eWindow);

INTERFACE MS_U16 MDrv_XC_PCMonitor_Get_HFreq(void *pInstance,MS_BOOL bSet,SCALER_WIN eWindow);

INTERFACE MS_BOOL MDrv_XC_Get_SCMI_Type(void); //NO_NEED

INTERFACE E_APIXC_ReturnValue MDrv_XC_OSDC_Control(void *pInstance, MS_U32 eCtrl_type);

//-------------------------------------------------------------------------------------------------
/// MDrv_XC_OSDC_TGEN_Setting
/// @param  pstOC_Tgen                 \b IN: the timing control
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_XC_OSDC_TGEN_Setting(void *pInstance, XC_OSDC_TIMING *pstOC_Tgen);

//-------------------------------------------------------------------------------------------------
/// MDrv_XC_OSDC_MISC_Control
/// @param  pstOC_Ctrl                 \b IN: the OSDC MISC control
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_XC_OSDC_MISC_Control(void *pInstance, XC_OSDC_MSIC_CTRL *pstOC_Ctrl);

//-------------------------------------------------------------------------------------------------
/// MDrv_XC_OSDC_GetDataFromRegister
/// @param  pstOC_Tgen                 \b IN: the timing control
/// @param  pstOC_Ctrl                 \b IN: the OSDC MISC control
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_XC_OSDC_GetDataFromRegister(void *pInstance, XC_OSDC_TIMING *pstOC_Tgen, XC_OSDC_MSIC_CTRL *pstOC_Ctrl);

INTERFACE void MDrv_XC_OSDC_TGENRST_EN(void *pInstance, MS_BOOL bEnable);
INTERFACE void MDrv_XC_OSDC_SWRST_EN(void *pInstance, MS_BOOL bEnable);

INTERFACE E_APIXC_ReturnValue MDrv_XC_GetChipCaps(E_XC_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);
INTERFACE void MDrv_XC_FreezeImg(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);


#if SC2_ENABLE
INTERFACE void MDrv_XC_SetDeviceOffset(void *pInstance);

#else //SC2_ENABLE
INTERFACE void MDrv_XC_SetDeviceOffset(void);

#endif //#if SC2_ENABLE


#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
// For HW internal test pattern using
INTERFACE MS_BOOL MDrv_SC_OP1_PatGen_Init(void *pInstance, EN_XC_OP1_PATGEN_MODE mode);
INTERFACE E_APIXC_ReturnValue MDrv_SC_Set_OP1_TestPattern(void *pInstance, EN_XC_OP1_PATTERN pattern, EN_XC_OP1_PATGEN_MODE mode);
INTERFACE E_APIXC_ReturnValue MDrv_SC_Set_OP1_TestPattern_RGB(void *pInstance, EN_XC_OP1_PATTERN ePattern,EN_XC_OP1_PATGEN_MODE eMode, MS_U16 u16RVal, MS_U16 u16GVal, MS_U16 u16BVal);
INTERFACE MS_BOOL MDrv_SC_OP1_PatGen_Disable(void *pInstance);
INTERFACE void MDrv_SC_OP1_Align_Width_Height(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt, MS_U16 *u16AlignWidth, MS_U16 *u16AlignHeight);
INTERFACE void MDrv_SC_OP1_HW_Default_Pattern(void *pInstance, MS_U8 u8Val);
INTERFACE void MDrv_SC_OP1_WB_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_Pure_Color_V_Bar_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_Pure_Color_H_Bar_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_Pure_Gray_Bar_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt, MS_U8 block_Size, MS_U8 repeat_Num, MS_U8 level);
INTERFACE void MDrv_SC_OP1_Pure_RGB_32_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt, MS_U8 block_Size,MS_U8 repeat_Num,MS_U8 level);
INTERFACE void MDrv_SC_OP1_Pure_RGB_Checkerboard_Pattern_GBChange(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_Dotfade_Gray_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_Calibratoin_Color_Dotfade_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_1_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_2_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_3_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_3D_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt);
INTERFACE void MDrv_SC_OP1_Pure_Color_Tile_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt, MS_U16 u16RVal, MS_U16 u16GVal, MS_U16 u16BVal);
#endif
#endif

INTERFACE E_HDMI_SYNC_TYPE MDrv_XC_GetHdmiSyncMode(void *pInstance,SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_IsCurrentRequest_FrameBufferLessMode(void *pInstance);
INTERFACE MS_BOOL MDrv_XC_IsCurrentFrameBufferLessMode(void *pInstance);
INTERFACE MS_BOOL MDrv_XC_GetStatus(void *pInstance, XC_ApiStatus *pDrvStatus, SCALER_WIN eWindow);
INTERFACE MirrorMode_t MDrv_XC_GetMirrorModeTypeEx(void *pInstance, SCALER_WIN eWindow);


#if SC2_ENABLE
INTERFACE MS_U32 MDrv_XC_Get_Semaphore(void *pInstance,E_XC_POOL_ID_TYPE eID_TYPE);
INTERFACE MS_U32 MDrv_XC_Release_Semaphore(void *pInstance,E_XC_POOL_ID_TYPE eID_TYPE);

#else //SC2_ENABLE
INTERFACE MS_U32 MDrv_XC_Get_Semaphore(E_XC_POOL_ID eID);
INTERFACE MS_U32 MDrv_XC_Release_Semaphore(E_XC_POOL_ID eID);

#endif //#if SC2_ENABLE

INTERFACE E_XC_POOL_ID MDrv_XC_Resource_Mapping(MS_U32 u32InstanceID, E_XC_POOL_ID eID);
INTERFACE E_APIXC_ReturnValue MDrv_XC_Init_MISC(void *pInstance, XC_INITMISC *pXC_Init_Misc, MS_U32 u32InitMiscDataLen);

#if 1

INTERFACE void MDrv_XC_Set_VOP(void *pInstance, MS_XC_VOP_Data *pstVOPData);


#endif //#if SC2_ENABLE

#undef INTERFACE
#endif /* MVIDEO_CONTEXT_H */

