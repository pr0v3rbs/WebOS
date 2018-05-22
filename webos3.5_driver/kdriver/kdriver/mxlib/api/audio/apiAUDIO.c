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
///
/// file    apiAUDIO.c
/// @brief  audio API functions
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#else
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvAUDIO_if.h"
#include "apiAUDIO.h"
#include "apiAUDIO_private.h"
#include "drvAUDIO.h"
#include "drvAUDIO_close_src_1.h"
#include "utopia_dapi.h"

#include "../drv/audio/internal/drvKTV.h"
#include "../drv/audio/internal/drvSIF.h"
#include "../drv/audio/internal/drvMAD.h"
#include "../drv/audio/internal/drvMAD2.h"
#include "../drv/audio/internal/drvSOUND.h"
#include "../drv/audio/internal/drvADVSOUND.h"
#include "../drv/audio/internal/drvAUDIO_internal.h"
#include "../drv/audio/internal/drvAudio_Common.h"
#include "drvWDT.h" //Refine power on sequence for earphone & DAC pop noise issue


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

    #define APIAUDIO_PRINT(fmt, args...)    LOGI("<<android>>      " fmt, ## args)
    #define APIAUDIO_ERROR(fmt, args...)    LOGE("<<android>>      " fmt, ## args)
#else
    #define APIAUDIO_PRINT(fmt, args...)
    #define APIAUDIO_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
#endif

#define AUDIO_DEBUG_MSG(x) //x

#define AUDIO_MIXER_THREAD_DEBUG FALSE

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#endif
#endif

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define AUD_ERRMSG(msg)  MS_CRITICAL_MSG(msg)
#define AUD_DBGMSG(msg)  MS_DEBUG_MSG(msg)

#define APIAUDIO_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            APIAUDIO_ERROR("%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            APIAUDIO_ERROR("%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (MDrv_AUDIO_SHM_Init() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

/*
 * AEON(MM) and MCU use independent memory space,
 * so they can not use the same flag
 */
#if defined(MCU_AEON) && !defined(__AEONR2__)
    /* AEON(MM) */
    #define APIAUDIO_CHECK_INIT_FLAG(bRet) \
        do{ \
            bRet = TRUE; \
        }while(0)
#else
    /* ARM, MIPS, and AEON_R2(NOS_MCU) */
    #define APIAUDIO_CHECK_INIT_FLAG(bRet) \
        do{ \
            bRet = MDrv_AUDIO_GET_INIT_FLAG(); \
            if (bRet == FALSE) \
            { \
                APIAUDIO_ERROR("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__); \
            } \
        }while(0)
#endif

#define COUNT_DOWN 25	//LG

//H/W register define, should check when bring up
#define PCM_MIXER_DEBUG_REG 0x2D3C

#ifndef UTOPIA_STRIP
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Functions Prototype
//------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------
extern MS_BOOL g_bDSPLoadCode;
extern AUDIO_SHARED_VARS2 * g_AudioVars2;
extern MS_S32 _s32AUDIOMutex;
extern MS_U8 CheckPlayDoneCnt;
extern MS_U32 u32Gpid;
extern AUDIO_PCM_t *g_PCM[AUDIO_PCM_MAXNUM];
extern MS_U32 g_Omx_bIsVP9_60P;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U32 u32DecoderPid_Table[AU_DEC_MAX]={0};
static MS_U32 u32RenderPid_Table[AUDIO_PCM_MAXNUM]={0};
AUDIO_SPDIF_OUTPUT_TYPE     gSpdifOutputType;
HDMI_TX_OUTPUT_TYPE         gHdmiOutputType;
AUDIOSTATUS                 m_eSifStatus;
AUDIOSTANDARD_TYPE          m_eSifStandard;
MS_U8                       SpeakerMaxVolume = 0x0C;            //(MS_U8)(TvVolumeTable[100]>>8);
MS_U8                       DynamicAvl_active_volume = 0x17;    //about -12dB for 2Vrms input, value of volume control register

static MS_U8                u8CurrAdvSndSys = 0xFF;
static En_DVB_decCmdType    enDecoderStatus;
static MS_U8                u8IntTag = 1;
MS_U32                      g_audioDumpInfo_interval = 500;

//[LM14]
static MS_U32               f_clipCount = COUNT_DOWN;
static MS_U32               _clipWriteLastAddr = 0;
static MS_BOOL              clipPlayDone = FALSE;
AUDIO_INIT_INFO             AudioSystemInfo;
AUDIO_OUT_INFO              AudioOutputInfo;        // Store the connectivity of audio output device
AUDIO_PATH_INFO             AudioPathInfo;          // Store the connectivity of audio DSP & output path
OUTPUT_SOURCE_INFO          AudioOutputSourceInfo;  // Store the output port source setting
AUDIO_SOURCE_INFO_TYPE      gAudioSourceInfo;
AUDIO_FILE_PARAMS           audioFileParams;
En_DVB_decSystemType        enCurDecSystem, enCurDecSystem2;
LGSEVolumeReset             LGSEVolumeSettings;
MS_U32                      g_Omx_Previous_bIsVP9_60P = FALSE;

static OUTPUT_SOURCE_INFO OutSourceInitSettings=
{
    E_CONNECT_MAIN,  // For Speaker
    E_CONNECT_MAIN,  // For HP
    E_CONNECT_MAIN,  // For Monitor out
    E_CONNECT_SCART,  // For Scart out
    E_CONNECT_MAIN,  // For Spdif out
    E_CONNECT_MAIN,   // For HDMI ARC
    E_CONNECT_MAIN   // For HDMI Tx
};

typedef enum
{
    E_MIXERSTATE_OPEN,
    E_MIXERSTATE_CLOSE,
    E_MIXERSTATE_START,
    E_MIXERSTATE_STOP
}MixerState;

typedef struct{
    MS_BOOL bFirstPush;
    MS_BOOL bFirstPop;
    MS_U32 u32StartTime;
    MS_U32 u32PauseTime;
    MixerState eState;
    MS_U32 u32ReadTime_BeforePause;
    MS_U32 u32OutCnt;
}MixerControl_t;

static void *g_pPcmMixerHandle = NULL;
static void *g_pPcmMixerHandle_tmp = NULL;
static MS_S16 *g_pPcmMixerInbuf[8] = {NULL};
static MS_S32 g_s32PcmMixerInlen[8] = {0};
MixerControl_t stMixerControl[8];


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void _ApiDecCmd_To_DrvDecCmd(En_DVB_decCmdType enDecComamnd, AU_DVB_DECCMD *u8DecCmd)
{
    switch(enDecComamnd)
    {
        case MSAPI_AUD_DVB_DECCMD_STOP:
            *u8DecCmd = AU_DVB_DECCMD_STOP;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAY:
            *u8DecCmd = AU_DVB_DECCMD_PLAY;
            break;

        case MSAPI_AUD_DVB_DECCMD_RESYNC:
            *u8DecCmd = AU_DVB_DECCMD_RESYNC;
            break;

        case MSAPI_AUD_DVB_DECCMD_FREE_RUN:
            *u8DecCmd = AU_DVB_DECCMD_FREERUN;
            break;

        case MSAPI_AUD_DVB_DECCMD_AVSYNC:
            *u8DecCmd = AU_DVB_DECCMD_AVSYNC;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFILE:
             *u8DecCmd = AU_DVB_DECCMD_PLAYFILE;
            break;

        case MSAPI_AUD_DVB_DECCMD_PAUSE:
            *u8DecCmd = AU_DVB_DECCMD_PAUSE;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFILETSP:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFILETSP;
            break;

        case MSAPI_AUD_DVB_DECCMD_STARTBROWSE:
            *u8DecCmd = AU_DVB_DECCMD_STARTBROWSE;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFRAME;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME_STAGE:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFRAME_STAGE;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME_GS:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFRAME_GS;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFILE2_PTS;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAY_AD:
            *u8DecCmd = AU_DVB_DECCMD_PLAY_AD;
            break;

        case MSAPI_AUD_DVB_DECCMD_STOP_AD:
            *u8DecCmd = AU_DVB_DECCMD_STOP_AD;
            break;

        case MSAPI_AUD_DVB_DECCMD_SKIP:
            *u8DecCmd = AU_DVB_DECCMD_SKIP;
            break;

        default:
            // Need refine
            *u8DecCmd = AU_DVB_DECCMD_STOP;
            break;
    }
    return;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//         AUDIO_SYSTEM RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus()
/// @brief \b Function \b Description:  Return audio status.
/// @return MS_U16     \b : return structure which include pcm, non pcm, sampling rate.
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts)
{
    return(MDrv_AUDIO_HDMI_RX_GetHdmiInAudioStatus(p_hdmiAudioSts));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPowerOn()
/// @brief \b Function \b Description: This routine is used to execute DSP power on/down setting.
/// @param bFlag       \b : TRUE --power on;
///                         FALSE--power off
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetPowerOn(MS_BOOL bFlag)
{
    MDrv_AUDIO_SetPowerOn(bFlag);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDspBaseAddr()
/// @brief \b Function \b Description:  This function is used to set bin file base address in flash for Audio module.
/// @param u8Index     \b :
/// @param u32Bin_Base_Address    \b :
/// @param u32Mad_Base_Buffer_Adr \b :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_U32 u32Bin_Base_Address, MS_U32 u32Mad_Base_Buffer_Adr)
{
    MDrv_AUDIO_SetDspBaseAddr(u8Index, u32Bin_Base_Address, u32Mad_Base_Buffer_Adr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDspMadBaseAddr()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param u8Index     \b     :
/// @return MS_U32     \b     :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index)
{
    return(MDrv_AUDIO_GetDspMadBaseAddr(u8Index));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_WriteDecMailBox()
/// @brief \b Function \b Description:  This routine is used to write Dec-DSP mail box
/// @param u8ParamNum  \b : Mail box address
/// @param u16Data     \b : value
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_WriteDecMailBox(MS_U8 u8ParamNum, MS_U16 u16Data)
{
    MDrv_AUDIO_WriteDecMailBox(u8ParamNum, u16Data);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_TriggerSifPLL()
/// @brief \b Function \b Description:  This function is used to initialize SIF analog part .
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_TriggerSifPLL(void)
{
    MDrv_AUDIO_TriggerSifPLL();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUD_Initialize()
/// @brief \b Function \b Description: Audio initialize function incluing DSP power on and load code.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_Initialize(void)
{
    if (MDrv_AUDIO_SHM_Init() == FALSE)
    {
        APIAUDIO_ERROR("MApi_AUDIO_Initialize failes to get SHM buffer\n");
        return;
    }

    if (MDrv_AUDIO_Mutex_Init() == FALSE)
    {
        APIAUDIO_ERROR("%s() : fails to init Audio Mutex\n", __FUNCTION__);
        return;
    }

    MDrv_AUDIO_PCM_Init();

    OS_OBTAIN_MUTEX(_s32AUDIOMutex, MSOS_WAIT_FOREVER);

    if (MDrv_AUDIO_GET_INIT_FLAG() == TRUE)
    {
        /* Patch for multi-process, some audio utopia call back function did not register */
        /* so if share memory have already init, check call back function again,          */
        /* if call back function is NULL, register it !!                                  */
        MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Set_CallBackFunc, 0, 0);

        /* Patch for multi-process, MMIO Base address need to setup for different process */
        MDrv_AUDIO_Init();

        OS_RELEASE_MUTEX(_s32AUDIOMutex);

        APIAUDIO_ERROR("MApi_AUDIO_Initialize doesn't need to init again\n");
        return;
    }

    g_AudioVars2->eAudioSource = E_AUDIO_INFO_ADC_IN;

    memcpy(&(g_AudioVars2->AudioOutputSourceInfo), &OutSourceInitSettings, sizeof(g_AudioVars2->AudioOutputSourceInfo));
    MDrv_AUDIO_Init();

    //Marked to reduce power on time
    //MsOS_DelayTaskUs(10*1000);    //Refine power on sequence for earphone & DAC pop noise issue

    MDrv_AUDIO_WriteInitTable();

    MDrv_AUDIO_SetPowerOn(TRUE);

    //To prevent RIU conflict with demod when stsyem boot up, because audio and demod init at the same time
    //Trigger SIF PLL is called when load DSP SIF code at HAL layer
    //MDrv_AUDIO_TriggerSifPLL();

    MDrv_MAD_SetMemInfo();
    MDrv_MAD2_SetMemInfo();

    // Load DSP DEC Code
    MDrv_SIF_Init();
    MDrv_SOUND_Init();

    // Load SIF DSP code stage
    MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);

    MDrv_MAD_Init();

    MDrv_AUDIO_DecoderLoadCode();  // Load decoder main code

    MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_FS, SPDIF_CS_FS_48K);

    if (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_DVB)
        MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_DVB);
    else if (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_ATSC)
        MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_ATSC);
    else if (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_ISDB)
        MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_ISDB);
    else
        MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_NONE);

    //MApi_AUDIO_SetAdvSndSys(MSAPI_AUD_ADVSND_SRS);

    if ( g_AudioVars2->AudioPathInfo.HpOut != AUDIO_PATH_NULL )
    {
        MDrv_AUDIO_SetInputPath(AUDIO_SRC_INPUT, g_AudioVars2->AudioPathInfo.HpOut);
    }

    //M2 doesn't use MStar balance
    //MDrv_SOUND_EnableBalance(TRUE);   // Enable Speaker Balance function in audio init .

    //MApi_AUDIO_SetEncInit(AU_DVB_ENCBR_192K, 16);

    //Refine power on sequence for earphone & DAC pop noise issue
    MDrv_AUDIO_EnaEarphone_LowPower_Stage();

//================================================
//  Execute Output Path connection
//================================================
    _MApi_AUDIO_SetOutConnectivity();
//================================================

    //Attach audio interrupts for DEC/SND DSP to MCU
    MDrv_AUDIO_AttachInterrupt(TRUE);

    //Enable LGSE at HAL layer
    //_MApi_AUDIO_ADVSOUND_SubProcessEnable(LG_SOUNDENGINE_EN, TRUE);

    //Rise init flag
    MDrv_AUDIO_SET_INIT_FLAG(TRUE);

    OS_RELEASE_MUTEX(_s32AUDIOMutex);

    //Initiate PCM Mixer
    if (_MApi_AUDIO_PcmMixer_Init() != 0)
    {
        APIAUDIO_ERROR("%s: Error! fail to initiate PCM Mixer!\n", __FUNCTION__);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_Shift()
/// @brief \b Function \b Description: This function is used to provide a API for application to choose what kind of VIF mdoe.
///
////////////////////////////////////////////////////////////////////////////////

void _MApi_AUDIO_SIF_Shift(En_AUD_VIF_Type type)
{
    if(MDrv_SIF_SendCmd(AU_SIF_CMD_GET_VIF_PATCH_ENABLE, NULL, NULL) == 0)
    {
        return;
    }

    switch (type)
    {
        case MSAPI_AUD_SIF_42M:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_SIF_SHIFT_CLK, 0x00, NULL);
            break;

        case MSAPI_AUD_SIF_43M:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_SIF_SHIFT_CLK, 0x01, NULL);
            break;

        case MSAPI_AUD_SIF_44M:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_SIF_SHIFT_CLK, 0x02, NULL);
            break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_Audio_Monitor()
/// @brief \b Function \b Description: This function is used to monitor DSP while-loop and ISR running counter.
///                                  If these counters stop longer than a certain period of time in any abnormal condition, DSP reboot will be launched to alive DSP process again.
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_Audio_Monitor(void)
{

    MS_U8   u8CurrentDecR2While, u8CurrentDecR2Timer, u8CurrentSeWhile, u8CurrentSeISR, u8CurrentSndR2While, u8CurrentSndR2Timer;
    MS_U32 u32Delay_Counter = 0;
    MS_U8   revive = 0;

    static MS_U8  u8LastDecR2While = 1;
    static MS_U8  u8LastDecR2Timer = 1;
    static MS_U8  u8LastSeWhile = 1;
    static MS_U8  u8LastSeISR = 1;
    static MS_U8  u8LastSndR2While = 1;
    static MS_U8  u8LastSndR2Timer = 1;
    static MS_U8  u8DecR2WhileFailCount = 0;
    static MS_U8  u8DecR2TimerFailCount = 0;
    static MS_U8  u8SeWhileFailCount = 0;
    static MS_U8  u8SeISRFailCount = 0;
    static MS_U8  u8SndR2WhileFailCount = 0;
    static MS_U8  u8SndR2TimerFailCount = 0;
    static MS_U32 Waittime = 0, Waittime2 = 0;

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return(revive);
    }

    MDrv_AUDIO_SPDIF_Monitor_SamplingRate();    // used in U4 for SPDIF raw out ADC clock setting

    if ( MsOS_Timer_DiffTimeFromNow(Waittime2) > g_audioDumpInfo_interval)
    {
        Waittime2 = MsOS_GetSystemTime();
        MDrv_AUDIO_DumpDspInfo();
    }

    if (MsOS_Timer_DiffTimeFromNow(Waittime) < 100)
    {
        return(revive);
    }

    if (g_bDSPLoadCode== TRUE)
    {
        return(revive);
    }

    Waittime = MsOS_GetSystemTime();

    if((u8SeWhileFailCount >= 7) || (u8SeISRFailCount >= 7)||(u8DecR2WhileFailCount>=7)||(u8DecR2TimerFailCount>=7))
    {
        u32Delay_Counter = u8LastDecR2While + u8LastDecR2Timer + u8LastSeWhile + u8LastSeISR;
        MsOS_DelayTaskUs(u32Delay_Counter);
    }

    u8CurrentDecR2While =  MDrv_AUDIO_ReadDspCounter(DEC_R2_MAIN_COUNTER);
    u8CurrentDecR2Timer = MDrv_AUDIO_ReadDspCounter(DEC_R2_TIMER_COUNTER);
    u8CurrentSeWhile = MDrv_AUDIO_ReadDspCounter(DSP_SE_MAIN_COUNTER);
    u8CurrentSeISR = MDrv_AUDIO_ReadDspCounter(DSP_SE_ISR_COUNTER);
    u8CurrentSndR2While =  MDrv_AUDIO_ReadDspCounter(SND_R2_MAIN_COUNTER);
    u8CurrentSndR2Timer = MDrv_AUDIO_ReadDspCounter(SND_R2_TIMER_COUNTER);

    // Checking Decoder DSP counter
    if ( u8CurrentDecR2While - u8LastDecR2While == 0 )
    {
        u8DecR2WhileFailCount++;
    }
    else
    {
        u8DecR2WhileFailCount=0;
    }

    if ( u8CurrentDecR2Timer - u8LastDecR2Timer == 0 )
    {
        u8DecR2TimerFailCount++;
    }
    else
    {
        u8DecR2TimerFailCount = 0;
    }

    if ( u8CurrentSeWhile - u8LastSeWhile == 0 )
    {
        u8SeWhileFailCount++;
    }
    else
    {
        u8SeWhileFailCount = 0;
    }

    if ( u8CurrentSeISR - u8LastSeISR == 0 )
    {
        u8SeISRFailCount++;
    }
    else
    {
        u8SeISRFailCount = 0;
    }

    if ( u8CurrentSndR2While == u8LastSndR2While)
    {
        u8SndR2WhileFailCount++;
    }
    else
    {
        u8SndR2WhileFailCount = 0;
    }

    if ( u8CurrentSndR2Timer == u8LastSndR2Timer)
    {
        u8SndR2TimerFailCount++;
    }
    else
    {
        u8SndR2TimerFailCount = 0;
    }

    if( ((u8SeWhileFailCount + u8SeISRFailCount)==3) || ((u8DecR2WhileFailCount + u8DecR2TimerFailCount)==3) || ((u8SndR2WhileFailCount + u8SndR2TimerFailCount)==3) )
    {
        MsOS_DelayTask(1);    //Fix counter mis-detection problem
    }
    else if( ((u8SeWhileFailCount + u8SeISRFailCount)==7) || ((u8DecR2WhileFailCount + u8DecR2TimerFailCount)==7) || ((u8SndR2WhileFailCount + u8SndR2TimerFailCount)==7) )
    {
        MsOS_DelayTask(1);
    }

    if ( (u8SeWhileFailCount >= 10) || (u8SeISRFailCount >= 10) || (u8SndR2WhileFailCount >= 10) || (u8SndR2TimerFailCount >= 10) )
    {
        AUD_DBGMSG(printf("u8SeWhileFailCount = %x\n",(MS_U16)u8SeWhileFailCount));
        AUD_DBGMSG(printf("u8SeISRFailCount = %x\n",(MS_U16)u8SeISRFailCount));
        AUD_DBGMSG(printf("u8SndR2WhileFailCount = %x\n",(MS_U16)u8SndR2WhileFailCount));
        AUD_DBGMSG(printf("u8SndR2TimerFailCount = %x\n",(MS_U16)u8SndR2TimerFailCount));

        u8SeWhileFailCount = 0;
        u8SeISRFailCount = 0;
        u8DecR2WhileFailCount = 0;
        u8DecR2TimerFailCount = 0;
        u8SndR2WhileFailCount = 0;
        u8SndR2TimerFailCount = 0;

        MDrv_AUDIO_DspReboot(0);

        if(!MDrv_AUDIO_IsDTV())
        {
            //msAPI_AuWriteSIFThresholdTbl();
        }

        //Reset LGSE param
        MDrv_ADVSOUND_SetParam2(LGSE_PARAM_RESET, 0, 0, 0);

        revive = 1;
    }
    else if ( (u8DecR2WhileFailCount >= 10) || (u8DecR2TimerFailCount >= 10) )//Checking if rebooting Decoder R2
    {
        AUD_DBGMSG(printf("u8DecWhileFailCount = %x\n",(MS_U16)u8DecR2WhileFailCount));
        AUD_DBGMSG(printf("u8DecTimerFailCount = %x\n",(MS_U16)u8DecR2TimerFailCount));
        u8DecR2WhileFailCount = 0;
        u8DecR2TimerFailCount = 0;

        MDrv_AUDIO_DspReboot(1);

        if(!MDrv_AUDIO_IsDTV())
        {
            //msAPI_AuWriteSIFThresholdTbl();
        }

        revive = 1;
    }


    // save status
    u8LastDecR2While = u8CurrentDecR2While;
    u8LastDecR2Timer = u8CurrentDecR2Timer;
    u8LastSeWhile = u8CurrentSeWhile;
    u8LastSeISR = u8CurrentSeISR;
    u8LastSndR2While = u8CurrentSndR2While;
    u8LastSndR2Timer = u8CurrentSndR2Timer;

    return(revive);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetOutConnectivity()
/// @brief \b Function \b Description:  Set the TV output connectivity information from APP layer
///                                                   to audio driver .  Then audio driver will set the audio output
///                                                   port with these information automatically.
///////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetOutConnectivity(void)
{
    AUDIO_PATH_GROUP_TYPE Audio_pathgroup_status;

    APIAUDIO_CHECK_SHM_INIT;

    Audio_pathgroup_status = MDrv_AUDIO_GetPathGroup();

    if (Audio_pathgroup_status == AUDIO_PATH_GROUP_1)
    {   // used in U3, T2, T4, Janus,
        if ( g_AudioVars2->AudioOutputInfo.SpeakerOut==AUDIO_I2S_OUTPUT )   // I2S Amp.
        {
            MDrv_AUDIO_SetOutputPath(AUDIO_PATH_MAIN, AUDIO_I2S_OUTPUT);
        }
        else                                                                               // Analog Amp
        {
            MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.SpeakerOut, g_AudioVars2->AudioOutputInfo.SpeakerOut);
            MDrv_AUDIO_SetInputPath(AUDIO_SRC_INPUT, AUDIO_PATH_0);
        }

        if ( g_AudioVars2->AudioOutputInfo.HpOut != AUDIO_NULL_OUTPUT )
        {
            MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.HpOut, g_AudioVars2->AudioOutputInfo.HpOut);
        }

        if ( g_AudioVars2->AudioOutputInfo.MonitorOut != AUDIO_NULL_OUTPUT )
        {
            MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.MonitorOut, g_AudioVars2->AudioOutputInfo.MonitorOut);
        }

        if ( g_AudioVars2->AudioOutputInfo.ScartOut != AUDIO_NULL_OUTPUT )
        {
            MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.ScartOut, g_AudioVars2->AudioOutputInfo.ScartOut);
        }
        MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.SpdifOut, AUDIO_SPDIF_OUTPUT);
    }
    else
    {   // used in U4, T3, T8, T9, T11, T12, T13, J2, A1, A2, A5,
     //Set path for Speaker out
        if (  g_AudioVars2->eAudioSource  == E_AUDIO_INFO_KTV_IN )
        {
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.SpeakerOut);
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.HpOut);
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.MonitorOut);
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.ScartOut);
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.SpdifOut);
        }
        else  // Normal case
        {
     //Set path for Speaker out
            if( (g_AudioVars2->AudioOutputSourceInfo.SpeakerOut == E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT)) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.SpeakerOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM_SE, g_AudioVars2->AudioOutputInfo.SpeakerOut);
            }
     //Set path for HP out
            if ( (g_AudioVars2->AudioOutputSourceInfo.HpOut == E_CONNECT_SUB) && ( g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT) ) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.HpOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM, g_AudioVars2->AudioOutputInfo.HpOut);
            }
     //Set path for Monitor out
            if ( (g_AudioVars2->AudioOutputSourceInfo.MonitorOut == E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT) ) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.MonitorOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM_DELAY, g_AudioVars2->AudioOutputInfo.MonitorOut);
            }
     //Set path for Scart out
            if ( (g_AudioVars2->AudioOutputSourceInfo.ScartOut == E_CONNECT_SUB) || (g_AudioVars2->AudioOutputSourceInfo.ScartOut == E_CONNECT_SCART) )
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.ScartOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM_DELAY, g_AudioVars2->AudioOutputInfo.ScartOut);
            }
     //Set path for SPDIF out
            if ( (g_AudioVars2->AudioOutputSourceInfo.SpdifOut == E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT) ) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.SpdifOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM, g_AudioVars2->AudioOutputInfo.SpdifOut); // For new mixer mode . AD sound is mixed in Inter_PCM
            }
     //Set path for ARC out
            if ( (g_AudioVars2->AudioOutputSourceInfo.ArcOut== E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT) ) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.ArcOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM, g_AudioVars2->AudioOutputInfo.ArcOut);
            }

            if( (g_AudioVars2->AudioOutputSourceInfo.HDMIOut == E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT)) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.HDMIOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM_SE, g_AudioVars2->AudioOutputInfo.HDMIOut);
            }

            MDrv_AUDIO_SetInternalPath(INTERNAL_LRDIV2, AUDIO_I2S2_OUTPUT);
        }
    }

    MDrv_DBXTV_SetVolPath(g_AudioVars2->AudioOutputInfo.SpeakerOut);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetOutputInfo()
/// @brief \b Function \b Description:  Get audio output information from APP
/// @param pout_info   \b : Audio output path information structure pointer.
///                                    Please refer  AUDIO_OUT_INFO   in header file
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info)
{
    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->AudioOutputInfo.SpeakerOut = pout_info->SpeakerOut;
    g_AudioVars2->AudioOutputInfo.HpOut = pout_info->HpOut;
    g_AudioVars2->AudioOutputInfo.MonitorOut = pout_info->MonitorOut;
    g_AudioVars2->AudioOutputInfo.ScartOut = pout_info->ScartOut;

    if(pout_info->SpeakerOut == AUDIO_SPDIF_OUTPUT)
    {
        g_AudioVars2->AudioOutputInfo.SpdifOut = AUDIO_NULL_OUTPUT;
    }
    else
    {
    g_AudioVars2->AudioOutputInfo.SpdifOut = AUDIO_SPDIF_OUTPUT;
    }

    g_AudioVars2->AudioOutputInfo.ArcOut = AUDIO_HDMI_ARC_OUTPUT;
    g_AudioVars2->AudioOutputInfo.HDMIOut = AUDIO_HDMI_OUTPUT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSystemInfo()
/// @brief \b Function \b Description:  Function for Driver layer to get the system compile option
///                                                  (TV system , Audio system ....)
/// @param pau_info    \b : Audio output path information structure pointer
///                                    Please refer  AUDIO_INIT_INFO   in header file
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSystemInfo(AUDIO_INIT_INFO *pau_info)
{
    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->AudioSystemInfo.tv_system = pau_info->tv_system;
    g_AudioVars2->AudioSystemInfo.dtv_system = pau_info->dtv_system;
    g_AudioVars2->AudioSystemInfo.au_system_sel = pau_info->au_system_sel;
    g_AudioVars2->AudioSystemInfo.miu = pau_info->miu;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPathInfo()
/// @brief \b Function \b Description:  Get audio output path information from APP
/// @param path_info   \b : path information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetPathInfo(AUDIO_PATH_INFO *path_info)
{
    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->AudioPathInfo.SpeakerOut=path_info->SpeakerOut;
    g_AudioVars2->AudioPathInfo.HpOut=path_info->HpOut;
    g_AudioVars2->AudioPathInfo.MonitorOut=path_info->MonitorOut;
    g_AudioVars2->AudioPathInfo.ScartOut=path_info->ScartOut;
    g_AudioVars2->AudioPathInfo.SpdifOut=path_info->SpdifOut;
    g_AudioVars2->AudioPathInfo.ArcOut=AUDIO_PATH_NULL;
    g_AudioVars2->AudioPathInfo.HDMIOut= AUDIO_T3_PATH_HDMI;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetOutputSourceInfo()
/// @brief \b Function \b Description:  Set audio output source information
/// @param path_info   \b : output port source information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
{
    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->AudioOutputSourceInfo.SpeakerOut = pSourceInfo->SpeakerOut;
    g_AudioVars2->AudioOutputSourceInfo.HpOut = pSourceInfo->HpOut;
    g_AudioVars2->AudioOutputSourceInfo.MonitorOut = pSourceInfo->MonitorOut;
    g_AudioVars2->AudioOutputSourceInfo.ScartOut = pSourceInfo->ScartOut;
    g_AudioVars2->AudioOutputSourceInfo.SpdifOut = pSourceInfo->SpdifOut;
    g_AudioVars2->AudioOutputSourceInfo.ArcOut = pSourceInfo->ArcOut;
    g_AudioVars2->AudioOutputSourceInfo.HDMIOut = pSourceInfo->HDMIOut;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetOutputSourceInfo()
/// @brief \b Function \b Description:  Get audio output path information from APP
/// @param pSourceInfo   \b : output port source information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_GetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
{
    APIAUDIO_CHECK_SHM_INIT;

    memcpy(pSourceInfo, &(g_AudioVars2->AudioOutputSourceInfo), sizeof(g_AudioVars2->AudioOutputSourceInfo));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSourceInfo()
/// @brief \b Function \b Description:  For driver layer to get audio source information from APP
/// @param eSourceType   \b : information structure pointer ==>AUDIO_SOURCE_INFO_TYPE
///                         - E_AUDIO_INFO_DTV_IN,
///                         - E_AUDIO_INFO_ATV_IN,
///                         - E_AUDIO_INFO_HDMI_IN,
///                         - E_AUDIO_INFO_ADC_IN,
///                         - E_AUDIO_INFO_MM_IN,
///                         - E_AUDIO_INFO_SPDIF_IN,
///                         - E_AUDIO_INFO_KTV_IN,
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType)
{
    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->eAudioSource = eSourceType;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_InputSwitch()
/// @brief \b Function \b Description:  Set audio Input connectivity (for S7 serious only)
/// @param enSource   \b :  Please refer  AUDIO_INPUT_TYPE  enum in header file
///@param  enGroup  \b : Please refer  AUDIO_SWITCH_GROUP  enum in header file
///////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup)
{
    AUDIO_PATH_GROUP_TYPE Audio_pathgroup_status;

    APIAUDIO_CHECK_SHM_INIT;

    Audio_pathgroup_status = MDrv_AUDIO_GetPathGroup();

    if (Audio_pathgroup_status == AUDIO_PATH_GROUP_1)
    {   // used in U3, T2, T4, Janus,
        if(enGroup==E_AUDIO_GROUP_MAIN)
        {
            MDrv_AUDIO_SetInputPath(enSource , AUDIO_PATH_MAIN);
            MDrv_AUDIO_SetInputPath(enSource , g_AudioVars2->AudioPathInfo.MonitorOut);
            MDrv_AUDIO_SetInputPath(enSource , g_AudioVars2->AudioPathInfo.SpdifOut);
        }
        else  if(enGroup==E_AUDIO_GROUP_SCART)
        {
            MDrv_AUDIO_SetInputPath(enSource , g_AudioVars2->AudioPathInfo.ScartOut);
        }

        if ( g_AudioVars2->eAudioSource == E_AUDIO_INFO_KTV_IN )
        {
            AUD_DBGMSG(printf("\r\n==Source=KTV ==\r\n"));
            MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_KTV);
        }
        else if ( g_AudioVars2->eAudioSource == E_AUDIO_INFO_GAME_IN )
        {
            MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_GAME);
        }
        else
        {
            MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_DTV);
        }
    }
    else
    {   // used in U4, T3, T8, T9, T11, T12, T13, J2, A1, A2, A5,
        if(enGroup==E_AUDIO_GROUP_MAIN)
        {
            MDrv_AUDIO_SetInputPath(enSource , AUDIO_PATH_4); // fixed in this path
            g_AudioVars2->eMainAudioSource = enSource;
        }
        else  if(enGroup==E_AUDIO_GROUP_SCART)
        {
            MDrv_AUDIO_SetInputPath(enSource , AUDIO_PATH_6); //fixed in this path
            g_AudioVars2->eScartAudioSource = enSource;
        }
        else if(enGroup==E_AUDIO_GROUP_SUB)  // PIP SUB or HP source
        {
            switch(enSource)  // Use 2nd ADC in sub mode
            {
               case AUDIO_AUIN0_INPUT:
                   enSource=AUDIO_ADC2_AUIN0_INPUT;
                   break;
               case AUDIO_AUIN1_INPUT:
                   enSource=AUDIO_ADC2_AUIN1_INPUT;
                   break;
               case AUDIO_AUIN2_INPUT:
                   enSource=AUDIO_ADC2_AUIN2_INPUT;
                   break;
               case AUDIO_AUIN3_INPUT:
                   enSource=AUDIO_ADC2_AUIN3_INPUT;
                   break;
               case AUDIO_AUIN4_INPUT:
                   enSource=AUDIO_ADC2_AUIN4_INPUT;
                   break;
               case AUDIO_AUIN5_INPUT:
                   enSource=AUDIO_ADC2_AUIN5_INPUT;
                   break;
               case AUDIO_AUMIC_INPUT:
                   enSource=AUDIO_ADC2_AUMIC_INPUT;
                   break;
               default:
                   break;
            }
            MDrv_AUDIO_SetInputPath(enSource , AUDIO_PATH_5); // PIP shared the same path with Scart
            g_AudioVars2->eSubAudioSource = enSource;
        }

#if 0 //LM15 No need
        // Set output source
        switch ( g_AudioVars2->eAudioSource )
        {
            case E_AUDIO_INFO_KTV_IN:
                MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_KTV);
                break;

            case E_AUDIO_INFO_GAME_IN:
                MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_GAME);
                break;

            case E_AUDIO_INFO_KTV_STB_IN:
                MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_KTV_STB);
                break;

            case E_AUDIO_INFO_ATV_IN:
                // ATV should not effected by entertainment mode
                break;

            default:
                MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_DTV);
                break;
        }
#endif

        //Reconnect audio output port again .
        _MApi_AUDIO_SetOutConnectivity();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ReadMailBox()
/// @brief \b Function \b Description:  This routine is used to read the DSP mail box value
/// @param bDspType    \b : Select audio DEC/SE DSP
/// @param u8ParamNum  \b : Mail box address
/// @return MS_U16     \b : Mail Box value
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum)
{
    if(bDspType==DSP_DEC)
    {
        return (MDrv_AUDIO_ReadDecMailBox(u8ParamNum));
    }
    else
    {
        return (MDrv_AUDIO_ReadSeMailBox(u8ParamNum));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteDecMailBox()
/// @brief \b Function \b Description:  This routine is used to write DSP mail box
/// @param bDspType    \b : Select audio DEC/SE DSP
/// @param u8ParamNum  \b : Mail box address
/// @param u16Data     \b : value
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data)
{
    if(bDspType==DSP_DEC)
    {
        MDrv_AUDIO_WriteDecMailBox(u8ParamNum, u16Data);
    }
    else
    {
        MDrv_AUDIO_WriteSeMailBox(u8ParamNum, u16Data);
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FwTriggerDSP()
/// @brief \b Function \b Description:  This routine send a PIO interrupt to DSP with a command .
/// @param u16Cmd      \b :    0xF0-- for MHEG5 file protocol
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FwTriggerDSP(MS_U16 u16Cmd)
{
    MDrv_AUDIO_FwTriggerDSP(u16Cmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetFwStatus()
/// @brief \b Function \b Description:  FW set DSP load/reload code from DDR or Flash
/// @param bBootOnDDR  \b :
///                     - TRUE==> Boot from DDR;
///                     - FALSE==> Boot from Flash
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetFwStatus(MS_BOOL bBootOnDDR)
{
    MDrv_AUDIO_DspBootOnDDR(bBootOnDDR);
    //audio_FW_Status = bBootOnDDR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ExitAudioSystem()
/// @brief \b Function \b Description: This routine is used to exit audio system
///                                  by performing DSP power down setting.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_ExitAudioSystem(void)
{
    MDrv_AUDIO_SetPowerOn(FALSE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_RebootDsp(void)
/// @brief \b Function \b Description:  This routine reboot DSP.
/// @param bDspType    \b : 0- DEC DSP; 1- SNDEff DSP
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_RebootDsp(MS_BOOL bDspType)
{
    MDrv_AUDIO_RebootDSP(bDspType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SendIntrupt()
/// @brief \b Function \b Description: This routine send a PIO8 interrupt to DSP.
/// @param bDspType    \b :  0- DEC DSP; 1- SNDEff DSP
/// @param u8Cmd       \b :  0xE0, for MHEG5 file protocol
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U8 u8Cmd)
{
    MDrv_AUDIO_SendIntrupt(bDspType,u8Cmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_I2S_SetMode()
/// @brief \b Function \b Description:  This routine is used to Set the I2S output mode.
/// @param u8Mode      \b : I2S mode select
///                       - AUDIO_I2S_MCLK               ==>Set I2S MCLK
///                       - AUDIO_I2S_WORD_WIDTH, ==>Set I2S Word-width
///                       - AUDIO_I2S_SOURCE_CH,    ==>Set I2S channel Source
///                       - AUDIO_I2S_FORMAT            ==>Set  I2S format
/// @param u8Val      \b : I2S mode value
///                      - I2S_MCLK_64FS=0x04,               ==> MCLK=64 Fs ;
///                      - I2S_MCLK_128FS=0x05,             ==> MCLK=128 Fs ;
///                      - I2S_MCLK_256FS=0x06,             ==> MCLK=256 Fs ; default setting
///                      - I2S_MCLK_384FS=0x07,             ==> MCLK=384 Fs ;
///                      - I2S_WORD_WIDTH_16BIT=0x4,  ==>Word width=16bit;
///                      - I2S_WORD_WIDTH_24BIT=0x5,  ==>Word width=24bit;
///                      - I2S_WORD_WIDTH_32BIT=0x6,  ==>Word width=32bit; default setting
///                      - I2S_FORMAT_STANDARD=0,       ==>I2S  in Standard format
///                      - I2S_FORMAT_LEFT_JUSTIFIED=1 ==> I2S in Left-justify format
///////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_I2S_SetMode(AUDIO_I2S_MODE_TYPE u8Mode, AUDIO_I2S_MODE_VALUE  u8Val)
{
   MDrv_AUDIO_I2S_SetMode((MS_U8)u8Mode, (MS_U8)u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_WritePreInitTable()
/// @brief \b Function \b Description: This routine is to set power on DAC sequence before setting init table.
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void _MApi_AUDIO_WritePreInitTable(void)
{
    MDrv_AUDIO_InitMMIO();

    if (MDrv_AUDIO_SHM_Init() == FALSE)
    {
        APIAUDIO_ERROR("%s() : fails to init Audio SHM\n", __FUNCTION__);
        return;
    }

    if (MDrv_AUDIO_Mutex_Init() == FALSE)
    {
        APIAUDIO_ERROR("%s() : fails to init Audio Mutex\n", __FUNCTION__);
        return;
    }

    OS_OBTAIN_MUTEX(_s32AUDIOMutex, MSOS_WAIT_FOREVER);

    if (MDrv_AUDIO_GET_INIT_FLAG() == TRUE)
    {
        OS_RELEASE_MUTEX(_s32AUDIOMutex);
        APIAUDIO_PRINT("%s() : doesn't need to init again\n", __FUNCTION__);
        return;
    }

    if (g_AudioVars2->g_Audio_PreInitFlag == FALSE)
    {
        MDrv_AUDIO_SwResetMAD();
        MDrv_AUDIO_WritePreInitTable();
        g_AudioVars2->g_Audio_PreInitFlag = TRUE;
    }

    OS_RELEASE_MUTEX(_s32AUDIOMutex);

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPowerOn()
/// @brief \b Function \b Description: This routine is used to enable earphone low power stage.
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void _MApi_AUDIO_EnaEarphone_LowPower_Stage(void)
{
    MDrv_AUDIO_EnaEarphone_LowPower_Stage();
    MDrv_AUDIO_EnaEarphone_HighDriving_Stage();
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPowerOn()
/// @brief \b Function \b Description: This routine is used to enable earphone high driving stage.
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void _MApi_AUDIO_EnaEarphone_HighDriving_Stage(void)
{
    APIAUDIO_PRINT("%s() : -->>NEW POWER ON SEQ: MApi_AUDIO_EnaEarphone_HighDriving_Stage\n\r", __FUNCTION__);
    MDrv_AUDIO_EnaEarphone_HighDriving_Stage();
    return;
}

////////////////////////////////////////////////////////////////////////////////
//
//          AUDIO_DTV RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSystem()
/// @brief \b Function \b Description:  This function will load
///                                     different audio decoder to audio system.
///
///                                     In Mstar audio system, we can support 1 or 2 decoders.
///                                     One is for main DTV audio or Multimedia audio player.
///                                     Another is for second audio decode (ex: used for Audio Description purpose)
///
/// @param enDecSystem  \b : (En_DVB_decSystemType) decoder type
///                        - parameter type "MSAPI_AUD_DVB_XXXX" is decoder type for main audio decoder
///                        - parameter type "MSAPI_AUD_DVB2_XXXX" is decoder type for second audio decoder
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
{
    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return bRet;
    }

    AUD_DBGMSG(printf("apiAud>> enDecSystem %d enCurDecSystem = %d\n", (MS_U8)enDecSystem, (MS_U8)enCurDecSystem));

    if ((enDecSystem & 0xf0) == 0xb0)
    {
        bRet = MDrv_SIF_SetSystem(enDecSystem);
    }
    else
    {
        if (enDecSystem >= 0x20)
        {
            enCurDecSystem2 = enDecSystem;
        }
        else
        {
            enCurDecSystem = enDecSystem;
        }
        bRet = MDrv_AUDIO_SetSystem(enDecSystem);
    }

    return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDecodeSystem()
/// @brief \b Function \b Description:  This function will set DecId decoder as  structure p_AudioDecStatus
/// @param\b DecId  \b : Decoder ID to be controled
/// @param\b p_AudioDecStatus  \b : decoder status structure pointer
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus)
{
    APIAUDIO_CHECK_SHM_INIT;

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return bRet;
    }

    if (NULL == p_AudioDecStatus)
    {
        APIAUDIO_ERROR("%s() : AudioDecStatus is null\n", __FUNCTION__);
        return bRet;
    }

    if ((AU_DEC_INVALID == DecId) || (AU_DEC_MAX == DecId))
    {
        APIAUDIO_ERROR("Invalid Dec ID = %d,  %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return bRet;
    }

    if ((MSAPI_AUD_DVB_INVALID == p_AudioDecStatus->eAudFormat) ||
        (MSAPI_AUD_DVB_NONE == p_AudioDecStatus->eAudFormat))
    {
        APIAUDIO_ERROR("Invalid Decoder Type %s() %d\n",__FUNCTION__,__LINE__);
        return bRet;
    }

    if (FALSE == g_AudioVars2->AudioDecStatus[DecId].bConnect)
    {
        APIAUDIO_ERROR("DecID %d not connected %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return bRet;
    }

    bRet = MDrv_AUDIO_SetDecodeSystem(DecId, p_AudioDecStatus);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDecodeSystem()
/// @brief \b Function \b Description:  This function will get DecId decoder status
/// @param DecId  \b : Decoder ID to get status
/// @param p_AudioDecStatus  \b : return status to p_AudioDecStatus pointer
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_GetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus)
{
    APIAUDIO_CHECK_SHM_INIT;

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return bRet;
    }

    if (NULL == p_AudioDecStatus)
    {
        APIAUDIO_ERROR("%s() : AudioDecStatus is null\n", __FUNCTION__);
        return bRet;
    }

    if ((AU_DEC_INVALID == DecId) || (AU_DEC_MAX == DecId))
    {
        APIAUDIO_PRINT("Invalid Dec ID %d %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return bRet;
    }

    if (FALSE == g_AudioVars2->AudioDecStatus[DecId].bConnect)
    {
        APIAUDIO_PRINT("DecID %d not connected %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return bRet;
    }

    // copy Dec status
    memcpy(p_AudioDecStatus,&g_AudioVars2->AudioDecStatus[DecId], sizeof(AudioDecStatus_t));

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_OpenDecodeSystem()
/// @brief \b Function \b Description:  This function will open decode system according audio dec status
/// @param p_AudioDecStatus  \b : decoder status stucture
/// @return\b AUDIO_DEC_ID  \b : return available decoder ID
////////////////////////////////////////////////////////////////////////////////
AUDIO_DEC_ID _MApi_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus)
{
    APIAUDIO_CHECK_SHM_INIT;

    AUDIO_DEC_ID DecRet = AU_DEC_INVALID;
    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return DecRet;
    }

    if (NULL == p_AudioDecStatus)
    {
        APIAUDIO_ERROR("%s() : p_AudioDecStatus is NULL\n", __FUNCTION__);
        return DecRet;
    }

    if ((MSAPI_AUD_DVB_INVALID == p_AudioDecStatus->eAudFormat) ||
        (MSAPI_AUD_DVB_NONE == p_AudioDecStatus->eAudFormat))
    {
        APIAUDIO_ERROR("%s() %d\n",__FUNCTION__,__LINE__);
        return DecRet;
    }

    if (p_AudioDecStatus->bIsAD) // Decide AD decoder
    {
        APIAUDIO_ERROR("%s() %d\n",__FUNCTION__,__LINE__);
    }

#if 0 //LM15U: ADC input also need ID
    if ((p_AudioDecStatus->eSourceType == E_AUDIO_INFO_ADC_IN) ||
       (p_AudioDecStatus->eSourceType == E_AUDIO_INFO_SPDIF_IN))
    {
        APIAUDIO_ERROR("%s() %d\n",__FUNCTION__,__LINE__);
        return DecRet;
    }
#endif

    // Open Decode system
    DecRet = MDrv_AUDIO_OpenDecodeSystem(p_AudioDecStatus);

    if (DecRet != AU_DEC_INVALID)
    {
        u32DecoderPid_Table[DecRet] = u32Gpid; // this is for decoder/pid mapping

        printf(" Open DecId %u by Gpid %u \n",  DecRet, (unsigned int)u32Gpid);/*@audio*/

        switch (p_AudioDecStatus->eSourceType)
        {
            case E_AUDIO_INFO_ATV_IN:
                if (DecRet == AU_DEC_ID1)
                {
                    g_AudioVars2->g_u8SifDspType = DSP_DEC;
                }
                else
                {
                    g_AudioVars2->g_u8SifDspType = DSP_SE;
                }
                break;

            case E_AUDIO_INFO_HDMI_IN:
                if (DecRet == AU_DEC_ID3)
                {
                    MDrv_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE, TRUE, 0);
                }
                else
                {
                    MDrv_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE, FALSE, 0);
                }
                break;

            default:
                break;
        }

        APIAUDIO_PRINT("DecRet = %d %s() %d \n",DecRet,__FUNCTION__,__LINE__);

        // reload DSP code, and back up AudioDecStatus
        MDrv_AUDIO_SetDecodeSystem(DecRet, p_AudioDecStatus);

        if(p_AudioDecStatus->eSourceType == E_AUDIO_INFO_MM_IN)
        {
            //In LM15U ATSC model, because ATSC has no scart output
            //So if HDMI Boot up, it still remain one ADEC,
            //in this time, if go to MM, then there is no any TV Service ADEC Connect and disconnect
            //So in here, also need to disable HDMI SPDIF Auto mute function.

            //#define REG_AUDIO_MUTE_CTRL1 0x2C94
            MDrv_AUDIO_WriteMaskByte(0x2C94 + 1, 0x78, 0x00);

            switch(DecRet)
            {
                case AU_DEC_ID1:
                {
                    if(g_AudioVars2->g_R2_NonPcm_Sel == 0x00) //0x00: ADEC1
                    {
                        //if MM Input and open ID is foreground, Need to set source info to MM_IN for SPDIF output for SPDIF NPCM output
                        _MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_MM_IN);

                        //LG SPEC, MM is line mode
                        _MApi_AUDIO_SetAudioParam2(AU_DEC_ID1, Audio_R2_SHM_PARAM_DOLBY_DRC_MODE, LINE_MODE);

                        //LG SPEC, DD/DD+ is Lt/Rt, DP is LoRo in MM
                        if( (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_AC3) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_AC3P) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_MS10_DDC) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB2_AC3) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB2_AC3P))
                        {
                            _MApi_AUDIO_SetAudioParam2(AU_DEC_ID1, Audio_R2_SHM_PARAM_DOLBY_DMX_MODE, (MS_U32)DOLBY_DOWNMIX_MODE_LTRT);
                        }
                        else if( (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_AAC) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_MS10_DDT) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB2_AAC) )
                        {
                            _MApi_AUDIO_SetAudioParam2(AU_DEC_ID1, Audio_R2_SHM_PARAM_DOLBY_DMX_MODE, (MS_U32)DOLBY_DOWNMIX_MODE_LORO);
                        }
                    }
                }
                break;
                case AU_DEC_ID3:
                {
                    if(g_AudioVars2->g_R2_NonPcm_Sel == 0x01) //0x01: ADEC2
                    {
                        //if MM Input and open ID is foreground, Need to set source info to MM_IN for SPDIF output for SPDIF NPCM output
                        _MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_MM_IN);

                        //LG SPEC, MM is line mode
                        _MApi_AUDIO_SetAudioParam2(AU_DEC_ID3, Audio_R2_SHM_PARAM_DOLBY_DRC_MODE, LINE_MODE);

                        //LG SPEC, DD/DD+ is Lt/Rt, DP is Lo/Ro in MM
                        if( (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_AC3) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_AC3P) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_MS10_DDC) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB2_AC3) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB2_AC3P))
                        {
                            _MApi_AUDIO_SetAudioParam2(AU_DEC_ID3, Audio_R2_SHM_PARAM_DOLBY_DMX_MODE, (MS_U32)DOLBY_DOWNMIX_MODE_LTRT);
                        }
                        else if( (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_AAC) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_MS10_DDT) || (p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB2_AAC) )
                        {
                            _MApi_AUDIO_SetAudioParam2(AU_DEC_ID3, Audio_R2_SHM_PARAM_DOLBY_DMX_MODE, (MS_U32)DOLBY_DOWNMIX_MODE_LORO);
                        }
                    }
                }
                break;
                default:
                    break;
            }
        }

        // Update status
        g_AudioVars2->AudioDecStatus[DecRet].bConnect = TRUE;

        if (DecRet == AU_DEC_ID3)
        {
            g_AudioVars2->AudioDecStatus[DecRet].eDSPId = AUDIO_DSP_ID_SND;
        }
        else if (DecRet == AU_DEC_ID1)
        {
            g_AudioVars2->AudioDecStatus[DecRet].eDSPId = AUDIO_DSP_ID_DEC;
        }
        else if (DecRet == AU_DEC_ID2)
        {
             // AU_DEC_ID2 only for ATV SIF
             // For other case should not use AU_DEC_ID2
            // Don't know how to set for ATV case
            g_AudioVars2->AudioDecStatus[DecRet].eDSPId = AUDIO_DSP_ID_ALL;
        }

    }
    if (((g_AudioVars2->g_R2_NonPcm_Sel == 0x0) && (DecRet == AU_DEC_ID1)) ||
        ((g_AudioVars2->g_R2_NonPcm_Sel == 0x1) && (DecRet == AU_DEC_ID3)))
    {
        if ((p_AudioDecStatus->eSourceType == E_AUDIO_INFO_HDMI_IN &&  p_AudioDecStatus->eAudFormat == MSAPI_AUD_DVB_MPEG) ||
            p_AudioDecStatus->eSourceType == E_AUDIO_INFO_ADC_IN)
        {
            //printf("\n\n\033[1;32m[A][DEC:%x][META] dmx meta, src_type:%d, format:%d\033[0m\n\n", DecRet, p_AudioDecStatus->eSourceType, p_AudioDecStatus->eAudFormat);
            MDrv_AUDIO_SetAudioParam2(DecRet, Audio_ParamType_MetaData_Select, E_ELECT_DOWN_MIX);
        }
        else
        {
            //printf("\n\n\033[1;32m[A][DEC:%x][META] decoder meta, src_type:%d, format:%d\033[0m\n\n", DecRet, p_AudioDecStatus->eSourceType, p_AudioDecStatus->eAudFormat);
            MDrv_AUDIO_SetAudioParam2(DecRet, Audio_ParamType_MetaData_Select, E_SELECT_DECODER);
        }
    }
    else if (p_AudioDecStatus->eSourceType == E_AUDIO_INFO_ATV_IN)
    {
        //printf("\n\n\033[1;32m[A][DEC:%x][META] dmx meta, src_type:%d, format:%d\033[0m\n\n", DecRet, p_AudioDecStatus->eSourceType, p_AudioDecStatus->eAudFormat);
        MDrv_AUDIO_SetAudioParam2(DecRet, Audio_ParamType_MetaData_Select, E_ELECT_DOWN_MIX);
    }
    else
    {
        //printf("\n\033[1;32m[A][DEC:%x] not main decoder. don't set metadata, nonPCM_Sel:%d, src_type:%d, format:%d\033[0m\n", DecRet, g_AudioVars2->g_R2_NonPcm_Sel, p_AudioDecStatus->eSourceType, p_AudioDecStatus->eAudFormat);
        //MDrv_AUDIO_SetAudioParam2(DecRet, Audio_ParamType_MetaData_Select, E_ELECT_DOWN_MIX);
    }

    APIAUDIO_PRINT("Dec Id %d opened %s() %d \n",DecRet,__FUNCTION__,__LINE__);
    return DecRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDecodeCmd()
/// @brief \b Function \b Description:  This function will send decode command to DecId decoder
/// @param DecId  \b : Decoder ID to be controled
/// @param enDecComamnd  \b : decoder command
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd)
{
    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return bRet;
    }

    if ((AU_DEC_INVALID == DecId) || (AU_DEC_MAX == DecId))
    {
        return FALSE;
    }

    if (FALSE == g_AudioVars2->AudioDecStatus[DecId].bConnect)
    {
        APIAUDIO_PRINT("DecID %d not connected %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return FALSE;
    }

    //Convert Api dec command to Drv dec command
    AU_DVB_DECCMD u8DecCmd;
    _ApiDecCmd_To_DrvDecCmd(enDecComamnd,&u8DecCmd);

    MDrv_MAD_DecoderCmd(DecId,u8DecCmd);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ReleaseDecodeSystem()
/// @brief \b Function \b Description:  This function will release/close decode system
/// @param DecId  \b : Decoder ID to be released
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId)
{
    APIAUDIO_CHECK_SHM_INIT;

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return bRet;
    }

    if ((AU_DEC_INVALID == DecId) || (AU_DEC_MAX == DecId))
    {
        bRet = FALSE;
        return bRet;
    }

    if((u32DecoderPid_Table[DecId] == u32Gpid)&&(u32Gpid != 0))  // mapping decoder resource and process pid
    {
        // Stop when release
        printf(" Release DecID %d by Gpid %u \n", DecId, (unsigned int)u32Gpid);
        MDrv_MAD_DecoderCmd(DecId,AU_DVB_DECCMD_STOP);
        if(DecId == AU_DEC_ID1)
        {
            g_AudioVars2->g_u8DecR2Adec1DecType = MSAPI_AUD_DVB_INVALID;
        }
        else
        {
            g_AudioVars2->g_u8DecR2Adec2DecType = MSAPI_AUD_DVB_INVALID;
        }
        memset(&g_AudioVars2->AudioDecStatus[DecId],  0, sizeof(AudioDecStatus_t));
        u32DecoderPid_Table[DecId] = 0;
        bRet = TRUE;
    }
    else  // DecId is used by other process
    {
        printf(" DecID %d can't be released by Gpid %u \n", DecId, (unsigned int)u32Gpid);
        bRet = FALSE;
    }

    return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDecSysSupportStatus()
/// @brief \b Function \b Description: This function will report Audio Decoder support feature status when giving DecSystem type
/// @param <IN>        \b   enDecSystem : Decoder type
/// @param <RET>       \b   BOOL        : True or False
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
{
    return(MDrv_AUDIO_GetDecSysSupportStatus(enDecSystem));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetH264StreamID_Mod()
/// @brief \b Function \b Description: This routine is used to AC3 Stream ID mod.
/// @param stream_id_mod  \b : DTV stream ID
///                          - stream_id_mod = 0, video MPEG mode;
///                          - stream_id_mod = 1, video H264 mode
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetH264StreamID_Mod(MS_U8 stream_id_mod)
{
    //MDrv_MAD_SetH264StreanID_Mod(stream_id_mod);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_IsMadLock()
/// @brief \b Function \b Description: This function show DTV Audio stream lock status
/// @return MS_BOOL    \b :       1 : lock  ; 0 : unlock
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_IsMadLock(void)
{
    return _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DecStatus);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDecStatus()
/// @brief \b Function \b Description: This routine is used to get the decoder's status (paly or stop)
/// @return En_DVB_decCmdType \b : Decoder status (paly or stop)
////////////////////////////////////////////////////////////////////////////////
En_DVB_decCmdType _MApi_AUDIO_GetDecStatus(void)
{
    return enDecoderStatus;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetCommand()
/// @brief \b Function \b Description: Set decoder Command for Digital Audio module
/// @param enDecComamnd \b : deccoder command for DVB Audio
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetCommand(En_DVB_decCmdType enDecComamnd)
{
    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return;
    }

    switch ( enDecComamnd )
    {
        //////////////////////////////////////////////////////
        //
        //     Command for Audio decoder 1
        //
        //////////////////////////////////////////////////////
        case MSAPI_AUD_DVB_DECCMD_PLAY:
            enDecoderStatus = MSAPI_AUD_DVB_DECCMD_PLAY;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY);
            break;

        case MSAPI_AUD_DVB_DECCMD_STOP:
            enDecoderStatus = MSAPI_AUD_DVB_DECCMD_STOP;
            g_AudioVars2->g_bMMPlayFlag = FALSE;
            u8IntTag = 1;
            MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);    // Clear file mode interrupt variable
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
            break;

        case MSAPI_AUD_DVB_DECCMD_RESYNC:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_RESYNC);
            break;

        case MSAPI_AUD_DVB_DECCMD_FREE_RUN:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
            break;

        case MSAPI_AUD_DVB_DECCMD_AVSYNC:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_AVSYNC);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFILE:     // start/continue playing file based audio in MHEG5
            MDrv_AUDIO_HDMI_AC3_PathCFG(FALSE);
            g_AudioVars2->g_bMMPlayFlag = TRUE;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILE);
            break;

        case MSAPI_AUD_DVB_DECCMD_PAUSE:    // pause playing file based audio in MHEG5
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PAUSE);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFILETSP:     // start/continue playing file based audio in MHEG5
            MDrv_AUDIO_HDMI_AC3_PathCFG(FALSE);
            g_AudioVars2->g_bMMPlayFlag = TRUE;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILETSP);
            break;

        case MSAPI_AUD_DVB_DECCMD_STARTBROWSE:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_STARTBROWSE);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME:
                g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] = 0;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME_STAGE:
                g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] = 0;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME_STAGE);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME_GS:
                g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] = 0;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME_GS);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2:
            MDrv_AUDIO_HDMI_AC3_PathCFG(FALSE);
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILE2_PTS);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAY_AD:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY_AD);
            break;

        case MSAPI_AUD_DVB_DECCMD_STOP_AD:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_STOP_AD);
            break;

        case MSAPI_AUD_DVB_DECCMD_SKIP:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_SKIP);
            break;
        //////////////////////////////////////////////////////
        //
        //     Command for Audio decoder 2
        //
        //////////////////////////////////////////////////////
        case MSAPI_AUD_DVB2_DECCMD_PLAY:
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_PLAY);
            break;

        case MSAPI_AUD_DVB2_DECCMD_STOP:
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_STOP);
            break;

        case MSAPI_AUD_DVB2_DECCMD_RESYNC:
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_RESYNC);
            break;

        case MSAPI_AUD_DVB2_DECCMD_FREE_RUN:
            MDrv_MAD2_SetFreeRun(AU_DVB2_FreeRunMode_FreeRun);
            break;

        case MSAPI_AUD_DVB2_DECCMD_AVSYNC:
            MDrv_MAD2_SetFreeRun(AU_DVB2_FreeRunMode_AVsync);
            break;

        case MSAPI_AUD_DVB2_DECCMD_PLAYFILE:     // start/continue playing file based audio in MHEG5
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_PLAYFILE);
            break;

        case MSAPI_AUD_DVB2_DECCMD_PAUSE:    // pause playing file based audio in MHEG5
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_PAUSE);
            break;

        case MSAPI_AUD_DVB2_DECCMD_PLAYFILETSP:     // start/continue playing file based audio in MHEG5
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_PLAYFILETSP);
            break;

        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetCommand()
/// @brief \b Function \b Description: This routine is to get  the DVB1 decode command.
/// @return AU_DVB_DECCMD \b :
///                          - AU_DVB_DECCMD_STOP,        //0
///                         - AU_DVB_DECCMD_PLAY,        //1
///                         - AU_DVB_DECCMD_PLAYFILETSP, //2
///                          - AU_DVB_DECCMD_RESYNC,
///                                 ....etc
///////////////////////////////////////////////////////////////////////////////////////
AU_DVB_DECCMD _MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo)
{
    if (AdoDecoderNo == DVB_Audio_Decoder1)
        return MDrv_MAD_GetDecCmd();
    else
        return MDrv_MAD2_GetDecCmd();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetMAD_LOCK()
/// @brief \b Function \b Description: This routine is used to report decoder lock information.
/// @retun MS_U8      \b the decoder status
///////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_GetMAD_LOCK(void)
{
    MS_U8   lock_id_reg = 0;
    MS_BOOL mad_lock_status = FALSE;

    lock_id_reg = MDrv_MAD_Rpt_DTVES() & 0x0F;

    if (lock_id_reg == 0x0)
    {
        mad_lock_status = FALSE;
    }
    else
    {
        mad_lock_status = TRUE;
    }

    return  mad_lock_status;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetADOutputMode()
/// @brief \b Function \b Description: Set AD output mode
/// @param out_mode    \b : out_mode:
///                       - AD_OUT_SPEAKER
///                       - AD_OUT_HP
///                       - AD_OUT_BOTH
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetADOutputMode (MS_U8 out_mode)
{
    switch (out_mode)
    {
    case AD_OUT_SPEAKER:
    case AD_OUT_HP:
    case AD_OUT_BOTH:
        MDrv_MAD_SetAdMixMode (AD_MIX_MAIN, MIX_ON);   // Decoder2 mix
        break;

    case AD_OUT_NONE:
    default:
        MDrv_MAD_SetAdMixMode (AD_MIX_MAIN, MIX_OFF);  // Decoder2 don't mix
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetADAbsoluteVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of AD.
/// @param AbsVolume   \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetADAbsoluteVolume (MS_U8 AbsVolume)
{
    MDrv_SOUND_SetAdAbsoluteVolume(AbsVolume, 0);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADSetMute()
/// @brief \b Function \b Description: This routine is used to set AD mute.
/// @param EnMute      \b :     TRUE --Mute AD;
///                             FALSE--Unmute AD
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_ADSetMute(MS_BOOL EnMute)
{
    MDrv_SOUND_SetAdMute(EnMute);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_PVR RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetEncodeInit()
/// @brief \b Function \b Description:  This function is used to set Encod init
/// @param BitRate \b :   Encode Output Bitrate
///                      -  AU_DVB_ENCBR_192K,    // 0
///                      -  AU_DVB_ENCBR_384K,    // 1
///                      -  AU_DVB_ENCBR_128K,    // 2
///                      -  AU_DVB_ENCBR_64K      // 3
/// @param EncFrameNum \b :   Frame number per interrupt
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetEncInit(AU_DVB_ENCBR BitRate, MS_U8 u8EncFrameNum)
{
    MDrv_AUDIO_SetEncodeInit(BitRate, u8EncFrameNum);
}
// [LM14]
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: MApi_AUDIO_SetEncCommand()
/// @brief \b Function   \b Description: Set Encoder Command for Digital Audio module
/// @param enEnccComamnd \b : Encoder command for DVB Audio
///                                       \b    AU_DVB_ENCCMD_STOP,      //0
///                                       \b   AU_DVB_ENCCMD_START      // 1
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD enEncComamnd, void* pCB, MS_U8 u8Param)
{
    MDrv_MAD_SetEncCmd(enEncComamnd, pCB, u8Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetEncodeFrameInfo()
/// @brief \b Function \b Description:  This function is used to get the Encoded frame info
/// @param input       \b EncFrameInfo:   structure to store encoded frame info
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo)
{
    MDrv_AUDIO_GetEncodeFrameInfo(EncFrameInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetEncodeDoneFlag()
/// @brief \b Function \b Description:  This function is used to get check encode done flag
/// @param input       \b None
/// @param output      \b BOOL  TRUE/FALSE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_GetEncodeDoneFlag(void)
{
    return (MDrv_AUDIO_GetEncodeDoneFlag());
}

MS_BOOL _MApi_AUDIO_SetEncInfo(AUDIO_ENC_INFO_TYPE infoType, MS_U32 param1, MS_U32 param2)
{
    return MDrv_MAD_SetEncInfo(infoType, param1, param2);
}

MS_U32 _MApi_AUDIO_GetEncInfo(AUDIO_ENC_INFO_TYPE infoType)
{
    return MDrv_MAD_GetEncInfo(infoType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetCaptureEnable()
/// @brief \b Function \b Description:  For Samsung PCM capture
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetCaptureEnable(MS_BOOL bEnable, void* PCM_CB)
{
    MDrv_AUDIO_SetCaptureEnable(bEnable, PCM_CB);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_ATV RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetCommand()
/// @brief \b Function \b Description: Set decoder Command for ATV module
/// @param enDecComamnd \b : deccoder command for ATV AUDIO
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2)
{

    switch( enAudSifCommand )
    {
        case MSAPI_AUD_SIF_CMD_SET_STOP:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_STOP, NULL, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_PLAY:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_PLAY, NULL, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_HIDEV:
            // comm_arg1 = TRUE or FALSE.
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_HIDEV, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL:
            // comm_arg1 = MSAPI_AUD_SIF_HIDEV_FILTER_BW_L1 ~ MSAPI_AUD_SIF_HIDEV_FILTER_BW_L3 or MSAPI_AUD_SIF_HIDEV_FILTER_BW_DEFAULT
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_RESET_FC_TRACKING:
            MDrv_SIF_SendCmd(AU_SIF_CMD_RESET_FC_TRACKING, NULL, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_FC_TRACKING:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_ADC_FROM_VIF_PATH:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_ADC_FROM_VIF_PATH, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_AUTO_MUTE:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AUTO_MUTE, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_AUTO_SOUNDMODE:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AUTO_SOUNDMODE, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_BEEPER_FUNCTION:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_BEEPER_FUNCTION, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_BEEPER_TONE:
            // comm_arg1 = tone, comm_arg2 = volume
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_BEEPER_TONE, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_AGC:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AGC, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_RESET_AGC:
            MDrv_SIF_SendCmd(AU_SIF_CMD_RESET_AGC, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_STANDARD_RESET:
            MDrv_SIF_SendCmd(AU_SIF_CMD_STANDARD_RESET, NULL, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_DETECT_MAIN_STD_ONLY:
            MDrv_SIF_SendCmd(AU_SIF_CMD_DETECT_MAIN_STD_ONLY, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_FMTX_SET_PREEMP:
            MDrv_SIF_SendCmd(AU_SIF_CMD_FMTX_SET_PREEMP, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_ENC_CMD:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_ENC_CMD, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_ENC_Input_attenuation, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_ENC_Output_scaling, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_M_Gain:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_BTSC_Enc_M_Gain, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_D_Gain:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_BTSC_Enc_D_Gain, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_SAP_Gain:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_BTSC_Enc_SAP_Gain, comm_arg1, comm_arg2);
            break;
        default:
            break;
    }
}

//******************************************************************************
//Function name:    MApi_AUDIO_SIF_SetPrescale
//Passing  para:    gain_type:  MSAPI_BTSC_GAIN
//                          MSAPI_FM_StdM_GAIN
//                  db_value:   The prescale value, unit is 0.25dB.
//                                  0: 0db, 1:  0.25dB,  2:  0.5dB, ...,   4: 1.0dB, ...,   8:  2dB
//                              -1:-0.25dB, -2:-0.5dB, ..., -4:-1.0dB, ..., -8:-2dB
//Return parameter: none
//Description:          SIF output level = (original level add db_value gain)
//******************************************************************************
void _MApi_AUDIO_SIF_SetPrescale(MS_U8 gain_type, int db_value)
{
    MDrv_SIF_SetPrescale((AUDIO_SIF_GAIN_TYPE)gain_type, db_value);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_IsPALType()
/// @brief \b Function \b Description: Check the DSP type is pal_type or not
/// @return MS_BOOL    \b : TRUE/FALSE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    return MDrv_SIF_IsPALType(pal_type);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetPALType()
/// @brief \b Function \b Description: Set DSP type to pal_type
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    MDrv_SIF_SetPALType(pal_type);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetStandard()
/// @brief \b Function \b Description: Set SIF standard
/// @param system_type \b : standard type
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type)
{
    if (MDrv_SIF_GetDspCodeType() == AU_SIF_PALSUM)
    {
        if(standard_type == E_AUDIOSTANDARD_L)
        {   // disable SIF AGC for AM
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AGC, FALSE, NULL);
            MDrv_SIF_SendCmd(AU_SIF_CMD_RESET_AGC, NULL, NULL);
        }
        else
        {
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AGC, TRUE, NULL);
        }
    }

    switch(standard_type)
    {
    case E_AUDIOSTANDARD_BG:
    case E_AUDIOSTANDARD_BG_A2:
        MDrv_SIF_SetStandardType(AU_SYS_BG_A2);
        break;

    case E_AUDIOSTANDARD_BG_NICAM:
        MDrv_SIF_SetStandardType(AU_SYS_BG_NICAM);
        break;

    case E_AUDIOSTANDARD_I:
        MDrv_SIF_SetStandardType(AU_SYS_I_NICAM);
        break;

    case E_AUDIOSTANDARD_DK:
    case E_AUDIOSTANDARD_DK1_A2:
        MDrv_SIF_SetStandardType(AU_SYS_DK1_A2);
        break;

    case E_AUDIOSTANDARD_DK2_A2:
        MDrv_SIF_SetStandardType(AU_SYS_DK2_A2);
        break;

    case E_AUDIOSTANDARD_DK3_A2:
        MDrv_SIF_SetStandardType(AU_SYS_DK3_A2);
        break;

    case E_AUDIOSTANDARD_DK_NICAM:
        MDrv_SIF_SetStandardType(AU_SYS_DK_NICAM);
        break;

    case E_AUDIOSTANDARD_L:
        MDrv_SIF_SetStandardType(AU_SYS_L_NICAM);
        break;

    case E_AUDIOSTANDARD_M:
    case E_AUDIOSTANDARD_M_A2:
        // MDrv_SIF_ReLoadCode(AU_SIF_PALSUM);
        MDrv_SIF_SetStandardType(AU_SYS_M_A2);
        break;

    case E_AUDIOSTANDARD_M_BTSC:
        //MDrv_SIF_ReLoadCode(AU_SIF_BTSC);
        MDrv_SIF_SetStandardType(AU_SYS_M_BTSC);
        break;

    case E_AUDIOSTANDARD_M_EIA_J:
        MDrv_SIF_ReLoadCode(AU_SIF_EIAJ);
        MDrv_SIF_SetStandardType(AU_SYS_M_EIAJ);
        break;

    default:
        return;
    }

    m_eSifStandard = standard_type;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetSoundMode()
/// @brief \b Function \b Description: Set sound mode for ATV source.
/// @param u8SifSoundMode  \b : sound mode type
///                           - AU_MODE_MONO
///                           - AU_MODE_STEREO
///                           - AU_MODE_SAP
///                           - AU_MODE_LANG_A
///                           - AU_MODE_LANG_B
///                           - AU_MODE_NICAM_MONO, ...etc
/// @return MS_BOOL    \b : success / fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode)
{
   switch(u8SifSoundMode)
    {
        case E_AUDIOMODE_MONO:
        case E_AUDIOMODE_FORCED_MONO:
            MDrv_SIF_SetSoundMode(AU_MODE_MONO);
            break;

        case E_AUDIOMODE_G_STEREO:
        case E_AUDIOMODE_K_STEREO:
            MDrv_SIF_SetSoundMode(AU_MODE_STEREO);
            break;

        case E_AUDIOMODE_MONO_SAP:
        case E_AUDIOMODE_STEREO_SAP:
            MDrv_SIF_SetSoundMode(AU_MODE_SAP);
            break;

        case E_AUDIOMODE_DUAL_A:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_A);
            break;

        case E_AUDIOMODE_DUAL_B:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_B);
            break;

        case E_AUDIOMODE_DUAL_AB:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_AB);
            break;

        case E_AUDIOMODE_NICAM_MONO:
            MDrv_SIF_SetSoundMode(AU_MODE_NICAM_MONO);
            break;

        case E_AUDIOMODE_NICAM_STEREO:
            MDrv_SIF_SetSoundMode(AU_MODE_STEREO);
            break;

        case E_AUDIOMODE_NICAM_DUAL_A:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_A);
            break;

        case E_AUDIOMODE_NICAM_DUAL_B:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_B);
            break;

        case E_AUDIOMODE_NICAM_DUAL_AB:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_AB);
            break;

        case E_AUDIOMODE_LEFT_LEFT:
            MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, MSAPI_AUD_MODE_LL, 0);
            break;

        case E_AUDIOMODE_RIGHT_RIGHT:
            MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, MSAPI_AUD_MODE_RR, 0);
            break;

        case E_AUDIOMODE_LEFT_RIGHT:
            MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, MSAPI_AUD_MODE_STEREO, 0);
            break;
        default:
            break;

    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_GetSoundMode()
/// @brief \b Function \b Description: Get SIF BTSC sound mode type
/// @return MS_U8      \b : Sound mode type
///                       - MS_U8 = 0x0 : Mute
///                       - bit 0 : Mono
///                       - bit 1 : Stereo
///                       - bit 2 : SAP
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_SIF_GetSoundMode(void)
{
    AUDIOSTATUS eCurrentAudioStatus;

    if((m_eSifStandard ==E_AUDIOSTANDARD_M_A2)||(m_eSifStandard ==E_AUDIOSTANDARD_M_EIA_J)||(m_eSifStandard ==E_AUDIOSTANDARD_M_BTSC))
        _MApi_AUDIO_SIF_GetAudioStatus(&eCurrentAudioStatus);

    eCurrentAudioStatus = E_STATE_AUDIO_NO_CARRIER;

    // coverity 8854
    if(m_eSifStatus & E_STATE_AUDIO_PRIMARY_CARRIER)
        eCurrentAudioStatus |= E_STATE_AUDIO_PRIMARY_CARRIER;

    if(m_eSifStatus & E_STATE_AUDIO_SECONDARY_CARRIER)
        eCurrentAudioStatus |= E_STATE_AUDIO_SECONDARY_CARRIER;

    if(m_eSifStatus & E_STATE_AUDIO_NICAM)
        eCurrentAudioStatus |= E_STATE_AUDIO_NICAM;

    if(m_eSifStatus & E_STATE_AUDIO_STEREO)
        eCurrentAudioStatus |= E_STATE_AUDIO_STEREO;

    if(m_eSifStatus & E_STATE_AUDIO_BILINGUAL)
        eCurrentAudioStatus |= E_STATE_AUDIO_BILINGUAL;

    AUD_DBGMSG(printf("\r\n MApi_AUDIO_SIF_GetSoundMode standard= %x",m_eSifStandard));
    AUD_DBGMSG(printf("\r\n eCurrentAudioStatus= %x",eCurrentAudioStatus));
    switch( m_eSifStandard )
    {
        case E_AUDIOSTANDARD_M_A2:
        case E_AUDIOSTANDARD_M_EIA_J:
            if( (eCurrentAudioStatus == E_STATE_AUDIO_PRIMARY_CARRIER) ||
                (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER)) )
            {
                return E_AUDIOMODE_MONO;
            }
            else if( eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER|E_STATE_AUDIO_STEREO) )
            {
                return E_AUDIOMODE_K_STEREO;
            }
            else if( eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_DUAL_A;
            }

            return E_AUDIOMODE_INVALID;

        case E_AUDIOSTANDARD_M_BTSC:
            if(eCurrentAudioStatus == E_STATE_AUDIO_PRIMARY_CARRIER)
            {
                return E_AUDIOMODE_MONO;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_STEREO))
            {
                return E_AUDIOMODE_K_STEREO;//E_AUDIOMODE_K_STEREO;
            }
            else if( eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_MONO_SAP;
            }
            else if( eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_STEREO|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_STEREO_SAP;
            }
            return E_AUDIOMODE_INVALID;

        default:
            if( (eCurrentAudioStatus == E_STATE_AUDIO_PRIMARY_CARRIER) ||
                (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER)) )
            {
                return E_AUDIOMODE_MONO;
            }
            else if( eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER|E_STATE_AUDIO_STEREO) )
            {
                return E_AUDIOMODE_G_STEREO;
            }
            else if( eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_DUAL_A;
            }
            else if( eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_NICAM) )
            {
                return E_AUDIOMODE_NICAM_MONO;
            }
            else if( eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_NICAM|E_STATE_AUDIO_STEREO) )
            {
                return E_AUDIOMODE_NICAM_STEREO;
            }
            else if( eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_NICAM|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_NICAM_DUAL_A;
            }

            return E_AUDIOMODE_INVALID;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_GetAudioStatus()
/// @brief \b Function \b Description: Get the current audio status.
///                                    It can be used for getting available sound modes and real-time audio standard monitor.
/// @param eAudioStatus \b : audio status pointer
/// @return MS_BOOL     \b : TRUE/FALSE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus)
{
    MS_U8 ucValue;
    m_eSifStatus = E_STATE_AUDIO_NO_CARRIER;

    if (MDrv_SIF_GetDspCodeType() == AU_SIF_BTSC) {
        ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_BTSC_MODE_STATUS, NULL, NULL);

        if (ucValue & _STATUS_MOD_MONO_EXIST)
            m_eSifStatus = E_STATE_AUDIO_PRIMARY_CARRIER;
        if (ucValue & _STATUS_MOD_STEREO_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_STEREO;
        if (ucValue & _STATUS_MOD_DUAL_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;

        *eAudioStatus = m_eSifStatus;
        return TRUE;
    }

    if (MDrv_SIF_IsPALType(AU_SIF_PAL_A2)) {
        ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL);

        if( (ucValue & _STATUS_MOD_A2_PRIMARY_EXIST) != _STATUS_MOD_A2_PRIMARY_EXIST )
        {
            *eAudioStatus = E_STATE_AUDIO_NO_CARRIER;
            //debugASPPrint("E_STATE_AUDIO_NO_CARRIER\r\n",0);
            return TRUE;
        }

        m_eSifStatus = E_STATE_AUDIO_PRIMARY_CARRIER;

        if( ucValue & _STATUS_MOD_A2_SECONDARY_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_SECONDARY_CARRIER;
        else
        {
            *eAudioStatus = m_eSifStatus;
            return TRUE;
        }

        if(ucValue & _STATUS_MOD_A2_PILOT_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_PILOT;
        else
        {
                *eAudioStatus = m_eSifStatus;
                return TRUE;
        }

        if( ucValue & _STATUS_MOD_STEREO_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_STEREO;
        else if( ucValue & _STATUS_MOD_DUAL_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;

        *eAudioStatus = m_eSifStatus;

        return TRUE;
    }
    else if (MDrv_SIF_IsPALType(AU_SIF_PAL_NICAM)) {
        ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_NICAM_MODE_STATUS, NULL, NULL);

        if( (ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_NOT_EXIST )
        {
            *eAudioStatus = E_STATE_AUDIO_NO_CARRIER;
            //debugASPPrint("E_STATE_AUDIO_NO_CARRIER1\r\n",0);
            return TRUE;
        }

        m_eSifStatus = E_STATE_AUDIO_PRIMARY_CARRIER;

        if( (ucValue & _MASK_NICAM_STATUS_INFO) != _NICAM_SOUND_MODE_LOCK_STATE )
        {
            *eAudioStatus = m_eSifStatus;
            return TRUE;
        }

        m_eSifStatus |= E_STATE_AUDIO_NICAM;

        if( ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_NICAM_STEREO) ||
            ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_n_NICAM_STEREO) )
        {
            m_eSifStatus |= E_STATE_AUDIO_STEREO;
        }
        else if( ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_NICAM_DUAL) ||
                 ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_n_NICAM_DUAL) )
        {
            m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;
        }

        *eAudioStatus = m_eSifStatus;

        return TRUE;
    }
    else {
        ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL);

        if( (ucValue & _STATUS_MOD_A2_PRIMARY_EXIST) != _STATUS_MOD_A2_PRIMARY_EXIST )
        {
            *eAudioStatus = E_STATE_AUDIO_NO_CARRIER;
            //debugASPPrint("E_STATE_AUDIO_NO_CARRIER4\r\n",0);
            return TRUE;
        }

        m_eSifStatus = E_STATE_AUDIO_PRIMARY_CARRIER;

        if ( ucValue & _STATUS_MOD_A2_SECONDARY_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_SECONDARY_CARRIER;
        else
        {
            ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_NICAM_MODE_STATUS, NULL, NULL);

            if( (ucValue & _MASK_NICAM_STATUS_INFO) != _NICAM_SOUND_MODE_LOCK_STATE )
            {
                *eAudioStatus = m_eSifStatus;
                return TRUE;
            }

            m_eSifStatus |= E_STATE_AUDIO_NICAM;

            if( ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_NICAM_STEREO) ||
                ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_n_NICAM_STEREO) )
            {
                m_eSifStatus |= E_STATE_AUDIO_STEREO;
            }
            else if( ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_NICAM_DUAL) ||
                     ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_n_NICAM_DUAL) )
            {
                m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;
            }

            *eAudioStatus = m_eSifStatus;

            return TRUE;
        }

        //Add pilot, DK2, DK3 status, C.P.Chen 2008/01/03
        if (ucValue & _STATUS_MOD_A2_PILOT_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_PILOT;
        else
        {
            *eAudioStatus = m_eSifStatus;
            return TRUE;
        }

        if (ucValue & _STATUS_MOD_A2_DK2_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_DK2;
        else if( ucValue & _STATUS_MOD_A2_DK3_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_DK3;

        if (ucValue & _STATUS_MOD_STEREO_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_STEREO;
        else if( ucValue & _STATUS_MOD_DUAL_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;

        *eAudioStatus = m_eSifStatus;

        return TRUE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_StartAutoStandardDetection()
/// @brief \b Function \b Description: To start automatic detection about input audio standard
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_StartAutoStandardDetection(void)
{
    if(m_eSifStandard != E_AUDIOSTANDARD_L)
    {
        MDrv_SIF_StartAutoDetection ();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_GetResultOfAutoStandardDetection()
/// @brief \b Function \b Description: To get result of audio standard after automatic detection called by MDrv_ASP_StartAutoStandardDetection.
/// @return AUDIOSTANDARD_TYPE \b :
////////////////////////////////////////////////////////////////////////////////
AUDIOSTANDARD_TYPE _MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void)
{
    MS_U8 cResult, i=0;
    AUDIOSTANDARD_TYPE eStandard;

    cResult = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_STANDARD_STATUS, NULL, NULL);

    while(cResult==0x80)
    {
        i++;
        if(i>=40)
            break;
        MsOS_DelayTask(10);
        cResult = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_STANDARD_STATUS, NULL, NULL);
    }

    AUD_DBGMSG(printf("AutoStandardDetection Check Times = %x\r\n",i));

    if(i>=40)
    {
        //debugASPPrint("Detected Audio NO Standard ",0);
        return E_AUDIOSTANDARD_NOTSTANDARD;
    }
    //debugASPPrint("Detected Audio System:0x%x",cResult);

    switch(cResult)
    {
    case _STD_SEL_M_BTSC:
        eStandard = E_AUDIOSTANDARD_M_BTSC;
        break;

    case _STD_SEL_M_EIAJ:
        eStandard = E_AUDIOSTANDARD_M_EIA_J;
        break;

    case _STD_SEL_M_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_M_A2;
        else
            eStandard = E_AUDIOSTANDARD_M;
        break;

    case _STD_SEL_BG_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_BG_A2;
        else
            eStandard = E_AUDIOSTANDARD_BG;
        break;

    case _STD_SEL_DK1_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_DK1_A2;
        else
            eStandard = E_AUDIOSTANDARD_DK;
        break;

    case _STD_SEL_DK2_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_DK2_A2;
        else
            eStandard = E_AUDIOSTANDARD_DK;
        break;

    case _STD_SEL_DK3_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_DK3_A2;
        else
            eStandard = E_AUDIOSTANDARD_DK;
        break;

    case _STD_SEL_BG_NICAM:
        eStandard = E_AUDIOSTANDARD_BG_NICAM;
        break;

    case _STD_SEL_DK_NICAM:
        eStandard = E_AUDIOSTANDARD_DK_NICAM;
        break;

    case _STD_SEL_I_NICAM:
        eStandard = E_AUDIOSTANDARD_I;
        break;

    case _STD_SEL_L_NICAM:
        eStandard = E_AUDIOSTANDARD_L;
        break;

    default:
        eStandard = E_AUDIOSTANDARD_NOTSTANDARD;
        break;
    }

    return eStandard;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_ConvertToBasicAudioStandard()
/// @brief \b Function \b Description: Convert the audio standard to the basic standard
/// @param eStandard   \b : Please refer AUDIOSTANDARD_TYPE enum in header file
/// @return AUDIOSTANDARD_TYPE \b : basic standard
///                               - E_AUDIOSTANDARD_BG
///                               - E_AUDIOSTANDARD_I
///                               - E_AUDIOSTANDARD_DK
///                               - E_AUDIOSTANDARD_L
///                               - E_AUDIOSTANDARD_M
///                               - E_AUDIOSTANDARD_NOTSTANDARD
////////////////////////////////////////////////////////////////////////////////
AUDIOSTANDARD_TYPE _MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard)
{
    switch(eStandard)
    {
    case E_AUDIOSTANDARD_BG:
    case E_AUDIOSTANDARD_BG_A2:
    case E_AUDIOSTANDARD_BG_NICAM:
        return E_AUDIOSTANDARD_BG;

    case E_AUDIOSTANDARD_I:
        return E_AUDIOSTANDARD_I;

    case E_AUDIOSTANDARD_DK:
    case E_AUDIOSTANDARD_DK1_A2:
    case E_AUDIOSTANDARD_DK2_A2:
    case E_AUDIOSTANDARD_DK3_A2:
    case E_AUDIOSTANDARD_DK_NICAM:
        return E_AUDIOSTANDARD_DK;

    case E_AUDIOSTANDARD_L:
        return E_AUDIOSTANDARD_L;

    case E_AUDIOSTANDARD_M:
    case E_AUDIOSTANDARD_M_A2:
    case E_AUDIOSTANDARD_M_BTSC:
    case E_AUDIOSTANDARD_M_EIA_J:
        return E_AUDIOSTANDARD_M;
    default:
        break;
    }

    return E_AUDIOSTANDARD_NOTSTANDARD;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetThreshold()
/// @brief \b Function \b Description:  This routine is used to set SIF threshold
/// @param ThrTbl  \b : SIF thresholds Table
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl)
{
    MS_U8 u8DspCodeType;
    u8DspCodeType = MDrv_SIF_GetDspCodeType();

    if(u8DspCodeType == AU_SIF_BTSC)
    {
       MDrv_SIF_SetThreshold(ThrTbl, BTSC_TBL_INDEX, BTSC_TBL_LENGTH, BTSC_STANDARD);
    }
    else if(u8DspCodeType == AU_SIF_FM_RADIO)
    {
       MDrv_SIF_SetThreshold(ThrTbl, FM_RADIO_TBL_INDEX, FM_RADIO_TBL_LENGTH, FM_RADIO_STANDARD);
    }
    else
    {
        MDrv_SIF_SetThreshold(ThrTbl, A2_M_TBL_INDEX, A2_M_TBL_LENGTH, A2_M_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, A2_BG_TBL_INDEX, A2_BG_TBL_LENGTH, A2_BG_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, A2_DK_TBL_INDEX, A2_DK_TBL_LENGTH, A2_DK_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, FM_I_TBL_INDEX, FM_I_TBL_LENGTH, A2_I_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, AM_TBL_INDEX, AM_TBL_LENGTH, AM_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, NICAM_BG_TBL_INDEX, NICAM_BG_TBL_LENGTH, NICAM_BG_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, NICAM_I_TBL_INDEX, NICAM_I_TBL_LENGTH, NICAM_I_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, HIDEV_M_TBL_INDEX, HIDEV_M_TBL_LENGTH, HIDEV_M_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, HIDEV_BG_TBL_INDEX, HIDEV_BG_TBL_LENGTH, HIDEV_BG_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, HIDEV_DK_TBL_INDEX, HIDEV_DK_TBL_LENGTH, HIDEV_DK_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, HIDEV_I_TBL_INDEX, HIDEV_I_TBL_LENGTH, HIDEV_I_STANDARD);

        if (((ThrTbl[HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH].HiByteValue == 0x5A) && (ThrTbl[HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH].LowByteValue == 0x5A))  &&
            ((ThrTbl[HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH+1].HiByteValue == 0xA5) && (ThrTbl[HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH+1].LowByteValue == 0xA5)))
        {
            MS_U16 value;
            int i = HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH+2;
            int j = 0;

            // A2_M Extension threshold
            while (ThrTbl[i].HiByteValue != 0xFF)
            {
                value = ( (0x00FF & ThrTbl[i].HiByteValue) <<8)|(0x00FF & ThrTbl[i].LowByteValue);
                MDrv_SIF_AccessThreshold(A2_M_STANDARD|WRITE_THRESHOLD, A2_M_TBL_LENGTH+j, value);
                i++;
                j++;
            }
            i++;
            j = 0;
            // A2_BG Extension threshold
            while (ThrTbl[i].HiByteValue != 0xFF)
            {
                value = ( (0x00FF & ThrTbl[i].HiByteValue) <<8)|(0x00FF & ThrTbl[i].LowByteValue);
                MDrv_SIF_AccessThreshold(A2_BG_STANDARD|WRITE_THRESHOLD, A2_BG_TBL_LENGTH+j, value);
                i++;
                j++;
            }
            i++;
            j = 0;
            // A2_DK Extension threshold
            while (ThrTbl[i].HiByteValue != 0xFF)
            {
                value = ( (0x00FF & ThrTbl[i].HiByteValue) <<8)|(0x00FF & ThrTbl[i].LowByteValue);
                MDrv_SIF_AccessThreshold(A2_DK_STANDARD|WRITE_THRESHOLD, A2_DK_TBL_LENGTH+j, value);
                i++;
                j++;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//        AUDIO_SPDIF RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_HWEN()
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param spdif_en    \b :    0--Disable S/PDIF out;
///                            1--Enable S/PDIF out
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_en)
{
    MDrv_AUDIO_SPDIF_HWEN(spdif_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ARC_HWEN()
/// @brief \b Function \b Description:  This routine is used to enable ARC output (Hardware)
/// @param spdif_en    \b :    0--Disable ARC out;
///                            1--Enable ARC out
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_ARC_HWEN(MS_BOOL arc_en)
{
    MDrv_AUDIO_ARC_HWEN(arc_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ARC_HWEN_BY_PORT()
/// @brief \b Function \b Description:  This routine is used to enable ARC output by port(Hardware)
/// @param spdif_en    \b :    0--Disable ARC out;
///                            1--Enable ARC out
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_ARC_HWEN_BY_PORT(AUDIO_HDMI_INDEX_T hdmiIndex, MS_BOOL bOnOff)
{
    MDrv_AUDIO_ARC_HWEN_BY_PORT(hdmiIndex, bOnOff);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_SetMute()
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param mute_en     \b :    0--Disable mute;
///                            1--Enable mute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SPDIF_SetMute(MS_BOOL mute_en)
{
    MDrv_AUDIO_SPDIF_SetMute(mute_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF mode.
/// @param spdif_mode  \b : PCM or Non-PCM mode
///                - spdif_mode = 0x0 : SPDIF enable, PCM mode
///                - spdif_mode = 0x1 : SPDIF off
///                - spdif_mode = 0x2 : SPDIF enable, nonPCM mode
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode)
{
    MS_U8 u8Source = 0;

    APIAUDIO_CHECK_SHM_INIT;

    u8Source = (MS_U8)g_AudioVars2->eAudioSource;

    if( spdif_mode == SPDIF_OUT_NONPCM )
    {
        gSpdifOutputType = SPDIF_OUT_NONPCM;
    }
    else if( spdif_mode == SPDIF_OUT_BYPASS )
    {
        gSpdifOutputType = SPDIF_OUT_BYPASS;
    }
    else if( spdif_mode == SPDIF_OUT_TRANSCODE )
    {
        gSpdifOutputType = SPDIF_OUT_TRANSCODE;
    }
    else
    {
        gSpdifOutputType = SPDIF_OUT_PCM;
    }

    MDrv_AUDIO_SPDIF_SetMode(spdif_mode, u8Source);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_GetMode()
/// @brief \b Function \b Description:  This routine is used to get S/PDIF mode.
/// @return MS_U8      \b :  register SPDIF_CTRL value
///                       - 0x0 : SPDIF enable, PCM mode
///                       - 0x1 : SPDIF OFF
///                       - 0x2 : SPDIF enable, non-PCM mode
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_SPDIF_GetMode(void)
{
    return(MDrv_AUDIO_SPDIF_GetMode());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_SetSCMS()
/// @brief \b Function \b Description:  This routine is used to set SPDIF SCMS.
/// @param C_bit_en  \b : copy right control bit
/// @param L_bit_en  \b : generation bit
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en)
{
    MDrv_AUDIO_SPDIF_SetSCMS(C_bit_en, L_bit_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_GetSCMS()
/// @brief \b Function \b Description:  This routine is used to get SPDIF SCMS.
/// @return MS_U8      \b :  SCMS[0] = C bit status, SCMS[1] = L bit status
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_SPDIF_GetSCMS(void)
{
    return(MDrv_AUDIO_SPDIF_GetSCMS());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_ChannelStatus_CTRL()
/// @brief \b Function \b Description:
/// @param pau_info    \b :  AUDIO INIT STRUCTURE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
{

    return(MDrv_AUDIO_SPDIF_ChannelStatus_CTRL(cs_mode, status));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_Audio_SPDIF_Monitor()
/// @brief \b Function \b Description:
/// @param pau_info    \b :  AUDIO INIT STRUCTURE
////////////////////////////////////////////////////////////////////////////////
void _MApi_Audio_SPDIF_Monitor(void)
{
    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return;
    }

    if (g_bDSPLoadCode== TRUE)
    {
        return;
    }

    MDrv_AUD_Monitor_SPDIF_NONPCM_SmpRate();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Digital_Out_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPDIF/HDMI/ARC channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    return(Mdrv_AUDIO_DigitalOut_SetChannelStatus(ePath, stChannelStatus));
}

MS_BOOL _MApi_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    return (MDrv_AUDIO_DigitalOut_GetChannelStatus(ePath, stChannelStatus));
}

////////////////////////////////////////////////////////////////////////////////
//
//         AUDIO_HDMI RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_Tx_SetMute()
/// @brief \b Function \b Description:  This routine is used to enable HDMI Tx output (Software)
/// @param mute_en     \b :    0--Disable mute;
///                            1--Enable mute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en)
{
    MDrv_AUDIO_HDMI_Tx_SetMute(mute_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_Tx_GetStatus()
/// @brief \b Function \b Description:  This routine is used to get HDMI Tx status
/// @param onOff     \b : HDMI tx function is availible in this chip ?
/// @param hdmi_SmpFreq \b : Audio current sample freq for tx
/// @param outType \b   : PCM mode or nonPCM mode
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
{
    if ((onOff == NULL) || (hdmi_SmpFreq == NULL) || (outType == NULL))
    {
        AUD_DBGMSG(printf("%s() : Invalid parameter\n", __FUNCTION__));
        return;
    }

    MDrv_AUDIO_HDMI_Tx_GetStatus( onOff, hdmi_SmpFreq, outType );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_Monitor()
/// @brief \b Function \b Description: This function report HDMI non-PCM or PCM format (register STATUS_INPUT)
/// @return MS_U8      \b :
///                     - non-PCM --> register STATUS_INPUT[7:6]= 0x1;
///                     - PCM   --> register STATUS_INPUT[7:6]= 0x0
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_HDMI_Monitor(void)
{
    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        AUD_DBGMSG(printf("%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__));
        return bRet;
    }

    return((MS_U8)MDrv_AUDIO_HDMI_NonpcmMonitor());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_SetOutputType()
/// @brief \b Function \b Description:  To set HDMI output type independently
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType)
{
    APIAUDIO_CHECK_SHM_INIT;

    gHdmiOutputType = (outType & HDMI_OUT_NONPCM) ? HDMI_OUT_NONPCM : HDMI_OUT_PCM;
    MDrv_AUDIO_HDMI_TX_SetMode(outType, g_AudioVars2->eAudioSource);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_GetNonpcmFlag()
/// @brief \b Function \b Description: This function report HDMI non-PCM or PCM format (register STATUS_INPUT)
/// @return MS_BOOL    \b :
///                     - TRUE : non-PCM --> register STATUS_INPUT[7:6]= 0x1 ;
///                     - FALSE : PCM     --> register STATUS_INPUT[7:6]= 0x0
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_HDMI_GetNonpcmFlag(void)
{
    return(MDrv_AUDIO_HDMI_NonpcmMonitor());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_SetNonpcm()
/// @brief \b Function \b Description: This function setting HDMI non-PCM or PCM relational register
/// @param nonPCM_en   \b :    1: nonPCM; 0: PCM
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en)
{
    MDrv_AUDIO_HDMI_SetNonpcm(nonPCM_en);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_RX_SetNonpcm()
/// @brief \b Function \b Description: This function setting HDMI non-PCM or PCM relational register
/// @param nonPCM_en   \b :    1: nonPCM; 0: PCM
/// @return AUDIO_HDMI_RX_TYPE \b : Return HDMI RX Type
////////////////////////////////////////////////////////////////////////////////
AUDIO_HDMI_RX_TYPE _MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en)
{
    return(MDrv_AUDIO_HDMI_SetNonpcm(nonPCM_en));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_RX_SetNonpcm()
/// @brief \b Function \b Description: This function setting HDMI non-PCM or PCM relational register
/// @param nonPCM_en   \b :    1: nonPCM; 0: PCM
/// @return AUDIO_HDMI_RX_TYPE \b : Return HDMI RX Type
////////////////////////////////////////////////////////////////////////////////
AUDIO_HDMI_RX_TYPE _MApi_AUDIO_HDMI_RX_GetNonPCM(void)
{
    MS_U8 HDMI_PC_Status;
    HDMI_PC_Status = MDrv_AUDIO_HDMI_DolbyMonitor();

    if(FALSE == MDrv_AUDIO_HDMI_NonpcmMonitor())
    {
        return(HDMI_RX_PCM);
    }

    if (HDMI_PC_Status == 0x1)
    {
        return(HDMI_RX_DD);
    }
    else if (HDMI_PC_Status == 0xB)
    {
        return(HDMI_RX_DTS);
    }
    else if (HDMI_PC_Status == 0x15)
    {
        return(HDMI_RX_DDP);
    }
    else if (HDMI_PC_Status == 0x0)
    {
        return(HDMI_RX_PCM);
    }
    else if (HDMI_PC_Status == 0x7)
    {
        return(HDMI_RX_DP);
    }
    else
    {
        return(HDMI_RX_Other);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DTV_HDMI_CFG()
/// @brief \b Function \b Description:  Set DVB/HDMI AC3 path configure
/// @param ctrl    \b : 0: DVB2_AD path;
///                     1: HDMI path
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_DTV_HDMI_CFG(MS_U8 ctrl)
{
    MDrv_AUDIO_HDMI_AC3_PathCFG(ctrl);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_GetSynthFreq()
/// @brief \b Function \b Description:  Audio HDMI CTS-N synthesizer input signal detect.
/// @return MS_U16     \b : return freq. If no signal, return 0
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_HDMI_GetSynthFreq(void)
{
    return MDrv_AUDIO_HDMI_GetSynthFreq();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_SetDownSample()
/// @brief \b Function \b Description:  set HDMI downsample rate
/// @param ratio       \b : donwsample ratio
///                       - 00: Normal (from 1x to 1x).
///                       - 01: Down sample from 2x to 1x.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_SetDownSample(MS_U8 ratio)
{
    MDrv_AUDIO_HDMI_SetDownSample(ratio);
}





////////////////////////////////////////////////////////////////////////////////
//
//         AUDIO_SOUND RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_SND_ProcessEnable()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable)
{
    return(MDrv_SND_ProcessEnable(Type, enable));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_SND_SetParam1()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_SND_SetParam1( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2)
{
    //if(Type == Sound_SET_PARAM_DCOffet)
    //{
    //    printf("-->>NEW POWER ON SEQ: Sound_SET_PARAM_DCOffet\n\r");
    //}
    return(MDrv_SND_SetParam(Type,  param1,  param2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SOUND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_SND_GetParam1( Sound_GET_PARAM_Type Type, MS_U16 param1)
{
    return MDrv_SND_GetParam(Type, param1);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SOUND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SOUND_SetParam( Sound_SET_Type Type, MS_U32 *p_param )
{
    return MDrv_SOUND_SetParam(Type, p_param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SeInit()
/// @brief \b Function \b Description: This routine is the initialization for Audio sound effect module.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SeInit(void)
{
    MDrv_SOUND_Init();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_AbsoluteVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of audio u8Path.
/// @param u8Path      \b : for audio u8Path0 ~ u8Path6
/// @param u8Vol1      \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param u8Vol2      \b : LSB 3-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAbsoluteVolume( MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2  )
{
    MDrv_SOUND_AbsoluteVolume(u8Path, u8Vol1, u8Vol2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPreScale()
/// @brief \b Function \b Description: This routine is used to set the prescale of audio u8Path.
/// @param u8Path      \b : for audio u8Path0 ~ u8Path5
/// @param u8PreScale  \b : range from 0x01 to 0xFF , gain: -13.75db to +18db (0.125 db per step)
///                       - 0x00: disable pre-scale
///                       - 0x6F: gain = 0db
///                       - 0xFF: gain = +18db
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetPreScale( MS_U8 u8Path, MS_U8 u8PreScale )
{
    AUD_DBGMSG(printf("apiAud>> pre-scale: Path = %d, PreScale = %d\n", u8Path, u8PreScale));

    //MDrv_SOUND_SetPreScale(u8Path, u8PreScale);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)u8Path;
    u32Param[1] = (MS_U32)u8PreScale;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetPreScale, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SourceConnect()
/// @brief \b Function \b Description: This routine is used to set audio mute by processor
/// @param u8Path      \b : eProcessor: Set Main/Sub processor,
/// @param EnMute      \b : TRUE --Mute;
///                                    FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SourceConnect(AUDIO_OUTPORT_SOURCE_TYPE eConnectSourceType, MS_BOOL bConnect )
{
    MS_BOOL EnMute = FALSE;

    if (bConnect == FALSE)
    {
        EnMute = TRUE;
    }
    else
    {
        EnMute = FALSE;
    }

    //Set mute for Speaker out
    if(g_AudioVars2->AudioOutputSourceInfo.SpeakerOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.SpeakerOut, EnMute);
    }

    //Set mute for HP out
    if(g_AudioVars2->AudioOutputSourceInfo.HpOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.HpOut, EnMute);
    }

    //Set mute for Monitor out
    if(g_AudioVars2->AudioOutputSourceInfo.MonitorOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.MonitorOut, EnMute);
    }

    //Set mute for Scart out
    if(g_AudioVars2->AudioOutputSourceInfo.ScartOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.ScartOut, EnMute);
    }

    //Set mute for SPDIF out
    if(g_AudioVars2->AudioOutputSourceInfo.SpdifOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.SpdifOut, EnMute);
    }

    //Set mute for ARC out
    if(g_AudioVars2->AudioOutputSourceInfo.ArcOut== eConnectSourceType)
    {
        //MDrv_AUDIO_HDMI_Tx_SetMute( EnMute);
    }

    //Set mute for HDMI out
    if(g_AudioVars2->AudioOutputSourceInfo.HDMIOut== eConnectSourceType)
    {
        MDrv_AUDIO_HDMI_Tx_SetMute( EnMute);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetMute()
/// @brief \b Function \b Description: This routine is used to set audio path S/W mute.
/// @param u8Path      \b : for audio path0 ~ path6
/// @param EnMute      \b : TRUE --Mute;
///                         FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetMute(MS_U8 u8Path, MS_BOOL EnMute )
{
    //MDrv_SOUND_SetMute( u8Path, EnMute);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)u8Path;
    u32Param[1] = (MS_U32)EnMute;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetMute, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetBalance()
/// @brief \b Function \b Description: This routine is used to set  balance value
/// @param u8Balance   \b : balance (0~100)
///                       - 0    :  R channel is muted ...
///                       - 50   :  L & R channel output the same level...
///                       -  100 :  L channel is muted .
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetBalance( MS_U8 u8Balance )
{
    //MDrv_SOUND_SetBalance(u8Balance);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)u8Balance;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetBalance, (MS_U32*)u32Param);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableEQ()
/// @brief \b Function \b Description: This routine is used to enable EQ function.
/// @param EnEQ        \b : TRUE --Enable EQ;
///                                   FALSE--Disable EQ
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableEQ(MS_BOOL EnEQ)
{
    //MDrv_SOUND_EnableEQ(EnEQ);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)EnEQ;
    MDrv_SOUND_SetParam(Sound_SET_Type_EnableEQ, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableTone()
/// @brief \b Function \b Description: This routine is used to enable Bass/Treble function.
/// @param EnTone      \b : TRUE --Enable Bass/Treble;
///                         FALSE--Disable Bass/Treble
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableTone(MS_BOOL EnTone)
{
    //MDrv_SOUND_EnableTone(EnTone);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)EnTone;
    MDrv_SOUND_SetParam(Sound_SET_Type_EnableTone, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableAutoVolume()
/// @brief \b Function \b Description: This routine is used to enable auto volume(AVC) function.
/// @param EnAVC       \b : TRUE --Enable AVC;
///                        FALSE--Disable AVC
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableAutoVolume(MS_BOOL EnAVC)
{
    //MDrv_SOUND_EnableAutoVolume(EnAVC);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)EnAVC;
    MDrv_SOUND_SetParam(Sound_SET_Type_EnableAutoVolume, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableBalance()
/// @brief \b Function \b Description: This routine is used to enable Balance function.
/// @param EnBalance   \b : Balance Enable bits
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableBalance(MS_BOOL EnBalance)
{
    //MDrv_SOUND_EnableBalance(EnBalance);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)EnBalance;
    MDrv_SOUND_SetParam(Sound_SET_Type_EnableBalance, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableSurround()
/// @brief \b Function \b Description: This routine is used to enable surround function.
/// @param EnSurround   \b : TRUE --Enable Surround;
///                          FALSE--Disable Surround
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableSurround(MS_BOOL EnSurround)
{
    //MDrv_SOUND_EnableSurround(EnSurround);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)EnSurround;
    MDrv_SOUND_SetParam(Sound_SET_Type_EnableSurround, (MS_U32*)u32Param);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDynamicBass()
/// @brief \b Function \b Description: This routine is used to set DynamicBass level.
/// @param u8Bass      \b :
///
///
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetDynamicBass( MS_U8 u8DynamicBass )
{
      MDrv_SOUND_SetDynamicBass(u8DynamicBass);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetBass()
/// @brief \b Function \b Description: This routine is used to set Bass level.
/// @param u8Bass      \b :Bass Level (0~100)
///                       - CHIP_FAMILY_U3 & S4LE==>mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7 ==>mapping to -12 ~ +12 dB Gain
///                       - CHIP_FAMILY_S7J       ==> mapping to -16 ~ +15 dB Gain
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetBass( MS_U8 u8Bass )
{
    //MDrv_SOUND_SetBass(u8Bass);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)u8Bass;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetBass, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_AbsoluteBass()
/// @brief \b Function \b Description: This routine is used to set abs. Bass value.
/// @param u8Bass      \b : register value
///                       - CHIP_FAMILY_U3 & S4LE ==>mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7 ==>mapping to -12 ~ +12 dB Gain
///                       - CHIP_FAMILY_S7J       ==> mapping to -16 ~ +15 dB Gain
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_AbsoluteBass(MS_U8 u8Bass)
{
    //MDrv_SOUND_AbsoluteBass(u8Bass);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)u8Bass;
    MDrv_SOUND_SetParam(Sound_SET_Type_AbsoluteBass, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetTreble()
/// @brief \b Function \b Description: This routine is used to set Treble level.
/// @param u8Treble    \b :Treble Level (0~100)
///                       - CHIP_FAMILY_U3 & S4LE ==>mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7 ==>mapping to -12 ~ +12 dB Gain
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetTreble ( MS_U8 u8Treble )
{
    //MDrv_SOUND_SetTreble(u8Treble);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)u8Treble;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetTreble, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_AbsoluteTreble()
/// @brief \b Function \b Description: This routine is used to set abs. Treble value.
/// @param u8Treble    \b : register value
///                       - CHIP_FAMILY_U3 & S4LE ==> mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7        ==> mapping to -12 ~ +12 dB Gain
///                       - CHIP_FAMILY_S7J       ==> mapping to -16 ~ +15 dB Gain
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_AbsoluteTreble( MS_U8 u8Treble )
{
    //MDrv_SOUND_AbsoluteTreble(u8Treble);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)u8Treble;
    MDrv_SOUND_SetParam(Sound_SET_Type_AbsoluteTreble, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetEq()
/// @brief \b Function \b Description: This routine is used to set the the 5-band EQ level  .
/// @param u8band      \b :  EQ band 0~4
/// @param u8level     \b :  Absolute EQ register value  ;
///                       - 0011-0000: +12.00db. (Max)
///                       - 0000-0001: +0.25db.
///                       - 0000-0000: +0.00db.
///                       - 1111-1111: -0.25db.
///                       - 1101-0000: -12.00db. (Min)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetEq(MS_U8 u8band, MS_U8 u8level)
{
    //MDrv_SOUND_SetEq(u8band,  u8level);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)u8band;
    u32Param[1] = (MS_U32)u8level;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetEq, (MS_U32*)u32Param);
}


#if 0
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetEq_7band()
/// @brief \b Function \b Description: This routine is used to set the the 7-band EQ level  .
/// @param u8band      \b :  EQ band 0~6
/// @param u8level     \b :  Absolute EQ register value  ;
///                          0011-0000: +12.00db. (Max)
///                          0000-0001: +0.25db.
///                          0000-0000: +0.00db.
///                          1111-1111: -0.25db.
///                          1101-0000: -12.00db. (Min)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetEq_7band(MS_U8 u8band, MS_U8 u8level)
{
    APIAUDIO_DBG("[%s] \n", __FUNCTION__);

    //MDrv_SOUND_SetEq7(u8band,  u8level);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)u8band;
    u32Param[1] = (MS_U32)u8level;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetEq_7band, (MS_U32*)u32Param);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcThreshold()
/// @brief \b Function \b Description: This routine is used to set the AVC  threshold level.
/// @param Level       \b :  AVC threshold level
///                       - 0x00--  0    dBFS
///                       - 0x01--  -0.5    dBFS
///                       - 0x20--  -16    dBFS
///                       - 0x50--  -40    dBFS
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAvcThreshold(MS_U8 Level)
{
    //MDrv_SOUND_SetAVCThreshold(Level);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)Level;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetAvcThreshold, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcMode()
/// @brief \b Function \b Description: This routine is used to set the AVC mode.
/// @param <IN>        \b u8AvcMode    :  AVC u8Mode
///                                    - 0: L mode
///                                    - 1: S mode
///                                    - 2: M mode
/// @image html L_mode.JPG "L mode"
/// @image html S_mode.JPG "S mode"
/// @image html M_mode.JPG "M mode"
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAvcMode( MS_U8 AvcMode )
{
    //MDrv_SOUND_SetAvcMode( AvcMode );
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)AvcMode;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetAvcMode, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcAT()
/// @brief \b Function \b Description: This routine is used to set the AVC  attack time .
/// @param AvcAT       \b :  AVC attack time
///                       - AvcAT = 0 --> 20  ms
///                       - AvcAT = 1 --> 100 ms
///                       - AvcAT = 2 --> 200 ms
///                       - AvcAT = 3 --> 1 sec
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAvcAT( MS_U8 AvcAT )
{
    //MDrv_SOUND_SetAvcAT( AvcAT );
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)AvcAT;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetAvcAT, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcRT()
/// @brief \b Function \b Description: This routine is used to set the AVC  release time .
/// @param AvcRT       \b :  AVC release time
///                       - AvcRT = 0 --> 1 sec
///                       - AvcRT = 1 --> 2 sec
///                       - AvcRT = 2 --> 100 ms
///                       - AvcRT = 3 --> 200 ms
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAvcRT( MS_U8 AvcRT )
{
    //MDrv_SOUND_SetAvcRT( AvcRT );
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)AvcRT;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetAvcRT, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetBufferProcess()
/// @brief \b Function \b Description: sets the value of audio delay time (ms)..
/// @param DelayTime   \b : 0x00~0xFA(0~250), each step=1 ms , 0~250 ms
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetBufferProcess(MS_U8 DelayTime)
{
    MDrv_SOUND_SetCH1AudioDelay(DelayTime);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundXA()
/// @brief \b Function \b Description: This routine is used to set the Surround xA echo Gain.
/// @param <IN>        \b mode    :     A Gain : register SURROUND[3:2]
///                       - 0 :    x0.1
///                       - 1 :    x0.15
///                       - 2 :    x0.2
///                       - 3 :    x0.25
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSurroundXA( MS_U8 mode )
{
    //MDrv_SOUND_SetSurroundXA(mode);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)mode;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetSurroundXA, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundXB()
/// @brief \b Function \b Description: This routine is used to set the Surround xB echo Gain.
/// @param <IN>        \b mode    :   B Gain : register SURROUND[5:4]
///                       - 0 :    x0.25
///                       - 1 :    x0.3
///                       - 2 :    x0.35
///                       - 3 :    x0.45
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSurroundXB( MS_U8 mode )
{
    //MDrv_SOUND_SetSurroundXB(mode);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)mode;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetSurroundXB, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundXK()
/// @brief \b Function \b Description: This routine is used to set the Surround xK feedback Gain.
/// @param <IN>        \b mode    :     K Gain : register SURROUND[10:8]
///                       - 0 :    x0.1
///                       - 1 :    x0.2
///                       - 2 :    x0.3
///                       - 3 :    x0.4
///                       - 4 :    x0.5
///                       - 5 :    x0.6
///                       - 6 :    x0.7
///                       - 7 :    x0.8
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSurroundXK( MS_U8 mode )
{
    //MDrv_SOUND_SetSurroundXK(mode);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)mode;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetSurroundXK, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundLPFGain()
/// @brief \b Function \b Description: This routine is used to set the Surround LPF Gain.
/// @image html surround.JPG "Surround block diagram"
/// @param <IN>        \b mode    : LPF Gain : register SURROUND[7:6]
///                       - 0 :    0 dB
///                       - 1 :    2 dB
///                       - 2 :    4 dB
///                       - 3 :    un-support
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSurroundLPFGain( MS_U8 mode )
{
    //MDrv_SOUND_SetSurroundLPFGain(mode);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)mode;
    MDrv_SOUND_SetParam(Sound_SET_Type_SetSurroundLPFGain, (MS_U32*)u32Param);
}

/////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ConvertVolumeUnit()
/// @brief \b Function \b Description: Convert UI Volume to Absolute Volume.
///                                                  valid value of UiVolume is between 0 and 100
/// @param UiVolume    \b :    UI Volume 0 -> 62, 50 -> 12, 100 -> 7
/// @param MS_U8       \b :    register value
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_ConvertVolumeUnit(MS_U8 UiVolume)
{
    MS_U8 tmpVolume = 0;

    if (UiVolume > 100)
    {
        tmpVolume = 7;
    }
    else if (UiVolume >= 50)
    {
        tmpVolume = (12 - ((UiVolume-50)/10));
    }
    else
    {
        tmpVolume = (12 + (50 - UiVolume));
    }

    return tmpVolume;
}

////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_ProcessEnable
/// @brief \b Function \b Description: This routine is used to enable process of advanced sound effect.
/// @param <IN>        \b ADVSND_TYPE type
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ADVSOUND_ProcessEnable(ADVSND_TYPE type)
{
    return MDrv_ADVSOUND_ProcessEnable(type);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, BOOL enable)
/// @brief \b Function \b Description: This routine is used to enable subprocess of advanced sound effect.
/// @param <IN>        \b proc, enable
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable)
{
    return MDrv_ADVSOUND_SubProcessEnable(proc, enable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, U16 value1, U16 value2)
/// @brief \b Function \b Description: This routine is used to set paramters advanced sound effect.
/// @param <IN>        \b param, value1, value2
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U16 u16value1, MS_U16 u16value2)
{
    return MDrv_ADVSOUND_SetParam(param, (MS_U32)u16value1, u16value2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, U32 value1, U16 value2)
/// @brief \b Function \b Description: This routine is used to set paramters advanced sound effect.
/// @param <IN>        \b param, u32value1, u16value2
///                              \b Parameter u32value1: range 0~0x7FFFFF
///                              \b Parameter u16value2: 0/1,
///                              - 0 : if u32value1 is u16, none fix point 24 bit parameter
///                              - 1 : if u32value1 is u32, fix point 24 bit parameter
/// @param <OUT>       \b NONE  :
/// @return <OUT>       \b FALSE : Setting Fail
///                               \b TRUE  : Setting OK
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ADVSND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
{
    return MDrv_ADVSOUND_SetParam(param, u32value1, u16value2);
}

// [LM14]
MS_BOOL _MApi_AUDIO_ADVSOUND_SetParam2(ADVSND_PARAM param, MS_U32 index, MS_U32 *u32value1, MS_U16 u16value2)
{
    u16value2 = u16value2 * 4;
    index = index*4;
    return MDrv_ADVSOUND_SetParam2(param,index, u32value1, u16value2);
}

MS_BOOL _MApi_AUDIO_ADVSOUND_GetParam2(ADVSND_PARAM param, MS_U32 index, MS_U32 *u32value1, MS_U16 u16value2)
{
    u16value2 = u16value2 * 4;
    index = index*4;
    return MDrv_ADVSOUND_GetParam2(param,index, u32value1, u16value2);
}

void MApi_AUDIO_LGSE_GetParam2(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
{
        switch ( param )
        {
            case LGSEFN009_VARIABLES:
                LGSEVolumeSettings.LGSEFN009_pParams = u32value1;
                LGSEVolumeSettings.LGSEFN009_noParam = u16value2;
                //printf("In %s, line(%d), u32value1 = 0x%lx, u16value2 = 0x%x\r\n", __FUNCTION__, __LINE__, u32value1, u16value2);
                break;

            case LGSEFN_MODE_REGISTER:
                LGSEVolumeSettings.LGSEMode_pParams = u32value1;
                LGSEVolumeSettings.LGSEMode_noParam= u16value2;
                //printf("In %s, line(%d), u32value1 = 0x%lx, u16value2 = 0x%x\r\n", __FUNCTION__, __LINE__, u32value1, u16value2);
                break;

            default:
                break;
        }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_DBXTV_SetMode(DBXTV_TotSonMode TotSonMode, DBXTV_TotVolMode TotVolMode, DBXTV_TotSurMode TotSurMode)
/// @brief \b Function \b Description: This routine is used to set dbx TV mode
/// @param <IN>        \b param, TotSonMode, TotVolMode, TotSurMode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable)
{
     MDrv_DBXTV_SetMode(totsonmode, totvolmode, totsurmode,enable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_GetInfo()
/// @brief \b Function \b Description: This routine is used for Get ADV Sound info
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_ADVSOUND_GetInfo( AdvSound_GET_Type Type)
{
    return (MDrv_ADVSOUND_GetInfo(Type));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_AseSetBinAddress()
/// @brief \b Function \b Description:  This function is used to assign Bin address of advanced sound effect
/// @param u8Index     \b : type index
/// @param BinAddr     \b : address of Bin file
////////////////////////////////////////////////////////////////////////////////
void MApi_AUDIO_AseSetBinAddress(MS_U8 u8Index, MS_U32 BinAddr)
{
    MDrv_SOUND_AseSetBinAddress(u8Index, BinAddr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAdvSndSys()
/// @brief \b Function \b Description: Check Advance sound mod and reload code
/// @param u8AdvSurrMod \b : Audio Advance Sound system mod
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAdvSndSys(MS_U8 u8AdvSurrMod)
{
     // Marked by coverity_0446
    AUD_DBGMSG(printf("apiAud chk u8AdvSurrMod/u8CurrAdvSndSys = %d / %d\n", (MS_U8)u8AdvSurrMod, u8CurrAdvSndSys));

    if ( u8AdvSurrMod != u8CurrAdvSndSys  )
    {
        // set advance surround sound system
        // 0: PL2,  1:BBE,  2: SRS  3:VDS, 4:VSPK, 5:SUPV, 6:TSHD, 7:XEN
        switch ( u8AdvSurrMod )
        {
            case MSAPI_AUD_ADVSND_BBE:
                MDrv_ADVSOUND_ProcessEnable(BBE);
                break;

            case MSAPI_AUD_ADVSND_SRS:
                MDrv_ADVSOUND_ProcessEnable(SRS_TSXT);
                break;

            case MSAPI_AUD_ADVSND_VDS:
                MDrv_ADVSOUND_ProcessEnable(DOLBY_PL2VDS);
                break;

            case MSAPI_AUD_ADVSND_VSPK:
                MDrv_ADVSOUND_ProcessEnable(DOLBY_PL2VDPK);
                break;

            case MSAPI_AUD_ADVSND_SUPV:
                MDrv_ADVSOUND_ProcessEnable(SUPER_VOICE);
                break;

            case MSAPI_AUD_ADVSND_TSHD:
                MDrv_ADVSOUND_ProcessEnable(SRS_TSHD);
                break;

            case MSAPI_AUD_ADVSND_DTS_ULTRATV:
                MDrv_ADVSOUND_ProcessEnable(DTS_ULTRATV);
                break;

            case MSAPI_AUD_ADVSND_AUDYSSEY:
                MDrv_ADVSOUND_ProcessEnable(DTS_ULTRATV);
                break;

            case MSAPI_AUD_ADVSND_DBX:
                MDrv_ADVSOUND_ProcessEnable(DBX);
                break;

            case MSAPI_AUD_ADVSND_SSHD_TSHD:
                MDrv_ADVSOUND_ProcessEnable(SRS_THEATERSOUND);
                break;

            case MSAPI_AUD_ADVSND_PURESND:
                MDrv_ADVSOUND_ProcessEnable(SRS_PURESND);
                break;

            default:
                break;
        }
        u8CurrAdvSndSys  = (En_DVB_advsndType)u8AdvSurrMod;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_VDS()
/// @brief \b Function \b Description: Dolby Virtual Surround
/// @param u8enVDS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_ADVSND_Set_Sound_Param(AdvSound_SET_Type Type, MS_U32 *pParam)
{
    MDrv_ADVSOUND_Set_Param(Type, pParam);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_VDS()
/// @brief \b Function \b Description: Dolby Virtual Surround
/// @param u8enVDS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetVDS(MS_U8 u8enVDS)
{
    //MDrv_SOUND_VDS_Enable(u8enVDS);

    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8enVDS;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_VDS_Enable, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetVSPK()
/// @brief \b Function \b Description: Dolby Virtual Speaker
/// @param u8enVSPK    \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetVSPK(MS_U8 u8enVSPK)
{
    //MDrv_SOUND_VSPK_Enable(u8enVSPK);

    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8enVSPK;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_VSPK_Enable, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSRS()
/// @brief \b Function \b Description: SRS TruSurr
/// @param u8enSRS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSRS(MS_U8 u8enSRS)
{
#if 1
    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = (u8enSRS == 0) ? 0 : 1;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_SRS_Enable, (MS_U32*)u32Param);
#else
    if(u8enSRS)
    {
        MDrv_SOUND_SRS_Enable(TRUE);
    }
    else
    {
        MDrv_SOUND_SRS_Enable(FALSE);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_SOUND_SRS_SetTsxtPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-TSXT
/// @param u8mode      \b : parameter mode
/// @param u8value     \b : parameter value
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value)
{
    //MDrv_SOUND_SRS_SetTsxtPara(u8mode, u8value);

    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8mode;
    u32Param[1] = u8value;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_SRS_SetTsxtPara, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSRSTSHD()
/// @brief \b Function \b Description: SRS TruSurround HD
/// @param u8enSRS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSRSTSHD(MS_U8 u8enTSHD)
{
#if 1
    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = (u8enTSHD == 0) ? 0 : 1;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_TSHD_Enable, (MS_U32*)u32Param);
#else
    if(u8enTSHD)
    {
        MDrv_SOUND_TSHD_Enable(TRUE);
    }
    else
    {
        MDrv_SOUND_TSHD_Enable(FALSE);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_SOUND_SRS_SetTshdPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-TSHD
/// @param u8mode      \b : parameter mode
/// @param u8value     \b : parameter value
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value)
{
    //MDrv_SOUND_SRS_SetTshdPara(u8mode, u8value);

    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8mode;
    u32Param[1] = u8value;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_SRS_SetTshdPara, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetBBE()
/// @brief \b Function \b Description: BBE Surround
/// @param u8enBBE     \b :   1: enable;  0: disable
/// @param u8BBEMode   \b :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetBBE(MS_U8 u8enBBE , MS_U8 u8BBEMode)
{
    //MDrv_SOUND_BBE_SetBbeMode(u8enBBE,u8BBEMode);
    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8enBBE;
    u32Param[1] = u8BBEMode;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_BBE_SetBbeMode, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_VSPK_WMod()
/// @brief \b Function \b Description: Dolby Virtual Speaker Wide Mod
/// @param u8VSPK_WMode \b : 0: reference mod; 1: wide mod
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_VSPK_WMod(MS_U8 u8VSPK_WMode)
{
    //MDrv_SOUND_VSPK_SetWMod(u8VSPK_WMode);
    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8VSPK_WMode;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_VSPK_SetWMod, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_VSPK_SMod()
/// @brief \b Function \b Description: Dolby Virtual Speaker Surround Mod
/// @param u8VSPK_SMode \b : 0: movie mod; 1: music mod
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_VSPK_SMod(MS_U8 u8VSPK_SMode)
{
    //MDrv_SOUND_VSPK_SetSMod(u8VSPK_SMode);
    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8VSPK_SMode;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_VSPK_SetSMod, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SRS_DC()
/// @brief \b Function \b Description: SRS Dialog Clarity
/// @param u8SRSenDC   \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SRS_DC(MS_U8 u8SRSenDC)
{
    //MDrv_SOUND_SRS_DC(u8SRSenDC);
    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8SRSenDC;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_SRS_DC, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SRS_TruBass()
/// @brief \b Function \b Description:  SRS TruBass
/// @param u8SRSenTruBass \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SRS_TruBass(MS_U8 u8SRSenTruBass)
{
    //MDrv_SOUND_SRS_TruBass(u8SRSenTruBass);
    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8SRSenTruBass;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_SRS_TruBass, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_TSHD_TruBass()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Trubass
/// @param u8TSHDenTruBass \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MApi_AUDIO_TSHD_TruBass(MS_U8 u8TSHDenTruBass)
{
    MDrv_SOUND_TSHD_TruBass(u8TSHDenTruBass);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_TSHD_Definition()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Definition
/// @param u8TSHDenDefinition \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_TSHD_Definition(MS_U8 u8TSHDenDefinition)
{
    //MDrv_SOUND_TSHD_Definition(u8TSHDenDefinition);
    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = u8TSHDenDefinition;
    MDrv_ADVSOUND_Set_Param(AdvSound_SET_Type_TSHD_Definition, (MS_U32*)u32Param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_TSHD_Clarity()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD DC
/// @param u8TSHDenClarity \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MApi_AUDIO_TSHD_Clarity(MS_U8 u8TSHDenClarity)
{
    MDrv_SOUND_TSHD_Clarity(u8TSHDenClarity);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_COPY_Parameter()
/// @brief \b Function \b Description: Copy parameter from APP level
/// @param u8enVSPK    \b :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_COPY_Parameter(ADVSND_PARAM type, MS_U32 *Parameter_ptr,MS_U32 size)
{
    MDrv_AUDIO_COPY_Parameter( type, Parameter_ptr, size);
}

////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_KTV RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetKTVInfo()
/// @brief \b Function \b Description: Set KTV parameter
/// @param modelType   \b : KTV model type
/// @param infoType    \b : KTV infomation type
/// @param param1      \b :
/// @param param2      \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
{
    return MDrv_AUDIO_SetKTVInfo (modelType, paramType, param1, param2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetKTVInfo()
/// @brief \b Function \b Description: Get KTV parameter
/// @param modelType   \b : KTV model type
/// @param infoType    \b : KTV infomation type
/// @return MS_U32     \b : return KTV value
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType)
{
    return (MDrv_AUDIO_GetKTVInfo (modelType, infoType));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetMixModeVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of mix mode.
/// @param eSourceType \b : for audio source
/// @param VolType     \b : for audio vol type
/// @param u8Vol1      \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param u8Vol2      \b : LSB 3-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2  )
{
      MDrv_SOUND_SetMixModeVolume(eSourceType, VolType, u8Vol1, u8Vol2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetMixModeMute()
/// @brief \b Function \b Description: This routine is used to set audio mix mode mute
/// @param eSourceType \b : for audio source
/// @param VolType     \b : for audio vol type
/// @param EnMute      \b : TRUE --Mute;
///                         FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute )
{
    MDrv_SOUND_SetMixModeMute(eSourceType, VolType, EnMute);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PlayMenuSound()
/// @brief \b Function \b Description: This routine is used to Play Menu Sound.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_PlayMenuSound(void)
{
     MDrv_SOUND_PlayMenuSound();
}




////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_OTHERS RELATIONAL FUNCTION (Temp)
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetCertMode()
/// @brief \b Function \b Description:
/// @param CertType    \b :
/// @param enable      \b :
///
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MApi_AUDIO_SetCertMode( Audio_CertMode CertType,Audio_CertEnable enCert )
{
    return(MDrv_AUDIO_SetCertMode(CertType, enCert));
}

/*
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetMaxClipping()
/// @brief \b Function \b Description: Limit output if volume setting is high and output is going to exceed maximum power.
///                                    It should be used only when OSD AVL option is Off.
/// @param MaxClippingValue \b : AVC threshold level for maximum volume
/// @param Volume           \b : Current volume value
////////////////////////////////////////////////////////////////////////////////
void MApi_AUDIO_SetMaxClipping( MS_U8 MaxClippingValue, MS_U16 Volume )
{
    MS_U8 value1,value2;
    MS_U16 AVL, VOL_Int, VOL_Fra;
    MS_U8 SpeakerMaxVolume = 0x0C; //(MS_U8)(TvVolumeTable[100]>>8);
    MS_U8 DynamicAvl_active_volume = 0x17; // about -12dB for 2Vrms input, value of volume control register

    value1 = (MS_U8)(Volume>>8) & 0x7F;
    value2 =(MS_U8)((Volume&0x00FF)>>4);

    // Dynamic AVL clipping algorithm
    if(value1 < DynamicAvl_active_volume)
    {
        VOL_Int= (value1 - SpeakerMaxVolume)*2;  //0.5*2 = 1
        VOL_Fra= (value2)/4;       //0.125*4 = 0.5
        if((VOL_Int + VOL_Fra) < MaxClippingValue)
            AVL = MaxClippingValue - (VOL_Int + VOL_Fra);
        else
            AVL = 0;
    }
    else  // value1 >= DynamicAvl_active_volume
        AVL = 0;

    MApi_AUDIO_SetAvcThreshold(AVL);
    //debugAudioPrint ("VOL_int [%d]\r\n", VOL_Int);
    //debugAudioPrint ("VOL_Fra [%d]\r\n", VOL_Fra);
    //debugAudioPrint ("AVL [%d]\r\n", AVL);

}
*/

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPEQCoef()
/// @brief \b Function \b Description: This routine is used to set PEQ Coefficient.
/// @param <IN>        \b peq_coef
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef)
{
    MDrv_SOUND_SetPEQCoef(peq_coef);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnablePEQ()
/// @brief \b Function \b Description: This routine is used to eable PEQ.
/// @param <IN>        \b Enpeq
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnablePEQ(MS_BOOL Enpeq)
{
    //MDrv_SOUND_EnablePEQ(Enpeq);
    MS_U32 u32Param[4] = {0, 0, 0, 0};
    u32Param[0] = (MS_U32)Enpeq;
    MDrv_SOUND_SetParam(Sound_SET_Type_EnablePEQ, (MS_U32*)u32Param);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableDcRemove()
/// @brief \b Function \b Description: This routine is used to eable PEQ.
/// @param <IN>        \b EnDcRemove
/// @param <OUT>       \b True / False
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_EnableDcRemove(MS_BOOL EnDcRemove)
{
    return MDrv_SOUND_EnableDcRemove(EnDcRemove);
}

////////////////////////////////////////////////////////////////////////////////
//
//         AUDIO_MM RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetCommAudioInfo()
/// @brief \b Function \b Description: This routine is used for adjust Common Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    return MDrv_AUDIO_SetCommAudioInfo(infoType, param1, param2);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_ADEC_SetDecodeParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_ADEC_SetDecodeParam( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32* Param )
{
    return MDrv_ADEC_SetDecodeParam(DecId, paramType, Param);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetMpegInfo()
/// @brief \b Function \b Description: This routine is used for adjust MPEG Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    //return MDrv_AUDIO_SetMpegInfo(infoType, param1, param2);

    MS_U32 u32Param[3] = {0,0,0};
    u32Param[0] = (MS_U32)infoType;
    u32Param[1] = param1;
    u32Param[2] = param2;
    return MDrv_ADEC_SetDecodeParam(AU_DEC_ID1, Audio_ParamType_setMPEGDecParam, (MS_U32*)u32Param);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetAC3Info()
/// @brief \b Function \b Description: This routine is used for adjust AC3 Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetAC3Info( Audio_AC3_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    //return MDrv_AUDIO_SetAC3Info(infoType, param1, param2);

    MS_U32 u32Param[3] = {0,0,0};
    u32Param[0] = (MS_U32)infoType;
    u32Param[1] = param1;
    u32Param[2] = param2;
    return MDrv_ADEC_SetDecodeParam(AU_DEC_ID1, Audio_ParamType_setAC3DecParam, (MS_U32*)u32Param);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetAC3PInfo()
/// @brief \b Function \b Description: This routine is used for adjust AC3+ Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    //return MDrv_AUDIO_SetAC3PInfo(infoType, param1, param2);

    MS_U32 u32Param[3] = {0,0,0};
    u32Param[0] = (MS_U32)infoType;
    u32Param[1] = param1;
    u32Param[2] = param2;
    return MDrv_ADEC_SetDecodeParam(AU_DEC_ID1, Audio_ParamType_setAC3PDecParam, (MS_U32*)u32Param);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetAACInfo()
/// @brief \b Function \b Description: This routine is used for adjust AAC Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    //return MDrv_AUDIO_SetAACInfo(infoType, param1, param2);

    MS_U32 u32Param[3] = {0,0,0};
    u32Param[0] = (MS_U32)infoType;
    u32Param[1] = param1;
    u32Param[2] = param2;
    return MDrv_ADEC_SetDecodeParam(AU_DEC_ID1, Audio_ParamType_setAACDecParam, (MS_U32*)u32Param);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetWmaInfo()
/// @brief \b Function \b Description: This routine is used for adjust WMA Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetWmaInfo( Audio_WMA_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    //return MDrv_AUDIO_SetWmaInfo(infoType, param1, param2);

    MS_U32 u32Param[3] = {0,0,0};
    u32Param[0] = (MS_U32)infoType;
    u32Param[1] = param1;
    u32Param[2] = param2;
    return MDrv_ADEC_SetDecodeParam(AU_DEC_ID1, Audio_ParamType_setWMADecParam, (MS_U32*)u32Param);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetDTSCommonCtrl()
/// @brief \b Function \b Description: This routine is used for adjust DTS Audio decoder control
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type )
{
    //return MDrv_AUDIO_SetDTSCommonCtrl(infoType, ctrl_type);

    MS_U32 u32Param[3] = {0,0,0};
    u32Param[0] = (MS_U32)infoType;
    u32Param[1] = (MS_U32)ctrl_type;
    u32Param[2] = 0;
    return MDrv_ADEC_SetDecodeParam(AU_DEC_ID1, Audio_ParamType_setDTSDecParam, (MS_U32*)u32Param);
}



//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetCommAudioInfo()
/// @brief \b Function \b Description: This routine will return the 64bit Common info according to info type
/// @return long long     \b : 64bit info
//******************************************************************************
long long _MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType )
{
    return MDrv_AUDIO_GetCommAudioInfo(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetMpegInfo()
/// @brief \b Function \b Description: This routine will return the 32bit mpeg info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType )
{
    return MDrv_AUDIO_GetMpegInfo(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetAC3Info()
/// @brief \b Function \b Description: This routine will return the 32bit ac3 info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetAC3Info( Audio_AC3_infoType infoType )
{
    return MDrv_AUDIO_GetAC3Info(infoType);
}


//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetAC3PInfo()
/// @brief \b Function \b Description: This routine will return the 32bit ac3+ info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType )
{
    return MDrv_AUDIO_GetAC3PInfo(infoType);
}


//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetAACInfo()
/// @brief \b Function \b Description: This routine will return the 32bit AAC info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetAACInfo( Audio_AAC_infoType infoType )
{
    return MDrv_AUDIO_GetAACInfo(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetWmaInfo()
/// @brief \b Function \b Description: This routine will return the 32bit wma info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType )
{
    return MDrv_AUDIO_GetWmaInfo(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetDTSInfo()
/// @brief \b Function \b Description: This routine will return the 32bit DTS info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType )
{
    return MDrv_AUDIO_GetDTSInfo(infoType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_XPCM_Param()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType      \b :    1--lpcm;
///                               2--MS-ADPCM;
///                               17--IMA-ADPCM
/// @param channels       \b :    0-- mono;
///                               1-- stereo
/// @param sampleRate     \b :    8, 11.025, 16, 22.05, 44.1 and 48 (KHz)
/// @param bitsPerSample  \b :    8--8 bits;
///                               16--16 bits
/// @param blockSize      \b :    block size
/// @param samplePerBlock \b :    samples per block
/// @return MS_U8         \b :    TRUE--parameters are correct;
///                               FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_XPCM_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    //return(MDrv_MAD_XPCM_setParam(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock));
    MS_U32 u32Param[6] = {0,0,0,0,0,0};
    u32Param[0] = (MS_U32)audioType;
    u32Param[1] = (MS_U32)channels;
    u32Param[2] = (MS_U32)sampleRate;
    u32Param[3] = (MS_U32)bitsPerSample;
    u32Param[4] = (MS_U32)blockSize;
    u32Param[5] = (MS_U32)samplePerBlock;

   return MDrv_ADEC_SetDecodeParam(AU_DEC_ID1, Audio_ParamType_setXPCMDecParam, (MS_U32*)u32Param);
}

#if 0
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_XPCM2_Param()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType      \b :    1--lpcm;
///                               2--MS-ADPCM;
///                               17--IMA-ADPCM
/// @param channels       \b :    0-- mono;
///                               1-- stereo
/// @param sampleRate     \b :    8, 11.025, 16, 22.05, 44.1 and 48 (KHz)
/// @param bitsPerSample  \b :    8--8 bits;
///                               16--16 bits
/// @param blockSize      \b :    block size
/// @param samplePerBlock \b :    samples per block
/// @return MS_U8         \b :    TRUE--parameters are correct;
///                               FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_XPCM2_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
     MDrv_MAD2_XPCM_setParam(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_XPCM2_CheckIntStatus()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value for int status by IDMA
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U16  :     sram value for interrupt status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_XPCM2_CheckIntStatus(void)
{
    return MDrv_MAD2_XPCM2_CheckIntStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_RA8_Param()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing RA8 LBR data
/// @param mNumCodecs  \b :    1--stereo or mono
///                            2~5-- multi channels
/// @param mSamples    \b :    output samples--256, 512, 1024
/// @param mSampleRate \b :    8, 11.025, 16, 22.05, 44.1 (KHz)
/// @param Channels    \b :    1-- mono
///                                    2-- stereo
/// @param cplStart    \b :    0 ~ 51
/// @param Regions     \b :    0 ~ 31
/// @param cplQbits    \b :    0, 2, 3, 4, 5, 6
/// @param FrameSize   \b :    bitstream size of every NumCodes (bytes)
///                                    Range--0 ~ 65535
/// @return MS_U8      \b :    TRUE--parameters are correct;
///                            FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate, MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits, MS_U16* FrameSize)
{
     return(MDrv_MAD_RA8_setParam(mNumCodecs, mSamples, mSampleRate, Channels, Regions, cplStart, cplQbits, FrameSize));
}

/******************************************************************************/
// Comomon Function for MM
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_Init()
/// @brief \b Function \b Description: MM Music mode Initial Audio decoder
/// @param <IN>        \b enDecSystem: Audio decoder type
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem)
{
    // need mute by APP

    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);

    u8IntTag = 1;
    MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);

    _MApi_AUDIO_SetSystem(enDecSystem);

    // need unmute by APP
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_StartDecode()
/// @brief \b Function \b Description: start play MM file
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_StartDecode(void)
{
    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);//MDrv_MAD_SetDecCmd(0x04);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_StartBrowse()
/// @brief \b Function \b Description: start browse file
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_StartBrowse(void)
{
    // decode data but output no sound
    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STARTBROWSE);//MDrv_MAD_SetDecCmd(0x05);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_StopDecode()
/// @brief \b Function \b Description: Stop decoder
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_StopDecode(void)
{
    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);// MDrv_MAD_SetDecCmd(0x00);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PauseDecode()
/// @brief \b Function \b Description: pause decoder
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_PauseDecode(void)
{
    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PAUSE);//MDrv_MAD_SetDecCmd(0x06);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_CheckPlayDone()
/// @brief \b Function \b Description: Check file play done. If file play done, will return TRUE
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U8   : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_CheckPlayDone(void)
{
    if (MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_MMResidualPCM) == 0)
    {
        _MApi_AUDIO_StopDecode();
        return TRUE;
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetResidualBufferSize()
/// @brief \b Function \b Description: Get ES buffer level.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16 : ES buffer level
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetResidualBufferSize(void)
{
    return MDrv_AUDIO_ReadDecMailBox(0) << 3; // line size is 8
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetPCMBufferSize()
/// @brief \b Function \b Description: Get MP3 PCM buffer level.
/// @param <IN>        \b u16BitRate    : Bit Rate
/// @param <OUT>       \b MS_U16 : MP3 PCM buffer level
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetPCMBufferSize(MS_U16 u16BitRate)
{
    MS_U32 u32Tmp;

    u32Tmp = (MS_U32)MDrv_AUDIO_ReadDecMailBox(1);
    // line size is 8, 1 sample 2 bytes and 2 channels (Left & Right)
    //u32Tmp = ((u32Tmp << 1) * ((U32)u16BitRate * 1000 / 8))/MDrv_MAD_MPEG_GetSampleRate();
    if (_MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_SampleRate) != 0)
        u32Tmp = ((u32Tmp << 1) * ((MS_U32)u16BitRate * 1000 / 8))/_MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_SampleRate);
    return (MS_U16)u32Tmp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetPCMBufferSize2()
/// @brief \b Function \b Description: Get PCM buffer level.
/// @param <IN>        \b u16BitRate : Bit Rate;
/// @param <IN>        \b u16SampleRate : Sample Rate
/// @param <OUT>       \b MS_U16 : PCM buffer level
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetPCMBufferSize2(MS_U16 u16BitRate,MS_U16 u16SampleRate)
{
    MS_U32 u32Tmp;

    u32Tmp = (MS_U32)MDrv_AUDIO_ReadDecMailBox(1);
    // line size is 8, 1 sample 2 bytes and 2 channels (Left & Right)
    if(u16SampleRate != 0)//UT
        u32Tmp = ((u32Tmp << 1) * ((MS_U32)u16BitRate * 1000 / 8))/u16SampleRate;

    return (MS_U16)u32Tmp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetInput()
/// @brief \b Function \b Description: This function will inform DSP that MCU already write data to ES buffer by interrupt.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetInput(void)
{
    MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DEC1_MMTag, (MS_U32)u8IntTag, 0);
    MDrv_AUDIO_SendIntrupt(DSP_DEC, 0xE0);
    u8IntTag++;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSampleRateIndex()
/// @brief \b Function \b Description: Set AAC sample rate index.
/// @param <IN>        \b u16Index    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSampleRateIndex(MS_U16 u16Index)
{
    MDrv_AUDIO_WriteDecMailBox(4,u16Index);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetXPCMParam()
/// @brief \b Function \b Description: Set XPCM paramaters
/// @param <IN>        \b audioType :
/// @param <IN>        \b channels :
/// @param <IN>        \b sampleRate :
/// @param <IN>        \b bitsPerSample :
/// @param <IN>        \b samplePerBlock :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U16 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    //MDrv_MAD_XPCM_setParam(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);
    //_MApi_AUDIO_XPCM_Param(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);
    MS_U32 u32Param[6] = {0,0,0,0,0,0};
    u32Param[0] = (MS_U32)audioType;
    u32Param[1] = (MS_U32)channels;
    u32Param[2] = (MS_U32)sampleRate;
    u32Param[3] = (MS_U32)bitsPerSample;
    u32Param[4] = (MS_U32)blockSize;
    u32Param[5] = (MS_U32)samplePerBlock;
    MDrv_ADEC_SetDecodeParam(AU_DEC_ID1, Audio_ParamType_setXPCMDecParam, (MS_U32*)u32Param);

   return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FileEndNotification()
/// @brief \b Function \b Description: This function will inform DSP that file end
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FileEndNotification(void)
{
    MDrv_AUDIO_WriteDecMailBox(6, (MS_U16)u8IntTag);
    MDrv_AUDIO_FwTriggerDSP(0xe0);        // should change to MDrv_AUDIO_FwTriggerDSP
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FileEndDataHandle()
/// @brief \b Function \b Description: Info DSP that the last ES data left size.
/// @param <IN>        \b u32DataLeft    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft)
{
    MDrv_AUDIO_WriteDecMailBox(5,(MS_U16)(u32DataLeft>>3));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_CheckInputRequest()
/// @brief \b Function \b Description: If MCU receive DSP interrupt, this function will return TRUE and report requested ES data buffer address and size
/// @param <IN>        \b pU32WrtAddr    :
/// @param <IN>        \b pU32WrtBytes   :
/// @param <OUT>       \b MS_U8          : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_CheckInputRequest(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
{
    if((pU32WrtAddr == NULL) || (pU32WrtBytes == NULL))//UT
        return FALSE;
    if (MDrv_AUDIO_GetPlayFileFlag(DSP_DEC))
    {
        MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
        *pU32WrtAddr = MDrv_AUDIO_GetDspMadBaseAddr(DSP_DEC)+_MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_BufferAddr);
        *pU32WrtBytes = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_BufferSize);
        return TRUE;
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetPlayTick()
/// @brief \b Function \b Description: Get Play Time stamp
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetPlayTick(void)
{
    return _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_1ms_PTS);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetEsMEMCnt()
/// @brief \b Function \b Description: Get ES Bufffer size
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16  : ES Bufffer size
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetEsMEMCnt(void)
{
    return MDrv_AUDIO_ReadDecMailBox(0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetSampleRate()
/// @brief \b Function \b Description: Get Sample Rate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16 : Sample Rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetSampleRate(void)
{
    MS_U16 samplerate = 0;
    switch (enCurDecSystem)
    {
        case MSAPI_AUD_DVB_MPEG:
        case MSAPI_AUD_DVB_MP3:
            samplerate = _MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_SampleRate);
            break;
        case MSAPI_AUD_DVB_WMA:
            samplerate = _MApi_AUDIO_GetWmaInfo(Audio_WMA_infoType_SampleRate);
            break;
        default:
            break;
    }
    return samplerate;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetBitRate()
/// @brief \b Function \b Description: Get Bit Rate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : Bit Rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetBitRate(void)
{
    MS_U32 bitrate = 0;
    switch (enCurDecSystem)
    {
        case MSAPI_AUD_DVB_MPEG:
        case MSAPI_AUD_DVB_MP3:
            bitrate = _MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_BitRate);
            break;
        case MSAPI_AUD_DVB_WMA:
            bitrate = _MApi_AUDIO_GetWmaInfo(Audio_WMA_infoType_BitRate);
            break;
        default:
            break;
    }
    return bitrate;
}

/******************************************************************************/
//  for MP3
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetLayer()
/// @brief \b Function \b Description: Get MP3 Layer
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b Audio_MP3_LAYER : MPEG Layer
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
Audio_MP3_LAYER _MApi_AUDIO_GetLayer(void)
{
    // MDrv_MAD_MPEG_GetLayer() return the layer of music. 1 & 2 for layer mpeg 1 & 2. 3 is for mp3
    Audio_MP3_LAYER enLayer = Audio_MP3_LAYER_NOT_DETERMINE_YET;

    switch(_MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_Layer))
    {
        case 1:
            enLayer = Audio_MP3_LAYER_1;
            break;
        case 2:
            enLayer = Audio_MP3_LAYER_2;
            break;
        case 3:
            enLayer = Audio_MP3_LAYER_3;
            break;
        default:
            break;
    }
    return(enLayer);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetCurrentFrameNumber()
/// @brief \b Function \b Description: Get MP3 Current Frame Number
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : Current Frame Number
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetCurrentFrameNumber(void)
{
    return _MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_FrameNum);
}

/******************************************************************************/
//  for WMA
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetASFParm()
/// @brief \b Function \b Description: Set WMA ASF Paramaters
/// @param <IN>        \b parm_type  :
/// @param <IN>        \b value   :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value)
{
    _MApi_AUDIO_SetWmaInfo(Audio_WMA_infoType_Asf_Param, parm_type, value);
}


/******************************************************************************/
//  for MM File Hand Shake
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM_SetInput()
/// @brief \b Function \b Description: MCU send interrupt to decoder1/2 in MM mode
/// @param <IN>        \b AUDDecoderNo  : DVB_Audio_Decoder1 / DVB_Audio_Decoder2
/// @param <IN>        \b u8IntTag   : Interrupt tag, increase by every time
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_MM_SetInput (En_DVB_AudioDecoder AUDDecoderNo, MS_U8 u8IntTag)
{
    if (AUDDecoderNo == DVB_Audio_Decoder1)
    {
        MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DEC1_MMTag, (MS_U32)u8IntTag, 0);
        MDrv_AUDIO_SendIntrupt(DSP_DEC, 0xE0);
    }
    else
    {
        MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DEC2_MMTag, (MS_U32)u8IntTag, 0);
        MDrv_AUDIO_SendIntrupt(DSP_SE, 0xE0);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM_CheckPlayDone()
/// @brief \b Function \b Description: MCU check audio play end in MM mode
/// @param <IN>        \b AUDDecoderNo  : DVB_Audio_Decoder1 / DVB_Audio_Decoder2
/// @param <OUT>       \b MS_U16    : Value of residual PCM level for file end stop
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_MM_CheckPlayDone (En_DVB_AudioDecoder AUDDecoderNo)
{
    if (AUDDecoderNo == DVB_Audio_Decoder1)
    {
        return((MS_U32)MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_MMResidualPCM));
    }
    else
    {
        return((MS_U32)MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC2_MMResidualPCM));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM_CheckInputRequest()
/// @brief \b Function \b Description  : Decoder request input addr & size in MM mode
/// @param <IN>        \b AUDDecoderNo : DVB_Audio_Decoder1 / DVB_Audio_Decoder2
/// @param <IN>        \b pU32WrtAddr  : Request input address in ES1 buffer
/// @param <IN>        \b pU32WrtBytes : Request input size
/// @param <OUT>       \b MS_U8   : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
{
    if (AUDDecoderNo == DVB_Audio_Decoder1)
    {
        if (MDrv_AUDIO_GetPlayFileFlag(DSP_DEC))
        {
            MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
            *pU32WrtAddr =  _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_esBuf_reqAddr);
            *pU32WrtBytes = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_BufferSize);
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        if (MDrv_AUDIO_GetPlayFileFlag(DSP_SE))
        {
            MDrv_AUDIO_SetPlayFileFlag(DSP_SE, 0);
            *pU32WrtAddr =  _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_esBuf_reqAddr);
            *pU32WrtBytes = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC2_BufferSize);
            return TRUE;
        }
        return FALSE;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM2_initAesInfo()
/// @brief \b Function \b Description  : Initialize AES info
/// @param <IN>        \b dec_id  : Decoder ID
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID dec_id )
{
    //return MDrv_AUDIO_MM2_initAesInfo( dec_id );

    MS_U32 u32Param[2] = {0,0};
    u32Param[0] = (MS_U32)dec_id;

    if (dec_id == AU_DEC_ID3)
    {
        return MDrv_ADEC_SetDecodeParam(AU_DEC_ID3, Audio_ParamType_MM2_initAesInfo, (MS_U32*)u32Param);
    }
    else
    {
        return MDrv_ADEC_SetDecodeParam(AU_DEC_ID1, Audio_ParamType_MM2_initAesInfo, (MS_U32*)u32Param);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM2_checkAesInfo()
/// @brief \b Function \b Description  : Get AES info from audio decoder
/// @param <IN>        \b dec_id       : Decoder ID
/// @param <IN>        \b aes_info     : AES info structure pointer
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info )
{
    return MDrv_AUDIO_MM2_checkAesInfo( dec_id, aes_info );
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM2_inputAesFinished()
/// @brief \b Function \b Description  : Inform DSP how much ES data is sent
/// @param <IN>        \b dec_id	   : Decoder ID
/// @param <IN>        \b es_size  : Size of ES data
/// @param <IN>        \b ptsExist : Whether ES data contains PTS or not
/// @param <IN>        \b pts	   : PTS
/// @param <OUT>       \b MS_BOOL  : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE     :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts )
{
    return MDrv_AUDIO_MM2_inputAesFinished( dec_id, es_size, ptsExist, pts );
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_Enable()  @@Need_Modify
/// @brief \b Function \b Description:  Enable/ Disable the path of USB PCM
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_USBPCM_Enable(MS_BOOL bEnable)
{
    MDrv_AUDIO_USBPCM_Enable(bEnable);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_SetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  clear interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b bEnable :  set false to clean interrupt flag
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable)
{
    MDrv_AUDIO_USBPCM_SetFlag(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_GetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  Get interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b BOOL    :   interrupt flag
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_USBPCM_GetFlag(void)
{
    return MDrv_AUDIO_USBPCM_GetFlag();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_GetMemInfo()  @@Need_Modify
/// @brief \b Function \b Description:  Get memory address and size for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : address, size
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void    _MApi_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo)
{
    MDrv_AUDIO_USBPCM_GetMemInfo(uploadInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b   eType   : sampleRate
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_DmaReader_Init(PCM_OUTPUT_FORMAT* pSettings)
{
    MDrv_AUDIO_DmaReader_Init(pSettings);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_AllInput_Init()
/// @brief \b Function \b Description: Used DMA reader in all audio source application
///                    \b              (ex: Key sound) (CHIP_T8/T12) (48KHz only)
/// @param <IN>        \b   eType   : sampleRate
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_DmaReader_AllInput_Init(void)
{
    MDrv_AUDIO_DmaReader_AllInput_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_WritePCM()
/// @brief \b Function \b Description: This routine is used to write PCM data into DMA reader Buffer
/// @param <IN>        \b   eType   : buffer size (must be multiple of 8 Byte)
/// @param <OUT>       \b MS_BOOL    : TRUE or FALSE
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_DmaReader_WritePCM(MS_U8 u8Idx, void* buffer, MS_U32 bytes)
{
    MS_BOOL bRet = FALSE;
    bRet = MDrv_AUDIO_DmaReader_WritePCM(u8Idx, buffer,bytes);
    return bRet;
}
// [LM14]
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SwDmaReader_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b   eType   : sampleRate
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SwDmaReader_Init(PCM_OUTPUT_FORMAT* pSettings)
{
    MDrv_AUDIO_SwDmaReader_Init(pSettings);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_WritePCM()
/// @brief \b Function \b Description: This routine is used to write PCM data into DMA reader Buffer
/// @param <IN>        \b   eType   : buffer size (must be multiple of 8 Byte)
/// @param <OUT>       \b MS_BOOL    : TRUE or FALSE
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SwDmaReader_WritePCM(MS_U8 u8Idx, void* buffer, MS_U32 bytes)
{
    return(MDrv_AUDIO_SwDmaReader_WritePCM(u8Idx, buffer,bytes));
}



////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DmaWriter_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_DmaWriter_Init(void)
{
    MDrv_AUDIO_DmaWriter_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_VoIP_Config()
/// @brief \b Function \b Description:  This routine is used for VoIP applications
/// @param <IN>        \b AUDIO_VoIP_CONFIG_TYPE : configType
/// @param <IN>        \b MS_U32 : param1
/// @param <IN>        \b MS_U32 : param2
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2)
{
    return MDrv_AUDIO_VoIP_Config(configType, param1, param2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ALSA_Check()
/// @brief \b Function \b Description:  Check if ALSA Interface is supported
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if it's supported, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ALSA_Check(void)
{
    return MDrv_AUDIO_ALSA_Check();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ALSA_Enable()
/// @brief \b Function \b Description:  Enable/ Disable the path of ALSA
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if ok, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ALSA_Enable(MS_BOOL bEnable)
{
    if (_MApi_AUDIO_ALSA_Check() != TRUE)
    {
        return FALSE;
    }

    if (bEnable == TRUE)
    {
        _MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
        _MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
    }

    return MDrv_AUDIO_ALSA_Enable(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_UNI_CheckDecodeDone()
/// @brief \b Function \b Description  : Decoder report pcm out addr & size in Uniplayer mode
/// @param <IN>        \b pU32WrtAddr  : Request input address in ES1 buffer
/// @param <IN>        \b pU32WrtBytes : Request input size
/// @param <OUT>       \b MS_U8   : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_UNI_CheckDecodeDone(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
{
    APIAUDIO_CHECK_SHM_INIT;

    if ( g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] != MDrv_AUDIO_GetUniDecodeDoneTag() )
    {
        g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] = MDrv_AUDIO_GetUniDecodeDoneTag();
        *pU32WrtAddr  = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_pcm_wptr);
        *pU32WrtBytes = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_pcm_decSize);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_UNI_SetOutput()
/// @brief \b Function \b Description: MCU send interrupt to decoder1 in Uniplayer mode
/// @param <IN>        \b PCMOutCnt   : Get output PCM count by MCU
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_UNI_SetOutput (MS_U32 PCMOutCnt)
{
    MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_PCMOutCnt, PCMOutCnt, 0);
    MDrv_AUDIO_SendIntrupt(DSP_DEC, 0xE3);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_UNI_Set_PCMInputWriteAddr()
/// @brief \b Function \b Description: MCU send DEC1-PCM3 wptr to decoder1 in Uniplayer mode
/// @param <IN>        \b PCMIn_Wptr   : PCM3 write pointer to DSP
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_UNI_Set_PCMInputWriteAddr (MS_U32 PCMIn_Wptr)
{
    MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_PCMInput_Addr, PCMIn_Wptr, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_UNI_Get_OutPCMLevel()
/// @brief \b Function \b Description: Decoder1 report DEC1-PCM3 buffer level in Uniplayer mode
/// @param <OUT>       \b MS_U32  : DEC1-PCM3 buffer level (byte unit)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_UNI_Get_OutPCMLevel (void)
{
    return (MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_OutPCMLevel));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_RingTask()
/// @brief \b Function \b Description :
/// @param <IN>        \b auRingTask  :
/// @param <GLOBAL>    \b NONE        :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_RingTask(audioRingTask* auRingTask)        // this function is used for auto ring address
{
    MS_U32 u32Addr_temp;
    MS_U32 u32Size_temp;
    auRingTask->u8RingStatus = 0;   // 0: no need to ring
                                    // 1: not ring, but at the edge of buffer, next time will start from the head of the buf
                                    // 2: ring
                                    // 3: special case, pointer out or range

    if((auRingTask->u32ReqAddr >= auRingTask->_stBufInfo.u32BufStartAddr)&&(auRingTask->u32ReqAddr <= auRingTask->_stBufInfo.u32BufEndAddr))
    {
        u32Addr_temp = auRingTask->u32ReqAddr + auRingTask->u32ReqSize; // target end address

        if(u32Addr_temp < auRingTask->_stBufInfo.u32BufEndAddr )   // no need to ring
        {
            auRingTask->u8RingStatus = 0;
            auRingTask->u32RetAddr1 = auRingTask->u32ReqAddr;
            auRingTask->u32RetSize1 = auRingTask->u32ReqSize;
            auRingTask->_stBufInfo.u32BufWrPtr = u32Addr_temp;
        }
        else if(u32Addr_temp == auRingTask->_stBufInfo.u32BufEndAddr)   // not ring, just make
        {
            auRingTask->u8RingStatus = 1;
            auRingTask->u32RetAddr1 = auRingTask->u32ReqAddr;
            auRingTask->u32RetSize1 = auRingTask->u32ReqSize;
            auRingTask->_stBufInfo.u32BufWrPtr = auRingTask->_stBufInfo.u32BufStartAddr;
        }
        else    // ring
        {
            auRingTask->u8RingStatus = 2;
            auRingTask->u32RetAddr1 = auRingTask->u32ReqAddr;
            auRingTask->u32RetSize1 = auRingTask->_stBufInfo.u32BufEndAddr - auRingTask->u32ReqAddr;
            auRingTask->u32RetAddr2 = auRingTask->_stBufInfo.u32BufStartAddr;
            auRingTask->u32RetSize2 = auRingTask->u32ReqSize -auRingTask->u32RetSize1;
            auRingTask->_stBufInfo.u32BufWrPtr = auRingTask->_stBufInfo.u32BufStartAddr + auRingTask->u32RetSize2;
        }
    }
    else if(auRingTask->u32ReqAddr > auRingTask->_stBufInfo.u32BufEndAddr)   // special case, pointer out of range
    {
        auRingTask->u8RingStatus = 3;
        u32Size_temp = auRingTask->u32ReqAddr - auRingTask->_stBufInfo.u32BufEndAddr;
        auRingTask->u32RetAddr1 = auRingTask->_stBufInfo.u32BufStartAddr + u32Size_temp;
        auRingTask->u32RetSize1 = auRingTask->u32ReqSize;
        auRingTask->_stBufInfo.u32BufWrPtr = auRingTask->u32RetAddr1 + auRingTask->u32RetSize1;
    }
    else
    {
        APIAUDIO_ERROR("request pointer error, smaller than start address  %s() %d\r\n",__FUNCTION__,__LINE__);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_Ring_DataTransfer()
/// @brief \b Function \b Description :
/// @param <IN>        \b auRingTask  :
/// @param <GLOBAL>    \b NONE        :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_Ring_DataTransfer(audioRingTask* auRingTask, audioRingTask* auRingTask_1)
{
    switch(auRingTask->u8RingStatus)     // copy data from PCM1 to PCM2
    {
        case 0:     // no need to ring for read
        case 1:     // just make, no need to ring
            if((auRingTask_1->u8RingStatus == 0)||(auRingTask_1->u8RingStatus == 1))
            {
                //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr1, auRingTask_1->u32RetAddr1, (MS_U16)auRingTask_1->u32RetSize1);
                AU_memcpy((void *) (MsOS_PA2KSEG1(auRingTask_1->u32RetAddr1)), (void *)MsOS_PA2KSEG1(auRingTask->u32RetAddr1), auRingTask_1->u32RetSize1);
            }
            else if(auRingTask_1->u8RingStatus == 2) // need to ring
            {
                //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr1, auRingTask_1->u32RetAddr1, (MS_U16)auRingTask_1->u32RetSize1);
                //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", (auRingTask->u32RetAddr1 + auRingTask_1->u32RetSize1), auRingTask_1->u32RetAddr2, (MS_U16)auRingTask_1->u32RetSize2);
                AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr1), auRingTask_1->u32RetSize1);
                AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr2)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr1 + auRingTask_1->u32RetSize1), auRingTask_1->u32RetSize2);
            }
            else
            {
               printf("\r\n ==== kochien print:  data control error @@@@ \r\n");
            }
            break;

        case 2:
            if((auRingTask_1->u8RingStatus == 0)||(auRingTask_1->u8RingStatus == 1))  // only PCM1 need to ring
            {
                //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr1, auRingTask_1->u32RetAddr1, (MS_U16)auRingTask->u32RetSize1);
                //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr2, auRingTask_1->u32RetAddr1 + auRingTask->u32RetSize1, (MS_U16)auRingTask->u32RetSize2);
                AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr1), auRingTask->u32RetSize1);
                AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr1 + auRingTask->u32RetSize1)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr2), auRingTask->u32RetSize2);
            }
            else if(auRingTask_1->u8RingStatus == 2) // both PCM1 and PCM2 need to ring
            {
                if(auRingTask->u32RetSize1 == auRingTask_1->u32RetSize1)
                {
                    //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr1, auRingTask_1->u32RetAddr1, (MS_U16)auRingTask->u32RetSize1);
                    //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr2, auRingTask_1->u32RetAddr2, (MS_U16)auRingTask->u32RetSize2);
                    AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr1), auRingTask->u32RetSize1);
                    AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr2)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr2), auRingTask->u32RetSize2);
                }
                else if(auRingTask->u32RetSize1 > auRingTask_1->u32RetSize1)
                {
                    //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr1, auRingTask_1->u32RetAddr1, (MS_U16)auRingTask->u32RetSize1);
                    //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr1 + auRingTask_1->u32RetSize1, auRingTask_1->u32RetAddr2, (MS_U16)(auRingTask->u32RetSize1 - auRingTask_1->u32RetSize1));
                    //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr2, auRingTask_1->u32RetAddr2 + auRingTask->u32RetSize1 - auRingTask_1->u32RetSize1, (MS_U16)auRingTask->u32RetSize2);
                    AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr1), auRingTask_1->u32RetSize1);
                    AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr2)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr1 + auRingTask_1->u32RetSize1), (auRingTask->u32RetSize1 - auRingTask_1->u32RetSize1));
                    AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr2 + auRingTask->u32RetSize1 - auRingTask_1->u32RetSize1)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr2), auRingTask->u32RetSize2);
                }
                else
                {
                    //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr1, auRingTask_1->u32RetAddr1, (MS_U16)auRingTask->u32RetSize1);
                    //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->u32RetAddr2, auRingTask_1->u32RetAddr1 + auRingTask->u32RetSize1, (MS_U16)(auRingTask_1->u32RetSize1 - auRingTask->u32RetSize1));
                    //printf(" Sr[0x%lx]->Des[0x%lx], size[0x%x]", (auRingTask->u32RetAddr2 + auRingTask_1->u32RetSize1 - auRingTask->u32RetSize1), auRingTask_1->u32RetAddr2, (MS_U16)auRingTask_1->u32RetSize2);
                    AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr1), auRingTask->u32RetSize1);
                    AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr1 + auRingTask->u32RetSize1)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr2), (auRingTask_1->u32RetSize1 - auRingTask->u32RetSize1));
                    AU_memcpy((void*)(MsOS_PA2KSEG1((MS_U32)auRingTask_1->u32RetAddr2)), (void*)MsOS_PA2KSEG1(auRingTask->u32RetAddr2 + auRingTask_1->u32RetSize1 - auRingTask->u32RetSize1), auRingTask_1->u32RetSize2);
                }
            }
            else
            {
                //AudioDbgPrint_Uniplayer(printf("\r\n ==== data control error \r\n" ));
                APIAUDIO_PRINT("\r\n %s() : ==== case2: data control error \r\n", __FUNCTION__);
            }
            break;

        case 3:
        default:
            APIAUDIO_PRINT("\r\n %s() : ==== case3: data control error \r\n", __FUNCTION__);
            break;
    }
    MsOS_FlushMemory();
    MsOS_Sync();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDataCaptureSource()
/// @brief \b Function \b Description:  Select source for data capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    return MDrv_AUDIO_SetDataCaptureSource(eID, eSource);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Init()
/// @brief \b Function \b Description:  Select source for pcm capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    return MDrv_AUDIO_PCMCapture_Init(eID, eSource);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Start()
/// @brief \b Function \b Description:  captrue pcm data to DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID)
{
   return MDrv_AUDIO_PCMCapture_Start(eID);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Stop()
/// @brief \b Function \b Description:  stop captrue pcm data from DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID)
{
     return MDrv_AUDIO_PCMCapture_Stop(eID);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Read()
/// @brief \b Function \b Description:  captrue pcm data from DDR to device
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b void* : destination buffer pointer
/// @param <IN>        \b MS_U32 : buffer size need transfered in byte
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes)
{
    return MDrv_AUDIO_PCMCapture_Read(eID, buffer, bytes);
}

//====== STAOS PIP START 2012/02/01 ========

MS_BOOL _MApi_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32 Param )
{
    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    if(paramType == Audio_ParamType_KillProcess)
    {
        int i = 0;
        for(i = AU_DEC_ID1; i<AU_DEC_MAX; i++)
        {
            if((u32DecoderPid_Table[i] == Param)&&(Param != 0))  // mapping decoder resource and process pid
            {
                // Stop when release
                printf(" Kill GST process with DecID %d \n", i);
                /* patch for Audio_ParamType_FreeDecoder will clean DSPId */
                if(i == AU_DEC_ID1)
                {
                    g_AudioVars2->AudioDecStatus[i].eDSPId = AUDIO_DSP_ID_DEC;
                }
                else if(i == AU_DEC_ID3)
                {
                    g_AudioVars2->AudioDecStatus[i].eDSPId = AUDIO_DSP_ID_SND;
                }
                MDrv_MAD_DecoderCmd(i,AU_DVB_DECCMD_STOP);
                memset(&g_AudioVars2->AudioDecStatus[i],  0, sizeof(AudioDecStatus_t));
                u32DecoderPid_Table[i] = 0;
            }
        }

        for(i = 0; i<AUDIO_PCM_MAXNUM; i++)
        {
            if((u32RenderPid_Table[i] == Param)&&(Param != 0))  // mapping decoder resource and process pid
            {
                // Stop when release
                printf(" Kill GST process with PCM ID %d \n", i);
                MDrv_AUDIO_PCM_Close(i);
                u32RenderPid_Table[i] = 0;
            }
        }

        return TRUE;
    }
    else
    {
        return MDrv_AUDIO_SetAudioParam2(DecId, paramType, Param);
    }
}

MS_BOOL _MApi_AUDIO_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo )
{
    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    if (NULL == pInfo)
    {
        APIAUDIO_ERROR ("%s: pInfo not be NULL !!\r\n", __FUNCTION__);
        return FALSE;
    }

    return MDrv_AUDIO_GetAudioInfo2(DecId, infoType, pInfo);
}

#if 0
//====== STAOS PIP END 2012/02/01 ========

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_GetSoundMode()
/// @brief \b Function \b Description: Getting FM Radio Sound Mode
/// @param <IN>        \b NONE :
/// @param <OUT>       \b MS_U8   : return E_AUDIOMODE_MONO, E_AUDIOMODE_K_STEREO or E_AUDIOMODE_INVALID
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_FM_RADIO_GetSoundMode(void)
{
    AUDIOSTATUS eCurrentAudioStatus = E_STATE_AUDIO_NO_CARRIER;

    if (MDrv_SIF_GetDspCodeType() != AU_SIF_FM_RADIO)
    {
        return E_AUDIOMODE_INVALID;
    }

    _MApi_AUDIO_SIF_GetAudioStatus(&eCurrentAudioStatus);

    if(m_eSifStatus & E_STATE_AUDIO_PRIMARY_CARRIER)
    {
        eCurrentAudioStatus |= E_STATE_AUDIO_PRIMARY_CARRIER;
    }

    if(m_eSifStatus & E_STATE_AUDIO_STEREO)
    {
        eCurrentAudioStatus |= E_STATE_AUDIO_STEREO;
    }

    if(eCurrentAudioStatus == E_STATE_AUDIO_PRIMARY_CARRIER)
    {
        return E_AUDIOMODE_MONO;
    }
    else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_STEREO))
    {
        return E_AUDIOMODE_K_STEREO;
    }
    else
    {
        return E_AUDIOMODE_INVALID;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_SetSoundMode()
/// @brief \b Function \b Description: Setting FM Radio Sound Mode
/// @param <IN>        \b MS_U8 : u8FMRadioSoundMode
/// @param <OUT>       \b NONE :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FM_RADIO_SetSoundMode(MS_U8 u8FMRadioSoundMode)
{
   switch(u8FMRadioSoundMode)
   {
        case E_AUDIOMODE_MONO:
        case E_AUDIOMODE_FORCED_MONO:
            MDrv_SIF_SetSoundMode(AU_MODE_MONO);
            break;

        case E_AUDIOMODE_G_STEREO:
        case E_AUDIOMODE_K_STEREO:
            MDrv_SIF_SetSoundMode(AU_MODE_STEREO);
            break;

        default:
            break;
   }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_DeEmphassisOption()
/// @brief \b Function \b Description: FM Radio De-emphassis Option
/// @param <IN>        \b MS_BOOL : Is_Europe
/// @param <OUT>       \b NONE :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FM_RADIO_DeEmphassisOption(MS_BOOL Is_Europe)
{
    if(Is_Europe)
        MDrv_SIF_SendCmd(AU_SIF_CMD_SET_FM_RADIO_DeEmp, 1, NULL);
    else
        MDrv_SIF_SendCmd(AU_SIF_CMD_SET_FM_RADIO_DeEmp, 0, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_GET_DC_AMP()
/// @brief \b Function \b Description: FM Radio get DC amp
/// @param <IN>        \b NONE
/// @param <OUT>       \b MS_S8 DC amp
////////////////////////////////////////////////////////////////////////////////
MS_S8 _MApi_AUDIO_FM_RADIO_GET_DC_AMP(void)
{
    return (MS_S8)MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_RADIO_DC_AMP, NULL, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_GET_NSR()
/// @brief \b Function \b Description: FM Radio get NSR
/// @param <IN>        \b NONE
/// @param <OUT>       \b MS_U8 NSR
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_FM_RADIO_GET_NSR(void)
{
    return MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_RADIO_NSR, NULL, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_RESET()
/// @brief \b Function \b Description: FM Radio reset
/// @param <IN>        \b NONE
/// @param <OUT>       \b NONE
////////////////////////////////////////////////////////////////////////////////
void MApi_AUDIO_FM_RADIO_RESET(void)
{
    MDrv_SIF_SendCmd(AU_SIF_CMD_FM_RADIO_RESET, NULL, NULL);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDDRInfo()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo)
{
    return MDrv_AUDIO_GetDDRInfo(DecId, DDRInfo);
}


// [LM14]
MS_U16 _MApi_AUDIO_ReadReg(MS_U32 u32RegAddr)
{
	return (MDrv_AUDIO_ReadReg(u32RegAddr));
}

void _MApi_AUDIO_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val)
{
	MDrv_AUDIO_WriteReg(u32RegAddr, u16Val);
}

void _MApi_AUDIO_LoadAudioClip(MS_U32 bufSize, void *clipBufPtr, MS_U32 dsp_sel)
{
    audioFileParams.u32AudioLength= bufSize;
    audioFileParams.pAudioAddr= clipBufPtr;
    audioFileParams.u32AudioFileIndex = 0;
    audioFileParams.clip_dsp = dsp_sel;
}

MS_U32 _MApi_AUDIO_GetClip_Position(void)
{
     return (audioFileParams.u32AudioFileIndex);
}

void _MApi_AUDIO_SeekAudioClip(MS_U32 clipData_offset)
{
    audioFileParams.u32AudioFileIndex = clipData_offset;
}

void _MApi_AUDIO_StopAudioClip(void)
{
    if((audioFileParams.clip_dsp) == DSP_DEC)
        MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
    else
        MDrv_MAD2_SetDecCmd(AU_DVB_DECCMD_STOP);

    _clipWriteLastAddr = 0;
}

void _MApi_AUDIO_PlayAudioClip(MS_U32 repeatTimes)
{
    if( repeatTimes != 0 )
    {
        audioFileParams.u32AudioLoopCounts = repeatTimes;
        audioFileParams.u8AudioIsInfinite = 0;
    }
    else
    {
        audioFileParams.u32AudioLoopCounts = 0;
        audioFileParams.u8AudioIsInfinite = 1;
    }

    if((audioFileParams.clip_dsp) == DSP_DEC)
    {
        //MDrv_AUDIO_WriteMaskReg(REG_SOUND_AUOUT0_VOL_FRAC,0x007f,0x0010);
        MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILE);
        MDrv_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT, AUDIO_PATH_MAIN);
    }
    else
    {
        //MDrv_AUDIO_WriteMaskReg(REG_SOUND_AUOUT0_VOL_FRAC,0x007f,0x0010);
        MDrv_MAD2_SetDecCmd(AU_DVB_DECCMD_PLAYFILE);
    }
}

MS_U32 _MApi_AUDIO_ClipPlayDone(void)
{
    return (clipPlayDone) ;
}

void _MApi_AUDIO_ClipPlayDone_Rest(void)
{
    clipPlayDone = FALSE ;
}

void _MApi_AUDIO_Clip_processor(void)
{
    MS_U32 writeAddress=0,writeNumber=0;
    static MS_U32 tag=1;
    //static int tmpTest=0;

    if(audioFileParams.clip_dsp == DSP_DEC)
    {
        writeAddress = (MS_U32)(MS_PA2KSEG1(MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_esBuf_reqAddr)));
        writeNumber = MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_BufferSize);
    }
    else
    {
        writeAddress = (MS_U32)(MS_PA2KSEG1(MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_esBuf_reqAddr)));
        writeNumber = MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC2_BufferSize);
    }
    //printk("writeAddress = 0x%x\n",writeAddress);
    //printk("_clipWriteLastAddr = 0x%x\n",_clipWriteLastAddr);
    //printk("writeNumber = %d\n",writeNumber);
    //printk("audioClipInfo.pVaAddr = 0x%x\n",audioFileParams.pAudioAddr);

    //memcpy((void*)writeAddress, (void*)audioClipInfo.pVaAddr, writeNumber);

    if ((!audioFileParams.u8AudioIsInfinite) &&(audioFileParams.u32AudioLoopCounts==0))
    {
        if((audioFileParams.clip_dsp) == DSP_DEC)
        {
            //MDrv_MAD_StopAudioClip();//Removed, StopAudioClip by AP's decision
            if (MApi_AUDIO_Decoded_Frame_Cnt(DSP_DEC) > 1)
            {
                if((MApi_AUDIO_ReadMailBox(DSP_DEC, 7)==0)&&  //check the deocded frame counter to avoid short file
                    (audioFileParams.u32AudioFileIndex >= audioFileParams.u32AudioLength)&&
                        ((MApi_AUDIO_ReadMailBox(DSP_DEC, 0)<=5) &&(MApi_AUDIO_ReadMailBox(DSP_DEC, 1)==0)))
                {
                        //printf("@@@@@@@@@....\n");

                        if(f_clipCount<=0)
                        {
                            MApi_AUDIO_StopAudioClip();
                            clipPlayDone = TRUE;;

                            tag = 1;
                            _clipWriteLastAddr = 0;

                            f_clipCount = COUNT_DOWN;
                            return;
                        }
                        else
                            f_clipCount--;
                }
            }
            // interrupt MAD
            MDrv_AUDIO_SendIntrupt(DSP_DEC,0xE0);
        }
        else
        {
            if (MApi_AUDIO_Decoded_Frame_Cnt(DSP_SE) > 1)
            {
                if((MApi_AUDIO_ReadMailBox(DSP_SE, 7)==0)&&
                (audioFileParams.u32AudioFileIndex >= audioFileParams.u32AudioLength)&&
                (MApi_AUDIO_ReadMailBox(DSP_SE, 0)<=5))
                {
                    //printk("\@@@@@@@@@....\n");
                    MApi_AUDIO_StopAudioClip();
                    clipPlayDone = TRUE;

                    tag = 1;
                    _clipWriteLastAddr = 0;
                    return;
                }
            }
            // interrupt MAD
            MDrv_AUDIO_SendIntrupt(DSP_SE,0xE0);
        }
    return;
    }

    if( writeAddress == _clipWriteLastAddr )
    {
        if((audioFileParams.clip_dsp) == DSP_DEC)
            MDrv_AUDIO_SendIntrupt(DSP_DEC,0xE0);
        else
            MDrv_AUDIO_SendIntrupt(DSP_SE,0xE0);

        return;
    }

    _clipWriteLastAddr = writeAddress ;
    if (audioFileParams.u32AudioFileIndex < audioFileParams.u32AudioLength)
    {
        //printk("$");
        //printk("\nver_Pcm_PlayAudioFile 1....\n");
        // copy to MAD buffer, enough to copy
        if ((audioFileParams.u32AudioFileIndex + writeNumber) <= audioFileParams.u32AudioLength)
        {
            //printf("\MDrv_MAD_Clip_Play(0x%x)(0x%x)(%d)(%d)....\n",writeAddress,audioFileParams.pAudioAddr,audioFileParams.u32AudioFileIndex,writeNumber);
            //printk("1.Copy to addr =  0x%x, Index = %d, sum = %d, Length = %d\n", writeAddress, audioFileParams.u32AudioFileIndex, (audioFileParams.u32AudioFileIndex + writeNumber), audioFileParams.u32AudioLength);
#if 1
            memcpy((void*)writeAddress,
                   (void*)((MS_U32)(audioFileParams.pAudioAddr)+audioFileParams.u32AudioFileIndex),
                   writeNumber);
#endif
            audioFileParams.u32AudioFileIndex += writeNumber;
//printk("\nver_Pcm_PlayAudioFile 2-0 , index = %d....\n", audioFileParams.u32AudioFileIndex);
            //check file end
            if (audioFileParams.u32AudioFileIndex ==    audioFileParams.u32AudioLength)
            {
            //printk("\nver_Pcm_PlayAudioFile 2....\n");
                if (audioFileParams.u8AudioIsInfinite)
                {
            //printk("\nver_Pcm_PlayAudioFile 2-1 ....\n");
                    audioFileParams.u32AudioFileIndex = 0;
                }
                else if (audioFileParams.u32AudioLoopCounts)
                {
            //printk("\nver_Pcm_PlayAudioFile 2-2 ....\n");
            //printk("1. LoopCounts:%d\n",audioFileParams.u32AudioLoopCounts);
                    audioFileParams.u32AudioFileIndex = 0;
                    audioFileParams.u32AudioLoopCounts--;
                }
            }
        }
        else
        {
           // printk("\nver_Pcm_PlayAudioFile 3....\n");
            // clear the buffer first
            memset((void*)writeAddress, 0, writeNumber);

            if (writeNumber <= audioFileParams.u32AudioLength)
            {
                // normal case
                int feedsize = 0;
                memcpy((void*)writeAddress,(void*)((MS_U32)(audioFileParams.pAudioAddr)+audioFileParams.u32AudioFileIndex),
                       audioFileParams.u32AudioLength - audioFileParams.u32AudioFileIndex);
                feedsize += (audioFileParams.u32AudioLength -audioFileParams.u32AudioFileIndex);

                //printk("2.Copy to addr = 0x%x, Feedsize = %d, writeNumber = %d, Index = %d\n", writeAddress, feedsize, writeNumber, audioFileParams.u32AudioFileIndex);
                //printk("\nver_Pcm_PlayAudioFile 3-1... index = %d....\n", audioFileParams.u32AudioFileIndex);

                if (!audioFileParams.u8AudioIsInfinite)
                {
                    audioFileParams.u32AudioLoopCounts--;
                    //printk("2.LoopCounts:%d\n",audioFileParams.u32AudioLoopCounts);
                }

                if ((audioFileParams.u8AudioIsInfinite)||(audioFileParams.u32AudioLoopCounts>0))
                {
                    audioFileParams.u32AudioFileIndex = 0;
                    memcpy((void*)(writeAddress+feedsize),
                           (void*)((MS_U32)(audioFileParams.pAudioAddr)+audioFileParams.u32AudioFileIndex),
                           writeNumber - feedsize);
                    //printk("3.Copy to addr = 0x%x\n", writeAddress+feedsize);
                    audioFileParams.u32AudioFileIndex = writeNumber- feedsize;
                    //printk("\nver_Pcm_PlayAudioFile 3.-3... index = %d....\n", audioFileParams.u32AudioFileIndex);
                }
                else
                {
                //printk("2.Counts=0, set Index = AudioLength\n");
                    // this is the last time, i.e. PcmAudioFileParams.u32PcmAudioLoopCounts==0
                    audioFileParams.u32AudioFileIndex = audioFileParams.u32AudioLength;
                   // printk("\nver_Pcm_PlayAudioFile 3-4.... index = %d....\n", audioFileParams.u32AudioFileIndex);
                }

            }
            else    // writeNumber > MHEGAudioLength
            {
            //printk("\nver_Pcm_PlayAudioFile 5....\n");
            //printk("2.Audio Clip Length small then once write number (May 1024)\n");
                // special case
                if (audioFileParams.u8AudioIsInfinite)
                {
                    // feed data to MAD
                    int feedsize = 0;

                   memcpy((void*)writeAddress,
                           (void*)((MS_U32)(audioFileParams.pAudioAddr)+audioFileParams.u32AudioFileIndex),
                           (audioFileParams.u32AudioLength -audioFileParams.u32AudioFileIndex));
                            feedsize += (audioFileParams.u32AudioLength -audioFileParams.u32AudioFileIndex);
                            audioFileParams.u32AudioFileIndex = 0;

                    while(feedsize+ audioFileParams.u32AudioLength <= writeNumber)
                    {
                        memcpy((void*)(writeAddress+feedsize),
                               (void*)((MS_U32)(audioFileParams.pAudioAddr)+audioFileParams.u32AudioFileIndex),
                               audioFileParams.u32AudioLength );
                            feedsize += audioFileParams.u32AudioLength;
                    }

                    if (feedsize == writeNumber)
                    {
                        audioFileParams.u32AudioFileIndex = 0;
                    }
                    else
                    {
                        memcpy((void*)(writeAddress+feedsize),
                               ((void*)((MS_U32)(audioFileParams.pAudioAddr)+audioFileParams.u32AudioFileIndex)),
                               writeNumber-feedsize);
                            audioFileParams.u32AudioFileIndex = writeNumber-feedsize;
                    }
                }
                else
                {
                    int feedsize = 0;
                    memcpy((void*)writeAddress,
                           (void*)((MS_U32)(audioFileParams.pAudioAddr)+audioFileParams.u32AudioFileIndex),
                           audioFileParams.u32AudioLength - audioFileParams.u32AudioFileIndex);
                            audioFileParams.u32AudioLoopCounts--;
                            feedsize += audioFileParams.u32AudioLength - audioFileParams.u32AudioFileIndex;

                    if (audioFileParams.u32AudioLoopCounts)
                        audioFileParams.u32AudioFileIndex = 0;
                    else
                        audioFileParams.u32AudioFileIndex = audioFileParams.u32AudioLength;

                    while(audioFileParams.u32AudioLoopCounts && (feedsize+audioFileParams.u32AudioLength <= writeNumber))
                    {
                        memcpy((void*)(writeAddress+feedsize),
                               (void*)((MS_U32)(audioFileParams.pAudioAddr)+audioFileParams.u32AudioFileIndex),
                               audioFileParams.u32AudioLength ),
                            feedsize += audioFileParams.u32AudioLength;
                            audioFileParams.u32AudioLoopCounts--;
                    }

                    if (audioFileParams.u32AudioLoopCounts)
                    {
                        memcpy((void*)(writeAddress+feedsize),
                               (void*)((MS_U32)(audioFileParams.pAudioAddr)+audioFileParams.u32AudioFileIndex),
                                      writeNumber- feedsize );
                                    audioFileParams.u32AudioFileIndex =  writeNumber - feedsize;
                    }
                }
            }
        }
    }

    // handshake parameter to MAD
    if((audioFileParams.clip_dsp) == DSP_DEC)
        MApi_AUDIO_WriteMailBox(DSP_DEC, 6, tag++);//CHECK, DSP_DEC or DSP_SE
    else
        MApi_AUDIO_WriteMailBox(DSP_SE, 6, tag++);//CHECK, DSP_DEC or DSP_SE

    //printk("\nver_Pcm_PlayAudioFile 4(%d)....\n",tag);
    // reset tag
    if ((tag%65535)==0)
        tag = 1;

    // interrupt MAD
    if((audioFileParams.clip_dsp) == DSP_DEC)
        MDrv_AUDIO_SendIntrupt(DSP_DEC,0xE0);
    else
        MDrv_AUDIO_SendIntrupt(DSP_SE,0xE0);

  //OS_Delayms(10);
}

MS_U32 _MApi_AUDIO_Decoded_Frame_Cnt(MS_BOOL bDspType)
{
    return(MDrv_AUDIO_Decoded_Frame_Cnt(bDspType));
}

MS_BOOL _MApi_AUDIO_GetBtscA2StereoLevel(MS_U16 *pLevel)
{
    return(MDrv_MAD_SIF_GetBtscA2StereoLevel(pLevel));
}

void _MApi_AUDIO_SIF_GetCurAnalogMode(MS_U8* pAlgMode)
{
    MDrv_MAD_SIF_GetCurAnalogMode(pAlgMode);
}

void _MApi_AUDIO_SIF_SetUserAnalogMode(MS_U16 AlgMode)
{
    MDrv_MAD_SIF_SetUserAnalogMode(AlgMode);
}

MS_BOOL _MApi_AUDIO_SIF_SetModeSetup(MS_U8 SystemType)
{
    return(MDrv_MAD_SIF_SetModeSetup(SystemType));
}

void _MApi_AUDIO_SIF_SetHDEVMode(MS_U8 bOnOff)
{
    MDrv_MAD_SIF_SetHDEVMode(bOnOff);
}

MS_BOOL _MApi_AUDIO_SIF_GetBandDetect(MS_U8 soundSystem, MS_U32 *pbandStrength)
{
    return(MDrv_MAD_SIF_GetBandDetect(soundSystem, pbandStrength));
}

MS_BOOL _MApi_AUDIO_SIF_GetBandDetect_Amp(MS_U8 soundSystem, MS_U32 *pbandStrength)
{
    return(MDrv_MAD_SIF_GetBandDetect_Amp(soundSystem, pbandStrength));
}

MS_BOOL _MApi_AUDIO_SIF_SetBandSetup(MS_U8 SifBand)
{
    return(MDrv_MAD_SIF_SetBandSetup(SifBand));
}

MS_BOOL _MApi_AUDIO_SIF_CheckAvailableSystem(MS_U8 standard, MS_U8 *pexist)
{
    return(MDrv_MAD_SIF_CheckAvailableSystem(standard, pexist));
}

MS_BOOL _MApi_AUDIO_SIF_CheckA2DK(MS_U8 SystemType, MS_U8 *pexist)
{
    return(MDrv_MAD_SIF_CheckA2DK(SystemType, pexist));
}

void _MApi_AUDIO_CheckNicamDigital(MS_U8 *pisNicamDetect)
{
    MDrv_MAD_CheckNicamDigital(pisNicamDetect);
}

MS_BOOL _MApi_AUDIO_SIF_GetSoundStandard(MS_U8 *B_SifStandard)
{
    return(MDrv_MAD_SIF_GetSoundStandard(B_SifStandard));
}

void _MApi_AUDIO_SIF_RedoSetStandard(void)
{
    MDrv_MAD_SIF_RedoSetStandard();
}

MS_BOOL _MApi_AUDIO_SetBtscA2ThresholdLevel(MS_U16 thresholdLevel, MS_U16 *pRF2NSR_map)
{
    return(MDrv_MAD_SetBtscA2ThresholdLevel(thresholdLevel, pRF2NSR_map));
}

void _MApi_AUDIO_SetThresholdType(THR_TBL_TYPE_C code *ThrTbl, MS_U8 num,  MS_U8 standardtype)
{
    MS_U8 i;
    MS_U16 value;

    for(i=0;i< num;i++)
    {
        value =( (0x00FF & ThrTbl->HiByteValue) <<8)|(0x00FF & ThrTbl->LowByteValue);
        MDrv_MAD_SIF_AccessThreshold(WRITE_THRESHOLD|standardtype,ThrTbl->Thrtype,&value);//CHECK
        ThrTbl++;
    }
}

MS_S32 _MApi_AUDIO_PcmMixer_Init(void)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
#ifdef MSOS_TYPE_LINUX_KERNEL
    struct task_struct *pPcmMixerThread = NULL;
#endif
    MS_S8 *pPcmMixerElementBuffer = NULL;
    MS_S32 s32RequireBytes = 0;
    MS_S32 index = 0;
    MS_S32 ret = 0;

    APIAUDIO_PRINT("\r\n === %s === \r\n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    if (g_AudioVars2->g_u8PcmMixerInitFlag == TRUE)
    {
        APIAUDIO_ERROR("%s: PCM Mixer is already initiated.\n", __FUNCTION__);
        return ret;
    }

    APIAUDIO_PRINT("%s: The base address of Element Info is 0x%08lX, and each size is %u\n", __FUNCTION__, (g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase), sizeof(Audio_PcmMixer_Element_Info_t));
    APIAUDIO_PRINT("%s: The base address of Element Buffer is 0x%08lX, and each size is %lu\n", __FUNCTION__, (g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementBufferBase), g_AudioVars2->g_u32PcmMixerElementBufferSize);
    APIAUDIO_PRINT("%s: The base address of Mixed1 Buffer is 0x%08lX, and size is %lu\n", __FUNCTION__, (g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixer1MixedBufferBase), g_AudioVars2->g_u32PcmMixerMixedBufferSize);
    APIAUDIO_PRINT("%s: The base address of Mixed2 Buffer is 0x%08lX, and size is %lu\n", __FUNCTION__, (g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixer2MixedBufferBase), g_AudioVars2->g_u32PcmMixerMixedBufferSize);

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementBufferBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Buffer doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementBuffer = (MS_S8 *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementBufferBase);
    if (pPcmMixerElementBuffer == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementBuffer shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    s32RequireBytes = MDrv_AUDIO_Mixer_Get_SramSize(MAX_PCM_MIXER_OUT_SIZE);
    if (s32RequireBytes > 0)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
        g_pPcmMixerHandle = kzalloc(s32RequireBytes, GFP_KERNEL);
#else
        g_pPcmMixerHandle = malloc(s32RequireBytes);
#endif
        if (g_pPcmMixerHandle == NULL)
        {
            APIAUDIO_ERROR("%s: Error! fail to allocate %ld bytes memory for g_pPcmMixerHandle!\n", __FUNCTION__, s32RequireBytes);
            ret = -1;
            return ret;
        }
        memset(g_pPcmMixerHandle, 0x00, s32RequireBytes);

        g_pPcmMixerHandle_tmp = MDrv_AUDIO_Mixer_Init(g_pPcmMixerHandle, MAX_PCM_MIXER_OUT_SIZE, NULL);
        if (g_pPcmMixerHandle_tmp == NULL)
        {
            APIAUDIO_ERROR("%s: Error! g_pPcmMixerHandle_tmp should not NULL!\n", __FUNCTION__);
            ret = -1;
            return ret;
        }
    }
    else
    {
        APIAUDIO_ERROR("%s: Error! invalid s32RequireBytes %ld!\n", __FUNCTION__, s32RequireBytes);
        ret = -1;
        return ret;
    }

    for (index = 0; index < g_AudioVars2->g_u32PcmMixerMaxElement; index++)
    {
        memset((void *)pPcmMixerElementInfo, 0x00, sizeof(Audio_PcmMixer_Element_Info_t));
        memset((void *)pPcmMixerElementBuffer, 0x00, g_AudioVars2->g_u32PcmMixerElementBufferSize);

        pPcmMixerElementInfo->u8StructVersion = AUDIO_PCMMIXER_VERSION;
        pPcmMixerElementInfo->u32StructSize = sizeof(Audio_PcmMixer_Element_Info_t);

#ifdef MSOS_TYPE_LINUX_KERNEL
        g_pPcmMixerInbuf[index] = kzalloc((sizeof(MS_S16) * MAX_PCM_MIXER_OUT_SIZE * MAX_PCM_MIXER_CH), GFP_KERNEL);
#else
        g_pPcmMixerInbuf[index] = malloc(sizeof(MS_S16) * MAX_PCM_MIXER_OUT_SIZE * MAX_PCM_MIXER_CH);
#endif
        if (g_pPcmMixerInbuf[index] == NULL)
        {
            APIAUDIO_ERROR("%s: Error! fail to allocate %d bytes memory for g_pPcmMixerInbuf[%ld]!\n", __FUNCTION__, (sizeof(MS_S16) * MAX_PCM_MIXER_OUT_SIZE * MAX_PCM_MIXER_CH), index);
            ret = -1;
            return ret;
        }
        memset((void *)g_pPcmMixerInbuf[index], 0x00, (sizeof(MS_S16) * MAX_PCM_MIXER_OUT_SIZE * MAX_PCM_MIXER_CH));

        pPcmMixerElementInfo++;
        pPcmMixerElementBuffer += g_AudioVars2->g_u32PcmMixerElementBufferSize;
    }

#ifdef MSOS_TYPE_LINUX_KERNEL
    pPcmMixerThread = kthread_create(_MApi_AUDIO_PcmMixer_Thread, NULL, "MStar PCM Mixer Thread");
    if (pPcmMixerThread == NULL)
    {
        APIAUDIO_ERROR("%s: Error! fail to create PCM Mixer's thread!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }
    wake_up_process(pPcmMixerThread);
#endif

    g_AudioVars2->g_u8PcmMixerInitFlag = TRUE;

    return ret;
}

MS_S32 _MApi_AUDIO_PcmMixer_DeInit(void)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_S32 index = 0;
    MS_S32 ret = 0;

    printf("\r\n === %s === \r\n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    if (g_AudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    for (index = 0; index < g_AudioVars2->g_u32PcmMixerMaxElement; index++)
    {
        if (g_pPcmMixerInbuf[index] != NULL)
        {
#ifdef MSOS_TYPE_LINUX_KERNEL
            kfree(g_pPcmMixerInbuf[index]);
#else
            free(g_pPcmMixerInbuf[index]);
#endif
            g_pPcmMixerInbuf[index] = NULL;
        }

        memset((void *)pPcmMixerElementInfo, 0x00, sizeof(Audio_PcmMixer_Element_Info_t));
        pPcmMixerElementInfo++;
    }

    if (g_pPcmMixerHandle != NULL)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
        kfree(g_pPcmMixerHandle);
#else
        free(g_pPcmMixerHandle);
#endif
        g_pPcmMixerHandle = NULL;
        g_pPcmMixerHandle_tmp = NULL;
    }

    g_AudioVars2->g_u8PcmMixerInitFlag = FALSE;

    return ret;
}

MS_S32 _MApi_AUDIO_PcmMixer_Open(void)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_S8 *pPcmMixerElementBuffer = NULL;
    MS_S32 index = 0;
    MS_S32 ret = 0;

    printf("\r\n === %s === \r\n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    if (g_AudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementBufferBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Buffer doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementBuffer = (MS_S8 *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementBufferBase);
    if (pPcmMixerElementBuffer == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementBuffer shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    for (index = 0; index < g_AudioVars2->g_u32PcmMixerMaxElement; index++)
    {
        if (pPcmMixerElementInfo->u8IsConnected == FALSE)
        {
            pPcmMixerElementInfo->u8IsConnected = TRUE;
            pPcmMixerElementInfo->u32Buffer = (MS_U32)pPcmMixerElementBuffer;
            pPcmMixerElementInfo->u32BufferSize = g_AudioVars2->g_u32PcmMixerElementBufferSize;
            pPcmMixerElementInfo->u32ReadPtr = pPcmMixerElementInfo->u32Buffer;
            pPcmMixerElementInfo->u32WritePtr = pPcmMixerElementInfo->u32Buffer;
            pPcmMixerElementInfo->u32Weighting = 100;
            pPcmMixerElementInfo->u32Volume = 96;

            /* init mixer control parameters */
            stMixerControl[index].eState = E_MIXERSTATE_OPEN;
            stMixerControl[index].bFirstPush = TRUE;
            stMixerControl[index].bFirstPop = TRUE;
            stMixerControl[index].u32PauseTime = 0;
            stMixerControl[index].u32ReadTime_BeforePause = 0;
            stMixerControl[index].u32StartTime = 0;
            stMixerControl[index].u32OutCnt = 0;

            printf("\r\n === %s , return ID(%d)=== \r\n", __FUNCTION__, (int)index);

            return index;
        }

        pPcmMixerElementInfo++;
        pPcmMixerElementBuffer += g_AudioVars2->g_u32PcmMixerElementBufferSize;
    }

    APIAUDIO_ERROR("%s: Error! No available PCM Mixer element can be used!\n", __FUNCTION__);

    ret = -1;
    return ret;
}

MS_S32 _MApi_AUDIO_PcmMixer_Close(MS_S32 s32PcmMixingId)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_S32 ret = 0;

    printf("\r\n === %s , ID(%d)=== \r\n", __FUNCTION__, (int)s32PcmMixingId);

    APIAUDIO_CHECK_SHM_INIT;

    if (g_AudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if ((s32PcmMixingId < 0) || (s32PcmMixingId >= g_AudioVars2->g_u32PcmMixerMaxElement))
    {
        APIAUDIO_ERROR("%s: Error! Invalid s32PcmMixingId (%ld), MAX PCM Mixing Element is %ld\n", __FUNCTION__, s32PcmMixingId, g_AudioVars2->g_u32PcmMixerMaxElement);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo += s32PcmMixingId;

    if (pPcmMixerElementInfo->u8IsConnected == TRUE)
    {
        pPcmMixerElementInfo->u8IsConnected = FALSE;
        pPcmMixerElementInfo->u8IsStarted = FALSE;
        pPcmMixerElementInfo->u32Group = AUDIO_PCMMIXER_GROUP_NONE;
        pPcmMixerElementInfo->u32Buffer = 0x00000000;
        pPcmMixerElementInfo->u32BufferSize = 0;
        pPcmMixerElementInfo->u32ReadPtr = pPcmMixerElementInfo->u32Buffer;
        pPcmMixerElementInfo->u32WritePtr = pPcmMixerElementInfo->u32Buffer;
        pPcmMixerElementInfo->u32Channel = 0;
        pPcmMixerElementInfo->u32SampleRate = 0;
        pPcmMixerElementInfo->u32BitWidth = 0;
        pPcmMixerElementInfo->u32Endien = 0;
        pPcmMixerElementInfo->u32Timestamp = 0;
        pPcmMixerElementInfo->u32Weighting = 0;
        pPcmMixerElementInfo->u32Volume = 0;
        pPcmMixerElementInfo->u32ReadTime = 0;
        pPcmMixerElementInfo->u32WriteTime = 0;
        pPcmMixerElementInfo->u32BufferLevel = 0;

        memset((void *)pPcmMixerElementInfo->u32Buffer, 0x00, pPcmMixerElementInfo->u32BufferSize);
    }

    return ret;

}

MS_S32 _MApi_AUDIO_PcmMixer_Flush(MS_S32 s32PcmMixingId)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_S32 ret = 0;

    printf("\r\n === %s , ID(%d)=== \r\n", __FUNCTION__, (int)s32PcmMixingId);

    APIAUDIO_CHECK_SHM_INIT;

    if (g_AudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo += s32PcmMixingId;

    if (pPcmMixerElementInfo->u8IsConnected == TRUE)
    {
        pPcmMixerElementInfo->u32ReadPtr = pPcmMixerElementInfo->u32Buffer;
        pPcmMixerElementInfo->u32WritePtr = pPcmMixerElementInfo->u32Buffer;
    }

    /* init mixer control parameters */
    //stMixerControl[index].eState = E_MIXERSTATE_OPEN;
    stMixerControl[s32PcmMixingId].bFirstPush = TRUE;
    stMixerControl[s32PcmMixingId].bFirstPop = TRUE;
    stMixerControl[s32PcmMixingId].u32PauseTime = 0;
    stMixerControl[s32PcmMixingId].u32ReadTime_BeforePause = 0;
    stMixerControl[s32PcmMixingId].u32StartTime = 0;


    return ret;
}

MS_S32 _MApi_AUDIO_PcmMixer_Start(MS_S32 s32PcmMixingId)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_S32 ret = 0;

    printf("\r\n === %s , ID(%d)=== \r\n", __FUNCTION__, (int)s32PcmMixingId);

    APIAUDIO_CHECK_SHM_INIT;

    if (g_AudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo += s32PcmMixingId;

    if (pPcmMixerElementInfo->u8IsConnected == TRUE)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
        pPcmMixerElementInfo->u32WriteTime = jiffies_to_msecs(jiffies);
#else
        pPcmMixerElementInfo->u32WriteTime = MsOS_GetSystemTime();
#endif
        pPcmMixerElementInfo->u8IsStarted = TRUE;
    }

    stMixerControl[s32PcmMixingId].eState = E_MIXERSTATE_START;
    return ret;
}

MS_S32 _MApi_AUDIO_PcmMixer_Stop(MS_S32 s32PcmMixingId)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_S32 ret = 0;

    printf("\r\n === %s , ID(%d)=== \r\n", __FUNCTION__, (int)s32PcmMixingId);

    APIAUDIO_CHECK_SHM_INIT;

    if (g_AudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo += s32PcmMixingId;

    if (pPcmMixerElementInfo->u8IsConnected == TRUE)
    {
        pPcmMixerElementInfo->u8IsStarted = FALSE;
    }

    if(stMixerControl[s32PcmMixingId].eState != E_MIXERSTATE_STOP)
    {
        stMixerControl[s32PcmMixingId].eState = E_MIXERSTATE_STOP;  // actually pause state, do not consume any PCM data
#ifdef MSOS_TYPE_LINUX_KERNEL
        stMixerControl[s32PcmMixingId].u32PauseTime = jiffies_to_msecs(jiffies);
#else
        stMixerControl[s32PcmMixingId].u32PauseTime = MsOS_GetSystemTime();
#endif
        stMixerControl[s32PcmMixingId].u32ReadTime_BeforePause = pPcmMixerElementInfo->u32ReadTime;
    }

    return ret;
}

MS_S32 _MApi_AUDIO_PcmMixer_Set(MS_S32 s32PcmMixingId, MS_U32 u32Cmd, void *pData)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_S32 ret = 0;

    APIAUDIO_CHECK_SHM_INIT;

    if (g_AudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (pData == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pData shoud not be NULL!\n", __FUNCTION__);
        ret = FALSE;
        return ret;
    }

    if ((s32PcmMixingId < 0) || (s32PcmMixingId >= g_AudioVars2->g_u32PcmMixerMaxElement))
    {
        APIAUDIO_ERROR("%s: Error! Invalid s32PcmMixingId (%ld), MAX PCM Mixing Element is %ld\n", __FUNCTION__, s32PcmMixingId, g_AudioVars2->g_u32PcmMixerMaxElement);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo += s32PcmMixingId;

    switch(u32Cmd)
    {
        case AUDIO_PCMMIXER_CMD_GROUP:
        {
            pPcmMixerElementInfo->u32Group = *((MS_U32*)pData);

            break;
        }

        case AUDIO_PCMMIXER_CMD_BUFFERSIZE:
        {
            if ((*((MS_U32*)pData) > 0 ) && (*((MS_U32*)pData) <= g_AudioVars2->g_u32PcmMixerElementBufferSize))
            {
                pPcmMixerElementInfo->u32BufferSize = *((MS_U32*)pData);
            }
            else
            {
                APIAUDIO_ERROR("%s: Error! Invalid u32BufferSize (%lu)\n", __FUNCTION__, *((MS_U32*)pData));
                ret = -1;
                pPcmMixerElementInfo->u32BufferSize = g_AudioVars2->g_u32PcmMixerElementBufferSize;
            }

            break;
        }

        case AUDIO_PCMMIXER_CMD_CHANNEL:
        {
            if ((*((MS_U32*)pData) > 0 ) && (*((MS_U32*)pData) <= 2))
            {
                pPcmMixerElementInfo->u32Channel = *((MS_U32*)pData);
            }
            else
            {
                APIAUDIO_ERROR("%s: Error! Invalid u32Channel (%lu)\n", __FUNCTION__, *((MS_U32*)pData));
                ret = -1;
                pPcmMixerElementInfo->u32Channel = 2;
            }

            break;
        }

        case AUDIO_PCMMIXER_CMD_SAMPLERATE:
        {
            if ((*((MS_U32*)pData) > 0 ) && (*((MS_U32*)pData) <= 96000))
            {
                pPcmMixerElementInfo->u32SampleRate = *((MS_U32*)pData);
            }
            else
            {
                APIAUDIO_ERROR("%s: Error! Invalid u32SampleRate (%lu)\n", __FUNCTION__, *((MS_U32*)pData));
                ret = -1;
                pPcmMixerElementInfo->u32SampleRate = 48000;
            }

            break;
        }

        case AUDIO_PCMMIXER_CMD_BITWIDTH:
        {
            if ((*((MS_U32*)pData) > 0 ) && (*((MS_U32*)pData) <= 16))
            {
                pPcmMixerElementInfo->u32BitWidth = *((MS_U32*)pData);
            }
            else
            {
                APIAUDIO_ERROR("%s: Error! Invalid u32BitWidth (%lu)\n", __FUNCTION__, *((MS_U32*)pData));
                ret = -1;
                pPcmMixerElementInfo->u32BitWidth = 16;
            }

            break;
        }

        case AUDIO_PCMMIXER_CMD_ENDIEN:
        {
            pPcmMixerElementInfo->u32Endien = *((MS_U32*)pData);

            break;
        }

        case AUDIO_PCMMIXER_CMD_TIMESTAMP:
        {
            pPcmMixerElementInfo->u32Timestamp = *((MS_U32*)pData);

            break;
        }

        case AUDIO_PCMMIXER_CMD_WEIGHTING:
        {
            if (*((MS_U32*)pData) <= 100)
            {
                pPcmMixerElementInfo->u32Weighting = *((MS_U32*)pData);
            }
            else
            {
                APIAUDIO_ERROR("%s: Error! Invalid u32Weighting (%lu)\n", __FUNCTION__, *((MS_U32*)pData));
                ret = -1;
                pPcmMixerElementInfo->u32Weighting = 100;
            }

            break;
        }

        case AUDIO_PCMMIXER_CMD_VOLUME:
        {
            if (*((MS_U32*)pData) <= 1016)
            {
                printf("\r\n === %s set volume (%lu) === \r\n", __FUNCTION__, *((MS_U32*)pData));
                pPcmMixerElementInfo->u32Volume = (*((MS_U32*)pData) | (pPcmMixerElementInfo->u32Volume & 0x80000000));
            }
            else
            {
                APIAUDIO_ERROR("%s: Error! Invalid u32Volume (%lu)\n", __FUNCTION__, *((MS_U32*)pData));
                ret = -1;
                pPcmMixerElementInfo->u32Volume = 96;
            }

            break;
        }

        case AUDIO_PCMMIXER_CMD_MUTE:
        {
            if (*((MS_U32*)pData) == TRUE)
            {
                pPcmMixerElementInfo->u32Volume |= 0x80000000;
            }
            else if (*((MS_U32*)pData) == FALSE)
            {
                pPcmMixerElementInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                APIAUDIO_ERROR("%s: Error! Invalid mute command (%lu)\n", __FUNCTION__, *((MS_U32*)pData));
                ret = -1;
            }

            break;
        }

        default :
        {
            APIAUDIO_ERROR("%s: Error! Invalid u32Cmd (%lu)\n", __FUNCTION__, u32Cmd);
            ret = -1;
            break;
        }
    }

    return ret;
}

MS_S32 _MApi_AUDIO_PcmMixer_Get(MS_S32 s32PcmMixingId, MS_U32 u32Cmd, void *pData)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_S32 ret = 0;

    APIAUDIO_CHECK_SHM_INIT;

    if (g_AudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (pData == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pData shoud not be NULL!\n", __FUNCTION__);
        ret = FALSE;
        return ret;
    }

    if ((s32PcmMixingId < 0) || (s32PcmMixingId >= g_AudioVars2->g_u32PcmMixerMaxElement))
    {
        APIAUDIO_ERROR("%s: Error! Invalid s32PcmMixingId (%ld), MAX PCM Mixing Element is %ld\n", __FUNCTION__, s32PcmMixingId, g_AudioVars2->g_u32PcmMixerMaxElement);
        ret = -1;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo += s32PcmMixingId;

    switch(u32Cmd)
    {
        case AUDIO_PCMMIXER_CMD_ALL:
        {
            //memcpy(pData, (void *)pPcmMixerElementInfo, sizeof(Audio_PcmMixer_Element_Info_t));

            break;
        }

        case AUDIO_PCMMIXER_CMD_GROUP:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32Group;

            break;
        }

        case AUDIO_PCMMIXER_CMD_BUFFER:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32Buffer;

            break;
        }

        case AUDIO_PCMMIXER_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32BufferSize;

            break;
        }

        case AUDIO_PCMMIXER_CMD_READPTR:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32ReadPtr;

            break;
        }

        case AUDIO_PCMMIXER_CMD_WRITEPTR:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32WritePtr;

            break;
        }

        case AUDIO_PCMMIXER_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32Channel;

            break;
        }

        case AUDIO_PCMMIXER_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32SampleRate;

            break;
        }

        case AUDIO_PCMMIXER_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32BitWidth;

            break;
        }

        case AUDIO_PCMMIXER_CMD_ENDIEN:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32Endien;

            break;
        }

        case AUDIO_PCMMIXER_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32Timestamp;

            break;
        }

        case AUDIO_PCMMIXER_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32Weighting;

            break;
        }

        case AUDIO_PCMMIXER_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmMixerElementInfo->u32Volume;

            break;
        }

        case AUDIO_PCMMIXER_CMD_BUFFERLEVEL:
        {

            MS_U32 u32wPtr = pPcmMixerElementInfo->u32WritePtr;
            MS_U32 u32rPtr = pPcmMixerElementInfo->u32ReadPtr;
            MS_U32 u32BufSize = pPcmMixerElementInfo->u32BufferSize;
            MS_U32 u32Level = 0, u32Level_correct = 0;
            MS_U32 u32CurrentTime = 0, u32ConsumeTime = 0;
            MS_U32 u32AdjustLevel = 0;      // in samples

#ifdef MSOS_TYPE_LINUX_KERNEL
            u32CurrentTime = jiffies_to_msecs(jiffies);
#else
            u32CurrentTime = MsOS_GetSystemTime();
#endif

            /*Level conmensate mechanism start*/
            // normal case:   mixer  init -->start-->write
            if(stMixerControl[s32PcmMixingId].eState == E_MIXERSTATE_START)
            {
                // thread read time, 0 if not yet reading
                if(stMixerControl[s32PcmMixingId].u32ReadTime_BeforePause != 0) // previous state is in pause
                {
                    u32ConsumeTime = stMixerControl[s32PcmMixingId].u32PauseTime- stMixerControl[s32PcmMixingId].u32ReadTime_BeforePause;
                    stMixerControl[s32PcmMixingId].u32ReadTime_BeforePause = 0;
                    pPcmMixerElementInfo->u32ReadTime = 0;
                    stMixerControl[s32PcmMixingId].u32StartTime = u32CurrentTime;   // reset start point
                }
                else if(pPcmMixerElementInfo->u32ReadTime != 0)
                {
                    u32ConsumeTime = u32CurrentTime - pPcmMixerElementInfo->u32ReadTime;
                }
                else if(stMixerControl[s32PcmMixingId].u32StartTime!= 0)
                {
                    // first read after start
                    u32ConsumeTime = u32CurrentTime - stMixerControl[s32PcmMixingId].u32StartTime;
                }
            }
            else if(stMixerControl[s32PcmMixingId].eState == E_MIXERSTATE_STOP)  // previous state is pause
            {
                // thread read time, 0 if not yet reading
                if(pPcmMixerElementInfo->u32ReadTime != 0)
                {
                    u32ConsumeTime = stMixerControl[s32PcmMixingId].u32PauseTime - stMixerControl[s32PcmMixingId].u32ReadTime_BeforePause;
                }
                else if(stMixerControl[s32PcmMixingId].u32StartTime!= 0)
                {
                    // first read after start
                    u32ConsumeTime = stMixerControl[s32PcmMixingId].u32PauseTime - stMixerControl[s32PcmMixingId].u32StartTime;
                }
            }

            if(u32wPtr >= u32rPtr)
            {
                u32Level = u32wPtr - u32rPtr;
            }
            else
            {
                u32Level =  u32BufSize - (u32rPtr - u32wPtr);
            }

            u32AdjustLevel = u32ConsumeTime*(int)(pPcmMixerElementInfo->u32SampleRate/1000);

            if(u32Level >= u32AdjustLevel*4)
            {
                u32Level_correct = u32Level - u32AdjustLevel*4;       // 2ch/sample, 2B/ch
            }
            else
            {
                u32Level_correct = 0;
            }

            *((MS_U32 *)pData) = u32Level_correct;

//            printf("\r\n === Get_Level, A_Level(%d), level(%d), delta(%d),  ",
    //            (int)u32Level_correct, (int)u32Level, (int)u32ConsumeTime);
            break;
        }

        case AUDIO_PCMMIXER_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmMixerElementInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;

            break;
        }

        default :
        {
            APIAUDIO_ERROR("%s: Error! Invalid u32Cmd (%lu)\n", __FUNCTION__, u32Cmd);
            ret = -1;
            break;
        }
    }

    return ret;
}

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
MS_U32  u32startTime = 0;
MS_U32  u32endTime = 0;
MS_S32  s32diffTime = 0;
MS_U32  u32threadtime = 0;
struct file *filp = NULL;
char str[128];

#define API_AUDIO_FILE_PATH      "/tmp/apiaudio.log"
int audio_write_file(char *filename, void *data, int count)
{
    mm_segment_t fs;
    int ret = 0;
    loff_t pos = 0;
    /* get current->addr_limit. It should be 0-3G */
    fs = get_fs();
    /* set current->addr_limit to 4G */
    set_fs(get_ds());

    if (filp == NULL)
        filp = (struct file *)filp_open(filename, O_CREAT | O_APPEND | O_RDWR, S_IRUSR|S_IWUSR);

    if (IS_ERR(filp)) {
        ret = PTR_ERR(filp);
        set_fs(fs);
        printf("%s fails to open file %s(errno = %d)\n", __func__, filename, ret);
        ret = -1;
        goto exit;
    }

    ret = vfs_write(filp, data, count, &pos);

    //if (filp->f_op && filp->f_op->write)
    //	filp->f_op->write(filp, data, count, &filp->f_pos);

exit:
    /* restore the addr_limit */
    set_fs(fs);

    return ret;
}
#endif
#endif

MS_BOOL _MApi_AUDIO_PcmMixer_Write(MS_S32 s32PcmMixingId, void *pBuf, MS_U32 u32Size)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_BOOL ret = TRUE;
    MS_U32 u32RequestSize = 0;
    MS_S32 s32AvailableSize = 0;
    MS_U32 u32NeedToCopy = 0;
    MS_U32 u32StopThreshold = 0;

    APIAUDIO_CHECK_SHM_INIT;

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    sprintf(str, "[%010lu] pcm write\n", (long unsigned int)jiffies_to_msecs(jiffies));
    audio_write_file(API_AUDIO_FILE_PATH, str, strlen(str));
#endif
#endif

    if (g_AudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        ret = FALSE;
        return ret;
    }

    if (pBuf == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pBuf shoud not be NULL!\n", __FUNCTION__);
        ret = FALSE;
        return ret;
    }

    if (u32Size == 0)
    {
        APIAUDIO_ERROR("%s: Error! u32Size shoud not be zero!\n", __FUNCTION__);
        ret = FALSE;
        return ret;
    }

    if ((s32PcmMixingId < 0) || (s32PcmMixingId >= g_AudioVars2->g_u32PcmMixerMaxElement))
    {
        APIAUDIO_ERROR("%s: Error! Invalid s32PcmMixingId (%ld), MAX PCM Mixing Element is %ld\n", __FUNCTION__, s32PcmMixingId, g_AudioVars2->g_u32PcmMixerMaxElement);
        ret = FALSE;
        return ret;
    }

    if (g_AudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = FALSE;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = FALSE;
        return ret;
    }

    pPcmMixerElementInfo += s32PcmMixingId;

    if (pPcmMixerElementInfo->u8IsConnected == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! s32PcmMixingId (%ld) is not opend yet!\n", __FUNCTION__, s32PcmMixingId);
        ret = FALSE;
        return ret;
    }

    if (pPcmMixerElementInfo->u8IsStarted == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! s32PcmMixingId (%ld) is not started yet!\n", __FUNCTION__, s32PcmMixingId);
        ret = FALSE;
        return ret;
    }

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    //u32startTime = jiffies_to_msecs(jiffies);
    //s32diffTime = u32startTime - u32endTime;
    //printf("\n[A][Utopia] diff:%lu\n", (long unsigned int)u32diffTime);
    //audio_write_file(API_AUDIO_FILE_PATH, pBuf, u32Size);
#endif
#endif

#if 0
    if((stMixerControl[s32PcmMixingId].u32OutCnt > 500)&&(stMixerControl[s32PcmMixingId].u32OutCnt%500 == 0))
    {
        if(u32Size >= 4)
        {
            printf("\r\n  === %s, ID(%d) , (0x%x),  (0x%x), (0x%x), (0x%x) === \r\n",
                __FUNCTION__, (int)s32PcmMixingId,
                (unsigned int)(*pBuf), (unsigned int)(*(pBuf+1)), (unsigned int)(*(pBuf+2)), (unsigned int)(*(pBuf+3));
        }
    }
#endif
    stMixerControl[s32PcmMixingId].u32OutCnt++;

    if (pPcmMixerElementInfo->u32Channel == 1 || pPcmMixerElementInfo->u32BitWidth == 8)
    {
        if (pPcmMixerElementInfo->u32Channel == 1 && pPcmMixerElementInfo->u32BitWidth == 8)
        {
            u32RequestSize = u32Size * 4;
        }
        else
        {
            u32RequestSize = u32Size * 2;
        }
    }
    else
    {
        u32RequestSize = u32Size;
    }

    u32StopThreshold = pPcmMixerElementInfo->u32BufferSize >> 3;
    s32AvailableSize = pPcmMixerElementInfo->u32ReadPtr - pPcmMixerElementInfo->u32WritePtr;

    if (s32AvailableSize <= 0)
    {
#if 0
        if (s32AvailableSize == 0)
        {
            APIAUDIO_ERROR("%s: Warning! PCM Mixer %ld buffer empty!\n", __FUNCTION__, s32PcmMixingId);
        }
#endif
        s32AvailableSize += pPcmMixerElementInfo->u32BufferSize;
    }

    if (s32AvailableSize > (u32RequestSize + u32StopThreshold))
    {
        MS_S16 *pBufTmp = (MS_S16 *)pBuf;
        MS_S8 *pBufTmp_S8 = (MS_S8 *)pBuf;
        MS_S16 *pWritePtrTmp = NULL;
        MS_S16 s16PcmL = 0;
        MS_S16 s16PcmR = 0;
        MS_S32 s32AvailbleElementPcm = 0;
        MS_U32 u32LRSampleIndex = 0;

        do
        {
            if ((pPcmMixerElementInfo->u32WritePtr + u32RequestSize) > (pPcmMixerElementInfo->u32Buffer + pPcmMixerElementInfo->u32BufferSize))
            {
                u32NeedToCopy = (pPcmMixerElementInfo->u32Buffer + pPcmMixerElementInfo->u32BufferSize) - pPcmMixerElementInfo->u32WritePtr;
            }
            else
            {
                u32NeedToCopy = u32RequestSize;
            }

            pWritePtrTmp = (MS_S16 *)pPcmMixerElementInfo->u32WritePtr;

            for (u32LRSampleIndex = 0; u32LRSampleIndex < (u32NeedToCopy / 4); u32LRSampleIndex++)
            {
                if (pPcmMixerElementInfo->u32Channel == 1)
                {
                    if (pPcmMixerElementInfo->u32BitWidth == 8)
                    {
                        s16PcmL = *(pBufTmp_S8++);
                        s16PcmL <<= 8;
                        s16PcmR = s16PcmL;
                    }
                    else
                    {
                        if(pPcmMixerElementInfo->u32Endien == 1)
                        {
                            s16PcmL = ((*(pBufTmp))>>8)&0x00FF;
                            s16PcmL = s16PcmL | (((*pBufTmp++)<<8)&0xFF00);
                        }
                        else
                        {
                            s16PcmL = *(pBufTmp++);
                        }
                        s16PcmR = s16PcmL;
                    }
                }
                else
                {
                    if (pPcmMixerElementInfo->u32BitWidth == 8)
                    {
                        s16PcmL = *(pBufTmp_S8++);
                        s16PcmL <<= 8;

                        s16PcmR = *(pBufTmp_S8++);
                        s16PcmR <<= 8;
                        /*
                        //for unsigned 8bit sample data
                        s16PcmL = (*(pBufTmp_S8++)) - 0x80;
                        s16PcmL <<= 8;

                        s16PcmR = (*(pBufTmp_S8++)) - 0x80;
                        s16PcmR <<= 8;
                        */
                    }
                    else
                    {
                        if(pPcmMixerElementInfo->u32Endien == 1)
                        {
                            s16PcmL = ((*(pBufTmp))>>8)&0x00FF;
                            s16PcmL = s16PcmL | (((*pBufTmp++)<<8)&0xFF00);
                            s16PcmR = ((*(pBufTmp))>>8)&0x00FF;
                            s16PcmR = s16PcmR | (((*pBufTmp++)<<8)&0xFF00);
                        }
                        else
                        {
                            s16PcmL = *(pBufTmp++);
                            s16PcmR = *(pBufTmp++);
                        }
                    }
                }
                *(pWritePtrTmp++) = s16PcmL;
                *(pWritePtrTmp++) = s16PcmR;
            }

            u32RequestSize -= u32NeedToCopy;
            pPcmMixerElementInfo->u32WritePtr += u32NeedToCopy;
            if (pPcmMixerElementInfo->u32WritePtr == (pPcmMixerElementInfo->u32Buffer + pPcmMixerElementInfo->u32BufferSize))
            {
                pPcmMixerElementInfo->u32WritePtr = pPcmMixerElementInfo->u32Buffer;
            }
        } while(u32RequestSize > 0);

        /* Collect specific information to report more precise PCM buffer level */
#ifdef MSOS_TYPE_LINUX_KERNEL
        pPcmMixerElementInfo->u32WriteTime = jiffies_to_msecs(jiffies);
#else
        pPcmMixerElementInfo->u32WriteTime = MsOS_GetSystemTime();
#endif
        s32AvailbleElementPcm = pPcmMixerElementInfo->u32WritePtr - pPcmMixerElementInfo->u32ReadPtr;
        if (s32AvailbleElementPcm < 0)
        {
            s32AvailbleElementPcm += pPcmMixerElementInfo->u32BufferSize;
        }
        pPcmMixerElementInfo->u32BufferLevel = (MS_U32)s32AvailbleElementPcm;
    }
    else
    {
        ret = FALSE;
    }

    if(stMixerControl[s32PcmMixingId].bFirstPush == TRUE)
    {
        stMixerControl[s32PcmMixingId].bFirstPush = FALSE;
#ifdef MSOS_TYPE_LINUX_KERNEL
        stMixerControl[s32PcmMixingId].u32StartTime = jiffies_to_msecs(jiffies);
#else
        stMixerControl[s32PcmMixingId].u32StartTime = MsOS_GetSystemTime();
#endif

        printf("\r\n === Start audio feeding, ID(%d), (%d) === \r\n",
            (int)s32PcmMixingId, (int)stMixerControl[s32PcmMixingId].u32StartTime);
    }

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    u32endTime = jiffies_to_msecs(jiffies);
    //if(ret == TRUE)
    //{
        sprintf(str, "[%010lu] write diff %5d msec, size:%5d, avail %5d, ret:%5d\n", jiffies_to_msecs(jiffies), s32diffTime, u32Size, s32AvailableSize, ret);
        audio_write_file(API_AUDIO_FILE_PATH, str, strlen(str));
    //}
#endif
#endif

    return ret;
}

MS_S32 _MApi_AUDIO_PcmMixer_Func(MS_U32 u32Arg)
{
    AUDIO_SHARED_VARS2 *pAudioVars2 = NULL;
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo_tmp = NULL;
    void *pPcmMixerHandle = NULL;
    MS_BOOL bIsOutputDone[8] = {FALSE};
    MS_S8 *pPcmMixer1MixedBuffer = NULL;
    MS_S8 *pPcmMixer2MixedBuffer = NULL;
    MS_U8 u8IsGroupInUsed[8] = {FALSE};
    static MS_U8 u8PreIsGroupInUsed[8] = {FALSE};
    MS_S16 *pPcmMixerMixedBufferArray[8] = {0x00000000};
    MS_S32 *pPcmMixerInlen = NULL;
    MS_S32 s32Outlen = 0;
    MS_S32 s32MaxOutSize = 0;
    MS_S32 index = 0;
    MS_S32 groupIndex = 0;
    MS_S32 ret = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32NeedToCopy = 0;

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32 t1 = 0;
    MS_U32 t2 = 0;
    MS_U32 t3 = 0;
    MS_U32 t4 = 0;
    MS_U32 t5 = 0;
    MS_U32 t6 = 0;
    MS_U32 t7 = 0;
    MS_U32 t8 = 0;
    MS_U32 t9 = 0;
    MS_U32 t10 = 0;
    MS_U32 t11 = 0;
    MS_U32 delta_t = 0;
    MS_U32 dma_delta_t = 0;
    MS_U32 counter = 0;
    MS_U32 counter1 = 0;
    MS_S32 free_size = 0;
#endif
#endif

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    t1 = jiffies_to_msecs(jiffies);
#endif
#endif

    APIAUDIO_CHECK_SHM_INIT;

    pAudioVars2 = g_AudioVars2;

    if (pAudioVars2->g_u8PcmMixerInitFlag == FALSE)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer is not initiated yet!\n", __FUNCTION__);
        return -1;
    }

    if (pAudioVars2->g_u32PcmMixerElementInfoBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Info doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(pAudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + pAudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (pAudioVars2->g_u32PcmMixerElementBufferBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer Element Buffer doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (pAudioVars2->g_u32PcmMixer1MixedBufferBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer1 Mixed Buffer doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixer1MixedBuffer = (MS_S8 *)MS_PA2KSEG1(pAudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + pAudioVars2->g_u32PcmMixer1MixedBufferBase);
    if (pPcmMixer1MixedBuffer == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixer1MixedBuffer shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if (pAudioVars2->g_u32PcmMixer2MixedBufferBase == 0x00000000)
    {
        APIAUDIO_ERROR("%s: Error! PCM Mixer2 Mixed Buffer doesn't exist!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    pPcmMixer2MixedBuffer = (MS_S8 *)MS_PA2KSEG1(pAudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + pAudioVars2->g_u32PcmMixer2MixedBufferBase);
    if (pPcmMixer2MixedBuffer == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixer2MixedBuffer shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    memset(u8IsGroupInUsed, 0x00, sizeof(u8IsGroupInUsed));

    /* Check if there is any active PCM Mixer Element and Group */
    for (index = 0; index < pAudioVars2->g_u32PcmMixerMaxElement; index++)
    {
        pPcmMixerElementInfo_tmp = pPcmMixerElementInfo + index;

        if ((pPcmMixerElementInfo_tmp->u8IsConnected == TRUE) && (pPcmMixerElementInfo_tmp->u8IsStarted == TRUE))
        {
            if (pPcmMixerElementInfo_tmp->u32Group != AUDIO_PCMMIXER_GROUP_NONE)
            {
                for (groupIndex = 0; groupIndex < AUDIO_PCMMIXER_MAXGROUP; groupIndex++)
                {
                    if (((pPcmMixerElementInfo_tmp->u32Group >> groupIndex) & 0x00000001))
                    {
                        u8IsGroupInUsed[groupIndex] = TRUE;
                    }
                }
            }
        }
    }

    /* DMA reader from no use to in use --> reset DMA reader */
    #if 0
    if ((u8IsGroupInUsed[0] == TRUE) && (u8PreIsGroupInUsed[0] == FALSE))
    {
        PCM_OUTPUT_FORMAT stPcmSettings;
        //MS_U32 u32Level = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DMAReader_BufferLevel);
        //printf("\r\n === DMA reader reset, level(0x%x) === \r\n", (unsigned int)u32Level);

        memset(&stPcmSettings, 0x00, sizeof(PCM_OUTPUT_FORMAT));
        stPcmSettings.eFs = SAMPLE_RATE_48000;
        stPcmSettings.bLittleEndian = TRUE;
        stPcmSettings.u8Channels = 2;
        stPcmSettings.u8ByteWidth = 2;
        stPcmSettings.bMainChannel = FALSE;
        MApi_AUDIO_DmaReader_Init(&stPcmSettings);
        MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DMAReader_Command, TRUE, 0);
    }
    #endif

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    t2 = jiffies_to_msecs(jiffies);
#endif
#endif

    /* Fill in NULL PCM data to avoid any DC offset in Group-0 (DMA Reader) */
    if (u8IsGroupInUsed[0] != u8PreIsGroupInUsed[0])
    {
        MS_U8 *u8NullPCM = NULL;
        MS_U32 u32PCMSize = (48000 << 2) * 20 /1000; //20ms Stereo silence PCM data

#ifdef MSOS_TYPE_LINUX_KERNEL
        u8NullPCM = kzalloc((sizeof(MS_U8) * u32PCMSize), GFP_KERNEL);
#else
        u8NullPCM = malloc(sizeof(MS_U8) * u32PCMSize);
#endif

        if (u8NullPCM != NULL)
        {
            do
            {
                // Daniel - Temp Hard code here to use first HW DMA reader
                ret = _MApi_AUDIO_DmaReader_WritePCM(0, u8NullPCM, u32PCMSize);

                if (ret != TRUE)
                {
#ifdef MSOS_TYPE_LINUX_KERNEL
                    msleep(1);
#if AUDIO_MIXER_THREAD_DEBUG
                    counter++;
#endif
#else
                    MsOS_DelayTask(1);
#endif
                }
            } while (ret != TRUE);

#ifdef MSOS_TYPE_LINUX_KERNEL
            kfree(u8NullPCM);
#else
            free(u8NullPCM);
#endif
        }
    }

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    t3 = jiffies_to_msecs(jiffies);
#endif
#endif

    /* Save current PCM Mixer Element's group information */
    memcpy(u8PreIsGroupInUsed, u8IsGroupInUsed, sizeof(u8IsGroupInUsed));

    for (groupIndex = 0; groupIndex < AUDIO_PCMMIXER_MAXGROUP; groupIndex++)
    {
        if (u8IsGroupInUsed[groupIndex] == TRUE)
        {
            break;
        }
    }

    if (groupIndex == AUDIO_PCMMIXER_MAXGROUP)
    {
        /* Do return here, because there is no any active PCM Mixer Element */
        ret = 0;
        return ret;
    }

    pPcmMixerHandle = g_pPcmMixerHandle_tmp;
    pPcmMixerInlen = &g_s32PcmMixerInlen[0];

    s32MaxOutSize = (MDrv_AUDIO_Mixer_Get_MaxOutsize(pPcmMixerHandle, (void *)pPcmMixerElementInfo) << 2);

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    t4 = jiffies_to_msecs(jiffies);
#endif
#endif

    /* Prepare for SRC */
    for (index = 0; index < pAudioVars2->g_u32PcmMixerMaxElement; index++)
    {
        MS_S32 s32AvailbleElementPcm = 0;
        MS_S32 s32SilencePcmSize = 0;
        MS_S32 inlen = 0;
        MS_S32 outlen = 0;
        MS_U32 u32PcmMixerInbuf_tmp = 0;

        pPcmMixerElementInfo_tmp = pPcmMixerElementInfo + index;
        u32PcmMixerInbuf_tmp = (MS_U32)g_pPcmMixerInbuf[index];

        /* Reset Mixer Element's tmp buffer */
        memset((void *)u32PcmMixerInbuf_tmp, 0x00, (sizeof(MS_S16) * MAX_PCM_MIXER_OUT_SIZE * MAX_PCM_MIXER_CH));

        if ((pPcmMixerElementInfo_tmp->u8IsConnected == TRUE) &&
            (pPcmMixerElementInfo_tmp->u8IsStarted == TRUE) &&
            (pPcmMixerElementInfo_tmp->u32SampleRate != 0))
        {
            inlen =  (MDrv_AUDIO_Mixer_Get_InputSize(pPcmMixerHandle, (s32MaxOutSize >> 2), index, (void *)pPcmMixerElementInfo_tmp) << 2);
            if (inlen == 0)
            {
                APIAUDIO_ERROR("%s: Error! Invalid inlen (%ld) at index (%ld)!\n", __FUNCTION__, inlen, index);
                continue;
            }

            outlen = (MDrv_AUDIO_Mixer_Get_OutputSize(pPcmMixerHandle, (inlen >> 2), index, (void *)pPcmMixerElementInfo_tmp) << 2);
            if (outlen != s32MaxOutSize)
            {
                APIAUDIO_ERROR("%s: Error! outlen (%ld) is not expected at index (%ld), supposed to be %ld!\n", __FUNCTION__, outlen, index, s32MaxOutSize);
                continue;
            }

            //APIAUDIO_ERROR("%s: inlen = %ld, outlen = %ld, s32MaxOutSize = %ld\n", __FUNCTION__, inlen, outlen, s32MaxOutSize);

            s32AvailbleElementPcm = pPcmMixerElementInfo_tmp->u32WritePtr - pPcmMixerElementInfo_tmp->u32ReadPtr;
            if (s32AvailbleElementPcm < 0)
            {
                s32AvailbleElementPcm += pPcmMixerElementInfo_tmp->u32BufferSize;
            }

            u32RequestSize = inlen;
            pPcmMixerInlen[index] = inlen >> 2;

            if (stMixerControl[index].bFirstPop == TRUE)
            {
                if (s32AvailbleElementPcm < u32RequestSize)
                {
                    continue;
                }

                stMixerControl[index].bFirstPop = FALSE;
            }

            u32RequestSize = (s32AvailbleElementPcm >= u32RequestSize) ? u32RequestSize : s32AvailbleElementPcm;
            s32SilencePcmSize = inlen - u32RequestSize;

#if AUDIO_MIXER_THREAD_DEBUG
            if (s32SilencePcmSize > 0)
            {
                APIAUDIO_ERROR("%s: Warning! s32SilencePcmSize %ld at index %ld\n", __FUNCTION__, s32SilencePcmSize, index);
                #ifdef MSOS_TYPE_LINUX_KERNEL
                sprintf(str, "[A][Utopia] silence:%d, index:%d, inlen:%x, req:%x\n", s32SilencePcmSize, index, (unsigned int)inlen, (unsigned int)u32RequestSize);
                audio_write_file(API_AUDIO_FILE_PATH, str, strlen(str));
                #endif
            }
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
            pPcmMixerElementInfo_tmp->u32ReadTime = jiffies_to_msecs(jiffies);
#else
            pPcmMixerElementInfo_tmp->u32ReadTime = MsOS_GetSystemTime();
#endif

            while (u32RequestSize > 0)
            {
                if ((pPcmMixerElementInfo_tmp->u32ReadPtr + u32RequestSize) > (pPcmMixerElementInfo_tmp->u32Buffer + pPcmMixerElementInfo_tmp->u32BufferSize))
                {
                    u32NeedToCopy = (pPcmMixerElementInfo_tmp->u32Buffer + pPcmMixerElementInfo_tmp->u32BufferSize) - pPcmMixerElementInfo_tmp->u32ReadPtr;
                }
                else
                {
                    u32NeedToCopy = u32RequestSize;
                }

                if(u32PcmMixerInbuf_tmp != NULL && pPcmMixerElementInfo_tmp->u32ReadPtr != NULL)
                {
                    memcpy((void *)u32PcmMixerInbuf_tmp, (void *)pPcmMixerElementInfo_tmp->u32ReadPtr, u32NeedToCopy);
                }
                else if(u32PcmMixerInbuf_tmp == NULL)
                {
                    APIAUDIO_ERROR("%s: Error! u32PcmMixerInbuf_tmp is NULL!\n", __FUNCTION__);
                    break;
                }
                else if(pPcmMixerElementInfo_tmp->u32ReadPtr == NULL)
                {
                    APIAUDIO_ERROR("%s: Error! u32ReadPtr is NULL!\n", __FUNCTION__);
                    break;
                }

                u32RequestSize -= u32NeedToCopy;
                u32PcmMixerInbuf_tmp += u32NeedToCopy;
                pPcmMixerElementInfo_tmp->u32ReadPtr += u32NeedToCopy;
                if (pPcmMixerElementInfo_tmp->u32ReadPtr == (pPcmMixerElementInfo_tmp->u32Buffer + pPcmMixerElementInfo_tmp->u32BufferSize))
                {
                    pPcmMixerElementInfo_tmp->u32ReadPtr = pPcmMixerElementInfo_tmp->u32Buffer;
                }
            }
        }
    }

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    t11 = jiffies_to_msecs(jiffies);
#endif
#endif

    /* SRC, Mix & Output */
    pPcmMixerMixedBufferArray[0] = (MS_S16 *)pPcmMixer1MixedBuffer;
#if 0 /* Not support for the moment */
    pPcmMixerMixedBufferArray[1] = (MS_S16 *)pPcmMixer2MixedBuffer;
#endif

    /* Reset Mixer's tmp buffer */
    for (groupIndex = 0; groupIndex < AUDIO_PCMMIXER_MAXGROUP; groupIndex++)
    {
        if (pPcmMixerMixedBufferArray[groupIndex] != NULL)
        {
            memset((void *)pPcmMixerMixedBufferArray[groupIndex], 0x00, s32MaxOutSize);
        }
    }

    s32Outlen = (MDrv_AUDIO_Mixer_Process(pPcmMixerHandle, &g_pPcmMixerInbuf[0], &pPcmMixerMixedBufferArray[0], pPcmMixerInlen, (void *)pPcmMixerElementInfo) << 2);

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    t5 = jiffies_to_msecs(jiffies);
#endif
#endif

    if (s32Outlen > 0)
    {
        if (s32Outlen ==  s32MaxOutSize)
        {
            do
            {
                ret = TRUE;

                if (bIsOutputDone[0] == FALSE)
                {
                    // Daniel - Temp Hard code here to use first HW DMA reader
#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
                    t7 = jiffies_to_msecs(jiffies);
#endif
#endif
                    ret = _MApi_AUDIO_DmaReader_WritePCM(0, pPcmMixer1MixedBuffer, s32Outlen);
#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
                    t8 = jiffies_to_msecs(jiffies);
                    dma_delta_t += (t8-t7);
#endif
#endif
                    if (ret == TRUE)
                    {
                        bIsOutputDone[0] = TRUE;
                    }
                }

#if 0 /* Not support for the moment */
                if (bIsOutputDone[1] == FALSE)
                {
                    ret = _MApi_AUDIO_SwDmaReader_WritePCM(pPcmMixer2MixedBuffer, s32Outlen);
                    if (ret == TRUE)
                    {
                        bIsOutputDone[1] = TRUE;
                    }
                }
#endif

                if (ret != TRUE)
               {
#ifdef MSOS_TYPE_LINUX_KERNEL
#if AUDIO_MIXER_THREAD_DEBUG
                    t9 = jiffies_to_msecs(jiffies);
#endif
                    msleep(1);
#if AUDIO_MIXER_THREAD_DEBUG
                    t10 = jiffies_to_msecs(jiffies);
                    delta_t += (t10-t9);
                    counter1++;
#endif
#else
                    MsOS_DelayTask(1);
#endif
                }
            } while (ret != TRUE);
        }
        else
        {
            APIAUDIO_ERROR("%s: Error! s32Outlen (%ld), s32MaxOutSize (%ld) are not match!\n", __FUNCTION__, s32Outlen, s32MaxOutSize);
        }
    }
    else
    {
        APIAUDIO_ERROR("%s: Error! Invalid s32Outlen (%ld)!\n", __FUNCTION__, s32Outlen);
    }

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    /*free_size = pPcmMixerElementInfo_tmp->u32WritePtr - pPcmMixerElementInfo_tmp->u32ReadPtr;
    if (free_size < 0)
    {
        free_size += pPcmMixerElementInfo_tmp->u32BufferSize;
    }*/
    t6 = jiffies_to_msecs(jiffies);

    sprintf(str, "[%010lu] mixer time %d silence %d cnt %d src %d mixer %d dma %d [%d, %d]cnt %d msec \n", jiffies_to_msecs(jiffies), (t6-t1), (t3-t2), counter, (t11-t4), (t5-t11),(t6-t5), dma_delta_t, delta_t,  counter1);
    audio_write_file(API_AUDIO_FILE_PATH, str, strlen(str));
#endif
#endif

    return ret;
}

int _MApi_AUDIO_PcmMixer_Thread(void *arg)
{
    Audio_PcmMixer_Element_Info_t *pPcmMixerElementInfo = NULL;
    MS_U32 u32MaxFrameTime = 0;
    MS_U32 u32ResidualTime = 0;
    MS_U32 u32SleepTime = 0;
    MS_U32 u32MixerStartTime = 0;
    MS_U32 u32MixerEndTime = 0;
    MS_U32 u32MixerDeltaTime = 0;
    MS_U32 u32SleepStartTime = 0;
    MS_U32 u32SleepEndTime = 0;
    MS_U32 u32SleepDeltaTime = 0;
    MS_S32 ret = 0;

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32  u32startTime = 0;
    MS_U32  u32endTime = 0;
    MS_U32  u32mixerstartTime = 0;
    MS_U32  u32mixerendTime = 0;
    MS_U32  u32diffTime = 0;
#endif
#endif

    printf("\r\n === %s === \r\n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xFFFF, 0x0001);

    pPcmMixerElementInfo = (Audio_PcmMixer_Element_Info_t *)MS_PA2KSEG1(g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE] + g_AudioVars2->g_u32PcmMixerElementInfoBase);
    if (pPcmMixerElementInfo == NULL)
    {
        APIAUDIO_ERROR("%s: Error! pPcmMixerElementInfo shoud not be NULL!\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xFFFF, 0x0002);

    APIAUDIO_ERROR("%s: Start!\n", __FUNCTION__);

    while (1)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL

        #ifdef M2_NSS  //NSS (Only NSS need to dynamic change Thread priority for VP9 60p
        {
            if(g_Omx_bIsVP9_60P == TRUE)
            {
                if(g_Omx_bIsVP9_60P != g_Omx_Previous_bIsVP9_60P)
                {
                    set_user_nice(current, 10);
                    g_Omx_Previous_bIsVP9_60P = g_Omx_bIsVP9_60P;
                }
            }
            else if(g_Omx_bIsVP9_60P == FALSE)
            {
                if(g_Omx_bIsVP9_60P != g_Omx_Previous_bIsVP9_60P)
                {
                    set_user_nice(current, 0);
                    g_Omx_Previous_bIsVP9_60P = g_Omx_bIsVP9_60P;
                }
            }
        }
        #endif

        set_user_nice(current, -20);

        #ifndef M2_NSS //WebOS
        {
            //do nothing
        }
        #endif

#endif

        u32MaxFrameTime = 20;
        u32MixerStartTime = 0;
        u32MixerEndTime = 0;
        u32MixerDeltaTime = 0;
        u32SleepStartTime = 0;
        u32SleepEndTime = 0;
        u32SleepDeltaTime = 0;

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
        u32SleepTime = 0;
        u32startTime = 0;
        u32endTime = 0;
        u32mixerstartTime = 0;
        u32mixerendTime = 0;
        u32diffTime = 0;
        u32startTime = jiffies_to_msecs(jiffies);
        u32diffTime = u32startTime - u32threadtime;
        //u32threadtime = u32startTime;
#endif
#endif

        MDrv_AUDIO_WriteMaskReg(0x2D2C, 0xF0F0, 0x0010);

#ifdef MSOS_TYPE_LINUX_KERNEL
        u32MixerStartTime = jiffies_to_msecs(jiffies);
#else
        u32MixerStartTime = MsOS_GetSystemTime();
#endif
        MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xF0F0, 0x0020);

        if (g_AudioVars2->g_u8PcmMixerInitFlag == TRUE)
        {
            MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xF0F0, 0x0030);

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
            u32mixerstartTime = jiffies_to_msecs(jiffies);
#endif
#endif

            do
            {
                MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xFF00, 0x0100);

                ret = _MApi_AUDIO_PcmMixer_Func(NULL);
                if (ret == TRUE)
                {
                    MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xFF00, 0x0200);

                    ret = MDrv_AUDIO_Mixer_Get_MaxFrameTime((void *)pPcmMixerElementInfo);
                    if (ret > 0)
                    {
                        u32MaxFrameTime = (MS_U32)ret;
                    }

                    MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xFF00, 0x0300);
                }

                u32ResidualTime = (u32ResidualTime >= u32MaxFrameTime) ? (u32ResidualTime - u32MaxFrameTime) : u32ResidualTime;

            } while (u32ResidualTime >= u32MaxFrameTime);

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
            u32mixerendTime = jiffies_to_msecs(jiffies);
#endif
#endif
        }

        MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xF0F0, 0x0040);

#ifdef MSOS_TYPE_LINUX_KERNEL
        u32MixerEndTime = jiffies_to_msecs(jiffies);
#else
        u32MixerEndTime = MsOS_GetSystemTime();
#endif

        MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xF0F0, 0x0050);

        if (u32MixerStartTime > u32MixerEndTime)
        {
            u32MixerDeltaTime = u32MixerEndTime + (0xFFFFFFFF - u32MixerStartTime);
        }
        else
        {
            u32MixerDeltaTime = u32MixerEndTime - u32MixerStartTime;
        }

        if (u32MaxFrameTime > (u32MixerDeltaTime + u32ResidualTime))
        {
            u32SleepTime = u32MaxFrameTime - (u32MixerDeltaTime + u32ResidualTime);

#ifdef MSOS_TYPE_LINUX_KERNEL
            u32SleepStartTime = jiffies_to_msecs(jiffies);
            msleep(u32SleepTime);
            u32SleepEndTime = jiffies_to_msecs(jiffies);
#else
            u32SleepStartTime = MsOS_GetSystemTime();
            MsOS_DelayTask(u32SleepTime);
            u32SleepEndTime = MsOS_GetSystemTime();
#endif

            if (u32SleepStartTime > u32SleepEndTime)
            {
                u32SleepDeltaTime = u32SleepEndTime + (0xFFFFFFFF - u32SleepStartTime);
            }
            else
            {
                u32SleepDeltaTime = u32SleepEndTime - u32SleepStartTime;
            }

            u32ResidualTime += (u32SleepDeltaTime - u32SleepTime);
        }
        else if (u32MixerDeltaTime > u32MaxFrameTime)
        {
                u32ResidualTime += (u32MixerDeltaTime - u32MaxFrameTime);
        }
        else
        {
                u32ResidualTime -= (u32MaxFrameTime - u32MixerDeltaTime);
        }

        MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0xF0F0, 0x0060);

#if AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
        u32endTime = jiffies_to_msecs(jiffies);
        u32threadtime = u32endTime;
        sprintf(str, "[%010lu] thread time %d  diff time %d res %d src time %d sleep time %d [%d], frame time %d\n", jiffies_to_msecs(jiffies), (u32endTime - u32startTime), u32diffTime, u32ResidualTime, (u32mixerendTime - u32mixerstartTime), u32SleepTime, u32SleepDeltaTime, u32MaxFrameTime);
        audio_write_file(API_AUDIO_FILE_PATH, str, strlen(str));
#endif
#endif

    }

    MDrv_AUDIO_WriteMaskReg(PCM_MIXER_DEBUG_REG, 0x000F, 0x0003);

    APIAUDIO_ERROR("%s: Exit!\n", __FUNCTION__);

    ret = 0;

    return ret;
}

MS_U16 _MApi_AUDIO_HDMI_RX_GetHdmiInChannelStatus(void)
{
    return(MDrv_AUDIO_HDMI_RX_GetHdmiInChannelStatus());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Open()
/// @brief \b Function \b Description : Open a PCM interface according to specific capabilities.
/// @param <IN>        \b void : Pointer to a block of memory with PCM information.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success a nonnegative value is returned, and this value is the chosen device-id. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Open(void *pData)
{
    MS_S32 s32RetId;

    s32RetId = MDrv_AUDIO_PCM_Open(pData);

    if (s32RetId >=0)
    {
        u32RenderPid_Table[s32RetId] = u32Gpid; // this is for decoder/pid mapping
        printf(" Open PCM ID %d by Gpid %u \n",  (int)s32RetId, (unsigned int)u32Gpid);/*@audio*/
    }

    return s32RetId;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Close()
/// @brief \b Function \b Description : Close an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Close(MS_S32 s32DeviceId)
{
    MS_S32 s32Ret = 0;

    if((u32RenderPid_Table[s32DeviceId] == u32Gpid)&&(u32Gpid != 0))  // mapping decoder resource and process pid
    {
        // Stop when release
        printf(" Release PCM ID %d by Gpid %u \n", (int)s32DeviceId, (unsigned int)u32Gpid);

        s32Ret = MDrv_AUDIO_PCM_Close(s32DeviceId);
        u32RenderPid_Table[s32DeviceId] = 0;
    }
    else  // DecId is used by other process
    {
        printf(" PCM ID %d can't be released by Gpid %u \n", (int)s32DeviceId, (unsigned int)u32Gpid);
        s32Ret = -A_EPERM;
    }

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Start()
/// @brief \b Function \b Description : Start an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Start(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_PCM_Start(s32DeviceId);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Stop()
/// @brief \b Function \b Description : Stop a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Stop(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_PCM_Stop(s32DeviceId);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Set()
/// @brief \b Function \b Description : Set configuration to an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b MS_U32 : The command type.
/// @param <IN>        \b void : Pointer to a block of memory with specific configuration, and this memory is not allowed to be updated by this API.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData)
{
    return MDrv_AUDIO_PCM_Set(s32DeviceId, u32Cmd, pData);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Get()
/// @brief \b Function \b Description : Get configuration from an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b MS_U32 : The command type.
/// @param <IN>        \b void : Pointer to a block of memory for receiving specific configuration.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData)
{
    return MDrv_AUDIO_PCM_Get(s32DeviceId, u32Cmd, pData);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Read()
/// @brief \b Function \b Description : Read a specific amount of PCM data from a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b void : Pointer to a block of memory with PCM data.
/// @param <IN>        \b MS_U32 : The size of PCM data.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_U32 : The total number of PCM data successfully written is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size)
{
    return MDrv_AUDIO_PCM_Read(s32DeviceId, pBuf, u32Size);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Write()
/// @brief \b Function \b Description : Write a specific amount of PCM data to a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b void : Pointer to a block of memory with PCM data, and this memory is not allowed to be updated by this API.
/// @param <IN>        \b MS_U32 : The size of PCM data.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_U32 : The total number of PCM data successfully written is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size)
{
    return MDrv_AUDIO_PCM_Write(s32DeviceId, pBuf, u32Size);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Flush()
/// @brief \b Function \b Description : Flush an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Flush(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_PCM_Flush(s32DeviceId);
}

#endif //UTOPIA_STRIP
