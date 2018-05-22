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
/// file    Mdrv_sc_display.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#else
#define do_div(x,y) ((x)/=(y))
#endif

// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
// Internal Definition
#include "drvXC_IOPort.h"
#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"
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
#include "drv_sc_scaling.h"
#include "mdrv_sc_3d.h"
#include "mhal_ip.h"
#include "apiXC_DWIN.h"
#include "drv_sc_DIP_scaling.h"
#include "mhal_dip.h"
#include "halCHIP.h"
#include "mhal_menuload.h"
#include "drvXC_HDMI_Internal.h"
#include "drvGPIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define VERIFY_DISPLAY_FPGA 0
#define PHASE_OFFSET_LIMIT  0x8000UL
#define PHASE_OFFSET_LIMIT_FREQ_ONLY  0x2000UL //0x03DFUL // 0x8000UL
#define PHASE_OFFSET_LIMIT_FREQ_ONLY_ATV_PATCH 0x1000UL //Fine tune for mantis 0810574 : ATV change channel
MS_U32 _u32ZorderMLoadCmd[MS_MLOAD_MAX_CMD_CNT];
MS_U16 _u16ZorderMLoadMsk[MS_MLOAD_MAX_CMD_CNT];
MS_U16 _u16ZorderMLoadVal[MS_MLOAD_MAX_CMD_CNT];
MS_U16 _u16ZorderMLoadCmdCnt = 0;
MS_BOOL _bZorderMLoadEn = FALSE;

// for high accurate input VFreq case, to avoid reconfig Vtt/Htt
#define CHANGE_VTT_TOLERANCE                10
#define CHANGE_HTT_TOLERANCE                10

#define EPIGPIO 103

static MS_BOOL bIsEPI = FALSE;
//-----------------------------------------------------------------------------
//  below definitions are for FSM FPLL
//-----------------------------------------------------------------------------
typedef enum
{
    EN_OUTPUT_TIMING_CHG_DCLK   = 0,
    EN_OUTPUT_TIMING_HTOTAL = 1,
    EN_OUTPUT_TIMING_VTOTAL = 2,
} EN_OUTPUT_TIMING_ADJUST_MODE;

#define FPLL_FSM_STABLE_TIME    (10000)         // if exceed this stable time, the FPLL FSM will re-init
#define FPLL_PRD_MAX            (0xFFFF)        // Max PRD value
#define FPLL_PHASE_VAR_LIMIT    (0x10)          // If phase error variation exceed this, means un-stable
#define FPLL_PRD_THRESHOLD      (0x100)         // The PRD must be smaller than this value to indicate almost finished PRD lock
#define FPLL_PRD_VAR_LIMIT      (0x10)          // If PRD variation exceed this, means un-stable
#define FPLL_PHASE_MAX          (0xFFFF)        // Max PHASE value
#define FPLL_PRD_STABLE_COUNT   0x06

#define GATE_PHASELIMIT         0x3000

// FPLL
#define IVS_OVS_MAX_DIFF    0x1000UL    // before calculate 2nd SET, will check if IVS/OVS diff too big to verify the LPLL SET accuracy
#define IVS_VAR_LIMIT       0x10        // before calculate 2nd SET, the IVS should be very stable, if the variable exceed this, it's un-stable and not cal. 2nd SET
#define VD_HSYNC_LOCKED                 BIT(14)

//Timing
#define VFREQ_DIFF_TOLERANCE         10        //Value to distinguish two Vfreq(x10Hz)

#define PHASE_LOCK_EN     0
#define LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE   1
#define DEFAULT_IP_VSYNC_PERIOD   (20)
#define ENABLE_LOCK_IVS_OVS_FOR_ATV						0

#define VsyncStart_protection 0xf000
#define VsyncEnd_protection 0x1700
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static SC_FRC_SETTING sFRC_Tbl_Default[6] =
{
    {0, 160, 1, 4, MS_FRC_1_4},
    {160, 245, 2, 5, MS_FRC_2_5},  // 24 to 48 {160, 245, 1, 2, MS_FRC_1_2}
    {245, 260, 1, 2, MS_FRC_1_2},
    {260, 400, 1, 2, MS_FRC_1_2},
    {400, 510, 1, 1, MS_FRC_1_1},
    {510, 0xFFFF, 1, 1, MS_FRC_1_1},
};

static void (*_fpPQSetFPLLThreshMode)(MS_U8) = NULL;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_SC_FBL_Calc_Vtt(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow);
MS_U16 MDrv_SC_VFreq_Select(void *pInstance, MS_U16 u16AutoVFreq);

// Output Dclk and Loop DIV : Ref from PNL driver. It is easy to maintain if XC and PNL use same function.
MS_U32  MDrv_PNL_CalculateLPLLSETbyDClk(void *pInstance, MS_U64 ldHz, MS_BOOL bHighAccurate);
MS_U8   MHal_PNL_Get_Loop_DIV(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz);
MS_U16  MHal_PNL_Get_LPLL_LoopGain(void *pInstance, MS_U8 eLPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz);

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// screen
//-----------------------------------------------------------------------------
void MDrv_SC_Set_BLSK(void *pInstance, MS_BOOL bEnable)
{
    if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
        Hal_SC_set_blsk_burst(pInstance, bEnable);
    else
        Hal_SC_set_blsk(pInstance, bEnable);
}

void MDrv_SC_GenerateBlackVideoForBothWin(void *pInstance, MS_BOOL bEnable)
{
    if ( MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED )
        Hal_SC_set_main_sub_black_screen_burst(pInstance, bEnable);
    else
        Hal_SC_set_main_sub_black_screen(pInstance, bEnable);
}

void MDrv_SC_GetCurrentFRCTable(SC_FRC_SETTING* pstFRCTable)
{
    if(pstFRCTable == NULL)
        return;

    memcpy(pstFRCTable, sFRC_Tbl_Default, sizeof(sFRC_Tbl_Default));
}

/******************************************************************************/
///enable/diable output black pattern
///@param bEnable \b IN
///-  False:Disable
///-  TRUE :enable
/******************************************************************************/
void MDrv_SC_GenerateBlackVideo(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    //MS_U8 u8Bank;

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    if(eWindow == MAIN_WINDOW)
    {
        if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetSkipWaitVsync(pInstance, eWindow)))
            Hal_SC_set_main_black_screen_burst(pInstance, bEnable);
        else
            Hal_SC_set_main_black_screen(pInstance, bEnable);
    }
    else
    {
        if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetSkipWaitVsync(pInstance, eWindow)))
            Hal_SC_set_sub_blue_screen_burst(pInstance, bEnable, DISABLE);
        else
            Hal_SC_set_sub_blue_screen(pInstance, bEnable, DISABLE);
    }

    #if VERIFY_DISPLAY_FPGA

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    //----- enable IP signal for FPLL ---//
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    //MDrv_WriteRegBit(H_BK_IP1F2(0x03), 1, BIT(7));
    //MDrv_WriteRegBit(H_BK_IP1F2(0x03), 1, BIT(3));
    //MDrv_WriteByte(BK_SELECT_00, u8Bank);

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0x8000, BIT(15));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0x0800, BIT(11));

    //--- set LVDS format ----//
#ifdef SUPPORT_MOD_ADBANK_SEPARATE

#else
    MOD_W2BYTEMSK(REG_MOD_BK00_40_L, 0x002C, 0x00FF);  // set LVDS format
    MOD_W2BYTEMSK(REG_MOD_BK00_4B_L, 0x0000, 0x00FF);  // set LVDS format
    MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x007E, 0x00FF);  // set LVDS driving
#endif
    #endif
    //--- restore bank select ---//

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);

}

void MDrv_XC_SetDispWindowColor(void *pInstance, MS_U8 u8Color, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8DispWindowColor_Suspend[eWindow] = u8Color;
    Hal_SC_set_nosignal_color(pInstance, u8Color,eWindow);
}

//-------------------------------------------------------------------------------------------------
/// set display window color
/// @param  u8Color                 \b IN: the color to set
/// @param  eWindow                 \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetDispWindowColor_U2(void* pInstance, MS_U8 u8Color, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_SetDispWindowColor(pInstance, u8Color, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
    MDrv_XC_SetDispWindowColor(pInstance, u8Color, eWindow);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_SetDispWindowColor(MS_U8 u8Color, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_DISPLAY_WINDOW_COLOR XCArgs;
    XCArgs.u8Color = u8Color;
    XCArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DISPLAY_WINDOW_COLOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//get the greatest common divisor
MS_U16 MDrv_SC_GCD(MS_U16 m, MS_U16 n)
{
    MS_U16 m_cup, n_cup, res;

    m_cup = m;
    n_cup = n;
    res = m_cup % n_cup;
    while (res != 0)
    {
        m_cup = n_cup;
        n_cup = res;
        res = m_cup % n_cup;
    }

    return n_cup;
}

void MDrv_SC_Handle_Special_FRCTbl(void *pInstance, MS_U16 u16DefaultVFreq, MS_U16 u16InputVFreq, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8Ratio = 0;
    MS_U16 u16FRCType=0;
#ifndef DISABLE_3D_FUNCTION
    MS_BOOL bIsOutput48hz = (!pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable) && MDrv_SC_3D_IsOutputVfreq48hz(pInstance, eWindow, u16InputVFreq);
#else
    MS_BOOL bIsOutput48hz = FALSE;
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@@ bIsOutput48hz = %s\n",bIsOutput48hz?"TRUE":"FALSE")
    memcpy(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl, sFRC_Tbl_Default, sizeof(sFRC_Tbl_Default));

    //when the panel is 120hz default vfreq, the FRC table should be double the ratio.
    //eg. input:30, original x2 out:60, but for 120hz panel, we should x4 out:120.
    //    the multiple is from x2 to x4
    if(u16DefaultVFreq > 900)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
        {
            if((u16InputVFreq > 235) && (u16InputVFreq <= 245))
            {
                u8Ratio = 5;
            }
            else
            {
                u8Ratio = 4;
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "FRC table FRC_OUT x4 for 240 hz panel output\n")
        }
        else
        {
            u8Ratio = 2;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "FRC table FRC_OUT x2 for 120 hz panel output\n")
        }
    }
    else
    {
            u8Ratio = 1;
    }

    for(u16FRCType = 0; u16FRCType < sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING); u16FRCType++)
    {
        // 24Hz -> 48Hz
        if (bIsOutput48hz &&((u16DefaultVFreq == 600)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)))
        {
            // check FRC, and modify FRC table.
            if((u16InputVFreq > pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16LowerBound) &&
            (u16InputVFreq <= pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16HigherBound))
            {
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In = 1;
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out = 2;
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].eFRC_Type= MS_FRC_1_2;
            }
        }

        if (u16DefaultVFreq > 900)
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
            {
                if(((u16InputVFreq > 235)
                    && (u16InputVFreq <= 245))
                   ||((u16InputVFreq > 290)
                      && (u16InputVFreq <= 310)))
                {
                    pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In = 1;
                    pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out = 2;
                    pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].eFRC_Type= MS_FRC_1_2;
                }
            }
        }

        pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out *= u8Ratio;
    }

}
//-----------------------------------------------------------------------------
// frame rate convert
//-----------------------------------------------------------------------------

E_APIXC_ReturnValue MApi_XC_CustomizeFRCTable_U2(void* pInstance, SC_FRC_SETTING* stFRCTable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (stFRCTable == NULL )
    {
        SC_FRC_SETTING sFRC_Tbl_Temp[6] =
        {
            {0, 160, 1, 4, MS_FRC_1_4},
            {160, 245, 2, 5, MS_FRC_2_5},
            {245, 260, 1, 2, MS_FRC_1_2},
            {260, 400, 1, 2, MS_FRC_1_2},
            {400, 510, 1, 1, MS_FRC_1_1},
            {510, 0xFFFF, 1, 1, MS_FRC_1_1},
        };
        pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable = FALSE;
        memcpy( sFRC_Tbl_Default, sFRC_Tbl_Temp , sizeof(sFRC_Tbl_Default) );
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Attention! default frc table is using!\n");
    }
    else
    {
        pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable = TRUE;
        memcpy( sFRC_Tbl_Default, stFRCTable , sizeof(sFRC_Tbl_Default) );
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Attention! custom frc table is using!\n");
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_CustomizeFRCTable(SC_FRC_SETTING* stFRCTable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_CUSTOMIZE_FRC_TABLE XCArgs;
    XCArgs.stFRCTable = stFRCTable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CUSTOMIZE_FRC_TABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Register PQ Set FPLL thresh mode WR bank mapping function.
/// @param  fpXCCB              \b IN: function pointer of PQ set FPLL thresh mode WR bank mapping
//-------------------------------------------------------------------------------------------------
void MApi_XC_RegisterPQSetFPLLThreshMode_U2(void* pInstance, void (*fpPQCB)(MS_U8 u8PQWin))
{
    _fpPQSetFPLLThreshMode = fpPQCB;
}

void MApi_XC_RegisterPQSetFPLLThreshMode(void (*fpPQCB)(MS_U8 u8PQWin))
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_REGISTER_PQ_FPLL_THRESH_MODE XCArgs;
    XCArgs.fpPQCB = fpPQCB;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_REGISTER_PQ_FPLL_THRESH_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16 MDrv_SC_Cal_FRC_Output_Vfreq(void *pInstance, MS_U16 u16InputVFreq,
                             MS_U16 u16DefaultVFreq,
                             MS_BOOL bFBL,
                             MS_U8 *u8FrcIn,
                             MS_U8 *u8FrcOut,
                             MS_BOOL *bSetFrameLock,
                             MS_U8 u8Lpll_bank,
                             SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16OutputVFreq = u16InputVFreq;
#ifndef DISABLE_3D_FUNCTION
    MS_BOOL bIsOutput48hz = (!pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable) && MDrv_SC_3D_IsOutputVfreq48hz(pInstance, eWindow, u16InputVFreq);
#else
    MS_BOOL bIsOutput48hz = FALSE;
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"u16DefaultVFreq = %d, u16InputVFreq =%d\n",
                                        u16DefaultVFreq, u16InputVFreq);

    MDrv_SC_Handle_Special_FRCTbl(pInstance, u16DefaultVFreq, u16InputVFreq, eWindow);

    if (!bFBL)
    {
        MS_U16 u16FRCType=0;
        for(u16FRCType = 0; u16FRCType < (sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING)); u16FRCType++)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,
                "InputV = %d, Tbl[%d] = (%d, %d)\n", u16InputVFreq, u16FRCType,
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16LowerBound,
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16HigherBound) ;

            // check FRC
            if((u16InputVFreq > pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16LowerBound) &&
            (u16InputVFreq <= pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16HigherBound))
            {
                break;
            }
        }

        MS_ASSERT(u16FRCType <= sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING));

        if(u16FRCType >= (sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING)))
        {
            u16FRCType = (sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING))-1;
        }

        // Add this condition to avoid the divider is 0
        if(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In == 0)
        {
            pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In = 1;
        }

        // Set output V
        if(u16FRCType < sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING))
        {
            u16OutputVFreq = u16InputVFreq * pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out
                                          / pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In;

            *u8FrcIn = pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In;
            *u8FrcOut = pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out;
        }
        else
        {
            u16OutputVFreq = 0;
            *u8FrcIn = 1;
            *u8FrcOut = 1;
        }

        #if FRC_INSIDE
        // For 4K2K 2nd FRC used!!!
        if ((1 == u8Lpll_bank) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal> 3000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal > 2000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_4K2K)
            {
                if (u16InputVFreq >= 320)
                {
                    u16OutputVFreq = u16InputVFreq/2;
                    *u8FrcIn = 2;
                    *u8FrcOut = 1;
                }
                else
                {
                    u16OutputVFreq = u16InputVFreq;
                    *u8FrcIn = 1;
                    *u8FrcOut = 1;
                }
            }

        }
        #endif
    }
    else
    {
        *u8FrcIn = 1;
        *u8FrcOut = 1;
    }

    LPLL_BK_STORE;
    if(u8Lpll_bank)
      LPLL_BK_SWITCH(0x01);
    else
      LPLL_BK_SWITCH(0x00);

    // 4K1K FRC 120Hz output case
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ) && (0 == u8Lpll_bank))
    {
        if ((u16InputVFreq >= 235) && (u16InputVFreq <= 245))
        {
            *u8FrcIn = 1;
            *u8FrcOut = 5;
        }
        else
        {
            *u8FrcOut = *u8FrcOut*2;
        }
    }

    // 60->120 or 50->100
    if((u16DefaultVFreq == 1200)||(u16DefaultVFreq == 1000))
    {
        if(((u16InputVFreq >= 1180) && (u16InputVFreq <= 1220))||
           ((u16InputVFreq >= 980) && (u16InputVFreq <= 1020)))
        {
            if(bIsOutput48hz)
            {
                *u8FrcIn = 1;
                *u8FrcOut = 5;
            }
            else
            {
                *u8FrcIn = 1;
                *u8FrcOut = 2;
            }
        }

    }

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
    {
        // Napoli 4K 0.5K FRC 240Hz output case
        if (0 == u8Lpll_bank)
            *u8FrcOut = *u8FrcOut*4;

    }
    else if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
        &&((u16DefaultVFreq == 600)||(u16DefaultVFreq == 500))
        &&(MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
        {
            if(bIsOutput48hz)
            {
                *u8FrcIn = 1;
                *u8FrcOut = 2;
            }
        }
        else
        {
            *u8FrcIn = 1;
            *u8FrcOut = 2;
        }
    }

#ifdef FA_1920X540_OUTPUT
    else
    if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&
      ((MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)||
       (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_TOP_BOTTOM)||
       (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FIELD_ALTERNATIVE))&&
      (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
    {
        *u8FrcIn = 1;
        *u8FrcOut = 2;
    }
#endif
#ifdef TBP_1920X2160_OUTPUT
    else if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&
            ((gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2P_VSIZE)||
             (gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2I_VSIZE))&&
            (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FRAME_PACKING)&&
            (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM))
    {
        *u8FrcIn = 1;
        *u8FrcOut = 1;
    }
#endif
    if((pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
        &&(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_FROM_SRC))
    {
        if((u16DefaultVFreq == 300) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal> 3000) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal > 2000))
        {
            *u8FrcIn = 2;
            *u8FrcOut = 1;
        }
        else
        {
            *u8FrcIn = 1;
            *u8FrcOut = 1;
        }
    }

	// For KR 3D, need 2:2 to frame lock
	if(*bSetFrameLock && *u8FrcIn == 1 && *u8FrcOut == 1 && MDrv_XC_Get_3D_KR3DMode(pInstance))
	{
		*u8FrcIn = 2;
		*u8FrcOut = 2;
	}

    MDrv_WriteByte(H_BK_LPLL(0x0C), ((*u8FrcOut-1) << 4) | (*u8FrcIn-1));
    LPLL_BK_RESTORE;

    //for force case
    if (pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "force free run\n") ;
        if(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_FROM_SRC)
            u16OutputVFreq = MDrv_SC_VFreq_Select(pInstance, u16InputVFreq);
        else
            u16OutputVFreq = MDrv_SC_VFreq_Select(pInstance, u16OutputVFreq);

        *bSetFrameLock = FALSE;
    }
    //for normal case
    else if (((((u16OutputVFreq >= 490) && (u16OutputVFreq <= 510)) ||
             ((u16OutputVFreq >= 590) && (u16OutputVFreq <= 610))) &&
             ((u16DefaultVFreq == 500)||(u16DefaultVFreq == 600)))|| 	//60hz panel
             
            ((((u16OutputVFreq >= 290) && (u16OutputVFreq <= 310)) ||
             ((u16OutputVFreq >= 230) && (u16OutputVFreq <= 260))) &&
		((u16DefaultVFreq == 250)||(u16DefaultVFreq == 300)))||   //30hz output
		
            ((((u16OutputVFreq >= 980) && (u16OutputVFreq <= 1020))||
            ((u16OutputVFreq >= 1180) && (u16OutputVFreq <= 1220)))&&
             ((u16DefaultVFreq == 1000)||(u16DefaultVFreq == 1200)))	//120hzoutput
#if (HW_DESIGN_3D_VER == 4)
            // for FRC FPP-FA 1280x1470@60/SBSH-FA/TBP-FA
            // for FRC FPP-FA 1280x1470@50/SBSH-FA/TBP-FA
             ||((((u16OutputVFreq >= 1180) && (u16OutputVFreq <= 1220))
               ||((u16OutputVFreq >= 980) && (u16OutputVFreq <= 1020))
               ||((u16OutputVFreq >= 2380) && (u16OutputVFreq <= 2420)))
             && ((u16DefaultVFreq == 600)||(u16DefaultVFreq == 500))
             && (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
             || ((u16OutputVFreq >= 1980) && (u16OutputVFreq <= 2020))
             || ((u16OutputVFreq >= 2380) && (u16OutputVFreq <= 2420))

#endif
             || ((pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable)
                && (u16OutputVFreq >= (u16DefaultVFreq - 40))
                && (u16OutputVFreq <= (u16DefaultVFreq + 10)))
              || ((pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable)
                && (u16OutputVFreq >=460)
                && (u16OutputVFreq <= 490) )
             || (bIsOutput48hz && (u16OutputVFreq >= 460) && (u16OutputVFreq <= 490))
             // For 4K2K 2nd FRC used!!
             || ((u16DefaultVFreq == 300)
                && (1 == u8Lpll_bank)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal> 3000)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal > 2000)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500)
                && (((u16OutputVFreq >= 490) && (u16OutputVFreq <= 510)) ||
                    ((u16OutputVFreq >= 590) && (u16OutputVFreq <= 610)) ||
                    ((u16OutputVFreq >= 290) && (u16OutputVFreq <= 310)) ||
                    ((u16OutputVFreq >= 230) && (u16OutputVFreq <= 260)))))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Frame Lock = True\n");
        //u16OutputVFreq = _MDrv_SC_RefineVFreq(MAIN_WINDOW, u16InputVFreq, u16OutputVFreq);
        *bSetFrameLock = TRUE;
    }
    //abnormal case
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Free Run\n");
        // For FRC panel and freerun at specific framerate
        if(u16DefaultVFreq > 900)
        {
            u16OutputVFreq = 1200;
        }
        else if(((u16OutputVFreq >= 480) && (u16OutputVFreq < 490))
                || ((u16OutputVFreq > 510) && (u16OutputVFreq <= 520)))
        {
            u16OutputVFreq = 500; //for better quality
        }
        else
        {
            // Free run at panel default timing.
            u16OutputVFreq = u16DefaultVFreq;
        }
        *bSetFrameLock = FALSE;
    }

    return u16OutputVFreq;
}


//-----------------------------------------------------------------------------
// LPLL
//-----------------------------------------------------------------------------
void MDrv_SC_set_fpll(void *pInstance, MS_BOOL bEnable, MS_BOOL bEnablePhaseLock, MS_U8 u8Lpll_bank)
{
    MS_U32 u32OrigLPLL = 0;
#if(!PHASE_LOCK_EN)
    bEnablePhaseLock = 0;
#endif
    // store bank
    LPLL_BK_STORE;
    // switch bank
    if(u8Lpll_bank)
        LPLL_BK_SWITCH(0x01);
    else
        LPLL_BK_SWITCH(0x00);

    u32OrigLPLL = MDrv_Read4Byte(L_BK_LPLL(0x0F));

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MDrv_SC_set_fpll(%u,%u)\n", bEnable, bEnablePhaseLock) ;

    if(bEnable)
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x0C), BIT(3), BIT(3));
        if(u8Lpll_bank==1)
        {
            //printf("1111\n\n");
            MDrv_Write2ByteMask(L_BK_LPLL(0x7F), 0x00, BIT(8));
        }
    }
    else
    {
        if(u8Lpll_bank==1)
        {
            //printf("2222\n\n");
            MDrv_Write2ByteMask(L_BK_LPLL(0x7F), BIT(8), BIT(8));
        }
        MDrv_Write2ByteMask(L_BK_LPLL(0x0C), 0x00, BIT(3));
    }
    MDrv_Write2ByteMask(L_BK_LPLL(0x0C), bEnablePhaseLock << 6, BIT(6));

    // !!Initial set have to re-update again to force LPLL free run at initial state or
    // lock from initial set!!
    MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL);

    // restore bank
    LPLL_BK_RESTORE;


}

void MDrv_SC_clear_lpll(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, 0x40, BIT(6));
}

void MDrv_SC_monitor_lpll(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16Tmp;
    //MS_U8 u8Bank;

    u16Tmp = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L);

    if (u16Tmp & BIT(6))
    {
        //MDrv_WriteByte(L_BK_IP1F2(0x0F), u8Tmp & ~(BIT(6)));
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, u16Tmp & ~(BIT(6)));
    }

}

static void MDrv_SC_adjust_output_htt_vtt(MS_U16 *u16Htt, MS_U16 *u16Vtt,
                                          MS_U32 u32HighAccurateInputVFreq,
                                          MS_U16 u16DefaultHtt,
                                          MS_U16 u16DefaultVtt,
                                          MS_U16 u16DefaultVFreq,
                                          MS_BOOL bChangeVtt)
{
    MS_U64 u64DCLK;
    u64DCLK = ((MS_U64)u16DefaultHtt) * u16DefaultVtt * u16DefaultVFreq;///10;
    do_div(u64DCLK,10);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,
        "default Htt=%u, Vtt=%u, VFreq=%u, dclk=%u\n",
        u16DefaultHtt,
        u16DefaultVtt,
        u16DefaultVFreq, (unsigned int)u64DCLK);

    if((u32HighAccurateInputVFreq == 0) || ((*u16Htt) == 0) || ((*u16Vtt) == 0))
    {
        return;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "1.Htt=%u, Vtt=%u, VFreq=%u, dclk=%u\n",
        *u16Htt, *u16Vtt, (unsigned int)(u32HighAccurateInputVFreq/100) ,(unsigned int)(((MS_U32)(*u16Htt)) * (*u16Vtt) * (u32HighAccurateInputVFreq/100)/10));

    if ((u32HighAccurateInputVFreq/100) != u16DefaultVFreq)
    {
        MS_U32 u32temp = 0;
        if (bChangeVtt)
        {
            //*u16Vtt = (u64DCLK*10 + (u32HighAccurateInputVFreq * (*u16Htt)/100)/2) / (u32HighAccurateInputVFreq * (*u16Htt)/100);
            u64DCLK = u64DCLK*10;
            u32temp = (u32HighAccurateInputVFreq * (*u16Htt)/100)/2;
            u64DCLK = u64DCLK + u32temp;
            do_div(u64DCLK,(u32HighAccurateInputVFreq * (*u16Htt)/100));
            //*u16Vtt = (u64DCLK*10 + (u32HighAccurateInputVFreq * (*u16Htt))/2) / (u32HighAccurateInputVFreq * (*u16Htt));
            *u16Vtt = (MS_U16)u64DCLK;
        }
        else
        {
            //*u16Htt = (u64DCLK*10 + (u32HighAccurateInputVFreq * (*u16Vtt)/100)/2) / (u32HighAccurateInputVFreq * (*u16Vtt)/100);
            u64DCLK = u64DCLK*10;
            u32temp = (u32HighAccurateInputVFreq * (*u16Vtt)/100)/2;
            u64DCLK = u64DCLK + u32temp;
            do_div(u64DCLK,(u32HighAccurateInputVFreq * (*u16Vtt)/100));
            *u16Htt = (MS_U16)u64DCLK;
            //*u16Htt = (u64DCLK*10 + (u32HighAccurateInputVFreq * (*u16Vtt))/2) / (u32HighAccurateInputVFreq * (*u16Vtt));
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "2.Adjusted Htt=%u, Vtt=%u, VFreq=%u, dclk=%u\n",
        *u16Htt, *u16Vtt, (unsigned int)(u32HighAccurateInputVFreq/100), (unsigned int)(((MS_U32)(*u16Htt)) * (*u16Vtt) * (u32HighAccurateInputVFreq/100)/10));

}

void MDrv_SC_set_fastlpll(void *pInstance, MS_BOOL bEn)
{
    MDrv_WriteRegBit(H_BK_LPLL(0x1E), bEn, 0x20);  //Enable/Disable Fast LPLL
}

MS_BOOL MDrv_SC_get_fastlpll(void *pInstance)
{
    return MDrv_ReadRegBit(H_BK_LPLL(0x1E), 0x20);  //Enable/Disable FBL
}
E_APIXC_ReturnValue MApi_XC_SetOutputAdjustSetting_U2(void* pInstance, XC_OUTPUT_TIMING_ADJUST_SETTING *stAdjSetting)
{
    XC_PANEL_INFO_EX stPanelInfoEx;
    if (stAdjSetting==NULL)
    {
        return E_APIXC_RET_FAIL;
    }

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    memset(&stPanelInfoEx, 0, sizeof(XC_PANEL_INFO_EX));
    stPanelInfoEx.bVttDelayValid = TRUE;
    stPanelInfoEx.u16HttDelay = stAdjSetting->u16VTotal_Delay;
    stPanelInfoEx.bVttStepsValid = TRUE;
    stPanelInfoEx.u16HttSteps = stAdjSetting->u16VTotal_Step;
    stPanelInfoEx.bHttDelayValid = TRUE;
    stPanelInfoEx.u16VttDelay = stAdjSetting->u16HTotal_Delay;
    stPanelInfoEx.bHttStepsValid = TRUE;
    stPanelInfoEx.u16VttSteps= stAdjSetting->u16HTotal_Step;
    stPanelInfoEx.bDclkDelayValid = TRUE;
    stPanelInfoEx.u16DclkDelay = stAdjSetting->u16Dclk_Delay;
    stPanelInfoEx.bDclkStepsValid = TRUE;
    stPanelInfoEx.u16DclkSteps= stAdjSetting->u16Dclk_Step;
    stPanelInfoEx.u32PanelInfoEx_Version = PANEL_INFO_EX_VERSION;
    stPanelInfoEx.u16PanelInfoEX_Length = sizeof(XC_PANEL_INFO_EX);
    //Step setting for change to 50hz
    stPanelInfoEx.u16VFreq = 500;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    if(MDrv_XC_SetExPanelInfo(pInstance, TRUE, &stPanelInfoEx) == FALSE)
    {
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return E_APIXC_RET_FAIL;
    }
#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    if(MDrv_XC_SetExPanelInfo(pInstance, TRUE, &stPanelInfoEx) == FALSE)
    {
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
        return E_APIXC_RET_FAIL;
    }
#endif //#if SC2_ENABLE


    //Step setting for change to 60hz
    stPanelInfoEx.u16VFreq = 600;
    if(MDrv_XC_SetExPanelInfo(pInstance, TRUE, &stPanelInfoEx) == FALSE)
    {

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return E_APIXC_RET_FAIL;
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
#endif //#if SC2_ENABLE
        return E_APIXC_RET_FAIL;
    }
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_SetOutputAdjustSetting(XC_OUTPUT_TIMING_ADJUST_SETTING *stAdjSetting)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OUTPUT_ADJUST_SETTING XCArgs;
    XCArgs.stAdjSetting = stAdjSetting;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OUTPUT_ADJUST_SETTING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_Set_CustomerSyncInfo_U2(void* pInstance, MS_U16 u16FrontPorch, MS_U16 u16SyncWidth, MS_U16 u16OutputVfreq)
{
    XC_PANEL_INFO_EX stPanelInfoEx;
    MS_U16 u16Vtt;
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode != E_XC_PNL_CHG_VTOTAL)
    {
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return bReturn;
    }
#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode != E_XC_PNL_CHG_VTOTAL)
    {
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
        return bReturn;
    }
#endif //#if SC2_ENABLE


    if(u16OutputVfreq == pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq)
    {
        u16Vtt = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;
    }
    else
    {
        u16Vtt = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal*pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq/u16OutputVfreq;
    }
    memset(&stPanelInfoEx, 0, sizeof(XC_PANEL_INFO_EX));
    stPanelInfoEx.bVSyncBackPorchValid = TRUE;
    stPanelInfoEx.bVSyncWidthValid     = TRUE;
    stPanelInfoEx.u16VSyncBackPorch = u16Vtt - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - u16FrontPorch - u16SyncWidth;
    stPanelInfoEx.u16VSyncWidth = u16SyncWidth;
    stPanelInfoEx.u16VFreq = u16OutputVfreq;
    stPanelInfoEx.u32PanelInfoEx_Version = PANEL_INFO_EX_VERSION;
    stPanelInfoEx.u16PanelInfoEX_Length = sizeof(XC_PANEL_INFO_EX);

    bReturn = MDrv_XC_SetExPanelInfo(pInstance, TRUE, &stPanelInfoEx);

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_Set_CustomerSyncInfo(MS_U16 u16FrontPorch, MS_U16 u16SyncWidth, MS_U16 u16OutputVfreq)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_CUSTOMER_SYNC_INFO XCArgs;
    XCArgs.u16FrontPorch = u16FrontPorch;
    XCArgs.u16SyncWidth = u16SyncWidth;
    XCArgs.u16OutputVfreq = u16OutputVfreq;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CUSTOMER_SYNC_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

/*
    Only for adjusting.
    E_XC_PNL_CHG_DCLK,
    E_XC_PNL_CHG_HTOTAL,
    E_XC_PNL_CHG_VTOTAL
    Adjust different type of output timing paramete base on Step/Delay and Ratio to u16Target_Value
*/
#define MIN_STEP_VALUE                  10
#define MIN_DIFF_VALUE_FOR_STEPCHANGE   50
static E_APIXC_ReturnValue _MDrv_XC_SetHttVtt_Steply(void *pInstance, EN_OUTPUT_TIMING_ADJUST_MODE AdjustMode,MS_U16 u16Target_Value,
                                                     MS_U16 u16UserStep, MS_U16 u16UserDelay)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
	
    MS_U16 u16Step, u16Temp, u16Old_Value = 0;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetHttVtt Steply: Mode=%u, Value=%u, Step=%u, Delay=%u\n", AdjustMode, u16Target_Value, u16UserStep, u16UserDelay)
    if (u16UserStep > 1 && AdjustMode != EN_OUTPUT_TIMING_CHG_DCLK)
    {
        if (AdjustMode==EN_OUTPUT_TIMING_VTOTAL)
        {
            u16Old_Value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L)+1;
        }
        else if (AdjustMode==EN_OUTPUT_TIMING_HTOTAL)
        {
            u16Old_Value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L)+1;
        }
        else
        {
            u16Old_Value = 0;
        }
        u16Temp = abs(u16Target_Value - u16Old_Value);
        u16Step = (u16Temp + u16UserStep - 1)/u16UserStep;
        if(u16Temp < MIN_DIFF_VALUE_FOR_STEPCHANGE)
        {
            //If the difference or stepvalue is too small, take it as one step to save time
            //Add this to prevent 60HZ FBL to 60HZ FB step change
            u16Step = u16Temp;
            u16UserStep = 1;
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Step start from %u\n", u16Old_Value)

        if ( u16Old_Value >= u16Target_Value )
        {
            while(u16Old_Value > u16Target_Value + u16Step)
            {
                u16Old_Value -= u16Step;
                if (AdjustMode==EN_OUTPUT_TIMING_VTOTAL)
                {
                    if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW)))
                    {

#if SC2_ENABLE
                        _MLOAD_ENTRY(pInstance);
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16Old_Value - 1)&0xFFF, 0xFFF  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN(pInstance);

#else
                        _MLOAD_ENTRY();
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16Old_Value - 1)&0xFFF, 0xFFF  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


                    }
                    else
                    {
                        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L,(u16Old_Value - 1)&0xFFF);
                    }
                }
                else if (AdjustMode==EN_OUTPUT_TIMING_HTOTAL)
                {
                    if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW)))
                    {

#if SC2_ENABLE
                        _MLOAD_ENTRY(pInstance);
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,(u16Old_Value - 1), 0x1FFF  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN(pInstance);

#else
                        _MLOAD_ENTRY();
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,(u16Old_Value - 1), 0x1FFF  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


                    }
                    else
                    {
                        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L,(u16Old_Value - 1));
                    }
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, " -> %u", (u16Old_Value - 1))
                MsOS_DelayTask(u16UserDelay);
            }
        }
        else
        {
            while(u16Old_Value < u16Target_Value - u16Step)
            {
                u16Old_Value += u16Step;
                if (AdjustMode==EN_OUTPUT_TIMING_VTOTAL)
                {
                    if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW)))
                    {

#if SC2_ENABLE
                        _MLOAD_ENTRY(pInstance);
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16Old_Value - 1)&0xFFF, 0xFFF  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN(pInstance);

#else
                        _MLOAD_ENTRY();
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16Old_Value - 1)&0xFFF, 0xFFF  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


                    }
                    else
                    {
                        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L,(u16Old_Value - 1)&0xFFF);
                    }
                }
                else if (AdjustMode==EN_OUTPUT_TIMING_HTOTAL)
                {
                    if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW)))
                    {

#if SC2_ENABLE
                        _MLOAD_ENTRY(pInstance);
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,(u16Old_Value - 1), 0x1FFF  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN(pInstance);

#else
                        _MLOAD_ENTRY();
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,(u16Old_Value - 1), 0x1FFF  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


                    }
                    else
                    {
                        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L,(u16Old_Value - 1));
                    }
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, " -> %u", (u16Old_Value - 1))
                MsOS_DelayTask(u16UserDelay);
            }
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16Target_Value -> %u\n", (u16Target_Value-1))
    if (AdjustMode==EN_OUTPUT_TIMING_VTOTAL)
    {
        // EPI panel use manual Vsync position setting. Vtt mod 4 must be 2         
        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_10_L,BIT(15)) && bIsEPI == TRUE)
        {
            if((u16Target_Value%4) == 0)
            {
                u16Target_Value += 2;				
            }
            else if((u16Target_Value%4) == 1)
            {
                u16Target_Value += 1;				
            }
            else if((u16Target_Value%4) == 3)
            {
                u16Target_Value -= 1;				
            }
        }
        if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW)))
        {

#if SC2_ENABLE
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16Target_Value-1)&0xFFF, 0xFFF  );
            _MLOAD_RETURN(pInstance);
#else
            _MLOAD_ENTRY();
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16Target_Value-1)&0xFFF, 0xFFF  );
            _MLOAD_RETURN();
#endif //#if SC2_ENABLE


        }
        else
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L,(u16Target_Value-1)&0xFFF);
        }
    }
    else if (AdjustMode==EN_OUTPUT_TIMING_HTOTAL)
    {
        if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW)))
        {

#if SC2_ENABLE
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,u16Target_Value-1, 0x1FFF  );
            _MLOAD_RETURN(pInstance);
#else
            _MLOAD_ENTRY();
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,u16Target_Value-1, 0x1FFF  );
            _MLOAD_RETURN();
#endif //#if SC2_ENABLE


        }
        else
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L,u16Target_Value-1);
        }
    }
    else
    {
        //Todo: Adjust Dclk
        return E_APIXC_RET_FAIL;
    }

    return E_APIXC_RET_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set the FPLL mode:
/// @param  bTrue      \b IN: True: FSM mode, False: Non FSM mode
//-------------------------------------------------------------------------------------------------
void MDrv_XC_FPLLCusReset(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    if(0)//(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support for device id %ld\n",__FUNCTION__,psXCInstPri->u32DeviceID);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MDrv_XC_FPLLCusReset() -> Not support for device id %ld\n", psXCInstPri->u32DeviceID);
        return;
    }
#else
    if(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support for device id %ld\n",__FUNCTION__,psXCInstPri->u32DeviceID);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MDrv_XC_FPLLCusReset() -> Not support for device id %ld\n", psXCInstPri->u32DeviceID);
        return;
    }
#endif //#if SC2_ENABLE



    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    int i;
    for (i = 0; i < E_FPLL_FLAG_MAX; i++)
    {
        pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[i]    = FALSE;
        pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[i]  = 0;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set the custmer FPLL parameters(You can just call this function once when system init for testing)
/// @param  eMode:Enable/disable customer setting      \b IN: @ref EN_FPLL_MODE
/// @param  eFlag:The specified FPLL parameter type which to be modified      \b IN: @ref EN_FPLL_FLAG
/// @param  eFlag:The value to set      \b IN: @ref MS_U32
/// Note: the setting will take effect after running of function MApi_XC_SetPanelTiming
//-------------------------------------------------------------------------------------------------
void MApi_XC_FPLLCustomerMode_U2(void* pInstance, EN_FPLL_MODE eMode, EN_FPLL_FLAG eFlag, MS_U32 u32Data)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    if(0)//(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support for device id %ld\n",__FUNCTION__,psXCInstPri->u32DeviceID);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MApi_XC_EX_FPLLCustomerMode() -> Not support for device id %ld\n", psXCInstPri->u32DeviceID);
        return;
    }
#else
    if(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support for device id %ld\n",__FUNCTION__,psXCInstPri->u32DeviceID);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MApi_XC_EX_FPLLCustomerMode() -> Not support for device id %ld\n", psXCInstPri->u32DeviceID);
        return;
    }
#endif //#if SC2_ENABLE



    if (eMode >= E_FPLL_MODE_MAX)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Wrong Fpll Mode selected!\n");
        return;
    }

    if (eFlag >= E_FPLL_FLAG_MAX)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Wrong Fpll Flag!\n");
        return;
    }
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Fpll Customer Mode: eMode=0x%x, eFlag=0x%x, u32Data=0x%lx\n", eMode, eFlag, u32Data);
    if (eMode == E_FPLL_MODE_DISABLE_ALL)
    {
        MDrv_XC_FPLLCusReset(pInstance);
    }
    else if (eMode == E_FPLL_MODE_DISABLE)
    {
        pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[eFlag]      = FALSE;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[eFlag]      = TRUE;
        pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[eFlag]    = u32Data;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_FPLLCustomerMode(EN_FPLL_MODE eMode, EN_FPLL_FLAG eFlag, MS_U32 u32Data)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_CUSTOMERMODE XCArgs;
    XCArgs.eMode = eMode;
    XCArgs.eFlag = eFlag;
    XCArgs.u32Data = u32Data;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_CUSTOMERMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//For debug purpose only
void MApi_XC_FPLLDbgMode(EN_FPLL_DBG_MODE DbgMode, EN_FPLL_DBG_FLAG DbgFlag, MS_U32 u32Data)
{
    MApi_XC_FPLLCustomerMode((EN_FPLL_MODE)DbgMode, (EN_FPLL_FLAG)DbgFlag, u32Data);
}

//-------------------------------------------------------------------------------------------------
// This function will force freerun in MDrv_Scaler_SetPanelTiming(),
// otherwise MDrv_Scaler_SetPanelTiming() will decide to do framelock or not by itself.
//
// Note: this function is difference with MDrv_SC_set_fpll() which is used to
//       unlock framelock after MDrv_Scaler_SetPanelTiming().
//       But this function is used to affect MDrv_Scaler_SetPanelTiming()'s behavior.
//-------------------------------------------------------------------------------------------------
void MApi_SC_ForceFreerun_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bForceFreerun = bEnable;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MApi_SC_ForceFreerun = %u\n", bEnable);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bForceFreerun = bEnable;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MApi_SC_ForceFreerun = %u\n", bEnable);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


}

void MApi_SC_ForceFreerun(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FORCE_FREERUN XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FORCE_FREERUN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
// This function will return the force freerun state
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SC_IsForceFreerun(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MDrv_SC_IsForceFreerun = %u\n", pXCResourcePrivate->stdrvXC_Display._bForceFreerun);
    return pXCResourcePrivate->stdrvXC_Display._bForceFreerun;
}

MS_BOOL MApi_SC_IsForceFreerun_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MS_BOOL bReturn = MDrv_SC_IsForceFreerun(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MS_BOOL bReturn = MDrv_SC_IsForceFreerun(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    return bReturn;
}

MS_BOOL MApi_SC_IsForceFreerun(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_FORCE_FREERUN XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_FORCE_FREERUN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_SC_SetFreerunVFreq_U2(void* pInstance, E_VFREQ_SEL VFreq)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MApi_SC_SetFreerunVFreq = %u\n", VFreq);
    pXCResourcePrivate->stdrvXC_Display._ForceVFreq     = VFreq;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MApi_SC_SetFreerunVFreq = %u\n", VFreq);
    pXCResourcePrivate->stdrvXC_Display._ForceVFreq     = VFreq;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
}

void MApi_SC_SetFreerunVFreq(E_VFREQ_SEL VFreq)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FREERUN_FREQ XCArgs;
    XCArgs.VFreq = VFreq;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FREERUN_FREQ, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16 MDrv_SC_VFreq_Select(void *pInstance, MS_U16 u16AutoVFreq)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_50HZ)
    {
        return 500;
    }
    else if(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_60HZ)
    {
        return 600;
    }
    else if(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_FROM_SRC)
    {
        return u16AutoVFreq;
    }
    else if (pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_FROM_PANEL)
    {
        return pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MDrv_SC_VFreq_Select(): wrong Vfreq_SEL input, use default logic!!");
        return u16AutoVFreq;
    }
}
//-------------------------------------------------------------------------------------------------
// This function will Get the exact ext_panel info structure for input vfreq
// IN:
//    pPanelInfoEx: ext-panel info for return,
//                  if u16VFreq=0, pPanelInfoEx= next address to save new panel info
//                  else return the panel info pointer of "VFreq"
//    u16VFreq: the request vfreq for getting ext-panel info
// Return:
//    TRUE: Success, FALSE: FAIL
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MDrv_XC_GetExPanelInfo(void *pInstance, XC_PANEL_INFO_EX **ppPanelInfoEx, MS_U16 u16VFreq)
{
    MS_BOOL bRet = FALSE;
    MS_U8 i;
    *ppPanelInfoEx = NULL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16VFreq=%u\n", u16VFreq)

    for(i=0; i<PANEL_INFO_EX_NUM; i++)
    {
        if(u16VFreq == 0)
        {
            //Get next available place for store
            if(gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i].u32PanelInfoEx_Version == 0)
            {
                bRet = TRUE;
                *ppPanelInfoEx = &gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i];
                break;
            }
        }
        else
        {
            //Get ex panel info of specified "u16VFreq"
            if((gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i].u32PanelInfoEx_Version != 0) &&
               (abs(gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i].u16VFreq - u16VFreq) <= VFREQ_DIFF_TOLERANCE) )
            {
                bRet = TRUE;
                *ppPanelInfoEx = &gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i];
                break;
            }
        }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Get ExPanelInfo u16VFreq=%u, pPanelInfoEx=0x%lx, bRet=%u\n", u16VFreq, (MS_U32)*ppPanelInfoEx, bRet);
    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Set external panel info for output
/// Currently, only support setting of two different vfreqs at same time
/// @param  bEnable             \b IN: TRUE: Enable this XC_PANEL_INFO_EX, FALSE: Disable this XC_PANEL_INFO_EX
/// @param  pPanelInfoEx        \b IN: The extern panel information to set @ref XC_PANEL_INFO_EX
/// @return @ref MS_BOOL               TRUE: Success, FALSE: Failed
/// Note: the setting will take effect after running of function MApi_XC_SetPanelTiming
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_SetExPanelInfo(void *pInstance, MS_BOOL bEnable, XC_PANEL_INFO_EX *pPanelInfoEx)
{
    MS_BOOL bRet = TRUE;
    XC_PANEL_INFO_EX *pPnlInfoEx = NULL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(NULL == pPanelInfoEx || pPanelInfoEx->u32PanelInfoEx_Version < 2)
    {
        //We cosider compatible operation form version2 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_EnablePanelInfo: Version or pPanelInfoEx is Invalid!!")
        bRet = FALSE;
    }
    if(bRet)
    {
        MS_U16 u16VFreq = pPanelInfoEx->u16VFreq;
        MS_U16 u16Length = (pPanelInfoEx->u16PanelInfoEX_Length > sizeof(XC_PANEL_INFO_EX)) ? \
               sizeof(XC_PANEL_INFO_EX) : pPanelInfoEx->u16PanelInfoEX_Length;
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MDrv_XC_SetExPanelInfo: bEnable=%u, u16VFreq=%u\n", bEnable, u16VFreq)
        if(bEnable)
        {
            bRet = _MDrv_XC_GetExPanelInfo(pInstance, &pPnlInfoEx, 0);
            if(bRet && pPnlInfoEx != NULL)
            {
                //There are still space to store this info, so copy it
                memcpy(pPnlInfoEx, pPanelInfoEx, u16Length);
            }
        }
        else
        {
            if(u16VFreq == PANEL_INFO_EX_INVALID_ALL)
            {
                //Clear all info in ex panel info structure
                memset(gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx, 0, (sizeof(XC_PANEL_INFO_EX))*PANEL_INFO_EX_NUM);
            }
            else
            {
                //Clear ex panel info structure of specified "VFreq"
                bRet = _MDrv_XC_GetExPanelInfo(pInstance, &pPnlInfoEx, u16VFreq);
                if(bRet && pPnlInfoEx != NULL)
                {
                    memset(pPnlInfoEx, 0, sizeof(XC_PANEL_INFO_EX));
                }
            }
        }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MDrv_XC_SetExPanelInfo Exit: bRet=%u, pPnlInfoEx=%lx\n", bRet, (MS_U32)pPnlInfoEx)
    return bRet;
}

MS_BOOL MApi_XC_SetExPanelInfo_U2(void* pInstance, MS_BOOL bEnable, XC_PANEL_INFO_EX *pPanelInfoEx)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_SetExPanelInfo(pInstance, bEnable, pPanelInfoEx);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bReturn = MDrv_XC_SetExPanelInfo(pInstance, bEnable, pPanelInfoEx);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}
MS_BOOL MApi_XC_SetExPanelInfo(MS_BOOL bEnable, XC_PANEL_INFO_EX *pPanelInfoEx)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_EXPANEL_INFO XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.pPanelInfoEx = pPanelInfoEx;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_EXPANEL_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

static void _MDrv_Scaler_SetIGainPGain(void *pInstance, MS_U8 u8FRC_Out, MS_U16 u16FRC_VFreq, SCALER_WIN eWindow)
{
    //    MS_U8   i,u8Vco,u8LGain2 = 1,u8LGain1=1;
    MS_U8   u8Vco, u8LGain1=1;
    MS_U64  u64dclk=0;
    MS_U8   u8GainI, u8GainP;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16HTotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    MS_U16 u16VTotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;

    //check parameter valid, avoiding the diving zero failure
    if(0 == u8FRC_Out)
    {
        u8FRC_Out = 1;
    }
    //=================================================
    //  FPLL  I/P gain setting
    //
    //                        MM * 524288*loop_gain
    //  i_gain = ---------------------------
    //                   8 * Htt * Vtt * (Ovs + 1 ) * Vco
    //
    //  MM = MPLL / Xtal = 216 / 12(MHz) = 18
    //  loop_gain = 2^(reg_loop_gain_1st) * loop_gain_2nd
    //  loop_gain_2nd = loop_gain_2nd == 0 ? 1 : reg_loop_gain_2nd;
    //  Vco = LVDS == Dual mode ? 7 : 3.5;
    //  p_gain = i_gain *2;
    //  i_gain = 2^(reg_i_gain-5)
    //  p_gain = 2^(reg_p_gain-5)
    //  reg_loop_gain_1st = BK_0x3101[1:0]
    //  loop_gain_2nd       = BK_0x3101[15:8]
    //  Dual mode              = BK_0x3103[8]
    //  Ovs                         = BK_0x310C[15:12]
    //  reg_i_gain              = BK_0x310B[11:8]
    //  reg_p_gain              = BK_0x310B[15:12]
    //--------------------------------------------------------
    u64dclk = ((MS_U64)u16VTotal)*u16HTotal*u16FRC_VFreq;

    u8LGain1 = MHal_PNL_Get_LPLL_LoopGain(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode
                                          , pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type
                                          , u64dclk);

    u8Vco = MHal_PNL_Get_Loop_DIV(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode
                                  , pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type
                                  , u64dclk);

    // MM = MPLL / Xtal = (LVDS_MPLL_CLOCK_MHZ*1000000UL) /g_XC_InitData.u32XTAL_Clock

    MHal_XC_Calc_IGainPGain(pInstance, &u8GainI
                           , &u8GainP
                           , pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock
                           , u8LGain1, u8Vco, u16HTotal, u16VTotal, u8FRC_Out);

    if (gSrcInfo[eWindow].bFBL || gSrcInfo[eWindow].bR_FBL)
    {
        u8GainP += 2;
        u8GainI += 2;
    }
#if !LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
    else
    {
        // u16FRC_Value = 0 means 1:1 case
        MS_U16 u16FRC_Value = (MS_U16)MDrv_ReadByte(H_BK_LPLL(0x0C));
        // since frame lock speed is slower than 30Hz input frame rate, we need
        // to accelerate the speed of framelock by adding i_gain and p_gain by 2
        if ((gSrcInfo[MAIN_WINDOW].u16InputVFreq >= 235)
            && (gSrcInfo[MAIN_WINDOW].u16InputVFreq <= 310)
            && (u16FRC_Value ==0))
        {
            u8GainP +=2;
            u8GainI +=2;
        }
    }
#endif

    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_IGAIN])
    {
        u8GainI = (MS_U8)pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_IGAIN];
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "user forces REG_IGain= 0x%X (%u)\n", u8GainI, u8GainI);
    }
    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PGAIN])
    {
        u8GainP = (MS_U8)pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PGAIN];
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "user forces REG_PGain= 0x%X (%u)\n", u8GainP, u8GainP);
    }

    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    MDrv_WriteByte(H_BK_LPLL(0x0B), (u8GainP<<4)|u8GainI);  //Gain
    LPLL_BK_RESTORE;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "REG_IGain= 0x%X (%u)\n", u8GainI, u8GainI)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "REG_PGain= 0x%X (%u)\n", u8GainP, u8GainP)
}
//-------------------------------------------------------------------------------------------------
// This function will Set the H or V sync information: SyncStart/end/width
// IN:
//    H/V Sync infomation and H/V output total that corresponding to this sync info.
// Return:
//    NONE
//-------------------------------------------------------------------------------------------------
static void _MDrv_XC_SetHVSync(void *pInstance, MS_BOOL bHSync, MS_U16 u16SyncWidth, MS_U16 u16SyncBackPorch, MS_U16 u16HVTotal)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //Change to big one: Change Vtt first to avoid changed vsync end > old vtt
    MS_U16 u16Temp;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "bHSync=%u, SyncWidth=%u, SyncBackPorch=%u, Total=%u\n",
                 bHSync, u16SyncWidth, u16SyncBackPorch, u16HVTotal)
    if(bHSync)
    {
        if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW)))
        {

#if SC2_ENABLE
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_04_L, u16SyncWidth + u16SyncBackPorch, VOP_DE_HSTART_MASK);  // output HDE start
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_01_L, u16SyncWidth-1, 0xFF);    // output Hsync Width
            _MLOAD_RETURN(pInstance);
#else
            _MLOAD_ENTRY();
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_04_L, u16SyncWidth + u16SyncBackPorch, VOP_DE_HSTART_MASK);  // output HDE start
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_01_L, u16SyncWidth-1, 0xFF);    // output Hsync Width
            _MLOAD_RETURN();
#endif //#if SC2_ENABLE


        }
        else
        {
            u16Temp = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, 0xFF);//Old Hsync Width
            if(u16Temp > u16SyncWidth-1)
            {
                //Change to small one
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, u16SyncWidth-1, 0xFF);  // output HDE start
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, u16SyncWidth + u16SyncBackPorch, VOP_DE_HSTART_MASK);
            }
            else
            {
                //Change to big one
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, u16SyncWidth + u16SyncBackPorch, VOP_DE_HSTART_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, u16SyncWidth-1, 0xFF);
            }
        }
    }
    else
    {
        MS_WINDOW_TYPE stWin;
        MS_U16 u16NewVsyncEnd;
        MS_U16 u16VsyncStart;
        MS_U16 u16NewVsyncStart;
        Hal_SC_get_disp_de_window(pInstance, &stWin);
        #if FRC_INSIDE
        XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
        {
            u16HVTotal = u16HVTotal/4;
            u16SyncBackPorch = u16SyncBackPorch/4;			
        }
        #endif

	// EPI panel use manual Vsync position setting. Vtt mod 4 must be 2         
        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_10_L,BIT(15)) && bIsEPI == TRUE)
        {
            if((u16HVTotal%4) == 0)
            {
                u16HVTotal += 2;				
            }
            else if((u16HVTotal%4) == 1)
            {
                u16HVTotal += 1; 
            }
            else if((u16HVTotal%4) == 3)
            {
                u16HVTotal -= 1;				
            }
        }

        u16NewVsyncEnd = u16HVTotal-u16SyncBackPorch;
        u16VsyncStart = u16NewVsyncEnd - u16SyncWidth;		
		// protect error case for VDE start smaller than VDE
        if ((u16VsyncStart - 2) <= stWin.height ||
			u16NewVsyncEnd > VsyncEnd_protection  ||
			u16VsyncStart > VsyncStart_protection  )
        {
            u16VsyncStart = stWin.height + 2;
            u16NewVsyncEnd = u16VsyncStart + u16SyncWidth;			
        }
        u16NewVsyncStart = (u16VsyncStart & 0x7FF) | ((u16VsyncStart & 0x800) << 1);
        if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW)))
        {

#if SC2_ENABLE
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_02_L, u16NewVsyncStart, 0x17FF);  // output Vsync start
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_03_L, u16NewVsyncEnd-1, VOP_VSYNC_END_MASK);    // output Vsync end
            _MLOAD_RETURN(pInstance);
#else
            _MLOAD_ENTRY();
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_02_L, u16NewVsyncStart, 0x17FF);  // output Vsync start
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_03_L, u16NewVsyncEnd-1, VOP_VSYNC_END_MASK);    // output Vsync end
            _MLOAD_RETURN();
#endif //#if SC2_ENABLE


        }
        else
        {
            u16Temp = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, VOP_VSYNC_END_MASK);//Old Vsync End
            if(u16Temp > u16NewVsyncEnd-1)
            {
                //Change to small one
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, u16NewVsyncStart, 0x17FF);  // output Vsync start
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, u16NewVsyncEnd-1);
            }
            else
            {
                //Change to big one
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, u16NewVsyncEnd-1);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, u16NewVsyncStart, 0x17FF);  // output Vsync start
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
// This function will Enable/Disable manual Vsync Mode
// IN:
//    bManualMode: TRUE---Manual Mode, False---Auto Mode
// Return:
//    NONE
//-------------------------------------------------------------------------------------------------
static void _MDrv_XC_SetOutputVSyncMode(void *pInstance, MS_BOOL bManualMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //Change to big one: Change Vtt first to avoid changed vsync end > old vtt
    MS_U16 u16OldVSyncMode = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L) & BIT(15);
    if((((MS_U16)bManualMode)<<15) !=  u16OldVSyncMode)
    {
        u16OldVSyncMode = (((MS_U16)bManualMode)<<15);
        if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW)))
        {

#if SC2_ENABLE
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_10_L, u16OldVSyncMode, BIT(15));  // output Vsync start
            _MLOAD_RETURN(pInstance);
#else
            _MLOAD_ENTRY();
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_10_L, u16OldVSyncMode, BIT(15));  // output Vsync start
            _MLOAD_RETURN();
#endif //#if SC2_ENABLE


        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, u16OldVSyncMode, BIT(15));    //Manual output Vsync
        }
    }
}

void MDrv_SC_set_Htt_Vtt(void *pInstance, XC_SetTiming_Info *pTimingInfo, MS_U16 *pu16VFreq,MS_U32 u32HighAccurateInputVFreq, XC_DISPLAYOUT_INFO *pDisplyout_Info, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16OldHtt, u16OldVtt, u16OPHtt;
    MS_U16 u16HSyncWidth, u16HBackPorch, u16VSyncWidth, u16VBackPorch;
    MS_BOOL bManualVsyncMode = FALSE;
    MS_U16 u16ChangeHttSteps=1,u16ChangeHttDelay=0,u16ChangeVttSteps=CHANGE_VTT_STEPS,u16ChangeVttDelay=CHANGE_VTT_DELAY;
    XC_PANEL_INFO_EX *pPanelInfoEx = NULL;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pDisplyout_Info->u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal;
    pDisplyout_Info->u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal;

#if (FRC_INSIDE)
    if(((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ))
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 1280)
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height== 720))
    {
        pDisplyout_Info->u16Htt_out = 2200;
        pDisplyout_Info->u16Vtt_out = 1130;
    }
#endif

    u16HSyncWidth = pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth;
    u16HBackPorch = pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncBackPorch;
    u16VSyncWidth = pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncWidth;
    u16VBackPorch = pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch;

    u16OldVtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, 0xFFF) + 1;
    u16OldHtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L, 0x1FFF) + 1;
    u16OPHtt = pDisplyout_Info->u16Htt_out;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Default SyncWidth/back porch: H=%u,%u,V=%u,%u, Old Htt/Vtt=%u,%u\n"
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncBackPorch
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncWidth
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch
                                      , u16OldHtt, u16OldVtt)
    if(_MDrv_XC_GetExPanelInfo(pInstance, &pPanelInfoEx, *pu16VFreq) /*&& pPanelInfoEx != NULL*/)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "PanelInfoEx assigned\n")
        if(pPanelInfoEx->bHttDelayValid)
        {
            u16ChangeHttDelay = pPanelInfoEx->u16HttDelay;
        }
        if(pPanelInfoEx->bHttStepsValid)
        {
            u16ChangeHttSteps = pPanelInfoEx->u16HttSteps;
        }
        if(pPanelInfoEx->bVttDelayValid)
        {
            u16ChangeVttDelay = pPanelInfoEx->u16VttDelay;
        }
        if(pPanelInfoEx->bVttStepsValid)
        {
            u16ChangeVttSteps = pPanelInfoEx->u16VttSteps;
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "PanelInfoEx: %s, org htt=%u, vtt=%u, vfreq=%u; HighAccurateVFreq=%u; Output Vfreq=%u, Mode=%u\n", (pPanelInfoEx==NULL) ? "NO": "YES",
                 pDisplyout_Info->u16Htt_out, pDisplyout_Info->u16Vtt_out,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                 *pu16VFreq,(unsigned int)u32HighAccurateInputVFreq, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode)
    if(gSrcInfo[eWindow].bFBL || gSrcInfo[eWindow].bR_FBL)
    {
        if(!(pXCResourcePrivate->stdrvXC_Display._bForceFreerun
            && (pXCResourcePrivate->stdrvXC_Display._ForceVFreq != VFREQ_FROM_SRC)))
        {
            //Not Force Free Run@Fixed 50/60 hz, need adjust vtt
            //calculate Vtt_out for FBL, Vtt_out = Vtt_in * Vde_out / Vde_in
            pDisplyout_Info->u16Vtt_out = MDrv_SC_FBL_Calc_Vtt(pInstance, pTimingInfo, eWindow);
        }
    }

    switch(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode)
    {
        case E_XC_PNL_CHG_HTOTAL:
            if((pPanelInfoEx == NULL) ||
               (pPanelInfoEx->bHTotalValid == FALSE))
            {
                //Change htt, all case can do it
                MDrv_SC_adjust_output_htt_vtt(&pDisplyout_Info->u16Htt_out, &pDisplyout_Info->u16Vtt_out,u32HighAccurateInputVFreq,
                            pDisplyout_Info->u16Htt_out,
                            pDisplyout_Info->u16Vtt_out,
                            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                            FALSE);
            }
            break;
        case E_XC_PNL_CHG_VTOTAL:
            if((pPanelInfoEx == NULL) ||
               (pPanelInfoEx->bVTotalValid == FALSE))
            {
                if((gSrcInfo[eWindow].bFBL || gSrcInfo[eWindow].bR_FBL))
                {
                    //FBL+FPLL or FBL+FreeRun@Src can not adjust vtt
                    if(pXCResourcePrivate->stdrvXC_Display._bForceFreerun
                        && (pXCResourcePrivate->stdrvXC_Display._ForceVFreq != VFREQ_FROM_SRC))
                    {
                        MDrv_SC_adjust_output_htt_vtt(&pDisplyout_Info->u16Htt_out, &pDisplyout_Info->u16Vtt_out,u32HighAccurateInputVFreq,
                                    pDisplyout_Info->u16Htt_out,
                                    pDisplyout_Info->u16Vtt_out,
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                                    TRUE);
                    }

                }
                else
                {
                    //Not FBL case, can adjust vtt
                    MDrv_SC_adjust_output_htt_vtt(&pDisplyout_Info->u16Htt_out, &pDisplyout_Info->u16Vtt_out,u32HighAccurateInputVFreq,
                                pDisplyout_Info->u16Htt_out,
                                pDisplyout_Info->u16Vtt_out,
                                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                                TRUE);
                }
            }
            break;
        case E_XC_PNL_CHG_DCLK:
        default:
            break;
    }

    if(pPanelInfoEx != NULL)
    {
        MS_BOOL bExceeded = FALSE;
        if(!pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@@ Set Htt/Vtt By Ex panel\n\n");
            if(pPanelInfoEx->bVTotalValid)
            {
                pDisplyout_Info->u16Vtt_out = pPanelInfoEx->u16VTotal;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "11: u16Vtt_out=%u\n",pDisplyout_Info->u16Vtt_out);
            }

            if(pPanelInfoEx->bHTotalValid)
            {
                pDisplyout_Info->u16Htt_out = pPanelInfoEx->u16HTotal;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "22: u16Htt_out=%u\n",pDisplyout_Info->u16Htt_out);
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "33: u16MaxVTotal = %u, u16MinVTotal = %u, u16MaxHTotal = %u, u16MinHTotal = %u\n",
            pPanelInfoEx->u16MaxVTotal, pPanelInfoEx->u16MinVTotal,
            pPanelInfoEx->u16MaxHTotal, pPanelInfoEx->u16MinHTotal);

            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "44: bMaxVTotalValid = %u, bMinVTotalValid = %u, bMaxHTotalValid = %u, bMinHTotalValid = %u\n",
            pPanelInfoEx->bMaxVTotalValid, pPanelInfoEx->bMinVTotalValid,
            pPanelInfoEx->bMaxHTotalValid, pPanelInfoEx->bMinHTotalValid);


            if(pPanelInfoEx->bMaxVTotalValid && (pDisplyout_Info->u16Vtt_out > pPanelInfoEx->u16MaxVTotal))
            {
                pDisplyout_Info->u16Vtt_out = pPanelInfoEx->u16MaxVTotal;
                bExceeded = TRUE;
            }

            if(pPanelInfoEx->bMinVTotalValid && (pDisplyout_Info->u16Vtt_out < pPanelInfoEx->u16MinVTotal))
            {
                pDisplyout_Info->u16Vtt_out = pPanelInfoEx->u16MinVTotal;
                bExceeded = TRUE;
            }

            if(pPanelInfoEx->bMaxHTotalValid && (pDisplyout_Info->u16Htt_out > pPanelInfoEx->u16MaxHTotal))
            {
                pDisplyout_Info->u16Htt_out = pPanelInfoEx->u16MaxHTotal;
                bExceeded = TRUE;
            }

            if(pPanelInfoEx->bMinHTotalValid && (pDisplyout_Info->u16Htt_out < pPanelInfoEx->u16MinHTotal))
            {
                pDisplyout_Info->u16Htt_out = pPanelInfoEx->u16MinHTotal;
                bExceeded = TRUE;
            }

            if(bExceeded)
            {
                printf("ALERT: HTotal or VTotal is out of range, Set to default timing and freerun !!!!!!\n");
                pDisplyout_Info->u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal;
                pDisplyout_Info->u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal;
                *pu16VFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
                MDrv_XC_SetFreeRunTiming(pInstance);
            }
        }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "new htt=%u, vtt=%u, vfreq=%u\n", pDisplyout_Info->u16Htt_out, pDisplyout_Info->u16Vtt_out, *pu16VFreq);
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal = pDisplyout_Info->u16Htt_out;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal = pDisplyout_Info->u16Vtt_out;

    if(pPanelInfoEx != NULL)
    {
        if(pPanelInfoEx->bHSyncBackPorchValid ||
           pPanelInfoEx->bHSyncWidthValid)
        {
            pXCResourcePrivate->stdrvXC_Display._bHSyncChanged = TRUE;
            if(pPanelInfoEx->bHSyncBackPorchValid)
            {
                u16HBackPorch = pPanelInfoEx->u16HSyncBackPorch;
            }
            if(pPanelInfoEx->bHSyncWidthValid)
            {
                u16HSyncWidth = pPanelInfoEx->u16HSyncWidth;
            }
        }
        if(pPanelInfoEx->bVSyncBackPorchValid ||
           pPanelInfoEx->bVSyncWidthValid)
        {
            pXCResourcePrivate->stdrvXC_Display._bVSyncChanged = TRUE;
            if(pPanelInfoEx->bVSyncBackPorchValid)
            {
                u16VBackPorch = pPanelInfoEx->u16VSyncBackPorch;
            }
            if(pPanelInfoEx->bVSyncWidthValid)
            {
                u16VSyncWidth = pPanelInfoEx->u16VSyncWidth;
            }
        }
    }
    // for high accurate input VFreq case, to avoid reconfig Vtt/Htt
    if(abs((MS_S16)pDisplyout_Info->u16Vtt_out - (MS_S16)u16OldVtt) > CHANGE_VTT_TOLERANCE)
    {

        if(pDisplyout_Info->u16Vtt_out > u16OldVtt)
        {
            //Change to big one: Change Vtt first to avoid changed Vsync end > old Vtt
            _MDrv_XC_SetHttVtt_Steply(pInstance, EN_OUTPUT_TIMING_VTOTAL, pDisplyout_Info->u16Vtt_out, u16ChangeVttSteps, u16ChangeVttDelay);
            if(pXCResourcePrivate->stdrvXC_Display._bVSyncChanged)
            {
                _MDrv_XC_SetHVSync(pInstance, FALSE, u16VSyncWidth, u16VBackPorch, pDisplyout_Info->u16Vtt_out);
            }
        }
        else
        {
            if(pXCResourcePrivate->stdrvXC_Display._bVSyncChanged)
            {            
                _MDrv_XC_SetHVSync(pInstance, FALSE, u16VSyncWidth, u16VBackPorch, pDisplyout_Info->u16Vtt_out);
            }
            _MDrv_XC_SetHttVtt_Steply(pInstance, EN_OUTPUT_TIMING_VTOTAL, pDisplyout_Info->u16Vtt_out, u16ChangeVttSteps, u16ChangeVttDelay);
        }

    }

    #if FRC_INSIDE
    if ((pDisplyout_Info->u16Htt_out > 3000) && (pDisplyout_Info->u16Vtt_out > 2000) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
        {
            // FHD 50/60 in case, OP timing is 2K2K, Htt is 2K
            u16OPHtt = u16OPHtt/2;
        }
    }
    #endif


    // for high accurate input VFreq case, to avoid reconfig Vtt/Htt
    if(abs((MS_S16)u16OPHtt - (MS_S16)u16OldHtt) > CHANGE_HTT_TOLERANCE)
    {
        if(u16OPHtt > u16OldHtt)
        {
            //Change to big one: Change Htt first to avoid changed Hsync end > old Htt
            _MDrv_XC_SetHttVtt_Steply(pInstance, EN_OUTPUT_TIMING_HTOTAL, u16OPHtt, u16ChangeHttSteps, u16ChangeHttDelay);
            if(pXCResourcePrivate->stdrvXC_Display._bHSyncChanged)
            {
                _MDrv_XC_SetHVSync(pInstance, TRUE, u16HSyncWidth, u16HBackPorch, pDisplyout_Info->u16Htt_out);
            }
        }
        else
        {
            if(pXCResourcePrivate->stdrvXC_Display._bHSyncChanged)
            {
                _MDrv_XC_SetHVSync(pInstance, TRUE, u16HSyncWidth, u16HBackPorch, pDisplyout_Info->u16Htt_out);
            }
            _MDrv_XC_SetHttVtt_Steply(pInstance, EN_OUTPUT_TIMING_HTOTAL, u16OPHtt, u16ChangeHttSteps, u16ChangeHttDelay);
        }
    }
    if(pXCResourcePrivate->stdrvXC_Display._bHSyncChanged || pXCResourcePrivate->stdrvXC_Display._bVSyncChanged)
    {
        bManualVsyncMode = TRUE;
        _MDrv_XC_SetOutputVSyncMode(pInstance, TRUE);
    }
    else
    {
        bManualVsyncMode = FALSE;
        _MDrv_XC_SetOutputVSyncMode(pInstance, FALSE);
    }

    if ((!bManualVsyncMode) && (gSrcInfo[SUB_WINDOW].enInputSourceType == INPUT_SOURCE_SCALER_OP))//Temp solution: DE mode not change it
    {
        //patch for DIP capture in DIP goes with sub_ip/sub_op case, the sub capture window x/y need adjust if OP vtt changes
        MS_S16 s16VttDiff = u16OldVtt - pDisplyout_Info->u16Vtt_out;
        MS_S16 s16HttDiff = u16OldHtt - pDisplyout_Info->u16Htt_out;
        if((MS_S16)(gSrcInfo[SUB_WINDOW].stCapWin.y) >= s16VttDiff)
        {
            gSrcInfo[SUB_WINDOW].stCapWin.y -= s16VttDiff;
        }
        if((MS_S16)(gSrcInfo[SUB_WINDOW].stCapWin.x) >= s16HttDiff)
        {
            gSrcInfo[SUB_WINDOW].stCapWin.x -= s16HttDiff;
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Change CapWin for source OP x/y=%u,%u, H/V Diff=%d,%d\n",
                     gSrcInfo[SUB_WINDOW].stCapWin.x, gSrcInfo[SUB_WINDOW].stCapWin.y, s16HttDiff, s16VttDiff)
        MDrv_XC_SetCaptureWindow(pInstance, &gSrcInfo[SUB_WINDOW].stCapWin,SUB_WINDOW, FALSE);
    }
    if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,eWindow)))
    {

#if SC2_ENABLE
        _MLOAD_ENTRY(pInstance);
        if (!MDrv_XC_MLoad_Fire(pInstance, TRUE))
        {
            printf("Set Htt/vtt ML Fire Error!!!!\n ");
        }
        _MLOAD_RETURN(pInstance);
#else
        _MLOAD_ENTRY();
        if (!MDrv_XC_MLoad_Fire(pInstance, TRUE))
        {
            printf("Set Htt/vtt ML Fire Error!!!!\n ");
        }
        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Htt_out, Vtt_out= %u , %u\n", pDisplyout_Info->u16Htt_out, pDisplyout_Info->u16Vtt_out)

#if (FRC_INSIDE)
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_HTOTAL )
    {
        MDrv_FRC_TGEN_SetHTotal(pInstance, pDisplyout_Info->u16Htt_out);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Set Htt_out to TGEN, Htt_out= %u \n", pDisplyout_Info->u16Htt_out)
    }
    else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_VTOTAL )
    {
        if(pDisplyout_Info->u16Vtt_out < (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height+42))
        {
            pDisplyout_Info->u16Vtt_out = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height+42);
        }
        MDrv_FRC_TGEN_SetVTotal(pInstance, pDisplyout_Info->u16Vtt_out);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Set Vtt_out to TGEN, Vtt_out= %u \n", pDisplyout_Info->u16Vtt_out)
    }

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
    {
        if ((*pu16VFreq>=2380) && (*pu16VFreq<=2420))
        {
            MDrv_FRC_TGEN_SetVTotal(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal/2);
        }
        else if ((*pu16VFreq>=1980) && (*pu16VFreq<=2020))
        {
            MDrv_FRC_TGEN_SetVTotal(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal*60/50/2);
        }
    }
    else
    {
        MDrv_FRC_TGEN_SetVTotal(pInstance, pDisplyout_Info->u16Vtt_out);
    }

    // 4K2K patch
    if( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 3840) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == 2160) )
    {
        MST_PANEL_INFO_t stFRCPanelInfo;
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@@ 4K2K panel patch for FRC TGEN\n");
        MDrv_FRC_PNLInfo_Transform(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo,&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc, &stFRCPanelInfo);
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ)
        {
            pDisplyout_Info->u16Vtt_out = stFRCPanelInfo.u16VTotal;
        }
        MDrv_FRC_Tx_SetTgen(pInstance, &stFRCPanelInfo);
    }
#endif
}

static void _MDrv_XC_Set_FPLL_Thresh_Mode(void)
{
#if FPLL_THRESH_MODE_SUPPORT
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    if (pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode
        && !(gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL) && gSrcInfo[MAIN_WINDOW].bEnableFPLL)
    {
        MDrv_Write2Byte(L_BK_LPLL(0x1F), 0x2EE0);
        MDrv_WriteRegBit(H_BK_LPLL(0x78), ENABLE, BIT(4));
    }
    else
    {
        MDrv_Write2Byte(L_BK_LPLL(0x1F), 0x0080);
        MDrv_WriteRegBit(H_BK_LPLL(0x78), DISABLE, BIT(4));
    }
    LPLL_BK_RESTORE;
#endif
}

static void _MDrv_XC_PQ_Set_FPLL_Thresh_Mode(void *pInstance)
{
#if FPLL_THRESH_MODE_SUPPORT
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode
        && !(gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL) && gSrcInfo[MAIN_WINDOW].bEnableFPLL)
    {
        if (_fpPQSetFPLLThreshMode != NULL)
        {
            _fpPQSetFPLLThreshMode(MAIN_WINDOW);
            if (MDrv_XC_Is_SubWindowEanble(pInstance))
            {
                _fpPQSetFPLLThreshMode(SUB_WINDOW);
            }
        }
    }
#endif
}

void MApi_XC_SetOutTimingMode_U2(void* pInstance, E_XC_PNL_OUT_TIMING_MODE enPnl_Out_Timing_Mode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode = enPnl_Out_Timing_Mode;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode = enPnl_Out_Timing_Mode;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


}

void MApi_XC_SetOutTimingMode(E_XC_PNL_OUT_TIMING_MODE enPnl_Out_Timing_Mode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OUTPUT_TIMING_MODE XCArgs;
    XCArgs.enPnl_Out_Timing_Mode = enPnl_Out_Timing_Mode;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OUTPUT_TIMING_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16 MDrv_SC_FBL_Calc_Vtt(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16  u16Vde_in, u16Vde_out, u16Vtt_in, u16Vtt_out;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    u16Vde_in = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L) & 0x1FFF;

    if ((MDrv_SC_Get_DE_Bypass_Mode(pInstance, eWindow)) && (u16Vde_in == 0x1FFF))
    {
        MS_WINDOW_TYPE stWinType;
        memset(&stWinType, 0, sizeof(MS_WINDOW_TYPE));
        MDrv_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(pInstance, eWindow, &stWinType);
        u16Vde_in = stWinType.height;
    }

    if(u16Vde_in == 0) // avoid divide by zero.
    {
        return 0;
    }
    // Remove storage Port later ( Driver level does not know what storagePort is )
    // Merge MVD port and Storage port
    if( pTimingInfo->bMVOPSrc)
    {
        u16Vtt_in = pTimingInfo->u16InputVTotal;//MDrv_Read2Byte(VOP_FRAME_VCOUNT) & 0x7FF;
    }
    else if(IsSrcTypeAnalog(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType))
    {
        u16Vtt_in = MDrv_XC_PCMonitor_Get_Vtotal(pInstance, eWindow);
    }
    else
    {
        u16Vtt_in = Hal_SC_ip_get_verticaltotal(pInstance, MAIN_WINDOW);//MDrv_Read2Byte(L_BK_IP1F2(0x1f)) & MST_V_TOTAL_MASK;
    }

    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //u16Vde_out = (MDrv_Read2Byte(L_BK_VOP(0x0B))&0x7FF) - (MDrv_Read2Byte(L_BK_VOP(0x0A))&0x7FF) + 1;
    u16Vde_out = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L)&0x7FF) - (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L)&0x7FF) + 1;
    MS_ASSERT(u16Vde_in);

    u16Vtt_out = (MS_U16)(((MS_U32)u16Vtt_in * u16Vde_out + u16Vde_in/2) / u16Vde_in);

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Vde_in= 0x%X (%u)\n", u16Vde_in, u16Vde_in)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Vde_out= 0x%X (%u)\n", u16Vde_out, u16Vde_out)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Vtt_in= 0x%X (%u)\n", u16Vtt_in, u16Vtt_in)

    return u16Vtt_out;
}

void MDrv_SC_Set_LockFreeze_Point(void *pInstance, XC_SetTiming_Info *pTimingInfo, MS_U16 u16Vtt_out, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16  u16LockPoint = 0, u16FreezePoint = 0;
    MS_BOOL bForceReadBank = FALSE;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if( !gSrcInfo[eWindow].bFBL)
    {
        if(gSrcInfo[eWindow].bR_FBL) //for M10 Reguest FBL mode, it doesn't the same with FBL
        {
            u16LockPoint = u16Vtt_out;
            u16FreezePoint = u16LockPoint - 2;

            // Setip Y lock
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x2, BITMASK(3:0));

            // Setup delay line
            if((pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow])
                && (!pTimingInfo->bInterlace))
            {
                gSrcInfo[eWindow].u8DelayLines = 7;//rfbl+uc+pmode, need to adjust delay+1
            }
            else
            {
                gSrcInfo[eWindow].u8DelayLines = 6;
            }

            bForceReadBank = FALSE;
        }
        else
        {
            /* 2010-07-05
            * according to the discussion from designer, we have the following formula:
            * freeze point = vde_end (reg_value) + 1;
            * lock = vde_end(reg_value) + 3;
            */
            u16FreezePoint = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L) + 1;
            u16LockPoint = u16FreezePoint + 2;

            if((!gSrcInfo[eWindow].bInterlace))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x3, BITMASK(3:0));
            }
            else
            {
                // T2: YLock = 4 for FB case
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x4, BITMASK(3:0));
            }
            if((gSrcInfo[eWindow].eDeInterlaceMode == MS_DEINT_2DDI_AVG) ||
                (gSrcInfo[eWindow].eDeInterlaceMode == MS_DEINT_2DDI_BOB))
            {
                gSrcInfo[eWindow].u8DelayLines = 1;    // 5
            }
            else
            {
                if(pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]
                    && (gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_4_FIELDS))  //For 25_4R_MC mode only
                {
                    gSrcInfo[eWindow].u8DelayLines = 20;
                }
                else
                {
                gSrcInfo[eWindow].u8DelayLines = 2;
            	}
            }

            //if HV mode with capture size 0x1fff, we need to adjust delay line to 6.
            if ((MDrv_XC_GetHdmiSyncMode(pInstance,eWindow) == HDMI_SYNC_HV) &&
                (IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)) &&
                (!pTimingInfo->bInterlace))
            {
                MS_WINDOW_TYPE stWinType;
                memset(&stWinType, 0, sizeof(MS_WINDOW_TYPE));
                MDrv_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(pInstance, eWindow, &stWinType);

                if(HVMODE_HDE_VALIDATION(stWinType.width))
                {
                    gSrcInfo[eWindow].u8DelayLines = 6;
                }
            }

            bForceReadBank = FALSE;
        }
        // Setup delay line
        Hal_SC_set_delayline(pInstance,  gSrcInfo[eWindow].u8DelayLines, eWindow );
    }
    else
    {
        //New method for lock point in FBL
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x2, BITMASK(3:0));

        MS_U16 u16Vde_in = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L) & 0x1FFF;

        if(pTimingInfo->bInterlace)
        {
            if (u16Vde_in <= 580)
            {
                gSrcInfo[eWindow].u8DelayLines = 6;
            }
            else
            {
                gSrcInfo[eWindow].u8DelayLines = 7;
            }
            bForceReadBank = FALSE;
        }
        else
        {
            MS_U32 u32VPSDRatio = 0;
            if (eWindow == MAIN_WINDOW)
            {
                u32VPSDRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L) & 0xFFFFFF;
            }
            else
            {
                u32VPSDRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_29_L) & 0xFFFFFF;
            }

            if((u32VPSDRatio & 0x3FFFFFL) > 0x100000)
            {
                //printf("---scaling down----\n");
                gSrcInfo[eWindow].u8DelayLines = DELAY_LINE_SC_DOWN;//8;
            }
            else
            {
                //printf("----scaling up----\n");
                gSrcInfo[eWindow].u8DelayLines = DELAY_LINE_SC_UP;// 7;
            }
            bForceReadBank = TRUE;
        }

        if ((MDrv_SC_Get_DE_Bypass_Mode(pInstance, eWindow)) && (u16Vde_in == 0x1FFF))
        {
            gSrcInfo[eWindow].u8DelayLines = 12;
        }

        Hal_SC_set_delayline(pInstance, gSrcInfo[eWindow].u8DelayLines, eWindow );

        //SC0F_55 = SC10_0D, SC0F_54 = SC0F_55 -2
        u16LockPoint = u16Vtt_out;
        u16FreezePoint = u16LockPoint - 2;
    }

    // Only turn on when FBL (not RFBL) and the signal is progressive mode.
    // to force R bank to be at the first frame
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, (bForceReadBank? ((MS_U16)0x8)<<8 : 0), BITMASK(11:8));

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, BIT(1), BIT(1));  //Using new ovs_ref
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_55_L, (u16LockPoint-1)&0xFFF);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_54_L, (u16FreezePoint-1)&0xFFF);    //FreezePoint = LockPoint-2

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "LockPoint= 0x%X (%u)\n", u16LockPoint, u16LockPoint)
}

void MApi_XC_SetLockPoint_U2(void* pInstance, MS_U16 u16LockPoint, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    UNUSED(eWindow);

    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_55_L, (u16LockPoint-1)&0x7FF);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_54_L, (u16LockPoint-2)&0x7FF);    //FreezePoint = LockPoint-2


#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_SetLockPoint(MS_U16 u16LockPoint, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_LOCK_POINT XCArgs;
    XCArgs.u16LockPoint = u16LockPoint;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LOCK_POINT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}

void MDrv_Scaler_SetPhaseLimit(void *pInstance, MS_U8 u8Lpll_bank)
{
    MS_U16 u16PhaseLimit = PHASE_OFFSET_LIMIT;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //when the dclk max-min value is too small, the panel has hw limitation. we should use
    //much safer parameter to do the FPLL
    //for now, we only found only AU46_T460hw03v1_HD has this problem.
    //So the value:0x0A is coming from AU46_T460hw03v1_HD
    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MaxSET - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MinSET)
        < GATE_PHASELIMIT)
    {
        u16PhaseLimit = 0x1000;
    }

    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PHASELIMIT])
    {
        u16PhaseLimit = (MS_U16)pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PHASELIMIT];
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "user forces Phase Limit = 0x%X\n", u16PhaseLimit);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Phase Limit = 0x%X\n", u16PhaseLimit);

    LPLL_BK_STORE;
    if(u8Lpll_bank)
        LPLL_BK_SWITCH(0x01);
    else
        LPLL_BK_SWITCH(0x00);
    
    MDrv_Write2Byte( L_BK_LPLL(0x0A), u16PhaseLimit);
    LPLL_BK_RESTORE;
}

void MDrv_Scaler_FastFrameLock(void *pInstance, MS_U16 u16InputVFreq,MS_U16 u16OutputVFreq)
{
    MS_U16 u16temp;
    MS_BOOL bCnt;

    //when calling MApi_XC_SetPanelTiming, input source is disabled
    //MApi_XC_DisableInputSource(TRUE, MAIN_WINDOW);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0, BIT(0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 1, BIT(0));
    MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);//enable FPLL
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Enable Fast FPLL\n")

    u16temp = 0;
    bCnt = 0;

    while(1)
    {
        u16temp++;
        MsOS_DelayTask(1);
        //if(MDrv_ReadByte(H_BK_S_VOP(0x56)) & BIT(7)) // get vcnt freeze status
        if(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_H) & BIT(7)) // get vcnt freeze status
        {
           //need wait vcnt down
           bCnt = 1 ;
           break;
        }
        else if(u16temp >((10000/u16OutputVFreq)+2)) // wait at lease one output vsync
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x08, BIT(3));  //SW Reset vcnt freeze
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x00, BIT(3));
            break;
        }
    }

    u16temp = 0;
    if(bCnt)
    {
        //while(MDrv_ReadByte(H_BK_S_VOP(0x56)) & BIT(7))
        while(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_H) & BIT(7))
        {
            u16temp++;
            MsOS_DelayTask(1);
            if(u16temp > ((10000/u16InputVFreq)+2))
            {
                //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 1, BIT(3));  //SW Reset vcnt freeze
                //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 0, BIT(3));
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x08, BIT(3));  //SW Reset vcnt freeze
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x00, BIT(3));
                break;
            }
        }
    }

    //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 1, BIT(3));  //SW Reset vcnt freeze
    //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 0, BIT(3));

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "temp= %u\n", u16temp)
}

void MDrv_SC_Set_Output_Dclk_Slowly(void *pInstance, MS_U32 u32PllSet, MS_U32 u32Steps, MS_U32 u32DelayMs, SCALER_WIN eWindow)
{
    //slowly move to target
    MS_U32 u32PLLStep, u32OrigLPLL = 0;
    MS_U8  i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    u32OrigLPLL = MDrv_Read4Byte(L_BK_LPLL(0x0F));

    u32PllSet = u32PllSet & 0xFFFFFF;

    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_INITSETSTEP]
        && pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_INITSETSTEP])
    {
        u32Steps = pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_INITSETSTEP];
    }
    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_INITSETDELAY]
        && pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_INITSETDELAY])
    {
        u32DelayMs = pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_INITSETDELAY];
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetDCLK: Steps=%lu, DelayMs=%lu\n", u32Steps, u32DelayMs);

    u32PLLStep = abs(u32PllSet - u32OrigLPLL) / u32Steps;

    if(u32PLLStep > (0x50000/u32Steps))
    {
        for(i=1; i<(u32Steps+1); i++)
        {
            if(u32DelayMs != 0)
            {
                MDrv_XC_DelayAndWaitOutVsync(pInstance, u32DelayMs, eWindow); // change output dclk when blanking
            }
            else
            {
                MDrv_XC_wait_output_vsync(pInstance, 2, 50, eWindow);        // change output dclk when blanking
            }
            if(u32PllSet > u32OrigLPLL)
            {
                //add
                MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL + (u32PLLStep * i));
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Slow steps add 0x%lx\n", u32OrigLPLL + (u32PLLStep * i));
            }
            else
            {
                //minus
                MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL - (u32PLLStep * i));
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Slow steps minus 0x%lx\n", u32OrigLPLL - (u32PLLStep * i));
            }
        }
    }
    else
    {
        // direct set
        MDrv_XC_wait_output_vsync(pInstance, 1, 50, eWindow);       // change output dclk when blanking
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32PllSet);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Direct set to 0x%lx\n", u32PllSet);
    }
    LPLL_BK_RESTORE;
}

static void _MDrv_SC_Set_Output_Dclk(MS_U32 u32PllSet)
{
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    u32PllSet = u32PllSet & 0xFFFFFF;
    MDrv_Write3Byte(L_BK_LPLL(0x0F), u32PllSet);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Direct set to 0x%lx\n", u32PllSet);
    LPLL_BK_RESTORE;
}

//-------------------------------------------------------------------------------------------------
/// Get the time for scaler output one frame(Vertical Period)
/// @return @ref MS_U16 return output vertical Period(unit: ms)
//-------------------------------------------------------------------------------------------------
MS_U16 _MDrv_SC_GetOutputVPeriod(void *pInstance)
{
    MS_U16 u16VFreq = MDrv_SC_GetOutputVFreqX100(pInstance);
    if (u16VFreq != 0)
    {
        return ((100000+u16VFreq/2)/u16VFreq); //Period = 1000/(vfreq/100)
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "!!!!!Warning u32VFreq = 0, Force u16IPVsyncPeriod: %u ms, need check input source.\n", DEFAULT_IP_VSYNC_PERIOD);
        return DEFAULT_IP_VSYNC_PERIOD;
    }
}

//-----------------------------------------------------------------------------
//  below functions are for FSM FPLL
//-----------------------------------------------------------------------------
static MS_U16 _MDrv_Scaler_GetAVDStatus(void)
{
    return Hal_SC_GetAVDStatus();
}

void MDrv_Scaler_GetLPLLPhaseOfs(MS_U32* pu32UpOfs, MS_BOOL *bDir)
{
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    *pu32UpOfs   = (MS_U32)MDrv_Read2Byte(L_BK_LPLL(0x11));
    *bDir = MDrv_ReadRegBit(L_BK_LPLL(0x12), BIT(0));
    LPLL_BK_RESTORE;
}

void MDrv_Scaler_GetLPLLPrdOfs(MS_U32* pu32UpOfs, MS_BOOL *bDir)
{
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    *pu32UpOfs   = (MS_U32)MDrv_Read2Byte(L_BK_LPLL(0x13));
    *bDir = MDrv_ReadRegBit(L_BK_LPLL(0x14), BIT(0));
    LPLL_BK_RESTORE;
}

//judge whether the fpll is done or not.
MS_BOOL MDrv_Scaler_GetFPLLDoneStatus(void *pInstance)
{
    MS_U32 u32PhaseOfs,u32PrdOfs;
    MS_BOOL bPhaseDir,bPrdDir;

    MDrv_Scaler_GetLPLLPhaseOfs(&u32PhaseOfs,&bPhaseDir);
    MDrv_Scaler_GetLPLLPrdOfs(&u32PrdOfs,&bPrdDir);

    //printf("Framelock: (%lu,%u,%lu,%u)\n",u32PhaseOfs,bPhaseDir,u32PrdOfs,bPrdDir);
    if(MDrv_ReadRegBit(L_BK_LPLL(0x0C), BIT(3)) &&
       (u32PhaseOfs <= FPLL_PHASE_VAR_LIMIT) &&
       (u32PrdOfs <= FPLL_PRD_VAR_LIMIT))
    {
        //printf("OK.\n");
        return TRUE;
    }
    else
    {
        //printf("No.\n");
        return FALSE;
    }
}

MS_BOOL MDrv_Scaler_IsPRDLock(void *pInstance, MS_U16 *u16OrigPrd)
{
    MS_U16 u16NewPrd = 0;
    //MS_U16 u16NewPhase = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    u16NewPrd = MDrv_Read2Byte(L_BK_LPLL(0x13));
    //u16NewPhase = MDrv_Read2Byte(L_BK_LPLL(0x11));
    LPLL_BK_RESTORE;
#if(PHASE_LOCK_EN)
    if( (u16NewPhase < (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PHASELIMIT] ? (pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PHASELIMIT]) : (PHASE_OFFSET_LIMIT)))
       && (u16NewPrd < FPLL_PRD_THRESHOLD) && (abs(*u16OrigPrd - u16NewPrd) < FPLL_PRD_VAR_LIMIT))
#else
    if((u16NewPrd < FPLL_PRD_THRESHOLD) && (abs(*u16OrigPrd - u16NewPrd) < FPLL_PRD_VAR_LIMIT))
#endif
    {
        *u16OrigPrd = u16NewPrd;
        return TRUE;
    }
    else
    {
        *u16OrigPrd = u16NewPrd;
        return FALSE;
    }
}

MS_BOOL MDrv_Scaler_IsPhaseStable(void *pInstance, MS_U16 *u16OrigPhase)
{
    MS_U16 u16NewPhase = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    u16NewPhase = MDrv_Read2Byte(L_BK_LPLL(0x11));
    LPLL_BK_RESTORE;


    if( (abs(*u16OrigPhase - u16NewPhase) < FPLL_PHASE_VAR_LIMIT) &&
        (u16NewPhase < (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PHASELIMIT] ? (pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PHASELIMIT]) : (PHASE_OFFSET_LIMIT))) )
    {
       *u16OrigPhase = u16NewPhase;
        return TRUE;
    }
    else
    {
       *u16OrigPhase = u16NewPhase;
        return FALSE;
    }
}

void MDrv_Scaler_SetPhaseLock_Thresh(MS_U16 u16Thresh, MS_U8 u8Lpll_bank)
{
    LPLL_BK_STORE;
    if(u8Lpll_bank)
        LPLL_BK_SWITCH(0x01);
    else
        LPLL_BK_SWITCH(0x00);
     XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16Thresh= %u\n", u16Thresh);
    MDrv_Write2ByteMask( L_BK_LPLL(0x05), u16Thresh<<8, 0xF00);
    LPLL_BK_RESTORE;
}

static MS_U16 _MDrv_SC_FPLL_GetMCNRDelayTime(void *pInstance)
{
    MS_U16 u16DelayCount = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW])
    {
        u16DelayCount = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[MAIN_WINDOW].Status2.eFrameStoreNumber,
                                                        FALSE,  //linear mode
                                                        TRUE);  //force field packing mode here to get correct time

        u16DelayCount *= _MDrv_SC_GetOutputVPeriod(pInstance); //get delay time
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MCNR Enable Delay=%u\n", u16DelayCount)
    }
    return u16DelayCount;
}

MS_BOOL  MApi_XC_FrameLockCheck_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if FPLL_THRESH_MODE_SUPPORT
    if (pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode
        && !(gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL))
    {
        MS_BOOL bFrameLock = FALSE;
        LPLL_BK_STORE;
        LPLL_BK_SWITCH(0x00);
        bFrameLock = (MS_BOOL)(MDrv_ReadRegBit(L_BK_LPLL(0x0C), BIT(3)) >> 3);
        LPLL_BK_RESTORE;

#if SC2_ENABLE
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return bFrameLock;
    }
    else
#endif
    {
        //check  PRD Lock            // check  Phase Stable  ->check Frame lock
        if(pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable  &&  pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock)
        {

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            return TRUE;
        }
        else
        {

#if SC2_ENABLE
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

            return FALSE;
        }
    }

}

MS_BOOL  MApi_XC_FrameLockCheck(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_FRAMELOCK XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_FRAMELOCK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#if ENABLE_LOCK_IVS_OVS_FOR_ATV
#define FPLL_ATV_FSM_FREERUN_STABLE_TIME  	100
#define FPLL_ATV_FSM_FREQ_STABLE_TIME  			10000
#define FPLL_ATV_FREQ_STABLE_COUNT	3

static MS_U32 gu32AtvIvsOvsLockFreqStableCount = 0;

static MS_BOOL _MApi_XC_FPLL_IsIvsFreqStable(void* pInstance,MS_U32* pu32VPeriod)
{
	static MS_U32 u32PreVPeriod = 0;
	MS_U32 u32VPeriodDiff;
	MS_U32 u32AccurateVPeriod = MDrv_SC_ip_get_verticalperiod_isr(pInstance,MAIN_WINDOW);

	if(u32AccurateVPeriod != 0)
	{
		u32VPeriodDiff = (u32PreVPeriod > u32AccurateVPeriod ? u32PreVPeriod - u32AccurateVPeriod: u32AccurateVPeriod - u32PreVPeriod);
		if(u32VPeriodDiff < 5)
		{
			gu32AtvIvsOvsLockFreqStableCount++;
		}
		else
		{
			gu32AtvIvsOvsLockFreqStableCount = 0;
		}
		u32PreVPeriod = u32AccurateVPeriod;

		if(pu32VPeriod != NULL)
		{
			*pu32VPeriod = u32AccurateVPeriod;
		}

		if(gu32AtvIvsOvsLockFreqStableCount > FPLL_ATV_FREQ_STABLE_COUNT)
		{
			return TRUE;
		}
	}

	return FALSE;
}

static void _MApi_XC_FPLL_AdjustOvsVtt(void* pInstance,MS_U32 u32AccurateVPeriod)
{
	MS_U16 u16OutVtt, u16OriOutVtt;
	MS_U16 u16AccurateVFreq = 0;
	XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

//psXCInstPri->u32DeviceID


	UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
	if(u32AccurateVPeriod != 0)
	{
	    MS_U64 u64temp = 0;
		//MS_U32 u32AccurateVFreqX1000 = (MS_U32)((MS_U64)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 1000 / u32AccurateVPeriod);
		MS_U32 u32AccurateVFreqX1000;
		u64temp = (MS_U64) pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 1000;
		do_div(u64temp, u32AccurateVPeriod);
		u32AccurateVFreqX1000 = (MS_U32) u64temp;
		//u16AccurateVFreq = (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 100 / u32AccurateVPeriod);
		u64temp = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 100;
		do_div(u64temp, u32AccurateVPeriod);
		u16AccurateVFreq = (MS_U16) u64temp;
		u16OriOutVtt = SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK10_0D_L) + 1;
		u16OutVtt = (MS_U16)(u16OriOutVtt * ((u16AccurateVFreq + 50) / 100 * 100) / u16AccurateVFreq);

		//if(u16OutVtt > u16OriOutVtt - 2 && u16OutVtt < u16OriOutVtt + 2)
		{
			SC_W2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK10_0D_L, u16OutVtt - 1);
		}

		MDrv_SC_set_output_dclk(pInstance,SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK10_0C_L) + 1, SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK10_0D_L) + 1, (u16AccurateVFreq + 50) / 100 * 10, u32AccurateVFreqX1000, 0);
	}
}

static void _MApi_XC_FPLL_EnableLockIvsOvsMode(void* pInstance,MS_BOOL bEnable)
{
	if(bEnable)
	{
		MDrv_Write2Byte(L_BK_LPLL(0x1F), 0x2EE);
		MDrv_Write2ByteMask(L_BK_LPLL(0x78), BIT(12), BIT(12));
		gu32AtvIvsOvsLockFreqStableCount = 0;
		MDrv_SC_set_fpll(pInstance,DISABLE, DISABLE, 0);
	}
	else
	{
		MDrv_Write2Byte(L_BK_LPLL(0x1F), 0);
		MDrv_Write2ByteMask(L_BK_LPLL(0x78), 0, BIT(12));
		gu32AtvIvsOvsLockFreqStableCount = 0;
		MDrv_SC_set_fpll(pInstance,ENABLE, DISABLE, 0);
	}
}
#endif

//==============================================================================
#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE

#define MAX_OUT_VTT_INCREASE_VALUE          10
#define MAX_LOCK_VALUE                                      0x30000
#define DISABLE_LOCK_VALUE                                  0x240000
static void _MApi_XC_FPLL_EnableLcokFreqOnly(MS_BOOL bEnable,INPUT_SOURCE_TYPE_t enInputSourceType)
{
    if(bEnable)
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x70), BIT(15), BIT(15));
        W2BYTEMSK(L_CLKGEN0(0x5A), BIT(2), BIT(2) | BIT(3));
        W2BYTEMSK(L_CLKGEN0(0x5A), 0 , BIT(0));  
        if( IsSrcTypeATV(enInputSourceType) )
        {
             MDrv_Write2Byte(L_BK_LPLL(0x0A), PHASE_OFFSET_LIMIT_FREQ_ONLY_ATV_PATCH);
        }
	else
	{
	     MDrv_Write2Byte(L_BK_LPLL(0x0A), PHASE_OFFSET_LIMIT_FREQ_ONLY);
	}
	 	
    }
    else
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x70), 0, BIT(15));
        W2BYTEMSK(L_CLKGEN0(0x5A), BIT(0), BIT(0));
    }
}

static MS_BOOL _MApi_XC_FPLL_IsSupportLockFreqOnly(void *pInstance)
{
#if HW_SUPPORT_LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
    //MS_U8 u8FrcIn, u8FrcOut;
    //MS_U16 u16FRCType=0;
    MS_BOOL bResult = TRUE;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL || gSrcInfo[MAIN_WINDOW].Status2.eMirrorMode || !gSrcInfo[MAIN_WINDOW].bFastFrameLock)
    {
        bResult = FALSE;
    }
    #if 0
    else
    {
        MS_U32 u32InDiv, u32OutDiv;
        u32OutDiv = ((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0xF000) >> 12) + 1;
        u32InDiv = ((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0x0F00) >> 8) + 1;

        //if(u32OutDiv % u32InDiv != 0) bResult = FALSE;
    }
    #endif

    _MApi_XC_FPLL_EnableLcokFreqOnly(bResult,gSrcInfo[MAIN_WINDOW].enInputSourceType);

    if(!bResult)
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x70), 0, BIT(15));
        W2BYTEMSK(L_CLKGEN0(0x59), BIT(8), BIT(8) | BIT(11));
    }
    else
    {
         if( IsSrcTypeATV(gSrcInfo[MAIN_WINDOW].enInputSourceType) )
         {
             MDrv_Write2Byte(L_BK_LPLL(0x0A), PHASE_OFFSET_LIMIT_FREQ_ONLY_ATV_PATCH);
         }
	 else
	 {
	     MDrv_Write2Byte(L_BK_LPLL(0x0A), PHASE_OFFSET_LIMIT_FREQ_ONLY);
	 }
    }

    return bResult;

#else

    return FALSE;

#endif

}

static MS_U32 _MApi_XC_FPLL_CalculateGCD(MS_U32 u32Input, MS_U32 u32Output)
{
    MS_U32 u32GCD = 1;
    while((u32Input !=0) && (u32Output !=0)) // GCD
    {
        if(u32Input > u32Output)
        {
            u32GCD = u32Output;
            u32Input %= u32Output;
        }
        else // Temp A < TempB
        {
            u32GCD = u32Input;
            u32Output %= u32Input;
        }
    }

    return (u32GCD > 0? u32GCD: 1);
}

static MS_BOOL _MApi_XC_FPLL_LockFreqOnly(void *pInstance)
{
    MS_U16 u16OutHtt, u16OutVtt, u16InHtt, u16InVtt;
    MS_U32 u32OutputClkDiv, u32InputClkDiv;
    MS_U32 u32InDiv, u32OutDiv=0;
    MS_U32 u32RealInDiv, u32RealOutDiv=0;
    MS_U32 u32GCD = 1;
    MS_U8   i;
    MS_U32 u32MaxOutVtt =1, u32MaxGCD = 1;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u16OutHtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L) + 1;
    u16OutVtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L) + 1;
    u16InHtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_28_L) + 1;
    u16InVtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_1F_L);
    u32OutDiv = ((((MDrv_Read2Byte(L_BK_LPLL(0x0E)) & 0x0002)>>1)<< 4 )|((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0xF000) >> 12)) + 1;
    u32InDiv  = ((((MDrv_Read2Byte(L_BK_LPLL(0x0E)) & 0x0001)>>0)<< 4 )|((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0x0F00) >>  8)) + 1;

    u32RealInDiv  = u32InDiv;
    u32RealOutDiv = u32OutDiv;

    if(gSrcInfo[MAIN_WINDOW].bInterlace) u32OutDiv *= 2;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u16InHtt=%u, u16InVtt=%u\n",__FUNCTION__, __LINE__, u16InHtt, u16InVtt);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u16OutHtt=%u, u16OutVtt=%u\n",__FUNCTION__, __LINE__, u16OutHtt, u16OutVtt);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InDiv =%lu, u32OutDiv=%lu\n",__FUNCTION__, __LINE__, u32InDiv, u32OutDiv);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%dgSrcInfo[MAIN_WINDOW].bInterlace=%x, reg_bInterlace=%u\n",__FUNCTION__, __LINE__, gSrcInfo[MAIN_WINDOW].bInterlace, SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, 0x01));

    u32InputClkDiv = u16InHtt * u16InVtt * u32InDiv;
#ifdef SUPPORT_FPLL_REFER_24MXTAL
    u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv/2;
#else
    u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv;
#endif
    if(u32InputClkDiv == 0 || u32OutputClkDiv == 0) return FALSE;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InputClkDiv=%lu, u32OutputClkDiv=%lu\n",__FUNCTION__, __LINE__, u32InputClkDiv, u32OutputClkDiv);

    for(i = 0; i < MAX_OUT_VTT_INCREASE_VALUE; i++)
    {
        u32InputClkDiv = u16InHtt * u16InVtt * u32InDiv;
#ifdef SUPPORT_FPLL_REFER_24MXTAL
        u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv/2;
#else
        u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv;
#endif
        u32GCD = _MApi_XC_FPLL_CalculateGCD(u32InputClkDiv, u32OutputClkDiv);
        if(u32GCD > u32MaxGCD)
        {
            u32MaxOutVtt = u16OutVtt;
            u32MaxGCD = u32GCD;
        }
        u16OutVtt++;
        
		u32InputClkDiv = u16InHtt * u16InVtt * u32InDiv / u32MaxGCD;
		u32OutputClkDiv = u16OutHtt * u32MaxOutVtt* u32OutDiv/ u32MaxGCD;
		if(u32InputClkDiv < MAX_LOCK_VALUE && u32OutputClkDiv < MAX_LOCK_VALUE)
		{
			break;
		}
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u16OutVtt=%u, u32MaxOutVtt=%lu\n",__FUNCTION__, __LINE__, u16OutVtt,u32MaxOutVtt);

    // Get HDMI Pixel Repetition
    MS_U8 u8CurHDMIPixelRep = 1;

    if( gSrcInfo[MAIN_WINDOW].enInputSourceType >= INPUT_SOURCE_HDMI
    && gSrcInfo[MAIN_WINDOW].enInputSourceType < INPUT_SOURCE_HDMI_MAX)
    {
        u8CurHDMIPixelRep = (MDrv_HDMI_avi_infoframe_info(_BYTE_5)& 0x0F)+1;
    }

    u32InputClkDiv = u16InHtt * u16InVtt * u32InDiv * u8CurHDMIPixelRep;

#ifdef SUPPORT_FPLL_REFER_24MXTAL
    u32OutputClkDiv = u16OutHtt * u32MaxOutVtt* u32OutDiv/2;
#else
    u32OutputClkDiv = u16OutHtt * u32MaxOutVtt* u32OutDiv;
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InputClkDiv=%lu, u32OutputClkDiv=%lu\n",__FUNCTION__, __LINE__, u32InputClkDiv, u32OutputClkDiv);

    u32InputClkDiv /= u32MaxGCD;
    u32OutputClkDiv /= u32MaxGCD;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InputClkDiv=%lu, u32OutputClkDiv=%lu\n",__FUNCTION__, __LINE__, u32InputClkDiv, u32OutputClkDiv);

    if(u32InputClkDiv > MAX_LOCK_VALUE || u32OutputClkDiv > MAX_LOCK_VALUE)
    {
        MS_U8 u8GainPI = MDrv_ReadByte(H_BK_LPLL(0x0B));
        if(u8GainPI > 0x21)
        {
            u8GainPI -= 0x11;
            MDrv_WriteByte(H_BK_LPLL(0x0B), u8GainPI);  //Gain
        }
    }
#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
    if(u32InputClkDiv > DISABLE_LOCK_VALUE || u32OutputClkDiv > DISABLE_LOCK_VALUE)
    {
	  _MApi_XC_FPLL_EnableLcokFreqOnly(FALSE,gSrcInfo[MAIN_WINDOW].enInputSourceType);	
    }
#endif
    //if(u32InputClkDiv < 3000 && u32OutputClkDiv < 3000)
    {
        if(u32InputClkDiv > u32OutputClkDiv)
        {
            u32GCD = 2000/u32OutputClkDiv + 1;
        }
        else
        {
            u32GCD = 2000/u32InputClkDiv + 1;
        }
        u32InputClkDiv *= u32GCD;
        u32OutputClkDiv *= u32GCD;
    }

    u32InputClkDiv -= 1;
    u32OutputClkDiv -= 1;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InputClkDiv=%lu, u32OutputClkDiv=%lu\n",__FUNCTION__, __LINE__, u32InputClkDiv, u32OutputClkDiv);
    LPLL_BK_SWITCH(0x00);
    MDrv_Write4Byte(L_BK_LPLL(0x1B), u32OutputClkDiv);
    MDrv_Write4Byte(L_BK_LPLL(0x19), u32InputClkDiv);

     SC_W2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK10_0D_L, u32MaxOutVtt - 1);

 #if 1
        //// after Vtt refactoring, the LPLL set need to re-calculate and update
        MS_U64 u64Htt = 0;
        MS_U64 u64Vtt = 0;
        MS_U32 u32OutputVfreq = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC;
    
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
        {
            u64Htt = SC_R2BYTE(0, REG_SC_BK46_02_L) + 1;
            u64Vtt = SC_R2BYTE(0, REG_SC_BK46_01_L) + 1;
        }
        else
        {
            u64Htt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L) + 1;
            u64Vtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L) + 1;
        }
    
    
        MS_U64 u64DClk = u64Htt * u64Vtt * (MS_U64)u32OutputVfreq;
        MS_U32 u32PllSet = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, u64DClk, FALSE);
    
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32PllSet);
    
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u64Htt=%u, u64Vtt=%u, u32OutputVfreq =%u\n",
                     __FUNCTION__, __LINE__, (unsigned int)u64Htt, (unsigned int)u64Vtt, (unsigned int)u32OutputVfreq);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u64DClk=%u, u32PllSet=0x%x\n",__FUNCTION__, __LINE__, (unsigned int)u64DClk, (unsigned int)u32PllSet);
     
#endif
    return TRUE;
}
#endif // LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
//=======================================================

/*
   Frame lock mechanism should be detected here.
   msAPI_Scaler_FPLL_FSM wait to lock frame only if the
   phase error is small enough.
*/

void _MApi_XC_FPLL_FSM_ISR(SC_INT_SRC eIntNum, void * pParam)
{
    void *pInstance = pu32XCInst_private;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 u32Ofs;
    MS_BOOL Dir ;
    MS_U16 u16PhaseLimit;
#if (!ENABLE_REGISTER_SPREAD)
    MS_U8 u8Bank;
#endif
    UNUSED(eIntNum); UNUSED(pParam);
    //printf("FSM looping pXCResourcePrivate->stdrvXC_Display._enFpllFsm = %x\n", pXCResourcePrivate->stdrvXC_Display._enFpllFsm);
    switch (pXCResourcePrivate->stdrvXC_Display._enFpllFsm)
    {
    case FPLL_IDLE_STATE:
        // no signal, do nothing
        break;

    case FPLL_INIT_STATE://dtv special
        //wait 2 input vsync for framelock phase error stable
        //according our test of 24P video, after enble mvop, need at least 2 input vsync, framelock
        //phase error can be stable.
        pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = FALSE;
        pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock     = FALSE;

        if((MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32InputVsyncStart) < pXCResourcePrivate->stdrvXC_Display.s_u32DelayT)
            break;

        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL INIT->PRD]\n")
        pXCResourcePrivate->stdrvXC_Display.s_u16OrigPrd = FPLL_PRD_MAX;

#if ENABLE_LOCK_IVS_OVS_FOR_ATV
			_MApi_XC_FPLL_EnableLockIvsOvsMode(pInstance,FALSE);
#endif

#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
        if(_MApi_XC_FPLL_IsSupportLockFreqOnly(pInstance))
        {
            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
            {
                pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_FREQ_ONLY_LOCK_STATE;
            }
        }
        else
        {
            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
            {
                pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_PRD_LOCK_STATE;
            }
        }
#else
        if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
        {
            pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_PRD_LOCK_STATE;
        }
#endif

        pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer = MsOS_GetSystemTime();
        pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer;//For MCNR enable counter
        //InISR, we can not wait XC entry, because interrupt is higher than xc mutex
#if (!ENABLE_REGISTER_SPREAD)
        u8Bank = MDrv_ReadByte(BK_SELECT_00);
#endif
        #if ENABLE_ENABLE_OPW_WITH_FRAMELOCK
            Hal_SC_SetOPWriteOff(pInstance, ENABLE);
        #endif
#if (!ENABLE_REGISTER_SPREAD)
        MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
        pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
        break;
    case FPLL_ATV_INIT_STATE://atv special
#if(PHASE_LOCK_EN)
        pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = FALSE;
#else
        pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = TRUE;
#endif
        pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock     = FALSE;
        pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer = MsOS_GetSystemTime();
        if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
        {
            pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_IP_STABLE_STATE;
        }
        break;

    case FPLL_WAIT_IP_STABLE_STATE://atv special
        if (_MDrv_Scaler_GetAVDStatus() & VD_HSYNC_LOCKED)
        {
            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
            {
                pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_LPLL_OFS_STATE;
            }

#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
            _MApi_XC_FPLL_EnableLcokFreqOnly(FALSE,gSrcInfo[MAIN_WINDOW].enInputSourceType);	

           if( IsSrcTypeATV(gSrcInfo[MAIN_WINDOW].enInputSourceType) )
           {
                MDrv_Write2Byte(L_BK_LPLL(0x0A), PHASE_OFFSET_LIMIT_FREQ_ONLY_ATV_PATCH);
           }
	   else
	   {
	        MDrv_Write2Byte(L_BK_LPLL(0x0A), PHASE_OFFSET_LIMIT_FREQ_ONLY);
	   }
#endif

#if ENABLE_LOCK_IVS_OVS_FOR_ATV
			_MApi_XC_FPLL_EnableLockIvsOvsMode(pInstance,TRUE);
			pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCK_ATV_ADJUST_IVS_STATE;
#endif

#if (!ENABLE_REGISTER_SPREAD)
            u8Bank = MDrv_ReadByte(BK_SELECT_00);
#endif
            #if ENABLE_ENABLE_OPW_WITH_FRAMELOCK
                Hal_SC_SetOPWriteOff(pInstance, ENABLE);
            #endif
#if (!ENABLE_REGISTER_SPREAD)
            MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
            pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
            pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[WAIT IP STABLE] -> [WAIT LPLL OFS]\n")
        }
        else if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
        {
            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
            {
                pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
            }
        }
        break;

#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
        case FPLL_FREQ_ONLY_LOCK_STATE:
        {
            _MApi_XC_FPLL_LockFreqOnly(pInstance);
            //pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;
            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
            {
                pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCKED_STATE;
            }
            pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
        }
        break;
#endif

#if ENABLE_LOCK_IVS_OVS_FOR_ATV
	case FPLL_LOCK_ATV_ADJUST_IVS_STATE:
		{
			MS_U32 u32VPeriod = 0;
			if(_MApi_XC_FPLL_IsIvsFreqStable(pInstance,&u32VPeriod))
			{
				_MApi_XC_FPLL_AdjustOvsVtt(pInstance,u32VPeriod);
				if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                {
                    pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                }
                else
                {
				    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCK_ATV_IVS_OVS_STATE;
				}
				pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
			}
			else if(FPLL_ATV_FSM_FREQ_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
	       {
	            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                {
                    pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                }
                else
                {
				    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCK_ATV_IVS_OVS_STATE;
				}
				pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
	       }
		}
		break;

	case FPLL_LOCK_ATV_IVS_OVS_STATE:
		{
			if(FPLL_ATV_FSM_FREERUN_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
	        {
				 MDrv_SC_set_fpll(pInstance,ENABLE, DISABLE, 0);
				if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                {
                    pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                }
                else
                {
	                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_VD_LOCKED_STATE;
	            }
	            pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
	        }
		}
		break;
#endif

    case FPLL_WAIT_LPLL_OFS_STATE://atv special
        // We set PHASE_OFFSET_LIMIT to "limit for lpll phase offset", the "phase dif value" will take
        // PHASE_OFFSET_LIMIT+1 as its max value. then we set the gate to PHASE_OFFSET_LIMIT,
        // every case the "phase dif value" is smaller or equal to the PHASE_OFFSET_LIMIT,
        // we consider the phase is changed to a level, then we can do the adjustment.
        MDrv_Scaler_GetLPLLPhaseOfs(&u32Ofs,&Dir);

        u16PhaseLimit = pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PHASELIMIT] ? pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PHASELIMIT] : PHASE_OFFSET_LIMIT;
 #if (!ENABLE_LOCK_IVS_OVS_FOR_ATV)   
        if (u32Ofs <= u16PhaseLimit)
        {
#endif
            MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_VD_LOCKED_STATE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "Phase offs go into limit~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n")
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[WAIT LPLL OFS] -> [VD LOCKED]\n")
 #if (!ENABLE_LOCK_IVS_OVS_FOR_ATV)   
        }
        else if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
        {
            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
            {
                pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
            }
        }
#endif
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "===> Phase Diff=0x%lx, Limit=0x%x\n", u32Ofs, u16PhaseLimit)
        break;

    case FPLL_VD_LOCKED_STATE://atv special
        {
            pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable  = TRUE;
            pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock      = TRUE;
            if(pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR
                && pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]
                && !pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
            {
                //Wait some vsync, then turn on MCNR
                if(_MDrv_SC_FPLL_GetMCNRDelayTime(pInstance) < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer))
                {
                    if (!pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[MAIN_WINDOW])
                    {
#if (!ENABLE_REGISTER_SPREAD)
                        u8Bank = MDrv_ReadByte(BK_SELECT_00);
#endif
                        #if ENABLE_ENABLE_OPW_WITH_FRAMELOCK
                            Hal_SC_SetOPWriteOff(pInstance, !pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]);
                        #endif
#if (!ENABLE_REGISTER_SPREAD)
                        MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
                    }
                    pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
                    if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                    {
                        pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                    }
                    else
                    {
                        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;
                    }
                }
            }
            else
            {
                if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                {
                    pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                }
                else
                {
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;
                }
            }
        }
        break;

    case FPLL_WAIT_PRD_LOCK_STATE://Dtv special
        if(MDrv_Scaler_IsPRDLock(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPrd))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL PRD locked->Phase]\n")
            pXCResourcePrivate->stdrvXC_Display.s_u16OrigPhase = FPLL_PHASE_MAX;
            pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer = MsOS_GetSystemTime();
#if(PHASE_LOCK_EN)
            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
            {
                pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_PHASE_STABLE_STATE;
            }
#else
            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
            {
                pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_CHECK_FRAME_LOCK_STATE;
            }
#endif
        }
        else
        {
            if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
            {
                if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                {
                    pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                }
                else
                {
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
                }
            }
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "PRD: phase %04x, prd %04x\n", MDrv_Read2Byte(L_BK_LPLL(0x11)), MDrv_Read2Byte(L_BK_LPLL(0x13)))
        break;
#if(PHASE_LOCK_EN)
    case FPLL_WAIT_PHASE_STABLE_STATE://Dtv special
        if(MDrv_Scaler_IsPhaseStable(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPhase))
        {

            if(MDrv_Scaler_IsPRDLock(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPrd)) // read current PRD again
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL phase->check Frame lock]\n")
            }

            MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);

            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL phase->check Frame lock]\n")

            pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer = MsOS_GetSystemTime();
            if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
            {
                pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_CHECK_FRAME_LOCK_STATE;
            }
        }
        else
        {
            if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
            {
                if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                {
                    pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                }
                else
                {
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
                }
            }
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "PHS: phase %04x, prd %04x\n", MDrv_Read2Byte(L_BK_LPLL(0x11)), MDrv_Read2Byte(L_BK_LPLL(0x13)))
        break;
#endif
    case FPLL_CHECK_FRAME_LOCK_STATE://Dtv special
        {

            //MDrv_Scaler_IsPRDLock(&s_u16OrigPrd);       // read current PRD again
            //MDrv_Scaler_IsPhaseStable(&s_u16OrigPhase); // read current phase again
            if(MDrv_Scaler_IsPRDLock(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPrd))
            {
                pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock = TRUE;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL PRD Lock->check Frame lock]\n")
            }
#if(PHASE_LOCK_EN)
            if(MDrv_Scaler_IsPhaseStable(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPhase))
            {
                pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = TRUE;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL phase Lock->check Frame lock]\n")
            }
#endif
            LPLL_BK_STORE;
            LPLL_BK_SWITCH(0x00);
#if(PHASE_LOCK_EN)
            if((MDrv_Read2Byte(L_BK_LPLL(0x11)) > FPLL_PHASE_VAR_LIMIT) || (MDrv_Read2Byte(L_BK_LPLL(0x13)) > FPLL_PRD_VAR_LIMIT))
#else
            if((MDrv_Read2Byte(L_BK_LPLL(0x13)) > FPLL_PRD_VAR_LIMIT))
#endif
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "CHK: phase %04x, prd %04x\n", MDrv_Read2Byte(L_BK_LPLL(0x11)), MDrv_Read2Byte(L_BK_LPLL(0x13)))
                if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
                {
                    if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                    {
                        pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                    }
                    else
                    {
                        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
                    }
                    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "CHK Stable count is outof range, force re-start.\n")
                }
            }
            else
            {
                if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                {
                    pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                }
                else
                {
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCKED_STATE;
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "Locked, the using SET is %lx\n", MDrv_Read4Byte(L_BK_LPLL(0x28)))
            }
            LPLL_BK_RESTORE;
        }
        break;

    case FPLL_LOCKED_STATE://Dtv special
        {
            if(pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR
                && pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]
                && !pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
            {
                //Wait some vsync, then turn on MCNR
                if(_MDrv_SC_FPLL_GetMCNRDelayTime(pInstance) < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer))
                {
                    if (!pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[MAIN_WINDOW])
                    {
#if (!ENABLE_REGISTER_SPREAD)
                        u8Bank = MDrv_ReadByte(BK_SELECT_00);
#endif
                        #if ENABLE_ENABLE_OPW_WITH_FRAMELOCK
                            Hal_SC_SetOPWriteOff(pInstance, !pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]);
                        #endif
#if (!ENABLE_REGISTER_SPREAD)
                        MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
                    }
                    pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
                    if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                    {
                        pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                    }
                    else
                    {
                        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;
                    }
                }
            }
            else
            {
                if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm == FPLL_IDLE_STATE)
                {
                    pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
                }
                else
                {
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;
                }
            }
         }
         break;

    case FPLL_DEATTACH_ISR_STATE:
        {
            //switch bank back
            //MS_BOOL bret;
            //u8Bank = MDrv_ReadByte(BK_SELECT_00);
            //bret = MDrv_XC_InterruptDeAttachWithoutMutex(SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &bFPLL_FSM);
            //MDrv_WriteByte(BK_SELECT_00, u8Bank);
            // If we cannot lock, the bank31_0c will be 0x48, we clear it to 0x08,
            // which is the same with the Non-FSM case.

           LPLL_BK_STORE;
            LPLL_BK_SWITCH(0x00);
            MDrv_WriteRegBit(L_BK_LPLL(0x0C), FALSE, BIT(6));
            LPLL_BK_RESTORE;
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
            pXCResourcePrivate->stdrvXC_Display.u8killframelock = 1;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "FSM ISR: de-attached ISR result=%u, and reset FSM to Idle\n",
                MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM));
        }
        break;

    default:
        break;
    }
}

void _MApi_XC_FPLL_ThreshMode_ISR(SC_INT_SRC eIntNum, void *pParam)
{
    void *pInstance = pu32XCInst_private;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    UNUSED(eIntNum); UNUSED(pParam);

    switch (pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode)
    {
        case FPLL_THRESH_MODE_IDLE_STATE:
        // no signal, do nothing
        break;

        case FPLL_THRESH_MODE_LOCKED_STATE:
        {
            if(pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW] && !pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
            {
                //Wait some vsync, then turn on MCNR
                if(_MDrv_SC_FPLL_GetMCNRDelayTime(pInstance) < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer))
                {
                    if (!pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[MAIN_WINDOW])
                    {
                        #if ENABLE_ENABLE_OPW_WITH_FRAMELOCK
                            Hal_SC_SetOPWriteOff(pInstance, DISABLE);
                        #endif
                    }
                    pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
                    pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode = FPLL_THRESH_MODE_DEATTACH_ISR_STATE;
                }
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode = FPLL_THRESH_MODE_DEATTACH_ISR_STATE;
            }
        }
        break;

        case FPLL_THRESH_MODE_DEATTACH_ISR_STATE:
        {
            MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL);
            pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode = FPLL_THRESH_MODE_IDLE_STATE;
        }
        break;

        default:
        break;
    }
}

/*
   Frame lock mechanism should be detected here.
   msAPI_Scaler_FPLL_FSM wait to lock frame only if the
   phase error is small enough.
*/
void msAPI_Scaler_FPLL_FSM_U2(void* pInstance, SCALER_WIN eWindow)
{
    // use _MApi_XC_FPLL_FSM_ISR() now
    UNUSED(eWindow);

    return;
}

void msAPI_Scaler_FPLL_FSM(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_FSM XCArgs;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_FSM, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_SetPanelTiming_FSM(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    //MS_U8   u8Bank;
    MS_U8   u8FRC_In, u8FRC_Out;
    MS_U32  u32HighAccurateInputVFreq = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_DISPLAYOUT_INFO eDisplyout_Info;
    eDisplyout_Info.u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    eDisplyout_Info.u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;

    if(!mdrv_gpio_get_level(EPIGPIO))
        bIsEPI = TRUE;
    
#if (FRC_INSIDE)
    if(((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ))
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 1280)
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height== 720))
    {
        eDisplyout_Info.u16Htt_out = 2200;
        eDisplyout_Info.u16Vtt_out = 1130;
    }
#endif

    //calculate the time of 2 input vsync
   if(pTimingInfo->u16InputVFreq > 0)
        pXCResourcePrivate->stdrvXC_Display.s_u32DelayT = 20000 / pTimingInfo->u16InputVFreq + 20;
   else
        pXCResourcePrivate->stdrvXC_Display.s_u32DelayT = 1;

    MDrv_SC_set_fpll(pInstance, DISABLE, ENABLE, 0);
    #if ENABLE_ENABLE_OPW_WITH_FRAMELOCK
        Hal_SC_SetOPWriteOff(pInstance, ENABLE);//Disable MCNR first, Enable MCNR when FPLL done
    #endif
    if(!pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug)
    {
        MDrv_Scaler_SetPhaseLimit(pInstance, 0);
    }

    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_S_VOP);
    if (!pTimingInfo->bMVOPSrc)
    {
        // according to customer side test experience, ATV disable framelock point,
        // could get better result.
        //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 0, BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x00 ,BIT(1));
    }
    else
    {
        //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 1, BIT(1));  //Using new ovs_ref
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x02 ,BIT(1));
    }

    gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC = MDrv_SC_Cal_FRC_Output_Vfreq(pInstance, pTimingInfo->u16InputVFreq,
                                              pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                                              gSrcInfo[eWindow].bFBL,
                                              &u8FRC_In,
                                              &u8FRC_Out,
                                              &gSrcInfo[eWindow].bEnableFPLL,
                                              0,
                                              eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "FSM: _u16OutputVFreqAfterFRC = %u\n", gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC );

    if (gSrcInfo[eWindow].bFBL)
    {
        //we don't use FSM flow to do FBL case
        MDrv_XC_SetFreeRunTiming(pInstance);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "---Now it's FBL case, FSM FPLL cannot do it. Just let it freerun.---\n")
    }

    if (gSrcInfo[eWindow].bEnableFPLL == FALSE)
    {
        // if free run, we need to use freerun vfrequency instead of direct calculation
        pTimingInfo->u32HighAccurateInputVFreq = gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC * 100;
        u32HighAccurateInputVFreq = pTimingInfo->u32HighAccurateInputVFreq;
    }
    else
    {
        u32HighAccurateInputVFreq = pTimingInfo->u32HighAccurateInputVFreq * u8FRC_Out / u8FRC_In;
        // 4K1K FRC 120Hz output case
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ)
        {
            u32HighAccurateInputVFreq = u32HighAccurateInputVFreq / 2;
        }
        else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
        {
            u32HighAccurateInputVFreq = u32HighAccurateInputVFreq / 4;
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32HighAccurateInputVFreq = %lu\n", u32HighAccurateInputVFreq);
    if( (u32HighAccurateInputVFreq > 61000) && (u32HighAccurateInputVFreq < 63000) )
    {
        u32HighAccurateInputVFreq = 60000;
    }

    MDrv_SC_set_Htt_Vtt(pInstance, pTimingInfo, &gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC,u32HighAccurateInputVFreq , &eDisplyout_Info, eWindow);
    MDrv_SC_Set_LockFreeze_Point(pInstance, pTimingInfo, eDisplyout_Info.u16Vtt_out, eWindow);


    MDrv_SC_set_output_dclk(pInstance, eDisplyout_Info.u16Htt_out,
                            eDisplyout_Info.u16Vtt_out,
                            gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC, u32HighAccurateInputVFreq, eWindow);

    _MDrv_Scaler_SetIGainPGain(pInstance, u8FRC_Out, gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC, eWindow);

#ifdef FA_1920X540_OUTPUT
    if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&
      ((MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)||
       (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_TOP_BOTTOM)||
       (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FIELD_ALTERNATIVE))&&
      (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal/2), 0x0FFF);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "###g_XC_InitData.stPanelInfo.u16DefaultVTotal=%x\n",pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal/2);
    }
#endif
#ifdef TBP_1920X2160_OUTPUT
    if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&
        ((gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2P_VSIZE)||
        (gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2I_VSIZE))&&
        (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FRAME_PACKING)&&
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM))
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal*2), 0x0FFF);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "###g_XC_InitData.stPanelInfo.u16DefaultVTotal=%x\n",pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal*2);
    }
#endif


    /* enable output free-run */
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1C_L, BIT(8), BIT(8));  //output free run

    _MDrv_XC_Set_FPLL_Thresh_Mode();

    /* Enable/Disable FrameLock */
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, (gSrcInfo[eWindow].bEnableFPLL)<<15, BIT(15));

    if ( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC )
    {
        MDrv_SC_set_fpll(pInstance, gSrcInfo[eWindow].bEnableFPLL, 0, 0);
    }
    else
    {
        MDrv_SC_set_fpll(pInstance, gSrcInfo[eWindow].bEnableFPLL, gSrcInfo[eWindow].bEnableFPLL, 0);
    }

    _MDrv_XC_PQ_Set_FPLL_Thresh_Mode(pInstance);

    pXCResourcePrivate->stdrvXC_Display.s_u32InputVsyncStart = MsOS_GetSystemTime();

    // THIS AREA IS AN EXPERIMENTAL VALUE, so if you encounter anything strange
    // tearing, please try to comment these code blocks.
    // if
    // (1) freerun
    // (2) with non-integer frame rate conversion value
    // we should consider manually control read bank switch
    MS_U16 u16SwitchCnt = 0;
    if(gSrcInfo[eWindow].bEnableFPLL == FALSE)
    {
        if ((u8FRC_In  == 5) && (u8FRC_Out  == 2))
        {
            u16SwitchCnt = 5;
            MDrv_SC_EnableReadBankSwitchControl(pInstance, ENABLE, u16SwitchCnt, eWindow);
        }
        else if ((u8FRC_In  == 2) && (u8FRC_Out  == 1))
        {
            u16SwitchCnt = 2;
            MDrv_SC_EnableReadBankSwitchControl(pInstance, ENABLE, u16SwitchCnt, eWindow);
        }
    }
    else
    {
        //MDrv_SC_EnableReadBankSwitchControl(pInstance, DISABLE, u16SwitchCnt, eWindow);
    }

    if(gSrcInfo[eWindow].bEnableFPLL)
    {
        if(IsSrcTypeDigitalVD(gSrcInfo[eWindow].enInputSourceType))
        {
#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
            gSrcInfo[eWindow].bFastFrameLock = FALSE;
            _MApi_XC_FPLL_EnableLcokFreqOnly(FALSE,gSrcInfo[MAIN_WINDOW].enInputSourceType);
#endif
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_ATV_INIT_STATE;
        }
        else
        {
            gSrcInfo[eWindow].bFastFrameLock = pTimingInfo->bFastFrameLock;
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_INIT_STATE;
        }
    }
    else
    {
        pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "FPLL disabled, reset FSM to Idle\n")
    }
}

//////////////////////////////////////////////////////----------------

void MApi_XC_SetPanelTiming_NonFSM(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    //MS_U8   u8Bank;
    MS_U8   u8FRC_In, u8FRC_Out;
    MS_U32  u32HighAccurateInputVFreq = 0;
    XC_DISPLAYOUT_INFO eDisplyout_Info;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    eDisplyout_Info.u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    eDisplyout_Info.u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;
#if (FRC_INSIDE)
    if(((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ))
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 1280)
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == 720))
    {
        eDisplyout_Info.u16Htt_out = 2200;
        eDisplyout_Info.u16Vtt_out = 1130;
    }
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "bFBL=%u, bMVOPSrc=%u, InputVFreq=%u, InputVtt=%u, bInterlace=%u bFastFL=%u\n",
        gSrcInfo[eWindow].bFBL,
        pTimingInfo->bMVOPSrc,
        pTimingInfo->u16InputVFreq,
        pTimingInfo->u16InputVTotal,
        pTimingInfo->bInterlace,
        pTimingInfo->bFastFrameLock)
    #if ENABLE_ENABLE_OPW_WITH_FRAMELOCK
        Hal_SC_SetOPWriteOff(pu32XCInst, ENABLE);//Disable MCNR first, Enable MCNR when FPLL done
    #endif
    gSrcInfo[eWindow].bFastFrameLock = pTimingInfo->bFastFrameLock;

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);           // backup first
    MDrv_SC_set_fpll(pInstance, DISABLE, ENABLE, 0);    // disable FPLL

    MDrv_Scaler_SetPhaseLimit(pInstance, 0);

    //MDrv_WriteByte(H_BK_LPLL(0x05), 0xF0);
    //MDrv_WriteByte(L_BK_LPLL(0x05), 0x22);

#if VERIFY_DISPLAY_FPGA
    MDrv_Write3Byte(L_BK_LPLL(0x06), 0x30000);
    MDrv_Write3Byte(L_BK_LPLL(0x08), 0x30000);
    MDrv_Write2Byte(L_BK_LPLL(0x0A), 0xFFF0);  // limit for lpll phase limit offset; set it to speed up frame lock

    u8FRC_In = 2;
    u8FRC_Out = 5;
    gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC = pTimingInfo->u16InputVFreq * u8FRC_Out/u8FRC_In;

    gSrcInfo[eWindow].bEnableFPLL = TRUE;

    if(!gSrcInfo[eWindow].bFBL )
    {
        MDrv_WriteByte(H_BK_LPLL(0x0C), ((u8FRC_Out-1) << 4) | (u8FRC_In-1));
    }
    else
    {
        // fps_in:fps_out = 1:1  for FBL, only main can use FBL
        MDrv_WriteByte(H_BK_LPLL(0x0C), 0x00);
    }
#else
    gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC = MDrv_SC_Cal_FRC_Output_Vfreq(pInstance, pTimingInfo->u16InputVFreq,
                                              pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                                              gSrcInfo[eWindow].bFBL,
                                              &u8FRC_In,
                                              &u8FRC_Out,
                                              &gSrcInfo[eWindow].bEnableFPLL,
                                              0,
                                              eWindow);
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "NON-FSM: _u16OutputVFreqAfterFRC = %u\n", gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC)
    gSrcInfo[eWindow].bDoneFPLL = !gSrcInfo[eWindow].bEnableFPLL;       // if enable FPLL, then not done fpll yet

    if (gSrcInfo[eWindow].bEnableFPLL == FALSE)
    {
        // if free run, we need to use freerun vfrequency instead of direct calculation
        pTimingInfo->u32HighAccurateInputVFreq = gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC * 100;
        u32HighAccurateInputVFreq = pTimingInfo->u32HighAccurateInputVFreq;
    }
    else
    {
        u32HighAccurateInputVFreq = pTimingInfo->u32HighAccurateInputVFreq * u8FRC_Out / u8FRC_In;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32HighAccurateInputVFreq = %lu\n", u32HighAccurateInputVFreq);

    MDrv_SC_set_Htt_Vtt(pInstance, pTimingInfo, &gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC,u32HighAccurateInputVFreq , &eDisplyout_Info, eWindow);

    MDrv_SC_Set_LockFreeze_Point(pInstance, pTimingInfo, eDisplyout_Info.u16Vtt_out, eWindow);

    MDrv_SC_set_output_dclk(pInstance, eDisplyout_Info.u16Htt_out,
                            eDisplyout_Info.u16Vtt_out,
                            gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC, 0, eWindow);

    _MDrv_Scaler_SetIGainPGain(pInstance, u8FRC_Out, gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC, eWindow);
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //MDrv_WriteRegBit(H_BK_VOP(0x1C), 1, BIT(0));  //output free run
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1C_L, BIT(8), BIT(8));  //output free run

    _MDrv_XC_Set_FPLL_Thresh_Mode();

    if (gSrcInfo[eWindow].bEnableFPLL)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //MDrv_WriteRegBit(H_BK_IP1F2(0x03), 1, BIT(7));  //enable frame lock
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, BIT(15), BIT(15));  //enable frame lock

        if (gSrcInfo[eWindow].bFastFrameLock) //MDrv_ReadRegBit(H_BK_LPLL(0x1E), 0x10)
        {
            MDrv_Scaler_FastFrameLock(pInstance, pTimingInfo->u16InputVFreq, gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC);
            //MDrv_XC_DisableInputSource(FALSE, MAIN_WINDOW);  //enable Input
        }
        else
        {
            //MDrv_XC_DisableInputSource(FALSE, MAIN_WINDOW);  //enable Input
            HAL_SC_EnableFPLL();
        }
        pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
        pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
    }

    _MDrv_XC_PQ_Set_FPLL_Thresh_Mode(pInstance);

    #if 0
    u16temp = 0;
    while( !(MDrv_ReadByte(H_BK_LPLL(0x2A)) & BIT0) &&  u16temp<3000 )
    {
        u16temp++;
        MsOS_DelayTask(1);
    }
    printf("[FPLL Speed]Time= %d\n", msAPI_Timer_DiffTimeFromNow(u32TestTimer));
    printf("[FPLL Speed]temp= %d\n", u16temp);
    #endif

    #if 0
    for(u16temp=0; u16temp<20; u16temp++)
    {
        printf("\n%d\n", u16temp);
        printf("[Freeze]Phase dif= 0x%X (%u)\n", MDrv_Read2Byte(L_BK_LPLL(0x11)), MDrv_Read2Byte(L_BK_LPLL(0x11)));
        printf("[Freeze]Phase up= 0x%X (%u)\n", MDrv_ReadByte(L_BK_LPLL(0x12)), MDrv_ReadByte(L_BK_LPLL(0x12)));
        printf("[Freeze]SET using= 0x%X (%u)\n", MDrv_Read2Byte(L_BK_LPLL(0x28))|(MDrv_ReadByte(L_BK_LPLL(0x29))<<16),
                                                 MDrv_Read2Byte(L_BK_LPLL(0x28))|(MDrv_ReadByte(L_BK_LPLL(0x29))<<16));
        MsOS_DelayTask((10000/pTimingInfo->u16InputVFreq));
    }
    #endif
}

#if FRC_INSIDE
void MApi_XC_SetFLock(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8  u8FRC_In, u8FRC_Out;
    MS_U16 u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
    MS_U16 u16InputVFreq = pTimingInfo->u16InputVFreq;
    MS_U16 u16OutputVfreqAfterFRC = 0;

    // For error handling
    if (u16InputVFreq == 0)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
            u16InputVFreq = 10;
        else
            u16InputVFreq = 5;
    }

    MS_U32 u32DClk;
    MS_U32 u32LpllLimitLow=0x10000;
    MS_U32 u32LpllLimitHigh=0x10000;
    MS_BOOL bEnableFPLL;
    MS_U16 u16HTotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    MS_U16 u16VTotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
        u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq / 2;
    else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ)
        u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;

    // For 4K2K used
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K))
    {
        u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq / 2;
    }
    else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
    {
        // Make OP timing 4K x 0.5K x 120Hz
        u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq * 2;
        u16VTotal = u16VTotal /4;
    }
    else if ((u16HTotal > 3000) && (u16VTotal > 2000) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
        {
            // FHD 50/60 in case
            u16HTotal = u16HTotal/2;
        }
        else
        {
            // 4K2K 24in/30in case
            u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq / 2;
        }
    }

    if(MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
    {
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_960x1080)
        {
            u16HTotal /= 2;
            u16InputVFreq *=2 ;
        }
        else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1920x540)
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
            {
                u16VTotal = u16VTotal;
                u16InputVFreq = u16InputVFreq;
            }
            else
            {
                u16VTotal = u16VTotal/2;
                u16InputVFreq *=2 ;
            }
        }
        else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1280x720)
        {
            u16InputVFreq *=2 ;
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "g_XC_Pnl_Misc.FRCInfo.u83D_FI_out=%u\n",
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out);

        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16HTotal=%u, u16VTotal=%u, u16InputVFreq=%u\n",
            u16HTotal, u16VTotal, u16InputVFreq);
    }
    MDrv_SC_set_fpll(pInstance, DISABLE, ENABLE, 1);
    MDrv_Scaler_SetPhaseLimit(pInstance, 1);

    u16OutputVfreqAfterFRC = MDrv_SC_Cal_FRC_Output_Vfreq(pInstance, u16InputVFreq,
                                              u16DefaultVFreq,
                                              gSrcInfo[eWindow].bFBL,
                                              &u8FRC_In,
                                              &u8FRC_Out,
                                              &bEnableFPLL,
                                              1,
                                              eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@========\n")
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetFlock: u16InputVFreq = %u,u16OutputVfreqAfterFRC = %u\n",
                                                   u16InputVFreq, u16OutputVfreqAfterFRC)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetFlock: u8FRC_In = %u,u8FRC_Out = %u, bEnableFPLL=%x\n",
                                                   u8FRC_In, u8FRC_Out, bEnableFPLL)

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@========\n")


    if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)&& (!MDrv_SC_GetSkipWaitVsync(pInstance,eWindow)) )
    {

#if SC2_ENABLE
        _MLOAD_ENTRY(pInstance);
#else
        _MLOAD_ENTRY();
#endif //#if SC2_ENABLE


        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16VTotal - 1)&0xFFF, 0xFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,(u16HTotal - 1), 0x1FFF);


        // 4K2K 60 out case
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K)
        {
            // Kepp OP timing as 4K2K, DE for bk10 is 4Kx2K
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), 0x0FFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1), 0x0FFF);  // DE V end
        }
        else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
        {
            // Kepp OP timing as 4K 0.5K, DE for bk10 is 4Kx0.5K
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), 0x0FFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/4 - 1), 0x0FFF);  // DE V end
        }
        else if ((u16VTotal > 2000) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
        {
            //if ((gSrcInfo[eWindow].u16InputVFreq != 0) && (u16InputVFreq > 450))
            // The condition above cause H scale problem on both video & OSD with FHD 24/25/30Hz
            // We need more restrict & accurate condition to tell the different between UHD & FHD input.
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
            {
                // FHD in, DE for bk10 is 2Kx2K
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 - 1), 0x0FFF);
            }
            else
            {
                // 4K2K in, DE for bk10 is 4Kx2K
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), 0x0FFF);
            }
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1), 0x0FFF);  // DE V end
        }

        MDrv_XC_MLoad_Fire(pInstance, TRUE);

#if SC2_ENABLE
        _MLOAD_RETURN(pInstance);
#else
        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    }
    else
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L,(u16VTotal - 1)&0xFFF);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L,(u16HTotal - 1));

        // 4K2K 60 out case
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K)
        {
            // Keep OP timing as 4K2K, DE for bk10 is 4Kx2K
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1));
        }
        else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
        {
            // Kepp OP timing as 4K 0.5K, DE for bk10 is 4Kx0.5K
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/4 - 1));  // DE V end
        }
        else if ((u16VTotal > 2000) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
            {
                // FHD in, DE for bk10 is 2Kx2K
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 - 1));
            }
            else
            {
                // 4K2K in, DE for bk10 is 4Kx2K
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1));
            }
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1));
        }
    }

    u32DClk = u16HTotal * u16VTotal * u16OutputVfreqAfterFRC;
    if((u16InputVFreq >= 235 )&&(u16InputVFreq <= 245 ))
        MDrv_Scaler_SetPhaseLock_Thresh(0x0F, 1);
    else
        MDrv_Scaler_SetPhaseLock_Thresh(0x02, 1);

    _MHal_SC_Flock_Caculate_LPLLSet(u32DClk);
    _MHal_SC_Flock_Set_IGainPGain(pInstance, u8FRC_Out, u16OutputVfreqAfterFRC, u16HTotal, u16VTotal);
    _MHal_SC_Set_LPLL_Limit(u32LpllLimitHigh, u32LpllLimitLow, 1);

    MDrv_SC_set_fpll(pInstance, bEnableFPLL, !bEnableFPLL, 1);

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Input Freq = %u, After FRC = %u\n\n", u16InputVFreq, u16OutputVfreqAfterFRC);

    gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC = u16OutputVfreqAfterFRC;


    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out != E_XC_3D_OUTPUT_FI_MODE_NONE)
    {
        Hal_XC_FRC_R2_Set_Timing((u16OutputVfreqAfterFRC+5)/10, (gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC+5)/10);
    }
    else
    {
        Hal_XC_FRC_R2_Set_Timing((u16OutputVfreqAfterFRC+5)/10, (gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC+5)/10);
    }

}

#endif

//-------------------------------------------------------------------------------------------------
/// Set the FPLL mode:
/// @param  bTrue      \b IN: True: FSM mode, False: Non FSM mode
//-------------------------------------------------------------------------------------------------

void MApi_SC_Enable_FPLL_FSM_U2(void* pInstance, MS_BOOL bTrue)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Setting = bTrue;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Setting = bTrue;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


}

void MApi_SC_Enable_FPLL_FSM(MS_BOOL bTrue)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_FSM_ENABLE XCArgs;
    XCArgs.bTrue = bTrue;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_FSM_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set the FPLL Thresh mode:
/// @param  bEnable      \b IN: TRUE: Thresh mode, FALSE: Non-Thresh mode
//-------------------------------------------------------------------------------------------------

void MApi_XC_Enable_FPLL_Thresh_Mode_U2(void* pInstance, MS_BOOL bEnable)
{
#if FPLL_THRESH_MODE_SUPPORT
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode = bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode = bEnable;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


#endif
}

void MApi_XC_Enable_FPLL_Thresh_Mode(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_THRESHMODE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_THRESHMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get the FPLL Thresh mode:
/// @return TRUE: Thresh mode, FALSE: Non-Thresh mode
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Get_FPLL_Thresh_Mode_U2(void* pInstance)
{
#if FPLL_THRESH_MODE_SUPPORT
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
#else
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Get_FPLL_Thresh_Mode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_FPLL_THRESHMODE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FPLL_THRESHMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Setup panel timing (output DCLK)/FRC/FramePLL, and enable input source
/// @param  pTimingInfo      \b IN: the information of the timing to set @ref XC_SetTiming_Info
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetPanelTiming_U2(void* pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);


    if(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("MApi_XC_EX_SetPanelTiming() -> Not support for device id %ld\n", psXCInstPri->u32DeviceID);
        return;
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
#endif //#if SC2_ENABLE



    #ifdef ENABLE_4K2K_PATCH
    if(MDrv_Read2Byte(L_BK_LPLL(0x10))<= 0x20)
    {

#if SC2_ENABLE
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
        _XC_RETURN();
        _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

        return;
    }
    #endif

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8LPLL_Type = 0;
    u8LPLL_Type =(MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type;

    if(pTimingInfo->u32HighAccurateInputVFreq == 0)
    {
        if(IsSrcTypeDigitalVD(gSrcInfo[eWindow].enInputSourceType))
        {
            pTimingInfo->u32HighAccurateInputVFreq = MDrv_XC_GetAccurateVFreqx1K(pInstance, eWindow);
        }
        else if(IsSrcTypeAnalog(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType))
        {
            pTimingInfo->u32HighAccurateInputVFreq = MDrv_XC_PCMonitor_Get_VFreqx1K(pInstance, eWindow);
        }
        else
        {
            pTimingInfo->u32HighAccurateInputVFreq = pTimingInfo->u16InputVFreq * 100;
        }
    }
		
    if(abs(pTimingInfo->u16InputVFreq * 100 - pTimingInfo->u32HighAccurateInputVFreq) > ACCURATE_VFREQ_TOLERANCE)
    {
        printf("[%s,%5d]Attention! using u16InputVFreq=%u instead of u32HighAccurateInputVFreq=%lu\n", __FUNCTION__, __LINE__, pTimingInfo->u16InputVFreq, pTimingInfo->u32HighAccurateInputVFreq);
        pTimingInfo->u32HighAccurateInputVFreq = pTimingInfo->u16InputVFreq * 100;
    }

    if((pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Setting) && (!gSrcInfo[eWindow].bFBL) && (!gSrcInfo[eWindow].bR_FBL))
    {
        //pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE; //FSM will restart, so clear FSM status to idle
        if (pXCResourcePrivate->stdrvXC_Display._enFpllFsm !=FPLL_IDLE_STATE)
        {
            MS_U8 u8timeout = 15;
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
    #if SC2_ENABLE
            _XC_RETURN(pInstance);
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    
    #else
            _XC_RETURN();
            _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    
    #endif //#if SC2_ENABLE
            while (u8timeout > 2)
            {
                MsOS_DelayTask(5);
                if (pXCResourcePrivate->stdrvXC_Display.u8killframelock == 1)
                {
                    u8timeout = 2;
                    break;
                }
                u8timeout--;
            }
    #if SC2_ENABLE
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        _XC_ENTRY(pInstance);
    #else
        _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
        _XC_ENTRY();
    #endif //#if SC2_ENABLE            
        }
        pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working = TRUE;
        pXCResourcePrivate->stdrvXC_Display.u8killframelock = 0;
        MApi_XC_SetPanelTiming_FSM(pInstance, pTimingInfo, eWindow);
        if (!pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode)
        {
            if(gSrcInfo[eWindow].bEnableFPLL)
            {
                //FSM enable fpll, then enable ISR
                if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM))
                {
                    MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM);
                }
            }
            else
            {
                //FSM disable FPLL, then disable ISR
                if(TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM))
                {
                    MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM);
                }
            }
        }
        else
        {
            if(gSrcInfo[eWindow].bEnableFPLL)
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode = FPLL_THRESH_MODE_LOCKED_STATE;
                pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
                pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
                #if ENABLE_ENABLE_OPW_WITH_FRAMELOCK
                    Hal_SC_SetOPWriteOff(pInstance, ENABLE);
                #endif
                //FSM enable fpll, then enable ISR
                if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL))
                {
                    MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL);
                }
            }
            else
            {
                //FSM disable FPLL, then disable ISR
                if(TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL))
                {
                    MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL);
                }
            }
        }
    }
    else
    {
        if(pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working &&
           (TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM)))
        {
            //State change from FSM to NON-FSM, disable ISR
            MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM);
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
        }
        pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working= FALSE;
        MApi_XC_SetPanelTiming_NonFSM(pInstance, pTimingInfo, eWindow);
    }
#if FRC_INSIDE
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        MApi_XC_SetFLock(pInstance, pTimingInfo, eWindow);
    }
    else
    {
        gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC;
    }
#else
    gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC;
#endif

    if((u8LPLL_Type >= E_XC_PNL_LPLL_VBY1_10BIT_4LANE)&&(u8LPLL_Type <= E_XC_PNL_LPLL_VBY1_8BIT_8LANE))
    {

	
        if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_EXTERNAL_URSA))
	{
		MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0, BIT(15));
		MsOS_DelayTask(1);
		MOD_W2BYTEMSK(REG_MOD_BK00_62_L, BIT(15), BIT(15));
	}
	
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


}

void MApi_XC_SetPanelTiming(XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_PANEL_TIMING XCArgs;
    XCArgs.pTimingInfo = pTimingInfo;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_PANEL_TIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_XC_SetFrameColor(void *pInstance, MS_U32 u32aRGB )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32FrameColor_Suspend = u32aRGB;
    Hal_XC_SetFrameColor(pInstance, u32aRGB);
}

//-------------------------------------------------------------------------------------------------
/// set free run(Disable input chanel) color
/// @param  u32aRGB
/// -  0:Black
/// -  1:White
/// -  2:Blue
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetFrameColor_U2 ( void* pInstance, MS_U32 u32aRGB )
{
    //MS_U8 u8Bank;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

    _XC_ENTRY(pInstance);
#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

    _XC_ENTRY();
#endif //#if SC2_ENABLE


    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //MDrv_WriteByte(H_BK_VOP(0x19), (MS_U8) (u32aRGB >> 16));                ///< R
    //MDrv_WriteByte(L_BK_VOP(0x1A), (MS_U8) (u32aRGB >> 8));                 ///< G
    //MDrv_WriteByte(H_BK_VOP(0x1A), (MS_U8) u32aRGB);                        ///< B
    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L,  (u32aRGB >> 8),0xFF00);            ///< R
    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1A_L,  (u32aRGB >> 8),0x00FF);            ///< G
    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1A_L,  (u32aRGB << 8),0xFF00);            ///< B
    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
    MDrv_XC_SetFrameColor(pInstance, u32aRGB);

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


}

void MApi_XC_SetFrameColor ( MS_U32 u32aRGB )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRAME_COLOR XCArgs;
    XCArgs.u32aRGB = u32aRGB;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRAME_COLOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

/******************************************************************************/
///set free run display window size
/******************************************************************************/
void MDrv_SC_set_freerun_window (void *pInstance)
{
    //MS_U8 u8Bank;

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);

    /*MDrv_Write2Byte(L_BK_VOP(0x08), g_XC_InitData.stPanelInfo.u16HStart); // Image H start
    MDrv_Write2Byte(L_BK_VOP(0x09), g_XC_InitData.stPanelInfo.u16HStart + g_XC_InitData.stPanelInfo.u16Width - 1); // Image H end
    MDrv_Write2Byte(L_BK_VOP(0x0A), g_XC_InitData.stPanelInfo.u16VStart + g_XC_InitData.stPanelInfo.u16Height - 1); // Image V end
    MDrv_Write2Byte(L_BK_VOP(0x0B), g_XC_InitData.stPanelInfo.u16VStart + g_XC_InitData.stPanelInfo.u16Height - 1); // DE V end*/

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart); // Image H start
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1); // Image H end
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1); // Image V end
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1); // DE V end*/

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

//-----------------------------------------------------------------------------
// frame lock
//-----------------------------------------------------------------------------
static void MDrv_Scaler_SetFrameLock(void *pInstance, MS_BOOL bSignal)
{
    //MS_U8 u8Bank;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16OutputVtotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal;

    MS_BOOL bFramelockEnable = DISABLE;
    MS_BOOL bEnFreerunOutput = DISABLE;
    MS_BOOL bUseMainFB = !gSrcInfo[0].bFBL;

    MS_BOOL bSetFPLL = DISABLE;

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if (bSignal == FALSE) // No Signal (Pure Freerun Mode)
    {
        u16OutputVtotal -= 1;
        bEnFreerunOutput = ENABLE;
    }
    else // Have Signal
    {
        if (bUseMainFB)
        {
            bSetFPLL = ENABLE;
            u16OutputVtotal -= 1;
            bEnFreerunOutput = ENABLE;

            bFramelockEnable = ENABLE;
        }
        else
        {
            bSetFPLL = DISABLE;
            u16OutputVtotal -= 1;
            bEnFreerunOutput = ENABLE;

            bFramelockEnable = DISABLE;
        }
    }

    /* set FPLL */
    MDrv_SC_set_fpll(pInstance, bSetFPLL, DISABLE, 0);
#if FRC_INSIDE
    MDrv_SC_set_fpll(pInstance, bSetFPLL, DISABLE, 1);
#endif

    /* enable output free-run */
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //MDrv_WriteByte(H_BK_VOP(0x1C), bEnFreerunOutput);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1C_L, bEnFreerunOutput ? BIT(8): 0,BIT(8));
    /* Enable/Disable FrameLock */
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    //MDrv_WriteRegBit(H_BK_IP1F2(0x03), bFramelockEnable, BIT(7));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, bFramelockEnable ? BIT(15): 0,BIT(15));

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

static void MDrv_XC_SetFrameLock_WithoutSEMAPHORE(void *pInstance,MS_BOOL bFramelock)
{

    MS_BOOL bFramelockEnable = DISABLE;
    MS_BOOL bEnFreerunOutput = DISABLE;
    MS_U32 u32OrigLPLL = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bSetFPLL = DISABLE;

    if (bFramelock == FALSE) // No Signal (Pure Freerun Mode)
    {
        bEnFreerunOutput = ENABLE;
    }
    else // Have Signal
    {
        bSetFPLL = DISABLE;
        bEnFreerunOutput = ENABLE;
        bFramelockEnable = DISABLE;
    }

    // store bank
    LPLL_BK_STORE;

    // switch bank
    // BANK 0
    LPLL_BK_SWITCH(0x00);
    u32OrigLPLL = MDrv_Read4Byte(L_BK_LPLL(0x0F));

    if(bSetFPLL)
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x0C), BIT(3), BIT(3));
    }
    else
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x0C), 0x00, BIT(3));
    }
    MDrv_Write2ByteMask(L_BK_LPLL(0x0C), 0, BIT(6));

    // !!Initial set have to re-update again to force LPLL free run at initial state or
    // lock from initial set!!
    MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL);

    // switch bank
    // BANK 1
    LPLL_BK_SWITCH(0x01);
    u32OrigLPLL = MDrv_Read4Byte(L_BK_LPLL(0x0F));

    if(bSetFPLL)
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x0C), BIT(3), BIT(3));
        MDrv_Write2ByteMask(L_BK_LPLL(0x7F), 0x00, BIT(8));
    }
    else
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x7F), BIT(8), BIT(8));
        MDrv_Write2ByteMask(L_BK_LPLL(0x0C), 0x00, BIT(3));
    }
    MDrv_Write2ByteMask(L_BK_LPLL(0x0C), 0, BIT(6));

    // !!Initial set have to re-update again to force LPLL free run at initial state or
    // lock from initial set!!
    MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL);

    // restore bank
    LPLL_BK_RESTORE;

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK10_1C_L, bEnFreerunOutput ? BIT(8): 0,BIT(8));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK01_03_L, bFramelockEnable ? BIT(15): 0,BIT(15));
}

//-------------------------------------------------------------------------------------------------
/// set free run LPLL without SEMAPHORE
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetFreeRunTiming_WithoutSEMAPHORE_U2(void* pInstance)
{
    MDrv_XC_SetFrameLock_WithoutSEMAPHORE(pInstance,FALSE);
}

void MApi_XC_SetFreeRunTiming_WithoutSEMAPHORE(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FREERUN_TIMING_WITHOUTSEMAPHORE, NULL) != UTOPIA_STATUS_SUCCESS)
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
/// set free run display window timing
//-------------------------------------------------------------------------------------------------
void MDrv_XC_SetFreeRunTiming (void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    if(0)//(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support FPLL for device id %ld\n",__FUNCTION__,psXCInstPri->u32DeviceID);
        printf("MDrv_XC_SetFreeRunTiming() -> Not support FPLL for device id %ld\n", psXCInstPri->u32DeviceID);
        return;
    }
#else
    if(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support FPLL for device id %ld\n",__FUNCTION__,psXCInstPri->u32DeviceID);
        printf("MDrv_XC_SetFreeRunTiming() -> Not support FPLL for device id %ld\n", psXCInstPri->u32DeviceID);
        return;
    }
#endif //#if SC2_ENABLE


    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working &&
       (TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM)))
    {
        //FSM, disable FPLL, so disable ISR
        MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM);
        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
    }
#if VERIFY_DISPLAY_FPGA  //value setting
    MDrv_SC_set_output_dclk(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal,
                            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal,
                            120, eWindow);
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetFreeRunTiming: Disable FPLL and Clear FSM to Idle state\n");
    gSrcInfo[MAIN_WINDOW].bEnableFPLL = FALSE;
    gSrcInfo[SUB_WINDOW].bEnableFPLL = FALSE;
    MDrv_Scaler_SetFrameLock(pInstance, FALSE);

    pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = FALSE;
    pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock     = FALSE;

    switch (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)
    {
        case E_XC_PNL_LPLL_DAC_I:
        case E_XC_PNL_LPLL_DAC_P:
            //MDrv_WriteByteMask(H_BK_VOP(0x10), BIT(7)|BIT(6), 0xFF); // set output vsync manually
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, BIT(15)|BIT(14), 0xFF00); // set output vsync manually
            break;
        default:
            //MDrv_WriteByteMask(H_BK_VOP(0x10), BIT(6), 0x7F); // set output sync ctl for Freerun
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, BIT(14), 0x7F00); // set output sync ctl for Freerun
    }

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

//-------------------------------------------------------------------------------------------------
/// set free run display window timing
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetFreeRunTiming_U2 (void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_SetFreeRunTiming(pInstance);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    _XC_ENTRY();
    MDrv_XC_SetFreeRunTiming(pInstance);
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_SetFreeRunTiming (void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FREERUN_TIMING, NULL) != UTOPIA_STATUS_SUCCESS)
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
/// Setup Gamma function ON/OFF
/// @param  bEnable      \b IN: Enable or Disable
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetGammaOnOff_U2 ( void* pInstance, MS_BOOL bEnable )
{
    //MS_U8 u8Bank;

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    /*u8Bank = MDrv_ReadByte(BK_SELECT_00);
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    MDrv_WriteRegBit(L_BK_VOP(0x50), bEnable, BIT(0));
    MDrv_WriteByte(BK_SELECT_00, u8Bank);*/
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_50_L, bEnable, BIT(0));

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else
    _XC_RETURN();
#endif //#if SC2_ENABLE


}

void MApi_XC_SetGammaOnOff ( MS_BOOL bEnable )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_GAMMA_ONOFF XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_GAMMA_ONOFF, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_SC_set_output_dclk(void *pInstance, MS_U16 u16Htt, MS_U16 u16Vtt, MS_U16 u16VFreq, MS_U32 u32HighAccurateVFreq, SCALER_WIN eWindow)
{
    MS_U32 u32UpBound, u32LowBound, u32Steps = 10, u32DelayMs = 0; //delaytime=0 means wait 2vsync as the old logic
    XC_PANEL_INFO_EX *pPanelInfoEx;
    MS_U32 u32PllSet = 0, u32HighAccuratePllSet = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U64 u64DClk = (MS_U64)u16Htt * u16Vtt * u16VFreq;
    MS_U64 u64HighAccurateDClk = 0;
    MS_U16 u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    MS_U16 u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;
    MS_U32 u32DefaultDClk = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal
                            * pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal
                            * pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
    MS_U32 u32AdjHighAccurateVFreq = u32HighAccurateVFreq;

    if (u32HighAccurateVFreq == 0)
    {
        u32AdjHighAccurateVFreq = (MS_U32)u16VFreq * 100;
    }

    u32DefaultDClk = u16Htt_out * u16Vtt_out * pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;

    pPanelInfoEx = NULL;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk u16VFreq=%u ***\n", u16VFreq);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk u32AdjHighAccurateVFreq=%lu ***\n", u32AdjHighAccurateVFreq);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "DefaultPanelInfo: MinSET = 0x%lx, MaxSET = 0x%lx\n",
                                        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MinSET,
                                        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MaxSET);

    MS_U32 u32Factor = MHal_SC_Get_LpllSet_Factor(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode,
        (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type,u32DefaultDClk);

    MS_U32 u32Factor_Denom = MHal_SC_Get_LpllSet_Factor_denominator(pInstance, 
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode,
        (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type,u32DefaultDClk);

    MS_U32 u32LPLLSet_Div = 1;

#if (HW_DESIGN_4K2K_VER == 4)
    u32LPLLSet_Div = MHal_SC_Get_LpllSet_Div(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode,
        (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type,u32DefaultDClk);


    if (u32LPLLSet_Div == 0)
        u32LPLLSet_Div = 1; // To avoid /0 case

#endif


    u32UpBound  = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MaxSET * u32Factor / u32Factor_Denom / u32LPLLSet_Div;
    u32LowBound = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MinSET * u32Factor / u32Factor_Denom / u32LPLLSet_Div;


    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "g_XC_InitData.stPanelInfo.eLPLL_Type.eLPLL_Type=%u, g_XC_InitData.stPanelInfo.eLPLL_Type.eLPLL_Mode=%u\n",
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode)

    if((pXCResourcePrivate->stdrvXC_Display._bForceFreerun == FALSE)
        && _MDrv_XC_GetExPanelInfo(pInstance, &pPanelInfoEx, u16VFreq) && (pPanelInfoEx != NULL))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "PanelInfoEx assigned\n")
        if(pPanelInfoEx->bDClkValid)
        {
            u64DClk = pPanelInfoEx->u32DClk*10000; //Dclk hz * 10
            if( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_MINILVDS) ||
                (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_ANALOG_MINILVDS) ||
                (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_DIGITAL_MINILVDS))
            {
                u32DefaultDClk = (MS_U32) u64DClk; //For minilvds
            }
        }
        //LowBound for MaxCLK, UpBound for MinCLK
        if(pPanelInfoEx->bMaxDClkValid)
        {
            u32LowBound = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)pPanelInfoEx->u32MaxDClk*10000, FALSE); //Dclk hz * 10
        }
        if(pPanelInfoEx->bMinDClkValid)
        {
            u32UpBound = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)pPanelInfoEx->u32MinDClk*10000, FALSE); //Dclk hz * 10
        }
        if(pPanelInfoEx->bDclkDelayValid)
        {
            u32DelayMs = (MS_U32)pPanelInfoEx->u16DclkDelay;
        }
        if(pPanelInfoEx->bDclkStepsValid)
        {
            u32Steps = (MS_U32)pPanelInfoEx->u16DclkSteps;
        }
    }

    u64HighAccurateDClk = (MS_U64)u32AdjHighAccurateVFreq * u16Htt * u16Vtt;

#if 0
    // No matter what, using high accurate VFreq to generate corresponding high accurate LPLL Set
    // is always desirable.
    if (!(gSrcInfo[eWindow].bFBL || gSrcInfo[eWindow].bFBL))
    {
        u32HighAccuratePllSet = MDrv_PNL_CalculateLPLLSETbyDClk(u64HighAccurateDClk, TRUE);
    }
    else
    {
        u32PllSet = MDrv_PNL_CalculateLPLLSETbyDClk(u64DClk, FALSE);
    }
#else
    u32PllSet = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, u64DClk, FALSE);
    u32HighAccuratePllSet = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, u64HighAccurateDClk, TRUE);
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32PllSet = 0x%lx, u32UpBound = 0x%lx, u32LowBound = 0x%lx\n", u32PllSet, u32UpBound, u32LowBound)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32HighAccuratePllSet = 0x%lx, u32UpBound = 0x%lx, u32LowBound = 0x%lx\n", u32HighAccuratePllSet, u32UpBound, u32LowBound)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "User forces Enable=%u, LPLL limit=0x%lx\n"
                                      , pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_D5D6D7]
                                      , pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_D5D6D7])
    if(pXCResourcePrivate->stdrvXC_Display._bForceFreerun == FALSE)
    {
       HAL_SC_Set_FPLL_Limit(pInstance, &u32HighAccuratePllSet
                             , u32LowBound
                             , u32UpBound
                             , pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_D5D6D7]
                             , pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_D5D6D7]);
    }

    // just first patch for TCON case
    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_DCLK) ||
       ((gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_VTOTAL)) ||
       ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type != E_XC_PNL_LPLL_MINILVDS) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type != E_XC_PNL_LPLL_ANALOG_MINILVDS) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type != E_XC_PNL_LPLL_DIGITAL_MINILVDS)))
    {
      	if((((E_XC_PNL_LPLL_EXT_TYPE) pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type) >= E_XC_PNL_LPLL_VBY1_10BIT_4LANE) &&
           (((E_XC_PNL_LPLL_EXT_TYPE) pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type) <= E_XC_PNL_LPLL_VBY1_8BIT_8LANE))
        {
            //2k1k vb1 8L 10bits if use accurate it overflow when calculate lpll set
            // temprority don't use accurate for 2k1k first
           /* if(u16Vtt<=1700 && u16Htt<=2500)
            {
                 u32PllSet = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)u64DClk, FALSE);
                MDrv_SC_Set_Output_Dclk_Slowly(pInstance, u32PllSet, u32Steps, u32DelayMs, eWindow);
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk-MINILVDS CLK=%lu, Set=0x%X***\n", u32DefaultDClk, (unsigned int)u32PllSet)
            }
			else*/
			{
                _MDrv_SC_Set_Output_Dclk(u32HighAccuratePllSet);
			}
        }
        else
        {
        MDrv_SC_Set_Output_Dclk_Slowly(pInstance, u32HighAccuratePllSet, u32Steps, u32DelayMs, eWindow);
		}
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk-Normal CLK=%llu, Set=0x%X***\n", u64DClk, (unsigned int)u32HighAccuratePllSet)
    }
    else
    {
        u32PllSet = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)u32DefaultDClk, FALSE);
        MDrv_SC_Set_Output_Dclk_Slowly(pInstance, u32PllSet, u32Steps, u32DelayMs, eWindow);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk-MINILVDS CLK=%lu, Set=0x%X***\n", u32DefaultDClk, (unsigned int)u32PllSet)
    }
}

//Obsolte function, do not use it any more
MS_U32 MApi_XC_Get_Current_OutputVFreqX100_U2(void* pInstance)
{
#if 0
    MS_U32 u32OutFrameRatex100 = 0;  //OutVFreq*100

    MS_U32 u32PllSet= 0; // 3BYTE
    MS_U16 u16Htt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L, 0x1FFF);
    MS_U16 u16Vtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, 0xFFF);
    MS_U64 u64tmp = 0;

    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    u32PllSet= MDrv_Read4Byte(L_BK_LPLL(0x0F)) & 0xFFFFFF; // 3BYTE
    LPLL_BK_RESTORE;
    if((g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_MINILVDS) || (g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_ANALOG_MINILVDS) || (g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_DIGITAL_MINILVDS))
    {
//        u32OutFrameRatex100 = (MS_U32)((((MS_U64)LVDS_MPLL_CLOCK_MHZ * 524288 * 2) *100000000)/((MS_U64)(u16Htt * u16Vtt) * u32PllSet));
//**NOTE! In 2.6.28.9 Linux Kernel does not support 64bit division**
//        u32OutFrameRatex100 = (MS_U32)((((MS_U64)(LVDS_MPLL_CLOCK_MHZ * 524288) * 2 * 100000000)/(u16Htt * u16Vtt))/ u32PllSet);
        u64tmp = ((MS_U64)LVDS_MPLL_CLOCK_MHZ * 524288 * 2) * 100000000;
        do_div(u64tmp,(MS_U64)(u16Htt * u16Vtt));
        do_div(u64tmp,(u32PllSet));
        u32OutFrameRatex100 = u64tmp;
    }
    else
    {
        if( g_XC_InitData.stPanelInfo.u8LPLL_Mode )
        {
            //u32OutFrameRatex100 = (MS_U32)((((MS_U64)(LVDS_MPLL_CLOCK_MHZ * 524288 * LPLL_LOOPGAIN) * 2 * 100000000)/(u16Htt * u16Vtt * 7))/ u32PllSet);
            u64tmp = ((MS_U64)LVDS_MPLL_CLOCK_MHZ * 524288 * LPLL_LOOPGAIN * 2) * 100000000;
            do_div(u64tmp,(u16Htt * u16Vtt * 7));
            do_div(u64tmp,(u32PllSet));
            u32OutFrameRatex100 = u64tmp;

        }
        else
        {
//            u32OutFrameRatex100 = (MS_U32)((((MS_U64)(LVDS_MPLL_CLOCK_MHZ * 524288 * LPLL_LOOPGAIN) * 100000000)/(u16Htt * u16Vtt * 7))/ u32PllSet);
            u64tmp = ((MS_U64)LVDS_MPLL_CLOCK_MHZ * 524288 * LPLL_LOOPGAIN) * 100000000;
            do_div(u64tmp,(u16Htt * u16Vtt * 7));
            do_div(u64tmp,(u32PllSet));
            u32OutFrameRatex100 = u64tmp;
        }
    }
    return u32OutFrameRatex100;
#else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MS_U32 u32OutputVFreqX100 = 0;
    u32OutputVFreqX100 = MDrv_SC_GetOutputVFreqX100(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MS_U32 u32OutputVFreqX100 = 0;
    u32OutputVFreqX100 = MDrv_SC_GetOutputVFreqX100(pInstance);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return u32OutputVFreqX100;
#endif
}

MS_U32 MApi_XC_Get_Current_OutputVFreqX100(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_CURRENT_OUTPUTVFREQX100 XCArgs;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CURRENT_OUTPUTVFREQX100, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Enable or disable IP auto coast
/// @param  bEnable                \b IN: Enable or Disable
//-------------------------------------------------------------------------------------------------
void MDrv_XC_EnableIPAutoCoast(void *pInstance, MS_BOOL bEnable)
{
    /*MS_U8 u8Bank;
    u8Bank = MDrv_ReadByte(BK_SELECT_00);
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    MDrv_WriteRegBit(L_BK_IP1F2(0x0F), bEnable, BIT(7));
    MDrv_WriteByte(BK_SELECT_00, u8Bank);*/
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, bEnable? BIT(7):0, BIT(7));
}

void MApi_XC_EnableIPAutoCoast_U2(void* pInstance, MS_BOOL bEnable)
{

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    MDrv_XC_EnableIPAutoCoast(pInstance, bEnable);
    _XC_RETURN(pInstance);
#else
    _XC_ENTRY();
    MDrv_XC_EnableIPAutoCoast(pInstance, bEnable);
    _XC_RETURN();
#endif //#if SC2_ENABLE


}

void MApi_XC_EnableIPAutoCoast(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_IPAUTOCOAST XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IPAUTOCOAST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Enable IP auto coast Debounce
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableIPCoastDebounce_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bDisableCoastDebounce = 0;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bDisableCoastDebounce = 0;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_EnableIPCoastDebounce(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IPCOAST_DEBOUNCE_ENABLE, NULL) != UTOPIA_STATUS_SUCCESS)
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
/// Clear IP auto coast status
//-------------------------------------------------------------------------------------------------
void MApi_XC_ClearIPCoastStatus_U2(void* pInstance)
{
    MS_U16 u16Tmp;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE



    u16Tmp = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L);
    if (u16Tmp & BIT(6))
    {
        pXCResourcePrivate->stdrvXC_Display._u8CoastClearDebounce++;
        //MDrv_WriteByte(L_BK_IP1F2(0x0F), u8Tmp & ~(BIT(6)));
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, u16Tmp & ~(BIT(6)));
        if(pXCResourcePrivate->stdrvXC_Display._u8CoastClearDebounce>3)
        {
            if (pXCResourcePrivate->stdrvXC_Display._bDisableCoastDebounce)
            {
                LPLL_BK_STORE;
                LPLL_BK_SWITCH(0x00);
                MDrv_WriteRegBit(L_BK_LPLL(0x0C), DISABLE, BIT(3));
                LPLL_BK_RESTORE;
            }
            else
            {
                MDrv_XC_wait_output_vsync(pInstance, 1, 50, MAIN_WINDOW);
                Hal_SC_ip_software_reset(pInstance, REST_IP_F2, MAIN_WINDOW);
                //MDrv_WriteByte(L_BK_IP1F2(0x0F), u8Tmp & ~(BIT(6)));
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, u16Tmp & ~(BIT(6)));
                pXCResourcePrivate->stdrvXC_Display._u8CoastClearDebounce=0;
                pXCResourcePrivate->stdrvXC_Display._bDisableCoastDebounce = 1;
            }
        }
    }
    else
    {
        MDrv_WriteRegBit(L_BK_LPLL(0x0C), ENABLE, BIT(3));
        pXCResourcePrivate->stdrvXC_Display._u8CoastClearDebounce=0;
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


}

void MApi_XC_ClearIPCoastStatus(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CLEAR_IPCOAST_STATUS, NULL) != UTOPIA_STATUS_SUCCESS)
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
/// Query the capability of scaler about source to Video encoder(VE)
/// @param  OutputCapability              \b OUT: Which part of scaler can output data to Video Encoder (VE).
///Check E_XC_SOURCE_TO_VE for capability bit. Take an example, if bit0 of OutputCapability equal to 1,
///Scaler can output data to ve from its E_XC_IP(IP)
///@return @ref MS_BOOL True if query success.
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SupportSourceToVE_U2(void* pInstance, MS_U16* pOutputCapability)
{
    return Hal_SC_support_source_to_ve(pInstance, pOutputCapability);
}

E_APIXC_ReturnValue MApi_XC_SupportSourceToVE(MS_U16* pOutputCapability)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_SUPPORT_SOURCE_TO_VE XCArgs;
    XCArgs.pOutputCapability = pOutputCapability;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SUPPORT_SOURCE_TO_VE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_SetOutputCapture_U2(void* pInstance, MS_BOOL bEnable,E_XC_SOURCE_TO_VE eSourceToVE)
{
    E_APIXC_ReturnValue ret;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
#endif //#if SC2_ENABLE


    ret = Hal_SC_set_output_capture_enable(pInstance, bEnable,eSourceToVE);
    if(bEnable)
    {
        gSrcInfo[MAIN_WINDOW].Status2.eSourceToVE = eSourceToVE;
    }
    else
    {
        gSrcInfo[MAIN_WINDOW].Status2.eSourceToVE = E_XC_NONE;
    }

#if SC2_ENABLE
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_RETURN();
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return ret;
}

E_APIXC_ReturnValue MApi_XC_SetOutputCapture(MS_BOOL bEnable,E_XC_SOURCE_TO_VE eSourceToVE)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OUTPUT_CAPTURE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eSourceToVE = eSourceToVE;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OUTPUT_CAPTURE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MApi_SC_get_disp_de_window_U2(void* pInstance, MS_WINDOW_TYPE *pWin)
{
    Hal_SC_get_disp_de_window(pInstance,pWin);
    return;
}

void MApi_SC_get_disp_de_window(MS_WINDOW_TYPE *pWin)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_GET_DE_WINDOW XCArgs;
    XCArgs.psWin = pWin;
    XCArgs.eWindow = MAIN_WINDOW;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DISP_DE_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}




void MDrv_SC_set_csc(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    Hal_SC_set_csc(pInstance, bEnable, eWindow );
}
void MApi_XC_SetCsc_U2( void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    MDrv_SC_set_csc(pInstance, bEnable,  eWindow );
    return ;
}

void MApi_XC_SetCsc( MS_BOOL bEnable, SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_CSC XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CSC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_SC_rep_window(void *pInstance, MS_BOOL bEnable,MS_WINDOW_TYPE Win,MS_U8 u8Color)
{
    Hal_SC_set_rep_window(pInstance, bEnable,Win.x,Win.y,Win.width,Win.height,u8Color);
}

//-------------------------------------------------------------------------------------------------
/// update display window registers with input window
/// @param  eWindow                          \b IN: Main or sub window
/// @param  pdspwin                          \b IN: window info that will be setted to registers
//-------------------------------------------------------------------------------------------------
void MDrv_XC_set_dispwin_to_reg(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin)
{
    Hal_SC_set_disp_window(pInstance, eWindow, pdspwin);
}

//-------------------------------------------------------------------------------------------------
/// update all window registers with input window(Including DE window, and try use MLOAD)
/// @param  pDEWin                          \b IN: Panel DE window info that will be setted to registers
/// @param  pMainWin                        \b IN: Main display window info that will be setted to registers
/// @param  pSubWin                         \b IN: Sub display window info that will be setted to registers
/// Notes: if one of the pxxWin=NULL, the related window will not be setted
//-------------------------------------------------------------------------------------------------
void MDrv_XC_UpdateWindow(void *pInstance, MS_WINDOW_TYPE *pDEWin, MS_WINDOW_TYPE *pMainWin, MS_WINDOW_TYPE *pSubWin, MS_WINDOW_TYPE *pReportWin, MS_WINDOW_TYPE *pNewOSDWin)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {

#if SC2_ENABLE
        _MLOAD_ENTRY(pInstance);
#else
        _MLOAD_ENTRY();
#endif //#if SC2_ENABLE


        // Panel DE window
        if(pDEWin != NULL)
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_04_L, pDEWin->x, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pDEWin->x+pDEWin->width-1), 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_06_L, pDEWin->y, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_07_L, (pDEWin->y+pDEWin->height-1), 0xFFFF);
            memcpy(&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo), pDEWin, sizeof(MS_WINDOW_TYPE));
            memcpy(&(gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), pDEWin, sizeof(MS_WINDOW_TYPE));
        }
        // Main Display window
        if(pMainWin != NULL)
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_08_L, pMainWin->x, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_09_L, (pMainWin->x+pMainWin->width-1), 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0A_L, pMainWin->y, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0B_L, (pMainWin->y+pMainWin->height-1), 0xFFFF);
            memcpy(&(gSrcInfo[MAIN_WINDOW].stDispWin), pMainWin, sizeof(MS_WINDOW_TYPE));
        }
        // Sub Display window
        if(pSubWin != NULL)
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_07_L, pSubWin->x, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_08_L, (pSubWin->x+pSubWin->width-1), 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_09_L, pSubWin->y, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_0A_L, (pSubWin->y+pSubWin->height-1), 0xFFFF);
            memcpy(&(gSrcInfo[SUB_WINDOW].stDispWin), pSubWin, sizeof(MS_WINDOW_TYPE));
        }
        //Report window
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableRepWindowForFrameColor && (pReportWin != NULL))
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_42_L, pReportWin->x, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_43_L, pReportWin->x+pReportWin->width-2, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_44_L, pReportWin->y, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_45_L, pReportWin->y+pReportWin->height-1, 0xFFFF);
            memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_RepWindow, pReportWin, sizeof(MS_WINDOW_TYPE));
        }

        // Please add here for GOP ...
        if ((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, BIT(15))) && (pNewOSDWin != NULL))
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2D_10_L, pNewOSDWin->x, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2D_11_L, pNewOSDWin->x+pNewOSDWin->width-1, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2D_12_L, pNewOSDWin->y, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2D_13_L, pNewOSDWin->y+pNewOSDWin->height-1, 0xFFFF);
        }
        MDrv_XC_MLoad_Fire(pInstance, TRUE);

#if SC2_ENABLE
        _MLOAD_RETURN(pInstance);
#else
        _MLOAD_RETURN();
#endif //#if SC2_ENABLE


    }
    else
    {
        // Panel DE window
        if(pDEWin != NULL)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, pDEWin->x);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pDEWin->x+pDEWin->width-1));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L, pDEWin->y);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pDEWin->y+pDEWin->height-1));
            memcpy(&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo), pDEWin, sizeof(MS_WINDOW_TYPE));
            memcpy(&(gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), pDEWin, sizeof(MS_WINDOW_TYPE));
        }
        // Main Display window
        if(pMainWin != NULL)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pMainWin->x);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, (pMainWin->x+pMainWin->width-1));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pMainWin->y);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, (pMainWin->y+pMainWin->height-1));
            memcpy(&(gSrcInfo[MAIN_WINDOW].stDispWin), pMainWin, sizeof(MS_WINDOW_TYPE));
        }
        // Sub Display window
        if(pSubWin != NULL)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pSubWin->x);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, (pSubWin->x+pSubWin->width-1));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pSubWin->y);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, (pSubWin->y+pSubWin->height-1));
            memcpy(&(gSrcInfo[SUB_WINDOW].stDispWin), pSubWin, sizeof(MS_WINDOW_TYPE));
        }
        //Report window
        if (pReportWin != NULL)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_42_L, pReportWin->x);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_43_L, pReportWin->x+pReportWin->width-2);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_44_L, pReportWin->y);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_45_L, pReportWin->y+pReportWin->height-1);
            //Skip enable here, SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, BIT(8)|BIT(9), BIT(8)|BIT(9)); // Enable display rep window
        }

        // Please add here for GOP ...
        if (pNewOSDWin != NULL)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, pNewOSDWin->x);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2D_11_L, pNewOSDWin->x+pNewOSDWin->width-1);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2D_12_L, pNewOSDWin->y);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2D_13_L, pNewOSDWin->y+pNewOSDWin->height-1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
/// get current display window registers setting
/// @param  eWindow                          \b IN : Main or sub window
/// @param  pdspwin                          \b OUT: Pointer for ouput disp window register
//-------------------------------------------------------------------------------------------------
void MDrv_XC_get_dispwin_from_reg(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin)
{
    Hal_SC_get_disp_window(pInstance, eWindow, pdspwin);
}

//-------------------------------------------------------------------------------------------------
/// Set VOP Gain for Main Window(after 3x3 before Gamma)
/// @param  eVop_Channel                \b IN: Select VOP R/G/B
/// @param  u16Val                           \b IN: Set value
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetPreGammaGain_U2(void* pInstance, SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val)
{

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    HAL_SC_VOP_Set_Contrast_Value(pInstance, eVop_Channel, u16Val, eWindow);
    _XC_RETURN(pInstance);
#else
    _XC_ENTRY();
    HAL_SC_VOP_Set_Contrast_Value(pInstance, eVop_Channel, u16Val, eWindow);
    _XC_RETURN();
#endif //#if SC2_ENABLE


}

void MApi_XC_SetPreGammaGain(SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_PREGAMMA_GAIN XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eVop_Channel = eVop_Channel;
    XCArgs.u16Val = u16Val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_PREGAMMA_GAIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set VOP Offset for Main Window(after 3x3 before Gamma)
/// @param  eVop_Channel                \b IN: Select VOP R/G/B
/// @param  u16Val                           \b IN: Set value
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetPreGammaOffset_U2(void* pInstance, SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val)
{

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    HAL_SC_VOP_Set_Brightness_Value(pInstance, eVop_Channel, u16Val, eWindow);
    _XC_RETURN(pInstance);
#else
    _XC_ENTRY();
    HAL_SC_VOP_Set_Brightness_Value(pInstance, eVop_Channel, u16Val, eWindow);
    _XC_RETURN();
#endif //#if SC2_ENABLE


}

void MApi_XC_SetPreGammaOffset(SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_PREGAMMA_OFFSET XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eVop_Channel = eVop_Channel;
    XCArgs.u16Val = u16Val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_PREGAMMA_OFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Wait FPLL finished
/// @return @ref MS_BOOL indicate FPLL finished or not (frame lock finished or not)
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MDrv_SC_WaitFPLLDone(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(HAL_SC_WaitFPLLDone(pInstance))
    {
        if(pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR
            && pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]
            && !pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
        {
            MS_U32 u32timer = MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer;
            MS_U32 u32DelayTime = (MS_U32)_MDrv_SC_FPLL_GetMCNRDelayTime(pInstance);
            if(u32DelayTime > u32timer)
            {
                MsOS_DelayTask(u32DelayTime-u32timer); //Turn on before 8 vsync
            }
            if (!pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[MAIN_WINDOW])
            {
            #if ENABLE_ENABLE_OPW_WITH_FRAMELOCK
#if SC2_ENABLE
                _XC_ENTRY(pInstance);
                Hal_SC_SetOPWriteOff(pInstance, !pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]);
                _XC_RETURN(pInstance);
#else
                _XC_ENTRY();
                Hal_SC_SetOPWriteOff(pInstance, !pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]);
                _XC_RETURN();
#endif //#if SC2_ENABLE
            #endif

            }
            pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-------------------------------------------------------------------------------------------------
/// Wait FPLL finished
/// @return @ref MS_BOOL indicate FPLL finished or not (frame lock finished or not)
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_WaitFPLLDone_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support FPLL for device id %ld\n",__FUNCTION__,psXCInstPri->u32DeviceID);
        printf("MApi_XC_EX_WaitFPLLDone() -> Not support FPLL for device id %ld\n", psXCInstPri->u32DeviceID);
        return FALSE;
    }
    // we didn't add mutex here because
    // in T3, MDrv_SC_WaitFPLLDone() will use ISR
    // in U3, MDrv_SC_WaitFPLLDone() won't use ISR and will use mutex in HAL layer
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bReturn = FALSE;
    if(gSrcInfo[MAIN_WINDOW].bEnableFPLL)
    {
        if(pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working)
        {
            gSrcInfo[MAIN_WINDOW].bDoneFPLL = TRUE; //FSM framelock open screen directly, no need to wait.
        }
        else
        {
            gSrcInfo[MAIN_WINDOW].bDoneFPLL = _MDrv_SC_WaitFPLLDone(pInstance);
        }
    }
    else
    {
        gSrcInfo[MAIN_WINDOW].bDoneFPLL = FALSE; //Freerun, NO fpll
    }
    bReturn = gSrcInfo[MAIN_WINDOW].bDoneFPLL;

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


    return bReturn;
}

MS_BOOL MApi_XC_WaitFPLLDone(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_WAIT_FPLL_DONE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_WAIT_FPLL_DONE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get scaler output vertical frequency
/// @return @ref MS_U16 return output vertical frequency x 100
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_SC_GetOutputVFreqX100(void *pInstance)
{
    MS_U16 u16OutputVFreqAfterFRC = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    u16OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC*10;
    return u16OutputVFreqAfterFRC;
}

//-------------------------------------------------------------------------------------------------
/// Get OP1 output vertical frequency
/// @return @ref MS_U16 return output vertical frequency x 100
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_SC_GetOP1OutputVFreqX100(void *pInstance)
{
    MS_U16 u16OP1_OutputVFreqAfterFRC = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if FRC_INSIDE
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        u16OP1_OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC*10;
    }
    else
    {
        u16OP1_OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC*10;
    }
#else
    u16OP1_OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC*10;
#endif
    return u16OP1_OutputVFreqAfterFRC;
}


//-------------------------------------------------------------------------------------------------
/// Set scaler display line buffer mode
/// @param  bEnable                \b IN: Select "Enable" to ring buffer mode or "Disable" for line buffer mode
//-------------------------------------------------------------------------------------------------
void MDrv_SC_SetDisplay_LineBuffer_Mode(void *pInstance, MS_BOOL bEnable)
{
    HAL_SC_SetDisplay_LineBuffer_Mode(pInstance, bEnable);
}

//-------------------------------------------------------------------------------------------------
/// Set scaler display line buffer Start mode
/// @param  bEnable                \b IN: Select "Enable" to start at faling edge of Vsync_init or
///                                       "Disable" for start at advance 1 display line
//-------------------------------------------------------------------------------------------------
void MDrv_SC_SetDisplay_Start_Mode(void *pInstance, MS_BOOL bEnable)
{
    HAL_SC_SetDisplay_Start_Mode(pInstance, bEnable);
}

void MDrv_XC_SetOSD2VEmode(void *pInstance, EN_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX)
{
    Hal_SC_Set_OSD2VE(pInstance, eVOPSelOSD_MUX);
}

MS_BOOL MDrv_XC_Check_HNonLinearScaling(void* pInstance)
{
    return Hal_SC_Check_HNonLinearScaling(pInstance);
}

MS_BOOL MApi_XC_Check_HNonLinearScaling_U2(void* pInstance)
{
    MS_BOOL bRet;

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    bRet = MDrv_XC_Check_HNonLinearScaling(pInstance);
    _XC_RETURN(pInstance);
#else
    _XC_ENTRY();
    bRet = MDrv_XC_Check_HNonLinearScaling(pInstance);
    _XC_RETURN();
#endif //#if SC2_ENABLE


    return bRet;
}

MS_BOOL MApi_XC_Check_HNonLinearScaling(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_H_NONLINEAR_SCALING XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_H_NONLINEAR_SCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_Set_Extra_fetch_adv_line_U2(void* pInstance, MS_U8 u8val)
{

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    Hal_SC_Set_extra_fetch_line(pInstance, u8val & 0x0F);
    Hal_SC_Set_extra_adv_line(pInstance, u8val>>4);
    _XC_RETURN(pInstance);
#else
    _XC_ENTRY();
    Hal_SC_Set_extra_fetch_line(pInstance, u8val & 0x0F);
    Hal_SC_Set_extra_adv_line(pInstance, u8val>>4);
    _XC_RETURN();
#endif //#if SC2_ENABLE


}

void MApi_XC_Set_Extra_fetch_adv_line(MS_U8 u8val)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_EXTRA_FETCH_ADV_LINE XCArgs;
    XCArgs.u8val = u8val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_EXTRA_FETCH_ADV_LINE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Enable FPLL related settings not by program but by user's setting from TV tool
/// Including PhaseLimit, limitD5D6D7_RK
/// @param bEnable @ref MS_BOOL          \b IN: enable or not
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableFpllManualSetting_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug = bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug = bEnable;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_EnableFpllManualSetting(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_MANUAL_SETTING_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_MANUAL_SETTING_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//--------------------------------------------------------------------------------------------------------------------
/// Test if panel output is ok by changing output DClk, this is to test max range that FPLL can do
/// @param u32FpllSetOffset @ref MS_U32     \b IN: the offset to be add/minus the typical DClk output, unit is SET
/// @param u16TestCnt       @ref MS_U16     \b IN: the times to do boundary test
//--------------------------------------------------------------------------------------------------------------------
void MApi_XC_FpllBoundaryTest_U2(void* pInstance, MS_U32 u32FpllSetOffset, MS_U16 u16TestCnt)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 i;
    MS_U32 u32OrigFpllSet;

    if(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support FPLL for device id %ld\n",__FUNCTION__,psXCInstPri->u32DeviceID);
        printf("MApi_XC_EX_EnableFpllBoundaryTest() -> Not support FPLL for device id %ld\n", psXCInstPri->u32DeviceID);
        return;
    }

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    // set to free run

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    MDrv_XC_SetFreeRunTiming(pInstance);
    _XC_RETURN(pInstance);
#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY();
    MDrv_XC_SetFreeRunTiming(pInstance);
    _XC_RETURN();
#endif //#if SC2_ENABLE



    // set to default output DClk
    printf("MApi_XC_EnableFpllBoundaryTest(0x%lx, %d)\n", u32FpllSetOffset, u16TestCnt);
    printf("MApi_XC_EnableFpllBoundaryTest() -> Default DClk = %d x %d x %d = %ld\n",
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal,
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal,
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
        MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal*
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal*
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq, FALSE)
    );

    MDrv_SC_set_output_dclk(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal,
                            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal,
                            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq, 0, MAIN_WINDOW);
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    u32OrigFpllSet = MDrv_Read4Byte(L_BK_LPLL(0x0F));

    printf("MApi_XC_EnableFpllBoundaryTest(): std set = 0x%lx\n", u32OrigFpllSet);

    // do FpllBoundary testst
    for(i=0; i<u16TestCnt; i++)
    {
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigFpllSet + u32FpllSetOffset);
        MsOS_DelayTask(2);
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigFpllSet - u32FpllSetOffset);
        MsOS_DelayTask(2);
    }
    LPLL_BK_RESTORE;

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_FpllBoundaryTest(MS_U32 u32FpllSetOffset, MS_U16 u16TestCnt)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_BOUNDARY_TEST XCArgs;
    XCArgs.u32FpllSetOffset = u32FpllSetOffset;
    XCArgs.u16TestCnt = u16TestCnt;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_BOUNDARY_TEST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_SkipOPWriteOffInSetWindow_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._abSkipOPWOffInSetWindow[eWindow] = bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._abSkipOPWOffInSetWindow[eWindow] = bEnable;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_SkipOPWriteOffInSetWindow(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_SKIP_OPWRITEOFF_IN_SETWINDOW XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SKIP_OPWRITEOFF_IN_SETWINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_XC_GetSkipOPWriteOffInSetWindow(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_Display._abSkipOPWOffInSetWindow[eWindow];
}

MS_BOOL MApi_XC_GetSkipOPWriteOffInSetWindow_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_GetSkipOPWriteOffInSetWindow(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bReturn = MDrv_XC_GetSkipOPWriteOffInSetWindow(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_GetSkipOPWriteOffInSetWindow(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_SKIP_OPWRITEOFF_IN_SETWINDOW XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SKIP_OPWRITEOFF_IN_SETWINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_XC_SetSkipDisableOPWriteOffInFPLL(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[eWindow] = bEnable;
}

void MApi_XC_SetSkipDisableOPWriteOffInFPLL_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_SetSkipDisableOPWriteOffInFPLL(pInstance, bEnable, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    MDrv_XC_SetSkipDisableOPWriteOffInFPLL(pInstance, bEnable, eWindow);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_SetSkipDisableOPWriteOffInFPLL(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL XCArgs;
    XCArgs.bEnable = FALSE;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_Set_OPWriteOffEnable_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow] = !bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow] = !bEnable;
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_Set_OPWriteOffEnable(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OPWRITEOFF_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OPWRITEOFF_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_Set_OPWriteOffEnableToReg_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow] = !bEnable;
    if(!pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
    {
        _XC_ENTRY(pInstance);
        Hal_SC_SetOPWriteOff(pInstance, bEnable);
        _XC_RETURN(pInstance);
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow] = !bEnable;
    if(!pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
    {
        _XC_ENTRY();
        Hal_SC_SetOPWriteOff(pInstance, bEnable);
        _XC_RETURN();
    }
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE


}

void MApi_XC_Set_OPWriteOffEnableToReg(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OPWRITEOFF_ENABLE_TO_REG XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OPWRITEOFF_ENABLE_TO_REG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_ReLoad_OPWriteOff(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
     pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
}


// If this function is called, driver will set OPW immediatly.
void MApi_XC_ForceSet_OPWriteOffEnable_U2(void* pInstance, MS_BOOL bEnable ,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;


#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

#else

    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (eWindow == MAIN_WINDOW )
    {
        if (bEnable)
        {

#if SC2_ENABLE
            _XC_ENTRY(pInstance);
            Hal_SC_SetOPWriteOff(pInstance, TRUE);
            _XC_RETURN(pInstance);

#else
            _XC_ENTRY();
            Hal_SC_SetOPWriteOff(pInstance, TRUE);
            _XC_RETURN();

#endif //#if SC2_ENABLE

            pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR = TRUE;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR = FALSE;
        }
    }

#if SC2_ENABLE
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

}

void MApi_XC_ForceSet_OPWriteOffEnable(MS_BOOL bEnable ,SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_FORCESET_OPWRITEOFF_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_FORCESET_OPWRITEOFF_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_XC_Get_OPWriteOffEnable(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return !pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow];
}

MS_BOOL MApi_XC_Get_OPWriteOffEnable_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if SC2_ENABLE
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#else
    _XC_SEMAPHORE_ENTRY(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));
    bReturn = MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID));

#endif //#if SC2_ENABLE

    return bReturn;
}

MS_BOOL MApi_XC_Get_OPWriteOffEnable(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_OPWRITEOFF_ENABLE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OPWRITEOFF_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
// This function will store the H or V sync information(SyncStart/end/width) of panel default for XC to use
// Because their is no other place to get them in XC Lib source code
// IN:
//    NONE
// Return:
//    NONE
//-------------------------------------------------------------------------------------------------
void MDrv_XC_GetDefaultHVSyncInfo(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //Note: H/V Total, H/V DE End, H/V Syncwidth all need minus 1 when set to register
    pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, 0xFF) + 1; //HsyncWidth= reg + 1
    pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncBackPorch = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, VOP_DE_HSTART_MASK) - pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth;
    pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncWidth = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, VOP_VSYNC_END_MASK) - ((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, 0x1000) >> 1) + SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, 0x7FF)) + 1;
    pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, 0xFFF) - SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, VOP_VSYNC_END_MASK); //V backPorch= VTT+1-(VsyncEnd+1)
    if(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L) & BIT(15)) //If manual Vsync mode, keep SyncWidth/back porch unchanged when vtt changes
    {
        pXCResourcePrivate->stdrvXC_Display._bHSyncChanged = TRUE;
        pXCResourcePrivate->stdrvXC_Display._bVSyncChanged = TRUE;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SyncWidth/back porch: V=%u,%u,H=%u,%u\n"
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncWidth
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncBackPorch)
}

///For K1 OSD Layer Switch
E_APIXC_ReturnValue MDrv_XC_SetOSDLayer(void *pInstance, E_VOP_OSD_LAYER_SEL eVOPOSDLayer, SCALER_WIN eWindow)
{
    return Hal_SC_Set_OSDLayer(pInstance, eVOPOSDLayer, eWindow);
}

E_VOP_OSD_LAYER_SEL MDrv_XC_GetOSDLayer(void *pInstance, SCALER_WIN eWindow)
{
    return Hal_SC_Get_OSDLayer(pInstance, eWindow);
}

E_APIXC_ReturnValue MDrv_XC_SetVideoAlpha(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow)
{
    return Hal_SC_Set_VideoAlpha(pInstance, u8Val, eWindow);
}

E_APIXC_ReturnValue MDrv_XC_GetVideoAlpha(void *pInstance, MS_U8 *pu8Val, SCALER_WIN eWindow)
{
    return Hal_SC_Get_VideoAlpha(pInstance, pu8Val, eWindow);
}

E_APIXC_ReturnValue MDrv_SC_ShiftDisplayWindowDirectly(void *pInstance, MS_S8 s8H, MS_S8 s8V, MS_S8 s8preH, MS_S8 s8preV)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_WINDOW_TYPE dsp_main, dsp_sub;
    MS_S8 s8dH = s8H - s8preH, s8dV = s8V - s8preV;
    MS_U16 u16Data_OSD_hs_st=0, u16Data_OSD_hs_end=0;
    MS_U16 u16Data_OSD_vfde_st=0, u16Data_OSD_vfde_end=0;
    MS_U8 u8OSD_vdelay = 0;
    MS_WINDOW_TYPE RepWindow;
    MS_WINDOW_TYPE OSDWindow;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy(&dsp_main, &(gSrcInfo[MAIN_WINDOW].stDispWin), sizeof(MS_WINDOW_TYPE));
    memcpy(&dsp_sub, &(gSrcInfo[SUB_WINDOW].stDispWin), sizeof(MS_WINDOW_TYPE));
    // Mirror case is handle by Mapp_SetPixelShift()
    dsp_main.x += s8dH;
    dsp_main.y += s8dV;
    dsp_sub.x += s8dH;
    dsp_sub.y += s8dV;
    //printf("dsp_main.x=%u,dsp_main.y=%u\n", dsp_main.x, dsp_main.y);

    // For report window
    RepWindow.x = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+ s8H;
    RepWindow.width = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
    RepWindow.y = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+ s8V;
    RepWindow.height = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;

    if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, BIT(15)))
    {
        if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK19_10_L, BIT(7)))
            u8OSD_vdelay +=2;

        if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK18_75_L, BIT(0)))
            u8OSD_vdelay +=1;

        //printf("New OSD ref=h=%d,v=%d\n", s8H, s8V);
        u16Data_OSD_vfde_st =  pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + s8V + u8OSD_vdelay;
        u16Data_OSD_vfde_end = u16Data_OSD_vfde_st + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;

        if( (-1 <= s8H) && (s8H <= 4) )
        {
            u16Data_OSD_hs_st = 0;
            u16Data_OSD_hs_end = 1 + s8H;
        }
        else if( (-4 <= s8H) && (s8H <= -2) )
        {
            // REG_SC_BK10_0C_L: Expect H total - 1
            // REG_SC_BK10_0E_L: Real H total - 1
            MS_U16 u16HTotal = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0E_L) + 1;
            u16Data_OSD_hs_st = 1;
            u16Data_OSD_hs_end = u16HTotal + 1 + s8H;
        }
        else
        {
            u16Data_OSD_hs_st = 0;
            u16Data_OSD_hs_end = 1;
        }
        OSDWindow.x = u16Data_OSD_hs_st;
        OSDWindow.y = u16Data_OSD_vfde_st;
        OSDWindow.width = u16Data_OSD_hs_end + 1 - u16Data_OSD_hs_st;
        OSDWindow.height = u16Data_OSD_vfde_end + 1 - u16Data_OSD_vfde_st;
    }

    MDrv_XC_UpdateWindow(pInstance, NULL, &dsp_main, &dsp_sub, &RepWindow, &OSDWindow);

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MDrv_XC_Set_Zorder_MLoadEn(void *pInstance, MS_BOOL bEn)
{

    if(bEn == FALSE)
    {
        if(_u16ZorderMLoadCmdCnt)
        {
            MApi_XC_MLoad_WriteCmds_And_Fire_U2(pInstance,
                &_u32ZorderMLoadCmd[0], &_u16ZorderMLoadVal[0], &_u16ZorderMLoadMsk[0], _u16ZorderMLoadCmdCnt);
            _u16ZorderMLoadCmdCnt = 0;
        }
    }
    _bZorderMLoadEn = bEn;
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MDrv_SC_SetVideoOnOSD(void *pInstance, E_VIDEO_ON_OSD_LAYER enlayer, SCALER_WIN eWindow)
{
#if (VIDEO_OSD_SWITCH_VER > 0)
    if ((enlayer >= E_VIDEO_ON_OSD_LAYER_NUM) || (eWindow >= MAX_WINDOW))
    {
        return E_APIXC_RET_FAIL;
    }

    if (eWindow == MAIN_WINDOW)
    {
        if (enlayer == E_VIDEO_ON_OSD_LAYER_0)    //osd layer 0 -> video -> osd layer 1 -> osd layer 2 -> osd layer 3
        {
            MDrv_SC_SetOSDLayerBlending(pInstance, 0, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 0, 0);
            MDrv_SC_SetOSDLayerBlending(pInstance, 1, FALSE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerBlending(pInstance, 2, FALSE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerBlending(pInstance, 3, FALSE, MAIN_WINDOW);
#if (OSD_LAYER_NUM > 4)
            MDrv_SC_SetOSDLayerBlending(pInstance, 4, FALSE, MAIN_WINDOW);
#endif
        }
        else if (enlayer == E_VIDEO_ON_OSD_LAYER_1)   //osd layer 0 -> osd layer 1 -> video -> osd layer 2 -> osd layer 3
        {
            MDrv_SC_SetOSDLayerBlending(pInstance, 0, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 0, 0);
            MDrv_SC_SetOSDLayerBlending(pInstance, 1, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 1, 0);
            MDrv_SC_SetOSDLayerBlending(pInstance, 2, FALSE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerBlending(pInstance, 3, FALSE, MAIN_WINDOW);
#if (OSD_LAYER_NUM > 4)
            MDrv_SC_SetOSDLayerBlending(pInstance, 4, FALSE, MAIN_WINDOW);
#endif
        }
        else if (enlayer == E_VIDEO_ON_OSD_LAYER_2)   //osd layer 1 -> osd layer 2 -> video -> osd layer 3
        {
            MDrv_SC_SetOSDLayerBlending(pInstance, 0, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 0, 0);
            MDrv_SC_SetOSDLayerBlending(pInstance, 1, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 1, 0);
            MDrv_SC_SetOSDLayerBlending(pInstance, 2, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 2, 0);
            MDrv_SC_SetOSDLayerBlending(pInstance, 3, FALSE, MAIN_WINDOW);
#if (OSD_LAYER_NUM > 4)
            MDrv_SC_SetOSDLayerBlending(pInstance, 4, FALSE, MAIN_WINDOW);
#endif
        }
        else if (enlayer == E_VIDEO_ON_OSD_LAYER_3)   //osd layer 0 -> osd layer 1 -> osd layer 2 -> osd layer 3 -> video
        {
            MDrv_SC_SetOSDLayerBlending(pInstance, 0, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 0, 0);
            MDrv_SC_SetOSDLayerBlending(pInstance, 1, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 1, 0);
            MDrv_SC_SetOSDLayerBlending(pInstance, 2, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 2, 0);
            MDrv_SC_SetOSDLayerBlending(pInstance, 3, TRUE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerAlpha(pInstance, 3, 0);
#if (OSD_LAYER_NUM > 4)
            MDrv_SC_SetOSDLayerBlending(pInstance, 4, FALSE, MAIN_WINDOW);
#endif
        }
        else    // default: video -> osd layer 0 -> osd layer 1 -> osd layer 2 -> osd layer 3
        {
            MDrv_SC_SetOSDLayerBlending(pInstance, 0, FALSE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerBlending(pInstance, 1, FALSE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerBlending(pInstance, 2, FALSE, MAIN_WINDOW);
            MDrv_SC_SetOSDLayerBlending(pInstance, 3, FALSE, MAIN_WINDOW);
#if (OSD_LAYER_NUM > 4)
            MDrv_SC_SetOSDLayerBlending(pInstance, 4, FALSE, MAIN_WINDOW);
#endif
        }
    }
    else
    {
        // do nothing
        printf("[SetVideoOnOSD] Not support sub window now \n");
    }
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MDrv_SC_SetOSDLayerBlending(void *pInstance, MS_U8 u8Layer, MS_BOOL bEnable, SCALER_WIN eWindow)
{
#if (VIDEO_OSD_SWITCH_VER > 0)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32Addr[] = {REG_SC_BK10_6B_L, REG_SC_BK10_60_L,
                        REG_SC_BK10_61_L, REG_SC_BK10_62_L,
                        REG_SC_BK10_1B_L};
    if ((u8Layer >= OSD_LAYER_NUM) || (eWindow >= MAX_WINDOW))
    {
        return E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }

    if (MAIN_WINDOW == eWindow)
    {
    #if (VIDEO_OSD_SWITCH_VER < 2)  // ver 1
    if(_bZorderMLoadEn)
    {
        _u32ZorderMLoadCmd[_u16ZorderMLoadCmdCnt]=u32Addr[u8Layer];
        _u16ZorderMLoadVal[_u16ZorderMLoadCmdCnt]=(bEnable<<8);
        _u16ZorderMLoadMsk[_u16ZorderMLoadCmdCnt]=BIT(8);
        _u16ZorderMLoadCmdCnt++;
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr[u8Layer], (bEnable<<8), BIT(8));
    }
    #elif (VIDEO_OSD_SWITCH_VER < 3)    // ver 2
    if(_bZorderMLoadEn)
    {
        _u32ZorderMLoadCmd[_u16ZorderMLoadCmdCnt]=u32Addr[u8Layer];
        _u16ZorderMLoadVal[_u16ZorderMLoadCmdCnt]=(bEnable<<9)|BIT(8);
        _u16ZorderMLoadMsk[_u16ZorderMLoadCmdCnt]=BIT(9)|BIT(8);
        _u16ZorderMLoadCmdCnt++;
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr[u8Layer], (bEnable<<9)|BIT(8), BIT(9)|BIT(8));
    }
    #endif
    }
    #if (VIDEO_OSD_SWITCH_VER > 1)  // ver > 1, with sub blending
    else
    {
        if(_bZorderMLoadEn)
        {
            _u32ZorderMLoadCmd[_u16ZorderMLoadCmdCnt]=u32Addr[u8Layer];
            _u16ZorderMLoadVal[_u16ZorderMLoadCmdCnt]=(bEnable<<10)|BIT(8);
            _u16ZorderMLoadMsk[_u16ZorderMLoadCmdCnt]=BIT(10)|BIT(8);
            _u16ZorderMLoadCmdCnt++;
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr[u8Layer], (bEnable<<10)|BIT(8), BIT(10)|BIT(8));
        }
    }
    #endif
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MDrv_SC_SetOSDLayerAlpha(void *pInstance, MS_U8 u8Layer, MS_U8 u8Alpha)
{
#if (VIDEO_OSD_SWITCH_VER > 0)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32Addr[] = {REG_SC_BK10_6B_L, REG_SC_BK10_60_L,
                        REG_SC_BK10_61_L, REG_SC_BK10_62_L,
                        REG_SC_BK10_1B_L};
    if ((u8Layer >= OSD_LAYER_NUM))
    {
        return E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }
    if ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_23_L) & BIT(5)) || (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_1C_L) & BIT(u8Layer)))
    {
        u8Alpha = 0xFF - u8Alpha;
    }
    if(_bZorderMLoadEn)
    {
        _u32ZorderMLoadCmd[_u16ZorderMLoadCmdCnt]=u32Addr[u8Layer];
        _u16ZorderMLoadVal[_u16ZorderMLoadCmdCnt]=u8Alpha;
        _u16ZorderMLoadMsk[_u16ZorderMLoadCmdCnt]=BITMASK(7:0);
        _u16ZorderMLoadCmdCnt++;
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr[u8Layer], u8Alpha, BITMASK(7:0));
    }

    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MDrv_SC_SetOSDBlendingFormula(void *pInstance, E_XC_OSD_INDEX enOsdIndex, E_XC_OSD_BlENDING_TYPE enType, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    E_APIXC_ReturnValue bRet = E_APIXC_RET_OK;
    if ((enOsdIndex >= E_XC_OSD_NUM) || (enType >= E_XC_OSD_BlENDING_TYPE_NUM) || (eWindow >= MAX_WINDOW))
    {
        return E_APIXC_RET_FAIL;
    }

    if (eWindow == MAIN_WINDOW)
    {
        switch(enOsdIndex)
        {
            case E_XC_OSD_0:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, BIT(5)|BIT(4), BIT(5)|BIT(4));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, 0x00, BIT(5)|BIT(4));
                }
                break;

            case E_XC_OSD_1:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, BIT(7)|BIT(6), BIT(7)|BIT(6));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, 0x00, BIT(7)|BIT(6));
                }
                break;

            case E_XC_OSD_2:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, BIT(9)|BIT(8), BIT(9)|BIT(8));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, 0x00, BIT(9)|BIT(8));
                }
                break;

            case E_XC_OSD_3:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, BIT(13)|BIT(12), BIT(13)|BIT(12));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, 0x00, BIT(13)|BIT(12));
                }
                break;

            default:
                printf("[MDrv_SC_SetOSDBlendingFormula]Invalid Parameter!enOsdIndex=%u\n", enOsdIndex);
                bRet = E_APIXC_RET_FAIL_INVALID_PARAMETER;
                break;
        }
    }
    else
    {
        // do nothing
        printf("[MDrv_SC_SetOSDBlendingFormula] Not support sub window now \n");
        bRet = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }

    return bRet;

}


void MDrv_SC_SetMCDIBufferAddress(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, E_XC_MCDI_TYPE eType)
{
    MS_U8 u8MIUSel;
    //SCALER_WIN _eWindow = MAIN_WINDOW;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    if (eType >= E_XC_MCDI_SUB_BOTH)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"the MDCI type(%d) is invalid for frame buffer setting \n",eType);
        return;
    }

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if((pXCResourcePrivate->stdrvXC_Display._u32MCDIFBAddress[eType]==u32FBAddress) && (pXCResourcePrivate->stdrvXC_Display._u32MCDIFBSize[eType]==u32FBSize))
    {
        return;
    }

    pXCResourcePrivate->stdrvXC_Display._u32MCDIFBAddress[eType]=u32FBAddress;
    pXCResourcePrivate->stdrvXC_Display._u32MCDIFBSize[eType]=u32FBSize;

    #if 0
    if (eType <= E_XC_MCDI_BOTH)
    {
        _eWindow = MAIN_WINDOW;
    }
    else if ( (eType > E_XC_MCDI_BOTH) && (eType <= E_XC_MCDI_SUB_BOTH))
    {
        _eWindow = SUB_WINDOW;
    }
    #endif

    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MCDIFBAddress_Suspend[eType] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MCDIFBSize_Suspend[eType] = u32FBSize;

    _phy_to_miu_offset(u8MIUSel, u32FBAddress, u32FBAddress);

    Hal_SC_Enable_MiuMask(pInstance);

    Hal_SC_set_mrq_miusel(pInstance, u8MIUSel);

    Hal_SC_set_mcdi_memoryaddress(pInstance, u32FBAddress, eType);

    Hal_SC_set_mcdi_write_limit(pInstance, ENABLE, (u32FBAddress + u32FBSize), eType);

    Hal_SC_Disable_MiuMask(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change MCDI mode = %d  FB_Start_Addr=%lx, Size=%lx\n",eType ,u32FBAddress, u32FBSize);
}

void MDrv_SC_EnableMCDI(void *pInstance, MS_BOOL bEnable, E_XC_MCDI_TYPE eType)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (E_POWER_SUSPEND != pXCResourcePrivate->stdrvXC_MVideo._enPrevPowerState)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableMCDI_Suspend[eType] = bEnable;
    }
    Hal_SC_enable_mcdi(pInstance, bEnable, eType);
}

//-------------------------------------------------------------------------------------------------
/// Adjust OSD DE reference position
/// @param bEnable allow OSD position reference enable or not
/// @param VStart the vertical starting position for OSD reference
/// @param VEnd the vertical ending position for OSD reference
/// @param HStart the horizontal starting position for OSD reference
/// @param HEnd the horizontal ending position for OSD reference
//-------------------------------------------------------------------------------------------------
void MDrv_XC_3D_AdjustOSDVDE(void *pInstance, MS_BOOL bEnable, MS_U16 VStart, MS_U16 VEnd, MS_U16 HStart, MS_U16 HEnd)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, HStart, BMASK(12:0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_11_L, HEnd, BMASK(12:0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_12_L, VStart, BMASK(11:0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_13_L, VEnd, BMASK(11:0));

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, bEnable?BIT(15):0, BIT(15));
}

E_APIXC_ReturnValue MDrv_SC_ForceReadFrame(void *pInstance, MS_BOOL bEnable, MS_U16 u16FrameIndex)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16value = (bEnable << 11) | ((u16FrameIndex & 0x07) << 8);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, u16value, 0x0F00);

    return E_APIXC_RET_OK;
}

void MDrv_XC_Set_ReadFrameBufferFrameCount(void* pInstance, MS_U16 value, SCALER_WIN eWindow)
{
    Hal_XC_Set_ReadFrameBufferFrameCount(pInstance, value, eWindow);
}

void MDrv_XC_Set_ReadFrameBufferFrameCount_FRC(void* pInstance, MS_U16 value, SCALER_WIN eWindow)
{
    Hal_XC_Set_ReadFrameBufferFrameCount_FRC(pInstance, value, eWindow);
}

void MDrv_XC_Set_RWPointerDiff(void* pInstance, MS_U16 value, SCALER_WIN eWindow)
{
    Hal_XC_Set_RWPointerDiff(pInstance, value, eWindow);
}

void MDrv_XC_Set_RWPointerDiff_FRC(void* pInstance, MS_U16 value, SCALER_WIN eWindow)
{
    Hal_XC_Set_RWPointerDiff_FRC(pInstance, value, eWindow);
}

MS_U16 MDrv_XC_Get_ReadFrameBufferFrameCount(void* pInstance, SCALER_WIN eWindow)
{
    return Hal_XC_Get_ReadFrameBufferFrameCount(pInstance, eWindow);
}

MS_U16 MDrv_XC_Get_ReadFrameBufferFrameCount_FRC(void* pInstance, SCALER_WIN eWindow)
{
    return Hal_XC_Get_ReadFrameBufferFrameCount_FRC(pInstance, eWindow);
}

MS_U16 MDrv_XC_Get_RWPointerDiff(void* pInstance, SCALER_WIN eWindow)
{
    return Hal_XC_Get_RWPointerDiff(pInstance, eWindow);
}

MS_U16 MDrv_XC_Get_RWPointerDiff_FRC(void* pInstance, SCALER_WIN eWindow)
{
    return Hal_XC_Get_RWPointerDiff_FRC(pInstance, eWindow);
}

//-------------------------------------------------------------------------------------------------
/// Get XC free run status
/// @return TRUE if it is free run, FALSE if not
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_GetFreeRunStatus_U2(void* pInstance)
{
    MS_BOOL bSourceSyncEn = FALSE, bFrameLPLLEn = FALSE;


#if SC2_ENABLE
    _XC_ENTRY(pInstance);
#else
    _XC_ENTRY();
#endif //#if SC2_ENABLE



    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    bSourceSyncEn = (MS_BOOL)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, BIT(15)) >> 15);

    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    bFrameLPLLEn = (MS_BOOL)(MDrv_ReadRegBit(L_BK_LPLL(0x0C), BIT(3)) >> 3);
    LPLL_BK_RESTORE;

#if SC2_ENABLE
    _XC_RETURN(pInstance);
#else
    _XC_RETURN();
#endif //#if SC2_ENABLE




    if (!bSourceSyncEn || !bFrameLPLLEn)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MApi_XC_GetFreeRunStatus(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_FREERUN_STATUS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FREERUN_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_BYPASS_SetCSC
/// @param  bEnable                              \b IN: ENABLE/DISABLE the color space convert
//-------------------------------------------------------------------------------------------------
void MApi_XC_BYPASS_SetCSC_U2(void* pInstance, MS_BOOL bEnable)
{

#if SC2_ENABLE
    _XC_ENTRY(pInstance);
    MHal_XC_BYPASS_SetCSC(pInstance, bEnable);
    _XC_RETURN(pInstance);
#else
    _XC_ENTRY();
    MHal_XC_BYPASS_SetCSC(pInstance, bEnable);
    _XC_RETURN();
#endif //#if SC2_ENABLE


}

void MApi_XC_BYPASS_SetCSC(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BYPASS_CSC XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BYPASS_CSC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_SC_EnableReadBankSwitchControl(void* pInstance, MS_BOOL bEnable, MS_U16 u16SwitchCnt, SCALER_WIN eWindow)
{
    MHal_SC_set_manual_rbank_switch_cnt(pInstance, bEnable, u16SwitchCnt, eWindow);
}

#if 1
void MApi_XC2_SetPanelTiming(XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_PANEL_TIMING XCArgs;
    XCArgs.pTimingInfo = pTimingInfo;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_PANEL_TIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC2 engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC2_WaitFPLLDone(void)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_WAIT_FPLL_DONE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_GET_WAIT_FPLL_DONE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC2 engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC2_SetFreeRunTiming (void)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_FREERUN_TIMING, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_SC2_Enable_FPLL_FSM(MS_BOOL bTrue)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC2_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_FSM_ENABLE XCArgs;
    XCArgs.bTrue = bTrue;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_FPLL_FSM_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC2 engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


void MApi_XC2_Set_OPWriteOffEnable(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OPWRITEOFF_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_SET_OPWRITEOFF_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_SC2_get_disp_de_window(MS_WINDOW_TYPE *pWin)
{
    if (pu32XCInst_1 == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_GET_DE_WINDOW XCArgs;
    XCArgs.psWin = pWin;
    XCArgs.eWindow = MAIN_WINDOW;

    if(UtopiaIoctl(pu32XCInst_1, E_XC_CMD_GET_DISP_DE_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#endif

