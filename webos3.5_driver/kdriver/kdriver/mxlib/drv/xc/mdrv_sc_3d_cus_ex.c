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

#ifndef DRV_SC_3D_EX_C
#define DRV_SC_3D_EX_C

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drv_sc_ip.h"

#include "mhal_pip.h"
#include "mhal_mux.h"
#include "mhal_ip.h"

#include "halCHIP.h"
#include "drv_sc_scaling.h"
#include "xc_hwreg_utility2.h"
#include "mhal_hdmi.h"

#include "mdrv_sc_3d_cus_ex.h"



#ifdef __cplusplus
extern "C"
{
#endif

#define SC_3D_DBG(x)    //x
//----------------------------------------
//--------3D macro define-----------------
//----------------------------------------

//#define MS_IPM_BASE0(eWin) (MDrv_XC_GetIPMBase(0, eWin))
//#define MS_IPM_BASE1(eWin) (MDrv_XC_GetIPMBase(1, eWin))

#define MS_IPM_BASE0(pInstance, eWin) (MDrv_XC_GetIPMBase(pInstance, 0, eWin))
#define MS_IPM_BASE1(pInstance, eWin) (MDrv_XC_GetIPMBase(pInstance, 1, eWin))

#if 1
MS_U32 MDrv_SC_3D_Adjust_Cus3DCrop_OPMBase(void *pInstance, XC_InternalStatus *pSrcInfo, MS_U16 u16DNROffset,
        MS_U32 u8BytesPer2Pixel,
        MS_U16 u16CropX,
        MS_U16 u16CropY)
{


//#if (HW_DESIGN_3D_VER == 2)
#if 0
    return 0;
#else
    MS_U32 u32OPMPixelOffset = 0;
    MS_U32 u32OPMCropOffset = 0;
    MS_U16 u16LBOffset = 0;

    SC_3D_DBG(printf("MDrv_SC_3D_Adjust_Cus3DCrop_OPMBase(): u16DNROffset=%u,u8BytesPer2Pixel=%lu,bInterlace=%u,u16CropX=%u,u16CropY=%u\n",
                     u16DNROffset,u8BytesPer2Pixel,pSrcInfo->bInterlace,u16CropX,u16CropY);)
    if(pSrcInfo->bInterlace)
    {
        u32OPMPixelOffset=(MS_U32)(u16CropY/2) * u16DNROffset;
    }
    else
    {
        u32OPMPixelOffset=(MS_U32)(u16CropY) * u16DNROffset;
    }
    u32OPMPixelOffset += (MS_U32)(u16CropX);

    // calculate LB offset
    u16LBOffset = u32OPMPixelOffset % OFFSET_PIXEL_ALIGNMENT;
    pSrcInfo->Status2.u16LBOffset = u16LBOffset;
    u32OPMPixelOffset -= u16LBOffset;

    // calcuate OPM offset
    u32OPMPixelOffset *= 2;

    if(u32OPMPixelOffset != 0)
    {
        u32OPMCropOffset = u32OPMPixelOffset * u8BytesPer2Pixel / 2 / BYTE_PER_WORD;
    }

    return ((MS_IPM_BASE0(pInstance,MAIN_WINDOW) + u32OPMCropOffset + 0x01) & ~0x01L);
#endif
}

MS_BOOL MDrv_SC_3D_SetCus3DWindow(XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow >= MAX_CHANNEL)
    {
        return FALSE;
    }

#if (HW_DESIGN_3D_VER == 2)
    return FALSE;
#else
    if(_stCus3DProcess.bSetCusCropWindow[eWindow])
    {
        //check para later
        pSrcInfo->stCropWin.x      = _stCus3DProcess.stCus3DCropWindow[eWindow].x;
        pSrcInfo->stCropWin.width  = _stCus3DProcess.stCus3DCropWindow[eWindow].width;
        pSrcInfo->stCropWin.y      = _stCus3DProcess.stCus3DCropWindow[eWindow].y;
        pSrcInfo->stCropWin.height = _stCus3DProcess.stCus3DCropWindow[eWindow].height;
    }

    if(_stCus3DProcess.bSetCusDispWindow[eWindow])
    {
        //check para later
        pSrcInfo->stDispWin.x      = _stCus3DProcess.stCus3DDispWindow[eWindow].x;
        pSrcInfo->stDispWin.width  = _stCus3DProcess.stCus3DDispWindow[eWindow].width;
        pSrcInfo->stDispWin.y      = _stCus3DProcess.stCus3DDispWindow[eWindow].y;
        pSrcInfo->stDispWin.height = _stCus3DProcess.stCus3DDispWindow[eWindow].height;
    }
    return TRUE;
#endif
}


//-------------------------------------------------------------------------------------------------
/// Get Custom 3D Mode
/// @param  eChannelType                      \b IN: which channel
/// @return CUS3DMODE                         \b OUT: Custom 3D Mode
//-------------------------------------------------------------------------------------------------
CUS3DMODE MApi_XC_GetCus3DMode(SCALER_WIN eChannelType)
{
    CUS3DMODE enCus3DMode = E_CUS3D_NO;
    MS_ASSERT(eChannelType < MAX_WINDOW);
    if(eChannelType >= MAX_CHANNEL)
    {
        return E_CUS3D_NO;
    }

    if(_stCus3DProcess.bDeMainPipSignal_Ver)
    {
        enCus3DMode = E_CUS3D_DEINTERLACESIGNAL_VER;
    }
    else if(_stCus3DProcess.bDeMainPipSignal)
    {
        enCus3DMode = E_CUS3D_DEINTERLACESIGNAL;
    }
    else if(_stCus3DProcess.eSyncType == MAIN_PIP_OUTPUT_ASYNC)
    {
        enCus3DMode = E_CUS3D_OUTPUTASYNC;
    }
    else if(_stCus3DProcess.bSetCusCropWindow[eChannelType])
    {
        enCus3DMode = E_CUS3D_NORMAL;
    }
    return enCus3DMode;
}


void MDrv_SC_3D_Modify_3DCropwindow(XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return;
    }

    if(eWindow == MAX_WINDOW)
    {
        return;
    }

#if (HW_DESIGN_3D_VER == 2)
	if((IS_INPUT_TOP_BOTTOM(eWindow)&&
		pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE-DOUBLEHD_720X2P_GARBAGE_VSIZE) ||// PDP 720 mvc T/B out case
	/*(MApi_XC_Get_mvc_TB_forDS()&& */(pSrcInfo->stCropWin.width == DOUBLEHD_720X2P_HSIZE))

	{
		/*if( gSrcInfo[eWindow].ScaledCropWin.height > ((pSrcInfo->u16V_SizeAfterPreScaling - gSrcInfo[eWindow].ScaledCropWin.y)/2) )
        {
            gSrcInfo[eWindow].ScaledCropWin.height = ((pSrcInfo->u16V_SizeAfterPreScaling - gSrcInfo[eWindow].ScaledCropWin.y)/2) & ~0x1;
        }*/
        gSrcInfo[eWindow].ScaledCropWin.y = 0;
		gSrcInfo[eWindow].ScaledCropWin.height= 720;
	}
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Scaled crop[4] x,y,w,h= %u,%u,%u,%u\n",gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y, gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height);

    return;
#else
    if((MApi_XC_GetCus3DMode(eWindow) == E_CUS3D_OUTPUTASYNC)
            || (MApi_XC_GetCus3DMode(eWindow) == E_CUS3D_DEINTERLACESIGNAL)
            || (MApi_XC_GetCus3DMode(eWindow) == E_CUS3D_DEINTERLACESIGNAL_VER))
    {
        _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].x     = (MS_U16)((MS_U32)_stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].x * pSrcInfo->u16H_SizeAfterPreScaling / pSrcInfo->stCapWin.width);
        _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].width = (MS_U16)((MS_U32)_stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].width * pSrcInfo->u16H_SizeAfterPreScaling / pSrcInfo->stCapWin.width);

        _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].y     = (MS_U16)((MS_U32)_stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].y * pSrcInfo->u16V_SizeAfterPreScaling / pSrcInfo->stCapWin.height);
        _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].height= (MS_U16)((MS_U32)_stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].height* pSrcInfo->u16V_SizeAfterPreScaling / pSrcInfo->stCapWin.height);

        _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].x = _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].x & ~0x1;
        _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].y = _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].y & ~0x1;
        _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].width = (_stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].width + 0x1) & ~0x1;
        _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].height = (_stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].height + 0x1) & ~0x1;

        if( _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].width > (pSrcInfo->u16H_SizeAfterPreScaling - _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].x) )
        {
            _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].width = (pSrcInfo->u16H_SizeAfterPreScaling - _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].x) & ~0x1;
        }

        if( _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].height > (pSrcInfo->u16V_SizeAfterPreScaling - _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].y) )
        {
            _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].height = (pSrcInfo->u16V_SizeAfterPreScaling - _stCus3DProcess.stCus3DCropWindow[SUB_WINDOW].y) & ~0x1;
        }
    }
#endif
}
#endif

void    MApi_XC_3D_Set_SpecialOutputTiming(E_XC_3D_EX_OUTPUT_SPECIFIC_TIMING SEC_SpecificTiming, MS_BOOL bEnable)
{
    if (SEC_SpecificTiming >=E_XC_3D_EX_OUTPUT_MAX)
    {
        return;
    }
    bEX_Specific_Output_Timing[SEC_SpecificTiming] = bEnable;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[%s]bSEC_Specific_Output_Timing[%u] = %s\n",
                 __FUNCTION__, SEC_SpecificTiming, bEnable?"Enable":"Disable");

}
MS_BOOL    MApi_XC_3D_Get_SpecialOutputTiming(E_XC_3D_EX_OUTPUT_SPECIFIC_TIMING SEC_SpecificTiming)
{
    if (SEC_SpecificTiming >= E_XC_3D_EX_OUTPUT_MAX)
    {
        return DISABLE;
    }

    MS_BOOL returnvalue = bEX_Specific_Output_Timing[SEC_SpecificTiming];
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[%s]bSEC_Specific_Output_Timing[%u] = %s\n",
                 __FUNCTION__, SEC_SpecificTiming, returnvalue?"Enable":"Disable");

    return returnvalue;
}

//by Aiken, we don't add this kind of special customer request in trunk code
#if 1
//--------------------------------------------------
//--------3D external Custom 3D interface-----------
//--------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Disable Custom 3D API
/// @param  void
/// @return MS_BOOL                           \b OUT: TRUE:Successful FALSE:FAIL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DisableCus3D(void)
{
    memset(&_stCus3DProcess, 0, sizeof(Cus_3DProcess));
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Set Custom 3D Crop Window, MAIN_SOURCE_CHANNEL is L, PIP_SOURCE_CHANNEL is R, and we will process
/// 3D based on L/R memory
/// @param  u16Upper                          \b IN: upper location in memory
/// @param  u16Bottom                         \b IN: bottom location in memory
/// @param  u16Left                           \b IN: left location in memory
/// @param  u16Right                          \b IN: right location in memory
/// @param  eChannelType                      \b IN: which channel
/// @return MS_BOOL                           \b OUT: TRUE:Successful FALSE:FAIL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Crop_MemoryWindow(MS_U16 u16Upper,
                                  MS_U16 u16Bottom,
                                  MS_U16 u16Left,
                                  MS_U16 u16Right,
                                  SOURCE_CHANNEL_TYPE eChannelType)
{
    MS_BOOL bRet = TRUE;
    MS_ASSERT(eChannelType < MAX_CHANNEL);

    if(eChannelType >= MAX_CHANNEL)
    {
        return FALSE;
    }

    if((u16Bottom <= u16Upper) || (u16Right <= u16Left))
    {
        MS_ASSERT(0);
        _stCus3DProcess.bSetCusCropWindow[eChannelType]         = FALSE;
        bRet = FALSE;
    }
    else
    {
        _stCus3DProcess.bSetCusCropWindow[eChannelType]         = TRUE;
        _stCus3DProcess.stCus3DCropWindow[eChannelType].x       = u16Left;
        _stCus3DProcess.stCus3DCropWindow[eChannelType].width   = u16Right - u16Left + 1;
        _stCus3DProcess.stCus3DCropWindow[eChannelType].y       = u16Upper;
        _stCus3DProcess.stCus3DCropWindow[eChannelType].height  = u16Bottom - u16Upper + 1;
    }
    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Set Custom 3D Display Window, which will be displayed for 3D signal
/// @param  u16XPos                            \b IN: x pos on panel
/// @param  u16YPos                            \b IN: y pos on panel
/// @param  u16Width                           \b IN: width on panel
/// @param  u16Height                          \b IN: height on panel
/// @param  eChannelType                       \b IN: which channel
/// @return MS_BOOL                            \b OUT: TRUE:Successful FALSE:FAIL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetSize_DisplayWindow(MS_U16 u16XPos,
                                      MS_U16 u16YPos,
                                      MS_U16 u16Width,
                                      MS_U16 u16Height,
                                      SOURCE_CHANNEL_TYPE eChannelType)
{
    MS_BOOL bRet = TRUE;
    MS_ASSERT(eChannelType < MAX_CHANNEL);

    if(eChannelType >= MAX_CHANNEL)
    {
        return FALSE;
    }

    if((u16Width == 0) || (u16Height == 0))
    {
        MS_ASSERT(0);
        _stCus3DProcess.bSetCusDispWindow[eChannelType]        = FALSE;
        bRet = FALSE;
    }
    else
    {
        _stCus3DProcess.bSetCusDispWindow[eChannelType]        = TRUE;
        _stCus3DProcess.stCus3DDispWindow[eChannelType].x      = u16XPos;
        _stCus3DProcess.stCus3DDispWindow[eChannelType].y      = u16YPos;
        _stCus3DProcess.stCus3DDispWindow[eChannelType].width  = u16Width;
        _stCus3DProcess.stCus3DDispWindow[eChannelType].height = u16Height;
    }
    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Deinterlace Custom 3D Main PIP Signal
/// 1st line is 1st line of MAIN_SOURCE_CHANNEL, 2nd line is 1st line of PIP_SOURCE_CHANNEL
/// 3rd line is 3rd line of MAIN_SOURCE_CHANNEL, 4th line is 3rd line of PIP_SOURCE_CHANNEL
/// and so on
/// @param  bMainFirst                         \b IN: Main Channel first or Sub Channel
/// @return MS_BOOL                            \b OUT: TRUE:Successful FALSE:FAIL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Deinterlace_MainPipSignal(MS_BOOL bMainFirst)
{
    _stCus3DProcess.bDeMainPipSignal = TRUE;
    _stCus3DProcess.bMainFirst       = bMainFirst;
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Deinterlace Custom 3D Main PIP Signal and need prescaling for Vertical Lines
/// if L or R > DispWin/2, need prescaling to DispWin/2 firstly
/// 1st line is 1st line of MAIN_SOURCE_CHANNEL, 2nd line is 1st line of PIP_SOURCE_CHANNEL
/// 3rd line is 2nd line of MAIN_SOURCE_CHANNEL, 4th line is 2nd line of PIP_SOURCE_CHANNEL
/// and so on
/// @param  bMainFirst                         \b IN: Main Channel first or Sub Channel
/// @return MS_BOOL                            \b OUT: TRUE:Successful FALSE:FAIL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_VerDeinterlace_MainPipSignal(MS_BOOL bMainFirst)
{
    _stCus3DProcess.bDeMainPipSignal_Ver = TRUE;
    _stCus3DProcess.bMainFirst_Ver       = bMainFirst;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Set Custom 3D Frequency and SyncType
/// @param  eFreq                              \b IN: output vfreq to panel
/// @param  eSync                              \b IN: async or sync
/// @return MS_BOOL                            \b OUT: TRUE:Successful FALSE:FAIL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetFrequency_DisplayWindow(FREQUENCY_TO_PANEL eFreq,
        MAIN_PIP_OUTPUT_TYPE eSync)
{
    _stCus3DProcess.bSetVFreq     = TRUE;
    _stCus3DProcess.eOutputVFreq = eFreq;
    _stCus3DProcess.eSyncType    = eSync;
    return TRUE;
}

#endif



#ifdef __cplusplus
}
#endif

#undef DRV_SC_3D_EX_C
#endif //DRV_SC_3D_EX_C
