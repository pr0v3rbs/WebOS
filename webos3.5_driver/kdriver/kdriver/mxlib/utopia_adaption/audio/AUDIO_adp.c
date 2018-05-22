//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2014 - 2016 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2014-2016 MStar Semiconductor, Inc.
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
/// file  DMX_adp.c
/// @brief  Demux adaption API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
//#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "apiAUDIO.h"
#include "apiAUDIO_v2.h"

#include "utopia_adp.h"
#include "AUDIO_adp.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern void* spt_MS_U32;
extern void* spt_MS_U16;
extern void* spt_MS_U8;

//UADP_SPT_0NXT_DEF(AUDIO_INIT_INFO);
//UADP_SPT_0NXT_DEF(AUDIO_PATH_INFO);
//UADP_SPT_0NXT_DEF(AUDIO_OUT_INFO);
//UADP_SPT_0NXT_DEF(OUTPUT_SOURCE_INFO);
//UADP_SPT_0NXT_DEF(AUDIO_PEQ_COEF);
UADP_SPT_0NXT_DEF(AudioDecStatus_t);
UADP_SPT_0NXT_DEF(AUDIO_ST_DECODER_COMMAND);
UADP_SPT_0NXT_DEF(AUDIO_ST_SET_AD);
UADP_SPT_0NXT_DEF(AU_DVB_ENC_FRAME_INFO);
UADP_SPT_0NXT_DEF(AUDIOSTATUS);
//UADP_SPT_0NXT_DEF(THR_TBL_TYPE);
UADP_SPT_0NXT_DEF(AUDIO_ST_SPDIF_OP);
UADP_SPT_0NXT_DEF(AUDIO_ST_SPDIF_SCMS);
UADP_SPT_0NXT_DEF(Digital_Out_Channel_Status_t);
UADP_SPT_0NXT_DEF(AUDIO_HDMI_TX_GETSTATUS);
UADP_SPT_0NXT_DEF(AUDIO_ST_HDMI_NONPCM_OP);
UADP_SPT_0NXT_DEF(ST_HDMI_AUDIO_STATUS);
UADP_SPT_0NXT_DEF(AUDIO_ST_KTV_OP);
//UADP_SPT_0NXT_DEF(AUDIO_UPLOAD_INFO);
UADP_SPT_0NXT_DEF(AUDIO_ST_PCMCAPTURE_INIT);
UADP_SPT_0NXT_DEF(audioRingTask);
UADP_SPT_0NXT_DEF(AES_INFO);
UADP_SPT_0NXT_DEF(AUDIO_ST_MM2_INPUTAES_FINISH);
UADP_SPT_0NXT_DEF(AUDIO_ST_ENC_INFO);
UADP_SPT_0NXT_DEF(PCM_OUTPUT_FORMAT);
UADP_SPT_0NXT_DEF(AUDIO_ST_SETBALABCE);
UADP_SPT_0NXT_DEF(AUDIO_ST_RWReg);
UADP_SPT_0NXT_DEF(AUDIO_ST_CLIP);
UADP_SPT_0NXT_DEF(AUDIO_ST_SIF_ANALOGMODE_OP);
//UADP_SPT_0NXT_DEF(AUDIO_ST_SIF_SETMODE_SETUP_OP);
UADP_SPT_0NXT_DEF(AUDIO_ST_DECODE_FRAME_CNT);

//UADP_SPT_1NXT_DEF(AUDIO_ST_AUDIO_INIT_INFO);
UADP_SPT_1NXT_DEF(AUDIO_ST_SOUND_SETPARAM);
//UADP_SPT_1NXT_DEF(AUDIO_ST_AUDIO_PATH_INFO);
//UADP_SPT_1NXT_DEF(AUDIO_ST_AUDIO_OUT_INFO);
//UADP_SPT_1NXT_DEF(AUDIO_ST_OUTPUT_SOURCE);
//UADP_SPT_1NXT_DEF(AUDIO_ST_AUDIO_PEQ_COEF);
UADP_SPT_1NXT_DEF(AUDIO_ST_DECODESYSTEM);
UADP_SPT_1NXT_DEF(AUDIO_ST_AU_DVB_ENC_FRAME_INFO);
UADP_SPT_1NXT_DEF(AUDIO_ST_SIF_GET_AUDIO_STATUS);
//UADP_SPT_1NXT_DEF(AUDIO_ST_THR_TBLL_TYPE);
UADP_SPT_1NXT_DEF(AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS);
UADP_SPT_1NXT_DEF(AUDIO_ST_RX_HDMI_AUDIO_STATUS);
UADP_SPT_1NXT_DEF(AUDIO_ST_ADV_SOUND_SETPARAM);
UADP_SPT_1NXT_DEF(AUDIO_ST_COPY_PARAMETER);
UADP_SPT_1NXT_DEF(AUDIO_ST_SETDECODEPARAM);
UADP_SPT_1NXT_DEF(AUDIO_ST_DMAREADER_WRITEPCM);
//UADP_SPT_1NXT_DEF(AUDIO_ST_UPLOAD_INFO);
UADP_SPT_1NXT_DEF(AUDIO_ST_PCMCAPTURE_READ);
UADP_SPT_1NXT_DEF(AUDIO_ST_AUDIORINGTASK);
UADP_SPT_1NXT_DEF(AUDIO_ST_MM2_CHECKAESINFO);
UADP_SPT_1NXT_DEF(AUDIO_ST_GETAUDIOINFO2);
UADP_SPT_1NXT_DEF(AUDIO_ST_SET_CAPTURE_ENABLE);
UADP_SPT_1NXT_DEF(AUDIO_ST_ADVSOUND_PARAM2);
UADP_SPT_1NXT_DEF(AUDIO_ST_SWDMAREADER_INIT);
UADP_SPT_1NXT_DEF(AUDIO_ST_LOAD_AUDIO_CLIP);
UADP_SPT_1NXT_DEF(AUDIO_ST_SIF_GETBAND_OP);
UADP_SPT_1NXT_DEF(AUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM);
UADP_SPT_1NXT_DEF(AUDIO_ST_SIF_CHECK_A2DK);
UADP_SPT_1NXT_DEF(AUDIO_ST_SIF_GETSOUNDSTANDARD);
UADP_SPT_1NXT_DEF(AUDIO_ST_GETBTSC_A2STEREO_LEVEL);
UADP_SPT_1NXT_DEF(AUDIO_ST_SETBTSCA2THRESHOLDLEVEL);
UADP_SPT_1NXT_DEF(AUDIO_ST_CHECKNICAMDIGITAL);
UADP_SPT_1NXT_DEF(AUDIO_ST_SETTHRESHOLDTYPE);

UADP_SPT_2NXT_DEF(AUDIO_ST_CHECK_INPUT_REQ);
UADP_SPT_2NXT_DEF(AUDIO_ST_MM_CHECK_INPUT_REQ);
UADP_SPT_2NXT_DEF(AUDIO_ST_UNI_CHECK_DECODEDONE);
UADP_SPT_2NXT_DEF(AUDIO_ST_RING_DATA_TRANS);

UADP_SPT_5NXT_DEF(AUDIO_ST_RA8_Param);

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER                   copy_from_user
#define CPY_to_USER                     copy_to_user
#else
#define CPY_FROM_USER                   memcpy
#define CPY_to_USER                     memcpy
#endif

#define AUDIO_ADP_MSG(_f, _a...)                             //printk("\033[1;35m"_f"\033[0m", ##_a)

#define AUDIO_REDUCE_STACK_SIZE TRUE

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_U32 _addr_offset(void* pHead, void* pMember)  //tony
{
    MS_U32 offset;
    offset =(MS_U32)pMember - (MS_U32)pHead;
    return offset;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 AUDIO_adp_Init(FUtopiaIOctl *pIoctl)
{
    //UADP_SPT_0NXT(AUDIO_INIT_INFO);
    //UADP_SPT_0NXT(AUDIO_PATH_INFO);
    //UADP_SPT_0NXT(AUDIO_OUT_INFO);
    //UADP_SPT_0NXT(OUTPUT_SOURCE_INFO);
    //UADP_SPT_0NXT(AUDIO_PEQ_COEF);
    UADP_SPT_0NXT(AudioDecStatus_t);
    UADP_SPT_0NXT(AUDIO_ST_DECODER_COMMAND);
    UADP_SPT_0NXT(AUDIO_ST_SET_AD);
    UADP_SPT_0NXT(AU_DVB_ENC_FRAME_INFO);
    UADP_SPT_0NXT(AUDIOSTATUS);
    //UADP_SPT_0NXT(THR_TBL_TYPE);
    UADP_SPT_0NXT(AUDIO_ST_SPDIF_OP);
    UADP_SPT_0NXT(AUDIO_ST_SPDIF_SCMS);
    UADP_SPT_0NXT(Digital_Out_Channel_Status_t);
    UADP_SPT_0NXT(AUDIO_HDMI_TX_GETSTATUS);
    UADP_SPT_0NXT(AUDIO_ST_HDMI_NONPCM_OP);
    UADP_SPT_0NXT(ST_HDMI_AUDIO_STATUS);
    UADP_SPT_0NXT(AUDIO_ST_KTV_OP);
    //UADP_SPT_0NXT(AUDIO_UPLOAD_INFO);
    UADP_SPT_0NXT(AUDIO_ST_PCMCAPTURE_INIT);
    UADP_SPT_0NXT(audioRingTask);
    UADP_SPT_0NXT(AES_INFO);
    UADP_SPT_0NXT(AUDIO_ST_MM2_INPUTAES_FINISH);
    UADP_SPT_0NXT(AUDIO_ST_ENC_INFO);
    UADP_SPT_0NXT(PCM_OUTPUT_FORMAT);
    //UADP_SPT_0NXT(AUDIO_ST_SETBALABCE);
    UADP_SPT_0NXT(AUDIO_ST_RWReg);
    UADP_SPT_0NXT(AUDIO_ST_CLIP);
    UADP_SPT_0NXT(AUDIO_ST_SIF_ANALOGMODE_OP);
    //UADP_SPT_0NXT(AUDIO_ST_SIF_SETMODE_SETUP_OP);
    UADP_SPT_0NXT(AUDIO_ST_DECODE_FRAME_CNT);

    //UADP_SPT_1NXT(AUDIO_ST_AUDIO_INIT_INFO, pau_info, AUDIO_INIT_INFO);
    UADP_SPT_1NXT(AUDIO_ST_SOUND_SETPARAM, pParam, MS_U32);
    //UADP_SPT_1NXT(AUDIO_ST_AUDIO_PATH_INFO, path_info, AUDIO_PATH_INFO);
    //UADP_SPT_1NXT(AUDIO_ST_AUDIO_OUT_INFO, pout_info, AUDIO_OUT_INFO);
    //UADP_SPT_1NXT(AUDIO_ST_OUTPUT_SOURCE, stOutputSourceInfo, OUTPUT_SOURCE_INFO);
    //UADP_SPT_1NXT(AUDIO_ST_AUDIO_PEQ_COEF, peq_coef, AUDIO_PEQ_COEF);
    UADP_SPT_1NXT(AUDIO_ST_DECODESYSTEM, p_AudioDecStatus, AudioDecStatus_t);
    UADP_SPT_1NXT(AUDIO_ST_AU_DVB_ENC_FRAME_INFO, EncFrameInfo, AU_DVB_ENC_FRAME_INFO);
    UADP_SPT_1NXT(AUDIO_ST_SIF_GET_AUDIO_STATUS, p_eAudioStatus, AUDIOSTATUS);
    //UADP_SPT_1NXT(AUDIO_ST_THR_TBLL_TYPE, ThrTbl, THR_TBL_TYPE);
    UADP_SPT_1NXT(AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS, stChannelStatus, Digital_Out_Channel_Status_t);
    UADP_SPT_1NXT(AUDIO_ST_RX_HDMI_AUDIO_STATUS, pHdmi_status, ST_HDMI_AUDIO_STATUS);
    UADP_SPT_1NXT(AUDIO_ST_ADV_SOUND_SETPARAM, pParam, MS_U32);
    UADP_SPT_1NXT(AUDIO_ST_COPY_PARAMETER, Parameter_ptr, MS_U32);
    UADP_SPT_1NXT(AUDIO_ST_SETDECODEPARAM, Param, MS_U32);
    //UADP_SPT_1NXT(AUDIO_ST_DMAREADER_WRITEPCM, pBuffer, void);
    UADP_SPT_1NXT(AUDIO_ST_DMAREADER_WRITEPCM, pBuffer, MS_U32);
    //UADP_SPT_1NXT(AUDIO_ST_UPLOAD_INFO, uploadInfo, AUDIO_UPLOAD_INFO);
    //UADP_SPT_1NXT(AUDIO_ST_PCMCAPTURE_READ, pBuffer, void);
    UADP_SPT_1NXT(AUDIO_ST_PCMCAPTURE_READ, pBuffer, MS_U32);
    UADP_SPT_1NXT(AUDIO_ST_AUDIORINGTASK, auRingTask, audioRingTask);
    UADP_SPT_1NXT(AUDIO_ST_MM2_CHECKAESINFO, paes_info, AES_INFO);
    //UADP_SPT_1NXT(AUDIO_ST_GETAUDIOINFO2, pInfo, void);
    UADP_SPT_1NXT(AUDIO_ST_GETAUDIOINFO2, pInfo, MS_U32);
    //UADP_SPT_1NXT(AUDIO_ST_SET_CAPTURE_ENABLE, PCM_CB, void);
    UADP_SPT_1NXT(AUDIO_ST_SET_CAPTURE_ENABLE, PCM_CB, MS_U32);
    UADP_SPT_1NXT(AUDIO_ST_ADVSOUND_PARAM2, u32value1, MS_U32);
    UADP_SPT_1NXT(AUDIO_ST_SWDMAREADER_INIT, pSettings, PCM_OUTPUT_FORMAT);
    //UADP_SPT_1NXT(AUDIO_ST_SWDMAREADER_WRITEPCM, buffer, void);
    //UADP_SPT_1NXT(AUDIO_ST_SWDMAREADER_WRITEPCM, buffer, MS_U32);
    //UADP_SPT_1NXT(AUDIO_ST_LOAD_AUDIO_CLIP, clipBufPtr, void);
    UADP_SPT_1NXT(AUDIO_ST_LOAD_AUDIO_CLIP, clipBufPtr, MS_U32);
    UADP_SPT_1NXT(AUDIO_ST_SIF_GETBAND_OP, pbandStrength, MS_U32);
    UADP_SPT_1NXT(AUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM, pexist, MS_U8);
    UADP_SPT_1NXT(AUDIO_ST_SIF_CHECK_A2DK, pexist, MS_U8);
    UADP_SPT_1NXT(AUDIO_ST_SIF_GETSOUNDSTANDARD, B_SifStandard, MS_U8);
    UADP_SPT_1NXT(AUDIO_ST_GETBTSC_A2STEREO_LEVEL, pLevel, MS_U16);
    UADP_SPT_1NXT(AUDIO_ST_SETBTSCA2THRESHOLDLEVEL, pRF2NSR_map, MS_U16);
    UADP_SPT_1NXT(AUDIO_ST_CHECKNICAMDIGITAL, pisNicamDetect, MS_U8);
    //UADP_SPT_1NXT(AUDIO_ST_SETTHRESHOLDTYPE, ThrTbl, THR_TBL_TYPE_C code);

    UADP_SPT_5NXT(AUDIO_ST_RA8_Param, pChannels, MS_U16, pRegions, MS_U16, pcplStart, MS_U16, pcplQbits, MS_U16, pFrameSize, MS_U16);

    UADP_SPT_2NXT(AUDIO_ST_CHECK_INPUT_REQ, pU32WrtAddr, MS_U32, pU32WrtBytes, MS_U32);
    UADP_SPT_2NXT(AUDIO_ST_MM_CHECK_INPUT_REQ, pU32WrtAddr, MS_U32, pU32WrtBytes, MS_U32);
    UADP_SPT_2NXT(AUDIO_ST_UNI_CHECK_DECODEDONE, pU32WrtAddr, MS_U32, pU32WrtBytes, MS_U32);
    UADP_SPT_2NXT(AUDIO_ST_RING_DATA_TRANS, pauRingTask, audioRingTask, pauRingTask_1, audioRingTask);

    *pIoctl= (FUtopiaIOctl)AUDIO_adp_Ioctl;
    return 0;
}

MS_U32 AUDIO_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 						u32Ret = UTOPIA_STATUS_FAIL, u32data = 1;

    //printf("start\n");
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
    //do nothing
#else
    char    buffer_arg[1024];
    char 	buffer_array0[1024];
    char 	buffer_array1[128];
    char 	buffer_array2[128];
    char 	buffer_array3[128];
    char 	buffer_array4[128];
#endif

    MS_U32 						*pu32Src                            = NULL, *pu32Des                                = NULL;
    MS_S32 						*ps32Src                            = NULL, *ps32Des                                = NULL;
    MS_U64 						*pu64Src                            = NULL, *pu64Des                                = NULL;
    PAUDIO_ST_SOUND_SETPARAM    pSoundSetParamSrc                   = NULL, pSoundSetParamDes                       = NULL;
    PAUDIO_ST_DECODESYSTEM      pDecodeSystemSrc                    = NULL, pDecodeSystemDes                        = NULL;
    PAUDIO_ST_OPEN_DECODESYSTEM pOpenDecodeSystemSrc                = NULL, pOpenDecodeSystemDes                    = NULL;
    PAUDIO_ST_RELEASE_DECODESYSTEM pReleaseDecodeSystemSrc          = NULL, pReleaseDecodeSystemDes                 = NULL;
    PAUDIO_ST_SET_DECODESYSTEM  pSetDecodeSystemSrc                 = NULL, pSetDecodeSystemDes                     = NULL;
    PAUDIO_ST_GET_DECODESYSTEM  pGetDecodeSystemSrc                 = NULL, pGetDecodeSystemDes                     = NULL;
    PAUDIO_ST_DECODER_COMMAND   pDecodeCommandSrc                   = NULL, pDecodeCommandDes                       = NULL;
    PAUDIO_ST_SET_AD            pSetADSrc                           = NULL, pSetADDes                               = NULL;
    PAUDIO_ST_AU_DVB_ENC_FRAME_INFO pAUDVBEncFrameInfoSrc           = NULL, pAUDVBEncFrameInfoDes                   = NULL;
    PAUDIO_ST_SIF_GET_AUDIO_STATUS pSIF_GetAudioStatusSrc           = NULL, pSIF_GetAudioStatusDes                  = NULL;
    PAUDIO_ST_SPDIF_OP          pSPDIF_OpSrc                        = NULL, pSPDIF_OpDes                            = NULL;
    PAUDIO_ST_SPDIF_SCMS        pSPDIF_SCMSSrc                      = NULL, pSPDIF_SCMSDes                          = NULL;
    PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS   pDigitalOut_SetChannelStatusSrc = NULL, pDigitalOut_SetChannelStatusDes = NULL;
    PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS   pDigitalOut_GetChannelStatusSrc = NULL, pDigitalOut_GetChannelStatusDes = NULL;
    PAUDIO_HDMI_TX_GETSTATUS    pHDMI_TX_GetStatusSrc               = NULL, pHDMI_TX_GetStatusDes                   = NULL;
    PAUDIO_ST_HDMI_NONPCM_OP    pHDMI_NonPCM_OpSrc                  = NULL, pHDMI_NonPCM_OpDes                      = NULL;
    PAUDIO_ST_RX_HDMI_AUDIO_STATUS  pHDMI_RX_AudioStatusSrc         = NULL, pHDMI_RX_AudioStatusDes                 = NULL;
    PAUDIO_ST_ADV_SOUND_SETPARAM pADV_Sound_SetParamSrc             = NULL, pADV_Sound_SetParamDes                  = NULL;
    PAUDIO_ST_COPY_PARAMETER    pCopyParameterSrc                   = NULL, pCopyParameterDes                       = NULL;
    PAUDIO_ST_KTV_OP            pKTV_OpSrc                          = NULL, pKTV_OpDes                              = NULL;
    PAUDIO_ST_SETDECODEPARAM    pSetDecodeParamSrc                  = NULL, pSetDecodeParamDes                      = NULL;
    PAUDIO_ST_RA8_Param         pRA8_ParamSrc                       = NULL, pRA8_ParamDes                           = NULL;
    PAUDIO_ST_CHECK_INPUT_REQ   pCheckInputReqSrc                   = NULL, pCheckInputReqDes                       = NULL;
    PAUDIO_ST_MM_CHECK_INPUT_REQ pMMCheckInputReqSrc                = NULL, pMMCheckInputReqDes                     = NULL;
    PAUDIO_ST_DMAREADER_WRITEPCM pDMAReader_WritePCMSrc             = NULL, pDMAReader_WrtiePCMDes                  = NULL;
    //PAUDIO_ST_UPLOAD_INFO       pUploadInfoSrc                      = NULL, pUploadInfoDes                          = NULL;
    PAUDIO_ST_PCMCAPTURE_INIT   pPCMCaptureInitSrc                  = NULL, pPCMCaptureInitDes                      = NULL;
    PAUDIO_ST_PCMCAPTURE_READ   pPCMCaptureReadSrc                  = NULL, pPCMCaptureReadDes                      = NULL;
    PAUDIO_ST_UNI_CHECK_DECODEDONE pUNI_Check_DecodeModeSrc         = NULL, pUNI_Check_DecodeModeDes                = NULL;
    PAUDIO_ST_AUDIORINGTASK     pAudioRingTaskSrc                   = NULL, pAudioRingTaskDes                       = NULL;
    PAUDIO_ST_RING_DATA_TRANS   pAudioRingTaskTransSrc              = NULL, pAudioRingTaskTransDes                  = NULL;
    PAUDIO_ST_MM2_CHECKAESINFO  pMM2_CheckAESInfoSrc                = NULL, pMM2_CheckAESInfoDes                    = NULL;
    PAUDIO_ST_MM2_INPUTAES_FINISH pMM2_InputAES_FinishSrc           = NULL, pMM2_InputAES_FinishDes                 = NULL;
    PAUDIO_ST_GETAUDIOINFO2     pGetAudioInfo2Src                   = NULL, pGetAudioInfo2Des                       = NULL;
    PAUDIO_ST_ENC_INFO          pEncInfoSrc                         = NULL, pEncInfoDes                             = NULL;
    PAUDIO_ST_SET_CAPTURE_ENABLE pSetCaptureEnableSrc               = NULL, pSetCaptureEnableDes                    = NULL;
    PAUDIO_ST_ADVSOUND_PARAM2   pADVSound_Param2Src                 = NULL, pADVSound_Param2Des                     = NULL;
    PAUDIO_ST_SWDMAREADER_WRITEPCM pSwDmaReaderWritePCMSrc          = NULL, pSwDmaReaderWritePCMDes                 = NULL;
    //PAUDIO_ST_SETBALABCE        pSetBalanceSrc                      = NULL, pSetBalanceDes                          = NULL;
    PAUDIO_ST_RWReg             pRWRegSrc                           = NULL, pRWRegDes                               = NULL;
    PAUDIO_ST_LOAD_AUDIO_CLIP   pLoadAudioClipSrc                   = NULL, pLoadAudioClipDes                       = NULL;
    PAUDIO_ST_CLIP              pClipSrc                            = NULL, pClipDes                                = NULL;
    PAUDIO_ST_SETENCCOMMAND     pSetEncCommandSrc                   = NULL, pSetEncCommandDes                       = NULL;
    PAUDIO_ST_SETTHRESHOLDTYPE  pSetThresholdTypeSrc                = NULL, pSetThresholdTypeDes                    = NULL;
    PAUDIO_ST_SIF_ANALOGMODE_OP pSIFAnalogMode_Src                  = NULL, pSIFAnalogMode_Des                      = NULL;
    PAUDIO_ST_SIF_SETMODE_SETUP_OP pSIF_SetMode_SetupSrc            = NULL, pSIF_SetMode_SetupDes                   = NULL;
    PAUDIO_ST_SIF_GETBAND_OP    pSIFGetBandSrc                      = NULL, pSIFGetBandDes                          = NULL;
    PAUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM pCheckAvaliableSystemSrc   = NULL, pCheckAvaliableSystemDes                = NULL;
    PAUDIO_ST_SIF_CHECK_A2DK    pSIFCheckA2DKSrc                    = NULL, pSIFCheckA2DKDes                        = NULL;
    PAUDIO_ST_SIF_GETSOUNDSTANDARD pSIFGetStandardSrc               = NULL, pSIFGetStandardDes                      = NULL;
    PAUDIO_ST_GETBTSC_A2STEREO_LEVEL pGetBTSCA2StereoLevelSrc       = NULL, pGetBTSCA2StereoLevelDes                = NULL;
    PAUDIO_ST_SETBTSCA2THRESHOLDLEVEL pSetBTSCA2ThresholdLevelSrc   = NULL, pSetBTSCA2ThresholdLevelDes             = NULL;
    PAUDIO_ST_CHECKNICAMDIGITAL pCheckNICAMDigitalSrc               = NULL, pCheckNICAMDigitalDes                   = NULL;
    PAUDIO_ST_DECODE_FRAME_CNT  pDecodeFrameCntSrc                  = NULL, pDecodeFrameCntDes                      = NULL;
    PAUDIO_ST_OUTPUT_SOURCE     pOutputSourceSrc                    = NULL, pOutputSourceDes                        = NULL;
    PAUDIO_ST_PCMMIXER_INIT     pPcmMixerInitSrc                    = NULL, pPcmMixerInitDes                        = NULL;
    PAUDIO_ST_PCMMIXER_DEINIT   pPcmMixerDeInitSrc                  = NULL, pPcmMixerDeInitDes                      = NULL;
    PAUDIO_ST_PCMMIXER_OPEN     pPcmMixerOpenSrc                    = NULL, pPcmMixerOpenDes                        = NULL;
    PAUDIO_ST_PCMMIXER_CLOSE    pPcmMixerCloseSrc                   = NULL, pPcmMixerCloseDes                       = NULL;
    PAUDIO_ST_PCMMIXER_FLUSH    pPcmMixerFlushSrc                   = NULL, pPcmMixerFlushDes                       = NULL;
    PAUDIO_ST_PCMMIXER_START    pPcmMixerStartSrc                   = NULL, pPcmMixerStartDes                       = NULL;
    PAUDIO_ST_PCMMIXER_STOP     pPcmMixerStopSrc                    = NULL, pPcmMixerStopDes                        = NULL;
    PAUDIO_ST_PCMMIXER_SET      pPcmMixerSetSrc                     = NULL, pPcmMixerSetDes                         = NULL;
    PAUDIO_ST_PCMMIXER_GET      pPcmMixerGetSrc                     = NULL, pPcmMixerGetDes                         = NULL;
    PAUDIO_ST_PCMMIXER_WRITE    pPcmMixerWriteSrc                   = NULL, pPcmMixerWriteDes                       = NULL;
    PAUDIO_ST_PCMMIXER_FUNC     pPcmMixerFuncSrc                    = NULL, pPcmMixerFuncDes                        = NULL;
    PAUDIO_ST_PCM_OPEN          pPcmOpenSrc                         = NULL, pPcmOpenDes                             = NULL;
    PAUDIO_ST_PCM_CLOSE         pPcmCloseSrc                        = NULL, pPcmCloseDes                            = NULL;
    PAUDIO_ST_PCM_START         pPcmStartSrc                        = NULL, pPcmStartDes                            = NULL;
    PAUDIO_ST_PCM_STOP          pPcmStopSrc                         = NULL, pPcmStopDes                             = NULL;
    PAUDIO_ST_PCM_SET           pPcmSetSrc                          = NULL, pPcmSetDes                              = NULL;
    PAUDIO_ST_PCM_GET           pPcmGetSrc                          = NULL, pPcmGetDes                              = NULL;
    PAUDIO_ST_PCM_READ          pPcmReadSrc                         = NULL, pPcmReadDes                             = NULL;
    PAUDIO_ST_PCM_WRITE         pPcmWriteSrc                        = NULL, pPcmWriteDes                            = NULL;
    PAUDIO_ST_PCM_FLUSH         pPcmFlushSrc                        = NULL, pPcmFlushDes                            = NULL;
    PAUDIO_ST_ARC_HWEN_BY_PORT  pARCHwenByPortSrc                   = NULL, pARCHwenByPortDes                       = NULL;

    AUDIO_ADP_MSG("[V] ADA Cmd %d \n", (int)u32Cmd);

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

    switch(u32Cmd)
    {
        //============================================================
        // AUDIO_SYSTEM RELATIONAL API FUNCTION
        //============================================================

        case MApi_CMD_AUDIO_Initialize:
        {
            // void MApi_AUDIO_Initialize(void)
            AUDIO_ADP_MSG("MApi_CMD_AUDIO_Initialize cmd %d\n", (int)u32Cmd);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_SetPowerOn:
        {
            //void MApi_AUDIO_SetPowerOn(MS_BOOL bFlag)

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetDSPBaseAddr:
        {
            AUDIO_ADP_MSG("MApi_CMD_AUDIO_SetDSPBaseAddr cmd %d\n", (int)u32Cmd);

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_GetDSPBaseAddr:
        {

#if 0   // original
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_WriteDecMailBox:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_TriggerSIFPLL:
        {
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_Monitor:
        {
#if 0
            //MS_U8 MApi_Audio_Monitor(void)
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else
            //MS_U8 MApi_Audio_Monitor(void)

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SetSystemInfo:
        {
            AUDIO_ADP_MSG("MApi_CMD_AUDIO_SetDSPBaseAddr cmd %d\n", (int)u32Cmd);
            //void MApi_AUDIO_SetSystemInfo(AUDIO_INIT_INFO *pau_info)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_AUDIO_INIT_INFO, NULL, buffer_arg,sizeof(buffer_arg));

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_INIT_INFO *buffer_arg;
            AUDIO_INIT_INFO data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_INIT_INFO));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetOutConnectivity:
        {
            //void MApi_AUDIO_SetOutConnectivity(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_SetPathInfo:
        {
            //void MApi_AUDIO_SetPathInfo(AUDIO_PATH_INFO *path_info)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_AUDIO_PATH_INFO, NULL, buffer_arg,sizeof(buffer_arg));

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_PATH_INFO *buffer_arg;
            AUDIO_PATH_INFO data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_PATH_INFO));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetOutputInfo:
        {
            //void MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info)

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_OUT_INFO *buffer_arg;
            AUDIO_OUT_INFO data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_OUT_INFO));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetSourceInfo:
        {
            //void MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType)

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_InputSwitch:
        {
            //void MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup)

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            AUDIO_ADP_MSG("\033[1;33m Input Switch buffer_arg [0] %d [1] %d \033[0m \n", (int)buffer_arg[0], (int)buffer_arg[1]);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetDataCaptureSource:
        {
#if 0       // original
            //MS_BOOL MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
            // u32Param[0] = 0(MS_BOOL); u32Param[1] = ((MS_U32)eID) & 0xFFFF;  u32Param[2] = ((MS_U32)eSource) & 0xFFFF;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else  // modified
            //MS_BOOL MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
            // u32Param[0] = 0(MS_BOOL); u32Param[1] = ((MS_U32)eID) & 0xFFFF;  u32Param[2] = ((MS_U32)eSource) & 0xFFFF;

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_OutputSourceInfo:
        {
#if 0   // original
            //void MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
            //Status.bSet = TRUE/FALSE; Status.stOutputSourceInfo = pSourceInfo;
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_OUTPUT_SOURCE, NULL, buffer_arg,sizeof(buffer_arg));
            pOutputSourceSrc = (PAUDIO_ST_OUTPUT_SOURCE)pArgs;
            pOutputSourceDes = (PAUDIO_ST_OUTPUT_SOURCE)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_OUTPUT_SOURCE));

            pOutputSourceDes->stOutputSourceInfo = (OUTPUT_SOURCE_INFO*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)pOutputSourceSrc->stOutputSourceInfo, sizeof(OUTPUT_SOURCE_INFO));

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            if(pOutputSourceSrc->bSet == FALSE)
            {
                CPY_to_USER((void*)(pOutputSourceSrc->stOutputSourceInfo), (void*)buffer_array0, sizeof(OUTPUT_SOURCE_INFO));
            }
#else   // modified

            //void MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
            //Status.bSet = TRUE/FALSE; Status.stOutputSourceInfo = pSourceInfo;
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_OUTPUT_SOURCE, NULL, buffer_arg,sizeof(buffer_arg));

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_OUTPUT_SOURCE *buffer_arg;
            AUDIO_ST_OUTPUT_SOURCE data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            OUTPUT_SOURCE_INFO *buffer_array0;
            OUTPUT_SOURCE_INFO data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            void* ptmp;
            MS_BOOL bSet;
            pOutputSourceSrc = (PAUDIO_ST_OUTPUT_SOURCE)pArgs;
            pOutputSourceDes = (PAUDIO_ST_OUTPUT_SOURCE)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_OUTPUT_SOURCE));
            ptmp = pOutputSourceDes->stOutputSourceInfo;    // copy user space ptr
            bSet = pOutputSourceDes->bSet;

            pOutputSourceDes->stOutputSourceInfo = (OUTPUT_SOURCE_INFO*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, ptmp, sizeof(OUTPUT_SOURCE_INFO));

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            if(bSet == FALSE)     // get
            {
                CPY_to_USER(ptmp, (void*)buffer_array0, sizeof(OUTPUT_SOURCE_INFO));
            }
#endif
        }
            break;

        case MApi_CMD_AUDIO_ReadMailBox:
        {
#if 0   // original
            //MS_U16 MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum)
            //u32Param[0] = return; u32Param[1] = bDspType; u32Param[2] = u8ParamNum;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else   // modified
            //MS_U16 MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum)
            //u32Param[0] = return; u32Param[1] = bDspType; u32Param[2] = u8ParamNum;

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_WriteMailBox:
        {
            //void MApi_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data)
            //u32Param[0] = bDspType; u32Param[1] = u8ParamNum; u32Param[2] = u16Data;

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_FwTriggerDSP:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_FwTriggerDSP(MS_U16 u16Cmd)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetFwStatus:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_SetFwStatus(MS_BOOL bBootOnDDR)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_ExitAudioSystem:
        {
            //void MApi_AUDIO_ExitAudioSystem(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_RebootDSP:
        {
            //void MApi_AUDIO_RebootDsp(MS_BOOL bDspType)

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SendIntrupt:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //void MApi_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U8 u8Cmd)
            //u32Param[0] = bDspType;u32Param[1] = u8Cmd);
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_I2S_SetMode:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //void MApi_AUDIO_I2S_SetMode(AUDIO_I2S_MODE_TYPE u8Mode, AUDIO_I2S_MODE_VALUE  u8Val)
            //u32Param[0] = u8Mode; u32Param[1] = u8Val;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_WritePreInitTable:
        {
            //void MApi_AUDIO_WritePreInitTable(void)
            AUDIO_ADP_MSG("MApi_CMD_AUDIO_WritePreInitTable cmd %d\n", (int)u32Cmd);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage:
        {
            //void MApi_AUDIO_EnaEarphone_LowPower_Stage(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage:
        {
            //void MApi_AUDIO_EnaEarphone_HighDriving_Stage(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        //=============================================================
        // AUDIO_SOUND RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_SND_ProcessEnable:
        {
#if 0   // original
            //MS_BOOL MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable)
            //u32Param[0] = return, u32Param[1] = Type; u32Param[2] = enable;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            //MS_BOOL MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable)
            //u32Param[0] = return, u32Param[1] = Type; u32Param[2] = enable;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_SND_SetParam1:
        {
#if 0
            //MS_BOOL MApi_SND_SetParam1( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2)
            //u32Param[0] = return; u32Param[1] = Type; u32Param[2] = param1; u32Param[3] = param2;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[4];
#endif

            //MS_BOOL MApi_SND_SetParam1( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2)
            //u32Param[0] = return; u32Param[1] = Type; u32Param[2] = param1; u32Param[3] = param2;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_SND_GetParam1:
        {
#if 0   // original
            //MS_U16 MApi_SND_GetParam1( Sound_GET_PARAM_Type Type, MS_U16 param1)
            //u32Param[0] = return; u32Param[1] = Type; u32Param[2] = param1;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            //MS_U16 MApi_SND_GetParam1( Sound_GET_PARAM_Type Type, MS_U16 param1)
            //u32Param[0] = return; u32Param[1] = Type; u32Param[2] = param1;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));

#endif
        }
            break;

        case MApi_CMD_AUDIO_Sound_SetParam:
            //MS_BOOL MApi_AUDIO_SOUND_SetParam(Sound_SET_Type Type, MS_U32 *p_param )
       {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SOUND_SETPARAM *buffer_arg;
            AUDIO_ST_SOUND_SETPARAM data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 buffer_array0[4];
#endif

            void* ptmp;
            pSoundSetParamSrc = (PAUDIO_ST_SOUND_SETPARAM)pArgs;
            pSoundSetParamDes = (PAUDIO_ST_SOUND_SETPARAM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SOUND_SETPARAM));
            ptmp = pSoundSetParamDes->pParam;    // bakup user space ptr

            pSoundSetParamDes->pParam = (MS_U32*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, ptmp, sizeof(MS_U32) * 4);

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER((void*)pSoundSetParamSrc, (void*)(&(pSoundSetParamDes->bStatus)), sizeof(MS_BOOL));
       }
            break;

        case MApi_CMD_AUDIO_SetAbsoluteVolume:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            //void MApi_AUDIO_SetAbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2)
            //u32Param[0] = u8Path; u32Param[1] = u8Vol1; u32Param[2] = u8Vol2;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SourceConnect:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //void MApi_AUDIO_SourceConnect(AUDIO_OUTPORT_SOURCE_TYPE eConnectSourceType, MS_BOOL bConnect )
            //u32Param[0] = eConnectSourceType; u32Param[1] = bConnect;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetDynamicBass:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_SetDynamicBass(MS_U8 u8DynamicBass )
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_AbsoluteTreble:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_AbsoluteTreble(MS_U8 u8Treble )
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetBufferProcess:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_SetBufferProcess(MS_U8 DelayTime)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_ConvertVolumeUnit:
        {
            //MS_U8 MApi_AUDIO_ConvertVolumeUnit(MS_U8 UiVolume)
            //u32Param[0] = return; u32Param[1] = UiVolume;
#if 0 // original
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else   // modified

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SetPEQCoef:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_PEQ_COEF *buffer_arg;
            AUDIO_PEQ_COEF data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_AUDIO_PEQ_COEF, NULL, buffer_arg,sizeof(buffer_arg));
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_PEQ_COEF));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_EnableDcRemove:
        {
#if 0 // original
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else   // modified

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;
        //============================================================
        // AUDIO_DTV RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_DecodeSystem:

#if 0
            //MS_BOOL MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId)
            //Status.bStatus = return; Status.eType = DECODE_SYS_RELEASE; Status.DecId = DecId;
            pDecodeSystemSrc = (PAUDIO_ST_DECODESYSTEM)pArgs;
            pDecodeSystemDes = (PAUDIO_ST_DECODESYSTEM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DECODESYSTEM));
            pDecodeSystemSrc->p_AudioDecStatus = (AudioDecStatus_t*)buffer_array0;
            CPY_FROM_USER((void*)pDecodeSystemDes->p_AudioDecStatus, (void*)pDecodeSystemSrc->p_AudioDecStatus, sizeof(AudioDecStatus_t));
            pDecodeSystemDes->RetDecId = (AUDIO_DEC_ID*)buffer_array1;
            CPY_FROM_USER((void*)pDecodeSystemDes->RetDecId, (void*)pDecodeSystemSrc->RetDecId, sizeof(AUDIO_DEC_ID));
            printk("\033[1;33m [V] [MApi_CMD_AUDIO_DecodeSystem] eType %d \033[0m \n", (int)pDecodeSystemSrc->eType);

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            if(pDecodeSystemSrc->eType == DECODE_SYS_GET)
            {
                CPY_to_USER((void*)(pDecodeSystemSrc->p_AudioDecStatus), (void*)buffer_array0, sizeof(AudioDecStatus_t));
                CPY_to_USER((void*)(&(pDecodeSystemSrc->bStatus)), (void*)(&(pDecodeSystemDes->bStatus)), sizeof(MS_BOOL));
            }
            else if(pDecodeSystemSrc->eType == DECODE_SYS_OPEN)
            {
                printk("\033[1;33m [V] [MApi_CMD_AUDIO_DecodeSystem -- OPEN] buffer_array1 %d \033[0m \n", (int)buffer_array1[0]);
                CPY_to_USER((void*)(pDecodeSystemSrc->RetDecId), (void*)buffer_array1, sizeof(AUDIO_DEC_ID));
            }
            else
            {
                CPY_to_USER((void*)(&(pDecodeSystemSrc->bStatus)), (void*)(&(pDecodeSystemDes->bStatus)), sizeof(MS_BOOL));
            }
            break;

#else
            break;

        case MApi_CMD_AUDIO_OpenDecodeSystem:
        {
#if 0     // original
            pOpenDecodeSystemSrc = (PAUDIO_ST_OPEN_DECODESYSTEM)pArgs;
            pOpenDecodeSystemDes = (PAUDIO_ST_OPEN_DECODESYSTEM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_OPEN_DECODESYSTEM));

            pOpenDecodeSystemDes->p_AudioDecStatus = (AudioDecStatus_t*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)pOpenDecodeSystemSrc->p_AudioDecStatus, sizeof(AudioDecStatus_t));

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER( (void*)(&(pOpenDecodeSystemSrc->DecId)), (void*)(&(pOpenDecodeSystemDes->DecId)), sizeof(AUDIO_DEC_ID));
            AUDIO_ADP_MSG("Open Decode System, DecId %d \n",(int)pOpenDecodeSystemSrc->DecId);
#else   // modified

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_OPEN_DECODESYSTEM *buffer_arg;
            AUDIO_ST_OPEN_DECODESYSTEM data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            AudioDecStatus_t *buffer_array0;
            AudioDecStatus_t data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif
            void* ptmp;
            pOpenDecodeSystemSrc = (PAUDIO_ST_OPEN_DECODESYSTEM)pArgs;
            pOpenDecodeSystemDes = (PAUDIO_ST_OPEN_DECODESYSTEM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_OPEN_DECODESYSTEM));
            ptmp = pOpenDecodeSystemDes->p_AudioDecStatus;  // bakup ptr

            pOpenDecodeSystemDes->p_AudioDecStatus = (AudioDecStatus_t*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(AudioDecStatus_t));

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER((void*)pOpenDecodeSystemSrc, (void*)(&(pOpenDecodeSystemDes->DecId)), sizeof(AUDIO_DEC_ID));
            AUDIO_ADP_MSG("Open Decode System, DecId %d \n",(int)pOpenDecodeSystemSrc->DecId);
#endif
        }
            break;

        case MApi_CMD_AUDIO_ReleaseDecodeSystem:
        {
#if 0
            pReleaseDecodeSystemSrc = (PAUDIO_ST_RELEASE_DECODESYSTEM)pArgs;
            pReleaseDecodeSystemDes = (PAUDIO_ST_RELEASE_DECODESYSTEM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_RELEASE_DECODESYSTEM));

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);

            CPY_to_USER( (void*)(&(pReleaseDecodeSystemSrc->bStatus)), (void*)(&(pReleaseDecodeSystemDes->bStatus)), sizeof(AUDIO_ST_RELEASE_DECODESYSTEM));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_RELEASE_DECODESYSTEM *buffer_arg;
            AUDIO_ST_RELEASE_DECODESYSTEM data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            pReleaseDecodeSystemSrc = (PAUDIO_ST_RELEASE_DECODESYSTEM)pArgs;
            pReleaseDecodeSystemDes = (PAUDIO_ST_RELEASE_DECODESYSTEM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_RELEASE_DECODESYSTEM));

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);

            CPY_to_USER((void*)pReleaseDecodeSystemSrc, (void*)(&(pReleaseDecodeSystemDes->bStatus)), sizeof(AUDIO_ST_RELEASE_DECODESYSTEM));
#endif
        }
            break;

        case MApi_CMD_AUDIO_GetDecodeSystem:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_GET_DECODESYSTEM *buffer_arg;
            AUDIO_ST_GET_DECODESYSTEM data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            AudioDecStatus_t *buffer_array0;
            AudioDecStatus_t data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            void* ptmp;
            pGetDecodeSystemSrc = (PAUDIO_ST_GET_DECODESYSTEM)pArgs;
            pGetDecodeSystemDes = (PAUDIO_ST_GET_DECODESYSTEM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_GET_DECODESYSTEM));

            ptmp = pGetDecodeSystemDes->p_AudioDecStatus;
            pGetDecodeSystemDes->p_AudioDecStatus = (AudioDecStatus_t*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, ptmp, sizeof(AudioDecStatus_t));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER(ptmp, (void*)buffer_array0, sizeof(AudioDecStatus_t));

        }
            break;

        case MApi_CMD_AUDIO_SetDecodeSystem:
        {
#if 0
            pSetDecodeSystemSrc = (PAUDIO_ST_SET_DECODESYSTEM)pArgs;
            pSetDecodeSystemDes = (PAUDIO_ST_SET_DECODESYSTEM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SET_DECODESYSTEM));

            pSetDecodeSystemDes->p_AudioDecStatus = (AudioDecStatus_t*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)pSetDecodeSystemSrc->p_AudioDecStatus, sizeof(AudioDecStatus_t));

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER( (void*)(&(pSetDecodeSystemSrc->bStatus)), (void*)(&(pSetDecodeSystemDes->bStatus)), sizeof(AUDIO_ST_SET_DECODESYSTEM));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SET_DECODESYSTEM *buffer_arg;
            AUDIO_ST_SET_DECODESYSTEM data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            AudioDecStatus_t *buffer_array0;
            AudioDecStatus_t data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            void* ptmp;
            pSetDecodeSystemSrc = (PAUDIO_ST_SET_DECODESYSTEM)pArgs;
            pSetDecodeSystemDes = (PAUDIO_ST_SET_DECODESYSTEM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SET_DECODESYSTEM));
            ptmp = pSetDecodeSystemDes->p_AudioDecStatus;

            pSetDecodeSystemDes->p_AudioDecStatus = (AudioDecStatus_t*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, ptmp, sizeof(AudioDecStatus_t));

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER((void*)pSetDecodeSystemSrc, (void*)(&(pSetDecodeSystemDes->bStatus)), sizeof(AUDIO_ST_SET_DECODESYSTEM));
#endif
        }
            break;
#endif
        case MApi_CMD_AUDIO_SetDecodeCmd:
        {
#if 0
            //MS_BOOL MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd)
            //u32Param[0] = return; u32Param[1] = DecId; u32Param[2] = enDecComamnd;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            //MS_BOOL MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd)
            //u32Param[0] = return; u32Param[1] = DecId; u32Param[2] = enDecComamnd;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SetSystem:
        {
#if 0
            //MS_BOOL MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
            //u32Param[0] = return; u32Param[1] = enDecSystem;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_BOOL MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
            //u32Param[0] = return; u32Param[1] = enDecSystem;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
       }
            break;

        case MApi_CMD_AUDIO_GetDecSysSupportStatus:
        {
#if 0 // original
            //MS_BOOL MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
            //u32Param[0] = return; u32Param[1] = enDecSystem;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else


#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_BOOL MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
            //u32Param[0] = return; u32Param[1] = enDecSystem;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_IsMadLock:
        {
#if 0
            //MS_BOOL MApi_AUDIO_IsMadLock(void)
            //u32param[1] = return;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_BOOL MApi_AUDIO_IsMadLock(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32);
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_GetDecStatus:
        {
#if 0
            //En_DVB_decCmdType MApi_AUDIO_GetDecStatus(void)
            //u32param[1] = return;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //En_DVB_decCmdType MApi_AUDIO_GetDecStatus(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            //CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U32));
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32);
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_Decode_Command:
        {
#if 0
            //AU_DVB_DECCMD MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo)
            //Status.bSet = TRUE/FALSE; Status.eDecCmd = Return; Status.eAdoDecoderNo = AdoDecoderNo;
            pDecodeCommandSrc = (PAUDIO_ST_DECODER_COMMAND)pArgs;
            pDecodeCommandDes = (PAUDIO_ST_DECODER_COMMAND)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DECODER_COMMAND));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            if(pDecodeCommandSrc->bSet == FALSE)
            {
                CPY_to_USER((void*)(&(pDecodeCommandSrc->eDecCmd)), (void*)(&(pDecodeCommandDes->eDecCmd)), sizeof(AU_DVB_DECCMD));
            }
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_DECODER_COMMAND *buffer_arg;
            AUDIO_ST_DECODER_COMMAND data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //AU_DVB_DECCMD MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo)
            //Status.bSet = TRUE/FALSE; Status.eDecCmd = Return; Status.eAdoDecoderNo = AdoDecoderNo;
            MS_BOOL bSet;
            pDecodeCommandSrc = (PAUDIO_ST_DECODER_COMMAND)pArgs;
            pDecodeCommandDes = (PAUDIO_ST_DECODER_COMMAND)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DECODER_COMMAND));
            bSet = pDecodeCommandDes->bSet;

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            if(bSet == FALSE)
            {
                MS_U32 u32addr = (MS_U32)pDecodeCommandSrc + _addr_offset(pDecodeCommandDes, &(pDecodeCommandDes->eDecCmd));
                CPY_to_USER((void*)u32addr, (void*)(&(pDecodeCommandDes->eDecCmd)), sizeof(AU_DVB_DECCMD));
            }
#endif
        }
            break;

        case MApi_CMD_AUDIO_GetMAD_Lock:
        {
#if 0
            //MS_BOOL MApi_AUDIO_GetMAD_LOCK(void)
            //u32param[1] = return;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            MS_U32 u32addr;
            //MS_BOOL MApi_AUDIO_GetMAD_LOCK(void)
            //u32param[1] = return;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32);
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SETAD:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SET_AD *buffer_arg;
            AUDIO_ST_SET_AD data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_SetADOutputMode(MS_U8 out_mode)
            //Status.eType = AD_OUTPUT_MODE; Status.u8Data = out_mode/AD_ABSOLUTE_VOLUME/AD_SET_MUTE ; Status.bMute = FALSE;
            pSetADSrc = (PAUDIO_ST_SET_AD)pArgs;
            pSetADDes = (PAUDIO_ST_SET_AD)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SET_AD));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        //============================================================
        // AUDIO_PVR RELATED FUNCTIONS
        //============================================================
        case MApi_CMD_AUDIO_SetEncInit:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //void MApi_AUDIO_SetEncInit(AU_DVB_ENCBR BitRate, MS_U8 u8EncFrameNum)
            //u32Param[0] = BitRate; u32Param[1] = u8EncFrameNum;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetEncCommand:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SETENCCOMMAND *buffer_arg;
            AUDIO_ST_SETENCCOMMAND data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD enEncComamnd)
            pSetEncCommandSrc = (PAUDIO_ST_SETENCCOMMAND)pArgs;
            pSetEncCommandDes = (PAUDIO_ST_SETENCCOMMAND)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SETENCCOMMAND));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_GetEncodeFrameInfo:     //HAL is empty
        {
#if 0
            //void MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo)
            pAUDVBEncFrameInfoSrc = (PAUDIO_ST_AU_DVB_ENC_FRAME_INFO)pArgs;
            pAUDVBEncFrameInfoDes = (PAUDIO_ST_AU_DVB_ENC_FRAME_INFO)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_AU_DVB_ENC_FRAME_INFO));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pAUDVBEncFrameInfoSrc->EncFrameInfo)), (void*)(&(pAUDVBEncFrameInfoDes->EncFrameInfo)), sizeof(AUDIO_ST_AU_DVB_ENC_FRAME_INFO));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_AU_DVB_ENC_FRAME_INFO *buffer_arg;
            AUDIO_ST_AU_DVB_ENC_FRAME_INFO data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo)
            pAUDVBEncFrameInfoSrc = (PAUDIO_ST_AU_DVB_ENC_FRAME_INFO)pArgs;
            pAUDVBEncFrameInfoDes = (PAUDIO_ST_AU_DVB_ENC_FRAME_INFO)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_AU_DVB_ENC_FRAME_INFO));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pAUDVBEncFrameInfoSrc->EncFrameInfo)), (void*)(&(pAUDVBEncFrameInfoDes->EncFrameInfo)), sizeof(AUDIO_ST_AU_DVB_ENC_FRAME_INFO));
#endif
        }
            break;

        case MApi_CMD_AUDIO_GetEncodeDoneFlag:
        {
#if 0
            //MS_BOOL MApi_AUDIO_GetEncodeDoneFlag(void)
            //u32param[1] = return;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_BOOL MApi_AUDIO_GetEncodeDoneFlag(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);

            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_EncInfo:
        {
#if 0
            //MS_BOOL MApi_AUDIO_SetEncInfo(AUDIO_ENC_INFO_TYPE infoType, MS_U32 param1, MS_U32 param2)
            //Status.bSet = TRUE/FALSE; Status.infoType = infoType; Status.param1 = param1; Status.param2 = param2; Status.bRet = Retuern; Status.u32Ret = Return
            pEncInfoSrc = (PAUDIO_ST_ENC_INFO)pArgs;
            pEncInfoDes = (PAUDIO_ST_ENC_INFO)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_ENC_INFO));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(pEncInfoSrc->bSet == TRUE)
            {
                CPY_to_USER((void*)(&(pEncInfoSrc->bRet)), (void*)(&(pEncInfoDes->bRet)), sizeof(MS_BOOL));
            }
            else
            {
                CPY_to_USER((void*)(&(pEncInfoSrc->u32Ret)), (void*)(&(pEncInfoDes->u32Ret)), sizeof(MS_U32));
            }
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_ENC_INFO *buffer_arg;
            AUDIO_ST_ENC_INFO data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //MS_BOOL MApi_AUDIO_SetEncInfo(AUDIO_ENC_INFO_TYPE infoType, MS_U32 param1, MS_U32 param2)
            //Status.bSet = TRUE/FALSE; Status.infoType = infoType; Status.param1 = param1; Status.param2 = param2; Status.bRet = Retuern; Status.u32Ret = Return
            MS_BOOL bSet;
            MS_U32 u32addr;
            pEncInfoSrc = (PAUDIO_ST_ENC_INFO)pArgs;
            pEncInfoDes = (PAUDIO_ST_ENC_INFO)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_ENC_INFO));
            bSet = pEncInfoDes->bSet;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(bSet == TRUE)
            {
                u32addr = (MS_U32)pEncInfoSrc + _addr_offset(pEncInfoDes, &pEncInfoDes->bRet);
                CPY_to_USER((void*)u32addr, (void*)(&(pEncInfoDes->bRet)), sizeof(MS_BOOL));
            }
            else
            {
                u32addr = (MS_U32)pEncInfoSrc + _addr_offset(pEncInfoDes, &pEncInfoDes->u32Ret);
                CPY_to_USER((void*)u32addr, (void*)(&(pEncInfoDes->u32Ret)), sizeof(MS_U32));
            }
#endif
        }
            break;

        case MApi_CMD_AUDIO_SetCaptureEnable:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SET_CAPTURE_ENABLE *buffer_arg;
            AUDIO_ST_SET_CAPTURE_ENABLE data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_SetCaptureEnable(MS_BOOL bEnable, void* PCM_CB)
            //Status.bEnable = bEnable; Status.PCM_CB = PCM_CB;
            pSetCaptureEnableSrc = (PAUDIO_ST_SET_CAPTURE_ENABLE)pArgs;
            pSetCaptureEnableDes = (PAUDIO_ST_SET_CAPTURE_ENABLE)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SET_CAPTURE_ENABLE));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        //============================================================
        // AUDIO_ATV RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_SIF_SetStandard:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SIF_GetSoundMode:
        {
#if 0
            //MS_U8 MApi_AUDIO_SIF_GetSoundMode(void)
            //u32param[1] = return;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U8 MApi_AUDIO_SIF_GetSoundMode(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32);
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SIF_SetSoundMode:
        {
#if 0
            //MS_BOOL MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode)
            //u32Param[0] = return; u32Param[1] = u8SifSoundMode;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_BOOL MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode)
            //u32Param[0] = return; u32Param[1] = u8SifSoundMode;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SIF_GetAudioStatus:
        {
#if 0
            //MS_BOOL MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus)
            //Status.bStatus = return;  Status.p_eAudioStatus = E_STATE_AUDIO_NO_CARRIER;
            pSIF_GetAudioStatusSrc = (PAUDIO_ST_SIF_GET_AUDIO_STATUS)pArgs;
            pSIF_GetAudioStatusDes = (PAUDIO_ST_SIF_GET_AUDIO_STATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SIF_GET_AUDIO_STATUS));

            pSIF_GetAudioStatusDes->p_eAudioStatus =(AUDIOSTATUS*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)pSIF_GetAudioStatusSrc->p_eAudioStatus, sizeof(AUDIOSTATUS));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(pSIF_GetAudioStatusSrc->p_eAudioStatus), (void*)buffer_array0, sizeof(AUDIOSTATUS));
            CPY_to_USER((void*)(&(pSIF_GetAudioStatusSrc->bStatus)), (void*)(&(pSIF_GetAudioStatusDes->bStatus)), sizeof(MS_BOOL));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SIF_GET_AUDIO_STATUS *buffer_arg;
            AUDIO_ST_SIF_GET_AUDIO_STATUS data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            AUDIOSTATUS *buffer_array0;
            AUDIOSTATUS data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            //MS_BOOL MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus)
            //Status.bStatus = return;  Status.p_eAudioStatus = E_STATE_AUDIO_NO_CARRIER;
            MS_U32 u32addr;
            void* ptmp;

            pSIF_GetAudioStatusSrc = (PAUDIO_ST_SIF_GET_AUDIO_STATUS)pArgs;
            pSIF_GetAudioStatusDes = (PAUDIO_ST_SIF_GET_AUDIO_STATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SIF_GET_AUDIO_STATUS));
            ptmp = pSIF_GetAudioStatusDes->p_eAudioStatus;  // bakup

            pSIF_GetAudioStatusDes->p_eAudioStatus =(AUDIOSTATUS*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(AUDIOSTATUS));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)ptmp, (void*)buffer_array0, sizeof(AUDIOSTATUS));
            CPY_to_USER((void*)pSIF_GetAudioStatusSrc, (void*)(&(pSIF_GetAudioStatusDes->bStatus)), sizeof(MS_BOOL));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SIF_StartAutoStandardDetection:
        {
            //void MApi_AUDIO_SIF_StartAutoStandardDetection(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection:
        {
#if 0
            //AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void)
            //u32param[1] = return;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32);
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard:
        {
#if 0
            //AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard)
            //u32Param[0] = return; u32param[1] = eStandard;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif
            //AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard)
            //u32Param[0] = return; u32param[1] = eStandard;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SIF_SetThreshold:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            THR_TBL_TYPE code *buffer_arg;
            THR_TBL_TYPE code data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_THR_TBLL_TYPE, NULL, buffer_arg,sizeof(buffer_arg));
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(THR_TBL_TYPE code));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SIF_SetPrescale:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //void MApi_AUDIO_SIF_SetPrescale(MS_U8 gain_type, int db_value)
            //u32Param[0] = gain_type; u32Param[1] = db_value;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SIF_IsPALType:
        {
#if 0
            //MS_BOOL MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type)
            //u32Param[0] = return; u32param[1] = pal_type;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_BOOL MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type)
            //u32Param[0] = return; u32param[1] = pal_type;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_SIF_SetPALType:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SIF_SendCmd:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            //void MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2)
            //u32Param[0] = enAudSifCommand; u32Param[1] = comm_arg1; u32Param[2] = comm_arg2;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SIF_Shift:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_SIF_Shift(En_AUD_VIF_Type type)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        //=============================================================
        // AUDIO_SPDIF RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_SPDIF_OP:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SPDIF_OP *buffer_arg;
            AUDIO_ST_SPDIF_OP data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_en)
            //Status.eType = SPDIF_HWEN/SPDIF_SETMUTE/SPDIF_SETMODE/SPDIF_GETMODE; Status.spdif_hwen = spdif_en; Status.mute_en = FALSE; Status.spdif_mode = SPDIF_OUT_NONE;
            pSPDIF_OpSrc = (PAUDIO_ST_SPDIF_OP)pArgs;
            pSPDIF_OpDes = (PAUDIO_ST_SPDIF_OP)buffer_arg;
            AUDIO_SPDIF_OP tmp_Value;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SPDIF_OP));
            tmp_Value = pSPDIF_OpDes->eType;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(tmp_Value == SPDIF_GETMODE)
            {
                CPY_to_USER((void*)pSPDIF_OpSrc, (void*)pSPDIF_OpDes, sizeof(AUDIO_ST_SPDIF_OP));
            }
        }
            break;

        case MApi_CMD_AUDIO_SPDIF_SCMS:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SPDIF_SCMS *buffer_arg;
            AUDIO_ST_SPDIF_SCMS data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en)
            //Status.bSetSCMS = TRUE/FALSE; Status.C_bit_en = C_bit_en; Status.L_bit_en = L_bit_en; Status.Ret_bit = 0;
            pSPDIF_SCMSSrc = (PAUDIO_ST_SPDIF_SCMS)pArgs;
            pSPDIF_SCMSDes = (PAUDIO_ST_SPDIF_SCMS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SPDIF_SCMS));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(pSPDIF_SCMSDes->bSetSCMS == FALSE)
            {
                MS_U32 u32Addr = (MS_U32)(pSPDIF_SCMSSrc) + _addr_offset(pSPDIF_SCMSDes, &pSPDIF_SCMSDes->Ret_bit);
                CPY_to_USER((void*)u32Addr, (void*)(&(pSPDIF_SCMSDes->Ret_bit)), sizeof(MS_U8));
            }
        }
            break;
        case MApi_CMD_AUDIO_SPDIF_Monitor:
        {
            //void MApi_Audio_SPDIF_Monitor(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL:
        {
#if 0
            //MS_BOOL MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
            //u32Param[0] = return; u32param[1] = cs_mode; u32param[2] = status;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            //MS_BOOL MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
            //u32Param[0] = return; u32param[1] = cs_mode; u32param[2] = status;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_DigitalOut_SetChannelStatus:
        {
#if 0
            //MS_BOOL MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
            //Status.bStatus = return; Status.ePath = ePath; Status.stChannelStatus = stChannelStatus;
            pDigitalOut_SetChannelStatusSrc = (PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS)pArgs;
            pDigitalOut_SetChannelStatusDes = (PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS));

            pDigitalOut_SetChannelStatusDes->stChannelStatus = (Digital_Out_Channel_Status_t*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)pDigitalOut_SetChannelStatusSrc->stChannelStatus, sizeof(Digital_Out_Channel_Status_t));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pDigitalOut_SetChannelStatusSrc->bStatus)), (void*)(&(pDigitalOut_SetChannelStatusDes->bStatus)), sizeof(MS_BOOL));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS *buffer_arg;
            AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            Digital_Out_Channel_Status_t *buffer_array0;
            Digital_Out_Channel_Status_t data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            //MS_BOOL MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
            //Status.bStatus = return; Status.ePath = ePath; Status.stChannelStatus = stChannelStatus;
            void* ptmp;
            pDigitalOut_SetChannelStatusSrc = (PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS)pArgs;
            pDigitalOut_SetChannelStatusDes = (PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS));
            ptmp = pDigitalOut_SetChannelStatusDes->stChannelStatus;

            pDigitalOut_SetChannelStatusDes->stChannelStatus = (Digital_Out_Channel_Status_t*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(Digital_Out_Channel_Status_t));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pDigitalOut_SetChannelStatusSrc, (void*)(&(pDigitalOut_SetChannelStatusDes->bStatus)), sizeof(MS_BOOL));
#endif
        }
            break;

        case MApi_CMD_AUDIO_DigitalOut_GetChannelStatus:
        {
#if 0
            pDigitalOut_GetChannelStatusSrc = (PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS)pArgs;
            pDigitalOut_GetChannelStatusDes = (PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DIGITALOUT_GETCHANNELSTATUS));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pDigitalOut_GetChannelStatusSrc->stChannelStatus->u8Category)), (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8Category)), sizeof(MS_U8));
            CPY_to_USER((void*)(&(pDigitalOut_GetChannelStatusSrc->stChannelStatus->stCopyRight.CBit)), (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->stCopyRight.CBit)), sizeof(MS_BOOL));
            CPY_to_USER((void*)(&(pDigitalOut_GetChannelStatusSrc->stChannelStatus->stCopyRight.LBit)), (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->stCopyRight.LBit)), sizeof(MS_BOOL));
            CPY_to_USER((void*)(&(pDigitalOut_GetChannelStatusSrc->stChannelStatus->u8PowerOnOff1)), (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8PowerOnOff1)), sizeof(MS_U8));
            CPY_to_USER((void*)(&(pDigitalOut_GetChannelStatusSrc->stChannelStatus->u8SoundBarWooferLevel)), (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8SoundBarWooferLevel)), sizeof(MS_U8));
            CPY_to_USER((void*)(&(pDigitalOut_GetChannelStatusSrc->stChannelStatus->u8SoundBarAutoVolume)), (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8SoundBarAutoVolume)), sizeof(MS_U8));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_DIGITALOUT_GETCHANNELSTATUS *buffer_arg;
            AUDIO_ST_DIGITALOUT_GETCHANNELSTATUS data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            MS_U32 u32addr;
            pDigitalOut_GetChannelStatusSrc = (PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS)pArgs;
            pDigitalOut_GetChannelStatusDes = (PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DIGITALOUT_GETCHANNELSTATUS));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);

            u32addr = (MS_U32)pDigitalOut_GetChannelStatusSrc + _addr_offset(pDigitalOut_GetChannelStatusDes, &(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8Category));
            CPY_to_USER((void*)u32addr, (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8Category)), sizeof(MS_U8));

            u32addr = (MS_U32)pDigitalOut_GetChannelStatusSrc + _addr_offset(pDigitalOut_GetChannelStatusDes, &(pDigitalOut_GetChannelStatusDes->stChannelStatus->stCopyRight.CBit));
            CPY_to_USER((void*)u32addr, (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->stCopyRight.CBit)), sizeof(MS_BOOL));

            u32addr = (MS_U32)pDigitalOut_GetChannelStatusSrc + _addr_offset(pDigitalOut_GetChannelStatusDes, &(pDigitalOut_GetChannelStatusDes->stChannelStatus->stCopyRight.LBit));
            CPY_to_USER((void*)u32addr, (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->stCopyRight.LBit)), sizeof(MS_BOOL));

            u32addr = (MS_U32)pDigitalOut_GetChannelStatusSrc + _addr_offset(pDigitalOut_GetChannelStatusDes, &(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8PowerOnOff1));
            CPY_to_USER((void*)u32addr, (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8PowerOnOff1)), sizeof(MS_U8));

            u32addr = (MS_U32)pDigitalOut_GetChannelStatusSrc + _addr_offset(pDigitalOut_GetChannelStatusDes, &(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8SoundBarWooferLevel));
            CPY_to_USER((void*)u32addr, (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8SoundBarWooferLevel)), sizeof(MS_U8));

            u32addr = (MS_U32)pDigitalOut_GetChannelStatusSrc + _addr_offset(pDigitalOut_GetChannelStatusDes, &(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8SoundBarAutoVolume));
            CPY_to_USER((void*)u32addr, (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8SoundBarAutoVolume)), sizeof(MS_U8));
#endif
        }
            break;

        //=============================================================
        // AUDIO_HDMI RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_HDMI_Tx_SetMute:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_HDMI_Tx_GetStatus:
        {
#if 0
            //void MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
            //Status.onOff = return; Status.hdmi_smpFreq = return; Status.outType = return;
            pHDMI_TX_GetStatusSrc = (PAUDIO_HDMI_TX_GETSTATUS)pArgs;
            pHDMI_TX_GetStatusDes = (PAUDIO_HDMI_TX_GETSTATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_HDMI_TX_GETSTATUS));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pHDMI_TX_GetStatusSrc->onOff)), (void*)(&(pHDMI_TX_GetStatusDes->onOff)), sizeof(MS_BOOL));
            CPY_to_USER((void*)(&(pHDMI_TX_GetStatusSrc->hdmi_smpFreq)), (void*)(&(pHDMI_TX_GetStatusDes->hdmi_smpFreq)), sizeof(AUDIO_FS_TYPE));
            CPY_to_USER((void*)(&(pHDMI_TX_GetStatusSrc->outType)), (void*)(&(pHDMI_TX_GetStatusDes->outType)), sizeof(HDMI_TX_OUTPUT_TYPE));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_HDMI_TX_GETSTATUS *buffer_arg;
            AUDIO_HDMI_TX_GETSTATUS data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
            //Status.onOff = return; Status.hdmi_smpFreq = return; Status.outType = return;
            pHDMI_TX_GetStatusSrc = (PAUDIO_HDMI_TX_GETSTATUS)pArgs;
            pHDMI_TX_GetStatusDes = (PAUDIO_HDMI_TX_GETSTATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_HDMI_TX_GETSTATUS));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pHDMI_TX_GetStatusSrc, (void*)pHDMI_TX_GetStatusDes, sizeof(AUDIO_HDMI_TX_GETSTATUS));
#endif
        }
            break;

        case MApi_CMD_AUDIO_HDMI_Monitor:
        {
#if 0
            //MS_U8 MApi_AUDIO_HDMI_Monitor(void)
            //u32param[1] = return;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U8 MApi_AUDIO_HDMI_Monitor(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_HDMI_GetNonpcmFlag:
        {
#if 0
            //MS_BOOL MApi_AUDIO_HDMI_GetNonpcmFlag(void)
            //u32param[1] = return;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif
            //MS_BOOL MApi_AUDIO_HDMI_GetNonpcmFlag(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32);
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_HDMI_SetNonpcm:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_HDMI_Nonpcm_OP:
        {
#if 0
            //AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en)
            //Status.bSet = TRUE/FALSE; Status.nonPCM_en = nonPCM_en;Status.eRet = return;
            pHDMI_NonPCM_OpSrc = (PAUDIO_ST_HDMI_NONPCM_OP)pArgs;
            pHDMI_NonPCM_OpDes = (PAUDIO_ST_HDMI_NONPCM_OP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_HDMI_NONPCM_OP));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(pHDMI_NonPCM_OpSrc->bSet == FALSE)
            {
                CPY_to_USER((void*)(&(pHDMI_NonPCM_OpSrc->eRet)), (void*)(&(pHDMI_NonPCM_OpDes->eRet)), sizeof(AUDIO_HDMI_RX_TYPE));
            }
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_HDMI_NONPCM_OP *buffer_arg;
            AUDIO_ST_HDMI_NONPCM_OP data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en)
            //Status.bSet = TRUE/FALSE; Status.nonPCM_en = nonPCM_en;Status.eRet = return;
            MS_BOOL bSet;
            MS_U32 u32addr;
            pHDMI_NonPCM_OpSrc = (PAUDIO_ST_HDMI_NONPCM_OP)pArgs;
            pHDMI_NonPCM_OpDes = (PAUDIO_ST_HDMI_NONPCM_OP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_HDMI_NONPCM_OP));
            bSet = pHDMI_NonPCM_OpDes-> bSet;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(bSet == FALSE)
            {
                u32addr = (MS_U32)pHDMI_NonPCM_OpSrc + _addr_offset(pHDMI_NonPCM_OpDes,&(pHDMI_NonPCM_OpDes->eRet));
                CPY_to_USER((void*)u32addr, (void*)(&(pHDMI_NonPCM_OpDes->eRet)), sizeof(AUDIO_HDMI_RX_TYPE));
            }
#endif
        }
            break;

        case MApi_CMD_AUDIO_DTV_HDMI_CFG:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_DTV_HDMI_CFG(MS_U8 ctrl)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_HDMI_GetSynthFreq:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif
            //MS_U16 MApi_AUDIO_HDMI_GetSynthFreq(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_HDMI_SetDownSample:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_HDMI_SetDownSample(MS_U8 ratio)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_HDMI_TX_SetMode:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_RX_HDMI_AUDIO_STATUS *buffer_arg;
            AUDIO_ST_RX_HDMI_AUDIO_STATUS data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            ST_HDMI_AUDIO_STATUS *buffer_array0;
            ST_HDMI_AUDIO_STATUS data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            //MS_BOOL MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts)
            //Status.bStatus = Return;  Status.pHdmi_status = p_hdmiAudioSts;
            void* ptmp;
            pHDMI_RX_AudioStatusSrc = (PAUDIO_ST_RX_HDMI_AUDIO_STATUS)pArgs;
            pHDMI_RX_AudioStatusDes = (PAUDIO_ST_RX_HDMI_AUDIO_STATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_RX_HDMI_AUDIO_STATUS));
            ptmp = pHDMI_RX_AudioStatusDes->pHdmi_status;

            pHDMI_RX_AudioStatusDes->pHdmi_status = (ST_HDMI_AUDIO_STATUS*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(ST_HDMI_AUDIO_STATUS));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)ptmp, (void*)buffer_array0, sizeof(ST_HDMI_AUDIO_STATUS));
            //bStatus is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pHDMI_RX_AudioStatusSrc, (void*)(&(pHDMI_RX_AudioStatusDes->bStatus)), sizeof(MS_BOOL));
        }
            break;
        //=============================================================
        // AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_ADVSOUND_ProcessEnable:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif
            //MS_BOOL MApi_AUDIO_ADVSOUND_ProcessEnable(ADVSND_TYPE type)
            //u32Param[0] = return; u32param[1] = type;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            //MS_BOOL MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable)
            //u32Param[0] = return; u32param[1] = proc; u32param[2] = enable;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_ADVSOUND_SetParam:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[4];
#endif

            //MS_BOOL MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U16 u16value1, MS_U16 u16value2)
            //u32Param[0] = return; u32param[1] = param; u32param[2] = u16value1; u32param[3] = u16value2;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_ADVSND_SetParam:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[4];
#endif

            //MS_BOOL MApi_AUDIO_ADVSND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
            //u32Param[0] = return; u32param[1] = param; u32param[2] = u32value1 ;u32param[3] = u16value2;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_ADVSOUND_Param2:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_ADVSOUND_PARAM2 *buffer_arg;
            AUDIO_ST_ADVSOUND_PARAM2 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            MS_U32 u32CopySize = 0;
            void * ptmp = NULL;
            pADVSound_Param2Src = (PAUDIO_ST_ADVSOUND_PARAM2)pArgs;
            pADVSound_Param2Des = (PAUDIO_ST_ADVSOUND_PARAM2)buffer_arg;
            CPY_FROM_USER((void *)pADVSound_Param2Des, (void *)pADVSound_Param2Src, sizeof(AUDIO_ST_ADVSOUND_PARAM2));
            ptmp = pADVSound_Param2Des->u32value1; // store pADVSound_Param2Src->u32value1

            if (pADVSound_Param2Des->u16value2 > 0)
            {
		        MS_U32 *u32value1;

                u32CopySize = pADVSound_Param2Des->u16value2 * sizeof(MS_U32);
                u32value1 = (MS_U32 *)kzalloc(u32CopySize, GFP_KERNEL);
                pADVSound_Param2Des->u32value1 = u32value1;
                if (pADVSound_Param2Des->u32value1 != NULL)
                {
                    CPY_FROM_USER((void *)pADVSound_Param2Des->u32value1, (void*)ptmp, u32CopySize);
                    u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pADVSound_Param2Des);
                    MS_U32 u32Addr = (MS_U32)pADVSound_Param2Src + _addr_offset(pADVSound_Param2Des, &pADVSound_Param2Des->bRet);
                    CPY_to_USER((void*)u32Addr, (void*)(&(pADVSound_Param2Des->bRet)), sizeof(MS_BOOL));

                    if (pADVSound_Param2Des->bSet == FALSE)
                    {
                        CPY_to_USER((void *)pADVSound_Param2Src->u32value1, (void *)pADVSound_Param2Des->u32value1, u32CopySize);
                    }

                    kfree(u32value1);
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                }
            }
            else
            {
                    u32Ret = UTOPIA_STATUS_ERR_INV;
            }

            break;
        }

        case MApi_CMD_AUDIO_ADVSOUND_GetInfo:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U32 MApi_AUDIO_ADVSOUND_GetInfo(AdvSound_GET_Type Type)
            //u32Param[0] = return; u32param[1] = Type;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_DBXTV_SetMode:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[4];
#endif

            //void MApi_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable)
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SeInit:
        {
            //void MApi_AUDIO_SeInit(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_SetAdvSndSys:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_SetAdvSndSys(MS_U8 u8AdvSurrMod)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_ADV_SOUND_SET:
        {


#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_ADV_SOUND_SETPARAM *buffer_arg;
            AUDIO_ST_ADV_SOUND_SETPARAM data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 buffer_array0[2];
#endif

            //void MApi_AUDIO_ADVSND_Set_Sound_Param(AdvSound_SET_Type Type, MS_U32 *pParam) // New api for cover below functions
            void* ptmp;
            pADV_Sound_SetParamSrc = (PAUDIO_ST_ADV_SOUND_SETPARAM)pArgs;
            pADV_Sound_SetParamDes = (PAUDIO_ST_ADV_SOUND_SETPARAM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_ADV_SOUND_SETPARAM));

            ptmp = pADV_Sound_SetParamDes->pParam;
            pADV_Sound_SetParamDes->pParam = (MS_U32*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U32) * 2);

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_Copy_Parameter:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_COPY_PARAMETER *buffer_arg;
            AUDIO_ST_COPY_PARAMETER data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 *buffer_array0;
            MS_U32 data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            //void MApi_AUDIO_COPY_Parameter(ADVSND_PARAM type, MS_U32 *Parameter_ptr,MS_U32 size)
            void* ptmp;
            pCopyParameterSrc = (PAUDIO_ST_COPY_PARAMETER)pArgs;
            pCopyParameterDes = (PAUDIO_ST_COPY_PARAMETER)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_COPY_PARAMETER));

            ptmp = pCopyParameterDes->Parameter_ptr;
            pCopyParameterDes->Parameter_ptr = (MS_U32*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U32));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)ptmp, (void*)buffer_array0, sizeof(MS_U32));
        }
            break;
        //=============================================================
        // AUDIO_KTV RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_KTV_OP:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_KTV_OP *buffer_arg;
            AUDIO_ST_KTV_OP data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //MS_BOOL MApi_AUDIO_SetKTVInfo(AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
            //Status.eType = KTV_SET_INFO; Status.modelType = modelType; Status.paramType = AUD_KTV_ParamType1; Status.param1 = param1; Status.param2 = param2; Status.bRet = Return;
            AUDIO_KTV_OP eType;
            MS_U32 u32addr;
            pKTV_OpSrc = (PAUDIO_ST_KTV_OP)pArgs;
            pKTV_OpDes = (PAUDIO_ST_KTV_OP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_KTV_OP));
            eType = pKTV_OpDes->eType;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(eType == KTV_SET_INFO)
            {
                u32addr=(MS_U32)pKTV_OpSrc + _addr_offset(pKTV_OpDes,&(pKTV_OpDes->bRet));
                CPY_to_USER((void*)u32addr, (void*)(&(pKTV_OpDes->bRet)), sizeof(MS_BOOL));
            }
            if(eType == KTV_GET_INFO)
            {
                u32addr=(MS_U32)pKTV_OpSrc + _addr_offset(pKTV_OpDes,&(pKTV_OpDes->u32Ret));
                CPY_to_USER((void*)u32addr, (void*)(&(pKTV_OpDes->u32Ret)), sizeof(MS_U32));
            }
        }
            break;

        case MApi_CMD_AUDIO_PlayMenuSound:
        {
            //void MApi_AUDIO_PlayMenuSound(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;
        //=============================================================
        // AUDIO_MM RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_SetCommAudioInfo:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[4];
#endif

            //MS_BOOL MApi_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_U32 param1, MS_U32 param2 )
            //u32Param[0] = return u32param[1] = infoType; u32param[2] = param1; u32param[3] = param2;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_SetDecodeParam:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SETDECODEPARAM *buffer_arg;
            AUDIO_ST_SETDECODEPARAM data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 buffer_array0[6];
#endif

            //MS_BOOL MApi_ADEC_SetDecodeParam( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32* Param )
            //Status.DecID = DecId; Status.paramType = paramType; Status.Param = Param;
            void* ptmp;
            pSetDecodeParamSrc = (PAUDIO_ST_SETDECODEPARAM)pArgs;
            pSetDecodeParamDes = (PAUDIO_ST_SETDECODEPARAM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SETDECODEPARAM));
            ptmp = pSetDecodeParamDes->Param;   // bakup

            pSetDecodeParamDes->Param = (MS_U32*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U32) * 6);

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(!(pSetDecodeParamDes->paramType == Audio_ParamType_setXPCMDecParam))
            {
                // bStatus is the first member of structure, so no need to calculate offset
                CPY_to_USER((void*)pSetDecodeParamSrc, (void*)(&(pSetDecodeParamDes->bStatus)), sizeof(MS_BOOL));
            }
        }
            break;
        case MApi_CMD_AUDIO_GetCommAudioInfo:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U64 buffer_arg[2];
#endif

            //long long MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType )
            //(long long)u64param[0] = return, u64param[1] = infoType;
            pu64Src = (MS_U64*)pArgs;
            pu64Des = (MS_U64*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U64) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            // pointer of first member of an array, is same as head of the array
            CPY_to_USER((void*)pu64Src, (void*)(&(pu64Des[0])), sizeof(MS_U64));
        }
            break;

        case MApi_CMD_AUDIO_GetMpegInfo:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif
            //MS_U32 MApi_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetAC3Info:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif
            //MS_U32 MApi_AUDIO_GetAC3Info(Audio_AC3_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetAC3PInfo:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U32 MApi_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetAACInfo:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U32 MApi_AUDIO_GetAACInfo( Audio_AAC_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetWmaInfo:
        {


#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U32 MApi_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetDTSInfo:
        {


#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U32 MApi_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_XPCM2_CheckIntStatus:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U16 MApi_AUDIO_XPCM2_CheckIntStatus(void)
            //u32Param[1] = return
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_RA8_Param:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_RA8_Param *buffer_arg;
            AUDIO_ST_RA8_Param data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U16 buffer_array0[5];
            MS_U16 buffer_array1[5];
            MS_U16 buffer_array2[5];
            MS_U16 buffer_array3[5];
            MS_U16 buffer_array4[5];
#endif

            //MS_U8 MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate, MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits, MS_U16* FrameSize)
            void* ptmp_Channels;
            void* ptmp_cplQbits;
            void* ptmp_cplStart;
            void* ptmp_FrameSize;
            void* ptmp_Regions;
            pRA8_ParamSrc = (PAUDIO_ST_RA8_Param)pArgs;
            pRA8_ParamDes = (PAUDIO_ST_RA8_Param)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_RA8_Param));
            ptmp_Channels = pRA8_ParamDes->pChannels;
            ptmp_cplQbits = pRA8_ParamDes->pcplQbits;
            ptmp_cplStart = pRA8_ParamDes->pcplStart;
            ptmp_FrameSize = pRA8_ParamDes->pFrameSize;
            ptmp_Regions = pRA8_ParamDes->pRegions;

            pRA8_ParamDes->pChannels    = (MS_U16*)buffer_array0;
            pRA8_ParamDes->pcplQbits    = (MS_U16*)buffer_array1;
            pRA8_ParamDes->pcplStart    = (MS_U16*)buffer_array2;
            pRA8_ParamDes->pFrameSize   = (MS_U16*)buffer_array3;
            pRA8_ParamDes->pRegions     = (MS_U16*)buffer_array4;

            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp_Channels,    sizeof(MS_U16) * 5);
            CPY_FROM_USER((void*)buffer_array1, (void*)ptmp_cplQbits,    sizeof(MS_U16) * 5);
            CPY_FROM_USER((void*)buffer_array2, (void*)ptmp_cplStart,    sizeof(MS_U16) * 5);
            CPY_FROM_USER((void*)buffer_array3, (void*)ptmp_FrameSize,   sizeof(MS_U16) * 5);
            CPY_FROM_USER((void*)buffer_array4, (void*)ptmp_Regions,     sizeof(MS_U16) * 5);

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            //u8Status is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pRA8_ParamSrc, (void*)(&(pRA8_ParamDes->u8Status)), sizeof(MS_U8));
        }
            break;

        case MApi_CMD_AUDIO_Init:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_StartDecode:
        {
            //void MApi_AUDIO_StartDecode(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_StartBroswe:
        {
            //void MApi_AUDIO_StartBrowse(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_StopDecode:
        {
            //void MApi_AUDIO_StopDecode(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_PauseDecode:
        {
            //void MApi_AUDIO_PauseDecode(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_CheckPlayDone:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U8 MApi_AUDIO_CheckPlayDone(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetResidualBufferSize:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U16 MApi_AUDIO_GetResidualBufferSize(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetPCMBufferSize:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U16 MApi_AUDIO_GetPCMBufferSize(MS_U16 u16BitRate)
            //u32Param[0] = return; u32param[1] = u16BitRate;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetPCMBufferSize2:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif

            //MS_U16 MApi_AUDIO_GetPCMBufferSize2(MS_U16 u16BitRate,MS_U16 u16SampleRate)
            //u32Param[0] = return u32param[1] = u16BitRate; u32param[2] = u16SampleRate;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetCurrentFrameNumber:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U32 MApi_AUDIO_GetCurrentFrameNumber(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetSampleRates:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U16 MApi_AUDIO_GetSampleRate(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetBitRates:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U32 MApi_AUDIO_GetBitRate(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetLayer:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //Audio_MP3_LAYER MApi_AUDIO_GetLayer(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_CheckInputRequest:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_CHECK_INPUT_REQ *buffer_arg;
            AUDIO_ST_CHECK_INPUT_REQ data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 *buffer_array1;
            MS_U32 data_buffer_array1;
            buffer_array1 = &data_buffer_array1;

            MS_U32 *buffer_array2;
            MS_U32 data_buffer_array2;
            buffer_array2 = &data_buffer_array2;
#endif

            //MS_U8 MApi_AUDIO_CheckInputRequest(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
            //Status.u8Status = return; Status.pU32WrtAddr = pU32WrtAddr; Status.pU32WrtBytes = pU32WrtBytes;
            void* ptmp_U32WrtAddr;
            void* ptmp_U32WrtBytes;

            pCheckInputReqSrc = (PAUDIO_ST_CHECK_INPUT_REQ)pArgs;
            pCheckInputReqDes = (PAUDIO_ST_CHECK_INPUT_REQ)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_CHECK_INPUT_REQ));

            ptmp_U32WrtAddr = pCheckInputReqDes->pU32WrtAddr;
            ptmp_U32WrtBytes = pCheckInputReqDes->pU32WrtBytes;
            pCheckInputReqDes->pU32WrtAddr = (MS_U32*)buffer_array1;
            pCheckInputReqDes->pU32WrtBytes = (MS_U32*)buffer_array2;
            CPY_FROM_USER((void*)buffer_array1, (void*)ptmp_U32WrtAddr, sizeof(MS_U32));
            CPY_FROM_USER((void*)buffer_array2, (void*)ptmp_U32WrtBytes, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);

            //u8Status is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pCheckInputReqSrc, (void*)(&(pCheckInputReqDes->u8Status)), sizeof(MS_U8));
            CPY_to_USER((void*)ptmp_U32WrtAddr, (void*)buffer_array1, sizeof(MS_U32));
            CPY_to_USER((void*)ptmp_U32WrtBytes, (void*)buffer_array2, sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_SetInput:
        {
            //void MApi_AUDIO_SetInput(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_SetSampleRateIndex:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_SetSampleRateIndex(MS_U16 u16Index)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_FileEndNotification:
        {
            //void MApi_AUDIO_FileEndNotification(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_FileEndDataHandle:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_GetPlayTick:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U32 MApi_AUDIO_GetPlayTick(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_GetEsMEMCnt:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U16 MApi_AUDIO_GetEsMEMCnt(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_SetASFParm:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //void MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value)
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_MM_SetInput:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //void MApi_AUDIO_MM_SetInput (En_DVB_AudioDecoder AUDDecoderNo, MS_U8 u8IntTag)
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_MM_CheckPlayDone:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_U32 MApi_AUDIO_MM_CheckPlayDone (En_DVB_AudioDecoder AUDDecoderNo)
            //u32Param[0] = return u32param[1] = AUDDecoderNo;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_MM_CheckInputRequest:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_MM_CHECK_INPUT_REQ *buffer_arg;
            AUDIO_ST_MM_CHECK_INPUT_REQ data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 *buffer_array1;
            MS_U32 data_buffer_array1;
            buffer_array1 = &data_buffer_array1;

            MS_U32 *buffer_array2;
            MS_U32 data_buffer_array2;
            buffer_array2 = &data_buffer_array2;
#endif

            //MS_U8 MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
            //Status.u8Status = return; Status.AUDDecoderNo = AUDDecoderNo;Status.pU32WrtAddr = pU32WrtAddr; Status.pU32WrtBytes = pU32WrtBytes;
            void* ptmp_U32WrtAddr;
            void* ptmp_U32WrtBytes;
            pMMCheckInputReqSrc = (PAUDIO_ST_MM_CHECK_INPUT_REQ)pArgs;
            pMMCheckInputReqDes = (PAUDIO_ST_MM_CHECK_INPUT_REQ)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_MM_CHECK_INPUT_REQ));

            ptmp_U32WrtAddr = pMMCheckInputReqDes->pU32WrtAddr;
            ptmp_U32WrtBytes = pMMCheckInputReqDes->pU32WrtBytes;
            pMMCheckInputReqDes->pU32WrtAddr = (MS_U32*)buffer_array1;
            pMMCheckInputReqDes->pU32WrtBytes = (MS_U32*)buffer_array2;
            CPY_FROM_USER((void*)buffer_array1, (void*)ptmp_U32WrtAddr, sizeof(MS_U32));
            CPY_FROM_USER((void*)buffer_array2, (void*)ptmp_U32WrtBytes, sizeof(MS_U32));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);

            //u8Status is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pMMCheckInputReqSrc, (void*)(&(pMMCheckInputReqDes->u8Status)), sizeof(MS_U8));
            CPY_to_USER((void*)ptmp_U32WrtAddr, (void*)buffer_array1, sizeof(MS_U32));
            CPY_to_USER((void*)ptmp_U32WrtBytes, (void*)buffer_array2, sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_DmaReader_Init:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            PCM_OUTPUT_FORMAT *buffer_arg;
            PCM_OUTPUT_FORMAT data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_DmaReader_Init(PCM_OUTPUT_FORMAT* pSettings)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(PCM_OUTPUT_FORMAT));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_DmaReader_AllInput_Init:
        {
            //void MApi_AUDIO_DmaReader_AllInput_Init(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_DmaReader_WritePCM:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_DMAREADER_WRITEPCM *buffer_arg;
            AUDIO_ST_DMAREADER_WRITEPCM data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            void* ptmp_Buffer;
            void* pPCMData;
            //MS_BOOL MApi_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes)
            //Status.bStatus = return; Status.pBuffer = buffer; Status.bytes = bytes;
            pDMAReader_WritePCMSrc = (PAUDIO_ST_DMAREADER_WRITEPCM)pArgs;
            pDMAReader_WrtiePCMDes = (PAUDIO_ST_DMAREADER_WRITEPCM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DMAREADER_WRITEPCM));

            pPCMData = kmalloc(pDMAReader_WrtiePCMDes->bytes, GFP_KERNEL);
            ptmp_Buffer = pDMAReader_WrtiePCMDes ->pBuffer;
            CPY_FROM_USER(pPCMData, (void*)ptmp_Buffer, pDMAReader_WrtiePCMDes->bytes);
            pDMAReader_WrtiePCMDes->pBuffer = pPCMData;
            //pDMAReader_WrtiePCMDes->pBuffer = (void*)buffer_array0;
            //CPY_FROM_USER((void*)buffer_array0, (void*)pDMAReader_WritePCMSrc->pBuffer, pDMAReader_WritePCMSrc->bytes);


            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            //bStatus is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pDMAReader_WritePCMSrc, (void*)(&(pDMAReader_WrtiePCMDes->bStatus)), sizeof(MS_BOOL));
            //CPY_to_USER((void*)(pDMAReader_WritePCMSrc->pBuffer), (void*)buffer_array0, sizeof(MS_U32));

            kfree(pPCMData);
        }
            break;

        case MApi_CMD_AUDIO_SwDmaReader_Init:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            PCM_OUTPUT_FORMAT *buffer_arg;
            PCM_OUTPUT_FORMAT data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_SwDmaReader_Init(PCM_OUTPUT_FORMAT* pSettings)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_SWDMAREADER_INIT, NULL, buffer_arg,sizeof(buffer_arg));
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(PCM_OUTPUT_FORMAT));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SwDmaReader_WritePCM:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SWDMAREADER_WRITEPCM *buffer_arg;
            AUDIO_ST_SWDMAREADER_WRITEPCM data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            void* ptmp_buffer;
            void* pPCMData;
            MS_U32 u32addr;
            //MS_BOOL MApi_AUDIO_SwDmaReader_WritePCM(void* buffer, MS_U32 bytes)
            //Status.buffer = buffer; Status.bytes = bytes; Status.bRet = return;
            pSwDmaReaderWritePCMSrc = (PAUDIO_ST_SWDMAREADER_WRITEPCM)pArgs;
            pSwDmaReaderWritePCMDes = (PAUDIO_ST_SWDMAREADER_WRITEPCM)buffer_arg;
            //CPY_FROM_USER((void *)&pSwDmaReaderWritePCMDes->u8Idx, (void*)&pSwDmaReaderWritePCMSrc->u8Idx, sizeof(MS_U8));
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SWDMAREADER_WRITEPCM));
/*
    MS_U8                       u8Idx;

    void*                       buffer;
    MS_U32                      bytes;
*/
            pPCMData = kmalloc(pSwDmaReaderWritePCMDes->bytes, GFP_KERNEL);
            ptmp_buffer = pSwDmaReaderWritePCMDes->buffer;
            CPY_FROM_USER(pPCMData, (void*)ptmp_buffer, pSwDmaReaderWritePCMDes->bytes);
            pSwDmaReaderWritePCMDes->buffer = pPCMData;
            //pSwDmaReaderWritePCMDes->buffer = (void*)buffer_array0;
            //CPY_FROM_USER((void*)buffer_array0, (void*)pSwDmaReaderWritePCMSrc->buffer, sizeof(MS_U32));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pSwDmaReaderWritePCMSrc + _addr_offset(pSwDmaReaderWritePCMDes, &(pSwDmaReaderWritePCMDes->bRet));
            CPY_to_USER((void*)u32addr, (void*)(&(pSwDmaReaderWritePCMDes->bRet)), sizeof(MS_BOOL));
            //CPY_to_USER((void*)(pSwDmaReaderWritePCMSrc->buffer), (void*)buffer_array0, sizeof(MS_U32));

            kfree(pPCMData);
        }
            break;

        case MApi_CMD_AUDIO_DmaWrite_Init:
        {
            //void MApi_AUDIO_DmaWriter_Init(void)
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_USBPCM_Enable:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_USBPCM_Enable(MS_BOOL bEnable)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_USBPCM_SetFlag:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_USBPCM_GetFlag:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif
            //MS_BOOL MApi_AUDIO_USBPCM_GetFlag(void)
            //u32param[1] = return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_USBPCM_GetMemInfo:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_UPLOAD_INFO *buffer_arg;
            AUDIO_UPLOAD_INFO data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_UPLOAD_INFO, NULL, buffer_arg,sizeof(buffer_arg));
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_UPLOAD_INFO));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_PCMCapture_Init:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMCAPTURE_INIT *buffer_arg;
            AUDIO_ST_PCMCAPTURE_INIT data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //MS_BOOL MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource)
            // Status.bStatus = return; Status.eID = eID; Status.eSource = eSource;
            pPCMCaptureInitSrc = (PAUDIO_ST_PCMCAPTURE_INIT)pArgs;
            pPCMCaptureInitDes = (PAUDIO_ST_PCMCAPTURE_INIT)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_PCMCAPTURE_INIT));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            //bStatus is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pPCMCaptureInitSrc, (void*)(&(pPCMCaptureInitDes->bStatus)), sizeof(MS_BOOL));
        }
            break;

        case MApi_CMD_AUDIO_PCMCapture_Start:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMCAPTURE_INIT *buffer_arg;
            AUDIO_ST_PCMCAPTURE_INIT data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //MS_BOOL MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID)
            // Status.bStatus = Return; Status.eID = eID;
            pPCMCaptureInitSrc = (PAUDIO_ST_PCMCAPTURE_INIT)pArgs;
            pPCMCaptureInitDes = (PAUDIO_ST_PCMCAPTURE_INIT)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_PCMCAPTURE_INIT));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            //bStatus is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pPCMCaptureInitSrc, (void*)(&(pPCMCaptureInitDes->bStatus)), sizeof(MS_BOOL));
        }
            break;

        case MApi_CMD_AUDIO_PCMCapture_Stop:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMCAPTURE_INIT *buffer_arg;
            AUDIO_ST_PCMCAPTURE_INIT data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //MS_BOOL MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID)
            // Status.bStatus = Return; Status.eID = eID;
            pPCMCaptureInitSrc = (PAUDIO_ST_PCMCAPTURE_INIT)pArgs;
            pPCMCaptureInitDes = (PAUDIO_ST_PCMCAPTURE_INIT)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_PCMCAPTURE_INIT));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            //bStatus is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pPCMCaptureInitSrc, (void*)(&(pPCMCaptureInitDes->bStatus)), sizeof(MS_BOOL));
        }
            break;

        case MApi_CMD_AUDIO_PCMCapture_Read:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMCAPTURE_READ *buffer_arg;
            AUDIO_ST_PCMCAPTURE_READ data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            void* ptmp_Buffer;
            MS_U32 tmp_bytes;
            //MS_BOOL MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes)
            // Status.bStatus = Return;Status.eID = eID;Status.pBuffer = buffer;Status.bytes = bytes;
            pPCMCaptureReadSrc = (PAUDIO_ST_PCMCAPTURE_READ)pArgs;
            pPCMCaptureReadDes = (PAUDIO_ST_PCMCAPTURE_READ)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_PCMCAPTURE_READ));

            void* pPCMCaptureData;
            void* pPCMCaptureData_Output;
            tmp_bytes = pPCMCaptureReadDes->bytes;
            pPCMCaptureData = kmalloc(tmp_bytes, GFP_KERNEL);
            ptmp_Buffer = pPCMCaptureReadDes->pBuffer;
            CPY_FROM_USER(pPCMCaptureData, ptmp_Buffer, pPCMCaptureReadDes->bytes);

            pPCMCaptureData_Output = ptmp_Buffer;
            pPCMCaptureReadDes->pBuffer = pPCMCaptureData;

            //pPCMCaptureReadDes->pBuffer = (void*)buffer_array0;
            //CPY_FROM_USER((void*)buffer_array0, (void*)pPCMCaptureReadSrc->pBuffer, sizeof(MS_U32));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            //bStatus is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pPCMCaptureReadSrc, (void*)(&(pPCMCaptureReadDes->bStatus)), sizeof(MS_BOOL));
            CPY_to_USER(pPCMCaptureData_Output, pPCMCaptureReadDes->pBuffer, tmp_bytes);
            //CPY_to_USER((void*)(pPCMCaptureReadSrc->pBuffer), (void*)buffer_array0, sizeof(MS_U32));
            kfree(pPCMCaptureData);
        }
            break;

        case MApi_CMD_AUDIO_VoIP_Config:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[4];
#endif

            //MS_BOOL MApi_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2)
            //u32Param[0] = return u32param[1] = configType; u32param[2] = param1; u32param[3] = param2;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_ALSA_Check:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_BOOL MApi_AUDIO_ALSA_Check(void)
            //u32param[1] = Return;
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_ALSA_Enable:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif

            //MS_BOOL MApi_AUDIO_ALSA_Enable(MS_BOOL bEnable)
            //u32Param[0] = return u32param[1] = bEnable;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_UNI_CheckDecodeDone:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_UNI_CHECK_DECODEDONE *buffer_arg;
            AUDIO_ST_UNI_CHECK_DECODEDONE data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 *buffer_array1;
            MS_U32 data_buffer_array1;
            buffer_array1 = &data_buffer_array1;

            MS_U32 *buffer_array2;
            MS_U32 data_buffer_array2;
            buffer_array2 = &data_buffer_array2;
#endif

            void* ptmp_U32WrtAddr;
            void* ptmp_pU32WrtBytes;
            //MS_BOOL MApi_AUDIO_UNI_CheckDecodeDone(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
            // Status.bStatus = return ;Status.pU32WrtAddr = pU32WrtAddr; Status.pU32WrtBytes = pU32WrtBytes;
            pUNI_Check_DecodeModeSrc = (PAUDIO_ST_UNI_CHECK_DECODEDONE)pArgs;
            pUNI_Check_DecodeModeDes = (PAUDIO_ST_UNI_CHECK_DECODEDONE)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_UNI_CHECK_DECODEDONE));

            ptmp_U32WrtAddr = pUNI_Check_DecodeModeDes->pU32WrtAddr;
            ptmp_pU32WrtBytes = pUNI_Check_DecodeModeDes->pU32WrtBytes;
            pUNI_Check_DecodeModeDes->pU32WrtAddr = (MS_U32*)buffer_array1;
            pUNI_Check_DecodeModeDes->pU32WrtBytes = (MS_U32*)buffer_array2;
            CPY_FROM_USER((void*)buffer_array1, (void*)ptmp_U32WrtAddr, sizeof(MS_U32));
            CPY_FROM_USER((void*)buffer_array2, (void*)ptmp_pU32WrtBytes, sizeof(MS_U32));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);

            //bStatus is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pUNI_Check_DecodeModeSrc, (void*)(&(pUNI_Check_DecodeModeDes->bStatus)), sizeof(MS_BOOL));
            CPY_to_USER((void*)ptmp_U32WrtAddr, (void*)buffer_array1, sizeof(MS_U32));
            CPY_to_USER((void*)ptmp_pU32WrtBytes, (void*)buffer_array2, sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_UNI_SetOutput:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_UNI_SetOutput (MS_U32 PCMOutCnt)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_UNI_Set_PCMInputWriteAddr (MS_U32 PCMIn_Wptr)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_UNI_Get_OutPCMLevel:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[2];
#endif
            //MS_U32 MApi_AUDIO_UNI_Get_OutPCMLevel (void)
            //u32param[1] = return
            MS_U32 u32addr;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pu32Src + sizeof(MS_U32); // pu32Src[1]
            CPY_to_USER((void*)u32addr, (void*)(&(pu32Des[1])), sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_RingTask:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_AUDIORINGTASK *buffer_arg;
            AUDIO_ST_AUDIORINGTASK data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            audioRingTask *buffer_array0;
            audioRingTask data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif
            //void MApi_AUDIO_RingTask(audioRingTask* auRingTask)
            void* ptmp_auRingTask;
            pAudioRingTaskSrc = (PAUDIO_ST_AUDIORINGTASK)pArgs;
            pAudioRingTaskDes = (PAUDIO_ST_AUDIORINGTASK)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_AUDIORINGTASK));

            ptmp_auRingTask = pAudioRingTaskDes->auRingTask;
            pAudioRingTaskDes->auRingTask = (audioRingTask*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp_auRingTask, sizeof(audioRingTask));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)ptmp_auRingTask, (void*)buffer_array0, sizeof(audioRingTask));
        }
            break;

        case MApi_CMD_AUDIO_Ring_DataTransfer:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_RING_DATA_TRANS *buffer_arg;
            AUDIO_ST_RING_DATA_TRANS data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            audioRingTask *buffer_array0;
            audioRingTask data_buffer_array0;
            buffer_array0 = &data_buffer_array0;

            audioRingTask *buffer_array1;
            audioRingTask data_buffer_array1;
            buffer_array1 = &data_buffer_array1;
#endif
            //void MApi_AUDIO_Ring_DataTransfer(audioRingTask* auRingTask, audioRingTask* auRingTask_1)
            void* ptmp_auRingTask;
            void* ptmp_auRingTask_1;
            pAudioRingTaskTransSrc = (PAUDIO_ST_RING_DATA_TRANS)pArgs;
            pAudioRingTaskTransDes = (PAUDIO_ST_RING_DATA_TRANS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_RING_DATA_TRANS));

            ptmp_auRingTask = pAudioRingTaskTransDes->pauRingTask;
            ptmp_auRingTask_1 = pAudioRingTaskTransDes->pauRingTask_1;
            pAudioRingTaskTransDes->pauRingTask = (audioRingTask*)buffer_array0;
            pAudioRingTaskTransDes->pauRingTask_1 = (audioRingTask*)buffer_array1;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp_auRingTask, sizeof(audioRingTask));
            CPY_FROM_USER((void*)buffer_array1, (void*)ptmp_auRingTask_1, sizeof(audioRingTask));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)ptmp_auRingTask, (void*)buffer_array0, sizeof(audioRingTask));
            CPY_to_USER((void*)ptmp_auRingTask_1, (void*)buffer_array1, sizeof(audioRingTask));
        }
            break;

        case MApi_CMD_AUDIO_MM2_CheckAesInfo:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_MM2_CHECKAESINFO *buffer_arg;
            AUDIO_ST_MM2_CHECKAESINFO data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            AES_INFO *buffer_array0;
            AES_INFO data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            //MS_BOOL MApi_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info )
            // Status.bStatus = FALSE; Status.dec_id = dec_id; Status.paes_info = aes_info;
            void* ptmp_paes_info;
            pMM2_CheckAESInfoSrc = (PAUDIO_ST_MM2_CHECKAESINFO)pArgs;
            pMM2_CheckAESInfoDes = (PAUDIO_ST_MM2_CHECKAESINFO)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_MM2_CHECKAESINFO));

            ptmp_paes_info = pMM2_CheckAESInfoDes->paes_info;
            pMM2_CheckAESInfoDes->paes_info = (AES_INFO*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp_paes_info, sizeof(AES_INFO));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)ptmp_paes_info, (void*)buffer_array0, sizeof(AES_INFO));
        }
            break;

        case MApi_CMD_AUDIO_MM2_InputAesFinished:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_MM2_INPUTAES_FINISH *buffer_arg;
            AUDIO_ST_MM2_INPUTAES_FINISH data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //MS_BOOL MApi_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts )
            // Status.bStatus = Return; Status.dec_id = dec_id; Status.es_size = es_size; Status.ptsExist = ptsExist; Status.pts = pts;
            pMM2_InputAES_FinishSrc = (PAUDIO_ST_MM2_INPUTAES_FINISH)pArgs;
            pMM2_InputAES_FinishDes = (PAUDIO_ST_MM2_INPUTAES_FINISH)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_MM2_INPUTAES_FINISH));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            //bStatus is the first member of structure, so no need to calculate offset
            CPY_to_USER((void*)pMM2_InputAES_FinishSrc, (void*)(&(pMM2_InputAES_FinishDes->bStatus)), sizeof(MS_BOOL));
        }
            break;

        case MApi_CMD_AUDIO_SetAudioParam2:
        {
#if 0 //original
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[4];
#endif

            //MS_BOOL MApi_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32 Param )
            //u32Param[0] = return u32param[1] = DecId; u32param[2] = paramType; u32param[3] = Param;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_GetAudioInfo2:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_GETAUDIOINFO2 *buffer_arg;
            AUDIO_ST_GETAUDIOINFO2 data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            char buffer_array0[128];
#endif
            void* ptmp=NULL;
            MS_U32 u32size = sizeof(MS_U32);

            pGetAudioInfo2Src = (PAUDIO_ST_GETAUDIOINFO2)pArgs;
            pGetAudioInfo2Des = (PAUDIO_ST_GETAUDIOINFO2)buffer_arg;
            CPY_FROM_USER((void *)pGetAudioInfo2Des, (void *)pGetAudioInfo2Src, sizeof(AUDIO_ST_GETAUDIOINFO2));
            ptmp = pGetAudioInfo2Des->pInfo;   // bakup user space ptr
            pGetAudioInfo2Des->pInfo = (void*)buffer_array0;

            if(pGetAudioInfo2Des->infoType == Audio_infoType_esBuf_req)
            {
                u32size = sizeof(Audio_MM_Data_Request);
            }
            else if(pGetAudioInfo2Des->infoType == Audio_infoType_UNI_PCM1_Info)
            {
                u32size = sizeof(Audio_MM_PCM_Info);
            }
            else if(pGetAudioInfo2Des->infoType == Audio_infoType_hashkey)
            {
                u32size = sizeof(Audio_Hashkey_Info);
            }
            else if(pGetAudioInfo2Des->infoType == Audio_R2_SHM_INFO_CURR_PTS)
            {
                u32size = sizeof(MS_U64);
            }
            CPY_FROM_USER(pGetAudioInfo2Des->pInfo, ptmp, u32size);
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pGetAudioInfo2Des);
            CPY_to_USER(ptmp, pGetAudioInfo2Des->pInfo, u32size);
            CPY_to_USER((void*)pGetAudioInfo2Src, (void*)&pGetAudioInfo2Des->bStatus, sizeof(MS_BOOL));
        }
            break;

        case MApi_CMD_AUDIO_GetDDRInfo:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 buffer_arg[3];
#endif
            //MS_U32 MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo)
            //u32Param[0] = return u32param[1] = DecId; u32param[2] = DDRInfo;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 3);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pu32Src, (void*)(&(pu32Des[0])), sizeof(MS_U32));
        }
            break;
#if 0
        case MApi_CMD_AUDIO_SetBalance:
            //void MApi_AUDIO_SetBalance_L( MS_U8 u8Balance_L )
            //Status.bLeft = TRUE/FALSE; Status.u32BalanceValue = u8Balance_L;
            pSetBalanceSrc = (PAUDIO_ST_SETBALABCE)pArgs;
            pSetBalanceDes = (PAUDIO_ST_SETBALABCE)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SETBALABCE));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            break;
#endif
        case MApi_CMD_AUDIO_RWReg:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_RWReg *buffer_arg;
            AUDIO_ST_RWReg data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            pRWRegSrc = (PAUDIO_ST_RWReg)pArgs;
            pRWRegDes = (PAUDIO_ST_RWReg)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, (void*)pArgs, sizeof(AUDIO_ST_RWReg));

            if(pRWRegDes->bRead != TRUE)
            {
                MS_U32 u32addr = (MS_U32)pRWRegSrc + _addr_offset(pRWRegDes, &(pRWRegDes->u16Val));
                CPY_FROM_USER((void*)&pRWRegDes->u16Val, (void*)u32addr, sizeof(MS_U16));
            }

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pRWRegDes);

            if(pRWRegDes->bRead == TRUE)
            {
                MS_U32 u32addr2 = (MS_U32)pRWRegSrc + _addr_offset(pRWRegDes, &(pRWRegDes->u16Ret));
                CPY_to_USER((void*)u32addr2, (void*)&(pRWRegDes->u16Ret), sizeof(MS_U16));
            }
        }
            break;

#if 0
        case MApi_CMD_AUDIO_SetCutBooost:
            //void MApi_AUDIO_Set_Cut_Boost(MS_U8 scale)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            break;

        case MApi_CMD_AUDIO_ES_STOP_THRESHOLD:
            //void MApi_AUDIO_ES_Stop_Threshold(MS_U32 threshold)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            break;

        case MApi_CMD_AUDIO_SetSPDIF_HW_Mute:
            //void MApi_AUDIO_SetSPDIFHWMute(MS_U8 mute)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            break;

        case MApi_CMD_AUDIO_SetARC_HW_Mute:
            //void MApi_AUDIO_Set_ARC_HWMute(MS_U8 mute)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            break;
#endif
        case MApi_CMD_AUDIO_SetARC_HWEN:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            MS_U32 *buffer_arg;
            MS_U32 data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_ARC_HWEN(MS_BOOL arc_en)
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
        }
            break;

        case MApi_CMD_AUDIO_SetARC_HWEN_By_Port:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_ARC_HWEN_BY_PORT *buffer_arg;
            AUDIO_ST_ARC_HWEN_BY_PORT data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_ARC_HWEN_BY_PORT(AUDIO_HDMI_INDEX_T hdmiIndex, MS_BOOL bOnOff)
            pARCHwenByPortSrc = (PAUDIO_ST_ARC_HWEN_BY_PORT)pArgs;
            pARCHwenByPortDes = (PAUDIO_ST_ARC_HWEN_BY_PORT)buffer_arg;
            CPY_FROM_USER((void *)pARCHwenByPortDes, (void*)pARCHwenByPortSrc, sizeof(AUDIO_ST_ARC_HWEN_BY_PORT));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pARCHwenByPortDes);
        }
            break;

        case MApi_CMD_AUDIO_LoadAudio_Clip:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_LOAD_AUDIO_CLIP *buffer_arg;
            AUDIO_ST_LOAD_AUDIO_CLIP data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pLoadAudioClipSrc = (PAUDIO_ST_LOAD_AUDIO_CLIP)pArgs;
            pLoadAudioClipDes = (PAUDIO_ST_LOAD_AUDIO_CLIP)buffer_arg;
            CPY_FROM_USER((void *)pLoadAudioClipDes, (void*)pLoadAudioClipSrc, sizeof(AUDIO_ST_LOAD_AUDIO_CLIP));
            //CPY_FROM_USER((void *)&pLoadAudioClipDes->bufSize, (void*)&pLoadAudioClipSrc->bufSize, sizeof(MS_S32));
            //CPY_FROM_USER((void *)&pLoadAudioClipDes->dsp_sel, (void*)&pLoadAudioClipSrc->dsp_sel, sizeof(MS_S32));
            if (pLoadAudioClipDes->bufSize > 0)
            {
                void *clipBufPtr = NULL;

                clipBufPtr = kzalloc(pLoadAudioClipDes->bufSize, GFP_KERNEL);
                if (clipBufPtr != NULL)
                {
                    void* ptmp = pLoadAudioClipDes->clipBufPtr;
                    pLoadAudioClipDes->clipBufPtr = clipBufPtr;
                    CPY_FROM_USER((void *)pLoadAudioClipDes->clipBufPtr, (void *)ptmp, pLoadAudioClipDes->bufSize);
                    u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pLoadAudioClipDes);
                    kfree(clipBufPtr);
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                }
            }
            else
            {
                u32Ret = UTOPIA_STATUS_ERR_INV;
            }
        }
            break;

        case MApi_CMD_AUDIO_Clip_OP:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_CLIP *buffer_arg;
            AUDIO_ST_CLIP data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_PlayAudioClip(MS_U32 repeatTimes)
            //Status.eType = CLIP_PLAY_AUDIO ~ CLIP_PROCESSOR; Status.u32Input = inputu32; Status.u32Ret = retunr;
            AUDIO_CLIP_OP tmp_eType;
            pClipSrc = (PAUDIO_ST_CLIP)pArgs;
            pClipDes = (PAUDIO_ST_CLIP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_CLIP));

            tmp_eType = pClipDes->eType;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if((tmp_eType == CLIP_GET_POSITION) || (tmp_eType == CLIP_PLAY_DONE))
            {
                MS_U32 u32addr = (MS_U32)pClipSrc + _addr_offset(pClipDes, &(pClipDes->u32Ret));
                CPY_to_USER((void*)u32addr, (void*)(&(pClipDes->u32Ret)), sizeof(MS_U32));
            }
        }
            break;

        case MApi_CMD_AUDIO_SIF_AnalogMode_OP:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SIF_ANALOGMODE_OP *buffer_arg;
            AUDIO_ST_SIF_ANALOGMODE_OP data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //void MApi_AUDIO_SIF_GetCurAnalogMode(MS_U8* pAlgMode)/void MApi_AUDIO_SIF_SetUserAnalogMode(MS_U16 AlgMode)
            //Status.eType = SIF_GET_CUR_ANALOG_MODE/SIF_SET_USER_ANALOG_MODE; Status.u16SetVal = 0; Status.u8GetVal = 0;
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_SIF_ANALOGMODE_OP, NULL,buffer_arg,sizeof(buffer_arg));
            AUDIO_SIF_ANA_MODE_OP tmp_eType;
            pSIFAnalogMode_Src = (PAUDIO_ST_SIF_ANALOGMODE_OP)pArgs;
            pSIFAnalogMode_Des = (PAUDIO_ST_SIF_ANALOGMODE_OP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SIF_ANALOGMODE_OP));

            tmp_eType = pSIFAnalogMode_Des->eType;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(tmp_eType == SIF_GET_CUR_ANALOG_MODE)
            {
                MS_U32 u32addr = (MS_U32)pSIFAnalogMode_Src + _addr_offset(pSIFAnalogMode_Des, &(pSIFAnalogMode_Des->u8GetVal));
                CPY_to_USER((void*)u32addr, (void*)(&(pSIFAnalogMode_Des->u8GetVal)), sizeof(MS_U8));
            }
        }
            break;

        case MApi_CMD_AUDIO_SIF_SetMode_Setup_OP:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SIF_ANALOGMODE_OP *buffer_arg;
            AUDIO_ST_SIF_ANALOGMODE_OP data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //MS_BOOL MApi_AUDIO_SIF_SetModeSetup(MS_U8 SystemType)
            //void MApi_AUDIO_SIF_SetHDEVMode(MS_U8 bOnOff)
            //MS_BOOL MApi_AUDIO_SIF_SetBandSetup(MS_U8 SifBand)
            //Status.eType = SIF_SETBAND_SETUP/ SIF_SETHDEV_MODE/ SIF_SETBAND_SETUP;Status.u8Input = SifBand;Status.bRet = FALSE;
            AUDIO_SIF_SETMODE_OP tmp_eType;
            pSIF_SetMode_SetupSrc = (PAUDIO_ST_SIF_SETMODE_SETUP_OP)pArgs;
            pSIF_SetMode_SetupDes = (PAUDIO_ST_SIF_SETMODE_SETUP_OP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SIF_ANALOGMODE_OP));

            tmp_eType = pSIF_SetMode_SetupDes->eType;
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if((tmp_eType == SIF_SETMODE_SETUP) || (tmp_eType == SIF_SETBAND_SETUP))
            {
                MS_U32 u32addr = (MS_U32)pSIF_SetMode_SetupSrc + _addr_offset(pSIF_SetMode_SetupDes, &(pSIF_SetMode_SetupDes->bRet));
                CPY_to_USER((void*)u32addr, (void*)(&(pSIF_SetMode_SetupDes->bRet)), sizeof(MS_BOOL));
            }
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_SIF_SETMODE_SETUP_OP, spt_AUDIO_ST_SIF_SETMODE_SETUP_OP,buffer_arg,sizeof(buffer_arg));
        }
            break;

        case MApi_CMD_AUDIO_SIF_GetBand_OP:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SIF_GETBAND_OP *buffer_arg;
            AUDIO_ST_SIF_GETBAND_OP data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 *buffer_array0;
            MS_U32 data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif
            //MS_BOOL MApi_AUDIO_SIF_GetBandDetect(MS_U8 soundSystem, MS_U32 *pbandStrength)
            //MS_BOOL MApi_AUDIO_SIF_GetBandDetect_Amp(MS_U8 soundSystem, MS_U32 *pbandStrength)
            //Status.eType = SIF_GET_BAND_DETECT/SIF_GET_BAND_DETECT_AMP;Status.SoundSystem = soundSystem;Status.pbandStrength = pbandStrength;Status.bRet = FALSE;
            void* ptmp;
            pSIFGetBandSrc = (PAUDIO_ST_SIF_GETBAND_OP)pArgs;
            pSIFGetBandDes = (PAUDIO_ST_SIF_GETBAND_OP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SIF_GETBAND_OP));

            ptmp = pSIFGetBandDes->pbandStrength;
            pSIFGetBandDes->pbandStrength = (MS_U32*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U32));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            MS_U32 u32addr = (MS_U32)pSIFGetBandSrc + _addr_offset(pSIFGetBandDes, &(pSIFGetBandDes->bRet));
            CPY_to_USER((void*)u32addr, (void*)(&(pSIFGetBandDes->bRet)), sizeof(MS_BOOL));
            CPY_to_USER((void*)ptmp, (void*)buffer_array0, sizeof(MS_U32));
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_SIF_GETBAND_OP, spt_AUDIO_ST_SIF_GETBAND_OP,buffer_arg,sizeof(buffer_arg));
        }
            break;

        case MApi_CMD_AUDIO_SIF_CheckAvailableSystem:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM *buffer_arg;
            AUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U8 *buffer_array0;
            MS_U8 data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif
            //MS_BOOL MApi_AUDIO_SIF_CheckAvailableSystem(MS_U8 standard, MS_U8 *pexist)
            //Status.standard = standard; Status.pexist = pexist; Status.bRet = FALSE;
            void* ptmp;
            pCheckAvaliableSystemSrc = (PAUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM)pArgs;
            pCheckAvaliableSystemDes = (PAUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM));

            ptmp = pCheckAvaliableSystemDes->pexist;
            pCheckAvaliableSystemDes->pexist = (MS_U8*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U8));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            MS_U32 u32addr = (MS_U32)pCheckAvaliableSystemSrc + _addr_offset(pCheckAvaliableSystemDes, &(pCheckAvaliableSystemDes->bRet));
            CPY_to_USER((void*)u32addr, (void*)(&(pCheckAvaliableSystemDes->bRet)), sizeof(MS_BOOL));
            CPY_to_USER((void*)ptmp, (void*)buffer_array0, sizeof(MS_U8));
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM, spt_AUDIO_ST_SIF_CHECK_AVAILABLE_SYSTEM,buffer_arg,sizeof(buffer_arg));
        }
            break;

        case MApi_CMD_AUDIO_SIF_CheckA2DK:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SIF_CHECK_A2DK *buffer_arg;
            AUDIO_ST_SIF_CHECK_A2DK data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U8 *buffer_array0;
            MS_U8 data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif
            //MS_BOOL MApi_AUDIO_SIF_CheckA2DK(MS_U8 SystemType, MS_U8 *pexist)
            //Status.SystemType= SystemType;Status.pexist = pexist, Status.bRet = FALSE;
            void* ptmp;
            pSIFCheckA2DKSrc = (PAUDIO_ST_SIF_CHECK_A2DK)pArgs;
            pSIFCheckA2DKDes = (PAUDIO_ST_SIF_CHECK_A2DK)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SIF_CHECK_A2DK));

            ptmp = pSIFCheckA2DKDes->pexist;
            pSIFCheckA2DKDes->pexist = (MS_U8*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U8));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            MS_U32 u32addr = (MS_U32)pSIFCheckA2DKSrc + _addr_offset(pSIFCheckA2DKDes, &(pSIFCheckA2DKDes->bRet));
            CPY_to_USER((void*)u32addr, (void*)(&(pSIFCheckA2DKDes->bRet)), sizeof(MS_BOOL));
            CPY_to_USER((void*)ptmp, (void*)buffer_array0, sizeof(MS_U8));
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_SIF_CHECK_A2DK, spt_AUDIO_ST_SIF_CHECK_A2DK,buffer_arg,sizeof(buffer_arg));
        }
            break;

        case MApi_CMD_AUDIO_SIF_GetSoundStandard:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SIF_GETSOUNDSTANDARD *buffer_arg;
            AUDIO_ST_SIF_GETSOUNDSTANDARD data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U8 *buffer_array0;
            MS_U8 data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif
            //MS_BOOL MApi_AUDIO_SIF_GetSoundStandard(MS_U8 *B_SifStandard)
            //Status.B_SifStandard = B_SifStandard;Status.bRet = FALSE;
            void* ptmp;
            pSIFGetStandardSrc = (PAUDIO_ST_SIF_GETSOUNDSTANDARD)pArgs;
            pSIFGetStandardDes = (PAUDIO_ST_SIF_GETSOUNDSTANDARD)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SIF_GETSOUNDSTANDARD));

            ptmp = pSIFGetStandardDes->B_SifStandard;
            pSIFGetStandardDes->B_SifStandard = (MS_U8*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U8));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)pSIFGetStandardSrc, (void*)(&(pSIFGetStandardDes->bRet)), sizeof(MS_BOOL));
            CPY_to_USER((void*)ptmp, (void*)buffer_array0, sizeof(MS_U8));
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_SIF_GETSOUNDSTANDARD, spt_AUDIO_ST_SIF_GETSOUNDSTANDARD,buffer_arg,sizeof(buffer_arg));
        }
            break;

        case MApi_CMD_AUDIO_SIF_RedoSetStandard:
        {
//        case MApi_CMD_AUDIO_SIF_ADC_Reset:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        }
            break;

        case MApi_CMD_AUDIO_GetBtscA2StereoLevel:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_GETBTSC_A2STEREO_LEVEL *buffer_arg;
            AUDIO_ST_GETBTSC_A2STEREO_LEVEL data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U16 *buffer_array0;
            MS_U16 data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif
            //MS_BOOL MApi_AUDIO_GetBtscA2StereoLevel(MS_U16 *pLevel)
            //Status.pLevel = pLevel; Status.bRet = FALSE;
            void* ptmp;
            pGetBTSCA2StereoLevelSrc = (PAUDIO_ST_GETBTSC_A2STEREO_LEVEL)pArgs;
            pGetBTSCA2StereoLevelDes = (PAUDIO_ST_GETBTSC_A2STEREO_LEVEL)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_GETBTSC_A2STEREO_LEVEL));

            ptmp = pGetBTSCA2StereoLevelDes->pLevel;
            pGetBTSCA2StereoLevelDes->pLevel = (MS_U16*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U16));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            MS_U32 u32addr = (MS_U32)pGetBTSCA2StereoLevelSrc + _addr_offset(pGetBTSCA2StereoLevelDes, &(pGetBTSCA2StereoLevelDes->bRet));
            CPY_to_USER((void*)u32addr, (void*)(&(pGetBTSCA2StereoLevelDes->bRet)), sizeof(MS_BOOL));
            CPY_to_USER((void*)ptmp, (void*)buffer_array0, sizeof(MS_U16));
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_GETBTSC_A2STEREO_LEVEL, spt_AUDIO_ST_GETBTSC_A2STEREO_LEVEL,buffer_arg,sizeof(buffer_arg));
        }
            break;

        case MApi_CMD_AUDIO_SetBtscA2ThresholdLevel:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SETBTSCA2THRESHOLDLEVEL *buffer_arg;
            AUDIO_ST_SETBTSCA2THRESHOLDLEVEL data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U16 buffer_array0[40];
#endif
            //MS_BOOL MApi_AUDIO_SetBtscA2ThresholdLevel(MS_U16 thresholdLevel, MS_U16 *pRF2NSR_map)
            //Status.thresholdLevel = thresholdLevel;Status.pRF2NSR_map = pRF2NSR_map;Status.bRet = FALSE;
            void* ptmp;
            pSetBTSCA2ThresholdLevelSrc = (PAUDIO_ST_SETBTSCA2THRESHOLDLEVEL)pArgs;
            pSetBTSCA2ThresholdLevelDes = (PAUDIO_ST_SETBTSCA2THRESHOLDLEVEL)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SETBTSCA2THRESHOLDLEVEL));

            ptmp = pSetBTSCA2ThresholdLevelDes->pRF2NSR_map;
            pSetBTSCA2ThresholdLevelDes->pRF2NSR_map = (MS_U16*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U16)*40 ); //pRF2NSR_map size is 40

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            MS_U32 u32addr = (MS_U32)pSetBTSCA2ThresholdLevelSrc + _addr_offset(pSetBTSCA2ThresholdLevelDes, &(pSetBTSCA2ThresholdLevelDes->bRet));
            CPY_to_USER((void*)u32addr, (void*)(&(pSetBTSCA2ThresholdLevelDes->bRet)), sizeof(MS_BOOL));
            //CPY_to_USER((void*)(pSetBTSCA2ThresholdLevelSrc->pRF2NSR_map)), (void*)buffer_array0, sizeof(MS_U16));
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_SETBTSCA2THRESHOLDLEVEL, spt_AUDIO_ST_SETBTSCA2THRESHOLDLEVEL,buffer_arg,sizeof(buffer_arg));
        }
            break;

        case MApi_CMD_AUDIO_CheckNicamDigital:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_CHECKNICAMDIGITAL *buffer_arg;
            AUDIO_ST_CHECKNICAMDIGITAL data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U8 *buffer_array0;
            MS_U8 data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif
            //void MApi_AUDIO_CheckNicamDigital(MS_U8 *pisNicamDetect)
            //Status.pisNicamDetect = pisNicamDetect;
            void* ptmp;
            pCheckNICAMDigitalSrc = (PAUDIO_ST_CHECKNICAMDIGITAL)pArgs;
            pCheckNICAMDigitalDes = (PAUDIO_ST_CHECKNICAMDIGITAL)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_CHECKNICAMDIGITAL));

            ptmp = pCheckNICAMDigitalDes->pisNicamDetect;
            pCheckNICAMDigitalDes->pisNicamDetect = (MS_U8*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)ptmp, sizeof(MS_U8));

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)ptmp, (void*)buffer_array0, sizeof(MS_U8));
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_CHECKNICAMDIGITAL, NULL ,buffer_arg,sizeof(buffer_arg));
        }
            break;

        case MApi_CMD_AUDIO_SetThreshold_Type:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_SETTHRESHOLDTYPE *buffer_arg;
            AUDIO_ST_SETTHRESHOLDTYPE data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            //void MApi_AUDIO_SetThresholdType(THR_TBL_TYPE_C code *ThrTbl, MS_U8 num,  MS_U8 standardtype)
            MS_U32 u32CopySize = 0;
            void* ptmp;
            pSetThresholdTypeSrc = (PAUDIO_ST_SETTHRESHOLDTYPE)pArgs;
            pSetThresholdTypeDes = (PAUDIO_ST_SETTHRESHOLDTYPE)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SETTHRESHOLDTYPE));
            ptmp = (void *)pSetThresholdTypeDes->ThrTbl;
            u32CopySize = (MS_U32)(pSetThresholdTypeDes->num) * sizeof(THR_TBL_TYPE_C code);

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            void *buffer_array0;
            MS_BOOL bMemAlloc = FALSE;
            if (u32CopySize > 0)
            {
                buffer_array0 = kzalloc(u32CopySize, GFP_KERNEL);
                if (buffer_array0 != NULL)
                {
                    bMemAlloc = TRUE;
                }
                else
                {
                    printk("[Audio] No memory for MApi_CMD_AUDIO_SetThreshold_Type\n");
                    break;
                }
            }
            else
            {
                printk("[Audio] u32CopySize is 0 for MApi_CMD_AUDIO_SetThreshold_Type\n");
                break;
            }
#endif

            pSetThresholdTypeDes->ThrTbl = (THR_TBL_TYPE_C code*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, ptmp, u32CopySize );

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            if(bMemAlloc == TRUE)
            {
                kfree(buffer_array0);
            }
#endif

            break;
        }

        case MApi_CMD_AUDIO_DecodeFramCnt:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_DECODE_FRAME_CNT *buffer_arg;
            AUDIO_ST_DECODE_FRAME_CNT data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            //MS_U32 MApi_AUDIO_Decoded_Frame_Cnt(MS_BOOL bDspType)
            //Status.bDspType = bDspType;Status.u32Ret = 0;
            MS_U32 u32addr;
            pDecodeFrameCntSrc = (PAUDIO_ST_DECODE_FRAME_CNT)pArgs;
            pDecodeFrameCntDes = (PAUDIO_ST_DECODE_FRAME_CNT)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DECODE_FRAME_CNT));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            u32addr = (MS_U32)pDecodeFrameCntSrc + _addr_offset(pDecodeFrameCntDes, &(pDecodeFrameCntDes->u32Ret));
            CPY_to_USER((void*)u32addr, (void*)(&(pDecodeFrameCntDes->u32Ret)), sizeof(MS_U32));
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_DECODE_FRAME_CNT, spt_AUDIO_ST_DECODE_FRAME_CNT ,buffer_arg,sizeof(buffer_arg));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Init:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_INIT *buffer_arg;
            AUDIO_ST_PCMMIXER_INIT data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmMixerInitSrc = (PAUDIO_ST_PCMMIXER_INIT)pArgs;
            pPcmMixerInitDes = (PAUDIO_ST_PCMMIXER_INIT)buffer_arg;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerInitDes);
            CPY_to_USER((void *)pPcmMixerInitSrc, (void*)&pPcmMixerInitDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_DeInit:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_DEINIT *buffer_arg;
            AUDIO_ST_PCMMIXER_DEINIT data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmMixerDeInitSrc = (PAUDIO_ST_PCMMIXER_DEINIT)pArgs;
            pPcmMixerDeInitDes = (PAUDIO_ST_PCMMIXER_DEINIT)buffer_arg;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerDeInitDes);
            CPY_to_USER((void *)pPcmMixerDeInitSrc, (void*)&pPcmMixerDeInitDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Open:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_OPEN *buffer_arg;
            AUDIO_ST_PCMMIXER_OPEN data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmMixerOpenSrc = (PAUDIO_ST_PCMMIXER_OPEN)pArgs;
            pPcmMixerOpenDes = (PAUDIO_ST_PCMMIXER_OPEN)buffer_arg;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerOpenDes);
            CPY_to_USER((void *)pPcmMixerOpenSrc, (void*)&pPcmMixerOpenDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Close:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_CLOSE *buffer_arg;
            AUDIO_ST_PCMMIXER_CLOSE data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmMixerCloseSrc = (PAUDIO_ST_PCMMIXER_CLOSE)pArgs;
            pPcmMixerCloseDes = (PAUDIO_ST_PCMMIXER_CLOSE)buffer_arg;
            CPY_FROM_USER((void *)&pPcmMixerCloseDes->s32PcmMixingId, (void*)pPcmMixerCloseSrc, sizeof(MS_S32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerCloseDes);
            MS_U32 u32addr = (MS_U32)pPcmMixerCloseSrc + _addr_offset(pPcmMixerCloseDes, &(pPcmMixerCloseDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmMixerCloseDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Flush:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_FLUSH *buffer_arg;
            AUDIO_ST_PCMMIXER_FLUSH data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmMixerFlushSrc = (PAUDIO_ST_PCMMIXER_FLUSH)pArgs;
            pPcmMixerFlushDes = (PAUDIO_ST_PCMMIXER_FLUSH)buffer_arg;
            CPY_FROM_USER((void *)&pPcmMixerFlushDes->s32PcmMixingId, (void*)pPcmMixerFlushSrc, sizeof(MS_S32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerFlushDes);
            MS_U32 u32addr = (MS_U32)pPcmMixerFlushSrc + _addr_offset(pPcmMixerFlushDes, &(pPcmMixerFlushDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmMixerFlushDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Start:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_START *buffer_arg;
            AUDIO_ST_PCMMIXER_START data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmMixerStartSrc = (PAUDIO_ST_PCMMIXER_START)pArgs;
            pPcmMixerStartDes = (PAUDIO_ST_PCMMIXER_START)buffer_arg;
            CPY_FROM_USER((void *)&pPcmMixerStartDes->s32PcmMixingId, (void*)pPcmMixerStartSrc, sizeof(MS_S32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerStartDes);
            MS_U32 u32addr = (MS_U32)pPcmMixerStartSrc + _addr_offset(pPcmMixerStartDes, &(pPcmMixerStartDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmMixerStartDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Stop:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_STOP *buffer_arg;
            AUDIO_ST_PCMMIXER_STOP data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmMixerStopSrc = (PAUDIO_ST_PCMMIXER_STOP)pArgs;
            pPcmMixerStopDes = (PAUDIO_ST_PCMMIXER_STOP)buffer_arg;
            CPY_FROM_USER((void *)&pPcmMixerStopDes->s32PcmMixingId, (void*)pPcmMixerStopSrc, sizeof(MS_S32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerStopDes);
            MS_U32 u32addr = (MS_U32)pPcmMixerStopSrc + _addr_offset(pPcmMixerStopDes, &(pPcmMixerStopDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmMixerStopDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Set:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_SET *buffer_arg;
            AUDIO_ST_PCMMIXER_SET data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 *buffer_array0;
            MS_U32 data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif
            void* ptmp;
            MS_U32 u32addr;
            pPcmMixerSetSrc = (PAUDIO_ST_PCMMIXER_SET)pArgs;
            pPcmMixerSetDes = (PAUDIO_ST_PCMMIXER_SET)buffer_arg;

            CPY_FROM_USER((void *)buffer_arg, pArgs, sizeof(AUDIO_ST_PCMMIXER_SET));
            ptmp = pPcmMixerSetDes->pData;  // bakup userspace ptr
            pPcmMixerSetDes->pData = (void *)buffer_array0;

            CPY_FROM_USER(pPcmMixerSetDes->pData, ptmp, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerSetDes);

            // calculate addr of pPcmMixerSetSrc->s32Ret
            u32addr = (unsigned int)pPcmMixerSetSrc + _addr_offset(pPcmMixerSetDes, &pPcmMixerSetDes->s32Ret);
            CPY_to_USER((void *)u32addr, (void*)&pPcmMixerSetDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Get:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_GET *buffer_arg;
            AUDIO_ST_PCMMIXER_GET data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            MS_U32 buffer_array0[16];
#endif
            void* ptmp;
            pPcmMixerGetSrc = (PAUDIO_ST_PCMMIXER_GET)pArgs;
            pPcmMixerGetDes = (PAUDIO_ST_PCMMIXER_GET)buffer_arg;
            CPY_FROM_USER((void *)pPcmMixerGetDes, (void*)pPcmMixerGetSrc, sizeof(AUDIO_ST_PCMMIXER_GET));
            ptmp = pPcmMixerGetDes->pData;
            pPcmMixerGetDes->pData = (void *)buffer_array0;
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerGetDes);
            MS_U32 u32addr = (MS_U32)pPcmMixerGetSrc + _addr_offset(pPcmMixerGetDes, &(pPcmMixerGetDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmMixerGetDes->s32Ret, sizeof(MS_S32));
            CPY_to_USER(ptmp, pPcmMixerGetDes->pData, sizeof(MS_U32));
        }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Write:
	    {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_WRITE *buffer_arg;
            AUDIO_ST_PCMMIXER_WRITE data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            void* pBuf;
            pPcmMixerWriteSrc = (PAUDIO_ST_PCMMIXER_WRITE)pArgs;
            pPcmMixerWriteDes = (PAUDIO_ST_PCMMIXER_WRITE)buffer_arg;
            CPY_FROM_USER((void *)pPcmMixerWriteDes, (void*)pPcmMixerWriteSrc, sizeof(AUDIO_ST_PCMMIXER_WRITE));
            if (pPcmMixerWriteDes->u32Size > 0)
            {
                void* ptmp;
                ptmp = pPcmMixerWriteDes->pBuf;
                pBuf = kzalloc(pPcmMixerWriteDes->u32Size, GFP_KERNEL);
                pPcmMixerWriteDes->pBuf = pBuf;
                if (pPcmMixerWriteDes->pBuf != NULL)
                {
                    CPY_FROM_USER(pPcmMixerWriteDes->pBuf, (void*)ptmp, pPcmMixerWriteDes->u32Size);
                    u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerWriteDes);

                    MS_U32 u32addr = (MS_U32)pPcmMixerWriteSrc + _addr_offset(pPcmMixerWriteDes, &(pPcmMixerWriteDes->bRet));
                    CPY_to_USER((void *)u32addr, (void*)&pPcmMixerWriteDes->bRet, sizeof(MS_BOOL));
                    kfree(pBuf);
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                }
            }
            else
            {
                u32Ret = UTOPIA_STATUS_ERR_INV;
            }
	    }
            break;

        case MApi_CMD_AUDIO_PcmMixer_Func:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCMMIXER_FUNC *buffer_arg;
            AUDIO_ST_PCMMIXER_FUNC data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmMixerFuncSrc = (PAUDIO_ST_PCMMIXER_FUNC)pArgs;
            pPcmMixerFuncDes = (PAUDIO_ST_PCMMIXER_FUNC)buffer_arg;
            CPY_FROM_USER((void *)&pPcmMixerFuncDes->u32Arg, (void*)&pPcmMixerFuncSrc->u32Arg, sizeof(MS_U32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmMixerFuncDes);
            CPY_to_USER((void *)&pPcmMixerFuncSrc->s32Ret, (void*)&pPcmMixerFuncDes->s32Ret, sizeof(MS_S32));
        }
            break;

#if 0
        case MApi_CMD_AUDIO_GetHDMIINChannelStatus:
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[1])), (void*)(&(pu32Des[1])), sizeof(MS_U16));
            break;

        case MApi_CMD_AUDIO_GetLIBVer:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case MApi_CMD_AUDIO_GetDSPPTS:
            pu64Src = (MS_U64*)pArgs;
            pu64Des = (MS_U64*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U64) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu64Src[1])), (void*)(&(pu64Des[1])), sizeof(MS_U64));
            break;
#endif

        case MApi_CMD_AUDIO_PCM_Open:
        {

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCM_OPEN *buffer_arg;
            AUDIO_ST_PCM_OPEN data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            AUDIO_PCM_INFO_t *buffer_array0;
            AUDIO_PCM_INFO_t data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            void* ptmp;
            pPcmOpenSrc = (PAUDIO_ST_PCM_OPEN)pArgs;
            pPcmOpenDes = (PAUDIO_ST_PCM_OPEN)buffer_arg;
            CPY_FROM_USER((void*)pPcmOpenDes, (void*)pPcmOpenSrc, sizeof(AUDIO_ST_PCM_OPEN));

            ptmp = pPcmOpenDes->pData;
            pPcmOpenDes->pData = (void *)buffer_array0;
            CPY_FROM_USER(pPcmOpenDes->pData, (void*)ptmp, sizeof(AUDIO_PCM_INFO_t));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmOpenDes);
            MS_U32 u32addr = (MS_U32)pPcmOpenSrc + _addr_offset(pPcmOpenDes, &(pPcmOpenDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmOpenDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PCM_Close:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCM_CLOSE *buffer_arg;
            AUDIO_ST_PCM_CLOSE data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmCloseSrc = (PAUDIO_ST_PCM_CLOSE)pArgs;
            pPcmCloseDes = (PAUDIO_ST_PCM_CLOSE)buffer_arg;
            CPY_FROM_USER((void *)&pPcmCloseDes->s32DeviceId, (void*)pPcmCloseSrc, sizeof(MS_S32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmCloseDes);
            MS_U32 u32addr = (MS_U32)pPcmCloseSrc + _addr_offset(pPcmCloseDes, &(pPcmCloseDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmCloseDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PCM_Start:
        {
 #if 0  //origin
            pPcmStartSrc = (PAUDIO_ST_PCM_START)pArgs;
            pPcmStartDes = (PAUDIO_ST_PCM_START)buffer_arg;
            CPY_FROM_USER((void *)&pPcmStartDes->s32DeviceId, (void*)&pPcmStartSrc->s32DeviceId, sizeof(MS_S32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmStartDes);
            CPY_to_USER((void *)&pPcmStartSrc->s32Ret, (void*)&pPcmStartDes->s32Ret, sizeof(MS_S32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCM_START *buffer_arg;
            AUDIO_ST_PCM_START data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmStartSrc = (PAUDIO_ST_PCM_START)pArgs;
            pPcmStartDes = (PAUDIO_ST_PCM_START)buffer_arg;
            CPY_FROM_USER((void *)&pPcmStartDes->s32DeviceId, (void*)pPcmStartSrc, sizeof(MS_S32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmStartDes);
            MS_U32 u32addr = (MS_U32)pPcmStartSrc + _addr_offset(pPcmStartDes, &(pPcmStartDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmStartDes->s32Ret, sizeof(MS_S32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_PCM_Stop:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCM_STOP *buffer_arg;
            AUDIO_ST_PCM_STOP data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmStopSrc = (PAUDIO_ST_PCM_STOP)pArgs;
            pPcmStopDes = (PAUDIO_ST_PCM_STOP)buffer_arg;
            CPY_FROM_USER((void *)&pPcmStopDes->s32DeviceId, (void*)pPcmStopSrc, sizeof(MS_S32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmStopDes);
            MS_U32 u32addr = (MS_U32)pPcmStopSrc + _addr_offset(pPcmStopDes, &(pPcmStopDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmStopDes->s32Ret, sizeof(MS_S32));
        }
            break;

        case MApi_CMD_AUDIO_PCM_Set:
        {
#if 0 //origin
            pPcmSetSrc = (PAUDIO_ST_PCM_SET)pArgs;
            pPcmSetDes = (PAUDIO_ST_PCM_SET)buffer_arg;
            pPcmSetDes->pData = (void *)buffer_array0;
            CPY_FROM_USER((void *)&pPcmSetDes->s32DeviceId, (void*)&pPcmSetSrc->s32DeviceId, sizeof(MS_S32));
            CPY_FROM_USER((void *)&pPcmSetDes->u32Cmd, (void*)&pPcmSetSrc->u32Cmd, sizeof(MS_U32));
            CPY_FROM_USER(pPcmSetDes->pData, pPcmSetSrc->pData, sizeof(AUDIO_PCM_INFO_t));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmSetDes);
            CPY_to_USER((void *)&pPcmSetSrc->s32Ret, (void*)&pPcmSetDes->s32Ret, sizeof(MS_S32));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCM_SET *buffer_arg;
            AUDIO_ST_PCM_SET data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            AUDIO_PCM_INFO_t *buffer_array0;
            AUDIO_PCM_INFO_t data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            void* ptmp;
            pPcmSetSrc = (PAUDIO_ST_PCM_SET)pArgs;
            pPcmSetDes = (PAUDIO_ST_PCM_SET)buffer_arg;
            CPY_FROM_USER((void *)pPcmSetDes, (void*)pPcmSetSrc, sizeof(AUDIO_ST_PCM_SET));
            ptmp = pPcmSetDes->pData;
            pPcmSetDes->pData = (void *)buffer_array0;

            CPY_FROM_USER((void*)pPcmSetDes->pData, (void*)ptmp, sizeof(AUDIO_PCM_INFO_t));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmSetDes);
            MS_U32 u32addr = (MS_U32)pPcmSetSrc + _addr_offset(pPcmSetDes, &(pPcmSetDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmSetDes->s32Ret, sizeof(MS_S32));
#endif
        }
            break;

        case MApi_CMD_AUDIO_PCM_Get:
        {
#if 0   //origin
            pPcmGetSrc = (PAUDIO_ST_PCM_GET)pArgs;
            pPcmGetDes = (PAUDIO_ST_PCM_GET)buffer_arg;
            pPcmGetDes->pData = (void *)buffer_array0;
            CPY_FROM_USER((void *)&pPcmGetDes->s32DeviceId, (void*)&pPcmGetSrc->s32DeviceId, sizeof(MS_S32));
            CPY_FROM_USER((void *)&pPcmGetDes->u32Cmd, (void*)&pPcmGetSrc->u32Cmd, sizeof(MS_U32));
            CPY_FROM_USER(pPcmGetDes->pData, pPcmGetSrc->pData, sizeof(AUDIO_PCM_INFO_t));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmGetDes);
            CPY_to_USER((void *)&pPcmGetSrc->s32Ret, (void*)&pPcmGetDes->s32Ret, sizeof(MS_S32));
            CPY_to_USER(pPcmGetSrc->pData, pPcmGetDes->pData, sizeof(AUDIO_PCM_INFO_t));
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCM_GET *buffer_arg;
            AUDIO_ST_PCM_GET data_buffer_arg;
            buffer_arg = &data_buffer_arg;

            AUDIO_PCM_INFO_t *buffer_array0;
            AUDIO_PCM_INFO_t data_buffer_array0;
            buffer_array0 = &data_buffer_array0;
#endif

            void* ptmp;
            pPcmGetSrc = (PAUDIO_ST_PCM_GET)pArgs;
            pPcmGetDes = (PAUDIO_ST_PCM_GET)buffer_arg;
            CPY_FROM_USER((void *)pPcmGetDes, (void*)pPcmGetSrc, sizeof(AUDIO_ST_PCM_GET));
            ptmp = pPcmGetDes->pData;
            pPcmGetDes->pData = (void *)buffer_array0;
            CPY_FROM_USER((void *)buffer_array0, (void*)ptmp, sizeof(AUDIO_PCM_INFO_t));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmGetDes);
            MS_U32 u32addr = (MS_U32)pPcmGetSrc + _addr_offset(pPcmGetDes, &(pPcmGetDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmGetDes->s32Ret, sizeof(MS_S32));
            CPY_to_USER((void*)ptmp, (void*)pPcmGetDes->pData, sizeof(AUDIO_PCM_INFO_t));
#endif
        }
            break;

        case MApi_CMD_AUDIO_PCM_Read:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCM_READ *buffer_arg;
            AUDIO_ST_PCM_READ data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmReadSrc = (PAUDIO_ST_PCM_READ)pArgs;
            pPcmReadDes = (PAUDIO_ST_PCM_READ)buffer_arg;
            //CPY_FROM_USER((void *)&pPcmReadDes->s32DeviceId, (void*)&pPcmReadSrc->s32DeviceId, sizeof(MS_S32));
            //CPY_FROM_USER((void *)&pPcmReadDes->u32Size, (void*)&pPcmReadSrc->u32Size, sizeof(MS_U32));
            CPY_FROM_USER((void *)pPcmReadDes, (void*)pPcmReadSrc, sizeof(AUDIO_ST_PCM_READ));
            if (pPcmReadDes->u32Size > 0)
            {
                void* ptmp;
                ptmp = pPcmReadDes->pBuf;
                pPcmReadDes->pBuf = kzalloc(pPcmReadDes->u32Size, GFP_KERNEL);
                if (pPcmReadDes->pBuf != NULL)
                {
                    CPY_FROM_USER(pPcmReadDes->pBuf, (void*)ptmp, pPcmReadDes->u32Size);
                    u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmReadDes);
                    CPY_to_USER(ptmp, pPcmReadDes->pBuf, pPcmReadDes->u32Size);
                    MS_U32 u32addr = (MS_U32)pPcmReadSrc + _addr_offset(pPcmReadDes, &(pPcmReadDes->u32Ret));
                    CPY_to_USER((void *)u32addr, (void*)&pPcmReadDes->u32Ret, sizeof(MS_U32));
                    kfree(pPcmReadDes->pBuf);
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                }
            }
            else
            {
                u32Ret = UTOPIA_STATUS_ERR_INV;
            }
        }
            break;

        case MApi_CMD_AUDIO_PCM_Write:
        {
#if 0 //origin
            pPcmWriteSrc = (PAUDIO_ST_PCM_WRITE)pArgs;
            pPcmWriteDes = (PAUDIO_ST_PCM_WRITE)buffer_arg;
            CPY_FROM_USER((void *)&pPcmWriteDes->s32DeviceId, (void*)&pPcmWriteSrc->s32DeviceId, sizeof(MS_S32));
            CPY_FROM_USER((void *)&pPcmWriteDes->u32Size, (void*)&pPcmWriteSrc->u32Size, sizeof(MS_U32));
            if (pPcmWriteDes->u32Size > 0)
            {
                pPcmWriteDes->pBuf = kzalloc(pPcmWriteDes->u32Size, GFP_KERNEL);
                if (pPcmWriteDes->pBuf != NULL)
                {
                    CPY_FROM_USER(pPcmWriteDes->pBuf, pPcmWriteSrc->pBuf, pPcmWriteDes->u32Size);
                    u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmWriteDes);
                    CPY_to_USER((void *)&pPcmWriteSrc->u32Ret, (void*)&pPcmWriteDes->u32Ret, sizeof(MS_U32));
                    kfree(pPcmWriteDes->pBuf);
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                }
            }
            else
            {
                u32Ret = UTOPIA_STATUS_ERR_INV;
            }
#else

#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCM_WRITE *buffer_arg;
            AUDIO_ST_PCM_WRITE data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif

            pPcmWriteSrc = (PAUDIO_ST_PCM_WRITE)pArgs;
            pPcmWriteDes = (PAUDIO_ST_PCM_WRITE)buffer_arg;
            CPY_FROM_USER((void *)pPcmWriteDes, (void*)pPcmWriteSrc, sizeof(AUDIO_ST_PCM_WRITE));
            if (pPcmWriteDes->u32Size > 0)
            {
                void* ptmp;
                ptmp = pPcmWriteDes->pBuf;
                pPcmWriteDes->pBuf = kzalloc(pPcmWriteDes->u32Size, GFP_KERNEL);
                if (pPcmWriteDes->pBuf != NULL)
                {
                    CPY_FROM_USER((void*)pPcmWriteDes->pBuf, (void*)ptmp, pPcmWriteDes->u32Size);
                    u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmWriteDes);
                    MS_U32 u32addr = (MS_U32)pPcmWriteSrc + _addr_offset(pPcmWriteDes, &(pPcmWriteDes->u32Ret));
                    CPY_to_USER((void *)u32addr, (void*)&pPcmWriteDes->u32Ret, sizeof(MS_U32));
                    kfree(pPcmWriteDes->pBuf);
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                }
            }
            else
            {
                u32Ret = UTOPIA_STATUS_ERR_INV;
            }
#endif
        }
            break;

        case MApi_CMD_AUDIO_PCM_Flush:
        {
#if(AUDIO_REDUCE_STACK_SIZE == TRUE)
            AUDIO_ST_PCM_FLUSH *buffer_arg;
            AUDIO_ST_PCM_FLUSH data_buffer_arg;
            buffer_arg = &data_buffer_arg;
#endif
            pPcmFlushSrc = (PAUDIO_ST_PCM_FLUSH)pArgs;
            pPcmFlushDes = (PAUDIO_ST_PCM_FLUSH)buffer_arg;
            CPY_FROM_USER((void *)&pPcmFlushDes->s32DeviceId, (void*)pPcmFlushSrc, sizeof(MS_S32));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)pPcmFlushDes);
            MS_U32 u32addr = (MS_U32)pPcmFlushSrc + _addr_offset(pPcmFlushDes, &(pPcmFlushDes->s32Ret));
            CPY_to_USER((void *)u32addr, (void*)&pPcmFlushDes->s32Ret, sizeof(MS_S32));
        }
            break;

        default:
            AUDIO_ADP_MSG("AUDIOADP - Unknown commend!!!\n");
            break;
    }

    //printf("stop\n");

	return u32Ret;

   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}



