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
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   dmx.c
/// @brief  Demux (DMX) API Interface
/// @author MStar Semiconductor,Inc.
/// @attention
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsTypes.h"
//#include <stdio.h>
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "apiAUDIO.h"
#include "apiAUDIO_v2.h"
#include "apiAUDIO_private.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

MS_U32 u32Gpid = 0;

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define AUDIO_MSG(_f, _a...)                             //printf("\033[1;32m"_f"\033[0m", ##_a)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
enum
{
    AUDIO_POOL_ID_AUDIO0,
} eAUDIOPoolID;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

#ifndef UTOPIA_STRIP
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _AUDIOProbeResourceToUtopia(AUDIO_INSTANT_PRIVATE *_pInstantAudio)
{
    //============================================================
    // AUDIO_SYSTEM RELATIONAL API FUNCTION
    //============================================================
    _pInstantAudio->fpAUDIOInitialize =                                 _MApi_AUDIO_Initialize;
    _pInstantAudio->fpAUDIOSetPowerOn =                                 _MApi_AUDIO_SetPowerOn;
    _pInstantAudio->fpAUDIOSetDSPBaseAddr =                             _MApi_AUDIO_SetDspBaseAddr;
    _pInstantAudio->fpAUDIOGetDSPMadBaseAddr =                          _MApi_AUDIO_GetDspMadBaseAddr;
    _pInstantAudio->fpAUDIOWriteDecMailBox =                            _MApi_AUDIO_WriteDecMailBox;
    _pInstantAudio->fpAUDIOTriggerSifPLL =                              _MApi_AUDIO_TriggerSifPLL;
    _pInstantAudio->fpAUDIOMonitor =                                    _MApi_Audio_Monitor;
    _pInstantAudio->fpAUDIOSetSystemInfo =                              _MApi_AUDIO_SetSystemInfo;
    _pInstantAudio->fpAUDIOSetOutConnectivity =                         _MApi_AUDIO_SetOutConnectivity;
    _pInstantAudio->fpAUDIOSetPathInfo =                                _MApi_AUDIO_SetPathInfo;
    _pInstantAudio->fpAUDIOSetOutputInfo =                              _MApi_AUDIO_SetOutputInfo;
    _pInstantAudio->fpAUDIOSetSourceInfo =                              _MApi_AUDIO_SetSourceInfo;
    _pInstantAudio->fpAUDIOInputSwitch =                                _MApi_AUDIO_InputSwitch;
    _pInstantAudio->fpAUDIOSetDataCaptureSource =                       _MApi_AUDIO_SetDataCaptureSource;
    _pInstantAudio->fpAUDIOSetOutputSourceInfo =                        _MApi_AUDIO_SetOutputSourceInfo;
    _pInstantAudio->fpAUDIOGetOutputSourceInfo =                        _MApi_AUDIO_GetOutputSourceInfo;
    _pInstantAudio->fpAUDIOReadMailBox =                                _MApi_AUDIO_ReadMailBox;
    _pInstantAudio->fpAUDIOWriteMailBox =                               _MApi_AUDIO_WriteMailBox;
    _pInstantAudio->fpAUDIOFwTriggerDsp =                               _MApi_AUDIO_FwTriggerDSP;
    _pInstantAudio->fpAUDIOSetFwStatus =                                _MApi_AUDIO_SetFwStatus;
    _pInstantAudio->fpAUDIOExitAudioSystem =                            _MApi_AUDIO_ExitAudioSystem;
    _pInstantAudio->fpAUDIORebootDSP =                                  _MApi_AUDIO_RebootDsp;
    _pInstantAudio->fpAUDIOSendIntrupt =                                _MApi_AUDIO_SendIntrupt;
    _pInstantAudio->fpAUDIOI2SSetMode =                                 _MApi_AUDIO_I2S_SetMode;
    _pInstantAudio->fpAUDIOWritePreInitTable =                          _MApi_AUDIO_WritePreInitTable;
    _pInstantAudio->fpAUDIOEnaEarphoneLowPowerStage =                   _MApi_AUDIO_EnaEarphone_LowPower_Stage;
    _pInstantAudio->fpAUDIOEnaEarphoneHighDrivingStage =                _MApi_AUDIO_EnaEarphone_HighDriving_Stage;

    //=============================================================
    // AUDIO_SOUND RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpSNDProcessEnable =                                _MApi_SND_ProcessEnable;
    _pInstantAudio->fpSNDSetParam1 =                                    _MApi_SND_SetParam1;
    _pInstantAudio->fpSNDGetParam1 =                                    _MApi_SND_GetParam1;
    _pInstantAudio->fpAUDIOSoundSetParam =                              _MApi_AUDIO_SOUND_SetParam;
    _pInstantAudio->fpAUDIOSetAbsoluteVolume =                          _MApi_AUDIO_SetAbsoluteVolume;
    //_pInstantAudio->fpAUDIOSetPreScale =                                _MApi_AUDIO_SetPreScale;
    //_pInstantAudio->fpAUDIOSetMute =                                    _MApi_AUDIO_SetMute;
    _pInstantAudio->fpAUDIOSourceConnect =                              _MApi_AUDIO_SourceConnect;
    //_pInstantAudio->fpAUDIOSetBalance =                                 _MApi_AUDIO_SetBalance;
    //_pInstantAudio->fpAUDIOEnableEQ =                                   _MApi_AUDIO_EnableEQ;
    //_pInstantAudio->fpAUDIOEnableTone =                                 _MApi_AUDIO_EnableTone;
    //_pInstantAudio->fpAUDIOEnableAutoVolume =                           _MApi_AUDIO_EnableAutoVolume;
    //_pInstantAudio->fpAUDIOEnableSurround =                             _MApi_AUDIO_EnableSurround;
    //_pInstantAudio->fpAUDIOEnableBalance =                              _MApi_AUDIO_EnableBalance;
    _pInstantAudio->fpAUDIOSetDynamicBass =                             _MApi_AUDIO_SetDynamicBass;
    //_pInstantAudio->fpAUDIOSetBass =                                    _MApi_AUDIO_SetBass;
    //_pInstantAudio->fpAUDIOAbsoluteBass =                               _MApi_AUDIO_AbsoluteBass;
    //_pInstantAudio->fpAUDIOSetTreble =                                  _MApi_AUDIO_SetTreble;
    _pInstantAudio->fpAUDIOAbsoluteTreble =                             _MApi_AUDIO_AbsoluteTreble;
    //_pInstantAudio->fpAUDIOSetEq =                                      _MApi_AUDIO_SetEq;
    //_pInstantAudio->fpAUDIOSetEq7band =                                 _MApi_AUDIO_SetEq_7band;
    //_pInstantAudio->fpAUDIOSetAvcThreshold =                            _MApi_AUDIO_SetAvcThreshold;
    //_pInstantAudio->fpAUDIOSetAvcMode =                                 _MApi_AUDIO_SetAvcMode;
    //_pInstantAudio->fpAUDIOSetAvcAT =                                   _MApi_AUDIO_SetAvcAT;
    //_pInstantAudio->fpAUDIOSetAvcRT =                                   _MApi_AUDIO_SetAvcRT;
    _pInstantAudio->fpAUDIOSetBufferProcess =                           _MApi_AUDIO_SetBufferProcess;
    //_pInstantAudio->fpAUDIOSetSurroundXA =                              _MApi_AUDIO_SetSurroundXA;
    //_pInstantAudio->fpAUDIOSetSurroundXB =                              _MApi_AUDIO_SetSurroundXB;
    //_pInstantAudio->fpAUDIOSetSurroundXK =                              _MApi_AUDIO_SetSurroundXK;
    //_pInstantAudio->fpAUDIOSetSurroundLPFGain =                         _MApi_AUDIO_SetSurroundLPFGain;
    _pInstantAudio->fpAUDIOConvertVolumeUnit =                          _MApi_AUDIO_ConvertVolumeUnit;
    _pInstantAudio->fpAUDIOSetPEQCoef =                                 _MApi_AUDIO_SetPEQCoef;
    //_pInstantAudio->fpAUDIOEnablePEQ =                                  _MApi_AUDIO_EnablePEQ;
    _pInstantAudio->fpAUDIOEnableDcRemove =                             _MApi_AUDIO_EnableDcRemove;

    //============================================================
    // AUDIO_DTV RELATIONAL API FUNCTION
    //============================================================
    _pInstantAudio->fpAUDIOReleaseDecodeSystem =                        _MApi_AUDIO_ReleaseDecodeSystem;
    _pInstantAudio->fpAUDIOSetDecodeSystem =                            _MApi_AUDIO_SetDecodeSystem;
    _pInstantAudio->fpAUDIOGetDecodeSystem =                            _MApi_AUDIO_GetDecodeSystem;
    _pInstantAudio->fpAUDIOSetDecodeCmd =                               _MApi_AUDIO_SetDecodeCmd;
    _pInstantAudio->fpAUDIOOpenDecodeSystem =                           _MApi_AUDIO_OpenDecodeSystem;
    _pInstantAudio->fpAUDIOSetSystem =                                  _MApi_AUDIO_SetSystem;
    _pInstantAudio->fpAUDIOGetDecSysSupportStatus =                     _MApi_AUDIO_GetDecSysSupportStatus;
    _pInstantAudio->fpAUDIOIsMadLock =                                  _MApi_AUDIO_IsMadLock;
    _pInstantAudio->fpAUDIOGetDecStatus =                               _MApi_AUDIO_GetDecStatus;
    _pInstantAudio->fpAUDIOGetCommand =                                 _MApi_AUDIO_GetCommand;
    _pInstantAudio->fpAUDIOSetCommand =                                 _MApi_AUDIO_SetCommand;
    _pInstantAudio->fpAUDIOGetMADLock =                                 _MApi_AUDIO_GetMAD_LOCK;
    _pInstantAudio->fpAUDIOSetADOutputMode =                            _MApi_AUDIO_SetADOutputMode;
    _pInstantAudio->fpAUDIOSetADAbsoluteVoluem =                        _MApi_AUDIO_SetADAbsoluteVolume;
    _pInstantAudio->fpAUDIOADSetMute =                                  _MApi_AUDIO_ADSetMute;

    //============================================================
    // AUDIO_PVR RELATED FUNCTIONS
    //============================================================
    _pInstantAudio->fpAUDIOSetEncInit =                                 _MApi_AUDIO_SetEncInit;
    _pInstantAudio->fpAUDIOSetEncCommand =                              _MApi_AUDIO_SetEncCommand;
    _pInstantAudio->fpAUDIOGetEncodeFrameInfo =                         _MApi_AUDIO_GetEncodeFrameInfo;
    _pInstantAudio->fpAUDIOGetEncodeDoneFlag =                          _MApi_AUDIO_GetEncodeDoneFlag;
    _pInstantAudio->fpAUDIOSetEncInfo =                                 _MApi_AUDIO_SetEncInfo;
    _pInstantAudio->fpAUDIOGetEncInfo =                                 _MApi_AUDIO_GetEncInfo;
    _pInstantAudio->fpAUDIOSetCaptureEnable =                           _MApi_AUDIO_SetCaptureEnable;

    //============================================================
    // AUDIO_ATV RELATIONAL API FUNCTION
    //============================================================
    _pInstantAudio->fpAUDIOSIFSetStandard =                             _MApi_AUDIO_SIF_SetStandard;
    _pInstantAudio->fpAUDIOSIFGetSoundMode =                            _MApi_AUDIO_SIF_GetSoundMode;
    _pInstantAudio->fpAUDIOSIFSetSoundMode =                            _MApi_AUDIO_SIF_SetSoundMode;
    _pInstantAudio->fpAUDIOSIFGetAudioStatus =                          _MApi_AUDIO_SIF_GetAudioStatus;
    _pInstantAudio->fpAUDIOSIFStartAutoStandardDetection =              _MApi_AUDIO_SIF_StartAutoStandardDetection;
    _pInstantAudio->fpAUDIOSIFGetResultOfAutoStandardDetection =        _MApi_AUDIO_SIF_GetResultOfAutoStandardDetection;
    _pInstantAudio->fpAUDIOSIFConvertToBasicAudioStandard =             _MApi_AUDIO_SIF_ConvertToBasicAudioStandard;
    _pInstantAudio->fpAUDIOSIFSetThreshold =                            _MApi_AUDIO_SIF_SetThreshold;
    _pInstantAudio->fpAUDIOSIFSetPrescale =                             _MApi_AUDIO_SIF_SetPrescale;
    _pInstantAudio->fpAUDIOSIFIsPALType =                               _MApi_AUDIO_SIF_IsPALType;
    _pInstantAudio->fpAUDIOSIFSetPALType =                              _MApi_AUDIO_SIF_SetPALType;
    _pInstantAudio->fpAUDIOSIFSendCmd =                                 _MApi_AUDIO_SIF_SendCmd;
    _pInstantAudio->fpAUDIOSIFShift =                                   _MApi_AUDIO_SIF_Shift;

#if 0
    //============================================================
    // AUDIO_FM_RADIO RELATIONAL API FUNCTION
    //============================================================
    _pInstantAudio->fpAUDIOFMRADIOGetSoundMode =                        _MApi_AUDIO_FM_RADIO_GetSoundMode;
    _pInstantAudio->fpAUDIOFMRADIOSetSoundMode =                        _MApi_AUDIO_FM_RADIO_SetSoundMode;
    _pInstantAudio->fpAUDIOFMRADIODeEmphassisOption =                   _MApi_AUDIO_FM_RADIO_DeEmphassisOption;
    _pInstantAudio->fpAUDIOFMRADIOGetDcAmp =                            _MApi_AUDIO_FM_RADIO_GET_DC_AMP;
    _pInstantAudio->fpAUDIOFMRADIOGetNSR =                              _MApi_AUDIO_FM_RADIO_GET_NSR;
    _pInstantAudio->fpAUDIOFMRADIOReset =                               _MApi_AUDIO_FM_RADIO_RESET;
#endif

    //=============================================================
    // AUDIO_SPDIF RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOSPDIFHWEN =                                  _MApi_AUDIO_SPDIF_HWEN;
    _pInstantAudio->fpAUDIOSPDIFSetMute =                               _MApi_AUDIO_SPDIF_SetMute;
    _pInstantAudio->fpAUDIOSPDIFSetMode =                               _MApi_AUDIO_SPDIF_SetMode;
    _pInstantAudio->fpAUDIOSPDIFGetMode =                               _MApi_AUDIO_SPDIF_GetMode;
    _pInstantAudio->fpAUDIOSPDIFSetSCMS =                               _MApi_AUDIO_SPDIF_SetSCMS;
    _pInstantAudio->fpAUDIOSPDIFGetSCMS =                               _MApi_AUDIO_SPDIF_GetSCMS;
    _pInstantAudio->fpAUDIOSPDIFMonitor =                               _MApi_Audio_SPDIF_Monitor;
    _pInstantAudio->fpAUDIOSPDIFChannelStatusCTRL =                     _MApi_AUDIO_SPDIF_ChannelStatus_CTRL;
    _pInstantAudio->fpAUDIODigitalOut_SetChannelStatus =                _MApi_AUDIO_DigitalOut_SetChannelStatus;
    _pInstantAudio->fpAUDIODigitalOut_GetChannelStatus =                _MApi_AUDIO_DigitalOut_GetChannelStatus;


    //=============================================================
    // AUDIO_HDMI RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOHDMITxSetMute =                              _MApi_AUDIO_HDMI_Tx_SetMute;
    _pInstantAudio->fpAUDIOHDMITxGetStatus =                            _MApi_AUDIO_HDMI_Tx_GetStatus;
    _pInstantAudio->fpAUDIOHDMIMonitor =                                _MApi_AUDIO_HDMI_Monitor;
    _pInstantAudio->fpAUDIOHDMIGetNonpcmFlag =                          _MApi_AUDIO_HDMI_GetNonpcmFlag;
    _pInstantAudio->fpAUDIOHDMISetNonpcm =                              _MApi_AUDIO_HDMI_SetNonpcm;
    _pInstantAudio->fpAUDIOHDMIRxSetNonPCM =                            _MApi_AUDIO_HDMI_RX_SetNonpcm;
    _pInstantAudio->fpAUDIOHDMIRxGetNonPCM =                            _MApi_AUDIO_HDMI_RX_GetNonPCM;
    _pInstantAudio->fpAUDIODTVHDMICFG =                                 _MApi_AUDIO_DTV_HDMI_CFG;
    _pInstantAudio->fpAUDIOHDMIGetSynthFreg =                           _MApi_AUDIO_HDMI_GetSynthFreq;
    _pInstantAudio->fpAUDIOHDMISetDownSample =                          _MApi_AUDIO_HDMI_SetDownSample;
    _pInstantAudio->fpAUDIOHDMITxSetMode =                              _MApi_AUDIO_HDMI_TX_SetMode;
    _pInstantAudio->fpAUDIOHDMIRxGetHDMIInAUDIOStatus =                 _MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus;

    //=============================================================
    // AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOADVSOUNDProcessEnable =                      _MApi_AUDIO_ADVSOUND_ProcessEnable;
    _pInstantAudio->fpAUDIOADVSOUNDSubProcessEnable =                   _MApi_AUDIO_ADVSOUND_SubProcessEnable;
    _pInstantAudio->fpAUDIOADVSOUNDSetParam =                           _MApi_AUDIO_ADVSOUND_SetParam;
    _pInstantAudio->fpAUDIOADVSNDSetParam =                             _MApi_AUDIO_ADVSND_SetParam;
    _pInstantAudio->fpAUDIOADVSOUNDSetParam2 =                          _MApi_AUDIO_ADVSOUND_SetParam2;
    _pInstantAudio->fpAUDIOADVSOUNDGetParam2 =                          _MApi_AUDIO_ADVSOUND_GetParam2;
    _pInstantAudio->fpAUDIOADVSOUNDGetInfo =                            _MApi_AUDIO_ADVSOUND_GetInfo;
    _pInstantAudio->fpAUDIODBXTV_SetMode =                              _MApi_DBXTV_SetMode;
    _pInstantAudio->fpAUDIOSeInit =                                     _MApi_AUDIO_SeInit;
    _pInstantAudio->fpAUDIOSetAdvSndSys =                               _MApi_AUDIO_SetAdvSndSys;
    _pInstantAudio->fpAUDIOAdvSndSetSoundParam =                        _MApi_AUDIO_ADVSND_Set_Sound_Param;
    //_pInstantAudio->fpAUDIOSetVDS =                                     _MApi_AUDIO_SetVDS;
    //_pInstantAudio->fpAUDIOSetVSPK =                                    _MApi_AUDIO_SetVSPK;
    //_pInstantAudio->fpAUDIOSetSRS=                                      _MApi_AUDIO_SetSRS;
    //_pInstantAudio->fpAUDIOSetBEE =                                     _MApi_AUDIO_SetBBE;
    //_pInstantAudio->fpAUDIOVSPKWMod =                                   _MApi_AUDIO_VSPK_WMod;
    //_pInstantAudio->fpAUDIOVSPKSMod =                                   _MApi_AUDIO_VSPK_SMod;
    //_pInstantAudio->fpAUDIOSRSDC =                                      _MApi_AUDIO_SRS_DC;
    //_pInstantAudio->fpAUDIOSRSTruBass =                                 _MApi_AUDIO_SRS_TruBass;
    //_pInstantAudio->fpAUDIOSRSSetTsxtPara =                             _MApi_AUDIO_SRS_SetTsxtPara;
    //_pInstantAudio->fpAUDIOSetSRSTSHD =                                 _MApi_AUDIO_SetSRSTSHD;
    //_pInstantAudio->fpAUDIOTSHDDefinition =                             _MApi_AUDIO_TSHD_Definition;
    //_pInstantAudio->fpAUDIOSRSSetTshdPara =                             _MApi_AUDIO_SRS_SetTshdPara;
    _pInstantAudio->fpAUDIOCopyParameter =                              _MApi_AUDIO_COPY_Parameter;

    //=============================================================
    // AUDIO_KTV RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOSetKTVInfo =                                 _MApi_AUDIO_SetKTVInfo;
    _pInstantAudio->fpAUDIOGetKTVInfo =                                 _MApi_AUDIO_GetKTVInfo;
    _pInstantAudio->fpAUDIOSetMixModeVolume =                           _MApi_AUDIO_SetMixModeVolume;
    _pInstantAudio->fpAUDIOSetMixModeMute =                             _MApi_AUDIO_SetMixModeMute;
    _pInstantAudio->fpAUDIOPlayMenuSound =                              _MApi_AUDIO_PlayMenuSound;

    //=============================================================
    // AUDIO_MM RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOSetCommAudioInfo =                           _MApi_AUDIO_SetCommAudioInfo;
    _pInstantAudio->fpAUDIOSetDecodeParam =                             _MApi_ADEC_SetDecodeParam;
    //_pInstantAudio->fpAUDIOSetMpegInfo =                                _MApi_AUDIO_SetMpegInfo;
    //_pInstantAudio->fpAUDIOSetAC3Info =                                 _MApi_AUDIO_SetAC3Info;
    //_pInstantAudio->fpAUDIOSetAC3PInfo =                                _MApi_AUDIO_SetAC3PInfo;
    //_pInstantAudio->fpAUDIOSetAACInfo =                                 _MApi_AUDIO_SetAACInfo;
    //_pInstantAudio->fpAUDIOSetWMAInfo =                                 _MApi_AUDIO_SetWmaInfo;
    //_pInstantAudio->fpAUDIOSetDTSCommonCtrl =                           _MApi_AUDIO_SetDTSCommonCtrl;
    _pInstantAudio->fpAUDIOGetCommAudioInfo =                           _MApi_AUDIO_GetCommAudioInfo;
    _pInstantAudio->fpAUDIOGetMpegInfo =                                _MApi_AUDIO_GetMpegInfo;
    _pInstantAudio->fpAUDIOGetAC3Info =                                 _MApi_AUDIO_GetAC3Info;
    _pInstantAudio->fpAUDIOGetAC3PInfo =                                _MApi_AUDIO_GetAC3PInfo;
    _pInstantAudio->fpAUDIOGetAACInfo =                                 _MApi_AUDIO_GetAACInfo;
    _pInstantAudio->fpAUDIOGetWmaInfo =                                 _MApi_AUDIO_GetWmaInfo;
    _pInstantAudio->fpAUIDOGetDTSInfo =                                 _MApi_AUDIO_GetDTSInfo;
    //_pInstantAudio->fpAUDIOXPCMParam =                                  _MApi_AUDIO_XPCM_Param;
    //_pInstantAudio->fpAUDIOXPCM2Param =                                 _MApi_AUDIO_XPCM2_Param;
    _pInstantAudio->fpAUDIOXPCM2CheckIntStatus =                        _MApi_AUDIO_XPCM2_CheckIntStatus;
    _pInstantAudio->fpAUDIORA8Param =                                   _MApi_AUDIO_RA8_Param;
    _pInstantAudio->fpAUDIOInit =                                       _MApi_AUDIO_Init;
    _pInstantAudio->fpAUDIOStartDecode =                                _MApi_AUDIO_StartDecode;
    _pInstantAudio->fpAUDIOStartBrower =                                _MApi_AUDIO_StartBrowse;
    _pInstantAudio->fpAUDIOStopDecode =                                 _MApi_AUDIO_StopDecode;
    _pInstantAudio->fpAUDIOPauseDecode =                                _MApi_AUDIO_PauseDecode;
    _pInstantAudio->fpAUDIOCheckPlayDone =                              _MApi_AUDIO_CheckPlayDone;
    _pInstantAudio->fpAUDIOGetResidualBufferSize =                      _MApi_AUDIO_GetResidualBufferSize;
    _pInstantAudio->fpAUDIOGetPCMBufferSize =                           _MApi_AUDIO_GetPCMBufferSize;
    _pInstantAudio->fpAUDIOGetPCMBufferSize2 =                          _MApi_AUDIO_GetPCMBufferSize2;
    _pInstantAudio->fpAUDIOGetCurrentFrameNumber =                      _MApi_AUDIO_GetCurrentFrameNumber;
    _pInstantAudio->fpAUDIOGetSampleRate =                              _MApi_AUDIO_GetSampleRate;
    _pInstantAudio->fpAUDIOGetBitRate =                                 _MApi_AUDIO_GetBitRate;
    _pInstantAudio->fpAUDIOGetLayer =                                   _MApi_AUDIO_GetLayer;
    _pInstantAudio->fpAUDIOCheckInputRequest =                          _MApi_AUDIO_CheckInputRequest;
    _pInstantAudio->fpAUDIOSetInput =                                   _MApi_AUDIO_SetInput;
    _pInstantAudio->fpAUDIOSetSampleRateIndex =                         _MApi_AUDIO_SetSampleRateIndex;
    //_pInstantAudio->fpAUDIOSetXPCMParam =                               _MApi_AUDIO_SetXPCMParam;
    _pInstantAudio->fpAUDIOFileEndNotification =                        _MApi_AUDIO_FileEndNotification;
    _pInstantAudio->fpAUDIOFileEndDataHandle =                          _MApi_AUDIO_FileEndDataHandle;
    _pInstantAudio->fpAUDIOGetPlayTick =                                _MApi_AUDIO_GetPlayTick;
    _pInstantAudio->fpAUDIOGetEsMEMCnt =                                _MApi_AUDIO_GetEsMEMCnt;
    _pInstantAudio->fpAUDIOSetASFParm =                                 _MApi_AUDIO_SetASFParm;
    _pInstantAudio->fpAUDIOMMSetInput =                                 _MApi_AUDIO_MM_SetInput;
    _pInstantAudio->fpAUDIOMMCheckPlayDone =                            _MApi_AUDIO_MM_CheckPlayDone;
    _pInstantAudio->fpAUDIOMMCheckInputRequest =                        _MApi_AUDIO_MM_CheckInputRequest;
    _pInstantAudio->fpAUDIODmaReaderInit =                              _MApi_AUDIO_DmaReader_Init;
    _pInstantAudio->fpAUDIODmaReaderAllInputInit =                      _MApi_AUDIO_DmaReader_AllInput_Init;
    _pInstantAudio->fpAUDIODmaReaderWritePCM =                          _MApi_AUDIO_DmaReader_WritePCM;
    _pInstantAudio->fpAUDIOSwDmaReaderInit =                            _MApi_AUDIO_SwDmaReader_Init;
    _pInstantAudio->fpAUDIOSwDmaReaderWritePCM =                        _MApi_AUDIO_SwDmaReader_WritePCM;
    _pInstantAudio->fpAUDIODmaWriterInit =                              _MApi_AUDIO_DmaWriter_Init;
    _pInstantAudio->fpAUDIOUSBPCMEnable =                               _MApi_AUDIO_USBPCM_Enable;
    _pInstantAudio->fpAUDIOUSBPCMSetFlag =                              _MApi_AUDIO_USBPCM_SetFlag;
    _pInstantAudio->fpAUDIOUSBPCMGetFlag =                              _MApi_AUDIO_USBPCM_GetFlag;
    _pInstantAudio->fpAUDIOUSBPCMGetMemInfo =                           _MApi_AUDIO_USBPCM_GetMemInfo;
    _pInstantAudio->fpAUDIOPCMCaptureInit =                             _MApi_AUDIO_PCMCapture_Init;
    _pInstantAudio->fpAUDIOPCMCaptureStart =                            _MApi_AUDIO_PCMCapture_Start;
    _pInstantAudio->fpAUDIOPCMCaptureStop =                             _MApi_AUDIO_PCMCapture_Stop;
    _pInstantAudio->fpAUDIOPCMCaptureRead =                             _MApi_AUDIO_PCMCapture_Read;
    _pInstantAudio->fpAUDIOVoIPConfig =                                 _MApi_AUDIO_VoIP_Config;
    _pInstantAudio->fpAUDIOALSACheck =                                  _MApi_AUDIO_ALSA_Check;
    _pInstantAudio->fpAUDIOALSAEnable =                                 _MApi_AUDIO_ALSA_Enable;
    _pInstantAudio->fpAUDIOUNICheckDecodeDone =                         _MApi_AUDIO_UNI_CheckDecodeDone;
    _pInstantAudio->fpAUDIOUNISetOutput =                               _MApi_AUDIO_UNI_SetOutput;
    _pInstantAudio->fpAUDIOUNISetPCMInputWriteAddr =                    _MApi_AUDIO_UNI_Set_PCMInputWriteAddr;
    _pInstantAudio->fpAUDIOUNIGetOutPCMLevel =                          _MApi_AUDIO_UNI_Get_OutPCMLevel;
    _pInstantAudio->fpAUDIORingTask =                                   _MApi_AUDIO_RingTask;
    _pInstantAudio->fpAUDIORingDataTransfer =                           _MApi_AUDIO_Ring_DataTransfer;
    //_pInstantAudio->fpAUDIOMM2InitAesInfo =                             _MApi_AUDIO_MM2_initAesInfo;
    _pInstantAudio->fpAUDIOMM2CheckAesInfo =                            _MApi_AUDIO_MM2_checkAesInfo;
    _pInstantAudio->fpAUDIOMM2InputAesFinished =                        _MApi_AUDIO_MM2_inputAesFinished;
    _pInstantAudio->fpAUDIOSetAudioParam2 =                             _MApi_AUDIO_SetAudioParam2;
    _pInstantAudio->fpAUDIOGetAudioInfo2 =                              _MApi_AUDIO_GetAudioInfo2;
    _pInstantAudio->fpAUDIOGetDDRInfo =                                 _MApi_AUDIO_GetDDRInfo;

    // LG add function
    //_pInstantAudio->fpAUDIOSetBalanceL =                                _MApi_AUDIO_SetBalance_L;
    //_pInstantAudio->fpAUDIOSetBalanceR =                                _MApi_AUDIO_SetBalance_R;
    _pInstantAudio->fpAUDIOReadReg =                                    _MApi_AUDIO_ReadReg;
    _pInstantAudio->fpAUDIOWriteReg =                                   _MApi_AUDIO_WriteReg;
    //_pInstantAudio->fpAUDIOSetCutBoost =                                _MApi_AUDIO_Set_Cut_Boost;
    //_pInstantAudio->fpAUDIOESStopThreshold =                            _MApi_AUDIO_ES_Stop_Threshold;
    //_pInstantAudio->fpAUDIOSetSPDIFHwMute =                             _MApi_AUDIO_SetSPDIFHWMute;
    //_pInstantAudio->fpAUDIOSetARCHwMute =                               _MApi_AUDIO_Set_ARC_HWMute;
    _pInstantAudio->fpAUDIOARCHwEn =                                    _MApi_AUDIO_ARC_HWEN;
    _pInstantAudio->fpAUDIOARCHwEnByPort =                              _MApi_AUDIO_ARC_HWEN_BY_PORT;
    _pInstantAudio->fpAUDIOLoadAudioClip =                              _MApi_AUDIO_LoadAudioClip;

    _pInstantAudio->fpAUDIOPlayAudioClip =                              _MApi_AUDIO_PlayAudioClip;
    _pInstantAudio->fpAUDIOGetClipPosition =                            _MApi_AUDIO_GetClip_Position;
    _pInstantAudio->fpAUDIOSeekAudioClip =                              _MApi_AUDIO_SeekAudioClip;
    _pInstantAudio->fpAUDIOStopAudioClip =                              _MApi_AUDIO_StopAudioClip;
    _pInstantAudio->fpAUDIOClipPlayDone =                               _MApi_AUDIO_ClipPlayDone;
    _pInstantAudio->fpAUDIOPlayDoneReset =                              _MApi_AUDIO_ClipPlayDone_Rest;
    _pInstantAudio->fpAUDIOClipProcessor =                              _MApi_AUDIO_Clip_processor;
    _pInstantAudio->fpAUDIOSIFGetCurAnalogMode =                        _MApi_AUDIO_SIF_GetCurAnalogMode;
    _pInstantAudio->fpAUDIOSIFSetUserAnalogMode =                       _MApi_AUDIO_SIF_SetUserAnalogMode;
    _pInstantAudio->fpAUDIOSIFSetModeSetup =                            _MApi_AUDIO_SIF_SetModeSetup;
    _pInstantAudio->fpAUDIOSIFSetHDEVMode =                             _MApi_AUDIO_SIF_SetHDEVMode;
    _pInstantAudio->fpAUDIOSIFSetBandSetup =                            _MApi_AUDIO_SIF_SetBandSetup;
    _pInstantAudio->fpAUDIOSIFGetBandDetect =                           _MApi_AUDIO_SIF_GetBandDetect;
    _pInstantAudio->fpAUDIOSIFGetBandDetectAmp =                        _MApi_AUDIO_SIF_GetBandDetect_Amp;
    _pInstantAudio->fpAUDIOSIFCheckAvailableSystem =                    _MApi_AUDIO_SIF_CheckAvailableSystem;
    _pInstantAudio->fpAUDIOSIFCheckA2KD =                               _MApi_AUDIO_SIF_CheckA2DK;
    _pInstantAudio->fpAUDIOSIFGetSoundStandard =                        _MApi_AUDIO_SIF_GetSoundStandard;
    _pInstantAudio->fpAUDIOSIFRedoSetStandard =                         _MApi_AUDIO_SIF_RedoSetStandard;
    //_pInstantAudio->fpAUDIOSIFADCRest =                                 _MApi_AUDIO_SIF_ADC_Reset;
    _pInstantAudio->fpAUDIOGetBtscA2StereoLevel =                       _MApi_AUDIO_GetBtscA2StereoLevel;
    _pInstantAudio->fpAUDIOSetBtscA2ThresholdLevel =                    _MApi_AUDIO_SetBtscA2ThresholdLevel;
    _pInstantAudio->fpAUDIOCheckNicamDigital =                          _MApi_AUDIO_CheckNicamDigital;
    _pInstantAudio->fpAUDIOSetThresholdType =                           _MApi_AUDIO_SetThresholdType;
    _pInstantAudio->fpAUDIODecodeFrameCnt =                             _MApi_AUDIO_Decoded_Frame_Cnt;
    _pInstantAudio->fpAUDIOPcmMixerInit =                               _MApi_AUDIO_PcmMixer_Init;
    _pInstantAudio->fpAUDIOPcmMixerDeInit =                             _MApi_AUDIO_PcmMixer_DeInit;
    _pInstantAudio->fpAUDIOPcmMixerOpen =                               _MApi_AUDIO_PcmMixer_Open;
    _pInstantAudio->fpAUDIOPcmMixerClose =                              _MApi_AUDIO_PcmMixer_Close;
    _pInstantAudio->fpAUDIOPcmMixerFlush =                              _MApi_AUDIO_PcmMixer_Flush;
    _pInstantAudio->fpAUDIOPcmMixerStart =                              _MApi_AUDIO_PcmMixer_Start;
    _pInstantAudio->fpAUDIOPcmMixerStop =                               _MApi_AUDIO_PcmMixer_Stop;
    _pInstantAudio->fpAUDIOPcmMixerSet =                                _MApi_AUDIO_PcmMixer_Set;
    _pInstantAudio->fpAUDIOPcmMixerGet =                                _MApi_AUDIO_PcmMixer_Get;
    _pInstantAudio->fpAUDIOPcmMixerWrite =                              _MApi_AUDIO_PcmMixer_Write;
    _pInstantAudio->fpAUDIOPcmMixerFunc =                               _MApi_AUDIO_PcmMixer_Func;
    _pInstantAudio->fpAUDIOHDMIRXGetHDMIInChannelStatus =               _MApi_AUDIO_HDMI_RX_GetHdmiInChannelStatus;
    _pInstantAudio->fpAUDIOPcmOpen =                                    _MApi_AUDIO_PCM_Open;
    _pInstantAudio->fpAUDIOPcmClose =                                   _MApi_AUDIO_PCM_Close;
    _pInstantAudio->fpAUDIOPcmStart =                                   _MApi_AUDIO_PCM_Start;
    _pInstantAudio->fpAUDIOPcmStop =                                    _MApi_AUDIO_PCM_Stop;
    _pInstantAudio->fpAUDIOPcmSet  =                                    _MApi_AUDIO_PCM_Set;
    _pInstantAudio->fpAUDIOPcmGet  =                                    _MApi_AUDIO_PCM_Get;
    _pInstantAudio->fpAUDIOPcmRead =                                    _MApi_AUDIO_PCM_Read;
    _pInstantAudio->fpAUDIOPcmWrite  =                                  _MApi_AUDIO_PCM_Write;
    _pInstantAudio->fpAUDIOPcmFlush  =                                  _MApi_AUDIO_PCM_Flush;
    //_pInstantAudio->fpAUDIOGetLibVer =                                  _MAPI_AUDIO_Get_LIB_Ver;
    //_pInstantAudio->fpAUDIOGetDSPPTS =                                  _MApi_AUDIO_GET_DSP_PTS;

    return TRUE;

}
#endif //UTOPIA_STRIP
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void AUDIORegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_AUDIO, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)AUDIOOpen, (FUtopiaClose)AUDIOClose, (FUtopiaIOctl)AUDIOIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, AUDIO_POOL_ID_AUDIO0);
    UtopiaResourceCreate("AUDIO", sizeof(AUDIO_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, AUDIO_POOL_ID_AUDIO0);
    UtopiaModuleAddResourceEnd(pUtopiaModule, AUDIO_POOL_ID_AUDIO0);
}

MS_U32 AUDIOOpen(void** ppInstance, void* pAttribute)
{
    AUDIO_MSG("AUDIO open \n");

    if(ppInstance == 0)
    {
        return UTOPIA_STATUS_PARAMETER_ERROR;
    }

    AUDIO_INSTANT_PRIVATE*    psAudioInstPri = NULL;

    UtopiaInstanceCreate(sizeof(AUDIO_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psAudioInstPri);
#ifndef UTOPIA_STRIP
    if(_AUDIOProbeResourceToUtopia(psAudioInstPri) == FALSE)
    {
        AUDIO_MSG("Audio module version is not supported \n");
        return UTOPIA_STATUS_NOT_SUPPORTED;
    }
#endif //UTOPIA_STRIP
    return UTOPIA_STATUS_SUCCESS;

}

MS_U32 AUDIOIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
#ifndef UTOPIA_STRIP
    AUDIO_INSTANT_PRIVATE* psAudioInstPri = NULL;
    void* pModule = NULL;
    void* pResource = NULL;
    void* pResPrivate = NULL;
    MS_BOOL bStatus = FALSE;
    MS_U8 u8Status = 0;
    //MS_S8 s8Status = 0;
    MS_U16 u16Status = 0;
    MS_U32 u32Status = 0;
//    MS_S32 s32Status = 0;
    long long ulong = 0;
    AUDIO_DEC_ID eDecId = 0;
    AU_DVB_DECCMD eDecCmd = 0;
    En_DVB_decCmdType eDecCmdType = 0;
    AUDIOSTANDARD_TYPE eAUDIOStand_Type = 0;
    AUDIO_HDMI_RX_TYPE eHDMI_RX_Type = 0;
    Audio_MP3_LAYER eMP3_Layer = 0;

    MS_U32*                                 pu32Param                       = NULL;
//    MS_S32*                                 ps32Param                       = NULL;
    MS_U64*                                 pu64Param                       = NULL;
    PAUDIO_ST_RX_HDMI_AUDIO_STATUS          pRxHdmiSts                      = NULL;
    PAUDIO_ST_SOUND_SETPARAM                pSoundSetParam                  = NULL;
    PAUDIO_ST_DECODESYSTEM                  pDecodeSystem                   = NULL;
    PAUDIO_ST_GET_DECODESYSTEM              pGetDecodeSystem                = NULL;
    PAUDIO_ST_OPEN_DECODESYSTEM             pOpenDecodeSystem               = NULL;
    PAUDIO_ST_SET_DECODESYSTEM              pSetDecodeSystem                = NULL;
    PAUDIO_ST_RELEASE_DECODESYSTEM              pReleaseDecodeSystem                = NULL;
    PAUDIO_ST_SIF_GET_AUDIO_STATUS          pSifGetAudioStatus              = NULL;
    PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS   pDigitalOut_SetChannelStatus    = NULL;
    PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS   pDigitalOut_GetChannelStatus    = NULL;
    PAUDIO_HDMI_TX_GETSTATUS                pHDMITx_GetStatus               = NULL;
    PAUDIO_ST_COPY_PARAMETER                pCopyParameter                  = NULL;
    PAUDIO_ST_RA8_Param                     pRA8Param                       = NULL;
    PAUDIO_ST_CHECK_INPUT_REQ               pCheckInputRequest              = NULL;
    PAUDIO_ST_MM_CHECK_INPUT_REQ            pMMCheckInputRequest            = NULL;
    PAUDIO_ST_DMAREADER_WRITEPCM            pDmaReaderWritePCM              = NULL;
    PAUDIO_ST_PCMCAPTURE_INIT               pPCMCapture_Init                = NULL;
    PAUDIO_ST_PCMCAPTURE_READ               pPCMCapture_Read                = NULL;
    PAUDIO_ST_UNI_CHECK_DECODEDONE          pUNICheck_DecodeDone            = NULL;
    PAUDIO_ST_RING_DATA_TRANS               pRing_Data_Trans                = NULL;
    PAUDIO_ST_MM2_CHECKAESINFO              pMM2_CheckAesInfo               = NULL;
    PAUDIO_ST_MM2_INPUTAES_FINISH           pMM2_InputAes_Finish            = NULL;
    PAUDIO_ST_GETAUDIOINFO2                 pGetAudioInfo2                  = NULL;
    PAUDIO_ST_ADV_SOUND_SETPARAM            pAdvSnd_Sound_Set_Param         = NULL;
    PAUDIO_ST_OUTPUT_SOURCE                 pOutputSourceInfo               = NULL;
    PAUDIO_ST_SET_AD                        pSetAD                          = NULL;
    PAUDIO_ST_DECODER_COMMAND               pDecoderCommand                 = NULL;
    PAUDIO_ST_SPDIF_OP                      pSpdif_op                       = NULL;
    PAUDIO_ST_SPDIF_SCMS                    pSpdif_scms                     = NULL;
    PAUDIO_ST_HDMI_NONPCM_OP                pHDMI_Nonpcm_op                 = NULL;
    PAUDIO_ST_KTV_OP                        pKTV_OP                         = NULL;
    PAUDIO_ST_SETDECODEPARAM                pSetDecodeParam                 = NULL;
    PAUDIO_ST_ENC_INFO                      pEncInfo                        = NULL;
    PAUDIO_ST_SET_CAPTURE_ENABLE            pSetCaptureEnable               = NULL;
    PAUDIO_ST_ADVSOUND_PARAM2               pADVSound_Param2                = NULL;
    PAUDIO_ST_SWDMAREADER_WRITEPCM          pSwDmaReader_WritePCM           = NULL;
    //PAUDIO_ST_SETBALABCE                    pSetBalance                     = NULL;
    PAUDIO_ST_RWReg                         pRWReg                          = NULL;
    PAUDIO_ST_LOAD_AUDIO_CLIP               pLoadAudioClip                  = NULL;
    PAUDIO_ST_CLIP                          pClip_op                        = NULL;
    PAUDIO_ST_SIF_ANALOGMODE_OP             pSIF_AnalogMode_op              = NULL;
    PAUDIO_ST_SIF_SETMODE_SETUP_OP          pSIF_SetModeSetUP_op            = NULL;
    PAUDIO_ST_SIF_GETBAND_OP                pSIF_GetBand_op                 = NULL;
    PAUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM    pSIF_CheckAvailableSystem       = NULL;
    PAUDIO_ST_SIF_CHECK_A2DK                pSIF_CheckA2DK                  = NULL;
    PAUDIO_ST_SIF_GETSOUNDSTANDARD          pSIF_GetSoundStandard           = NULL;
    PAUDIO_ST_GETBTSC_A2STEREO_LEVEL        pGetBtsc_A2Stereo_level         = NULL;
    PAUDIO_ST_SETBTSCA2THRESHOLDLEVEL       pSetBtsc_A2Threshold_level      = NULL;
    PAUDIO_ST_CHECKNICAMDIGITAL             pCheckNicamDigial               = NULL;
    PAUDIO_ST_SETTHRESHOLDTYPE              pSetThresholdType               = NULL;
    PAUDIO_ST_DECODE_FRAME_CNT              pDecodeFrameCnt                 = NULL;
    PAUDIO_ST_SETENCCOMMAND                 pSetEncCommand                  = NULL;
    PAUDIO_ST_PCMMIXER_INIT                 pPcmMixerInit                   = NULL;
    PAUDIO_ST_PCMMIXER_DEINIT               pPcmMixerDeInit                 = NULL;
    PAUDIO_ST_PCMMIXER_OPEN                 pPcmMixerOpen                   = NULL;
    PAUDIO_ST_PCMMIXER_CLOSE                pPcmMixerClose                  = NULL;
    PAUDIO_ST_PCMMIXER_FLUSH                pPcmMixerFlush                  = NULL;
    PAUDIO_ST_PCMMIXER_START                pPcmMixerStart                  = NULL;
    PAUDIO_ST_PCMMIXER_STOP                 pPcmMixerStop                   = NULL;
    PAUDIO_ST_PCMMIXER_SET                  pPcmMixerSet                    = NULL;
    PAUDIO_ST_PCMMIXER_GET                  pPcmMixerGet                    = NULL;
    PAUDIO_ST_PCMMIXER_WRITE                pPcmMixerWrite                  = NULL;
    PAUDIO_ST_PCMMIXER_FUNC                 pPcmMixerFunc                   = NULL;
    PAUDIO_ST_PCM_OPEN                      pPcmOpen                        = NULL;
    PAUDIO_ST_PCM_CLOSE                     pPcmClose                       = NULL;
    PAUDIO_ST_PCM_START                     pPcmStart                       = NULL;
    PAUDIO_ST_PCM_STOP                      pPcmStop                        = NULL;
    PAUDIO_ST_PCM_SET                       pPcmSet                         = NULL;
    PAUDIO_ST_PCM_GET                       pPcmGet                         = NULL;
    PAUDIO_ST_PCM_READ                      pPcmRead                        = NULL;
    PAUDIO_ST_PCM_WRITE                     pPcmWrite                       = NULL;
    PAUDIO_ST_PCM_FLUSH                     pPcmFlush                       = NULL;
    PAUDIO_ST_ARC_HWEN_BY_PORT              pARCHwenByPort                  = NULL;

    if(pInstance == NULL)
    {
        AUDIO_MSG("pInstance is NULL!!! \n");
        return UTOPIA_STATUS_FAIL;
    }

    if(UtopiaInstanceGetPrivate(pInstance, (void*)&psAudioInstPri) != 0)
    {
        AUDIO_MSG("UtopiaInstanceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    if(UtopiaInstanceGetModule(pInstance, &pModule) != 0)
    {
        AUDIO_MSG("UtopiaInstanceGetModule fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    if(UtopiaResourceObtain(pModule, AUDIO_POOL_ID_AUDIO0, &pResource) != 0)
    {
        AUDIO_MSG("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    if(UtopiaResourceGetPrivate(pResource, &pResPrivate) != 0)
    {
        AUDIO_MSG("UtopiaResourceGetPrivatefail\n");
        goto IOCTRL_END;
    }

    #if 0   //[Q] Need to check sharead mem
    // init share resource of driver and assign share mem pointer to process
    if(psDmxInstPri->fpDMXInitLibRes(pResPrivate) != DMX_FILTER_STATUS_OK)
    {
        AUDIO_MSG("DMX init share memory failed \n");
        tRes = DMX_FILTER_STATUS_ERROR;
        goto IOCTRL_END;
    }
    #endif
#ifdef MSOS_TYPE_LINUX_KERNEL
    u32Gpid = UtopiaInstanceGetPid(pInstance);
#endif

    switch(u32Cmd)
    {
        //============================================================
        // AUDIO_SYSTEM RELATIONAL API FUNCTION
        //============================================================
        case CMD_USIGNAL_PROCESS_KILL:
#ifdef MSOS_TYPE_LINUX_KERNEL
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetAudioParam2(0, Audio_ParamType_KillProcess, (MS_U32)pu32Param[0]);
#endif
            break;
        case MApi_CMD_AUDIO_Initialize:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_Init\n");
            psAudioInstPri->fpAUDIOInitialize();
            break;
        case MApi_CMD_AUDIO_SetPowerOn:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetPowerOn\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetPowerOn((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_GetDSPBaseAddr:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDSPBaseAddr\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOGetDSPMadBaseAddr((MS_BOOL)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = (MS_U32)bStatus;
            break;
        case MApi_CMD_AUDIO_SetDSPBaseAddr:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetDSPBaseAddr\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetDSPBaseAddr((MS_U8)pu32Param[0], pu32Param[1], pu32Param[2]);
            break;
        case MApi_CMD_AUDIO_WriteDecMailBox:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_WriteDecMailBox\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOWriteDecMailBox(pu32Param[0], pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_TriggerSIFPLL:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_TriggerSIFPLL\n");
            psAudioInstPri->fpAUDIOTriggerSifPLL();
            break;
        case MApi_CMD_AUDIO_Monitor:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_Monitor\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOMonitor();
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SetSystemInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSystemInfo\n");
            psAudioInstPri->fpAUDIOSetSystemInfo((AUDIO_INIT_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_SetOutConnectivity:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetOutConnectivity\n");
            psAudioInstPri->fpAUDIOSetOutConnectivity();
            break;
        case MApi_CMD_AUDIO_SetPathInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetPathInfo\n");
            psAudioInstPri->fpAUDIOSetPathInfo((AUDIO_PATH_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_SetOutputInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetOutputInfo\n");
            psAudioInstPri->fpAUDIOSetOutputInfo((AUDIO_OUT_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_SetSourceInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSourceInfo\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSourceInfo((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_InputSwitch:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_InputSwitch\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOInputSwitch((AUDIO_INPUT_TYPE)pu32Param[0], (AUDIO_SWITCH_GROUP)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SetDataCaptureSource:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetDataCaptureSource\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetDataCaptureSource((AUDIO_INPUT_TYPE)pu32Param[1], (AUDIO_SWITCH_GROUP)pu32Param[2]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_OutputSourceInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_OutputSourceInfo\n");
            pOutputSourceInfo = (PAUDIO_ST_OUTPUT_SOURCE)pArgs;
            if(pOutputSourceInfo->bSet == TRUE)
            {
                psAudioInstPri->fpAUDIOSetOutputSourceInfo((OUTPUT_SOURCE_INFO *)pOutputSourceInfo->stOutputSourceInfo);
            }
            else
            {
                psAudioInstPri->fpAUDIOGetOutputSourceInfo((OUTPUT_SOURCE_INFO *)pOutputSourceInfo->stOutputSourceInfo);
            }
            break;
#if 0
        case MApi_CMD_AUDIO_SetOutputSourceInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetOutputSourceInfo\n");
            psAudioInstPri->fpAUDIOSetOutputSourceInfo((OUTPUT_SOURCE_INFO *)pArgs);
            break;
        case MApi_CMD_AUDIO_GetOutputSourceInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMS_AUDIO_GetOutputSourceInfo\n");
            psAudioInstPri->fpAUDIOGetOutputSourceInfo((OUTPUT_SOURCE_INFO *)pArgs);
            break;
#endif
        case MApi_CMD_AUDIO_ReadMailBox:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ReadMailBox\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOReadMailBox((MS_BOOL)pu32Param[1], (MS_U8)pu32Param[2]);
            *((MS_U16*)&(pu32Param[0])) = u16Status;
            break;
        case MApi_CMD_AUDIO_WriteMailBox:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_WriteMailBox\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOWriteMailBox((MS_BOOL)pu32Param[0], (MS_U8)pu32Param[1], (MS_U16)pu32Param[2]);
            break;
        case MApi_CMD_AUDIO_FwTriggerDSP:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_FwTriggerDSP\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOFwTriggerDsp((MS_U16)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetFwStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetFwStatus\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetFwStatus((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_ExitAudioSystem:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ExitAudioSystem\n");
            psAudioInstPri->fpAUDIOExitAudioSystem();
            break;
        case MApi_CMD_AUDIO_RebootDSP:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_RebootDSP\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIORebootDSP((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SendIntrupt:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SendIntrupt\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSendIntrupt((MS_BOOL)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_I2S_SetMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_I2S_SetMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOI2SSetMode((AUDIO_I2S_MODE_TYPE)pu32Param[0], (AUDIO_I2S_MODE_VALUE)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_WritePreInitTable:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_WritePreInitTable\n");
            psAudioInstPri->fpAUDIOWritePreInitTable();
            break;
        case MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage\n");
            psAudioInstPri->fpAUDIOEnaEarphoneLowPowerStage();
            break;
        case MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage\n");
            psAudioInstPri->fpAUDIOEnaEarphoneHighDrivingStage();
            break;

        //=============================================================
        // AUDIO_SOUND RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_SND_ProcessEnable:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_SND_ProcessEnable\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpSNDProcessEnable((Sound_ENABLE_Type)pu32Param[1], (MS_BOOL)pu32Param[2]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_SND_SetParam1:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_SND_SetParam1\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpSNDSetParam1((Sound_SET_PARAM_Type)pu32Param[1], (MS_U16)pu32Param[2], (MS_U16)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_SND_GetParam1:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_SND_GetParam1\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpSNDGetParam1((Sound_GET_PARAM_Type)pu32Param[1], (MS_U16)pu32Param[2]);
            *((MS_U16*)&(pu32Param[0])) = u16Status;
            break;
        case MApi_CMD_AUDIO_Sound_SetParam:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_Sound_SetParam\n");
            pSoundSetParam = (PAUDIO_ST_SOUND_SETPARAM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSoundSetParam((Sound_SET_Type)pSoundSetParam->eType, (MS_U32*)pSoundSetParam->pParam);
            pSoundSetParam->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAbsoluteVolume:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetAbsoluteVolume\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAbsoluteVolume((MS_U8)pu32Param[0], (MS_U8)pu32Param[1], (MS_U8)pu32Param[2]);
            break;
#if 0
        case MApi_CMD_AUDIO_SetPreScale:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetPreScale\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetPreScale((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SetMute:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetPreScale\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetMute((MS_U8)pu32Param[0], (MS_BOOL)pu32Param[1]);
            break;
#endif
        case MApi_CMD_AUDIO_SourceConnect:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SourceConnect\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSourceConnect((AUDIO_OUTPORT_SOURCE_TYPE)pu32Param[0], (MS_BOOL)pu32Param[1]);
            break;
#if 0
        case MApi_CMD_AUDIO_SetBalance:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetBalance\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetBalance((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableEQ:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EnableEQ\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableEQ((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableTone:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EnableTone\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableTone((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableAutoVolume:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EnableAutoVolume\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableAutoVolume((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableSurround:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EnableSurround\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableSurround((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableBalance:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EnableBalance\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableBalance((MS_U8)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_SetDynamicBass:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetDynamicBass\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetDynamicBass((MS_U8)pu32Param[0]);
            break;
#if 0
        case MApi_CMD_AUDIO_SetBass:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetBass\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetBass((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_AbsoluteBass:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_AbsoluteBass\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOAbsoluteBass((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetTreble:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetTreble\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetTreble((MS_U8)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_AbsoluteTreble:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_AbsoluteTreble\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOAbsoluteTreble((MS_U8)pu32Param[0]);
            break;
#if 0
        case MApi_CMD_AUDIO_SetEq:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetEq\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetEq((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SetEq_7band:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetEq_7band\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetEq7band((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SetAvcThreshold:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetAvcThreshold\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAvcThreshold((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetAvcMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetAvcMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAvcMode((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetAvcAT:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetAvcAT\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAvcAT((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetAvcRT:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetAvcRT\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAvcRT((MS_U8)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_SetBufferProcess:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetBufferProcess\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetBufferProcess((MS_U8)pu32Param[0]);
            break;
#if 0
        case MApi_CMD_AUDIO_SetSurroundXA:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSurroundXA\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSurroundXA((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetSurroundXB:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSurroundXB\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSurroundXB((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetSurroundXK:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSurroundXK\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSurroundXK((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetSurrounLPFGain:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSurrounLPFGain\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSurroundLPFGain((MS_U8)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_ConvertVolumeUnit:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ConvertVolumeUnit\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status= psAudioInstPri->fpAUDIOConvertVolumeUnit((MS_U8)pu32Param[1]);
            *((MS_U8*)&(pu32Param[0])) = u8Status;
            break;
        case MApi_CMD_AUDIO_SetPEQCoef:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetPEQCoef\n");
            psAudioInstPri->fpAUDIOSetPEQCoef((AUDIO_PEQ_COEF*)pArgs);
            break;
#if 0
        case MApi_CMD_AUDIO_EnablePEQ:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EnablePEQ\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnablePEQ((MS_U8)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_EnableDcRemove:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EnableDcRemove\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOEnableDcRemove((MS_U8)pu32Param[1]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;

        //============================================================
        // AUDIO_DTV RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_DecodeSystem:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DecodeSystem\n");
            pDecodeSystem = (PAUDIO_ST_DECODESYSTEM)pArgs;
            if(pDecodeSystem->eType == DECODE_SYS_RELEASE)
            {
                bStatus= psAudioInstPri->fpAUDIOReleaseDecodeSystem(pDecodeSystem->DecId);
                pDecodeSystem->bStatus = bStatus;
            }
            else if(pDecodeSystem->eType == DECODE_SYS_SET)
            {
                bStatus = psAudioInstPri->fpAUDIOSetDecodeSystem(pDecodeSystem->DecId, pDecodeSystem->p_AudioDecStatus);
                pDecodeSystem->bStatus = bStatus;
            }
            else if(pDecodeSystem->eType == DECODE_SYS_GET)
            {
                bStatus = psAudioInstPri->fpAUDIOGetDecodeSystem(pDecodeSystem->DecId, pDecodeSystem->p_AudioDecStatus);
                pDecodeSystem->bStatus = bStatus;
            }
            else if(pDecodeSystem->eType == DECODE_SYS_OPEN)
            {
                eDecId = psAudioInstPri->fpAUDIOOpenDecodeSystem(pDecodeSystem->p_AudioDecStatus);
                pDecodeSystem->DecId = eDecId;
            }
            break;
#if 1
        case MApi_CMD_AUDIO_ReleaseDecodeSystem:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ReleaseDecodeSystem\n");
            pReleaseDecodeSystem = (PAUDIO_ST_RELEASE_DECODESYSTEM)pArgs;
            bStatus= psAudioInstPri->fpAUDIOReleaseDecodeSystem(pReleaseDecodeSystem->DecId);
            pReleaseDecodeSystem->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_SetDecodeSystem:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetDecodeSystem\n");
            pSetDecodeSystem = (PAUDIO_ST_SET_DECODESYSTEM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetDecodeSystem(pSetDecodeSystem->DecId, pSetDecodeSystem->p_AudioDecStatus);
            pSetDecodeSystem->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_GetDecodeSystem:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDecodeSystem\n");
            pGetDecodeSystem = (PAUDIO_ST_GET_DECODESYSTEM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetDecodeSystem(pGetDecodeSystem->DecId, pGetDecodeSystem->p_AudioDecStatus);
            pGetDecodeSystem->bStatus = bStatus;
            break;
#endif
        case MApi_CMD_AUDIO_SetDecodeCmd:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetDecodeCmd\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus= psAudioInstPri->fpAUDIOSetDecodeCmd((MS_U8)pu32Param[1], (MS_U8)pu32Param[2]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
#if 1
        case MApi_CMD_AUDIO_OpenDecodeSystem:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_OpenDecodeSystem\n");
            pOpenDecodeSystem = (PAUDIO_ST_OPEN_DECODESYSTEM)pArgs;
            eDecId = psAudioInstPri->fpAUDIOOpenDecodeSystem(pOpenDecodeSystem->p_AudioDecStatus);
            pOpenDecodeSystem->DecId = eDecId;
            break;
#endif
        case MApi_CMD_AUDIO_SetSystem:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSystem\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus= psAudioInstPri->fpAUDIOSetSystem((En_DVB_decSystemType)pu32Param[1]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_GetDecSysSupportStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDecSysSupportStatus\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetDecSysSupportStatus((En_DVB_decSystemType)pu32Param[1]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_IsMadLock:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_IsMadLock\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOIsMadLock();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_GetDecStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDecStatus\n");
            pu32Param = (MS_U32*)pArgs;
            eDecCmdType = psAudioInstPri->fpAUDIOGetDecStatus();
            *((En_DVB_decCmdType*)&(pu32Param[1])) = eDecCmdType;
            break;
        case MApi_CMD_AUDIO_Decode_Command:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_Decode_Command\n");
            pDecoderCommand = (PAUDIO_ST_DECODER_COMMAND)pArgs;
            if(pDecoderCommand->bSet == TRUE)
            {
                psAudioInstPri->fpAUDIOSetCommand(pDecoderCommand->enCommandType);
            }
            else
            {
                eDecCmd = psAudioInstPri->fpAUDIOGetCommand(pDecoderCommand->eAdoDecoderNo);
                pDecoderCommand->eDecCmd = eDecCmd;
            }
            break;
#if 0
        case MApi_CMD_AUDIO_GetCommand:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetCommand\n");
            pu32Param = (MS_U32*)pArgs;
            eDecCmd = psAudioInstPri->fpAUDIOGetCommand((En_DVB_AudioDecoder)pu32Param[1]);
            *((AU_DVB_DECCMD*)&(pu32Param[0])) = eDecCmd;
            break;
        case MApi_CMD_AUDIO_SetCommand:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetCommand\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetCommand((En_DVB_decCmdType)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_GetMAD_Lock:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetMAD_Lock\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetMADLock();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SETAD:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SETAD\n");
            pSetAD = (PAUDIO_ST_SET_AD)pArgs;
            if(pSetAD->eType == AD_OUTPUT_MODE)
            {
                psAudioInstPri->fpAUDIOSetADOutputMode(pSetAD->u8Data);
            }
            else if(pSetAD->eType == AD_ABSOLUTE_VOLUME)
            {
                psAudioInstPri->fpAUDIOSetADAbsoluteVoluem(pSetAD->u8Data);
            }
            else if(pSetAD->eType == AD_SET_MUTE)
            {
                psAudioInstPri->fpAUDIOADSetMute(pSetAD->bMute);
            }
            break;
#if 0
        case MApi_CMD_AUDIO_SetADOutputMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetADOutputMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetADOutputMode((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetADAbsoluteVolume:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetADAbsoluteVolume\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetADAbsoluteVoluem((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_ADSetMute:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ADSetMute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOADSetMute((MS_BOOL)pu32Param[0]);
            break;
#endif
        //============================================================
        // AUDIO_PVR RELATED FUNCTIONS
        //============================================================
        case MApi_CMD_AUDIO_SetEncInit:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetEncInit\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetEncInit((AU_DVB_ENCBR)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SetEncCommand:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetEncCommand\n");
            pSetEncCommand = (PAUDIO_ST_SETENCCOMMAND)pArgs;
            psAudioInstPri->fpAUDIOSetEncCommand((AU_DVB_ENCCMD)pSetEncCommand->enEncComamnd, (void*)pSetEncCommand->pCB, (MS_U8)pSetEncCommand->u8Param);
            break;
        case MApi_CMD_AUDIO_GetEncodeFrameInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetEncodeFrameInfo\n");
            psAudioInstPri->fpAUDIOGetEncodeFrameInfo((AU_DVB_ENC_FRAME_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_GetEncodeDoneFlag:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetEncodeDoneFlag\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetEncodeDoneFlag();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_EncInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_EncInfo\n");
            pEncInfo = (PAUDIO_ST_ENC_INFO)pArgs;
            if(pEncInfo->bSet == TRUE)
            {
                bStatus = psAudioInstPri->fpAUDIOSetEncInfo(pEncInfo->infoType, pEncInfo->param1, pEncInfo->param2);
                pEncInfo->bRet = bStatus;
            }
            else
            {
                u32Status = psAudioInstPri->fpAUDIOGetEncInfo(pEncInfo->infoType);
                pEncInfo->u32Ret = u32Status;
            }
            break;

        case MApi_CMD_AUDIO_SetCaptureEnable:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetCaptureEnable\n");
            pSetCaptureEnable  = (PAUDIO_ST_SET_CAPTURE_ENABLE)pArgs;
            psAudioInstPri->fpAUDIOSetCaptureEnable(pSetCaptureEnable->bEnable, pSetCaptureEnable->PCM_CB);
            break;

        //============================================================
        // AUDIO_ATV RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_SIF_SetStandard:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetStandard\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSIFSetStandard((AUDIOSTANDARD_TYPE)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SIF_GetSoundMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_GetSoundMode\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOSIFGetSoundMode();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_SIF_SetSoundMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetSoundMode\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSIFSetSoundMode((MS_U8)pu32Param[1]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SIF_GetAudioStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_GetAudioStatus\n");
            pSifGetAudioStatus = (PAUDIO_ST_SIF_GET_AUDIO_STATUS)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSIFGetAudioStatus(pSifGetAudioStatus->p_eAudioStatus);
            pSifGetAudioStatus->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_SIF_StartAutoStandardDetection:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_StartAutoStandardDetection\n");
            psAudioInstPri->fpAUDIOSIFStartAutoStandardDetection();
            break;
        case MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection\n");
            pu32Param = (MS_U32*)pArgs;
            eAUDIOStand_Type = psAudioInstPri->fpAUDIOSIFGetResultOfAutoStandardDetection();
            *((AUDIOSTANDARD_TYPE*)&(pu32Param[1])) = eAUDIOStand_Type;
            break;
        case MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard\n");
            pu32Param = (MS_U32*)pArgs;
            eAUDIOStand_Type = psAudioInstPri->fpAUDIOSIFConvertToBasicAudioStandard((AUDIOSTANDARD_TYPE)pu32Param[1]);
            *((AUDIOSTANDARD_TYPE*)&(pu32Param[0])) = eAUDIOStand_Type;
            break;
        case MApi_CMD_AUDIO_SIF_SetThreshold:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetThreshold\n");
            psAudioInstPri->fpAUDIOSIFSetThreshold((THR_TBL_TYPE code*)pArgs);
            break;
        case MApi_CMD_AUDIO_SIF_SetPrescale:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetThreshold\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSIFSetPrescale((MS_U8)pu32Param[0], (int)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SIF_IsPALType:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_IsPALType\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus= psAudioInstPri->fpAUDIOSIFIsPALType((AUDIO_SIF_PAL_TYPE)pu32Param[1]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SIF_SetPALType:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetPALType\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSIFSetPALType((AUDIO_SIF_PAL_TYPE)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SIF_SendCmd:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_SendCmd\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSIFSendCmd((En_AUD_SIF_CmdType)pu32Param[0], (MS_U8)pu32Param[1], (MS_U8)pu32Param[2]);
            break;
        case MApi_CMD_AUDIO_SIF_Shift:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_Shift\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSIFShift((En_AUD_VIF_Type)pu32Param[0]);
            break;

#if 0
        //============================================================
        // AUDIO_FM_RADIO RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_FM_RADIO_GetSoundMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_GetSoundMode\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOFMRADIOGetSoundMode();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_FM_RADIO_SetSoundMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_SetSoundMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOFMRADIOSetSoundMode((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_FM_RADIO_DeEmphassisOption:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_DeEmphassisOption\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOFMRADIODeEmphassisOption((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_FM_RADIO_GetDC_Amp:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_GetDC_Amp\n");
            ps32Param = (MS_S32*)pArgs;
            s8Status = psAudioInstPri->fpAUDIOFMRADIOGetDcAmp();
            *((MS_S8*)&(ps32Param[1])) = s8Status;
            break;
        case MApi_CMD_AUDIO_FM_RADIO_Get_NSR:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_Get_NSR\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOFMRADIOGetNSR();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_FM_RADIO_Reset:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_Reset\n");
            psAudioInstPri->fpAUDIOFMRADIOReset();
            break;
#endif

        //=============================================================
        // AUDIO_SPDIF RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_SPDIF_OP:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_OP\n");
            pSpdif_op = (PAUDIO_ST_SPDIF_OP)pArgs;
            if(pSpdif_op->eType == SPDIF_HWEN)
            {
                psAudioInstPri->fpAUDIOSPDIFHWEN(pSpdif_op->spdif_hwen);
            }
            else if(pSpdif_op->eType == SPDIF_SETMUTE)
            {
                psAudioInstPri->fpAUDIOSPDIFSetMute(pSpdif_op->mute_en);
            }
            else if(pSpdif_op->eType == SPDIF_SETMODE)
            {
                psAudioInstPri->fpAUDIOSPDIFSetMode(pSpdif_op->spdif_mode);
            }
            else if(pSpdif_op->eType == SPDIF_GETMODE)
            {
                u8Status = psAudioInstPri->fpAUDIOSPDIFGetMode();
                pSpdif_op->spdif_mode = u8Status;
            }
            break;
        case MApi_CMD_AUDIO_SPDIF_SCMS:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_SCMS\n");
            pSpdif_scms = (PAUDIO_ST_SPDIF_SCMS)pArgs;
            if(pSpdif_scms->bSetSCMS == TRUE)
            {
                psAudioInstPri->fpAUDIOSPDIFSetSCMS(pSpdif_scms->C_bit_en, pSpdif_scms->L_bit_en);
            }
            else
            {
                u8Status = psAudioInstPri->fpAUDIOSPDIFGetSCMS();
                pSpdif_scms->Ret_bit = u8Status;
            }
            break;
#if 0
        case MApi_CMD_AUDIO_SPDIF_HWEN:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_HWEN\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSPDIFHWEN((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SPDIF_SetMute:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_SetMute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSPDIFSetMute((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SPDIF_SetMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_SetMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSPDIFSetMode((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SPDIF_GetMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_GetMode\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOSPDIFGetMode();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_SPDIF_SetSCMS:
            AUDIO_MSG("AUDIOIoctl - IOCTL_AUDIO_SPDIF_SETSCMS\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSPDIFSetSCMS((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SPDIF_GetSCMS:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_GetSCMS\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOSPDIFGetSCMS();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
#endif
        case MApi_CMD_AUDIO_SPDIF_Monitor:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_Monitor\n");
            psAudioInstPri->fpAUDIOSPDIFMonitor();
            break;
        case MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSPDIFChannelStatusCTRL((AUDIO_SPDIF_CS_TYPE)pu32Param[1], (AUDIO_SPDIF_CS_TYPE_STATUS)pu32Param[2]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_DigitalOut_SetChannelStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DigitalOut_SetChannelStatus\n");
            pDigitalOut_SetChannelStatus = (PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS)pArgs;
            bStatus = psAudioInstPri->fpAUDIODigitalOut_SetChannelStatus((DIGITAL_OUTPUT_TYPE)pDigitalOut_SetChannelStatus->ePath, (Digital_Out_Channel_Status_t*)pDigitalOut_SetChannelStatus->stChannelStatus);
            pDigitalOut_SetChannelStatus->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_DigitalOut_GetChannelStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DigitalOut_GetChannelStatus\n");
            pDigitalOut_GetChannelStatus = (PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS)pArgs;
            psAudioInstPri->fpAUDIODigitalOut_GetChannelStatus((DIGITAL_OUTPUT_TYPE)pDigitalOut_GetChannelStatus->ePath, (Digital_Out_Channel_Status_t*)pDigitalOut_GetChannelStatus->stChannelStatus);
            break;

        //=============================================================
        // AUDIO_HDMI RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_HDMI_Tx_SetMute:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Tx_SetMute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOHDMITxSetMute((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_Tx_GetStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Tx_GetStatus\n");
            pHDMITx_GetStatus = (PAUDIO_HDMI_TX_GETSTATUS)pArgs;
            psAudioInstPri->fpAUDIOHDMITxGetStatus(&(pHDMITx_GetStatus->onOff), &(pHDMITx_GetStatus->hdmi_smpFreq), &(pHDMITx_GetStatus->outType));
            break;
        case MApi_CMD_AUDIO_HDMI_Monitor:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Monitor\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status= psAudioInstPri->fpAUDIOHDMIMonitor();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_HDMI_GetNonpcmFlag:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_GetNonpcmFlag\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOHDMIGetNonpcmFlag();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_HDMI_SetNonpcm:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_SetNonpcm\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOHDMISetNonpcm((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_Nonpcm_OP:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Nonpcm_OP\n");
            pHDMI_Nonpcm_op = (PAUDIO_ST_HDMI_NONPCM_OP)pArgs;
            if(pHDMI_Nonpcm_op->bSet == TRUE)
            {
                eHDMI_RX_Type = psAudioInstPri->fpAUDIOHDMIRxSetNonPCM(pHDMI_Nonpcm_op->nonPCM_en);
            }
            else
            {
                eHDMI_RX_Type = psAudioInstPri->fpAUDIOHDMIRxGetNonPCM();
            }
            pHDMI_Nonpcm_op->eRet = eHDMI_RX_Type;
            break;
#if 0
        case MApi_CMD_AUDIO_HDMI_Rx_SetNonPcm:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Rx_SetNonPcm\n");
            pu32Param = (MS_U32*)pArgs;
            eHDMI_RX_Type = psAudioInstPri->fpAUDIOHDMIRxSetNonPCM((MS_U8)pu32Param[1]);
            *((AUDIO_HDMI_RX_TYPE*)&(pu32Param[0])) = eHDMI_RX_Type;
            break;

        case MApi_CMD_AUDIO_HDMI_RX_GetNonpcm:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_RX_GetNonpcm\n");
            pu32Param = (MS_U32*)pArgs;
            eHDMI_RX_Type = psAudioInstPri->fpAUDIOHDMIRxGetNonPCM();
            *((AUDIO_HDMI_RX_TYPE*)&(pu32Param[1])) = eHDMI_RX_Type;
            break;
#endif
        case MApi_CMD_AUDIO_DTV_HDMI_CFG:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DTV_HDMI_CFG\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIODTVHDMICFG((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_GetSynthFreq:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_GetSynthFreq\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOHDMIGetSynthFreg();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_HDMI_SetDownSample:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_SetDownSample\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOHDMISetDownSample((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_TX_SetMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_TX_SetMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOHDMITxSetMode((HDMI_TX_OUTPUT_TYPE)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus\n");
            pRxHdmiSts = (PAUDIO_ST_RX_HDMI_AUDIO_STATUS)pArgs;
            bStatus = psAudioInstPri->fpAUDIOHDMIRxGetHDMIInAUDIOStatus(pRxHdmiSts->pHdmi_status);
            pRxHdmiSts->bStatus = bStatus;
            break;

        //=============================================================
        // AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_ADVSOUND_ProcessEnable:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_ProcessEnable\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOADVSOUNDProcessEnable((ADVSND_TYPE)pu32Param[1]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOADVSOUNDSubProcessEnable((ADVFUNC)pu32Param[1], (MS_BOOL)pu32Param[2]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_ADVSOUND_SetParam:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOADVSOUNDSetParam((ADVSND_PARAM)pu32Param[1], (MS_U16)pu32Param[2], (MS_U16)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_ADVSND_SetParam:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ADVSND_SetParam\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOADVSNDSetParam((ADVSND_PARAM)pu32Param[1], (MS_U32)pu32Param[2], (MS_U16)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_ADVSOUND_Param2:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_Param2\n");
            pADVSound_Param2 = (PAUDIO_ST_ADVSOUND_PARAM2)pArgs;
            if(pADVSound_Param2->bSet == TRUE)
            {
                bStatus = psAudioInstPri->fpAUDIOADVSOUNDSetParam2(pADVSound_Param2->param, pADVSound_Param2->index, pADVSound_Param2->u32value1, pADVSound_Param2->u16value2);
            }
            else
            {
                bStatus = psAudioInstPri->fpAUDIOADVSOUNDGetParam2(pADVSound_Param2->param, pADVSound_Param2->index, pADVSound_Param2->u32value1, pADVSound_Param2->u16value2);
            }
            pADVSound_Param2->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_ADVSOUND_GetInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_GetInfo\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOADVSOUNDGetInfo((AdvSound_GET_Type)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
        case MApi_CMD_AUDIO_DBXTV_SetMode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DBXTV_SetMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIODBXTV_SetMode((EN_DBXTV_TotSonMode)pu32Param[0], (EN_DBXTV_TotVolMode)pu32Param[1], (EN_DBXTV_TotSurMode)pu32Param[2],(MS_U32)pu32Param[3]);
            break;
        case MApi_CMD_AUDIO_SeInit:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DBXTV_SetMode\n");
            psAudioInstPri->fpAUDIOSeInit();
            break;
        case MApi_CMD_AUDIO_SetAdvSndSys:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DBXTV_SetMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAdvSndSys((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_ADV_SOUND_SET:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ADV_SOUND_SET\n");
            pAdvSnd_Sound_Set_Param = (PAUDIO_ST_ADV_SOUND_SETPARAM)pArgs;
            psAudioInstPri->fpAUDIOAdvSndSetSoundParam((AdvSound_SET_Type)pAdvSnd_Sound_Set_Param->eType, (MS_U32*)pAdvSnd_Sound_Set_Param->pParam);
            break;
#if 0
        case MApi_CMD_AUDIO_SetVDS:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetVDS\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetVDS((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetVSPK:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetVSPK\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetVSPK((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetSRS:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSRS\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSRS((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetBEE:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetBEE\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetBEE((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_VSPK_WMod:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_VSPK_WMod\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOVSPKWMod((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_VSPK_SMod:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_VSPK_SMod\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOVSPKSMod((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SRS_DC:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SRS_DC\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSRSDC((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SRS_TruBass:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SRS_TruBass\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSRSTruBass((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SRSTsxtPara:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SRSTsxtPara\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSRSSetTsxtPara((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SetSRSTSHD:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSRSTSHD\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSRSTSHD((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_TSHD_Definition:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_TSHD_Definition\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOTSHDDefinition((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SRSSetTshdPara:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SRSSetTshdPara\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSRSSetTshdPara((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
#endif
        case MApi_CMD_AUDIO_Copy_Parameter:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_Copy_Parameter\n");
            pCopyParameter = (PAUDIO_ST_COPY_PARAMETER)pArgs;
            psAudioInstPri->fpAUDIOCopyParameter((ADVSND_PARAM)pCopyParameter->type, (MS_U32*)pCopyParameter->Parameter_ptr, (MS_U32)pCopyParameter->size);
            break;
        //=============================================================
        // AUDIO_KTV RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_KTV_OP:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_KTV_OP\n");
            pKTV_OP = (PAUDIO_ST_KTV_OP)pArgs;
            if(pKTV_OP->eType == KTV_SET_INFO)
            {
                bStatus = psAudioInstPri->fpAUDIOSetKTVInfo(pKTV_OP->modelType, pKTV_OP->paramType, pKTV_OP->param1, pKTV_OP->param2);
                pKTV_OP->bRet = bStatus;
            }
            else if(pKTV_OP->eType == KTV_GET_INFO)
            {
                u32Status = psAudioInstPri->fpAUDIOGetKTVInfo(pKTV_OP->modelType, pKTV_OP->infoType);
                pKTV_OP->u32Ret = u32Status;
            }
            else if(pKTV_OP->eType == KTV_SET_MIX_VOL)
            {
                psAudioInstPri->fpAUDIOSetMixModeVolume(pKTV_OP->eSourceType, pKTV_OP->VolType, pKTV_OP->u8Vol1, pKTV_OP->u8Vol2);
            }
            else if (pKTV_OP->eType == KTV_SET_MIX_MUTE)
            {
                psAudioInstPri->fpAUDIOSetMixModeMute(pKTV_OP->eSourceType, pKTV_OP->VolType, pKTV_OP->EnMute);
            }
            break;
#if 0
        case MApi_CMD_AUDIO_SetKTVInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetKTVInfo\n");
            pu32Param = (MS_U32*)pArgs;
            *((MS_BOOL*)&(pu32Param[0])) = psAudioInstPri->fpAUDIOSetKTVInfo((AUDIO_KTV_MODEL)pu32Param[1], (AUDIO_KTV_PARAMTYPE)pu32Param[2], (MS_U32)pu32Param[3], (MS_U32)pu32Param[4]);
            break;
        case MApi_CMD_AUDIO_GetKTVInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetKTVInfo\n");
            pu32Param = (MS_U32*)pArgs;
            *((MS_U32*)&(pu32Param[0])) = psAudioInstPri->fpAUDIOGetKTVInfo((AUDIO_KTV_MODEL)pu32Param[1], (AUDIO_KTV_INFOTYPE)pu32Param[2]);
            break;
        case MApi_CMD_AUDIO_SetMixModeVolume:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetMixModeVolume\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetMixModeVolume((AUDIO_SOURCE_INFO_TYPE)pu32Param[0], (AUDIO_MIX_VOL_TYPE)pu32Param[1], (MS_U8)pu32Param[2], (MS_U8)pu32Param[3]);
            break;
        case MApi_CMD_AUDIO_SetMixModeMute:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetMixModeMute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetMixModeMute((AUDIO_SOURCE_INFO_TYPE)pu32Param[0], (AUDIO_MIX_VOL_TYPE)pu32Param[1], (MS_BOOL)pu32Param[2]);
            break;
#endif
        case MApi_CMD_AUDIO_PlayMenuSound:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PlayMenuSound\n");
            psAudioInstPri->fpAUDIOPlayMenuSound();
            break;

        //=============================================================
        // AUDIO_MM RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_SetCommAudioInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetCommAudioInfo\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetCommAudioInfo((Audio_COMM_infoType)pu32Param[1], (MS_U32)pu32Param[2], (MS_U32)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SetDecodeParam:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetDecodeParam\n");
            pSetDecodeParam = (PAUDIO_ST_SETDECODEPARAM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetDecodeParam(pSetDecodeParam->DecID, pSetDecodeParam->paramType, pSetDecodeParam->Param);
            pSetDecodeParam->bStatus = bStatus;
            break;
#if 0
        case MApi_CMD_AUDIO_SetMpegInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetMpegInfo\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetMpegInfo((Audio_MPEG_infoType)pu32Param[1], (MS_U32)pu32Param[2], (MS_U32)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAC3Info:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetAC3Info\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetAC3Info((Audio_AC3_infoType)pu32Param[1], (MS_U32)pu32Param[2], (MS_U32)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAC3PInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetAC3PInfo\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetAC3PInfo((Audio_AC3P_infoType)pu32Param[1], (MS_U32)pu32Param[2], (MS_U32)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAACInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetAACInfo\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetAACInfo((Audio_AAC_infoType)pu32Param[1], (MS_U32)pu32Param[2], (MS_U32)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SetWMAInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetWMAInfo\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetWMAInfo((Audio_WMA_infoType)pu32Param[1], (MS_U32)pu32Param[2], (MS_U32)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SetDTSCommonCtrl:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetDTSCommonCtrl\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetDTSCommonCtrl((Audio_DTS_infoType)pu32Param[1], (Audio_DTS_CtrlType)pu32Param[2]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
#endif
        case MApi_CMD_AUDIO_GetCommAudioInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetCommAudioInfo\n");
            pu64Param = (MS_U64*)pArgs;
            ulong= psAudioInstPri->fpAUDIOGetCommAudioInfo((Audio_COMM_infoType)pu64Param[1]);
            *((long long*)&(pu64Param[0])) = ulong;
            break;
        case MApi_CMD_AUDIO_GetMpegInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetMpegInfo\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetMpegInfo((Audio_MPEG_infoType)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetAC3Info:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetAC3Info\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetAC3Info((Audio_AC3_infoType)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetAC3PInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetAC3PInfo\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetAC3PInfo((Audio_AC3P_infoType)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetAACInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetAACInfo\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetAACInfo((Audio_AAC_infoType)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetWmaInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetWmaInfo\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetWmaInfo((Audio_WMA_infoType)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetDTSInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDTSInfo\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status= psAudioInstPri->fpAUIDOGetDTSInfo((Audio_DTS_infoType)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
#if 0
        case MApi_CMD_AUDIO_XPCM_Param:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_XPCM_Param\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOXPCMParam((XPCM_TYPE)pu32Param[1], (MS_U8)pu32Param[2], (MS_U16)pu32Param[3],
                        (MS_U8)pu32Param[4], (MS_U16)pu32Param[5], (MS_U16)pu32Param[6]);
            *((MS_U8*)&(pu32Param[0])) = u8Status;
            break;
        case MApi_CMD_AUDIO_XPCM2_Param:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_XPCM2_Param\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOXPCM2Param((XPCM_TYPE)pu32Param[0], (MS_U8)pu32Param[1], (MS_U16)pu32Param[2], (MS_U8)pu32Param[3],
                (MS_U16)pu32Param[4], (MS_U16)pu32Param[5]);
            break;
#endif
        case MApi_CMD_AUDIO_XPCM2_CheckIntStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_XPCM2_CheckIntStatus\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOXPCM2CheckIntStatus();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_RA8_Param:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_RA8_Param\n");
            pRA8Param = (PAUDIO_ST_RA8_Param)pArgs;
            u8Status = psAudioInstPri->fpAUDIORA8Param((MS_U16)pRA8Param->mNumCodecs, (MS_U16)pRA8Param->mSamples, (MS_U16)pRA8Param->mSampleRate,
                        (MS_U16*)pRA8Param->pChannels, (MS_U16*)pRA8Param->pRegions, (MS_U16*)pRA8Param->pcplStart, (MS_U16*)pRA8Param->pcplQbits, (MS_U16*)pRA8Param->pFrameSize);
            pRA8Param->u8Status = u8Status;
            break;
        case MApi_CMD_AUDIO_Init:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_Init\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOInit((En_DVB_decSystemType)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_StartDecode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_StartDecode\n");
            psAudioInstPri->fpAUDIOStartDecode();
            break;
        case MApi_CMD_AUDIO_StartBroswe:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_StartBroswe\n");
            psAudioInstPri->fpAUDIOStartBrower();
            break;
        case MApi_CMD_AUDIO_StopDecode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_StopDecode\n");
            psAudioInstPri->fpAUDIOStopDecode();
            break;
        case MApi_CMD_AUDIO_PauseDecode:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PauseDecode\n");
            psAudioInstPri->fpAUDIOPauseDecode();
            break;
        case MApi_CMD_AUDIO_CheckPlayDone:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_CheckPlayDone\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOCheckPlayDone();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_GetResidualBufferSize:
            AUDIO_MSG("AUDIOIoctl - IOCTL_AUDIO_GETRESIDUALBUFFERSIZE\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOGetResidualBufferSize();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_GetPCMBufferSize:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetPCMBufferSize\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOGetPCMBufferSize((MS_U16)pu32Param[1]);
            *((MS_U16*)&(pu32Param[0])) = u16Status;
            break;
        case MApi_CMD_AUDIO_GetPCMBufferSize2:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetPCMBufferSize2\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOGetPCMBufferSize2((MS_U16)pu32Param[1], (MS_U16)pu32Param[2]);
            *((MS_U16*)&(pu32Param[0])) = u16Status;
            break;
        case MApi_CMD_AUDIO_GetCurrentFrameNumber:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetCurrentFrameNumber\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOGetCurrentFrameNumber();
            *((MS_U32*)&(pu32Param[1])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetSampleRates:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetSampleRates\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOGetSampleRate();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_GetBitRates:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetBitRates\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOGetBitRate();
            *((MS_U32*)&(pu32Param[1])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetLayer:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetLayer\n");
            pu32Param = (MS_U32*)pArgs;
            eMP3_Layer = psAudioInstPri->fpAUDIOGetLayer();
            *((Audio_MP3_LAYER*)&(pu32Param[1])) = eMP3_Layer;
            break;
        case MApi_CMD_AUDIO_CheckInputRequest:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_CheckInputRequest\n");
            pCheckInputRequest = (PAUDIO_ST_CHECK_INPUT_REQ)pArgs;
            u8Status = psAudioInstPri->fpAUDIOCheckInputRequest((MS_U32*)pCheckInputRequest->pU32WrtAddr, (MS_U32*)pCheckInputRequest->pU32WrtBytes);
            pCheckInputRequest->u8Status = u8Status;
            break;
        case MApi_CMD_AUDIO_SetInput:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetInput\n");
            psAudioInstPri->fpAUDIOSetInput();
            break;
        case MApi_CMD_AUDIO_SetSampleRateIndex:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSampleRateIndex\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSampleRateIndex((MS_U16)pu32Param[0]);
            break;
#if 0
        case MApi_CMD_AUDIO_SetXPCMParam:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetXPCMParam\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetXPCMParam((XPCM_TYPE)pu32Param[0], (MS_U8)pu32Param[1], (MS_U16)pu32Param[2], (MS_U8)pu32Param[3], (MS_U16)pu32Param[4], (MS_U16)pu32Param[5]);
            break;
#endif
        case MApi_CMD_AUDIO_FileEndNotification:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_FileEndNotification\n");
            psAudioInstPri->fpAUDIOFileEndNotification();
            break;
        case MApi_CMD_AUDIO_FileEndDataHandle:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_FileEndDataHandle\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOFileEndDataHandle((MS_U32)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_GetPlayTick:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetPlayTick\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOGetPlayTick();
            *((MS_U32*)&(pu32Param[1])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetEsMEMCnt:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetEsMEMCnt\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status= psAudioInstPri->fpAUDIOGetEsMEMCnt();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_SetASFParm:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetASFParm\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetASFParm((WMA_ASF_PARMTYPE)pu32Param[0], (MS_U32)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_MM_SetInput:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_MM_SetInput\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOMMSetInput((En_DVB_AudioDecoder)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_MM_CheckPlayDone:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_MM_CheckPlayDone\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOMMCheckPlayDone((En_DVB_AudioDecoder)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
        case MApi_CMD_AUDIO_MM_CheckInputRequest:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_MM_CheckInputRequest\n");
            pMMCheckInputRequest = (PAUDIO_ST_MM_CHECK_INPUT_REQ)pArgs;
            u8Status = psAudioInstPri->fpAUDIOMMCheckInputRequest((En_DVB_AudioDecoder)pMMCheckInputRequest->AUDDecoderNo, (MS_U32*)pMMCheckInputRequest->pU32WrtAddr, (MS_U32*)pMMCheckInputRequest->pU32WrtBytes);
            pMMCheckInputRequest->u8Status = u8Status;
            break;
        case MApi_CMD_AUDIO_DmaReader_Init:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DmaReader_Init\n");
            psAudioInstPri->fpAUDIODmaReaderInit((PCM_OUTPUT_FORMAT*)pArgs);
            break;
        case MApi_CMD_AUDIO_DmaReader_AllInput_Init:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DmaReader_AllInput_Init\n");
            psAudioInstPri->fpAUDIODmaReaderAllInputInit();
            break;
        case MApi_CMD_AUDIO_DmaReader_WritePCM:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DmaReader_WritePCM\n");
            pDmaReaderWritePCM = (PAUDIO_ST_DMAREADER_WRITEPCM)pArgs;
            bStatus = psAudioInstPri->fpAUDIODmaReaderWritePCM(pDmaReaderWritePCM->u8Idx, (void*)pDmaReaderWritePCM->pBuffer, (MS_U32)pDmaReaderWritePCM->bytes);
            pDmaReaderWritePCM->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_SwDmaReader_Init:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DmaReader_WritePCM\n");
            psAudioInstPri->fpAUDIOSwDmaReaderInit((PCM_OUTPUT_FORMAT*)pArgs);
            break;
        case MApi_CMD_AUDIO_SwDmaReader_WritePCM:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SwDmaReader_WritePCM\n");
            pSwDmaReader_WritePCM = (PAUDIO_ST_SWDMAREADER_WRITEPCM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSwDmaReaderWritePCM(pSwDmaReader_WritePCM->u8Idx, pSwDmaReader_WritePCM->buffer, pSwDmaReader_WritePCM->bytes);
            pSwDmaReader_WritePCM->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_DmaWrite_Init:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DmaWrite_Init\n");
            psAudioInstPri->fpAUDIODmaWriterInit();
            break;
        case MApi_CMD_AUDIO_USBPCM_Enable:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_USBPCM_Enable\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOUSBPCMEnable((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_USBPCM_SetFlag:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_USBPCM_SetFlag\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOUSBPCMSetFlag((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_USBPCM_GetFlag:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_USBPCM_GetFlag\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOUSBPCMGetFlag();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_USBPCM_GetMemInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_USBPCM_GetMemInfo\n");
            psAudioInstPri->fpAUDIOUSBPCMGetMemInfo((AUDIO_UPLOAD_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_PCMCapture_Init:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCMCapture_Init\n");
            pPCMCapture_Init = (PAUDIO_ST_PCMCAPTURE_INIT)pArgs;
            bStatus = psAudioInstPri->fpAUDIOPCMCaptureInit((const AUDIO_DEVICE_TYPE)pPCMCapture_Init->eID, (const AUDIO_CAPTURE_SOURCE_TYPE)pPCMCapture_Init->eSource);
            pPCMCapture_Init->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_PCMCapture_Start:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCMCapture_Start\n");
            pPCMCapture_Init = (PAUDIO_ST_PCMCAPTURE_INIT)pArgs;
            bStatus = psAudioInstPri->fpAUDIOPCMCaptureStart((const AUDIO_DEVICE_TYPE)pPCMCapture_Init->eID);
            pPCMCapture_Init->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_PCMCapture_Stop:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCMCapture_Stop\n");
            pPCMCapture_Init = (PAUDIO_ST_PCMCAPTURE_INIT)pArgs;
            bStatus = psAudioInstPri->fpAUDIOPCMCaptureStop((const AUDIO_DEVICE_TYPE)pPCMCapture_Init->eID);
            pPCMCapture_Init->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_PCMCapture_Read:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCMCapture_Stop\n");
            pPCMCapture_Read = (PAUDIO_ST_PCMCAPTURE_READ)pArgs;
            bStatus = psAudioInstPri->fpAUDIOPCMCaptureRead((const AUDIO_DEVICE_TYPE)pPCMCapture_Read->eID, (void *)pPCMCapture_Read->pBuffer, (const MS_U32)pPCMCapture_Read->bytes);
            pPCMCapture_Read->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_VoIP_Config:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_VoIP_Config\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOVoIPConfig((AUDIO_VoIP_CONFIG_TYPE)pu32Param[1], (MS_U32)pu32Param[2], (MS_U32)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_ALSA_Check:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ALSA_Check\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOALSACheck();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_ALSA_Enable:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ALSA_Enable\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOALSAEnable((MS_BOOL)pu32Param[1]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_UNI_CheckDecodeDone:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_UNI_CheckDecodeDone\n");
            pUNICheck_DecodeDone = (PAUDIO_ST_UNI_CHECK_DECODEDONE)pArgs;
            bStatus = psAudioInstPri->fpAUDIOUNICheckDecodeDone((MS_U32*)pUNICheck_DecodeDone->pU32WrtAddr, (MS_U32*)pUNICheck_DecodeDone->pU32WrtBytes);
            pUNICheck_DecodeDone->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_UNI_SetOutput:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_UNI_SetOutput\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOUNISetOutput(pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOUNISetPCMInputWriteAddr(pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_UNI_Get_OutPCMLevel:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_UNI_Get_OutPCMLevel\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status= psAudioInstPri->fpAUDIOUNIGetOutPCMLevel();
            *((MS_U32*)&(pu32Param[1])) = u32Status;
            break;
        case MApi_CMD_AUDIO_RingTask:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_RingTask\n");
            psAudioInstPri->fpAUDIORingTask((audioRingTask*)pArgs);
            break;
        case MApi_CMD_AUDIO_Ring_DataTransfer:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_Ring_DataTransfer\n");
            pRing_Data_Trans = (PAUDIO_ST_RING_DATA_TRANS)pArgs;
            psAudioInstPri->fpAUDIORingDataTransfer((audioRingTask*)pRing_Data_Trans->pauRingTask, (audioRingTask*)pRing_Data_Trans->pauRingTask_1);
            break;
#if 0
        case MApi_CMD_AUDIO_MM2_InitAesInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_MM2_InitAesInfo\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOMM2InitAesInfo((AUDIO_DEC_ID)pu32Param[1]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
#endif
        case MApi_CMD_AUDIO_MM2_CheckAesInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_MM2_CheckAesInfo\n");
            pMM2_CheckAesInfo = (PAUDIO_ST_MM2_CHECKAESINFO)pArgs;
            bStatus = psAudioInstPri->fpAUDIOMM2CheckAesInfo((AUDIO_DEC_ID)pMM2_CheckAesInfo->dec_id, (AES_INFO*)pMM2_CheckAesInfo->paes_info);
            break;
        case MApi_CMD_AUDIO_MM2_InputAesFinished:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_MM2_InputAesFinished\n");
            pMM2_InputAes_Finish = (PAUDIO_ST_MM2_INPUTAES_FINISH)pArgs;
            bStatus = psAudioInstPri->fpAUDIOMM2InputAesFinished((AUDIO_DEC_ID)pMM2_InputAes_Finish->dec_id, (MS_U32)pMM2_InputAes_Finish->es_size,
                        (MS_BOOL)pMM2_InputAes_Finish->ptsExist, (MS_U64)pMM2_InputAes_Finish->pts);
            pMM2_InputAes_Finish->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAudioParam2:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetAudioParam2\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetAudioParam2((AUDIO_DEC_ID)pu32Param[1], (Audio_ParamType)pu32Param[2], (MS_U32)pu32Param[3]);
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_GetAudioInfo2:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetAudioInfo2\n");
            pGetAudioInfo2 = (PAUDIO_ST_GETAUDIOINFO2)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetAudioInfo2((AUDIO_DEC_ID)pGetAudioInfo2->DecId, (Audio_InfoType)pGetAudioInfo2->infoType, (void*)pGetAudioInfo2->pInfo);
            pGetAudioInfo2->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_GetDDRInfo:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDDRInfo\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOGetDDRInfo((AUDIO_DEC_ID)pu32Param[1], (EN_AUDIO_DDRINFO)pu32Param[2]);
           *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;

        // LG add function
#if 0
        case MApi_CMD_AUDIO_SetBalance:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDDRInfo\n");
            pSetBalance = (PAUDIO_ST_SETBALABCE)pArgs;
            if(pSetBalance->bLeft == TRUE)
            {
                psAudioInstPri->fpAUDIOSetBalanceL(pSetBalance->u32BalanceValue);
            }
            else
            {
                psAudioInstPri->fpAUDIOSetBalanceR(pSetBalance->u32BalanceValue);
            }
            break;
#endif
        case MApi_CMD_AUDIO_RWReg:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDDRInfo\n");
            pRWReg = (PAUDIO_ST_RWReg)pArgs;
            if(pRWReg->bRead == TRUE)
            {
                pRWReg->u16Ret = psAudioInstPri->fpAUDIOReadReg(pRWReg->u32RegAddr);
            }
            else
            {
                psAudioInstPri->fpAUDIOWriteReg(pRWReg->u32RegAddr, pRWReg->u16Val);
            }
            break;
#if 0
        case MApi_CMD_AUDIO_SetCutBooost:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDDRInfo\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetCutBoost((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_ES_STOP_THRESHOLD:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_ES_STOP_THRESHOLD\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOESStopThreshold(pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetSPDIF_HW_Mute:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetSPDIF_HW_Mute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSPDIFHwMute((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetARC_HW_Mute:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetARC_HW_Mute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetARCHwMute((MS_U8)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_SetARC_HWEN:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetARC_HWEN\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOARCHwEn((MS_BOOL)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetARC_HWEN_By_Port:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetARC_HWEN_By_Port\n");
            pARCHwenByPort = (PAUDIO_ST_ARC_HWEN_BY_PORT)pArgs;
            psAudioInstPri->fpAUDIOARCHwEnByPort(pARCHwenByPort->hdmiIndex, pARCHwenByPort->bOnOff);
            break;

        case MApi_CMD_AUDIO_LoadAudio_Clip:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_LoadAudio_Clip\n");
            pLoadAudioClip = (PAUDIO_ST_LOAD_AUDIO_CLIP)pArgs;
            psAudioInstPri->fpAUDIOLoadAudioClip(pLoadAudioClip->bufSize, pLoadAudioClip->clipBufPtr, pLoadAudioClip->dsp_sel);
            break;

        case MApi_CMD_AUDIO_Clip_OP:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_Clip_OP\n");
            pClip_op = (PAUDIO_ST_CLIP)pArgs;
            if(pClip_op->eType == CLIP_PLAY_AUDIO)
            {
                psAudioInstPri->fpAUDIOPlayAudioClip(pClip_op->u32Input);
            }
            if(pClip_op->eType == CLIP_GET_POSITION)
            {
                u32Status = psAudioInstPri->fpAUDIOGetClipPosition();
                pClip_op->u32Ret = u32Status;
            }
            if(pClip_op->eType == CLIP_SEEK_AUDIO)
            {
                psAudioInstPri->fpAUDIOSeekAudioClip(pClip_op->u32Input);
            }
            if(pClip_op->eType == CLIP_STOP_AUDIO)
            {
                psAudioInstPri->fpAUDIOStopAudioClip();
            }
            if(pClip_op->eType == CLIP_PLAY_DONE)
            {
                u32Status = psAudioInstPri->fpAUDIOClipPlayDone();
                pClip_op->u32Ret = u32Status;
            }
            if(pClip_op->eType == CLIP_PLAY_DONE_RESET)
            {
                psAudioInstPri->fpAUDIOPlayDoneReset();
            }
            if(pClip_op->eType == CLIP_PROCESSOR)
            {
                psAudioInstPri->fpAUDIOClipProcessor();
            }
            break;

        case MApi_CMD_AUDIO_SIF_AnalogMode_OP:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_AnalogMode_OP\n");
            pSIF_AnalogMode_op = (PAUDIO_ST_SIF_ANALOGMODE_OP)pArgs;
            if(pSIF_AnalogMode_op->eType == SIF_GET_CUR_ANALOG_MODE)
            {
                psAudioInstPri->fpAUDIOSIFGetCurAnalogMode(&(pSIF_AnalogMode_op->u8GetVal));
            }
            else
            {
                psAudioInstPri->fpAUDIOSIFSetUserAnalogMode(pSIF_AnalogMode_op->u16SetVal);
            }
            break;

        case MApi_CMD_AUDIO_SIF_SetMode_Setup_OP:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetMode_Setup_OP\n");
            pSIF_SetModeSetUP_op = (PAUDIO_ST_SIF_SETMODE_SETUP_OP)pArgs;
            if(pSIF_SetModeSetUP_op->eType == SIF_SETMODE_SETUP)
            {
                bStatus = psAudioInstPri->fpAUDIOSIFSetModeSetup(pSIF_SetModeSetUP_op->u8Input);
                pSIF_SetModeSetUP_op->bRet = bStatus;
            }
            if(pSIF_SetModeSetUP_op->eType == SIF_SETHDEV_MODE)
            {
                psAudioInstPri->fpAUDIOSIFSetHDEVMode(pSIF_SetModeSetUP_op->u8Input);
            }
            if(pSIF_SetModeSetUP_op->eType == SIF_SETBAND_SETUP)
            {
                bStatus = psAudioInstPri->fpAUDIOSIFSetBandSetup(pSIF_SetModeSetUP_op->u8Input);
                pSIF_SetModeSetUP_op->bRet = bStatus;
            }
            break;

        case MApi_CMD_AUDIO_SIF_GetBand_OP:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_GetBand_OP\n");
            pSIF_GetBand_op = (PAUDIO_ST_SIF_GETBAND_OP)pArgs;
            if(pSIF_GetBand_op->eType == SIF_GET_BAND_DETECT)
            {
                bStatus = psAudioInstPri->fpAUDIOSIFGetBandDetect(pSIF_GetBand_op->SoundSystem, pSIF_GetBand_op->pbandStrength);
            }
            if(pSIF_GetBand_op->eType == SIF_GET_BAND_DETECT_AMP)
            {
                bStatus = psAudioInstPri->fpAUDIOSIFGetBandDetectAmp(pSIF_GetBand_op->SoundSystem, pSIF_GetBand_op->pbandStrength);
            }
            pSIF_GetBand_op->bRet = bStatus;
            break;

        case MApi_CMD_AUDIO_SIF_CheckAvailableSystem:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_CheckAvailableSystem\n");
            pSIF_CheckAvailableSystem = (PAUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSIFCheckAvailableSystem(pSIF_CheckAvailableSystem->standard, pSIF_CheckAvailableSystem->pexist);
            pSIF_CheckAvailableSystem->bRet = bStatus;
            break;

        case MApi_CMD_AUDIO_SIF_CheckA2DK:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_CheckA2DK\n");
            pSIF_CheckA2DK = (PAUDIO_ST_SIF_CHECK_A2DK)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSIFCheckA2KD(pSIF_CheckA2DK->SystemType, pSIF_CheckA2DK->pexist);
            pSIF_CheckA2DK->bRet = bStatus;
            break;

        case MApi_CMD_AUDIO_SIF_GetSoundStandard:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_GetSoundStandard\n");
            pSIF_GetSoundStandard = (PAUDIO_ST_SIF_GETSOUNDSTANDARD)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSIFGetSoundStandard(pSIF_GetSoundStandard->B_SifStandard);
            pSIF_GetSoundStandard->bRet = bStatus;
            break;

        case MApi_CMD_AUDIO_SIF_RedoSetStandard:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_RedoSetStandard\n");
            psAudioInstPri->fpAUDIOSIFRedoSetStandard();
            break;
#if 0
        case MApi_CMD_AUDIO_SIF_ADC_Reset:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SIF_ADC_Reset\n");
            psAudioInstPri->fpAUDIOSIFADCRest();
            break;
#endif
        case MApi_CMD_AUDIO_GetBtscA2StereoLevel:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetBtscA2StereoLevel\n");
            pGetBtsc_A2Stereo_level =(PAUDIO_ST_GETBTSC_A2STEREO_LEVEL)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetBtscA2StereoLevel(pGetBtsc_A2Stereo_level->pLevel);
            pGetBtsc_A2Stereo_level->bRet = bStatus;
            break;

        case MApi_CMD_AUDIO_SetBtscA2ThresholdLevel:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetBtscA2ThresholdLevel\n");
            pSetBtsc_A2Threshold_level = (PAUDIO_ST_SETBTSCA2THRESHOLDLEVEL)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetBtscA2ThresholdLevel(pSetBtsc_A2Threshold_level->thresholdLevel, pSetBtsc_A2Threshold_level->pRF2NSR_map);
            pSetBtsc_A2Threshold_level->bRet = bStatus;
            break;

        case MApi_CMD_AUDIO_CheckNicamDigital:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_CheckNicamDigital\n");
            pCheckNicamDigial = (PAUDIO_ST_CHECKNICAMDIGITAL)pArgs;
            psAudioInstPri->fpAUDIOCheckNicamDigital(pCheckNicamDigial->pisNicamDetect);
            break;

        case MApi_CMD_AUDIO_SetThreshold_Type:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_SetThreshold_Type\n");
            pSetThresholdType = (PAUDIO_ST_SETTHRESHOLDTYPE)pArgs;
            psAudioInstPri->fpAUDIOSetThresholdType(pSetThresholdType->ThrTbl, pSetThresholdType->num, pSetThresholdType->standardtype);
            break;

        case MApi_CMD_AUDIO_DecodeFramCnt:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_DecodeFramCnt\n");
            pDecodeFrameCnt = (PAUDIO_ST_DECODE_FRAME_CNT)pArgs;
            u32Status = psAudioInstPri->fpAUDIODecodeFrameCnt(pDecodeFrameCnt->bDspType);
            pDecodeFrameCnt->u32Ret = u32Status;
            break;

        case MApi_CMD_AUDIO_PcmMixer_Init:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Init\n");
            pPcmMixerInit = (PAUDIO_ST_PCMMIXER_INIT)pArgs;
            pPcmMixerInit->s32Ret = psAudioInstPri->fpAUDIOPcmMixerInit();
            break;

        case MApi_CMD_AUDIO_PcmMixer_DeInit:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_DeInit\n");
            pPcmMixerDeInit = (PAUDIO_ST_PCMMIXER_DEINIT)pArgs;
            pPcmMixerDeInit->s32Ret = psAudioInstPri->fpAUDIOPcmMixerDeInit();
            break;

        case MApi_CMD_AUDIO_PcmMixer_Open:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Open\n");
            pPcmMixerOpen = (PAUDIO_ST_PCMMIXER_OPEN)pArgs;
            pPcmMixerOpen->s32Ret = psAudioInstPri->fpAUDIOPcmMixerOpen();
            break;

        case MApi_CMD_AUDIO_PcmMixer_Close:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Close\n");
            pPcmMixerClose = (PAUDIO_ST_PCMMIXER_CLOSE)pArgs;
            pPcmMixerClose->s32Ret = psAudioInstPri->fpAUDIOPcmMixerClose(pPcmMixerClose->s32PcmMixingId);
            break;

        case MApi_CMD_AUDIO_PcmMixer_Flush:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Flush\n");
            pPcmMixerFlush = (PAUDIO_ST_PCMMIXER_FLUSH)pArgs;
            pPcmMixerFlush->s32Ret = psAudioInstPri->fpAUDIOPcmMixerFlush(pPcmMixerFlush->s32PcmMixingId);
            break;

        case MApi_CMD_AUDIO_PcmMixer_Start:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Start\n");
            pPcmMixerStart = (PAUDIO_ST_PCMMIXER_START)pArgs;
            pPcmMixerStart->s32Ret = psAudioInstPri->fpAUDIOPcmMixerStart(pPcmMixerStart->s32PcmMixingId);
            break;

        case MApi_CMD_AUDIO_PcmMixer_Stop:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Stop\n");
            pPcmMixerStop = (PAUDIO_ST_PCMMIXER_STOP)pArgs;
            pPcmMixerStop->s32Ret = psAudioInstPri->fpAUDIOPcmMixerStop(pPcmMixerStop->s32PcmMixingId);
            break;

        case MApi_CMD_AUDIO_PcmMixer_Set:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Set\n");
            pPcmMixerSet = (PAUDIO_ST_PCMMIXER_SET)pArgs;
            pPcmMixerSet->s32Ret = psAudioInstPri->fpAUDIOPcmMixerSet(pPcmMixerSet->s32PcmMixingId, pPcmMixerSet->u32Cmd, pPcmMixerSet->pData);
            break;

        case MApi_CMD_AUDIO_PcmMixer_Get:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Get\n");
            pPcmMixerGet = (PAUDIO_ST_PCMMIXER_GET)pArgs;
            pPcmMixerGet->s32Ret = psAudioInstPri->fpAUDIOPcmMixerGet(pPcmMixerGet->s32PcmMixingId, pPcmMixerGet->u32Cmd, pPcmMixerGet->pData);
            break;

        case MApi_CMD_AUDIO_PcmMixer_Write:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Write\n");
            pPcmMixerWrite = (PAUDIO_ST_PCMMIXER_WRITE)pArgs;
            pPcmMixerWrite->bRet = psAudioInstPri->fpAUDIOPcmMixerWrite(pPcmMixerWrite->s32PcmMixingId, pPcmMixerWrite->pBuf, pPcmMixerWrite->u32Size);
            break;

        case MApi_CMD_AUDIO_PcmMixer_Func:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PcmMixer_Func\n");
            pPcmMixerFunc = (PAUDIO_ST_PCMMIXER_FUNC)pArgs;
            pPcmMixerFunc->s32Ret = psAudioInstPri->fpAUDIOPcmMixerFunc(pPcmMixerFunc->u32Arg);
            break;

        case MApi_CMD_AUDIO_GetHDMIINChannelStatus:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetHDMIINChannelStatus\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status= psAudioInstPri->fpAUDIOHDMIRXGetHDMIInChannelStatus();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;

        case MApi_CMD_AUDIO_PCM_Open:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCM_Open\n");
            pPcmOpen = (PAUDIO_ST_PCM_OPEN)pArgs;
            pPcmOpen->s32Ret = psAudioInstPri->fpAUDIOPcmOpen(pPcmOpen->pData);
            break;

        case MApi_CMD_AUDIO_PCM_Close:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCM_Close\n");
            pPcmClose = (PAUDIO_ST_PCM_CLOSE)pArgs;
            pPcmClose->s32Ret = psAudioInstPri->fpAUDIOPcmClose(pPcmClose->s32DeviceId);
            break;

        case MApi_CMD_AUDIO_PCM_Start:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCM_Start\n");
            pPcmStart = (PAUDIO_ST_PCM_START)pArgs;
            pPcmStart->s32Ret = psAudioInstPri->fpAUDIOPcmStart(pPcmStart->s32DeviceId);
            break;

        case MApi_CMD_AUDIO_PCM_Stop:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCM_Stop\n");
            pPcmStop = (PAUDIO_ST_PCM_STOP)pArgs;
            pPcmStop->s32Ret = psAudioInstPri->fpAUDIOPcmStop(pPcmStop->s32DeviceId);
            break;

        case MApi_CMD_AUDIO_PCM_Set:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCM_Set\n");
            pPcmSet = (PAUDIO_ST_PCM_SET)pArgs;
            pPcmSet->s32Ret = psAudioInstPri->fpAUDIOPcmSet(pPcmSet->s32DeviceId, pPcmSet->u32Cmd, pPcmSet->pData);
            break;

        case MApi_CMD_AUDIO_PCM_Get:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCM_Get\n");
            pPcmGet = (PAUDIO_ST_PCM_GET)pArgs;
            pPcmGet->s32Ret = psAudioInstPri->fpAUDIOPcmGet(pPcmGet->s32DeviceId, pPcmGet->u32Cmd, pPcmGet->pData);
            break;

        case MApi_CMD_AUDIO_PCM_Read:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCM_Read\n");
            pPcmRead = (PAUDIO_ST_PCM_READ)pArgs;
            pPcmRead->u32Ret = psAudioInstPri->fpAUDIOPcmRead(pPcmRead->s32DeviceId, pPcmRead->pBuf, pPcmRead->u32Size);
            break;

        case MApi_CMD_AUDIO_PCM_Write:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCM_Write\n");
            pPcmWrite = (PAUDIO_ST_PCM_WRITE)pArgs;
            pPcmWrite->u32Ret = psAudioInstPri->fpAUDIOPcmWrite(pPcmWrite->s32DeviceId, pPcmWrite->pBuf, pPcmWrite->u32Size);
            break;

        case MApi_CMD_AUDIO_PCM_Flush:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_PCM_Flush\n");
            pPcmFlush = (PAUDIO_ST_PCM_FLUSH)pArgs;
            pPcmFlush->s32Ret = psAudioInstPri->fpAUDIOPcmFlush(pPcmFlush->s32DeviceId);
            break;

#if 0
        case MApi_CMD_AUDIO_GetLIBVer:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetLIBVer\n");
            psAudioInstPri->fpAUDIOGetLibVer();
            break;

        case MApi_CMD_AUDIO_GetDSPPTS:
            AUDIO_MSG("AUDIOIoctl - MApi_CMD_AUDIO_GetDSPPTS\n");
            pu64Param = (MS_U64*)pArgs;
            ulong= psAudioInstPri->fpAUDIOGetDSPPTS();
            *((long long*)&(pu64Param[1])) = ulong;
            break;
#endif
        default:
            AUDIO_MSG("AUDIOIoctl - Unknown commend!!!\n");
            return UTOPIA_STATUS_FAIL;

    }

IOCTRL_END:

    UtopiaResourceRelease(pResource);

    return UTOPIA_STATUS_SUCCESS;
#else
    return UTOPIA_STATUS_SUCCESS;
#endif //UTOPIA_STRIP
}

MS_U32 AUDIOClose(void* pInstance)
{

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

