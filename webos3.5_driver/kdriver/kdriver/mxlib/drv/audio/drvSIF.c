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
///////////////////////////////////////////////////////////////////////////////
#define _AUSIF_C_

///////////////////////////////////////////////////////////////////////////////
//
/// file drvSIF.c
/// @author MStar Semiconductor Inc.
/// @brief SIF control driver
///////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsTypes.h"

// Internal
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"

#include "./internal/drvMAD.h"
#include "./internal/drvSIF.h"
#include "./internal/SifInitialTable.h"

#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halSIF.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "drvSYS.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/delay.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define SIF_ERRMSG(msg)  MS_CRITICAL_MSG(msg)
#define SIF_DBGMSG(msg)  MS_DEBUG_MSG(msg)

#define HINIBBLE(MS_U8)    ((MS_U8) / 0x10)
#define LONIBBLE(MS_U8)    ((MS_U8) & 0x0F)

#define AU_CMD_FC_TRACK         REG_SIF_CMD_FC_TRACK // [LM14]
// [LM14]
MS_BOOL hidevEnabled = 0;
ADEC_SIF_AVAILE_STANDARD_T u8SifStandardType; // A2, Nimca, FM..
ADEC_SIF_SOUNDSYSTEM_T u8SifSoundSystemType;  // BG, DK, I...
MS_U8  g_ucCarrierStableCnt = 0, g_ucPreCarrierStatus=0;

extern AU_CHIP_TYPE gAudioChipType;
extern void    HAL_MAD2_DisEn_MIUREQ(void);
// [LM14]
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
MS_U16 code c_RF2NSR_map[40] = /* defult 11  modified for 40 steps*/
{
    0x1db8,0x1cbb,0x1baa,0x19db,0x18d8,0x17ea,0x16e4,0x1491,0x1300,0x111a,      //
    0x0fb9,0x0d00,0x0cd6,0x0ba0,0x0a77,0x0964,0x0887,0x07a0,0x06d8,0x0635,      //
    0x058b,0x050c,0x0492,0x0420,0x03c0,0x037e,0x0320,0x02e6,0x02bd,0x028d,      //
    0x0278,0x0252,0x0200,0x01b0,0x0180,0x0140,0x0100,0x00b0,0x0080,0x0040,                                                              //
};

MS_U16 c_au_pal_sys_threshold[11][12]=
{
    {M_A2_THRESHOLD_ADDR_SE    , M_A2_THRESHOLD_ADDR_SE+1     , M_A2_THRESHOLD_ADDR_SE+2     , M_A2_THRESHOLD_ADDR_SE+3     , M_A2_THRESHOLD_ADDR_SE+4   , M_A2_THRESHOLD_ADDR_SE+5 , M_A2_THRESHOLD_ADDR_SE+6 , M_A2_THRESHOLD_ADDR_SE+7 ,M_A2_THRESHOLD_ADDR_SE+8 ,M_A2_THRESHOLD_ADDR_SE+9 ,M_A2_THRESHOLD_ADDR_SE+10 ,M_A2_THRESHOLD_ADDR_SE+11, },
    {BG_A2_THRESHOLD_ADDR_SE   , BG_A2_THRESHOLD_ADDR_SE+1    , BG_A2_THRESHOLD_ADDR_SE+2    , BG_A2_THRESHOLD_ADDR_SE+3    , BG_A2_THRESHOLD_ADDR_SE+4  , BG_A2_THRESHOLD_ADDR_SE+5, BG_A2_THRESHOLD_ADDR_SE+6, BG_A2_THRESHOLD_ADDR_SE+7,BG_A2_THRESHOLD_ADDR_SE+8 ,BG_A2_THRESHOLD_ADDR_SE+9 , BG_A2_THRESHOLD_ADDR_SE+10 ,BG_A2_THRESHOLD_ADDR_SE+11 ,},
    {DK_A2_THRESHOLD_ADDR_SE   , DK_A2_THRESHOLD_ADDR_SE+1    , DK_A2_THRESHOLD_ADDR_SE+2    , DK_A2_THRESHOLD_ADDR_SE+3    , DK_A2_THRESHOLD_ADDR_SE+4  , DK_A2_THRESHOLD_ADDR_SE+5, DK_A2_THRESHOLD_ADDR_SE+6, DK_A2_THRESHOLD_ADDR_SE+7,DK_A2_THRESHOLD_ADDR_SE+8 ,DK_A2_THRESHOLD_ADDR_SE+9 , DK_A2_THRESHOLD_ADDR_SE+10 ,DK_A2_THRESHOLD_ADDR_SE+11 ,},
    {I_FM_THRESHOLD_ADDR_SE    , I_FM_THRESHOLD_ADDR_SE+1     , I_FM_THRESHOLD_ADDR_SE+2     , I_FM_THRESHOLD_ADDR_SE+3     , NULL                    , NULL                  , NULL                  , NULL                  , NULL                  ,NULL                  ,NULL                  ,NULL                  ,},
    {L_AM_THRESHOLD_ADDR_SE      , L_AM_THRESHOLD_ADDR_SE+1       , NULL                      , NULL                      , NULL                    , NULL                  , NULL                  , NULL                  , NULL                  ,NULL                  ,NULL                  ,NULL                  ,},
    {NICAM_BGDKL_THRESHOLD_ADDR_SE     , NICAM_BGDKL_THRESHOLD_ADDR_SE+1      , NULL                      , NULL                      , NULL                    , NULL                  , NULL                  , NULL                  , NULL                  ,NULL                  ,NULL                  ,NULL                  ,},
    {NICAM_I_THRESHOLD_ADDR_SE      , NICAM_I_THRESHOLD_ADDR_SE+1       , NULL                      , NULL                      , NULL                    , NULL                  , NULL                  , NULL                  , NULL                  ,NULL                  ,NULL                  ,NULL                  ,},
    {HIDEV_M_THRESHOLD_ADDR_SE , HIDEV_M_THRESHOLD_ADDR_SE+1  , HIDEV_M_THRESHOLD_ADDR_SE+2  , HIDEV_M_THRESHOLD_ADDR_SE+3  , NULL                    , NULL                  , NULL                  , NULL                  , NULL                  ,NULL                  ,NULL                  ,NULL                  ,},
    {HIDEV_BG_THRESHOLD_ADDR_SE, HIDEV_BG_THRESHOLD_ADDR_SE+1 , HIDEV_BG_THRESHOLD_ADDR_SE+2 , HIDEV_BG_THRESHOLD_ADDR_SE+3 , NULL                    , NULL                  , NULL                  , NULL                  , NULL                  ,NULL                  ,NULL                  ,NULL                  ,},
    {HIDEV_DK_THRESHOLD_ADDR_SE, HIDEV_DK_THRESHOLD_ADDR_SE+1 , HIDEV_DK_THRESHOLD_ADDR_SE+2 , HIDEV_DK_THRESHOLD_ADDR_SE+3 , NULL                    , NULL                  , NULL                  , NULL                  , NULL                  ,NULL                  ,NULL                  ,NULL                  ,},
    {HIDEV_I_THRESHOLD_ADDR_SE , HIDEV_I_THRESHOLD_ADDR_SE+1  , HIDEV_I_THRESHOLD_ADDR_SE+2  , HIDEV_I_THRESHOLD_ADDR_SE+3  , NULL                    , NULL                  , NULL                  , NULL                  , NULL                  ,NULL                  ,NULL                  ,NULL                  ,},
};

MS_U16 code c_au_ntsc_sys_threshold[1][12]=
{
    {
        BTSC_THRESHOLD_ADDR_SE  + BTSC_MONO_ON_NSR_RATIO,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_MONO_OFF_NSR_RATIO,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_PILOT_AMPLITUDE_ON,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_PILOT_AMPLITUDE_OFF,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_SAP_ON_NSR_RATIO,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_SAP_OFF_NSR_RATIO,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_STEREO_ON_RATIO,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_STEREO_OFF_NSR_RATIO,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_SAP_AMPLITUDE_ON,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_SAP_AMPLITUDE_OFF,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_HIDEV_ON_NSR_RATIO,
        BTSC_THRESHOLD_ADDR_SE  + BTSC_HIDEV_OFF_NSR_RATIO,
    },
};

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_ReLoadCode()
/// @brief \b Function \b Description: This routine is used to reload the SIF algorithm code
/// @param u8Type1     \b type    :  sif algorithm type
/// @return MS_BOOL    \b TRUE --DSP reload code okay
///                       FALSE--DSP reload code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_ReLoadCode(MS_U8 u8Type)
{
    return HAL_SIF_ReLoadCode(u8Type);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SIF_Init()
/// @brief \b Function \b Description: This routine is the initialization for SIF
////////////////////////////////////////////////////////////////////////////////
void MDrv_SIF_Init(void)
{
    HAL_SIF_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name  : MDrv_SIF_SetSoundMode
/// @brief \b Function  \b Description : This routine is to set the sound mode for SIF module.
/// @param u8ModeType   \b : Sound Mode type
/// @return MS_BOOL     \b : set SIF sound mode
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_SetSoundMode(AUDIO_SIF_SOUND_MODE u8ModeType)
{
   return (HAL_SIF_SetSoundMode(u8ModeType));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_SetSystem()
/// @brief \b Function \b Description: This routine is to set  sif decoder system.
/// @param u8System_type \b system codec type  :
///                                  AU_SIF_BTSC,             //0
///                                  AU_SIF_PALSUM,           //1
///                                  AU_SIF_EIAJ,             //2
///                                  ......etc
/// @return MS_BOOL \b Set System ok or failure
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_SetSystem(MS_U8 u8SystemType)
{
    return HAL_SIF_SetSystem(u8SystemType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_StartAutoDetection
/// @brief \b Function \b Description : This routine is to start to detect the sound system for SIF Palsum module.
/// @return MS_BOOL    \b : start auto tuning
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_StartAutoDetection (void)
{
    return HAL_SIF_StartAutoDetect();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_SetThreshold()
/// @brief \b Function \b Description:  This routine is the subfunction of setting SIF threshold
/// @param ThrTbl  \b :          Thresholds Table.
///                             From MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl)
/// @param start_i  \b :         Thresholds Table starting index
///                - BTSC_TBL_INDEX
///                - A2_M_TBL_INDEX
///                - A2_M_TBL_INDEX
///                - A2_DK_TBL_INDEX
///                - FM_I_TBL_INDEX
///                - AM_TBL_INDEX
///                - NICAM_BG_TBL_INDEX
///                - NICAM_I_TBL_INDEX
///                - HIDEV_M_TBL_INDEX
///                - HIDEV_BG_TBL_INDEX
///                - HIDEV_DK_TBL_INDEX
///                - HIDEV_I_TBL_INDEX
/// @param num    \b :            Thresholds Table Length
///                - BTSC_TBL_LENGTH
///                - A2_M_TBL_LENGTH
///                - A2_M_TBL_LENGTH
///                - A2_DK_TBL_LENGTH
///                - FM_I_TBL_LENGTH
///                - AM_TBL_LENGTH
///                - NICAM_BG_TBL_LENGTH
///                - NICAM_I_TBL_LENGTH
///                - HIDEV_M_TBL_LENGTH
///                - HIDEV_BG_TBL_LENGTH
///                - HIDEV_DK_TBL_LENGTH
///                - HIDEV_I_TBL_LENGTH
/// @param standard_type  \b :         Standard Type
///                          - BTSC_STANDARD
///                          - A2_M_STANDARD
///                          - A2_BG_STANDARD
///                          - A2_DK_STANDARD
///                          - A2_I_STANDARD
///                          - AM_STANDARD
///                          - NICAM_BG_STANDARD
///                          - NICAM_I_STANDARD
///                          - HIDEV_M_STANDARD
///                          - HIDEV_BG_STANDARD
///                          - HIDEV_DK_STANDARD
///                          - HIDEV_I_STANDARD
////////////////////////////////////////////////////////////////////////////////
void MDrv_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl, MS_U8 start_i, MS_U8 num, AUDIO_SIF_SYSTEM_TYPE standard_type)
{
    MS_U8 i;
    MS_U16 value;
    AUDIO_SIF_SYSTEM_TYPE system_type = standard_type;

    system_type |= WRITE_THRESHOLD;  // coverity 8855
    ThrTbl = ThrTbl+start_i;
    for(i=start_i; i<(start_i+num); i++)
    {
        value =( (0x00FF & ThrTbl->HiByteValue) <<8)|(0x00FF & ThrTbl->LowByteValue);
        HAL_SIF_AccessThreshold(system_type,(AUDIO_SIF_THRESHOLD_TYPE)(i-start_i),value);  // coverity 8855
        ThrTbl++;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_AccessThreshold
/// @brief \b Function \b Description : This routine to read/write SIF threshold
/// @param rw_standard_type \b :   standard type
/// @param u8Threshold_type \b :   Threshold  type
/// @param u16Value         \b : write DRAM data value
/// @return  MS_U32         \b : return DRAM data value
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_SIF_AccessThreshold(AUDIO_SIF_SYSTEM_TYPE rw_standard_type, AUDIO_SIF_THRESHOLD_TYPE  u8Threshold_type, MS_U16 u16Value)
{
    return HAL_SIF_AccessThreshold(rw_standard_type, u8Threshold_type, u16Value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_GetOrginalGain
/// @brief \b Function \b Description : This routine to read current SIF gain
/// @return MS_BOOL    \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_GetOrginalGain(void)
{
    return HAL_SIF_GetOrginalGain();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_SetPrescale
/// @brief \b Function \b Description : This routine is to set the SIF Prescale.
/// @param u8Type      \b :
///                             SET_PRESCALE_A2_FM
///                            SET_PRESCALE_NICAM
///                             SET_PRESCALE_AM
///                             SET_PRESCALE_HIDEV
///                             SET_PRESCALE_FM_M
///                             SET_PRESCALE_HIDEV_M
///                             SET_PRESCALE_BTSC
///                             SET_PRESCALE_BTSC_MONO
///                             SET_PRESCALE_BTSC_STEREO
///                             SET_PRESCALE_BTSC_SAP
/// @param db_value    \b   :   The prescale value, unit is 0.25dB.
///                             0   : 0db
///                             1   :  0.25dB
///                             -1  : -0.25dB
/// @return MS_BOOL    \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_SetPrescale(AUDIO_SIF_GAIN_TYPE u8Type,int db_value)
{
    return HAL_SIF_SetPrescale(u8Type, (MS_S32)db_value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_IsPALType
/// @brief \b Function \b Description : This routine to check if the Palsum is in pal_type state or not
/// @return MS_BOOL    \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    MS_U8  sub_carrier_std;

    if ((HAL_SIF_GetDspCodeType()) != AU_SIF_PALSUM)
    {
        SIF_DBGMSG(printf("DSP type not palsum\n"));
        return FALSE;
    }

    sub_carrier_std = HAL_SIF_SendCmd(AU_SIF_CMD_GET_STANDARD_CMD, NULL, NULL) & 0xF0;
    switch( pal_type )
        {
        case AU_SIF_PAL_MONO:
            if (sub_carrier_std > SIF_MODE_HIDEV )
                return FALSE;
            else if ((HAL_SIF_SendCmd(AU_SIF_CMD_GET_HIDEV_ENABLE, NULL, NULL) == TRUE) != (sub_carrier_std == SIF_MODE_HIDEV))
                return FALSE;
            break;
        case AU_SIF_PAL_HIDEV:
            if (sub_carrier_std != SIF_MODE_HIDEV )
                return FALSE;
            break;
        case AU_SIF_PAL_A2:
            if (sub_carrier_std != SIF_MODE_A2 )
                return FALSE;
            break;
        case AU_SIF_PAL_NICAM:
            if (sub_carrier_std != SIF_MODE_NICAM )
                return FALSE;
            break;
        default:
            return FALSE;// UT
        }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_SetPALType
/// @brief \b Function \b Description : Set Palsum to the pal_type state
/// @return MS_BOOL    \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    MS_U8  sif_standard;
    MS_U8  u8NicamSupport = TRUE;

    if ((HAL_SIF_GetDspCodeType()) != AU_SIF_PALSUM)
    {
        SIF_DBGMSG(printf("DSP type not palsum\n"));
        return FALSE;
    }

    sif_standard = HAL_SIF_SendCmd(AU_SIF_CMD_GET_STANDARD_CMD, NULL, NULL) & 0x0F;

    switch (pal_type) {
        case AU_SIF_PAL_MONO:
            SIF_DBGMSG(printf("Utopia Set MONO \r\n"));
            if (HAL_SIF_SendCmd(AU_SIF_CMD_GET_HIDEV_ENABLE, NULL, NULL) == TRUE) {
                HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_HIDEV, NULL);
                HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, HAL_SIF_SendCmd(AU_SIF_CMD_GET_HIDEV_FILTER_BW_LEVEL, NULL, NULL));
                HAL_SIF_SendCmd(AU_SIF_CMD_RESET_FC_TRACKING, NULL, NULL);
                HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, TRUE, NULL);
            }
            else {
                HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, FALSE, NULL);
                HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_MONO, NULL);
                HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, 0x00);
            }
            break;
        case AU_SIF_PAL_HIDEV:
            HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_HIDEV, NULL);
            HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, HAL_SIF_SendCmd(AU_SIF_CMD_GET_HIDEV_FILTER_BW_LEVEL, NULL, NULL));
            HAL_SIF_SendCmd(AU_SIF_CMD_RESET_FC_TRACKING, NULL, NULL);
            HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, TRUE, NULL);
            break;
        case AU_SIF_PAL_A2:
            HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, FALSE, NULL);
            HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_A2, NULL);
            HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, 0x00);
            break;
        case AU_SIF_PAL_NICAM:
            if (gAudioChipType == AU_CHIP_M12 || gAudioChipType == AU_CHIP_Euler) {
                if (TRUE == MDrv_SYS_Query(E_SYS_QUERY_TTXNICAM_SUPPORTED))
                {
                    u8NicamSupport= TRUE;
                }
                else
                {
                    u8NicamSupport= FALSE;
                }
            }
            if(u8NicamSupport) {
                SIF_DBGMSG(printf("NICAM support, code = %x\n",u8NicamSupport));
                HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, FALSE, NULL);
                HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_NICAM, NULL);
                HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, 0x00);
                break;
            }
            else {
                SIF_DBGMSG(printf("NICAM not support, code = %x\n",u8NicamSupport));
                HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, FALSE, NULL);
                HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard, NULL);
                HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, 0x00);
                break;
            }
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_SetStandardType
/// @brief \b Function \b Description : This routine to set sif audio system
/// @param sif_type    \b : sif type
////////////////////////////////////////////////////////////////////////////////
void MDrv_SIF_SetStandardType(AUDIO_SIF_SET_STANDARD sif_type)
{
    HAL_SIF_SendCmd(AU_SIF_CMD_SET_STD, sif_type, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_GetDspCodeType
/// @brief \b Function \b Description : This routine to get audio DSP type
/// @return AUDIO_DSP_CODE_TYPE  \b : audio dsp code type
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_SIF_GetDspCodeType(void)
{
    return HAL_SIF_GetDspCodeType();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_GetDspType()
/// @brief \b Function \b Description:  This function is used to get the DSP(DSP_DEC or DSP_SE) which SIF module used.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8   : DSP which SIF modules (DSP_DEC or DSP_SE)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_SIF_GetDspType(void)
{
    return HAL_SIF_GetDspType();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_SendCmd()
/// @brief \b Function \b Description: This routine is to set play/stop command for SIF.
/// @param u8DecCmd    \b command type   :
///                                AU_DVB_DECCMD_STOP,      //0
///                                AU_DVB_DECCMD_PLAY,      //1
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_SIF_SendCmd(AU_SIF_CMD u8DecCmd, MS_U8 comm_arg1, MS_U8 comm_arg2)
{
    MS_U8 u8Return = 0;
    u8Return = HAL_SIF_SendCmd(u8DecCmd, comm_arg1, comm_arg2);

    return u8Return;
}

// [LM14]
//------------------------------------------------------------------------
//  Customer Function
//------------------------------------------------------------------------
MS_BOOL MDrv_MAD_SIF_GetBtscA2StereoLevel(MS_U16 *pLevel)
{
    MS_U8  data[2];
    MS_U8 u8codeTypeDSP = HAL_SIF_GetDspCodeType();

    if (u8codeTypeDSP == AU_SIF_PALSUM)
    {
    // use debug command 0x32 to get strength of 2nd fm amplitude
    // use debug command 0x3D to get strength of 2nd fm NSR by Allan.Liang
        HAL_SIF_WriteByte(AU_CMD_DEBUG, 0x3D);
        MsOS_DelayTask(5);

       data[1] = (MS_U8)HAL_SIF_ReadByte(REG_MB_SE_ACK1);
       data[0] = (MS_U8)HAL_SIF_ReadByte(REG_MB_SE_ACK2);

        *pLevel =   (((MS_U32)data[1])<<8)  | data[0];
        return (TRUE);
    }
    else if (u8codeTypeDSP == AU_SIF_BTSC)
    {
        HAL_SIF_WriteByte(AU_CMD_DEBUG, 0x87);
        MsOS_DelayTask(5);
        data[1] = (MS_U8)HAL_SIF_ReadByte(AU_STATUS_DBG_H);
        data[0] = (MS_U8)HAL_SIF_ReadByte(AU_STATUS_DBG_M);
        *pLevel =   (((MS_U32)data[1])<<8)  | data[0];
        return (TRUE);
    }
    return (FALSE);
}

void MDrv_MAD_SIF_GetCurAnalogMode(MS_U8* pAlgMode)
{
    MS_U8 mode=0,val_mode1, val_mode2;
    MS_U8  sif_standard=HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xFF;
    MS_U8 u8codeTypeDSP = HAL_SIF_GetDspCodeType();

    if (u8codeTypeDSP == AU_SIF_BTSC)
    {
        val_mode1 = HAL_SIF_ReadByte(AU_STATUS_MODE1);
        if((val_mode1&0x07)==0x07)
            mode = ATV_GET_NTSC_BTSC_SAP_STEREO;
        else if((val_mode1&0x07)==0x03)
            mode = ATV_GET_NTSC_BTSC_STEREO;
        else if ((val_mode1&0x07)==0x05)
            mode = ATV_GET_NTSC_BTSC_SAP_MONO;
        else
            mode = ATV_GET_NTSC_BTSC_MONO; // for LGE spec, no singal it need to white noise
    }

    else if (u8SifStandardType == ADEC_SIF_NICAM) {
        val_mode2 = HAL_SIF_ReadByte(AU_STATUS_MODE2);
        if((val_mode2&0x70)==0x10)
            mode = ATV_GET_PAL_NICAM_MONO;
        else if((val_mode2&0x70)==0x20)
            mode = ATV_GET_PAL_NICAM_STEREO;
        else if ((val_mode2&0x70)==0x30)
            mode = ATV_GET_PAL_NICAM_DUAL;
        else
            mode = ATV_GET_PAL_MONO;
    }
    else if ((u8SifStandardType == ADEC_SIF_A2)&&(sif_standard!=0x23)) {/*한국향 stereo,dual 안 됨 수정  2008.10.29 change from (uc2D20!=0x03)to (uc2D20!=0x23) */
        val_mode1 = HAL_SIF_ReadByte(AU_STATUS_MODE1);
        if((val_mode1&0x3B)==0x3B)
            mode = ATV_GET_PAL_STEREO;
        else if((val_mode1&0x3D)==0x3D)
            mode = ATV_GET_PAL_DUAL;
        else
            mode = ATV_GET_PAL_MONO;
    }
    else if ((u8SifStandardType == ADEC_SIF_A2)&&(sif_standard==0x23)) {/*한국향 stereo,dual 안 됨 수정  2008.10.29 change from (uc2D20!=0x03)to (uc2D20!=0x23) */
        val_mode1 = HAL_SIF_ReadByte(AU_STATUS_MODE1);
        if((val_mode1&0x3B)==0x3B)
            mode = ATV_GET_NTSC_A2_STEREO;
        else if((val_mode1&0x3D)==0x3D)
            mode = ATV_GET_NTSC_A2_SAP;
        else
            mode = ATV_GET_NTSC_A2_MONO;
    }
    else // for ADEC_SIF_FM
            mode = ATV_GET_PAL_MONO;

    *pAlgMode = mode;
}


MS_U16 MDrv_MAD_SIF_SetUserAnalogMode(MS_U16 AlgMode)
{
    SIF_DBGMSG(printf("%s is called.Set Analog Mode = 0x%x\r\n", __FUNCTION__, AlgMode));

    switch(AlgMode){
        case ATV_SET_PAL_MONO:
            if (u8SifStandardType == ADEC_SIF_NICAM)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE2,0x01);
                HAL_SIF_WriteByte(AU_CMD_MODE1,0x00);
            }
            else // (u8SifStandardType == ADEC_SIF_A2) or (u8SifStandardType == ADEC_SIF_FM)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE2,0x01);
                HAL_SIF_WriteByte(AU_CMD_MODE1,0x00);// uese the force mode by Allan.Liang
            }
            SIF_DBGMSG(printf("[SetUserAnalogMode:SET_MONO]\n"));
            break;
        case ATV_SET_PAL_MONO_FORCED:
            if (u8SifStandardType == ADEC_SIF_NICAM)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE2,0x01);
                HAL_SIF_WriteByte(AU_CMD_MODE1,0x00);
            }
            else // (u8SifStandardType == ADEC_SIF_A2) or (u8SifStandardType == ADEC_SIF_FM)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE2,0x01);
                HAL_SIF_WriteByte(AU_CMD_MODE1,0x00);
            }
            SIF_DBGMSG(printf("[SetUserAnalogMode:SET_MONO]\n"));
            break;
        case ATV_SET_PAL_STEREO:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x01);// uese the force mode by Allan.Liang
            SIF_DBGMSG(printf("[SetUserAnalogMode:SET_STEREO]\n"));
            break;
        case ATV_SET_PAL_STEREO_FORCED:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x00);
            break;
        case ATV_SET_PAL_DUALI:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x00);
            break;
        case ATV_SET_PAL_DUALII:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x02);
            break;
        case ATV_SET_PAL_DUALI_II:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x03);
            break;
        case ATV_SET_PAL_NICAM_MONO:
            HAL_SIF_WriteByte(AU_CMD_MODE2, 0x08);
            break;
        case ATV_SET_PAL_NICAM_MONO_FORCED:
            HAL_SIF_WriteByte(AU_CMD_MODE2, 0x01);
            break;
        case ATV_SET_PAL_NICAM_STEREO:
            HAL_SIF_WriteByte(AU_CMD_MODE2, 0x02);
            break;
        case ATV_SET_PAL_NICAM_STEREO_FORCED:
            HAL_SIF_WriteByte(AU_CMD_MODE2, 0x01);
            break;
        case ATV_SET_PAL_NICAM_DUALI:
            HAL_SIF_WriteByte(AU_CMD_MODE2, 0x06);
            break;
        case ATV_SET_PAL_NICAM_DUALII:
            HAL_SIF_WriteByte(AU_CMD_MODE2, 0x07);
            break;
        case ATV_SET_PAL_NICAM_DUALI_II:
            HAL_SIF_WriteByte(AU_CMD_MODE2, 0x05);
            break;
        case ATV_SET_PAL_NICAM_DUAL_FORCED:
            HAL_SIF_WriteByte(AU_CMD_MODE2, 0x01);
            break;
         case ATV_SET_NTSC_A2_MONO:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x00);
            break;
         case ATV_SET_NTSC_A2_STEREO:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x01);
            break;
         case ATV_SET_NTSC_A2_SAP:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x02);
            break;
         case ATV_SET_NTSC_BTSC_MONO:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x00);
            break;
         case ATV_SET_NTSC_BTSC_STEREO:
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x01);
            break;
         case ATV_SET_NTSC_BTSC_SAP_MONO:   //need to check what is SAP Mono
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x02);
            break;
         case ATV_SET_NTSC_BTSC_SAP_STEREO: // need to check what is SAP Stereo
            HAL_SIF_WriteByte(AU_CMD_MODE1, 0x02);
            break;

          default:
              SIF_DBGMSG(printf("[SetUserAnalogMode]:ERROR!!\n"));
              break;
        }

	return 0;
}

MS_BOOL MDrv_MAD_SIF_SetModeSetup(MS_U8 SystemType)
{
    MS_U8 sifStandardCmd;
    static MS_U8 sifOldStandardCmd = -1;

    // fix me: set to driver
    if (SystemType == ADEC_SIF_BG_NICAM)
    {
        sifStandardCmd = 0x48;
        u8SifStandardType = ADEC_SIF_NICAM;
    }
    else if (SystemType == ADEC_SIF_BG_FM)
    {
        if (hidevEnabled)
            sifStandardCmd = 0x14;
        else
        sifStandardCmd = 0x04;
        u8SifStandardType = ADEC_SIF_FM;
    }
    else if (SystemType == ADEC_SIF_BG_A2) {
        sifStandardCmd = 0x24;
        u8SifStandardType = ADEC_SIF_A2;
    }
     else if (SystemType == ADEC_SIF_I_NICAM) {
    	sifStandardCmd = 0x4A;
        u8SifStandardType = ADEC_SIF_NICAM;
    }
    else if (SystemType == ADEC_SIF_I_FM) {
        if (hidevEnabled)
            sifStandardCmd = 0x1A;
        else
        sifStandardCmd = 0x4A;
        u8SifStandardType = ADEC_SIF_FM;
    }
      else if (SystemType == ADEC_SIF_DK_NICAM)   {
        sifStandardCmd = 0x49;
        u8SifStandardType = ADEC_SIF_NICAM;
    }
    else if (SystemType == ADEC_SIF_DK_FM)  {
        if (hidevEnabled)
            sifStandardCmd = 0x15;
        else
        sifStandardCmd = 0x05;
        u8SifStandardType = ADEC_SIF_FM;
    }
    else if (SystemType == ADEC_SIF_DK1_A2) {
        sifStandardCmd = 0x25;
        u8SifStandardType = ADEC_SIF_A2;
    }
    else if (SystemType == ADEC_SIF_DK2_A2) {
        sifStandardCmd = 0x26;
        u8SifStandardType = ADEC_SIF_A2;
    }
    else if (SystemType == ADEC_SIF_DK3_A2) {
        sifStandardCmd = 0x27;
        u8SifStandardType = ADEC_SIF_A2;
    }
    else if ((SystemType == ADEC_SIF_L_NICAM) || (SystemType == ADEC_SIF_L_AM)) {
        sifStandardCmd = 0x4B;
        u8SifStandardType = ADEC_SIF_NICAM;
    }
    else if (SystemType == ADEC_SIF_MN_A2)  {
        sifStandardCmd = 0x23;
        u8SifStandardType = ADEC_SIF_A2;
    }
        else if (SystemType == ADEC_SIF_MN_BTSC)    {
        sifStandardCmd = 0x01;
        u8SifStandardType = ADEC_SIF_MN_BTSC;
    }
    else
    {
        return FALSE;
    }

    if(sifStandardCmd != sifOldStandardCmd )
    {
    	HAL_SIF_WriteMaskByte(AU_CMD_STANDARD, 0xFF, 0);

    	MsOS_DelayTask(1);
    }

    sifOldStandardCmd = sifStandardCmd;
    HAL_SIF_WriteMaskByte(AU_CMD_STANDARD, 0xFF, (MS_U16)sifStandardCmd);

    if ((SystemType == ADEC_SIF_L_NICAM) || (SystemType == ADEC_SIF_L_AM)){
        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x01, 0x00);//disable the SIF AGC by Allan Liang
        MsOS_DelayTask(10);
        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x02, 0x02);//reset the SIF AGC by Allanliang
        MsOS_DelayTask(10);
        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x02, 0x00);//reset the SIF AGC by Allanliang
        //MAD_DEBUG_P2(printk("######MDrv_MAD_SIF_SetModeSetup AGL L: type(%x) \r\n",SystemType));

    }
    else{
    	HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x01, 0x01 );//enable the SIF AGC by Allan Liang
        //MAD_DEBUG_P2(printk("######MDrv_MAD_SIF_SetModeSetup AGC: type(%x)  \r\n",SystemType));
    }

    return TRUE;
}

MS_BOOL MDrv_MAD_SIF_SetHDEVMode(MS_U8 bOnOff)
{
    hidevEnabled = bOnOff;
    MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_HIDEV, hidevEnabled, NULL);
    return TRUE;
}


MS_BOOL MDrv_MAD_SIF_GetBandDetect(MS_U8 soundSystem, MS_U32 *pbandStrength)
{
    MS_U8  sifStandardCmd;
    MS_U8  data[3];

    // fix me: get from driver
    *pbandStrength = 0; // initial strength to 0.

    if (soundSystem == ADEC_SIF_SYSTEM_BG) sifStandardCmd = 0x04;
    else if (soundSystem == ADEC_SIF_SYSTEM_DK) sifStandardCmd = 0x05;
    else if (soundSystem == ADEC_SIF_SYSTEM_I) sifStandardCmd = 0x0A;
    else if (soundSystem == ADEC_SIF_SYSTEM_MN) sifStandardCmd = 0x03;
    else return (FALSE);

    HAL_SIF_WriteMaskByte(AU_CMD_STANDARD, 0xEF, (MS_U16)sifStandardCmd);
#if 1 // LGE ::  Sound System detection is sometimes wrong for AUS. Jonghyuk, Lee 090216
	MsOS_DelayTask(300); // Richard.Ni change from 100 to 300 for test
#endif

    HAL_SIF_WriteByte(AU_CMD_DEBUG, 0x3C); //to used the 1/NSR value replace the amplitude
#if 1 // LGE ::  Sound System detection is sometimes wrong for AUS. Jonghyuk, Lee 090216
    MsOS_DelayTask(200/*100*/);        // 200ms
#endif

    data[2] = (MS_U8)HAL_SIF_ReadByte(AU_STATUS_DBG_H);
    data[1] = (MS_U8)HAL_SIF_ReadByte(AU_STATUS_DBG_M);
    data[0] = (MS_U8)HAL_SIF_ReadByte(AU_STATUS_DBG_L);
    *pbandStrength = ((((MS_U32)data[2])<<16)  |  (((MS_U32)data[1])<<8)  | data[0]);
    return (TRUE);
}


MS_BOOL MDrv_MAD_SIF_GetBandDetect_Amp(MS_U8 soundSystem, MS_U32 *pbandStrength)
{
    MS_U8	sifStandardCmd;
    MS_U8	data[3];

    // fix me: get from driver
    *pbandStrength = 0; // initial strength to 0.

    if (soundSystem == ADEC_SIF_SYSTEM_BG)
    {
        sifStandardCmd = 0x04;
    }
    else if (soundSystem == ADEC_SIF_SYSTEM_DK)
    {
        sifStandardCmd = 0x05;
    }
    else if (soundSystem == ADEC_SIF_SYSTEM_I)
    {
        sifStandardCmd = 0x0A;
    }
    else if (soundSystem == ADEC_SIF_SYSTEM_MN)
    {
        sifStandardCmd = 0x03;
    }
    else
    {
        return (FALSE);
    }

    HAL_SIF_WriteMaskByte(AU_CMD_STANDARD, 0xEF, (MS_U16)sifStandardCmd);
    MsOS_DelayTask(300); // Richard.Ni change from 100 to 300 for test

    HAL_SIF_WriteByte(AU_CMD_DEBUG, 0x45);
    MsOS_DelayTask(200/*100*/);		// 200ms

    if (HAL_SIF_GetDspCodeType()!= AU_SIF_PALSUM)
    {
        printf("[AUDIO UTOPIA SIF] DSP type is not SIF, need reload SIF again !!! \r\n");
    }

    data[2] = (MS_U8)HAL_SIF_ReadByte(AU_STATUS_DBG_H);
    data[1] = (MS_U8)HAL_SIF_ReadByte(AU_STATUS_DBG_M);
    data[0] = (MS_U8)HAL_SIF_ReadByte(AU_STATUS_DBG_L);
    *pbandStrength = ((((MS_U32)data[2])<<16)  |  (((MS_U32)data[1])<<8)  | data[0]);
    printf("[AUDIO UTOPIA SIF] soundSystem(%d), *pbandStrength(0x%lx) \r\n", soundSystem, *pbandStrength);

    return (TRUE);
}

MS_BOOL MDrv_MAD_SIF_SetBandSetup(MS_U8 SifBand)
{
    MS_U8  sifStandardCmd;

    // fix me: set to driver
    if (SifBand == ADEC_SIF_SYSTEM_BG) {
        sifStandardCmd = 0x04;
        u8SifSoundSystemType = ADEC_SIF_SYSTEM_BG;
    }
    else if (SifBand == ADEC_SIF_SYSTEM_DK) {
        sifStandardCmd = 0x05;
        u8SifSoundSystemType = ADEC_SIF_SYSTEM_DK;
    }
    else if (SifBand == ADEC_SIF_SYSTEM_I) {
        sifStandardCmd = 0x0A;
        u8SifSoundSystemType = ADEC_SIF_SYSTEM_I;
    }
    else if (SifBand == ADEC_SIF_SYSTEM_L) {
        sifStandardCmd = 0x04B;
        u8SifSoundSystemType = ADEC_SIF_SYSTEM_L;
    }
    else if (SifBand == ADEC_SIF_SYSTEM_MN) {
        sifStandardCmd = 0x03;
        u8SifSoundSystemType = ADEC_SIF_SYSTEM_MN;
    }
    else return (FALSE);

    HAL_SIF_WriteMaskByte(AU_CMD_STANDARD, 0xFF, sifStandardCmd);

    if (SifBand == ADEC_SIF_SYSTEM_L)
    {
        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x01, 0x00);//disable the SIF AGC by Allan Liang

#ifdef MSOS_TYPE_LINUX_KERNEL
        msleep(10);
#else
        MsOS_DelayTask(10);
#endif

        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x02, 0x02 );//reset the SIF AGC by Allanliang

#ifdef MSOS_TYPE_LINUX_KERNEL
        msleep(10);
#else
        MsOS_DelayTask(10);
#endif

        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x02, 0x00 );//reset the SIF AGC by Allanliang
    }
    else
    {
        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x01, 0x01 );//enable the SIF AGC by Allan Liang
    }

    return (TRUE) ;
}

MS_BOOL MDrv_MAD_SIF_CheckAvailableSystem(MS_U8 standard, MS_U8 *exist)
{
    MS_U8 reg_standard=0, reg_mode1=0, reg_mode2=0; // register buffer
    MS_U8 sifStandardCmd;
    MS_U8 u8codeTypeDSP = HAL_SIF_GetDspCodeType();
    HAL_SIF_WriteMaskByte(AU_CMD_FC_TRACK, 0x80, 0x80); // enable the DK real-time monitor //CHECK
    *exist  = ADEC_SIF_ABSENT;

    reg_standard  = HAL_SIF_ReadByte(AU_CMD_STANDARD);
    sifStandardCmd = reg_standard & 0x0F;
    if (u8codeTypeDSP == AU_SIF_PALSUM)
    {
        if ((sifStandardCmd < 0x03) || (sifStandardCmd > 0xB))
            return FALSE;
    }
    else return FALSE;

    reg_mode1 = HAL_SIF_ReadByte(AU_STATUS_MODE1);//CHECK
    reg_mode2 = HAL_SIF_ReadByte(AU_STATUS_MODE2);//CHECK

    if (standard == ADEC_SIF_NICAM)
    {
        if ((reg_mode2&0x0f) == 0x05)
        {
            *exist  = ADEC_SIF_PRESENT;
        }
    }
    else if (standard == ADEC_SIF_A2)
    {
        if (((reg_mode1&0x3B) == 0x3B) || ((reg_mode1&0x3D) == 0x3D))
        {
            *exist  = ADEC_SIF_PRESENT;
        }
    }
    else if (standard == ADEC_SIF_FM)
    {
        //By Jonghyuk, LEE 080910:: To avoid FM mode setting even if the system is not FM.
        //Need refine
        if((((reg_mode1&0x10) == 0x10) && (((reg_mode1&0x3B) != 0x3B) && ((reg_mode1&0x3D) != 0x3D) && ((reg_mode2&0x0f) != 0x05)))
        ||(((reg_mode1&0xFF) == 0x01) && ((reg_mode2&0x0f) != 0x05)))
        {
            *exist  = ADEC_SIF_PRESENT;
        }
    }

    // fix me: check if standard is avaliable and return to *availability
    return(TRUE);
}

MS_BOOL MDrv_MAD_SIF_CheckA2DK(MS_U8 SystemType, MS_U8 *exist)
{
    MS_U8 reg_standard=0, reg_mode1=0, reg_mode2=0; // register buffer
    MS_U8 sifStandardCmd;
    MS_U8 u8codeTypeDSP = HAL_SIF_GetDspCodeType();

    *exist  = ADEC_SIF_ABSENT;
    HAL_SIF_WriteMaskByte(AU_CMD_FC_TRACK, 0x80, 0x80); // enable the DK real-time monitor //CHECK

    reg_standard  = HAL_SIF_ReadByte(AU_CMD_STANDARD);
    sifStandardCmd = reg_standard & 0x0F;
    if (u8codeTypeDSP == AU_SIF_PALSUM) {
        if ((sifStandardCmd < 0x05) || (sifStandardCmd > 9))
            return FALSE;
    }
    else return FALSE;

    reg_mode1 = HAL_SIF_ReadByte(AU_STATUS_MODE1);//CHECK
    reg_mode2 = HAL_SIF_ReadByte(AU_STATUS_MODE2);//CHECK
#if 1	// 080925 Need to check DK3 A2. So, disable check for other mode. By LGE Jonghyuk, Lee
    if (SystemType == ADEC_SIF_DK_NICAM) {
        if ((reg_mode2&0x0f) == 0x05)
            *exist  = ADEC_SIF_PRESENT;
    }
    else if (SystemType == ADEC_SIF_DK1_A2) {
        if (((reg_mode1&0xFB) == 0x3B) || ((reg_mode1&0xFD) == 0x3D)) /* 09.23 chnageed by mstar*/
            *exist  = ADEC_SIF_PRESENT;
    }
    else if (SystemType == ADEC_SIF_DK2_A2) {
        if (((reg_mode1&0xFB) == 0x7B) || ((reg_mode1&0xFD) == 0x7D))
            *exist  = ADEC_SIF_PRESENT;
    }
    else if (SystemType == ADEC_SIF_DK3_A2) {
        if (((reg_mode1&0xFB) == 0xBB) || ((reg_mode1&0xFD) == 0xBD))// update the value for DK3 carrier report 090316
            *exist  = ADEC_SIF_PRESENT;
    }
    else if (SystemType == ADEC_SIF_DK_FM) {
        if(reg_mode1&0x10)
            *exist  = ADEC_SIF_PRESENT;
    }
#else
	if (SystemType == ADEC_SIF_DK3_A2) {
        if (((uc2D44&0xFB) == 0xBB) || ((uc2D44&0xFD) == 0xBD))// update the value for DK3 carrier report 090316
            *exist  = ADEC_SIF_PRESENT;
	}
#endif

    // fix me: check if standard is avaliable and return to *availability
    return(TRUE);
}

void MDrv_MAD_CheckNicamDigital(MS_U8 *isNicamDetect)
{
    MS_U8 reg_mode2=0; // register buffer

    *isNicamDetect = ADEC_SIF_ABSENT;

    reg_mode2 = HAL_SIF_ReadByte(AU_STATUS_MODE2);//CHECK
    if ((reg_mode2&0x0f) == 0x05)
        *isNicamDetect  = ADEC_SIF_PRESENT;
}

MS_BOOL MDrv_MAD_SIF_GetSoundStandard(MS_U8 *B_SifStandard)
{
    MS_U8 sifStandard;
    MS_U8 reg_mode1=0, reg_mode2=0; // register buffer
    MS_U8 curr_sound_sys, ucCurStatus = 0x00; // current status buffer

    HAL_SIF_WriteByte(AU_CMD_STANDARD, 0x00);
    MsOS_DelayTask(1);
    HAL_SIF_WriteMaskByte(AU_CMD_STANDARD, 0xC0, 0xC0);
    MsOS_DelayTask(800);
    if( 0x80 == ( sifStandard = HAL_SIF_ReadByte(AU_STATUS_STANDARD)) )
    {
        //MAD_DEBUG_P2(printk("Audio DSP Busy, %x\n", sifStandard));
        return FALSE;
    }
    //MAD_DEBUG_P2(printk("mode standard: %x\n", sifStandard));

    reg_mode1 = HAL_SIF_ReadByte(AU_STATUS_MODE1);//CHECK
    reg_mode2 = HAL_SIF_ReadByte(AU_STATUS_MODE2);//CHECK
    curr_sound_sys = sifStandard & 0xFF; //MAdp_MAD_ReadReg(0x2D20)&0x0F;
    if (_bit4_(reg_mode1))
        ucCurStatus |= IS_MAIN_CARRIER;

    if (curr_sound_sys != AU_SYS_I_NICAM){
        if(_bit5_(reg_mode1) )
        ucCurStatus |= IS_A2;
    }

    if ((reg_mode2&0x0f) == 0x05)
        ucCurStatus |= IS_NICAM;

    else if (curr_sound_sys == 0x03){
        sifStandard = ADEC_SIF_MN_A2;   // palsum only support M_A2, no BTSC.
    }

    if (curr_sound_sys == 0x04) {
        if (ucCurStatus & IS_A2)
            sifStandard = ADEC_SIF_BG_A2;
        else sifStandard = ADEC_SIF_BG_FM;
    }
    else if (curr_sound_sys == 0x08)
        sifStandard = ADEC_SIF_BG_NICAM;
    else if (curr_sound_sys == 0x09)
        sifStandard = ADEC_SIF_DK_NICAM;
    else if (curr_sound_sys == 0x05) {
        if (ucCurStatus & IS_A2)
            sifStandard = ADEC_SIF_DK1_A2;
        else sifStandard = ADEC_SIF_DK_FM;
    }
    else if (curr_sound_sys == 0x06){
        sifStandard = ADEC_SIF_DK2_A2;
    }
    else if (curr_sound_sys == 0x07){
        sifStandard = ADEC_SIF_DK3_A2;
    }
    else if (curr_sound_sys == 0x0A){
        if (ucCurStatus & IS_NICAM)
            sifStandard = ADEC_SIF_I_NICAM;
        else sifStandard = ADEC_SIF_I_FM;
    }

    *B_SifStandard = sifStandard;
    return( TRUE ) ;

}


MS_BOOL MDrv_MAD_SIF_RedoSetStandard(void)
{
    MS_U8   ori_val;

    ori_val = HAL_SIF_ReadByte(AU_CMD_STANDARD);
    HAL_SIF_WriteByte(AU_CMD_STANDARD, 0);                             // clear cmd
    MsOS_DelayTask(1);
    HAL_SIF_WriteByte(AU_CMD_STANDARD, ori_val);
    MsOS_DelayTask(1);
    return TRUE;
}

MS_BOOL MDrv_MAD_SetBtscA2ThresholdLevel(MS_U16 thresholdLevel, MS_U16 *pRF2NSR_map)
{
    MS_U8 u8codeTypeDSP = HAL_SIF_GetDspCodeType();

    MS_DEBUG_MSG(printf("\r\n------ MDrv_MAD_SetBtscA2ThresholdLevel is------%x",thresholdLevel));

    if (u8codeTypeDSP == AU_SIF_PALSUM)
    {
        //MDrv_MAD_SIF_AccessThreshold(WRITE_THRESHOLD|A2_BG_STANDARD, A2_PILOT_ON_AMP, (MS_U16*)(pRF2NSR_map + thresholdLevel));
        //MDrv_MAD_SIF_AccessThreshold(WRITE_THRESHOLD|A2_DK_STANDARD, A2_PILOT_ON_AMP, (MS_U16*)(&thresholdLevel));
        MDrv_MAD_SIF_AccessThreshold(WRITE_THRESHOLD|A2_M_STANDARD, CARRIER2_ON_NSR, (MS_U16*)(pRF2NSR_map + ((thresholdLevel+Thr_C2ONOFF_Gap)>39?39:(thresholdLevel+Thr_C2ONOFF_Gap))));// modified for 40 step //CHECK
        MDrv_MAD_SIF_AccessThreshold(WRITE_THRESHOLD|A2_M_STANDARD, CARRIER2_OFF_NSR, (MS_U16*)(pRF2NSR_map + thresholdLevel));//CHECK
        MDrv_MAD_SIF_RedoSetStandard();
        return (TRUE);
    }
    else if (u8codeTypeDSP == AU_SIF_BTSC)
    {
        //MDrv_MAD_SIF_AccessThreshold(WRITE_THRESHOLD,   BTSC_PILOT_AMPLITUDE_ON, (U16*)(&BTSC_Pilot_map[thresholdLevel]));//CHECK
        //MDrv_MAD_SIF_AccessThreshold(WRITE_THRESHOLD,   BTSC_PILOT_AMPLITUDE_OFF, (U16*)(&BTSC_Pilot_map[(thresholdLevel+Thr_BTSC_PilotONOFF_Gap)>31?31:(thresholdLevel+Thr_BTSC_PilotONOFF_Gap)]));//CHECK
        return (TRUE);
    }
    return (FALSE);
}

MS_BOOL MDrv_MAD_SIF_AccessThreshold(AUDIO_SIF_SYSTEM_TYPE rw_standard_type, AUDIO_SIF_THRESHOLD_TYPE  u8Threshold_type, MS_U16 *u16Value)
{
    MS_BOOL  writethd;
    MS_U16 standard_type;
    MS_U8   SIF_DSP_type;
    writethd = HINIBBLE(rw_standard_type) ;
    standard_type = LONIBBLE(rw_standard_type) ;
    SIF_DSP_type=HAL_SIF_GetDspCodeType();

    if (SIF_DSP_type==AU_SIF_BTSC)
    {
        if (writethd)
        {
            HAL_MAD2_Write_DSP_sram((MS_U16)c_au_ntsc_sys_threshold[0][u8Threshold_type],((MS_U32) *u16Value)<<8, DSP_MEM_TYPE_PM);
            return TRUE;
        }
        else
        {
            return (HAL_MAD2_Read_DSP_sram((MS_U16)c_au_ntsc_sys_threshold[0][u8Threshold_type],DSP_MEM_TYPE_PM));
        }

    }
    else
    {
        if (writethd)
        {
            HAL_MAD2_Write_DSP_sram((MS_U16)c_au_pal_sys_threshold[standard_type][u8Threshold_type],((MS_U32) *u16Value)<<8, DSP_MEM_TYPE_PM);
            return TRUE;
        }
        else
        {
            return (HAL_MAD2_Read_DSP_sram((MS_U16)c_au_pal_sys_threshold[standard_type][u8Threshold_type],DSP_MEM_TYPE_PM));
        }

    }
}

void MDrv_MAD_SIF_ADC_Reset(void)
{
    //HAL_SIF_ADC_Reset();
}

