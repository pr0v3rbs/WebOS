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
#define _MDRV_HDMI_EX_C_

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvXC_HDMI_Internal.h"
#include "mhal_hdmi.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_adc.h"
#include "xc_hwreg_utility2.h"
#include "drv_sc_mux.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "apiXC_cus_ex.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drvXC_HDMI_if_v2.h"

#include "mhal_hdmi_ex.h"
//-------------------------------------------------------------------------------------------------
/// mpeg info frame, lenth 5, byte[0]~[4]
/// @param  u8byte                \b IN: Byte number
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_mpeg_info(MS_U8 u8byte)
{
    return Hal_HDMI_mpeg_info(u8byte);
}

//--------------------------------------------------------------------------------------------------
/// source product descriptor info, length = 25 from byte[0]~[24]
/// @param  u8byte                \b IN: Byte number
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_SPD_info(MS_U8 u8byte)
{
    return Hal_HDMI_SPD_info(u8byte);
}

//--------------------------------------------------------------------------------------------------
/// Get Vender Specific Packet data
/// @param  u8byte                \b IN: byte info selection
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_Get_VS_PacketData(MS_U8 u8byte)
{
    return Hal_HDMI_Get_VS_PacketData(u8byte);
}

//--------------------------------------------------------------------------------------------------
/// Get Vender Specific Packet Header (3 bytes)
/// @param  u8byte                \b IN: byte info selection
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_Get_VS_PacketHeader(MS_U8 u8byte)
{
    return Hal_HDMI_Get_VS_PacketHeader(u8byte);
}

//=========================================================//
// Function : Hal_HDCP_GetAuthstatus
// Description: get hdcp authentication status
//=========================================================//
MS_U16 Hal_HDCP_GetAuthstatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 u16bank_offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:
            u16bank_offset = 0x0000;
            break; // BK 0x110A60
        case INPUT_PORT_DVI1:
            u16bank_offset = 0x2900;
            break; // BK 0x113360
        case INPUT_PORT_DVI3:
            u16bank_offset = 0x2B00;
            break; // BK 0x113560
        case INPUT_PORT_DVI2:
            u16bank_offset = 0x2D00;
            break; // BK 0x113760
    }
    return (HDCP_R2BYTE(REG_HDCP_01_L+u16bank_offset) & 0x0F00);
}

//=========================================================//
// Function : Hal_HDCP_clearstatus
// Description: clear hdcp status, must during hpd low
//=========================================================//
void Hal_HDCP_clearstatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 u16bank_offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:
            u16bank_offset = 0x0000;
            break; // BK 0x110A60
        case INPUT_PORT_DVI1:
            u16bank_offset = 0x2900;
            break; // BK 0x113360
        case INPUT_PORT_DVI3:
            u16bank_offset = 0x2B00;
            break; // BK 0x113560
        case INPUT_PORT_DVI2:
            u16bank_offset = 0x2D00;
            break; // BK 0x113760
    }
    HDCP_W2BYTE(REG_HDCP_01_L+u16bank_offset, 0xFF00); // write 1 clear
}

MS_U16 MDrv_HDCP_GetAuthstatus(E_MUX_INPUTPORT enInputPortType)
{
    return Hal_HDCP_GetAuthstatus(enInputPortType);
}

void MDrv_HDCP_clearstatus(E_MUX_INPUTPORT enInputPortType)
{
    Hal_HDCP_clearstatus(enInputPortType);
}


