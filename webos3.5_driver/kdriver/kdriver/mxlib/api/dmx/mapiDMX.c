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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/errno.h>
#else
#include <stdio.h>
#include <string.h>
#endif

#include "MsTypes.h"
#include "apiDMX.h"
#include "apiDMX_private.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define DMX_MSG(x)                           // x

#if 1

#define TSPlog	printf

#else

#ifdef MSOS_TYPE_LINUX_KERNEL

#define TSPlog(fmt, ...) { struct file *filep; char buf[255];  \
filep=(struct file *)filp_open("/tmp/tsplog",O_CREAT | O_APPEND | O_RDWR,0);  \
memset(buf, 0x00, 255);  \
sprintf(buf, fmt, ##__VA_ARGS__);   \
filep->f_op->write(filep,buf,strlen(buf),&filep->f_pos);  \
filp_close(filep,0);  \
}

#else

#define TSPlog(fmt, ...) { FILE *fp;  \
fp = fopen("/tmp/tsplog", "a+t");  \
fprintf(fp, fmt, ##__VA_ARGS__);   \
fclose(fp);  \
}

#endif

#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

enum
{
    DMX_POOL_ID_DMX0,
} eDmxPoolID;


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _DMXProbeResourceToUtopia(DMX_INSTANT_PRIVATE *pDmxInstant)
{
#ifndef UTOPIA_STRIP  

    pDmxInstant->fpDMXInitLibRes =                      MApi_DMX_InitLibResource;
    pDmxInstant->fpDMXSetFWBuf =                        _MApi_DMX_SetFW;
    pDmxInstant->fpDMXSetFWMiuDataAddr =                _MApi_DMX_SetFwDataAddr;
    pDmxInstant->fpDMXSetHK =                           _MApi_DMX_SetHK;
    pDmxInstant->fpDMXInit =                            _MApi_DMX_Init;
    pDmxInstant->fpDMXTSPInit =                         _MApi_DMX_TSPInit;
    pDmxInstant->fpDMXExit =                            _MApi_DMX_Exit;
    pDmxInstant->fpDMXForceExit =                       _MApi_DMX_ForceExit;
    pDmxInstant->fpDMXSuspend =                         _MApi_DMX_Suspend;
    pDmxInstant->fpDMXResume =                          _MApi_DMX_Resume;
    pDmxInstant->fpDMXChkFwAlive =                      _MApi_DMX_ChkAlive;
    pDmxInstant->fpDMXReset =                           _MApi_DMX_Reset;
    pDmxInstant->fpDMXReleaseHwSemp =                   _MApi_DMX_ReleaseSemaphone;
    pDmxInstant->fpDMXWProtectEnable =                  _MApi_DMX_WriteProtect_Enable;
    pDmxInstant->fpDMXOrzWProtectEnable =               _MApi_DMX_OrzWriteProtect_Enable;
    pDmxInstant->fpDMXReadDropCount =                   _MApi_DMX_Read_DropPktCnt;
    pDmxInstant->fpDMXSetPowerState =                   _MApi_DMX_SetPowerState;

    pDmxInstant->fpDMXSetOwner =                        _MApi_DMX_SetOwner;
    pDmxInstant->fpDMXGetCap =                          _MApi_DMX_GetCap;
    pDmxInstant->fpDMXGetCapEx =                        _MApi_DMX_GetCap_Ex;
    pDmxInstant->fpDMXParlInvert =                      _MApi_DMX_Parl_Invert;
    pDmxInstant->fpDMXSetBurstLen =                     _MApi_DMX_SetBurstLen;

    pDmxInstant->fpDMXFlowSet =                         _MApi_DMX_FlowSet;
    pDmxInstant->fpDMXPvrFlowSet =                      _MApi_DMX_PVR_FlowSet;
    pDmxInstant->fpDMXGetFlowInputSts =                 _MApi_DMX_Get_FlowInput_Status;
    pDmxInstant->fpDMXFlowEnable =                      _MApi_DMX_FlowEnable;
    pDmxInstant->fpDMXTsOutpadCfg =                     _MApi_DMX_TsOutputPadCfg;
    pDmxInstant->fpDMXTsS2POutPhase =                   _MApi_DMX_TsS2POutputClkPhase;
    pDmxInstant->fpDMXFlowDscmbEng =                    _MApi_DMX_Flow_DscmbEng;
    pDmxInstant->fpDMXDropScmbPkt =                     _MApi_DMX_DropScmbPkt;

    pDmxInstant->fpDMX64bitModeEn =                     _MApi_DMX_STC64_Mode_Enable;
    pDmxInstant->fpDMXGetPcrEng =                       _MApi_DMX_Pcr_Eng_Get;
    pDmxInstant->fpDMXGetStcEng =                       _MApi_DMX_Stc_Eng_Get;
    pDmxInstant->fpDMXSetStcEng =                       _MApi_DMX_Stc_Eng_Set;
    pDmxInstant->fpDMXStcUpdateCtrl =                   _MApi_DMX_STC_UpdateCtrl;
    pDmxInstant->fpDMXSetStcOffset =                    _MApi_DMX_Stc_Eng_SetOffset;
    pDmxInstant->fpDMXStcClkAdjust =                    _MApi_DMX_Stc_Clk_Adjust;
    pDmxInstant->fpDMXStcSel =                          _MApi_DMX_Stc_Select;

    pDmxInstant->fpDMXOpen =                            _MApi_DMX_Open;
    pDmxInstant->fpDMXClose =                           _MApi_DMX_Close;
    pDmxInstant->fpDMXStart =                           _MApi_DMX_Start;
    pDmxInstant->fpDMXStop =                            _MApi_DMX_Stop;
    pDmxInstant->fpDMXInfo =                            _MApi_DMX_Info;
    pDmxInstant->fpDMXPid =                             _MApi_DMX_Pid;
    pDmxInstant->fpDMXIsStart =                         _MApi_DMX_IsStart;
    pDmxInstant->fpDMXCopyData =                        _MApi_DMX_CopyData;
    pDmxInstant->fpDMXProc =                            _MApi_DMX_Proc;
    pDmxInstant->fpDMXChangeFltSrc =                    _MApi_DMX_Change_FilterSource;
    pDmxInstant->fpDMXSwitchLiveFltSrc =                _MApi_DMX_LiveSrcSwitch;
    pDmxInstant->fpDMXGetOnwer =                        _MApi_DMX_GetOwner;

    pDmxInstant->fpDMXGetFltScmbSts =                   _MApi_DMX_Get_FltScmbSts;
    pDmxInstant->fpDMXGetPesScmbSts =                   _MApi_DMX_Get_PesScmbSts;
    pDmxInstant->fpDMXGetTsScmbSts =                    _MApi_DMX_Get_TsScmbSts;

    pDmxInstant->fpDMXSecReset =                        _MApi_DMX_SectReset;
    pDmxInstant->fpDMXSecSetReadAddr =                  _MApi_DMX_SectReadSet;
    pDmxInstant->fpDMXSecGetReadAddr =                  _MApi_DMX_SectReadGet;
    pDmxInstant->fpDMXSecGetWriteAddr =                 _MApi_DMX_SectWriteGet;
    pDmxInstant->fpDMXSecGetStartAddr =                 _MApi_DMX_SectStartGet;
    pDmxInstant->fpDMXSecGetEndAddr =                   _MApi_DMX_SectEndGet;
    pDmxInstant->fpDMXSecSetPattern =                   _MApi_DMX_SectPatternSet;

    pDmxInstant->fpDMXGetTTXWrite =                     _MApi_DMX_TTX_WriteGet;
    pDmxInstant->fpDMXGetAccess =                       _MApi_DMX_GetAccess;
    pDmxInstant->fpDMXReleaseAccess =                   _MApi_DMX_ReleaseAccess;

    pDmxInstant->fpDMXAVFifoReset =                     _MApi_DMX_AVFifo_Reset;
    pDmxInstant->fpDMXAVFifoStatus =                    _MApi_DMX_AVFifo_Status;
    pDmxInstant->fpDMXRemoveDupAVPkt =                  _MApi_DMX_RemoveDupAVPkt;
    pDmxInstant->fpDMXRemoveDupAVFifoPkt =              _MApi_DMX_RemoveDupAVFifoPkt;
    pDmxInstant->fpDMXAUBDModeEn =                      _MApi_DMX_AU_BD_Mode_Enable;
    pDmxInstant->fpDMXSetPktMode =                      _MApi_DMX_SetPacketMode;
    pDmxInstant->fpDMXSetMerStrSyc =                    _MApi_DMX_SetMergeStrSyncByte;

    pDmxInstant->fpDMXFileSetPlayStamp =                _MApi_DMX_Filein_Eng_SetPlaybackStamp;
    pDmxInstant->fpDMXFileGetPlayStamp =                _MApi_DMX_Filein_Eng_GetPlaybackStamp;
    pDmxInstant->fpDMXFileTimestampEnable =             _MApi_DMX_Filein_Eng_PlaybackTimeStampEnable;
    pDmxInstant->fpDMXFileTimestampDisable =            _MApi_DMX_Filein_Eng_PlaybackTimeStampDisable;

    pDmxInstant->fpDMXPvrEngOpen =                      _MApi_DMX_Pvr_Eng_Open;
    pDmxInstant->fpDMXPvrEngClose =                     _MApi_DMX_Pvr_Eng_Close;
    pDmxInstant->fpDMXPvrEngPidOpen =                   _MApi_DMX_Pvr_Eng_Pid_Open;
    pDmxInstant->fpDMXPvrEngPidClose =                  _MApi_DMX_Pvr_Eng_Pid_Close;
    pDmxInstant->fpDMXPvrEngStart =                     _MApi_DMX_Pvr_Eng_Start;
    pDmxInstant->fpDMXPvrEngStop =                      _MApi_DMX_Pvr_Eng_Stop;
    pDmxInstant->fpDMXPvrEngGetWriteAddr =              _MApi_DMX_Pvr_Eng_WriteGet;
    pDmxInstant->fpDMXPvrEngSetPktMode =                _MApi_DMX_Pvr_Eng_SetPacketMode;
    pDmxInstant->fpDMXPvrEngSetRecordStamp =            _MApi_DMX_Pvr_Eng_SetRecordStamp;
    pDmxInstant->fpDMXPvrEngGetRecordStamp =            _MApi_DMX_Pvr_Eng_GetRecordStamp;
    pDmxInstant->fpDMXPvrEngMobfEn =                    _MApi_DMX_Pvr_Eng_MOBF_Enable;
    pDmxInstant->fpDMXPvrEngSetRecordStampClk =         _MApi_DMX_Pvr_Eng_SetRecordStampClk;
    pDmxInstant->fpDMXPvrEngCBSize            =         _MApi_DMX_Pvr_Eng_CallbackSize;
    pDmxInstant->fpDMXPvrEngSetCaMode         =         _MApi_DMX_Pvr_Eng_SetCaMode;
    pDmxInstant->fpDMXPvrIsStart =                      _MApi_DMX_Pvr_Eng_IsStart;

    pDmxInstant->fpDMXFileEngSetPlayStampClk =          _MApi_DMX_Pvr_Eng_SetPlaybackStampClk;

    pDmxInstant->fpDMXPvrcaEngPidOpen =                 _MApi_DMX_PvrCA_Eng_Pid_Open;
    pDmxInstant->fpDMXPvrcaEngPidClose =                _MApi_DMX_PvrCA_Eng_Pid_Close;
    pDmxInstant->fpDMXPvrcaEngStart =                   _MApi_DMX_PvrCA_Eng_Start;
    pDmxInstant->fpDMXPvrcaEngStop =                    _MApi_DMX_PvrCA_Eng_Stop;

    pDmxInstant->fpDMXFileStart =                       _MApi_DMX_Filein_Eng_Start;
    pDmxInstant->fpDMXFileStop =                        _MApi_DMX_Filein_Eng_Stop;
    pDmxInstant->fpDMXFileInfo =                        _MApi_DMX_Filein_Info;
    pDmxInstant->fpDMXFileEngInfo =                     _MApi_DMX_Filein_Eng_Info;
    pDmxInstant->fpDMXFilePause =                       _MApi_DMX_Filein_Eng_Pause;
    pDmxInstant->fpDMXFileResume =                      _MApi_DMX_Filein_Eng_Resume;
    pDmxInstant->fpDMXFileIsIdle =                      _MApi_DMX_Filein_Eng_IsIdle;
    pDmxInstant->fpDMXFileIsBusy =                      _MApi_DMX_Filein_Eng_IsBusy;
    pDmxInstant->fpDMXFileIsPause =                     _MApi_DMX_Filein_Eng_IsPause;
    pDmxInstant->fpDMXFileCmdQReset =                   _MApi_DMX_Filein_Eng_CMDQ_Reset;
    pDmxInstant->fpDMXFileCmdQGetEmptyNum =             _MApi_DMX_Filein_Eng_CMDQ_GetEmptyNum;
    pDmxInstant->fpDMXFileBypassFileTimeStamp =         _MApi_DMX_Filein_Eng_BypassFileInTimeStamp;
    pDmxInstant->fpDMXFileCmdQGetFifoWLevel =           _MApi_DMX_Filein_Eng_CMDQ_FIFOWriteLevel;
    pDmxInstant->fpDMXFileGetFileTimestamp =            _MApi_DMX_Filein_Eng_GetFileInTimeStamp;
    pDmxInstant->fpDMXFileGetReadAddr =                 _MApi_DMX_Filein_Eng_GetReadAddr;
    pDmxInstant->fpDMXFileMobfEn =                      _MApi_DMX_Filein_Eng_MOBF_Enable;

    pDmxInstant->fpDMXMMFIIsIdle =                      _MApi_DMX_MMFI_Filein_IsIdle;
    pDmxInstant->fpDMXMMFIIsBusy =                      _MApi_DMX_MMFI_Filein_IsBusy;
    pDmxInstant->fpDMXMMFICmdQReset =                   _MApi_DMX_MMFI_Filein_CMDQ_Reset;
    pDmxInstant->fpDMXMMFIGetEmptyNum =                 _MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum;
    pDmxInstant->fpDMXMMFIStart =                       _MApi_DMX_MMFI_Filein_Start;
    pDmxInstant->fpDMXMMFIGetFileTimestamp =            _MApi_DMX_MMFI_GetFileInTimeStamp;

    pDmxInstant->fpDMXMMFIPidOpen =                     _MApi_DMX_MMFI_Pid_Open;
    pDmxInstant->fpDMXMMFIPidClose =                    _MApi_DMX_MMFI_Pid_Close;
    pDmxInstant->fpDMXMMFIGetFifoLevel =                _MApi_DMX_MMFI_Filein_CMDQ_FIFOWriteLevel;
    pDmxInstant->fpDMXMMFISetPlayTimestamp =            _MApi_DMX_MMFI_SetPlaybackTimeStamp;
    pDmxInstant->fpDMXMMFIGetPlayTimestamp =            _MApi_DMX_MMFI_GetPlaybackTimeStamp;
    pDmxInstant->fpDMXMMFISetTimestampClk =             _MApi_DMX_MMFI_TimeStampClk;
	pDmxInstant->fpDMXMMFIInfo = 						_MApi_DMX_MMFI_Filein_Info;
	pDmxInstant->fpDMXMMFIBypassStamp = 				_MApi_DMX_MMFI_Filein_BypassTimeStamp;
    pDmxInstant->fpDMXMMFIRemoveDupAVPkt =              _MApi_DMX_MMFI_RemoveDupAVPkt;
    pDmxInstant->fpDMXMMFIMobfEn =                      _MApi_DMX_MMFI_MOBF_Enable;
	pDmxInstant->fpDMXMMFITimestampEn = 				_MApi_DMX_MMFI_TimeStampEnable;	
	pDmxInstant->fpDMXMMFITimestampDisable = 			_MApi_DMX_MMFI_TimeStampDisable;

    pDmxInstant->fpDMXTsoFileInfo =                     _MApi_DMX_TSO_Filein_Info;
    pDmxInstant->fpDMXTsoFileIsIdle =                   _MApi_DMX_TSO_Filein_IsIdle;
    pDmxInstant->fpDMXTsoFileGetCmdQEmptyNum =          _MApi_DMX_TSO_Filein_CMDQ_GetEmptyNum;
    pDmxInstant->fpDMXTsoFileGetCmdQReset =             _MApi_DMX_TSO_Filein_CMDQ_Reset;
    pDmxInstant->fpDMXTsoFileStart =                    _MApi_DMX_TSO_Filein_Start;
    pDmxInstant->fpDMXTsoFileStop =                     _MApi_DMX_TSO_Filein_Stop;
    pDmxInstant->fpDMXTsoFileSetPlayStamp =             _MApi_DMX_TSO_SetPlaybackTimeStamp;
    pDmxInstant->fpDMXTsoFileGetPlayStamp =             _MApi_DMX_TSO_GetPlaybackStamp;
    pDmxInstant->fpDMXTsoFileGetTimeStamp =             _MApi_DMX_TSO_GetFileInTimeStamp;
    pDmxInstant->fpDMXTsoFileBypassStamp =              _MApi_DMX_TSO_BypassFileInTimeStamp;
    pDmxInstant->fpDMXTsoFileTimeStampEnable =          _MApi_DMX_TSO_TimeStampEnable;
    pDmxInstant->fpDMXTsoFileTimeStampDisable =         _MApi_DMX_TSO_TimeStampDisable;
    pDmxInstant->fpDMXTsoSetOutClk =                    _MApi_DMX_TSO_SetOutClk;
	pDmxInstant->fpDMXTsoOutputEn = 					_MApi_DMX_TSO_OutputEnable;
    pDmxInstant->fpDMXTsoLocStrId =                     _MApi_DMX_TSO_LocalStreamId;
    pDmxInstant->fpDMXTsoSvqBufSet =                    _MApi_DMX_TSO_SVQBuf_Set;
	pDmxInstant->fpDMXTsoFlowInputCfg = 				_MApi_DMX_TSO_Flow_InputCfg;
	pDmxInstant->fpDMXTsoFlowOutputCfg = 				_MApi_DMX_TSO_Flow_OutputCfg;
    pDmxInstant->fpDMXTsoConfigure =                    _MApi_DMX_TSO_Configure;

    pDmxInstant->fpDMXSetDbgLevel =                     _MApi_DMX_SetDbgLevel;
    pDmxInstant->fpDMXGetFwVer =                        _MApi_TSP_Get_FW_VER;
    pDmxInstant->fpDMXGetLibVer =                       _MApi_DMX_GetLibVer;
    pDmxInstant->fpDMXGetDisConCnt =                    _MApi_DMX_Get_DisContiCnt;
    pDmxInstant->fpDMXGetDropPktCnt =                   _MApi_DMX_Get_DropPktCnt;
    pDmxInstant->fpDMXGetLockPktCnt =                   _MApi_DMX_Get_LockPktCnt;
    pDmxInstant->fpDMXGetAVPktCnt =                     _MApi_DMX_Get_AVPktCnt;
    pDmxInstant->fpDMXGetSecTeiPktCnt =                 _MApi_DMX_Get_SecTEI_PktCount;
    pDmxInstant->fpDMXResetSecTeiPktCnt =               _MApi_DMX_Reset_SecTEI_PktCount;
    pDmxInstant->fpDMXGetSecDisConPktCnt =              _MApi_DMX_Get_SecDisCont_PktCount;
    pDmxInstant->fpDMXResetSecDisContPktCnt =           _MApi_DMX_Reset_SecDisCont_PktCount;

    pDmxInstant->fpDMXCmdRun =                          _MApi_DMX_CMD_Run;

    pDmxInstant->fpDMXFQSetFltRushPass =                _MApi_DMX_FQ_SetFltRushPass;  
    pDmxInstant->fpDMXFQInit =                          _MApi_DMX_FQ_Init;    
    pDmxInstant->fpDMXFQExit =                          _MApi_DMX_FQ_Exit;    
    pDmxInstant->fpDMXFQRushEn =                        _MApi_DMX_FQ_RushEnable;
    pDmxInstant->fpDMXFQSkipRushData =                  _MApi_DMX_FQ_SkipRushData;

    pDmxInstant->fpDMXMStrSyncBy =                      _MApi_DMX_MStr_SyncByte;
    
    pDmxInstant->fpDMXWaitTspSig =                      _MApi_DMX_Wait_TspIoSignal;

    pDmxInstant->fpDMXGetIntCnt =                       _MApi_DMX_Get_Intr_Count;
    pDmxInstant->fpDMXDropEn =                          _MApi_DMX_Drop_Enable;
    pDmxInstant->fpDMXTeiRmErrPkt =                     _MApi_DMX_TEI_RemoveErrorPkt;      
    pDmxInstant->fpDMXSetFwDbgParam =                   _MApi_DMX_SetFwDbgParam;
    pDmxInstant->fpDMXPvrMobfEn =                       _MApi_DMX_Pvr_MOBF_Enable;
	pDmxInstant->fpDMXGetDbgportInfo = 					_MApi_DMX_GetDbgPortInfo;
	pDmxInstant->fpDMXOpenMultiFlt = 					_MApi_DMX_Open_MultiFlt;

	pDmxInstant->fpDMXFilePvrPidOpen = 					_MApi_DMX_Filein_Pvr_Eng_Pid_Open;
	pDmxInstant->fpDMXFilePvrPidClose = 				_MApi_DMX_Filein_Pvr_Eng_Pid_Close;
	pDmxInstant->fpDMXFilePvrStart = 					_MApi_DMX_Filein_Pvr_Eng_Start;
	pDmxInstant->fpDMXFilePvrStop = 					_MApi_DMX_Filein_Pvr_Eng_Stop;

	pDmxInstant->fpDMXPvrPause = 						_MApi_DMX_Pvr_Eng_Pause;
	pDmxInstant->fpDMXPvrPid = 							_MApi_DMX_Pvr_Eng_Pid;
#endif //undef UTOPIA_STRIP
    
    return TRUE;

}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void DMXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    void* pUtopiaModule = NULL;
    void* psResource = NULL;
    MS_U32 u32ResSize = 0;

    // 1. deal with module    
    UtopiaModuleCreate(MODULE_DMX, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)DMXOpen, (FUtopiaClose)DMXClose, (FUtopiaIOctl)DMXIoctl);

    // 2. deal with resource   
    UtopiaModuleAddResourceStart(pUtopiaModule, DMX_POOL_ID_DMX0);

#ifndef UTOPIA_STRIP     
    _MApi_DMX_GetCap(DMX_CAP_RESOURCE_SIZE, (void*)(&u32ResSize));
#endif

    UtopiaResourceCreate("DMX", u32ResSize, &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, DMX_POOL_ID_DMX0);
    UtopiaModuleAddResourceEnd(pUtopiaModule, DMX_POOL_ID_DMX0);
}

MS_U32 DMXOpen(void** ppInstance, void* pAttribute)
{   
    DMX_INSTANT_PRIVATE*    psDmxInstPri = NULL;

    DMX_MSG(printf("DMX open \n"));

    if(ppInstance == 0)
    {
        return UTOPIA_STATUS_PARAMETER_ERROR;
    }
    
    UtopiaInstanceCreate(sizeof(DMX_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psDmxInstPri);

    if(_DMXProbeResourceToUtopia(psDmxInstPri) == FALSE)
    {
        DMX_MSG(printf("DMX module version is not supported \n"));
        return UTOPIA_STATUS_NOT_SUPPORTED;
    }

    return UTOPIA_STATUS_SUCCESS;

}

MS_U32 DMXIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    DMX_INSTANT_PRIVATE* psDmxInstPri = NULL;
    void* pModule = NULL;
    void* pResource = NULL;
    void* pResPrivate = NULL;
    MS_U32 u32Res = UTOPIA_STATUS_SUCCESS;

    MS_U32*                             pu32Param           = NULL;
    DMX_TSPParam*                       pTSPParam           = NULL;
    PDMX_CAP_PARAM                      pCapParam           = NULL;
    PDMX_CAP_EX_PARAM                   pCapExParam         = NULL;
    PDMX_FLOW_PARAM                     pFlowParam          = NULL;
    PDMX_PVRFLOW_PARAM                  pPvrFlowParam       = NULL;
    PDMX_FLT_TYPE_PARAM                 pFltTypeParam       = NULL;
    PDMX_FLT_INFO_PARAM                 pFltInfoParam       = NULL;
    PDMX_FLT_COPY_PARAM                 pFltCopyParam       = NULL;
    PDMX_FLT_EVENT_PARAM                pFltEventParam      = NULL;
    PDMX_FLT_SEC_PAT_PARAM              pFltSecPatParam     = NULL;
    PDMX_AVFIFO_PARAM                   pAVFifoParam        = NULL;
    PDMX_AVFIFO_DROP_PARAM              pAVFifoDropParam    = NULL;
    PDMX_TIMESTAMP_CLK_PARAM            pStampClkParam      = NULL;
    PDMX_PVR_INFO_PARAM                 pPvrInfoParam       = NULL;
    PDMX_PVR_FLTTYPE_PARAM              pPvrFltTypeParam    = NULL;
    PDMX_FILE_START_PARAM               pFileStartParam     = NULL;
    PDMX_FILE_INFO_PARAM                pFileInfoParam      = NULL;
    PDMX_MMFI_FLT_PARAM                 pMMFIFltParam       = NULL;
    PDMX_CMDRUN_PARAM                   pCmdRunParam        = NULL;
    PDMX_POWSTATE_PARAM                 pCmdPowerState      = NULL;
    PDMX_MMFI_START_PARAM               pMMFIStartParam     = NULL;
    PDMX_PKT_MODE_PARAM                 pPktModeParam       = NULL;
    PDMX_PKT_DISCONCNT_PARAM            pPktDisConCntParam  = NULL;
    PDMX_PKT_DROPCNT_PARAM              pPktDropCntParam    = NULL;
    PDMX_PKT_LOCKCNT_PARAM              pPktLockCntParam    = NULL;
    PDMX_PKT_AVCNT_PARAM                pPktAVCNTParam      = NULL;
    PDMX_WP_PARAM                       pWPParam            = NULL;
    PDMX_PKT_SECTEI_PARAM               pPktSecTeiParam     = NULL;
    PDMX_TSO_OUTCLK_PARAM               pTsoOutClkParam     = NULL;
    PDMX_TSO_LOCSTRID_PARAM             pTsoLocStrId        = NULL;
    PDMX_FQ_INIT_PARAM                  pFQInitParam        = NULL;
    PDMX_FQ_SKIP_RUSH_DATA_PARAM        pFQSkipRushData     = NULL;
    PDMX_MSTR_SYNCBY_PARAM              pMStrSync           = NULL;
    DMX_TSP_IOSIGNAL*                   pstTspSig           = NULL;
    DMX_OutputPad_Cfg*                  pstOutPadCfg        = NULL;
    PDMX_FLOW_DSCMBENG_PARAM            pstFlowDscmbEng     = NULL;
    PDMX_DROP_SCMB_PKT_PARAM            pstDropScmbPkt      = NULL;

    if(pInstance == NULL)
    {
        printf("DMXIoctl - pInstance is NULL!!! \n");
        return UTOPIA_STATUS_FAIL;
    }
    
    //wait forever events, don't block mutex...
    if(u32Cmd == MApi_CMD_DMX_WaitTspCbEvt)
    {
        DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_WaitTspCbEvt\n"));
        pstTspSig = (DMX_TSP_IOSIGNAL*)pArgs;
        #ifdef MSOS_TYPE_LINUX_KERNEL
        
        #ifndef UTOPIA_STRIP        
        if(_MApi_DMX_Wait_TspIoSignal(pstTspSig) != DMX_FILTER_STATUS_OK)
        {
            pstTspSig->u32DmxEvt = 0;
            return -ERESTARTSYS;
        }
        return UTOPIA_STATUS_SUCCESS;
        #else
        {
            return UTOPIA_STATUS_FAIL;
        }
        #endif

        #endif //MSOS_TYPE_LINUX_KERNEL
    }

    if(UtopiaInstanceGetPrivate(pInstance, (void*)&psDmxInstPri) != 0)
    {
        printf("DMXIoctl - UtopiaInstanceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }    
    if(UtopiaInstanceGetModule(pInstance, &pModule) != 0)
    {
        printf("DMXIoctl - UtopiaInstanceGetModule fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    if(UtopiaResourceObtain(pModule, DMX_POOL_ID_DMX0, &pResource) != 0)
    {
        printf("DMXIoctl - UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    if(UtopiaResourceGetPrivate(pResource, &pResPrivate) != 0)
    {
        printf("DMXIoctl - UtopiaResourceGetPrivatefail\n");
        goto IOCTRL_END;
    }

    // init share resource of driver and assign share mem pointer to process
    if(psDmxInstPri->fpDMXInitLibRes(pResPrivate) != DMX_FILTER_STATUS_OK)
    {
        printf("DMXIoctl - DMX init share memory failed \n");
        u32Res = UTOPIA_STATUS_ERR_USER;
        goto IOCTRL_END;
    }    

    switch(u32Cmd)
    {
        // General API
        case MApi_CMD_DMX_Init:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Init\n"));

            pTSPParam = (DMX_TSPParam*)pArgs;
            if(psDmxInstPri->fpDMXSetFWBuf(pTSPParam->phyFWAddr, pTSPParam->u32FWSize) != DMX_FILTER_STATUS_OK)
            {
                DMX_MSG(printf("DMX Set FW failed \n"));
                pTSPParam->u32Res = (MS_U32)DMX_FILTER_STATUS_ERROR;
                break;
            }
            if(psDmxInstPri->fpDMXSetHK((MS_BOOL)pTSPParam->u32IsHK) != DMX_FILTER_STATUS_OK)
            {
                DMX_MSG(printf("DMX Set HK failed \n"));
                pTSPParam->u32Res = (MS_U32)DMX_FILTER_STATUS_ERROR;
                break;
            }
            if(psDmxInstPri->fpDMXInit() != DMX_FILTER_STATUS_OK)
            {
                DMX_MSG(printf("DMX Set init failed \n)"));
                pTSPParam->u32Res = (MS_U32)DMX_FILTER_STATUS_ERROR;
                break;
            }
            pTSPParam->u32Res = (MS_U32)psDmxInstPri->fpDMXTSPInit(pTSPParam);
            break;

        case MApi_CMD_DMX_Exit:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Exit\n)"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[0] = (MS_U32)psDmxInstPri->fpDMXExit();
            break;

        case MApi_CMD_DMX_ForceExit:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_ForceExit\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[0] = (MS_U32)psDmxInstPri->fpDMXForceExit();
            break;

        case MApi_CMD_DMX_Suspend:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Suspend\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[0] = (MS_U32)psDmxInstPri->fpDMXSuspend();
            break;

        case MApi_CMD_DMX_Resume:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Resume\n"));
            pTSPParam = (DMX_TSPParam*)pArgs;
            pTSPParam->u32Res = (MS_U32)psDmxInstPri->fpDMXResume(pTSPParam->phyFWAddr, pTSPParam->u32FWSize);
            break;

        case MApi_CMD_DMX_ChkFwAlive:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_ChkFwAlive\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[0] = (MS_U32)psDmxInstPri->fpDMXChkFwAlive();
            break;

        case MApi_CMD_DMX_Reset:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Reset\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[0] = (MS_U32)psDmxInstPri->fpDMXReset();
            break;

        case MApi_CMD_DMX_SetFwMiuDataAddr:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetFwMiuDataAddr\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXSetFWMiuDataAddr(pu32Param[0], pu32Param[1]);
            break;

        case MApi_CMD_DMX_WProtectEnable:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_WProtectEnable\n"));
            pWPParam = (PDMX_WP_PARAM)pArgs;
            psDmxInstPri->fpDMXWProtectEnable(pWPParam->bEnable, pWPParam->pu32StartAddr, pWPParam->pu32EndAddr);
            break;
            
        case MApi_CMD_DMX_OrzWProtectEnable:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_OrzWProtectEnable\n"));
            pu32Param = (MS_U32*)pArgs;
            psDmxInstPri->fpDMXOrzWProtectEnable((MS_BOOL)pu32Param[0], pu32Param[1], pu32Param[2]);
            break;

        case MApi_CMD_DMX_ReadDropCount:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_ReadDropCount\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXReadDropCount((MS_U16*)&(pu32Param[0]), (MS_U16*)&(pu32Param[1]));
            break;

        case MApi_CMD_DMX_SetPowerState:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetPowerState\n"));
            pCmdPowerState = (PDMX_POWSTATE_PARAM)pArgs;
            pCmdPowerState->u32Res = (MS_U32)psDmxInstPri->fpDMXSetPowerState(pCmdPowerState->u16PowerState, pCmdPowerState->u32FWAddr, pCmdPowerState->u32FWSize);
            break;

        case MApi_CMD_DMX_SetOwner:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetOwner\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXSetOwner((MS_U8)(pu32Param[0]), (MS_U8)(pu32Param[1]), (MS_BOOL)(pu32Param[2]));
            break;

        case MApi_CMD_DMX_GetCap:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetCap\n"));
            pCapParam = (PDMX_CAP_PARAM)pArgs;
            pCapParam->u32Res = (MS_U32)psDmxInstPri->fpDMXGetCap(pCapParam->Quetype, pCapParam->pdata);
            break;

        case MApi_CMD_DMX_GetCapEx:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetCapEx\n"));
            pCapExParam = (PDMX_CAP_EX_PARAM)pArgs;
            pCapExParam->u32Res = (MS_U32)psDmxInstPri->fpDMXGetCapEx(pCapExParam->StrQuetype, pCapExParam->InputStrLen, pCapExParam->pdata, &(pCapExParam->OutputSize));
            break;    

        case MApi_CMD_DMX_SetBurstLen:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetBurstLen\n"));
            psDmxInstPri->fpDMXSetBurstLen(*((DMX_BURSTTYPE*)pArgs));
            break;

        //Flow Control
        case MApi_CMD_DMX_Flow:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Flow\n"));
            pFlowParam = (PDMX_FLOW_PARAM)pArgs;
            if(pFlowParam->bSet == 1)
            {
                pFlowParam->u32Res = (MS_U32)psDmxInstPri->fpDMXFlowSet(pFlowParam->DmxFlow, pFlowParam->DmxFlowInput, pFlowParam->bClkInv,
                            pFlowParam->bExtSync, pFlowParam->bParallel);
            }
            else
            {
                pFlowParam->u32Res = (MS_U32)psDmxInstPri->fpDMXGetFlowInputSts(pFlowParam->DmxFlow, &(pFlowParam->DmxFlowInput),
                            &(pFlowParam->bClkInv), &(pFlowParam->bExtSync), &(pFlowParam->bParallel));
            }
            break;

        case MApi_CMD_DMX_PVR_Flow:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_PVR_Flow\n"));
            pPvrFlowParam = (PDMX_PVRFLOW_PARAM)pArgs;
            if(pPvrFlowParam->bSet)
            {
                pPvrFlowParam->u32Res = (MS_U32)psDmxInstPri->fpDMXPvrFlowSet(pPvrFlowParam->eEng, pPvrFlowParam->eSrcTSIf, pPvrFlowParam->bDscmbRec);
            }
            else
            {
                pPvrFlowParam->u32Res = 1;
                u32Res = UTOPIA_STATUS_NOT_SUPPORTED;
                printf("Commend does not support yet! \n");
            }
            break;

        case MApi_CMD_DMX_Flow_ParlInvert:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Flow_ParlInvert\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXParlInvert((DMX_FLOW)pu32Param[0], (MS_BOOL)pu32Param[1]);
            break;

        case MApi_CMD_DMX_OutPadCfg:   
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_OutPadCfg\n"));
            pstOutPadCfg = (PDMX_OutputPad_Cfg)pArgs;
            pstOutPadCfg->u32Res = (MS_U32)psDmxInstPri->fpDMXTsOutpadCfg(pstOutPadCfg->eOutPad, pstOutPadCfg->eInSrcPad, 
                pstOutPadCfg->bInParallel, pstOutPadCfg->u32ResvNum, pstOutPadCfg->pu32Resv);
            break;

        case MApi_CMD_DMX_TsOutPhase:   
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TsOutPhase\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXTsS2POutPhase((MS_U16)pu32Param[0], (MS_BOOL)pu32Param[1], pu32Param[2]);
            break;                

        case MApi_CMD_DMX_FlowDscmbEng:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FlowDscmbEng\n"));
            pstFlowDscmbEng = (PDMX_FLOW_DSCMBENG_PARAM)pArgs;
            pstFlowDscmbEng->u32Res = psDmxInstPri->fpDMXFlowDscmbEng(pstFlowDscmbEng->eTsif, &pstFlowDscmbEng->u32DscmbEng, pstFlowDscmbEng->bSet);
            break;
        case MApi_CMD_DMX_DropScmbPkt:
            DMX_MSG(printf("DMXIoctl - MApi_DMX_DropScmbPkt\n"));
            pstDropScmbPkt = (PDMX_DROP_SCMB_PKT_PARAM)pArgs;
            pstDropScmbPkt->u32Res = (MS_U32)psDmxInstPri->fpDMXDropScmbPkt(pstDropScmbPkt->eType, pstDropScmbPkt->bEnable);
            break;
        case MApi_CMD_DMX_Flow_ENABLE:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Flow_ENABLE\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFlowEnable((DMX_FLOW)pu32Param[0], (MS_BOOL)pu32Param[1]);
            break;

        //STC API
        case MApi_CMD_DMX_GetPcr:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetPcr\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXGetPcrEng((MS_U8)(pu32Param[0]),&(pu32Param[1]), &(pu32Param[2]));
            break;

        case MApi_CMD_DMX_Stc_Get:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Stc_Get\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXGetStcEng((MS_U8)(pu32Param[0]), &(pu32Param[1]), &(pu32Param[2]));
            break;

        case MApi_CMD_DMX_Stc_Set:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Stc_Set\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXSetStcEng((MS_U8)(pu32Param[0]), pu32Param[1], pu32Param[2]);
            break;

        case MApi_CMD_DMX_Stc_UpdateCtrl:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Stc_UpdateCtrl\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXStcUpdateCtrl((MS_U8)(pu32Param[0]), (eStcUpdateCtrlMode)pu32Param[1]);
            break;

        case MApi_CMD_DMX_Stc_SetOffset:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Stc_SetOffset\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXSetStcOffset((MS_U32)(pu32Param[0]), (MS_U32)(pu32Param[1]), (MS_BOOL)pu32Param[2]);
            break;

        case MApi_CMD_DMX_Stc_ClkAdjust:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Stc_ClkAdjust\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXStcClkAdjust(pu32Param[0], (MS_BOOL)pu32Param[1], pu32Param[2]);
            break;

        //Filter API
        case MApi_CMD_DMX_Open:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Open\n"));
            pFltTypeParam = (PDMX_FLT_TYPE_PARAM)pArgs;
            pFltTypeParam->u32Res = (MS_U32)psDmxInstPri->fpDMXOpen(pFltTypeParam->DmxFltType, (MS_U8*)(&(pFltTypeParam->u32DmxId)));
            break;

        case MApi_CMD_DMX_Close:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Close\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXClose((MS_U8)(pu32Param[0]));
            break;

        case MApi_CMD_DMX_Start:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Start\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXStart((MS_U8)(pu32Param[0]));
            break;

        case MApi_CMD_DMX_Stop:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Stop\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXStop((MS_U8)(pu32Param[0]));
            break;

        case MApi_CMD_DMX_Info:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Info\n"));
            pFltInfoParam = (PDMX_FLT_INFO_PARAM)pArgs;
            pFltInfoParam->u32Res = (MS_U32)psDmxInstPri->fpDMXInfo((MS_U8)(pFltInfoParam->u32DmxId),
                pFltInfoParam->pDmxFltInfo, pFltInfoParam->pDmxFltType, pFltInfoParam->bSet);
            break;

        case MApi_CMD_DMX_Pid:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pid\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXPid((MS_U8)(pu32Param[0]), (MS_U16*)(&(pu32Param[1])), (MS_BOOL)(pu32Param[2]));
            break;

        case MApi_CMD_DMX_IsStart:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_IsStart\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXIsStart((MS_U8)(pu32Param[0]), (MS_BOOL*)(&(pu32Param[1])));
            break;

        case MApi_CMD_DMX_CopyData:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_CopyData\n"));
            pFltCopyParam = (PDMX_FLT_COPY_PARAM)pArgs;
            pFltCopyParam->u32Res = (MS_U32)psDmxInstPri->fpDMXCopyData((MS_U8)(pFltCopyParam->u32DmxId), pFltCopyParam->pu8Buf,
                pFltCopyParam->u32BufSize, &(pFltCopyParam->u32ActualSize), &(pFltCopyParam->u32RmnSize),
                pFltCopyParam->pfCheckCB);
            break;

        case MApi_CMD_DMX_Proc:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Proc\n"));
            pFltEventParam = (PDMX_FLT_EVENT_PARAM)pArgs;
            pFltEventParam->u32Res = (MS_U32)psDmxInstPri->fpDMXProc((MS_U8)(pFltEventParam->u32DmxId), pFltEventParam->pEvent);
            break;

        case MApi_CMD_DMX_ChangeFltSrc:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_ChangeFltSrc\n"));
            pFltTypeParam = (PDMX_FLT_TYPE_PARAM)pArgs;
            pFltTypeParam->u32Res = (MS_U32)psDmxInstPri->fpDMXChangeFltSrc((MS_U8)(pFltTypeParam->u32DmxId), pFltTypeParam->DmxFltType);
            break;

        case MApi_CMD_DMX_GetOwner:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetOwner\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXGetOnwer((MS_U8)(pu32Param[0]), (MS_BOOL*)(&(pu32Param[1])));
            break;

        case MApi_CMD_DMX_GetFltScmbSts:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetFltScmbSts\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[4] = (MS_U32)psDmxInstPri->fpDMXGetFltScmbSts(pu32Param[0], pu32Param[1], pu32Param[2], &(pu32Param[3]));
            break;

        case MApi_CMD_DMX_GetPESScmbSts:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetPESScmbSts\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXGetPesScmbSts((MS_U8)pu32Param[0], (MS_U8*)(&(pu32Param[1])));
            break;

        case MApi_CMD_DMX_GetTsScmbSts:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetTsScmbSts\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXGetTsScmbSts((MS_U8)pu32Param[0], (MS_U8*)(&(pu32Param[1])));
            break;

        // Section API
        case MApi_CMD_DMX_Sec_Reset:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Sec_Reset\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXSecReset((MS_U8)(pu32Param[0]));
            break;

        case MApi_CMD_DMX_Sec_ReadAddr:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Sec_ReadAddr\n"));
            pu32Param = (MS_U32*)pArgs;
            if(pu32Param[2] == 1)
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXSecSetReadAddr((MS_U8)(pu32Param[0]), pu32Param[1]);
            }
            else
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXSecGetReadAddr((MS_U8)(pu32Param[0]), &(pu32Param[1]));
            }
            break;

        case MApi_CMD_DMX_Sec_GetWriteAddr:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Sec_GetWriteAddr\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXSecGetWriteAddr((MS_U8)(pu32Param[0]), &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_Sec_GetStartAddr:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Sec_GetStartAddr\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXSecGetStartAddr((MS_U8)(pu32Param[0]), &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_Sec_GetEndAddr:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Sec_GetEndAddr\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXSecGetEndAddr((MS_U8)(pu32Param[0]), &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_Sec_SetPattern:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Sec_SetPattern\n"));
            pFltSecPatParam = (PDMX_FLT_SEC_PAT_PARAM)pArgs;
            pFltSecPatParam->u32Res = (MS_U32)psDmxInstPri->fpDMXSecSetPattern((MS_U8)(pFltSecPatParam->u32DmxId), pFltSecPatParam->pu8Pattern,
                        pFltSecPatParam->pu8Mask, pFltSecPatParam->pu8NotMask, pFltSecPatParam->u32MatchSize);
            break;

        //TTX API
        case MApi_CMD_DMX_GetTTXWrite:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetTTXWrite\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXGetTTXWrite((MS_U8)(pu32Param[0]), &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_GetAccess:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetAccess\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXGetAccess(pu32Param[0]);
            break;

        case MApi_CMD_DMX_ReleaseAccess:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_ReleaseAccess\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[0] = (MS_U32)psDmxInstPri->fpDMXReleaseAccess();
            break;

        // AVFIFO control
        case MApi_CMD_DMX_AVFIFO_Reset:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_AVFIFO_Reset\n"));
            pAVFifoParam = (PDMX_AVFIFO_PARAM)pArgs;
            pAVFifoParam->u32Res = (MS_U32)psDmxInstPri->fpDMXAVFifoReset(pAVFifoParam->DmxFltType, (MS_BOOL)(pAVFifoParam->u32Data));
            break;

        case MApi_CMD_DMX_AVFIFO_Status:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_AVFIFO_Status\n"));
            pAVFifoParam = (PDMX_AVFIFO_PARAM)pArgs;
            pAVFifoParam->u32Res = (MS_U32)psDmxInstPri->fpDMXAVFifoStatus(pAVFifoParam->DmxFltType, (DMX_FIFO_STATUS*)(&(pAVFifoParam->u32Data)));
            break;

        case MApi_CMD_DMX_RemoveDupAVFifoPkt:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_RemoveDupAVFifoPkt\n"));
            pAVFifoDropParam = (PDMX_AVFIFO_DROP_PARAM)pArgs;
            if(pAVFifoDropParam->bAllFifo)
            {
                pAVFifoDropParam->u32Res = (MS_U32)psDmxInstPri->fpDMXRemoveDupAVPkt(pAVFifoDropParam->bEnable);
            }
            else
            {
                pAVFifoDropParam->u32Res = (MS_U32)psDmxInstPri->fpDMXRemoveDupAVFifoPkt(pAVFifoDropParam->DmxFltType, pAVFifoDropParam->bEnable);
            }
            break;

        case MApi_CMD_DMX_AUBD_Enable:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_AUBD_Enable\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXAUBDModeEn((MS_BOOL)pu32Param[0]);
            break;

        //PVR Playback API
        case MApi_CMD_DMX_FI_PlayStamp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_PlayStamp\n"));
            pu32Param = (MS_U32*)pArgs;
            if(pu32Param[2] == 0)
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXFileGetPlayStamp((DMX_FILEIN_PATH)pu32Param[0], &(pu32Param[1]));
            }
            else
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXFileSetPlayStamp((DMX_FILEIN_PATH)pu32Param[0], pu32Param[1]);
            }
            break;

        case MApi_CMD_DMX_FI_TimestampMode:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_TimestampMode\n"));
            pu32Param = (MS_U32*)pArgs;
            if(pu32Param[1] == 0)
            {
                pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFileTimestampDisable((DMX_FILEIN_PATH)pu32Param[0]);
            }
            else
            {
                pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFileTimestampEnable((DMX_FILEIN_PATH)pu32Param[0]);
            }
            break;

        case MApi_CMD_DMX_FI_SetPlaybackStampClk:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_SetPlaybackStampClk\n"));
            pStampClkParam = (PDMX_TIMESTAMP_CLK_PARAM)pArgs;
            pStampClkParam->u32Res = (MS_U32)psDmxInstPri->fpDMXFileEngSetPlayStampClk((MS_U8)(pStampClkParam->u32EngId), pStampClkParam->eClk);
            break;

        // PVR Engine API
        case MApi_CMD_DMX_Pvr_Open:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_Open\n"));
            pPvrInfoParam = (PDMX_PVR_INFO_PARAM)pArgs;
            pPvrInfoParam->u32Res = (MS_U32)psDmxInstPri->fpDMXPvrEngOpen((DMX_PVR_ENG)(pPvrInfoParam->u32EngId), pPvrInfoParam->pPvrInfo);
            break;

        case MApi_CMD_DMX_Pvr_Close:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_Close\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXPvrEngClose((DMX_PVR_ENG)(pu32Param[0]));
            break;

        case MApi_CMD_DMX_Pvr_PidOpen:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_PidOpen\n"));
            pPvrFltTypeParam = (PDMX_PVR_FLTTYPE_PARAM)pArgs;
            if(pPvrFltTypeParam->u32CAEngSel == 0xFFFFFFFF)
            {
                pPvrFltTypeParam->u32Res = (MS_U32)psDmxInstPri->fpDMXPvrEngPidOpen((DMX_PVR_ENG)(pPvrFltTypeParam->u32EngId), pPvrFltTypeParam->u32Pid,
                            (MS_U8*)(&(pPvrFltTypeParam->u32DmxId)));
            }
            else
            {
                pPvrFltTypeParam->u32Res = (MS_U32)psDmxInstPri->fpDMXPvrcaEngPidOpen((DMX_PVR_ENG)(pPvrFltTypeParam->u32EngId), pPvrFltTypeParam->u32Pid,
                            (MS_U8*)(&(pPvrFltTypeParam->u32DmxId)), (MS_U8)(pPvrFltTypeParam->u32SelShareFlt));
            }
            break;

        case MApi_CMD_DMX_Pvr_PidClose:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_PidClose\n"));
            pu32Param = (MS_U32*)pArgs;
            if(pu32Param[2] == 0)
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXPvrEngPidClose((DMX_PVR_ENG)(pu32Param[0]), (MS_U8)(pu32Param[1]));
            }
            else
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXPvrcaEngPidClose((DMX_PVR_ENG)(pu32Param[0]), (MS_U8)(pu32Param[1]));
            }
            break;

        case MApi_CMD_DMX_Pvr_Start:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_Start\n"));
            pu32Param = (MS_U32*)pArgs;
            if(pu32Param[2] == 0)
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXPvrEngStart((DMX_PVR_ENG)(pu32Param[0]), (MS_BOOL)(pu32Param[1]));
            }
            else
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXPvrcaEngStart((DMX_PVR_ENG)(pu32Param[0]), (MS_BOOL)(pu32Param[1]));
            }
            break;

        case MApi_CMD_DMX_Pvr_Stop:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_Stop\n"));
            pu32Param = (MS_U32*)pArgs;
            if(pu32Param[1] == 0)
            {
                pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXPvrEngStop((DMX_PVR_ENG)(pu32Param[0]));
            }
            else
            {
                pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXPvrcaEngStop((DMX_PVR_ENG)(pu32Param[0]));
            }
            break;

        case MApi_CMD_DMX_Pvr_GetWriteAddr:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_GetWriteAddr\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXPvrEngGetWriteAddr((DMX_PVR_ENG)(pu32Param[0]), &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_Pvr_SetPktMode:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_SetPktMode\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXPvrEngSetPktMode((DMX_PVR_ENG)(pu32Param[0]), (MS_U32)(pu32Param[1]));
            break;

        case MApi_CMD_DMX_Pvr_SetRecordStamp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_SetRecordStamp\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXPvrEngSetRecordStamp((DMX_PVR_ENG)(pu32Param[0]), pu32Param[1]);
            break;

        case MApi_CMD_DMX_Pvr_GetRecordStamp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_GetRecordStamp\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXPvrEngGetRecordStamp((DMX_PVR_ENG)(pu32Param[0]), &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_Pvr_SetMobfKey:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_SetMobfKey\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[4] = (MS_U32)psDmxInstPri->fpDMXPvrEngMobfEn((DMX_PVR_ENG)(pu32Param[0]), ((pu32Param[1] || pu32Param[2]) != 0),
                        pu32Param[1], pu32Param[2]);
            break;

        case MApi_CMD_DMX_Pvr_SetRecordStampClk:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_SetRecordStampClk\n"));
            pStampClkParam = (PDMX_TIMESTAMP_CLK_PARAM)pArgs;
            pStampClkParam->u32Res = (MS_U32)psDmxInstPri->fpDMXPvrEngSetRecordStampClk((DMX_PVR_ENG)(pStampClkParam->u32EngId), pStampClkParam->eClk);
            break;

        // File-in Engine API
        case MApi_CMD_DMX_FI_Start:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_Start\n"));
            pFileStartParam = (PDMX_FILE_START_PARAM)pArgs;
            pFileStartParam->u32Res = (MS_U32)psDmxInstPri->fpDMXFileStart((DMX_FILEIN_PATH)pFileStartParam->u32EngId, pFileStartParam->Dst, pFileStartParam->pBuf, pFileStartParam->u32BufSize);
            break;

        case MApi_CMD_DMX_FI_Stop:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_Stop\n"));
			pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXFileStop((DMX_FILEIN_PATH)pu32Param[0]);
            break;

        case MApi_CMD_DMX_FI_Info:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_Info\n"));
            pFileInfoParam = (PDMX_FILE_INFO_PARAM)pArgs;
            pFileInfoParam->u32Res = (MS_U32)psDmxInstPri->fpDMXFileInfo(pFileInfoParam->pFileinInfo);
            break;

        case MApi_CMD_DMX_FI_Eng_Info:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_Eng_Info\n"));
            pFileInfoParam = (PDMX_FILE_INFO_PARAM)pArgs;
            pFileInfoParam->u32Res = (MS_U32)psDmxInstPri->fpDMXFileEngInfo((DMX_FILEIN_PATH)pFileInfoParam->u32EngId, pFileInfoParam->pFileinInfo);
            break;

        case MApi_CMD_DMX_FI_Pause:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_Pause\n"));
			pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXFilePause((DMX_FILEIN_PATH)pu32Param[0]);
            break;

        case MApi_CMD_DMX_FI_Resume:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_Resume\n"));
			pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXFileResume((DMX_FILEIN_PATH)pu32Param[0]);
            break;

        case MApi_CMD_DMX_FI_IsIdle:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_IsIdle\n"));
            pu32Param = (MS_U32*)pArgs;
            *((MS_U8*)&(pu32Param[1])) = psDmxInstPri->fpDMXFileIsIdle((DMX_FILEIN_PATH)pu32Param[0]);
            break;

        case MApi_CMD_DMX_FI_IsBusy:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_IsBusy\n"));
            pu32Param = (MS_U32*)pArgs;
            *((MS_U8*)&(pu32Param[1])) = psDmxInstPri->fpDMXFileIsBusy((DMX_FILEIN_PATH)pu32Param[0]);
            break;

        case MApi_CMD_DMX_FI_IsPause:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_IsPause\n"));
            pu32Param = (MS_U32*)pArgs;
            *((MS_U8*)&(pu32Param[1])) = psDmxInstPri->fpDMXFileIsPause((DMX_FILEIN_PATH)pu32Param[0]);
            break;

        case MApi_CMD_DMX_FI_CmdQReset:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_CmdQReset\n"));
			pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXFileCmdQReset((DMX_FILEIN_PATH)pu32Param[0]);
            break;

        case MApi_CMD_DMX_FI_CmdQGetEmptyNum:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_CmdQGetEmptyNum\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFileCmdQGetEmptyNum((DMX_FILEIN_PATH)pu32Param[0], &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_FI_BypassFileTimestamp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_BypassFileTimestamp\n"));
            pu32Param = (MS_U32*)pArgs;
            psDmxInstPri->fpDMXFileBypassFileTimeStamp((DMX_FILEIN_PATH)pu32Param[0], (MS_BOOL)(pu32Param[1]));
            break;

        case MApi_CMD_DMX_FI_CmdQGetFifoLevel:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_CmdQGetFifoLevel\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFileCmdQGetFifoWLevel((DMX_FILEIN_PATH)pu32Param[0], (MS_U8*)(&(pu32Param[1])));
            break;

        case MApi_CMD_DMX_FI_GetFileTimeStamp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_GetFileTimeStamp\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFileGetFileTimestamp((DMX_FILEIN_PATH)pu32Param[0], &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_FI_GetReadAddr:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_GetReadAddr\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFileGetReadAddr((DMX_FILEIN_PATH)pu32Param[0], &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_FI_SetMobfKey:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FI_SetMobfKey\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXFileMobfEn((DMX_FILEIN_PATH)pu32Param[0], (pu32Param[1] != 0), pu32Param[2]);
            break;

        //MMFI  API
        case MApi_CMD_DMX_MMFI_IsIdle:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_IsIdle\n"));
            pu32Param = (MS_U32*)pArgs;
            *((MS_U8*)&(pu32Param[1])) = psDmxInstPri->fpDMXMMFIIsIdle((DMX_MMFI_PATH)(pu32Param[0]));
            break;

        case MApi_CMD_DMX_MMFI_IsBusy:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_IsBusy\n"));
            pu32Param = (MS_U32*)pArgs;
            *((MS_U8*)&(pu32Param[1])) = psDmxInstPri->fpDMXMMFIIsBusy((DMX_MMFI_PATH)(pu32Param[0]));
            break;

        case MApi_CMD_DMX_MMFI_CmdQReset:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_CmdQReset\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXMMFICmdQReset((DMX_MMFI_PATH)(pu32Param[0]));
            break;

        case MApi_CMD_DMX_MMFI_GetEmptyNum:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_GetEmptyNum\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFIGetEmptyNum((DMX_MMFI_PATH)(pu32Param[0]), &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_MMFI_Start:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_Start\n"));
            pMMFIStartParam = (PDMX_MMFI_START_PARAM)pArgs;
            pMMFIStartParam->u32Res = (MS_U32)psDmxInstPri->fpDMXMMFIStart(pMMFIStartParam->Dst, pMMFIStartParam->pBuf, pMMFIStartParam->u32BufSize);
            break;

         case MApi_CMD_DMX_MMFI_GetFileTimeStamp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_GetFileTimeStamp\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFIGetFileTimestamp((DMX_MMFI_PATH)(pu32Param[0]), &(pu32Param[1]));
            break;

         case MApi_CMD_DMX_MMFI_PidOpen:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_PidOpen\n"));
            pMMFIFltParam = (PDMX_MMFI_FLT_PARAM)pArgs;
            pMMFIFltParam->u32Res = (MS_U32)psDmxInstPri->fpDMXMMFIPidOpen(pMMFIFltParam->flttype, (MS_U16)(pMMFIFltParam->u32Pid),
                        (MS_U8*)(&(pMMFIFltParam->u32DmxId)));
            break;

        case MApi_CMD_DMX_MMFI_PidClose:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_PidClose\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFIPidClose((MS_U8)(pu32Param[1]));
            break;

        case MApi_CMD_DMX_MMFI_GetFifoLevel:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_GetFifoLevel\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFIGetFifoLevel((DMX_MMFI_PATH)(pu32Param[0]), (MS_U8*)(&(pu32Param[1])));
            break;

        case MApi_CMD_DMX_MMFI_PlayStamp_Set:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_PlayStamp_Set\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFISetPlayTimestamp((DMX_MMFI_PATH)(pu32Param[0]), pu32Param[1]);
            break;

        case MApi_CMD_DMX_MMFI_PlayStamp_Get:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_PlayStamp_Get\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFIGetPlayTimestamp((DMX_MMFI_PATH)(pu32Param[0]), &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_MMFI_SetTimeStampClk:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_SetTimeStampClk\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFISetTimestampClk((DMX_MMFI_PATH)(pu32Param[0]), (DMX_TimeStamp_Clk)(pu32Param[1]));
            break;


        case MApi_CMD_DMX_MMFI_RemoveDupAVPkt:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_RemoveDupAVPkt\n"));
            pAVFifoParam = (PDMX_AVFIFO_PARAM)pArgs;
            pAVFifoParam->u32Res = (MS_U32)psDmxInstPri->fpDMXMMFIRemoveDupAVPkt((MS_BOOL)(pAVFifoParam->u32Data));
            break;

        case MApi_CMD_DMX_MMFI_SetMOBFKey:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_SetMOBFKey\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFIMobfEn((DMX_MMFI_PATH)(pu32Param[0]), (pu32Param[1] != 0), pu32Param[1]);
            break;

		case MApi_CMD_DMX_MMFI_Timestamp_En:
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_TIMESTAMP_EN\n"));
			pu32Param = (MS_U32*)pArgs;
			if(pu32Param[1] == 1)
			{
				pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFITimestampEn((DMX_MMFI_PATH)(pu32Param[0]));
			}
			else
			{
				pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXMMFITimestampDisable((DMX_MMFI_PATH)(pu32Param[0]));
			}
			break;					

        //Debug API
        case MApi_CMD_DMX_SetDbgLevel:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetDbgLevel\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXSetDbgLevel((DMX_DBGMSG_LEVEL)pu32Param[0]);
            break;

        case MApi_CMD_DMX_GetFwVer:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetFwVer\n"));
            pu32Param = (MS_U32*)pArgs;
            psDmxInstPri->fpDMXGetFwVer(pu32Param);
            break;

        case MApi_CMD_DMX_CmdRun:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_CmdRun\n"));
            pCmdRunParam = (PDMX_CMDRUN_PARAM)pArgs;
            pCmdRunParam->u32Res = (MS_U32)psDmxInstPri->fpDMXCmdRun(pCmdRunParam->u32Cmd,
                pCmdRunParam->u32Config, pCmdRunParam->u32DataNum, pCmdRunParam->pData);
            break;

        //Utopia 1.0 compatible API
        case MApi_CMD_DMX_SetFw:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetFw\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXSetFWBuf(pu32Param[0], pu32Param[1]);            
            break;

        case MApi_CMD_DMX_SetHK:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetHK\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXSetHK((MS_BOOL)pu32Param[0]);
            break;

        case MApi_CMD_DMX_Init_1_0:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Init_1_0\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[0] = (MS_U32)psDmxInstPri->fpDMXInit();
            break;

        case MApi_CMD_DMX_TSPInit_1_0:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSPInit_1_0\n"));
            pTSPParam = (DMX_TSPParam*)pArgs;
            pTSPParam->u32Res = (MS_U32)psDmxInstPri->fpDMXTSPInit(pTSPParam);
            break;

        case MApi_CMD_DMX_ReleseHWSemp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_ReleseHWSemp\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[0] = (MS_U32)psDmxInstPri->fpDMXReleaseHwSemp();
            break;

        case MApi_CMD_DMX_GetLibVer:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetLibVer\n"));
            psDmxInstPri->fpDMXGetLibVer((const MSIF_Version **)pArgs);
            break;  

        case MApi_CMD_DMX_STC64ModeEnable:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_STC64ModeEnable\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMX64bitModeEn((MS_BOOL)pu32Param[0]);
            break;

        //Merge stream API
        case MApi_CMD_DMX_SetPktMode:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetPktMode\n"));
            pPktModeParam = (PDMX_PKT_MODE_PARAM)pArgs;
            pPktModeParam->u32Res = (MS_U32)psDmxInstPri->fpDMXSetPktMode(pPktModeParam->eFlow, pPktModeParam->ePktMode);
            break;

        case MApi_CMD_DMX_SetMergeStrSync:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetMergeStrSync\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXSetMerStrSyc(pu32Param[0], (MS_U8)pu32Param[1]);
            break;

        //TSO API
        case MApi_CMD_DMX_TSO_FileInfo:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_FileInfo\n"));
            pFileInfoParam = (PDMX_FILE_INFO_PARAM)pArgs;
            pFileInfoParam->u32Res = (MS_U32)psDmxInstPri->fpDMXTsoFileInfo((MS_U8)(pFileInfoParam->u32EngId), pFileInfoParam->pFileinInfo);
            break;
        case MApi_CMD_DMX_TSO_FileIsIdle:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_FileIsIdle\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXTsoFileIsIdle((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_DMX_TSO_FileGetCmdQEmptyNum:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_FileGetCmdQEmptyNum\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXTsoFileGetCmdQEmptyNum((MS_U8)pu32Param[0], &(pu32Param[1]));
            break;

        case MApi_CMD_DMX_TSO_FileStart:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_FileStart\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXTsoFileStart((MS_U8)pu32Param[0], pu32Param[1], pu32Param[2]);
            break;
        case MApi_CMD_DMX_TSO_FileStop:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_FileStop\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXTsoFileStop((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_DMX_TSO_FilePlayStamp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_FilePlayStamp\n"));
            pu32Param = (MS_U32*)pArgs;
            if(pu32Param[2] == 1)
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXTsoFileSetPlayStamp((MS_U8)pu32Param[0], pu32Param[1]);
            }
            else
            {
                pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXTsoFileGetPlayStamp((MS_U8)pu32Param[0], &(pu32Param[1]));
            }
            break;
        case MApi_CMD_DMX_TSO_FileGetTimeStamp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_FileGetTimeStamp\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXTsoFileGetTimeStamp((MS_U8)pu32Param[0], &(pu32Param[1]));
            break;
        case MApi_CMD_DMX_TSO_FileBypassStamp:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_FileBypassStamp\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXTsoFileBypassStamp((MS_U8)pu32Param[0], (MS_BOOL)(pu32Param[1]));
            break;
        case MApi_CMD_DMX_TSO_FileTimeStampEnable:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_FileTimeStampEnable\n"));
            pu32Param = (MS_U32*)pArgs;
            if(pu32Param[1] == 1)
            {
                pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXTsoFileTimeStampEnable((MS_U8)pu32Param[0]);
            }
            else
            {
                pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXTsoFileTimeStampDisable((MS_U8)pu32Param[0]);
            }
            break;

        // DBG INFO API
        case MApi_CMD_DMX_DBG_GET_DISCONCNT:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_DBG_GET_DISCONCNT\n"));
            pPktDisConCntParam = (PDMX_PKT_DISCONCNT_PARAM)pArgs;
            pPktDisConCntParam->u32Res = (MS_U32)psDmxInstPri->fpDMXGetDisConCnt(pPktDisConCntParam->pInfo, &(pPktDisConCntParam->u32Cnt));
            break;
        case MApi_CMD_DMX_DBG_GET_DROPPKTCNT:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_DBG_GET_DROPPKTCNT\n"));
            pPktDropCntParam = (PDMX_PKT_DROPCNT_PARAM)pArgs;
            pPktDropCntParam->u32Res = (MS_U32)psDmxInstPri->fpDMXGetDropPktCnt(pPktDropCntParam->pInfo, &(pPktDropCntParam->u32Cnt));
            break;
        case MApi_CMD_DMX_DBG_GET_LOCKPKTCNT:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_DBG_GET_LOCKPKTCNT\n"));
            pPktLockCntParam = (PDMX_PKT_LOCKCNT_PARAM)pArgs;
            pPktLockCntParam->u32Res = (MS_U32)psDmxInstPri->fpDMXGetLockPktCnt(pPktLockCntParam->pInfo, &(pPktLockCntParam->u32Cnt));
            break;
        case MApi_CMD_DMX_DBG_GET_AVPKTCNT:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_DBG_GET_AVPKTCNT\n"));
            pPktAVCNTParam = (PDMX_PKT_AVCNT_PARAM)pArgs;
            pPktAVCNTParam->u32Res = (MS_U32)psDmxInstPri->fpDMXGetAVPktCnt(pPktAVCNTParam->pInfo, &(pPktAVCNTParam->u32Cnt));
            break;
        case MApi_CMD_DMX_GET_SECTEI_PKTCNT:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GET_SECTEI_PKTCNT\n"));
            pPktSecTeiParam = (PDMX_PKT_SECTEI_PARAM)pArgs;
            pPktSecTeiParam->u32Res = (MS_U32)psDmxInstPri->fpDMXGetSecTeiPktCnt(pPktSecTeiParam->eType, &(pPktSecTeiParam->u32Cnt));
            break;
        case MApi_CMD_DMX_RESET_SECTEI_PKTCNT:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_RESET_SECTEI_PKTCNT\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXResetSecTeiPktCnt((DMX_FILTER_TYPE)pu32Param[0]);
            break;
        case MApi_CMD_DMX_GET_SECDISCON_PKTCNT:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GET_SECDISCON_PKTCNT\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXGetSecDisConPktCnt(pu32Param[0], &(pu32Param[1]));
            break;
        case MApi_CMD_DMX_RESET_SECDISCON_PKTCNT:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_RESET_SECDISCON_PKTCNT\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXResetSecDisContPktCnt(pu32Param[0]);
            break;

        // BOX Series Only
        case MApi_CMD_DMX_Pvr_CBSize:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_CBSize\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXPvrEngCBSize((DMX_PVR_ENG)pu32Param[0], &(pu32Param[1]), (MS_BOOL)pu32Param[2]);
            break;
        case MApi_CMD_DMX_Pvr_SetCaMode:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_SetCaMode\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXPvrEngSetCaMode((DMX_PVR_ENG)pu32Param[0], (DMX_CA_PVRMODE)pu32Param[1], (MS_BOOL)pu32Param[2]);
            break;

        case MApi_CMD_DMX_Pvr_IsStart:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_IsStart\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXPvrIsStart((DMX_PVR_ENG)pu32Param[0], (MS_BOOL*)(&(pu32Param[1])));
            break;

        case MApi_CMD_DMX_TSO_OutClk:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_OutClk\n"));
            pTsoOutClkParam = (PDMX_TSO_OUTCLK_PARAM)pArgs;
            if(pTsoOutClkParam->bSet == TRUE)
            {
                pTsoOutClkParam->u32Res = (MS_U32)psDmxInstPri->fpDMXTsoSetOutClk((MS_U8)pTsoOutClkParam->u32Eng, pTsoOutClkParam->eTsoOutClk, pTsoOutClkParam->eTsoOutClkSrc,
                                                        pTsoOutClkParam->u16DivNum, pTsoOutClkParam->bClkInv);
            }
            else
            {
                u32Res = UTOPIA_STATUS_NOT_SUPPORTED;
                DMX_MSG(printf("DMXIoctl - Not Support\n"));
            }
            break;

        case MApi_CMD_DMX_STCEng_Sel:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_STCEng_Sel\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXStcSel(pu32Param[0], pu32Param[1]);
            break;      
            
        case MApi_CMD_DMX_TSO_LocStrId:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_LocStrId\n"));
            pTsoLocStrId = (PDMX_TSO_LOCSTRID_PARAM)pArgs;
            pTsoLocStrId->u32Res = (MS_U32)psDmxInstPri->fpDMXTsoLocStrId(pTsoLocStrId->u8Eng, pTsoLocStrId->eIf, pTsoLocStrId->pu8StrId, pTsoLocStrId->bSet);
            break;

        case MApi_CMD_DMX_TSO_CmdQReset:    
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_CmdQ\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXTsoFileGetCmdQReset(*pu32Param);
            break;

		case MApi_CMD_DMX_TSO_InputCfg:
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_InputCfg\n"));
			((DMX_TSO_InputCfg*)pArgs)->u32Res = (MS_U32)psDmxInstPri->fpDMXTsoFlowInputCfg((DMX_TSO_InputCfg*)pArgs);
			break;

		case MApi_CMD_DMX_TSO_OutputCfg:
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_OutputCfg\n"));
			((DMX_TSO_OutputCfg*)pArgs)->u32Res = (MS_U32)psDmxInstPri->fpDMXTsoFlowOutputCfg((DMX_TSO_OutputCfg*)pArgs);
			break;
        case MApi_CMD_DMX_TSO_Configure:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_Configure\n"));
            ((DMX_TSO_Cfg*)pArgs)->u32Res = (MS_U32)psDmxInstPri->fpDMXTsoConfigure((DMX_TSO_Cfg*)pArgs);
            break;
			
		case MApi_CMD_DMX_TSO_OutputEn:		
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_OutputEn\n"));
			pu32Param = (MS_U32*)pArgs;
			pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXTsoOutputEn((MS_U8)pu32Param[0], (MS_BOOL)pu32Param[1]);
			break;

        case MApi_CMD_DMX_FQ_SetFltRushPass:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FQ_SetFltRushPass\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXFQSetFltRushPass(pu32Param[0], pu32Param[1]);
            break;

        case MApi_CMD_DMX_FQ_Init:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FQ_Init\n"));
            pFQInitParam = (PDMX_FQ_INIT_PARAM)pArgs;
            pFQInitParam->u32Res = (MS_U32)psDmxInstPri->fpDMXFQInit(pFQInitParam->u32Eng,pFQInitParam->pInfo);
            break;

        case MApi_CMD_DMX_FQ_Exit:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FQ_Exit\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFQExit(pu32Param[0]);
            break;

        case MApi_CMD_DMX_FQ_RushEnable:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FQ_RushEnable\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFQRushEn(pu32Param[0]);
            break;

        case MApi_CMD_DMX_FQ_SkipRushData:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_FQ_SkipRushData\n"));
            pFQSkipRushData = (PDMX_FQ_SKIP_RUSH_DATA_PARAM)pArgs;
            pFQSkipRushData->u32Res = (MS_U32)psDmxInstPri->fpDMXFQSkipRushData(pFQSkipRushData->u32Eng, pFQSkipRushData->eSkipPath);
            break;

        case MApi_CMD_DMX_TSO_SvqBufSet:    
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_TSO_SvqBufSet\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXTsoSvqBufSet((MS_U8)pu32Param[0], pu32Param[1], pu32Param[2]);
            break;

        case MApi_CMD_DMX_MStr_SyncBy:  
            DMX_MSG("DMXIoctl - MApi_CMD_DMX_MStr_SyncBy\n");
            pMStrSync = (PDMX_MSTR_SYNCBY_PARAM)pArgs;
            pMStrSync->u32Res = (MS_U32)psDmxInstPri->fpDMXMStrSyncBy(pMStrSync->eIf, pMStrSync->u8StrId, pMStrSync->pu8SyncByte, pMStrSync->bSet);
            break;

        case MApi_CMD_DMX_GetIntCnt:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetIntCnt\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXGetIntCnt((MS_U32*)pArgs);
            break;
            
        case MApi_CMD_DMX_DropEn:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_GetIntCnt\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXDropEn((MS_BOOL)pu32Param[0]);
            break;
            
        case MApi_CMD_DMX_TeiRmErrPkt:    
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_VqEn\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXTeiRmErrPkt((DMX_TEI_RmPktType)pu32Param[0], (MS_BOOL)pu32Param[1]);
            break;            
        case MApi_CMD_DMX_SetFwDbgParam:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_SetFwDbgParam\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXSetFwDbgParam(pu32Param[0], pu32Param[1], pu32Param[2]);
            break;

        case MApi_CMD_DMX_PVR_MOBFEn:
            DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_PVR_MOBFEn\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXPvrMobfEn((MS_BOOL)pu32Param[0], pu32Param[1], pu32Param[2]);
            break;                      
            
		case MApi_CMD_DMX_MMFI_Info:		
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_Info\n"));
            pFileInfoParam = (PDMX_FILE_INFO_PARAM)pArgs;
            pFileInfoParam->u32Res = (MS_U32)psDmxInstPri->fpDMXMMFIInfo((DMX_MMFI_PATH)pFileInfoParam->u32EngId, pFileInfoParam->pFileinInfo);
            break;

		case MApi_CMD_DMX_MMFI_BypassStamp:	
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_MMFI_BypassStamp\n"));
            pu32Param = (MS_U32*)pArgs;
            psDmxInstPri->fpDMXMMFIBypassStamp((DMX_MMFI_PATH)pu32Param[0], (MS_BOOL)pu32Param[1]);
            break;

		case MApi_CMD_DMX_Get_DbgPortInfo:	
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Get_DbgPortInfo\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXGetDbgportInfo(pu32Param[0], &(pu32Param[1]));
            break;

		case MApi_CMD_DMX_Open_MultiFlt:
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Open_MultiFlt\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[3] = (MS_U32)psDmxInstPri->fpDMXOpenMultiFlt((DMX_FILTER_TYPE)pu32Param[0], (MS_U8*)&(pu32Param[1]), (MS_U8)pu32Param[2]);
            break;			
			
		case MApi_CMD_DMX_File_PVR_PidOpen:
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_File_PVR_PidOpen\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[4] = (MS_U32)psDmxInstPri->fpDMXFilePvrPidOpen((DMX_PVR_ENG)pu32Param[0], pu32Param[1], (MS_U8*)&(pu32Param[2]), (MS_U8)pu32Param[3]);
            break;		

		case MApi_CMD_DMX_File_PVR_PidClose:
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_File_PVR_PidClose\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFilePvrPidClose((DMX_PVR_ENG)pu32Param[0], (MS_U8)pu32Param[1]);
            break;			

		case MApi_CMD_DMX_File_PVR_Start:
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_File_PVR_Start\n"));
            pu32Param = (MS_U32*)pArgs;
            pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXFilePvrStart((DMX_PVR_ENG)pu32Param[0], (MS_BOOL)pu32Param[1]);
            break;			

		case MApi_CMD_DMX_File_PVR_Stop:
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_File_PVR_Stop\n"));
			pu32Param = (MS_U32*)pArgs;
			pu32Param[1] = (MS_U32)psDmxInstPri->fpDMXFilePvrStop((DMX_PVR_ENG)pu32Param[0]);
			break;		

		case MApi_CMD_DMX_Pvr_Pause:	
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_Pause\n"));
			pu32Param = (MS_U32*)pArgs;
			pu32Param[2] = (MS_U32)psDmxInstPri->fpDMXPvrPause((DMX_PVR_ENG)pu32Param[0], (MS_BOOL)pu32Param[1]);
			break;					
			
		case MApi_CMD_DMX_Pvr_Pid:
			DMX_MSG(printf("DMXIoctl - MApi_CMD_DMX_Pvr_Pid\n"));
			pu32Param = (MS_U32*)pArgs;
			pu32Param[4] = (MS_U32)psDmxInstPri->fpDMXPvrPid((DMX_PVR_ENG)pu32Param[0], (MS_U32)pu32Param[1], &(pu32Param[2]), (MS_BOOL)pu32Param[3]);
			break;					
			
        default:
            DMX_MSG(printf("DMXIoctl - Unknown commend!!!\n"));
            u32Res = UTOPIA_STATUS_NOT_SUPPORTED;
            break;

    }

IOCTRL_END:

    UtopiaResourceRelease(pResource);

    return u32Res;
}

MS_U32 DMXClose(void* pInstance)
{

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

