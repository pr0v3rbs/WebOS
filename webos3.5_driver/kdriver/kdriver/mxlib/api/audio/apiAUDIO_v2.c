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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiAUDIO_v2.c
/// @brief  Audio 1.0 API wrapper for Audio 2.0
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "utopia.h"
#include "apiAUDIO_private.h"
#include "apiAUDIO_v2.h"

#include "utopia_dapi.h"
//#include "util_symbol.h"
#include "MsTypes.h"
#include "MsOS.h"
#include "MsCommon.h"
#include "MsVersion.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#endif

MS_U8 g_api_audio_V2_Dbg_Msg_bEnable = 0;
MS_U8 g_api_audio_V2_Dbg_Return_bEnable = 0;

#define AUDIO_V2DBGMSG(_f, _a...)                                                                   \
    {                                                                                               \
        if(g_api_audio_V2_Dbg_Msg_bEnable)                                                          \
        {                                                                                           \
            printf("\033[1;35m" "[%s]" "[%d]" _f "\033[0m" "\n", __FUNCTION__, __LINE__, ##_a);     \
        }                                                                                           \
    }

#define AUDIO_DBGRT(_f, _a...)                                                                      \
    {                                                                                               \
        if(g_api_audio_V2_Dbg_Return_bEnable)                                                       \
        {                                                                                           \
            printf("\033[1;35m" "[%s]" "[%d]" _f "\033[0m" "\n", __FUNCTION__, __LINE__, ##_a);     \
        }                                                                                           \
    }


//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
#ifdef AUDIO_UTOPIA_20
static void*                        _pInstantAudio    = NULL;
static void*                        _pAttributeAudio  = NULL;
#endif

//------------------------------------------------------------------------------
// Internal implementation
//------------------------------------------------------------------------------

#ifdef AUDIO_UTOPIA_20
static MS_BOOL _AUDIO_IOCtrlOpen(void)
{

    if(_pInstantAudio == NULL)
    {
        #ifdef  AUDIO_UTOPIA2K
        if(UtopiaOpen(MODULE_AUDIO|KERNEL_MODE, &_pInstantAudio, 0, _pAttributeAudio) !=  UTOPIA_STATUS_SUCCESS)
        #else
        if(UtopiaOpen(MODULE_AUDIO, &_pInstantAudio, 0, _pAttributeAudio) !=  UTOPIA_STATUS_SUCCESS)
        #endif
        {
            printf("[MAPI AUDIO][%06d] Open Audio Lib fail\n", __LINE__);
            return FALSE;
        }
    }

    return TRUE;
}
#endif

//============================================================
// AUDIO_SYSTEM RELATIONAL API FUNCTION
//============================================================

void MApi_AUDIO_Initialize(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Audio Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Initialize, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Audio Init fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_Initialize();

#endif
}


void MApi_AUDIO_SetPowerOn(MS_BOOL bFlag)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param = ((MS_U32)bFlag);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Audio Set Power on  fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetPowerOn, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Audio Set Power on fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetPowerOn(bFlag);

#endif
}

void MApi_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_U32 u32Bin_Base_Address, MS_U32 u32Mad_Base_Buffer_Adr)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[3] = {0,0,0};
    u32Param[0] = u8Index;
    u32Param[1] = u32Bin_Base_Address;
    u32Param[2] = u32Mad_Base_Buffer_Adr;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set DSP Base Address  fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDSPBaseAddr, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set DSP Base Address fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetDspBaseAddr(u8Index, u32Bin_Base_Address, u32Mad_Base_Buffer_Adr);

#endif
}

MS_U32 MApi_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0,0};
    u32Param[1] = u8Index;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get  DSP Base Address  fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDSPBaseAddr, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get DSP Base Address fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return u32Param[0];

#else

    return _MApi_AUDIO_GetDspMadBaseAddr(u8Index);

#endif
}

void MApi_AUDIO_WriteDecMailBox(MS_U8 u8ParamNum, MS_U16 u16Data)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8ParamNum;
    u32Param[1] = u16Data;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Write Dec Mail Box  fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_WriteDecMailBox, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Write Dec Mail Box fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_WriteDecMailBox(u8ParamNum, u16Data);

#endif
}

void MApi_AUDIO_TriggerSifPLL(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Trigger SIF PLL fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_TriggerSIFPLL, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Trigger SIF PLL fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_TriggerSifPLL();

#endif
}

MS_U8 MApi_Audio_Monitor(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Monitor fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Monitor, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Monitor fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return (MS_U8)u32Param[0];

#else

    return _MApi_Audio_Monitor();

#endif
}

void MApi_AUDIO_SetSystemInfo(AUDIO_INIT_INFO *pau_info)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set System Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSystemInfo, (void*)pau_info) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set System Info fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSystemInfo(pau_info);

#endif
}

void MApi_AUDIO_SetOutConnectivity(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set out connectivity fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetOutConnectivity, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set out connectivity fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetOutConnectivity();

#endif
}

void MApi_AUDIO_SetPathInfo(AUDIO_PATH_INFO *path_info)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Path Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetPathInfo, (void*)path_info) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Path Info fai\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetPathInfo(path_info);

#endif
}

void MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Output Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetOutputInfo, (void*)pout_info) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Output Info fai\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetOutputInfo(pout_info);

#endif
}

void MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType)
{
    AUDIO_V2DBGMSG("[%s] %d \n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param = ((MS_U32)eSourceType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Source Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSourceInfo, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Source Info fai\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSourceInfo(eSourceType);

#endif
}

void MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0};
    u32Param[0] = ((MS_U32)enSource);
    u32Param[1] = ((MS_U32)enGroup);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Input Switch fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_InputSwitch, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Input Switch fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_InputSwitch(enSource, enGroup);

#endif
}

MS_BOOL MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[3] = {0};
    u32Param[0] = 0;
    u32Param[1] = ((MS_U32)eID);
    u32Param[2] = ((MS_U32)eSource);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Data Capture Source fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDataCaptureSource, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Data Capture Source fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32Param[0]);
    return (MS_BOOL)u32Param[0];

#else

    return _MApi_AUDIO_SetDataCaptureSource(eID, eSource);

#endif
}

void MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_OUTPUT_SOURCE Status;
    Status.bSet = TRUE;
    Status.stOutputSourceInfo = pSourceInfo;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Output Source Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_OutputSourceInfo, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Output Source Info fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("\033[1;33m [%s] Return, Speaker %d Hp %d Monit %d Scart %d Spdif %d Arc %d HDMI %d \033[0m \n",
        __FUNCTION__, (int)pSourceInfo->SpeakerOut, (int)pSourceInfo->HpOut, (int)pSourceInfo->MonitorOut,
        (int)pSourceInfo->ScartOut, (int)pSourceInfo->SpdifOut, (int)pSourceInfo->ArcOut, (int)pSourceInfo->HDMIOut);

    return;

#else

    return _MApi_AUDIO_SetOutputSourceInfo(pSourceInfo);

#endif
}

void  MApi_AUDIO_GetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_OUTPUT_SOURCE Status;
    Status.bSet = FALSE;
    Status.stOutputSourceInfo = pSourceInfo;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Output Source Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_OutputSourceInfo, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Output Source Info fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("\033[1;33m [%s] Return, Speaker %d Hp %d Monit %d Scart %d Spdif %d Arc %d HDMI %d \033[0m \n",
        __FUNCTION__, (int)pSourceInfo->SpeakerOut, (int)pSourceInfo->HpOut, (int)pSourceInfo->MonitorOut,
        (int)pSourceInfo->ScartOut, (int)pSourceInfo->SpdifOut, (int)pSourceInfo->ArcOut, (int)pSourceInfo->HDMIOut);
    return;

#else

    return _MApi_AUDIO_GetOutputSourceInfo(pSourceInfo);

#endif
}

MS_U16 MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[3] = {0};
    u32Param[1] = ((MS_U32)bDspType);
    u32Param[2] = ((MS_U32)u8ParamNum);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Read MailBox fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ReadMailBox, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Read MailBox fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32Param[0]);
    return (MS_U16)u32Param[0];

#else

    return _MApi_AUDIO_ReadMailBox(bDspType, u8ParamNum);

#endif
}

void MApi_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[3] = {0};
    u32Param[0] =((MS_U32)bDspType);
    u32Param[1] = ((MS_U32)u8ParamNum);
    u32Param[2] = ((MS_U32)u16Data);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Write MailBox fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_WriteMailBox, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Write MailBox fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_WriteMailBox(bDspType, u8ParamNum, u16Data);

#endif
}

void MApi_AUDIO_FwTriggerDSP(MS_U16 u16Cmd)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param = ((MS_U32)u16Cmd);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Fw Trigger DSP fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FwTriggerDSP, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Fw Trigger DSP fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_FwTriggerDSP(u16Cmd);

#endif
}

void MApi_AUDIO_SetFwStatus(MS_BOOL bBootOnDDR)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param = ((MS_U32)bBootOnDDR);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Fw Status fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetFwStatus, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Fw Status fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetFwStatus(bBootOnDDR);

#endif
}

void MApi_AUDIO_ExitAudioSystem(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Exit Auido system fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ExitAudioSystem, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Exit Auido system fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_ExitAudioSystem();

#endif
}

void MApi_AUDIO_RebootDsp(MS_BOOL bDspType)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param = ((MS_U32)bDspType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Reboot DSP fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_RebootDSP, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Reboot DSP fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_RebootDsp(bDspType);

#endif
}

void MApi_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U8 u8Cmd)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[2] = {0};
    u32Param[0] = ((MS_U32)bDspType);
    u32Param[1] = ((MS_U32)u8Cmd);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Send Intrupt fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SendIntrupt, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Send Intrupt fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SendIntrupt(bDspType, u8Cmd);

#endif
}

void MApi_AUDIO_I2S_SetMode(AUDIO_I2S_MODE_TYPE u8Mode, AUDIO_I2S_MODE_VALUE  u8Val)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[2] = {0};
    u32Param[0] = ((MS_U32)u8Mode);
    u32Param[1] = ((MS_U32)u8Val);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] I2S SetMode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_I2S_SetMode, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] I2S SetMode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_I2S_SetMode(u8Mode, u8Val);

#endif
}

void MApi_AUDIO_WritePreInitTable(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Write PreInit Table fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_WritePreInitTable, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Write PreInit Table fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_WritePreInitTable();

#endif
}

void MApi_AUDIO_EnaEarphone_LowPower_Stage(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable EarPhone LowPower Stage fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable EarPhone LowPower Stage fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_EnaEarphone_LowPower_Stage();

#endif
}

void MApi_AUDIO_EnaEarphone_HighDriving_Stage(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable EarPhone HighDriving Stage fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable EarPhone HighDriving Stage fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_EnaEarphone_HighDriving_Stage();

#endif
}

//=============================================================
// AUDIO_SOUND RELATIONAL API FUNCTION
//=============================================================
MS_BOOL MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[3] = {0};
    u32Param[1] = ((MS_U32)Type);
    u32Param[2] = ((MS_U32)enable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SND Process Enable fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_SND_ProcessEnable, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SND Process Enable fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return input1 %d input2 %d val %d\n", __FUNCTION__, (int)Type, (int)enable, (int)u32Param[0]);
    return (MS_BOOL)u32Param[0];

#else

    return _MApi_SND_ProcessEnable(Type, enable);

#endif
}

MS_BOOL MApi_SND_SetParam1( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[4] = {0};
    u32Param[1] = ((MS_U32)Type);
    u32Param[2] = ((MS_U32)param1);
    u32Param[3] = ((MS_U32)param2);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SND Set Param1 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_SND_SetParam1, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SND Set Param1 fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32Param[0]);
    return (MS_BOOL)u32Param[0];

#else

    return _MApi_SND_SetParam1(Type, param1, param2);

#endif
}

MS_U16 MApi_SND_GetParam1( Sound_GET_PARAM_Type Type, MS_U16 param1)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[3] = {0};
    u32Param[1] = ((MS_U32)Type);
    u32Param[2] = ((MS_U32)param1);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SND Get Param1 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_SND_GetParam1, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SND Get Param1 fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32Param[0]);
    return (MS_U16)u32Param[0];

#else

    return _MApi_SND_GetParam1(Type, param1);

#endif
}

MS_BOOL MApi_AUDIO_SOUND_SetParam(Sound_SET_Type Type, MS_U32 *p_param )        // This function didn't call by upper layer
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Type;
    Status.pParam = p_param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Sound Set Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Sound Set Param fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_SOUND_SetParam(Type, p_param);

#endif
}

void MApi_AUDIO_SetAbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[3] = {0};
    u32Param[0] = ((MS_U32)u8Path);
    u32Param[1] = ((MS_U32)u8Vol1);
    u32Param[2] = ((MS_U32)u8Vol2);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Absolute Volume fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAbsoluteVolume, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Absolute Volume fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetAbsoluteVolume(u8Path, u8Vol1, u8Vol2);

#endif
}

void MApi_AUDIO_SetPreScale(MS_U8 u8Path, MS_U8 u8PreScale )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Path;
    u32Param[1] = u8PreScale;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetPreScale;
    Status.pParam = (MS_U32*)u32Param;
    //MS_U32 u32Param[2] = {0};
    //u32Param[0] = ((MS_U32)u8Path);
    //u32Param[1] = ((MS_U32)u8PreScale);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set PreScale fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set PreScale fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetPreScale(u8Path, u8PreScale);

#endif
}

void MApi_AUDIO_SetMute(MS_U8 u8Path, MS_BOOL EnMute )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Path;
    u32Param[1] = EnMute;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetMute;
    Status.pParam = (MS_U32*)u32Param;

    AUDIO_V2DBGMSG("[%s] pParam[0] %d pParam[1] %d\n", __FUNCTION__, (int)Status.pParam[0], (int)Status.pParam[1]);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mute fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0 u8Path %d EnMute %d\n", __FUNCTION__, (int)u8Path, (int)EnMute);
    return;

#else

    return _MApi_AUDIO_SetMute(u8Path, EnMute);

#endif
}

void MApi_AUDIO_SourceConnect(AUDIO_OUTPORT_SOURCE_TYPE eConnectSourceType, MS_BOOL bConnect )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[2] = {0};
    u32Param[0] = ((MS_U32)eConnectSourceType);
    u32Param[1] = ((MS_U32)bConnect);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Source connect fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SourceConnect, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Source connect fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SourceConnect(eConnectSourceType, bConnect);

#endif
}

void MApi_AUDIO_SetBalance(MS_U8 u8Balance)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Balance;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetBalance;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Balance fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Balance fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetBalance(u8Balance);

#endif
}

void MApi_AUDIO_EnableEQ(MS_BOOL EnEQ)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnEQ;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableEQ;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable EQ fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable EQ fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_EnableEQ(EnEQ);

#endif
}

void MApi_AUDIO_EnableTone(MS_BOOL EnTone)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnTone;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableTone;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Tone fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Tone fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_EnableTone(EnTone);

#endif
}

void MApi_AUDIO_EnableAutoVolume(MS_BOOL EnAVC)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnAVC;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableAutoVolume;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_EnableAutoVolume(EnAVC);

#endif
}

void MApi_AUDIO_EnableSurround(MS_BOOL EnSurround)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnSurround;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableSurround;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Surround fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Surround fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_EnableSurround(EnSurround);

#endif
}

void MApi_AUDIO_EnableBalance(MS_BOOL EnBalance)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnBalance;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableBalance;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Balance fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Balance fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_EnableBalance(EnBalance);

#endif
}

void MApi_AUDIO_SetDynamicBass(MS_U8 u8DynamicBass )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param = ((MS_U32)u8DynamicBass);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Dynamic Bass fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDynamicBass, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Dynamic Bass fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetDynamicBass(u8DynamicBass);

#endif
}

void MApi_AUDIO_SetBass(MS_U8 u8Bass)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Bass;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetBass;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Bass fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Bass fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetBass(u8Bass);

#endif
}

void MApi_AUDIO_AbsoluteBass(MS_U8 u8Bass)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Bass;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_AbsoluteBass;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Absolute Bass fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Absolute Bass fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_AbsoluteBass(u8Bass);

#endif
}

void MApi_AUDIO_SetTreble(MS_U8 u8Treble)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Treble;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetTreble;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Treble fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Treble fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetTreble(u8Treble);

#endif
}

void MApi_AUDIO_AbsoluteTreble(MS_U8 u8Treble )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param = ((MS_U32)u8Treble);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Absolute Treble fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_AbsoluteTreble, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Absolute Treble fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_AbsoluteTreble(u8Treble);

#endif
}

void MApi_AUDIO_SetEq(MS_U8 u8band, MS_U8 u8level)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8band;
    u32Param[1] = u8level;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetEq;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set EQ fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set EQ fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetEq(u8band, u8level);

#endif
}

#if 0
void MApi_AUDIO_SetEq_7band(MS_U8 u8band, MS_U8 u8level)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8band;
    u32Param[1] = u8level;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetEq_7band;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set EQ 7band fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set EQ 7band fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetEq_7band(u8band, u8level);

#endif
}
#endif

void MApi_AUDIO_SetAvcThreshold(MS_U8 Level)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = Level;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetAvcThreshold;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AVC Threshold fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AVC Threshold fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetAvcThreshold(Level);

#endif
}

void MApi_AUDIO_SetAvcMode(MS_U8 AvcMode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = AvcMode;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetAvcMode;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AVC Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AVC Mode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetAvcMode(AvcMode);

#endif
}

void MApi_AUDIO_SetAvcAT(MS_U8 AvcAT)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = AvcAT;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetAvcAT;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AVC AT fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AVC AT fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetAvcAT(AvcAT);

#endif
}

void MApi_AUDIO_SetAvcRT(MS_U8 AvcRT)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = AvcRT;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetAvcRT;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AVC RT fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AVC RT fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetAvcRT(AvcRT);

#endif
}

void MApi_AUDIO_SetBufferProcess(MS_U8 DelayTime)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param = ((MS_U32)DelayTime);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Buffer Process fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetBufferProcess, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Buffer Process fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetBufferProcess(DelayTime);

#endif
}

void MApi_AUDIO_SetSurroundXA(MS_U8 mode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = mode;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetSurroundXA;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Surround XA fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Surround XA fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSurroundXA(mode);

#endif
}

void MApi_AUDIO_SetSurroundXB(MS_U8 mode )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = mode;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetSurroundXB;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Surround XB fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Surround XB fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSurroundXB(mode);

#endif
}

void MApi_AUDIO_SetSurroundXK( MS_U8 mode )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = mode;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetSurroundXK;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Surround XK fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Surround XK fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSurroundXK(mode);

#endif
}

void MApi_AUDIO_SetSurroundLPFGain(MS_U8 mode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = mode;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetSurroundLPFGain;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Surround LPF Gain fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Surround LPF Gain fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSurroundLPFGain(mode);

#endif
}

MS_U8 MApi_AUDIO_ConvertVolumeUnit(MS_U8 UiVolume)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[2] = {0};
    u32Param[1] = ((MS_U32)UiVolume);


    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Convert Volume Unit fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ConvertVolumeUnit, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Convert Volume Unit fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32Param[0]);
    return (MS_U8)u32Param[0];

#else

    return _MApi_AUDIO_ConvertVolumeUnit(UiVolume);

#endif
}

void MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef)
{
#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set PEQ Coef fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetPEQCoef, (void*)peq_coef) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set PEQ Coef fai\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetPEQCoef(peq_coef);

#endif
}

void MApi_AUDIO_EnablePEQ(MS_BOOL Enpeq)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = Enpeq;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnablePEQ;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable PEQ fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable PEQ fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_EnablePEQ(Enpeq);

#endif
}

MS_BOOL MApi_AUDIO_EnableDcRemove(MS_BOOL EnDcRemove)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[2] = {0};
    u32Param[1] = ((MS_U32)EnDcRemove);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable DC Remove fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EnableDcRemove, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable DC Remove fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__,(int)u32Param[0]);
    return (MS_BOOL)u32Param[0];

#else

    return _MApi_AUDIO_EnableDcRemove(EnDcRemove);

#endif
}
//============================================================
// AUDIO_DTV RELATIONAL API FUNCTION
//============================================================
MS_BOOL MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_RELEASE_DECODESYSTEM Status;
    Status.bStatus = FALSE;
    Status.DecId = DecId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Release Decode System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ReleaseDecodeSystem, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Release Decode System fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__,(int)Status.bStatus);
    return (MS_BOOL)Status.bStatus;

#else

    return _MApi_AUDIO_ReleaseDecodeSystem(DecId);

#endif
}

MS_BOOL MApi_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SET_DECODESYSTEM Status;
    Status.bStatus = FALSE;
    Status.DecId = DecId;
    Status.p_AudioDecStatus = p_AudioDecStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Decode System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeSystem, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Decode System fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("\033[1;32m[%s] Return val %d, bStart %d bConnect %d bAVSyndOn %d bIsAD %d eGroup %d eDSPID %d eSourceType %d eAudFormat %d eStrcSource %d eAfifoSource %d \033[0m\n",
                __FUNCTION__,(int)Status.bStatus,
                (int)p_AudioDecStatus->bStart, (int)p_AudioDecStatus->bConnect, (int)p_AudioDecStatus->bAVSyncOn, (int)p_AudioDecStatus->bIsAD, (int)p_AudioDecStatus->eGroup,
                (int)p_AudioDecStatus->eDSPId, (int)p_AudioDecStatus->eSourceType, (int)p_AudioDecStatus->eAudFormat, (int)p_AudioDecStatus->eStcSource, (int)p_AudioDecStatus->eAfifoSource);

    return Status.bStatus;

#else

    return _MApi_AUDIO_SetDecodeSystem(DecId, p_AudioDecStatus);

#endif
}

MS_BOOL MApi_AUDIO_GetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_GET_DECODESYSTEM Status;
    Status.bStatus = FALSE;
    Status.DecId = DecId;
    Status.p_AudioDecStatus = p_AudioDecStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Decode System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDecodeSystem, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Decode System fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("\033[1;32m[%s] Return val %d, bStart %d bConnect %d bAVSyndOn %d bIsAD %d eGroup %d eDSPID %d eSourceType %d eAudFormat %d eStrcSource %d eAfifoSource %d \033[0m\n",
                __FUNCTION__,(int)Status.bStatus,
                (int)p_AudioDecStatus->bStart, (int)p_AudioDecStatus->bConnect, (int)p_AudioDecStatus->bAVSyncOn, (int)p_AudioDecStatus->bIsAD, (int)p_AudioDecStatus->eGroup,
                (int)p_AudioDecStatus->eDSPId, (int)p_AudioDecStatus->eSourceType, (int)p_AudioDecStatus->eAudFormat, (int)p_AudioDecStatus->eStcSource, (int)p_AudioDecStatus->eAfifoSource);
    return Status.bStatus;

#else

    return _MApi_AUDIO_GetDecodeSystem(DecId, p_AudioDecStatus);

#endif
}

MS_BOOL MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[3] = {0};
    u32Param[1] = ((MS_U32)DecId);
    u32Param[2] = ((MS_U32)enDecComamnd);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Decode Cmd fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeCmd, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Decode Cmd fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32Param[0]);
    return (MS_BOOL)u32Param[0];

#else

    return _MApi_AUDIO_SetDecodeCmd(DecId, enDecComamnd);

#endif
}

AUDIO_DEC_ID MApi_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_OPEN_DECODESYSTEM Status;
    Status.DecId = AU_DEC_INVALID;
    Status.p_AudioDecStatus = p_AudioDecStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Open Decode System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_OpenDecodeSystem, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Open Decode System fail\n", __LINE__);
        //return;
    }

    AUDIO_DBGRT("\033[1;31m [AUDIO][%s] [%s] [%d] [RetDecId %d] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, (int)Status.DecId);

    return Status.DecId;

#else

    return _MApi_AUDIO_OpenDecodeSystem(p_AudioDecStatus);

#endif
}

MS_BOOL MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0};
    u32Param[1] = ((MS_U32)enDecSystem);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSystem, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set System fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return input %d val %d\n", __FUNCTION__,(int)enDecSystem, (int)u32Param[0]);
    return (MS_BOOL)u32Param[0];

#else

    return _MApi_AUDIO_SetSystem(enDecSystem);

#endif
}

MS_BOOL MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0};
    u32Param[1] = ((MS_U32)enDecSystem);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Dec Sys Support Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDecSysSupportStatus, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Dec Sys Support Status fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32Param[0]);
    return (MS_BOOL)u32Param[0];

#else

    return _MApi_AUDIO_GetDecSysSupportStatus(enDecSystem);

#endif
}

MS_BOOL MApi_AUDIO_IsMadLock(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Is Mad Lock fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_IsMadLock, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Is Mad Lock fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((MS_BOOL)u32param[1]);

#else

    return _MApi_AUDIO_IsMadLock();

#endif
}

En_DVB_decCmdType MApi_AUDIO_GetDecStatus(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Dec Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDecStatus, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Dec Status fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((En_DVB_decCmdType)u32param[1]);

#else

    return _MApi_AUDIO_GetDecStatus();

#endif
}

AU_DVB_DECCMD MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_DECODER_COMMAND Status;
    Status.bSet = FALSE;
    Status.eDecCmd = AU_DVB_DECCMD_STOP;
    Status.eAdoDecoderNo = AdoDecoderNo;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Command fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Decode_Command, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Command fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.eDecCmd);
    return Status.eDecCmd;

#else

    return _MApi_AUDIO_GetCommand(AdoDecoderNo);

#endif
}

void MApi_AUDIO_SetCommand(En_DVB_decCmdType enDecComamnd)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_DECODER_COMMAND Status;
    Status.bSet = TRUE;
    Status.eDecCmd = AU_DVB_DECCMD_STOP;
    Status.enCommandType = enDecComamnd;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Command fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Decode_Command, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Command fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetCommand(enDecComamnd);

#endif
}

MS_BOOL MApi_AUDIO_GetMAD_LOCK(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get MAD Lock fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetMAD_Lock, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get MAD Lock fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((MS_BOOL)u32param[1]);;

#else

    return _MApi_AUDIO_GetMAD_LOCK();

#endif
}

void MApi_AUDIO_SetADOutputMode(MS_U8 out_mode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SET_AD Status;
    Status.eType = AD_OUTPUT_MODE;
    Status.u8Data = out_mode;
    Status.bMute = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AD Output Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SETAD, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AD Output Mode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return input0 %d\n", __FUNCTION__, (int)out_mode);
    return;

#else

    return _MApi_AUDIO_SetADOutputMode(out_mode);

#endif
}

void MApi_AUDIO_SetADAbsoluteVolume(MS_U8 AbsVolume)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SET_AD Status;
    Status.eType = AD_ABSOLUTE_VOLUME;
    Status.u8Data = AbsVolume;
    Status.bMute = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AD Absolute Volume fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SETAD, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AD Absolute Volume fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetADAbsoluteVolume(AbsVolume);

#endif
}

void MApi_AUDIO_ADSetMute(MS_BOOL EnMute)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SET_AD Status;
    Status.eType = AD_SET_MUTE;
    Status.u8Data = 0;
    Status.bMute = EnMute;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AD Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SETAD, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AD Mute fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_ADSetMute(EnMute);

#endif
}

//============================================================
// AUDIO_PVR RELATED FUNCTIONS
//============================================================
void MApi_AUDIO_SetEncInit(AU_DVB_ENCBR BitRate, MS_U8 u8EncFrameNum)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0};
    u32Param[0] = ((MS_U32)BitRate);
    u32Param[1] = ((MS_U32)u8EncFrameNum);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Enc Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetEncInit, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Enc Init fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetEncInit(BitRate, u8EncFrameNum);

#endif
}

void MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD enEncComamnd, void* pCB, MS_U8 u8Param)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    AUDIO_ST_SETENCCOMMAND Status;
    Status.enEncComamnd = enEncComamnd;
    Status.pCB = pCB;
    Status.u8Param = u8Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Enc Command fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetEncCommand, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Enc Command fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetEncCommand(enEncComamnd, pCB, u8Param);

#endif
}

void MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Enc Frame Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetEncodeFrameInfo, (void*)EncFrameInfo) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Enc Frame Info fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return u32Addr 0x%x u32Size 0x%x u64Idx 0x%x\n", __FUNCTION__, (int)EncFrameInfo->u32Addr, (int)EncFrameInfo->u32Size, (int)EncFrameInfo->u64Idx);
    return;

#else

    return _MApi_AUDIO_GetEncodeFrameInfo(EncFrameInfo);

#endif
}

MS_BOOL MApi_AUDIO_GetEncodeDoneFlag(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Encode Done Flag fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetEncodeDoneFlag, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Encode Done Flag fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((MS_BOOL)u32param[1]);;

#else

    return _MApi_AUDIO_GetEncodeDoneFlag();

#endif
}

MS_BOOL MApi_AUDIO_SetEncInfo(AUDIO_ENC_INFO_TYPE infoType, MS_U32 param1, MS_U32 param2)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_ENC_INFO Status;
    Status.bSet = TRUE;
    Status.infoType = infoType;
    Status.param1 = param1;
    Status.param2 = param2;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Encode Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EncInfo, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Encode Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bRet);
    return Status.bRet;

#else

    return _MApi_AUDIO_SetEncInfo(infoType, param1,  param2);

#endif
}

MS_U32 MApi_AUDIO_GetEncInfo(AUDIO_ENC_INFO_TYPE infoType)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_ENC_INFO Status;
    Status.bSet = FALSE;
    Status.infoType = infoType;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Encode Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EncInfo, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Encode Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.u32Ret);
    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetEncInfo( infoType);

#endif
}

void MApi_AUDIO_SetCaptureEnable(MS_BOOL bEnable, void* PCM_CB)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SET_CAPTURE_ENABLE Status;
    Status.bEnable = bEnable;
    Status.PCM_CB = PCM_CB;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Capture Enable fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetCaptureEnable, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Capture Enable fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetCaptureEnable(bEnable, PCM_CB);

#endif

}
//============================================================
// AUDIO_ATV RELATIONAL API FUNCTION
//============================================================
void MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param = ((MS_U32)standard_type);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set Standard fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetStandard, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set Standard fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_SetStandard(standard_type);

#endif
}

MS_U8 MApi_AUDIO_SIF_GetSoundMode(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Get Sound Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_GetSoundMode, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Get Sound Mode fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((MS_BOOL)u32param[1]);

#else

    return _MApi_AUDIO_SIF_GetSoundMode();

#endif
}

MS_BOOL MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0};
    u32Param[1] = ((MS_U32)u8SifSoundMode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set Sound Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetSoundMode, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set Sound Mode fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32Param[0]);
    return (MS_BOOL)u32Param[0];

#else

    return _MApi_AUDIO_SIF_SetSoundMode(u8SifSoundMode);

#endif
}

MS_BOOL MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    AUDIO_ST_SIF_GET_AUDIO_STATUS Status;

    Status.bStatus = FALSE;
    Status.p_eAudioStatus = eAudioStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Get Audio Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_GetAudioStatus, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Get Audio Status fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[AUDIO] [%s] [%d] [eAudioStatus 0x%x] \n", __FUNCTION__, __LINE__, (int)*eAudioStatus);

    return Status.bStatus;

#else

    return _MApi_AUDIO_SIF_GetAudioStatus(eAudioStatus);

#endif
}

void MApi_AUDIO_SIF_StartAutoStandardDetection(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Start Auto standard Detection fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_StartAutoStandardDetection, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Start Auto standard Detection fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_StartAutoStandardDetection();

#endif
}

AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Get Result of Auto Standard Detection fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Get Result of Auto Standard Detection fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((AUDIOSTANDARD_TYPE)u32param[1]);

#else

    return _MApi_AUDIO_SIF_GetResultOfAutoStandardDetection();

#endif
}

AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[1] = ((MS_U32)eStandard);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Convert To Basic Audio Standard fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Convert To Basic Audio Standard fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return ((AUDIOSTANDARD_TYPE)u32param[0]);

#else

    return _MApi_AUDIO_SIF_ConvertToBasicAudioStandard(eStandard);

#endif
}

void MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set Threshold fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetThreshold, (THR_TBL_TYPE code*)ThrTbl) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set Threshold fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_SetThreshold(ThrTbl);

#endif
}

void MApi_AUDIO_SIF_SetPrescale(MS_U8 gain_type, int db_value)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0};
    u32Param[0] = ((MS_U32)gain_type);
    u32Param[1] = ((MS_U32)db_value);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set Prescale fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetPrescale, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set Prescale fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_SetPrescale(gain_type, db_value);

#endif
}

MS_BOOL MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[2] = {0};
    u32Param[1] = ((MS_U32)pal_type);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Is PAL Type fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_IsPALType, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Is PAL Type fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32Param[0]);
    return (MS_BOOL)u32Param[0];

#else

    return _MApi_AUDIO_SIF_IsPALType(pal_type);

#endif
}

void MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param = ((MS_U32)pal_type);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set PAL Type fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetPALType, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Set PAL Type fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_SetPALType(pal_type);

#endif
}

void MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param[3] = {0};
    u32Param[0] = ((MS_U32)enAudSifCommand);
    u32Param[1] = ((MS_U32)comm_arg1);
    u32Param[2] = ((MS_U32)comm_arg2);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Send Cmd fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SendCmd, (void*)u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Send Cmd fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_SendCmd(enAudSifCommand, comm_arg1, comm_arg2);

#endif
}

void MApi_AUDIO_SIF_Shift(En_AUD_VIF_Type type)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    MS_U32 u32Param = ((MS_U32)type);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Shift fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_Shift, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SIF Shift fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_Shift(type);

#endif
}

//============================================================
// AUDIO_FM_RADIO RELATIONAL API FUNCTION
//============================================================
#if 0
MS_U8 MApi_AUDIO_FM_RADIO_GetSoundMode(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Get Sound Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_GetSoundMode, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Get Sound Mode fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((MS_U8)u32param[1]);

#else

    return _MApi_AUDIO_FM_RADIO_GetSoundMode();

#endif
}

void MApi_AUDIO_FM_RADIO_SetSoundMode(MS_U8 u8FMRadioSoundMode)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)u8FMRadioSoundMode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Set Sound Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_SetSoundMode, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Set Sound Mode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_FM_RADIO_SetSoundMode(u8FMRadioSoundMode);

#endif
}

void MApi_AUDIO_FM_RADIO_DeEmphassisOption(MS_BOOL Is_Europe)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)Is_Europe);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio DeEmphassisOption fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_DeEmphassisOption, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio DeEmphassisOption fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_FM_RADIO_DeEmphassisOption(Is_Europe);

#endif
}

MS_S8 MApi_AUDIO_FM_RADIO_GET_DC_AMP(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_S32 s32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Get DC AMP fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_GetDC_Amp, (void*)s32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Get DC AMP fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)s32param[1]);
    return ((MS_S8)s32param[1]);

#else

    return _MApi_AUDIO_FM_RADIO_GET_DC_AMP();

#endif
}

MS_U8 MApi_AUDIO_FM_RADIO_GET_NSR(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Get NSR fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_Get_NSR, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Get NSR fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((MS_U8)u32param[1]);

#else

    return _MApi_AUDIO_FM_RADIO_GET_NSR();

#endif
}

void MApi_AUDIO_FM_RADIO_RESET(void)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Reset fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_Reset, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] FM Radio Reset fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_FM_RADIO_RESET();

#endif
}
#endif
//=============================================================
// AUDIO_SPDIF RELATIONAL API FUNCTION
//=============================================================
void MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_en)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SPDIF_OP Status;
    Status.eType = SPDIF_HWEN;
    Status.spdif_hwen = spdif_en;
    Status.mute_en = FALSE;
    Status.spdif_mode = SPDIF_OUT_NONE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF HWEN fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF HWEN fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SPDIF_HWEN(spdif_en);

#endif
}

void MApi_AUDIO_SPDIF_SetMute(MS_BOOL mute_en)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SPDIF_OP Status;
    Status.eType = SPDIF_SETMUTE;
    Status.spdif_hwen = FALSE;
    Status.mute_en = mute_en;
    Status.spdif_mode = SPDIF_OUT_NONE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Set Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Set Mute fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SPDIF_SetMute(mute_en);

#endif
}

void MApi_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode)
{
     AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SPDIF_OP Status;
    Status.eType = SPDIF_SETMODE;
    Status.spdif_hwen = FALSE;
    Status.mute_en = FALSE;
    Status.spdif_mode = spdif_mode;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Set Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Set Mode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SPDIF_SetMode(spdif_mode);

#endif
}

MS_U8 MApi_AUDIO_SPDIF_GetMode(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SPDIF_OP Status;
    Status.eType = SPDIF_GETMODE;
    Status.spdif_hwen = FALSE;
    Status.mute_en = FALSE;
    Status.spdif_mode = SPDIF_OUT_NONE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Get Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Get Mode fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.spdif_mode);
    return Status.spdif_mode;

#else

    return _MApi_AUDIO_SPDIF_GetMode();

#endif
}

void MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SPDIF_SCMS Status;
    Status.bSetSCMS = TRUE;
    Status.C_bit_en = C_bit_en;
    Status.L_bit_en = L_bit_en;
    Status.Ret_bit = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Set SCMS fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_SCMS, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Set SCMS fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SPDIF_SetSCMS(C_bit_en, L_bit_en);

#endif
}

MS_U8 MApi_AUDIO_SPDIF_GetSCMS(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SPDIF_SCMS Status;
    Status.bSetSCMS = FALSE;
    Status.C_bit_en = 0;
    Status.L_bit_en = 0;
    Status.Ret_bit = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Get SCMS fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_SCMS, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Get SCMS fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.Ret_bit);
    return Status.Ret_bit;

#else

    return _MApi_AUDIO_SPDIF_GetSCMS();

#endif
}

void MApi_Audio_SPDIF_Monitor(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Monitor fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_Monitor, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Monitor fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_Audio_SPDIF_Monitor();

#endif
}

MS_BOOL MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[3] = {0};
    u32param[1] = ((MS_U32)cs_mode);
    u32param[2] = ((MS_U32)status);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Channel Status CTRL fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SPDIF Channel Status CTRL fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return input1 %d input2 %d val %d\n", __FUNCTION__, (int)cs_mode, (int)status, (int)u32param[0]);
    return ((MS_BOOL)u32param[0]);

#else

    return _MApi_AUDIO_SPDIF_ChannelStatus_CTRL(cs_mode, status);

#endif
}

MS_BOOL MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS Status;
    Status.bStatus = FALSE;
    Status.ePath = ePath;
    Status.stChannelStatus = stChannelStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] DigitalOut Set Channel Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DigitalOut_SetChannelStatus, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] DigitalOut Set Channel Status fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__,(int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_DigitalOut_SetChannelStatus(ePath, stChannelStatus);

#endif
}

MS_BOOL MApi_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    if (_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] DigitalOut Get Channel Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DigitalOut_GetChannelStatus, (void*)stChannelStatus) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] DigitalOut Get Channel Status fail\n", __LINE__);
        //return;
    }
    return TRUE;//add for coverity issue
#else
    return _MApi_AUDIO_DigitalOut_GetChannelStatus(ePath, stChannelStatus);
#endif
}

//=============================================================
// AUDIO_HDMI RELATIONAL API FUNCTION
//=============================================================
void MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)mute_en);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Tx Set Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Tx_SetMute, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Tx Set MuteL fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_HDMI_Tx_SetMute(mute_en);

#endif
}

void MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    AUDIO_HDMI_TX_GETSTATUS Status;

    Status.onOff = FALSE;
    Status.hdmi_smpFreq = AUDIO_FS_NONE;
    Status.outType = HDMI_OUT_PCM;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI TX Get Status fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Tx_GetStatus, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI TX Get Status fail\n", __LINE__);
        return;
    }
    *onOff = Status.onOff;
    *hdmi_SmpFreq = Status.hdmi_smpFreq;
    *outType = Status.onOff;

    AUDIO_DBGRT("[%s] Return onOff %d hdmi_SmpFreq %d outTYpe %d\n", __FUNCTION__, (int)*onOff, (int)*hdmi_SmpFreq, (int)*outType);
    return;

#else

    return _MApi_AUDIO_HDMI_Tx_GetStatus(onOff, hdmi_SmpFreq, outType);

#endif
}

MS_U8 MApi_AUDIO_HDMI_Monitor(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Monitor fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Monitor, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Monitor fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((MS_U8)u32param[1]);

#else

    return _MApi_AUDIO_HDMI_Monitor();

#endif
}

MS_BOOL MApi_AUDIO_HDMI_GetNonpcmFlag(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Get NonPCM Flag fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_GetNonpcmFlag, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Get NonPCM Flag fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((MS_BOOL)u32param[1]);

#else

    return _MApi_AUDIO_HDMI_GetNonpcmFlag();

#endif
}

void MApi_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en)     //[Q] should be replace by MApi_AUDIO_HDMI_RX_SetNonpcm
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)nonPCM_en);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Set NonPCM fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_SetNonpcm, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Set NonPCM fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_HDMI_SetNonpcm(nonPCM_en);

#endif
}

AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_HDMI_NONPCM_OP Status;
    Status.bSet = TRUE;
    Status.nonPCM_en = nonPCM_en;
    Status.eRet = HDMI_RX_PCM;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI RX Set Non PCM fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Nonpcm_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI RX Set Non PCM fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.eRet);
    return Status.eRet;

#else

    return _MApi_AUDIO_HDMI_RX_SetNonpcm(nonPCM_en);

#endif
}

AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_GetNonPCM(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_HDMI_NONPCM_OP Status;
    Status.bSet = FALSE;
    Status.nonPCM_en = 0;
    Status.eRet = HDMI_RX_PCM;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI RX Get Non PCM fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Nonpcm_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI RX Get Non PCM fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.eRet);
    return Status.eRet;

#else

    return _MApi_AUDIO_HDMI_RX_GetNonPCM();

#endif
}

void MApi_AUDIO_DTV_HDMI_CFG(MS_U8 ctrl)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)ctrl);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] DTV HDMI CFG fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DTV_HDMI_CFG, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] DTV HDMI CFG fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_DTV_HDMI_CFG(ctrl);

#endif
}

MS_U16 MApi_AUDIO_HDMI_GetSynthFreq(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Get Synth Freq fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_GetSynthFreq, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Get Synth Freq fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return ((MS_U16)u32param[1]);

#else

    return _MApi_AUDIO_HDMI_GetSynthFreq();

#endif
}

void MApi_AUDIO_HDMI_SetDownSample(MS_U8 ratio)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)ratio);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Set Down Sample fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_SetDownSample, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI Set Down Sample fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_HDMI_SetDownSample(ratio);

#endif
}

void MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)outType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI TX Set Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_TX_SetMode, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI TX Set Mode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_HDMI_TX_SetMode(outType);

#endif
}

MS_BOOL MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    AUDIO_ST_RX_HDMI_AUDIO_STATUS Status;

    Status.bStatus = FALSE;
    Status.pHdmi_status = p_hdmiAudioSts;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI RX Get HDMI Audio Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] HDMI RX Get HDMI Audio Status fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d audio_type %d, SampleRate %d\n", __FUNCTION__, (int)Status.bStatus, (int)p_hdmiAudioSts->audio_type, (int)p_hdmiAudioSts->sampleRate);
    return Status.bStatus;

#else

    return _MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(p_hdmiAudioSts);

#endif

}
//=============================================================
// AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
//=============================================================
MS_BOOL MApi_AUDIO_ADVSOUND_ProcessEnable(ADVSND_TYPE type)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)type);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSOUND Precess Enable fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_ProcessEnable, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSOUND Precess Enable fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return input %d val %d\n", __FUNCTION__, (int)type, (int)u32param[0]);
    return (MS_BOOL)u32param[0];

#else

    return _MApi_AUDIO_ADVSOUND_ProcessEnable(type);

#endif
}

MS_BOOL MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[3] = {0};
    u32param[1] = ((MS_U32)proc);
    u32param[2] = ((MS_U32)enable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSOUND Sub Process Enable fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSOUND Sub Precess Enable fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return intput1 %d input2 %d val %d\n", __FUNCTION__, (int)proc, (int)enable, (int)u32param[0]);
    return (MS_BOOL)u32param[0];

#else

    return _MApi_AUDIO_ADVSOUND_SubProcessEnable(proc, enable);

#endif
}

MS_BOOL MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U16 u16value1, MS_U16 u16value2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[4] = {0};
    u32param[1] = ((MS_U32)param);
    u32param[2] = ((MS_U32)u16value1);
    u32param[3] = ((MS_U32)u16value2);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSOUND Set Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_SetParam, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSOUND Set Param fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__,(int)u32param[0]);
    return (MS_BOOL)u32param[0];

#else

    return _MApi_AUDIO_ADVSOUND_SetParam(param, u16value1, u16value2);

#endif
}

MS_BOOL MApi_AUDIO_ADVSND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[4] = {0};
    u32param[1] = ((MS_U32)param);
    u32param[2] = ((MS_U32)u32value1);
    u32param[3] = ((MS_U32)u16value2);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSND Set Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSND_SetParam, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSND Set Param fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_BOOL)u32param[0];

#else

    return _MApi_AUDIO_ADVSND_SetParam(param, u32value1, u16value2);

#endif
}

MS_BOOL MApi_AUDIO_ADVSOUND_SetParam2(ADVSND_PARAM param, MS_U32 index, MS_U32 *u32value1, MS_U16 u16value2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_ADVSOUND_PARAM2 Status;
    Status.bSet = TRUE;
    Status.param = param;
    Status.index = index;
    Status.u32value1 = u32value1;
    Status.u16value2 = u16value2;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSND Set Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_Param2, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSND Set Param fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bRet);
    return Status.bRet;

#else

    return _MApi_AUDIO_ADVSOUND_SetParam2( param,  index,  u32value1, u16value2);

#endif
}

MS_BOOL MApi_AUDIO_ADVSOUND_GetParam2(ADVSND_PARAM param, MS_U32 index, MS_U32 *u32value1, MS_U16 u16value2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_ADVSOUND_PARAM2 Status;
    Status.bSet = FALSE;
    Status.param = param;
    Status.index = index;
    Status.u32value1 = u32value1;
    Status.u16value2 = u16value2;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSND Set Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_Param2, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSND Set Param fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bRet);
    return Status.bRet;

#else

    return _MApi_AUDIO_ADVSOUND_GetParam2( param,  index,  u32value1, u16value2);

#endif
}

MS_U32 MApi_AUDIO_ADVSOUND_GetInfo(AdvSound_GET_Type Type)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)Type);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSOUND Get Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_GetInfo, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADVSOUND Get Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_ADVSOUND_GetInfo(Type);

#endif
}

void MApi_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[4] = {0};
    u32param[0] = ((MS_U32)totsonmode);
    u32param[1] = ((MS_U32)totvolmode);
    u32param[2] = ((MS_U32)totsurmode);
    u32param[3] = ((MS_U32)enable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] DBXTV Set Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DBXTV_SetMode, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] DBXTV Set Mode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_DBXTV_SetMode(totsonmode, totvolmode, totsurmode, enable);

#endif
}

void MApi_AUDIO_SeInit(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Se Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SeInit, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Se Init fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SeInit();

#endif
}

void MApi_AUDIO_SetAdvSndSys(MS_U8 u8AdvSurrMod)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)u8AdvSurrMod);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Adv Snd Sys fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAdvSndSys, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Adv Snd Sys fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetAdvSndSys(u8AdvSurrMod);

#endif
}

void MApi_AUDIO_ADVSND_Set_Sound_Param(AdvSound_SET_Type Type, MS_U32 *pParam) // New api for cover below functions
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = Type;
    Status.pParam = pParam;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set VDS fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set VDS fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_ADVSND_Set_Sound_Param(Type, pParam);

#endif

}

void MApi_AUDIO_SetVDS(MS_U8 u8enVDS)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0,0};
    u32param[0] = u8enVDS;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_VDS_Enable;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set VDS fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set VDS fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetVDS(u8enVDS);

#endif
}

void MApi_AUDIO_SetVSPK(MS_U8 u8enVSPK)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0,0};
    u32param[0] = u8enVSPK;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_VSPK_Enable;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set VSPK fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set VSPK fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetVSPK(u8enVSPK);

#endif
}

void MApi_AUDIO_SetSRS(MS_U8 u8enSRS)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0,0};
    u32param[0] = u8enSRS;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_SRS_Enable;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set SRS fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set SRS fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSRS(u8enSRS);

#endif
}

void MApi_AUDIO_SetBBE(MS_U8 u8enBBE , MS_U8 u8BBEMode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[0] = u8enBBE;
    u32param[1] = u8BBEMode;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_BBE_SetBbeMode;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set BEE fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set BEE fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetBBE(u8enBBE, u8BBEMode);

#endif
}

void MApi_AUDIO_VSPK_WMod(MS_U8 u8VSPK_WMode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[0] = u8VSPK_WMode;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_VSPK_SetWMod;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] VSPK WMod fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] VSPK WMod fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_VSPK_WMod(u8VSPK_WMode);

#endif
}

void MApi_AUDIO_VSPK_SMod(MS_U8 u8VSPK_SMode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[0] = u8VSPK_SMode;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_VSPK_SetSMod;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] VSPK SMod fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] VSPK SMod fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_VSPK_SMod(u8VSPK_SMode);

#endif
}

void MApi_AUDIO_SRS_DC(MS_U8 u8SRSenDC)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[0] = u8SRSenDC;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_SRS_DC;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SRS DC fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SRS DC fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SRS_DC(u8SRSenDC);

#endif
}

void MApi_AUDIO_SRS_TruBass(MS_U8 u8SRSenTruBass)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[0] = u8SRSenTruBass;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_SRS_TruBass;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SRS TruBass fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SRS TruBass fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SRS_TruBass(u8SRSenTruBass);

#endif
}

void  MApi_AUDIO_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[0] = u8mode;
    u32param[1] = u8value;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_SRS_SetTsxtPara;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SRS Set Tsxt Para fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SRS Set Tsxt Para fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SRS_SetTsxtPara(u8mode, u8value);

#endif
}

void MApi_AUDIO_SetSRSTSHD(MS_U8 u8enTSHD)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[0] = u8enTSHD;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_TSHD_Enable;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set SRS TSHD fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set SRS TSHD fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSRSTSHD(u8enTSHD);

#endif
}

void  MApi_AUDIO_TSHD_Definition(MS_U8 u8TSHDenDefinition)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[0] = u8TSHDenDefinition;

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_TSHD_Definition;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] TSHD Definition fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] TSHD Definition fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_TSHD_Definition(u8TSHDenDefinition);

#endif
}

void  MApi_AUDIO_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0,0};
    u32param[0] = ((MS_U32)u8mode);
    u32param[1] = ((MS_U32)u8value);

    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = AdvSound_SET_Type_SRS_SetTshdPara;
    Status.pParam = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SRS Set Tshd Para fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SRS Set Tshd Para fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SRS_SetTshdPara(u8mode, u8value);

#endif
}

void MApi_AUDIO_COPY_Parameter(ADVSND_PARAM type, MS_U32 *Parameter_ptr,MS_U32 size)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    AUDIO_ST_COPY_PARAMETER Parameter;

    Parameter.type = type;
    Parameter.Parameter_ptr = Parameter_ptr;
    Parameter.size = size;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] COPY Parameter fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Copy_Parameter, (void*)&Parameter) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] COPY Parameter fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[AUDIO][%s] [%d] [Parameter_ptr 0x%x] \n", __FUNCTION__, __LINE__, (int)Parameter_ptr[0]);

    return;

#else

    return _MApi_AUDIO_COPY_Parameter(type, Parameter_ptr, size);

#endif
}

//=============================================================
// AUDIO_KTV RELATIONAL API FUNCTION
//=============================================================
MS_BOOL MApi_AUDIO_SetKTVInfo(AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_KTV_OP Status;
    Status.eType = KTV_SET_INFO;
    Status.modelType = modelType;
    Status.paramType = AUD_KTV_ParamType1;
    Status.param1 = param1;
    Status.param2 = param2;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set KTV Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_KTV_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set KTV Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bRet);
    return Status.bRet;

#else

    return _MApi_AUDIO_SetKTVInfo(modelType, paramType, param1, param2);

#endif
}

MS_U32 MApi_AUDIO_GetKTVInfo(AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_KTV_OP Status;
    Status.eType = KTV_GET_INFO;
    Status.modelType = modelType;
    Status.infoType = infoType;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set KTV Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_KTV_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set KTV Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.u32Ret);
    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetKTVInfo(modelType, infoType);

#endif
}

void MApi_AUDIO_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_KTV_OP Status;
    Status.eType = KTV_SET_MIX_VOL;
    Status.eSourceType = eSourceType;
    Status.VolType = VolType;
    Status.u8Vol1 = u8Vol1;
    Status.u8Vol2 = u8Vol2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mix Mode Volume fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_KTV_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mix Mode Volume fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetMixModeVolume(eSourceType, VolType, u8Vol1, u8Vol2);

#endif
}

void MApi_AUDIO_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_KTV_OP Status;
    Status.eType = KTV_SET_MIX_MUTE;
    Status.eSourceType = eSourceType;
    Status.VolType = VolType;
    Status.EnMute = EnMute;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mix Mode Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_KTV_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mix Mode Mute fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetMixModeMute(eSourceType, VolType, EnMute);

#endif
}

//Tempory debug function for crash
//used to enable / Disable Utopia debug
void MApi_AUDIO_PlayMenuSound(void)
{
    if(g_api_audio_V2_Dbg_Msg_bEnable)
    {
        g_api_audio_V2_Dbg_Msg_bEnable    = 0;
        g_api_audio_V2_Dbg_Return_bEnable = 0;
    }
    else
    {
        g_api_audio_V2_Dbg_Msg_bEnable    = 1;
        g_api_audio_V2_Dbg_Return_bEnable = 1;
    }
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Play Menu Sound fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PlayMenuSound, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Play Menu Sound fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_PlayMenuSound();

#endif
}

//=============================================================
// AUDIO_MM RELATIONAL API FUNCTION
//=============================================================
MS_BOOL MApi_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[4] = {0};
    u32param[1] = (MS_U32)infoType;
    u32param[2] = (MS_U32)param1;
    u32param[3] = (MS_U32)param2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Comm Audio Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetCommAudioInfo, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Comm Audio Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return input0 %d input1 %d input2 %d val %d\n", __FUNCTION__,(int)infoType, (int)param1, (int)param2, (int)u32param[0]);
    return (MS_BOOL)u32param[0];

#else

    return _MApi_AUDIO_SetCommAudioInfo(infoType, param1, param2);

#endif
}

MS_BOOL MApi_ADEC_SetDecodeParam( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32* Param )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);
#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = DecId;
    Status.paramType = paramType;
    Status.Param = Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Decode Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Decode Param fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_ADEC_SetDecodeParam(DecId, paramType, Param);

#endif

}

MS_BOOL MApi_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0,0,0,0,0,0};
    u32param[0] = (MS_U32)infoType;
    u32param[1] = param1;
    u32param[2] = param2;

    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = AU_DEC_ID1;
    Status.paramType = Audio_ParamType_setMPEGDecParam;
    Status.Param = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mpeg Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mpeg Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_SetMpegInfo(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetAC3Info(Audio_AC3_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0,0,0,0,0,0};
    u32param[0] = (MS_U32)infoType;
    u32param[1] = param1;
    u32param[2] = param2;

    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = AU_DEC_ID1;
    Status.paramType = Audio_ParamType_setAC3DecParam;
    Status.Param = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AC3 Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AC3 Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_SetAC3Info(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0,0,0,0,0,0};
    u32param[0] = (MS_U32)infoType;
    u32param[1] = param1;
    u32param[2] = param2;

    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = AU_DEC_ID1;
    Status.paramType = Audio_ParamType_setAC3PDecParam;
    Status.Param = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AC3P Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AC3P Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_SetAC3PInfo(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0,0,0,0,0,0};
    u32param[0] = (MS_U32)infoType;
    u32param[1] = param1;
    u32param[2] = param2;

    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = AU_DEC_ID1;
    Status.paramType = Audio_ParamType_setAACDecParam;
    Status.Param = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AAC Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set AAC Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_SetAACInfo(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetWmaInfo(Audio_WMA_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0,0,0,0,0,0};
    u32param[0] = (MS_U32)infoType;
    u32param[1] = param1;
    u32param[2] = param2;

    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = AU_DEC_ID1;
    Status.paramType = Audio_ParamType_setWMADecParam;
    Status.Param = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set WMA Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set WMA Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_SetWmaInfo(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0,0,0,0,0,0};
    u32param[0] = infoType;
    u32param[1] = ctrl_type;
    u32param[2] = 0;

    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = AU_DEC_ID1;
    Status.paramType = Audio_ParamType_setDTSDecParam;
    Status.Param = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set DTS Common Ctrl fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set DTS Common Ctrl fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_SetDTSCommonCtrl(infoType, ctrl_type);

#endif
}

long long MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U64 u64param[2] = {0};
    u64param[1] = ((MS_U32)infoType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Comm Audio Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetCommAudioInfo, (void*)u64param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Comm Audio Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u64param[0]);
    return (long long)u64param[0];

#else

    return _MApi_AUDIO_GetCommAudioInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)infoType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Mpeg Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetMpegInfo, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Mpeg Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_GetMpegInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetAC3Info(Audio_AC3_infoType infoType )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)infoType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get AC3 Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetAC3Info, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get AC3 Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_GetAC3Info(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)infoType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get AC3P Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetAC3PInfo, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get AC3P Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_GetAC3PInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetAACInfo( Audio_AAC_infoType infoType )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)infoType) & 0xFF;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get AAC Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetAACInfo, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get AAC Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_GetAACInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)infoType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Wma Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetWmaInfo, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Wma Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_GetWmaInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)infoType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get DTS Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDTSInfo, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get DTS Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_GetDTSInfo(infoType);

#endif
}

MS_U8 MApi_AUDIO_XPCM_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0,0,0,0,0,0};
    u32param[0] = (MS_U32)audioType;
    u32param[1] = (MS_U32)channels;
    u32param[2] = (MS_U32)sampleRate;
    u32param[3] = (MS_U32)bitsPerSample;
    u32param[4] = (MS_U32)blockSize;
    u32param[5] = (MS_U32)samplePerBlock;

    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = AU_DEC_ID1;
    Status.paramType = Audio_ParamType_setXPCMDecParam;
    Status.Param = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] XPCM Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] XPCM Param fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_XPCM_Param(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);

#endif
}

#if 0
void MApi_AUDIO_XPCM2_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0};
    u32param[0] = ((MS_U32)audioType);
    u32param[1] = ((MS_U32)channels);
    u32param[2] = ((MS_U32)sampleRate);
    u32param[3] = ((MS_U32)bitsPerSample);
    u32param[4] = ((MS_U32)blockSize);
    u32param[5] = ((MS_U32)samplePerBlock);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] XPCM2 Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_XPCM2_Param, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] XPCM2 Param fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_XPCM2_Param(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);

#endif
}
#endif

MS_U16 MApi_AUDIO_XPCM2_CheckIntStatus(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] XPCM2 Check Int Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_XPCM2_CheckIntStatus, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] XPCM2 Check Int Status fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_U16)u32param[1];

#else

    return _MApi_AUDIO_XPCM2_CheckIntStatus();

#endif
}

MS_U8 MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate, MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits, MS_U16* FrameSize)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_RA8_Param Status;
    Status.u8Status = FALSE;
    Status.mNumCodecs = mNumCodecs;
    Status.mSamples = mSamples;
    Status.mSampleRate= mSampleRate;
    Status.pChannels = Channels;
    Status.pRegions = Regions;
    Status.pcplStart = cplStart;
    Status.pcplQbits = cplQbits;
    Status.pFrameSize = FrameSize;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] RA8 Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_RA8_Param, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] RA8 Param fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.u8Status);
    return Status.u8Status;

#else

    return _MApi_AUDIO_RA8_Param(mNumCodecs, mSamples, mSampleRate, Channels, Regions, cplStart, cplQbits,  FrameSize);

#endif
}

void MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param =((MS_U32)enDecSystem);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Init, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Init fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_Init(enDecSystem);

#endif
}

void MApi_AUDIO_StartDecode(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Start Decode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_StartDecode, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Start Decode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_StartDecode();

#endif
}

void MApi_AUDIO_StartBrowse(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Start Broswe fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_StartBroswe, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Start Broswe fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_StartBrowse();

#endif
}

void MApi_AUDIO_StopDecode(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Stop Decode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_StopDecode, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Stop Decode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_StopDecode();

#endif
}

void MApi_AUDIO_PauseDecode(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Pause Decode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PauseDecode, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Pause Decode fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_PauseDecode();

#endif
}

MS_U8 MApi_AUDIO_CheckPlayDone(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check Play Done fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_CheckPlayDone, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check Play Done fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_U8)u32param[1];

#else

    return _MApi_AUDIO_CheckPlayDone();

#endif
}

MS_U16 MApi_AUDIO_GetResidualBufferSize(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Residual Buffer Size fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetResidualBufferSize, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Residual Buffer Size fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_U16)u32param[1];

#else

    return _MApi_AUDIO_GetResidualBufferSize();

#endif
}

MS_U16 MApi_AUDIO_GetPCMBufferSize(MS_U16 u16BitRate)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[1] = ((MS_U32)u16BitRate);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get PCM Buffer Size fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetPCMBufferSize, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get PCM Buffer Size fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U16)u32param[0];

#else

    return _MApi_AUDIO_GetPCMBufferSize(u16BitRate);

#endif
}

MS_U16 MApi_AUDIO_GetPCMBufferSize2(MS_U16 u16BitRate,MS_U16 u16SampleRate)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[3] = {0, 0};
    u32param[1] = ((MS_U32)u16BitRate);
    u32param[2] = ((MS_U32)u16SampleRate);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get PCM Buffer Size2 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetPCMBufferSize2, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get PCM Buffer Size2 fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U16)u32param[0];

#else

    return _MApi_AUDIO_GetPCMBufferSize2(u16BitRate, u16SampleRate);

#endif
}

MS_U32 MApi_AUDIO_GetCurrentFrameNumber(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Current Frame Number fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetCurrentFrameNumber, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Current Frame Number fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_U32)u32param[1];

#else

    return _MApi_AUDIO_GetCurrentFrameNumber();

#endif
}

MS_U16 MApi_AUDIO_GetSampleRate(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Sample Rate fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetSampleRates, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Sample Rate fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_U16)u32param[1];

#else

    return _MApi_AUDIO_GetSampleRate();

#endif
}

MS_U32 MApi_AUDIO_GetBitRate(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Bit Rate fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetBitRates, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Bit Rate fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_U32)u32param[1];

#else

    return _MApi_AUDIO_GetBitRate();

#endif
}

Audio_MP3_LAYER MApi_AUDIO_GetLayer(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Layer fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetLayer, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Layer fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (Audio_MP3_LAYER)u32param[1];

#else

    return _MApi_AUDIO_GetLayer();

#endif
}

MS_U8 MApi_AUDIO_CheckInputRequest(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_CHECK_INPUT_REQ Status;
    Status.u8Status = FALSE;
    Status.pU32WrtAddr = pU32WrtAddr;
    Status.pU32WrtBytes = pU32WrtBytes;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check Input Request fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_CheckInputRequest, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check Input Request fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[AUDIO][%s] [%d] Return val %d [Wrt Add 0x%x] [Wrt byte 0x%x]\033[0m \n", __FUNCTION__, __LINE__, (int)Status.u8Status, (int)*pU32WrtAddr, (int)*pU32WrtBytes);
    return Status.u8Status;

#else

    return _MApi_AUDIO_CheckInputRequest(pU32WrtAddr, pU32WrtBytes);

#endif
}

void MApi_AUDIO_SetInput(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Input fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetInput, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Input fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetInput();

#endif
}

void MApi_AUDIO_SetSampleRateIndex(MS_U16 u16Index)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)u16Index);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Sample Rate Index fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSampleRateIndex, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Sample Rate Index fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSampleRateIndex(u16Index);

#endif
}

void MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U16 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0,0,0,0,0,0};
    u32param[0] = (MS_U32)audioType;
    u32param[1] = (MS_U32)channels;
    u32param[2] = (MS_U32)sampleRate;
    u32param[3] = (MS_U32)bitsPerSample;
    u32param[4] = (MS_U32)blockSize;
    u32param[5] = (MS_U32)samplePerBlock;

    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = AU_DEC_ID1;
    Status.paramType = Audio_ParamType_setXPCMDecParam;
    Status.Param = (MS_U32*)u32param;


    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set XPCM Param fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set XPCM Param fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetXPCMParam(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);

#endif
}

void MApi_AUDIO_FileEndNotification(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] File End Notification fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FileEndNotification, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] File End Notification fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_FileEndNotification();

#endif
}

void MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)u32DataLeft);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] File End Data Handle fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FileEndDataHandle, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] File End Data Handle fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_FileEndDataHandle(u32DataLeft);

#endif
}

MS_U32 MApi_AUDIO_GetPlayTick(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Play Tick fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetPlayTick, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Play Tick fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return u32param[1];

#else

    return _MApi_AUDIO_GetPlayTick();

#endif
}

MS_U16 MApi_AUDIO_GetEsMEMCnt(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Es MEM Cnt fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetEsMEMCnt, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Es MEM Cnt fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_U16)u32param[1];

#else

    return _MApi_AUDIO_GetEsMEMCnt();

#endif
}

void MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[0] = ((MS_U32)parm_type);
    u32param[1] = value;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set ASF Parm fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetASFParm, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set ASF Parm fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetASFParm(parm_type, value);

#endif
}

void MApi_AUDIO_MM_SetInput (En_DVB_AudioDecoder AUDDecoderNo, MS_U8 u8IntTag)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[0] = ((MS_U32)AUDDecoderNo);
    u32param[1] = ((MS_U32)u8IntTag);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM Set Input fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM_SetInput, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM Set Input fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_MM_SetInput(AUDDecoderNo, u8IntTag);

#endif
}

MS_U32 MApi_AUDIO_MM_CheckPlayDone (En_DVB_AudioDecoder AUDDecoderNo)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)AUDDecoderNo);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM Check Play Done fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM_CheckPlayDone, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM Check Play Done fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return u32param[0];

#else

    return _MApi_AUDIO_MM_CheckPlayDone(AUDDecoderNo);

#endif
}

MS_U8 MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_MM_CHECK_INPUT_REQ Status;
    Status.u8Status = FALSE;
    Status.AUDDecoderNo = AUDDecoderNo;
    Status.pU32WrtAddr = pU32WrtAddr;
    Status.pU32WrtBytes = pU32WrtBytes;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM Check Input Request fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM_CheckInputRequest, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM Check Input Request fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[AUDIO][%s] [%d] Return val %d [Wrt Add 0x%x] [Wrt byte 0x%x]\n", __FUNCTION__, __LINE__, (int)Status.u8Status, (int)*pU32WrtAddr, (int)*pU32WrtBytes);
    return Status.u8Status;

#else

    return _MApi_AUDIO_MM_CheckInputRequest(AUDDecoderNo, pU32WrtAddr, pU32WrtBytes);

#endif
}

void MApi_AUDIO_DmaReader_Init(PCM_OUTPUT_FORMAT* pSettings)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Dma Reader Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DmaReader_Init, (void*)pSettings) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Dma Reader Init fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_DmaReader_Init(pSettings);

#endif
}

void MApi_AUDIO_DmaReader_AllInput_Init(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Dma Reader All Input Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DmaReader_AllInput_Init, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Dma Reader All Input Init fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_DmaReader_AllInput_Init();

#endif
}

MS_BOOL MApi_AUDIO_DmaReader_WritePCM(MS_U8 u8Idx, void* buffer, MS_U32 bytes)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_DMAREADER_WRITEPCM Status;
    Status.bStatus = FALSE;
    Status.pBuffer = buffer;
    Status.bytes = bytes;
    Status.u8Idx = u8Idx;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Dma Reader WritePCM fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DmaReader_WritePCM, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Dma Reader WritePCM fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_DmaReader_WritePCM(u8Idx, buffer, bytes);

#endif
}

void MApi_AUDIO_SwDmaReader_Init(PCM_OUTPUT_FORMAT* pSettings)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SW DMA Reader Init fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SwDmaReader_Init, (void*)pSettings) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SW DMA Reader Init faill\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SwDmaReader_Init(pSettings);

#endif
}

MS_BOOL MApi_AUDIO_SwDmaReader_WritePCM(MS_U8 u8Idx, void* buffer, MS_U32 bytes)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    AUDIO_ST_SWDMAREADER_WRITEPCM Status;

//    AUDIO_ST_PCMMIXER_WRITE sPcmMixerWrite;

    if (buffer == NULL)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! pBuf should not be NULL!\n", __LINE__);
    }

    if (bytes == 0)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! u32Size should not be zero!\n", __LINE__);
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SW DMA Reader Write PCM fail\n", __LINE__);
        //return;
    }

    memset(&Status, 0x0, sizeof(AUDIO_ST_SWDMAREADER_WRITEPCM));
    Status.u8Idx = u8Idx;
    Status.buffer = buffer;
    Status.bytes = bytes;
    Status.bRet = FALSE;


    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SwDmaReader_WritePCM, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] SW DMA Reader Write PCM faill\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bRet);
    return Status.bRet;

#else

    return _MApi_AUDIO_SwDmaReader_WritePCM(u8Idx, buffer, bytes);

#endif
}

void MApi_AUDIO_DmaWriter_Init(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Dma Writer Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DmaWrite_Init, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Dma Writer Init fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return ;

#else

    return _MApi_AUDIO_DmaWriter_Init();

#endif
}

void MApi_AUDIO_USBPCM_Enable(MS_BOOL bEnable)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)bEnable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] USBPCM Enable fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_USBPCM_Enable, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] USBPCM Enable fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return ;

#else

    return _MApi_AUDIO_USBPCM_Enable(bEnable);

#endif
}

void MApi_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = ((MS_U32)bEnable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] USBPCM SetFlag fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_USBPCM_SetFlag, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] USBPCM SetFlag fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return ;

#else

    return _MApi_AUDIO_USBPCM_SetFlag(bEnable);

#endif
}

MS_BOOL MApi_AUDIO_USBPCM_GetFlag(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] USBPCM GetFlag fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_USBPCM_GetFlag, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] USBPCM GetFlag fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_BOOL)u32param[1];

#else

    return _MApi_AUDIO_USBPCM_GetFlag();

#endif
}

void MApi_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] USBPCM Get Mem Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_USBPCM_GetMemInfo, (void*)uploadInfo) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] USBPCM Get Mem Info fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return u32Addr 0x%x u32Size 0x%x u32Idx %d\n", __FUNCTION__, (int)uploadInfo->u32Addr, (int)uploadInfo->u32Size, (int)uploadInfo->u32Idx);
    return;

#else

    return _MApi_AUDIO_USBPCM_GetMemInfo(uploadInfo);

#endif
}

MS_BOOL MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMCAPTURE_INIT Status;
    Status.bStatus = FALSE;
    Status.eID = eID;
    Status.eSource = eSource;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Capture Init fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCMCapture_Init, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Capture Init fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_PCMCapture_Init(eID, eSource);

#endif
}

MS_BOOL MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMCAPTURE_INIT Status;
    Status.bStatus = FALSE;
    Status.eID = eID;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Capture Start fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCMCapture_Start, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Capture Start fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_PCMCapture_Start(eID);

#endif
}

MS_BOOL MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMCAPTURE_INIT Status;
    Status.bStatus = FALSE;
    Status.eID = eID;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Capture Stop fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCMCapture_Stop, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Capture Stop fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_PCMCapture_Stop(eID);

#endif
}

MS_BOOL MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMCAPTURE_READ Status;
    Status.bStatus = FALSE;
    Status.eID = eID;
    Status.pBuffer = buffer;
    Status.bytes = bytes;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Capture Read fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCMCapture_Read, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Capture Read fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_PCMCapture_Read(eID, buffer, bytes);

#endif
}

MS_BOOL MApi_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[4] = {0};
    u32param[1] = ((MS_U32)configType);
    u32param[2] = ((MS_U32)param1);
    u32param[3] = ((MS_U32)param2);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] VoIP Config fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_VoIP_Config, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] VoIP Config fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_BOOL)u32param[0];

#else

    return _MApi_AUDIO_VoIP_Config(configType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_ALSA_Check(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ALSA Check fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ALSA_Check, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ALSA Check fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_BOOL)u32param[1];

#else

    return _MApi_AUDIO_ALSA_Check();

#endif
}

MS_BOOL MApi_AUDIO_ALSA_Enable(MS_BOOL bEnable)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};
    u32param[1] = ((MS_U32)bEnable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ALSA Enable fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ALSA_Enable, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ALSA Enable fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_BOOL)u32param[0];

#else

    return _MApi_AUDIO_ALSA_Enable(bEnable);

#endif
}

MS_BOOL MApi_AUDIO_UNI_CheckDecodeDone(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_UNI_CHECK_DECODEDONE Status;
    Status.bStatus = FALSE;
    Status.pU32WrtAddr = pU32WrtAddr;
    Status.pU32WrtBytes = pU32WrtBytes;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] UNI Check Decode Done fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_UNI_CheckDecodeDone, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] UNI Check Decode Done fail\n", __LINE__);
        //return;
    }

    AUDIO_DBGRT("[AUDIO][%s] [%d] Return val %d [Wrt Addr 0x%x] [Wrt Byte 0x%x] \n", __FUNCTION__, __LINE__, (int)Status.bStatus, (int)*pU32WrtAddr, (int)*pU32WrtBytes);
    return Status.bStatus;

#else

    return _MApi_AUDIO_UNI_CheckDecodeDone(pU32WrtAddr, pU32WrtBytes);

#endif
}

void MApi_AUDIO_UNI_SetOutput (MS_U32 PCMOutCnt)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = PCMOutCnt;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] UNI Set Output fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_UNI_SetOutput, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] UNI Set Output fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_UNI_SetOutput(PCMOutCnt);

#endif
}

void MApi_AUDIO_UNI_Set_PCMInputWriteAddr (MS_U32 PCMIn_Wptr)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = PCMIn_Wptr;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] UNI Set PCM Input Write Addr fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] UNI Set PCM Input Write Addr fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_UNI_Set_PCMInputWriteAddr(PCMIn_Wptr);

#endif
}

MS_U32 MApi_AUDIO_UNI_Get_OutPCMLevel (void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] UNI Get Out PCM Level fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_UNI_Get_OutPCMLevel, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] UNI Get Out PCM Level fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_BOOL)u32param[1];

#else

    return _MApi_AUDIO_UNI_Get_OutPCMLevel();

#endif
}

void MApi_AUDIO_RingTask(audioRingTask* auRingTask)        // this function is used for auto ring address
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Ring Task fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_RingTask, (void*)auRingTask) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Ring Task fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_RingTask(auRingTask);

#endif
}

void MApi_AUDIO_Ring_DataTransfer(audioRingTask* auRingTask, audioRingTask* auRingTask_1)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_RING_DATA_TRANS st_auRingTask;
    st_auRingTask.pauRingTask = auRingTask;
    st_auRingTask.pauRingTask_1 = auRingTask_1;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Ring Task Data Transfer fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Ring_DataTransfer, (void*)&st_auRingTask) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Ring Task Data Transfer fail\n", __LINE__);
        return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_Ring_DataTransfer(auRingTask, auRingTask_1);

#endif
}

MS_BOOL MApi_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID dec_id )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[6] = {0,0,0,0,0,0};
    u32param[0] = (MS_U32)dec_id;

    AUDIO_ST_SETDECODEPARAM Status;
    Status.bStatus = FALSE;
    Status.DecID = AU_DEC_ID1;
    Status.paramType = Audio_ParamType_MM2_initAesInfo;
    Status.Param = (MS_U32*)u32param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM2 Init Aes Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM2 Init Aes Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return (MS_BOOL)Status.bStatus;

#else

    return _MApi_AUDIO_MM2_initAesInfo(dec_id);

#endif
}

MS_BOOL MApi_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_MM2_CHECKAESINFO Status;
    Status.paes_info = aes_info;
    Status.dec_id = dec_id;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM2 Check Aes Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM2_CheckAesInfo, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM2 Check Aes Info fail\n", __LINE__);
        //return;
    }

    AUDIO_DBGRT("[%s] Ptr[w:%lx,r:%lx] LvL:%lx, FreeSpace:%lx, tmpFree:%lx \n", __FUNCTION__,
        aes_info->aes_write_addr,
        aes_info->aes_read_addr,
        aes_info->aes_level,
        aes_info->aes_BufSize - aes_info->aes_level,
        aes_info->aes_freeSpace);

    return TRUE;

#else

    return _MApi_AUDIO_MM2_checkAesInfo(dec_id, aes_info);

#endif
}

MS_BOOL MApi_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_MM2_INPUTAES_FINISH Status;
    Status.bStatus = FALSE;
    Status.dec_id = dec_id;
    Status.es_size = es_size;
    Status.ptsExist = ptsExist;
    Status.pts = pts;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM2 Input Aes Finished fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM2_InputAesFinished, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] MM2 Input Aes Finished fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_MM2_inputAesFinished(dec_id, es_size, ptsExist, pts);

#endif
}

MS_BOOL MApi_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32 Param )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[4] = {0};
    u32param[1] = ((MS_U32)DecId);
    u32param[2] = ((MS_U32)paramType);
    u32param[3] = ((MS_U32)Param);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Audio Param2 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAudioParam2, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Audio Param2 fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_BOOL)u32param[0];

#else

    return _MApi_AUDIO_SetAudioParam2(DecId, paramType, Param );

#endif
}

MS_BOOL MApi_AUDIO_GetAudioInfo2(AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_GETAUDIOINFO2 Status;
    Status.bStatus = FALSE;
    Status.DecId = DecId;
    Status.infoType = infoType;
    Status.pInfo = pInfo;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Audio Info2 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetAudioInfo2, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Audio Info2 fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.bStatus);
    return Status.bStatus;

#else

    return _MApi_AUDIO_GetAudioInfo2(DecId, infoType, pInfo);

#endif
}

MS_U32 MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[3] = {0};
    u32param[1] = ((MS_U32)DecId);
    u32param[2] = ((MS_U32)DDRInfo);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get DDR Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDDRInfo, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get DDR Info fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[0]);
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_GetDDRInfo(DecId, DDRInfo);

#endif
}


// LG add function
#if 0
void MApi_AUDIO_SetBalance_L( MS_U8 u8Balance_L )
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SETBALABCE Status;
    Status.bLeft = TRUE;
    Status.u32BalanceValue = u8Balance_L;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Balance L fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetBalance, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Balance L fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetBalance_L(u8Balance_L);

#endif
}

void MApi_AUDIO_SetBalance_R( MS_U8 u8Balance_R)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SETBALABCE Status;
    Status.bLeft = FALSE;
    Status.u32BalanceValue = u8Balance_R;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Balance R fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetBalance, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Balance R fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetBalance_R(u8Balance_R);

#endif
}

#endif
MS_U16 MApi_AUDIO_ReadReg(MS_U32 u32RegAddr)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_RWReg Status;
    Status.bRead = TRUE;
    Status.u32RegAddr = u32RegAddr;
    Status.u16Val = 0;
    Status.u16Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Read Reg fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_RWReg, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Read Reg fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, Status.u16Ret);
    return Status.u16Ret;

#else

    return _MApi_AUDIO_ReadReg(u32RegAddr);

#endif
}

void MApi_AUDIO_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_RWReg Status;
    Status.bRead = FALSE;
    Status.u32RegAddr = u32RegAddr;
    Status.u16Val = u16Val;
    Status.u16Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Write Reg fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_RWReg, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Write Reg fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_WriteReg(u32RegAddr, u16Val);

#endif
}

#if 0
void MApi_AUDIO_Set_Cut_Boost(MS_U8 scale)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = (MS_U32)scale;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Cut Boost fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetCutBooost, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Cut Boost fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_Set_Cut_Boost(scale);

#endif
}

void MApi_AUDIO_ES_Stop_Threshold(MS_U32 threshold)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = (MS_U32)threshold;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ES Stop Threshold fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ES_STOP_THRESHOLD, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ES Stop Threshold fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_ES_Stop_Threshold(threshold);

#endif
}

void MApi_AUDIO_SetSPDIFHWMute(MS_U8 mute)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = (MS_U32)mute;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set SPDIF HW Mute fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSPDIF_HW_Mute, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set SPDIF HW Mute fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetSPDIFHWMute(mute);

#endif
}

void MApi_AUDIO_Set_ARC_HWMute(MS_U8 mute)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = (MS_U32)mute;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set ARC HW Mute fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetARC_HW_Mute, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set ARC HW Mute fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_Set_ARC_HWMute(mute);

#endif
}
#endif

void MApi_AUDIO_ARC_HWEN(MS_BOOL arc_en)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param = (MS_U32)arc_en;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set ARC HW EN fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetARC_HWEN, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set ARC HW EN fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_ARC_HWEN(arc_en);

#endif
}

void MApi_AUDIO_ARC_HWEN_BY_PORT(AUDIO_HDMI_INDEX_T hdmiIndex, MS_BOOL bOnOff)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_ARC_HWEN_BY_PORT arc_status;

    arc_status.hdmiIndex = hdmiIndex;
    arc_status.bOnOff = bOnOff;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set ARC HW EN BY PORT fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetARC_HWEN_By_Port, (void*)&arc_status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set ARC HW EN BY PORT fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_ARC_HWEN_BY_PORT(hdmiIndex, bOnOff);

#endif
}

void MApi_AUDIO_LoadAudioClip(MS_U32 bufSize, void *clipBufPtr, MS_U32 dsp_sel)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_LOAD_AUDIO_CLIP Status;
    Status.bufSize = bufSize;
    Status.clipBufPtr = clipBufPtr;
    Status.dsp_sel = dsp_sel;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Load Audio Clip fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_LoadAudio_Clip, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Load Audio Clip fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_LoadAudioClip(bufSize, clipBufPtr, dsp_sel);

#endif
}

void MApi_AUDIO_PlayAudioClip(MS_U32 repeatTimes)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_CLIP Status;
    Status.eType = CLIP_PLAY_AUDIO;
    Status.u32Input = repeatTimes;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Play Audio Clip fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Clip_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Play Audio Clip fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_PlayAudioClip(repeatTimes);

#endif
}

MS_U32 MApi_AUDIO_GetClip_Position(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_CLIP Status;
    Status.eType = CLIP_GET_POSITION;
    Status.u32Input = 0;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Clip Position fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Clip_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Clip Position fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.u32Ret);
    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetClip_Position();

#endif
}

void MApi_AUDIO_SeekAudioClip(MS_U32 clipData_offset)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_CLIP Status;
    Status.eType = CLIP_SEEK_AUDIO;
    Status.u32Input = clipData_offset;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Seek Audio Clip fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Clip_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Seek Audio Clip fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return ;

#else

    return _MApi_AUDIO_SeekAudioClip(clipData_offset);

#endif
}

void MApi_AUDIO_StopAudioClip(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_CLIP Status;
    Status.eType = CLIP_STOP_AUDIO;
    Status.u32Input = 0;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Stop Audio Clip fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Clip_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Stop Audio Clip fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return ;

#else

    return _MApi_AUDIO_StopAudioClip();

#endif
}

MS_U32 MApi_AUDIO_ClipPlayDone(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_CLIP Status;
    Status.eType = CLIP_PLAY_DONE;
    Status.u32Input = 0;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Clip Play Done fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Clip_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Clip Play Done fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)Status.u32Ret);
    return Status.u32Ret;

#else

    return _MApi_AUDIO_ClipPlayDone();

#endif
}

void MApi_AUDIO_ClipPlayDone_Rest(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_CLIP Status;
    Status.eType = CLIP_PLAY_DONE_RESET;
    Status.u32Input = 0;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Clip Play Done Reset fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Clip_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Clip Play Done Reset fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return ;

#else

    return _MApi_AUDIO_ClipPlayDone_Rest();

#endif
}

void MApi_AUDIO_Clip_processor(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_CLIP Status;
    Status.eType = CLIP_PROCESSOR;
    Status.u32Input = 0;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Clip Processor fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Clip_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Clip Processor fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return ;

#else

    return _MApi_AUDIO_Clip_processor();

#endif
}

void MApi_AUDIO_SIF_GetCurAnalogMode(MS_U8* pAlgMode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_ANALOGMODE_OP Status;
    Status.eType = SIF_GET_CUR_ANALOG_MODE;
    Status.u16SetVal = 0;
    Status.u8GetVal = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Cur Analog Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_AnalogMode_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Cur Analog Mode fail\n", __LINE__);
        //return;
    }
    *pAlgMode = Status.u8GetVal;
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return ;

#else

    return _MApi_AUDIO_SIF_GetCurAnalogMode(pAlgMode);

#endif
}

void MApi_AUDIO_SIF_SetUserAnalogMode(MS_U16 AlgMode)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_ANALOGMODE_OP Status;
    Status.eType = SIF_SET_USER_ANALOG_MODE;
    Status.u16SetVal = AlgMode;
    Status.u8GetVal = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set User Analog Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_AnalogMode_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set User Analog Mode fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return ;

#else

    return _MApi_AUDIO_SIF_SetUserAnalogMode(AlgMode);

#endif
}

MS_BOOL MApi_AUDIO_SIF_SetModeSetup(MS_U8 SystemType)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_SETMODE_SETUP_OP Status;
    Status.eType = SIF_SETMODE_SETUP;
    Status.u8Input = SystemType;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mode Setup fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetMode_Setup_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Mode Setup fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d \n", __FUNCTION__, (int)Status.bRet);
    return Status.bRet;

#else

    return _MApi_AUDIO_SIF_SetModeSetup(SystemType);

#endif
}

void MApi_AUDIO_SIF_SetHDEVMode(MS_U8 bOnOff)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_SETMODE_SETUP_OP Status;
    Status.eType = SIF_SETHDEV_MODE;
    Status.u8Input = bOnOff;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set HDEV Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetMode_Setup_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set HDEV Mode fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0 \n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_SetHDEVMode(bOnOff);

#endif
}

MS_BOOL MApi_AUDIO_SIF_SetBandSetup(MS_U8 SifBand)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_SETMODE_SETUP_OP Status;
    Status.eType = SIF_SETBAND_SETUP;
    Status.u8Input = SifBand;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Band Setup fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetMode_Setup_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Band Setup fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d \n", __FUNCTION__, (int)Status.bRet);
    return Status.bRet;

#else

    return _MApi_AUDIO_SIF_SetBandSetup(SifBand);

#endif
}

MS_BOOL MApi_AUDIO_SIF_GetBandDetect(MS_U8 soundSystem, MS_U32 *pbandStrength)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_GETBAND_OP Status;
    Status.eType = SIF_GET_BAND_DETECT;
    Status.SoundSystem = soundSystem;
    Status.pbandStrength = pbandStrength;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Band Detect fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_GetBand_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Band Detect fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[AUDIO] [%d] Return val %d [pbandStrength 0x%x] \n",  __LINE__, (int)Status.bRet, (int)*pbandStrength);
    return Status.bRet;

#else

    return _MApi_AUDIO_SIF_GetBandDetect(soundSystem, pbandStrength);

#endif
}

MS_BOOL MApi_AUDIO_SIF_GetBandDetect_Amp(MS_U8 soundSystem, MS_U32 *pbandStrength)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_GETBAND_OP Status;
    Status.eType = SIF_GET_BAND_DETECT_AMP;
    Status.SoundSystem = soundSystem;
    Status.pbandStrength = pbandStrength;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Band Detect Amp fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_GetBand_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Band Detect Amp fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d \n", __FUNCTION__, (int)Status.bRet);
    return Status.bRet;

#else

    return _MApi_AUDIO_SIF_GetBandDetect_Amp(soundSystem, pbandStrength);

#endif
}

MS_BOOL MApi_AUDIO_SIF_CheckAvailableSystem(MS_U8 standard, MS_U8 *pexist)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM Status;
    Status.standard = standard;
    Status.pexist = pexist;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check Available System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_CheckAvailableSystem, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check Available Systemp fail\n", __LINE__);
        //return;
    }

    AUDIO_DBGRT("[AUDIO] [%s] [%d] Return val %d [pexist 0x%x]\n",  __FUNCTION__, __LINE__, (int)Status.bRet, (int)*pexist);
    return Status.bRet;

#else

    return _MApi_AUDIO_SIF_CheckAvailableSystem(standard, pexist);

#endif
}

MS_BOOL MApi_AUDIO_SIF_CheckA2DK(MS_U8 SystemType, MS_U8 *pexist)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_CHECK_A2DK Status;
    Status.SystemType= SystemType;
    Status.pexist = pexist;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check A2DK fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_CheckA2DK, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check A2DK fail\n", __LINE__);
        //return;
    }

    AUDIO_DBGRT("[AUDIO] [%s] [%d] Return val %d [pexist 0x%x]\n",  __FUNCTION__, __LINE__, (int)Status.bRet, (int)*pexist);
    return Status.bRet;

#else

    return _MApi_AUDIO_SIF_CheckA2DK(SystemType, pexist);

#endif
}

MS_BOOL MApi_AUDIO_SIF_GetSoundStandard(MS_U8 *B_SifStandard)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SIF_GETSOUNDSTANDARD Status;
    Status.B_SifStandard = B_SifStandard;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Sound Standard fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_GetSoundStandard, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Sound Standard fail\n", __LINE__);
        //return;
    }

    AUDIO_DBGRT("[AUDIO] [%s] [%d] Return val %d [B_SifStandard 0x%x] \n",  __FUNCTION__, __LINE__, (int)Status.bRet, (int)*B_SifStandard);
    return Status.bRet;

#else

    return _MApi_AUDIO_SIF_GetSoundStandard(B_SifStandard);

#endif
}

void MApi_AUDIO_SIF_RedoSetStandard(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Redo Set Standard fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_RedoSetStandard, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Redo Set Standard fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_RedoSetStandard();

#endif
}

#if 0
void MApi_AUDIO_SIF_ADC_Reset(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADC Reset fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_ADC_Reset, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] ADC Reset fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SIF_ADC_Reset();

#endif
}
#endif

MS_BOOL MApi_AUDIO_GetBtscA2StereoLevel(MS_U16 *pLevel)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_GETBTSC_A2STEREO_LEVEL Status;
    Status.pLevel = pLevel;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Btsc A2 Stereo Level fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetBtscA2StereoLevel, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get Btsc A2 Stereo Level fail\n", __LINE__);
        //return;
    }

    AUDIO_DBGRT("[AUDIO] [%s] [%d] Return val %d [pLevel 0x%x] \n", __FUNCTION__, __LINE__, (int)Status.bRet, (int)*pLevel);
    return Status.bRet;

#else

    return _MApi_AUDIO_GetBtscA2StereoLevel(pLevel);

#endif
}

MS_BOOL MApi_AUDIO_SetBtscA2ThresholdLevel(MS_U16 thresholdLevel, MS_U16 *pRF2NSR_map)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SETBTSCA2THRESHOLDLEVEL Status;
    Status.thresholdLevel = thresholdLevel;
    Status.pRF2NSR_map = pRF2NSR_map;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Btsc A2 Threshold Level fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetBtscA2ThresholdLevel, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Btsc A2 Threshold Level fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d \n", __FUNCTION__, (int)Status.bRet);
    return Status.bRet;

#else

    return _MApi_AUDIO_SetBtscA2ThresholdLevel(thresholdLevel,  pRF2NSR_map);

#endif
}

void MApi_AUDIO_CheckNicamDigital(MS_U8 *pisNicamDetect)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_CHECKNICAMDIGITAL Status;
    Status.pisNicamDetect = pisNicamDetect;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check NiCam Digital fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_CheckNicamDigital, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Check NiCam Digital fail\n", __LINE__);
        //return;
    }

    AUDIO_DBGRT(" [AUDIO] [%s] [%d] [pisNicamDetect 0x%x]  \n", __FUNCTION__, __LINE__, (int)*pisNicamDetect);
    return;

#else

    return _MApi_AUDIO_CheckNicamDigital(pisNicamDetect);

#endif
}

void MApi_AUDIO_SetThresholdType(THR_TBL_TYPE_C code *ThrTbl, MS_U8 num,  MS_U8 standardtype)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_SETTHRESHOLDTYPE Status;
    Status.ThrTbl = ThrTbl;
    Status.num = num;
    Status.standardtype = standardtype;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Threshold Type fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetThreshold_Type, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Set Threshold Type fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0 \n", __FUNCTION__);
    return;

#else

    return _MApi_AUDIO_SetThresholdType(ThrTbl, num, standardtype);

#endif
}

MS_U32 MApi_AUDIO_Decoded_Frame_Cnt(MS_BOOL bDspType)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_DECODE_FRAME_CNT Status;
    Status.bDspType = bDspType;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Decode Frame Cnt fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DecodeFramCnt, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Decode Frame Cnt fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d \n", __FUNCTION__, (int)Status.u32Ret);
    return Status.u32Ret;

#else

    return _MApi_AUDIO_Decoded_Frame_Cnt(bDspType);

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Init()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_Init(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_INIT sPcmMixerInit;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Init fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerInit, 0x00, sizeof(AUDIO_ST_PCMMIXER_INIT));
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Init, (void*)&sPcmMixerInit) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Init fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmMixerInit.s32Ret);
    return (sPcmMixerInit.s32Ret);

#else

    return _MApi_AUDIO_PcmMixer_Init();

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_DeInit()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_DeInit(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_DEINIT sPcmMixerDeInit;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer DeInit fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerDeInit, 0x00, sizeof(AUDIO_ST_PCMMIXER_DEINIT));
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_DeInit, (void*)&sPcmMixerDeInit) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer DeInit fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmMixerDeInit.s32Ret);
    return (sPcmMixerDeInit.s32Ret);

#else

    return _MApi_AUDIO_PcmMixer_DeInit();

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Open()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_Open(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_OPEN sPcmMixerOpen;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Open fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerOpen, 0x00, sizeof(AUDIO_ST_PCMMIXER_OPEN));
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Open, (void*)&sPcmMixerOpen) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Open fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmMixerOpen.s32Ret);
    return (sPcmMixerOpen.s32Ret);

#else

    return _MApi_AUDIO_PcmMixer_Open();

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Close()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_Close(MS_S32 s32PcmMixingId)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_CLOSE sPcmMixerClose;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Close fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerClose, 0x00, sizeof(AUDIO_ST_PCMMIXER_CLOSE));
    sPcmMixerClose.s32PcmMixingId = s32PcmMixingId;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Close, (void*)&sPcmMixerClose) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Close fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmMixerClose.s32Ret);
    return (sPcmMixerClose.s32Ret);

#else

    return _MApi_AUDIO_PcmMixer_Close(s32PcmMixingId);

#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Flush()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_Flush(MS_S32 s32PcmMixingId)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_FLUSH sPcmMixerFlush;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Start fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerFlush, 0x00, sizeof(AUDIO_ST_PCMMIXER_FLUSH));
    sPcmMixerFlush.s32PcmMixingId = s32PcmMixingId;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Flush, (void*)&sPcmMixerFlush) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Flush fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmMixerFlush.s32Ret);
    return (sPcmMixerFlush.s32Ret);

#else

    return _MApi_AUDIO_PcmMixer_Flush(s32PcmMixingId);

#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Start()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_Start(MS_S32 s32PcmMixingId)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_START sPcmMixerStart;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Start fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerStart, 0x00, sizeof(AUDIO_ST_PCMMIXER_START));
    sPcmMixerStart.s32PcmMixingId = s32PcmMixingId;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Start, (void*)&sPcmMixerStart) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Start fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmMixerStart.s32Ret);
    return (sPcmMixerStart.s32Ret);

#else

    return _MApi_AUDIO_PcmMixer_Start(s32PcmMixingId);

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Stop()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_Stop(MS_S32 s32PcmMixingId)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_STOP sPcmMixerStop;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Stop fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerStop, 0x00, sizeof(AUDIO_ST_PCMMIXER_STOP));
    sPcmMixerStop.s32PcmMixingId = s32PcmMixingId;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Stop, (void*)&sPcmMixerStop) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Stop fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmMixerStop.s32Ret);
    return (sPcmMixerStop.s32Ret);

#else

    return _MApi_AUDIO_PcmMixer_Stop(s32PcmMixingId);

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Set()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_Set(MS_S32 s32PcmMixingId, MS_U32 u32Cmd, void *pData)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_SET sPcmMixerSet;

    if (pData == NULL)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! pData should not be NULL!\n", __LINE__);
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Set fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerSet, 0x00, sizeof(AUDIO_ST_PCMMIXER_SET));
    sPcmMixerSet.s32PcmMixingId = s32PcmMixingId;
    sPcmMixerSet.u32Cmd = u32Cmd;
    sPcmMixerSet.pData = pData;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Set, (void*)&sPcmMixerSet) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Set fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmMixerSet.s32Ret);
    return (sPcmMixerSet.s32Ret);

#else

    return _MApi_AUDIO_PcmMixer_Set(s32PcmMixingId, u32Cmd, pData);

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Get()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_Get(MS_S32 s32PcmMixingId, MS_U32 u32Cmd, void *pData)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_GET sPcmMixerGet;

    if (pData == NULL)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! pData should not be NULL!\n", __LINE__);
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Set fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerGet, 0x00, sizeof(AUDIO_ST_PCMMIXER_GET));
    sPcmMixerGet.s32PcmMixingId = s32PcmMixingId;
    sPcmMixerGet.u32Cmd = u32Cmd;
    sPcmMixerGet.pData = pData;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Get, (void*)&sPcmMixerGet) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Set fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmMixerGet.s32Ret);
    return (sPcmMixerGet.s32Ret);

#else

    return _MApi_AUDIO_PcmMixer_Get(s32PcmMixingId, u32Cmd, pData);

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Write()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MApi_AUDIO_PcmMixer_Write(MS_S32 s32PcmMixingId, void *pBuf, MS_U32 u32Size)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_WRITE sPcmMixerWrite;

    if (pBuf == NULL)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! pBuf should not be NULL!\n", __LINE__);
    }

    if (u32Size == 0)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! u32Size should not be zero!\n", __LINE__);
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Set fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerWrite, 0x00, sizeof(AUDIO_ST_PCMMIXER_WRITE));
    sPcmMixerWrite.s32PcmMixingId = s32PcmMixingId;
    sPcmMixerWrite.pBuf = pBuf;
    sPcmMixerWrite.u32Size = u32Size;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Write, (void*)&sPcmMixerWrite) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Set fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d \n", __FUNCTION__, sPcmMixerWrite.bRet);
    return (sPcmMixerWrite.bRet);

#else

    return _MApi_AUDIO_PcmMixer_Write(s32PcmMixingId, pBuf, u32Size);

#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PcmMixer_Func()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MApi_AUDIO_PcmMixer_Func(MS_U32 u32Arg)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCMMIXER_FUNC sPcmMixerFunc;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Set fail\n", __LINE__);
        //return;
    }

    memset(&sPcmMixerFunc, 0x00, sizeof(AUDIO_ST_PCMMIXER_FUNC));
    sPcmMixerFunc.u32Arg = u32Arg;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmMixer_Func, (void*)&sPcmMixerFunc) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Mixer Set fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%06d] Return val %ld \n", __LINE__, sPcmMixerFunc.s32Ret);
    return sPcmMixerFunc.s32Ret;

#else

    return _MApi_AUDIO_PcmMixer_Func(u32Arg);

#endif
}

MS_U16 MApi_AUDIO_HDMI_RX_GetHdmiInChannelStatus(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get HDMI IN Channel Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetHDMIINChannelStatus, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get HDMI IN Channel Status fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u32param[1]);
    return (MS_U16)u32param[1];

#else

    return _MApi_AUDIO_HDMI_RX_GetHdmiInChannelStatus();

#endif
}

MS_S32 MApi_AUDIO_PCM_Open(VOID *pData)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCM_OPEN sPcmOpen;

    if (pData == NULL)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! pData should not be NULL!\n", __LINE__);
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Open fail\n", __LINE__);
        //return;
    }

    memset(&sPcmOpen, 0x00, sizeof(AUDIO_ST_PCM_OPEN));
    sPcmOpen.pData = pData;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCM_Open, (void*)&sPcmOpen) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Open fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmOpen.s32Ret);
    return (sPcmOpen.s32Ret);

#else

    return _MApi_AUDIO_PCM_Open(pData);

#endif
}

MS_S32 MApi_AUDIO_PCM_Close(MS_S32 s32DeviceId)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCM_CLOSE sPcmClose;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Close fail\n", __LINE__);
        //return;
    }

    memset(&sPcmClose, 0x00, sizeof(AUDIO_ST_PCM_CLOSE));
    sPcmClose.s32DeviceId = s32DeviceId;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCM_Close, (void*)&sPcmClose) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Close fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmClose.s32Ret);
    return (sPcmClose.s32Ret);

#else

    return _MApi_AUDIO_PCM_Close(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_PCM_Start(MS_S32 s32DeviceId)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCM_START sPcmStart;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Start fail\n", __LINE__);
        //return;
    }

    memset(&sPcmStart, 0x00, sizeof(AUDIO_ST_PCM_START));
    sPcmStart.s32DeviceId = s32DeviceId;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCM_Start, (void*)&sPcmStart) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Start fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmStart.s32Ret);
    return (sPcmStart.s32Ret);

#else

    return _MApi_AUDIO_PCM_Start(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_PCM_Stop(MS_S32 s32DeviceId)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCM_STOP sPcmStop;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Stop fail\n", __LINE__);
        //return;
    }

    memset(&sPcmStop, 0x00, sizeof(AUDIO_ST_PCM_STOP));
    sPcmStop.s32DeviceId = s32DeviceId;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCM_Stop, (void*)&sPcmStop) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Stop fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmStop.s32Ret);
    return (sPcmStop.s32Ret);

#else

    return _MApi_AUDIO_PCM_Stop(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const VOID *pData)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCM_SET sPcmSet;

    if (pData == NULL)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! pData should not be NULL!\n", __LINE__);
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Set fail\n", __LINE__);
        //return;
    }

    memset(&sPcmSet, 0x00, sizeof(AUDIO_ST_PCM_SET));
    sPcmSet.s32DeviceId = s32DeviceId;
    sPcmSet.u32Cmd = u32Cmd;
    sPcmSet.pData = pData;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCM_Set, (void*)&sPcmSet) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Set fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmSet.s32Ret);
    return (sPcmSet.s32Ret);

#else

    return _MApi_AUDIO_PCM_Set(s32DeviceId, u32Cmd, pData);

#endif
}

MS_S32 MApi_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, VOID *pData)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCM_GET sPcmGet;

    if (pData == NULL)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! pData should not be NULL!\n", __LINE__);
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Get fail\n", __LINE__);
        //return;
    }

    memset(&sPcmGet, 0x00, sizeof(AUDIO_ST_PCM_GET));
    sPcmGet.s32DeviceId = s32DeviceId;
    sPcmGet.u32Cmd = u32Cmd;
    sPcmGet.pData = pData;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCM_Get, (void*)&sPcmGet) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Get fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmGet.s32Ret);
    return (sPcmGet.s32Ret);

#else

    return _MApi_AUDIO_PCM_Get(s32DeviceId, u32Cmd, pData);

#endif
}

MS_U32 MApi_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCM_READ sPcmRead;

    if (pBuf == NULL)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! pBuf should not be NULL!\n", __LINE__);
    }

    if (u32Size == 0)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! u32Size should not be zero!\n", __LINE__);
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Read fail\n", __LINE__);
        //return;
    }

    memset(&sPcmRead, 0x00, sizeof(AUDIO_ST_PCM_READ));
    sPcmRead.s32DeviceId = s32DeviceId;
    sPcmRead.pBuf = pBuf;
    sPcmRead.u32Size = u32Size;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCM_Read, (void*)&sPcmRead) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Read fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %u \n", __FUNCTION__, (unsigned int)sPcmRead.u32Ret);
    return (sPcmRead.u32Ret);

#else

    return _MApi_AUDIO_PCM_Read(s32DeviceId, pBuf, u32Size);

#endif
}

MS_U32 MApi_AUDIO_PCM_Write(MS_S32 s32DeviceId, const VOID *pBuf, MS_U32 u32Size)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCM_WRITE sPcmWrite;

    if (pBuf == NULL)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! pBuf should not be NULL!\n", __LINE__);
    }

    if (u32Size == 0)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Error! u32Size should not be zero!\n", __LINE__);
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Write fail\n", __LINE__);
        //return;
    }

    memset(&sPcmWrite, 0x00, sizeof(AUDIO_ST_PCM_WRITE));
    sPcmWrite.s32DeviceId = s32DeviceId;
    sPcmWrite.pBuf = pBuf;
    sPcmWrite.u32Size = u32Size;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCM_Write, (void*)&sPcmWrite) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Write fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %u \n", __FUNCTION__, (unsigned int)sPcmWrite.u32Ret);
    return (sPcmWrite.u32Ret);

#else

    return _MApi_AUDIO_PCM_Write(s32DeviceId, pBuf, u32Size);

#endif
}

MS_S32 MApi_AUDIO_PCM_Flush(MS_S32 s32DeviceId)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    AUDIO_ST_PCM_FLUSH sPcmFlush;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Flush fail\n", __LINE__);
        //return;
    }

    memset(&sPcmFlush, 0x00, sizeof(AUDIO_ST_PCM_FLUSH));
    sPcmFlush.s32DeviceId = s32DeviceId;
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCM_Flush, (void*)&sPcmFlush) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] PCM Flush fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %ld \n", __FUNCTION__, sPcmFlush.s32Ret);
    return (sPcmFlush.s32Ret);

#else

    return _MApi_AUDIO_PCM_Flush(s32DeviceId);

#endif
}

#if 0
void MAPI_AUDIO_Get_LIB_Ver(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get LIB Ver fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetLIBVer, 0) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get LIB Ver fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else

    return _MAPI_AUDIO_Get_LIB_Ver();

#endif
}

long long MApi_AUDIO_GET_DSP_PTS(void)
{
    AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u64param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get DSP PTS fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDSPPTS, (void*)u64param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Get DSP PTS fail\n", __LINE__);
        //return;
    }
    AUDIO_DBGRT("[%s] Return val %d\n", __FUNCTION__, (int)u64param[1]);
    return (MS_U16)u64param[1];

#else

    return _MApi_AUDIO_GET_DSP_PTS();

#endif
}

////     TEST   Block   ////
#define TEST_GOP1 0
#define TEST_GOP2 0
#define TEST_GOP3 0
#define TEST_GOP4 0
#define TEST_GOP5 0
#define TEST_GOP7 0
#define TEST_GOP8 0
#define TEST_GOP9 0
#define TEST_GOP10 0
#define TEST_GOP11 0

int VT_Count = 0;

void MApi_AUDIO_EnableAutoVolume(MS_BOOL EnAVC)
{
    //AUDIO_V2DBGMSG("[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef AUDIO_UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnAVC;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableAutoVolume;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
        return;
    }

    #if 1
    if(VT_Count < 5)
    {
        if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
        {
            AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
            return;
        }
        VT_Count++;
        printf("\033[1;32m [%s] VT_Count %d \033[0m \n", __FUNCTION__, (int)VT_Count);

    }
    else
    {
        printf("\033[1;32m [%s] VT_Count %d  Verity Test Start......\033[0m \n", __FUNCTION__, (int)VT_Count);

        // Test block
        VT_Count++;
        // Test block end

        #if (TEST_GOP1 == 1)        //audio system
        MS_U16 u16Data = 0;
        MApi_AUDIO_SetPowerOn(TRUE);
        u16Data = MApi_AUDIO_ReadMailBox(0, 5);
        MApi_AUDIO_WriteMailBox(0, 5, u16Data);
        MApi_AUDIO_FwTriggerDSP(0);
        MApi_AUDIO_SetFwStatus(TRUE);
        MApi_AUDIO_I2S_SetMode(AUDIO_I2S_SOURCE_CH, 0);
        MApi_AUDIO_EnaEarphone_LowPower_Stage();
        MApi_AUDIO_EnaEarphone_HighDriving_Stage();
        MApi_AUDIO_SendIntrupt(0, 0);
        MApi_AUDIO_RebootDsp(DSP_SE);
        MApi_AUDIO_ExitAudioSystem();
        MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
        #endif

        #if (TEST_GOP2 == 1)        //audio sound
        MS_U16 u16Data = 0;
        MS_U32 u32Data = 0;
        u32Data = u16Data = MApi_SND_GetParam1(Sound_GET_PARAM_EQ, 0);
        MApi_AUDIO_SOUND_SetParam(Sound_GET_PARAM_EQ, &u32Data);
        MApi_AUDIO_SetPreScale(0, 0x6F);
        MApi_AUDIO_SourceConnect(E_CONNECT_KTV, TRUE);
        MApi_AUDIO_SetDynamicBass(0x0C);
        MApi_AUDIO_AbsoluteBass(0);
        MApi_AUDIO_AbsoluteTreble(0);
        MApi_AUDIO_ConvertVolumeUnit(100);
        AUDIO_PEQ_COEF eq_coef;
        eq_coef.band = 0;
        eq_coef.sfs = 0;
        eq_coef.scale = 0;
        eq_coef.a0 = 0;
        eq_coef.a1 = 0;
        eq_coef.a2 = 0;
        eq_coef.b1 = 0;
        eq_coef.b2 = 0;
        eq_coef.type = AUDIO_COEF_PEQ;
        eq_coef.enable = FALSE;
        eq_coef.precision = AUDIO_SINGLE_PRECISION;
        MApi_AUDIO_SetPEQCoef(&eq_coef);
        MApi_AUDIO_EnablePEQ(FALSE);
        MApi_AUDIO_EnableDcRemove(TRUE);
        #endif

        #if (TEST_GOP3 == 1)        //audio DTV
        MApi_AUDIO_GetMAD_LOCK();
        MApi_AUDIO_ReleaseDecodeSystem(AU_DEC_MAX);
        AudioDecStatus_t AudioDecStatus;
        MApi_AUDIO_GetDecodeSystem(AU_DEC_ID1, &AudioDecStatus);
        MApi_AUDIO_SetDecodeSystem(AU_DEC_ID1, &AudioDecStatus);
        MApi_AUDIO_SetDecodeCmd(AU_DEC_ID1, MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_GetDecSysSupportStatus(MSAPI_AUD_DVB_MPEG);
        MApi_AUDIO_GetDecStatus();
        MApi_AUDIO_GetCommand(DVB_Audio_Decoder1);
        #endif

        #if (TEST_GOP4 == 1)        //audio pvr
        MApi_AUDIO_SetEncInit(AU_DVB_ENCBR_192K, 2);
        MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD_STOP);
        AU_DVB_ENC_FRAME_INFO EncFrameInfo;
        MApi_AUDIO_GetEncodeFrameInfo(&EncFrameInfo);
        MApi_AUDIO_GetEncodeDoneFlag();
        #endif

        #if (TEST_GOP5 == 1)        // audio atv
        MApi_AUDIO_SIF_StartAutoStandardDetection();
        MApi_AUDIO_SIF_GetResultOfAutoStandardDetection();
        #endif

        #if (TEST_GOP7 == 1)        // audio SPDIF
        MApi_AUDIO_SPDIF_SetMute(FALSE);
        MApi_AUDIO_SPDIF_GetMode();
        Digital_Out_Channel_Status_t stChannelStatus;
        MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stChannelStatus);
        #endif

        #if (TEST_GOP8 == 1)        //audio hdmi
        MS_BOOL onOff = FALSE;
        AUDIO_FS_TYPE hdmi_smpFreq = 0;
        HDMI_TX_OUTPUT_TYPE outType = 1;
        MApi_AUDIO_HDMI_Tx_GetStatus(&onOff, &hdmi_smpFreq, &outType);
        MApi_AUDIO_HDMI_Monitor();
        MApi_AUDIO_HDMI_SetNonpcm(FALSE);
        MApi_AUDIO_DTV_HDMI_CFG(1);
        MApi_AUDIO_HDMI_GetSynthFreq();
        MApi_AUDIO_HDMI_SetDownSample(0);
        ST_HDMI_AUDIO_STATUS hdmiAudioSts;
        MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(&hdmiAudioSts);
        #endif

        #if (TEST_GOP9 == 1)        // audio advance
        MApi_AUDIO_SeInit();
        MApi_AUDIO_ADVSOUND_SetParam(0, 0, 0);
        MApi_AUDIO_ADVSND_SetParam(0, 0, 0);
        MApi_AUDIO_ADVSOUND_GetInfo(0);
        MApi_DBXTV_SetMode(0, 0, 0, 0);
        MApi_AUDIO_SetAdvSndSys(MSAPI_AUD_ADVSND_BBE);
        MApi_AUDIO_SetVDS(0);
        MApi_AUDIO_SetVSPK(0);
        MApi_AUDIO_SetSRS(0);
        MApi_AUDIO_SetBBE(0, 1);
        MApi_AUDIO_VSPK_WMod(1);
        MApi_AUDIO_VSPK_SMod(1);
        MApi_AUDIO_SRS_DC(1);
        MApi_AUDIO_SRS_TruBass(1);
        MApi_AUDIO_SRS_SetTsxtPara(0, 0);
        MApi_AUDIO_SetSRSTSHD(1);
        MApi_AUDIO_TSHD_Definition(0);
        MApi_AUDIO_SRS_SetTshdPara(0, 0);
        #endif

        #if (TEST_GOP10 == 1)       //audio dtv
        MApi_AUDIO_GetKTVInfo (0, 0);
        MApi_AUDIO_SetKTVInfo (0, 0, 0, 2);
        MApi_AUDIO_SetMixModeVolume(0, 0, 0, 0);
        MApi_AUDIO_SetMixModeMute(0, 0, TRUE);
        MApi_AUDIO_PlayMenuSound();
        #endif

        #if (TEST_GOP11 == 1)      //audio MM
        MApi_AUDIO_Init(MSAPI_AUD_DVB_MPEG);
        MApi_AUDIO_SetAACInfo(Audio_AAC_error_count, 0, 0);
        MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_DecStatus);
        MApi_AUDIO_GetAC3Info(Audio_AC3_infoType_DecStatus);
        MApi_AUDIO_GetAACInfo(Audio_AAC_error_count);
        MApi_AUDIO_GetWmaInfo(Audio_WMA_infoType_DecStatus);
        MApi_AUDIO_GetDTSInfo(Audio_DTS_infoType_DecStatus);
        MApi_AUDIO_XPCM_Param(LPCM, 0, 0, 0, 0, 0);
        MApi_AUDIO_XPCM2_CheckIntStatus();
        MApi_AUDIO_StartBrowse();
        MApi_AUDIO_PauseDecode();
        MApi_AUDIO_CheckPlayDone();
        MApi_AUDIO_GetResidualBufferSize();
        MApi_AUDIO_GetPCMBufferSize(100);
        MApi_AUDIO_GetPCMBufferSize2(100,100);
        MApi_AUDIO_GetCurrentFrameNumber();
        MApi_AUDIO_GetSampleRate();
        MApi_AUDIO_GetBitRate();
        MApi_AUDIO_GetLayer();
        MApi_AUDIO_SetSampleRateIndex(1);
        MApi_AUDIO_FileEndNotification();
        MApi_AUDIO_GetEsMEMCnt();
        MApi_AUDIO_MM_SetInput (0, 0);
        MApi_AUDIO_MM_CheckPlayDone (0);
        MS_U32 u32WrtAddr = 0x1000;
        MS_U32 u32WrtByte = 10;
        MApi_AUDIO_MM_CheckInputRequest(0, &u32WrtAddr, &u32WrtByte);
        MApi_AUDIO_DmaReader_Init(0);
        MApi_AUDIO_DmaReader_AllInput_Init();
        MS_U32 buff = 0;
        //MApi_AUDIO_DmaReader_WritePCM(&buff, u32WrtByte);     // core dump cause by buffer address
        MApi_AUDIO_DmaWriter_Init();
        MApi_AUDIO_USBPCM_Enable(FALSE);
        MApi_AUDIO_USBPCM_SetFlag(FALSE);
        MApi_AUDIO_USBPCM_GetFlag();
        AUDIO_UPLOAD_INFO uploadInfo;
        MApi_AUDIO_USBPCM_GetMemInfo(&uploadInfo);
        MApi_AUDIO_PCMCapture_Init(0, 0);
        MApi_AUDIO_PCMCapture_Start(0);
        MApi_AUDIO_PCMCapture_Stop(0);
        MApi_AUDIO_PCMCapture_Read(0, &buff, u32WrtByte);
        MApi_AUDIO_VoIP_Config(0, 0, 0);
        MApi_AUDIO_ALSA_Check();
        MApi_AUDIO_ALSA_Enable(FALSE);
        MApi_AUDIO_UNI_CheckDecodeDone(&u32WrtAddr, &u32WrtByte);
        MApi_AUDIO_UNI_SetOutput(0);
        MApi_AUDIO_UNI_Set_PCMInputWriteAddr (0);
        MApi_AUDIO_UNI_Get_OutPCMLevel();
        audioRingTask auRingTask, auRingTask_1;
        MApi_AUDIO_RingTask(&auRingTask) ;
        MApi_AUDIO_Ring_DataTransfer(&auRingTask, &auRingTask_1);
        MApi_AUDIO_SetAudioParam2(0, 0, 0);
        MApi_AUDIO_GetAudioInfo2(0, 0, &buff);
        MApi_AUDIO_GetDDRInfo(0, 0);
        #endif
    }
    #else
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EnableAutoVolume, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        AUDIO_V2DBGMSG("[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
        return;
    }
    #endif
    //AUDIO_DBGRT("[%s] Return 0\n", __FUNCTION__);
    return;

#else   //utopia 1.0
    if(VT_Count < 5)
    {
        VT_Count++;
        printf("\033[1;32m [%s] VT_Count %d \033[0m \n", __FUNCTION__, (int)VT_Count);
        return _MApi_AUDIO_EnableAutoVolume(EnAVC);
    }
    else
    {
        printf("\033[1;32m [%s] VT_Count %d  Verity Test Start......\033[0m \n", __FUNCTION__, (int)VT_Count);

        #if (TEST_GOP1 == 1)        //audio system
        MS_U16 u16Data = 0;
        MApi_AUDIO_SetPowerOn(TRUE);
        u16Data = MApi_AUDIO_ReadMailBox(0, 5);
        MApi_AUDIO_WriteMailBox(0, 5, u16Data);
        MApi_AUDIO_FwTriggerDSP(0);
        MApi_AUDIO_SetFwStatus(TRUE);
        MApi_AUDIO_I2S_SetMode(AUDIO_I2S_SOURCE_CH, 0);
        MApi_AUDIO_EnaEarphone_LowPower_Stage();
        MApi_AUDIO_EnaEarphone_HighDriving_Stage();
        MApi_AUDIO_SendIntrupt(0, 0);
        MApi_AUDIO_RebootDsp(DSP_SE);
        MApi_AUDIO_ExitAudioSystem();
        MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
        #endif

        #if (TEST_GOP2 == 1)        //audio sound
        MS_U16 u16Data = 0;
        MS_U32 u32Data = 0;

        u32Data = u16Data = MApi_SND_GetParam1(Sound_GET_PARAM_EQ, 0);
        MApi_AUDIO_SOUND_SetParam(Sound_GET_PARAM_EQ, &u32Data);
        MApi_AUDIO_SetPreScale(0, 0x6F);
        MApi_AUDIO_SourceConnect(E_CONNECT_KTV, TRUE);
        MApi_AUDIO_SetDynamicBass(0x0C);
        MApi_AUDIO_AbsoluteBass(0);
        MApi_AUDIO_AbsoluteTreble(0);
        MApi_AUDIO_ConvertVolumeUnit(100);

        AUDIO_PEQ_COEF eq_coef;
        eq_coef.band = 0;
        eq_coef.sfs = 0;
        eq_coef.scale = 0;
        eq_coef.a0 = 0;
        eq_coef.a1 = 0;
        eq_coef.a2 = 0;
        eq_coef.b1 = 0;
        eq_coef.b2 = 0;
        eq_coef.type = AUDIO_COEF_PEQ;
        eq_coef.enable = FALSE;
        eq_coef.precision = AUDIO_SINGLE_PRECISION;
        MApi_AUDIO_SetPEQCoef(&eq_coef);
        MApi_AUDIO_EnablePEQ(FALSE);
        MApi_AUDIO_EnableDcRemove(TRUE);
        #endif

        #if (TEST_GOP3 == 1)        //audio DTV
        MApi_AUDIO_GetMAD_LOCK();
        MApi_AUDIO_ReleaseDecodeSystem(AU_DEC_MAX);
        AudioDecStatus_t AudioDecStatus;
        MApi_AUDIO_GetDecodeSystem(AU_DEC_ID1, &AudioDecStatus);
        MApi_AUDIO_SetDecodeSystem(AU_DEC_ID1, &AudioDecStatus);
        MApi_AUDIO_SetDecodeCmd(AU_DEC_ID1, MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_GetDecSysSupportStatus(MSAPI_AUD_DVB_MPEG);
        MApi_AUDIO_GetDecStatus();
        MApi_AUDIO_GetCommand(DVB_Audio_Decoder1);
        #endif

        #if (TEST_GOP4 == 1)        //audio pvr
        MApi_AUDIO_SetEncInit(AU_DVB_ENCBR_192K, 2);
        MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD_STOP);
        AU_DVB_ENC_FRAME_INFO EncFrameInfo;
        MApi_AUDIO_GetEncodeFrameInfo(&EncFrameInfo);
        MApi_AUDIO_GetEncodeDoneFlag();
        #endif

        #if (TEST_GOP5 == 1)        // audio atv
        MApi_AUDIO_SIF_StartAutoStandardDetection();
        MApi_AUDIO_SIF_GetResultOfAutoStandardDetection();
        #endif

        #if (TEST_GOP7 == 1)        // audio SPDIF
        MApi_AUDIO_SPDIF_SetMute(FALSE);
        MApi_AUDIO_SPDIF_GetMode();
        Digital_Out_Channel_Status_t stChannelStatus;
        MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stChannelStatus);
        #endif

        #if (TEST_GOP8 == 1)        //audio hdmi
        MS_BOOL onOff = FALSE;
        AUDIO_FS_TYPE hdmi_smpFreq = 0;
        HDMI_TX_OUTPUT_TYPE outType = 1;
        MApi_AUDIO_HDMI_Tx_GetStatus(&onOff, &hdmi_smpFreq, &outType);
        MApi_AUDIO_HDMI_Monitor();
        MApi_AUDIO_HDMI_SetNonpcm(FALSE);
        MApi_AUDIO_DTV_HDMI_CFG(1);
        MApi_AUDIO_HDMI_GetSynthFreq();
        MApi_AUDIO_HDMI_SetDownSample(0);
        ST_HDMI_AUDIO_STATUS hdmiAudioSts;
        MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(&hdmiAudioSts);
        #endif

        #if (TEST_GOP9 == 1)        // audio advance
        MApi_AUDIO_SeInit();
        MApi_AUDIO_ADVSOUND_SetParam(0, 0, 0);
        MApi_AUDIO_ADVSND_SetParam(0, 0, 0);
        MApi_AUDIO_ADVSOUND_GetInfo(0);
        MApi_DBXTV_SetMode(0, 0, 0, 0);
        MApi_AUDIO_SetAdvSndSys(MSAPI_AUD_ADVSND_BBE);
        MApi_AUDIO_SetVDS(0);
        MApi_AUDIO_SetVSPK(0);
        MApi_AUDIO_SetSRS(0);
        MApi_AUDIO_SetBBE(0, 1);
        MApi_AUDIO_VSPK_WMod(1);
        MApi_AUDIO_VSPK_SMod(1);
        MApi_AUDIO_SRS_DC(1);
        MApi_AUDIO_SRS_TruBass(1);
        MApi_AUDIO_SRS_SetTsxtPara(0, 0);
        MApi_AUDIO_SetSRSTSHD(1);
        MApi_AUDIO_TSHD_Definition(0);
        MApi_AUDIO_SRS_SetTshdPara(0, 0);
        #endif

        #if (TEST_GOP10 == 1)       //audio dtv
        MApi_AUDIO_GetKTVInfo (0, 0);
        MApi_AUDIO_SetKTVInfo (0, 0, 0, 2);
        MApi_AUDIO_SetMixModeVolume(0, 0, 0, 0);
        MApi_AUDIO_SetMixModeMute(0, 0, TRUE);
        MApi_AUDIO_PlayMenuSound();
        #endif

        #if (TEST_GOP11 == 1)      //audio MM
        MApi_AUDIO_Init(MSAPI_AUD_DVB_MPEG);
        MApi_AUDIO_SetAACInfo(Audio_AAC_error_count, 0, 0);
        MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_DecStatus);
        MApi_AUDIO_GetAC3Info(Audio_AC3_infoType_DecStatus);
        MApi_AUDIO_GetAACInfo(Audio_AAC_error_count);
        MApi_AUDIO_GetWmaInfo(Audio_WMA_infoType_DecStatus);
        MApi_AUDIO_GetDTSInfo(Audio_DTS_infoType_DecStatus);
        MApi_AUDIO_XPCM_Param(LPCM, 0, 0, 0, 0, 0);
        MApi_AUDIO_XPCM2_Param(LPCM, 0, 0, 0, 0, 0);
        MApi_AUDIO_XPCM2_CheckIntStatus();
        MApi_AUDIO_StartBrowse();
        MApi_AUDIO_PauseDecode();
        MApi_AUDIO_CheckPlayDone();
        MApi_AUDIO_GetResidualBufferSize();
        MApi_AUDIO_GetPCMBufferSize(100);
        MApi_AUDIO_GetPCMBufferSize2(100,100);
        MApi_AUDIO_GetCurrentFrameNumber();
        MApi_AUDIO_GetSampleRate();
        MApi_AUDIO_GetBitRate();
        MApi_AUDIO_GetLayer();
        MApi_AUDIO_SetSampleRateIndex(1);
        MApi_AUDIO_FileEndNotification();
        MApi_AUDIO_GetEsMEMCnt();
        MApi_AUDIO_MM_SetInput (0, 0);
        MApi_AUDIO_MM_CheckPlayDone (0);
        MS_U32 u32WrtAddr = 0x1000;
        MS_U32 u32WrtByte = 10;
        MApi_AUDIO_MM_CheckInputRequest(0, &u32WrtAddr, &u32WrtByte);
        MApi_AUDIO_DmaReader_Init(0);
        MApi_AUDIO_DmaReader_AllInput_Init();
        MS_U32 buff = 0;
        //MApi_AUDIO_DmaReader_WritePCM(&buff, u32WrtByte);     // core dump cause by buffer address
        MApi_AUDIO_DmaWriter_Init();
        MApi_AUDIO_USBPCM_Enable(FALSE);
        MApi_AUDIO_USBPCM_SetFlag(FALSE);
        MApi_AUDIO_USBPCM_GetFlag();
        AUDIO_UPLOAD_INFO uploadInfo;
        MApi_AUDIO_USBPCM_GetMemInfo(&uploadInfo);
        MApi_AUDIO_PCMCapture_Init(0, 0);
        MApi_AUDIO_PCMCapture_Start(0);
        MApi_AUDIO_PCMCapture_Stop(0);
        MApi_AUDIO_PCMCapture_Read(0, &buff, u32WrtByte);
        MApi_AUDIO_VoIP_Config(0, 0, 0);
        MApi_AUDIO_ALSA_Check();
        MApi_AUDIO_ALSA_Enable(FALSE);
        MApi_AUDIO_UNI_CheckDecodeDone(&u32WrtAddr, &u32WrtByte);
        MApi_AUDIO_UNI_SetOutput(0);
        MApi_AUDIO_UNI_Set_PCMInputWriteAddr (0);
        MApi_AUDIO_UNI_Get_OutPCMLevel();
        audioRingTask auRingTask, auRingTask_1;
        MApi_AUDIO_RingTask(&auRingTask) ;
        MApi_AUDIO_Ring_DataTransfer(&auRingTask, &auRingTask_1);
        MApi_AUDIO_SetAudioParam2(0, 0, 0);
        MApi_AUDIO_GetAudioInfo2(0, 0, &buff);
        MApi_AUDIO_GetDDRInfo(0, 0);
        #endif
    }
#endif
}
#endif
