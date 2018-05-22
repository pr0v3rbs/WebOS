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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/delay.h>
#include <asm/div64.h>
#else
#include <string.h>
#define do_div(x,y) ((x)/=(y))
#endif

// Internal Definition
#include "regCHIP.h"
#include "drvAUDIO.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "halSOUND.h"
#include "halSIF.h"
#include "r2_shm_comm.h"
#include "decR2_shm.h"
#include "sndR2_shm.h"
#include "halAUR2.h"
#include "ddr_config.h"
#include "audio_comm2.h"

#define OMX_AUDIO_DEBUG  0

#if (defined ANDROID)

#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#ifndef LOGI // android 4.1 rename LOGx to ALOGx
#define LOGI ALOGI
#endif

#ifndef LOGE // android 4.1 rename LOGx to ALOGx
#define LOGE ALOGE
#endif

  #if(OMX_AUDIO_DEBUG)
    #define HALMAD_PRINT(fmt, args...)    LOGI("<<android>>      " fmt, ## args)
  #else
    #define HALMAD_PRINT(fmt, args...)
  #endif

#define HALMAD_ERROR(fmt, args...)    LOGE("<<android>>      " fmt, ## args)

#else
    #ifdef CONFIG_MBOOT //mboot Speed up
        #define HALMAD_PRINT(fmt, args...)
        #define HALMAD_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
    #else
        #if(OMX_AUDIO_DEBUG)
            #define HALMAD_PRINT(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
        #else
            #define HALMAD_PRINT(fmt, args...)
        #endif

        #define HALMAD_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
    #endif
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DBG_MAD(msg) //msg
#define AUD_ERRMSG(msg)  MS_CRITICAL_MSG(msg)
#define AUD_DBGMSG(msg)  MS_DEBUG_MSG(msg)

#define HALMAD_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            HALMAD_ERROR("%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            HALMAD_ERROR("%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (HAL_AUDIO_InitialVars() == FALSE) \
            { \
                MS_ASSERT(0);\
            } \
        } \
    } while(0)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define H2BYTE(value)             ((MS_U8)((value) / 0x10000))
#define HIBYTE(value)             ((MS_U8)((value >>8) & 0x0000FF))
#define LOBYTE(value)             ((MS_U8)((value) & 0x0000FF))

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

AES_INFO audio_aes_info[MAX_ADEC];
static MS_U64 prv_PTS[MAX_ADEC];
static MS_U32 accum_ES[MAX_ADEC], prv_accum_ES[MAX_ADEC];

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern AUDIO_SOURCE_INFO_TYPE AudioInputSrcInfo;
extern MS_U32                 _gMIO_MapBase;
extern MS_U32                 g_audioDumpInfo_interval;
extern AUDIO_SPDIF_OUTPUT_TYPE gSpdifOutputType;
extern MS_U32 g_DSPDDRBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2];
MS_U32                        g_AudioDumpInfoCtrl = Audio_DumpInfo_Off;

extern MS_S32  _s32MutexLoadCode;
extern MS_S32  _s32AUDIOMutexIDMA;

//[LM14A]
MS_BOOL g_TV_Service_CH5_bMute = FALSE;
MS_BOOL g_TV_Service_CH6_bMute = FALSE;
MS_BOOL g_TV_Service_bForeground = TRUE; //TRUE: TV Service is foreground. FALSE: TV Service is background.

//====== STAOS PIP START 2012/02/12 ========

extern pFuncPtr_SetAudioParam2  g_FuncPrt_Hal_SetAudioParam2;
extern pFuncPtr_GetAudioInfo2   g_FuncPrt_Hal_GetAudioInfo2;

//====== STAOS PIP END 2012/02/12 ========

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
void (*Audio_Encoder_callback)(LGE_AENC_MSG_TYPE_T* pMassage) = NULL; //[LM14]
//static MS_BOOL EncodeInProcess;
//#define ENC_BUF_SIZE 6;
extern AUDIO_INPUT_TYPE  g_audioSrcType;
extern AUDIO_SHARED_VARS2 *g_AudioVars2;
#ifndef MSOS_TYPE_NUTTX
extern AUDIO_TEE_INFO_SHARE_MEM   *pAudioTeeInfoShm;
extern MS_BOOL g_bAudioTeeEnabled, g_bAudioTeeMbxInitialized;
extern MS_BOOL g_bDecDspSecureTeeAccessed, g_bSndDspSecureTeeAccessed;
#endif
extern MS_BOOL (*g_FuncPtr_CheckCapability)(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus);

extern MS_BOOL g_bAudio_EnableKernelMsg;

MS_U32  EncFrameIdx;
MS_U32  EncBuf_W_idx;
MS_U32  EncBuf_R_idx;
MS_U8   EncBuf_Count;
MS_U16  Reg_REG_SOUND_MAIN_SNDEFFECT = 0x0;
MS_U16  Reg_REG_SOUND_ADV_CFG0 = 0x0;
MS_BOOL Flag_MAIN_SNDEFFET_CHG = FALSE;

AU_DVB_ENC_FRAME_INFO MPEG_EN_BUF[6];
MS_BOOL FLAG_HDMI_AC3_IN_SE = TRUE;
MS_BOOL g_DDPBypassenable_1 = TRUE;
MS_BOOL g_DDPBypassenable_2 = TRUE;

static MS_U32 g_ReadTargetAddr = 0x112D00;
static MS_U32 g_WriteTargetAddr = 0x112D00;

static MS_U32 g_ReadDMTargetAddr=0;
static MS_U32 g_WriteDMTargetAddr=0;

static MS_U32 g_ReadPMTargetAddr=0;
static MS_U32 g_WritePMTargetAddr=0;

static MS_BOOL g_bR2Adec1AD = FALSE;
static MS_BOOL g_bR2Adec2AD = FALSE;

//for STR Resume
static MS_BOOL bIsSTR_Resume_Boot = FALSE;
static MS_BOOL bIsSTR_Resume_LGSE_Fail = FALSE;

//[M2]for GST output delay.
static MS_U32 g_Omx_OutputDelayOffset_bEnable = TRUE;

//[M2]for GST VP9 60p, need to dynamic changing PCM Mixer thread priority
MS_U32 g_Omx_bIsVP9_60P = FALSE;

#define LAYER_MIN   1
#define LAYER_MAX   3
#define MPEG_BITRATE_TABLE_ROW_NUM  6
#define MPEG_BITRATE_TABLE_COL_NUM  16

MS_BOOL bSwDmaAvailible[2] = {TRUE, TRUE};

#define AUDIO_HASHKEY_DISABLE_AC3                 0x1
#define AUDIO_HASHKEY_DISABLE_AC3P                0x2
#define AUDIO_HASHKEY_DISABLE_DDCO                0x4
#define AUDIO_HASHKEY_DISABLE_DTS                 0x8
#define AUDIO_HASHKEY_DISABLE_DOLBY_MS10          0x10
#define AUDIO_HASHKEY_DISABLE_WMA                 0x20
#define AUDIO_HASHKEY_DISABLE_DRA                 0x40
#define AUDIO_HASHKEY_DISABLE_DTSLBR              0x80
#define AUDIO_HASHKEY_DISABLE_GAAC                0x100
#define AUDIO_HASHKEY_DISABLE_DOLBY_MS11          0x200
#define AUDIO_HASHKEY_DISABLE_DTSE                0x400
#define AUDIO_HASHKEY_DISABLE_DTS_NEO_ULTRA       0x800
#define AUDIO_HASHKEY_DEMO_MODE                   0x1000
#define AUDIO_HASHKEY_DISABLE_SRS_TSHD            0x2000
#define AUDIO_HASHKEY_DISABLE_SRS_THEATERSOUND    0x4000
#define AUDIO_HASHKEY_DISABLE_DTS_STUDIO_SOUND_3D 0x8000
#define AUDIO_HASHKEY_DISABLE_COOK                0x10000
#define AUDIO_HASHKEY_DISABLE_DTS_XLL             0x20000
#define AUDIO_HASHKEY_DISABLE_MS12_LC_PROFILE     0x40000
#define AUDIO_HASHKEY_DISABLE_MS12_C_PROFILE      0x80000
#define AUDIO_HASHKEY_DISABLE_ABS_3D              0x100000
#define AUDIO_HASHKEY_DISABLE_MS12_B_PROFILE      0x200000

const MS_U16 MpegBitRateTable[MPEG_BITRATE_TABLE_ROW_NUM][MPEG_BITRATE_TABLE_COL_NUM] =
{
    // V1, L1
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0},
    // V1, L2
    {0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, 0},
    // V1, L3
    {0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 0},
    // V2, L1
    {0, 32, 48, 56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, 0},
    // V2, L2
    {0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0},
    // V2, L3
    {0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0},
};

MS_U32  u32SeProcessFrameSmpUnit = SE_PROCESS_FRAME_SMP_UNIT;  // For Bifrost audio MT. Default : SE_PROCESS_FRAME_SMP_UNIT. For some advance sound effect ip like SRS TheaterSound, it is 256 because it is 256 samples per frame processing.

//[LM14A]
#define SUPPORTED_SAMPLE_RATE 10

typedef struct
{
    MS_U32 uSample_rate;
    MS_U8  divisor;
    MS_U32 synthrate;
} HW_DMA_READER_SETTING;

/* New DMA Reader setting
 * Formula is :
 * Synthesizer Rate = 216MHz / Divisor(1, 2, 4 or 8) * 1024 / 256 / Sampling Rate(32Khz, 44.1KHz or 48KHz)
 */
//{ SAMPLE_RATE, sample_rate, divisor, synthrate}
HW_DMA_READER_SETTING hw_dma_reader_setting[SUPPORTED_SAMPLE_RATE] =
{
   { 8000,  2, 0x6978 },
   { 11025, 2, 0x4C87 },
   { 12000, 2, 0x4650 },
   { 16000, 1, 0x6978 },
   { 22050, 1, 0x4C87 },
   { 24000, 1, 0x4650 },
   { 32000, 1, 0x6978 },
   { 44100, 1, 0x4C87 },
   { 48000, 0, 0x4650 },
   { 96000, 0, 0x2328 },
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MMA_AudioType _GetAudioCaps(AUDIO_DEC_ID eDecId)
{
    MMA_AudioType eAudioCaps = 0;

    if ( eDecId == AU_DEC_ID1 )
    {
        eAudioCaps = Audio_DEC_PCM        | Audio_DEC_MPEG     |
                     Audio_DEC_MP3        | Audio_DEC_AC3      |
                     Audio_DEC_AC3P       | Audio_DEC_AAC_LC   |
                     Audio_DEC_HEAAC_V1   | Audio_DEC_HEAAC_V2 |
                     Audio_DEC_DOLBYPULSE | Audio_DEC_RA8      |
                     Audio_DEC_WMA        | Audio_DEC_WMAPRO   |
                     Audio_DEC_XPCM       | Audio_DEC_AMR_NB   |
                     Audio_DEC_AMR_WB     | Audio_DEC_VORBIS   |
                     Audio_DEC_DTS        | Audio_DEC_FLAC     |
                     Audio_DEC_DTSLBR     | Audio_DEC_DTS_HD_ADO_R2 |
                     Audio_DEC_DRA;
    }
    else if ( eDecId == AU_DEC_ID2 )
    {
        eAudioCaps = 0;
    }
    else if ( eDecId == AU_DEC_ID3 )
    {
        eAudioCaps = Audio_DEC_PCM        | Audio_DEC_MPEG     |
                     Audio_DEC_MP3        | Audio_DEC_AC3      |
                     Audio_DEC_AC3P       | Audio_DEC_AAC_LC   |
                     Audio_DEC_HEAAC_V1   | Audio_DEC_HEAAC_V2 |
                     Audio_DEC_DOLBYPULSE | Audio_DEC_RA8      |
                     Audio_DEC_WMA        | Audio_DEC_WMAPRO   |
                     Audio_DEC_XPCM       | Audio_DEC_AMR_NB   |
                     Audio_DEC_AMR_WB     | Audio_DEC_VORBIS   |
                     Audio_DEC_DTS        | Audio_DEC_FLAC     |
                     Audio_DEC_DTSLBR     | Audio_DEC_DTS_HD_ADO_R2 |
                     Audio_DEC_DRA;
    }

    return eAudioCaps;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAudioCapability()
/// @brief \b Function \b Description: This routine is used to get DSP capability
/// @param <IN>        \b eDecId    : Dec ID
/// @param <IN>        \b p_AudioDecStatus :   Audio Dec Status structure
/// @param <RET>       \b value :     0-- eDecId not support
///                                                  1--  eDecId support
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_GetAudioCapability(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus)
{
    MS_BOOL ret = FALSE;

    if (eDecId == AU_DEC_ID2)
    {
        return ret;
    }

    switch ((p_AudioDecStatus->eSourceType))
    {
        case E_AUDIO_INFO_HDMI_IN: // HDMI should decode in SE and DEC DSP, always return true
        case E_AUDIO_INFO_ATV_IN:
                    ret = TRUE;
            break;

        case E_AUDIO_INFO_DTV_IN:
            if ((p_AudioDecStatus->eGroup == E_CONNECT_MAIN) ||
                 (p_AudioDecStatus->eGroup == E_CONNECT_SUB))
                {
                    ret = TRUE;
                }
            break;

        case E_AUDIO_INFO_MM_IN:
        case E_AUDIO_INFO_GAME_IN:
            ret = TRUE;
            break;

        default:
            if ((p_AudioDecStatus->eAudFormat) == (_GetAudioCaps(eDecId) & (p_AudioDecStatus->eAudFormat)))
            {
                ret = TRUE;
            }
            break;
    }

    return ret;
}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD SYSETM Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Read_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value by IDMA
/// @param <IN>        \b dsp_addr    : DSP internal sram address
/// @param <IN>        \b dsp_memory_type :    0--DSP_MEM_TYPE_PM
///                                1--DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b value :    U32 sram value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_Read_DSP_sram(MS_U16 u16Dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
// Do nothing because no DEC-DSP in Monoco
    u16Dsp_addr = u16Dsp_addr;
    dsp_memory_type = dsp_memory_type;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Write_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Write DSP internal sram value by IDMA
/// @param <IN>        \b dsp_addr    : DSP internal sram address
/// @param <IN>        \b value     : data want to write
/// @param <IN>        \b dsp_memory_type :    0-- write to DSP_MEM_TYPE_PM
///                                        1-- write to DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  : TRUE/FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_Write_DSP_sram(MS_U16 dsp_addr, MS_U32 value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
 // Do nothing because no DEC-DSP in Monoco
    dsp_addr = dsp_addr;
    value = value;
    dsp_memory_type = dsp_memory_type;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Init()
/// @brief \b Function \b Description:  This routine is the initialization for DVB module
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_Init(void)
{
    HAL_AUDIO_WriteMaskReg(REG_DEC_IDMA_CTRL0, 0x00FF, 0x0002);
    AUDIO_DELAY1US(2*1000);
    HAL_AUDIO_WriteMaskReg(REG_DEC_IDMA_CTRL0, 0x00FF, 0x0003);

    g_FuncPrt_Hal_SetAudioParam2 = HAL_MAD_SetAudioParam2;
    g_FuncPrt_Hal_GetAudioInfo2 = HAL_MAD_GetAudioInfo2;
    g_FuncPtr_CheckCapability = HAL_MAD_GetAudioCapability;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetMemInfo()
/// @brief \b Function \b Description:  This routine is used to set the DEC-DSP memory information
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetMemInfo(void)
{
  // Remove all memory Info(SE-DSP & R2) to SetMemInfo2
    return;
}

// [LM14]
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetEncCmd()
/// @brief \b Function \b Description: This routine is to set encode command.
/// @param <IN>        \b u8EncCmd    :
///                                    0--STOP
///                                    1--START ENCODE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetEncCmd(AU_DVB_ENCCMD enEncComamnd, void* pCB, MS_U8 u8Param)
{
    switch(enEncComamnd)
    {
        case AU_DVB_ENCCMD_START:
            Audio_Encoder_callback = pCB;
            HAL_AUDIO_DigitalOut_Set_Encode(MP3_ENCODE, TRUE);
            break;

        case AU_DVB_ENCCMD_STOP:
            HAL_AUDIO_DigitalOut_Set_Encode(MP3_ENCODE, FALSE);
            Audio_Encoder_callback = NULL;
            break;

        case AU_DVB_ENCCMD_MUTE:
            if(u8Param == 1)
            {
                //HAL_AUDIO_WriteMaskByte(REG_SOUND_MP3_ENCODE_PARAM_1, 0x01, 0x1 );
            }
            else
            {
                //HAL_AUDIO_WriteMaskByte(REG_SOUND_MP3_ENCODE_PARAM_1, 0x01, 0x0 );
            }
            break;

        case AU_DVB_ENCCMD_DEBUG:
            g_AudioVars2->bMP3EncodeDebug = u8Param;

        default:
            break;
    }
}

MS_U32 HAL_MAD_GetEncInfo(AUDIO_ENC_INFO_TYPE infoType)
{
    MS_U32 u32Val = 0;

    switch(infoType)
    {
        case AUDIO_ENC_BITRATE:
                // default 192kB in mpeg encoder
            break;

        case AUDIO_ENC_FRANUM:
                u32Val = (MS_U32)HAL_AUDIO_ReadReg(REG_MPEG_ENCODER_BUF_IDX);
            break;

        case AUDIO_ENC_SRC:
            break;

        case AUDIO_ENC_FORMAT:
//                HAL_AUDIO_WriteMaskByte(REG_ENCODE_TYPE_SEL, 0xF, 0x4);
            break;

        case AUDIO_ENC_DATA_LEN:
            u32Val = HAL_AUDIO_ReadReg(REG_MPEG_ENCODER_FRM_SIZE);
            break;

        case AUDIO_ENC_BUFFER_START_ADDR:
            printf(" %s(%u) Not Support!!!\n", __FUNCTION__, (unsigned int)infoType);
            u32Val = HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC); // + R2_MPEG_ENC_BASE;
            break;

        case AUDIO_ENC_BUFFER_END_ADDR:
            printf(" %s(%u) Not Support!!!\n", __FUNCTION__, (unsigned int)infoType);
            u32Val = HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC); // + R2_MPEG_ENC_BASE + R2_MPEG_ENC_SIZE;
            break;

        default:
            break;
    }

    return u32Val;
}

//[LM14]
MS_BOOL HAL_MAD_SetEncInfo(AUDIO_ENC_INFO_TYPE infoType, MS_U32 param1, MS_U32 param2)
{
    MS_BOOL bSupport = TRUE;

    switch(infoType)
    {
        case AUDIO_ENC_BITRATE:
                // default 192kB in mpeg encoder
            break;
        case AUDIO_ENC_FRANUM:
                // default 2 frames in mpeg encoder, will be same for mp3 encode case
            break;
        case AUDIO_ENC_SRC:
            break;
        case AUDIO_ENC_FORMAT:
//                HAL_AUDIO_WriteMaskByte(REG_ENCODE_TYPE_SEL, 0xF, 0x4);
            break;
        default:
            break;
    }

    return bSupport;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_EncodeInit()    @@ function can be extended
/// @brief \b Function \b Description: This routine is to init encode parameter.
/// @param <IN>        \b u8EncBr    :
///                               Encode Output Bitrate:   0:192kBps, 1:384kBps, 2:128kBps, 3:64kBps
///                           \b
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_EncodeInit(AU_DVB_ENCBR u8EncBr, MS_U8 u8EncFrameNum)
{
    // not use,  remove
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetEncodeFrameInfo()   @@kochien added for PVR
/// @brief \b Function \b Description:  This routine is used to get encoded frame information
/// @param <IN>        \b u32Addr    : encoded frame start address
/// @param <IN>        \b u32Size     : encoded frame data size
/// @param <IN>        \b u64EncIdx  :  frame idx for PTS
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *FrameInfo)
{
    // not use,  remove
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_setDecCmd()
/// @brief \b Function \b Description: This routine is to set  the DVB1 decode command.
/// @param <IN>        \b u8DecCmd    :
///                                    0--STOP
///                                    1--PLAY
///                                    1--RESYNC
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_S32 uniPlay_ackCnt[2] = {0};
void HAL_MAD_SetDecCmd(AU_DVB_DECCMD u8DecCmd)
{
    MS_U16  u16_timeout = 0;
    MS_U32  u32_r2_cmd = 0, u32_driver_cmd = 0;

    /******** for function calling time measure, do not remove ********/
    switch(u8DecCmd)
    {
        case AU_DVB_DECCMD_STOP:
        {
            printf("\033[1;36m [%s]Stop \033[0m \n", __FUNCTION__);/*@audio*/
        }
        break;

        case AU_DVB_DECCMD_PLAY:
        {
            printf("\033[1;36m [%s]DTV Play \033[0m \n", __FUNCTION__);/*@audio*/
        }
        break;

        case AU_DVB_DECCMD_PLAYFRAME_GS:
        {
            printf("\033[1;36m [%s]GST Play \033[0m \n", __FUNCTION__);/*@audio*/
        }
        break;

        default:
            break;
    }

    DBG_MAD(printf("HAL_MAD_setDecCmd:(%x)\r\n",u8DecCmd));

    if (u8DecCmd == AU_DVB_DECCMD_AVSYNC)
    {
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x80, 0x00);  // bit[15] = 0
    }
    else if (u8DecCmd == AU_DVB_DECCMD_FREERUN)
    {
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x80, 0x80); // bit[15] = 1, AU_DEC_R2_CMD_FREERUN
    }
    else
    {

        switch(u8DecCmd)
        {
            case AU_DVB_DECCMD_STOP:
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, ADEC1, 0, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0, 0);           //Set ES write pointer  to start point.
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x6F, AU_DEC_R2_CMD_STOP);
                uniPlay_ackCnt[0] = 0;
            }
            break;

            case AU_DVB_DECCMD_PLAY:
            {
                // SPDIF av-sync threshold
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE3_CMD, 0x7F, 0x00);

                if (g_AudioVars2->g_u8MADSkipResetFlag == TRUE)
                {
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, ADEC1, 0, 0);
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0, 0);           //Set ES write pointer  to start point.
                    HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0xEF, AU_DEC_R2_CMD_STOP);
                    g_AudioVars2->g_u8MADSkipResetFlag = FALSE;
                    AUDIO_DELAY1MS(20);
                }
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x47, AU_DEC_R2_CMD_PLAY);
            }
            break;

            case AU_DVB_DECCMD_PLAYFILETSP:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x47, AU_DEC_R2_CMD_PLAYFILETSP);
                break;

            case AU_DVB_DECCMD_PLAYFRAME_GS :
            {
                // SPDIF av-sync threshold
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE3_CMD, 0x7F, 0x10);

                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x6F, AU_DEC_R2_CMD_PLAYUNI);
            }
            break;

            case AU_DVB_DECCMD_PLAYFILE:
            case AU_DVB_DECCMD_PLAYFILE2_PTS:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x6F, AU_DEC_R2_CMD_PLAYFILE);
                break;

            case AU_DVB_DECCMD_PLAY_AD:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x10, AU_DEC_R2_CMD_PLAY_AD);
                break;

            case AU_DVB_DECCMD_STOP_AD:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x10, 0x00);
                break;

            case AU_DVB_DECCMD_PAUSE:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x40, AU_DEC_R2_CMD_PAUSE);
                break;

            case AU_DVB_DECCMD_SKIP:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x6F, AU_DEC_R2_CMD_SKIP);
                g_AudioVars2->g_u8MADSkipResetFlag = TRUE;
                break;

            default:
                break;
        }

        /* handshake scheme between R2 and Driver */
        if((u8DecCmd == AU_DVB_DECCMD_PLAY)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILETSP)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILE)
        ||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME)||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME_STAGE)||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME_GS)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILE2_PTS))
        {
            u32_driver_cmd = R2_DECCMD_PLAY;  //DEC_PLAYSTATE_PLAY
            //set decoder metadata for PCMR
            //printf("\n\033[1;32m[A] set decoder metadata\033[0m\n");
            HAL_MAD_SetAudioParam2(AU_DEC_ID1, Audio_ParamType_MetaData_Select, E_SELECT_DECODER);
        }
        else if(u8DecCmd == AU_DVB_DECCMD_STOP)
        {
            u32_driver_cmd = R2_DECCMD_STOP;  //DEC_PLAYSTATE_STOP
        }
        else
        {
            return;
        }

        /* wait command passing to r2 */
        while(u16_timeout++ < 100)
        {
            //printf("\033[1;36m [AUDIO][UTOPIA][%s] [%d] [u16_timeout = %d] \033[0m \n", __FUNCTION__, __LINE__, u16_timeout);/*@audio*/
            u32_r2_cmd = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAY_STATE, ADEC1)&0x00FFFFFF;
            AUDIO_DELAY1MS(1);
            if (u32_r2_cmd == u32_driver_cmd)
                break;
        }

        if (u16_timeout >= 100 )
        {
            HALMAD_ERROR(" ===  HAL_MAD_SetDecCmd %d fail===\n",u8DecCmd);
        }
    }

    /******** for function calling time measure, do not remove ********/
    switch(u8DecCmd)
    {
        case AU_DVB_DECCMD_STOP:
        case AU_DVB_DECCMD_PLAY:
        {
            printf(" [%s]End \n", __FUNCTION__);/*@audio*/
        }
        break;

        default:
            break;
    }
}

void HAL_MAD_SetDecCmd2(AUDIO_DEC_ID DecId, AU_DVB_DECCMD u8DecCmd)
{
    if (AUDIO_DSP_ID_DEC == g_AudioVars2->AudioDecStatus[DecId].eDSPId)
    {
        // ADEC DSP
        HAL_MAD_SetDecCmd(u8DecCmd);
    }
    else if (AUDIO_DSP_ID_SND == g_AudioVars2->AudioDecStatus[DecId].eDSPId)
    {
        // ASND DSP
        HAL_MAD_SetDSP2DecCmd(u8DecCmd);
    }
    else
    {
        // AU_DEC_ID2 only for ATV SIF
        // For other case should not use AU_DEC_ID2
        return;
    }

    // Check AD play command
    if((AU_DVB_DECCMD_PLAY_AD == u8DecCmd) || (AU_DVB_DECCMD_STOP_AD == u8DecCmd))
    {
        if(g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_DTV_IN)
        {
            switch(DecId)
            {
                case AU_DEC_ID1:
                case AU_DEC_ID2:
                {
                    switch(u8DecCmd)
                    {
                        case AU_DVB_DECCMD_PLAY_AD:
                        {
                            g_bR2Adec1AD = TRUE;
                        }
                        break;

                        case AU_DVB_DECCMD_STOP_AD:
                        {
                            g_bR2Adec1AD = FALSE;
                        }
                        break;
                        default:
                            break;
                    }
                }
                break;

                case AU_DEC_ID3:
                {
                    switch(u8DecCmd)
                    {
                        case AU_DVB_DECCMD_PLAY_AD:
                        {
                            g_bR2Adec2AD = TRUE;
                        }
                        break;
                        case AU_DVB_DECCMD_STOP_AD:
                        {
                            g_bR2Adec2AD = FALSE;
                        }
                        break;
                    default:
                        break;
                    }
                }
                break;

                default:
                    break;
            }
        }
    }

    switch(DecId)
    {
        case AU_DEC_ID1:
        case AU_DEC_ID2:
        {
            switch(g_bR2Adec1AD)
            {
                case TRUE:
                {
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY_AD);
                }
                break;

                case FALSE:
                {
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP_AD);
                }
                break;

                default:
                    break;
            }
        }
        break;

        case AU_DEC_ID3:
        {
            switch(g_bR2Adec2AD)
            {
                case TRUE:
                {
                    HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAY_AD);
                }
                break;

                case FALSE:
                {
                    HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STOP_AD);
                }
                break;

                default:
                    break;
            }
        }
        break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetDSP2DecCmd()
/// @brief \b Function \b Description: This routine is to set  the DVB3 decode command.
/// @param <IN>        \b u8DecCmd    :
///                                    0--STOP
///                                    1--PLAY
///                                    1--RESYNC
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD u8DecCmd)
{
    MS_U8   FREERUN_STATE = 0;
    MS_U16  u16_timeout = 0;
    MS_U32  u32_r2_cmd = 0, u32_driver_cmd = 0;

    /******** for function calling time measure, do not remove ********/
    switch(u8DecCmd)
    {
        case AU_DVB_DECCMD_STOP:
        {
            printf("\033[1;36m [%s]Stop \033[0m \n", __FUNCTION__);/*@audio*/
        }
        break;

        case AU_DVB_DECCMD_PLAY:
        {
            printf("\033[1;36m [%s]DTV Play \033[0m \n", __FUNCTION__);/*@audio*/
        }
        break;

        case AU_DVB_DECCMD_PLAYFRAME_GS:
        {
            printf("\033[1;36m [%s]GST Play \033[0m \n", __FUNCTION__);/*@audio*/
        }
        break;

        default:
            break;
    }

    DBG_MAD(printf("HAL_MAD_SetDSP2DecCmd:(%x)\r\n", u8DecCmd));

    if (u8DecCmd == AU_DVB_DECCMD_AVSYNC)
    {
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x80, 0x00);  // bit[15] = 0
    }
    else if (u8DecCmd == AU_DVB_DECCMD_FREERUN)
    {
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x80, 0x80); // bit[15] = 1, AU_DEC_R2_CMD_FREERUN
    }
    else
    {

        switch(u8DecCmd)
        {
            case AU_DVB_DECCMD_STOP:
            {
                uniPlay_ackCnt[1] = 0;
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, ADEC2, 0, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0, 0);           //Set ES write pointer  to start point.
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD,0x6F, AU_DEC_R2_CMD_STOP);
            }
            break;

            case AU_DVB_DECCMD_PLAY:
            {
                // SPDIF av-sync threshold
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE3_CMD, 0x7F, 0x00);

                if (g_AudioVars2->g_u8MAD2SkipResetFlag == TRUE)
                {
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, ADEC2, 0, 0);
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0, 0);           //Set ES write pointer  to start point.
                    HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0xEF, AU_DEC_R2_CMD_STOP);
                    g_AudioVars2->g_u8MAD2SkipResetFlag = FALSE;
                    AUDIO_DELAY1MS(20);
                }
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x47, AU_DEC_R2_CMD_PLAY);
            }
            break;

            case AU_DVB_DECCMD_PLAYFILETSP:
               HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x47, AU_DEC_R2_CMD_PLAYFILETSP);
               break;

            case AU_DVB_DECCMD_PLAYFRAME_GS :
            {
                // SPDIF av-sync threshold
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE3_CMD, 0x7F, 0x0C);

                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x7F, AU_DEC_R2_CMD_PLAYUNI);
            }
            break;

            case AU_DVB_DECCMD_PLAYFILE:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x7F, AU_DEC_R2_CMD_PLAYFILE);
                break;

            case AU_DVB_DECCMD_PLAY_AD:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x10, AU_DEC_R2_CMD_PLAY_AD);
                break;

            case AU_DVB_DECCMD_STOP_AD:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x10, 0x00);
                break;

            case AU_DVB_DECCMD_PAUSE:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x40, AU_DEC_R2_CMD_PAUSE);
                break;

            case AU_DVB_DECCMD_SKIP:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x7F, AU_DEC_R2_CMD_SKIP);
                g_AudioVars2->g_u8MAD2SkipResetFlag = TRUE;
                break;

            default:
                break;
        }

        /* handshake scheme between R2 and Driver */
        if((u8DecCmd == AU_DVB_DECCMD_PLAY)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILETSP)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILE)
        ||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME)||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME_STAGE)||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME_GS)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILE2_PTS))
        {
            u32_driver_cmd = R2_DECCMD_PLAY;  //DEC_PLAYSTATE_PLAY
            //set decoder metadata for PCMR
            //printf("\n\033[1;32m[A] set decoder metadata\033[0m\n");
            HAL_MAD_SetAudioParam2(AU_DEC_ID3, Audio_ParamType_MetaData_Select, E_SELECT_DECODER);
        }
        else if(u8DecCmd == AU_DVB_DECCMD_STOP)
        {
            u32_driver_cmd = R2_DECCMD_STOP;  //DEC_PLAYSTATE_STOP
        }
        else
        {
            return;
        }

        /* wait command passing to r2 */
        while(u16_timeout++ < 100)
        {
            //printf("\033[1;36m [AUDIO][UTOPIA][%s] [%d] [u16_timeout = %d] \033[0m \n", __FUNCTION__, __LINE__, u16_timeout);/*@audio*/
            u32_r2_cmd = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAY_STATE, ADEC2)&0x00FFFFFF;
            AUDIO_DELAY1MS(1);
            if (u32_r2_cmd == u32_driver_cmd)
                break;
        }

        if (u16_timeout >= 100 )
        {
            HALMAD_ERROR(" ===  HAL_MAD_SetDSP2DecCmd %d fail===\n",u8DecCmd);
        }
    }

    /******** for function calling time measure, do not remove ********/
    switch(u8DecCmd)
    {
        case AU_DVB_DECCMD_STOP:
        case AU_DVB_DECCMD_PLAY:
        {
            printf(" [%s]End \n", __FUNCTION__);/*@audio*/
        }
        break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetDecCmd()
/// @brief \b Function \b Description: This routine is to get  the DVB1 decode command.
/// @param <OUT>        \b AU_DVB_DECCMD   :
//                                          AU_DVB_DECCMD_STOP,      //0
//                                          AU_DVB_DECCMD_PLAY,      //1
//                                          AU_DVB_DECCMD_PLAYFILETSP = 2,
//                                          AU_DVB_DECCMD_RESYNC,
//                                          ....etc
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AU_DVB_DECCMD HAL_MAD_GetDecCmd(void)
{
    AU_DEC_R2_CMD   r2cmd_status = AU_DEC_R2_CMD_STOP;
    AU_DVB_DECCMD  ret_deccmd = AU_DVB_DECCMD_STOP;

    r2cmd_status = HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_CMD);
    r2cmd_status &= ~AU_DEC_R2_CMD_FREERUN; /* Discard AU_DEC_R2_CMD_FREERUN */

    /*
        need to report status cmd first (stauts cmd : free-run, pause, browse, play AD)
    */

    if((r2cmd_status & AU_DEC_R2_CMD_PAUSE) == AU_DEC_R2_CMD_PAUSE) // check pause
    {
        ret_deccmd = AU_DVB_DECCMD_PAUSE;
    }
    else if((r2cmd_status & AU_DEC_R2_CMD_STARTBROWSE) == AU_DEC_R2_CMD_STARTBROWSE)   // check startbrowse
    {
        ret_deccmd = AU_DVB_DECCMD_STARTBROWSE;
    }
    else if((r2cmd_status & AU_DEC_R2_CMD_PLAY_AD) == AU_DEC_R2_CMD_PLAY_AD)    // check play AD
    {
        ret_deccmd = AU_DVB_DECCMD_PLAY_AD;
    }
    else    // others : not status cmd
    {
        switch(r2cmd_status)
        {
            case AU_DEC_R2_CMD_STOP:
                ret_deccmd = AU_DVB_DECCMD_STOP;
                break;

            case AU_DEC_R2_CMD_PLAY:
                ret_deccmd = AU_DVB_DECCMD_PLAY;
                break;

            case AU_DEC_R2_CMD_PLAYFILETSP:
                ret_deccmd = AU_DVB_DECCMD_PLAYFILETSP;
                break;

            case AU_DEC_R2_CMD_PLAYUNI:
                ret_deccmd = AU_DVB_DECCMD_PLAYFRAME_GS;
                break;

            case AU_DEC_R2_CMD_PLAYFILE:
                ret_deccmd = AU_DVB_DECCMD_PLAYFILE;
                break;

            default:
                break;
        }
    }

    return ret_deccmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetDSP2DecCmd()
/// @brief \b Function \b Description: This routine is to get  the DVB3 decode command.
/// @param <OUT>        \b AU_DVB_DECCMD   :
//                                          AU_DVB_DECCMD_STOP,      //0
//                                          AU_DVB_DECCMD_PLAY,      //1
//                                          AU_DVB_DECCMD_PLAYFILETSP = 2,
//                                          AU_DVB_DECCMD_RESYNC,
//                                          ....etc
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AU_DVB_DECCMD HAL_MAD_GetDSP2DecCmd(void)
{
    AU_DEC_R2_CMD   r2cmd_status = AU_DEC_R2_CMD_STOP;
    AU_DVB_DECCMD  ret_deccmd = AU_DVB_DECCMD_STOP;

    r2cmd_status = HAL_AUDIO_AbsReadByte(REG_R2_DECODE2_CMD);
    r2cmd_status = r2cmd_status&0x7F;

    /*
        need to report status cmd first (stauts cmd : free-run, pause, browse, play AD)
    */

    if((r2cmd_status & AU_DEC_R2_CMD_PAUSE) == AU_DEC_R2_CMD_PAUSE) // check pause
    {
        ret_deccmd = AU_DVB_DECCMD_PAUSE;
    }
    else if((r2cmd_status & AU_DEC_R2_CMD_STARTBROWSE) == AU_DEC_R2_CMD_STARTBROWSE)   // check startbrowse
    {
        ret_deccmd = AU_DVB_DECCMD_STARTBROWSE;
    }
    else if((r2cmd_status & AU_DEC_R2_CMD_PLAY_AD) == AU_DEC_R2_CMD_PLAY_AD)    // check play AD
    {
        ret_deccmd = AU_DVB_DECCMD_PLAY_AD;
    }
    else    // others : not status cmd
    {
        switch(r2cmd_status)
        {
            case AU_DEC_R2_CMD_STOP:
                ret_deccmd = AU_DVB_DECCMD_STOP;
                break;

            case AU_DEC_R2_CMD_PLAY:
                ret_deccmd = AU_DVB_DECCMD_PLAY;
                break;

            case AU_DEC_R2_CMD_PLAYFILETSP:
                ret_deccmd = AU_DVB_DECCMD_PLAYFILETSP;
                break;

            case AU_DEC_R2_CMD_PLAYUNI:
                ret_deccmd = AU_DVB_DECCMD_PLAYFRAME_GS;
                break;

            case AU_DEC_R2_CMD_PLAYFILE:
                ret_deccmd = AU_DVB_DECCMD_PLAYFILE;
                break;

            default:
                break;
        }
    }

    return ret_deccmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_setFreeRun()
/// @brief \b Function \b Description: This routine is to set  the decoder in free run or AV-sync mode.
/// @param <IN>        \b u8FreeRun    :    0--normal AV sync
///                                        1--free-run mode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetFreeRun( MS_U8  u8FreeRun )
{
    DBG_MAD(printf("HAL_MAD_setFreeRun:(%x)\r\n",u8FreeRun));
    if( u8FreeRun >= 2 ){
        DBG_MAD(printf("Invalid mode\r\n"));
        return;
    }

    HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x80, (u8FreeRun << 7));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB1 MIU request command.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_DisEn_MIUREQ(void)
{
    // Disable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0040 );
    AUDIO_DELAY1MS(1);
    // Enable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0000 );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Dis_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB1 MIU request command.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_Dis_MIUREQ(void)
{
    // Disable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0040 );        // disable
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0080, 0x0080 );          // reset MAD module
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_RSTMAD_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB1 MIU request command.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_RSTMAD_DisEn_MIUREQ(void)
{
    // Disable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0040 );        // disable
    // Reset MAD
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0080, 0x0080 );          // reset MAD module
    AUDIO_DELAY1MS(1);
    // Set MAD
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0080, 0x00 );
    // Enable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0000 );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_DSPACK()
/// @brief \b Function \b Description: This routine is to report DSP reload ACK cmd.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_GetReloadCodeAck(void)
{
    //NOT_DEFINE : to do
    return(HAL_AUDIO_ReadByte(REG_MB_DE_ACK2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_DSPACK()
/// @brief \b Function \b Description: This routine is to report DSP reload ACK cmd.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_GetLoadCodeAck(void)
{
    //NOT_DEFINE : to do
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Rpt_DTVES()
/// @brief \b Function \b Description: This routine is used to report AC3/MPEG stream if exist or not.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8  :    the decoder status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_Rpt_DTVES(void)
{
    MS_U8   Rpt_DTVES_tmp;
    Rpt_DTVES_tmp = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1);

    return Rpt_DTVES_tmp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetStreamID_Mod()
/// @brief \b Function \b Description: This routine is used to AC3 Stream ID mod.
/// @param <IN>        \b stream_id_mod    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetStreamID_Mod(MS_U8 stream_id_mod)
{
    //NOT_DEFINE : not used, remove
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_ReadTimeStamp()
/// @brief \b Function \b Description: This routine returns the stmestamp while decoding file format.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b timestamp    :    time stamp value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_ReadTimeStamp(void)
{
    MS_U32 timestamp = 0;
    MS_U32  pts = 0;

    pts = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC1);
    timestamp = pts/45;

    return timestamp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD__SetAdMixMode()
/// @brief \b Function \b Description: This routine is used to set MPEG mixed with AD or AD mixed with MPEG or not.
/// @param <IN>        \b u8Mix_mode    :    0--main mix to AD
///                                    1--AD mix to main
/// @param <IN>        \b bEnMix    :    0--disable AD mix
///                                    1--enable AD mix
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetAdMixMode (AD_MIX_MODE u8Mix_mode, MS_BOOL bEnMix)
{
    //NOT_DEFINE
    switch (u8Mix_mode)
    {
    case MAIN_MIX_AD:
    case AD_MIX_MAIN:
        HAL_AUDIO_WriteMaskReg(REG_MB_DEC3_CTRL, 0x8000, bEnMix<<15);  // IO_100 [15]
        break;

    default:
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetMcuCmd()
/// @brief \b Function \b Description: This routine is to write MCU cmd for PIO.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetPIOCmd(MS_U8 cmd)
{
    //NOT_DEFINE : not used, remove
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetMcuCmd()
/// @brief \b Function \b Description: This routine is to write MCU cmd.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetMcuCmd(MS_U8 cmd)
{
    //NOT_DEFINE : not used, remove
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetDspIDMA()
/// @brief \b Function \b Description:  This function is used to set DSP IDMA.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetDspIDMA(void)
{
    //NOT_DEFINE : not used, remove
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_TriggerPIO8()
/// @brief \b Function \b Description:  This function is used to trigger PIO8 init.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_TriggerPIO8(void)
{
    //NOT_DEFINE : not used, remove
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_DvbFLockSynthesizer_En()
/// @brief \b Function \b Description:  This function is used to set DSP IDMA.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_DvbFLockSynthesizer_En(void)
{
    // enable force lock current DVB SYNC synthesizer freq
    // SIF only in SE DSP
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_INPUT_CFG, 0x0040, 0x0040 );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetCertMode()
/// @brief \b Function \b Description:
/// @param CertType    \b :   AUDIO_DOLBY_DD_Cert,
/// @param CertType    \b :   AUDIO_DOLBY_DDP_Cert,
/// @param CertType    \b :   AUDIO_DOLBY_PULSE_Cert,
/// @param CertType    \b :   AUDIO_DOLBY_MS10_Cert,
/// @param CertType    \b :   AUDIO_DTS_DMP_Cert,
/// @param CertType    \b :   AUDIO_DTS_BC_Cert,
/// @param CertType    \b :   AUDIO_DTS_PremiumTV_Cert,
/// @param CertType    \b :   AUDIO_SRS_XT_Cert,
/// @param enable      \b :   AUDIO_CERT_Enable / AUDIO_CERT_Disable
///
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SetCertMode( Audio_CertMode CertType,Audio_CertEnable enCert )
{
    switch(CertType)
    {
        case AUDIO_DOLBY_DD_Cert:
        case AUDIO_DOLBY_DDP_Cert:
        case AUDIO_DOLBY_PULSE_Cert:
        case AUDIO_DOLBY_MS10_Cert:
        case AUDIO_DTS_DMP_Cert:
        case AUDIO_DTS_BC_Cert:
            // sound effect bypass setting
            break;

        case AUDIO_DOLBY_DDCO_Cert:
            break;

        case AUDIO_DTS_SYMMETRY_Cert:
            break;

        case AUDIO_DTS_ENVELO_Cert:
            break;

        case AUDIO_SRS_XT_Cert:
            break;

        default:
            break;
    }
    return(true);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MM2_initAesInfo()
/// @brief \b Function \b Description  : Initialize AES info
/// @param <IN>        \b dec_id  : Decoder ID
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_MM2_initAesInfo( AUDIO_DEC_ID dec_id )
{
    AES_INFO *p_decEsInfo;
    MS_U32   decEsBase, decEsSize;
    Audio_id adec_id;

    if ( dec_id == AU_DEC_ID1 )
    {
        p_decEsInfo = &audio_aes_info[0];
        decEsBase = OFFSET_ES1_DRAM_ADDR;
        decEsSize = ES1_DRAM_SIZE;
        adec_id = ADEC1;
    }
    else if ( dec_id == AU_DEC_ID3 )
    {
        p_decEsInfo = &audio_aes_info[1];
        decEsBase = OFFSET_ES3_DRAM_ADDR;
        decEsSize = ES3_DRAM_SIZE;
        adec_id = ADEC2;
    }
    else
    {
        return FALSE;
    }

    p_decEsInfo->aes_base_addr =  decEsBase;
    p_decEsInfo->aes_BufSize = decEsSize;
    p_decEsInfo->aes_end_addr = p_decEsInfo->aes_base_addr + p_decEsInfo->aes_BufSize;
    p_decEsInfo->aes_write_addr = p_decEsInfo->aes_base_addr;
    p_decEsInfo->aes_freeSpace = p_decEsInfo->aes_BufSize -  (BYTES_IN_MIU_LINE*2);
    p_decEsInfo->aes_level = 0;
    p_decEsInfo->aes_lastPTS = 0;
    p_decEsInfo->aes_ptsCnt = 0;

    if(adec_id < MAX_ADEC)
    {
        prv_PTS[adec_id] = 0;
        accum_ES[adec_id] = 0;
        prv_accum_ES[adec_id] = 0;
    }

    /* Set ES write pointer to DSP */
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, adec_id, 0, 0);           //Set ES write pointer  to start point.

    DBG_MAD(printf("HAL_AUDIO_MM2_init for ADEC%X end !!\r\n", adec_id));
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MM2_checkAesInfo()
/// @brief \b Function \b Description  : Get AES info from audio decoder
/// @param <IN>        \b dec_id       : Decoder ID
/// @param <IN>        \b aes_info     : AES info structure pointer
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info )
{

    AES_INFO *p_decEsInfo;
    Audio_id adec_id;
    MS_U32 decEsBase, tmp1, tmp2;

    if ( dec_id == AU_DEC_ID1 )
    {
        p_decEsInfo = &audio_aes_info[0];
        decEsBase = OFFSET_ES1_DRAM_ADDR;
        adec_id = ADEC1;
    }
    else if ( dec_id == AU_DEC_ID3 )
    {
        p_decEsInfo = &audio_aes_info[1];
        decEsBase = OFFSET_ES3_DRAM_ADDR;
        adec_id = ADEC2;
    }
    else
    {
        return FALSE;
    }

    /* es level */
    p_decEsInfo->aes_level = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, adec_id);

    /* calculate free space for next data input */
    tmp1 = p_decEsInfo->aes_end_addr - p_decEsInfo->aes_write_addr;
    tmp2 = p_decEsInfo->aes_BufSize - p_decEsInfo->aes_level;

    if ( tmp2 < (p_decEsInfo->aes_BufSize)/2 )
        tmp2 = 0;
    else
        tmp2 -= ((p_decEsInfo->aes_BufSize)/2);

    if ( tmp2 > (p_decEsInfo->aes_BufSize)/4 )
        tmp2 = (p_decEsInfo->aes_BufSize/4);

    p_decEsInfo->aes_freeSpace = (tmp1 > tmp2)?tmp2:tmp1;
    p_decEsInfo->aes_read_addr = decEsBase + HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_ES_RD_PTR, adec_id );

    memcpy( (void *)aes_info,  (void *)p_decEsInfo, sizeof(AES_INFO));
    DBG_MAD(printf("[HAL_MAD_MM2_checkAesInfo ADEC%X ] Ptr[w:%x,r:%x] LvL:%x, FreeSpace:%x, tmpFree:%x \n",
        adec_id,
        p_decEsInfo->aes_write_addr,
        p_decEsInfo->aes_read_addr,
        p_decEsInfo->aes_level,
        p_decEsInfo->aes_BufSize - p_decEsInfo->aes_level,
        p_decEsInfo->aes_freeSpace));

    return TRUE;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MM2_inputAesFinished()
/// @brief \b Function \b Description  : Inform DSP how much ES data is sent
/// @param <IN>        \b dec_id       : Decoder ID
/// @param <IN>        \b es_size  : Size of ES data
/// @param <IN>        \b ptsExist : Whether ES data contains PTS or not
/// @param <IN>        \b pts      : PTS
/// @param <OUT>       \b MS_BOOL  : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE     :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts )
{
    AES_INFO *p_decEsInfo;
    Audio_id adec_id;

    MS_U32 es_writeLineOffset;
    MS_U32 tmp1, tmp2;

    if ( dec_id == AU_DEC_ID1 )
    {
        p_decEsInfo = &audio_aes_info[0];
        adec_id = ADEC1;
    }
    else if ( dec_id == AU_DEC_ID3 )
    {
        p_decEsInfo = &audio_aes_info[1];
        adec_id = ADEC2;
    }
    else
    {
        return FALSE;
    }

    if ( (es_size == 0) || (adec_id >= MAX_ADEC) )
        return FALSE;

    /* es level */
    p_decEsInfo->aes_level = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, adec_id);

    /* calculate free space for next data input */
    tmp1 = p_decEsInfo->aes_end_addr - p_decEsInfo->aes_write_addr;
    tmp2 = p_decEsInfo->aes_BufSize - p_decEsInfo->aes_level;

    if ( tmp2 < (BYTES_IN_MIU_LINE*2) )         //�ܤ� keep 2 �� line ���Ŷ�
        tmp2 = 0;
    else
        tmp2 -= (BYTES_IN_MIU_LINE*2) ;

    p_decEsInfo->aes_freeSpace = (tmp1 - tmp2)?tmp2:tmp1;

    if ( es_size > (p_decEsInfo->aes_freeSpace) )
    {
        HALMAD_ERROR("==> Error, ADEC%X input AES size(0x%08u) is larger than current Free space(0x%08u)\r\n", adec_id, (unsigned int)es_size, (unsigned int)(p_decEsInfo->aes_freeSpace) );
        return FALSE;
    }

    DBG_MAD(printf("[HAL_MAD_MM2_inputAesFinished] ADEC%X wPtr:%08lx => ", adec_id, p_decEsInfo->aes_write_addr));

    /* update write pointer */
    p_decEsInfo->aes_write_addr += es_size;

    if ( (p_decEsInfo->aes_write_addr) >= (p_decEsInfo->aes_end_addr) )
        (p_decEsInfo->aes_write_addr) -= (p_decEsInfo->aes_BufSize);

    DBG_MAD(printf("ADEC%X, %08x, size=%08x, isPtsExist:%d, PTS=%09llx\n",
        adec_id,
        p_decEsInfo->aes_write_addr,
        es_size,
        ptsExist,
        do_div(pts,90)));

    if (ptsExist)
    {
        if (prv_PTS[adec_id] != 0)
        {
            DBG_MAD(printf("ADEC%X ,PTS_diff = %08lld, accum_ES = %08lx, ES_diff= %08ld, ES PTS diff = %08ld\n",
                adec_id,
                do_div((pts - prv_PTS[adec_id]),90),
                accum_ES[adec_id],
                accum_ES[adec_id] - prv_accum_ES[adec_id],
                (accum_ES[adec_id] - prv_accum_ES[adec_id])*32/1536));
        }
        else
        {
            DBG_MAD(printf("ADEC%X, PTS_diff=%08x, accum_ES = %08x\n", adec_id, 0, 0));
        }

        prv_PTS[adec_id] = pts;
        prv_accum_ES[adec_id] = accum_ES[adec_id];
    }
    else
    {
        DBG_MAD(printf("\n"));
    }

    accum_ES[adec_id] += es_size;

    es_writeLineOffset = p_decEsInfo->aes_write_addr - p_decEsInfo->aes_base_addr;

    if (ptsExist)
    {
        if ((p_decEsInfo->aes_lastPTS  == pts)&&(p_decEsInfo->aes_ptsCnt != 0))
        {
            HALMAD_ERROR(" ======== Error PTS is same !! =============\n");
        }
        else
        {
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_INPUT_PTS, adec_id, (pts >> 1), 0);
            p_decEsInfo->aes_ptsCnt++;
            p_decEsInfo->aes_lastPTS = pts;
            AUDIO_DELAY1US(500);
        }
    }

    /* Set ES write pointer to DSP */
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, adec_id, es_writeLineOffset, 0);
    AUDIO_DELAY1US(250);                                                                                        //Since DSP timer update ES_MEM_CNT at each 125us, need this delay to keep

    return TRUE;
}


///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD MPEG Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MPEG_GetHeaderInfo()
/// @brief \b Function \b Description: This routine will return the 32bit mpeg header
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b U32    : mpeg header information
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_MPEG_GetHeaderInfo(void)
{
    MS_U16 data;

    data = (MS_U16)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MPEG_HEADER, ADEC1);

    return (data & 0x0FFF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MPEG_GetSampleRate()
/// @brief \b Function \b Description: This routine will return the 16bit mpeg samplerate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b U16    : mpeg sampling rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MAD_MPEG_GetSampleRate(void)
{
    return (MS_U16)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MPEG_GetBitRate()
/// @brief \b Function \b Description: This routine will return the 16-bit mpeg bit rate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b U16    : mpeg bit rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MAD_MPEG_GetBitRate(void)
{
   return (MS_U16) HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MPEG_GetSoundMode()
/// @brief \b Function \b Description: This routine will return the 8bit mpeg sound mode
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MPEG_SOUND_MODE :
///                                         MPEG_SOUND_MODE_STEREO =0,
///                                         MPEG_SOUND_MODE_JOINT_STEREO,
///                                         MPEG_SOUND_MODE_DUAL_CHAN,
///                                         MPEG_SOUND_MODE_MONO
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MPEG_SOUND_MODE HAL_MAD_MPEG_GetSoundMode(void)
{
    return (MPEG_SOUND_MODE)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MPEG_STEREOMODE, ADEC1);
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD Dolby Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Monitor_DDPlus_SPDIF_Rate()
/// @brief \b Function \b Description: This routine is used to set reference ADC clock by DD+ dsp code
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_Monitor_DDPlus_SPDIF_Rate(void)
{
    MS_U8 u8SpdifMode = SPDIF_OUT_PCM;
    MS_U8 eAudioSource = 0;
    MS_U8 u8MainDecodeId = 0;
    AUDIO_DEC_ID MainDecId = AU_DEC_ID1;
    MS_U8 u8MainDspCodeType = MSAPI_AUD_DVB_INVALID;
    MS_U8 u8ADEC1CodeType = MSAPI_AUD_DVB_INVALID;
    MS_U8 u8ADEC2CodeType = MSAPI_AUD_DVB_INVALID;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    MS_BOOL retTx1, retTx2;
    MS_U32 u32spdif_info_flag = 0;  //0: disable. 1: bypass/convert,  2: transcoder, 3: pcm

    HALMAD_CHECK_SHM_INIT;

    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));

    eAudioSource = (MS_U8)g_AudioVars2->eAudioSource;
    u8ADEC1CodeType = g_AudioVars2->g_u8DecR2Adec1DecType;
    u8ADEC2CodeType = g_AudioVars2->g_u8DecR2Adec2DecType;

    HAL_AUDIO_SPDIF_SetMode(gSpdifOutputType, eAudioSource);

    if (_HAL_AUDIO_SPDIF_GetMode() == SPDIF_OUT_PCM)
    {
        u8SpdifMode = SPDIF_OUT_PCM;
    }
    else
    {
        u8SpdifMode = SPDIF_OUT_NONPCM;
    }

    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
    if (retTx1 == FALSE || retTx2 == FALSE)
    {
        HALMAD_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
    }

    switch(eAudioSource)
    {
        case E_AUDIO_INFO_DTV_IN:
        case E_AUDIO_INFO_MM_IN:
        case E_AUDIO_INFO_GAME_IN:
        {
            if(g_AudioVars2->g_R2_NonPcm_Sel == ADEC1)
            {
                u8MainDspCodeType = u8ADEC1CodeType;
                u8MainDecodeId = ADEC1;
                MainDecId = AU_DEC_ID1;
            }
            else
            {
                u8MainDspCodeType = u8ADEC2CodeType;
                u8MainDecodeId = ADEC2;
                MainDecId = AU_DEC_ID3;
            }

            if (u8SpdifMode == SPDIF_OUT_PCM)
            {
                break;
            }

            switch(u8MainDspCodeType)
            {
                case AU_DVB_STANDARD_AC3:
                case AU_DVB_STANDARD_AC3P:
                case AU_DVB_STANDARD_MS10_DDC:
                case AU_DVB_STANDARD_MS10_DDT:
                case AU_DVB_STANDARD_AAC:
                {

                    // 0: disable, 1: bypass, 2: transcoder
                    HAL_MAD_GetAudioInfo2(MainDecId , Audio_R2_SHM_INFO_SPDIF_INFO_FLAG, (void *)&u32spdif_info_flag);

                    // DDEncode at DSP/SND-R2 or DEC-R2
                    if(u32spdif_info_flag == 0 || u32spdif_info_flag == 2)
                    {
                        /* 48KHz */
                        stDigitalChannelStatusTx1.u8SamplingRate = 0x40;     /* change SPDIF Tx1 channel status --> 48KHz */
                        stDigitalChannelStatusTx2.u8SamplingRate = 0x40;     /* change SPDIF Tx2 channel status --> 48KHz */
                    }
                    else// Bypass at DEC-R2
                    {
                        switch(HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, u8MainDecodeId))
                        {
                            case 44100:
                            {
                                /* change SPDIF Tx 1&2 channel status --> 44.1KHz */
                                stDigitalChannelStatusTx1.u8SamplingRate = 0x00;
                                stDigitalChannelStatusTx2.u8SamplingRate = 0x00;
                            }
                            break;

                            case 32000:
                            {
                                /* change SPDIF Tx 1&2 channel status --> 32KHz */
                                stDigitalChannelStatusTx1.u8SamplingRate = 0xC0;
                                stDigitalChannelStatusTx2.u8SamplingRate = 0xC0;
                            }
                            break;

                            case 48000:
                            default:
                            {
                                /* change SPDIF Tx 1&2 channel status --> 48KHz */
                                stDigitalChannelStatusTx1.u8SamplingRate = 0x40;
                                stDigitalChannelStatusTx2.u8SamplingRate = 0x40;
                            }
                            break;
                        }
                    }
                }
                break;

                case AU_DVB_STANDARD_DTS:
                case AU_DVB_STANDARD_DTSLBR:
                case AU_DVB_STANDARD_DTSXLL:
                {

                    // 0: disable, 1: bypass, 2: transcoder
                    HAL_MAD_GetAudioInfo2(MainDecId , Audio_R2_SHM_INFO_SPDIF_INFO_FLAG, (void *)&u32spdif_info_flag);

                    // Transcode at DSP/SND-R2
                    if(u32spdif_info_flag == 0)
                    {
                        /* 48KHz */
                        stDigitalChannelStatusTx1.u8SamplingRate = 0x40;     /* change SPDIF Tx1 channel status --> 48KHz */
                        stDigitalChannelStatusTx2.u8SamplingRate = 0x40;     /* change SPDIF Tx2 channel status --> 48KHz */
                    }
                    else// Bypass or Transcode at DEC-R2
                    {
                        switch(HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, u8MainDecodeId))
                        {
                            case 44100:
                            {
                                /* change SPDIF Tx 1&2 channel status --> 44.1KHz */
                                stDigitalChannelStatusTx1.u8SamplingRate = 0x00;
                                stDigitalChannelStatusTx2.u8SamplingRate = 0x00;
                            }
                            break;

                            case 48000:
                            default:
                            {
                                /* change SPDIF Tx 1&2 channel status --> 48KHz */
                                stDigitalChannelStatusTx1.u8SamplingRate = 0x40;
                                stDigitalChannelStatusTx2.u8SamplingRate = 0x40;
                            }
                            break;
                        }
                    }
                }
                break;

                default:
                    break;
            }
        }
        break;

        default:
            break;
    }
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD WMA Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_WMA_SetASFParm()
/// @brief \b Function \b Description: This routine will set WMA ASF Paramaters.
/// @param <IN>        \b NONE    : WMA_ASF_PARMTYPE, value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_WMA_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value)
{
    switch ( parm_type )
    {
        case WMA_PARAMTYPE_VERSION:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_VERSION, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_CHANNELS:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELS, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_SAMPLERATE:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_SAMPLERATE, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_BYTERATE:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BYTERATE, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_BLOCKALIGN:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BLOCKALIGN, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_ENCOPT:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_ENCOPT, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_PARSINGBYAPP:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_PARSINGBYAPP, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_BITS_PER_SAMPLE:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BITS_PER_SAMPLE, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_CHANNELMASK:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELMASK, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_PARAM_EXIST:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PARAM_EXIST, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_RMS_AMP_REF:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_REF, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_RMS_AMP_TARGET:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_TARGET, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_PEAK_AMP_REF:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_REF, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_PEAK_AMP_TARGET:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_TARGET, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_MAX_PACKET_SIZE:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_MAX_PACKET_SIZE, ADEC1, value, 0);
            break;
        default:
            break;
    }
}


///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD OTHER Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_RA8_setParam()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing RA8 LBR data
/// @param <IN>        \b mNumCodecs    :    1--stereo or mono
///                                        2~5-- multi channels
/// @param <IN>        \b mSamples    :         output samples--256, 512, 1024
/// @param <IN>        \b mSampleRate    :    8, 11.025, 16, 22.05, 44.1, 48 (KHz)
/// @param <IN>        \b Channels    :    1-- mono
///                                    2-- stereo
/// @param <IN>        \b cplStart    :    0 ~ 51
/// @param <IN>        \b Regions    :    0 ~ 31
/// @param <IN>        \b cplQbits    :    0, 2, 3, 4, 5, 6
/// @param <IN>        \b FrameSize    :    bitstream size of every NumCodes (bytes)
///                                    Range--0 ~ 65535
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8 :    TRUE--parameters are correct
///                            FALSE--parameters are incorrect
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_RA8_setParam(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                           MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                           MS_U16* FrameSize)
{

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_NUMCODECS, ADEC1, mNumCodecs, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLES, ADEC1, mSamples, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLERATES, ADEC1, mSampleRate, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[0], 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[0], 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[0], 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[0], 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[0], 0 );

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[1], 1 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[1], 1 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[1], 1 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[1], 1 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[1], 1 );

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[2], 2 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[2], 2 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[2], 2 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[2], 2 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[2], 2 );

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[3], 3 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[3], 3 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[3], 3 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[3], 3 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[3], 3 );

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[4], 4 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[4], 4 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[4], 4 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[4], 4 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[4], 4 );

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_XPCM_GetParaBase()
/// @brief \b Function \b Description: This routine is used to get the base address for XPCM parameter
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U16  : XPCM parameter base address in DSP
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MAD_XPCM_GetParaBase(void)
{
    //NOT_DEFINE : not used
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_RA8_GetParaBase()
/// @brief \b Function \b Description: This routine is used to get the base address for Ra8 parameter
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U16  : Ra8 parameter base address in DSP
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MAD_Ra8_GetParaBase(void)
{
    //NOT_DEFINE : not used
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetCommInfo()
/// @brief \b Function \b Description: This routine set different paramter to audio decoder
/// @param <IN>        \b Audio_COMM_infoType    : common audio parameter type
/// @param <IN>        \b MS_U32    : common audio parameter 1
/// @param <IN>        \b MS_U32    : common audio parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetCommInfo( Audio_COMM_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    HALMAD_CHECK_SHM_INIT;

    MS_BOOL retTx1, retTx2;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));

    switch(infoType)
    {
        /********************************************************************************
        *       Other CMD
        ********************************************************************************/
        case Audio_Comm_infoType_Set_UNI_NEED_DECODE_FRMCNT:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_SETUP_DECODE_FRMCNT, ADEC1, param1, 0);
            break;

        case Audio_Comm_infoType_Set_UNI_ES_Wptr:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, param1, 0);
            break;

        case Audio_Comm_infoType_Set_SE_Test_Init:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(REG_SOUND_MAIN_SNDEFFECT,0xFFFF, 0x00);
                HAL_AUDIO_WriteMaskReg(REG_SOUND_ADV_CFG0,0xFFFF, 0x00);
                HAL_AUDIO_WriteByte(REG_SOUND_NR_THRESHOLD, 0x00);
                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_GPA_ISR_DISABLE_AND_SE_DMA_Init);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_GPA_ISR_DISABLE_AND_SE_DMA_Init);
                AUDIO_DELAY1MS(10); // need this, don't mark.
            }
            u32SeProcessFrameSmpUnit = SE_PROCESS_FRAME_SMP_UNIT;
            break;

        case Audio_Comm_infoType_Set_SE_Test_End:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_GPA_ISR_ENABLE);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_GPA_ISR_ENABLE);
                HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_SRS_SEL, 0x01, 0x00);
                HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_ABS3D_SEL, 0x01, 0x00);
            }
            break;

        case Audio_Comm_infoType_Set_SE_TONE_Test_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_TONE_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_TONE_TEST_BEGIN);
            }
            break;

        case Audio_Comm_infoType_Set_SE_PEQ_Test_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_PEQ_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_PEQ_TEST_BEGIN);
            }
            break;

        case Audio_Comm_infoType_Set_SE_GEQ_Test_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_GEQ_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_GEQ_TEST_BEGIN);
            }
            break;

        case Audio_Comm_infoType_Set_SE_AVC_Test_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_AVC_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_AVC_TEST_BEGIN);
            }
            break;

        case Audio_Comm_infoType_Set_SE_SRS_THEATER_SOUND_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SRS_THEATER_SOUND_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SRS_THEATER_SOUND_TEST_BEGIN);
            }
            u32SeProcessFrameSmpUnit = 256;
            break;

        case Audio_Comm_infoType_Set_SE_SONICEMOTION_ABS3D_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SONICEMOTION_ABS3D_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SONICEMOTION_ABS3D_TEST_BEGIN);
            }
            u32SeProcessFrameSmpUnit = 256;
            break;

        case Audio_Comm_infoType_Set_SE_BONGIOVI_DPS_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_BONGIOVI_DPS_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_BONGIOVI_DPS_TEST_BEGIN);
            }
            u32SeProcessFrameSmpUnit = 256;
            break;

        case Audio_Comm_infoType_Set_SE_DTS_STUDIOSOUND_3D_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_DTS_STUDIOSOUND_3D_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_DTS_STUDIOSOUND_3D_TEST_BEGIN);
            }
            u32SeProcessFrameSmpUnit = 256;
            break;

        case Audio_Comm_infoType_Set_SE_Input_Main_Buffer_Feed:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SE_PROCESS_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SE_PROCESS_BEGIN);
//                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD2, 0xFFFF, param1);
                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SE_PROCESS_FEED);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SE_PROCESS_FEED);
            }
            break;

        case Audio_Comm_infoType_Set_SE_Output_Main_Buffer_Consume:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SE_PROCESS_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SE_PROCESS_BEGIN);
//                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD2, 0xFFFF, param1);
                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SE_PROCESS_CONSUME);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while ((u16RegVal != MBOX_DBGCMD_SE_PROCESS_CONSUME_NEED) && (u16RegVal != MBOX_DBGCMD_SE_PROCESS_CONSUME_NO_NEED));
                if (u16RegVal == MBOX_DBGCMD_SE_PROCESS_CONSUME_NEED)
                    return TRUE;
                else
                    return FALSE;
            }
            break;

        case Audio_Comm_infoType_setBypassSPDIF_PAPB_chk:       //HAL_MAD_SetDtsBypassFlag
            break;

        case Audio_Comm_infoType_MMA_init:
            HAL_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_GAME);
            HAL_SOUND_SetMixModeVolume(E_AUDIO_INFO_GAME_IN, GAME1_VOL, 0x12, 0x00);     //DEC1 Vol
            HAL_SOUND_SetMixModeVolume(E_AUDIO_INFO_GAME_IN, GAME2_VOL, 0x12, 0x00);     //DEC2 Vol
            HAL_SOUND_SetMixModeVolume(E_AUDIO_INFO_GAME_IN, PCM_VOL,   0x12, 0x00);     //DEC3 Vol
            break;

        case Audio_Comm_infoType_MMA_finish:
            HAL_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_DTV);
            break;

        //[LM14A]
        //param1: 0:off, 1:on,
        //param2: 0:HW DMA0, 1:HW DMA1
        case Audio_Comm_infoType_DMAReader_Command:
        {
            switch(param2)
            {
                case 0: //HW DMA0
                {
                    if (param1)
                    {
                        HAL_AUDIO_WriteMaskReg(0x2B80, 0x0001, 0x0001);
                        AUDIO_DELAY1MS(2);
                        HAL_AUDIO_WriteMaskReg(0x2B80, 0x0100, 0x0100);
                    }
                    else
                    {
                        HAL_AUDIO_WriteMaskReg(0x2B80, 0x0001, 0x0000);
                        AUDIO_DELAY1MS(2);
                        HAL_AUDIO_WriteMaskReg(0x2B80, 0x0100, 0x0000);
                        AUDIO_DELAY1MS(2);
                        HAL_AUDIO_WriteMaskReg(0x2B80, 0x0040, 0x0040); //read_init, clear dma reader data & pointer
                        AUDIO_DELAY1MS(1);
                        HAL_AUDIO_WriteMaskReg(0x2B80, 0x0040, 0x0000);
                    }
                }
                break;

                case 1: //HW DMA1
                {
                    //@Todo: Daniel will help to compelte~
                }
                break;

                default:
                    break;
            }

        }
        break;

        case Audio_Comm_infoType_DMAReader_Pause:
            if (param1)
            {
                HAL_AUDIO_WriteMaskReg(0x2B80, 0x0100, 0x0000);
            }
            else
            {
                HAL_AUDIO_WriteMaskReg(0x2B80, 0x0100, 0x0100);
            }
            break;

        case Audio_Comm_infoType_SWDMAReader_Pause:
            if (param1 == 0) // path 0
            {
                HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x0002, (!param2) << 1);        // bit 1 --> start
            }
            else if (param1 == 1) // path 1
            {
                HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x0002, (!param2) << 1);        // bit 1 --> start
            }
            break;

        case Audio_Comm_infoType_PreNotMM_Flag:
            if (param1 == 0)
            {
                g_AudioVars2->g_bPreNotMM_Flag = FALSE;
            }
            else if (param1 == 1)
            {
                g_AudioVars2->g_bPreNotMM_Flag = TRUE;
            }
            break;

        case Audio_Comm_infoType_CompressBin_LoadCode:
            g_AudioVars2->CompressInfo = param1;
            break;

        case Audio_Comm_infoType_CompressBin_DDRAddress:
            g_AudioVars2->g_DSPDDRBinBaseAddress[param1] = param2;
            g_DSPDDRBinBaseAddress[param1] = param2;
            break;

        case Audio_Comm_infoType_SetSCMS:
        {
            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            retTx2= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

            if (retTx1 == FALSE || retTx2 == FALSE)
            {
                HALMAD_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            if(param1)
            {
                stDigitalChannelStatusTx1.stCopyRight.CBit = 1;
                stDigitalChannelStatusTx2.stCopyRight.CBit = 1;
            }
            else
            {
                stDigitalChannelStatusTx1.stCopyRight.CBit = 0;
                stDigitalChannelStatusTx2.stCopyRight.CBit = 0;
            }

            if(param2)
            {
                stDigitalChannelStatusTx1.stCopyRight.LBit = 1;
                stDigitalChannelStatusTx2.stCopyRight.LBit = 1;
            }
            else
            {
                stDigitalChannelStatusTx1.stCopyRight.LBit = 0;
                stDigitalChannelStatusTx2.stCopyRight.LBit = 0;
            }

            HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
       }
            break;

        case Audio_Comm_infoType_ADC_InputGain:
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, (AUD_ADC_GAIN_TYPE)param1<<5);
            break;

        case Audio_Comm_infoType_ADC1_InputGain:
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0x1C, (AUD_ADC_GAIN_TYPE)param1<<2);
            break;

        case Audio_Comm_infoType_KTV_SetType:
            switch((AUDIO_KTV_MODEL)param1)
            {
                case AUD_KTV_MODEL1:     // iSing99
                    HAL_AUDIO_WriteMaskByte(0x2D47 , 0x7F, 0x00);  // No this model in T12
                    break;
                case AUD_KTV_MODEL2:     // Multak
                    HAL_AUDIO_WriteMaskByte(0x2D47 , 0x7F, 0x21);
                    break;
                case AUD_KTV_MODEL3:     // �p��
                    HAL_AUDIO_WriteMaskByte(0x2D47 , 0x7F, 0x02);
                    break;
                case AUD_KTV_MODEL4:     // Konka
                    HAL_AUDIO_WriteMaskByte(0x2D47 , 0x7F, 0x14);
                    break;
                default:
                    break;
            }
            break;

        case Audio_Comm_infoType_syncSTC_in_mmTs:
            if ( param1 )
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, ADEC1, 0, 0);
            break;

        case Audio_Comm_infoType_PUSI_disable_Info:
            if(param1)
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_PUSI_DISABLE, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_PUSI_DISABLE, ADEC1, 0, 0);
            break;

        case Audio_Comm_infoType_Parser_Spec:
            if(param1)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_SPEC, ADEC1, 1, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_PUSI_DISABLE, ADEC1, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_SPEC, ADEC1, 0, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_PUSI_DISABLE, ADEC1, 0, 0);
            }
            break;

        case Audio_Comm_infoType_setNR_Threshold:
            HAL_AUDIO_WriteByte(0x2D32, (MS_U8) param1);
            break;

        case Audio_Comm_infoType_Set_CallBackFunc:
            if ( g_FuncPrt_Hal_SetAudioParam2 == NULL )
            {
                g_FuncPrt_Hal_SetAudioParam2 = HAL_MAD_SetAudioParam2;
                HALMAD_ERROR ("%s: g_FuncPrt_Hal_SetAudioParam2 hook OK !!\r\n", __FUNCTION__);
            }
            else
            {
                HALMAD_ERROR ("%s: g_FuncPrt_Hal_SetAudioParam2 already register !!\r\n", __FUNCTION__);
            }
            if ( g_FuncPrt_Hal_GetAudioInfo2 == NULL )
            {
                g_FuncPrt_Hal_GetAudioInfo2 = HAL_MAD_GetAudioInfo2;
                HALMAD_ERROR ("%s: g_FuncPrt_Hal_GetAudioInfo2 hook OK !!\r\n", __FUNCTION__);
            }
            else
            {
                HALMAD_ERROR ("%s: g_FuncPrt_Hal_GetAudioInfo2 already register !!\r\n", __FUNCTION__);
            }
            break;

        case Audio_Comm_infoType_SetSoundBarPowerOnOff: // LG SPDIF POWER ON/OFF

            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            if (param2) // Power on
            {
                stDigitalChannelStatusTx1.u8PowerOnOff1 = 0x00;
                stDigitalChannelStatusTx1.u8PowerOnOff2 = 0x00 ;
                HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            }
            else            // Power off
            {
                stDigitalChannelStatusTx1.u8PowerOnOff1 = 0x08;
                stDigitalChannelStatusTx1.u8PowerOnOff2 = 0x50 ;
                HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            }
            break;

        case Audio_Comm_infoType_SetSoundBarMuteOnOff: // LG OPT MUTE ON/OFF

            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            if ((param2 == 1) ||(stDigitalChannelStatusTx1.u8SoundBarVolumeMuteOnOff == 0x01)) // Mute on
            {
                stDigitalChannelStatusTx1.u8MuteOnOff = 0x10;
            }
            else            // Mute off
            {
                stDigitalChannelStatusTx1.u8MuteOnOff = 0x00;
            }
            HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            break;

         case Audio_Comm_infoType_SetSoundBarIDData:       // LG SetSoundBarIDData

            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

                stDigitalChannelStatusTx1.u8SoundBarID1 = HAL_MAD_GetBitReverse((param2) & 0x0F);
                stDigitalChannelStatusTx1.u8SoundBarID2 = HAL_MAD_GetBitReverse((param2 >> 4) & 0x0F);
                stDigitalChannelStatusTx1.u8SoundBarID3 = HAL_MAD_GetBitReverse((param2 >> 8) & 0x0F);
                stDigitalChannelStatusTx1.u8SoundBarID4 = HAL_MAD_GetBitReverse((param2 >> 12) & 0x0F);
                stDigitalChannelStatusTx1.u8SoundBarID5 = HAL_MAD_GetBitReverse((param2 >> 16) & 0x0F);
                stDigitalChannelStatusTx1.u8SoundBarID6 = HAL_MAD_GetBitReverse((param2 >> 20) & 0x0F);

                stDigitalChannelStatusTx1.u8SoundBarVolume1 = HAL_MAD_GetBitReverse(param1 & 0x0F);
                stDigitalChannelStatusTx1.u8SoundBarVolume2 = HAL_MAD_GetBitReverse((param1 >> 4) & 0x0F);
                stDigitalChannelStatusTx1.u8SoundBarVolumeMuteOnOff = (stDigitalChannelStatusTx1.u8SoundBarVolume2) & 0x01;

                if (stDigitalChannelStatusTx1.u8SoundBarVolumeMuteOnOff == 0x01)
                {
                    stDigitalChannelStatusTx1.u8MuteOnOff = 0x10; // mute on
                }
                else
                {
                    stDigitalChannelStatusTx1.u8MuteOnOff = 0x00; // mute off
                }

                HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            break;

         case Audio_Comm_infoType_SetSoundBarWooferLevel:       // LG OPT WooferLevel

            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            stDigitalChannelStatusTx1.u8SoundBarWooferLevel = HAL_MAD_GetBitReverse(param1 & 0x0F);

            HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            break;

        case Audio_Comm_infoType_SetSoundBarAutoVolume: // LG OPT AutoVolume

            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            if (param2 == 1) // AutoVolume on
            {
                stDigitalChannelStatusTx1.u8SoundBarAutoVolume = 0x80;
            }
            else            // AutoVolume off
            {
                stDigitalChannelStatusTx1.u8SoundBarAutoVolume = 0x00;
            }
            HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            break;

        /********************************************************************************
        *       DECODER 1
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC1_Set_play_smpRate:
        {
            MS_U16 play_cmd;
            MS_U32 synth_H;
            MS_U64 SYNTH_CALC_NUMERATOR = 216000000;

            play_cmd = HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD) & 0xF;
            if (play_cmd != 0x02)
            {
                printf("[%s : Audio_Comm_infoType_ADEC1_Set_play_smpRate]: audio play cmd is not Ts file Play !!\n", __FUNCTION__);
                break;
            }

            if (param1)
            {
                if ((param2 > 50000) ||(param2<6000))
                {
                    printf("[%s : Audio_Comm_infoType_ADEC1_Set_play_smpRate]: play rate [%ld] is invalid !!\n", __FUNCTION__, param2);
                    break;
                }

                synth_H = (MS_U32)do_div(SYNTH_CALC_NUMERATOR, param2);
                synth_H <<= 16;
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_EXT_SMPRATE_CTRL, ADEC1, 1, synth_H);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_EXT_SMPRATE_CTRL, ADEC1, 0, 0x11940000);
            }
        }
        break;

        case Audio_Comm_infoType_ADEC1_soundmod:
            {
                MS_U8 value = 0;

                switch((AUD_Dolby_SOUNDMODE)param1)
                {
                    case AUD_Dolby_SOUNDMODE_LR:     value = 0x0;  break;
                    case AUD_Dolby_SOUNDMODE_LL:     value = 0x1;  break;
                    case AUD_Dolby_SOUNDMODE_RR:     value = 0x2;  break;
                    default:                         value = 0x0;  break;
                }

                //NOT_DEFINE : to do
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SOUND_MODE, ADEC1, value, 0);
            }
            break;

        case Audio_Comm_infoType_SoundMode:                     //HAL_MAD_SetSoundMode
            HAL_AUDIO_SeWriteMaskByte(REG_MB_MODE_SELECT, 0x03, (AU_DVB_MODE) param1);
            break;

        case Audio_Comm_infoType_ADEC1_reqDataSize:
        case Audio_Comm_infoType_MMFileSize:                    //HAL_MAD_SetMadFileSize
            {
                MS_U32   file_size_value = 0x800;

                switch (param1)
                {
                    case FILE_SIZE_1KB:     file_size_value = 0x00400;        break;
                    case FILE_SIZE_2KB:     file_size_value = 0x00800;        break;
                    case FILE_SIZE_4KB:     file_size_value = 0x01000;        break;
                    case FILE_SIZE_8KB:     file_size_value = 0x02000;        break;
                    case FILE_SIZE_16KB:    file_size_value = 0x04000;        break;
                    case FILE_SIZE_32KB:    file_size_value = 0x08000;        break;
                    case FILE_SIZE_64KB:    file_size_value = 0x10000;        break;
                    default:                                                 break;
                }
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, ADEC1, file_size_value, 0);
            }
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_informDataRdy:
        case Audio_Comm_infoType_DEC1_MMTag:                    //HAL_MAD_SetMMIntTag
            {
               MS_U32 es_wrPtr = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0);
               MS_U32 es_bufSz = ES1_DRAM_SIZE;
               MS_U32 es_reqsz = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC1);
                es_wrPtr += es_reqsz;

                if ( es_wrPtr >= es_bufSz )
                    es_wrPtr -= es_bufSz;

                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, es_wrPtr, 0);
            }
            break;

        case Audio_Comm_infoType_ADEC1_FF2X:
        case Audio_Comm_infoType_MM_FFx2:                       //HAL_MAD_SetMMFFx2Mode
            if ( param1 )
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FF2X, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FF2X, ADEC1, 0, 0);
            break;

        case Audio_Comm_infoType_ADEC1_mute:
            if ( param1 )
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DECODE_MUTE, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DECODE_MUTE, ADEC1, 0, 0);
            break;

        case Audio_Comm_infoType_ADEC1_DPFG_Flag:
            if ( param1 )
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DPGA_FLAG, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DPGA_FLAG, ADEC1, 0, 0);
            break;

        case Audio_Comm_infoType_ADEC1_play_wo_output:
            if (param1)
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DEC_CTRL, 0x80, 0x80);
            else
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DEC_CTRL, 0x80, 0x00);
            break;

        case Audio_Comm_infoType_ADEC1_playControl:
            switch((MMA_AUDIO_CONTROL)param1)
            {
                case MMA_STOP:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_AVSYNC);
                    g_AudioVars2->g_bDecPlayFileFlag = FALSE;
                    break;

                case MMA_PLAY:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY);
                    break;

                case MMA_PLAY_FILETSP:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILETSP);
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                    break;

                case MMA_RESYNC:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_RESYNC);
                    break;

                case MMA_PLAY_FILE:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILE);
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                    break;

                case MMA_BROWSE:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STARTBROWSE);
                    break;

                case MMA_PAUSE:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PAUSE);
                    break;

                case MMA_PLAY_FRAME:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME);
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                    break;

                default:
                    break;
            }
            break;

        case Audio_Comm_infoType_ADEC1_setDecParam:
            //NOT_DEFINE : remove
            break;

        case Audio_Comm_infoType_ADEC1_UNI_PCMOutCnt:
            uniPlay_ackCnt[0]++;
            HALMAD_PRINT("\r\nTrigger decInt[%d] %d\r\n", 0, uniPlay_ackCnt[0]);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_PCM_SET_OUTPUT_CNT, ADEC1, param1, 0);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_PCMInput_Addr:   //assume UNI get data from PCM1.
            HAL_AUDIO_WriteReg(REG_DEC1_UNI_PCM3_WPTR, (MS_U16)((param1>>BYTES_IN_MIU_LINE_LOG2) - OFFSET_PCM1_DRAM_ADDR));
            break;

        case Audio_Comm_infoType_DEC1_setBufferProcess:
            // Need Decoder Add delay support in DEC1-DSP
            break;

        case Audio_Comm_infoType_33Bit_PTS:
        case Audio_Comm_infoType_ADEC1_33bit_PTS:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_REPORT_MM_TS_PTS_WITH_MSB, ADEC1, param1, 0);
            break;

        /********************************************************************************
        *       DECODER 2
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC2_soundmod:
        case Audio_Comm_infoType_ADEC2_reqDataSize:
        case Audio_Comm_infoType_ADEC2_FF2X:
        case Audio_Comm_infoType_ADEC2_mute:
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_informDataRdy:
        case Audio_Comm_infoType_ADEC2_syncSTC_in_mmTs:
            if ( param1 )
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, ADEC2, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, ADEC2, 0, 0);
            break;

        case Audio_Comm_infoType_ADEC2_DPFG_Flag:
            if ( param1 )
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DPGA_FLAG, ADEC2, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DPGA_FLAG, ADEC2, 0, 0);
            break;

        case Audio_Comm_infoType_DEC2_MMTag:                    //HAL_MAD2_SetMMIntTag
            {
                MS_U32 es_wrPtr, es_bufSz, es_reqsz;
                es_wrPtr = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0);
                es_bufSz = ES2_DRAM_SIZE;
                es_reqsz = param2;
                es_wrPtr += es_reqsz;

                if ( es_wrPtr >= es_bufSz )
                   es_wrPtr -= es_bufSz;
                 HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, es_wrPtr, 0);
            }
            break;

        case Audio_Comm_infoType_ADEC2_playControl:
            {
                switch((MMA_AUDIO_CONTROL)param1)
                {
                    case MMA_STOP:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STOP);
                        g_AudioVars2->g_bSePlayFileFlag = FALSE;
                        break;

                    case MMA_PLAY:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAY);
                        break;

                    case MMA_PLAY_FILETSP:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAYFILETSP);
                        break;

                    case MMA_RESYNC:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_RESYNC);
                        break;

                    case MMA_PLAY_FILE:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAYFILE);
                        break;

                    case MMA_BROWSE:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STARTBROWSE);
                        break;

                    case MMA_PAUSE:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PAUSE);
                        break;

                    case MMA_PLAY_FRAME:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAYFRAME);
                        break;

                    default:
                        break;
                }
            }
            break;

        case Audio_Comm_infoType_ADEC2_setDecParam:
            //NOT_DEFINE : remove
            break;

        case Audio_Comm_infoType_ADEC2_33bit_PTS:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_REPORT_MM_TS_PTS_WITH_MSB, ADEC2, param1, 0);
            break;

        /********************************************************************************
        *       OTHERS
        ********************************************************************************/
        case Audio_Comm_infoType_setSpdif_BufferProcess:
        case Audio_Comm_infoType_setSpdifDelay: // set spdif dealy time
            HAL_SOUND_SetSpdifAudioDelay((MS_U16)param1);
            break;

        case Audio_Comm_infoType_ReadByte: // For APP to read  register value directly.
            g_ReadTargetAddr=param1;
            break;

        case Audio_Comm_infoType_WriteByte: // For APP to write register value directly.
            HAL_AUDIO_AbsWriteMaskByte(param1,0xFF,(MS_U8)(param2&0xFF));
            break;

        case Audio_Comm_infoType_PTS_info:
            HAL_AUDIO_PTS_info(param1);
            break;

        case Audio_Comm_infoType_RTSP_Mem:
            {
                AUDIO_RTSP_INFO *ptmp = (AUDIO_RTSP_INFO *)param1;
                ptmp->Mem_addr = RTSP_MEM_ADDR;
                ptmp->Mem_size= RTSP_MEM_SIZE;
            }
            break;

        case Audio_Comm_infoType_Dump_Dsp_Info:
            {
                 g_AudioDumpInfoCtrl = param1;

                if (param2 > 50 )
                    g_audioDumpInfo_interval = param2;
            }
            break;

        case Audio_Comm_infoType_Set_Video_STCPTS_DIFF:
            //NOT_DEFINE : to do
            HAL_MAD_Write_DSP_sram(DSP1PmAddr_video_TD, param1, DSP_MEM_TYPE_PM);
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Protect:
            //NOT_DEFINE : to do
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_hdmi_unstable_protect, param1, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_Vorbis_header_size:
            if ( param1 )
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_VORBIS_HEADER_SIZE, ADEC1, param2, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_VORBIS_HEADER_SIZE, ADEC1, param2, 0);
            }
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Threshold:
            //NOT_DEFINE : to do
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_hdmi_unstable_threshold, param1, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_Set_Parser:

            switch(param1)
            {
                case 0 :   // MainWindow ADEC1 :  AFIFO_A->ES1  ; AFIFO_B->ES2
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0030, 0x0000); // AFIFO_A-> ES1
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0180, 0x0000); // AFIFO_B-> ES2
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0C00, 0x0000); // AFIFO_C-> ES3(Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x6000, 0x0000); // AFIFO_D-> ES4(Dummy)
                    break;

                case 1 :   // MainWindow ADEC2 :  AFIFO_A->ES3  ; AFIFO_B->ES4
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0030, 0x0020); // AFIFO_C-> ES1 (Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0180, 0x0100); // AFIFO_D-> ES2 (Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0C00, 0x0800); // AFIFO_A-> ES3
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x6000, 0x4000); // AFIFO_B-> ES4
                    break;

                case 2 :  // SubWindow ADEC1 :  AFIFO_B->ES1  ;
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0030, 0x0010); // AFIFO_B-> ES1
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0180, 0x0080); // AFIFO_C-> ES2(Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0C00, 0x0800); // AFIFO_A-> ES3
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x6000, 0x0000); // AFIFO_D-> ES4(Dummy)
                    break;

                case 3 :  // SubWindow ADEC3 :  AFIFO_B->ES3  ;
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0030, 0x0000); // AFIFO_A-> ES1
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0180, 0x0080); // AFIFO_C-> ES2(Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0C00, 0x0C00); // AFIFO_B-> ES3
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x6000, 0x0000); // AFIFO_D-> ES4(Dummy)
                    break;

                default :
                    break;
            };

            break;

        case Audio_Comm_infoType_mainAudio_setBufferProcess:
            HAL_SOUND_SetCH1AudioDelay(param1);
            break;

        case Audio_Comm_infoType_STC_Select:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_STC_SELECT, ADEC1, param1, 0);
            break;

#ifndef MSOS_TYPE_NUTTX
        case Audio_Comm_infoType_Set_TEE_Capibility:
            HALAUDIO_CHECK_TEE_INFO_SHM_INIT;
            switch ((Audio_TEE_Capability)param1)
            {
                case Audio_TEE_Setup:
                    g_bAudioTeeEnabled = TRUE;       audio_tee_enabled = TRUE;
                    if (audio_tee_mbx_initialized == FALSE)
                    {
                        if (HAL_AUDIO_RegisterMBX() == TRUE)
                        {
                            g_bAudioTeeMbxInitialized = TRUE;   audio_tee_mbx_initialized = TRUE;
                        }
                    }
                    break;

                case Audio_TEE_Setup_and_ALL_DSP_Secure_Accessed:
                    g_bAudioTeeEnabled = TRUE;        audio_tee_enabled = TRUE;
                    if (audio_tee_mbx_initialized == FALSE)
                    {
                        if (HAL_AUDIO_RegisterMBX() == TRUE)
                        {
                            g_bAudioTeeMbxInitialized = TRUE;             audio_tee_mbx_initialized = TRUE;
                            g_bDecDspSecureTeeAccessed = TRUE;        dec_dsp_secure_tee_accessed = TRUE;
                            g_bSndDspSecureTeeAccessed = TRUE;        snd_dsp_secure_tee_accessed = TRUE;
                        }
                    }
                    break;

                case Audio_TEE_Set_DSP_Secure_Accessed:
                    if ((audio_tee_enabled ==  TRUE) && (audio_tee_mbx_initialized == TRUE))
                    {
                        if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_DEC)
                        {
                            g_bDecDspSecureTeeAccessed = TRUE;            dec_dsp_secure_tee_accessed = TRUE;
                        }
                        else if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_SND)
                        {
                            g_bSndDspSecureTeeAccessed = TRUE;           snd_dsp_secure_tee_accessed = TRUE;
                        }
                        else if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_ALL)
                        {
                            g_bDecDspSecureTeeAccessed = TRUE;            dec_dsp_secure_tee_accessed = TRUE;
                            g_bSndDspSecureTeeAccessed = TRUE;            snd_dsp_secure_tee_accessed = TRUE;
                        }
                    }
                    break;

                case Audio_TEE_Set_DSP_Normal_Accessed:
                    if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_DEC)
                    {
                        g_bDecDspSecureTeeAccessed = FALSE;            dec_dsp_secure_tee_accessed = FALSE;
                    }
                    else if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_SND)
                    {
                        g_bSndDspSecureTeeAccessed = FALSE;           snd_dsp_secure_tee_accessed = FALSE;
                    }
                    else if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_ALL)
                    {
                        g_bDecDspSecureTeeAccessed = FALSE;           dec_dsp_secure_tee_accessed = FALSE;
                        g_bSndDspSecureTeeAccessed = FALSE;           snd_dsp_secure_tee_accessed = FALSE;
                    }
                    break;

                default:
                    break;
            }
            break;
#endif

        case Audio_Comm_infoType_SPDIF_SetChannelStatus:
            HAL_AUDIO_SetChannelStatus(param1, param2);
            break;

        case Audio_Comm_infoType_Multi_Channel:
        {
            if ((param1 == TRUE) || (param1 == FALSE))
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MULTI_CHANNEL, ADEC1, param1, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MULTI_CHANNEL, ADEC2, param1, 0);
            }
            else
            {
                HALMAD_ERROR("%s: Error! Invalid value(%lu) to enable/disable Multi-Channel\n", __FUNCTION__, param1);
            }

            break;
        }

        /* not supported cmd */
        default:
            HALMAD_ERROR("%s: infoType[0x%x,0x%lx,0x%lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetCommInfo()
/// @brief \b Function \b Description: This routine info of audio decoders
/// @param <IN>        \b Audio_COMM_infoType    : request info type of audio decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b long long :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
long long HAL_MAD_GetCommInfo( Audio_COMM_infoType infoType )
{
    long long result = 0;

    HALMAD_CHECK_SHM_INIT;
    MS_BOOL retTx1;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1;

    switch(infoType)
    {
        /********************************************************************************
        *       Common
        ********************************************************************************/
        case Audio_Comm_infoType_getHDMI_CopyRight_C_Bit:
            result = _AU_AbsReadByte(0x1027B4) & 0x20;
            break;

        case Audio_Comm_infoType_getHDMI_CopyRight_L_Bit:
            result = _AU_AbsReadByte(0x1027B5) & 0x01;
            break;

        case Audio_Comm_infoType_Get_UNI_ES_MEMCNT:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, ADEC1);
            break;

        case Audio_Comm_infoType_Get_UNI_ES_Base:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES1_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_Get_UNI_ES_Size:
            result = ES1_DRAM_SIZE ;
            break;

        case Audio_Comm_infoType_Get_UNI_PCM_Base:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_ADDR, ADEC1);
            break;

        case Audio_Comm_infoType_Get_UNI_PCM_Size:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_SIZE, ADEC1);
            break;

        case Audio_Comm_infoType_Get_AccumEsCnt:
//            result = HAL_MAD_Read_DSP_sram(REG_PTS_AccumEsCnt, DSP_MEM_TYPE_DM);
            result = 0;
            break;

        case Audio_Comm_infoType_Is_Decoder_In_R2:
            result = 1;
            break;

        case Audio_Comm_infoType_SWDMAReader_Pause:
        {
            MS_U8 pause1 = 0;
            MS_U8 pause2 = 0;
            pause1 = HAL_AUDIO_ReadByte(M2S_MBOX_SW_DMA_READER_DDR_Ctrl+1) & 0x80;
            pause2 = HAL_AUDIO_ReadByte(M2S_MBOX_SW_DMA_READER_DDR_Ctrl+1) & 0x80;
            if( pause1 == 1 && pause2 == 1)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
        }
            break;

        case Audio_Comm_infoType_PreNotMM_Flag:
            result = g_AudioVars2->g_bPreNotMM_Flag;
            break;
        /********************************************************************************
        *       DECODER 1
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC1_capability:
        {
            MMA_AudioType audioType = Audio_DEC_NULL;
            audioType = _GetAudioCaps(AU_DEC_ID1);

        #if defined (CONFIG_UTOPIA_ANDROID_L)
            result = (MS_U64)audioType;
        #else
            result = (long long)audioType;
        #endif

            break;
        }

        case Audio_Comm_infoType_ADEC1_currAudDecStatus:
        case Audio_Comm_infoType_DecStatus:                     //no change
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_AudioSyncStatus:
            if(HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_CMD)&0x80)
                result = 0;  // Free-run
            else
                result = 1;  // AV-sync
            break;

        case Audio_Comm_infoType_ADEC1_sampleRate:
        case Audio_Comm_infoType_SampleRate:                    //HAL_MAD_GetMadSamplingRate
        {
            MS_U32 r2_smpRate = 0xFFFF;
            r2_smpRate = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
            result = AUDIO_FS_NONE;

            switch(r2_smpRate)
            {
                case 8000:   result = AUDIO_FS_8KHZ;   break;
                case 11025:  result = AUDIO_FS_11KHZ;  break;
                case 12000:  result = AUDIO_FS_12KHZ;  break;
                case 16000:  result = AUDIO_FS_16KHZ;  break;
                case 22050:  result = AUDIO_FS_22KHZ;  break;
                case 24000:  result = AUDIO_FS_24KHZ;  break;
                case 32000:  result = AUDIO_FS_32KHZ;  break;
                case 44100:  result = AUDIO_FS_44KHZ;  break;
                case 48000:  result = AUDIO_FS_48KHZ;  break;
                case 65535:  result = AUDIO_FS_NONE;   break;
                case 64000:  result = AUDIO_FS_64KHZ;  break;
                case 88200:  result = AUDIO_FS_88KHZ;  break;
                case 96000:  result = AUDIO_FS_96KHZ;  break;
                case 176000: //result = AUDIO_FS_176KHZ; break;
                case 192000: //result = AUDIO_FS_192KHZ; break;
                default:     result = AUDIO_FS_NONE;   break;
            }
        }
        break;

        case Audio_Comm_infoType_ADEC1_bitRate:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_acmod:
        case Audio_Comm_infoType_ChannelMode:                   //HAL_MAD_GetMadChannelMode
        {
            MS_U32 ac_mode = 0xFF;
            MS_U32 channel_mapping = 0x0;
            ac_mode = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MODE, ADEC1);
            channel_mapping = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC1);

            switch(ac_mode)
            {
                case adec_ch_mono:
                    result =  AUD_CH_MODE_MONO;
                    break;

                case adec_ch_joint_stereo:
                    result =  AUD_CH_MODE_JOINT_STEREO;
                    break;

                case adec_ch_stereo:
                    result =  AUD_CH_MODE_STEREO;
                    break;

                case adec_ch_dual_mono:
                    result =  AUD_CH_MODE_DUAL_MONO;
                    break;

                case adec_ch_multi:
                    {
                        MS_U32 channel_mapping_mask_C_L_R, channel_mapping_mask_Ls_Rs =0x00;
                        channel_mapping_mask_Ls_Rs = (channel_mapping&(ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS));
                        channel_mapping_mask_C_L_R = (channel_mapping&(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R));

                        if(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result =  AUD_CH_MODE_3_2_MODE;
                            else if(channel_mapping_mask_Ls_Rs == ACMOD_CH_MASK_NULL)
                               result =  AUD_CH_MODE_3_0_MODE;
                            else
                               result =  AUD_CH_MODE_3_1_MODE;
                        }
                        else if((channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L))||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                                ||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_R)))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result =  AUD_CH_MODE_2_2_MODE;
                            else
                               result =  AUD_CH_MODE_2_1_MODE;
                        }
                        else //other multi-channel case, set it to AUD_CH_MODE_3_2_MODE
                        {
                            result =  AUD_CH_MODE_3_2_MODE;
                        }
                    }
                    break;

                 case adec_ch_unknown:
                 default:
                    result =  AUD_CH_MODE_NONE;
                    break;
            }
        }
            break;

        case Audio_Comm_infoType_ADEC1_soundmod:
        case Audio_Comm_infoType_SoundMode:
            result = HAL_AUDIO_ReadReg(REG_MB_MODE_SELECT)&0x3;
            break;

        case Audio_Comm_infoType_ADEC1_okFrmCnt:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_errFrmCnt:
        case Audio_Comm_infoType_DecodeErrorCnt:                //HAL_MAD_Get_DecodeErrorCnt
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_1ms_timeStamp:
        case Audio_Comm_infoType_1ms_PTS:                       //no change
            {
                MS_U32  pts;
                pts = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC1);
                result = pts/45;
            }
            break;

        case Audio_Comm_infoType_ADEC1_33bit_PTS:
        case Audio_Comm_infoType_33Bit_PTS:                     //HAL_MAD_GetMadPts
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC1);
            result <<= 1;
            break;

        case Audio_Comm_infoType_ADEC1_getlatest_PTS:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS, ADEC1);
            result <<= 1;
            break;

        case Audio_Comm_infoType_ADEC1_33bit_STCPTS_DIFF:
        case Audio_Comm_infoType_33Bit_STCPTS_DIFF:             //HAL_MAD_GetMadPtsStcDelta
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TD, ADEC1);

            //sign ext
            result <<= 32;
            result >>= 31;
            break;

        case Audio_Comm_infoType_ADEC1_Get_PTS_latency:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS_LATENCY, ADEC1);

            //sign ext
            result <<= 32;
            result >>= 31;
            break;

        case Audio_Comm_infoType_ADEC1_Get_PCM_bufferEmpty:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_EMPTY_CNT, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_reqFlag:
        {
            if (HAL_AUDIO_GetPlayFileFlag(DSP_DEC))
                result = 1;
            else
                result = 0;
        }
            break;

        case Audio_Comm_infoType_DEC1_BufferAddr:               //HAL_MAD_GetBufferAddr
            result = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0) + OFFSET_ES1_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_reqAddr:
            result = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0) + OFFSET_ES1_DRAM_ADDR + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_reqSize:
        case Audio_Comm_infoType_DEC1_BufferSize:               //HAL_MAD_GetBufferSize
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_currLevel:
        case Audio_Comm_infoType_DEC1_ESBufferSize:             //HAL_MAD_GetESBufferSize
             result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_pcmBuf_currLevel:
        case Audio_Comm_infoType_DEC1_MMResidualPCM:            //HAL_MAD_GetMMResidualPCM
        case Audio_Comm_infoType_DEC1_PCMBufferSize:            //HAL_MAD_GetPCMBufferSize
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_pcm_wptr:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM1_DRAM_ADDR + HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_WR_PTR, ADEC1);
            break;

        case Audio_Comm_infoType_DMAReader_BufferSize:
           result = ((DSP2_DMA_READER_DRAM_SIZE+1)*BYTES_IN_MIU_LINE);
           break;

        case Audio_Comm_infoType_Get_SE_Input_Main_BufferBase:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_SE_MAIN_IN_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_Get_SE_Input_Main_BufferSize:
            result = SE_MAIN_IN_DRAM_SIZE;
            break;

        case Audio_Comm_infoType_Get_SE_Output_Main_BufferBase:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_SE_MAIN_OUT_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_Get_SE_Output_Main_BufferSize:
            result = SE_MAIN_OUT_DRAM_SIZE;
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_FRAME_SMP_UNIT:
            result = u32SeProcessFrameSmpUnit;
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_FETCH_CHANNELS:
            result = SE_PROCESS_FETCH_CHANNELS;
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_STORE_CHANNELS:
            result = SE_PROCESS_STORE_CHANNELS;
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_FETCH_FRAME_BYTE_SIZE:
            result = SE_PROCESS_FETCH_FRAME_LINE_SIZE * BYTES_IN_MIU_LINE * (u32SeProcessFrameSmpUnit/SE_PROCESS_FRAME_SMP_UNIT);
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_STORE_FRAME_BYTE_SIZE:
            result = SE_PROCESS_STORE_FRAME_LINE_SIZE * BYTES_IN_MIU_LINE * (u32SeProcessFrameSmpUnit/SE_PROCESS_FRAME_SMP_UNIT);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_pcm_decSize:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_SIZE, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_OutPCMLevel:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_getDecInfo1:
        case Audio_Comm_infoType_ADEC1_getDecInfo2:
        case Audio_Comm_infoType_ADEC1_getDecInfo3:
        case Audio_Comm_infoType_ADEC1_getDecInfo4:
        case Audio_Comm_infoType_ADEC1_getDecInfo5:
        case Audio_Comm_infoType_ADEC1_getDecInfo6:
        case Audio_Comm_infoType_ADEC1_getDecInfo7:
        case Audio_Comm_infoType_ADEC1_getDecInfo8:
            {
                // not used, remove
            }
            break;

        case Audio_Comm_infoType_DMAReader_BufferLevel:
            HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0010);
            result = ((MS_U32)HAL_AUDIO_ReadReg(0x2B8C)<<3);      // 0x2B8C is line size
            HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0000);
            break;

        case Audio_Comm_infoType_syncSTC_in_mmTs:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_MMTsSyncFlag, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_getDecChannelInfo:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_audio_unsupported:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNSUPPORT_TYPE, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_avSyncOffset:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_OFFSET, ADEC1) & 0x00FFFFFF;
            break;

        /*******************************************************************************
        *       DECODER 2
        *******************************************************************************/
        case Audio_Comm_infoType_ADEC2_capability:
            result = Audio_DEC_MP3;
            break;

        case Audio_Comm_infoType_ADEC2_currAudDecStatus:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_sampleRate:
            {
                MS_U32 r2_smpRate = 0xFFFF;
                r2_smpRate = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC2);
                result = AUDIO_FS_NONE;

                switch(r2_smpRate)
                {
                    case 8000:   result = AUDIO_FS_8KHZ;   break;
                    case 11025:  result = AUDIO_FS_11KHZ;  break;
                    case 12000:  result = AUDIO_FS_12KHZ;  break;
                    case 16000:  result = AUDIO_FS_16KHZ;  break;
                    case 22050:  result = AUDIO_FS_22KHZ;  break;
                    case 24000:  result = AUDIO_FS_24KHZ;  break;
                    case 32000:  result = AUDIO_FS_32KHZ;  break;
                    case 44100:  result = AUDIO_FS_44KHZ;  break;
                    case 48000:  result = AUDIO_FS_48KHZ;  break;
                    case 65535:  result = AUDIO_FS_NONE;   break;
                    case 64000:  result = AUDIO_FS_64KHZ;  break;
                    case 88200:  result = AUDIO_FS_88KHZ;  break;
                    case 96000:  result = AUDIO_FS_96KHZ;  break;
                    case 176000: //result = AUDIO_FS_176KHZ; break;
                    case 192000: //result = AUDIO_FS_192KHZ; break;
                    default:     result = AUDIO_FS_NONE;   break;
                }
            }
            break;

        case Audio_Comm_infoType_ADEC2_bitRate:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_acmod:
        {
            MS_U32 ac_mode = 0xFF;
            MS_U32 channel_mapping = 0x0;
            ac_mode = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MODE, ADEC2);
            channel_mapping = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC2);

            switch(ac_mode)
            {
                case adec_ch_mono:
                    result =  AUD_CH_MODE_MONO;
                    break;

                case adec_ch_joint_stereo:
                    result =  AUD_CH_MODE_JOINT_STEREO;
                    break;

                case adec_ch_stereo:
                    result =  AUD_CH_MODE_STEREO;
                    break;

                case adec_ch_dual_mono:
                    result =  AUD_CH_MODE_DUAL_MONO;
                    break;

                case adec_ch_multi:
                    {
                        MS_U32 channel_mapping_mask_C_L_R, channel_mapping_mask_Ls_Rs =0x00;
                        channel_mapping_mask_Ls_Rs = (channel_mapping&(ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS));
                        channel_mapping_mask_C_L_R = (channel_mapping&(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R));

                        if(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result = AUD_CH_MODE_3_2_MODE;
                            else if(channel_mapping_mask_Ls_Rs == ACMOD_CH_MASK_NULL)
                               result = AUD_CH_MODE_3_0_MODE;
                            else
                               result = AUD_CH_MODE_3_1_MODE;
                        }
                        else if((channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L))||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                                ||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_R)))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result = AUD_CH_MODE_2_2_MODE;
                            else
                               result = AUD_CH_MODE_2_1_MODE;
                        }
                        else //other multi-channel case, set it to AUD_CH_MODE_3_2_MODE
                        {
                            result = AUD_CH_MODE_3_2_MODE;
                        }
                    }
                    break;

                 case adec_ch_unknown:
                 default:
                    result =  AUD_CH_MODE_NONE;
                    break;
            }
        }
            break;

        case Audio_Comm_infoType_ADEC2_soundmod:
        case Audio_Comm_infoType_ADEC2_okFrmCnt:
        case Audio_Comm_infoType_ADEC2_errFrmCnt:
            break;

        case Audio_Comm_infoType_ADEC2_1ms_timeStamp:
            {
                MS_U32  pts;
                pts = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC2);
                result = pts/45;
            }
            break;

        case Audio_Comm_infoType_ADEC2_33bit_PTS:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC2);
            result <<= 1;
            break;

        case Audio_Comm_infoType_ADEC2_33bit_STCPTS_DIFF:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TD, ADEC2);
            //sign ext
            result <<= 32;
            result >>= 31;
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_reqFlag:
        {
            if (HAL_AUDIO_GetPlayFileFlag(DSP_SE))
                result = 1;
            else
                result = 0;
        }
            break;

        case Audio_Comm_infoType_DEC2_BufferAddr:               //HAL_MAD2_GetBufferAddr
            result = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0) + OFFSET_ES2_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_reqAddr:
            result = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0) + OFFSET_ES2_DRAM_ADDR + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_reqSize:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC2);
            break;

        case Audio_Comm_infoType_DEC2_BufferSize:               //HAL_MAD2_GetBufferSize
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_currLevel:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_pcmBuf_currLevel:
        case Audio_Comm_infoType_DEC2_MMResidualPCM:            //HAL_MAD2_GetMMResidualPCM
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_syncSTC_in_mmTs:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_MMTsSyncFlag, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_getDecInfo1:
        case Audio_Comm_infoType_ADEC2_getDecInfo2:
        case Audio_Comm_infoType_ADEC2_getDecInfo3:
        case Audio_Comm_infoType_ADEC2_getDecInfo4:
        case Audio_Comm_infoType_ADEC2_getDecInfo5:
        case Audio_Comm_infoType_ADEC2_getDecInfo6:
        case Audio_Comm_infoType_ADEC2_getDecInfo7:
        case Audio_Comm_infoType_ADEC2_getDecInfo8:
            //{
            //    MS_U8 param_offset = infoType - Audio_Comm_infoType_ADEC2_getDecInfo1;
            //    result = HAL_MAD2_Read_DSP_sram(REG_DEC2_INFO_BASE+param_offset, DSP_MEM_TYPE_DM);
            //}
            break;

        case Audio_Comm_infoType_ADEC2_getDecChannelInfo:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC2);
            break;

        /********************************************************************************
        *       DECODER 3
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC3_capability:
        case Audio_Comm_infoType_ADEC3_currAudDecStatus:
        case Audio_Comm_infoType_ADEC3_sampleRate:
        case Audio_Comm_infoType_ADEC3_bitRate:
        case Audio_Comm_infoType_ADEC3_acmod:
        case Audio_Comm_infoType_ADEC3_soundmod:
        case Audio_Comm_infoType_ADEC3_reqDataSize:
        case Audio_Comm_infoType_ADEC3_okFrmCnt:
        case Audio_Comm_infoType_ADEC3_errFrmCnt:
        case Audio_Comm_infoType_ADEC3_1ms_timeStamp:
        case Audio_Comm_infoType_ADEC3_33bit_PTS:
        case Audio_Comm_infoType_ADEC3_33bit_STCPTS_DIFF:
        case Audio_Comm_infoType_ADEC3_esBuf_reqFlag:
        case Audio_Comm_infoType_ADEC3_esBuf_reqAddr:
        case Audio_Comm_infoType_ADEC3_esBuf_reqSize:
        case Audio_Comm_infoType_ADEC3_esBuf_currLevel:
        case Audio_Comm_infoType_ADEC3_pcmBuf_currLevel:
        case Audio_Comm_infoType_ADEC3_getDecInfo1:
        case Audio_Comm_infoType_ADEC3_getDecInfo2:
        case Audio_Comm_infoType_ADEC3_getDecInfo3:
        case Audio_Comm_infoType_ADEC3_getDecInfo4:
        case Audio_Comm_infoType_ADEC3_getDecInfo5:
        case Audio_Comm_infoType_ADEC3_getDecInfo6:
        case Audio_Comm_infoType_ADEC3_getDecInfo7:
        case Audio_Comm_infoType_ADEC3_getDecInfo8:
        case Audio_Comm_infoType_ADEC3_getDecChannelInfo:
        case Audio_Comm_infoType_ADEC3_getlatest_PTS:
            break;

        case Audio_Comm_infoType_ReadByte:
            result = (long long)HAL_AUDIO_AbsReadByte(g_ReadTargetAddr);
            break;

        case Audio_Comm_infoType_getSignal_Energy:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_dec1_signal_energy, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_getNR_Status:
            result = (HAL_AUDIO_ReadByte(S2M_MBOX_NR_STATUS)& 0x01);
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Protect:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_hdmi_unstable_protect, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Threshold:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_hdmi_unstable_threshold, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_ADEC3_avSyncOffset:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_OFFSET, ADEC2)& 0x00FFFFFF;
            break;

        case Audio_comm_infoType_Get_audio_crash_status:
            result = g_AudioVars2->g_u8AudioCrashFlag;
            break;

        case Audio_Comm_infoType_Get_MultiPlayer_Capability:
            result = 0;
            break;

        case Audio_Comm_infoType_Get_Parser_Architecture:
            result = (MS_U64)E_AUD_4_PARSER_type2; // 4 AFIFO & 4 audio parser arch
            break;

        case Audio_comm_infoType_Get_Main_Input_Type:
            result = g_AudioVars2->eMainAudioSource;
            break;

        case Audio_comm_infoType_Get_Sub_Input_Type:
            result = g_AudioVars2->eSubAudioSource;
            break;

        case Audio_Comm_infoType_GetSoundBarPowerOnOff:
            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            if ((stDigitalChannelStatusTx1.u8PowerOnOff1 == 0x00) && (stDigitalChannelStatusTx1.u8PowerOnOff2 == 0x00))// Power on
            {
                result = 0x1;
            }
            else            // Power off
            {
                result = 0x0;
            }
            break;

        case Audio_Comm_infoType_GetSoundBarMuteOnOff:
            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            if (stDigitalChannelStatusTx1.u8MuteOnOff == 0x10)// Mute on
            {
                result = 0x1;
            }
            else            // Mute off
            {
                result = 0x0;
            }
            break;

        case Audio_Comm_infoType_GetSoundBarIDData:
            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            result = (HAL_MAD_GetBitReverse(stDigitalChannelStatusTx1.u8SoundBarID1)|
                         (HAL_MAD_GetBitReverse(stDigitalChannelStatusTx1.u8SoundBarID2)<<4)|
                         (HAL_MAD_GetBitReverse(stDigitalChannelStatusTx1.u8SoundBarID3)<<8)|
                         (HAL_MAD_GetBitReverse(stDigitalChannelStatusTx1.u8SoundBarID4)<<12)|
                         (HAL_MAD_GetBitReverse(stDigitalChannelStatusTx1.u8SoundBarID5)<<16)|
                         (HAL_MAD_GetBitReverse(stDigitalChannelStatusTx1.u8SoundBarID6)<<20));
            break;

        case Audio_Comm_infoType_GetSoundBarVolume:
            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            result = (HAL_MAD_GetBitReverse(stDigitalChannelStatusTx1.u8SoundBarVolume2)<<4)|HAL_MAD_GetBitReverse(stDigitalChannelStatusTx1.u8SoundBarVolume1);
            break;

        case Audio_Comm_infoType_GetSoundBarAutoVolume:
            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            if (stDigitalChannelStatusTx1.u8SoundBarAutoVolume == 0x80)// AutoVolume on
            {
                result = 0x1;
            }
            else            // AutoVolume off
            {
                result = 0x0;
            }
            break;

        case Audio_Comm_infoType_GetSoundBarWooferLevel:
            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            result = HAL_MAD_GetBitReverse(stDigitalChannelStatusTx1.u8SoundBarWooferLevel);
            break;

        case Audio_Comm_infoType_GetSoundBarCheckSum:
            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);

            if (retTx1 == FALSE)
            {
                HALMAD_PRINT("%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            result = stDigitalChannelStatusTx1.u8SoundBarCheckSum;
            break;

        case Audio_Comm_infoType_HDMI_DolbyMonitor:
            {
                result = HAL_AUDIO_HDMI_DolbyMonitor();
            }
            break;

        case Audio_Comm_infoType_AUDIO_GetDsp2CodeType:
            {
                result = HAL_SIF_GetDspCodeType();
            }
            break;

        case Audio_Comm_infoType_AUDIO_Get_ADEC0_R2CodeType:
            {
                result = HAL_AUDIO_GetDecR2_ADEC1_DecType();
            }
            break;

        case Audio_Comm_infoType_AUDIO_Get_ADEC1_R2CodeType:
            {
                result = HAL_AUDIO_GetDecR2_ADEC2_DecType();
            }
            break;

        case Audio_comm_infoType_NonPCM_Mixed_System_Sound:
            {
                result = g_AudioVars2->g_bNonPCMMixedSystemSound;
            }
            break;
        /* not support cmd */
        default:
            HALMAD_ERROR("%s: cmd not Supported\n", __FUNCTION__ );
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetMpegInfo()
/// @brief \b Function \b Description: This routine set different paramter to mpeg decoder
/// @param <IN>        \b Audio_MPEG_infoType    : mpeg audio parameter type
/// @param <IN>        \b MS_U32    : common mpeg parameter 1
/// @param <IN>        \b MS_U32    : common mpeg parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetMpegInfo( Audio_MPEG_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    switch(infoType)
    {
        case Audio_MPEG_infoType_SoundMode:                 //HAL_MAD_MPEG_SetSoundMode
            {
                MS_U8 value = 0;                            //default LL mode

                switch((AUD_MPEG_SOUNDMODE)param1)
                {
                    case AUD_MPEG_SOUNDMODE_LL:     value = 0x4;  break;
                    case AUD_MPEG_SOUNDMODE_RR:     value = 0x8;  break;
                    case AUD_MPEG_SOUNDMODE_LR:     value = 0x0;  break;
                    case AUD_MPEG_SOUNDMODE_MIX_LR: value = 0xC;  break;
                    default:                        value = 0x0;  break;
                }
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPEG_SOUNDMODE, ADEC1, (MS_U32)value, 0);
            }
            break;

        case Audio_MPEG_infoType_MMFileSize:                //HAL_MAD_MPEG_SetFileSize
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_MMFileSize, param1, param2);
            break;

        case Audio_MPEG_DTV_AVSYNC_OFFSET:
            if ((param1<DELAY_LOWER_LIMIT)||(param1>DELAY_UPPER_LIMIT))              //delay offset range from 30ms ~ 300ms
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPG_AVOFFSET, ADEC1, (MS_U32)MPG_AV_DEFAULT_DELAY, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPG_AVOFFSET, ADEC1, (MS_U32)param1, 0);
            break;

        case Audio_MPEG_DTV_AVSYNC_DISABLE_SETTING:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPG_AVOFFSET, ADEC1, (MS_U32)MPG_AV_DEFAULT_DELAY, 0);
            break;

        default:
            HALMAD_ERROR("%s: infoType[0x%x,0x%lx,0x%lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetMpegInfo()
/// @brief \b Function \b Description: This routine info of mpeg decoder
/// @param <IN>        \b Audio_MPEG_infoType    : request info type of mpeg decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetMpegInfo( Audio_MPEG_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        case Audio_MPEG_infoType_DecStatus:     //no change
            if (((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_mpeg) && (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1) & 0x0001))
                result = 1;
            else
                result = 0;
            break;

        case Audio_MPEG_infoType_BitRate:       //HAL_MAD_MPEG_GetBitRate
            result = HAL_MAD_MPEG_GetBitRate();
            break;

        case Audio_MPEG_infoType_FrameNum:      //HAL_MAD_MPEG_GetframeNum
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1);
            break;

        case Audio_MPEG_infoType_Layer:         //HAL_MAD_MPEG_GetLayer
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MPEG_LAYER, ADEC1);
            break;

        case Audio_MPEG_infoType_SampleRate:    //HAL_MAD_MPEG_GetSampleRate
            result = HAL_MAD_MPEG_GetSampleRate();
            break;

        case Audio_MPEG_infoType_stereoMode:    //HAL_MAD_MPEG_GetSoundMode
            result = HAL_MAD_MPEG_GetSoundMode();
            break;

        case Audio_MPEG_infoType_Header:        //HAL_MAD_MPEG_GetHeaderInfo
            result = HAL_MAD_MPEG_GetHeaderInfo();
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetWmaInfo()
/// @brief \b Function \b Description: This routine set different paramter to wma decoder
/// @param <IN>        \b Audio_WMA_infoType    : wma audio parameter type
/// @param <IN>        \b MS_U32    : common wma parameter 1
/// @param <IN>        \b MS_U32    : common wma parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetWmaInfo( Audio_WMA_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    switch(infoType)
    {
        case Audio_WMA_infoType_Asf_Param:
            HAL_MAD_WMA_SetASFParm((WMA_ASF_PARMTYPE)param1, param2);       //HAL_MAD_WMA_SetASFParm
            break;

        case Audio_WMA_infoType_FFSpeed:                                    //HAL_MAD_WMA_SetFastFowardSpeed
            HALMAD_ERROR("Currently, it is not used\n");
            break;

        case Audio_WMA_infoType_MMFileSize:
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_MMFileSize, param1, param2);
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x,%lx,%lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetWmaInfo()
/// @brief \b Function \b Description: This routine info of wma decoder
/// @param <IN>        \b Audio_WMA_infoType    : request info type of wma decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetWmaInfo( Audio_WMA_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        case Audio_WMA_infoType_BitRate:                    //HAL_MAD_WMA_GetBitRate
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);
            break;

        case Audio_WMA_infoType_SampleRate:                 //HAL_MAD_WMA_GetSampleRate
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetAC3PInfo()
/// @brief \b Function \b Description: This routine set different parameter to ac3+ decoder
/// @param <IN>        \b Audio_AC3P_infoType    : AC3+ parameter type
/// @param <IN>        \b MS_U32    : AC3+ parameter 1
/// @param <IN>        \b MS_U32    : AC3+ parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    HALMAD_CHECK_SHM_INIT;

    switch(infoType)
    {
        case Audio_AC3P_infoType_DownmixMode:                //HAL_MAD_SetDolbyDownmixMode
            if((DOLBY_DOWNMIX_MODE)param1 == DOLBY_DOWNMIX_MODE_LORO)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 0, 0);
            }
            break;

        case Audio_AC3P_infoType_DrcMode:                    //HAL_MAD_SetDolbyDrcMode
            if((DOLBY_DRC_MODE)param1 == RF_MODE)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 1, 0);
                g_AudioVars2->g_drcMode_adec1 = RF_MODE;
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 0, 0);
                g_AudioVars2->g_drcMode_adec1 = LINE_MODE;
            }
            break;

        case Audio_AC3P_infoType_MMFileSize:            //HAL_MAD_ac3p_setFileSize
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_MMFileSize, param1, param2);
            break;

        case Audio_AC3P_infoType_syncSTC_in_mmTs:
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_syncSTC_in_mmTs, param1, param2);
            break;

        case Audio_AC3P_infoType_hdmiTxBypass_enable:
            if (param1 > 1)
            {
                #if defined (CONFIG_UTOPIA_ANDROID_L)
                    printf("%s: param1[%x,%08x,%08x] out of range\n",__FUNCTION__, infoType, param1, param2);
                #else
                    printf("%s: param1[%x,%x,%x] out of range\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
                #endif
            }

            if( param2 == 0)
            {
                if(g_DDPBypassenable_1 != param1)
                {
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DDP_HDMI_BYPASS,  param2, param1, 0);
                    HAL_AUDIO_DecWriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x40, (param1 << 6));
                    g_DDPBypassenable_1 = param1;
                }
            }
            else
            {
                if(g_DDPBypassenable_2 != param1)
                {
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DDP_HDMI_BYPASS,  param2, param1, 0);
                    HAL_AUDIO_DecWriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x40, (param1 << 6));
                    g_DDPBypassenable_2 = param1;
                }
            }
            break;

        case Audio_AC3P_infoType_BYTESWAP:
             if(param1 == 1) {
                 HAL_AUDIO_WriteMaskByte(0x2C48, 0x10, 0x10);
                 HAL_AUDIO_WriteMaskByte(0x2C49, 0x05, 0x05);
                 }else{
                 HAL_AUDIO_WriteMaskByte(0x2C48, 0x10, 0x00);
                 HAL_AUDIO_WriteMaskByte(0x2C49, 0x05, 0x00);
        }
            break;

        case Audio_AC3P_DTV_AVSYNC_OFFSET:
            if ((param1<DELAY_LOWER_LIMIT)||(param1>DELAY_UPPER_LIMIT))
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AC3_AVOFFSET, ADEC1, AC3_AV_DEFAULT_DELAY, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AC3_AVOFFSET, ADEC1, param1, 0);
            break;

        case Audio_AC3P_DTV_AVSYNC_DISABLE_SETTING:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AC3_AVOFFSET, ADEC1, AC3_AV_DEFAULT_DELAY, 0);
            break;

        case Audio_AC3P_infoType_hdmiAC3inSE:
            if(param1 == TRUE)
            {
                FLAG_HDMI_AC3_IN_SE = TRUE;
            }
            else
            {
                FLAG_HDMI_AC3_IN_SE = FALSE;
            }
            break;

        case Audio_AC3P_infoType_enableDolbyBulletin11:
        case Audio_AC3P_infoType_LoudnessMatching:
            if ( param1 )
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x80, 0x80);
                g_AudioVars2->g_dolbyLoudness_Enable = TRUE;
            }
            else
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x80, 0x00);
                g_AudioVars2->g_dolbyLoudness_Enable = FALSE;
            }

            if ((g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_ATSC) || (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_DMBT))
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x60, 0x40);
                g_AudioVars2->g_dolbyLoudness_DTVsystem = ATSC_MODE;
            }
            else if(g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_DVB)
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x60, 0x00);
                g_AudioVars2->g_dolbyLoudness_DTVsystem = DVB_MODE;
            }
            else
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x60, 0x20);
                g_AudioVars2->g_dolbyLoudness_DTVsystem = OTHER_MODE;
            }

            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_RFSIGNAL_TYPE, ADEC1, g_AudioVars2->g_dolbyLoudness_DTVsystem, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_TB11_ENABLE, ADEC1, g_AudioVars2->g_dolbyLoudness_Enable, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_RFSIGNAL_TYPE, ADEC2, g_AudioVars2->g_dolbyLoudness_DTVsystem, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_TB11_ENABLE, ADEC2, g_AudioVars2->g_dolbyLoudness_Enable, 0);
            break;

        case Audio_AC3P_infoType_Drc_HighCutScale:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_HIGH_CUT, ADEC1, param1, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_HIGH_CUT, ADEC2, param1, 0);
            break;

        case Audio_AC3P_infoType_Drc_LowBootScale:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_LOW_BOOST, ADEC1, param1, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_LOW_BOOST, ADEC2, param1, 0);
            break;

        case Audio_AC3P_infoType_Mixer_Balance:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_MIXER_BALANCE, ADEC1, param1, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_MIXER_BALANCE, ADEC2, param1, 0);
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x,%lx,%lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAC3PInfo()
/// @brief \b Function \b Description: This routine return info type of ac3+ decoder
/// @param <IN>        \b Audio_AC3P_infoType    : AC3+ parameter type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32 :    AC3+ paramter info
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetAC3PInfo( Audio_AC3P_infoType infoType )
{
    MS_U32 result = 0;

    HALMAD_CHECK_SHM_INIT;

    switch(infoType)
    {
        case Audio_AC3P_infoType_DecStatus:
            if ((((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3) ||
                ((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3p)) &&
                (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1)&0x0001))
                result = 1;
            else
                result = 0;
            break;

        case Audio_AC3P_infoType_SampleRate:
            {
                MS_U32 SMP_RATE = 0;

                SMP_RATE = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);        //DD+
                switch(SMP_RATE)
                {
                    case 44100: result = SAMPLE_RATE_44100; break;
                    case 32000: result = SAMPLE_RATE_32000; break;
                    default:
                    case 48000: result = SAMPLE_RATE_48000; break;
                }
            }
            break;

        case Audio_AC3P_infoType_BitRate:  //unit : kbps
            {
                result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);        //DD+
            }
            break;

        case Audio_AC3P_infoType_Acmod:
            result = HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_acmod);
            break;

        case Audio_AC3P_infoType_DrcMode:
            if (HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 0) == 1)
                result = RF_MODE;
            else
                result = LINE_MODE;
            break;

        case Audio_AC3P_infoType_DownmixMode:
            if (HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 0) == 1)
            {
                result = DOLBY_DOWNMIX_MODE_LORO;
            }
            else
            {
                result = DOLBY_DOWNMIX_MODE_LTRT;
            }
            break;

        case Audio_AC3P_infoType_AC3Type:
            if ((((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3) ||
                ((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3p)) &&
                (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1) & 0x0001))
            {
                result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_DEC_TYPE, ADEC1);
                result += 1;
            }
            else
            {
                result = 0;
            }
            break;

        case Audio_AC3P_infoType_FrameSize:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_FRAME_SIZE, ADEC1);
            break;

        case Audio_AC3P_infoType_BSMode:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_BS_MODE, ADEC1);
            break;


        case Audio_AC3P_infoType_hdmiAC3inSE:
            if(FLAG_HDMI_AC3_IN_SE == TRUE)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
            break;

         case  Audio_AC3P_infoType_hdmi_in_support_DDP:
            result = 1;
            break;

        case Audio_AC3P_infoType_ATMOS_support:
            result = ((g_AudioVars2->stDigitalOutCodecCapability[0][CODEC_DDP].support_SAD_byte3) & 0x1);
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetAC3Info()
/// @brief \b Function \b Description: This routine set different paramter to ac3 decoder
/// @param <IN>        \b Audio_WMA_infoType    : ac3 audio parameter type
/// @param <IN>        \b MS_U32    : common ac3 parameter 1
/// @param <IN>        \b MS_U32    : common ac3 parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetAC3Info( Audio_AC3_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    switch(infoType)
    {
        case Audio_AC3_infoType_DownmixMode:                //HAL_MAD_SetDolbyDownmixMode
            HAL_MAD_SetAC3PInfo( Audio_AC3P_infoType_DownmixMode, param1, param2);
            break;

        case Audio_AC3_infoType_DrcMode:                    //HAL_MAD_SetDolbyDrcMode
            HAL_MAD_SetAC3PInfo( Audio_AC3P_infoType_DrcMode, param1, param2);
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x,%lx,%lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAC3Info()
/// @brief \b Function \b Description: This routine info of ac3 decoder
/// @param <IN>        \b Audio_AC3_infoType    : request info type of ac3 decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetAC3Info( Audio_AC3_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        case Audio_AC3_infoType_DecStatus:
            if ((((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3) ||
                ((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3p)) &&
                (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1) & 0x0001))
                result = 1;
            else
                result = 0;
            break;

        case Audio_AC3_infoType_Acmod:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_Acmod);
            break;

        case Audio_AC3_infoType_BitRate:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_BitRate);
            break;

        case Audio_AC3_infoType_SampleRate:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_SampleRate);
            break;

        case Audio_AC3_infoType_DownmixMode:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_DownmixMode);
            break;

        case Audio_AC3_infoType_DrcMode:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_DrcMode);
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetDTSCommonCtrl()
/// @brief \b Function \b Description: This routine set different control to DTS decoder
/// @param <IN>        \b Audio_DTS_infoType    : DTS control type
/// @param <IN>        \b MS_U16    : DTS control value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type )
{
    switch(infoType)
    {
        case Audio_DTS_infoType_DMIX_LFE_EN:
            if (ctrl_type == Audio_DTS_Ctrl_DMIX_LFE_OFF)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_LFE_ENABLE, ADEC1, 0, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_LFE_ENABLE, ADEC1, 1, 0);
            }
            break;

        case Audio_DTS_infoType_Dialnorm_EN:
            // always enable
            break;

        case Audio_DTS_infoType_DrcMode:
            // always disable in T3
            break;

        case Audio_DTS_infoType_DownmixMode:
            if (ctrl_type == Audio_DTS_Ctrl_Downmix_LoRo)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_MODE, ADEC1, 0, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_MODE, ADEC1, 1, 0);
            }
            break;

        case Audio_DTS_infoType_ESDataServiceMode:
            //force Audio_DTS_Ctrl_INTRPTMode in T3
            break;

        case Audio_DTS_infoType_ESData_ByteSwap:
            HALMAD_ERROR("Currently, it is not used\n");
            break;

        case Audio_DTS_infoType_CertMode:
            //DTS Certification Mode, Need to disable Sound Eeffect
            if (ctrl_type == Audio_DTS_Ctrl_CertMode_ON)
            {
                Flag_MAIN_SNDEFFET_CHG = TRUE;
                Reg_REG_SOUND_MAIN_SNDEFFECT = HAL_AUDIO_ReadReg(REG_SOUND_MAIN_SNDEFFECT);
                HAL_AUDIO_WriteReg(REG_SOUND_MAIN_SNDEFFECT,0x0000);
                Reg_REG_SOUND_ADV_CFG0 = HAL_AUDIO_ReadReg(REG_SOUND_ADV_CFG0);
                HAL_AUDIO_WriteReg(REG_SOUND_ADV_CFG0,0x0000);
            }
            if (ctrl_type == Audio_DTS_Ctrl_CertMode_OFF)
            {
                if(Flag_MAIN_SNDEFFET_CHG)
                {
                    HAL_AUDIO_WriteReg(REG_SOUND_MAIN_SNDEFFECT, Reg_REG_SOUND_MAIN_SNDEFFECT);
                    HAL_AUDIO_WriteReg(REG_SOUND_ADV_CFG0, Reg_REG_SOUND_ADV_CFG0);
                    Flag_MAIN_SNDEFFET_CHG = FALSE;
                }
            }
            break;

        default:
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetDTSInfo()
/// @brief \b Function \b Description: This routine return info type of DTS decoder
/// @param <IN>        \b Audio_DTS_infoType    : DTS parameter type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :    DTS paramter info
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetDTSInfo( Audio_DTS_infoType infoType )
{
    MS_U32 result = 0;

    HALMAD_CHECK_SHM_INIT;

    switch(infoType)
    {
        case Audio_DTS_infoType_DecStatus:
            if (((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_dts) && (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1)&0x0001))
                result = 1;
            else
                result = 0;
            break;

        case Audio_DTS_infoType_KernelVersion:
            HALMAD_ERROR("Currently, it is not used\n");
            break;

        case Audio_DTS_infoType_SampleRate: // Original FS from Header
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
            break;

        case Audio_DTS_infoType_BitRate:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);
            break;

        case Audio_DTS_infoType_CD_MODE:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DTS_CD_MODE, ADEC1);
            break;

        case Audio_DTS_infoType_Amode:
        {
            MS_U32 ac_mode = 0xFF;
            MS_U32 channel_mapping = 0x0;
            ac_mode = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MODE, ADEC1);
            channel_mapping = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC1);

            switch(ac_mode)
            {
                case adec_ch_mono:
                    result =  AUD_CH_MODE_MONO;
                    break;

                case adec_ch_joint_stereo:
                    result =  AUD_CH_MODE_JOINT_STEREO;
                    break;

                case adec_ch_stereo:
                    result =  AUD_CH_MODE_STEREO;
                    break;

                case adec_ch_dual_mono:
                    result =  AUD_CH_MODE_DUAL_MONO;
                    break;

                case adec_ch_multi:
                    {
                        MS_U32 channel_mapping_mask_C_L_R, channel_mapping_mask_Ls_Rs =0x00;
                        channel_mapping_mask_Ls_Rs = (channel_mapping&(ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS));
                        channel_mapping_mask_C_L_R = (channel_mapping&(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R));

                        if(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result =  AUD_CH_MODE_3_2_MODE;
                            else if(channel_mapping_mask_Ls_Rs == ACMOD_CH_MASK_NULL)
                               result =  AUD_CH_MODE_3_0_MODE;
                            else
                               result =  AUD_CH_MODE_3_1_MODE;
                        }
                        else if((channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L))||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                                ||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_R)))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result =  AUD_CH_MODE_2_2_MODE;
                            else
                               result =  AUD_CH_MODE_2_1_MODE;
                        }
                        else //other multi-channel case, set it to AUD_CH_MODE_3_2_MODE
                        {
                            result =  AUD_CH_MODE_3_2_MODE;
                        }
                    }
                    break;

                 case adec_ch_unknown:
                 default:
                    result =  AUD_CH_MODE_NONE;
                    break;
            }
        }
            break;

        case Audio_DTS_infoType_HDMI_DSP_SEL:
            result = 0x0;                   //0: in DEC DSP, 1: in SND DSP
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetAACInfo()
/// @brief \b Function \b Description: This routine set different paramter to aac decoder
/// @param <IN>        \b Audio_WMA_infoType    : aac audio parameter type
/// @param <IN>        \b MS_U32    : common aac parameter 1
/// @param <IN>        \b MS_U32    : common aac parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetAACInfo( Audio_AAC_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    switch(infoType)
    {
        case Audio_AAC_infoType_DownmixMode:                //HAL_MAD_SetDolbyDownmixMode
            if((DOLBY_DOWNMIX_MODE)param1 == DOLBY_DOWNMIX_MODE_LORO)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 0, 0);
            }
            break;

        case Audio_AAC_infoType_DrcMode:                    //HAL_MAD_SetDolbyDrcMode
            if((DOLBY_DRC_MODE)param1 == RF_MODE)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 1, 0);
                g_AudioVars2->g_drcMode_adec1 = RF_MODE;
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 0, 0);
                g_AudioVars2->g_drcMode_adec1 = LINE_MODE;
            }
            break;

        case Audio_AAC_DTV_AVSYNC_OFFSET:
            if ((param1<DELAY_LOWER_LIMIT)||(param1>DELAY_UPPER_LIMIT))                                                      //delay offset range from 30ms ~ 300ms
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AAC_AVOFFSET, ADEC1, AAC_AV_DEFAULT_DELAY, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AAC_AVOFFSET, ADEC1, param1, 0);
            break;

        case Audio_AAC_mm_single_frame_mode:
            if (param1 > 0)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_MUL_FRAME_HEADER, ADEC1, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_MUL_FRAME_HEADER, ADEC1, 0, 0);
            }
            break;

        case Audio_AAC_DTV_AVSYNC_DISABLE_SETTING:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AAC_AVOFFSET, ADEC1, AAC_AV_DEFAULT_DELAY, 0);
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x,%lx,%lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAACInfo()
/// @brief \b Function \b Description: This routine info of aac decoder
/// @param <IN>        \b Audio_AAC_infoType    : request info type of aac decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
#define DEC_UNSUPPORT_CNT   10
#define AAC_UNSUPPORT_ADDR  0x3824
MS_U32 HAL_MAD_GetAACInfo( Audio_AAC_infoType infoType )
{
    MS_U32 result=0;
    //MS_U32 result1=0, result2=0,
    MS_U8 u8MainDecCodeType = 0;

    HALMAD_CHECK_SHM_INIT;

    u8MainDecCodeType = g_AudioVars2->g_u8DecR2Adec1DecType;
    if( (u8MainDecCodeType != AU_DVB_STANDARD_AAC) && (u8MainDecCodeType != AU_DVB_STANDARD_MS10_DDT) )
        return result;

    switch(infoType)
    {
        case Audio_AAC_error_count:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1);
            break;

        case Audio_AAC_dec_unsupported:
            //decode unsupported, threshold 10
            HALMAD_ERROR("Currently, it is not used\n");
            break;

        case Audio_AAC_type:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_AAC_TYPE, ADEC1);
            break;

        default:
            HALMAD_ERROR("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetDtsInfo()
/// @brief \b Function \b Description: This routine set different paramter to aac decoder
/// @param <IN>        \b Audio_WMA_infoType    : aac audio parameter type
/// @param <IN>        \b MS_U32    : common aac parameter 1
/// @param <IN>        \b MS_U32    : common aac parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetDtsInfo( Audio_DTS_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    switch(infoType)
    {
        default:
            HALMAD_ERROR("%s: infoType[%x,%lx,%lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAACInfo()
/// @brief \b Function \b Description: This routine info of aac decoder
/// @param <IN>        \b Audio_AAC_infoType    : request info type of aac decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetDtsInfo( Audio_DTS_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        default:
            HALMAD_ERROR("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetRA8LBRInfo()
/// @brief \b Function \b Description: This routine set different paramter to ra8 lbr decoder
/// @param <IN>        \b Audio_RA8LBR_infoType : ra8 lbr audio parameter type
/// @param <IN>        \b MS_U32                : common ra8 lbr parameter 1
/// @param <IN>        \b MS_U32                : common ra8 lbr parameter 2
/// @param <OUT>       \b NONE                  :
/// @param <RET>       \b MS_BOOL               :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE                  :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetRA8LBRInfo( Audio_RA8LBR_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    switch ( infoType )
    {
        case Audio_RA8LBR_NumCodes:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_NUMCODECS, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_Samples:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLES, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_SampleRate:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLERATES, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_Channels1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_Regions1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_cplStart1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_cplQbits1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_FrameSize1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_Channels2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_Regions2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_cplStart2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_cplQbits2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_FrameSize2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_Channels3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_Regions3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_cplStart3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_cplQbits3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_FrameSize3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_Channels4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_Regions4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_cplStart4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_cplQbits4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_FrameSize4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_Channels5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 4 );
            break;
        case Audio_RA8LBR_Regions5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 4 );
            break;
        case Audio_RA8LBR_cplStart5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 4 );
            break;
        case Audio_RA8LBR_cplQbits5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 4 );
            break;
        case Audio_RA8LBR_FrameSize5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 4 );
            break;
        default:
            HALMAD_ERROR("%s: infoType[%x,%lx,%lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetRA8LBRInfo()
/// @brief \b Function \b Description: This routine info of ra8 lbr decoder
/// @param <IN>        \b Audio_RA8LBR_infoType : request info type of ra8 lbr decoder
/// @param <OUT>       \b NONE                  :
/// @param <RET>       \b MS_U32                :  info type
/// @param <GLOBAL>    \b NONE                  :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetRA8LBRInfo( Audio_RA8LBR_infoType infoType )
{
    MS_U32 result = 0;

    switch ( infoType )
    {
        default:
            HALMAD_ERROR("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_LoadCode()
/// @brief \b Function \b Description: This routine is used to load the DVB algorithm code
/// @param u8Type      \b algorithm type
/// @return MS_BOOL    \b TRUE --DSP load code okay
///                       FALSE--DSP load code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_LoadCode(AUDIO_DSP_CODE_TYPE u8Type)
{
    u8Type = u8Type;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_XPCM_setParam
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_XPCM_setParam(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_TYPE, ADEC1, audioType, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_CHANNELS, ADEC1, channels, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLERATES, ADEC1, sampleRate, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BITS_PER_SAMPLE, ADEC1, bitsPerSample, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BLOCK_SIZE, ADEC1, blockSize, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLE_PER_BLOCK, ADEC1, samplePerBlock, 0 );

    return TRUE;
}

//====== STAOS PIP START 2012/02/01 ========

MS_BOOL HAL_MAD_XPCM_setParam2( AUDIO_DEC_ID DecId, Audio_XPCM_Param * pParam )
{
    Audio_XPCM_Param    * pParamXPCM = pParam;
    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

    if ( adec_id < ADEC3 )
    {
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_TYPE, adec_id, (MS_U32) pParamXPCM->audioType, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_CHANNELS, adec_id, (MS_U32) pParamXPCM->channels, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLERATES, adec_id, (MS_U32) pParamXPCM->sampleRate, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BITS_PER_SAMPLE, adec_id, (MS_U32) pParamXPCM->bitsPerSample, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BLOCK_SIZE, adec_id, (MS_U32) pParamXPCM->blockSize, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLE_PER_BLOCK, adec_id, (MS_U32) pParamXPCM->samplePerBlock, 0 );
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_MAD_COOK_setParam2( AUDIO_DEC_ID DecId, Audio_COOK_Param * pParam )
{
    Audio_COOK_Param * pParamCOOK = pParam;
    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

    if ( adec_id < ADEC3 )
    {
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_NUMCODECS, adec_id, pParamCOOK->mNumCodecs, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLES, adec_id, pParamCOOK->mSamples, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLERATES, adec_id, pParamCOOK->mSampleRate, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[0], 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[0], 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[0], 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[0], 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[0], 0 );

        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[1], 1 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[1], 1 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[1], 1 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[1], 1 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[1], 1 );

        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[2], 2 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[2], 2 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[2], 2 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[2], 2 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[2], 2 );

        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[3], 3 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[3], 3 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[3], 3 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[3], 3 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[3], 3 );

        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[4], 4 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[4], 4 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[4], 4 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[4], 4 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[4], 4 );

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_MAD_WMA_SetASFParm2( AUDIO_DEC_ID DecId, Audio_ASF_Param * pParam )
{
    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

    if ( adec_id < ADEC3 )
    {
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_VERSION, adec_id, pParam->u32Version, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELS, adec_id, pParam->u32Channels, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_SAMPLERATE, adec_id, pParam->u32SampleRate, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BYTERATE, adec_id, pParam->u32ByteRate, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BLOCKALIGN, adec_id, pParam->u32BlockAlign, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_ENCOPT, adec_id, pParam->u32Encopt, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_PARSINGBYAPP, adec_id, pParam->u32ParsingByApp, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BITS_PER_SAMPLE, adec_id, pParam->u32BitsPerSample, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELMASK, adec_id, pParam->u32ChannelMask, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PARAM_EXIST, adec_id, pParam->u32DrcParamExist, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_REF, adec_id, pParam->u32DrcRmsAmpRef, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_TARGET, adec_id, pParam->u32DrcRmsAmpTarget, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_REF, adec_id, pParam->u32DrcPeakAmpRef, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_TARGET, adec_id, pParam->u32DrcPeakAmpTarget, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_MAX_PACKET_SIZE, adec_id, pParam->u32MaxPacketSize, 0);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_MAD_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32 Param )
{
    MS_BOOL  bRet = TRUE;

    if((DecId <= AU_DEC_INVALID) || (DecId >= AU_DEC_MAX))
    {
        //[LM14A]
        if(g_bAudio_EnableKernelMsg)
        {
            HALMAD_ERROR("%s: DecId not support. parameter:[0x%x,0x%x,0x%x]\n",__FUNCTION__, DecId, paramType, (unsigned int)Param);
        }
        return bRet;   // can't set invalid ID to R2
    }

    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

    if(adec_id > ADEC2)
    {
        HALMAD_ERROR("%s: adec_id not support. parameter:[0x%x,0x%x,0x%x]\n",__FUNCTION__, DecId, paramType, (unsigned int)Param);
        return bRet;
    }

    HALMAD_CHECK_SHM_INIT;

    switch ( paramType )
    {
        case Audio_ParamType_setAudioDecoder:
        {
            En_DVB_decSystemType eAudioFormat;

            HALMAD_PRINT("\r\n=====ID:%X  Audio_ParamType_setAudioDecoder =0x%X==========\r\n", (unsigned int)adec_id, (unsigned int)Param);

            switch ( (MMA_AudioType) Param )
            {
                case Audio_DEC_PCM:
                case Audio_DEC_XPCM:        eAudioFormat = MSAPI_AUD_DVB_XPCM;        break;
                case Audio_DEC_MPEG:        eAudioFormat = MSAPI_AUD_DVB_MPEG;        break;
                case Audio_DEC_MP3:         eAudioFormat = MSAPI_AUD_DVB_MP3;         break;
                case Audio_DEC_AC3:         eAudioFormat = MSAPI_AUD_DVB_AC3;         break;
                case Audio_DEC_AC3P:        eAudioFormat = MSAPI_AUD_DVB_AC3P;        break;
                case Audio_DEC_AAC_LC:
                case Audio_DEC_HEAAC_V1:
                case Audio_DEC_HEAAC_V2:    eAudioFormat = MSAPI_AUD_DVB_AAC;         break;
                case Audio_DEC_DOLBYPULSE:  eAudioFormat = MSAPI_AUD_DVB_MS10_DDT;    break;
                case Audio_DEC_WMA:         eAudioFormat = MSAPI_AUD_DVB_WMA;         break;
                case Audio_DEC_WMAPRO:      eAudioFormat = MSAPI_AUD_DVB_WMA_PRO;     break;
                case Audio_DEC_RA8:         eAudioFormat = MSAPI_AUD_DVB_RA8LBR;      break;
                case Audio_DEC_DTS:         eAudioFormat = MSAPI_AUD_DVB_DTS;         break;
                case Audio_DEC_FLAC:        eAudioFormat = MSAPI_AUD_DVB_FLAC;        break;
                case Audio_DEC_VORBIS:      eAudioFormat = MSAPI_AUD_DVB_VORBIS;      break;
                case Audio_DEC_AMR_NB:      eAudioFormat = MSAPI_AUD_DVB_AMR_NB;      break;
                case Audio_DEC_AMR_WB:      eAudioFormat = MSAPI_AUD_DVB_AMR_WB;      break;
                case Audio_DEC_DRA:         eAudioFormat = MSAPI_AUD_DVB_DRA;         break;
                case Audio_DEC_DTSLBR:      eAudioFormat = MSAPI_AUD_DVB_DTSLBR;      break;
                case Audio_DEC_DTSXLL:      eAudioFormat = MSAPI_AUD_DVB_DTSXLL;         break;
                case Audio_DEC_KTV:
                case Audio_DEC_KTV2:
                default:                    eAudioFormat = MSAPI_AUD_DVB_INVALID;     break;
            }
            bRet = HAL_AUDIO_SetSystem2(DecId, eAudioFormat);
            if ( bRet == TRUE )
            {
                g_AudioVars2->AudioDecStatus[DecId].bConnect= TRUE;
                g_AudioVars2->AudioDecStatus[DecId].eSourceType = E_AUDIO_INFO_MM_IN;
                g_AudioVars2->AudioDecStatus[DecId].eAudFormat = (En_DVB_decSystemType) eAudioFormat;
            }
            break;
        }

        case Audio_ParamType_playControl:
        {
            //printf("\033[1;36m [AUDIO][UTOPIA][%s] [%s] [%d] [DecId = %d] [paramType = %d] [Param = %ld] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, DecId, paramType, Param);/*@audio*/
            if ( DecId == AU_DEC_ID3 )
            {
                switch ( (MMA_AUDIO_CONTROL) Param )
                {
                    case MMA_STOP:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_STOP);
                        g_AudioVars2->u16DecodeFrmCnt[DecId] = 0;
                        g_AudioVars2->g_bSePlayFileFlag = FALSE;
                        break;

                    case MMA_PLAY:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAY);
                        if(g_AudioVars2->g_bR2Adec2AD)
                        {
                            HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAY_AD);
                        }
                        break;

                    case MMA_PLAY_FILETSP:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAYFILETSP);
                        break;

                    case MMA_RESYNC:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_RESYNC);
                        break;

                    case MMA_PLAY_FILE:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAYFILE);
                        break;

                    case MMA_BROWSE:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_STARTBROWSE);
                        break;

                    case MMA_PAUSE:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PAUSE);
                        break;

                    case MMA_PLAY_FRAME:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAYFRAME);
                        break;

                    case MMA_PLAY_FRAME_GS:
                        if ((g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_MM_IN) ||
                            (g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_GAME_IN))
                        {
                            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG, 0x07, 0);
                            HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAYFRAME_GS);
                        }
                        break;

                    case MMA_PLAY_AD:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAY_AD);
                        g_AudioVars2->g_bR2Adec2AD = TRUE;
                        break;

                    case MMA_STOP_AD:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_STOP_AD);
                        g_AudioVars2->g_bR2Adec2AD = FALSE;
                        break;

                    case MMA_AVSYNC:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_AVSYNC);
                        break;

                    case MMA_FREERUN:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_FREERUN);
                        break;

                    default:
                        break;
                }
            }
            else
            {
                switch ( (MMA_AUDIO_CONTROL) Param )
                {
                    case MMA_STOP:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
                        g_AudioVars2->u16DecodeFrmCnt[DecId] = 0;
                        g_AudioVars2->g_bDecPlayFileFlag = FALSE;
                        break;

                    case MMA_PLAY:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY);
                        if(g_AudioVars2->g_bR2Adec1AD)
                        {
                            HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY_AD);
                        }
                        break;

                    case MMA_PLAY_FILETSP:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILETSP);
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                        break;

                    case MMA_RESYNC:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_RESYNC);
                        break;

                    case MMA_PLAY_FILE:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILE);
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                        break;

                    case MMA_BROWSE:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STARTBROWSE);
                        break;

                    case MMA_PAUSE:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PAUSE);
                        break;

                    case MMA_PLAY_FRAME:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME);
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                        break;

                    case MMA_PLAY_FRAME_GS:
                        if ((g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_MM_IN) ||
                            (g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_GAME_IN))
                        {
                            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG, 0x07, 0);
                            HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME_GS);
                        }
                        break;

                    case MMA_PLAY_AD:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY_AD);
                        g_AudioVars2->g_bR2Adec1AD = TRUE;
                        break;

                    case MMA_STOP_AD:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP_AD);
                        g_AudioVars2->g_bR2Adec1AD = FALSE;
                        break;

                    case MMA_AVSYNC:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_AVSYNC);
                        break;

                    case MMA_FREERUN:
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                        break;

                    default:
                        break;
                }
            }
            break;
        }

        case Audio_ParamType_SoundMode:                     //HAL_MAD_SetSoundMode
        {
            //printf("\033[1;36m [AUDIO][UTOPIA][%s] [%s] [%d] [DecId = %d] [paramType = %d] [Param = %ld] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, DecId, paramType, Param);/*@audio*/
            if ( DecId == AU_DEC_ID1 )
            {
                HAL_AUDIO_SeWriteMaskByte(REG_MB_MODE_SELECT, 0x03, (AU_DVB_MODE) Param);
            }
            else
            {
                HAL_AUDIO_SeWriteMaskByte(REG_MB_MODE_SELECT, 0x0C, ((AU_DVB_MODE)Param) << 2);
            }
            break;
        }

        case Audio_ParamType_PTS_info:
        {
            //HAL_AUDIO_PTS_info(Param);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_INPUT_PTS, adec_id, Param, 0);
            break;
        }

        case Audio_ParamType_syncSTC:
        {
            //printf("\033[1;36m [AUDIO][UTOPIA][%s] [%s] [%d] [DecId = %d] [paramType = %d] [Param = %ld] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, DecId, paramType, Param);/*@audio*/
            if ( Param )
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, adec_id, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, adec_id, 0, 0);
            }
            break;
        }

        case Audio_ParamType_acMode:
        {
            // DD, DDP, HEAAC
            switch ( (AUD_Dolby_SOUNDMODE) Param )
            {
                case AUD_Dolby_SOUNDMODE_LR:     Param = 0x0;  break;
                case AUD_Dolby_SOUNDMODE_LL:     Param = 0x1;  break;
                case AUD_Dolby_SOUNDMODE_RR:     Param = 0x2;  break;
                default:                         Param = 0x0;  break;
            }
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SOUND_MODE, ADEC1, Param, 0);
            break;
        }

        case Audio_ParamType_reqDataSize:
        {
            if ( DecId == AU_DEC_ID3 )
            {
                if ( (HAL_AUR2_ReadByte(REG_R2_DECODE2_CMD) & 0x7F) != 0 )
                {
                    break;
                }
            }
            else
            {
                if ( (HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD) & 0x7F) != 0 )
                {
                    break;
                }
            }

            switch ( (FILE_SIZE) Param )
            {
                case FILE_SIZE_64:      Param = 0x0004;        break;
                case FILE_SIZE_128:     Param = 0x0008;        break;
                case FILE_SIZE_256:     Param = 0x0010;        break;
                case FILE_SIZE_512:     Param = 0x0020;        break;
                case FILE_SIZE_1KB:     Param = 0x0040;        break;
                case FILE_SIZE_2KB:     Param = 0x0080;        break;
                case FILE_SIZE_4KB:     Param = 0x0100;        break;
                case FILE_SIZE_8KB:     Param = 0x0200;        break;
                case FILE_SIZE_16KB:    Param = 0x0400;        break;
                case FILE_SIZE_32KB:    Param = 0x0800;        break;
                case FILE_SIZE_64KB:    Param = 0x1000;        break;
                default:                Param = 0x0100;        break;  // default = 2KB
            }

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }

            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, adec_id, Param, 0);
            break;
        }

        case Audio_ParamType_MM_FFx2:
        {
            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }

            if ( Param )
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FF2X, adec_id, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FF2X, adec_id, 0, 0);
            }
            break;
        }

        case Audio_ParamType_mute:
        {
            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }

            if ( Param )
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DECODE_MUTE, adec_id, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DECODE_MUTE, adec_id, 0, 0);
            }
            break;
        }

        case Audio_ParamType_STR_Resume:
        {
            //STR Resume, Mute on, enable bIsSTR_Resume_Boot
            if(Param)
            {
                bIsSTR_Resume_Boot = TRUE;
            }
            else
            {
                bIsSTR_Resume_Boot = FALSE;
            }
        }
        break;

        case Audio_ParamType_esBuf_informDataRdy:
            break;

        case Audio_ParamType_setWMADecParam:
            bRet = HAL_MAD_WMA_SetASFParm2(DecId, (Audio_ASF_Param *) Param);
            break;

        case Audio_ParamType_setCOOKDecParam:
            bRet = HAL_MAD_COOK_setParam2(DecId, (Audio_COOK_Param *) Param);
            break;

        case Audio_ParamType_setXPCMDecParam:
            bRet = HAL_MAD_XPCM_setParam2(DecId, (Audio_XPCM_Param *) Param);
            break;

        case Audio_ParamType_UNI_PCM3_Input_Addr:
            HALMAD_ERROR("Currently, it is not used\n");    // transfer to line unit
            break;

        case Audio_ParamType_UNI_NEED_DECODE_FRMCNT:

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }

#if (OMX_AUDIO_DEBUG)
            HALMAD_PRINT("\r\n=====ID:%X   Audio_ParamType_UNI_NEED_DECODE_FRMCNT =0x%X==========\r\n", (unsigned int)adec_id,(unsigned int)Param);
#endif
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_SETUP_DECODE_FRMCNT, adec_id, Param, 0);
            break;

        case Audio_ParamType_UNI_TrickMode:
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TRICK, adec_id, Param, 0);
            break;

        case Audio_ParamType_UNI_ES_Wptr:  // OMX Wptr = offset to MAD start address
        {
            MS_U32 u32MadBase, u32EsBase;

            u32MadBase = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);
            if ( DecId == AU_DEC_ID3 )
            {
                u32EsBase = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES3_DRAM_ADDR);
                Param = Param - (MS_U32)(u32EsBase - u32MadBase);
                if(Param > ES3_DRAM_SIZE)
                {
                    HALMAD_ERROR("\r\n===== UNI_ES3_Wptr(0x%X) is too big !!! ======\r\n", (unsigned int)Param);
                    break;
                }
            }
            else
            {
                u32EsBase = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES1_DRAM_ADDR);
                Param = Param - (MS_U32)(u32EsBase - u32MadBase);
                if(Param > ES1_DRAM_SIZE)
                {
                    HALMAD_ERROR("\r\n===== UNI_ES1_Wptr(0x%X) is too big !!! ======\r\n", (unsigned int)Param);
                    break;
                }
            }


            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }
#if (OMX_AUDIO_DEBUG)
            HALMAD_PRINT("\r\n=====ID:%X  Audio_ParamType_UNI_ES_Wptr =0x%X, madBase(0x%x), ES_base(0x%x)==========\r\n", (unsigned int)adec_id, (unsigned int)Param, (unsigned int)u32MadBase, (unsigned int)u32EsBase);
#endif
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, adec_id, Param, 0);
            break;
        }

        case Audio_ParamType_UNI_PCMOutCnt:
        {
            if(adec_id < MAX_ADEC)
            {
                uniPlay_ackCnt[adec_id]++;
                HALMAD_PRINT("\r\nTrigger decInt[%d] %d\r\n", adec_id, uniPlay_ackCnt[adec_id]);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_PCM_SET_OUTPUT_CNT, adec_id, Param, 0);
            }
            else
            {
                HALMAD_ERROR ("%s: adec_id %d over array size !!\r\n", __FUNCTION__, adec_id);
                return FALSE;
            }
            break;
        }

        case Audio_ParamType_AllocDecoder:
        {
            HALMAD_PRINT("\r\n=====DecID:%X  Audio_ParamType_AllocDecoder =0x%X==========\r\n", (unsigned int)DecId, (unsigned int)Param);
            MMA_AudioType eAudioType = (MMA_AudioType) Param;
            MMA_AudioType eAudioCaps;

            OS_OBTAIN_MUTEX(_s32MutexLoadCode, MSOS_WAIT_FOREVER);

            if ( g_AudioVars2->AudioDecStatus[DecId].bConnect == TRUE )
            {
                HALMAD_ERROR ("%s: DecId %d in Used !!\r\n", __FUNCTION__, DecId);
                bRet = FALSE;

                OS_RELEASE_MUTEX(_s32MutexLoadCode);

                break;
            }

            eAudioCaps = _GetAudioCaps(DecId);

            if ( (eAudioCaps & eAudioType) == eAudioType )
            {
                En_DVB_decSystemType eAudioFormat;
                switch ( eAudioType )
                {
                    case Audio_DEC_PCM:
                    case Audio_DEC_XPCM:        eAudioFormat = MSAPI_AUD_DVB_XPCM;        break;
                    case Audio_DEC_MPEG:        eAudioFormat = MSAPI_AUD_DVB_MPEG;        break;
                    case Audio_DEC_MP3:         eAudioFormat = MSAPI_AUD_DVB_MP3;         break;
                    case Audio_DEC_AC3:         eAudioFormat = MSAPI_AUD_DVB_AC3;         break;
                    case Audio_DEC_AC3P:        eAudioFormat = MSAPI_AUD_DVB_AC3P;        break;
                    case Audio_DEC_AAC_LC:
                    case Audio_DEC_HEAAC_V1:
                    case Audio_DEC_HEAAC_V2:    eAudioFormat = MSAPI_AUD_DVB_AAC;         break;
                    case Audio_DEC_DOLBYPULSE:  eAudioFormat = MSAPI_AUD_DVB_MS10_DDT;    break;
                    case Audio_DEC_WMA:         eAudioFormat = MSAPI_AUD_DVB_WMA;         break;
                    case Audio_DEC_WMAPRO:      eAudioFormat = MSAPI_AUD_DVB_WMA_PRO;     break;
                    case Audio_DEC_RA8:         eAudioFormat = MSAPI_AUD_DVB_RA8LBR;      break;
                    case Audio_DEC_DTS:         eAudioFormat = MSAPI_AUD_DVB_DTS;         break;
                    case Audio_DEC_FLAC:        eAudioFormat = MSAPI_AUD_DVB_FLAC;        break;
                    case Audio_DEC_VORBIS:      eAudioFormat = MSAPI_AUD_DVB_VORBIS;      break;
                    case Audio_DEC_AMR_NB:      eAudioFormat = MSAPI_AUD_DVB_AMR_NB;      break;
                    case Audio_DEC_AMR_WB:      eAudioFormat = MSAPI_AUD_DVB_AMR_WB;      break;
                    case Audio_DEC_DRA:         eAudioFormat = MSAPI_AUD_DVB_DRA;         break;
                    case Audio_DEC_DTSLBR:      eAudioFormat = MSAPI_AUD_DVB_DTSLBR;      break;
                    case Audio_DEC_DTS_HD_ADO_R2:   eAudioFormat = MSAPI_AUD_DVB_DTSHDADO;    break;
                    case Audio_DEC_DTSXLL:    eAudioFormat = MSAPI_AUD_DVB_DTSXLL;         break;
                    case Audio_DEC_KTV:
                    case Audio_DEC_KTV2:
                    default:                    eAudioFormat = MSAPI_AUD_DVB_INVALID;     break;
                }
                bRet = HAL_AUDIO_SetSystem2(DecId, eAudioFormat);
                if ( bRet == TRUE )
                {
                    g_AudioVars2->AudioDecStatus[DecId].bConnect = TRUE;
                    g_AudioVars2->AudioDecStatus[DecId].eSourceType = E_AUDIO_INFO_MM_IN;
                    g_AudioVars2->AudioDecStatus[DecId].eAudFormat =(En_DVB_decSystemType) eAudioFormat;

                    /* expect used this path is OMX MM, so need to setup input source path to 0x82 */
                    HAL_AUDIO_SetInputPath(AUDIO_DSP2_INPUT, AUDIO_PATH_4);
                }
            }
            else
            {
                HALMAD_PRINT ("%s: DecId %d Audio Type (0x%08X) not Support !!\r\n", __FUNCTION__, DecId, g_AudioVars2->AudioDecStatus[DecId].eAudFormat );
                bRet = FALSE;
            }

            OS_RELEASE_MUTEX(_s32MutexLoadCode);

            break;
        }

        case Audio_ParamType_FreeDecoder:

            OS_OBTAIN_MUTEX(_s32MutexLoadCode, MSOS_WAIT_FOREVER);

            if ( g_AudioVars2->AudioDecStatus[DecId].bConnect == TRUE )
            {
                if ((g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_MM_IN) ||
                    (g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_GAME_IN))
                {
                    memset(&g_AudioVars2->AudioDecStatus[DecId],  0, sizeof(AudioDecStatus_t));
                    if ( DecId == AU_DEC_ID1 )
                    {
                        g_AudioVars2->g_u8DecR2Adec1DecType = MSAPI_AUD_DVB_NONE;
                    }
                    else if ( DecId == AU_DEC_ID3 )
                    {
                        g_AudioVars2->g_u8DecR2Adec2DecType = MSAPI_AUD_DVB_NONE;
                    }
                }
                else
                {
                    HALMAD_PRINT ("%s: DecId %d NOT used by ANDROID MM, used by %d !!\r\n", __FUNCTION__, DecId, g_AudioVars2->AudioDecStatus[DecId].eSourceType);
                }
            }
            else
            {
                HALMAD_PRINT ("%s: DecId %d NOT in USED !!\r\n", __FUNCTION__, DecId);
                bRet = FALSE;
            }

            OS_RELEASE_MUTEX(_s32MutexLoadCode);

            break;

        case Audio_ParamType_omx_spdif_mode:
        if ( adec_id < ADEC3 )
        {
            MS_U32 u32SpdifParam;
            Audio_OMX_SPDIF_Param eMode;
            u32SpdifParam = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_CTRL, adec_id, 0);
            eMode = (Audio_OMX_SPDIF_Param) Param;
            if ( eMode == Audio_OMX_SPDIF_ON )
            {
                u32SpdifParam |= 0x00000001;
            }
            else
            {
                u32SpdifParam &= 0xFFFFFFFE;
            }
            //HALMAD_ERROR ("%s: DecId %d SpdifParam 0x%08X !!\r\n", __FUNCTION__, DecId, u32SpdifParam);
            if ( (bRet = HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_CTRL, adec_id, u32SpdifParam, 0)) != TRUE )
            {
                HALMAD_ERROR ("%s: DecId %d Fail to switch SPDIF mode to %s !!\r\n", __FUNCTION__, DecId, (eMode == Audio_OMX_SPDIF_ON ? "ON" : "OFF"));
            }
        }
        else
        {
            HALMAD_ERROR ("%s: Param Type %d DecId %d Un-Support Now !!\r\n", __FUNCTION__, Audio_ParamType_omx_spdif_mode, DecId);
            bRet = FALSE;
        }
        break;

        case Audio_ParamType_omx_spdif_play:
        if ( adec_id < ADEC3 )
        {
            MS_U32 u32SpdifParam;
            Audio_OMX_SPDIF_Param eMode;
            u32SpdifParam = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_CTRL, adec_id, 0);
            eMode = (Audio_OMX_SPDIF_Param) Param;
            if ( eMode == Audio_OMX_SPDIF_PLAY )
            {
                u32SpdifParam |= 0x00000002;
            }
            else
            {
                u32SpdifParam &= 0xFFFFFFFD;
            }
            //HALMAD_ERROR ("%s: DecId %d SpdifParam 0x%08X !!\r\n", __FUNCTION__, DecId, u32SpdifParam);
            if ( (bRet = HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_CTRL, adec_id, u32SpdifParam, 0)) != TRUE )
            {
                    HALMAD_ERROR ("%s: DecId %d Fail to switch SPDIF mode to %s !!\r\n", __FUNCTION__, DecId, (eMode == Audio_OMX_SPDIF_ON ? "ON" : "OFF"));
            }
        }
        else
        {
            HALMAD_ERROR ("%s: Param Type %d DecId %d Un-Support Now !!\r\n", __FUNCTION__, Audio_ParamType_omx_spdif_play, DecId);
            bRet = FALSE;
        }
        break;

        case Audio_ParamType_omx_spdif_pcmLevel:

        if ( adec_id < ADEC3 )
        {
            if ( (bRet = HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_PCM_LEVEL, adec_id, Param, 0)) != TRUE )
            {
                HALMAD_ERROR ("%s: DecId %d Fail to setup PCM Level !!\r\n", __FUNCTION__, DecId);
            }
        }
        else
        {
            HALMAD_ERROR ("%s: Param Type %d DecId %d Un-Support Now !!\r\n", __FUNCTION__, Audio_ParamType_omx_spdif_pcmLevel, DecId);
            bRet = FALSE;
        }
        break;

        case Audio_ParamType_Spdif_RDptr:
        {
            MS_U8 adec_decision_cnt = 0;

            if(HAL_MAD_GetDSP2DecCmd()==AU_DVB_DECCMD_PLAYFRAME_GS)
            {
                adec_id = ADEC2;
                adec_decision_cnt +=1;
            }
            if(HAL_MAD_GetDecCmd()==AU_DVB_DECCMD_PLAYFRAME_GS)
            {
                adec_id = ADEC1;
                adec_decision_cnt +=1;
            }


            if(adec_decision_cnt ==2)    //GST dual decode need to decide by spdif control
            {
                if((HAL_AUR2_ReadByte(REG_R2_HDMI_NONPCM_PATH) & 0x1) != 0)
                    adec_id = ADEC2;
                else
                    adec_id = ADEC1;
            }

            Param = (Param<<BYTES_IN_MIU_LINE_LOG2);

            HAL_DEC_R2_SetCommInfo(AUR2_CommParam_ADEC_UNI_set_SPDIF_RdPtr, adec_id, Param>>16, Param&0xFFFF);
        }
        break;

        case Audio_ParamType_omx_spdif_flush_en:
            {
                printf("Flush En %u \r\n", (unsigned int)Param);
                //DMR Fixed Me
                //HAL_MAD2_Write_DSP_sram(DSP2DmAddr_GST_SPDIF_FLUSH_EN, Param, DSP_MEM_TYPE_DM);
            }
            break;

        case Audio_ParamType_omx_spdif_flush:
            {
                printf("Flush %u ms\r\n", (unsigned int)Param);
                //DMR Fixed Me
                //HAL_MAD2_Write_DSP_sram(DSP2DmAddr_GST_SPDIF_FLUSH_MS, Param, DSP_MEM_TYPE_DM);
            }
            break;
        case Audio_ParamType_omx_spdif_mute:
            {
                if(g_TV_Service_bForeground == TRUE)
                {
                    //if TV is foreground, don't let MM to control SPDIF Mute
                }
                else
                {
                    if (1 == Param)
                    {
                        printf("\033[1;36m SPDIF mute %u \033[0m  \r\n", (unsigned int)Param);
                        _HAL_AUDIO_SPDIF_SetMute(TRUE);
                    }
                    else
                    {
                        printf("\033[1;36m SPDIF mute %u \033[0m \r\n", (unsigned int)g_AudioVars2->g_SPDIF_MuteStatus);
                        _HAL_AUDIO_SPDIF_SetMute(g_AudioVars2->g_SPDIF_MuteStatus);
                    }
                }
            }
            break;

        case Audio_ParamType_Hdmi_RDptr:
            {
                MS_U8 adec_decision_cnt = 0;

                if(HAL_MAD_GetDSP2DecCmd()==AU_DVB_DECCMD_PLAYFRAME_GS)
                {
                    adec_id = ADEC2;
                    adec_decision_cnt +=1;
                }
                if(HAL_MAD_GetDecCmd()==AU_DVB_DECCMD_PLAYFRAME_GS)
                {
                    adec_id = ADEC1;
                    adec_decision_cnt +=1;
                }

                if(adec_decision_cnt ==2)    //GST dual decode need to decide by spdif control
                {
                    if((HAL_AUR2_ReadByte(REG_R2_HDMI_NONPCM_PATH) & 0x1) != 0)
                        adec_id = ADEC2;
                    else
                        adec_id = ADEC1;
                }

                Param = (Param<<BYTES_IN_MIU_LINE_LOG2);
                HAL_DEC_R2_SetCommInfo(AUR2_CommParam_ADEC_UNI_set_HDMI_RdPtr, adec_id, Param>>16, Param&0xFFFF);
            }
                break;

        case Audio_ParamType_omx_pcm3_pcmLevel:
            {
                MS_U8 adec_decision_cnt = 0;

                if(HAL_MAD_GetDSP2DecCmd()==AU_DVB_DECCMD_PLAYFRAME_GS)
                {
                    adec_id = ADEC2;
                    adec_decision_cnt +=1;
                }
                if(HAL_MAD_GetDecCmd()==AU_DVB_DECCMD_PLAYFRAME_GS)
                {
                    adec_id = ADEC1;
                    adec_decision_cnt +=1;
                }


                if(adec_decision_cnt ==2)    //GST dual decode need to decide by spdif control
                {
                    if((HAL_AUR2_ReadByte(REG_R2_HDMI_NONPCM_PATH) & 0x1) != 0)
                        adec_id = ADEC2;
                    else
                        adec_id = ADEC1;
                }

                Param = (Param<<BYTES_IN_MIU_LINE_LOG2);

                HAL_DEC_R2_SetCommInfo(AUR2_CommParam_ADEC_UNI_set_DMAReader_PCM_Level, adec_id, Param>>16, Param&0xFFFF);
            }
                break;



        //-----------------------------------------------------------------
        //  LM15U Added Start (Need same as R2_SHM_INFO_TYPE in halAUR2.h)
        //-----------------------------------------------------------------
        case Audio_R2_SHM_PARAM_ES_WR_PTR:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_MPG_AVOFFSET:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPG_AVOFFSET, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_AC3_AVOFFSET:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AC3_AVOFFSET, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_AAC_AVOFFSET:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AAC_AVOFFSET, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_DOLBY_RF_MODE:
        case Audio_R2_SHM_PARAM_DOLBY_DRC_MODE:
        {
            DOLBY_DRC_MODE dolbyDrcMode;
            if((DOLBY_DRC_MODE)Param == RF_MODE)
            {
                dolbyDrcMode = RF_MODE;
            }
            else
            {
                dolbyDrcMode = LINE_MODE;
            }

            switch(DecId)
            {
                case AU_DEC_ID1:
                {
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, dolbyDrcMode, 0);
                    g_AudioVars2->g_drcMode_adec1 = dolbyDrcMode;
                }
                break;

                case AU_DEC_ID3:
                {
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC2, dolbyDrcMode, 0);
                    g_AudioVars2->g_drcMode_adec2 = dolbyDrcMode;
                }
                break;

                default:
                    break;
            }

        }
        break;

        case Audio_R2_SHM_PARAM_DOLBY_DMX_MODE:
        {
            if((DOLBY_DOWNMIX_MODE)Param == DOLBY_DOWNMIX_MODE_LORO)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, adec_id, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, adec_id, 0, 0);
            }
        }
        break;
        case Audio_R2_SHM_PARAM_DOLBY_HIGH_CUT:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_HIGH_CUT, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_DOLBY_LOW_BOOST:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_LOW_BOOST, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_DOLBY_GAIN:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_GAIN, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_DOLBY_MULTI_FRAME_HEADER:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_MUL_FRAME_HEADER, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_MM_TS_SYNC_STC:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_MM_FF2X:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FF2X, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_DECODE_MUTE:
        {
            MS_BOOL bTVMute;
            MS_BOOL bDecoderMute;

            if(Param)
            {
                bTVMute = TRUE;
            }
            else
            {
                bTVMute = FALSE;
            }

            if(adec_id == ADEC1)
            {
                g_AudioVars2->g_bAdec1TVMute = bTVMute;
                bDecoderMute = g_AudioVars2->g_bAdec1GSTMute | g_AudioVars2->g_bAdec1TVMute;
                HAL_SOUND_SetMute(AUDIO_PATH_R2_DMA_1, bDecoderMute);
            }
            else
            {
                g_AudioVars2->g_bAdec2TVMute = bTVMute;
                bDecoderMute = g_AudioVars2->g_bAdec2GSTMute | g_AudioVars2->g_bAdec2TVMute;
                HAL_SOUND_SetMute(AUDIO_PATH_R2_DMA_2, bDecoderMute);
            }
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DECODE_MUTE, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_SOUND_MODE:
        {
            // DD, DDP, HEAAC
            switch ( (AUD_Dolby_SOUNDMODE) Param )
            {
                case AUD_Dolby_SOUNDMODE_LR:     Param = 0x0;  break;
                case AUD_Dolby_SOUNDMODE_LL:     Param = 0x1;  break;
                case AUD_Dolby_SOUNDMODE_RR:     Param = 0x2;  break;
                default:                         Param = 0x0;  break;
            }
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SOUND_MODE, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_MM_FILE_REQ_SIZE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_MM_INPUT_PTS:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_INPUT_PTS, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_UNI_PCM_SET_OUTPUT_CNT:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_PCM_SET_OUTPUT_CNT, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_UNI_SETUP_DECODE_FRMCNT:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_SETUP_DECODE_FRMCNT, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ES2_WR_PTR:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES2_WR_PTR, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_OMX_SPDIF_CTRL:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_CTRL, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_OMX_SPDIF_PCM_LEVEL:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_PCM_LEVEL, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_MPEG_SOUNDMODE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPEG_SOUNDMODE, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_MPEG_GAIN:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPEG_GAIN, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_COOK_NUMCODECS:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_NUMCODECS, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_COOK_SAMPLES:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLES, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_COOK_SAMPLERATES:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLERATES, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_COOK_CHANNELS:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_COOK_REGIONS:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_COOK_CPLSTART:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_COOK_CPLQBITS:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_COOK_FRAMESIZE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_DTS_DMX_LFE_ENABLE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_LFE_ENABLE, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_DTS_DMX_MODE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_MODE, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_ASF_VERSION:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_VERSION, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_CHANNELS:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELS, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_SAMPLERATE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_SAMPLERATE, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_BYTERATE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BYTERATE, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_BLOCKALIGN:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BLOCKALIGN, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_ENCOPT:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_ENCOPT, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_PARSINGBYAPP:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_PARSINGBYAPP, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_BITS_PER_SAMPLE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BITS_PER_SAMPLE, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_CHANNELMASK:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELMASK, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_DRC_PARAM_EXIST:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PARAM_EXIST, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_DRC_RMS_AMP_REF:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_REF, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_DRC_RMS_AMP_TARGET:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_TARGET, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_DRC_PEAK_AMP_REF:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_REF, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_DRC_PEAK_AMP_TARGET:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_TARGET, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_ASF_MAX_PACKET_SIZE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_MAX_PACKET_SIZE, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_XPCM_TYPE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_TYPE, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_XPCM_CHANNELS:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_CHANNELS, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_XPCM_SAMPLERATES:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLERATES, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_XPCM_BITS_PER_SAMPLE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BITS_PER_SAMPLE, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_XPCM_BLOCK_SIZE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BLOCK_SIZE, adec_id, Param, 0);
        }
        break;
        case Audio_R2_SHM_PARAM_XPCM_SAMPLE_PER_BLOCK:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLE_PER_BLOCK, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_HASH_KEY:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_HASH_KEY, adec_id, Param, 0);
        }
        break;
        //-----------------------------------------------------------------
        //  LM15U Added End (Need same as R2_SHM_INFO_TYPE in halAUR2.h)
        //-----------------------------------------------------------------

        //-----------------------------------------------------------------
        //  LM15U Customization  Added Start
        //-----------------------------------------------------------------
        case Audio_SE_Set_I2S_OutNumber:
        {
            if(Param < 5)
            {
                HAL_MAD2_Write_DSP_sram(DSP2DmAddr_IIS_Num, Param, DSP_MEM_TYPE_DM);
            }
            else
            {
                HAL_MAD2_Write_DSP_sram(DSP2DmAddr_IIS_Num, 0, DSP_MEM_TYPE_DM);
            }
        }
        break;

        case Audio_SE_Set_HP_Delay:
        {
            // to do : add HP delay : Param = delay time
            HAL_SOUND_SetHPAudioDelay(Param);
        }
        break;

        case Audio_SE_Set_AENC_Input_Mux:
        {
            // to do : add Encoder input mux
        }
        break;

        case Audio_SE_Set_AENC_Mute:
        {
            // to do : add Encoder mute control
        }
        break;

        case Audio_SE_Set_ADEC0_Input_delay:
        {
            HAL_SOUND_SetCH5AudioDelay((MS_U16)Param);
        }
        break;

        case Audio_SE_Set_ADEC1_Input_delay:
        {
            HAL_SOUND_SetCH6AudioDelay((MS_U16)Param);
        }
        break;

        case Audio_R2_Set_NonPcm_Sel:
        {
            g_AudioVars2->g_R2_NonPcm_Sel = adec_id;
            if(adec_id == ADEC1)
            {
                HAL_AUR2_WriteMaskReg(REG_SNDR2_MBOX_MAIN_SEL, 0x01, 0x00);
            }
            else
            {
                HAL_AUR2_WriteMaskReg(REG_SNDR2_MBOX_MAIN_SEL, 0x01, 0x01);
            }
        }
        break;

        case Audio_SE_Set_PCM_Capture_Volume:
        {
            MS_U16 VolumeStep = 0;
            MS_U16 u16Vol1 = 0,u16Vol2 = 0;

            VolumeStep = ((0x7F00 & (MS_U16)Param) >> 5) + ((MS_U16)Param & 0x7);
            u16Vol1 = VolumeStep >> 3 ;
            u16Vol2 = VolumeStep & 0x7;

            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_pcmCapture_volume, (u16Vol1 << 8) & 0x7F00, DSP_MEM_TYPE_DM);
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_pcmCapture_volume, (u16Vol2 & 0xE0) >> 5, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_SE_Set_PCM_Capture2_Volume:
        {
            MS_U16 VolumeStep = 0;
            MS_U16 u16Vol1 = 0,u16Vol2 = 0;

            VolumeStep = ((0x7F00 & (MS_U16)Param) >> 5) + ((MS_U16)Param & 0x7);
            u16Vol1 = VolumeStep >> 3 ;
            u16Vol2 = VolumeStep & 0x7;

            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_pcmCapture2_volume, (u16Vol1 << 8) & 0x7F00, DSP_MEM_TYPE_DM);
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_pcmCapture2_volume, (u16Vol2 & 0xE0) >> 5, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_R2_Set_DEC_Parser://[LM14A]
        {
            if(DecId == 0)
            {
                HAL_AUDIO_WriteMaskReg(0x2BF2, 0xFFFF, 0x0000);
            }
            else
            {
                HAL_AUDIO_WriteMaskReg(0x2BF2, 0xFFFF, 0x4920);
            }
        }
        break;

        case Audio_R2_SHM_PARAM_AD_Volume:
        {
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AD_VOLUME, adec_id, Param, 0);
        }
        break;

        case Audio_ParamType_ATV_Exist:
        {
            g_AudioVars2->g_bATVExist = (MS_BOOL)Param;
        }
        break;

        case Audio_ParamType_Close_SWDmaReader:
        {
            printf("\r\n === sw dma id (%d) release== \r\n", adec_id);

            if(adec_id > ADEC2) adec_id = ADEC2;
            bSwDmaAvailible[adec_id] = TRUE;

            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x7FFF, 0x0001); // 0x2D36
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x7FFF, 0x0000); // 0x2D36

            if (Param == 0) // Set CH5/CH6 intput mux to NULL
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH5_CFG, 0x1F,0x01);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0x1F,0x01);
            }
        }
        break;

        case Audio_R2_SHM_PARAM_DTS_SEAMLESS_MODE:
        {
            bRet =  HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_SEAMLESS_MODE, adec_id, Param, 0);
        }
        break;

        case Audio_ParamType_Abs_ReadReg_Target:
        {
            g_ReadTargetAddr = Param;
        }
        break;

        case Audio_ParamType_Abs_WriteReg_Target:
        {
            g_WriteTargetAddr = Param;
        }
        break;

        case Audio_ParamType_Abs_WriteReg_Value:
        {
            HAL_AUDIO_AbsWriteReg(g_WriteTargetAddr, Param);
        }
        break;

        case Audio_ParamType_bEnableKernelMsg:
        {
            g_bAudio_EnableKernelMsg = Param;
        }
        break;

        case Audio_ParamType_ReadDM_Target:
        {
            g_ReadDMTargetAddr = Param;
        }
        break;

        case Audio_ParamType_WriteDM_Target:
        {
            g_WriteDMTargetAddr = Param;
        }
        break;

        case Audio_ParamType_WriteDM_Value:
        {
            HAL_MAD2_Write_DSP_sram(g_WriteDMTargetAddr, Param, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_ParamType_ReadPM_Target:
        {
            g_ReadPMTargetAddr = Param;
        }
        break;

        case Audio_ParamType_WritePM_Target:
        {
            g_WritePMTargetAddr = Param;
        }
        break;

        case Audio_ParamType_WritePM_Value:
        {
             HAL_MAD2_Write_DSP_sram(g_WritePMTargetAddr, Param, DSP_MEM_TYPE_PM);
        }
        break;

		//[LM14A]
        case Audio_ParamType_SE_Set_SW_DMA_FS:
        {
            Audio_FastPlay_Info * pSW_DMA_FS = NULL;
            MS_U32 magicNum = 0216;
            MS_U32 uIndexMax = 1;

            if ((void *)Param == NULL)
            {
                 HALMAD_ERROR ("[AUDIO]%s: SW DMA FS param is NULL\r\n", __FUNCTION__);
                 bRet = FALSE;
                 break;
            }

            pSW_DMA_FS = (Audio_FastPlay_Info *) Param;
            /*
            printf("[AUDIO]pSW_DMA_FS->magicNum :%lu\r\n"    , pSW_DMA_FS->magicNum);
            printf("[AUDIO]pSW_DMA_FS->uIdx :%lu\r\n"        , pSW_DMA_FS->uIdx);
            printf("[AUDIO]pSW_DMA_FS->uSample_rate :%lu\r\n", pSW_DMA_FS->uSample_rate);
            printf("[AUDIO]pSW_DMA_FS->uPlayRate :%lu\r\n"   , pSW_DMA_FS->uPlayRate);
            */
            if (pSW_DMA_FS->magicNum != magicNum)
            {
                HALMAD_ERROR ("%s: pSW_DMA_FS->magicNum invalid %lu\r\n", __FUNCTION__, pSW_DMA_FS->magicNum);
                bRet = FALSE;
                break;
            }

            if (pSW_DMA_FS->uIdx > uIndexMax)
            {
                HALMAD_ERROR ("%s: pSW_DMA_FS->uIdx invalid %lu\r\n", __FUNCTION__, pSW_DMA_FS->uIdx);
                bRet = FALSE;
                break;
            }

            if (pSW_DMA_FS->uPlayRate > 40000)
            {
                HALMAD_ERROR ("%s: pSW_DMA_FS->uPlayRate %lu, not support play rate\r\n", __FUNCTION__, pSW_DMA_FS->uPlayRate);
                bRet = FALSE;
                break;
            }

            if (pSW_DMA_FS->uIdx == 0)//HW DMA 1
            {
                MS_U32 i = 0;
                MS_U8  divisor = 0;
                MS_U32 org_synthrate = 0;
                MS_U32 calcu_synthrate = 0;

                for( i = 0; i < SUPPORTED_SAMPLE_RATE; i++ )
                    if( pSW_DMA_FS->uSample_rate == hw_dma_reader_setting[i].uSample_rate )
                        break;

                if(i == SUPPORTED_SAMPLE_RATE)
                {
                    HALMAD_ERROR ("%s: pSW_DMA_FS->uSample_rate %lu, not support sample rate\r\n", __FUNCTION__, pSW_DMA_FS->uSample_rate);
                    bRet = FALSE;
                    break;
                }

                divisor = hw_dma_reader_setting[i].divisor;
                org_synthrate = hw_dma_reader_setting[i].synthrate;
                calcu_synthrate = (org_synthrate * 10000) / pSW_DMA_FS->uPlayRate;

                HALMAD_ERROR ("%s: divisor:%u, org_synthrate:0x%04x, calcu_synthrate:0x%04x\r\n", __FUNCTION__, (unsigned int)divisor, (unsigned int)org_synthrate, (unsigned int)calcu_synthrate);
                /* synthersizer setting update */                   //DMA reader
                HAL_AUR2_WriteMaskReg(NewDMAreader_Enable_ctrl, 0x0030, divisor << 4); //Set divisor
                HAL_AUR2_WriteReg(NewDMAreader_Syth, calcu_synthrate);  //DMA synthesizer N.F.
            }
            else if (pSW_DMA_FS->uIdx == 1)//SW DMA 0 = 15U SW DMA 1
            {
                MS_U32 freq = 0;
                MS_U16 synthrate_H = 0;
                MS_U16 synthrate_L = 0;
                MS_U32 tmp1 = pSW_DMA_FS->uSample_rate / 25;
                MS_U32 tmp2 = pSW_DMA_FS->uPlayRate;
                MS_U32 tmp3 = 160000000; // = ((10000000 / 25) >> 1) * 16 * 25 * 2
                MS_U32 tmp4 = 262144; // = 33554432 / 64 / 2
                MS_U32 tmp5 = 135; // = 843.75 * 4 / 25
                MS_U32 tmp6 = tmp1 * tmp2;

                /*
                 * The original formula:
                 * ((843.75 * 33554432 * 10000000) / (pSW_DMA_FS->uSample_rate * pSW_DMA_FS->uPlayRate)) >> 1;
                 */
                for ( ; ; )
                {
                    if ((((tmp6 >> 1) & 0x1) == 0x0) && (tmp6 > 0x02))
                    {
                        tmp6 >>= 1;
                        tmp3 >>= 1;
                    }
                    else
                    {
                        break;
                    }
                }
                tmp3 /= tmp6;
                freq = tmp5 * tmp4 * tmp3;

                synthrate_L = (MS_U16) freq;
                synthrate_H = (MS_U16) (freq >> 16);

                HALMAD_ERROR ("%s: idx %lu sampleRate %lu playRate %lu synthrate_H 0x%04x, synthrate_L 0x%04x\r\n", __FUNCTION__, pSW_DMA_FS->uIdx, pSW_DMA_FS->uSample_rate, pSW_DMA_FS->uPlayRate, synthrate_H, synthrate_L);
                //DMR Fixed Me
                //HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_SynthFreq_H, 0xFFFF, synthrate_H);
                //HAL_MAD2_Write_DSP_sram(DSP2DmAddr_swDmaRdr_synthFreq_L, synthrate_L, DSP_MEM_TYPE_DM);
            }
            else
            {
                HALMAD_ERROR ("%s: DMA FS un-support index !!\r\n", __FUNCTION__);
                bRet = FALSE;
            }
        }
        break;

        case Audio_ParamType_PreNotMM_Flag:
        {
            if (Param == 0)
            {
                g_AudioVars2->g_bPreNotMM_Flag = FALSE;
            }
            else if (Param == 1)
            {
                g_AudioVars2->g_bPreNotMM_Flag = TRUE;
            }
        }
        break;

        case Audio_ParamType_TrickPlay:
        {
            if (Param == 0)
            {
                g_AudioVars2->g_bTrickPlay = FALSE;
            }
            else if (Param == 1)
            {
                g_AudioVars2->g_bTrickPlay = TRUE;
            }
        }
        break;

        case Audio_ParamType_TV_Service_CH5_Mute_Status: //[LM14A]
        {
            g_TV_Service_CH5_bMute = Param;
        }
        break;

        case Audio_ParamType_TV_Service_CH6_Mute_Status: //[LM14A]
        {
            g_TV_Service_CH6_bMute = Param;
        }
        break;

        case Audio_Set_DMA_Buffer_Limiter:
        {
            HAL_AUDIO_Set_DMA_Buffer(Param);
        }
        break;

        case Audio_R2_SHM_PARAM_ES_Limiter_EN:
        {
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_Limiter_EN, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_ES_Limiter_Threshold:
        {
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_Limiter_Threshold, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_PCM_Limiter_EN:
        {
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PCM_Limiter_EN, adec_id, Param, 0);
        }
        break;

        case Audio_R2_SHM_PARAM_PCM_Limiter_Threshold:
        {
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PCM_Limiter_Threshold, adec_id, Param, 0);
        }
        break;

        case Audio_Set_HP_GAIN_BOOST_flag:
        {
            //DMR Fixed Me
            //HAL_MAD2_Write_DSP_sram(DSP2DmAddr_HP_GAIN_BOOST_flag, Param, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_ParamType_STR_Resume_LGSE_Fail:
        {
            if(Param)
            {
                bIsSTR_Resume_LGSE_Fail = TRUE;
            }
            else
            {
                bIsSTR_Resume_LGSE_Fail = FALSE;
            }
        }
        break;

        case Audio_ParamType_omx_OutputDelayOffset_bEnable:
        {
            g_Omx_OutputDelayOffset_bEnable = Param;
        }
        break;

        case Audio_ParamType_omx_bIsVP9_60P:
        {
            g_Omx_bIsVP9_60P = Param;
        }
        break;

        case Audio_ParamType_TV_Service_bForeground:
        {
            g_TV_Service_bForeground = Param;
        }
        break;

        case Audio_ParamType_CommonCmd:
        {
            HAL_MAD_CommonCmd((AUDIO_COMMON_CMD_t *)Param);
        }
        break;


        case Audio_ParamType_Optical_Output_AC3P_Enable:
        {
            if(Param)
            {
                g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32HdmiArcOutputCodec = DIGITAL_OUTPUT_CODEC_AC3P;
            }
            else
            {
                g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32HdmiArcOutputCodec = DIGITAL_OUTPUT_CODEC_AC3;
            }
        }
        break;

        case Audio_ParamType_Optical_Output_DTS_Enable:  //For DMR Half-Auto case
        {
            if(Param)
            {
                g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32SpdifOutputCodec = DIGITAL_OUTPUT_CODEC_DTS;
            }
            else
            {
                g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32SpdifOutputCodec = DIGITAL_OUTPUT_CODEC_AC3;
            }
        }
        break;

        case Audio_ParamType_LGSE_NO_PROCESSING:
        {
            if(Param)
            {
                HAL_AUR2_WriteMaskReg(REG_DECR2_LGSE_SEL, 0x0002, 0x0002);
            }
            else
            {
                HAL_AUR2_WriteMaskReg(REG_DECR2_LGSE_SEL, 0x0002, 0x0000);
            }
        }
        break;

        case Audio_ParamType_MultiCH_Delay:
        {
            HAL_SOUND_SetMultiCHAudioDelay((MS_U16)Param);
        }
        break;

        case Audio_ParamType_MetaData_Select:
        {
            //Param: 0 = R2 decoder is activated, 2 = R2 decoder is not activated
            //only main decoder can set metadata
            if (((g_AudioVars2->g_R2_NonPcm_Sel == 0x0) && (DecId == AU_DEC_ID1)) ||
                ((g_AudioVars2->g_R2_NonPcm_Sel == 0x1) && (DecId == AU_DEC_ID3)) ||
                ((g_AudioVars2->g_R2_NonPcm_Sel == 0x0) && (DecId == AU_DEC_ID2)) ||
                ((g_AudioVars2->g_R2_NonPcm_Sel == 0x1) && (DecId == AU_DEC_ID2)))
            {
                //printf("\n\033[1;32m[A] metadata :%x\033[0m\n", (AUDIO_META_DATA_SELECT)(Param << 1));
                //printf("\033[1;32m[A] Sel:%d, DecID:%d\033[0m\n", g_AudioVars2->g_R2_NonPcm_Sel, DecId);

                HAL_AUR2_WriteMaskByte(SNDR2_MBOX_BYTE_DAP_SEL, 0x6, (AUDIO_META_DATA_SELECT)(Param << 1));
            }
            else
            {
                //printf("\n\033[1;32m[A] can't set metadata\033[0m\n");
                //printf("\n\033[1;32m[A] Sel:%d, DecID:%d\033[0m\n", g_AudioVars2->g_R2_NonPcm_Sel, DecId);
            }
        }
        break;

        //! MixerGroup1, [0]ch5,  [1]ch6, [2]ch7, [3]ch8
        case Audio_ParamType_DMXLR_Mixed_Channel:
        {
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_MixerGroup1_Ctrl_addr, Param, DSP_MEM_TYPE_DM);
        }
        break;
        //! MixerGroup2, [0]ch5, [1]ch6,  [2]ch7, [3]ch8
        case Audio_ParamType_System_Sound_Mixed_Channel:
        {
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_MixerGroup2_Ctrl_addr, Param, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_ParamType_HDMI_RX_Unstable_Mute:
        {
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_hdmi_unstable_protect, Param, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_ParamType_HDMI_RX_Unstable_Threshold:
        {
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_hdmi_unstable_threshold, Param, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_ParamType_HDMI_RX_Unstable_Mute_Duration_CH5:
        {
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_clock_mute_time_ch5, Param, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_ParamType_HDMI_RX_Unstable_Mute_Duration_CH6:
        {
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_clock_mute_time_ch6, Param, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_ParamType_HDMI_Event_Mute_Enable:
        {
            if(Param == TRUE)
            {
                HAL_AUDIO_AbsWriteReg(0x101A0E, 0x00AC);
            }
            else
            {
                HAL_AUDIO_AbsWriteReg(0x101A0E, 0x0000);
            }
        }
        break;
        //-----------------------------------------------------------------
        //  LM15U Customization  Added End
        //-----------------------------------------------------------------

        /* not supported cmd */
        default:
            HALMAD_PRINT ("%s: infoType[%x] not Supported\n", __FUNCTION__, paramType);
            bRet = FALSE;
            break;
    }

    return bRet;
}

MS_BOOL HAL_MAD_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo )
{
    MS_BOOL result = TRUE;

    if((DecId <= AU_DEC_INVALID) || (DecId >= AU_DEC_MAX))
    {
        //[LM14A]
        if(g_bAudio_EnableKernelMsg)
        {
            HALMAD_ERROR("%s: DecId not support. parameter:[0x%x,0x%x]\n",__FUNCTION__, DecId, infoType);
        }
        return result;   // can't set invalid ID to R2
    }

    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

    if(adec_id > ADEC2)
    {
        HALMAD_ERROR("%s: adec_id not support. parameter:[0x%x,0x%x]\n",__FUNCTION__, DecId, infoType);
        return result;
    }

    HALMAD_CHECK_SHM_INIT;

    switch ( infoType )
    {
        // kochien added for GST
        case Audio_infoType_UNI_PCM3_Base:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            *pRet = (MS_U32)(HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC) + OFFSET_OUTPUT_TEMP_DRAM_BASE + ASND_DSP_DDR_SIZE);
            break;
        }

        // kochien added for GST
        case Audio_infoType_UNI_PCM3_Size:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            *pRet = OUTPUT_TEMP_DRAM_SIZE;
            break;
        }
        case Audio_infoType_capability:
        {
            MMA_AudioType * pA_Type = (MMA_AudioType *) pInfo;
            *pA_Type = _GetAudioCaps(DecId);
            break;
        }

        case Audio_infoType_DecStatus:                     //no change
        {
            MS_U8 * pResult = (MS_U8 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            if ( HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, adec_id) != 0 )
            {
                *pResult = 1;
            }
            else
            {
                *pResult = 0;
            }
            break;
        }

        case Audio_infoType_AudioSyncStatus:
        {
            MS_U8 * pResult = (MS_U8 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                if(HAL_AUDIO_AbsReadByte(REG_R2_DECODE2_CMD)&0x80)
                    *pResult = 0;  // Free-run
                else
                    *pResult = 1;  // AV-sync
            }
            else
            {
                if(HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_CMD)&0x80)
                    *pResult = 0;  // Free-run
                else
                    *pResult = 1;  // AV-sync
            }
            break;
        }

        case Audio_infoType_SampleRate:                    //HAL_MAD_GetMadSamplingRate
        {
            MS_U32 fs_type = 0xFFFFFFFF;
            AUDIO_FS_TYPE * pFS_Type = (AUDIO_FS_TYPE *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            fs_type = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, adec_id);

            *pFS_Type = AUDIO_FS_NONE;

            switch ( fs_type )
            {
                case 8000:   *pFS_Type = AUDIO_FS_8KHZ;   break;
                case 11025:  *pFS_Type = AUDIO_FS_11KHZ;  break;
                case 12000:  *pFS_Type = AUDIO_FS_12KHZ;  break;
                case 16000:  *pFS_Type = AUDIO_FS_16KHZ;  break;
                case 22050:  *pFS_Type = AUDIO_FS_22KHZ;  break;
                case 24000:  *pFS_Type = AUDIO_FS_24KHZ;  break;
                case 32000:  *pFS_Type = AUDIO_FS_32KHZ;  break;
                case 44100:  *pFS_Type = AUDIO_FS_44KHZ;  break;
                case 48000:  *pFS_Type = AUDIO_FS_48KHZ;  break;
                case 64000:  *pFS_Type = AUDIO_FS_64KHZ;  break;
                case 88200:  *pFS_Type = AUDIO_FS_88KHZ;  break;
                case 96000:  *pFS_Type = AUDIO_FS_96KHZ;  break;
                default:     *pFS_Type = AUDIO_FS_NONE;   break;
            }
            break;
        }

        case Audio_infoType_SoundMode:
        {
            MS_U16 * pRet = (MS_U16 *) pInfo;
            *pRet = HAL_AUDIO_ReadReg(REG_MB_MODE_SELECT) & 0x3;

            break;
        }

        case Audio_infoType_errFrmCnt:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, adec_id);

            break;
        }

        case Audio_infoType_1ms_timeStamp:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            MS_U32  pts;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            pts = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, adec_id);
            *pRet = pts/45;

            break;
        }

        case Audio_infoType_33bit_PTS:
        {
            long long * pRet = (long long *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = ((long long)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, adec_id)<<1);
            break;
        }

        case Audio_infoType_BufferAddr:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0) + OFFSET_ES3_DRAM_ADDR;
            }
            else
            {
                *pRet = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0) + OFFSET_ES1_DRAM_ADDR;
            }

            break;
        }

        case Audio_infoType_BufferSize:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, adec_id);

            break;
        }

        case Audio_infoType_esBuf_req:
        {
            MS_BOOL bFlag = FALSE;

            if ( DecId == AU_DEC_ID3 )
            {
                bFlag = g_AudioVars2->g_bSePlayFileFlag;
            }
            else
            {
                bFlag = g_AudioVars2->g_bDecPlayFileFlag;
            }

            if ( bFlag )
            {
                Audio_MM_Data_Request * pDataReq = (Audio_MM_Data_Request *) pInfo;

                if ( DecId == AU_DEC_ID3 )
                {
                    pDataReq->U32WrtBytes = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC2);
                    pDataReq->U32WrtAddr  = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0) + OFFSET_ES2_DRAM_ADDR + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
                }
                else
                {
                    pDataReq->U32WrtBytes = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC1);
                    pDataReq->U32WrtAddr  = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0) + OFFSET_ES1_DRAM_ADDR + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
                }

                result = TRUE;
            }
            else
            {
                result = FALSE;
            }

            break;
        }

        case Audio_infoType_esBuf_currLevel:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, adec_id);

            break;
        }

        case Audio_infoType_pcmBuf_currLevel:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, adec_id);

            break;
        }

        case Audio_infoType_get_audio_filter:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = (MS_U32) FILTER_TYPE_AUDIO_2;
            }
            else if ( DecId == AU_DEC_ID1 )
            {
                *pRet = (MS_U32) FILTER_TYPE_AUDIO_1;
            }
            else
            {
                *pRet = (MS_U32) FILTER_TYPE_AUDIO_INVALID;
            }

            break;
        }

        case Audio_infoType_audio_unsupported:
        {
            HALMAD_ERROR("Currently, it is not used\n");
            break;
        }

        case Audio_infoType_UNI_PCM1_Info:
        {
            Audio_MM_PCM_Info * pPCMInfo = (Audio_MM_PCM_Info *) pInfo;
            MS_U16 u16FrmCnt = HAL_AUDIO_GetUniDecodeFrameCnt(DecId);

           if ( u16FrmCnt != g_AudioVars2->u16DecodeFrmCnt[DecId] )
           {
                g_AudioVars2->u16DecodeFrmCnt[DecId] = u16FrmCnt;

                if ( DecId == AU_DEC_ID3 )
                {
                    pPCMInfo->U32WrtAddr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM2_DRAM_ADDR+HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_ADDR, adec_id);
                }
                else
                {
                    pPCMInfo->U32WrtAddr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM1_DRAM_ADDR+HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_ADDR, adec_id);
                }

                pPCMInfo->U32WrtBytes = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_SIZE, adec_id);
#if (OMX_AUDIO_DEBUG)
                HALMAD_PRINT("\r\n===== Audio_infoType_UNI_PCM1_Info  addr :0x%X     size:0x%X==========\r\n", (unsigned int)pPCMInfo->U32WrtAddr, (unsigned int)pPCMInfo->U32WrtBytes);
#endif
            }

            pPCMInfo->U32DecodeFrame = g_AudioVars2->u16DecodeFrmCnt[DecId];
        }
            break;

        case Audio_infoType_UNI_PCM3_Level:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = (MS_U32)HAL_AUDIO_DmaReader_GetLevel(DecId)*BYTES_IN_MIU_LINE;
            break;
        }

        case Audio_infoType_SWDmaReader_Level:
        {
            #if 0

            MS_U32 * pRet = (MS_U32 *) pInfo;
            MS_U32 u32Level  = 0;
//            printf("\r\n === id (%d) == \r\n", (int)DecId)
            if(DecId == 0)
            {
                u32Level = BYTES_IN_MIU_LINE*HAL_AUDIO_ReadReg(S2M_MBOX_SW_DMA_READER_DDR_Level);   // in bytes
            }
            else
            {
                u32Level = BYTES_IN_MIU_LINE*HAL_AUDIO_ReadReg(S2M_MBOX_SW_DMA_READER2_DDR_Level);   // in bytes
            }
            * pRet = u32Level;

//            printf("\r\n === id (%d), value(0x%x) == \r\n", (int)DecId, (unsigned int)u32Level);
            #else

            MS_U32 * pRet = (MS_U32 *) pInfo;


            *pRet = (MS_U32)HAL_AUDIO_SwDmaReader_GetLevel(DecId);
            #endif
            break;
        }

        case Audio_infoType_Open_SWDmaReader:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            #if 1
            int SwDmaId;
            for (SwDmaId = 0;SwDmaId<2;SwDmaId++)
            {
                if (bSwDmaAvailible[SwDmaId] ==TRUE)
                {
                    bSwDmaAvailible[SwDmaId] = FALSE;
                    printf("\r\n === sw dma id (%d) get== \r\n", SwDmaId);
                    * pRet = SwDmaId;
                    break;
                }
            }
            #else   // for test, use s/w dma 1 for high priority
            if(bSwDmaAvailible[1] ==TRUE)
            {
                    bSwDmaAvailible[1] = FALSE;
                    printf("\r\n === sw dma id (1) get== \r\n");
                    * pRet = 1;
            }
            else if(bSwDmaAvailible[0] ==TRUE)
            {
                    bSwDmaAvailible[0] = FALSE;
                    printf("\r\n === sw dma id (0) get== \r\n");
                    * pRet = 0;
            }
            #endif
//            printf("\r\n === id (%d), value(0x%x) == \r\n", (int)DecId, (unsigned int)u32Level);

            break;
        }

        case Audio_infoType_UNI_ES_Level:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, adec_id);
#if (OMX_AUDIO_DEBUG)
            HALMAD_PRINT("\r\n=====Audio_infoType_UNI_ES_Level =0x%X==========\r\n", (unsigned int)*pRet);
#endif
            break;
        }

        case Audio_infoType_UNI_ES_Base:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES3_DRAM_ADDR);
            }
            else
            {
                *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES1_DRAM_ADDR);
            }
            break;
        }

        case Audio_infoType_UNI_ES_Size:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = ES3_DRAM_SIZE;
            }
            else
            {
                *pRet = ES1_DRAM_SIZE;
            }
            break;
        }

        case Audio_infoType_UNI_PCM_Base:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM2_DRAM_ADDR);
            }
            else
            {
                *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM1_DRAM_ADDR);
            }
            break;
        }

        case Audio_infoType_UNI_PCM_Size:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = PCM2_DRAM_SIZE;
            }
            else
            {
                *pRet = PCM1_DRAM_SIZE;
            }
            break;
        }

        case Audio_infoType_MadBaseAddr:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);
            }
            else
            {
                *pRet = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);
            }
            break;
        }

        case Audio_infoType_hashkey:
        {
            Audio_Hashkey_Info * pHashkeyInfo = (Audio_Hashkey_Info *) pInfo;
            MS_U32 audio_ipauth = HAL_MAD2_Read_DSP_sram(REG_DEC2_AUDIO_IPAUTH, DSP_MEM_TYPE_PM);

            pHashkeyInfo->U32IsHashkeySupported = TRUE;

            switch ( pHashkeyInfo->U32Name )
            {
                case Audio_DEC_AC3:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_AC3)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DOLBY_MS11)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_LC_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_C_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_B_PROFILE))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_AC3P:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_AC3P)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DOLBY_MS11)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_LC_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_C_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_B_PROFILE))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_AAC_LC:
                case Audio_DEC_HEAAC_V1:
                case Audio_DEC_HEAAC_V2:
                case Audio_DEC_DOLBYPULSE:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_DOLBY_MS10)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_GAAC)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DOLBY_MS11)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_C_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_B_PROFILE))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_WMA:
                case Audio_DEC_WMAPRO:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_WMA))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_DTS:
                {
                	if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_DTS)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DTS_XLL))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_DTSLBR:
                {
                	if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_DTSLBR)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DTS_XLL))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_DRA:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_DRA))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_RA8:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_COOK))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Audio_infoType_AccumEsCnt:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ACCUM_ES_BYTECNT, adec_id);
            break;
        }

        case Audio_infoType_Decoder_Type:
        {
            MS_U8 u8Decoder = MSAPI_AUD_DVB_INVALID;
            MS_U32 u32DecoderType = 0;
            MS_U32 *pRet = (MS_U32 *)pInfo;

            switch (adec_id)
            {
                case ADEC1:
                {
                    u8Decoder = g_AudioVars2->g_u8DecR2Adec1DecType;
                    break;
                }

                case ADEC2:
                {
                    u8Decoder = g_AudioVars2->g_u8DecR2Adec2DecType;
                    break;
                }

                default:
                {
                    HALMAD_ERROR ("%s: invalid ID (%d, %d)\n", __FUNCTION__, DecId, adec_id);
                    result = FALSE;
                    break;
                }
            }

            if (result == TRUE)
            {
                switch (u8Decoder)
                {
                    case AU_DVB_STANDARD_DTS:
                    case AU_DVB_STANDARD_DTSLBR:
                    {
                        u32DecoderType = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DTS_ES_TYPE, adec_id) & 0x00FFFFFF;

                        switch (u32DecoderType)
                        {
                            case DTS_TYPE_CORE:
                            case DTS_TYPE_CORE_XLL:
                            {
                                *pRet = AUDIO_DTS_TYPE_CORE;
                                break;
                            }

                            case DTS_TYPE_LBR:
                            {
                                *pRet = AUDIO_DTS_TYPE_LBR;
                                break;
                            }

                            case DTS_TYPE_XLL:
                            {
                                *pRet = AUDIO_DTS_TYPE_HD;
                                break;
                            }

                            default:
                            {
                                result = FALSE;
                                break;
                            }
                        }
                        break;
                    }

                    default:
                    {
                        HALMAD_ERROR ("%s: invalid ID (%d, %d)\n", __FUNCTION__, DecId, adec_id);
                        result = FALSE;
                        break;
                    }
                }
            }

            break;
        }

        case Audio_infoType_Pcm_Output_Channel:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_OUTPUT_CHANNEL, adec_id);
            break;
        }

        //-----------------------------------------------------------------
        //  LM15U Added Start (Need same as R2_SHM_INFO_TYPE in halAUR2.h)
        //-----------------------------------------------------------------
        case Audio_R2_SHM_INFO_CAPABILITY:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CAPABILITY, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_DEC_STAUS:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_DEC_ERROR_ID:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_ERROR_ID, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_DEC_CHANNEL_MODE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if(adec_id == ADEC1)
            {
                *pRet = (MS_U32)HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_acmod);
            }
            else
            {
                *pRet = (MS_U32)HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC2_acmod);
            }
        }
        break;

        case  Audio_R2_SHM_INFO_DRA_CHANNEL_MAPPING: // This is for DRA use only
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            dra_acmod channelMapping = 0x0;
            channelMapping = (dra_acmod)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, adec_id);
            *pRet = (MS_U32)channelMapping;
        }
        break;

        case Audio_R2_SHM_INFO_SMP_RATE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_BIT_RATE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_SYNTH_RATE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNTH_RATE, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_ES_LEVEL:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_PCM_LEVEL:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_ES_RD_PTR:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_RD_PTR, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_PCM_WR_PTR:
        case Audio_infoType_Multi_Pcm_WRptr:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_WR_PTR, adec_id)>>BYTES_IN_MIU_LINE_LOG2);    //line unit
        }
        break;
        case Audio_R2_SHM_INFO_MM_FILE_REQ_SIZE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_ACCUM_ES_BYTECNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ACCUM_ES_BYTECNT, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_WAIT_1STPTS_CNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_WAIT_1STPTS_CNT, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_WAIT_STC_CNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_WAIT_STC_CNT, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_SMP_FLUSH_CNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_FLUSH_CNT, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_AVSYNC_STATE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_STATE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_AVSYNC_FREERUN_TYPE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_FREERUN_TYPE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_AVSYNC_OFFSET:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_OFFSET, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_PTS_TAG_WR_IDX:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS_TAG_WR_IDX, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_PTS_TAG_RD_IDX:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS_TAG_RD_IDX, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_PLAY_STATE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAY_STATE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_DEC_TYPE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_TYPE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_PLAYSMPFLAG:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAYSMPFLAG, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_DEC_CALLING_CNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CALLING_CNT, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_RECEIVE_STOP_CNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_RECEIVE_STOP_CNT, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_OK_FRMCNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_ERR_FRMCNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_SKIP_FRMCNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SKIP_FRMCNT, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_REPT_FRMCNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_REPT_FRMCNT, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_SYNC_MISS_CNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNC_MISS_CNT, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_STC:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_STC, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_PTS:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_CURR_PTS:
        {
            long long * pRet = (long long *) pInfo;
            *pRet = ((long long)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, adec_id))<<1;

        }
        break;
        case Audio_R2_SHM_INFO_TD:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TD, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_UNI_DECODE_DONE_CNT:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_CNT, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_UNI_DECODE_DONE_PCM_ADDR:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_ADDR, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_UNI_DECODE_DONE_PCM_SIZE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_SIZE, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_PCM_ADDR:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_ADDR, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_PCM_SIZE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_SIZE, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_spdifbuf_LEVEL:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_spdifbuf_LEVEL, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_spdifbuf_WR_PTR:
        case Audio_infoType_Spdif_WRptr:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_spdifbuf_WR_PTR, adec_id)>>BYTES_IN_MIU_LINE_LOG2);    //line unit
        }
        break;

        case Audio_R2_SHM_INFO_DOLBY_DEC_TYPE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_DEC_TYPE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_DOLBY_FRAME_SIZE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_FRAME_SIZE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_DOLBY_BS_MODE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_BS_MODE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_DOLBY_AAC_TYPE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_AAC_TYPE, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_DTS_CD_MODE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DTS_CD_MODE, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_DTS_PCM_FRAME_SIZE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DTS_PCM_FRAME_SIZE, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_DTS_ES_TYPE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DTS_ES_TYPE, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_SPDIF_INFO_FLAG:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SPDIF_INFO_FLAG, adec_id);
        }
        break;

        case Audio_R2_SHM_INFO_MPEG_LAYER:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MPEG_LAYER, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_MPEG_STEREOMODE:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MPEG_STEREOMODE, adec_id);
        }
        break;
        case Audio_R2_SHM_INFO_MPEG_HEADER:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MPEG_HEADER, adec_id);
        }
        break;
        //-----------------------------------------------------------------
        //  LM15U Added End (Need same as R2_SHM_INFO_TYPE in halAUR2.h)
        //-----------------------------------------------------------------

        //-----------------------------------------------------------------
        //  M2 Customization  Added Start
        //-----------------------------------------------------------------

        case Audio_SE_SPEAKER_MUTE_STATUS:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            MS_U16 mute_status = HAL_AUDIO_ReadReg(0x2D08) & 0x8000;

            if(mute_status)
            {
                *pRet = 1;
            }
            else
            {
                *pRet = 0;
            }
        }
        break;

        case Audio_SE_HPOUT_MUTE_STATUS:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            MS_U16 mute_status = HAL_AUDIO_ReadReg(0x2D02) & 0x8000;

            if(mute_status)
            {
                *pRet = 1;
            }
            else
            {
                *pRet = 0;
            }
        }
        break;

        case Audio_SE_SPDIFOUT_MUTE_STATUS:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            MS_U16 mute_status = HAL_AUDIO_ReadReg(REG_M2D_MAILBOX_SPDIF_CTRL) & 0x0001;

            if(mute_status)
            {
                *pRet = 1;
            }
            else
            {
                *pRet = 0;
            }
        }
        break;

        case Audio_SE_SCARTOUT_MUTE_STATUS:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            MS_U16 mute_status = HAL_AUDIO_ReadReg(0x2D04) & 0x8000;

            if(mute_status)
            {
                *pRet = 1;
            }
            else
            {
                *pRet = 0;
            }
        }
        break;

        case Audio_SE_Decoder_Delay:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = HAL_SOUND_GetCH6AudioDelay();
            }
            else
            {
                *pRet = HAL_SOUND_GetCH5AudioDelay();
            }
        }
        break;

        case Audio_SE_SpeakerOut_Delay:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (MS_U32)HAL_SOUND_GetCH1AudioDelay();
        }
        break;

        case Audio_SE_HpOut_Delay:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (MS_U32)HAL_SOUND_GetHPAudioDelay();
        }
        break;

        case Audio_SE_SPDIFOut_Delay:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (MS_U32)HAL_SOUND_GetSpdifAudioDelay();
        }
        break;

        case Audio_R2_Dec_Type:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = (MS_U32)HAL_AUDIO_AbsReadByte(REG_R2_DECODE2_TYPE);
            }
            else
            {
                *pRet = (MS_U32)HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE);
            }

            if(*pRet == adec_type_ac3)
            {
                *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_DEC_TYPE, adec_id);
                if(*pRet == 1) //DDP
                {
                    *pRet = adec_type_ac3p;
                }
                else //DD
                {
                    *pRet = adec_type_ac3;
                }
            }

            /*AAC 0x0  HE-AACv1= 0x1,HE-AACv2=0x2 */
            if(*pRet == adec_type_aac)
            {
                *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_AAC_TYPE, adec_id);
                if(*pRet == 0) //AAC
                {
                    *pRet = adec_type_aac;
                }
                else //HE-AACv1= 0x1,HE-AACv2=0x2
                {
                    *pRet = adec_type_aacp;
                }
            }

        }
        break;

        case Audio_infoType_Abs_ReadReg:
        {
            MS_U16 * pRet = (MS_U16 *) pInfo;

            *pRet = HAL_AUDIO_AbsReadReg(g_ReadTargetAddr);
        }
        break;

        case Audio_infoType_ReadDM_Value:
        {
            MS_U16 * pRet = (MS_U16 *) pInfo;

            *pRet = (MS_U16)HAL_MAD2_Read_DSP_sram(g_ReadDMTargetAddr, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_infoType_ReadPM_Value:
        {
            MS_U16 * pRet = (MS_U16 *) pInfo;

            *pRet = (MS_U16)HAL_MAD2_Read_DSP_sram(g_ReadPMTargetAddr, DSP_MEM_TYPE_PM);
        }
        break;

        case Audio_infoType_SWDMAReader_Pause:
        {
            MS_U8 pause1 = 0;
            MS_U8 pause2 = 0;
            MS_U16 * pRet = (MS_U16 *) pInfo;
            pause1 = HAL_AUDIO_ReadByte(M2S_MBOX_SW_DMA_READER_DDR_Ctrl+1) & 0x80;
            pause2 = HAL_AUDIO_ReadByte(M2S_MBOX_SW_DMA_READER_DDR_Ctrl+1) & 0x80;
            if( pause1 == 1 && pause2 == 1)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
            *pRet = result;
        }

        break;

        case Audio_infoType_PreNotMM_Flag:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            *pRet = g_AudioVars2->g_bPreNotMM_Flag;
        }
        break;

        case Audio_R2_SHM_INFO_TP_ES_FreeSize:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if(adec_id == ADEC1)
            {
                *pRet = ES1_DRAM_SIZE -HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, adec_id);
            }
            else
            {
                *pRet = ES3_DRAM_SIZE -HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, adec_id);
            }
        }
        break;

        case Audio_infoType_STR_Resume:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = bIsSTR_Resume_Boot;
        }
        break;

        case Audio_infoType_STR_Resume_LGSE_Fail:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = bIsSTR_Resume_LGSE_Fail;
        }
        break;

        case Audio_infoType_omx_OutputDelayOffset_bEnable:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = g_Omx_OutputDelayOffset_bEnable;
        }
        break;

        case Audio_infoType_omx_bIsVP9_60P:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = g_Omx_bIsVP9_60P;
        }
        break;

        case Audio_infoType_TV_Service_bForeground:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = g_TV_Service_bForeground;
        }
        break;

        //-----------------------------------------------------------------
        //  M2 Customization  Added End
        //-----------------------------------------------------------------

        //-----------------------------------------------------------------
        //  DMR Added Start
        //-----------------------------------------------------------------

        case Audio_infoType_HDMI_NPCM_Lock_Status:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_AUDIO_HDMI_NonpcmMonitor2();
        }
        break;

        case Audio_infoType_MetaData_Select:
        {
            // 0x112E93[2:1] = 0: R2 decoder is activated
            // 0x112E93[2:1] = 2: R2 decoder is not activated
            MS_U8 * pRet = (MS_U8 *) pInfo;
            *pRet = ((HAL_AUR2_ReadByte(SNDR2_MBOX_BYTE_DAP_SEL)&0x06)>>1);
        }
        break;

        case Audio_infoType_DMXLR_Mixed_Channel:
        {
            // MixerGroup1, [0]ch5,  [1]ch6, [2]ch7, [3]ch8
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_MixerGroup1_Ctrl_addr, DSP_MEM_TYPE_DM);
        }
        break;

        case Audio_infoType_System_Sound_Mixed_Channel:
        {
            // MixerGroup2, [0]ch5, [1]ch6,  [2]ch7, [3]ch8
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_MixerGroup2_Ctrl_addr, DSP_MEM_TYPE_DM);
        }
        break;

        //-----------------------------------------------------------------
        //  DMR Added End
        //-----------------------------------------------------------------

        /* not support cmd */
        default:
            HALMAD_PRINT ("%s: cmd not Supported\n", __FUNCTION__ );
            result = FALSE;
            break;
    }

    return result;
}

//====== STAOS PIP END 2012/02/01 ========

// [LM14]
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetBitReverse()
/// @brief \b Function \b Description: This routine is used to only get 4 bit reverse for 8 bit input
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_GetBitReverse(MS_U8 input_byte)
{
    MS_U8 i, temp = 0;
    for (i = 0; i < 4; i++)
    {
       temp = temp | (((input_byte>>i) & 0x01) << (3-i));
    }

    return temp;
}

MS_S32 HAL_MAD_CommonCmd_TrasferData(void *pSrc, MS_U32 u32SrcSize, void *pDst, MS_U32 u32DstSzie)
{
    MS_S32 s32Ret = 0;
    MS_U32 u32TransferSize = 0;

    if (pSrc == NULL)
    {
        HALMAD_ERROR("%s: Error! pSrc should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (u32SrcSize == 0)
    {
        HALMAD_ERROR("%s: Error! u32SrcSize should not be zero!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pDst == NULL)
    {
        HALMAD_ERROR("%s: Error! pDst should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (u32DstSzie == 0)
    {
        HALMAD_ERROR("%s: Error! u32DstSzie should not be zero!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    u32TransferSize = (u32SrcSize < u32DstSzie) ? u32SrcSize : u32DstSzie;
    memcpy(((MS_U8 *)pDst + 8), ((MS_U8 *)pSrc + 8),(u32TransferSize - 8));
    if(u32SrcSize < u32DstSzie)
    {
        memset(((MS_U8 *)pDst + u32TransferSize), 0x00, (u32DstSzie - u32SrcSize));
    }

    return s32Ret;
}


MS_S32 HAL_MAD_SpecifyDigitalOutputCodec(AUDIO_SPECIFIED_DIGITAL_OUTPUT_t * pAudioSpecifiedDigitalOutput, DIGITAL_OUTPUT_TYPE u32DigitalOutputType)
{
    MS_S32 s32Ret  = 0;
    MS_U32 u32UserStrucVer = 0;
    MS_U32 u32UserStrucSize = 0;
    MS_U32 u32DriverStrucVer = 0;
    MS_U32 u32DriverStrucSize = 0;
    AUDIO_SPECIFIED_DIGITAL_OUTPUT_t tSpecifiedDigitalOutput;

    HALMAD_CHECK_SHM_INIT;

    if (pAudioSpecifiedDigitalOutput == NULL)
    {
        HALMAD_ERROR("%s: Error! pAudioSpecifiedDigitalOutput should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    u32UserStrucVer = pAudioSpecifiedDigitalOutput->u32StructVersion;
    u32DriverStrucVer = g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32StructVersion;

    if ((u32UserStrucVer & AUDIO_MAJOR_VERSION_MASK) != (u32DriverStrucVer & AUDIO_MAJOR_VERSION_MASK))
    {
        HALMAD_ERROR("%s(): structure version is mismatch, user is 0x%08X, driver is 0x%08X!\n", __FUNCTION__,(unsigned int)u32UserStrucVer,(unsigned int)u32DriverStrucVer);
        return -A_EPERM;
    }

    u32UserStrucSize = pAudioSpecifiedDigitalOutput->u32StructSize;
    u32DriverStrucSize = g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32StructSize;

    memset((void *)&tSpecifiedDigitalOutput, 0x00, sizeof(AUDIO_SPECIFIED_DIGITAL_OUTPUT_t));
    tSpecifiedDigitalOutput.u32StructVersion = AUDIO_SPECIFIED_DIGITAL_OUTPUT_VERSION;
    tSpecifiedDigitalOutput.u32StructSize = sizeof(AUDIO_SPECIFIED_DIGITAL_OUTPUT_t);

    s32Ret = HAL_MAD_CommonCmd_TrasferData(pAudioSpecifiedDigitalOutput, u32UserStrucSize, &tSpecifiedDigitalOutput, u32DriverStrucSize);
    if (s32Ret < 0)
    {
        return s32Ret;
    }

    switch (u32DigitalOutputType)
    {
        case DIGITAL_SPDIF_OUTPUT:
        {
            g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32SpdifOutputCodec = tSpecifiedDigitalOutput.u32SpdifOutputCodec;
            break;
        }

        case DIGITAL_HDMI_ARC_OUTPUT:
        {
            g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32HdmiArcOutputCodec = tSpecifiedDigitalOutput.u32HdmiArcOutputCodec;
            break;
        }

        case DIGITAL_HDMI_OUTPUT:
        {
            g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32HdmiTxOutputCodec = tSpecifiedDigitalOutput.u32HdmiTxOutputCodec;
            break;
        }

        default:
        {
            memcpy((void *)&g_AudioVars2->g_AudioSpecifiedDigitalOutput, (void *)&tSpecifiedDigitalOutput, sizeof(AUDIO_SPECIFIED_DIGITAL_OUTPUT_t));
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_MAD_CommonCmd(AUDIO_COMMON_CMD_t * cmd)
{
    char *name = (char *)cmd->u8StrCmd;
    MS_S32 s32Ret = 0;

    if (cmd == NULL)
    {
        HALMAD_ERROR("%s: Error! cmd should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (!(strcmp(name, "SpecifyDigitalOutputCodec")))
    {
        s32Ret = HAL_MAD_SpecifyDigitalOutputCodec((AUDIO_SPECIFIED_DIGITAL_OUTPUT_t *)cmd->pData, DIGITAL_MAX_OUTPUT);
    }
    else if (!(strcmp(name, "SpecifyDigitalOutputCodec_Spdif")))
    {
        s32Ret = HAL_MAD_SpecifyDigitalOutputCodec((AUDIO_SPECIFIED_DIGITAL_OUTPUT_t *)cmd->pData, DIGITAL_SPDIF_OUTPUT);
    }
    else if (!(strcmp(name, "SpecifyDigitalOutputCodec_HdmiArc")))
    {
        s32Ret = HAL_MAD_SpecifyDigitalOutputCodec((AUDIO_SPECIFIED_DIGITAL_OUTPUT_t *)cmd->pData, DIGITAL_HDMI_ARC_OUTPUT);
    }
    else if (!(strcmp(name, "SpecifyDigitalOutputCodec_HdmiTx")))
    {
        s32Ret = HAL_MAD_SpecifyDigitalOutputCodec((AUDIO_SPECIFIED_DIGITAL_OUTPUT_t *)cmd->pData, DIGITAL_HDMI_OUTPUT);
    }
    else
    {
        HALMAD_ERROR("%s(): Error! Unsupportted Common Cmd (%s)!!\n", __FUNCTION__, cmd->u8StrCmd);
        s32Ret = -A_EINVAL;
    }

    return s32Ret;
}
