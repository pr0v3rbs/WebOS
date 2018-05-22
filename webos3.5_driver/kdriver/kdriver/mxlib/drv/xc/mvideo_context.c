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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// file    Mvideo_context.c
/// @brief  TEMP Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define  MVIDEO_CONTEXT_C
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

// Internal Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#endif

#ifndef MSOS_TYPE_NOS
#define _GNU_SOURCE // need this _GNU_SOURCE because we need to check tid by "syscall"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/unistd.h>
#include <linux/types.h>
#else
#include <unistd.h>
//#include <sys/syscall.h>  // remove for ecos project
#include <sys/types.h>
#endif
#endif

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
#include "apiXC_DWIN.h"
#include "drv_sc_scaling.h"
#include "drv_sc_DIP_scaling.h"
#include "xc_hwreg_utility2.h"
#include "xc_Analog_Reg.h"
#include "mhal_hdmi.h"
#include "mhal_ip.h"
#include "mhal_dip.h"
#include "drvXC_HDMI_Internal.h"
#include "mhal_dynamicscaling.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define MSXC_WR_DBG(x)  //x
#define SC_3D_DBG(x)    //x
#define SeamlessZapping_DBG 0




//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_U8 SC_VBuf[32] =  {
    'M','S','V','C','0','0',                //0,0 fixed
    'S','3',                                //LIB ID
    '0','0',                                //Interface
    '0','0','0','0',                        //Build number
    '0','0','0','0','0','0','0','0',        // Change List
    'A','E','0','0','0','0','0','0','0',    // AE:Saturn4LE  0: MStar Trunk
    'T'                                     // check sum
};
#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
// For HW internal test pattern using
//static MS_BOOL bOP1_PatGen_Init = FALSE;
#endif
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_SC_Cal_FRC_Output_Vfreq(void *pInsatnce, MS_U16 u16InputVFreq,
                             MS_U16 u16DefaultVFreq,
                             MS_BOOL bFBL,
                             MS_U8 *u8FrcIn,
                             MS_U8 *u8FrcOut,
                             MS_BOOL *bSetFrameLock,
                             MS_U8 u8Lpll_bank,
                             SCALER_WIN eWin);


extern PQ_Function_Info    s_PQ_Function_Info[MAX_XC_DEVICE_NUM];
void MApi_PNL_CalExtLPLLSETbyDClk(MS_U32 ldHz);
void MApi_PNL_SetExt_LPLL_Type(MS_U16 u16Ext_lpll_type);
//==============================================================================
//==============================================================================

#if SC2_ENABLE
void MDrv_XC_SetDeviceOffset(void *pInstance)
{
    MS_U8 i;
    MS_U32 u32Offset;
    _XC_ENTRY(pInstance);

    for(i = 0; i < MAX_XC_DEVICE_NUM; i++)
    {
        u32Offset = Hal_SC_Get_Device_Offset(i);
        _XC_Device_Offset[i] = u32Offset;
    }

    _XC_RETURN(pInstance);
}

#else //SC2_ENABLE
void MDrv_XC_SetDeviceOffset(void)
{
    MS_U8 i;
    MS_U32 u32Offset;
    _XC_ENTRY();

    for(i = 0; i < MAX_XC_DEVICE_NUM; i++)
    {
        u32Offset = Hal_SC_Get_Device_Offset(i);
        _XC_Device_Offset[i] = u32Offset;
    }

#if SC2_ENABLE
            _XC_RETURN(pInstance);
#else //SC2_ENABLE
           _XC_RETURN();
#endif //#if SC2_ENABLE
}
#endif //#if SC2_ENABLE



void mvideo_sc_variable_init(void *pInstance, MS_BOOL bFirstInstance, XC_INITDATA *pXC_InitData)
{
    MS_U8 i, j;
    MS_U8 *pData;
    XC_PANEL_INFO_EX stPanelInfoEx;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // context
#if(XC_CONTEXT_ENABLE)
    pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug = FALSE;
    if (bFirstInstance)
    {
        gSrcInfo[MAIN_WINDOW].stDispWin.x       = 0;
        gSrcInfo[MAIN_WINDOW].stDispWin.y       = 0;
        gSrcInfo[MAIN_WINDOW].stDispWin.width   = 0;
        gSrcInfo[MAIN_WINDOW].stDispWin.height  = 0;

        gSrcInfo[MAIN_WINDOW].bEn3DNR = FALSE;
        gSrcInfo[MAIN_WINDOW].bUseYUVSpace = FALSE;
        gSrcInfo[MAIN_WINDOW].bMemYUVFmt = FALSE;
        gSrcInfo[MAIN_WINDOW].bForceNRoff = FALSE;
        gSrcInfo[MAIN_WINDOW].bFBL = FALSE;
        gSrcInfo[MAIN_WINDOW].bR_FBL = FALSE;
        gSrcInfo[MAIN_WINDOW].bForceRGBin = FALSE;
        gSrcInfo[MAIN_WINDOW].bFastFrameLock = FALSE;//mvideo_sc_set_fastframelock(FALSE);
        gSrcInfo[MAIN_WINDOW].bRWBankAuto= TRUE;

        gSrcInfo[MAIN_WINDOW].u16BytePerWord   = BYTE_PER_WORD;
        gSrcInfo[MAIN_WINDOW].u16OffsetPixelAlignment = OFFSET_PIXEL_ALIGNMENT;
        gSrcInfo[MAIN_WINDOW].Status2.bAutoBestPreScaling = FALSE;
        gSrcInfo[MAIN_WINDOW].bInterlace = FALSE;
        gSrcInfo[MAIN_WINDOW].Status2.u8IP_PixelRep = INVALID_IP_PIXELREPTITION;

        gSrcInfo[SUB_WINDOW].stDispWin.x       = 0;
        gSrcInfo[SUB_WINDOW].stDispWin.y       = 0;
        gSrcInfo[SUB_WINDOW].stDispWin.width   = 0;
        gSrcInfo[SUB_WINDOW].stDispWin.height  = 0;

        gSrcInfo[SUB_WINDOW].bEn3DNR = FALSE;
        gSrcInfo[SUB_WINDOW].bUseYUVSpace = FALSE;
        gSrcInfo[SUB_WINDOW].bForceNRoff = FALSE;
        gSrcInfo[SUB_WINDOW].bMemYUVFmt = FALSE;
        gSrcInfo[SUB_WINDOW].bFBL = FALSE;
        gSrcInfo[SUB_WINDOW].bForceRGBin = FALSE;
        gSrcInfo[SUB_WINDOW].bFastFrameLock = FALSE;
        gSrcInfo[SUB_WINDOW].bRWBankAuto= TRUE;

        gSrcInfo[SUB_WINDOW].u16BytePerWord   = BYTE_PER_WORD;
        gSrcInfo[SUB_WINDOW].u16OffsetPixelAlignment = OFFSET_PIXEL_ALIGNMENT;
        gSrcInfo[SUB_WINDOW].Status2.bAutoBestPreScaling = FALSE;
        gSrcInfo[SUB_WINDOW].bInterlace = FALSE;

        memset(&stPanelInfoEx, 0, sizeof(XC_PANEL_INFO_EX));
        stPanelInfoEx.u16VFreq = PANEL_INFO_EX_INVALID_ALL;
        stPanelInfoEx.u32PanelInfoEx_Version = PANEL_INFO_EX_VERSION;
        stPanelInfoEx.u16PanelInfoEX_Length = sizeof(XC_PANEL_INFO_EX);
        MDrv_XC_SetExPanelInfo(pInstance, FALSE, &stPanelInfoEx);
        gSrcInfo[SUB_WINDOW].Status2.u8IP_PixelRep = INVALID_IP_PIXELREPTITION;

        pXCResourcePrivate->stdrvXC_MVideo.bNeedInitShared = FALSE;
        pXCResourcePrivate->btvservice = FALSE;
        pXCResourcePrivate->bsetwindow_multiprocess = FALSE;
        pXCResourcePrivate->u16PreScalingAutoMute =0;
        gSrcInfo[MAIN_WINDOW].bpqmap     = 0;
        gSrcInfo[MAIN_WINDOW].btimingchg = 0;
        gSrcInfo[MAIN_WINDOW].bxcoutput  = 0;
        gSrcInfo[MAIN_WINDOW].bmemfmt    = 0;
        gSrcInfo[MAIN_WINDOW].bcropwindowchg   = 0;
        gSrcInfo[MAIN_WINDOW].u32PreCMAMemSCMSize[CMA_XC_SELF_MEM]    = 0;
        gSrcInfo[MAIN_WINDOW].u32PreCMAMemSCMSize[CMA_XC_COBUFF_MEM]    = 0;
        gSrcInfo[MAIN_WINDOW].u32PreCMAMemFRCMSize[CMA_XC_SELF_MEM]    = 0;
        gSrcInfo[MAIN_WINDOW].u32PreCMAMemFRCMSize[CMA_XC_COBUFF_MEM]    = 0;
        gSrcInfo[MAIN_WINDOW].Status2.u32PreCMAMemDualSCMSize  = 0;
        gSrcInfo[SUB_WINDOW].u32PreCMAMemSCMSize[CMA_XC_SELF_MEM]    = 0;
        gSrcInfo[SUB_WINDOW].u32PreCMAMemSCMSize[CMA_XC_COBUFF_MEM]    = 0;
        gSrcInfo[SUB_WINDOW].u32PreCMAMemFRCMSize[CMA_XC_SELF_MEM]    = 0;
        gSrcInfo[SUB_WINDOW].u32PreCMAMemFRCMSize[CMA_XC_COBUFF_MEM]    = 0;
        gSrcInfo[SUB_WINDOW].Status2.u32PreCMAMemDualSCMSize   = 0;
        gSrcInfo[SUB_WINDOW].bpqmap      = 0;
        gSrcInfo[SUB_WINDOW].btimingchg  = 0;
        gSrcInfo[SUB_WINDOW].bxcoutput   = 0;
        gSrcInfo[SUB_WINDOW].bmemfmt     = 0;
        gSrcInfo[SUB_WINDOW].bcropwindowchg    = 0;
        memset(&s_PQ_Function_Info[psXCInstPri->u32DeviceID],0,sizeof(s_PQ_Function_Info[psXCInstPri->u32DeviceID]));
    }
#endif

    if(pXC_InitData != NULL)//only valid pointer can init the following variable
    {
        if(gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC == 0)//only assign significant value for one time
        {
            gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC = pXC_InitData->stPanelInfo.u16DefaultVFreq;
        }

        if(gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC == 0)//only assign significant value for one time
        {
            gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC;
        }

        //save panel default timing
        memcpy(&(gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), &(pXC_InitData->stPanelInfo), sizeof(XC_PANEL_TIMING));
        // Here is the third place user can set mirror mode flag !!!
        if (pXC_InitData->bMirror)
        {
            // So we need update variable here.
            gSrcInfo[MAIN_WINDOW].Status2.eMirrorMode = MIRROR_HV;
            gSrcInfo[SUB_WINDOW].Status2.eMirrorMode = MIRROR_HV;
        }
        pXCResourcePrivate->bSetPanelTiming = 0;
        pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[MAIN_WINDOW] = INPUT_SOURCE_NONE;
        pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[SUB_WINDOW] = INPUT_SOURCE_NONE;

        memset(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc, 0, sizeof(XC_INITMISC));

        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableRepWindowForFrameColor = FALSE;
        pXCResourcePrivate->stdrvXC_MVideo_Context.bMainIsOnTop = FALSE;

        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32InitDataLen_Suspend = 0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32InitMiscDataLen_Suspend = 0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MLoadPhyAddr_Suspend = 0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MLoadBufByteLen_Suspend = 0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8DispWindowColor_Suspend[MAIN_WINDOW] = 0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8DispWindowColor_Suspend[SUB_WINDOW] = 0;
        for(i = 0; i < E_XC_MCDI_MAX; i++)
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MCDIFBAddress_Suspend[i] = 0;
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MCDIFBSize_Suspend[i] = 0;
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableMCDI_Suspend[i] = FALSE;
        }
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32OSDCCtrltype_Suspend = 0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32FrameColor_Suspend = 0;
        pXCResourcePrivate->u16XCDbgSwitch_InternalUseOnly = 0;

        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_pu8HdcpKeyDataBackUp == NULL)
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_pu8HdcpKeyDataBackUp = (MS_U8*) malloc(HDCP_KEY_LENGTH * sizeof(MS_U8));

            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_pu8HdcpKeyDataBackUp == NULL)
            {
                printf("malloc for g_pu8HdcpKeyDataBackUp failed\n");
                MS_ASSERT(0);
            }
        }

        if ( pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp == NULL)
        {
            // 4 is for 4 HDMI ports
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp= (MS_U8 **)malloc(4*sizeof(MS_U8 *)+4*EDID_KEY_LENGTH*sizeof(MS_U8));

            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp == NULL)
            {
                printf("malloc for g_ppEDID_DVIBackUp failed\n");
                MS_ASSERT(0);
            }
            else
            {
            	for (i=0,pData= (MS_U8 *)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp+4); i<4; i++, pData+=EDID_KEY_LENGTH)
            	{
                	if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp != NULL)
                	{
                    		pXCResourcePrivate->stdrvXC_MVideo_Context.g_ppEDID_DVIBackUp[i] = pData;
                	}
            	}
	    }
        }
#ifndef DISABLE_3D_FUNCTION
        if(psXCInstPri->u32DeviceID == 0)
        {
            MDrv_SC_3D_Variable_Init(pInstance);
        }
#endif
        // interrupts
	MDrv_SC_ISR_fnptr_init();
#if 0
if(MDrv_XC_GetMainProcessISR(pInstance)==TRUE)	//TV service need reset
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK03_42_L, 0xFF,0xFF);
        for(i=0; i<MAX_SC_INT; i++)
        {
            for(j=0; j<MAX_ISR_NUM_OF_EACH_INT; j++)
            {
                pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[i][j].aryXC_ISR_Param = NULL;
                pXCResourcePrivate->stdrvXC_ISR.stXCIsrInfo[i][j].aryXC_ISR = NULL;
            }
        }
	}	
#endif

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
#endif //#if SC2_ENABLE



        MDrv_XC_GetDefaultHVSyncInfo(pInstance);

        // SW patch for Eisntein
        #if (FRC_INSIDE)
            MHal_FRC_interrupt_mask(pInstance, 0xFF);
        #endif

        // With HW Auto no signal case,  sub video source change would causes OP1 broken, looks like main video broken
        Hal_SC_ip_enable_turnoff_OP1_for_AutoNoSignal(pInstance, DISABLE);

        Hal_SC_SetOPWriteOff(pInstance, TRUE); //OPW Disable
        MDrv_Write2ByteMask(REG_MIU0_BASE + 0x66, 0x00, BIT(1));//MIU OPW Unmask
#if SC2_ENABLE
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_RETURN();
#endif //#if SC2_ENABLE



    }
}

void MDrv_XC_SetHdmiSyncMode(void *pInstance, E_HDMI_SYNC_TYPE eSynctype,SCALER_WIN eWindow)
{
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"set HDMI sync mode mode: %u (0 is DE mode, 1 is HV mode)\n",eSynctype);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.s_HDMISYNC_type[eWindow] = eSynctype;
}

static void _MDrv_XC_SetHdmiSyncMode(void *pInstance, E_HDMI_SYNC_TYPE eSynctype, SCALER_WIN eWindow)
{
    //MS_U16 u16HDE = 0;
    //MS_U16 u16HTT = 0;
    MS_U8 u8InputSrcSel, u8SyncSel, u8VideoSel=0, u8isYPbPr;
    //Whether we should support special timing with HV sync mode. Some timing, like 720@30p must use HV mode
    //  to detect it. so this API is actually to enable or disable the capablity for these special timing.
    //  DE mode: not support special timing, HV mode: support special timing
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if SUPPORT_IP_HDMI_FOR_HV_MODE
    if(HDMI_SYNC_HV == eSynctype)
    {
        MDrv_SC_ip_set_input_vsync_delay(pInstance, NO_DELAY, eWindow );
        MDrv_SC_ip_set_de_only_mode(pInstance, DISABLE, eWindow );
        // Use DE as capture window
        MDrv_SC_ip_set_de_bypass_mode(pInstance, ENABLE, eWindow);
        MDrv_SC_ip_set_input_sync_sample_mode(pInstance, GLITCH_REMOVAL, eWindow );
        MDrv_SC_ip_set_post_glitch_removal(pInstance, ENABLE, 0x01, eWindow );
        //720p with IP_HDMI, color space problem. must set it to IP_VIDEO
        u8InputSrcSel = IP_HDMI;
        u8VideoSel = IP_MST_VD_A;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = TRUE;
        MDrv_SC_ip_set_input_source(pInstance, u8InputSrcSel, u8SyncSel, u8VideoSel, u8isYPbPr, eWindow);
        MDrv_XC_reset_ip(pInstance, eWindow );
    }
    else
#endif
    {
        MDrv_SC_ip_set_input_vsync_delay(pInstance, DELAY_QUARTER_HSYNC, eWindow );
        MDrv_SC_ip_set_de_only_mode(pInstance, ENABLE, eWindow );
        MDrv_SC_ip_set_de_bypass_mode(pInstance, DISABLE, eWindow);
        MDrv_SC_ip_set_input_sync_sample_mode(pInstance, NORMAL_MODE, eWindow );
        MDrv_SC_ip_set_post_glitch_removal(pInstance, ENABLE, 0x07, eWindow );
        //720p with IP_HDMI, color space problem. must set it to IP_VIDEO
        //here should recover the setting of 720p
        u8InputSrcSel = IP_HDMI;
        u8VideoSel = IP_HDMI;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = TRUE;
        MDrv_SC_ip_set_input_source(pInstance, u8InputSrcSel, u8SyncSel, u8VideoSel, u8isYPbPr, eWindow);
        MDrv_XC_reset_ip(pInstance, eWindow );
    }
}

void MApi_XC_SetHdmiSyncMode_U2(void* pInstance, E_HDMI_SYNC_TYPE eSynctype, SCALER_WIN eWindow)
{
    //MS_U16 u16HDE = 0;
    //MS_U16 u16HTT = 0;
    //MS_U8 u8InputSrcSel, u8SyncSel, u8VideoSel=0, u8isYPbPr;
    //Whether we should support special timing with HV sync mode. Some timing, like 720@30p must use HV mode
    //  to detect it. so this API is actually to enable or disable the capablity for these special timing.
    //  DE mode: not support special timing, HV mode: support special timing
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if SUPPORT_IP_HDMI_FOR_HV_MODE
    if(HDMI_SYNC_HV == eSynctype)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context._bSupportHVMode[eWindow] = TRUE;
        pXCResourcePrivate->stdrvXC_MVideo_Context.s_HDMISYNC_type[eWindow] = eSynctype;

    }
    else
#endif
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context._bSupportHVMode[eWindow] = FALSE;
        pXCResourcePrivate->stdrvXC_MVideo_Context.s_HDMISYNC_type[eWindow] = HDMI_SYNC_DE;
    }
    _MDrv_XC_SetHdmiSyncMode(pInstance, eSynctype, eWindow);


#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_SetHdmiSyncMode(E_HDMI_SYNC_TYPE eSynctype, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_HDMI_SYNCMODE XCArgs;
    XCArgs.eSynctype = eSynctype;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_HDMI_SYNCMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

E_HDMI_SYNC_TYPE MDrv_XC_GetHdmiSyncMode(void *pInstance,SCALER_WIN eWindow)
{
    //Check whether which mode are we using: DE or HV.
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_MVideo_Context.s_HDMISYNC_type[eWindow];
}

E_HDMI_SYNC_TYPE MApi_XC_GetHdmiSyncMode_U2(void* pInstance, SCALER_WIN eWindow)
{
#if 0
    E_HDMI_SYNC_TYPE eReturn = HDMI_SYNC_DE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    eReturn = MDrv_XC_GetHdmiSyncMode(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    eReturn = MDrv_XC_GetHdmiSyncMode(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    return eReturn;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return (E_HDMI_SYNC_TYPE)pXCResourcePrivate->stdrvXC_MVideo_Context._bSupportHVMode[eWindow];
}

E_HDMI_SYNC_TYPE MApi_XC_GetHdmiSyncMode(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return HDMI_SYNC_DE;
    }

    stXC_GET_HDMI_SYNCMODE XCArgs;
    XCArgs.eReturnValue = HDMI_SYNC_DE;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_HDMI_SYNCMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return HDMI_SYNC_DE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MDrv_XC_Switch_DE_HV_Mode_By_Timing(void *pInstance, SCALER_WIN eWindow)
{
#if SUPPORT_IP_HDMI_FOR_HV_MODE
#if ONLY_ALLOW_HV_MODE_FOR_720P
    MS_U16 u16HDE = 0;
    MS_U16 u16HTT = 0;
    MS_U8 u8InputSrcSel, u8SyncSel, u8VideoSel=0, u8isYPbPr/*, u8HankShakingMD*/;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_MVideo_Context._bSupportHVMode[eWindow])
    {

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
#endif //#if SC2_ENABLE


        u16HDE = _MDrv_HDMI_GetHDE();
        u16HTT = _MDrv_HDMI_GetHTT();
        //printf("---------------------MDrv_HDMI_GetHDE=%u\n",u16HDE);
        if(HVMODE_HDE_VALIDATION(u16HDE) || (u16HTT%2))
        {
            if(HDMI_SYNC_HV != MDrv_XC_GetHdmiSyncMode(pInstance,eWindow))
            {
                MDrv_XC_SetHdmiSyncMode(pInstance, HDMI_SYNC_HV,eWindow);
                //printf("@@@---%s:%d u8IP_PixelRep=%u\n", __FUNCTION__, __LINE__, gSrcInfo[eWindow].Status2.u8IP_PixelRep);
                if(gSrcInfo[eWindow].Status2.u8IP_PixelRep != INVALID_IP_PIXELREPTITION)
                {
                    gSrcInfo[eWindow].Status2.u8IP_PixelRep = INVALID_IP_PIXELREPTITION;
                    Hal_SC_ip_set_fir_down_sample_divider(pInstance, DISABLE,0x00,eWindow); //Reset Pixel repetition, when switch hv mode
                }
                MDrv_SC_ip_set_input_vsync_delay(pInstance, NO_DELAY, eWindow );
                MDrv_SC_ip_set_de_only_mode(pInstance, DISABLE, eWindow );
                // Use DE as capture window
                MDrv_SC_ip_set_de_bypass_mode(pInstance, ENABLE, eWindow);
                // Disable scaling for get correct DE.
                //MDrv_SC_Enable_PreScaling(FALSE, FALSE, eWindow);
                // Set full range capture size for de-bypass mode
                //MDrv_SC_SetFullRangeCapture(eWindow);
                MDrv_SC_ip_set_input_sync_sample_mode(pInstance, GLITCH_REMOVAL, eWindow );
                MDrv_SC_ip_set_post_glitch_removal(pInstance, ENABLE, 0x01, eWindow );
                Hal_HDMI_Set_YUV422to444_Bypass(TRUE);

                //720p with IP_HDMI, color space problem. must set it to IP_VIDEO
                u8InputSrcSel = IP_HDMI;
                u8VideoSel = IP_MST_VD_A;
                u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
                u8isYPbPr = TRUE;
                //u8HankShakingMD = FALSE;
                MDrv_SC_ip_set_input_source(pInstance, u8InputSrcSel, u8SyncSel, u8VideoSel, u8isYPbPr, eWindow);
                MDrv_XC_reset_ip(pInstance, eWindow );
            }
        }
        else
        {
            if(HDMI_SYNC_DE != MDrv_XC_GetHdmiSyncMode(pInstance, eWindow))
            {
                MDrv_XC_SetHdmiSyncMode(pInstance, HDMI_SYNC_DE, eWindow);
                //printf("@@@---%s:%d u8IP_PixelRep=%u\n", __FUNCTION__, __LINE__, gSrcInfo[eWindow].Status2.u8IP_PixelRep);
                if(gSrcInfo[eWindow].Status2.u8IP_PixelRep != INVALID_IP_PIXELREPTITION)
                {
                    gSrcInfo[eWindow].Status2.u8IP_PixelRep = INVALID_IP_PIXELREPTITION;
                    Hal_SC_ip_set_fir_down_sample_divider(pInstance, DISABLE,0x00,eWindow); //Reset Pixel repetition, when switch hv mode
                }
                MDrv_SC_ip_set_input_vsync_delay(pInstance, DELAY_QUARTER_HSYNC, eWindow );
                MDrv_SC_ip_set_de_only_mode(pInstance, ENABLE, eWindow );
                MDrv_SC_ip_set_de_bypass_mode(pInstance, DISABLE, eWindow);
                MDrv_SC_ip_set_input_sync_sample_mode(pInstance, NORMAL_MODE, eWindow );
                MDrv_SC_ip_set_post_glitch_removal(pInstance, ENABLE, 0x07, eWindow );
                Hal_HDMI_Set_YUV422to444_Bypass(TRUE);

                //720p with IP_HDMI, color space problem. must set it to IP_VIDEO
                //here should recover the setting of 720p
                u8InputSrcSel = IP_HDMI;
                u8VideoSel = IP_HDMI;
                u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
                u8isYPbPr = TRUE;
                //u8HankShakingMD = FALSE;
                MDrv_SC_ip_set_input_source(pInstance, u8InputSrcSel, u8SyncSel, u8VideoSel, u8isYPbPr, eWindow);
                MDrv_XC_reset_ip(pInstance, eWindow );
            }
        }

#if SC2_ENABLE
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_RETURN();
#endif //#if SC2_ENABLE


    }
#endif
#endif
}

void MDrv_XC_Set_DE_Lock_Mode(void *pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16HDE = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    _XC_ENTRY(pInstance);
	u16HDE = Hal_HDMI_GetHDE();
    if(HVMODE_HDE_VALIDATION(u16HDE))
    {
        MDrv_SC_ip_set_de_only_mode(pInstance, DISABLE, eWindow );
        MDrv_SC_ip_set_de_lock_mode(pInstance, ENABLE, eWindow);
    }
    else
    {
        MDrv_SC_ip_set_de_only_mode(pInstance, ENABLE, eWindow );
        MDrv_SC_ip_set_de_lock_mode(pInstance, DISABLE, eWindow);
    }
    _XC_RETURN(pInstance);
}

void mvideo_sc_getcropwin( void *pInstance, MS_WINDOW_TYPE *pCropWin, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pCropWin->x = gSrcInfo[eWindow].stCropWin.x;
    pCropWin->y = gSrcInfo[eWindow].stCropWin.y;
    pCropWin->width = gSrcInfo[eWindow].stCropWin.width;
    pCropWin->height = gSrcInfo[eWindow].stCropWin.height;
}

MS_U8  mvideo_pnl_is_dualport(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode; // 0: single mode, 1: dual mode
}

void mvideo_pnl_set_dclkmode(void *pInstance, E_XC_PNL_OUT_TIMING_MODE enPnl_Out_Timing_Mode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode = enPnl_Out_Timing_Mode;
}

MS_BOOL mvideo_sc_is_enable_3dnr(void *pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return gSrcInfo[eWindow].bEn3DNR;
}

void mvideo_sc_set_3dnr(void *pInstance, MS_BOOL bEnable,  SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].bEn3DNR = bEnable;
}

//-------------------------------------------------------------------------------------------------
/// check whether the specific window is YUV space
/// @param  eWindow                \b IN: which window we are going to set
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_IsYUVSpace_U2( void* pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bReturn = gSrcInfo[eWindow].bUseYUVSpace;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bReturn = gSrcInfo[eWindow].bUseYUVSpace;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    return bReturn;
}

MS_BOOL MApi_XC_IsYUVSpace( SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_YUVSPACE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_YUVSPACE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#if 1
MS_BOOL MApi_XC2_IsYUVSpace( SCALER_WIN eWindow )
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_YUVSPACE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_CHECK_YUVSPACE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
#endif

//-------------------------------------------------------------------------------------------------
/// Enable/Disable force RGB in
/// @param  bEnable                 \b IN: enable or disable
/// @param  eWindow                 \b IN: which window we are going to enable or disable
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableForceRGBin_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].bForceRGBin = bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].bForceRGBin = bEnable;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_EnableForceRGBin(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FORCE_RGBIN XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FORCE_RGBIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
//-------------------------------------------------------------------------------------------------
/// This function will enable/diable framebufferless mode
/// @param  bEnable      \b IN: TRUE: enable, FALSE: otherwise
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableFrameBufferLess_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[MAIN_WINDOW].bFBL = bEnable;
    _XC_ENTRY(pInstance);
    MDrv_SC_set_fbl(pInstance, bEnable);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[MAIN_WINDOW].bFBL = bEnable;
    _XC_ENTRY();
    MDrv_SC_set_fbl(pInstance, bEnable);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


}

void MApi_XC_EnableFrameBufferLess(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRAMEBUFFERLESS XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRAMEBUFFERLESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Query whether current XC is framebufferless or not
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_IsCurrentFrameBufferLessMode(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //gSrcInfo[MAIN_WINDOW].bFBL = MDrv_SC_get_fbl();
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return gSrcInfo[MAIN_WINDOW].bFBL;
}

MS_BOOL MApi_XC_IsCurrentFrameBufferLessMode_U2(void* pInstance)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_IsCurrentFrameBufferLessMode(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bReturn = MDrv_XC_IsCurrentFrameBufferLessMode(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_IsCurrentFrameBufferLessMode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_FRAMEBUFFERLESS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_FRAMEBUFFERLESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This function will enable/diable request framebufferless mode
/// @param  bEnable      \b IN: TRUE: enable, FALSE: otherwise
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableRequest_FrameBufferLess_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(Hal_SC_Detect_RequestFBL_Mode(pInstance))
        gSrcInfo[MAIN_WINDOW].bR_FBL = bEnable;
    else
        printf("Not support\n");

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_EnableRequest_FrameBufferLess(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_REQUEST_FRAMEBUFFERLESS XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_REQUEST_FRAMEBUFFERLESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Query whether current XC is request framebufferless or not
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_IsCurrentRequest_FrameBufferLessMode(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return gSrcInfo[MAIN_WINDOW].bR_FBL;
}

MS_BOOL MApi_XC_IsCurrentRequest_FrameBufferLessMode_U2(void* pInstance)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_IsCurrentRequest_FrameBufferLessMode(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bReturn = MDrv_XC_IsCurrentRequest_FrameBufferLessMode(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_IsCurrentRequest_FrameBufferLessMode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_REQUEST_FRAMEBUFFERLESS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_REQUEST_FRAMEBUFFERLESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//Obsolte function, do not use it any more
MS_U16 mvideo_sc_get_output_vfreq_U2(void* pInstance)
{
    MS_U16 u16OutputVFreq = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u16OutputVFreq = MDrv_SC_GetOutputVFreqX100(pInstance)/10;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    u16OutputVFreq = MDrv_SC_GetOutputVFreqX100(pInstance)/10;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return u16OutputVFreq;
}

MS_U16 mvideo_sc_get_output_vfreq(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_OUTPUT_VFREQ XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OUTPUT_VFREQ, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// check whether specific window is memory format 422
/// @param  eWindow                \b IN: which window we are going to query
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_IsMemoryFormat422_U2( void* pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bReturn = gSrcInfo[eWindow].bMemFmt422;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bReturn = gSrcInfo[eWindow].bMemFmt422;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    return bReturn;
}

MS_BOOL MApi_XC_IsMemoryFormat422( SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_MEMORYFORMAT422 XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_MEMORYFORMAT422, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set XC mirror hv mode for main window
/// @param  bEnable                \b IN: enable or disable XC mirror mode
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableMirrorMode_U2( void* pInstance, MS_BOOL bEnable )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    if (bEnable)
    {
        MDrv_XC_EnableMirrorModeEx(pInstance, MIRROR_HV, MAIN_WINDOW);
    }
    else
    {
        MDrv_XC_EnableMirrorModeEx(pInstance, MIRROR_NORMAL, MAIN_WINDOW);
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_EnableMirrorMode( MS_BOOL bEnable )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MIRROR_MODE_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MIRROR_MODE_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get XC mirror mode for main window
/// @return @ref MirrorMode_t
//-------------------------------------------------------------------------------------------------
MirrorMode_t MApi_XC_GetMirrorModeType( void )
{
    //This API not use anymore
    //instance use XC0
    // not use this for kernel driver
    return MIRROR_NORMAL;
    //return MDrv_XC_GetMirrorModeTypeEx(pu32XCInst, MAIN_WINDOW);
}

//-------------------------------------------------------------------------------------------------
/// Set XC mirror mode for main and sub window
/// @param  bEnable                \b IN: enable or disable XC mirror mode
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableMirrorMode2_U2( void* pInstance, MirrorMode_t eMirrorMode )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_EnableMirrorModeEx(pInstance, eMirrorMode, MAIN_WINDOW);
    MDrv_XC_EnableMirrorModeEx(pInstance, eMirrorMode, SUB_WINDOW);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
    MDrv_XC_EnableMirrorModeEx(pInstance, eMirrorMode, MAIN_WINDOW);
    MDrv_XC_EnableMirrorModeEx(pInstance, eMirrorMode, SUB_WINDOW);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_EnableMirrorMode2( MirrorMode_t eMirrorMode )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MIRROR_MODE2_ENABLE XCArgs;
    XCArgs.eMirrorMode = eMirrorMode;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MIRROR_MODE2_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set XC mirror mode for main or sub window
/// @param  eMirrorMode                 \b IN: enable or disable XC mirror mode
/// @param  eWindow                     \b IN: main or sub window
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_EnableMirrorModeEx_U2( void* pInstance, MirrorMode_t eMirrorMode,  SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    bReturn = MDrv_XC_EnableMirrorModeEx(pInstance, eMirrorMode, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
    bReturn = MDrv_XC_EnableMirrorModeEx(pInstance, eMirrorMode, eWindow);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;

}

MS_BOOL MApi_XC_EnableMirrorModeEx( MirrorMode_t eMirrorMode,  SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_MIRRORMODE_EX XCArgs;
    XCArgs.eMirrorMode = eMirrorMode;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MIRRORMODE_EX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
//-------------------------------------------------------------------------------------------------
/// Get XC mirror mode for main or sub window
/// @param  eWindow                \b IN: main or sub window
/// @return @ref MirrorMode_t
//-------------------------------------------------------------------------------------------------
MirrorMode_t MDrv_XC_GetMirrorModeTypeEx(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MirrorMode_t eReturn = gSrcInfo[eWindow].Status2.eMirrorMode;
    return eReturn;
}

MirrorMode_t MApi_XC_GetMirrorModeTypeEx_U2(void* pInstance, SCALER_WIN eWindow)
{
    MirrorMode_t eMirrorMode = MIRROR_MAX;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    eMirrorMode = MDrv_XC_GetMirrorModeTypeEx(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    eMirrorMode = MDrv_XC_GetMirrorModeTypeEx(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return eMirrorMode;
}

MirrorMode_t MApi_XC_GetMirrorModeTypeEx(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return MIRROR_MAX;
    }

    stXC_GET_MIRRORMODE_TYPEEX XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = MIRROR_MAX;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MIRRORMODE_TYPEEX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return MIRROR_MAX;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

#if 1
MirrorMode_t MApi_XC2_GetMirrorModeTypeEx(SCALER_WIN eWindow)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return MIRROR_MAX;
    }

    stXC_GET_MIRRORMODE_TYPEEX XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = MIRROR_MAX;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_GET_MIRRORMODE_TYPEEX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return MIRROR_MAX;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
#endif

#if SeamlessZapping_DBG // for SeamLess Zapping Debug
void _MDrv_XC_GetXCReg_ByISR(SC_INT_SRC eIntNum, void * pParam)
{
/// Add the reg info for debug
    eIntNum = eIntNum;
    pParam = pParam;
    printf("@@#@@[%s][%d]BK1F_79=%x\n",__FUNCTION__, __LINE__, (MS_U16)(SC_R2BYTEMSK(REG_SC_BK1F_79_L, 0xFFFF)));
    printf("@@#@@[%s][%d]BK12_3A=%x\n",__FUNCTION__, __LINE__, (SC_R2BYTEMSK(REG_SC_BK12_3A_L, 0xFF)));
    printf("@@#@@[%s][%d]BK12_0E=%x, BK12_0F=%x\n",
        __FUNCTION__, __LINE__, (MS_U16)(SC_R2BYTEMSK(REG_SC_BK12_0E_L, 0x7FF)),(MS_U16)(SC_R2BYTEMSK(REG_SC_BK12_0F_L, 0x7FF)));
    printf("@@#@@[%s][%d]BK12_16=%x, BK12_17=%x\n",
        __FUNCTION__, __LINE__, (MS_U16)(SC_R2BYTEMSK(REG_SC_BK12_16_L, 0x7FF)),(MS_U16)(SC_R2BYTEMSK(REG_SC_BK12_17_L, 0x7FF)));
    printf("@@#@@[%s][%d]SC3_2A=%x\n",
        __FUNCTION__, __LINE__, (MS_U16)(R2BYTEMSK(L_BK_LPLL(0x2A), 0x3FF)));

}
#endif
#define LPLL_TIMEOUT  300  // ms
//-------------------------------------------------------------------------------------------------
/// let display video freeze by disable scaler buffer write
/// @param  bEnable        \b IN: TRUE: enable, FALSE: otherwise
/// @param  eWindow        \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MDrv_XC_FreezeImg(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{

    PQ_WIN ePQWin;
#ifndef K3_U2
#if SUPPORT_SEAMLESS_ZAPPING
    MS_U32 u32Delayms = 0;
#endif
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(gSrcInfo[eWindow].bFBL)
    {
        return; //not support freeze for FBL mode
    }
    ePQWin = (eWindow == MAIN_WINDOW) ? PQ_MAIN_WINDOW : PQ_SUB_WINDOW;

    if(bEnable)
    {

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
#endif //#if SC2_ENABLE



        if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,eWindow)))
        {
            Hal_SC_set_freezeimg_burst(pInstance, bEnable, eWindow);
        }
        else
        {
            Hal_SC_set_freezeimg(pInstance, bEnable, eWindow);
        }
#if SC2_ENABLE
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_RETURN();
#endif //#if SC2_ENABLE



        //warning: This function is already protected with mutex
#ifdef ENABLE_TV_SC2_PQ
        if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
        {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode)
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode(ePQWin, bEnable);
        }
#ifdef ENABLE_TV_SC2_PQ
        else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
        {
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode_ex)
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode_ex(psXCInstPri->u32DeviceID, ePQWin, bEnable);
        }
#endif
        //

#if 0
#if SC2_ENABLE
                   _XC_ENTRY(pInstance);
                MDrv_XC_wait_input_vsync(pInstance,2, 50 , eWindow);
                   _XC_RETURN(pInstance);

#else //SC2_ENABLE
                   _XC_ENTRY();
                MDrv_XC_wait_input_vsync(pInstance,2, 50 , eWindow);
                   _XC_RETURN();

#endif //#if SC2_ENABLE
#endif


#if 0
#if SC2_ENABLE
                    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
                   _XC_ENTRY();
#endif //#if SC2_ENABLE
        MDrv_XC_wait_input_vsync(pInstance,2, 50 , eWindow);


        #if SC2_ENABLE
                    _XC_RETURN(pInstance);
#else //SC2_ENABLE
                   _XC_RETURN();
#endif //#if SC2_ENABLE
#endif

#if SeamlessZapping_DBG
        printf("@@#@@ Freeze\n");
        if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_XC_GetXCReg_ByISR, NULL))
        {
            printf("###ISR ###\n");
            _MDrv_XC_GetXCReg_ByISR(0,NULL);
            MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_XC_GetXCReg_ByISR, NULL);
        }
#endif
    }
    else
    {
        //warning: This function is already protected with mutex
#ifdef ENABLE_TV_SC2_PQ
        if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#endif
        {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode)
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode(ePQWin, bEnable);
        }
#ifdef ENABLE_TV_SC2_PQ
        else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
        {
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode_ex)
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_disable_filmmode_ex(psXCInstPri->u32DeviceID, ePQWin, bEnable);
        }
#endif
        //
#ifndef K3_U2
#if SUPPORT_SEAMLESS_ZAPPING
        //// for SLZ debug, wait lpll lock, for the stable OP Vsync
        if(Hal_SC_GetSeamlessZappingStatus(pInstance, eWindow))
        {
            //printf("@@#@@ u32Delayms = %u\n", LPLL_TIMEOUT);
            while(((R2BYTEMSK(L_BK_LPLL(0x2A), 0x3FF))!=0x1F4)&&(u32Delayms < LPLL_TIMEOUT))
            {
                MsOS_DelayTask(1);
                u32Delayms++;
            }
        }
        /// end
#endif
#endif

#if SC2_ENABLE
        _XC_ENTRY(pInstance);
#else //SC2_ENABLE
        _XC_ENTRY();
#endif //#if SC2_ENABLE



        if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,eWindow)))
        {
            Hal_SC_set_freezeimg_burst(pInstance, bEnable, eWindow);
        }
        else
        {
            Hal_SC_set_freezeimg(pInstance, bEnable, eWindow);
        }
#if SC2_ENABLE
        _XC_RETURN(pInstance);
#else //SC2_ENABLE
        _XC_RETURN();
#endif //#if SC2_ENABLE


    }

    XC_LOG_TRACE(XC_DBGLEVEL_SEAMLESSZAP, "[%s][%06d]SLZ[%s] Freeze = %x\n",
        __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", bEnable);


}

void MApi_XC_FreezeImg_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_FreezeImg(pInstance, bEnable,eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_XC_FreezeImg(pInstance, bEnable,eWindow);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_FreezeImg(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FREEZEIMG XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FREEZEIMG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Query whether the specific window is freeze image status or not
/// @param  eWindow                \b IN: which window we are going to query
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_IsFreezeImg_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bXCFreezeImg;

#if SC2_ENABLE
    _XC_ENTRY(pInstance);

    bXCFreezeImg = Hal_SC_get_freezeimg(pInstance, eWindow);
    //return Hal_SC_get_freezeimg(eWindow);
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();

    bXCFreezeImg = Hal_SC_get_freezeimg(pInstance, eWindow);
    //return Hal_SC_get_freezeimg(eWindow);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    return bXCFreezeImg;
}

MS_BOOL MApi_XC_IsFreezeImg(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_FREEZEIMG XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_FREEZEIMG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_XC_GetInfo(void *pInstance, XC_ApiInfo *pXC_Info)
{
    UNUSED(pInstance);

    pXC_Info->u8MaxWindowNum = MAX_WINDOW_NUM;
    pXC_Info->u8NumOfDigitalDDCRam = NUM_OF_DIGITAL_DDCRAM;
    pXC_Info->u8MaxFrameNumInMem = MAX_FRAME_NUM_IN_MEM;
    pXC_Info->u8MaxFieldNumInMem = MAX_FIELD_NUM_IN_MEM;
}

#ifdef MS_DEBUG
void MDrv_XC_Printk(char *fmt, ...)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
#else
    #include "string.h"
    #include <unistd.h>

    FILE *fd = NULL;
    va_list args;
    if ((fd = fopen("/dev/kmsg", "w")) == NULL)
        return;

    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fflush(fd);
    fsync((int)fd);
    fclose(fd);
#endif
}

void MDrv_XC_PrintFile(char *fmt, ...)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
#else
    #include <stdio.h>
    #include "string.h"
    #include <unistd.h>
    FILE *fd = NULL;

    va_list args;
    if ((fd = fopen( "/tmp/xc.log","a+" )) == NULL)
        return;

    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fclose(fd);
#endif
}

void MDrv_XC_ShowTrace_Header(MS_U16 u16LogSwitch)
{
    if(_eXCDbgSwitch_MessageType == E_XC_MESSAGE_TYPE_USER)
    {
        printf("[XC,");
        if (u16LogSwitch & XC_DBGLEVEL_SETTIMING)
            printf("LPLL]");
        else if (u16LogSwitch & XC_DBGLEVEL_SETWINDOW)
            printf("SetWindow]");
        else if (u16LogSwitch & XC_DBGLEVEL_MUX)
            printf("Mux]");
        else if (u16LogSwitch & XC_DGBLEVEL_CROPCALC)
            printf("Cropping]");
        else if (u16LogSwitch & XC_DBGLEVEL_AUTOGEOMETRY)
            printf("AUTOGEO]");
        else if (u16LogSwitch & XC_DBGLEVEL_SETTIMING_ISR)
            printf("LPLL_ISR]");
        else if (u16LogSwitch & XC_DBGLEVEL_FUNCTION_TRACE)
            printf("FUNCTION_TRACE]");
        else if (u16LogSwitch & XC_DBGLEVEL_PARAMETER_TRACE)
            printf("PARAMETER_TRACE]");
        else
        printf("Unknown]");
    }
    else if(_eXCDbgSwitch_MessageType == E_XC_MESSAGE_TYPE_KERNEL)
    {
        MDrv_XC_Printk("[XC,");
        if (u16LogSwitch & XC_DBGLEVEL_SETTIMING)
            MDrv_XC_Printk("LPLL]");
        else if (u16LogSwitch & XC_DBGLEVEL_SETWINDOW)
            MDrv_XC_Printk("SetWindow]");
        else if (u16LogSwitch & XC_DBGLEVEL_MUX)
            MDrv_XC_Printk("Mux]");
        else if (u16LogSwitch & XC_DGBLEVEL_CROPCALC)
            MDrv_XC_Printk("Cropping]");
        else if (u16LogSwitch & XC_DBGLEVEL_AUTOGEOMETRY)
            MDrv_XC_Printk("AUTOGEO]");
        else if (u16LogSwitch & XC_DBGLEVEL_SETTIMING_ISR)
            MDrv_XC_Printk("LPLL_ISR]");
        else if (u16LogSwitch & XC_DBGLEVEL_FUNCTION_TRACE)
            MDrv_XC_Printk("FUNCTION_TRACE]");
        else if (u16LogSwitch & XC_DBGLEVEL_PARAMETER_TRACE)
            MDrv_XC_Printk("PARAMETER_TRACE]");
        else
            MDrv_XC_Printk("Unknown]");
    }
    else if(_eXCDbgSwitch_MessageType == E_XC_MESSAGE_TYPE_FILE)
    {
        MDrv_XC_PrintFile("[XC,");
        if (u16LogSwitch & XC_DBGLEVEL_SETTIMING)
            MDrv_XC_PrintFile("LPLL]");
        else if (u16LogSwitch & XC_DBGLEVEL_SETWINDOW)
            MDrv_XC_PrintFile("SetWindow]");
        else if (u16LogSwitch & XC_DBGLEVEL_MUX)
            MDrv_XC_PrintFile("Mux]");
        else if (u16LogSwitch & XC_DGBLEVEL_CROPCALC)
            MDrv_XC_PrintFile("Cropping]");
        else if (u16LogSwitch & XC_DBGLEVEL_AUTOGEOMETRY)
            MDrv_XC_PrintFile("AUTOGEO]");
        else if (u16LogSwitch & XC_DBGLEVEL_SETTIMING_ISR)
            MDrv_XC_PrintFile("LPLL_ISR]");
        else if (u16LogSwitch & XC_DBGLEVEL_FUNCTION_TRACE)
            MDrv_XC_PrintFile("FUNCTION_TRACE]");
        else if (u16LogSwitch & XC_DBGLEVEL_PARAMETER_TRACE)
            MDrv_XC_PrintFile("PARAMETER_TRACE]");
        else
            MDrv_XC_PrintFile("Unknown]");
    }
}
#endif

//-------------------------------------------------------------------------------------------------
/// General Read Byte Function
/// @param  u32Reg                \b IN: which value shouldn't contain xc sub bank and address
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8   MApi_XC_ReadByte_U2(void* pInstance, MS_U32 u32Reg)
{return (MS_U8)MDrv_ReadByte( u32Reg );}

MS_U8   MApi_XC_ReadByte(MS_U32 u32Reg)
{
    return (MS_U8)MDrv_ReadByte( u32Reg );
//Coverity Structually dead code
#if 0

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_READBYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_READBYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// General Write Byte Function
/// @param  u32Reg                \b IN: which value shouldn't contain xc sub bank and address
/// @param  u8Val                \b IN: setting value
//-------------------------------------------------------------------------------------------------
void   MApi_XC_WriteByte_U2(void* pInstance, MS_U32 u32Reg, MS_U8 u8Val)
{MDrv_WriteByte( u32Reg, u8Val );}

void   MApi_XC_WriteByte(MS_U32 u32Reg, MS_U8 u8Val)
{
    MDrv_WriteByte( u32Reg, u8Val );
    return;
//Coverity Structually dead code
#if 0
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_WRITEBYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u8Val = u8Val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_WRITEBYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// General 2 Write Byte Function
/// @param  u32Reg                \b IN: which value shouldn't contain xc sub bank and address
/// @param  u16Val                \b IN: setting value
/// @param  u16Msk                \b IN: setting value
//-------------------------------------------------------------------------------------------------
void MApi_XC_Write2ByteMask_U2(void* pInstance, MS_U32 u32Reg, MS_U16 u16val, MS_U16 u16Mask)
{MDrv_Write2ByteMask(u32Reg, u16val, u16Mask);}

void MApi_XC_Write2ByteMask(MS_U32 u32Reg, MS_U16 u16val, MS_U16 u16Mask)
{
    MDrv_Write2ByteMask(u32Reg, u16val, u16Mask);
    return;
//Coverity Structually dead code
#if 0
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_WRITE2BYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16val = u16val;
    XCArgs.u16Mask = u16Mask;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_WRITE2BYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// General Write Byte Mask Function
/// @param  u32Reg                \b IN: which value shouldn't contain xc sub bank and address
/// @param  u8Val                \b IN: setting value
/// @param  u8Msk                \b IN: decide which bit is valied
//-------------------------------------------------------------------------------------------------
void    MApi_XC_WriteByteMask_U2(void* pInstance, MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Msk)
{MDrv_WriteByteMask( u32Reg, u8Val, u8Msk );}

void    MApi_XC_WriteByteMask(MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Msk)
{
    MDrv_WriteByteMask( u32Reg, u8Val, u8Msk );
    return;

//Coverity Structually dead code
#if 0

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_WRITEBYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u8Val = u8Val;
    XCArgs.u8Msk = u8Msk;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_WRITEBYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// Scaler Write 2 Byte Function, and use mutex to protect while switch bank and write address
/// @param  u32Reg                \b IN: which value contain xc sub bank and address
/// @param  u16Val                \b IN: setting value
//-------------------------------------------------------------------------------------------------
void    MApi_XC_W2BYTE_U2(void* pInstance, MS_U32 u32Reg, MS_U16 u16Val)
{
    MSXC_WR_DBG(printf("W2 BK= %x, REG = %x, Val =%x\n", (MS_U16)((u32Reg&0xFF00)>>8), (MS_U16)(u32Reg&0x00FF), u16Val));
    #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
	_XC_ENTRY();
#endif //#if SC2_ENABLE



    #endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTE(psXCInstPri->u32DeviceID,  u32Reg, u16Val);
     #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE


	#endif
}

void    MApi_XC_W2BYTE(MS_U32 u32Reg, MS_U16 u16Val)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_W2BYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16Val = u16Val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_W2BYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Scaler Read 2 Byte Function, and use mutex to protect while switch bank and write address
/// @param  u32Reg                \b IN: which value contain xc sub bank and address
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16  MApi_XC_R2BYTE_U2(void* pInstance, MS_U32 u32Reg)
{
    MS_U16 u16tmp;
    MSXC_WR_DBG(printf("R2 BK= %x, REG = %x\n", (MS_U16)((u32Reg&0xFF00)>>8), (MS_U16)(u32Reg&0x00FF)));
    // removed for not need to mutex, just spread function
    #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
	_XC_ENTRY();
#endif //#if SC2_ENABLE


    #endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u16tmp = (MS_U16)SC_R2BYTE(psXCInstPri->u32DeviceID,  u32Reg );
    #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE


    #endif
    return u16tmp;
}

MS_U16  MApi_XC_R2BYTE(MS_U32 u32Reg)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_R2BYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_R2BYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

#if 1
MS_U16  MApi_XC2_R2BYTE(MS_U32 u32Reg)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_R2BYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_R2BYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}
#endif

//-------------------------------------------------------------------------------------------------
/// Scaler Write 4 Byte Function, and use mutex to protect while switch bank and write address
/// @param  u32Reg                \b IN: which value contain xc sub bank and address
/// @param  u32Val                \b IN: setting value
//-------------------------------------------------------------------------------------------------
void    MApi_XC_W4BYTE_U2(void* pInstance, MS_U32 u32Reg, MS_U32 u32Val)
{
    MSXC_WR_DBG(printf("W4 BK= %x, REG = %x, Val =%lx\n", (MS_U16)((u32Reg&0xFF00)>>8), (MS_U16)(u32Reg&0x00FF), u32Val));
    // removed for not need to mutex, just spread function
    #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
	_XC_ENTRY();
#endif //#if SC2_ENABLE


    #endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W4BYTE(psXCInstPri->u32DeviceID,  u32Reg, u32Val);
    #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE


    #endif
}

void    MApi_XC_W4BYTE(MS_U32 u32Reg, MS_U32 u32Val)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_W4BYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u32Val = u32Val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_W4BYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Scaler Read 4 Byte Function, and use mutex to protect while switch bank and write address
/// @param  u32Reg                \b IN: which value contain xc sub bank and address
/// @return @ref MS_U32
//-------------------------------------------------------------------------------------------------
MS_U32  MApi_XC_R4BYTE_U2(void* pInstance, MS_U32 u32Reg)
{
    MS_U32 u32tmp;
    MSXC_WR_DBG(printf("R4 BK= %x, REG = %x\n", (MS_U16)((u32Reg&0xFF00)>>8), (MS_U16)(u32Reg&0x00FF)));
    // removed for not need to mutex, just spread function
    #if (!ENABLE_REGISTER_SPREAD)
#if SC2_ENABLE
             _XC_ENTRY(pInstance);
#else //SC2_ENABLE
            _XC_ENTRY();
#endif //#if SC2_ENABLE
    #endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u32tmp = (MS_U32)SC_R4BYTE(psXCInstPri->u32DeviceID,  u32Reg );
    #if (!ENABLE_REGISTER_SPREAD)
#if SC2_ENABLE
            _XC_RETURN(pInstance);
#else //SC2_ENABLE
           _XC_RETURN();
#endif //#if SC2_ENABLE
    #endif
    return u32tmp;

}

MS_U32  MApi_XC_R4BYTE(MS_U32 u32Reg)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_R4BYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_R4BYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Scaler Read 2 Byte Mask Function, and use mutex to protect while switch bank and write address
/// @param  u32Reg                \b IN: which value contain xc sub bank and address
/// @param  u16mask              \b IN: decide which bit is valied
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16  MApi_XC_R2BYTEMSK_U2(void* pInstance, MS_U32 u32Reg, MS_U16 u16mask)
{
    MS_U16 u16tmp;
    //MS_U8  u8OldBank;

    MSXC_WR_DBG(printf("R2M BK= %x, REG = %x\n", (MS_U16)((u32Reg&0xFF00)>>8), (MS_U16)(u32Reg&0x00FF)));
    // removed for not need to mutex, just spread function
    #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    #endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8OldBank = MDrv_ReadByte(BK_SELECT_00);
    if(psXCInstPri == NULL)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
        printk("psXCInstPri is NULL! [%s()][%d]\n",__func__,__LINE__);
        return 0;
#else
        printf("psXCInstPri is NULL! [%s()][%d]\n",__func__,__LINE__);
        return 0;
#endif
    }

    u16tmp = (MS_U16)SC_R2BYTEMSK(psXCInstPri->u32DeviceID,  u32Reg, u16mask);

    //MDrv_WriteByte(BK_SELECT_00, u8OldBank);
    #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE


    #endif
    return u16tmp;
}

MS_U16  MApi_XC_R2BYTEMSK(MS_U32 u32Reg, MS_U16 u16mask)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_R2BYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16mask = u16mask;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_R2BYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Scaler Write 2 Byte Mask Function, and use mutex to protect while switch bank and write address
/// @param  u32Reg                \b IN: which value contain xc sub bank and address
/// @param  u16Val              \b IN: Setting value
/// @param  u16mask              \b IN: decide which bit is valied
//-------------------------------------------------------------------------------------------------
void    MApi_XC_W2BYTEMSK_U2(void* pInstance, MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16mask)
{
    //MS_U8 u8OldBank;

    MSXC_WR_DBG(printf("W2M BK= %x, REG = %x\n", (MS_U16)((u32Reg&0xFF00)>>8), (MS_U16)(u32Reg&0x00FF)));
    // removed for not need to mutex, just spread function
    #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    #endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //u8OldBank = MDrv_ReadByte(BK_SELECT_00);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,  u32Reg, u16Val, u16mask);

    //MDrv_WriteByte(BK_SELECT_00, u8OldBank);
    #if (!ENABLE_REGISTER_SPREAD)

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else //SC2_ENABLE
    _XC_RETURN();
#endif //#if SC2_ENABLE


    #endif
}

void    MApi_XC_W2BYTEMSK(MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16mask)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_W2BYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16Val = u16Val;
    XCArgs.u16mask = u16mask;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_W2BYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#if 1
void    MApi_XC2_W2BYTEMSK(MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16mask)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_W2BYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16Val = u16Val;
    XCArgs.u16mask = u16mask;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_W2BYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16  MApi_XC2_R2BYTEMSK(MS_U32 u32Reg, MS_U16 u16mask)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_R2BYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16mask = u16mask;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_R2BYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

#endif

#if FPGA
void mvideo_set_pnl_info_fpga(XC_PANEL_INFO *pPanel_Info)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart = pPanel_Info->u16HStart;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width = pPanel_Info->u16Width;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart = pPanel_Info->u16VStart;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height = pPanel_Info->u16Height;
}
#endif

void MApi_XC_EnableRepWindowForFrameColor_U2(void* pInstance, MS_BOOL bEnable)
{

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else //SC2_ENABLE
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableRepWindowForFrameColor = bEnable;

#if 0
    if( bEnable == FALSE )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, 0, BIT(8)|BIT(9) ); // Disable display rep window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_42_L, 0, 0xFFFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_43_L, 0, 0xFFFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_44_L, 0, 0xFFFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_45_L, 0, 0xFFFF);
    }
#endif

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_EnableRepWindowForFrameColor(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_REPORTWINDOW_FOR_FRAMECOLOR XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_REPORTWINDOW_FOR_FRAMECOLOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

// scmi type 0: A1/A7/T12/T13, 1: M10/J2/A5/A6/A3
MS_BOOL MDrv_XC_Get_SCMI_Type(void)
{
    return _FIELD_PACKING_MODE_SUPPORTED;
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable RW bank auto
/// @param  bEnable                 \b IN: enable or disable
/// @param  eWindow                 \b IN: which window we are going to enable or disable
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableRWBankAuto_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].bRWBankAuto = bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else //SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].bRWBankAuto = bEnable;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_EnableRWBankAuto(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_AUTO_RWBANK_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_AUTO_RWBANK_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// MDrv_XC_OSDC_TGEN_Setting
/// @param  pstOC_Tgen                 \b IN: the timing control
//-------------------------------------------------------------------------------------------------
void MDrv_XC_OSDC_TGEN_Setting(void *pInstance, XC_OSDC_TIMING *pstOC_Tgen)
{
    MHAL_SC_set_osdc_tgen_htotal(pInstance, pstOC_Tgen->u16OC_Tgen_HTotal);
    MHAL_SC_set_osdc_tgen_vtotal(pInstance, pstOC_Tgen->u16OC_Tgen_VTotal);

    MHAL_SC_set_osdc_tgen_hsync_start(pInstance, pstOC_Tgen->u16OC_Tgen_Hsync_Start);
    MHAL_SC_set_osdc_tgen_hsync_end(pInstance, pstOC_Tgen->u16OC_Tgen_Hsync_End);
    MHAL_SC_set_osdc_tgen_hframe_de_start(pInstance, pstOC_Tgen->u16OC_Tgen_HframDE_Start);
    MHAL_SC_set_osdc_tgen_hframe_de_end(pInstance, pstOC_Tgen->u16OC_Tgen_HframDE_End);

    MHAL_SC_set_osdc_tgen_vsync_start(pInstance, pstOC_Tgen->u16OC_Tgen_Vsync_Start);
    MHAL_SC_set_osdc_tgen_vsync_end(pInstance, pstOC_Tgen->u16OC_Tgen_Vsync_End);
    MHAL_SC_set_osdc_tgen_vframe_de_start(pInstance, pstOC_Tgen->u16OC_Tgen_VframDE_Start);
    MHAL_SC_set_osdc_tgen_vframe_de_end(pInstance, pstOC_Tgen->u16OC_Tgen_VframDE_End);
}

extern void MDrv_PNL_SetOSDCOutputType(void *pInstance,MS_U16 eLPLL_Type, MS_U8 eOC_OutputFormat);
E_APIXC_ReturnValue MDrv_XC_OSDC_Control(void *pInstance, MS_U32 eCtrl_type)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    MS_U32 u32ldHz = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32OSDCCtrltype_Suspend  = eCtrl_type;

    if(gSrcInfo[MAIN_WINDOW].Status2.bIsInitOSDC)
    {
        if(eCtrl_type == E_XC_OSDC_CTRL_MISC)
        {
            MDrv_XC_OSDC_MISC_Control(pInstance, &gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL);
            eRet = E_APIXC_RET_OK;
        }
        else if(eCtrl_type == E_XC_OSDC_CTRL_TGEN)
        {
            MDrv_XC_OSDC_TGEN_Setting(pInstance, &gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING);

            //Set OSDC Output clk
            u32ldHz = (MS_U32)gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HTotal*
                      gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VTotal*
                      pXCResourcePrivate->stdrvXC_MVideo.OSDC_FREQ;

            if((gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type ==0)||
               (gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type >= E_XC_PNL_LPLL_TYPE_MAX))
            {
                gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type = E_XC_PNL_LPLL_HS_LVDS;
            }
            MApi_PNL_SetExt_LPLL_Type(gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type);

            MApi_PNL_CalExtLPLLSETbyDClk(u32ldHz);
            eRet = E_APIXC_RET_OK;
        }
        else if(eCtrl_type == E_XC_OSDC_INIT)
        {
            if(gSrcInfo[MAIN_WINDOW].Status2.u16OC_PreLplltype !=gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type)
            {

                if((gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type >= E_XC_PNL_LPLL_VBY1_10BIT_4LANE)
                    &&(gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type <= E_XC_PNL_LPLL_VBY1_8BIT_8LANE))
                {
               // #ifdef SUPPORT_MOD_ADBANK_SEPARATE

                //#else
                    //Close the RT51 OSD handshack code flow
                    MOD_W2BYTEMSK(REG_MOD_BK00_44_L, BIT(15), BIT(15));
                //#endif
                }

                MDrv_XC_OSDC_MISC_Control(pInstance, &gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL);
                MDrv_XC_OSDC_TGEN_Setting(pInstance, &gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING);

                //Set OSDC Output clk
                u32ldHz = (MS_U32)gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_HTotal*
                          gSrcInfo[MAIN_WINDOW].Status2.stXCOSDCTIMING.u16OC_Tgen_VTotal*
                          pXCResourcePrivate->stdrvXC_MVideo.OSDC_FREQ;
                if((gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type ==0)||
                   (gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type >= E_XC_PNL_LPLL_TYPE_MAX))
                {
                    gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type = E_XC_PNL_LPLL_HS_LVDS;
                }
                MApi_PNL_SetExt_LPLL_Type(gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type);
                MDrv_PNL_SetOSDCOutputType(pInstance,gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type,
                                           gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u8OC_OutputFormat);
                MApi_PNL_CalExtLPLLSETbyDClk(u32ldHz);

                if((gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type >= E_XC_PNL_LPLL_VBY1_10BIT_4LANE)
                    &&(gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type <= E_XC_PNL_LPLL_VBY1_8BIT_8LANE))
                {

              //  #ifdef SUPPORT_MOD_ADBANK_SEPARATE

              //  #else
                    //Open the RT51 OSD handshack code flow
                    MOD_W2BYTEMSK(REG_MOD_BK00_44_L, 0, BIT(15));
              //  #endif
                }
                gSrcInfo[MAIN_WINDOW].Status2.u16OC_PreLplltype =gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type;

            }
            eRet = E_APIXC_RET_OK;
        }
        else if(eCtrl_type == E_XC_OSDC_CTRL_OUTPUT_FORMAT)
        {
            MDrv_PNL_SetOSDCOutputType(pInstance,gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u16OC_Lpll_type,
                                       gSrcInfo[MAIN_WINDOW].Status2.stXCOSDC_CTRL.u8OC_OutputFormat);
            eRet = E_APIXC_RET_OK;
        }
        else
        {
            printf("[Note]Please re-select the control item!\n");
            eRet = E_APIXC_RET_FAIL_ENUM_NOT_SUPPORT;
        }
    }
    else
    {
        eRet = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// MDrv_XC_OSDC_MISC_Control
/// @param  pstOC_Ctrl                 \b IN: the OSDC MISC control
//-------------------------------------------------------------------------------------------------
void MDrv_XC_OSDC_MISC_Control(void *pInstance, XC_OSDC_MSIC_CTRL *pstOC_Ctrl)
{
    MHAL_SC_enable_osdc(pInstance, pstOC_Ctrl->bOC_ClK_En);
    MHAL_SC_set_osdc_mixer_bypass_enable(pInstance, pstOC_Ctrl->bOC_Mixer_Bypass_En);
    MHAL_SC_set_osdc_mixer_inv_alpha_enable(pInstance, pstOC_Ctrl->bOC_Mixer_InvAlpha_En);
    MHAL_SC_set_osdc_mixer_hs_n_vfde_enable(pInstance, pstOC_Ctrl->bOC_Mixer_Hsync_Vfde_Out);
    MHAL_SC_set_osdc_mixer_hfde_n_vfde_enable(pInstance, pstOC_Ctrl->bOC_Mixer_Hfde_Vfde_Out);
}

//-------------------------------------------------------------------------------------------------
/// MDrv_XC_OSDC_GetDataFromRegister
/// @param  pstOC_Tgen                 \b IN: the timing control
/// @param  pstOC_Ctrl                 \b IN: the OSDC MISC control
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_OSDC_GetDataFromRegister(void *pInstance, XC_OSDC_TIMING *pstOC_Tgen, XC_OSDC_MSIC_CTRL *pstOC_Ctrl)
{
    if((NULL == pstOC_Tgen)||(NULL == pstOC_Ctrl))
    {
        return FALSE;
    }

    pstOC_Tgen->u16OC_Tgen_HTotal              = MHAL_SC_get_osdc_tgen_htotal(pInstance);
    pstOC_Tgen->u16OC_Tgen_Hsync_Start         = MHAL_SC_get_osdc_tgen_hsync_start(pInstance);
    pstOC_Tgen->u16OC_Tgen_Hsync_End           = MHAL_SC_get_osdc_tgen_hsync_end(pInstance);
    pstOC_Tgen->u16OC_Tgen_HframDE_Start       = MHAL_SC_get_osdc_tgen_hframe_de_start(pInstance);
    pstOC_Tgen->u16OC_Tgen_HframDE_End         = MHAL_SC_get_osdc_tgen_hframe_de_end(pInstance);
    pstOC_Tgen->u16OC_Tgen_VTotal              = MHAL_SC_get_osdc_tgen_vtotal(pInstance);
    pstOC_Tgen->u16OC_Tgen_Vsync_Start         = MHAL_SC_get_osdc_tgen_vsync_start(pInstance);
    pstOC_Tgen->u16OC_Tgen_Vsync_End           = MHAL_SC_get_osdc_tgen_vsync_end(pInstance);
    pstOC_Tgen->u16OC_Tgen_VframDE_Start       = MHAL_SC_get_osdc_tgen_vframe_de_start(pInstance);
    pstOC_Tgen->u16OC_Tgen_VframDE_End         = MHAL_SC_get_osdc_tgen_vframe_de_end(pInstance);

    pstOC_Ctrl->bOC_ClK_En                     = MHAL_SC_get_osdc_onoff_status(pInstance);
    pstOC_Ctrl->bOC_Mixer_Bypass_En            = MHAL_SC_get_osdc_mixer_bypass_status(pInstance);
    pstOC_Ctrl->bOC_Mixer_InvAlpha_En          = MHAL_SC_get_osdc_mixer_inv_alpha_status(pInstance);
    pstOC_Ctrl->bOC_Mixer_Hsync_Vfde_Out       = MHAL_SC_get_osdc_mixer_hs_n_vfde_status(pInstance);
    pstOC_Ctrl->bOC_Mixer_Hfde_Vfde_Out        = MHAL_SC_get_osdc_mixer_hfde_n_vfde_status(pInstance);

    return TRUE;
}

void MDrv_XC_OSDC_TGENRST_EN(void *pInstance, MS_BOOL bEnable)
{
    MHAL_SC_set_osdc_tgen_reset_enable(pInstance, bEnable);
}

void MDrv_XC_OSDC_SWRST_EN(void *pInstance, MS_BOOL bEnable)
{
    MHAL_SC_set_osdc_swreset_enable(pInstance, bEnable);
}

E_APIXC_ReturnValue MDrv_XC_GetChipCaps(E_XC_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size)
{
    switch(eCapType)
    {
        case E_XC_IMMESWITCH:
            *pRet = SUPPORT_IMMESWITCH;
            break;
        case E_XC_DVI_AUTO_EQ:
            *pRet = SUPPORT_DVI_AUTO_EQ;
            break;
        case E_XC_FRC_INSIDE:
            *pRet = FRC_INSIDE;
            break;
        case E_XC_SUPPORT_FRCM_MODE:
            *pRet = SUPPORT_FRCM_MODE;
            break;
        case E_XC_DIP_CHIP_CAPS:
            {
                if( ret_size != sizeof(ST_XC_DIP_CHIPCAPS))
                {
                    return E_APIXC_RET_FAIL_INVALID_PARAMETER;
                }
                ST_XC_DIP_CHIPCAPS *stDipCaps;
                stDipCaps = (ST_XC_DIP_CHIPCAPS *)pRet;
                DIP_CHIP_CAP(stDipCaps->eWindow,stDipCaps->u32DipChipCaps);
                break;
            }
        case E_XC_3D_FBL_CAPS:
#ifndef DISABLE_3D_FUNCTION
            *pRet = MDrv_SC_3D_IsSupportFBL3D();
#else
            *pRet = FALSE;
#endif
            break;

        case E_XC_SUPPORT_DETECT3D_IN3DMODE:
#ifndef DISABLE_3D_FUNCTION
    #if HW_2DTO3D_SUPPORT
            *pRet = (HW_2DTO3D_VER >= 3);
    #else
            *pRet = FALSE;
    #endif
#else
            *pRet = FALSE;
#endif
            break;

        case E_XC_HW_SEAMLESS_ZAPPING:
            *pRet = SUPPORT_SEAMLESS_ZAPPING;
            break;
        case E_XC_SUPPORT_DEVICE1:
            *pRet = SUPPORT_DEVICE1;
            break;

        case E_XC_2DTO3D_VERSION:
#if HW_2DTO3D_SUPPORT
            *pRet = HW_2DTO3D_VER;
#else
            *pRet = 0xFFFFFFFF;
#endif
            break;
        case E_XC_SUPPORT_FORCE_VSP_IN_DS_MODE:
            *pRet = HW_SUPPORT_FORCE_VSP_IN_DS_MODE;
            break;
        default:
            return E_APIXC_RET_FAIL_ENUM_NOT_SUPPORT;
    }
    return E_APIXC_RET_OK;
}

#if SC2_ENABLE
MS_U32 MDrv_XC_Get_Semaphore(void *pInstance,E_XC_POOL_ID_TYPE eID_TYPE)
{
   // printf("\33[0;35m Jayce!1 : [%s][%d],pInstance=[%d] \33[m \n",__FUNCTION__,__LINE__,pInstance);
	E_XC_POOL_ID eID=E_XC_POOL_ID_MAX;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

	if(pInstance == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }
    else
	{
    	UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
	}

    if(eID_TYPE == E_XC_ID_REG)
    {
		eID=_XC_SELECT_INTERNAL_REGISTER(psXCInstPri->u32DeviceID);
    }
	else if(eID_TYPE == E_XC_ID_VAR)
    {
		eID=_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID);
    }
	else if(eID_TYPE == E_XC_ID_MLD)
    {
		eID=_XC_SELECT_INTERNAL_MENULOAD(psXCInstPri->u32DeviceID);
    }

    if (eID >= E_XC_POOL_ID_MAX)
    {
        printf("[%s,%5d] Unknown XC Pool ID\n",__func__, __LINE__);
        return u32Return;
    }
	if (eID == E_XC_POOL_ID_INTERNAL_REGISTER || eID == E_XC_POOL_ID_INTERNAL_REGISTER_1 ||
        eID == E_XC_POOL_ID_INTERNAL_MENULOAD || eID == E_XC_POOL_ID_INTERNAL_MENULOAD_1)
    {
        return u32Return;
    }


    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    //printf("\33[0;35m Jayce! : [%s][%d],eID_TYPE=[%d],eID=[%d] pModule = [%d] \33[m \n",__FUNCTION__,__LINE__,eID_TYPE,eID,pModule);

    if(UtopiaResourceObtain(pModule, eID, &g_pXCResource[eID]) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    u32Return = UTOPIA_STATUS_SUCCESS;
    //printf("\33[0;35m Jayce!0 : [%s][%d],pInstance=[%d] \33[m \n",__FUNCTION__,__LINE__,pInstance);
    return u32Return;
}
#else //SC2_ENABLE
MS_U32 MDrv_XC_Get_Semaphore(E_XC_POOL_ID eID)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (eID >= E_XC_POOL_ID_MAX)
    {
        printf("[%s,%5d] Unknown XC Pool ID\n",__func__, __LINE__);
        return u32Return;
    }

//K3_U2
    if (eID == E_XC_POOL_ID_INTERNAL_REGISTER || eID == E_XC_POOL_ID_INTERNAL_REGISTER_1 ||
        eID == E_XC_POOL_ID_INTERNAL_MENULOAD || eID == E_XC_POOL_ID_INTERNAL_MENULOAD_1)
    {
        return u32Return;
    }

    if (((eID >= E_XC_POOL_ID_NUM) && (pu32XCInst_1 == NULL)) ||
        ((eID < E_XC_POOL_ID_NUM) && (pu32XCInst_private == NULL)))
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first, resourceID %d\n",__FUNCTION__,__LINE__,eID);
        return u32Return;
    }

    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    void* pModule = NULL;
    UtopiaInstanceGetModule((eID >= E_XC_POOL_ID_NUM)? pu32XCInst_1: pu32XCInst_private, &pModule);

    if(UtopiaResourceObtain(pModule, eID, &g_pXCResource[eID]) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    u32Return = UTOPIA_STATUS_SUCCESS;
    return u32Return;
}
#endif //#if SC2_ENABLE



#if SC2_ENABLE
MS_U32 MDrv_XC_Release_Semaphore(void *pInstance,E_XC_POOL_ID_TYPE eID_TYPE)
{
	E_XC_POOL_ID eID=E_XC_POOL_ID_MAX;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;

	if(pInstance == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }else
	{
    	UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
	}
	if(eID_TYPE == E_XC_ID_REG)
		eID=_XC_SELECT_INTERNAL_REGISTER(psXCInstPri->u32DeviceID);
	else if(eID_TYPE == E_XC_ID_VAR)
		eID=_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID);
	else if(eID_TYPE == E_XC_ID_MLD)
		eID=_XC_SELECT_INTERNAL_MENULOAD(psXCInstPri->u32DeviceID);

    if (eID >= E_XC_POOL_ID_MAX)
    {
        printf("[%s,%5d] Unknown XC Pool ID\n",__func__, __LINE__);
        return u32Return;
    }
    //printf("\33[0;35m Jayce! : [%s][%d],eID_TYPE=[%d],eID=[%d] \33[m \n",__FUNCTION__,__LINE__,eID_TYPE,eID);

#if SC2_ENABLE

	if (eID == E_XC_POOL_ID_INTERNAL_REGISTER || eID == E_XC_POOL_ID_INTERNAL_REGISTER_1 ||
        eID == E_XC_POOL_ID_INTERNAL_MENULOAD || eID == E_XC_POOL_ID_INTERNAL_MENULOAD_1)
    {
        return u32Return;
    }

#endif //#if SC2_ENABLE



    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    u32Return = UtopiaResourceRelease(g_pXCResource[eID]);
    return u32Return;
}
#else //SC2_ENABLE
MS_U32 MDrv_XC_Release_Semaphore(E_XC_POOL_ID eID)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (eID >= E_XC_POOL_ID_MAX)
    {
        printf("[%s,%5d] Unknown XC Pool ID\n",__func__, __LINE__);
        return u32Return;
    }

//K3_U2
    if ((eID == E_XC_POOL_ID_INTERNAL_REGISTER) || (eID == E_XC_POOL_ID_INTERNAL_REGISTER_1) ||
        (eID == E_XC_POOL_ID_INTERNAL_MENULOAD) || (eID == E_XC_POOL_ID_INTERNAL_MENULOAD_1))
    {
        return u32Return;
    }

    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    u32Return = UtopiaResourceRelease(g_pXCResource[eID]);
    return u32Return;
}
#endif //#if SC2_ENABLE


E_XC_POOL_ID MDrv_XC_Resource_Mapping(MS_U32 u32InstanceID, E_XC_POOL_ID eID)
{
    if(eID >= E_XC_POOL_ID_MAX)
    {
        printf("Error, Invalid POOL ID!!!\n");
        return E_XC_POOL_ID_MAX;
    }
    if(u32InstanceID == 0)
    {
        //FIXME?
        return eID;
    }
    else if(u32InstanceID == 1)
    {
        switch(eID)
        {
            case E_XC_POOL_ID_INTERNAL_REGISTER:
            case E_XC_POOL_ID_INTERNAL_REGISTER_1:
                return E_XC_POOL_ID_INTERNAL_REGISTER_1;
            case E_XC_POOL_ID_INTERNAL_VARIABLE:
            case E_XC_POOL_ID_INTERNAL_VARIABLE_1:
                return E_XC_POOL_ID_INTERNAL_VARIABLE_1;
            case E_XC_POOL_ID_INTERNAL_MENULOAD:
            case E_XC_POOL_ID_INTERNAL_MENULOAD_1:
                return E_XC_POOL_ID_INTERNAL_MENULOAD_1;
            default:
                return eID;
        }
    }
    else
    {
        printf("u32InstanceID = %ld is over 1!!\n", u32InstanceID);
        return E_XC_POOL_ID_MAX;
    }
}

#if 1
void MDrv_XC_Set_VOP(void *pInstance, MS_XC_VOP_Data *pstVOPData)

{
    //Note: H/V Total, H/V DE End, H/V Syncwidth all need minus 1 when set to register
    MS_U16 u16DE_HEND = pstVOPData->u16HStart + pstVOPData->u16Width - 1;
    MS_U16 u16DE_HStart = pstVOPData->u16HStart;
    MS_U16 u16DE_VEND = pstVOPData->u16VStart + pstVOPData->u16Height - 1;
    MS_U16 u16DE_VStart = pstVOPData->u16VStart;
    MS_U16 u16VsyncStart = pstVOPData->u16VTotal - pstVOPData->u16VSyncStart;// vsync start = Vtt - (VSyncWidth + BackPorch)


    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    printf("##@##[%s][%d][%x]\n", __FUNCTION__, __LINE__, psXCInstPri->u32DeviceID);



    // Htotal/Vtotal
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L, (pstVOPData->u16HTotal - 1), 0x1FFF);         // output htotal
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, (pstVOPData->u16VTotal - 1), 0xFFF);         // output vtotal

    // DE H/V start/size
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, u16DE_HStart, VOP_DE_HSTART_MASK);                                       // DE H start
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, u16DE_HEND  , VOP_DE_HEND_MASK);  // DE H end
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L, u16DE_VStart, 0xFFF);                                       // DE V start
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, u16DE_VEND  , 0xFFF);  // DE V end

    // Display H/V start/size
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, u16DE_HStart , VOP_DISPLAY_HSTART_MASK);                                               // Display H start
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, u16DE_HEND   , VOP_DISPLAY_HEND_MASK);  // Display H end
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, u16DE_VStart , 0xFFF);                                               // Display V start
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, u16DE_VEND   , 0xFFF);  // Display V end

    // H/V sync start/width
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, (pstVOPData->u8HSyncWidth-1), LBMASK);                    // hsync width
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, ((u16VsyncStart & 0x7FF) | ((u16VsyncStart & 0x800) << 1)), 0x17FF);     // vsync start = Vtt - (VSyncWidth + BackPorch)
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, (u16VsyncStart + pstVOPData->u8VSyncWidth - 1), VOP_VSYNC_END_MASK);    // vsync end = Vsync start + Vsync Width
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, (pstVOPData->bManuelVSyncCtrl << 15), BIT(15));                                  // manual vsync control
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, BIT(14), BIT(14));       //default set to mode1                                                                       // disable auto_htotal
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_23_L, BIT(7), BIT(7));

    // output control
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_21_L, pstVOPData->u16OCTRL);

    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_46_L, pstVOPData->u16OSTRL);

    // output driving current
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_47_L, pstVOPData->u16ODRV, LBMASK);
}


#endif //#if SC2_ENABLE

#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
MS_BOOL MDrv_SC_OP1_PatGen_Init(void *pInstance, EN_XC_OP1_PATGEN_MODE eMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(!pXCResourcePrivate->stdrvXC_MVideo_Context.bOP1_PatGen_Init)
    {
        MHal_SC_OP1_Pattern_backup_setting(pInstance);
        pXCResourcePrivate->stdrvXC_MVideo_Context.bOP1_PatGen_Init = TRUE;
    }

    if(FALSE == MHal_SC_OP1_Pattern_set_patgen_mode(pInstance,eMode))
    {
        return FALSE;
    }
    MHal_SC_OP1_Pattern_init_setting(pInstance);

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);

    //MS_U16 u16OP1_PatGen_HSize = 0;
    //MS_U16 u16OP1_PatGen_VSize = 0;

    //Set the hSize and VSize
    //u16OP1_PatGen_HSize = gSrcInfo[MAIN_WINDOW].stDispWin.width - gSrcInfo[MAIN_WINDOW].stDispWin.x ;
    //u16OP1_PatGen_VSize = gSrcInfo[MAIN_WINDOW].stDispWin.height - gSrcInfo[MAIN_WINDOW].stDispWin.y;

    //MHal_SC_OP1_Pattern_set_frame_size(u16OP1_PatGen_HSize, u16OP1_PatGen_VSize);

    MHal_SC_OP1_Pattern_set_frame_size(pInstance,pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height);

    MHal_SC_OP1_Pattern_enable(pInstance,ENABLE);

    // due to change di1lb, need to wait HW done
    MsOS_DelayTask(50);

    return TRUE;

}

E_APIXC_ReturnValue MDrv_SC_Set_OP1_TestPattern_RGB(void *pInstance, EN_XC_OP1_PATTERN ePattern,EN_XC_OP1_PATGEN_MODE eMode, MS_U16 u16RVal, MS_U16 u16GVal, MS_U16 u16BVal)
{
    E_APIXC_ReturnValue enResult = E_APIXC_RET_OK;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16PanelWidth,u16PanelHeight;
    u16PanelWidth =  pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
    u16PanelHeight = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_62_L, u16PanelWidth,0xFFFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_52_L, u16PanelWidth,0xFFFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_61_L, u16PanelHeight,0xFFFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_51_L, u16PanelHeight,0xFFFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_55_L, u16PanelHeight,0xFFFF);

    //initialize the op1 pattern gen

    //temp solution for fixing the broken problem when changing pattern directly.
    //need to mute first before any pattern change ot disable pattern

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(1),BIT(1)); //window color en : on
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, BIT(7),BIT(7)); //input source disable

    //MDrv_SC_OP1_PatGen_Init(mode);

    // Disable then enable to make a reset, to avoid SRAM data wrong
    MDrv_SC_OP1_PatGen_Disable(pInstance);

    //MsOS_DelayTask(50);
    MDrv_SC_OP1_PatGen_Init(pInstance, eMode);
    //MsOS_DelayTask(50);

    if(gSrcInfo[MAIN_WINDOW].bUseYUVSpace)
    {
        //if(!bSkipColorMatrix)
        MHAL_SC_set_r2y_en(pInstance, ENABLE, MAIN_WINDOW);

    }
    else if(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_26_L)!= 0x400 )
    {
        // prevent fist time boot up with no signal
        //if(!bSkipColorMatrix)
        MHAL_SC_set_r2y_en(pInstance, ENABLE, MAIN_WINDOW);

    }

    if(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L) == 0 ) //by pass 3x3 color matrix
    {
        //if(!bSkipColorMatrix)
        MHAL_SC_set_r2y_en(pInstance, DISABLE, MAIN_WINDOW);
    }
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_6F_L, 0x00,0x000C); //Sub SRAM clock gating
    MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,u16RVal,u16GVal,u16BVal);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, 0x00,BIT(7)); //input source enable
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, 0x00,BIT(1)); //window color en : off

    return enResult;
}
E_APIXC_ReturnValue MDrv_SC_Set_OP1_TestPattern(void *pInstance, EN_XC_OP1_PATTERN ePattern, EN_XC_OP1_PATGEN_MODE eMode)
{
    E_APIXC_ReturnValue enResult = E_APIXC_RET_OK;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //initialize the op1 pattern gen

    //temp solution for fixing the broken problem when changing pattern directly.
    //need to mute first before any pattern change ot disable pattern

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(1),BIT(1)); //window color en : on
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, BIT(7),BIT(7)); //input source disable

    //MDrv_SC_OP1_PatGen_Init(mode);

    // Disable then enable to make a reset, to avoid SRAM data wrong
    MDrv_SC_OP1_PatGen_Disable(pInstance);
    //MsOS_DelayTask(50);
    MDrv_SC_OP1_PatGen_Init(pInstance, eMode);
    //MsOS_DelayTask(50);

    if(gSrcInfo[MAIN_WINDOW].bUseYUVSpace)
    {
        //if(!bSkipColorMatrix)
        MHAL_SC_set_r2y_en(pInstance, ENABLE, MAIN_WINDOW);

    }
    else if(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_26_L)!= 0x400 )
    {
        // prevent fist time boot up with no signal
        //if(!bSkipColorMatrix)
        MHAL_SC_set_r2y_en(pInstance, ENABLE, MAIN_WINDOW);

    }

    if(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L) == 0 ) //by pass 3x3 color matrix
    {
        //if(!bSkipColorMatrix)
        MHAL_SC_set_r2y_en(pInstance, DISABLE, MAIN_WINDOW);
    }

    switch (ePattern)
    {
        case E_XC_OP1_WB_PATTERN:
            MDrv_SC_OP1_WB_Pattern(pInstance, 8, 12);
            break;
        case E_XC_OP1_PURE_RED_PATTERN:
            MDrv_SC_OP1_HW_Default_Pattern(pInstance, 0x20);
            break;
        case E_XC_OP1_PURE_GREEN_PATTERN:
            MDrv_SC_OP1_HW_Default_Pattern(pInstance, 0x30);
            break;
        case E_XC_OP1_PURE_BLUE_PATTERN:
            MDrv_SC_OP1_HW_Default_Pattern(pInstance, 0x40);
            break;
        case E_XC_OP1_PURE_WHITE_PATTERN:
            MDrv_SC_OP1_HW_Default_Pattern(pInstance, 0x50);
            break;
        case E_XC_OP1_PURE_BLACK_PATTERN:
            MDrv_SC_OP1_HW_Default_Pattern(pInstance, 0x60);
            break;
        case E_XC_OP1_PURE_GREY_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,155,155,155);
            break;
        case E_XC_OP1_PURE_COLOR_V_BAR_PATTERN:
            MDrv_SC_OP1_Pure_Color_V_Bar_Pattern(pInstance, 8, 12);
            break;
        case E_XC_OP1_PURE_COLOR_H_BAR_PATTERN:
            MDrv_SC_OP1_Pure_Color_H_Bar_Pattern(pInstance, 8, 8);
            break;
        case E_XC_OP1_PURE_GREY_BAR_16_PATTERN:
            MDrv_SC_OP1_Pure_Gray_Bar_Pattern(pInstance, 32, 12, 2, 11, 16);
            break;
        case E_XC_OP1_PURE_GREY_BAR_32_PATTERN:
            MDrv_SC_OP1_Pure_Gray_Bar_Pattern(pInstance, 32, 12, 1, 11, 32);
            break;
        case E_XC_OP1_PURE_GREY_BAR_64_PATTERN:
            MDrv_SC_OP1_Pure_Gray_Bar_Pattern(pInstance, 128, 12, 2,11,64);
            break;
        case E_XC_OP1_PURE_GREY_RGB_32_PATTERN:
            MDrv_SC_OP1_Pure_RGB_32_Pattern(pInstance, 32, 8, 1, 1, 32);
            break;
        case E_XC_OP1_PURE_RGB_CHECKERBOARD_PATTERN:
            MDrv_SC_OP1_Pure_RGB_Checkerboard_Pattern_GBChange(pInstance, 3, 3);
            break;
        case E_XC_OP1_PURE_RGB_CHECKERBOARD_PATTERN_2:
            MDrv_SC_OP1_HW_Default_Pattern(pInstance, 0x70);
            break;
        case E_XC_OP1_DOAFADE_GRAY_PATTERN:
            MDrv_SC_OP1_Dotfade_Gray_Pattern(pInstance, 8, 12);
            break;
        case E_XC_OP1_CALIBRATION_PATTERN:
            MDrv_SC_OP1_HW_Default_Pattern(pInstance, 0x80);
            break;
        case E_XC_OP1_CALIBRATION_COLOR_DOTFADE_PATTERN:
            MDrv_SC_OP1_Calibratoin_Color_Dotfade_Pattern(pInstance, 8,12);
            break;
        case E_XC_OP1_PATTERN_1:
            MDrv_SC_OP1_1_Pattern(pInstance, 192,216);
            break;
        case E_XC_OP1_PATTERN_2:
            MDrv_SC_OP1_2_Pattern(pInstance, 16,27);
            break;
        case E_XC_OP1_PATTERN_3:
            MDrv_SC_OP1_3_Pattern(pInstance, 24,108);
            break;
        case E_XC_OP1_3D_PATTERN:
            MDrv_SC_OP1_3D_Pattern(pInstance, 64,30);
            break;
        case E_XC_OP1_PURE_RED_100IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,940,0,0);
            break;
        case E_XC_OP1_PURE_GREEN_100IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,940,0);
            break;
        case E_XC_OP1_PURE_BLUE_100IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,0,940);
            break;
        case E_XC_OP1_PURE_RED_70IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,658,0,0);
            break;
        case E_XC_OP1_PURE_GREEN_70IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,658,0);
            break;
        case E_XC_OP1_PURE_BLUE_70IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,0,658);
            break;
        case E_XC_OP1_PURE_RED_40IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,376,0,0);
            break;
        case E_XC_OP1_PURE_GREEN_40IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,376,0);
            break;
        case E_XC_OP1_PURE_BLUE_40IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,0,376);
            break;
        case E_XC_OP1_PURE_RED_20IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,188,0,0);
            break;
        case E_XC_OP1_PURE_GREEN_20IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,188,0);
            break;
        case E_XC_OP1_PURE_BLUE_20IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,0,188);
            break;
        case E_XC_OP1_PURE_RED_10IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,94,0,0);
            break;
        case E_XC_OP1_PURE_GREEN_10IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,94,0);
            break;
        case E_XC_OP1_PURE_BLUE_10IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,0,0,94);
            break;
        case E_XC_OP1_PURE_WHITE_100IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,940,940,940);
            break;
        case E_XC_OP1_PURE_WHITE_90IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,846,846,846);
            break;
        case E_XC_OP1_PURE_WHITE_80IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,752,752,752);
            break;
        case E_XC_OP1_PURE_WHITE_70IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,658,658,658);
            break;
        case E_XC_OP1_PURE_WHITE_60IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,564,564,564);
            break;
        case E_XC_OP1_PURE_WHITE_50IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,470,470,470);
            break;
        case E_XC_OP1_PURE_WHITE_40IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,376,376,376);
            break;
        case E_XC_OP1_PURE_WHITE_30IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,282,282,282);
            break;
        case E_XC_OP1_PURE_WHITE_20IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,188,188,188);
            break;
        case E_XC_OP1_PURE_WHITE_10IRE_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,94,94,94);
            break;
        case E_XC_OP1_PURE_GAMMA_GREY1_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,178,178,178);
            break;
        case E_XC_OP1_PURE_GAMMA_GREY2_PATTERN:
            MDrv_SC_OP1_Pure_Color_Tile_Pattern(pInstance, 8,12,50,50,50);
            break;
        default:
            printf("Error : Not support this pattern !!");
            enResult = E_APIXC_RET_FAIL_INVALID_PARAMETER;
            break;
    }

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, 0x00,BIT(7)); //input source enable
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, 0x00,BIT(1)); //window color en : off

    return enResult;
}

MS_BOOL MDrv_SC_OP1_PatGen_Disable(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(!pXCResourcePrivate->stdrvXC_MVideo_Context.bOP1_PatGen_Init)
        return FALSE;

    //disable the op1 test pattern
    MHal_SC_OP1_Pattern_enable(pInstance,DISABLE);

    //recover to original setting
    MHal_SC_OP1_Pattern_restore_setting(pInstance);

    // due to change di1lb, need to wait HW done
    MsOS_DelayTask(50);

    return TRUE;
}

void MDrv_SC_OP1_Align_Width_Height(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt, MS_U16 *u16AlignWidth, MS_U16 *u16AlignHeight)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    #if 0
    if ((g_XC_InitData.stPanelInfo.u16Width > 1900) && (g_XC_InitData.stPanelInfo.u16Height > 1000))
    {
        // FHD case
        *u16AlignWidth = (MS_U16)(1920/u16HTileCnt +0.5) * u16HTileCnt;
        *u16AlignHeight = (MS_U16)(1080/u16VTileCnt +0.5) * u16VTileCnt;
    }
    else
    {
        // HD case
        *u16AlignWidth = (MS_U16)(1366/u16HTileCnt +0.5) * u16HTileCnt;
        *u16AlignHeight = (MS_U16)(768/u16VTileCnt +0.5) * u16VTileCnt;
    }
    #else
    MS_U16 u16_tmp = 0;
    u16_tmp = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width % u16HTileCnt;
    if(u16_tmp !=0)
        *u16AlignWidth = ((MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/u16HTileCnt) + 1) * u16HTileCnt;
    else
        *u16AlignWidth = (MS_U16)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/u16HTileCnt * u16HTileCnt;
    u16_tmp = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/u16VTileCnt;
    if(u16_tmp !=0)
        *u16AlignHeight = ((MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/u16VTileCnt + 1)) * u16VTileCnt;
    else
        *u16AlignHeight = (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/u16VTileCnt) * u16VTileCnt;
    #endif

}

void MDrv_SC_OP1_HW_Default_Pattern(void *pInstance, MS_U8 u8Val)
{
    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;
    MDrv_SC_OP1_Align_Width_Height(pInstance, 1, 1, &u16HTotalSize, &u16VTotalSize);
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_pure_color(pInstance,u8Val);
    MHal_SC_OP1_Pattern_enable(pInstance,ENABLE);

}

void MDrv_SC_OP1_WB_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);


    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,2,true,true,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,true,false,160,160,160,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,2,false,false,263,263,263,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,false,true,100,100,100,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,true,false,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,2,false,false,678,678,678,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,false,true,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,2,true,true,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);
    }
    else
    {
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,2,true,true,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,true,false,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,2,false,false,678,678,678,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,false,true,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,true,false,160,160,160,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,2,false,false,263,263,263,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,false,true,100,100,100,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,1,true,true,512,512,512,E_XC_OP1_PATTERN_TYPE_PURE);
    }

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);
}

void MDrv_SC_OP1_Pure_Color_V_Bar_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if (!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        MDrv_SC_OP1_HW_Default_Pattern(pInstance, 0x10);
    }
    else
    {

    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);


    #if 1
    // For mirror case
    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,11,true,false,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,11,false,false,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,11,false,false,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,11,false,false,1023,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,11,false,false,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,11,false,false,0,1023,1023,E_XC_OP1_PATTERN_TYPE_PURE);
    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,11,false,false,1023,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,11,false,true,1023,1023,1023,E_XC_OP1_PATTERN_TYPE_PURE);
    #else
    if (g_XC_InitData.bMirror)
    {
        MHal_SC_OP1_Pattern_customize_blcok(1,11,true,false,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,1023,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,0,1023,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,1023,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,true,1023,1023,1023,E_XC_OP1_PATTERN_TYPE_PURE);
    }
    else
    {
        MHal_SC_OP1_Pattern_customize_blcok(1,11,true,false,1023,1023,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,1023,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,0,1023,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,1023,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,false,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(1,11,false,true,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
    }
    #endif

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);
    }

}
void MDrv_SC_OP1_Pure_Color_H_Bar_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if (!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        MDrv_SC_OP1_HW_Default_Pattern(pInstance, 0x90);
    }
    else
    {
        MS_U16 u16HTileSize = 0, u16VTileSize = 0;
        MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;

        //clear default pattern
        MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

        //disable border
        MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

        MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

        u16HTileSize = u16HTotalSize/u16HTileCnt;
        u16VTileSize = u16VTotalSize/u16VTileCnt;
        MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

        MHal_SC_OP1_Pattern_sram_writestart(pInstance);

        MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,1023,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,0,1023,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,1023,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,0,true,true,1023,1023,1023,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_sram_writedone(pInstance);
    }
}

void MDrv_SC_OP1_Pure_Gray_Bar_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt, MS_U8 block_Size, MS_U8 repeat_Num, MS_U8 level)
{
    MS_U8 i;
    MS_U16 u16ValPerLevel_10bits=0;
    MS_U16 u16Cur_Val =0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u16ValPerLevel_10bits = (1024/level) ;

    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0, 0, 0,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,u16Cur_Val, u16Cur_Val, u16Cur_Val,E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Cur_Val -= ((u16ValPerLevel_10bits) & 0x3ff);
        }
    }
    else
    {
        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,u16Cur_Val, u16Cur_Val, u16Cur_Val,E_XC_OP1_PATTERN_TYPE_PURE);
            }

            u16Cur_Val += ((u16ValPerLevel_10bits) & 0x3ff);
        }
    }

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);

}

void MDrv_SC_OP1_Pure_RGB_32_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt, MS_U8 block_Size,MS_U8 repeat_Num,MS_U8 level)
{
    MS_U8 i;
    MS_U16 u16ValPerLevel_10bits=0;
    MS_U16 u16Cur_Val =0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u16ValPerLevel_10bits = (1024/level) ;

    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        //Blue
        u16Cur_Val = 1024;

        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x00, 0x00, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,0x00, 0x00, u16Cur_Val,E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Cur_Val -= ((u16ValPerLevel_10bits) & 0x3ff);
        }

        u16Cur_Val = 1024;
        //Green
        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x00, 0x3FF, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,0x00, u16Cur_Val, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Cur_Val -= ((u16ValPerLevel_10bits) & 0x3ff);
        }

        u16Cur_Val = 1024;
        //Red
        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x3FF, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,u16Cur_Val, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Cur_Val -= ((u16ValPerLevel_10bits) & 0x3ff);
        }

        u16Cur_Val = 1024;
        //Gray
        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0x00, 0x00, 0x0,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,u16Cur_Val, u16Cur_Val, u16Cur_Val,E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Cur_Val -= ((u16ValPerLevel_10bits) & 0x3ff);
        }
        u16Cur_Val = 1024;
    }
    else
    {
        //Gray
        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,u16Cur_Val, u16Cur_Val, u16Cur_Val,E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Cur_Val += ((u16ValPerLevel_10bits) & 0x3ff);
        }

        u16Cur_Val = 0;

        //Red
        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0x3FF, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,u16Cur_Val, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Cur_Val += ((u16ValPerLevel_10bits) & 0x3ff);
        }
        u16Cur_Val = 0;

        //Green
        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0x00, 0x3FF, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,0x00, u16Cur_Val, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Cur_Val += ((u16ValPerLevel_10bits) & 0x3ff);
        }
        u16Cur_Val = 0;

        //Blue
        for(i=0;i<level;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,TRUE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
            }else if(i== (level -1))
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,TRUE,0x00, 0x00, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,block_Size,repeat_Num,FALSE,FALSE,0x00, 0x00, u16Cur_Val,E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Cur_Val += ((u16ValPerLevel_10bits) & 0x3ff);
        }
        u16Cur_Val = 0;
    }

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);
}

void MDrv_SC_OP1_Pure_RGB_Checkerboard_Pattern_GBChange(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,TRUE,FALSE,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,TRUE,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,TRUE,FALSE,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,TRUE,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,TRUE,FALSE,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,TRUE,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
    }
    else
    {
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,TRUE,FALSE,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,TRUE,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,TRUE,FALSE,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,TRUE,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,TRUE,FALSE,0,1023,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,0,0,1023,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,TRUE,1023,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
    }

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);
}

void MDrv_SC_OP1_Dotfade_Gray_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        MHal_SC_OP1_Pattern_set_dotfade_value(pInstance,236, 236, 236);
        MHal_SC_OP1_Pattern_set_dotfade_overfollow_protection(pInstance,TRUE, TRUE, TRUE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8, 11, TRUE, TRUE, 1023, 1023, 1023, E_XC_OP1_PATTERN_TYPE_DOTFADE);
    }
    else
    {
        MHal_SC_OP1_Pattern_set_dotfade_value(pInstance,18, 18, 18);
        MHal_SC_OP1_Pattern_set_dotfade_overfollow_protection(pInstance,TRUE, TRUE, TRUE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8, 11, TRUE, TRUE, 0, 0, 0, E_XC_OP1_PATTERN_TYPE_DOTFADE);
    }

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);

}

void MDrv_SC_OP1_Calibratoin_Color_Dotfade_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        //dotfade---------------------------------------------------------------------------------
        MHal_SC_OP1_Pattern_set_dotfade_value(pInstance,236, 236, 236);
        MHal_SC_OP1_Pattern_set_dotfade_overfollow_protection(pInstance,TRUE, TRUE, TRUE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,1, TRUE, TRUE, 1023, 1023, 1023, E_XC_OP1_PATTERN_TYPE_DOTFADE);

        //entry8: black
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry7: blue
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry6: red
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry5: pink
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x00,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry4: green
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x3FF,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry3: (0,255,255)
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry2: yellow
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry1: white
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);

        //
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
    }
    else
    {
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00, 0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x3FF, 0x3FF, 0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);

        //entry1: white
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry2: yellow
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x3FF,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry3: (0,255,255)
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry4: green
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x3FF,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry5: pink
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x00,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry6: red
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x3FF, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry7: blue
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0x00, 0x00,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        //entry8: black
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        //dotfade---------------------------------------------------------------------------------
        MHal_SC_OP1_Pattern_set_dotfade_value(pInstance,18, 18, 18);
        MHal_SC_OP1_Pattern_set_dotfade_overfollow_protection(pInstance,TRUE, TRUE, TRUE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,8,1, TRUE, TRUE, 0, 0, 0, E_XC_OP1_PATTERN_TYPE_DOTFADE);
    }

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);

}

void MDrv_SC_OP1_Set_Border_Type(MS_U8 *u8GetType, MS_U8 u8Type1, MS_U8 u8Type2, MS_U8 u8Type3, MS_U8 u8Type4, MS_U8 u8Type5)
{
    u8GetType[0] = u8Type1;
    u8GetType[1] = u8Type2;
    u8GetType[2] = u8Type3;
    u8GetType[3] = u8Type4;
    u8GetType[4] = u8Type5;
}

XC_OP1_PatGen_Border MDrv_SC_OP1_Set_Border_Val(MS_U8 u8BoldValType, MS_U8 u8ValType)
{
    XC_OP1_PatGen_Border tile;
    memset(&tile , 0 , sizeof(XC_OP1_PatGen_Border));

    switch(u8BoldValType)
    {
            case 0:
                tile.u8TopBold = 1;
                tile.u8BottomBold =1;
                tile.u8LeftBold =1;
                tile.u8RightBold =1;
                break;
            case 1:
                tile.u8TopBold = 1;
                tile.u8BottomBold =1;
                tile.u8LeftBold =3;
                tile.u8RightBold =1;

                break;
            case 2:
                tile.u8TopBold = 3;
                tile.u8BottomBold =1;
                tile.u8LeftBold =1;
                tile.u8RightBold =1;

                break;
            case 3:
                tile.u8TopBold = 1;
                tile.u8BottomBold =1;
                tile.u8LeftBold =1;
                tile.u8RightBold =3;

                break;
            case 4:
                tile.u8TopBold = 1;
                tile.u8BottomBold =3;
                tile.u8LeftBold =1;
                tile.u8RightBold =1;

                break;
            case 5:
                tile.u8TopBold = 3;
                tile.u8BottomBold =1;
                tile.u8LeftBold =3;
                tile.u8RightBold =1;

                break;
            case 6:
                tile.u8TopBold = 3;
                tile.u8BottomBold =1;
                tile.u8LeftBold =1;
                tile.u8RightBold =3;

                break;
            case 7:
                tile.u8TopBold = 1;
                tile.u8BottomBold =3;
                tile.u8LeftBold =1;
                tile.u8RightBold =3;

                break;
            case 8:
                tile.u8TopBold = 1;
                tile.u8BottomBold =3;
                tile.u8LeftBold =3;
                tile.u8RightBold =1;

                break;
            case 9:
                tile.u8TopBold = 3;
                tile.u8BottomBold =3;
                tile.u8LeftBold =1;
                tile.u8RightBold =1;

                break;
            case 10:
                tile.u8TopBold = 1;
                tile.u8BottomBold =1;
                tile.u8LeftBold =3;
                tile.u8RightBold =3;

                break;
            case 11:
                tile.u8TopBold = 3;
                tile.u8BottomBold =1;
                tile.u8LeftBold =3;
                tile.u8RightBold =3;

            case 12:
                tile.u8TopBold = 3;
                tile.u8BottomBold =3;
                tile.u8LeftBold =1;
                tile.u8RightBold =3;

                break;
            case 13:
                tile.u8TopBold = 1;
                tile.u8BottomBold =3;
                tile.u8LeftBold =3;
                tile.u8RightBold =3;

                break;
            case 14:
                tile.u8TopBold = 3;
                tile.u8BottomBold =3;
                tile.u8LeftBold =3;
                tile.u8RightBold =1;

                break;

            case 15:
                tile.u8TopBold = 3;
                tile.u8BottomBold =3;
                tile.u8LeftBold =3;
                tile.u8RightBold =3;

                break;

            default:
                tile.u8TopBold = 1;
                tile.u8BottomBold =1;
                tile.u8LeftBold =1;
                tile.u8RightBold =1;
                break;
    }

    switch(u8ValType)
    {
            case 0:
                tile.bTop = 0;
                tile.bBottom =0;
                tile.bLeft =0;
                tile.bRight =0;
                break;
            case 1:
                tile.bTop = 0;
                tile.bBottom =0;
                tile.bLeft =1;
                tile.bRight =0;
                break;
            case 2:
                tile.bTop = 1;
                tile.bBottom =0;
                tile.bLeft =0;
                tile.bRight =0;
                break;
            case 3:
                tile.bTop = 0;
                tile.bBottom =0;
                tile.bLeft =0;
                tile.bRight =1;
                break;
            case 4:
                tile.bTop = 0;
                tile.bBottom =1;
                tile.bLeft =0;
                tile.bRight =0;
                break;
            case 5:
                tile.bTop = 1;
                tile.bBottom =0;
                tile.bLeft =1;
                tile.bRight =0;
                break;
            case 6:
                tile.bTop = 1;
                tile.bBottom =0;
                tile.bLeft =0;
                tile.bRight =1;
                break;
            case 7:
                tile.bTop = 0;
                tile.bBottom =1;
                tile.bLeft =0;
                tile.bRight =1;
                break;
            case 8:
                tile.bTop = 0;
                tile.bBottom =1;
                tile.bLeft =1;
                tile.bRight =0;
                break;
            case 9:
                tile.bTop = 1;
                tile.bBottom =1;
                tile.bLeft =0;
                tile.bRight =0;
                break;
            case 10:
                tile.bTop = 0;
                tile.bBottom =0;
                tile.bLeft =1;
                tile.bRight =1;
                break;
            case 11:
                tile.bTop = 1;
                tile.bBottom =0;
                tile.bLeft =1;
                tile.bRight =1;
                break;
            case 12:
                tile.bTop = 1;
                tile.bBottom =1;
                tile.bLeft =0;
                tile.bRight =1;
                break;
            case 13:
                tile.bTop = 0;
                tile.bBottom =1;
                tile.bLeft =1;
                tile.bRight =1;
                break;
            case 14:
                tile.bTop = 1;
                tile.bBottom =1;
                tile.bLeft =1;
                tile.bRight =0;
                break;

            case 15:
                tile.bTop = 1;
                tile.bBottom =1;
                tile.bLeft =1;
                tile.bRight =1;
                break;

            default:
                tile.bTop = 0;
                tile.bBottom =0;
                tile.bLeft =0;
                tile.bRight =0;
                break;
    }

    return tile;
}

void MDrv_SC_OP1_Set_Border_Sram(void *pInstance,MS_U8 *u8BoldValType, MS_U8 *u8ValType)
{
    XC_OP1_PatGen_Border tile1,tile2,tile3,tile4,tile5;

    tile1 = MDrv_SC_OP1_Set_Border_Val(u8BoldValType[0], u8ValType[0]);
    tile2 = MDrv_SC_OP1_Set_Border_Val(u8BoldValType[1], u8ValType[1]);
    tile3 = MDrv_SC_OP1_Set_Border_Val(u8BoldValType[2], u8ValType[2]);
    tile4 = MDrv_SC_OP1_Set_Border_Val(u8BoldValType[3], u8ValType[3]);
    tile5 = MDrv_SC_OP1_Set_Border_Val(u8BoldValType[4], u8ValType[4]);

    MHal_SC_OP1_Pattern_set_border_sram(pInstance,tile1, tile2, tile3, tile4, tile5);
}

void MDrv_SC_OP1_Set_Pattern2_Border(void *pInstance)
{
    MS_U8 u8BoldValType[5] = {0};
    MS_U8 u8ValType[5] = {0};
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MHal_SC_OP1_Pattern_enable_border(pInstance,TRUE,0x3FF,0x3FF,0x3FF);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    { //mirror mode

     //0~50
     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     //51~100
     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,5);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,5,2,2);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,2,2,2);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,5,2,6);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     //101~150
     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     //151~200
     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,3,8,4,7,8);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,4,4,4);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,4,7,8);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,4,7,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     //201~250
     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,5);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,5,2,2);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,2,2,2);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,5,2,6);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,7,1,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     //251~300
     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,3,5,2,6,1);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,8,4);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,7,1,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     //301~350
     MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,5);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,1,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,7,8,4);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,4,4,4);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,7,8,4);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,7,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     //351~400
     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     //401~432
     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,11,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

     MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,0,0,0);
     MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);
    }
    else
    {
        //0~50
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        //51~100
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,5,2,6,5);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        //101~150
        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,2,2,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,2,6,5);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,7,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,5,2,6,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        //151~200
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,8,4);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,7,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,5);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,8,4,7);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,4,4,4);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        //201~250
        MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,4,4,7);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,7,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,5,2,6,5,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,2,2,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        //251~300
        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,6,5,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,6,1,0,3,1);// 1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);// 2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);// 3
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        //301~350
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);// 4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);// 5
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);// 6
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        //351~400
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,8,4,7);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,4,4,4);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,4,4,7);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,7,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,15,15,15,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        //401~432
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    }
}

void MDrv_SC_OP1_1_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width < 1900)
        && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height < 1000))
    {
        // For special pattern!!!!  HD/FHD use different tile cnt !!!!
        u16HTileCnt = 20;
        u16VTileCnt = 18;
    }

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > 1900)
        && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > 1000))
    {
        // FHD case
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,48,15,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,48,15,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,48,15,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,48,15,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,10,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,10,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,10,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,10,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,10,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,10,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,11,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,9,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,9,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,11,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,37,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,37,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,11,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,9,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,9,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,11,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,37,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,37,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,11,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,9,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,9,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,11,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,37,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,37,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,37,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,11,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,9,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,6,11,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,9,11,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,11,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);


        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,10,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,10,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,10,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,10,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,42,10,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,10,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,48,15,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,48,15,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,48,15,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,48,15,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

    }
    else
    {
        // HD case
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x14,0x00,TRUE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x12,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x02,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x12,0x02,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x02,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x03,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x12,0x03,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x03,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x02,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x12,0x02,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x02,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x04,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x12,0x00,FALSE,FALSE,0x3FF, 0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x14,0x00,TRUE,TRUE,0x00, 0x00,0x00,E_XC_OP1_PATTERN_TYPE_PURE);
    }

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);

}

void MDrv_SC_OP1_2_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        //Fade: 3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,TRUE,FALSE,((255)<<2),((255)<<2),((255)<<2),E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((255)<<2),((255)<<2),((255)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((242)<<2),((238)<<2),((239)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((242)<<2),((238)<<2),((239)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((226)<<2),((226)<<2),((226)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((226)<<2),((226)<<2),((226)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((217)<<2),((213)<<2),((214)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((217)<<2),((213)<<2),((214)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((79)<<2),((79)<<2),((79)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((79)<<2),((79)<<2),((79)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((56)<<2),((56)<<2),((56)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((56)<<2),((56)<<2),((56)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((37)<<2),((37)<<2),((37)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((37)<<2),((37)<<2),((37)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,TRUE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        //Fade:  2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,(239)<<2, (88)<<2,(204)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(233)<<2, (89)<<2,(239)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(238)<<2, (73)<<2,(248)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(249)<<2, (52)<<2,(254)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(253)<<2, (2)<<2,(253)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(0)<<2, (219)<<2,(223)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(79)<<2, (209)<<2,(211)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(82)<<2, (223)<<2,(230)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(67)<<2, (229)<<2,(231)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(44)<<2, (240)<<2,(238)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(0)<<2, (255)<<2,(253)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(221)<<2, (254)<<2,(22)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(223)<<2, (252)<<2,(76)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(255)<<2, (249)<<2,(80)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(254)<<2, (248)<<2,(48)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,(255)<<2, (255)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        //Fade: 1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,(255)<<2, (255)<<2,(255)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(247)<<2, (247)<<2,(247)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(229)<<2, (229)<<2,(229)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(211)<<2, (211)<<2,(211)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(194)<<2, (194)<<2,(194)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(181)<<2, (181)<<2,(181)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(160)<<2, (160)<<2,(160)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(145)<<2, (145)<<2,(145)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(128)<<2, (128)<<2,(128)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(112)<<2, (112)<<2,(112)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(97)<<2, (97)<<2,(97)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(80)<<2, (80)<<2,(80)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(64)<<2, (64)<<2,(64)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(49)<<2, (49)<<2,(49)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(19)<<2, (15)<<2,(12)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        //rigth pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x03,0x07,TRUE,FALSE,0x13c, 0x28,0x1d5, E_XC_OP1_PATTERN_TYPE_PURE);

        //middle pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x0a,0x07,FALSE,FALSE,0x161, 0x161,0x161, E_XC_OP1_PATTERN_TYPE_PURE);

        //left pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x03,0x07,FALSE,TRUE,0x00, 0x1c9,0x2f2, E_XC_OP1_PATTERN_TYPE_PURE);


        //pure gray 55
        int i=0;
        for(i=0;i<16;i++)
        {
            if(i==0){
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,TRUE,FALSE,0xDC, 0xDC,0xDC,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
            }else if (i==15)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,TRUE,0xDC, 0xDC,0xDC,E_XC_OP1_PATTERN_TYPE_PURE);
            }else
                {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,0xDC, 0xDC,0xDC,E_XC_OP1_PATTERN_TYPE_PURE);
            }
        }
         //item:3
         //LineDot: right
         MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,TRUE,FALSE,0x470,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);
        //end of linedot item 3-----------------------------------------------------------------------------


        //Pure between LineDot itme3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,1,FALSE,FALSE,0x336, 0x336,0x336, E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16


         //linedot item3--------------------------------------------------------------------------------------
         //LineDot : left

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,TRUE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //itme2
        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,2,TRUE,FALSE,0x570,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot itme2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,2,FALSE,FALSE,0x336, 0x336,0x336, E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

         //linedot item2--------------------------------------------------------------------------------------
         //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,2,FALSE,TRUE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //itme1:
        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,TRUE,FALSE,0x1F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot item1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,1,FALSE,FALSE,0x336, 0x336,0x336, E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

         //linedot item1--------------------------------------------------------------------------------------
         //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,TRUE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,(255)<<2, (255)<<2,(255)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(241)<<2, (241)<<2,(241)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(224)<<2, (224)<<2,(224)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(205)<<2, (205)<<2,(205)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(187)<<2, (187)<<2,(187)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(172)<<2, (172)<<2,(172)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(157)<<2, (157)<<2,(157)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(139)<<2, (139)<<2,(139)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(121)<<2, (121)<<2,(121)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(106)<<2, (106)<<2,(106)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(88)<<2, (88)<<2,(88)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(71)<<2, (71)<<2,(71)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(55)<<2, (55)<<2,(55)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(37)<<2, (37)<<2,(37)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(19)<<2, (19)<<2,(19)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,TRUE,FALSE,(0)<<2, (0)<<2,(254)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (1)<<2,(239)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (1)<<2,(223)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (0)<<2,(202)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (1)<<2,(187)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (0)<<2,(170)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (2)<<2,(154)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(2)<<2, (0)<<2,(136)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (0)<<2,(120)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (2)<<2,(105)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(2)<<2, (0)<<2,(84)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (0)<<2,(72)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(3)<<2, (0)<<2,(57)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (1)<<2,(32)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (0)<<2,(20)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,TRUE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,(254)<<2, (0)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(240)<<2, (1)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(224)<<2, (0)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(203)<<2, (0)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(185)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(171)<<2, (1)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(153)<<2, (1)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(136)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(121)<<2, (1)<<2,(3)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(105)<<2, (0)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(85)<<2, (1)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(72)<<2, (0)<<2,(3)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(56)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(36)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(21)<<2, (1)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,TRUE,FALSE,((2)<<2),((255)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((0)<<2),((242)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((3)<<2),((225)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((3)<<2),((204)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((3)<<2),((186)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((171)<<2),((4)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((156)<<2),((3)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((2)<<2),((138)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((120)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((0)<<2),((105)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((2)<<2),((87)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((2)<<2),((72)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((0)<<2),((55)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((36)<<2),((3)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((0)<<2),((19)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,TRUE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
    }
    else
    {
        //Fade: Green
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,TRUE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((0)<<2),((19)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((36)<<2),((3)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((0)<<2),((55)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((2)<<2),((72)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((2)<<2),((87)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((0)<<2),((105)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((120)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((2)<<2),((138)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((156)<<2),((3)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((171)<<2),((4)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((3)<<2),((186)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((3)<<2),((204)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((3)<<2),((225)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((0)<<2),((242)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,TRUE,((2)<<2),((255)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16


        //Fade: Red
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(21)<<2, (1)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(36)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(56)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(72)<<2, (0)<<2,(3)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(85)<<2, (1)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(105)<<2, (0)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(121)<<2, (1)<<2,(3)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(136)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(153)<<2, (1)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(171)<<2, (1)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(185)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(203)<<2, (0)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(224)<<2, (0)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(240)<<2, (1)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,(254)<<2, (0)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //Fade: Blue
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,TRUE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (0)<<2,(20)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (1)<<2,(32)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(3)<<2, (0)<<2,(57)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (0)<<2,(72)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(2)<<2, (0)<<2,(84)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (2)<<2,(105)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (0)<<2,(120)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(2)<<2, (0)<<2,(136)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (2)<<2,(154)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(0)<<2, (0)<<2,(170)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (1)<<2,(187)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (0)<<2,(202)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (1)<<2,(223)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,(1)<<2, (1)<<2,(239)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,TRUE,(0)<<2, (0)<<2,(254)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16


        //Fade:  White
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(19)<<2, (19)<<2,(19)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(37)<<2, (37)<<2,(37)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(55)<<2, (55)<<2,(55)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(71)<<2, (71)<<2,(71)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(88)<<2, (88)<<2,(88)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(106)<<2, (106)<<2,(106)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(121)<<2, (121)<<2,(121)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(139)<<2, (139)<<2,(139)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(157)<<2, (157)<<2,(157)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(172)<<2, (172)<<2,(172)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(187)<<2, (187)<<2,(187)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(205)<<2, (205)<<2,(205)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(224)<<2, (224)<<2,(224)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(241)<<2, (241)<<2,(241)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,(255)<<2, (255)<<2,(255)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //linedot item1--------------------------------------------------------------------------------------
        //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,TRUE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,1,FALSE,FALSE,0x336, 0x336,0x336, E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,TRUE,0x0F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

         //linedot item2--------------------------------------------------------------------------------------
         //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,2,TRUE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

         //Pure between LineDot
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,2,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

         //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,2,FALSE,TRUE,0x570,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

         //linedot item3--------------------------------------------------------------------------------------
         //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,TRUE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,1,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

         //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,TRUE,0x470,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //pure gray 55
        int i=0;
        for(i=0;i<16;i++)
        {
            if(i==0){
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,TRUE,FALSE,0xDC, 0xDC,0xDC, E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
            }else if (i==15)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,TRUE,0xDC, 0xDC,0xDC, E_XC_OP1_PATTERN_TYPE_PURE);
            }else
                {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,0xDC, 0xDC,0xDC, E_XC_OP1_PATTERN_TYPE_PURE);
            }
        }

        //left pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x03,0x07,TRUE,FALSE,0x00, 0x1c9,0x2f2,E_XC_OP1_PATTERN_TYPE_PURE);
        //middle pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x0a,0x07,FALSE,FALSE,0x161, 0x161,0x161,E_XC_OP1_PATTERN_TYPE_PURE);
        //rigth pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x03,0x07,FALSE,TRUE,0x13c, 0x28,0x1d5,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: 1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(19)<<2, (15)<<2,(12)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(49)<<2, (49)<<2,(49)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(64)<<2, (64)<<2,(64)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(80)<<2, (80)<<2,(80)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(97)<<2, (97)<<2,(97)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(112)<<2, (112)<<2,(112)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(128)<<2, (128)<<2,(128)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(145)<<2, (145)<<2,(145)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(160)<<2, (160)<<2,(160)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(181)<<2, (181)<<2,(181)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(194)<<2, (194)<<2,(194)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(211)<<2, (211)<<2,(211)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(229)<<2, (229)<<2,(229)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(247)<<2, (247)<<2,(247)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,(255)<<2, (255)<<2,(255)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16


        //Fade:  2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,TRUE,FALSE,(255)<<2, (255)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(254)<<2, (248)<<2,(48)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(255)<<2, (249)<<2,(80)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(223)<<2, (252)<<2,(76)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(221)<<2, (254)<<2,(22)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(0)<<2, (255)<<2,(253)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(44)<<2, (240)<<2,(238)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(67)<<2, (229)<<2,(231)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(82)<<2, (223)<<2,(230)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(79)<<2, (209)<<2,(211)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(0)<<2, (219)<<2,(223)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(253)<<2, (2)<<2,(253)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(249)<<2, (52)<<2,(254)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(238)<<2, (73)<<2,(248)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,FALSE,(233)<<2, (89)<<2,(239)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x00,FALSE,TRUE,(239)<<2, (88)<<2,(204)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //Fade: 3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,TRUE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((37)<<2),((37)<<2),((37)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((37)<<2),((37)<<2),((37)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((56)<<2),((56)<<2),((56)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((56)<<2),((56)<<2),((56)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((79)<<2),((79)<<2),((79)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((79)<<2),((79)<<2),((79)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((217)<<2),((213)<<2),((214)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((217)<<2),((213)<<2),((214)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((226)<<2),((226)<<2),((226)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((226)<<2),((226)<<2),((226)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((242)<<2),((238)<<2),((239)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((242)<<2),((238)<<2),((239)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,FALSE,((255)<<2),((255)<<2),((255)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,0x01,0x01,FALSE,TRUE,((255)<<2),((255)<<2),((255)<<2),E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

    }

    MHal_SC_OP1_Pattern_sram_write_enable(pInstance,FALSE);

    MHal_SC_OP1_Pattern_sram_write_fastmode(pInstance,FALSE);

    MHal_SC_OP1_Pattern_set_wraddr(pInstance,0x00);

    //reg_sram_jump_thr
    MHal_SC_OP1_Pattern_set_jump_threshold(pInstance,0x00);

    MHal_SC_OP1_Pattern_sram_write_enable(pInstance,TRUE);

    MHal_SC_OP1_Pattern_sram_write_fastmode(pInstance,TRUE);

    //Write the border command to second SRAM
    MDrv_SC_OP1_Set_Pattern2_Border(pInstance);

    MHal_SC_OP1_Pattern_sram_write_enable(pInstance,FALSE);

    MHal_SC_OP1_Pattern_sram_write_fastmode(pInstance,FALSE);

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);

}

void MDrv_SC_OP1_Set_Pattern3_Border(void* pInstance)
{
    MS_U8 u8BoldValType[5] = {0};
    MS_U8 u8ValType[5] = {0};
    MS_U16 i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MHal_SC_OP1_Pattern_enable_border(pInstance,TRUE,0x3FF,0x3FF,0x3FF);

    for(i=0;i< ((((24*17))/5));i++)   //tile1~ tile405
    {
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);
    }

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    //tile411 ~ 415
    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,3,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);
    //row1 : starts at 437

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,1,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,1);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,1,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,3,11,11,11,11); //row2
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,1,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,11,11); //row 3
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,1);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,1,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,3,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,1,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    ///-----
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,5,2,6,5,2); //pure block -1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,2,2,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,6,5,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,6,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);//pure block -2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);//pure block -3

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3); //pure block -4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1); //pure block -5
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);//pure block -6
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);//pure block -7
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);//pure block -8

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);//pure block -9
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);//pure block -10
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0); //pure block -11
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1); //pure block -12
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType); //pure block -13

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);//pure block -14
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);//pure block -15
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);//pure block -16
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);//pure block -17
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);//pure block -18

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);//pure block -19
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);//pure block -20
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,7,8,4);//pure block -21
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,4,4,4);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,7,8,4);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,7,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,11);//middle pure - 1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,15,15,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,10,10);//middle pure - 2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,10,10,10);//middle pure - 3
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,10,10,10,10);//middle pure - 4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13); //middle pure - 5
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,5); // linedot1-1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,5,2,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,15,15,2,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,5,2,6);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);// linedot1-2

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);// linedot1-3
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1); // linedot1-4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0); // linedot1-5
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);// linedot1-6
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,8,4);// linedot1-7
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,7,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,5,2,6);// linedot2-1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1); // linedot2-2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);// linedot2-3
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);// linedot2-4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);// linedot2-5

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);// linedot2-6
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,8,4,7,1);// linedot2-7
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,5,2,6,1,0);// linedot3-1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);// linedot3-2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);// linedot3-3

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);// linedot3-4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);// linedot3-5
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);// linedot3-6
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,8);// linedot3-7
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,4,7,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,11,11);// bottom row
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);
    }
    else
    {
        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11); //row4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,5,2,6); // linedot1-1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,5,2,2,2,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,2,2,6);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,5,2,6,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1); // linedot1-2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);// linedot1-3
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);// linedot1-4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);// linedot1-5

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);// linedot1-6
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);// linedot1-7
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,8);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,4,7,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0); //linedot2-1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,5,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,6,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);//linedot2-2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);//linedot2-3

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);//linedot2-4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);//linedot2-5
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);//linedot2-6
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);//linedot2-7
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,8,4,7);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);//linedot3-1

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,5,2,6,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);//linedot3-2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);//linedot3-3
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);//linedot3-4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);//linedot3-5
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);//linedot3-6

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,8,4,7);//linedot3-7
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,4,4,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,4,4,4,7);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,8,4,7,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,11,11,11,11);//middle pure - 1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10); //middle pure - 2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,10);//middle pure - 3
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,10,10);//middle pure - 4
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,13,13,13);//middle pure - 5
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,15);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,15,13,13,13,13);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,5,2,6,5); // pure block - 1
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,2,2,2);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,2,2,6,5);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,2,6,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0); // pure block - 2
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);// pure block - 3
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);// pure block - 4

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);// pure block - 5
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1); // pure block - 6
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);// pure block - 7
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);// pure block - 9

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);// pure block - 10
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);// pure block - 11
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);// pure block - 12
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);// pure block - 13
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);// pure block - 14

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);// pure block - 15
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);// pure block - 16
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);// pure block - 17
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);// pure block - 18
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);// pure block - 19

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,1,0,3,1);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,1,0,3);// pure block - 20
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,3,1,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,3,8,4,7,8);// pure block - 21
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,4,4,4);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8ValType,4,4,4,7,8);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,4,7,1,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

        MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    }
    ///------

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);// bottom row 1 -1
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,10); // bottom row 1 -2
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,13,13);// bottom row 1 -3
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,1);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,11,11,11); // bottom row 2 -1
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,1,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,3,10,10,10,10);// bottom row 2 -2
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,1,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);// bottom row 2 -3
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,13,13,13,13);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,11);// bottom row 3 -1
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,11,11,11,11,11);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,3,10,10);// bottom row 3 -2
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,1);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,3,10,10,10);// bottom row 3 -3
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,3,1,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,1,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,3,10,10,10,10);// bottom row 3 -4
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,3,1,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,1,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);// bottom row 3 -5
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,10,10,10,10,10);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,3,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,13,1,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,3,1);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,3);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    MDrv_SC_OP1_Set_Border_Type(u8BoldValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Type(u8ValType,1,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

    for(i=0;i<(412/5)-1;i++)
    {
        MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
        MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);
    }

    MDrv_SC_OP1_Set_Border_Type(u8ValType,0,0,0,0,0);
    MDrv_SC_OP1_Set_Border_Sram(pInstance,u8BoldValType, u8ValType);

}

void MDrv_SC_OP1_3_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {

        //bottom black border
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,16,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,16,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,16,0,FALSE,FALSE,0x3FF,0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: 3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((255)<<2),((255)<<2),((255)<<2),E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((255)<<2),((255)<<2),((255)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((242)<<2),((238)<<2),((239)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((242)<<2),((238)<<2),((239)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((226)<<2),((226)<<2),((226)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((226)<<2),((226)<<2),((226)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((217)<<2),((213)<<2),((214)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((217)<<2),((213)<<2),((214)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((79)<<2),((79)<<2),((79)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((79)<<2),((79)<<2),((79)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((56)<<2),((56)<<2),((56)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((56)<<2),((56)<<2),((56)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((37)<<2),((37)<<2),((37)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((37)<<2),((37)<<2),((37)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        //left black border - no.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade:  2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(239)<<2, (88)<<2,(204)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(233)<<2, (89)<<2,(239)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(238)<<2, (73)<<2,(248)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(249)<<2, (52)<<2,(254)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(253)<<2, (2)<<2,(253)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (219)<<2,(223)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(79)<<2, (209)<<2,(211)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(82)<<2, (223)<<2,(230)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(67)<<2, (229)<<2,(231)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(44)<<2, (240)<<2,(238)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (255)<<2,(253)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(221)<<2, (254)<<2,(22)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(223)<<2, (252)<<2,(76)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(255)<<2, (249)<<2,(80)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(254)<<2, (248)<<2,(48)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(255)<<2, (255)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

         //left black border - no.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: 1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(255)<<2, (255)<<2,(255)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(247)<<2, (247)<<2,(247)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(229)<<2, (229)<<2,(229)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(211)<<2, (211)<<2,(211)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(194)<<2, (194)<<2,(194)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(181)<<2, (181)<<2,(181)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(160)<<2, (160)<<2,(160)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(145)<<2, (145)<<2,(145)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(128)<<2, (128)<<2,(128)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(112)<<2, (112)<<2,(112)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(97)<<2, (97)<<2,(97)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(80)<<2, (80)<<2,(80)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(64)<<2, (64)<<2,(64)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(49)<<2, (49)<<2,(49)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(19)<<2, (15)<<2,(12)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

         //left black border - no.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,20,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,20,FALSE,FALSE,0x13c, 0x28,0x1d5,E_XC_OP1_PATTERN_TYPE_PURE);

        //middle pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,20,FALSE,FALSE,0x161, 0x161,0x161,E_XC_OP1_PATTERN_TYPE_PURE);

        //left pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,20,FALSE,FALSE,0x00, 0x1c9,0x2f2,E_XC_OP1_PATTERN_TYPE_PURE);

         //left black border - no.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,20,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //pure gray 55
        int i=0;
        for(i=0;i<16;i++)
        {
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,0xDC, 0xDC,0xDC,E_XC_OP1_PATTERN_TYPE_PURE);
        }

        //left black border - no.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border -
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //item:4
        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,0,FALSE,FALSE,0x470,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);
        //end of linedot item 4-----------------------------------------------------------------------------


        //Pure between LineDot itme4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,0xDC, 0xDC,0xDC,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,0xDC, 0xDC,0xDC,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);

         //linedot item4--------------------------------------------------------------------------------------
         //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,0,FALSE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //left black border -
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,5,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //item:3
        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,5,FALSE,FALSE,0x470,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);
        //end of linedot item 3-----------------------------------------------------------------------------


        //Pure between LineDot itme3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,5,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16


        //linedot item3--------------------------------------------------------------------------------------
        //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,5,FALSE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //left black border - no.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,5,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);


        //rigth black border - no.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,6,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //itme2
        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,6,FALSE,FALSE,0x570,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot itme2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,6,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //linedot item2--------------------------------------------------------------------------------------
        //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,6,FALSE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //left black border - no.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,6,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.17
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,6,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //itme1:
        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,6,FALSE,FALSE,0x1F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot item1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,6,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //linedot item1--------------------------------------------------------------------------------------
        //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,6,FALSE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //left black border - no.17
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,6,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.18
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade:  White
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(255)<<2, (255)<<2,(255)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(241)<<2, (241)<<2,(241)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(224)<<2, (224)<<2,(224)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(205)<<2, (205)<<2,(205)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(187)<<2, (187)<<2,(187)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(172)<<2, (172)<<2,(172)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(157)<<2, (157)<<2,(157)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(139)<<2, (139)<<2,(139)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(121)<<2, (121)<<2,(121)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(106)<<2, (106)<<2,(106)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(88)<<2, (88)<<2,(88)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(71)<<2, (71)<<2,(71)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(55)<<2, (55)<<2,(55)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(37)<<2, (37)<<2,(37)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(19)<<2, (19)<<2,(19)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        //left black border - no.18
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.19
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: Blue
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(254)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(239)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(223)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (0)<<2,(202)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(187)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(170)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (2)<<2,(154)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(2)<<2, (0)<<2,(136)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(120)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (2)<<2,(105)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(2)<<2, (0)<<2,(84)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(72)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(3)<<2, (0)<<2,(57)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (1)<<2,(32)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(20)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        //left black border - no.19
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.20
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: Red
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(254)<<2, (0)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(240)<<2, (1)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(224)<<2, (0)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(203)<<2, (0)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(185)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(171)<<2, (1)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(153)<<2, (1)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(136)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(121)<<2, (1)<<2,(3)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(105)<<2, (0)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(85)<<2, (1)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(72)<<2, (0)<<2,(3)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(56)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(36)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(21)<<2, (1)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        //left black border - no.20
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.21
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: Green
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((2)<<2),((255)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((0)<<2),((242)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((3)<<2),((225)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((3)<<2),((204)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((3)<<2),((186)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((171)<<2),((4)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((156)<<2),((3)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((2)<<2),((138)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((120)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((0)<<2),((105)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((2)<<2),((87)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((2)<<2),((72)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((0)<<2),((55)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((36)<<2),((3)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((0)<<2),((19)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1

        //left black border - no.21
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,16,0,FALSE,FALSE,0x3FF,0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //top black border
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,16,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,16,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
    }
    else
    {
        //top black border
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,16,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,16,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //top white border
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,16,0,FALSE,FALSE,0x3FF,0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: Green
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((0)<<2),((19)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((36)<<2),((3)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((0)<<2),((55)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((2)<<2),((72)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((2)<<2),((87)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((0)<<2),((105)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((120)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((2)<<2),((138)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((156)<<2),((3)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((171)<<2),((4)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((3)<<2),((186)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((3)<<2),((204)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((3)<<2),((225)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((0)<<2),((242)<<2),((0)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((2)<<2),((255)<<2),((2)<<2),E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //rigth black border - no.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: Red
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(21)<<2, (1)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(36)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(56)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(72)<<2, (0)<<2,(3)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(85)<<2, (1)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(105)<<2, (0)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(121)<<2, (1)<<2,(3)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(136)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(153)<<2, (1)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(171)<<2, (1)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(185)<<2, (0)<<2,(0)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(203)<<2, (0)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(224)<<2, (0)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(240)<<2, (1)<<2,(4)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(254)<<2, (0)<<2,(2)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //rigth black border - no.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: Blue
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(20)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (1)<<2,(32)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(3)<<2, (0)<<2,(57)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(72)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(2)<<2, (0)<<2,(84)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (2)<<2,(105)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(120)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(2)<<2, (0)<<2,(136)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (2)<<2,(154)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(170)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(187)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (0)<<2,(202)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(223)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(239)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (0)<<2,(254)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //rigth black border - no.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade:  White
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(19)<<2, (19)<<2,(19)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(37)<<2, (37)<<2,(37)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(55)<<2, (55)<<2,(55)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(71)<<2, (71)<<2,(71)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(88)<<2, (88)<<2,(88)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(106)<<2, (106)<<2,(106)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(121)<<2, (121)<<2,(121)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(139)<<2, (139)<<2,(139)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(157)<<2, (157)<<2,(157)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(172)<<2, (172)<<2,(172)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(187)<<2, (187)<<2,(187)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(205)<<2, (205)<<2,(205)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(224)<<2, (224)<<2,(224)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(241)<<2, (241)<<2,(241)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(255)<<2, (255)<<2,(255)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //rigth black border - no.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,6,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //linedot item1--------------------------------------------------------------------------------------
        //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,6,FALSE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,6,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,6,FALSE,FALSE,0x0F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //rigth black border - no.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,6,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,6,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //linedot item2--------------------------------------------------------------------------------------
        //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,6,FALSE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,6,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,6,FALSE,FALSE,0x570,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //rigth black border - no.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,6,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,5,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //linedot item3--------------------------------------------------------------------------------------
        //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,5,FALSE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,5,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,5,FALSE,FALSE,0x470,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);
        //end of linedot item1~3-----------------------------------------------------------------------------

        //rigth black border - no.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,5,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);


        //left black border -
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //linedot item4--------------------------------------------------------------------------------------
        //LineDot : left
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,0,FALSE,FALSE,0x7F0,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);

        //Pure between LineDot
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,0xDC, 0xDC,0xDC,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,0,FALSE,FALSE,0xDC, 0xDC,0xDC,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,FALSE,0x336, 0x336,0x336,E_XC_OP1_PATTERN_TYPE_PURE);


        //LineDot: right
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,0,FALSE,FALSE,0x470,0x03,0x3FF,E_XC_OP1_PATTERN_TYPE_LINEDOT);
        //end of linedot item 4-----------------------------------------------------------------------------

        //rigth black border -
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //pure gray 55
        int i=0;
        for(i=0;i<16;i++)
        {
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,0xDC, 0xDC,0xDC,E_XC_OP1_PATTERN_TYPE_PURE);
        }

        //rigth black border - no.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,20,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,20,FALSE,FALSE,0x00, 0x1c9,0x2f2,E_XC_OP1_PATTERN_TYPE_PURE);
        //middle pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,10,20,FALSE,FALSE,0x161, 0x161,0x161,E_XC_OP1_PATTERN_TYPE_PURE);
        //rigth pure block
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,20,FALSE,FALSE,0x13c, 0x28,0x1d5,E_XC_OP1_PATTERN_TYPE_PURE);

        //rigth black border - no.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,20,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: 1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(1)<<2, (1)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(19)<<2, (15)<<2,(12)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(49)<<2, (49)<<2,(49)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(64)<<2, (64)<<2,(64)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(80)<<2, (80)<<2,(80)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(97)<<2, (97)<<2,(97)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(112)<<2, (112)<<2,(112)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(128)<<2, (128)<<2,(128)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(145)<<2, (145)<<2,(145)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(160)<<2, (160)<<2,(160)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(181)<<2, (181)<<2,(181)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(194)<<2, (194)<<2,(194)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(211)<<2, (211)<<2,(211)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(229)<<2, (229)<<2,(229)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(247)<<2, (247)<<2,(247)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(255)<<2, (255)<<2,(255)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //rigth black border - no.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade:  2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(255)<<2, (255)<<2,(1)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(254)<<2, (248)<<2,(48)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(255)<<2, (249)<<2,(80)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(223)<<2, (252)<<2,(76)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(221)<<2, (254)<<2,(22)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (255)<<2,(253)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(44)<<2, (240)<<2,(238)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(67)<<2, (229)<<2,(231)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(82)<<2, (223)<<2,(230)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(79)<<2, (209)<<2,(211)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(0)<<2, (219)<<2,(223)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(253)<<2, (2)<<2,(253)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(249)<<2, (52)<<2,(254)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(238)<<2, (73)<<2,(248)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(233)<<2, (89)<<2,(239)<<2,E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,(239)<<2, (88)<<2,(204)<<2,E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //rigth black border - no.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,2,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //left black border - no.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        //Fade: 3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((1)<<2),((1)<<2),((1)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((37)<<2),((37)<<2),((37)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((37)<<2),((37)<<2),((37)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((56)<<2),((56)<<2),((56)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((56)<<2),((56)<<2),((56)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((79)<<2),((79)<<2),((79)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((79)<<2),((79)<<2),((79)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((217)<<2),((213)<<2),((214)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((217)<<2),((213)<<2),((214)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((226)<<2),((226)<<2),((226)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((226)<<2),((226)<<2),((226)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((242)<<2),((238)<<2),((239)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((242)<<2),((238)<<2),((239)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((255)<<2),((255)<<2),((255)<<2),E_XC_OP1_PATTERN_TYPE_PURE);   //NO.15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,4,FALSE,FALSE,((255)<<2),((255)<<2),((255)<<2),E_XC_OP1_PATTERN_TYPE_PURE);    //NO.16

        //rigth black border - no.12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,4,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,16,0,FALSE,FALSE,0x3FF,0x3FF,0x3FF,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,4,0,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);


        //bottom black border
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,16,TRUE,FALSE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,12,16,FALSE,TRUE,0,0,0,E_XC_OP1_PATTERN_TYPE_PURE);

    }

    MHal_SC_OP1_Pattern_sram_write_enable(pInstance,FALSE);

    MHal_SC_OP1_Pattern_sram_write_fastmode(pInstance,FALSE);

    MHal_SC_OP1_Pattern_set_wraddr(pInstance,0x00);

    //reg_sram_jump_thr
    MHal_SC_OP1_Pattern_set_jump_threshold(pInstance,0x00);

    MHal_SC_OP1_Pattern_sram_write_enable(pInstance,TRUE);

    MHal_SC_OP1_Pattern_sram_write_fastmode(pInstance,TRUE);

    //Write the border command to second SRAM
    MDrv_SC_OP1_Set_Pattern3_Border(pInstance);

    MHal_SC_OP1_Pattern_sram_write_enable(pInstance,FALSE);

    MHal_SC_OP1_Pattern_sram_write_fastmode(pInstance,FALSE);

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);
}

void MDrv_SC_OP1_3D_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt)
{
    int i=0;
    MS_U16 u16Val =0;
    MS_U16 u16Step = 1024/32;
    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //clear default pattern
    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;
    u16VTileSize = u16VTotalSize/u16VTileCnt;
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror)
    {
        //no.7----------------------------------------------------------------------
        //RIGHT
        u16Val = 1023;
        for(i=0;i<32;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,TRUE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else if(i== 31)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,FALSE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //LEFT
        u16Val = 1023;
        for(i=0;i<32;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else if(i== 31)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,TRUE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //no.6----------------------------------------------------------------------
        u16Val = 1023;
        for(i=0;i<18;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //green box -16
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2), E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        //RIGHT
        for(i=0;i<12;i++)
        {
            if(i==11)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        u16Val = 1023;
        for(i=0;i<10;i++)
        {
           MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
           u16Val -= u16Step;
        }

        // green box - 15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;
        u16Val -= u16Step;

        for(i=0;i<7;i++)
        {
           MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
           u16Val -= u16Step;
        }

        //green box -14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;
        u16Val -= u16Step;

        //LEFT
        for(i=0;i<9;i++)
        {
           if(i==8)
           {
               MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
           }
           else
           {
               MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
           }
           u16Val -= u16Step;
         }

        //no.5
        for(i=0;i<8;i++)
        {
            if(i== 0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //green box -13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;

        //green box -12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        for(i=0;i<5;i++)
        {
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            u16Val -= u16Step;
        }

        //green box -11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        //RIGHT
        for(i=0;i<12;i++)
        {
            if(i==11)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //LEFT
        u16Val =1023;
        for(i=0;i<32;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else if(i== 31)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //no.4
        u16Val = 1023;
        for(i=0;i<8;i++)
        {
            if(i== 0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
         }

        //green box -10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;

        //green box -9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        for(i=0;i<5;i++)
        {
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            u16Val -= u16Step;
        }

        //green box -8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        //RIGHT
        for(i=0;i<12;i++)
        {
            if(i==11)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        u16Val = 1023;
        for(i=0;i<20;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //green box -7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;
        u16Val -= u16Step;

        //LEFT
        for(i=0;i<9;i++)
        {
            if(i==8)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //no.3
        u16Val = 1023;
        for(i=0;i<8;i++)
        {
            if(i== 0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //green box -6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;

        //green box -5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        for(i=0;i<5;i++)
        {
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            u16Val -= u16Step;
        }

        //green box -4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        //RIGHT
        for(i=0;i<12;i++)
        {
            if(i==11)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //LEFT
        u16Val =1023;
        for(i=0;i<32;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else if(i== 31)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //no.2
        u16Val =1023;
        for(i=0;i<18;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //green box -3
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;

        //RIGTH
        for(i=0;i<12;i++)
        {
            if(i==11)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        u16Val = 1023;
        for(i=0;i<10;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        // green box - 2
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;
        u16Val -= u16Step;

        for(i=0;i<7;i++)
        {
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            u16Val -= u16Step;
        }

        //green box -1
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val -= u16Step;
        u16Val -= u16Step;
        u16Val -= u16Step;

        //LEFT
        for(i=0;i<9;i++)
        {
            if(i==8)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        // no.1
        //RIGHT
        u16Val = 1023;
        for(i=0;i<32;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,TRUE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else if(i== 31)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }

        //LEFT
        u16Val = 1023;
        for(i=0;i<32;i++)
        {
            if(i==0)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else if(i== 31)
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,TRUE,0,0,0, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            else
            {
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val -= u16Step;
        }
    }
    else
    {
         //no.1----------------------------------------------------------------------
         //LEFT
         for(i=0;i<32;i++){
                if(i==0){
                    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,TRUE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
                }else if(i== 31){
                    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
                }else{
                    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
                }
                u16Val += u16Step;
          }

         //RIGHT
         u16Val = 0;
         for(i=0;i<32;i++){
                if(i==0){
                    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
                }else if(i== 31){
                    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,TRUE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
                }else{
                    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,2,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
                }
                u16Val += u16Step;
          }

         //no.2-----------------------------------------------------------------------
         //LEFT
         u16Val = 0;
         for(i=0;i<9;i++){
                if(i==0){
                    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
                }
                else{
                    MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
                }
                u16Val += u16Step;
          }

         //green box -1
         MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
         u16Val += u16Step;
         u16Val += u16Step;
         u16Val += u16Step;

         for(i=0;i<7;i++){
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            u16Val += u16Step;
         }

         // green box - 2
         MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
         u16Val += u16Step;
         u16Val += u16Step;
         u16Val += u16Step;

         for(i=0;i<10;i++){
            if(i==9){
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
            }else{
                MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
            u16Val += u16Step;
         }

         //RIGTH
         u16Val=0;
         for(i=0;i<12;i++){
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            u16Val += u16Step;
         }

         //green box -3
         MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
         u16Val += u16Step;
         u16Val += u16Step;

        for(i=0;i<18;i++){
               if(i==17){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
         }

        //no.3
        //LEFT
        u16Val =0;
        for(i=0;i<32;i++){
               if(i==0){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }else if(i== 31){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
         }

        //RIGHT
         u16Val=0;
         for(i=0;i<12;i++){
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            u16Val += u16Step;
         }

        //green box -4
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;

        for(i=0;i<5;i++){
           MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
           u16Val += u16Step;
        }

        //green box -5
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;

        //green box -6
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;

        for(i=0;i<8;i++){
                if(i== 7){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
         }

        //no.4
        //LEFT
        u16Val = 0;
        for(i=0;i<9;i++){
               if(i==0){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
         }

        //green box -7
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;
        u16Val += u16Step;

        for(i=0;i<20;i++){
               if(i==19){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
         }

        //RIGHT
        u16Val=0;
        for(i=0;i<12;i++){
           MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
           u16Val += u16Step;
        }

        //green box -8
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;

        for(i=0;i<5;i++){
           MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
           u16Val += u16Step;
        }

        //green box -9
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;

        //green box -10
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;

        for(i=0;i<8;i++){
                if(i== 7){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
                }
               u16Val += u16Step;
        }

        //no.5
        //LEFT
        u16Val =0;
        for(i=0;i<32;i++){
               if(i==0){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }else if(i== 31){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
         }

        //RIGHT
         u16Val=0;
         for(i=0;i<12;i++){
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            u16Val += u16Step;
        }

        //green box -11
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2),E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;

        for(i=0;i<5;i++){
           MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
           u16Val += u16Step;
        }

        //green box -12
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2), E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;

        MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;

        //green box -13
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2), E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;

        for(i=0;i<8;i++){
                if(i== 7){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
         }

        //no.6
        //LEFT
        u16Val = 0;
        for(i=0;i<9;i++){
               if(i==0){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,TRUE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            }
               u16Val += u16Step;
        }

        //green box -14
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2), E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;
        u16Val += u16Step;

        for(i=0;i<7;i++){
           MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
           u16Val += u16Step;
        }

        // green box - 15
        MHal_SC_OP1_Pattern_customize_blcok(pInstance,3,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2), E_XC_OP1_PATTERN_TYPE_PURE);
        u16Val += u16Step;
        u16Val += u16Step;
        u16Val += u16Step;

        for(i=0;i<10;i++){
           if(i==9){
               MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
           }else{
               MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
           }
           u16Val += u16Step;
        }

        //RIGHT
        u16Val = 0;
         for(i=0;i<12;i++){
            MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
            u16Val += u16Step;
         }

         //green box -16
         MHal_SC_OP1_Pattern_customize_blcok(pInstance,2,1,FALSE,FALSE,(30<<2),(200<<2),(30<<2), E_XC_OP1_PATTERN_TYPE_PURE);
         u16Val += u16Step;
         u16Val += u16Step;

        for(i=0;i<18;i++){
               if(i==17){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,TRUE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,1,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
        }

        //no.7----------------------------------------------------------------------
        //LEFT
        for(i=0;i<32;i++){
               if(i==0){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,TRUE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }else if(i== 31){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,FALSE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
         }

        //RIGHT
        u16Val =0;
        for(i=0;i<32;i++){
               if(i==0){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }else if(i== 31){
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,TRUE,1023,1023,1023, E_XC_OP1_PATTERN_TYPE_PURE);
               }else{
                   MHal_SC_OP1_Pattern_customize_blcok(pInstance,1,16,FALSE,FALSE,u16Val,u16Val,u16Val, E_XC_OP1_PATTERN_TYPE_PURE);
               }
               u16Val += u16Step;
         }
    }

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);

}

void MDrv_SC_OP1_Pure_Color_Tile_Pattern(void *pInstance, MS_U16 u16HTileCnt, MS_U16 u16VTileCnt, MS_U16 u16RVal, MS_U16 u16GVal, MS_U16 u16BVal)
{
    MS_U16 u16HTileSize = 0, u16VTileSize = 0;
    MS_U16 u16HTotalSize = 0, u16VTotalSize = 0;

    //clear default pattern

    MHal_SC_OP1_Pattern_pure_color(pInstance,0x00);

    //disable border
    MHal_SC_OP1_Pattern_enable_border(pInstance,FALSE,0x3FF,0x3FF,0x3FF);

    MDrv_SC_OP1_Align_Width_Height(pInstance, u16HTileCnt, u16VTileCnt, &u16HTotalSize, &u16VTotalSize);

    u16HTileSize = u16HTotalSize/u16HTileCnt;//3840/8=480
    u16VTileSize = u16VTotalSize/u16VTileCnt;//2160/12=180
    MHal_SC_OP1_PatGen_adjust_frame_size(pInstance,u16HTotalSize, u16VTotalSize);

    MHal_SC_OP1_Pattern_sram_writestart(pInstance);

    MHal_SC_OP1_Pattern_define_tile(pInstance,u16HTileSize, u16VTileSize);

    MHal_SC_OP1_Pattern_customize_blcok(pInstance,u16HTileCnt,(u16VTileCnt-1),true,true,u16RVal,u16GVal,u16BVal,E_XC_OP1_PATTERN_TYPE_PURE);

    MHal_SC_OP1_Pattern_sram_writedone(pInstance);
}
#endif
#endif

#ifndef MSOS_TYPE_NOS
#undef _GNU_SOURCE // need this _GNU_SOURCE because we need to check tid by "syscall"
#endif
