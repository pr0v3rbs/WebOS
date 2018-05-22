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

////////////////////////////////////////////////////////////////////////////////
/// file    XC_HDMI_if_adp.c
/// @brief  Scaler API layer Interface.
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _XC_HDMI_IF_ADP_C_
#define _XC_HDMI_IF_ADP_C_

//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
// Common Definition
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "MsOS.h"
#include "MsDevice.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvXC_HDMI_if.h"
#include "drvXC_HDMI_if_v2.h"
#include "apiXC_v2.h"
#include "XC_HDMI_if_adp.h"
#include "XC_adp.h"
#include "mvideo_context.h"

#ifdef MSOS_TYPE_LINUX_KERNEL

#ifndef CPY_FROM_USER
#define CPY_FROM_USER                   copy_from_user
#endif
#ifndef CPY_to_USER
#define CPY_to_USER                     copy_to_user
#endif

#else
#ifndef CPY_FROM_USER
#define CPY_FROM_USER                   memcpy
#endif
#ifndef CPY_to_USER
#define CPY_to_USER                     memcpy
#endif

#endif

//------------------------------------------------------------------------------
//  Driver Compiler Options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Structurs
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Variables
//------------------------------------------------------------------------------

//Function parameter
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_FUNC_CAPS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_PKT_RESET[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_PULLHPD[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GC_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_PACKET_RECEIVED[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_COLORFORMAT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_COLORRANGE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_SET_EQ[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_SET_EQ_TO_PORT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_AUDIO_MUTE_ENABLE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_ASPECTRATIO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_AVIINFOFRAMEVER[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_ERR_STATUS_UPDATE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_POLLINGSTATUS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_ISHDMI_MODE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_MID_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_PACKET_VALUE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DVI_CHANNELPHASESTATUS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DVI_SOFTWARERESET[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_AVIIINFO_ACTIVEINFOPRESENT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DVI_CLKPULLLOW[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DVI_SWITCHSRC[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_SETFORCLOCKLESSTHAN70MHZ[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_DVI_ADJUST[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_SETUPDATEPHASELINECOUNT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDCP_ENABLE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_SET_HDCPENABLE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDCP_INITPRODUCTIONKEY[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDCP_GETSTATUS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDCP_VSYNC_END_EN[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_AUDIO_OUTPUT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_AUDIO_CP_HDR_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_AUDIO_CHANNEL_STATUS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GETLIBVER[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GETINFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GETSTATUS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_READ_DDCRAM[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_PROG_DDCRAM[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_CONTENT_TYPE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_EXT_COLORIMETRY[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_PIXEL_REPETITION[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_CHECK_4K2K[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_AVG_SCALERINGDOWN[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_SetHPD_HL[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_CHECK_ADDITIONAL_FORMAT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_3D_STRUCTURE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_3D_EXT_DATA[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_3D_META_FIELD[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_VIC_CODE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_GET_4KX2K_VIC_CODE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_ARC_PINCONTROL[5];
UADP_STRUCT_POINTER_TABLE spt_XC_HDMI_SET_POWERSTATE[5];


//Function parameter's pointer
UADP_STRUCT_POINTER_TABLE spt_XC_DrvHDMI_u8ManufactorData[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DrvHDMI_TypeU8[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DrvHDMI_u8GET_PACKETValue[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DrvHDMI_u8HdcpKeyData[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DrvMS_HDCP_STATUS_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DrvHDMI_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DrvHDMI_Status[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DDCRAM_PROG_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DrvHDMI_EDID[5];
UADP_STRUCT_POINTER_TABLE spt_XC_DrvHDMI_3D_META_FIELD[5];

//------------------------------------------------------------------------------
//  Local Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Debug Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MS_U32 XC_HDMI_if_adp_Init(void)
{

//set table
    MS_U32 temp=0;

    //Function parameter
    //pstHDMI_FUNC_CAPS           pXC_HDMI_FUNC_CAPS         = (pstHDMI_FUNC_CAPS)temp;
	//pstHDMI_PKT_RESET           pXC_HDMI_PKT_RESET         = (pstHDMI_PKT_RESET)temp;
	//pstHDMI_PULLHPD             pXC_HDMI_PULLHPD           = (pstHDMI_PULLHPD)temp;
	//pstHDMI_GC_INFO             pXC_HDMI_GC_INFO           = (pstHDMI_GC_INFO)temp;
	//pstHDMI_PACKET_RECEIVED     pXC_HDMI_PACKET_RECEIVED   = (pstHDMI_PACKET_RECEIVED)temp;
	//pstHDMI_GET_COLORFORMAT     pXC_HDMI_GET_COLORFORMAT   = (pstHDMI_GET_COLORFORMAT)temp;
	//pstHDMI_GET_COLORRANGE      pXC_HDMI_GET_COLORRANGE    = (pstHDMI_GET_COLORRANGE)temp;
	//pstHDMI_SET_EQ              pXC_HDMI_SET_EQ            = (pstHDMI_SET_EQ)temp;
	//pstHDMI_SET_EQ_TO_PORT      pXC_HDMI_SET_EQ_TO_PORT    = (pstHDMI_SET_EQ_TO_PORT)temp;
	//pstHDMI_AUDIO_MUTE_ENABLE   pXC_HDMI_AUDIO_MUTE_ENABLE = (pstHDMI_AUDIO_MUTE_ENABLE)temp;
	//pstHDMI_GET_ASPECTRATIO     pXC_HDMI_GET_ASPECTRATIO   = (pstHDMI_GET_ASPECTRATIO)temp;
	//pstHDMI_GET_ACTIVEFORMAT_ASPECTRATIO  pXC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO = (pstHDMI_GET_ACTIVEFORMAT_ASPECTRATIO)temp;
	//pstHDMI_GET_AVIINFOFRAMEVER           pXC_HDMI_GET_AVIINFOFRAMEVER          = (pstHDMI_GET_AVIINFOFRAMEVER)temp;
	//pstHDMI_ERR_STATUS_UPDATE             pXC_HDMI_ERR_STATUS_UPDATE            = (pstHDMI_ERR_STATUS_UPDATE)temp;
	//pstHDMI_GET_POLLINGSTATUS             pXC_HDMI_GET_POLLINGSTATUS            = (pstHDMI_GET_POLLINGSTATUS)temp;
    //pstHDMI_ISHDMI_MODE                   pXC_HDMI_ISHDMI_MODE                  = (pstHDMI_ISHDMI_MODE)temp;
    pstHDMI_GET_MID_INFO                  pXC_HDMI_GET_MID_INFO                 = (pstHDMI_GET_MID_INFO)temp;
    pstHDMI_GET_PACKET_VALUE              pXC_HDMI_GET_PACKET_VALUE             = (pstHDMI_GET_PACKET_VALUE)temp;
    //pstDVI_CHANNELPHASESTATUS             pXC_DVI_CHANNELPHASESTATUS            = (pstDVI_CHANNELPHASESTATUS)temp;
    //pstDVI_SOFTWARERESET                  pXC_DVI_SOFTWARERESET                 = (pstDVI_SOFTWARERESET)temp;
    //pstHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT  pXC_HDMI_GET_AVIIINFO_ACTIVEINFOPRESENT = (pstHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT)temp;
    //pstDVI_CLKPULLLOW                 pXC_DVI_CLKPULLLOW                = (pstDVI_CLKPULLLOW)temp;
    //pstDVI_SWITCHSRC                  pXC_DVI_SWITCHSRC                 = (pstDVI_SWITCHSRC)temp;
    //pstHDMI_SETFORCLOCKLESSTHAN70MHZ  pXC_HDMI_SETFORCLOCKLESSTHAN70MHZ = (pstHDMI_SETFORCLOCKLESSTHAN70MHZ)temp;
    //pstHDMI_DVI_ADJUST                pXC_HDMI_DVI_ADJUST               = (pstHDMI_DVI_ADJUST)temp;
    //pstHDMI_SETUPDATEPHASELINECOUNT   pXC_HDMI_SETUPDATEPHASELINECOUNT  = (pstHDMI_SETUPDATEPHASELINECOUNT)temp;
    //pstHDCP_ENABLE                    pXC_HDCP_ENABLE                   = (pstHDCP_ENABLE)temp;
    //pstHDMI_SET_HDCPENABLE            pXC_HDMI_SET_HDCPENABLE           = (pstHDMI_SET_HDCPENABLE)temp;
    pstHDCP_INITPRODUCTIONKEY         pXC_HDCP_INITPRODUCTIONKEY        = (pstHDCP_INITPRODUCTIONKEY)temp;
    pstHDCP_GETSTATUS                 pXC_HDCP_GETSTATUS                = (pstHDCP_GETSTATUS)temp;
    //pstHDCP_VSYNC_END_EN              pXC_HDCP_VSYNC_END_EN             = (pstHDCP_VSYNC_END_EN)temp;
    //pstHDMI_AUDIO_OUTPUT              pXC_HDMI_AUDIO_OUTPUT             = (pstHDMI_AUDIO_OUTPUT)temp;
    //pstHDMI_AUDIO_CP_HDR_INFO         pXC_HDMI_AUDIO_CP_HDR_INFO        = (pstHDMI_AUDIO_CP_HDR_INFO)temp;
    //pstHDMI_AUDIO_CHANNEL_STATUS      pXC_HDMI_AUDIO_CHANNEL_STATUS     = (pstHDMI_AUDIO_CHANNEL_STATUS)temp;
    //pstHDMI_GETLIBVER                 pXC_HDMI_GETLIBVER                = (pstHDMI_GETLIBVER)temp;
    pstHDMI_GETINFO                   pXC_HDMI_GETINFO                  = (pstHDMI_GETINFO)temp;
    pstHDMI_GETSTATUS                 pXC_HDMI_GETSTATUS                = (pstHDMI_GETSTATUS)temp;
    pstHDMI_READ_DDCRAM               pXC_HDMI_READ_DDCRAM              = (pstHDMI_READ_DDCRAM)temp;
    pstHDMI_PROG_DDCRAM               pXC_HDMI_PROG_DDCRAM              = (pstHDMI_PROG_DDCRAM)temp;
    //pstHDMI_GET_CONTENT_TYPE          pXC_HDMI_GET_CONTENT_TYPE         = (pstHDMI_GET_CONTENT_TYPE)temp;
    //pstHDMI_GET_EXT_COLORIMETRY       pXC_HDMI_GET_EXT_COLORIMETRY      = (pstHDMI_GET_EXT_COLORIMETRY)temp;
    //pstHDMI_GET_PIXEL_REPETITION      pXC_HDMI_GET_PIXEL_REPETITION     = (pstHDMI_GET_PIXEL_REPETITION)temp;
    //pstHDMI_CHECK_4K2K                pXC_HDMI_CHECK_4K2K               = (pstHDMI_CHECK_4K2K)temp;
    //pstHDMI_AVG_SCALERINGDOWN         pXC_HDMI_AVG_SCALERINGDOWN        = (pstHDMI_AVG_SCALERINGDOWN)temp;
    //pstHDMI_SetHPD_HL                 pstHDMI_SetHPD_HL               = (pstHDMI_SetHPD_HL)temp;
    //pstHDMI_CHECK_ADDITIONAL_FORMAT   pXC_HDMI_CHECK_ADDITIONAL_FORMAT  = (pstHDMI_CHECK_ADDITIONAL_FORMAT)temp;
    //pstHDMI_GET_3D_STRUCTURE          pXC_HDMI_GET_3D_STRUCTURE         = (pstHDMI_GET_3D_STRUCTURE)temp;
    //pstHDMI_GET_3D_EXT_DATA           pXC_HDMI_GET_3D_EXT_DATA          = (pstHDMI_GET_3D_EXT_DATA)temp;
    pstHDMI_GET_3D_META_FIELD         pXC_HDMI_GET_3D_META_FIELD        = (pstHDMI_GET_3D_META_FIELD)temp;
    //pstHDMI_GET_VIC_CODE              pXC_HDMI_GET_VIC_CODE             = (pstHDMI_GET_VIC_CODE)temp;
    //pstHDMI_GET_4KX2K_VIC_CODE        pXC_HDMI_GET_4KX2K_VIC_CODE       = (pstHDMI_GET_4KX2K_VIC_CODE)temp;
    //pstHDMI_ARC_PINCONTROL            pXC_HDMI_ARC_PINCONTROL           = (pstHDMI_ARC_PINCONTROL)temp;
    //pstHDMI_SET_POWERSTATE            pXC_HDMI_SET_POWERSTATE           = (pstHDMI_SET_POWERSTATE)temp;

	//Function parameter's pointer
	//MS_U8* pstXC_HDMI_TYPE_U8 = (MS_U8*)temp;
	//HDMI_POLLING_STATUS_t*
	//MS_U8* pXC_DrvHDMI_u8ManufactorData = (MS_U8*)temp;
	//MS_U8* pXC_DrvHDMI_GET_PACKET_VALUE = (MS_U8*)temp;
	//MS_U8* pXC_Drvu8HdcpKeyData         = (MS_U8*)temp;
	//MS_HDCP_STATUS_INFO_t* pXC_DrvHDCP_STATUS_INFO = (MS_HDCP_STATUS_INFO_t*)temp;
	//version
	//MS_HDMI_INFO*          pXC_DrvHDMI_INFO        = (MS_HDMI_INFO*)temp;
	//MS_HDMI_Status*        pXC_DrvHDMI_Status      = (MS_HDMI_Status*)temp;
	//XC_DDCRAM_PROG_INFO*   pXC_DrvHDMI_DDCRAM_PROG_INFO = (XC_DDCRAM_PROG_INFO*)temp;
	//MS_U8*                 pXC_DrvHDMI_u8EDID      = (MS_U8*)temp;
	//sHDMI_3D_META_FIELD*   pXC_DrvHDMI_3D_META_FIELD = (sHDMI_3D_META_FIELD*)temp;
    //MS_WINDOW_TYPE*        pXC_PCMOTITOR_msWin     = (MS_WINDOW_TYPE*)temp;

    //Function parameter's pointer
    UADPBypassSetSPT(&spt_XC_DrvHDMI_u8ManufactorData[0],UADP_SPT_SELF_SIZE,0, MID_KEY_LENGTH*sizeof(MS_U8));
    UADPBypassSetSPT(&spt_XC_DrvHDMI_u8ManufactorData[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_DrvHDMI_u8GET_PACKETValue[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_U8));
    UADPBypassSetSPT(&spt_XC_DrvHDMI_u8GET_PACKETValue[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_DrvHDMI_u8HdcpKeyData[0],UADP_SPT_SELF_SIZE,0, HDCP_KEY_LENGTH *sizeof(MS_U8));
    UADPBypassSetSPT(&spt_XC_DrvHDMI_u8HdcpKeyData[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_DrvMS_HDCP_STATUS_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_HDCP_STATUS_INFO_t));
    UADPBypassSetSPT(&spt_XC_DrvMS_HDCP_STATUS_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_DrvHDMI_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_HDMI_INFO));
    UADPBypassSetSPT(&spt_XC_DrvHDMI_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_DrvHDMI_Status[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_HDMI_Status));
    UADPBypassSetSPT(&spt_XC_DrvHDMI_Status[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_DDCRAM_PROG_INFO[0],UADP_SPT_SELF_SIZE,0, EDID_KEY_LENGTH*sizeof(MS_U8));
    UADPBypassSetSPT(&spt_XC_DDCRAM_PROG_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_DrvHDMI_EDID[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_U8));
    UADPBypassSetSPT(&spt_XC_DrvHDMI_EDID[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_DrvHDMI_3D_META_FIELD[0],UADP_SPT_SELF_SIZE,0, sizeof(sHDMI_3D_META_FIELD));
    UADPBypassSetSPT(&spt_XC_DrvHDMI_3D_META_FIELD[1],UADP_SPT_END , 0, 0);


    //Function parameter
    UADPBypassSetSPT(&spt_XC_HDMI_FUNC_CAPS[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_FUNC_CAPS));
    UADPBypassSetSPT(&spt_XC_HDMI_FUNC_CAPS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_PKT_RESET[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_PKT_RESET));
    UADPBypassSetSPT(&spt_XC_HDMI_PKT_RESET[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_PULLHPD[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_PULLHPD));
    UADPBypassSetSPT(&spt_XC_HDMI_PULLHPD[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_GC_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GC_INFO));
    UADPBypassSetSPT(&spt_XC_HDMI_GC_INFO[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_PACKET_RECEIVED[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_PACKET_RECEIVED));
    UADPBypassSetSPT(&spt_XC_HDMI_PACKET_RECEIVED[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_GET_COLORFORMAT[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_COLORFORMAT));
    UADPBypassSetSPT(&spt_XC_HDMI_GET_COLORFORMAT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_GET_COLORRANGE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_COLORRANGE));
    UADPBypassSetSPT(&spt_XC_HDMI_GET_COLORRANGE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_SET_EQ[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_SET_EQ));
    UADPBypassSetSPT(&spt_XC_HDMI_SET_EQ[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_SET_EQ_TO_PORT[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_SET_EQ_TO_PORT));
	UADPBypassSetSPT(&spt_XC_HDMI_SET_EQ_TO_PORT[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_AUDIO_MUTE_ENABLE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_AUDIO_MUTE_ENABLE));
	UADPBypassSetSPT(&spt_XC_HDMI_AUDIO_MUTE_ENABLE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_GET_ASPECTRATIO[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_ASPECTRATIO));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_ASPECTRATIO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_ACTIVEFORMAT_ASPECTRATIO));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_AVIINFOFRAMEVER[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_AVIINFOFRAMEVER));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_AVIINFOFRAMEVER[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_ERR_STATUS_UPDATE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_ERR_STATUS_UPDATE));
	UADPBypassSetSPT(&spt_XC_HDMI_ERR_STATUS_UPDATE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_POLLINGSTATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_POLLINGSTATUS));
	// pointer to point
	UADPBypassSetSPT(&spt_XC_HDMI_GET_POLLINGSTATUS[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_ISHDMI_MODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_ISHDMI_MODE));
	UADPBypassSetSPT(&spt_XC_HDMI_ISHDMI_MODE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_MID_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_MID_INFO));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_MID_INFO[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned int)(&(pXC_HDMI_GET_MID_INFO->pu8ManufactorData)) - (unsigned int)pXC_HDMI_GET_MID_INFO), (unsigned int)spt_XC_DrvHDMI_u8ManufactorData);
	UADPBypassSetSPT(&spt_XC_HDMI_GET_MID_INFO[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_GET_PACKET_VALUE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_PACKET_VALUE));
    UADPBypassSetSPT(&spt_XC_HDMI_GET_PACKET_VALUE[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned int)(&(pXC_HDMI_GET_PACKET_VALUE->pValue)) - (unsigned int)pXC_HDMI_GET_PACKET_VALUE), (unsigned int)spt_XC_DrvHDMI_u8GET_PACKETValue);
	UADPBypassSetSPT(&spt_XC_HDMI_GET_PACKET_VALUE[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_DVI_CHANNELPHASESTATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stDVI_CHANNELPHASESTATUS));
	UADPBypassSetSPT(&spt_XC_DVI_CHANNELPHASESTATUS[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_DVI_SOFTWARERESET[0],UADP_SPT_SELF_SIZE,0, sizeof(stDVI_SOFTWARERESET));
	UADPBypassSetSPT(&spt_XC_DVI_SOFTWARERESET[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_AVIIINFO_ACTIVEINFOPRESENT[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_AVIIINFO_ACTIVEINFOPRESENT[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_DVI_CLKPULLLOW[0],UADP_SPT_SELF_SIZE,0, sizeof(stDVI_CLKPULLLOW));
	UADPBypassSetSPT(&spt_XC_DVI_CLKPULLLOW[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_DVI_SWITCHSRC[0],UADP_SPT_SELF_SIZE,0, sizeof(stDVI_SWITCHSRC));
	UADPBypassSetSPT(&spt_XC_DVI_SWITCHSRC[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_SETFORCLOCKLESSTHAN70MHZ[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_SETFORCLOCKLESSTHAN70MHZ));
	UADPBypassSetSPT(&spt_XC_HDMI_SETFORCLOCKLESSTHAN70MHZ[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_DVI_ADJUST[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_DVI_ADJUST));
	UADPBypassSetSPT(&spt_XC_HDMI_DVI_ADJUST[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_SETUPDATEPHASELINECOUNT[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_SETUPDATEPHASELINECOUNT));
	UADPBypassSetSPT(&spt_XC_HDMI_SETUPDATEPHASELINECOUNT[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDCP_ENABLE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDCP_ENABLE));
	UADPBypassSetSPT(&spt_XC_HDCP_ENABLE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_SET_HDCPENABLE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_SET_HDCPENABLE));
	UADPBypassSetSPT(&spt_XC_HDMI_SET_HDCPENABLE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDCP_INITPRODUCTIONKEY[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDCP_INITPRODUCTIONKEY));
	UADPBypassSetSPT(&spt_XC_HDCP_INITPRODUCTIONKEY[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned int)(&(pXC_HDCP_INITPRODUCTIONKEY->pu8HdcpKeyData)) - (unsigned int)pXC_HDCP_INITPRODUCTIONKEY), (unsigned int)spt_XC_DrvHDMI_u8HdcpKeyData);
	UADPBypassSetSPT(&spt_XC_HDCP_INITPRODUCTIONKEY[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDCP_GETSTATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDCP_GETSTATUS));
	UADPBypassSetSPT(&spt_XC_HDCP_GETSTATUS[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned int)(&(pXC_HDCP_GETSTATUS->stReturnValue)) - (unsigned int)pXC_HDCP_GETSTATUS), (unsigned int)spt_XC_DrvMS_HDCP_STATUS_INFO);
	UADPBypassSetSPT(&spt_XC_HDCP_GETSTATUS[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDCP_VSYNC_END_EN[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDCP_VSYNC_END_EN));
	UADPBypassSetSPT(&spt_XC_HDCP_VSYNC_END_EN[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_AUDIO_OUTPUT[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_AUDIO_OUTPUT));
	UADPBypassSetSPT(&spt_XC_HDMI_AUDIO_OUTPUT[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_AUDIO_CP_HDR_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_AUDIO_CP_HDR_INFO));
	UADPBypassSetSPT(&spt_XC_HDMI_AUDIO_CP_HDR_INFO[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_AUDIO_CHANNEL_STATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_AUDIO_CHANNEL_STATUS));
	UADPBypassSetSPT(&spt_XC_HDMI_AUDIO_CHANNEL_STATUS[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GETLIBVER[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GETLIBVER));
	//point to point
	UADPBypassSetSPT(&spt_XC_HDMI_GETLIBVER[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_GETINFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GETINFO));
    UADPBypassSetSPT(&spt_XC_HDMI_GETINFO[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned int)(&(pXC_HDMI_GETINFO->eReturnValue)) - (unsigned int)pXC_HDMI_GETINFO), (unsigned int)spt_XC_DrvHDMI_INFO);
	UADPBypassSetSPT(&spt_XC_HDMI_GETINFO[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GETSTATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GETSTATUS));
	UADPBypassSetSPT(&spt_XC_HDMI_GETSTATUS[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned int)(&(pXC_HDMI_GETSTATUS->pStatus)) - (unsigned int)pXC_HDMI_GETSTATUS), (unsigned int)spt_XC_DrvHDMI_Status);
	UADPBypassSetSPT(&spt_XC_HDMI_GETSTATUS[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_READ_DDCRAM[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_READ_DDCRAM));
	UADPBypassSetSPT(&spt_XC_HDMI_READ_DDCRAM[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned int)(&(pXC_HDMI_READ_DDCRAM->pstDDCRam_Info)) - (unsigned int)pXC_HDMI_READ_DDCRAM), (unsigned int)spt_XC_DDCRAM_PROG_INFO);
    UADPBypassSetSPT(&spt_XC_HDMI_READ_DDCRAM[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_PROG_DDCRAM[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_PROG_DDCRAM));
	UADPBypassSetSPT(&spt_XC_HDMI_PROG_DDCRAM[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned int)(&(pXC_HDMI_PROG_DDCRAM->pstDDCRam_Info)) - (unsigned int)pXC_HDMI_PROG_DDCRAM), (unsigned int)spt_XC_DDCRAM_PROG_INFO);
    UADPBypassSetSPT(&spt_XC_HDMI_PROG_DDCRAM[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_CONTENT_TYPE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_CONTENT_TYPE));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_CONTENT_TYPE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_EXT_COLORIMETRY[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_HDMI_CONTENT_TYPE));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_EXT_COLORIMETRY[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_PIXEL_REPETITION[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_PIXEL_REPETITION));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_PIXEL_REPETITION[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_CHECK_4K2K[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_CHECK_4K2K));
	UADPBypassSetSPT(&spt_XC_HDMI_CHECK_4K2K[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_SetHPD_HL[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_SetHPD_HL));
	UADPBypassSetSPT(&spt_XC_HDMI_SetHPD_HL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_HDMI_AVG_SCALERINGDOWN[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_AVG_SCALERINGDOWN));
	UADPBypassSetSPT(&spt_XC_HDMI_AVG_SCALERINGDOWN[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_CHECK_ADDITIONAL_FORMAT[0],UADP_SPT_SELF_SIZE,0, sizeof(E_HDMI_ADDITIONAL_VIDEO_FORMAT));
	UADPBypassSetSPT(&spt_XC_HDMI_CHECK_ADDITIONAL_FORMAT[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_3D_STRUCTURE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_3D_STRUCTURE));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_3D_STRUCTURE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_3D_EXT_DATA[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_3D_EXT_DATA));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_3D_EXT_DATA[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_3D_META_FIELD[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_3D_META_FIELD));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_3D_META_FIELD[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned int)(&(pXC_HDMI_GET_3D_META_FIELD->pdata)) - (unsigned int)pXC_HDMI_GET_3D_META_FIELD), (unsigned int)spt_XC_DrvHDMI_3D_META_FIELD);
	UADPBypassSetSPT(&spt_XC_HDMI_GET_3D_META_FIELD[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_VIC_CODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_VIC_CODE));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_VIC_CODE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_GET_4KX2K_VIC_CODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_GET_4KX2K_VIC_CODE));
	UADPBypassSetSPT(&spt_XC_HDMI_GET_4KX2K_VIC_CODE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_ARC_PINCONTROL[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_ARC_PINCONTROL));
	UADPBypassSetSPT(&spt_XC_HDMI_ARC_PINCONTROL[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_HDMI_SET_POWERSTATE[0],UADP_SPT_SELF_SIZE,0, sizeof(stHDMI_SET_POWERSTATE));
	UADPBypassSetSPT(&spt_XC_HDMI_SET_POWERSTATE[1],UADP_SPT_END , 0, 0);


    return 0;

}

MS_U32 XC_HDMI_if_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[1];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        case E_XC_HDMI_IF_CMD_FUNC_CAPS :
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_XC_HDMI_FUNC_CAPS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_INIT:
        {
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }

        case E_XC_HDMI_IF_CMD_EXIT:
        {
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }

        case E_XC_HDMI_IF_CMD_PKT_RESET:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_PKT_RESET, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_PULLHPD:
        {
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_PULLHPD, NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_HDMI_IF_CMD_GC_INFO:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_GC_INFO,spt_XC_HDMI_GC_INFO,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_PACKET_RECEIVED:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_PACKET_RECEIVED,spt_XC_HDMI_PACKET_RECEIVED,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_COLORFORMAT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_COLORFORMAT,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_COLORRANGE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_COLORRANGE,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_HDMI_IF_CMD_SET_EQ:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_SET_EQ,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_SET_EQ_TO_PORT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_SET_EQ_TO_PORT,spt_XC_HDMI_SET_EQ_TO_PORT,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_AUDIO_MUTE_ENABLE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_AUDIO_MUTE_ENABLE,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_AUDIO_STATUS_CLEAR:
        {
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_ASPECTRATIO:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_GET_ASPECTRATIO,spt_XC_HDMI_GET_ASPECTRATIO,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_ACTIVEFORMAT_ASPECTRATIO:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO,spt_XC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_AVIINFOFRAMEVER:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_AVIINFOFRAMEVER,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_ERR_STATUS_UPDATE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_ERR_STATUS_UPDATE,spt_XC_HDMI_ERR_STATUS_UPDATE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_POLLINGSTATUS:
        {
            // pointer to pointer
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_POLLINGSTATUS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_ISHDMI_MODE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_ISHDMI_MODE,buffer_arg,sizeof(buffer_arg));
            break;
        }

	    case E_XC_HDMI_IF_CMD_GET_MID_INFO:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_MID_INFO,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_PACKET_VALUE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_GET_PACKET_VALUE,spt_XC_HDMI_GET_PACKET_VALUE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_DVI_IF_CMD_CHANNELPHASESTATUS:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_DVI_CHANNELPHASESTATUS,spt_XC_DVI_CHANNELPHASESTATUS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_DVI_IF_CMD_SOFTWARERESET:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_DVI_SOFTWARERESET,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_DVI_IF_CMD_RESET:
        {
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }

        case E_XC_HDMI_IF_CMD_AVIINFO_ACTIVEINFOPRESENT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_AVIIINFO_ACTIVEINFOPRESENT,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_DVI_IF_CMD_CLKPULLLOW:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_DVI_CLKPULLLOW,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_DVI_IF_CMD_SWITCHSRC:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_DVI_SWITCHSRC,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_SETFORCLOCKLESSTHAN70MHZ:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_SETFORCLOCKLESSTHAN70MHZ,spt_XC_HDMI_SETFORCLOCKLESSTHAN70MHZ,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_DVI_ADJUST:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_DVI_ADJUST,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_SETUPDATEPHASELINECOUNT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_SETUPDATEPHASELINECOUNT,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDCP_IF_CMD_ENABLE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDCP_ENABLE,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_SET_HDCP_ENABLE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_SET_HDCPENABLE,spt_XC_HDMI_SET_HDCPENABLE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDCP_IF_CMD_CLEARSTATUS:
        {
		    u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
		    break;
        }

		case E_XC_HDCP_IF_CMD_INITPRODUCTIONKEY:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDCP_INITPRODUCTIONKEY,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDCP_IF_CMD_GETSTATUS:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDCP_GETSTATUS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDCP_IF_CMD_VSYNC_END_EN:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDCP_VSYNC_END_EN,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_AUDIO_OUTPUT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_AUDIO_OUTPUT,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_AUDIO_CP_HDR_INFO:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_AUDIO_CP_HDR_INFO,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_AUDIO_CHANNEL_STATUS:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_AUDIO_CHANNEL_STATUS,spt_XC_HDMI_AUDIO_CHANNEL_STATUS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GETLIBVER:
        {
            // pointer
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDCP_VSYNC_END_EN,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GETINFO:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GETINFO,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GETSTATUS:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GETSTATUS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_READ_DDCRAM:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_READ_DDCRAM,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_PROG_DDCRAM:
        {
            #if 1
            stHDMI_PROG_DDCRAM stHDMIPROGDDC_kernel_user;
            stHDMI_PROG_DDCRAM stHDMIPROGDDC_kernel;
            CPY_FROM_USER((void*)(&stHDMIPROGDDC_kernel_user),(void*)(pArgs),sizeof(stHDMI_PROG_DDCRAM));
            XC_DDCRAM_PROG_INFO stXCDDCRAM;
            u32Ret = UTOPIA_STATUS_FAIL;
            if(stHDMIPROGDDC_kernel_user.pstDDCRam_Info->u16EDIDSize )
            {
                void* u8EDID = kmalloc(stHDMIPROGDDC_kernel_user.pstDDCRam_Info->u16EDIDSize,GFP_KERNEL);;
                if(u8EDID)
                {
                    CPY_FROM_USER(u8EDID,(void*)(stHDMIPROGDDC_kernel_user.pstDDCRam_Info->EDID),stHDMIPROGDDC_kernel_user.pstDDCRam_Info->u16EDIDSize);
                    stXCDDCRAM.EDID = NULL;
                    stHDMIPROGDDC_kernel.pstDDCRam_Info = &stXCDDCRAM;
                    stHDMIPROGDDC_kernel.u32SizeOfInfo = stHDMIPROGDDC_kernel_user.u32SizeOfInfo;
                    CPY_FROM_USER((void*)(stHDMIPROGDDC_kernel.pstDDCRam_Info),(void*)(stHDMIPROGDDC_kernel_user.pstDDCRam_Info),sizeof(XC_DDCRAM_PROG_INFO));
                    stXCDDCRAM.EDID = u8EDID;
                    u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&stHDMIPROGDDC_kernel);         
                    kfree(u8EDID);
                }
            }
           
            #else
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_PROG_DDCRAM,NULL,buffer_arg,sizeof(buffer_arg));
            #endif
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_CONTENT_TYPE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_CONTENT_TYPE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_EXTCOLORIMETRY:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_EXT_COLORIMETRY,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_HDMI_IF_CMD_GET_PIXEL_REPETITION:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_PIXEL_REPETITION,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_CHECK_4K2K:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_CHECK_4K2K,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_HDMI_IF_CMD_3D_4KX2K_PROCESS:
        {
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }
        case E_XC_HDMI_IF_CMD_SetHPD_HL:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_SetHPD_HL,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_XC_HDMI_IF_CMD_AVG_SCALERINGDOWN:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_AVG_SCALERINGDOWN,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_CHECK_ADDITIONAL_FORMAT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_CHECK_ADDITIONAL_FORMAT,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_3D_STRUCTURE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_3D_STRUCTURE,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_HDMI_CMD_GET_3D_EXT_DATA:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_3D_EXT_DATA,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_3D_META_FIELD:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_3D_META_FIELD,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_VIC_CODE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_VIC_CODE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_GET_4KX2K_VIC_CODE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_HDMI_GET_4KX2K_VIC_CODE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_ARC_PINCONTROL:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_ARC_PINCONTROL,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_SET_POWERSTATE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_HDMI_SET_POWERSTATE,spt_XC_HDMI_SET_POWERSTATE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_HDMI_IF_CMD_DVI_HF_ADJUST:
        {
		    u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
		    break;
        }

        case E_XC_HDMI_IF_CMD_NONE:
        case E_XC_HDMI_IF_CMD_MAX:
        default:
        {
            printf("Command %ld is not existed\n",u32Cmd);
                u32Ret = UTOPIA_STATUS_ERR_INVALID_HANDLE;
                break;
        }

    }

	return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

#endif

