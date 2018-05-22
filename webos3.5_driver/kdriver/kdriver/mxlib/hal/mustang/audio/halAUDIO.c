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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#else
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "drvSYS.h"
#include "../sys/halSYS.h"

// Internal Definition
#include "drvAUDIO.h"
#include "regCHIP.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "halSIF.h"
#include "halSOUND.h"
#include "halAUR2.h"
#include "decR2_version.h"
#include "sndR2_version.h"
#include "audio_comm2.h"

#include "../audsp/halAUDSP.h"

#if defined(MSOS_TYPE_NUTTX)
extern int lib_lowprintf(const char *format, ...);
#define DBG_PREFIX lib_lowprintf
#else
#define DBG_PREFIX printf
#endif

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

#define HALAUDIO_PRINT(fmt, args...)    LOGI("<<android>>      " fmt, ## args)
#define HALAUDIO_ERROR(fmt, args...)    LOGE("<<android>>      " fmt, ## args)
#else
    #ifdef CONFIG_MBOOT
        #define HALAUDIO_PRINT(fmt, args...)
        #define HALAUDIO_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
    #else
        #define HALAUDIO_PRINT(fmt, args...)
        #define HALAUDIO_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
    #endif
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DBG_AUDIO(args...)       //DBG_PREFIX(args)
#define DBG_AUDIO_LOAD(args...)  //DBG_PREFIX(args)
#define DBG_AUDIO_ERROR(args...) //DBG_PREFIX(args)

#define AUD_ERRMSG(args...)  DBG_PREFIX(args)
#define AUD_DBGMSG(args...)  DBG_PREFIX(args)

#define HALAUDIO_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            HALAUDIO_ERROR("%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            HALAUDIO_ERROR("%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (HAL_AUDIO_InitialVars() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

#ifndef MSOS_TYPE_NOS
void* MDrv_MPool_PA2KSEG1(void* pAddrPhys);
#endif

#define HAL_AUDIO_MIXER_THREAD_DEBUG FALSE

#if HAL_AUDIO_MIXER_THREAD_DEBUG
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
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define AUDIO_HAL_ERR(x, args...)        //{printf(x, ##args);}
#define HINIBBLE(MS_U8)    ((MS_U8) / 0x10)
#define LONIBBLE(MS_U8)    ((MS_U8) & 0x0F)
#if (AUDIO_R2_DMA_READER1_SUPPORT || AUDIO_R2_DMA_READER2_SUPPORT)
#define AUDIO_R2_DMA_RDER_CFG_REG(base, offset)    ((MS_U32)(base + (offset * 2)))
#endif

#define HW_DMA_RDR1_BUF_UNIT    4096

#if AUDIO_HW_DMA_READER1_SUPPORT
#define HW_DMA_RDR1_BUF_ADDR    (DSP2_DMA_READER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define HW_DMA_RDR1_BUF_SIZE    ((DSP2_DMA_READER_DRAM_SIZE + 1) * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_HW_DMA_READER2_SUPPORT
#define HW_DMA_RDR2_BUF_ADDR    (DSP2_HW_DMA_READER2_DRAM_BASE * BYTES_IN_MIU_LINE)
#define HW_DMA_RDR2_BUF_SIZE    ((DSP2_HW_DMA_READER2_DRAM_SIZE + 1) * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_SW_DMA_READER1_SUPPORT
#define SW_DMA_RDR1_BUF_ADDR    (DSP2_SW_DMA_READER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define SW_DMA_RDR1_BUF_SIZE    ((DSP2_SW_DMA_READER_DRAM_SIZE + 1) * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_R2_DMA_READER1_SUPPORT
#define R2_DMA_RDR1_BUF_ADDR    (ASND_DSP_DDR_SIZE + OFFSET_MCU_R2_DMA1_READER_DRAM_BASE)
#define R2_DMA_RDR1_BUF_SIZE    MCU_R2_DMA1_READER_DRAM_SIZE
#endif

#if AUDIO_R2_DMA_READER2_SUPPORT
#define R2_DMA_RDR2_BUF_ADDR    (ASND_DSP_DDR_SIZE + OFFSET_MCU_R2_DMA2_READER_DRAM_BASE)
#define R2_DMA_RDR2_BUF_SIZE    MCU_R2_DMA2_READER_DRAM_SIZE
#endif

#if AUDIO_PCM_CAPTURE1_SUPPORT
#define PCM_CAPTURE1_BUF_UNIT    128
#define PCM_CAPTURE1_BUF_ADDR    (DSP2_PCM_CAPTURE_BUFFER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define PCM_CAPTURE1_BUF_SIZE    (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_PCM_CAPTURE2_SUPPORT
#define PCM_CAPTURE2_BUF_UNIT    128
#define PCM_CAPTURE2_BUF_ADDR    (DSP2_PCM_CAPTURE2_BUFFER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define PCM_CAPTURE2_BUF_SIZE    (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_HW_DMA_WRITER1_SUPPORT
#define HW_DMA_WTR1_BUF_ADDR    (DSP2_PCM_CAPTURE_BUFFER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define HW_DMA_WTR1_BUF_SIZE    ((DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE) * BYTES_IN_MIU_LINE)
#endif

//DMA Reader configuration
#define DMA_RDR_PCM_BUF_UNIT        4096
#define DMA_RDR_BYTES_IN_LINE       BYTES_IN_MIU_LINE
#define DMA_RDR_PCM_BUF_ADDR        (DSP2_DMA_READER_DRAM_BASE*DMA_RDR_BYTES_IN_LINE)
#define DMA_RDR_PCM_BUF_SIZE        ((DSP2_DMA_READER_DRAM_SIZE+1)*DMA_RDR_BYTES_IN_LINE)
#define DMA_RDR_PCM_BUF_TIME        100 /* ms */ //[LM14]

//PCM Capture configuration
#define PCM_CAPTURE_BYTES_IN_LINE     BYTES_IN_MIU_LINE
#define PCM_CAPTURE_BUF_ADDR_BASE     (DSP2_PCM_CAPTURE_BUFFER_DRAM_BASE*PCM_CAPTURE_BYTES_IN_LINE)
#define PCM_CAPTURE2_BUF_ADDR_BASE     (DSP2_PCM_CAPTURE2_BUFFER_DRAM_BASE*PCM_CAPTURE_BYTES_IN_LINE)

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define StereoAACOutputPCM 0

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
static int _PCMCallBackFunc(MS_U8 *buf, MS_U16 len) // [LM14]
{
    //printf("== buf(0x%lx), len(0x%x) == \r\n", (MS_U32)buf, len);
    return 1;
}

//mp3 encode
static MS_U32 MP3Encode_baseaddr = 0;
static MS_U32 MP3EncodeReadIndex = 0;
static MS_U32 MP3EncodeCopySize = 0x180; //byte

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
AUDIO_SOURCE_INFO_TYPE      AudioInputSrcInfo;
AUDIO_INPUT_TYPE            g_audioSrcType = AUDIO_DSP1_DVB_INPUT;
AUDIO_INPUT_TYPE            g_audioSubSrcType = AUDIO_DSP1_DVB_INPUT;
MS_U16                      g_BalanceMask = 0x0000;
MS_BOOL                     g_bAudio_loadcode_from_dram = 0, g_bIsDTV;
MS_U32                      _gMIO_MapBase;
MS_S32                      _s32AUDIOMutex = -1;
MS_S32                      _s32AUDIOMutexReboot = -1;
MS_S32                      _s32MutexLoadCode = -1;
MS_S32                      _s32AUDIOMutexIDMA = -1;
AUDIO_KERNEL_DEBUG_MSG      g_bAudio_EnableKernelMsg = E_AUDIO_KERNEL_DEBUG_MSG_OFF;
extern MS_BOOL g_bDSPLoadCode;
MS_U8 CheckPlayDoneCnt = 0;
MS_U32  g_u32bDTSCD = 0x00;
MS_BOOL bReBoot_All_Aud_Proc = FALSE;

extern AUDIO_SHARED_VARS2   * g_AudioVars2;

#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
#else
AUDIO_SHARED_VARS2          g_audioShared;
#endif

#ifndef MSOS_TYPE_NUTTX
static MS_BOOL g_bAudioTeeInfoShmInitFlag = FALSE;
#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
#else
AUDIO_TEE_INFO_SHARE_MEM   gAudioTeeInfoShm;
#endif
AUDIO_TEE_INFO_SHARE_MEM   *pAudioTeeInfoShm = NULL;

MS_BOOL g_bAudioTeeEnabled = FALSE, g_bAudioTeeMbxInitialized = FALSE;
MS_BOOL g_bDecDspSecureTeeAccessed = FALSE, g_bSndDspSecureTeeAccessed = FALSE;
#endif

// [LM14A]
PCM_OUTPUT_FORMAT stPcmSettings[NUM_OF_HW_DMA];
PCM_OUTPUT_FORMAT stSwDMAPcmSettings[NUM_OF_SW_DMA];
AUDIO_OMX_ARENDER_PATH_ID g_OMX_ArenderPathID = AUDIO_OMX_ARENDER_PATH_ID_NULL;

extern void (*Audio_Encoder_callback)(LGE_AENC_MSG_TYPE_T* pMassage);
int (*PCM_capture_callback)(MS_U8 *buf, MS_U16 len) = _PCMCallBackFunc;

MS_U32 DMA_Buffer_limit = 1;//for audio direct
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U32 curr_OS_Time=0;
static MS_S64   curr_PTS = 0;
static MS_BOOL  g_bInitShmFlag = FALSE;
static MS_BOOL  g_bEncodeDoneFlag = 0;
// for HW DMA Reader
static MS_U8  * dmaReader_writePtr;
static MS_U8  * dmaReader_bufStartAddress;
static MS_U8  * dmaReader_bufEndAddress;
// for HW DMA Writer
static MS_U8  * dmaWriter_readPtr;
static MS_U8  * dmaWriter_bufStartAddress;
static MS_U8  * dmaWriter_bufEndAddress;

static MS_U8    Dvb2DecCmd_tmp;
static AUDIO_DEC_ID pre_DecId = AU_DEC_ID1;
static MS_U8    Dvb2DecCmd_tmp;
static AUDIO_DEC_ID DTVDecPriority[AU_DEC_MAX] = {AU_DEC_ID1, AU_DEC_ID3 , AU_DEC_INVALID};
static AUDIO_DEC_ID HDMIDecPriority[AU_DEC_MAX] = {AU_DEC_ID3, AU_DEC_ID1 , AU_DEC_INVALID};
static AUDIO_DEC_ID MMDecPriority[AU_DEC_MAX] = {AU_DEC_ID3, AU_DEC_ID1 , AU_DEC_INVALID};
static AUDIO_DEC_ID VDDecPriority[AU_DEC_MAX] = {AU_DEC_ID1, AU_DEC_INVALID , AU_DEC_INVALID};

static MS_U8  * r2_dma1_writePtr;
static MS_U8  * r2_dma1_bufStartAddress;
static MS_U8  * r2_dma1_bufEndAddress;

static MS_U8  * r2_dma2_writePtr;
static MS_U8  * r2_dma2_bufStartAddress;
static MS_U8  * r2_dma2_bufEndAddress;

#ifdef MSOS_TYPE_NUTTX
static MS_S32  _s32MadEventId = -1;
static MS_S32  _s32MadTaskId = -1;
static MBX_Msg TeeToReeMbxAckMsg;
static MBX_Msg msg;
static void _MAD_Proc(void);
static void HAL_AUDIO_HandleDynamicMBX(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue);
#endif

MS_U32 g_pcm_capture_base_addr = 0;
MS_U32 g_pcm_capture2_base_addr = 0;

MS_U8 g_DSPMadMIUBank[MAX_AUDIO_BASE_ADDRESS_NUM2] = {0};
MS_U32 g_DSPBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2] = {0};
MS_U32 g_DSPMadBaseBufferAdr[MAX_AUDIO_BASE_ADDRESS_NUM2] = {0};
MS_U32 g_DSPDDRBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2] = {0};
extern MS_U32 (* gGetDDRInfoFuncPtr)(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

extern MS_U32  g_AudioDumpInfoCtrl;
extern MS_BOOL bIsNonPCMInDec2;
extern MS_U32 EncFrameIdx;
extern MS_U32 EncBuf_W_idx;
extern MS_U8   EncBuf_Count;
extern AU_DVB_ENC_FRAME_INFO MPEG_EN_BUF[6];

extern AUDIO_DEC_ID (* gOpenDecodeSystemFuncPtr)(AudioDecStatus_t * p_AudioDecStatus);
extern MS_BOOL (*pFuncPtr_Setsystem)(AUDIO_DEC_ID DecId, AudioDecStatus_t *p_AudioDecStatus);
extern MS_BOOL (* gDigitalOutChannelStatusFuncPtr)(DIGITAL_OUTPUT_TYPE , Digital_Out_Channel_Status_t *);
extern MS_U32 (* gGetDDRInfoFuncPtr)(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);


const MS_U32 u32PathArray[8]={0x2C64, 0x2C66, 0x2C68, 0x2C6A, 0x2C65, 0x2C67, 0x2C69, 0x2C6B};

//[LM14]
static MS_U8 u8LastMP3EncodeIndex = 0;
PCMRingBuffer RingBuffer_SwDmaReader[NUM_OF_SW_DMA];
PCMRingBuffer RingBuffer_DmaReader[NUM_OF_HW_DMA];
static MS_U32 audio_pcm_dmaRdr_bufSz;
static MS_U32 PCMCaptureBaseAddr = 0;

static unsigned int u32SwDmaLimitSize[2];

//[LM14A]
//device0: for BT
//device1: for AAC encode
//device2: for mp3 encode
//device3
//device4
//device5
MS_BOOL g_pcm_capture_device0_enable = FALSE;
MS_BOOL g_pcm_capture_device1_enable = FALSE;
MS_BOOL g_pcm_capture_device2_enable = FALSE;
MS_BOOL g_pcm_capture_device3_enable = FALSE;
MS_BOOL g_pcm_capture_device4_enable = FALSE;
MS_BOOL g_pcm_capture_device5_enable = FALSE;

//Refine power on sequence for earphone & DAC pop noise issue
const AUDIO_REG_TYPE  AudioPreInitTbl_Clock[] =
{
    //----------------pre-AUPLL-------------------------//Item 23
    {0x112c9e, 0xff, 0x20}, //[15:0]  2nd order synthesizer n.f[23:8]
    {0x112c9f, 0xff, 0x1c},
    {0x112c9c, 0xff, 0x00}, //[7:0]   2nd order synthesizer n.f[7:0]
    {0x112c9d, 0xff, 0xc0}, //[15]    2nd order synthesizer enable, [14] load
    {0x112ca0, 0xff, 0x20}, //[15:0]  codec synthesizer n.f << 112ca2[4] do not select codec synthesizer 1024FS clk
    {0x112ca1, 0xff, 0x1c},
    {0x112c9c, 0xff, 0x00},
    {0x112c9d, 0xff, 0x80}, //[14]    2nd order synthesizer load
    {0x112ca2, 0x08, 0x08}, //[3]     Enable reference clock
    {0x112ca2, 0x10, 0x10}, //[4]     Select 1st-order or 2nd-order synthesizer
    //----------------AUPLL control-------------------------//Item 23
    {0x112c70, 0xff, 0xd0},
    {0x112c71, 0xff, 0x12},
    {0x112c76, 0xff, 0x00},
    {0x112c77, 0xff, 0x03},

    //----------------AUR2PLL------------------------//Item 22
    {0x160342, 0xff, 0x01}, //
    {0x160343, 0xff, 0x00}, //[8]     Set AUR2PLL power down = 0
    {0x160348, 0x70, 0x30}, //AUR2PLL modify design after code freeze
    {0x160344, 0x30, 0x10}, //AUR2PLL modify design after code freeze
    {0x160346, 0xff, 0x24}, //AUR2PLL modify design after code freeze
    {0xffffff, 0x01, 0x01}, // delay 1m sec

    //----------------- DFS ---------------------------//Item 36
    {0x163c61, 0x03, 0x00}, //[9:8]   CLK DSP DFS SEL[1:0] = AUPLL
    {0x163c61, 0x10, 0x00}, //[12]    CLK DSP DFS SEL[2] = 0
    {0x163c61, 0x08, 0x08}, //[11]    CLK DSP DFS EN
    {0x163c60, 0x1f, 0x1f}, //[4:0]   DSP DFS DIV
    {0x163c60, 0x40, 0x40}, //[6]     DSP DFS EN
    {0x163c60, 0x80, 0x00}, //[7]     DSP DFS UPDATE
    {0x163c60, 0x80, 0x80}, //[7]     DSP DFS UPDATE
    {0x163c60, 0x80, 0x00}, //[7]     DSP DFS UPDATE
    {0x163c63, 0x03, 0x00}, //[9:8]   CLK R2 DFS SEL[1:0] = AUR2PLL
    {0x163c63, 0x10, 0x00}, //[12]    CLK R2 DFS SEL[2] = 0
    {0x163c63, 0x08, 0x08}, //[11]    CLK R2 DFS EN
    {0x163c62, 0x1f, 0x1f}, //[4:0]   R2 DFS DIV
    {0x163c62, 0x40, 0x40}, //[6]     R2 DFS EN
    {0x163c62, 0x80, 0x00}, //[7]     R2 DFS UPDATE
    {0x163c62, 0x80, 0x80}, //[7]     R2 DFS UPDATE
    {0x163c62, 0x80, 0x00}, //[7]     R2 DFS UPDATE


    //---------------- Enable CLK----------------------------//Item 32
    {0x112ca4, 0xff, 0xff}, //[10:0]  CLKGEN's clock enable
    {0x112ca5, 0xff, 0x1f},
    {0x112cb2, 0x90, 0x90}, //[7]     CLK R2 EN[3], [4] CLK PARSER EN
    {0x112cb3, 0x60, 0x60}, //[14:13] CLK R2 EN[2:1]
    {0x112ccb, 0x3c, 0x3c}, //[13:10] CLK DSP EN[4:1]
    //DMIO a0xx reset
    {0x112a80, 0xff, 0x40},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x5a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x5e},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x62},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x66},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x6a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x6e},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x72},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x76},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x76},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7e},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x82},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x86},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x8a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xb5},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xe0},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7a},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7e},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x82},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x86},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x8a},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x76},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xe0},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xe8},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x00},
    {0x112a81, 0xff, 0x00},
    {0x112c00, 0xff, 0x0d},
    {0x112c01, 0xff, 0x7f},
    {0x112c00, 0xff, 0x00},
    {0x112c01, 0xff, 0x00},
    //end DMIO a0xx reset

    //----------------Audio Reset---------------------------//Item 24
    {0x112c00, 0xff, 0x0d},
    {0x112c01, 0xff, 0x7f},
    {0x112c00, 0xff, 0x0f}, //[1]       CLKGEN_RESET
    {0x112c01, 0xff, 0x7f},
    {0x112c00, 0xff, 0x0d},
    {0x112c01, 0xff, 0x7f},
    {0x112c00, 0xff, 0x00},
    {0x112c01, 0xff, 0x00},

    //-----------------Synthesizer--------------------------//Item 29
    {0x112bcf, 0x01, 0x01}, // DVB1
    {0x112bce, 0x01, 0x01},
    {0x112ba8, 0xff, 0x94},
    {0x112ba9, 0xff, 0x11},
    {0x112baa, 0xff, 0x00},
    {0x112bab, 0xff, 0x00},
    {0x112c60, 0x80, 0x80},
    {0x112bce, 0x01, 0x00},
    {0x112bcf, 0x02, 0x02}, // DVB2
    {0x112bce, 0x02, 0x02},
    {0x112bac, 0xff, 0x94},
    {0x112bad, 0xff, 0x11},
    {0x112bae, 0xff, 0x00},
    {0x112baf, 0xff, 0x00},
    {0x112c62, 0x80, 0x80},
    {0x112bce, 0x02, 0x00},
    {0x112bcf, 0x04, 0x04}, // DVB3
    {0x112bce, 0x04, 0x04},
    {0x112bb0, 0xff, 0x94},
    {0x112bb1, 0xff, 0x11},
    {0x112bb2, 0xff, 0x00},
    {0x112bb3, 0xff, 0x00},
    {0x112c61, 0x80, 0x80},
    {0x112bce, 0x04, 0x00},
    {0x112bcf, 0x08, 0x08}, // DVB4
    {0x112bce, 0x08, 0x08},
    {0x112bb4, 0xff, 0x94},
    {0x112bb5, 0xff, 0x11},
    {0x112bb6, 0xff, 0x00},
    {0x112bb7, 0xff, 0x00},
    {0x112c63, 0x80, 0x80},
    {0x112bce, 0x08, 0x00},
    {0x112bcf, 0x10, 0x10}, // NonPCM
    {0x112bce, 0x20, 0x20},
    {0x112bb8, 0xff, 0x94},
    {0x112bb9, 0xff, 0x11},
    {0x112bba, 0xff, 0x00},
    {0x112bbb, 0xff, 0x00},
    {0x112c8b, 0x40, 0x40},
    {0x112bce, 0x20, 0x00},
    {0x112bcf, 0x20, 0x20}, // NonPCM2
    {0x112bce, 0x10, 0x10},
    {0x112bd0, 0xff, 0x94},
    {0x112bd1, 0xff, 0x11},
    {0x112bd2, 0xff, 0x00},
    {0x112bd3, 0xff, 0x00},
    {0x112c5f, 0x40, 0x40},
    {0x112bce, 0x10, 0x00},
    {0x112bcf, 0x40, 0x40}, // NonPCM3
    {0x112bcf, 0x80, 0x80},
    {0x112bd6, 0xff, 0x94},
    {0x112bd7, 0xff, 0x11},
    {0x112bd8, 0xff, 0x00},
    {0x112bd9, 0xff, 0x00},
    {0x112c48, 0x80, 0x80},
    {0x112bcf, 0x80, 0x00},
    {0x112c25, 0x20, 0x20}, // DVB5
    {0x112c25, 0x10, 0x10},
    {0x112c26, 0xff, 0x94},
    {0x112c27, 0xff, 0x11},
    {0x112c28, 0xff, 0x00},
    {0x112c29, 0xff, 0x00},
    {0x112c25, 0x80, 0x80},
    {0x112c25, 0x10, 0x00},
    {0x112c25, 0x02, 0x02}, // DVB6
    {0x112c25, 0x01, 0x01},
    {0x112c2a, 0xff, 0x94},
    {0x112c2b, 0xff, 0x11},
    {0x112c2c, 0xff, 0x00},
    {0x112c2d, 0xff, 0x00},
    {0x112c25, 0x08, 0x08},
    {0x112c25, 0x01, 0x00},
    {0x112bcf, 0x01, 0x00}, // DVB1    is controlled by DSP/R2
    {0x112bcf, 0x02, 0x00}, // DVB2    is controlled by DSP/R2
    {0x112bcf, 0x04, 0x00}, // DVB3    is controlled by DSP/R2
    {0x112bcf, 0x08, 0x00}, // DVB4    is controlled by DSP/R2
    {0x112c25, 0x20, 0x00}, // DVB5    is controlled by DSP/R2
    {0x112c25, 0x02, 0x00}, // DVB6    is controlled by DSP/R2
    {0x112bcf, 0x10, 0x00}, // NonPCM1 is controlled by DSP/R2
    {0x112bcf, 0x20, 0x00}, // NonPCM2 is controlled by DSP/R2
    {0x112bcf, 0x40, 0x00}, // NonPCM3 is controlled by DSP/R2

    //---------------- AudioBand ROM----------------------------//Item 35
    //additional 8 SRC needed

    //-----------------AudioBand----------------------------//Item 21
    {0x112b58, 0xff, 0xff}, //[15:0]  DWA fix value
    {0x112b59, 0xff, 0x01}, //[15:0]  DWA fix value
    {0x112b55, 0xf0, 0xf0}, //[15:12] DWA fix enable
    {0x112b46, 0xff, 0x00}, //[15:0]  Enable Group C
    {0x112b47, 0xff, 0xff},
    {0x112b56, 0xff, 0x30}, //[15:0]  Enable ADC1/2
    {0x112b57, 0xff, 0x00},
    {0x112c6e, 0xff, 0x88}, //[7:0]   Enable Grp A/B
    {0x112b50, 0xff, 0x80}, //[15:0]  DC offset value 9L
    {0x112b10, 0xff, 0x80}, //[15:0]  DC offset value 17L
    {0x112b51, 0xff, 0x00},
    {0x112b60, 0xff, 0xc0},
    {0x112b61, 0xff, 0x38},
    {0x112b62, 0xff, 0x38},
    {0x112b63, 0xff, 0x38},
    {0x112b40, 0xff, 0x4e}, //[15:0]  Init CODEC SRC
    {0x112b41, 0xff, 0x00},
    {0x112b54, 0xff, 0x00}, //[15:0]  DITHER select 0.5LSB
    {0x112b55, 0xff, 0x01},
    {0x112b52, 0xff, 0x0f}, //[15:0]  DC offset ENABLE, Dither ENABLE
    {0x112b53, 0xff, 0xf0},
    {0x112b40, 0xff, 0x0c}, //[15:0]  Init CODEC SRC
    {0x112b41, 0xff, 0x00},
    {0x112b54, 0x01, 0x01}, //[0]     SDM EN

    //-----------------De-POP_1-----------------------------//Item 25

    //-----------------Auto De-POP-----------------------------//Item 37
    {0x112cda, 0xff, 0x28}, //[1:0] PD_ADC
    {0x112cdb, 0xff, 0x03},
    {0x112ce3, 0x03, 0x00}, //[9:8] PD_INMUX
    {0x112ce6, 0x02, 0x00}, //[2] PD_MITOP
    {0x103346, 0x0f, 0x00}, //[0] clk_gate, [1]clk_inv, [3:2]clk_sel 0:3MHz/1:6MHz/2:24MHz/3:1.5MHz
    {0x112ce3, 0x20, 0x00}, // init
    {0x112ce9, 0x20, 0x00},
    {0x112ce9, 0x10, 0x00},
    {0x112cdd, 0x04, 0x04},
    {0x112cdd, 0x08, 0x08},
    {0x112cdd, 0x10, 0x10},
    {0x112cdd, 0x20, 0x20},
    {0x112cdc, 0x40, 0x40},
    {0x112cdc, 0x80, 0x80},
    {0x112cdd, 0x01, 0x01},
    {0x112cdd, 0x02, 0x02},
    {0x112cdc, 0x20, 0x20},
    {0x112cdc, 0x10, 0x10},
    {0x112cdf, 0x40, 0x00},
    {0x112cdf, 0x20, 0x00},
    {0x112cdf, 0x10, 0x00},
    {0x112cdf, 0x08, 0x00},
    {0x112ce1, 0x10, 0x10},
    {0x112ce0, 0x20, 0x20},
    {0x112ce0, 0x10, 0x10},
    {0x112ce3, 0x10, 0x00},
    {0x112ce6, 0x01, 0x01},
    {0x112ce9, 0x80, 0x80},
    {0x112ce9, 0x40, 0x00},
    {0x112ced, 0x10, 0x00},
    {0x112cec, 0x80, 0x80},
    {0x112cec, 0x10, 0x10},
    {0x112cec, 0x20, 0x00},
    {0x112cec, 0x08, 0x00},
    {0x112cef, 0x10, 0x00},
    {0x112ee0, 0x20, 0x20},
    {0x112cee, 0x80, 0x00},
    {0x112ee0, 0x04, 0x04},
    {0x112ee0, 0x08, 0x08},
    {0x112ee2, 0x01, 0x01},
    {0x112e72, 0xff, 0x00}, //step value
    {0x112e73, 0xff, 0x00},
    {0x112ec2, 0xff, 0x00},
    {0x112ec3, 0xff, 0x00},
    {0x112ec6, 0xff, 0x00},
    {0x112ec7, 0xff, 0x00},
    {0x112eca, 0xff, 0x00},
    {0x112ecb, 0xff, 0x00},
    {0x112ecc, 0xff, 0x00},
    {0x112ecd, 0xff, 0x00},
    {0x112ed2, 0xff, 0x00},
    {0x112ed3, 0xff, 0x00},
    {0x112ed6, 0xff, 0x00},
    {0x112ed7, 0xff, 0x00},
    {0x112e70, 0xff, 0x1a},
    {0x112e71, 0xff, 0xa0},
    {0x112ec0, 0xff, 0x1a},
    {0x112ec1, 0xff, 0xa0},
    {0x112ec4, 0xff, 0x9a},
    {0x112ec5, 0xff, 0xa0},
    {0x112ec8, 0xff, 0x9a},
    {0x112ec9, 0xff, 0xa0},
    {0x112ecc, 0xff, 0x9a},
    {0x112ecd, 0xff, 0xa0},
    {0x112ed0, 0xff, 0x9a},
    {0x112ed1, 0xff, 0xa0},
    {0x112ed4, 0xff, 0x9a},
    {0x112ed5, 0xff, 0xa0},
    {0x112e68, 0xff, 0x00}, //step time
    {0x112e69, 0xff, 0x03},
    {0x112e6a, 0xff, 0x4b},
    {0x112e6b, 0xff, 0x3e},
    {0x112e6c, 0xff, 0x2b},
    {0x112e6d, 0xff, 0x45},
    {0x112e6e, 0xff, 0x00},
    {0x112e6f, 0xff, 0x00},
    {0x112ed8, 0xff, 0x09}, //ausdm auto boot en
    {0x112ed9, 0xff, 0x00},
    {0x112ce3, 0x20, 0x00}, //final value
    {0x112ce9, 0x20, 0x00},
    {0x112ce9, 0x10, 0x00},
    {0x112cdd, 0x04, 0x00},
    {0x112cdd, 0x08, 0x00},
    {0x112cdd, 0x10, 0x00},
    {0x112cdd, 0x20, 0x00},
    {0x112cdc, 0x40, 0x00},
    {0x112cdc, 0x80, 0x00},
    {0x112cdd, 0x01, 0x00},
    {0x112cdd, 0x02, 0x00},
    {0x112cdc, 0x20, 0x00},
    {0x112cdc, 0x10, 0x00},
    {0x112cdf, 0x40, 0x40},
    {0x112cdf, 0x20, 0x20},
    {0x112cdf, 0x10, 0x10},
    {0x112cdf, 0x08, 0x08},
    {0x112ce1, 0x10, 0x00},
    {0x112ce0, 0x20, 0x00},
    {0x112ce0, 0x10, 0x00},
    {0x112ce3, 0x10, 0x10},
    {0x112ce6, 0x01, 0x00},
    {0x112ce9, 0x80, 0x00},
    {0x112ce9, 0x40, 0x00},
    {0x112ced, 0x10, 0x10},
    {0x112cec, 0x80, 0x00},
    {0x112cec, 0x10, 0x00},
    {0x112cec, 0x20, 0x20},
    {0x112cec, 0x08, 0x08},
    {0x112cef, 0x10, 0x10},
    {0x112ee0, 0x20, 0x20},
    {0x112cee, 0x80, 0x00},
    {0x112ee0, 0x04, 0x04},
    {0x112ee0, 0x08, 0x08},
    {0x112ee2, 0x01, 0x00},
    {0xffffff, 0x00, 0x00}, //        end of table
};

const AUDIO_REG_TYPE2 ResetDMIOTbl[]=
{

    //DMIO a0xx reset
    {0x112a80, 0xff, 0x40},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x5a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x5e},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x62},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x66},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x6a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x6e},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x72},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x76},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x76},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7e},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x82},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x86},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x8a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xb5},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xe0},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7a},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7e},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x82},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x86},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x8a},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x76},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xe0},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xe8},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x00},
    {0x112a81, 0xff, 0x00},
    {0x112c00, 0xff, 0x0d},
    {0x112c01, 0xff, 0x7f},
    {0x112c00, 0xff, 0x00},
    {0x112c01, 0xff, 0x00},
    //end DMIO a0xx reset
    {0xffffff, 0x00, 0x00}, // end of table
};

const AUDIO_REG_TYPE  AudioInitTbl[]=
{

    //-----------------De-POP_2-----------------------------//Item 26

    //-----------------Input: HDMI (only Vivaldi side)------//Item 1
    {0x112c44, 0xff, 0x00}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x00}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x01}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x01}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x02}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x02}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x03}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x03}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x04}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x04}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x05}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x05}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x06}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x06}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x07}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x07}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x08}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x08}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x09}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x09}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0a}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0a}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0b}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0b}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0c}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0c}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0d}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0d}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0e}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0e}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0f}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0f}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x10}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x10}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x11}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x11}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x12}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x12}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x13}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x13}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x14}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x14}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x15}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x15}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x16}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x16}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x17}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x17}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x18}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x18}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x19}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x19}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1a}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1a}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1b}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1b}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1c}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1c}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1d}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1d}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1e}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1e}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1f}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1f}, // [12:8]HDMI Matrix WD
    {0x153d6a, 0xff, 0x7f}, // [6:0] HDMI RX LAUNCH CFG
    {0x153d18, 0x33, 0x20}, // [2:0]HDMI status channel SEL, [5:4]HDMI status SEL
    {0x153d19, 0xff, 0x0d}, // [8]HDMI left justified, [9]MSB first, [15:10]HDMI status block start SEL
    {0x112c47, 0x83, 0x01}, // [8]DSD MODE SW SEL, [9]DSD MODE SW, [15]MCH EN
    {0x112c48, 0x10, 0x10}, // [4] npcm byte mode
    {0x112c48, 0x08, 0x08}, // [3] npcm decoder PA/PB order
    {0x112c48, 0x04, 0x00}, // [2] npcm decoder fast lock
    {0x112c48, 0x03, 0x00}, // [1:0] npcm decoder time out SEL

    //-----------------Input: I2S (default slave)---------------------------//Item 2
    {0x112c02, 0xff, 0x1f}, // [3:0] I2S/SIF/SPDIF clk EN, [4] I2S left-justified
    {0x112c03, 0xff, 0xc0}, // [14]AUTO_CLEAR_PC_PD_BAR, [15]SPDIF_IN_BLOCK_NO_CHECK
    {0x112cae, 0xff, 0x0e}, // [3:1] non-pcm synthesizer source SEL 432MHz
    {0x112caf, 0xff, 0x40}, // [15:13]i2s fs synthesizer SEL source clock 432MHz

    //-----------------Input: SPDIF-------------------------//Item 3
    {0x112c0a, 0xff, 0x70},
    {0x112c0b, 0xff, 0x13},

    //-----------------Input: ADC---------------------------//Item 4
    {0x112cda, 0xff, 0x2b},
    {0x112cdb, 0xff, 0x03},
    {0x112cda, 0xff, 0x28}, // [1:0]PD_ADC
    {0x112cdb, 0xff, 0x03},
    {0x112cf2, 0xff, 0x21}, // ADC set invert
    {0x112cf3, 0xff, 0x00}, //[11:8] REG_ADC1/2P_L/R_INV

    //-----------------Decode Path--------------------------//Item 28
    {0x112c60, 0xff, 0x80}, // Decoder 1 source from DVB1
    {0x112c61, 0xff, 0x80}, // Decoder 3 source from DVB3
    {0x112c62, 0xff, 0x80}, // Decoder 2 source from DVB2
    {0x112c63, 0xff, 0x87}, // Decoder 4 source from SIF

    //-----------------Input: Channel Source----------------//Item 6
    {0x112c64, 0xff, 0x80}, // CH1: Source0 , CH5: ADC0
    {0x112c65, 0xff, 0x80}, // Set CH5 default source from decoder
    {0x112c66, 0xff, 0x80}, // CH2: Source0 , CH6: Source1
    {0x112c67, 0xff, 0x81},
    {0x112c68, 0xff, 0x80}, // CH3: Source0 , CH7: Source3
    {0x112c69, 0xff, 0x83},
    {0x112c6a, 0xff, 0x80}, // CH4: Source0 , CH8: Source7
    {0x112c6b, 0xff, 0x87},

    //-----------------Output: I2S (BCK 64fs, msater)--------------------------//Item 10
    {0x112c90, 0x40, 0x00},
    {0x112c90, 0x1e, 0x00},
    {0x112c8d, 0x02, 0x00},
    {0x112c8d, 0x01, 0x00},
    {0x112c8c, 0x70, 0x20},
    {0x112c8c, 0x08, 0x00},
    {0x112c8c, 0x07, 0x02},
    {0x112c8d, 0x80, 0x80},
    {0x112c8d, 0x40, 0x40},
    {0x112c8d, 0x08, 0x08},
    {0xffffff, 0x01, 0x01}, // delay 1u sec
    {0x112c8d, 0x40, 0x00},
    {0x112c8d, 0x08, 0x00},

    //-----------------Output: SPDIF TX1--------------------//Item 11
    {0x112c90, 0x01, 0x00},
    {0x112c8b, 0x80, 0x80},
    {0x112c8b, 0x20, 0x00},
    {0x112c8b, 0x10, 0x00},
    {0x112c8b, 0x08, 0x00},
    {0x112c8b, 0x04, 0x00},
    {0x112c8b, 0x02, 0x00},
    {0x112c8b, 0x01, 0x00},
    {0x112c8a, 0x10, 0x10}, //[4]spdif status from RIU
    {0x112c8a, 0x08, 0x00},
    {0x112c8a, 0x04, 0x00},
    {0x112c8a, 0x03, 0x00},
    {0x112c8a, 0x80, 0x80},
    {0x112c8a, 0x40, 0x40},
    {0x112c8a, 0x20, 0x20},
    {0xffffff, 0x01, 0x01}, // delay 1u sec
    {0x112c8a, 0x40, 0x00},
    {0x112c8a, 0x20, 0x00},

    //-----------------Output: DAC--------------------------//Item 12
    {0x112cea, 0x08, 0x00}, //[3]  PD_OPLP_EAR
    {0x112cee, 0xff, 0x00}, //[1:0]SEL_DEPOP_EAR
    {0x112cef, 0xff, 0x03},
    {0x112cec, 0xff, 0x00},
    {0x112ced, 0xff, 0x18},

    //-----------------Output: SPDIF TX2--------------------//Item 13
    {0x112c5f, 0x80, 0x80},
    {0x112c5f, 0x40, 0x40},
    {0x112c5f, 0x20, 0x00},
    {0x112c5f, 0x10, 0x00},
    {0x112c5f, 0x08, 0x00},
    {0x112c5f, 0x04, 0x00},
    {0x112c5f, 0x02, 0x00},
    {0x112c5f, 0x01, 0x00},
    {0x112c5e, 0x10, 0x10}, //[4]spdif status from DSP/RIU
    {0x112c5e, 0x08, 0x00},
    {0x112c5e, 0x04, 0x00},
    {0x112c5e, 0x03, 0x00},
    {0x112c5e, 0x80, 0x80},
    {0x112c5e, 0x40, 0x40},
    {0x112c5e, 0x20, 0x20},
    {0xffffff, 0x01, 0x01}, // delay 1u sec
    {0x112c5e, 0x40, 0x00},
    {0x112c5e, 0x20, 0x00},
    {0x112c5e, 0x90, 0x90},

    //---------------------------------------------------//Item 30
    {0x112c94, 0x01, 0x01}, // enable CH1 HW force mute, [0] ch1 mute all
    {0x112c95, 0x01, 0x01}, // enable CH5 HW force mute, [8] ch5 mute all
    {0x112c96, 0x01, 0x01}, // enable CH2 HW force mute, [0] ch2 mute all
    {0x112c97, 0x01, 0x01}, // enable CH6 HW force mute, [8] ch6 mute all
    {0x112c98, 0x01, 0x01}, // enable CH3 HW force mute, [0] ch3 mute all
    {0x112c99, 0x01, 0x01}, // enable CH7 HW force mute, [8] ch7 mute all
    {0x112c9a, 0x01, 0x01}, // enable CH4 HW force mute, [0] ch4 mute all
    {0x112c9b, 0x01, 0x01}, // enable CH8 HW force mute, [8] ch8 mute all

    //-----------------Interrupt--------------------------//Item 35
    {0x112aa2, 0x40, 0x40}, //[6]       IRQ2n      select SPDIF_TX
    {0x112aa2, 0x10, 0x00}, //[4]       IRQ1Ln not select SPDIF_TX
    {0x112ad8, 0x20, 0x20}, //[5]       IRQ1Ln     select SPDIF_TX2

    //-----------------MIU & DMA Setting---------------------------//Item 34
    {0x112a44, 0xff, 0x00}, //[15]    maddma fifo addr demeta
    {0x112a45, 0xff, 0x80},
    {0x112ac4, 0xff, 0x00}, //[15]    Sedma fifo addr demeta
    {0x112ac5, 0xff, 0x80},
    {0x112a21, 0x04, 0x00}, //[10]    maddma fifo new mode
    {0x112aa1, 0x04, 0x00}, //[10]    sedma fifo new mode(no function)
    {0x163d03, 0x60, 0x20}, //[14:13] =01 for auR2 use MIU0, =10 for auR2 use MIU1
    {0x163d07, 0x60, 0x20}, //[14:13] =01 for auR2_1 use MIU0, =10 for auR2_1 use MIU1
    {0x112940, 0x20, 0x00}, //[5]     enable R2 Write buffer burst mode
    {0x113040, 0x20, 0x00}, //[5]     enable R2_1 Write buffer burst mode
    {0x112f68, 0xff, 0xff}, //        dma arb
    {0x112f69, 0xff, 0xff},
    {0x112f66, 0xff, 0x00}, //        dma arb
    {0x112f67, 0xff, 0x00},
    {0x112f64, 0xff, 0x00}, //        dma arb
    {0x112f65, 0xff, 0x04},
    {0x112f62, 0xff, 0x08}, //        dma arb
    {0x112f63, 0xff, 0x20},
    {0x112f60, 0xff, 0x15}, //        dma arb
    {0x112f61, 0xff, 0x80},
    {0x112f88, 0xff, 0xff}, //        cache arb
    {0x112f89, 0xff, 0xff},
    {0x112f86, 0xff, 0x00}, //        cache arb
    {0x112f87, 0xff, 0x00},
    {0x112f84, 0xff, 0x00}, //        cache arb
    {0x112f85, 0xff, 0x04},
    {0x112f82, 0xff, 0x08}, //        cache arb
    {0x112f83, 0xff, 0x20},
    {0x112f80, 0xff, 0x15}, //        cache arb
    {0x112f81, 0xff, 0x80},
    {0x163d88, 0xff, 0xff}, //        R2 arb
    {0x163d89, 0xff, 0xff},
    {0x163d86, 0xff, 0x00}, //        R2 arb
    {0x163d87, 0xff, 0x00},
    {0x163d84, 0xff, 0x00}, //        R2 arb
    {0x163d85, 0xff, 0x04},
    {0x163d82, 0xff, 0x08}, //        R2 arb
    {0x163d83, 0xff, 0x20},
    {0x163d80, 0xff, 0x15}, //        R2 arb
    {0x163d81, 0xff, 0x80},
    {0x163da8, 0xff, 0xff}, //        R2_1 arb
    {0x163da9, 0xff, 0xff},
    {0x163da6, 0xff, 0x00}, //        R2_1 arb
    {0x163da7, 0xff, 0x00},
    {0x163da4, 0xff, 0x00}, //        R2_1 arb
    {0x163da5, 0xff, 0x04},
    {0x163da2, 0xff, 0x08}, //        R2_1 arb
    {0x163da3, 0xff, 0x20},
    {0x163da0, 0xff, 0x15}, //        R2_1 arb
    {0x163da1, 0xff, 0x80},
    {0x163e28, 0xff, 0xff}, //        dma_l2 arb
    {0x163e29, 0xff, 0xff},
    {0x163e26, 0xff, 0x00}, //        dma_l2 arb
    {0x163e27, 0xff, 0x00},
    {0x163e24, 0xff, 0x00}, //        dma_l2 arb
    {0x163e25, 0xff, 0x04},
    {0x163e22, 0xff, 0x08}, //        dma_l2 arb
    {0x163e23, 0xff, 0x20},
    {0x163e20, 0xff, 0x15}, //        dma_l2 arb
    {0x163e21, 0xff, 0x80},

    //----------------- DFS ---------------------------//Item 36
    {0x163c61, 0x03, 0x00}, //[9:8]   CLK DSP DFS SEL[1:0] = AUPLL
    {0x163c61, 0x10, 0x00}, //[12]    CLK DSP DFS SEL[2] = 0
    {0x163c61, 0x08, 0x08}, //[11]    CLK DSP DFS EN
    {0x163c60, 0x1f, 0x1f}, //[4:0]   DSP DFS DIV
    {0x163c60, 0x40, 0x40}, //[6]     DSP DFS EN
    {0x163c60, 0x80, 0x00}, //[7]     DSP DFS UPDATE
    {0x163c60, 0x80, 0x80}, //[7]     DSP DFS UPDATE
    {0x163c60, 0x80, 0x00}, //[7]     DSP DFS UPDATE
    {0x163c63, 0x03, 0x00}, //[9:8]   CLK R2 DFS SEL[1:0] = AUR2PLL
    {0x163c63, 0x10, 0x00}, //[12]    CLK R2 DFS SEL[2] = 0
    {0x163c63, 0x08, 0x08}, //[11]    CLK R2 DFS EN
    {0x163c62, 0x1f, 0x1f}, //[4:0]   R2 DFS DIV
    {0x163c62, 0x40, 0x40}, //[6]     R2 DFS EN
    {0x163c62, 0x80, 0x00}, //[7]     R2 DFS UPDATE
    {0x163c62, 0x80, 0x80}, //[7]     R2 DFS UPDATE
    {0x163c62, 0x80, 0x00}, //[7]     R2 DFS UPDATE

// ----------------------------------------------------
//  sound effect init settings
//-----------------------------------------------------

    {0x112D01, 0xFF, 0x8C},      // AUOUT0 volume :  0dB ; Mute  @@VVV
    {0x112D03, 0xFF, 0x8C},      // AUOUT1 volume :  0dB ; Mute  @@VVV
    {0x112D05, 0xFF, 0x8C},      // AUOUT2 volume :  0dB ; Mute  @@VVV
    {0x112D07, 0xFF, 0x8C},      // AUOUT3 volume :  0dB ; Mute  @@VVV
    {0x112D09, 0xFF, 0x8C},      // I2S_OUT volume :  0dB ; Mute @@VVV
    {0x112D0B, 0xFF, 0x0C},      // SPDIF_OUT volume :  0dB ; UnMute   @@VVV

    {0x112D20, 0x80, 0x00},      // Disable EQ @@VVV
    {0x112D21, 0xFF, 0xC8},      // Enable Sound effect & tone @@VVV
    {0x112D22, 0x3F, 0x3F},      // Enable all output Volume control @@VVV
    {0x112D23, 0x01, 0x01},      // Enable SPDIF_OUT volume control   @@VVV
    {0x112D31, 0x02, 0x00},      // disable SE-DSP power-down command    @@VVV

    {0x112D50, 0xFF, 0xFF},      // DAC0/1 source default in NULL path  @@VVV
    {0x112D51, 0xFF, 0xFF},      // DAC2/3 source default in NULL path  @@VVV
    {0x112D52, 0xFF, 0xFF},      // I2S source default in NULL path  @@VVV
    {0x112D53, 0xFF, 0xFF},      // I2S source default in NULL path  @@VVV
    {0x112D54, 0xF0, 0x40},      // SRC source from PCM  @@VVV

    {0x112D59, 0xFF, 0x0C},      // set ch7 volume : 0dB; unmute
    {0x112D58, 0xFF, 0x00},
    {0x112D5D, 0xFF, 0x0C},      // set mix volume of CH5, CH6, CH8 to 0dB
    {0x112D5C, 0xFF, 0x00},
    {0x112D5F, 0xFF, 0x0C},
    {0x112D5E, 0xFF, 0x00},
    {0x112D5B, 0xFF, 0x0C},
    {0x112D5A, 0xFF, 0x00},
    {0xFFFFFF, 0x00, 0x00},       // end of table
};
#if AUDIO_HW_DMA_READER1_SUPPORT
AUDIO_PCM_t Audio_Pcm_HwDma_Reader1 = {
    .u8Name = AUDIO_HW_DMA_READER1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_HwDma_Reader1_Open,
        .close = HAL_AUDIO_PCM_HwDma_Reader1_Close,
        .start = HAL_AUDIO_PCM_HwDma_Reader1_Start,
        .stop = HAL_AUDIO_PCM_HwDma_Reader1_Stop,
        .set = HAL_AUDIO_PCM_HwDma_Reader1_Set,
        .get = HAL_AUDIO_PCM_HwDma_Reader1_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_HwDma_Reader1_Write,
        .flush = HAL_AUDIO_PCM_HwDma_Reader1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_HW_DMA_READER2_SUPPORT
AUDIO_PCM_t Audio_Pcm_HwDma_Reader2 = {
    .u8Name = AUDIO_HW_DMA_READER2,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_HwDma_Reader2_Open,
        .close = HAL_AUDIO_PCM_HwDma_Reader2_Close,
        .start = HAL_AUDIO_PCM_HwDma_Reader2_Start,
        .stop = HAL_AUDIO_PCM_HwDma_Reader2_Stop,
        .set = HAL_AUDIO_PCM_HwDma_Reader2_Set,
        .get = HAL_AUDIO_PCM_HwDma_Reader2_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_HwDma_Reader2_Write,
        .flush = HAL_AUDIO_PCM_HwDma_Reader2_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_SW_DMA_READER1_SUPPORT
AUDIO_PCM_t Audio_Pcm_SwDma_Reader1 = {
    .u8Name = AUDIO_SW_DMA_READER1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_SwDma_Reader1_Open,
        .close = HAL_AUDIO_PCM_SwDma_Reader1_Close,
        .start = HAL_AUDIO_PCM_SwDma_Reader1_Start,
        .stop = HAL_AUDIO_PCM_SwDma_Reader1_Stop,
        .set = HAL_AUDIO_PCM_SwDma_Reader1_Set,
        .get = HAL_AUDIO_PCM_SwDma_Reader1_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_SwDma_Reader1_Write,
        .flush = HAL_AUDIO_PCM_SwDma_Reader1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = TRUE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2, 10},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 96000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_R2_DMA_READER1_SUPPORT
AUDIO_PCM_t Audio_Pcm_R2Dma_Reader1 = {
    .u8Name = AUDIO_R2_DMA_READER1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_R2Dma_Reader1_Open,
        .close = HAL_AUDIO_PCM_R2Dma_Reader1_Close,
        .start = HAL_AUDIO_PCM_R2Dma_Reader1_Start,
        .stop = HAL_AUDIO_PCM_R2Dma_Reader1_Stop,
        .set = HAL_AUDIO_PCM_R2Dma_Reader1_Set,
        .get = HAL_AUDIO_PCM_R2Dma_Reader1_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_R2Dma_Reader1_Write,
        .flush = HAL_AUDIO_PCM_R2Dma_Reader1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = TRUE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2, 10},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_R2_DMA_READER2_SUPPORT
AUDIO_PCM_t Audio_Pcm_R2Dma_Reader2 = {
    .u8Name = AUDIO_R2_DMA_READER2,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_R2Dma_Reader2_Open,
        .close = HAL_AUDIO_PCM_R2Dma_Reader2_Close,
        .start = HAL_AUDIO_PCM_R2Dma_Reader2_Start,
        .stop = HAL_AUDIO_PCM_R2Dma_Reader2_Stop,
        .set = HAL_AUDIO_PCM_R2Dma_Reader2_Set,
        .get = HAL_AUDIO_PCM_R2Dma_Reader2_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_R2Dma_Reader2_Write,
        .flush = HAL_AUDIO_PCM_R2Dma_Reader2_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = TRUE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2, 10},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_PCM_CAPTURE1_SUPPORT
AUDIO_PCM_t Audio_Pcm_Capture1 = {
    .u8Name = AUDIO_PCM_CAPTURE1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_Capture1_Open,
        .close = HAL_AUDIO_PCM_Capture1_Close,
        .start = HAL_AUDIO_PCM_Capture1_Start,
        .stop = HAL_AUDIO_PCM_Capture1_Stop,
        .set = HAL_AUDIO_PCM_Capture1_Set,
        .get = HAL_AUDIO_PCM_Capture1_Get,
        .read = HAL_AUDIO_PCM_Capture1_Read,
        .write = NULL,
        .flush = HAL_AUDIO_PCM_Capture1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = TRUE,
        .u32Channel = {2},
        .u32SampleRate = {48000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_PCM_CAPTURE2_SUPPORT
AUDIO_PCM_t Audio_Pcm_Capture2 = {
    .u8Name = AUDIO_PCM_CAPTURE2,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_Capture2_Open,
        .close = HAL_AUDIO_PCM_Capture2_Close,
        .start = HAL_AUDIO_PCM_Capture2_Start,
        .stop = HAL_AUDIO_PCM_Capture2_Stop,
        .set = HAL_AUDIO_PCM_Capture2_Set,
        .get = HAL_AUDIO_PCM_Capture2_Get,
        .read = HAL_AUDIO_PCM_Capture2_Read,
        .write = NULL,
        .flush = HAL_AUDIO_PCM_Capture2_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = TRUE,
        .u32Channel = {2},
        .u32SampleRate = {48000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_HW_DMA_WRITER1_SUPPORT
AUDIO_PCM_t Audio_Pcm_HwDma_Writer1 = {
    .u8Name = AUDIO_HW_DMA_WRITER1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_HwDma_Writer1_Open,
        .close = HAL_AUDIO_PCM_HwDma_Writer1_Close,
        .start = HAL_AUDIO_PCM_HwDma_Writer1_Start,
        .stop = HAL_AUDIO_PCM_HwDma_Writer1_Stop,
        .set = HAL_AUDIO_PCM_HwDma_Writer1_Set,
        .get = HAL_AUDIO_PCM_HwDma_Writer1_Get,
        .read = HAL_AUDIO_PCM_HwDma_Writer1_Read,
        .write = NULL,
        .flush = HAL_AUDIO_PCM_HwDma_Writer1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = TRUE,
        .u32Channel = {2},
        .u32SampleRate = {48000},
        .u32BitWidth = {16},
    },
};
#endif

MS_U8 g_Common_PCM_IO_Init = FALSE;

AUDIO_PCM_t *g_PCM[AUDIO_PCM_MAXNUM] = {
#if AUDIO_HW_DMA_READER1_SUPPORT
    &Audio_Pcm_HwDma_Reader1,
#endif
#if AUDIO_HW_DMA_READER2_SUPPORT
    &Audio_Pcm_HwDma_Reader2,
#endif
#if AUDIO_SW_DMA_READER1_SUPPORT
    &Audio_Pcm_SwDma_Reader1,
#endif
#if AUDIO_R2_DMA_READER1_SUPPORT
    &Audio_Pcm_R2Dma_Reader1,
#endif
#if AUDIO_R2_DMA_READER2_SUPPORT
    &Audio_Pcm_R2Dma_Reader2,
#endif
#if AUDIO_PCM_CAPTURE1_SUPPORT
    &Audio_Pcm_Capture1,
#endif
#if AUDIO_PCM_CAPTURE2_SUPPORT
    &Audio_Pcm_Capture2,
#endif
#if AUDIO_HW_DMA_WRITER1_SUPPORT
    &Audio_Pcm_HwDma_Writer1,
#endif
};


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_RX_GetHdmiInAudioStatus()
/// @brief \b Function \b Description:  Return audio status.
/// @return MS_U16     \b : return structure which include pcm, non pcm, sampling rate.
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts)
{
    MS_U32  smpRate;
    MS_BOOL ret;
    ret = true;
    if (HAL_AUDIO_HDMI_NonpcmMonitor())
    {
        switch(HAL_AUDIO_HDMI_DolbyMonitor())
        {
            case 0x01:  p_hdmiAudioSts->audio_type = HDMI_RX_DD;        break;
            case 0x07:  p_hdmiAudioSts->audio_type = HDMI_RX_DP;        break;
            case 0x0b:  p_hdmiAudioSts->audio_type = HDMI_RX_DTS;       break;
            case 0x15:  p_hdmiAudioSts->audio_type = HDMI_RX_DDP;       break;
            default:    p_hdmiAudioSts->audio_type = HDMI_RX_Other;     break;
        }
    }
    else
        p_hdmiAudioSts->audio_type = HDMI_RX_PCM;

    if (_AU_AbsRead2Byte(0x101A04)&0x8000)
    {
        p_hdmiAudioSts->sampleRate = AUDIO_FS_NONE;
        ret = false;
    }
    else
    {
        smpRate = 216000000/_AU_AbsRead2Byte(0x101A04);

        if (smpRate>182400)  //192k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_192KHZ;
        }
        else if (smpRate>167200)  //176k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_176KHZ;
        }
        else if (smpRate>91200)  //92k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_96KHZ;
        }
        else if (smpRate>83600)  //88k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_88KHZ;
        }
        else if (smpRate>60800)  //64k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_64KHZ;
        }
        else if (smpRate>45600)  //48k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_48KHZ;
        }
        else if (smpRate>41800)  //44k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_44KHZ;
        }
        else if (smpRate>30400)  //32k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_32KHZ;
        }
        else if (smpRate>22800)  //24k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_24KHZ;
        }
        else if (smpRate>20900)  //22k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_22KHZ;
        }
        else if (smpRate>15200)  //16k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_16KHZ;
        }
        else if (smpRate>11400)  //12k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_12KHZ;
        }
        else if (smpRate>10450)  //11k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_11KHZ;
        }
        else if (smpRate>7600)  //8k*0.95
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_8KHZ;
        }
        else
        {
            p_hdmiAudioSts->sampleRate = AUDIO_FS_NONE;
        }
    }
    return ret;

}

MS_BOOL HAL_AUDIO_ResetDefaultVars(AUDIO_SHARED_VARS2 * pAUDIOShared)
{
    MS_U8 i = 0;
    MS_U8 array_mount = 0;

    if (pAUDIOShared == NULL)
    {
        return FALSE;
    }

    memset(pAUDIOShared, 0x00, sizeof(AUDIO_SHARED_VARS2));

    /* Fill in default value */
    pAUDIOShared->g_u8DspCodeType = MSAPI_AUD_DVB_INVALID;
    pAUDIOShared->g_u8Dsp2CodeType = MSAPI_AUD_DVB_INVALID;
    pAUDIOShared->g_u8SifCodeType = MSAPI_AUD_DVB_INVALID;
    pAUDIOShared->g_u8DecR2Adec1DecType = MSAPI_AUD_DVB_INVALID;
    pAUDIOShared->g_u8DecR2Adec2DecType = MSAPI_AUD_DVB_INVALID;
    pAUDIOShared->g_u8MADSkipResetFlag = FALSE;
    pAUDIOShared->g_u8MAD2SkipResetFlag = FALSE;
    pAUDIOShared->g_u8AdvSndEffDspCodeType = MSAPI_AUD_DVB_INVALID;
    pAUDIOShared->g_u8DspAliveFlag = TRUE;
    pAUDIOShared->g_u8AudioCrashFlag = FALSE;
    pAUDIOShared->g_eSpdifOutputType = SPDIF_OUT_NONE;
    pAUDIOShared->g_SpdifNpcmPath = AUDIO_DSP_ID_ALL;

    pAUDIOShared->g_u8SifDspType = DSP_SE;
    pAUDIOShared->g_dolby_truehd_enable = FALSE;
    pAUDIOShared->g_PreMainDspCodeType = MSAPI_AUD_DVB_INVALID;

    array_mount = sizeof(pAUDIOShared->sif_gain_0) / sizeof(pAUDIOShared->sif_gain_0[0]);
    for (i = 0; i < array_mount; i++)
    {
        /* Note! sif_gain_0[] is expected same length as sif_shift_0[] */
        pAUDIOShared->sif_gain_0[i] = 0x7FFF;
        pAUDIOShared->sif_shift_0[i] = 0x0000;
    }
    pAUDIOShared->g_u8IntTag = 1;
    pAUDIOShared->int_mb_cnt = 0;

    pAUDIOShared->CompressInfo = 0;
    for ( i = 0; i < MAX_AUDIO_BASE_ADDRESS_NUM2; i++ )
    {
        pAUDIOShared->g_DSPMadMIUBank[i] = g_DSPMadMIUBank[i];
        pAUDIOShared->g_DSPBinBaseAddress[i] = g_DSPBinBaseAddress[i];
        pAUDIOShared->g_DSPMadBaseBufferAdr[i] = g_DSPMadBaseBufferAdr[i];
        pAUDIOShared->g_DSPDDRBinBaseAddress[i] = g_DSPDDRBinBaseAddress[i];
    }

    pAUDIOShared->eMainSourceType = E_AUDIO_INFO_DTV_IN;
    pAUDIOShared->ePrevSourceType = E_AUDIO_INFO_DTV_IN;

    pAUDIOShared->eAudioSource = E_AUDIO_INFO_ADC_IN;
    pAUDIOShared->eMainAudioSource = AUDIO_NULL_INPUT;
    pAUDIOShared->eSubAudioSource = AUDIO_NULL_INPUT;
    pAUDIOShared->eScartAudioSource = AUDIO_NULL_INPUT;

    pAUDIOShared->AudioSystemInfo.dtv_system = DVB_MODE;

    pAUDIOShared->g_SPDIF_MuteStatus = 0;

    pAUDIOShared->stDigitalOutSetting[DIGITAL_SPDIF_OUTPUT].eDigitalOutfMode  = DIGITAL_OUT_NONE;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_SPDIF_OUTPUT].u8R2NonPcmSetting  = 0x00;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_SPDIF_OUTPUT].eSourceType  = E_AUDIO_INFO_ADC_IN;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_SPDIF_OUTPUT].u8NonPcmPath = AUDIO_DSP_ID_ALL;

    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_ARC_OUTPUT].eDigitalOutfMode  = DIGITAL_OUT_NONE;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_ARC_OUTPUT].u8R2NonPcmSetting  = 0x00;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_ARC_OUTPUT].eSourceType  = E_AUDIO_INFO_ADC_IN;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_ARC_OUTPUT].u8NonPcmPath = AUDIO_DSP_ID_ALL;

    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_OUTPUT].eDigitalOutfMode  = DIGITAL_OUT_NONE;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_OUTPUT].u8R2NonPcmSetting  = 0x00;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_OUTPUT].eSourceType  = E_AUDIO_INFO_ADC_IN;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_OUTPUT].u8NonPcmPath = AUDIO_DSP_ID_ALL;

    pAUDIOShared->g_dolbyLoudness_DTVsystem = DVB_MODE;
    pAUDIOShared->g_dolbyLoudness_Enable = 1;

    /* 1st decoder (depends on chip) */
    pAUDIOShared->AudioCaps[AU_DEC_ID1].u32AudioCapsVersion = AUD_CAP_VERSION;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].u32AudioCapsStructSize = sizeof(AUDIO_CAPABILITIES);
    pAUDIOShared->AudioCaps[AU_DEC_ID1].u8MaxDecoderNum = 2;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].u8MaxDmaReaderNum = 2;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsADSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsPusiDetectSupported = FALSE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsMpegSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAc3Supported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAc3pSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAacSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsMp3Supported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsWmaSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsRa8LbrSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsXpcmSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsDtsSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsWmaProSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsFlacSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsVorbisSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsDtsLbrSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAmrNbSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAmrWbSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsDraSupported = TRUE;

    /* 2nd decoder (depends on chip) */
    pAUDIOShared->AudioCaps[AU_DEC_ID3].u32AudioCapsVersion = AUD_CAP_VERSION;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].u32AudioCapsStructSize = sizeof(AUDIO_CAPABILITIES);
    pAUDIOShared->AudioCaps[AU_DEC_ID3].u8MaxDecoderNum = 2;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].u8MaxDmaReaderNum = 2;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsADSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsPusiDetectSupported = FALSE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsMpegSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAc3Supported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAc3pSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAacSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsMp3Supported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsWmaSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsRa8LbrSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsXpcmSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsDtsSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsWmaProSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsFlacSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsVorbisSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsDtsLbrSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAmrNbSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAmrWbSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsDraSupported = TRUE;

    pAUDIOShared->g_AudioSpecifiedDigitalOutput.u32StructVersion = AUDIO_SPECIFIED_DIGITAL_OUTPUT_VERSION;
    pAUDIOShared->g_AudioSpecifiedDigitalOutput.u32StructSize = sizeof(AUDIO_SPECIFIED_DIGITAL_OUTPUT_t);
    pAUDIOShared->g_AudioSpecifiedDigitalOutput.u32SpdifOutputCodec = DIGITAL_OUTPUT_CODEC_NOT_SPECIFIED;
    pAUDIOShared->g_AudioSpecifiedDigitalOutput.u32HdmiArcOutputCodec = DIGITAL_OUTPUT_CODEC_NOT_SPECIFIED;

    // [LM14]
    pAUDIOShared->bMP3EncodeDebug = FALSE;
    pAUDIOShared ->g_drcMode_adec1 = RF_MODE;
    pAUDIOShared ->g_drcMode_adec2 = RF_MODE;

    pAUDIOShared->g_u32PcmMixerMaxElement = DSP2_PCM_MIXER_MAX_ELEMENT;
    pAUDIOShared->g_u32PcmMixerMaxGroup = DSP2_PCM_MIXER_MAX_GROUP;
    pAUDIOShared->g_u32PcmMixerElementInfoBase = DSP2_PCM_MIXER_ELEMENT_INFO_BASE;
    pAUDIOShared->g_u32PcmMixerElementBufferBase = DSP2_PCM_MIXER_ELEMENT_BUFFER_BASE;
    pAUDIOShared->g_u32PcmMixerElementBufferSize = DSP2_PCM_MIXER_ELEMENT_BUFFER_SIZE;
    pAUDIOShared->g_u32PcmMixer1MixedBufferBase = DSP2_PCM_MIXER1_MIXED_BUFFER_BASE;
    pAUDIOShared->g_u32PcmMixer2MixedBufferBase = DSP2_PCM_MIXER2_MIXED_BUFFER_BASE;
    pAUDIOShared->g_u32PcmMixerMixedBufferSize = DSP2_PCM_MIXER_MIXED_BUFFER_SIZE;

    // [LM15]
    pAUDIOShared->g_bR2Adec1AD = FALSE;
    pAUDIOShared->g_bR2Adec2AD = FALSE;

    pAUDIOShared->g_R2_NonPcm_Sel = ADEC1;
    pAUDIOShared->g_Pre_R2_NonPcm_Sel = -1;

    pAUDIOShared->g_bATVExist = FALSE;
    pAUDIOShared->g_bPreNotMM_Flag = TRUE;
    pAUDIOShared->g_bTrickPlay = FALSE;

    pAUDIOShared->g_bNonPCMMixedSystemSound = FALSE;

    pAUDIOShared->g_bAdec1TVMute = FALSE;
    pAUDIOShared->g_bAdec2TVMute = FALSE;
    pAUDIOShared->g_bAdec1GSTMute = FALSE;
    pAUDIOShared->g_bAdec2GSTMute = FALSE;
    return TRUE;
}

AUDIO_SHARED_VARS2 * HAL_AUDIO_AllocateVars(void)
{
    AUDIO_SHARED_VARS2 * pAUDIOShared = NULL;

#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
    MS_U32 u32ShmId = 0;
    MS_U32 u32Addr = 0;
    MS_U32 u32BufSize = 0;

    HALAUDIO_ERROR("Start %s InitShmFlag=%d\r\n", __FUNCTION__, g_bInitShmFlag);

    if (MsOS_SHM_GetId((MS_U8 *)AUDIO_SHM_NAME, sizeof(AUDIO_SHARED_VARS2), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY) == TRUE)
    {
        pAUDIOShared = (AUDIO_SHARED_VARS2 *) u32Addr;

        HALAUDIO_ERROR("%s QUERY ClientCounter=%ld\r\n", __FUNCTION__, pAUDIOShared->g_u32ClientCounter);

        if (pAUDIOShared->g_u32ClientCounter == 0)
        {
            HALAUDIO_ERROR("Resume from Suspend mode, reset Audio SHM data ...\n");
        }
    }
    else
    {
        HALAUDIO_ERROR("create Audio SHM data ...\n");

        if (MsOS_SHM_GetId((MS_U8 *)AUDIO_SHM_NAME, sizeof(AUDIO_SHARED_VARS2), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE) == TRUE)
        {
            HALAUDIO_ERROR("Audio SHM data is created\n");
            pAUDIOShared = (AUDIO_SHARED_VARS2 *) u32Addr;

            HALAUDIO_ERROR("reset Audio SHM data ...\n");
            HAL_AUDIO_ResetDefaultVars(pAUDIOShared);
        }
        else
        {
            HALAUDIO_ERROR("AUDIO SHM allocation failed!\n");
            return NULL;
        }
    }

    if (g_bInitShmFlag == FALSE)
    {
        g_bInitShmFlag = TRUE;
        pAUDIOShared->g_u32ClientCounter++;
    }

    HALAUDIO_ERROR("End %s InitShmFlag=%d, ClientCounter=%ld \r\n", __FUNCTION__, g_bInitShmFlag, pAUDIOShared->g_u32ClientCounter);

#else
    pAUDIOShared = &g_audioShared;

    if (g_bInitShmFlag == FALSE)
    {
        HALAUDIO_PRINT("create Audio SHM data ...\n");
        g_bInitShmFlag = TRUE;

        HAL_AUDIO_ResetDefaultVars(pAUDIOShared);

        pAUDIOShared->g_u32ClientCounter++;
    }
#endif

    gOpenDecodeSystemFuncPtr = HAL_AUDIO_OpenDecodeSystem;
    gDigitalOutChannelStatusFuncPtr = HAL_AUDIO_DigitalOut_SetChannelStatus;
    pFuncPtr_Setsystem = HAL_AUDIO_SetDecodeSystem;
    gGetDDRInfoFuncPtr = HAL_AUDIO_GetDDRInfo;

    return pAUDIOShared;
}

MS_BOOL HAL_AUDIO_DeAllocateVars(void)
{
    AUDIO_SHARED_VARS2 * pAUDIOShared = NULL;

#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
    MS_U32 u32ShmId = 0;
    MS_U32 u32Addr = 0;
    MS_U32 u32BufSize = 0;

    HALAUDIO_ERROR("Start HAL_AUDIO_DeAllocateVars g_bInitShmFlag=%d\r\n", g_bInitShmFlag);

    if (g_bInitShmFlag == TRUE)
    {
        g_bInitShmFlag = FALSE;

        if (MsOS_SHM_GetId((MS_U8 *)AUDIO_SHM_NAME, sizeof(AUDIO_SHARED_VARS2), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY) == FALSE)
        {
            HALAUDIO_ERROR("%s() : No AUDIO SHM is created!\n", __FUNCTION__);
            return FALSE;
        }

        pAUDIOShared = (AUDIO_SHARED_VARS2 *)u32Addr;
        pAUDIOShared->g_u32ClientCounter--;

        HALAUDIO_ERROR("HAL_AUDIO_DeAllocateVars ClientCounter=%ld\r\n", pAUDIOShared->g_u32ClientCounter);

        if (pAUDIOShared->g_u32ClientCounter == 0)
        {
            HAL_AUDIO_ResetDefaultVars(pAUDIOShared);

            HALAUDIO_ERROR("free Audio SHM data ...\n");
#if defined(MSOS_TYPE_LINUX)
            if (MsOS_SHM_FreeId((MS_U8 *)AUDIO_SHM_NAME, u32ShmId) == TRUE)
            {
                HALAUDIO_ERROR("Audio SHM data is freed\n");
            }
            else
            {
                HALAUDIO_ERROR("%s() : AUDIO SHM de-allocation failed!\n", __FUNCTION__);
                return FALSE;
            }
#endif

            HAL_AUDIO_Mutex_DeInit();
        }
        /* patch for DC off/on no sound issue start */
        else
        {
            HAL_AUDIO_ResetDefaultVars(pAUDIOShared);
        }
        /* patch for DC off/on no sound issue end */
    }

    HALAUDIO_ERROR("End HAL_AUDIO_DeAllocateVars g_bInitShmFlag=%d\r\n", g_bInitShmFlag);
#else
    pAUDIOShared = &g_audioShared;

    if (g_bInitShmFlag == TRUE)
    {
        HALAUDIO_PRINT("free Audio SHM data ...\n");
        g_bInitShmFlag = FALSE;

        memset(pAUDIOShared, 0x00, sizeof(AUDIO_SHARED_VARS2));
    }
#endif

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_InitialVars()
/// @brief \b Function \b Description: This routine set audio share memory of Aduio module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_InitialVars(void)
{
    if (HAL_AUDIO_GET_INIT_FLAG() == FALSE)
    {
        g_AudioVars2 = HAL_AUDIO_AllocateVars();
        if (g_AudioVars2 == NULL)
        {
            return FALSE;
        }

#ifndef MSOS_TYPE_NUTTX
        pAudioTeeInfoShm = HAL_AUDIO_AllocateTeeInfoShm();
        if (pAudioTeeInfoShm == NULL)
        {
            HALAUDIO_ERROR("Create Audio TEE INFO SHM data fail...\n");
            return FALSE;
        }
    }
#else
        {
#ifdef NoMBX
            MS_U8 audio_mbx_class=0;

            MApi_MBX_GenerateDynamicClass(E_MBX_CPU_MIPS, "MAD", (MS_U8 *)&audio_mbx_class);
            if ( E_MBX_SUCCESS != MApi_MBX_RegisterMSGWithCallBack(audio_mbx_class, 10, HAL_AUDIO_HandleDynamicMBX))
            {
                HALAUDIO_ERROR("MAD Register MBX MSG error\n");
               return FALSE;
            }
            else
            {
                DBG_AUDIO("MAD MBX register msg ok %d\n", audio_mbx_class);
            }
#endif

            _s32MadEventId = MsOS_CreateEventGroup("MAD_Event");
            if (_s32MadEventId < 0)
            {
                HALAUDIO_ERROR("MAD CreateEventGroup error....\n");
                return FALSE;
            }

            _s32MadTaskId = MsOS_CreateTask((TaskEntry)_MAD_Proc,
                    NULL,
                    E_TASK_PRI_MEDIUM,
                    TRUE,
                    NULL,
                    32, // stack size..
                    "MAD_ISR_Task");
            if (_s32MadTaskId < 0)
            {
                MsOS_DeleteEventGroup(_s32MadEventId);
                HALAUDIO_ERROR("MAD CreateTask error....\n");
                return FALSE;
            }
        }
    }
#endif

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DeInitialVars()
/// @brief \b Function \b Description: This routine delete audio share memory of Aduio module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_DeInitialVars(void)
{
    MS_BOOL ret = TRUE;

    if (HAL_AUDIO_GET_INIT_FLAG() == TRUE)
    {
        HAL_AUDIO_SET_INIT_FLAG(FALSE);

        ret = HAL_AUDIO_DeAllocateVars();
        if (ret == FALSE)
        {
            HALAUDIO_ERROR ("%s() : AUDIO SHM de-allocation failed!\n", __FUNCTION__);
        }

#if 0  /* patch for STR resume segmentation fault issue */
        g_AudioVars2 = NULL;
#endif

#ifndef MSOS_TYPE_NUTTX
        {
            MS_BOOL ret1;

            ret1 = HAL_AUDIO_DeAllocateTeeInfoShm();
            if (ret1 == FALSE)
            {
                HALAUDIO_ERROR("%s() : AUDIO TEE Info SHM de-allocation failed!\n", __FUNCTION__);
                ret = FALSE;
            }
        }
#endif
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SET_INIT_FLAG()
/// @brief \b Function \b Description: This routine set the initial status of Aduio module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  MS_BOOL    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SET_INIT_FLAG(MS_BOOL bSet)
{
    HALAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_Audio_InitFlag = bSet;

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GET_INIT_FLAG()
/// @brief \b Function \b Description: This routine get the initial status of Aduio module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  MS_BOOL    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GET_INIT_FLAG(void)
{
    if (g_AudioVars2 == NULL)
    {
        return FALSE;
    }

    return g_AudioVars2->g_Audio_InitFlag;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Mutex_Init()
/// @brief \b Function \b Description: This routine is the initialization of Mutex for Audio module.
/// @param <IN>        \b NONE :
/// @param <OUT>       \b NONE :
/// @param <RET>       \b  MS_BOOL : TRUE - Success, FALSE - Fail
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_Mutex_Init(void)
{
    MS_BOOL ret = TRUE;

    if ((_s32AUDIOMutex != -1) &&
        (_s32AUDIOMutexReboot != -1) &&
        (_s32MutexLoadCode != -1) &&
        (_s32AUDIOMutexIDMA != -1))
    {
        return ret;
    }

    if (_s32AUDIOMutex == -1)
    {
        _s32AUDIOMutex = OS_CREATE_MUTEX(AUDIO);
        if (_s32AUDIOMutex == -1)
        {
            HALAUDIO_ERROR("Error! Create Mutex failed!\n");
            ret = FALSE;
        }
    }

    if (_s32AUDIOMutexReboot == -1)
    {
        _s32AUDIOMutexReboot = OS_CREATE_MUTEX(AUDIOreboot);
        if (_s32AUDIOMutexReboot == -1)
        {
            HALAUDIO_ERROR("Error! Create Mutex for Reboot failed!\n");
            ret = FALSE;
        }
    }

    if (_s32MutexLoadCode == -1)
    {
        _s32MutexLoadCode = OS_CREATE_MUTEX(LOAD_DECDSP);
        if (_s32MutexLoadCode == -1)
        {
            HALAUDIO_ERROR("Error! Create Mutex for Load DSP failed!\n");
            ret = FALSE;
        }
    }

    if (_s32AUDIOMutexIDMA == -1)
    {
        _s32AUDIOMutexIDMA = OS_CREATE_MUTEX(AUDIO_IDMA);
        if (_s32AUDIOMutexIDMA == -1)
        {
            HALAUDIO_ERROR("Error! Create Mutex for IMDA failed!\n");
            ret = FALSE;
        }
    }

    if (ret == FALSE)
    {
        if (_s32AUDIOMutex != -1)
        {
            OS_DELETE_MUTEX(_s32AUDIOMutex);
            _s32AUDIOMutex = -1;
        }

        if (_s32AUDIOMutexReboot != -1)
        {
            OS_DELETE_MUTEX(_s32AUDIOMutexReboot);
            _s32AUDIOMutexReboot = -1;
        }

        if (_s32MutexLoadCode != -1)
        {
            OS_DELETE_MUTEX(_s32MutexLoadCode);
            _s32MutexLoadCode = -1;
        }

        if (_s32AUDIOMutexIDMA != -1)
        {
            OS_DELETE_MUTEX(_s32AUDIOMutexIDMA);
            _s32AUDIOMutexIDMA = -1;
        }
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Mutex_DeInit()
/// @brief \b Function \b Description: This routine is the deletion of Mutex for Audio module.
/// @param <IN>        \b NONE :
/// @param <OUT>       \b NONE :
/// @param <RET>       \b  MS_BOOL : TRUE - Success, FALSE - Fail
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_Mutex_DeInit(void)
{
    MS_BOOL ret = TRUE;

    if ((_s32AUDIOMutex == -1) &&
        (_s32AUDIOMutexReboot == -1) &&
        (_s32MutexLoadCode == -1) &&
        (_s32AUDIOMutexIDMA == -1))
    {
        return ret;
    }

    if (_s32AUDIOMutex != -1)
    {
        if (OS_DELETE_MUTEX(_s32AUDIOMutex) != TRUE)
        {
            HALAUDIO_ERROR("Error! Delete Mutex failed!\n");
            ret = FALSE;
        }
        _s32AUDIOMutex = -1;
    }

    if (_s32AUDIOMutexReboot != -1)
    {
        if (OS_DELETE_MUTEX(_s32AUDIOMutexReboot) != TRUE)
        {
            HALAUDIO_ERROR("Error! Delete Mutex for Reboot failed!\n");
            ret = FALSE;
        }
        _s32AUDIOMutexReboot = -1;
    }

    if (_s32MutexLoadCode != -1)
    {
        if (OS_DELETE_MUTEX(_s32MutexLoadCode) != TRUE)
        {
            HALAUDIO_ERROR("Error! Delete Mutex for Load DSP failed!\n");
            ret = FALSE;
        }
        _s32MutexLoadCode = -1;
    }

    if (_s32AUDIOMutexIDMA != -1)
    {
        if (OS_DELETE_MUTEX(_s32AUDIOMutexIDMA) != TRUE)
        {
            HALAUDIO_ERROR("Error! Delete Mutex for IMDA failed!\n");
            ret = FALSE;
        }
        _s32AUDIOMutexIDMA = -1;
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetChipType()  @@Cathy
/// @brief \b Function \b Description: This routine is used to get the IC version for API.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  AU_CHIP_TYPE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AU_CHIP_TYPE HAL_AUDIO_GetChipType(void)
{
     return AU_CHIP_MUSTANG;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Set_MMIO_BASE()  @@Cathy
/// @brief \b Function \b Description: This routine is used to set MMIO base
/// @param <IN>        \b u32_MMIO_MapBase    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b      :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_Set_MMIO_BASE(MS_U32 u32_MMIO_MapBase)
{
    _gMIO_MapBase = u32_MMIO_MapBase; // Get the absolute MMIO address
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsReadByte  @@Cathy
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8 : 8-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_AbsReadByte(MS_U32 u32RegAddr)
{
    return (_AU_AbsReadByte((u32RegAddr-0x100000)));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_ReadByte  @@Cathy
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8 : 8-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_ReadByte(MS_U32 u32RegAddr)
{
    return (_AU_AbsReadByte(u32RegAddr+0x010000));  // Add  audio bank offset
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsReadReg  @@Cathy
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16 : 16-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_AbsReadReg(MS_U32 u32RegAddr)
{
    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    return _AU_AbsRead2Byte((u32RegAddr-0x100000));  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_ReadReg  @@Cathy
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16 : 16-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_ReadReg(MS_U32 u32RegAddr)
{
    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    return (_AU_AbsRead2Byte(u32RegAddr+0x010000));  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsWriteReg
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_AbsWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    _AU_AbsWrite2Byte((u32RegAddr-0x100000),u16Val);  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_WriteReg  @@Cathy
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    _AU_AbsWrite2Byte((u32RegAddr+0x010000),u16Val);  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsWriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_AbsWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
   _AU_AbsWriteByte((u32RegAddr-0x100000), u8Val); // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_WriteByte  @@Cathy
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
   _AU_AbsWriteByte((u32RegAddr+0x010000), u8Val); // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsWriteMaskByte  @@Morris
/// @brief \b Function  \b Description: Mask write 8-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 8-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_AbsWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val)
{
    _AU_AbsWriteMaskByte((u32RegAddr-0x100000), u8Mask, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_WriteMaskByte  @@Cathy
/// @brief \b Function  \b Description: Mask write 8-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 8-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val)
{
    _AU_AbsWriteMaskByte((u32RegAddr+0x010000), u8Mask, u8Val);  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsWriteMaskReg
/// @brief \b Function  \b Description: Mask write 16-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 16-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_AbsWriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val)
{
    MS_U16 u16RegVal;

    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    u16RegVal = _AU_AbsRead2Byte((u32RegAddr-0x100000));
    u16RegVal = ((u16RegVal & (~(u16Mask))) | (u16Val & u16Mask));
    _AU_AbsWrite2Byte((u32RegAddr-0x100000), u16RegVal);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_WriteMaskReg  @@Cathy
/// @brief \b Function  \b Description: Mask write 16-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 16-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val)
{
    MS_U16 u16RegVal;

    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    u16RegVal = HAL_AUDIO_ReadReg(u32RegAddr);
    u16RegVal = ((u16RegVal & (~(u16Mask))) | (u16Val & u16Mask));
    HAL_AUDIO_WriteReg(u32RegAddr, u16RegVal);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _HAL_AUDIO_Write4Byte  @@Cathy
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr+0x010000] =(MS_U16) (u32Val&0xFFFF);
    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr+2+0x010000] =(MS_U16) (u32Val>>16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ReadMailBox()    @@Need_Modify
/// @brief \b Function \b Description:  This routine is used to read the Dec or SE DSP mail box value
/// @param <IN>        \b bDspType    : 0 --DEC     1 -- SE
/// @param <IN>        \b u8ParamNum    : Mail box address
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  U16    : Mail Box value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum)
{
    MS_U16 u16Tmp1, u16Tmp2;
    MS_U32 i, u32MailReg;


    for (i=0; i<1000; i++)
    {
        if(bDspType==DSP_SE)
        {
            if(u8ParamNum<8)
            {
                u32MailReg = 0x2D70+(MS_U32)u8ParamNum * 2;
            }
            else
            {
                u32MailReg = REG_SE_D2M_MAIL_BOX_BASE+(MS_U32)(u8ParamNum-8) * 2;
            }
            u16Tmp1 = HAL_AUDIO_ReadReg(u32MailReg);
            u16Tmp2 = HAL_AUDIO_ReadReg(u32MailReg);
        }
        else
        {
            if(u8ParamNum<8)
            {
                u32MailReg = 0x2D60+(MS_U32)u8ParamNum * 2;
            }
            else
            {
                u32MailReg = REG_DEC_D2M_MAIL_BOX_BASE+(MS_U32)(u8ParamNum-8) * 2;
            }
            u16Tmp1 = HAL_AUDIO_ReadReg(u32MailReg);
            u16Tmp2 = HAL_AUDIO_ReadReg(u32MailReg);
        }
        if(u16Tmp1==u16Tmp2)
        {
          return u16Tmp1;
        }
    }

    HALAUDIO_ERROR("Read Mailbox fail! \r\n");
    return 0;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WriteDecMailBox()   @@Need_Modify
/// @brief \b Function \b Description:  This routine is used to write Dec-DSP mail box
/// @param <IN>        \b bDspType    : 0 --DEC     1 -- SE
/// @param <IN>        \b u8ParamNum    : Mail box address
/// @param <IN>        \b u16Data    :  value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data)
{
    MS_U32 u32MailReg;

    if(bDspType==DSP_SE)
    {
        u32MailReg = REG_SE_M2D_MAIL_BOX_BASE + (MS_U32)u8ParamNum* 2;
        HAL_AUDIO_WriteReg(u32MailReg, u16Data);
    }
    else
    {
        u32MailReg = REG_DEC_M2D_MAIL_BOX_BASE + (MS_U32)u8ParamNum* 2;
        HAL_AUDIO_WriteReg(u32MailReg, u16Data);
    }
}


///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO Initialize Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WriteInitTable()  @@Cathy
/// @brief \b Function \b Description:  This function is used to write initial register table(8-bit mode)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void  HAL_AUDIO_WriteInitTable(void)
{
    MS_U16 i = 0;

    while( !((AudioInitTbl[i].u32Addr == 0xFFFFFF) && (AudioInitTbl[i].u8Mask == 0)) )
    {
        if((AudioInitTbl[i].u32Addr == 0xFFFFFF))
        {
            if(AudioInitTbl[i].u8Mask == 1)
            {
                if(AudioInitTbl[i].u8Value!=0)
                    AUDIO_DELAY1MS((MS_U32)(AudioInitTbl[i].u8Value));
            }
        }
        else
        {
            HAL_AUDIO_AbsWriteMaskByte(AudioInitTbl[i].u32Addr, AudioInitTbl[i].u8Mask, AudioInitTbl[i].u8Value);
        }

        i++;
    }

    HAL_AUDIO_AbsWriteMaskReg(0x112CEE, 0x1000, 0x1000); //ADC§Ö³t¥R¹q
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WritePreInitTable()
/// @brief \b Function \b Description:  This function is used to set power on DAC sequence before setting init table.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void HAL_AUDIO_WritePreInitTable(void)
{
    MS_U16 i = 0;

    while( !((AudioPreInitTbl_Clock[i].u32Addr == 0xFFFFFF) && (AudioPreInitTbl_Clock[i].u8Mask == 0)) )
    {
        if((AudioPreInitTbl_Clock[i].u32Addr == 0xFFFFFF))
        {
            if(AudioPreInitTbl_Clock[i].u8Mask == 1)
            {
                if(AudioPreInitTbl_Clock[i].u8Value!=0)
                    MsOS_DelayTask((MS_U32)(AudioPreInitTbl_Clock[i].u8Value));
            }
        }
        else
        {
            HAL_AUDIO_AbsWriteMaskByte(AudioPreInitTbl_Clock[i].u32Addr, AudioPreInitTbl_Clock[i].u8Mask, AudioPreInitTbl_Clock[i].u8Value);
        }
       i++;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WritePostInitTable()
/// @brief \b Function \b Description:  This function is used to enable earphone low power stage.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void HAL_AUDIO_EnaEarphone_LowPower_Stage(void)
{
    HAL_AUDIO_WriteMaskByte(0x2CEC, 0xFF, 0x00);  //wriu -w 0x112cec 0x1800
    HAL_AUDIO_WriteMaskByte(0x2CED, 0xFF, 0x18);
    HAL_AUDIO_WriteMaskByte(0x2CEE, 0xFF, 0x03);   //wriu -w 0x112cee 0x0303
    HAL_AUDIO_WriteMaskByte(0x2CEF, 0xEF, 0x03);
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WritePostInitTable()
/// @brief \b Function \b Description:  This function is used to enable earphone high driving stage.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void HAL_AUDIO_EnaEarphone_HighDriving_Stage(void)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetPowerOn()    @@Need_Modify
/// @brief \b Function \b Description: This routine is used to execute DSP power on/down setting.
/// @param <IN>        \b bPower_on    : TRUE --power on
///                                      FALSE--power off
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetPowerOn(MS_BOOL bPower_on)
{
    DBG_AUDIO("HAL_AUDIO_SetPowerOn(%x)\r\n", bPower_on);
    if(bPower_on)
    {
       HAL_AUDIO_WriteMaskByte(REG_D2M_MAILBOX_SE_POWERCTRL, 0x02, 0x00);       // DSP power up command, DO NOT touch bit3
       HAL_AUDIO_WriteMaskByte(0x2B42, 0xFF, 0x00);      // Disable BB by-pass
    }
    else
    {
        HAL_AUDIO_DeInitialVars();

        HAL_AUDIO_WriteMaskByte(REG_D2M_MAILBOX_SE_POWERCTRL, 0x02, 0x02);       // DSP power down command
        AUDIO_DELAY1MS(100);

        // Disable MIU Request for DEC-DSP
        HAL_MAD_Dis_MIUREQ();

        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x80, 0x00);      // SPDIF power down
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ADCInit()    @@Cathy
/// @brief \b Function \b Description:  This routine is ADC relational register Init.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_ADCInit(void)
{
    HAL_AUDIO_WriteMaskByte(0x2CDA, 0x03, 0x00);    // power on ADC0 & ADC1
    HAL_AUDIO_WriteMaskByte(0x2CEE, 0x03, 0x03);    //enable ADC dither
    HAL_AUDIO_WriteMaskByte(0x2CE3, 0x03, 0x00);    // power on ADC PGA
    HAL_AUDIO_WriteMaskByte(0x2CE5, 0xFC, 0x00);    //ADC0 & ADC1 PGAain=0dB
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SwResetMAD()  @@Cathy
/// @brief \b Function \b Description:  This function is used to software reset MAD
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SwResetMAD(void)
{
    MS_U16 j = 0;

    while( !((ResetDMIOTbl[j].u32Addr == 0xFFFFFF) && (ResetDMIOTbl[j].u16Mask == 0)) )
    {
        if((ResetDMIOTbl[j].u32Addr == 0xFFFFFF))
        {
            if(ResetDMIOTbl[j].u16Mask == 1)
            {
                if(ResetDMIOTbl[j].u16Value!=0)
                    AUDIO_DELAY1MS((MS_U32)(ResetDMIOTbl[j].u16Value));
            }
        }
        else
        {
            HAL_AUDIO_AbsWriteMaskReg(ResetDMIOTbl[j].u32Addr, ResetDMIOTbl[j].u16Mask, ResetDMIOTbl[j].u16Value);
        }

        j++;
    }

    /* Audio software engine reset */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SOFT_RESET, 0x0F, 0x0D);
    AUDIO_DELAY1MS(1);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SOFT_RESET, 0x0F, 0x0F);
    AUDIO_DELAY1MS(1);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SOFT_RESET, 0x0F, 0x0D);
    AUDIO_DELAY1MS(1);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SOFT_RESET, 0x0F, 0x00);
    AUDIO_DELAY1MS(1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ResetDSP()  @@Cathy
/// @brief \b Function \b Description:  This function is used to reset DSP.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_ResetDSP(void)
{
    HAL_AUDIO_DecWriteByte(REG_DEC_IDMA_CTRL0, 0x02);     // Reset DSP
    AUDIO_DELAY1MS(2);
    HAL_AUDIO_DecWriteByte(REG_DEC_IDMA_CTRL0, 0x03);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DecDspISR()   @@Need_Modify
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DecDspISR(void) //Cathy need modify
{
    MS_U8 dec_ISR_cmdType;

    dec_ISR_cmdType = HAL_AUDIO_ReadByte(REG_D2M_MAILBOX_DEC_ISRCMD);

    HALAUDIO_CHECK_SHM_INIT;

    if(dec_ISR_cmdType == 0x3)
    {
        g_AudioVars2->g_bDecPlayFileFlag = TRUE;
    }
    else if(dec_ISR_cmdType == 0x13)                   // MPEG encode ISR
    {
        HAL_AUDIO_SetEncodeDoneFlag(1);         // for PVR encode done, kochien modified
        EncBuf_Count++;                        // remaining data in buffer
        EncFrameIdx += 16;                   //default 16 frames / interrupt
        MPEG_EN_BUF[EncBuf_W_idx].u32Addr = ((MS_U32)HAL_AUDIO_ReadReg(REG_DEC_D2M_MAIL_BOX_ENC_LINEADDR))*16;
        MPEG_EN_BUF[EncBuf_W_idx].u32Size= ((MS_U32)HAL_AUDIO_ReadReg(REG_DEC_D2M_MAIL_BOX_ENC_LINESIZE))*16;
        MPEG_EN_BUF[EncBuf_W_idx].u64Idx= (MS_U64)EncFrameIdx;      //(MS_U64)(((EncFrameIdx-1)*1152*90)/48);

        if(EncBuf_W_idx == 5)// (ENC_BUF_SIZE-1))     // circular buffer  // manually circular...
            EncBuf_W_idx = 0;
        else
            EncBuf_W_idx++;
    }
    else if (dec_ISR_cmdType == 0x07)  // 2nd Decoder ISR in MM
    {
        g_AudioVars2->g_bSePlayFileFlag = TRUE;
    }
}

// [LM14]
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SeDspISR()  @@Need_Modify
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 preIdx = 0;
MS_U8 u8Index = 0; //current Idx
MS_U32 preSysTime = 0;
MS_U32 u32SysTime = 0; //current system time

void HAL_AUDIO_SeDspISR(void)
{
    MS_U8 se_ISR_cmdType;

    se_ISR_cmdType = HAL_AUDIO_ReadByte(REG_D2M_MAILBOX_SE_ISRCMD);

    /* add for PIP ASND Decode */
    if ( se_ISR_cmdType == 0x03 )
    {
        if (g_AudioVars2 != NULL)
        {
            g_AudioVars2->g_bSePlayFileFlag = TRUE;
        }
    }
    #if 0
    else if( se_ISR_cmdType == (MS_U8)(SE_ISR_CMDTYPE_PCM_CAPTURE>>8) ) //capture
        {
            //OS_OBTAIN_MUTEX(_s32AudioMutexPCM_capture_callback, MSOS_WAIT_FOREVER);
            if(PCM_capture_callback != NULL)
            {
                preIdx = u8Index; //record preIdx
                preSysTime = u32SysTime; //record preSysTime
                MS_U16 tmpidx = (MS_U16)HAL_MAD2_Read_DSP_sram(DSP2DmAddr_capture_idx, DSP_MEM_TYPE_DM);
                switch(tmpidx)
                {
                    default:
                    case 0x0:
                        u8Index = 0;
                      break;
                    case 0x1:
                        u8Index = 1;
                      break;
                    case 0x2:
                        u8Index = 2;
                      break;
                    case 0x3:
                        u8Index = 3;
                      break;
                    case 0x4:
                        u8Index = 4;
                      break;
                    case 0x5:
                        u8Index = 5;
                      break;
                    case 0x6:
                        u8Index = 6;
                      break;
                    case 0x7:
                        u8Index = 7;
                      break;
                }
                #if 0 //use for BT sound debug (check interrupt loss)
                u32SysTime = MsOS_GetSystemTime();
                if( (u8Index - preIdx) >= 2 )
                {
                    printf("\033[1;36m [System Time (%08ld)] [Pre System time (%08ld)] [TD = %ld ms]  ==Capture addr(0x%lx) , index(%d)==, preIdx(%d) \033[0m \r\n", u32SysTime, preSysTime, u32SysTime - preSysTime, PCMCaptureBaseAddr+u8Index*PCM_CAPTURE_STEP_SIZE, u8Index, preIdx);
                    printf("\033[1;31m [AUDIO][%s] [%s] [%d] [loss idx!!!] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);
                }
                #endif
                (*PCM_capture_callback)((MS_U8 *)MsOS_PA2KSEG1((MS_U32)(PCMCaptureBaseAddr+u8Index*PCM_CAPTURE_STEP_SIZE)), PCM_CAPTURE_STEP_SIZE);
            }
            //OS_RELEASE_MUTEX(_s32AudioMutexPCM_capture_callback);
        }
    #endif
}

// [LM14]
void HAL_AUDIO_R2ISR_0(void)
{
    if(Audio_Encoder_callback != NULL)
    {
        MS_U8 u8Index = 0;
        u8Index = HAL_AUDIO_ReadByte(REG_MPEG_ENCODER_BUF_IDX);

        if (g_AudioVars2->bMP3EncodeDebug == TRUE)
        {
            if (u8Index == 0)
            {
                if (u8LastMP3EncodeIndex != 3)
                {
                    HALAUDIO_ERROR("=== Interrupt Loss 1.===\r\n");
                    HALAUDIO_ERROR("=== u8Index = %d , u8LastIndex = %d===\r\n",u8Index, u8LastMP3EncodeIndex);
                }
            }
            else
            {
                if (u8Index - u8LastMP3EncodeIndex != 1)
                {
                    HALAUDIO_ERROR("=== Interrupt Loss 2.===\r\n");
                    HALAUDIO_ERROR("=== u8Index = %d , u8LastIndex = %d===\r\n",u8Index, u8LastMP3EncodeIndex);
                }
            }
        }

        LGE_AENC_MSG_TYPE_T AuFrameInfo;
        AuFrameInfo.channel = 2;
        AuFrameInfo.dataLen = HAL_MAD_GetEncInfo(AUDIO_ENC_DATA_LEN);
        //AuFrameInfo.pData = (MS_U32 *)MsOS_PA2KSEG1(HAL_MAD_GetEncInfo(AUDIO_ENC_BUFFER_START_ADDR) + (u8Index * R2_MPEG_ENC_INTR_SIZE));
        AuFrameInfo.pRStart = (MS_U32 *)MsOS_PA2KSEG1(HAL_MAD_GetEncInfo(AUDIO_ENC_BUFFER_START_ADDR));
        AuFrameInfo.pREnd = (MS_U32 *)MsOS_PA2KSEG1(HAL_MAD_GetEncInfo(AUDIO_ENC_BUFFER_END_ADDR));
        AuFrameInfo.pts = MsOS_GetSystemTime();

        u8LastMP3EncodeIndex = u8Index;
        (*Audio_Encoder_callback)(&AuFrameInfo);
    }
}

void HAL_AUDIO_R2ISR_1(void)
{
    MS_U8 r2_ISR_cmdType_1 = 0;
    r2_ISR_cmdType_1 = 0;
    UNUSED(r2_ISR_cmdType_1);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetPlayFileFlag()  @Cathy
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetPlayFileFlag(MS_BOOL bDspType, MS_BOOL bSet)
{
    HALAUDIO_CHECK_SHM_INIT;

    if(bDspType == DSP_DEC)
    {
        g_AudioVars2->g_bDecPlayFileFlag = bSet;
    }
    else
    {
        g_AudioVars2->g_bSePlayFileFlag = bSet;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetPlayFileFlag()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GetPlayFileFlag(MS_BOOL bDspType)
{
        MS_U32      es_rdPtr, es_wrPtr;
        MS_S32      es_level, es_bufSz, es_freeSpace;
        MS_U32      es_reqSize;
        MS_U8       r2_dec_id;

        if(bDspType ==DSP_DEC)
            r2_dec_id = ADEC1;
        else if(bDspType ==DSP_SE)
            r2_dec_id = ADEC2;
        else
            r2_dec_id = ADEC1; //default case

        es_rdPtr = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_ES_RD_PTR, r2_dec_id );
        es_wrPtr = HAL_DEC_R2_Get_SHM_PARAM( R2_SHM_PARAM_ES_WR_PTR, r2_dec_id, 0 );
        es_reqSize = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, r2_dec_id);

        es_bufSz = ES1_DRAM_SIZE;
        es_level = es_wrPtr - es_rdPtr;

        if (es_level < 0)
            es_level += es_bufSz;

        es_freeSpace = es_bufSz - es_level;

        if ( es_freeSpace > es_reqSize )
            return TRUE;
        else
            return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetEncodeDoneFlag()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetEncodeDoneFlag(MS_BOOL bSet)
{
    if(bSet)
       g_bEncodeDoneFlag= 1;
    else
       g_bEncodeDoneFlag = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetEncodeDoneFlag()  @@Cathy
/// @brief \b Function \b Description:  This function is used to get the Encoder flag status
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GetEncodeDoneFlag(void)
{
    return g_bEncodeDoneFlag;
}

/////////////////////////////////////////////////////////////////////////////////
///                                                                                                                                   ///
///        AUDIO I/O Config Relational Hal Function                                                               ///
///                                                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetPathGroup()
/// @brief \b Function \b Description: This routine is used to get Audio path group type .
/// @param output      \b : Audio output path-group type
////////////////////////////////////////////////////////////////////////////////
AUDIO_PATH_GROUP_TYPE HAL_AUDIO_GetPathGroup(void)
{
    // AUDIO_PATH_GROUP_1 : U3, Janus
    // AUDIO_PATH_GROUP_2 : T3, U4, T8, T9, T12, T13, J2
    return(AUDIO_PATH_GROUP_2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetNormalPath()  @@ No used in T3 @@VVV
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Input and Output.
/// @param <IN>        \b u8Path    : Audio DSP channel
/// @param <IN>        \b input    : Audio input type
/// @param <IN>        \b output    : Audio output type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetNormalPath(AUDIO_PATH_TYPE u8Path, AUDIO_INPUT_TYPE input, AUDIO_OUTPUT_TYPE output)
{
    u8Path=u8Path;
    input=input;
    output=output;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetInputPath()   @@VVV
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Input .
/// @param <IN>        \b input    : Audio input type
/// @param <IN>        \b u8Path    : Audio DSP channel
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetInputPath(AUDIO_INPUT_TYPE input , AUDIO_PATH_TYPE u8Path)
{
    MS_U32 u32path_reg;
    MS_U8  u8input_src, u8input_idx, u8temp, path;
    AUDIO_INPUT_TYPE     input1; // define this in order to do more check for ATV input source.
    input1 = input;

    HALAUDIO_CHECK_SHM_INIT;

    if((input == AUDIO_NULL_INPUT) || (u8Path ==AUDIO_PATH_NULL))
    {
        return;
    }

    u8input_src = LONIBBLE(input);
    u8input_idx = HINIBBLE(input);

    if (u8input_idx == 7)   // if input source is ATV, change input depends on the definition of SIF_DSP_TYPE instead of the ATV input mux at boarddef
    {
        input1 =    AUDIO_DSP4_SIF_INPUT;
        u8input_src = LONIBBLE(input1);
    }

#if 0 //[M2] no need to use DC Remove function
    if( u8Path == AUDIO_PATH_MAIN )
    {
        if (u8input_src == AUDIO_ADC_INPUT || u8input_src == AUDIO_ADC2_INPUT)
        {
            HAL_SOUND_EnableDcRemove(TRUE);
        }
        else
        {
            HAL_SOUND_EnableDcRemove(FALSE);
        }
    }
#endif

    path=(MS_U8)u8Path;

    /* save main speaker audio input */
    if( u8Path == AUDIO_PATH_MAIN )
    {
        g_AudioVars2->eMainAudioSource = input1;
    }
    /* save sub channel audio input */
    else if( u8Path == AUDIO_PATH_6 )
    {
        g_AudioVars2->eSubAudioSource = input1;
    }

    if( path >= (sizeof(u32PathArray)/sizeof(u32PathArray[0])))
    {
            DBG_AUDIO("DSP is not support more than CH8\n");
            return;
    }

    u32path_reg = u32PathArray[path];

#if 0 //Path Debug
    if ((u32path_reg == 0x2C65) || (u32path_reg == 0x2C67))
    {

        switch(u32path_reg)
        {
            case 0x2C65:
            {
                printf("\033[1;36m [AUDIO][%s] [%s] [%d] [0x2C65] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);/*@audio*/
            }
            break;

            case 0x2C67:
            {
                printf("\033[1;36m [AUDIO][%s] [%s] [%d] [0x2C67] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);/*@audio*/
            }
            break;

            default:
                break;
        }

        switch(u8input_src)
        {
            case AUDIO_DSP1_INPUT                   :///< 0: DSP Decoder1 Input
            {
                printf("\033[1;36m [AUDIO][%s] [%s] [%d] [AUDIO_DSP1_INPUT] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);/*@audio*/
            }
            break;
            case AUDIO_DSP2_INPUT                   :///< 1: DSP Decoder2 Input
            {
                printf("\033[1;36m [AUDIO][%s] [%s] [%d] [AUDIO_DSP2_INPUT] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);/*@audio*/
            }
            break;
            case AUDIO_ADC_INPUT                    :///< 2: ADC Input
            {
                printf("\033[1;36m [AUDIO][%s] [%s] [%d] [AUDIO_ADC_INPUT] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);/*@audio*/
            }
            break;
            case AUDIO_DSP3_INPUT                   :// = 0x06,     ///< 6: DSP Decoder3 Input
            {
                printf("\033[1;36m [AUDIO][%s] [%s] [%d] [AUDIO_DSP3_INPUT] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);/*@audio*/
            }
            break;
            case AUDIO_DSP4_INPUT                   :// = 0x08,     ///< 8: DSP Decoder4 Input
            {
                printf("\033[1;36m [AUDIO][%s] [%s] [%d] [AUDIO_DSP4_INPUT] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);/*@audio*/
            }
            break;
            case AUDIO_ADC2_INPUT                   :// = 0x09,     ///< 9: ADC2 Input
            {
                printf("\033[1;36m [AUDIO][%s] [%s] [%d] [AUDIO_ADC2_INPUT] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);/*@audio*/
            }
            break;
            case AUDIO_DMA_INPUT                    :// = 0x0E,     ///< E: DMA Input
            {
                printf("\033[1;36m [AUDIO][%s] [%s] [%d] [AUDIO_DMA_INPUT] \033[0m \n", __FILE__, __FUNCTION__, __LINE__);/*@audio*/
            }
            break;
            default:
                break;
        }
    }
#endif

    // Set input
    switch(u8input_src)
    {
        case AUDIO_DSP1_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x80);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG, 0x07,u8input_idx);  // Set main parser source
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER2_CFG, 0x07,u8input_idx);  // Set AD parser source
         break;

        case AUDIO_DSP2_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x81);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER2_CFG, 0x07,u8input_idx);
            break;

        case AUDIO_DSP3_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x82);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG, 0x07,u8input_idx);
            break;

        case AUDIO_DSP4_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x83);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER4_CFG, 0x07,u8input_idx);
            break;

        case AUDIO_ADC_INPUT:
            if(u8input_idx==0x0A)
                u8temp=0x40;
            else if(u8input_idx==0x0B)
                u8temp=0x50;
            else
                u8temp = (u8input_idx<<4);
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x1F, 0x08);
            HAL_AUDIO_WriteMaskByte(0x2CE2, 0xF0, u8temp );
            HAL_AUDIO_WriteMaskByte(0x2CE2, 0x0F, u8temp>>4 );
            break;

        case AUDIO_ADC2_INPUT:
            if(u8input_idx==0x0A)
                u8temp=0x04;
            else if(u8input_idx==0x0B)
                u8temp=0x05;
            else
                u8temp = u8input_idx;
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x1F, 0x09);
            HAL_AUDIO_WriteMaskByte(0x2CE2, 0xF0, u8temp<<4);
            HAL_AUDIO_WriteMaskByte(0x2CE2, 0x0F, u8temp );
            break;

        case AUDIO_SPDIF_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x86);
            break;

        case AUDIO_I2S_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x85);
            break;

        case AUDIO_HDMI_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x84);
            break;

        case AUDIO_DMA_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x1F, 0x1F);
            break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetOutputPath()    @@Need_Modify
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Output.
/// @param <IN>        \b u8Path    : Audio DSP channel
/// @param <IN>        \b output    : Audio output type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetOutputPath(AUDIO_PATH_TYPE u8Path,  AUDIO_OUTPUT_TYPE u8Output)
{
    u8Path=u8Path;
    u8Output=u8Output;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetInternalPath()    @@VVV
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Output.
/// @param <IN>        \b u8Path    : Audio internal path
/// @param <IN>        \b output    : Audio output type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetInternalPath(AUDIO_INTERNAL_PATH_TYPE u8Path,  AUDIO_OUTPUT_TYPE u8Output)
{
    MS_U8   path;

    if(u8Path >= INTERNAL_MAX_NUM)
        return;

    path = (MS_U8)u8Path;

    if(u8Path==INTERNAL_PCM_SE)  // Only speaker use this path
    {
        HAL_SOUND_SetBalanceMask(u8Output, TRUE);
    }

    // Set output
      switch(u8Output)
    {
        case AUDIO_AUOUT0_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL1, 0x0F, path);
            break;

        case AUDIO_AUOUT1_OUTPUT:
        case AUDIO_HP_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL1, 0xF0, (path<<4));
            break;

        case AUDIO_AUOUT2_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL1+1, 0x0F, path);
            break;

        case AUDIO_AUOUT3_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL1+1, 0xF0, (path<<4));
            break;

        case AUDIO_I2S_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL2, 0x0F, path);
            break;

        case AUDIO_SPDIF_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL3+1, 0x0F, path); // Fix PCM in
            break;

        case AUDIO_HDMI_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL3, 0x0F, path);
            break;

        case AUDIO_I2S2_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL2, 0xF0, (path<<4));
            break;

        default:
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_FwTriggerDSP()    @@Need_Modify
/// @brief \b Function \b Description:  This routine send a PIO11 interrupt to DSP with a command on 0x2D34.
/// @param <IN>        \b cmd    :    0xF0-- for MHEG5 file protocol
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_FwTriggerDSP(MS_U16 u16Cmd)  //Cathy need modify
{
    u16Cmd = u16Cmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DspBootOnDDR()
/// @brief \b Function \b Description:  Enable DSP load / reload code from DDR
/// @param <IN>        \b bEnable    : 0 -- load code from FLASH
///                                      1 -- load code from DDR
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DspBootOnDDR(MS_BOOL bEnable)
{
    g_bAudio_loadcode_from_dram = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetIsDtvFlag()  @@Cathy
/// @brief \b Function \b Description:  Report the decoder type is ATV or DTV
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b g_bIsDTV: 0 -> ATV , 1 -> DTV
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GetIsDtvFlag(void)
{
    return g_bIsDTV;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetIsDtvFlag()  @@Cathy
/// @brief \b Function \b Description:  Report the decoder type is ATV or DTV
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b g_bIsDTV: 0 -> ATV , 1 -> DTV
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetIsDtvFlag(MS_BOOL bIsDTV)
{
    g_bIsDTV=bIsDTV;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Dec_Status()  @@Cathy
/// @brief \b Function \b Description:  This routine is used to read the Decoder status.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : Decoder Status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_Dec_Status(void)
{
    return(HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Enc_Status()  @@Kochien.Kuo
/// @brief \b Function \b Description:  This routine is used to read the Encoder status.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : Encoder Status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_Enc_Status(void)
{
    return(HAL_AUDIO_ReadByte(REG_DEC_ENCODE_CMD));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SE_Status()  @@Cathy
/// @brief \b Function \b Description:  This routine is used to read the SE status.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : Decoder Status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_Se_Status(void)
{
    return(HAL_AUR2_ReadByte(REG_SE_DECODE_CMD));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Set_Fading()  @@Need_Modify
/// @brief \b Function \b Description:  This routine is used to set the Fading response time
/// @param <IN>        \b u32VolFading    : Fading response time parameter
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_Set_Fading(MS_U32 u32VolFading)
{
     HAL_MAD2_Write_DSP_sram(0x114C, u32VolFading, DSP_MEM_TYPE_PM);        //need touch
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_BT_SetUploadRate()   @@Need_Modify
/// @brief \b Function \b Description:  According Blue tooth upload path, for different sampling rate setting the Synthesizer.
/// @param <IN>        \b bEnable    : 0 -- Disable Blue Tooth upload
///                                      1 -- Enable Blue Tooth upload
/// @param <IN>        \b u8Samprate: Sampling Rate
///                                    0--no change
///                                    1--48KHz
///                                    2--44KHz
///                                    3--32KHz
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_BT_SetUploadRate(MS_BOOL bEnable,MS_U8 u8Samprate)
{
    u8Samprate &= 0x0003;

    if(bEnable)
    {
       HAL_AUDIO_DecWriteByte(0x2C5A, 0x55);
       HAL_AUDIO_DecWriteMaskByte(0x2D6C,0x10,0x10);
       HAL_AUDIO_DecWriteMaskByte(0x2D34,0x03,u8Samprate);
    }
    else
    {
       HAL_AUDIO_DecWriteByte(0x2C5A, 0);
       HAL_AUDIO_DecWriteMaskByte(0x2D6C,0x10,0);
       HAL_AUDIO_DecWriteMaskByte(0x2D34,0x03,0);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_BT_SetBufferCounter()  @@Need_Modify
/// @brief \b Function \b Description:  Set the DDR buffer according the sampling rate and the frame time
/// @param <IN>        \b u32Counter    : if the sampling rate is 48KHz, the frame time is 40ms
///                            ==> the frame buffer size is 48000*0x04*2 (L/R) *2(Bytes/sample) = 0x1E00
///                                 the counter is 0x1E00/8 = 960 (For 1*Burst DMA)
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_BT_SetBufferCounter(MS_U32 u32Counter)
{
    u32Counter &= 0x00FFFFFF;
    HAL_MAD2_Write_DSP_sram(DSP2PmAddr_btFrameSize, u32Counter, DSP_MEM_TYPE_PM);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_USBPCM_Enable()  @@Need_Modify
/// @brief \b Function \b Description:  Enable/ Disable the path of USB PCM
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_USBPCM_Enable(MS_BOOL bEnable)
{
    //this funcion is removed from DSP
    UNUSED(bEnable);
    HALAUDIO_ERROR("%s() : Error! unsupported funcion !!!\n", __FUNCTION__);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_USBPCM_GetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  Get interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b BOOL    :   interrupt flag
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_USBPCM_GetFlag()
{
    //this funcion is removed from DSP
    HALAUDIO_ERROR("%s() : Error! unsupported funcion !!!\n", __FUNCTION__);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_USBPCM_SetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  clear interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b bEnable :  set false to clean interrupt flag
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void    HAL_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable)
{
    //this funcion is removed from DSP
    UNUSED(bEnable);
    HALAUDIO_ERROR("%s() : Error! unsupported funcion !!!\n", __FUNCTION__);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_USBPCM_GetMemInfo()  @@Need_Modify
/// @brief \b Function \b Description:  Get memory address and size for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : address, size
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo)
{
    //this funcion is removed from DSP
    HALAUDIO_ERROR("%s() : Error! unsupported funcion !!!\n", __FUNCTION__);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ I2S_SetMode()    @@Need_Modify
/// @brief \b Function \b Description:  This routine is used to Set the I2S output mode.
/// @param <IN>        \b Mode    : MCLK , WORD_WIDTH , SOURCE_CH , FORMAT
/// @param <IN>        \b Value    : Please reference the register table 0x2C8C & 0x2C8D .
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_I2S_SetMode(MS_U8 u8Mode, MS_U8 u8Val)
{
   switch(u8Mode)
   {
     case AUDIO_I2S_MCLK:         //0x2C8C[6:4]
       HAL_AUDIO_WriteMaskByte(REG_AUDIO_I2S_OUT1_CFG,0x70,u8Val<<4);
       break;

     case AUDIO_I2S_WORD_WIDTH:   //0x2C8C[2:0]
       HAL_AUDIO_WriteMaskByte(REG_AUDIO_I2S_OUT1_CFG,0x07,u8Val);
       break;

     case AUDIO_I2S_FORMAT:      //0x2C8C[3]
       HAL_AUDIO_WriteMaskByte(REG_AUDIO_I2S_OUT1_CFG,0x08,u8Val<<3);
       break;

     case AUDIO_I2S_SOURCE_CH:
      // No need ; Only select Group A in T3 .
       break;

        case AUDIO_I2S_RXMODE:
            if(u8Val==I2S_SLAVE_MODE)      //slave mode
            {   //0x2C8C[2], 2CAE[13], 2CCE[15:13][1:0]
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, 0x04, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_CLK_CFG5+1, 0xA0, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_ASIF_TST+1, 0xE0, 0x20);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_ASIF_TST, 0x03, 0x00);
            }
            else      //master mode
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, 0x04, 0x04);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_CLK_CFG5+1, 0xA0, 0xA0);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_ASIF_TST+1, 0xE0, 0xC0);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_ASIF_TST, 0x03, 0x03);
            }
            break;

        case AUDIO_I2S_TXMODE:
            // Tx always Master Mode;
            break;

     default:
       break;
   }
}



void HAL_AUDIO_DigitalOut_Set_Encode(AUDIO_ENCODE_TYPE eType, MS_BOOL bEnable)
{
    switch(eType)
    {
        case DD_DDCO:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(0x2E95, 0x01, 0x1 );
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(0x2E95, 0x01, 0x0 );
            }
            break;

        case DD_DDENCODE:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(0x2E95, 0x02, 0x2 );
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(0x2E95, 0x02, 0x0 );
            }
            break;

        case MP3_ENCODE:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_MP3_ENCODE_CTRL, 0x01, 0x1 );
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_MP3_ENCODE_CTRL, 0x01, 0x0 );
            }
            break;

        case DTS_ENCODE_2CH:
        case DTS_ENCODE_MULTI:
        default:
            HALAUDIO_ERROR("%s() - Choosen Encoder not exist\n",__FUNCTION__);
            break;
    }
}

static void HAL_AUDIO_DigitalOut_NPCM_SrcSwitch(DIGITAL_OUTPUT_TYPE ePath, AUDIO_DSP_ID eDspID)
{
    switch (ePath)
    {
        case DIGITAL_SPDIF_OUTPUT:
            {
                switch(eDspID)
                {
                    case AUDIO_DSP_ID_R2:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0xC, 0x4);
                        break;
                    case AUDIO_DSP_ID_SND:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0xC, 0x8);
                        break;
                    case AUDIO_DSP_ID_DEC:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0xC, 0x0);
                        break;
                    default:
                        HALAUDIO_ERROR("%s() - DEC ID %d not exist\n",__FUNCTION__,eDspID);
                        break;
                }
            }
            break;

        case DIGITAL_HDMI_ARC_OUTPUT:
            {
                switch(eDspID)
                {
                    case AUDIO_DSP_ID_R2:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x4);
                        break;
                    case AUDIO_DSP_ID_SND:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x8);
                        break;
                    case AUDIO_DSP_ID_DEC:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x0);
                        break;
                    default:
                        HALAUDIO_ERROR("%s() - DEC ID %d not exist\n",__FUNCTION__,eDspID);
                        break;
                }
            }
            break;

        case DIGITAL_HDMI_OUTPUT:
            {
                switch(eDspID)
                {
                    case AUDIO_DSP_ID_R2:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x4);
                        break;
                    case AUDIO_DSP_ID_SND:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x8);
                        break;
                    case AUDIO_DSP_ID_DEC:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x0);
                        break;
                    default:
                        HALAUDIO_ERROR("%s() - DEC ID %d not exist\n",__FUNCTION__,eDspID);
                        break;
                }
            }
            break;

        default:
            HALAUDIO_ERROR("%s() - Digital Path %d not exist\n",__FUNCTION__,ePath);
            break;
    }

}

static void HAL_AUDIO_DigitalOut_NPCMEnable(DIGITAL_OUTPUT_TYPE ePath, MS_BOOL bEnable)
{

    switch(ePath)
    {
        case DIGITAL_SPDIF_OUTPUT:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x07, 0x02); /* Grp C in */
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x01, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x2, 0x2);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x0C, 0x00); /* Select DSP1 */
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x2, 0x0); /* PCM Mode */
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x07, 0x00); /* Tx1 Grp A in */
            }
            break;

        case DIGITAL_HDMI_ARC_OUTPUT:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x02); /* Grp C in */
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x01, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x2, 0x2);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x0C, 0x00); /* Select DSP1 */
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x2, 0x0);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x00); /* Tx2 Grp A in */
            }
            break;

        case DIGITAL_HDMI_OUTPUT:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x02); /* Grp C in */
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x01, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x2, 0x2);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x0C, 0x00); /* Select DSP1 */
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x2, 0x0);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x00); /* Tx2 Grp A in */
            }
            break;

        default:
            HALAUDIO_ERROR("%s() - Digital Path %d not exist\n",__FUNCTION__,ePath);
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF output mode
/// @param <IN>        \b u8Spdif_mode    :
///                                    bit[0] = 0: spdif enable,   1: spdif disable (Se-DSP)
///                                    bit[1] = 0: PCM mode,     1: non-PCM mode
///                                    bit[2] = 1: non-PCM NULL Payload
/// @param <IN>        \b u8Input_src  0 : DTV
///                                    1 : ATV
///                                    2 : HDMI
///                                    3 : ADC
///                                    4 : CardReader
///                                    5 : SPDIF
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
static void HAL_AUDIO_DigitalOut_SetDataPath(DIGITAL_OUTPUT_TYPE ePath, DigitalOutSetting_t *stDigitalOutSetting)
{
    MS_BOOL ret;

    Digital_Out_Channel_Status_t stDigitalChannelStatus;
    memset(&stDigitalChannelStatus, 0x00, sizeof(Digital_Out_Channel_Status_t));

    if (ePath == DIGITAL_SPDIF_OUTPUT)
    {
        _HAL_AUDIO_SPDIF_SetMute(TRUE);
    }
    else if (ePath == DIGITAL_HDMI_ARC_OUTPUT)
    {
        //TODO: Mute HDMI , ARC
    }
    else if (ePath == DIGITAL_HDMI_OUTPUT)
    {

    }

    ret = HAL_AUDIO_DigitalOut_GetChannelStatus(ePath, &stDigitalChannelStatus);

    if (ret == FALSE)
    {
        HALAUDIO_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
        return;
    }

    switch (stDigitalOutSetting->eDigitalOutfMode)
    {
        case DIGITAL_OUT_PCM:
            HAL_AUDIO_DigitalOut_NPCMEnable(ePath, FALSE);
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, FALSE);
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, FALSE);

            stDigitalChannelStatus.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;
            break;

        case DIGITAL_OUT_DOLBYDIGITAL:
            if (stDigitalOutSetting->u8R2NonPcmSetting == 0x01)
            {
                HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, FALSE);
                HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, TRUE);
            }
            else if (stDigitalOutSetting->u8R2NonPcmSetting == 0x02)
            {
                HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, FALSE);
                HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, TRUE);
            }

            stDigitalChannelStatus.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            HAL_AUDIO_DigitalOut_NPCM_SrcSwitch(ePath, stDigitalOutSetting->u8NonPcmPath);
            HAL_AUDIO_DigitalOut_NPCMEnable(ePath, TRUE);
            break;

        case DIGITAL_OUT_DTS:
            stDigitalChannelStatus.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, FALSE);
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, FALSE);
            HAL_AUDIO_DigitalOut_NPCM_SrcSwitch(ePath, stDigitalOutSetting->u8NonPcmPath);
            HAL_AUDIO_DigitalOut_NPCMEnable(ePath, TRUE);
            break;

        case DIGITAL_OUT_AAC_LC:
            stDigitalChannelStatus.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, FALSE);
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, FALSE);
            HAL_AUDIO_DigitalOut_NPCM_SrcSwitch(ePath, stDigitalOutSetting->u8NonPcmPath);
            HAL_AUDIO_DigitalOut_NPCMEnable(ePath, TRUE);
            break;

        case DIGITAL_OUT_NONE:
        default:
            HALAUDIO_ERROR("%s() - Digital Mode %d not exist\n",__FUNCTION__,stDigitalOutSetting->eDigitalOutfMode);
            break;
    }

    HAL_AUDIO_DigitalOut_SetChannelStatus(ePath, &stDigitalChannelStatus);
    // Restore Digital out mode
    memcpy(&(g_AudioVars2->stDigitalOutChannelStatus[ePath]), &stDigitalOutSetting, sizeof(stDigitalOutSetting));

    if (ePath == DIGITAL_SPDIF_OUTPUT)
    {
        if (g_AudioVars2->g_SPDIF_MuteStatus == FALSE)
        {
            _HAL_AUDIO_SPDIF_SetMute(FALSE);
        }
    }
    else
    {
        //TODO: um-Mute HDMI , ARC
    }

}



//static MS_BOOL HAL_AUDIO_DigitalOut_GetDataSource(AUDIO_OUTPORT_SOURCE_TYPE ePath, AUDIO_DIGITAL_OUTPUT_TYPE *retOutputType, AUDIO_DSP_ID *retNcmPath)

static MS_BOOL HAL_AUDIO_DigitalOut_DetermineDataPath(DIGITAL_OUTPUT_TYPE ePath, DigitalOutSetting_t *stDigitalOutSegtting)
{
    MS_U8 u8SourceDspCodeType = MSAPI_AUD_DVB_INVALID;
    MS_U8 u8ADEC1CodeType = MSAPI_AUD_DVB_INVALID;
    MS_U8 u8ADEC2CodeType = MSAPI_AUD_DVB_INVALID;

    stDigitalOutSegtting->eSourceType = g_AudioVars2->eAudioSource;
    u8ADEC1CodeType = g_AudioVars2->g_u8DecR2Adec1DecType;
    u8ADEC2CodeType = g_AudioVars2->g_u8DecR2Adec2DecType;

    HALAUDIO_PRINT("%s() - eDigitalOutfMode  = %x, eNonPcmPath = %x , Source = %x, R2NonPcmSetting = %x\n\r", \
    __FUNCTION__, \
    stDigitalOutSegtting->eDigitalOutfMode, \
    stDigitalOutSegtting->u8NonPcmPath, \
    stDigitalOutSegtting->eSourceType, \
    stDigitalOutSegtting->u8R2NonPcmSetting);

    switch (stDigitalOutSegtting->eDigitalOutfMode)
    {
        case DIGITAL_OUT_PCM:
        {
            stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC;
            break;
        }

        case DIGITAL_OUT_DOLBYDIGITAL:
        {
            switch (stDigitalOutSegtting->eSourceType)
            {
                case E_AUDIO_INFO_HDMI_IN:
                {
                    if (bIsNonPCMInDec2)
                    {
                        if ((HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE)) &&
                            (u8ADEC2CodeType == AU_DVB_STANDARD_AC3 || u8ADEC2CodeType == AU_DVB_STANDARD_AC3P))
                        {
                            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP2 */
                        }
                    }
                    else if ((u8ADEC1CodeType == AU_DVB_STANDARD_AC3) ||
                        (u8ADEC1CodeType == AU_DVB_STANDARD_AC3P))
                    {
                        stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    }
                    break;
                }

                case E_AUDIO_INFO_DTV_IN:
                case E_AUDIO_INFO_MM_IN:
                case E_AUDIO_INFO_GAME_IN:
                {
                    u8SourceDspCodeType = u8ADEC1CodeType;

                    switch(u8SourceDspCodeType)
                    {
                        case AU_DVB_STANDARD_AAC:
                        case AU_DVB_STANDARD_MS10_DDT:
                        {
#ifdef NoIPCheck
                            if ((!MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_MS10_DDT)) &&
                                 (!MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DDE)))
#endif
                            {
                                stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                            }
#ifdef NoIPCheck
                            else
                            {
                                if ((HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1)&0xFFFF) < 32000)
                                {
                                    stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                                }
                                else
                                {
                                    stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_R2; /* R2 */
                                    stDigitalOutSegtting->u8R2NonPcmSetting = (u8SourceDspCodeType == AU_DVB_STANDARD_AAC) ? 0x01 : 0x02; /* DDCO : 0x2E95[0], DDEnc : 0x2E95[1] */
                                }
                            }
#endif
                            break;
                        }

                        case AU_DVB_STANDARD_MS10_DDC:
                        case AU_DVB_STANDARD_AC3:
                        case AU_DVB_STANDARD_AC3P:
                        {
                            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                            break;
                        }

                        default:
                        {
                            stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                            break;
                        }

                    }

                    break;
                }

                case E_AUDIO_INFO_ATV_IN:
                case E_AUDIO_INFO_ADC_IN:
                case E_AUDIO_INFO_KTV_IN:
                default:
                {
                    stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                    stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    break;
                }
            }
            break;
        }

        case DIGITAL_OUT_DTS:
        {
            switch (stDigitalOutSegtting->eSourceType)
            {
                case E_AUDIO_INFO_HDMI_IN:
                {
                    if (bIsNonPCMInDec2)
                    {
                        if (u8ADEC2CodeType == AU_DVB2_STANDARD_DTS)  // Need check
                        {
                            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_SND; /* DSP2 */
                        }
                    }
                    else if (u8ADEC1CodeType == AU_DVB_STANDARD_DTS)
                    {
                        stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    }
                    break;
                }

                case E_AUDIO_INFO_MM_IN:
                case E_AUDIO_INFO_GAME_IN:
                {
                    stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_DTS;
                    stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    break;
                }

                default:
                {
                    stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                    stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    break;
                }
            }

            break;
        }

        case DIGITAL_OUT_AAC_LC:
        {
            stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
            break;
        }

        case DIGITAL_OUT_NONE:
        default:
        {
            stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
            break;
        }

    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF output mode
/// @param <IN>        \b
/// @param <IN>        \b
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDigitalOut(DIGITAL_OUTPUT_TYPE ePath, AUDIO_DIGITAL_OUTPUT_TYPE eDigitalMode)
{

    HALAUDIO_CHECK_SHM_INIT;

    //TODO: add HDMI/ARC digital out status

    // Set Digital mode to Digital out Status structure
    DigitalOutSetting_t stTempDigitalOutStatus;
    stTempDigitalOutStatus.eDigitalOutfMode = eDigitalMode;
    stTempDigitalOutStatus.eSourceType = E_AUDIO_INFO_DTV_IN; // Temp initial value
    stTempDigitalOutStatus.u8NonPcmPath  = AUDIO_DSP_ID_ALL;
    stTempDigitalOutStatus.u8R2NonPcmSetting = 0x00;
    stTempDigitalOutStatus.eGroup = E_CONNECT_NULL;

    if (ePath == DIGITAL_SPDIF_OUTPUT)
    {
        stTempDigitalOutStatus.eGroup = g_AudioVars2->AudioOutputSourceInfo.SpdifOut;
    }
    else if (ePath == DIGITAL_HDMI_ARC_OUTPUT)
    {
        stTempDigitalOutStatus.eGroup = g_AudioVars2->AudioOutputSourceInfo.ArcOut;
    }

    //Determin Data Path
    HAL_AUDIO_DigitalOut_DetermineDataPath(ePath, &stTempDigitalOutStatus);

    // if setting is difference to previous, set digital out mode
    if ((stTempDigitalOutStatus.eDigitalOutfMode != g_AudioVars2->stDigitalOutSetting[ePath].eDigitalOutfMode) ||
        (stTempDigitalOutStatus.eSourceType != g_AudioVars2->stDigitalOutSetting[ePath].eSourceType) ||
        (stTempDigitalOutStatus.u8NonPcmPath != g_AudioVars2->stDigitalOutSetting[ePath].u8NonPcmPath) ||
        (stTempDigitalOutStatus.u8R2NonPcmSetting != g_AudioVars2->stDigitalOutSetting[ePath].u8R2NonPcmSetting) ||
        (stTempDigitalOutStatus.eGroup != g_AudioVars2->stDigitalOutSetting[ePath].eGroup))

    {
        HALAUDIO_PRINT("%s() - eDigitalOutfMode  = %x, eNonPcmPath = %x\n\r", \
                           __FUNCTION__, \
                           stTempDigitalOutStatus.eDigitalOutfMode, \
                           stTempDigitalOutStatus.u8NonPcmPath);

        HAL_AUDIO_DigitalOut_SetDataPath(ePath, &stTempDigitalOutStatus);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Digital_Out_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPdif channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    MS_BOOL ret = FALSE;
    MS_U8 ck_count, checksum_bit, checksum = 0x00;               // used for LG SoundBar checksum
    MS_U8 checksum1, checksum2, checksum3, checksum4, checksum5, checksum6; // used for LG SoundBar checksum
    HALAUDIO_CHECK_SHM_INIT;

    switch (ePath)
    {
        case DIGITAL_SPDIF_OUTPUT:
        {

            // Copy protection
            //C bit
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS0 ,0x20, stChannelStatus->stCopyRight.CBit << 5);
            // L bit
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS1 ,0x01, stChannelStatus->stCopyRight.LBit);

            // PCM format
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS0 ,0xDF, stChannelStatus->u8PcmFormatControl);

            // Category
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS1 , 0xFE, stChannelStatus->u8Category);

            // Source number
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS2, 0xF0, stChannelStatus->u8SourceNumber);

            // Channel number
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS2, 0x0F, stChannelStatus->u8ChannelNumber);

            // Sampling rate
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS3 , 0xF0, stChannelStatus->u8SamplingRate);

            // Clock precision
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS3 , 0x0F, stChannelStatus->u8ClockPrecision);

            // Word Length
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS4 , 0xF0, stChannelStatus->u8WordLength);

            // Original sampling rate
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS4 , 0x0F, stChannelStatus->u8OriginalSamplingRate);

#if 0
            printf("u8MuteOnOff: 0x%x \n", stChannelStatus->u8MuteOnOff);
            printf("u8PowerOnOff1: 0x%x \n", stChannelStatus->u8PowerOnOff1);
            printf("u8PowerOnOff2: 0x%x \n", stChannelStatus->u8PowerOnOff2);
            printf("u8SoundBarID1: 0x%x \n", stChannelStatus->u8SoundBarID1);
            printf("u8SoundBarID2: 0x%x \n", stChannelStatus->u8SoundBarID2);
            printf("u8SoundBarID3: 0x%x \n", stChannelStatus->u8SoundBarID3);
            printf("u8SoundBarID4: 0x%x \n", stChannelStatus->u8SoundBarID4);
            printf("u8SoundBarID5: 0x%x \n", stChannelStatus->u8SoundBarID5);
            printf("u8SoundBarID6: 0x%x \n", stChannelStatus->u8SoundBarID6);
            printf("u8SoundBarVolume1: 0x%x \n", stChannelStatus->u8SoundBarVolume1);
            printf("u8SoundBarVolume2: 0x%x \n", stChannelStatus->u8SoundBarVolume2);
            printf("u8SoundBarVolumeMuteOnOff: 0x%x \n", stChannelStatus->u8SoundBarVolumeMuteOnOff);
            printf("u8SoundBarAutoVolume: 0x%x \n", stChannelStatus->u8SoundBarAutoVolume);
            printf("u8SoundBarWooferLevel: 0x%x \n", stChannelStatus->u8SoundBarWooferLevel);
#endif

            // Set ID / Volume
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS4, 0x0F, stChannelStatus->u8SoundBarID1);
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_00, 0xF0, (stChannelStatus->u8SoundBarID2) << 4);
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_00, 0x0F, stChannelStatus->u8SoundBarID3);
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_01, 0xF0, (stChannelStatus->u8SoundBarID4) << 4);
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_01, 0x0F, stChannelStatus->u8SoundBarID5);
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_02, 0xF0, (stChannelStatus->u8SoundBarID6) << 4);

            // Set Volume
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_02, 0x0F, stChannelStatus->u8SoundBarVolume1);
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_03, 0xE0, (stChannelStatus->u8SoundBarVolume2) <<4);

            // Mute on/off
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_03, 0x10, stChannelStatus->u8MuteOnOff);

            // Power on/off
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_03, 0x08, stChannelStatus->u8PowerOnOff1);
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_04, 0x50, stChannelStatus->u8PowerOnOff2);

            // WooferLevel
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_04, 0x0F, stChannelStatus->u8SoundBarWooferLevel);

            // AutoVolume
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_05, 0x80, stChannelStatus->u8SoundBarAutoVolume);

            /* The bit order is key point
            checksum1:SPDIF_CS[43:36]
            checksum2:SPDIF_CS[51:44]
            checksum3:SPDIF_CS[59:52]
            checksum4:SPDIF_CS[67:60]
            checksum5:SPDIF_CS[75:68]
            checksum6:SPDIF_CS[83:76]
            reserved :SPDIF_CS[99:84] ,not used here

            checksum:SPDIF_CS[107:100]
            */
            checksum1 = ((HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarID1)) |(HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarID2)<<4)) ;
            checksum2 = ((HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarID3)) |(HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarID4)<<4)) ;
            checksum3 = ((HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarID5)) |(HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarID6)<<4));
            checksum4 = ((HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarVolume1)) |(HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarVolume2)<<4)|((stChannelStatus->u8MuteOnOff<<3)));
            checksum5 = (HAL_MAD_GetBitReverse(stChannelStatus->u8PowerOnOff1) |(HAL_MAD_GetBitReverse(stChannelStatus->u8PowerOnOff2>>4)<<4));
            checksum6 = (HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarWooferLevel) |(HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarAutoVolume>>4)<<4));

#if 0
            printf("checksum1 = 0x%x \n", checksum1);
            printf("checksum2 = 0x%x \n", checksum2);
            printf("checksum3 = 0x%x \n", checksum3);
            printf("checksum4 = 0x%x \n", checksum4);
            printf("checksum5 = 0x%x \n", checksum5);
            printf("checksum6 = 0x%x \n", checksum6);
#endif

            // exclusive OR(XOR) SoundBar (checksum1, checksum2, checksum3, checksum4, checksum5, checksum6)
            // checksum_bit is XOR result for each bit position
            // checksum is total result
            for (ck_count = 0; ck_count < 8; ck_count++)
            {
                checksum_bit = 0x0;

                checksum_bit = checksum_bit + ((checksum1>>ck_count)&0x01);
                checksum_bit = checksum_bit ^((checksum2>>ck_count)&0x01) ;
                checksum_bit = checksum_bit ^((checksum3>>ck_count)&0x01) ;
                checksum_bit = checksum_bit ^((checksum4>>ck_count)&0x01) ;
                checksum_bit = checksum_bit ^((checksum5>>ck_count)&0x01) ;
                checksum_bit = checksum_bit ^((checksum6>>ck_count)&0x01) ;
                checksum = checksum |(checksum_bit<<ck_count);
            }

            stChannelStatus->u8SoundBarCheckSum = checksum;

            // SoundBar CheckSum
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_07, 0x0F, HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarCheckSum&0x0F));
            HAL_AUDIO_WriteMaskByte(REG_SPDIF_OUT_CS_PRO_08, 0xF0, (HAL_MAD_GetBitReverse(stChannelStatus->u8SoundBarCheckSum>>4))<<4);

            if ((stChannelStatus->u8PcmFormatControl & SPDIF_CS_FORMAT_NONPCM) == SPDIF_CS_FORMAT_NONPCM)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x80, 0x80);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x80, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS3 , 0xF0, SPDIF_CS_FS_48K);
            }

            HAL_AUDIO_WriteMaskByte(REG_SPDIFTX_CHANNEL_STATUS_TOGGLE, 0x40, 0x00); //Tx1 Toggle bit[14]
            HAL_AUDIO_WriteMaskByte(REG_SPDIFTX_CHANNEL_STATUS_TOGGLE, 0x40, 0x40);
            HAL_AUDIO_WriteMaskByte(REG_SPDIFTX_CHANNEL_STATUS_TOGGLE, 0x40, 0x00);
            memcpy(&(g_AudioVars2->stDigitalOutChannelStatus[ePath]), stChannelStatus, sizeof(Digital_Out_Channel_Status_t));
            break;
        }

        case DIGITAL_HDMI_ARC_OUTPUT:
        {
            // Copy protection
            //C bit
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS0 ,0x20, stChannelStatus->stCopyRight.CBit << 5);
            // L bit
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS1 ,0x01, stChannelStatus->stCopyRight.LBit);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS0 ,0xDF, stChannelStatus->u8PcmFormatControl);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS1 , 0xFE, stChannelStatus->u8Category);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS2, 0xF0, stChannelStatus->u8SourceNumber);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS2, 0x0F, stChannelStatus->u8ChannelNumber);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS3 , 0xF0, stChannelStatus->u8SamplingRate);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS3 , 0x0F, stChannelStatus->u8ClockPrecision);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS4 , 0xF0, stChannelStatus->u8WordLength);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS4 , 0x0F, stChannelStatus->u8OriginalSamplingRate);

            if ((stChannelStatus->u8PcmFormatControl & SPDIF_CS_FORMAT_NONPCM) == SPDIF_CS_FORMAT_NONPCM)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x80, 0x80);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x80, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS3 , 0xF0, SPDIF_CS_FS_48K);
            }

            HAL_AUDIO_WriteMaskByte(REG_SPDIFTX_CHANNEL_STATUS_TOGGLE, 0x10, 0x00); //Tx2 Toggle bit[12]
            HAL_AUDIO_WriteMaskByte(REG_SPDIFTX_CHANNEL_STATUS_TOGGLE, 0x10, 0x10);
            HAL_AUDIO_WriteMaskByte(REG_SPDIFTX_CHANNEL_STATUS_TOGGLE, 0x10, 0x00);
            memcpy(&(g_AudioVars2->stDigitalOutChannelStatus[ePath]), stChannelStatus, sizeof(Digital_Out_Channel_Status_t));
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s() - Invalid SPDIF Path\n",__FUNCTION__);
            break;
        }
    }

   return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Digital_Out_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPdif channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    MS_BOOL ret = FALSE;

    HALAUDIO_CHECK_SHM_INIT;

    if (stChannelStatus == NULL)
    {
        HALAUDIO_ERROR("%s() : stChannelStatus shouldn't be NULL !!!\n", __FUNCTION__);
    }
    else
    {
        memcpy(stChannelStatus, &(g_AudioVars2->stDigitalOutChannelStatus[ePath]), sizeof(Digital_Out_Channel_Status_t));
        ret = TRUE;
    }

    return ret;
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO SPDIF Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_SPDIF_HWEN()  @@Cathy
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param <IN>        \b bEnable:    0--Disable S/PDIF out
///                                    1--Enable S/PDIF out
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_SPDIF_HWEN(MS_BOOL bEnable)
{
    if(bEnable)
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x08, 0x00); //0x2C8A[3] Synch. Fixed output
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x10, 0x00); // 0x2C8A[12] REG_SPDIF_PULL_LOW : output force zero.
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x80, 0x80);     // 0x2C8A[7] 1:Enable  SPDIF   0: Disable SPDIF
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x08, 0x00); //0x2C8A[3] Synch. Fixed output
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x10, 0x10); // 0x2C8A[12] REG_SPDIF_PULL_LOW : output force zero.
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_SPDIF_SetMute()  @@KH
/// @brief \b Function \b Description:  This routine is used to mute S/PDIF output(by DSP)
/// @param <IN>        \b bEnMute    :    0--Disable mute
///                                        1--Enable mute
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_SPDIF_SetMute(MS_BOOL bEnMute)
{
    if (bEnMute == TRUE)
    {
        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x01, 0x01);        // Software mute
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x01, 0x00);        // Software unmute
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_SPDIF_ByPassChannel()  @@Need_Modify
/// @brief \b Function \b Description:  ByPass the SPDIF channel (CH4)
/// @param <IN>        \b \b enable    :     TRUE --BYPASS CH4
///                                        FALSE--OPEN CH4
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_SPDIF_ByPassChannel(MS_BOOL bEnable)
{
    bEnable = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF output mode
/// @param <IN>        \b u8Spdif_mode    :
///                                    bit[0] = 0: spdif enable,   1: spdif disable (Se-DSP)
///                                    bit[1] = 0: PCM mode,     1: non-PCM mode
///                                    bit[2] = 1: non-PCM NULL Payload
/// @param <IN>        \b u8Input_src  0 : DTV
///                                    1 : ATV
///                                    2 : HDMI
///                                    3 : ADC
///                                    4 : CardReader
///                                    5 : SPDIF
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SPDIF_SetMode(MS_U8 u8Spdif_mode, MS_U8 u8Input_src)
{
    // Multiple npcm source, still need total rework
    AUDIO_SOURCE_INFO_TYPE eAudioSource = -1;
    MS_U8 u8SpdifMode = ((u8Spdif_mode == SPDIF_OUT_NONE) ? SPDIF_OUT_PCM : u8Spdif_mode);
    MS_U8 u8MainDecId = 0;
    AUDIO_DSP_CODE_TYPE u8MainDspCodeType = AU_DVB_STANDARD_INVALID;
    AUDIO_DSP_CODE_TYPE u8ADEC1CodeType = AU_DVB_STANDARD_INVALID;
    AUDIO_DSP_CODE_TYPE u8ADEC2CodeType = AU_DVB_STANDARD_INVALID;
    MS_U32 u32bDTSCD = 0;
    MS_U32 u32DTSTranscodeFlag = 0;
    MS_U32 u32HDMITxSamplingRate = 0;
    MS_U32 u32SPDIFTxSamplingRate = 0;
    MS_U64 u64DecSamplingRate = 0;
    static MS_U32 u32bPreDTSCD = 0xFF;
    static MS_U32 u32PreHDMITxSamplingRate = 0;
    static MS_U32 u32PreSPDIFTxSamplingRate = 0;
    MS_U8 u8R2NonPcmSel = ADEC1;
    static MS_S32 s32Pre_ddp_enc_enable = 0;
    MS_S32 ddp_enc_enable = 0;
    MS_U32 u32HdmiArcSpecifiedOutputCodec = DIGITAL_OUTPUT_CODEC_NOT_SPECIFIED;
    MS_U32 u32SPDIFSpecifiedOutputCodec = DIGITAL_OUTPUT_CODEC_NOT_SPECIFIED;
    MS_U32  u32IsAC3P = 0;
    MS_BOOL bNonPCMMixedSystemSound = FALSE;

    HALAUDIO_CHECK_SHM_INIT;

    UNUSED(u8Input_src);
    eAudioSource = g_AudioVars2->eAudioSource;
    u8ADEC1CodeType = g_AudioVars2->g_u8DecR2Adec1DecType;
    u8ADEC2CodeType = g_AudioVars2->g_u8DecR2Adec2DecType;
    u32HdmiArcSpecifiedOutputCodec = g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32HdmiArcOutputCodec;
    u32SPDIFSpecifiedOutputCodec = g_AudioVars2->g_AudioSpecifiedDigitalOutput.u32SpdifOutputCodec;

    //For GST trick play mode, SPDIF can only output PCM
    if(g_AudioVars2->g_bTrickPlay == TRUE)
    {
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SPDIF_TYPE, ADEC1, 1, 0); /* PCM */
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SPDIF_TYPE, ADEC2, 1, 0); /* PCM */
        u8SpdifMode = SPDIF_OUT_PCM;
    }
    else // Follow UI Setting
    {
        if( u8SpdifMode == SPDIF_OUT_PCM )
        {
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SPDIF_TYPE, ADEC1, 1, 0); /* PCM */
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SPDIF_TYPE, ADEC2, 1, 0); /* PCM */
        }
        else
        {
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SPDIF_TYPE, ADEC1, 2, 0); /* Non-PCM */
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SPDIF_TYPE, ADEC2, 2, 0); /* Non-PCM */
        }
    }


    if(g_AudioVars2->g_R2_NonPcm_Sel == ADEC1)
    {
        u8MainDspCodeType = u8ADEC1CodeType;
        u8R2NonPcmSel = ADEC1;
        u8MainDecId = AU_DEC_ID1;
        u64DecSamplingRate = HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_sampleRate);
        HAL_MAD2_Write_DSP_sram(DSP2DmAddr_spdif_nonpcm_source, 0, DSP_MEM_TYPE_DM);
    }
    else
    {
        u8MainDspCodeType = u8ADEC2CodeType;
        u8R2NonPcmSel = ADEC2;
        u8MainDecId = AU_DEC_ID3;
        u64DecSamplingRate = HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC2_sampleRate);
        HAL_MAD2_Write_DSP_sram(DSP2DmAddr_spdif_nonpcm_source, 1, DSP_MEM_TYPE_DM);
    }

    if(g_AudioVars2->AudioDecStatus[u8MainDecId].eSourceType != E_AUDIO_INFO_MM_IN)   // the foreground is not MM
    {
        g_AudioVars2->g_bPreNotMM_Flag = TRUE;
    }
    else if((g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eSourceType == E_AUDIO_INFO_MM_IN) && (g_AudioVars2->AudioDecStatus[AU_DEC_ID3].eSourceType == E_AUDIO_INFO_MM_IN))   // for MM/youtube switch case
    {
        if(pre_DecId != u8MainDecId)
        {
            g_AudioVars2->g_bPreNotMM_Flag = TRUE;
        }
        pre_DecId = u8MainDecId;
    }

    //HALAUDIO_ERROR("\n\033[1;36m  [%s] mainsource = [%d] g_bPreNotMM_Flag[%d] \033[0m \n", __FUNCTION__, g_AudioVars2->AudioDecStatus[u8MainDecId].eSourceType, g_AudioVars2->g_bPreNotMM_Flag);
    //HALAUDIO_ERROR("%s: u8Spdif_mode = %d, eAudioSource = %d, u8MainDspCodeType = %d, u64DecSamplingRate = %u, u32SPDIFSpecifiedOutputCodec = %u, u32HdmiArcSpecifiedOutputCodec = %u\r\n", __FUNCTION__, u8Spdif_mode, eAudioSource, u8MainDspCodeType, (unsigned int)u64DecSamplingRate, (unsigned int)u32SPDIFSpecifiedOutputCodec, (unsigned int)u32HdmiArcSpecifiedOutputCodec);

    /* Configure SPDIF's Output Setting */
    switch(eAudioSource)
    {
        case E_AUDIO_INFO_DTV_IN:
        case E_AUDIO_INFO_MM_IN:
        case E_AUDIO_INFO_GAME_IN:
        {
            switch(u8MainDspCodeType)
            {
                case AU_DVB_STANDARD_AAC:
                case AU_DVB_STANDARD_MS10_DDT:
                {
                    if (u8SpdifMode != SPDIF_OUT_PCM )
                    {
                        switch(u8SpdifMode)
                        {
                            case SPDIF_OUT_NONPCM:
                            case SPDIF_OUT_TRANSCODE:
                            default:
                            {
                                if ( u64DecSamplingRate < 4) //DDE can't be enabled for AAC SampleRate<16k
                                {
                                    u8SpdifMode  = SPDIF_OUT_PCM;
                                }
                                else
                                {
                                    bNonPCMMixedSystemSound = TRUE;
                                }
                                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC1, 0, 0);
                                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC2, 0, 0);
                            }
                                break;

                            case SPDIF_OUT_BYPASS:
                            {
                                if(eAudioSource == E_AUDIO_INFO_MM_IN) // MM always output AC3
                                {
                                    u8SpdifMode  = SPDIF_OUT_TRANSCODE;
                                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC1, 0, 0);
                                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC2, 0, 0);
                                    bNonPCMMixedSystemSound = TRUE;
                                }
                                else
                                {
                                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC1, 1, 0);
                                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC2, 1, 0);
                                }
                            }
                                break;
                        }

                    }
                    break;
                }

                case AU_DVB_STANDARD_AC3:
                case AU_DVB_STANDARD_AC3P:
                case AU_DVB_STANDARD_MS10_DDC:
                {
                    if (u8SpdifMode != SPDIF_OUT_PCM)
                    {
                        u8SpdifMode = SPDIF_OUT_TRANSCODE;
                        bNonPCMMixedSystemSound = TRUE;

                        u32IsAC3P = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_DEC_TYPE, u8R2NonPcmSel);
                        if ((u32HdmiArcSpecifiedOutputCodec == DIGITAL_OUTPUT_CODEC_AC3P) && (u32IsAC3P == 1))
                        {
                            HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_DAP_SEL, 0x20, 0x20);   //enable DDP encoder
                            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DDP_ENCODE_EN, u8R2NonPcmSel, 1, 0);
                            ddp_enc_enable = 1;
                        }
                    }

                    //always disable DDP bypass in DMR
                    HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, 0, ADEC1);
                    HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, 0, ADEC2);

                    break;
                }

                case AU_DVB_STANDARD_DTS:
                case AU_DVB_STANDARD_DTSLBR:
                case AU_DVB_STANDARD_DTSXLL:
                {
                    if (u8MainDspCodeType == AU_DVB_STANDARD_DTS)
                    {
                        HAL_MAD_GetAudioInfo2(u8MainDecId , Audio_R2_SHM_INFO_DTS_CD_MODE, (void *)&u32bDTSCD);
                    }

                    if (u8SpdifMode != SPDIF_OUT_PCM)
                    {
                        u8SpdifMode = SPDIF_OUT_TRANSCODE;
                        /* 0: disable, 1: bypass/convert, 2: transcoder, 3: pcm */
                        u32DTSTranscodeFlag = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TRANSCODE_FLAG, u8R2NonPcmSel);
                        if ( u32SPDIFSpecifiedOutputCodec != DIGITAL_OUTPUT_CODEC_DTS )
                        {
                            u8SpdifMode  = SPDIF_OUT_PCM;
                        }
                    }
                    break;
                }

                default:
                {
                    u8SpdifMode  = SPDIF_OUT_PCM;
                    break;
                }
            }

            break;
        }

        case E_AUDIO_INFO_ATV_IN:
        case E_AUDIO_INFO_ADC_IN:
        case E_AUDIO_INFO_KTV_IN:
        {
            u8SpdifMode  = SPDIF_OUT_PCM;
            break;
        }

        case E_AUDIO_INFO_HDMI_IN:
        {
            if (HAL_AUDIO_HDMI_NonpcmMonitor() == FALSE)
            {
                u8SpdifMode = SPDIF_OUT_PCM;
            }
            else
            {
                switch(u8MainDspCodeType)
                {
                    case AU_DVB_STANDARD_AAC:
                    case AU_DVB_STANDARD_MS10_DDT:
                    {
                        if (u8SpdifMode != SPDIF_OUT_PCM )
                        {
                            switch(u8SpdifMode)
                            {
                                case SPDIF_OUT_NONPCM:
                                case SPDIF_OUT_TRANSCODE:
                                default:
                                {
                                    if ( u64DecSamplingRate < 4) //DDE can't be enabled for AAC SampleRate<16k
                                    {
                                        u8SpdifMode  = SPDIF_OUT_PCM;
                                    }
                                    else
                                    {
                                        bNonPCMMixedSystemSound = TRUE;
                                    }
                                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC1, 0, 0);
                                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC2, 0, 0);
                                }
                                    break;

                                case SPDIF_OUT_BYPASS:
                                {
                                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC1, 1, 0);
                                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, ADEC2, 1, 0);
                                }
                                    break;
                            }

                        }
                        break;
                    }

                    case AU_DVB_STANDARD_AC3:
                    case AU_DVB_STANDARD_AC3P:
                    case AU_DVB_STANDARD_MS10_DDC:
                    {
                        if (u8SpdifMode != SPDIF_OUT_PCM)
                        {
                            u8SpdifMode = SPDIF_OUT_TRANSCODE;
                            bNonPCMMixedSystemSound = TRUE;

                            u32IsAC3P = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_DEC_TYPE, u8R2NonPcmSel);
                            if ((u32HdmiArcSpecifiedOutputCodec == DIGITAL_OUTPUT_CODEC_AC3P) && (u32IsAC3P == 1))
                            {
                                HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_DAP_SEL, 0x20, 0x20);   //enable DDP encoder
                                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DDP_ENCODE_EN, u8R2NonPcmSel, 1, 0);
                                ddp_enc_enable = 1;
                            }
                        }
                        //always disable DDP bypass in DMR
                        HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, 0, ADEC1);
                        HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, 0, ADEC2);

                        break;
                    }

                    case AU_DVB_STANDARD_DTS:
                    {
                        HAL_MAD_GetAudioInfo2(u8MainDecId , Audio_R2_SHM_INFO_DTS_CD_MODE, (void *)&u32bDTSCD);

                        if (u8SpdifMode != SPDIF_OUT_PCM)
                        {
                            u8SpdifMode = SPDIF_OUT_TRANSCODE;
                            /* 0: disable, 1: bypass/convert, 2: transcoder, 3: pcm */
                            u32DTSTranscodeFlag = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TRANSCODE_FLAG, u8R2NonPcmSel);
                            if ( u32SPDIFSpecifiedOutputCodec != DIGITAL_OUTPUT_CODEC_DTS )
                            {
                                u8SpdifMode  = SPDIF_OUT_PCM;
                            }
                        }

                        break;
                    }

                    default:
                    {
                        u8SpdifMode  = SPDIF_OUT_PCM;
                        break;
                    }
                }
            }

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! invalid eAudioSource(%d)!\r\n", __FUNCTION__, eAudioSource);
            break;
        }
    }

    if(ddp_enc_enable == 0)
    {
        HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_DAP_SEL, 0x20, 0x0);   //disable DDP encoder
		HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DDP_ENCODE_EN, u8R2NonPcmSel, 0, 0);
    }

    if(bNonPCMMixedSystemSound != g_AudioVars2->g_bNonPCMMixedSystemSound)
    {
        g_AudioVars2->g_bNonPCMMixedSystemSound = bNonPCMMixedSystemSound;
    }

    //HALAUDIO_ERROR("%s: u8SpdifMode = %d, u8R2NonPcmSel = %d, u32bDTSCD = %u, u32IsAC3P = %u, ddp_enc_enable = %u\r\n", __FUNCTION__, u8SpdifMode, u8R2NonPcmSel, (unsigned int)u32bDTSCD, (unsigned int)u32IsAC3P, (unsigned int)ddp_enc_enable);

    if(u8SpdifMode == SPDIF_OUT_PCM)
    {
        u32HDMITxSamplingRate = 48000;
        u32SPDIFTxSamplingRate = 48000;
    }
    else
    {
        u32HDMITxSamplingRate = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_HDMITx_SMP_RATE, u8R2NonPcmSel);
        u32SPDIFTxSamplingRate = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SPDIFTx_SMP_RATE, u8R2NonPcmSel);
    }

    if ((_HAL_AUDIO_SPDIF_GetMode() == u8SpdifMode) &&
        (g_AudioVars2->g_Pre_R2_NonPcm_Sel == u8R2NonPcmSel) &&
        (g_AudioVars2->g_PreMainDspCodeType == u8MainDspCodeType) &&
        (u32PreHDMITxSamplingRate == u32HDMITxSamplingRate) &&
        (u32PreSPDIFTxSamplingRate == u32SPDIFTxSamplingRate) &&
        (s32Pre_ddp_enc_enable == ddp_enc_enable))
    {
        if (u8MainDspCodeType == AU_DVB_STANDARD_DTS)
        {
            if (u32bPreDTSCD == u32bDTSCD)
            {
                /* In DTS case, if DTS-CD information is same, no need to to change SPDIF setting */
                return;
            }
        }
        else
        {
            /* No need to change SPDIF setting */
            return;
        }
    }

    MS_BOOL retTx1, retTx2;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));
    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);


    if (retTx1 == FALSE || retTx2 == FALSE)
    {
        HALAUDIO_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
        return;
    }

    /* Mute SPDIF before changing setting */
    _HAL_AUDIO_SPDIF_SetMute(TRUE);

    switch(u8R2NonPcmSel)
    {
        case ADEC1:
        {
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH1_CFG, 0x07, 0x00); //for SE-DSP NonPCM input select
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH2_CFG, 0x07, 0x00);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH3_CFG, 0x07, 0x00);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH4_CFG, 0x07, 0x00);
            HAL_AUR2_WriteByte(REG_R2_HDMI_NONPCM_PATH, 0);   //for SPDIF input decoder select
        }
            break;

        case ADEC2:
        default:
        {
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH1_CFG, 0x07, 0x02); //for SE-DSP NonPCM input select
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH2_CFG, 0x07, 0x02);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH3_CFG, 0x07, 0x02);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH4_CFG, 0x07, 0x02);
            HAL_AUR2_WriteByte(REG_R2_HDMI_NONPCM_PATH, 1);   //for SPDIF input decoder select
        }
            break;
    }

    /* Apply SPDIF's Output Setting */
    switch (u8SpdifMode)
    {
        case SPDIF_OUT_NONPCM:
        case SPDIF_OUT_BYPASS:
        case SPDIF_OUT_TRANSCODE:
        {
            //HALAUDIO_PRINT("HAL SPDIF set as Non-PCM\n");

            if (u8SpdifMode == SPDIF_OUT_TRANSCODE)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC1, TRUE, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC2, TRUE, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC1, FALSE, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC2, FALSE, 0);
            }

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x07, 0x02); /* Grp C in */
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x01, 0x00);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x02); /* Grp C in */
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x01, 0x00);
            stDigitalChannelStatusTx1.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            stDigitalChannelStatusTx2.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x02, 0x02); /* Non-PCM */

            break;
        }
        case SPDIF_OUT_PCM:
        default:
        {
            //HALAUDIO_PRINT("HAL SPDIF set as PCM\n");

            AUDIO_DELAY1MS(5);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC1, FALSE, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC2, FALSE, 0);
            HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x2, 0x0); /* PCM Mode */
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x07, 0x00); /* Tx1 Grp A in */
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x00); /* Tx2 Grp A in */
            stDigitalChannelStatusTx1.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;
            stDigitalChannelStatusTx2.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;

            break;
        }
    }

    u32bPreDTSCD = u32bDTSCD;
	s32Pre_ddp_enc_enable = ddp_enc_enable;
    g_u32bDTSCD = u32bDTSCD;
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

    g_AudioVars2->g_eSpdifOutputType = u8SpdifMode;
    g_AudioVars2->g_Pre_R2_NonPcm_Sel = u8R2NonPcmSel;
    g_AudioVars2->g_PreMainDspCodeType = u8MainDspCodeType;

    u32PreHDMITxSamplingRate = u32HDMITxSamplingRate;
    u32PreSPDIFTxSamplingRate = u32SPDIFTxSamplingRate;
    HAL_AUDIO_Set_DecimationMode(AUDIO_HDMI_ARC_OUTPUT, u8R2NonPcmSel);
    HAL_AUDIO_Set_DecimationMode(AUDIO_SPDIF_OUTPUT, u8R2NonPcmSel);

    _HAL_AUDIO_SPDIF_SetMute(g_AudioVars2->g_SPDIF_MuteStatus);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_SPDIF_GetMode()  @@KH
/// @brief \b Function \b Description:  This routine is used to get S/PDIF mode.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  BYTE    :  Se-DSP REG_M2D_MAILBOX_SPDIF_CTRL value
///                                u8Spdif_mode = 0x0 : SPDIF enable, PCM mode
///                                u8Spdif_mode = 0x1 : SPDIF OFF
///                                u8Spdif_mode = 0x2 : SPDIF enable, non-PCM mode
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 _HAL_AUDIO_SPDIF_GetMode(void)
{
    return g_AudioVars2->g_eSpdifOutputType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetSCMS()
/// @brief \b Function \b Description:  This routine is used to set SPDIF SCMS.
/// @param C_bit_en  \b : copy right control bit, register in 0x2C80[5]
/// @param L_bit_en  \b : generation bit, register in 0x2C82[7]
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en)
{
    MS_BOOL retTx1, retTx2;

    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));
    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

    if (retTx1 == TRUE && retTx2 == TRUE)
    {
        stDigitalChannelStatusTx1.stCopyRight.CBit = C_bit_en;
        stDigitalChannelStatusTx1.stCopyRight.LBit = L_bit_en;
        stDigitalChannelStatusTx2.stCopyRight.CBit = C_bit_en;
        stDigitalChannelStatusTx2.stCopyRight.LBit = L_bit_en;
    }
    else
    {
        HALAUDIO_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
        return;
    }

    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_GetSCMS()
/// @brief \b Function \b Description:  This routine is used to get SPDIF SCMS.
/// @return MS_U8      \b :  SCMS[0] = C bit status, SCMS[1] = L bit status
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_SPDIF_GetSCMS(void)
{

    MS_U8   SCMS_status, SCMS_C_bit_tmp, SCMS_L_bit_tmp;

    HALAUDIO_CHECK_SHM_INIT;

    if(g_AudioVars2->stDigitalOutChannelStatus[DIGITAL_SPDIF_OUTPUT].stCopyRight.CBit)
    {
        SCMS_C_bit_tmp = 0x01;
    }
    else
    {
        SCMS_C_bit_tmp = 0x00;
    }

    if(g_AudioVars2->stDigitalOutChannelStatus[DIGITAL_SPDIF_OUTPUT].stCopyRight.LBit)
    {
        SCMS_L_bit_tmp = 0x02;
    }
    else
    {
        SCMS_L_bit_tmp = 0x00;
    }

    SCMS_status = SCMS_C_bit_tmp | SCMS_L_bit_tmp;

    return(SCMS_status);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPdif channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetChannelStatus(SPDIF_CS_MODE_TYPE eType, SPDIF_CS_MODE_VALUE eValue)
{
    MS_U8 u8Type;

    u8Type=(MS_U8)eType;

    MS_BOOL retTx1, retTx2;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));
    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);


    if (retTx1 == FALSE || retTx2 == FALSE)
    {
        HALAUDIO_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
        return;
    }

    switch(u8Type)
    {
        case SPDIF_CHANNEL_STATUS_FS:
            stDigitalChannelStatusTx1.u8SamplingRate = (MS_U8)eValue; // S/PDIF Tx1
            stDigitalChannelStatusTx2.u8SamplingRate = (MS_U8)eValue; // S/PDIF Tx2
            break;
        case SPDIF_CHANNEL_STATUS_CATEGORY:
            stDigitalChannelStatusTx1.u8Category = (MS_U8)eValue; //  Tx1 By-pass L- bit
            stDigitalChannelStatusTx2.u8Category = (MS_U8)eValue; //  Tx2 By-pass L- bit
            break;
        case SPDIF_CHANNEL_STATUS_PCM_FORMAT:
            stDigitalChannelStatusTx1.u8PcmFormatControl = (MS_U8)eValue; // Tx1 By-pass Copy right bit
            stDigitalChannelStatusTx2.u8PcmFormatControl = (MS_U8)eValue; // Tx2 By-pass Copy right bit
            break;

        default :
            break;
    }

    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_ChannelStatus_CTRL()
/// @brief \b Function \b Description:  This routine is used to control SPDIF CS in detail.
/// @return MS_U8      \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
{
    MS_BOOL status_rtn = FALSE;
    MS_BOOL retTx1, retTx2;

    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));
    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

    if (retTx1 == FALSE || retTx2 == FALSE)
    {
        HALAUDIO_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
        return status_rtn;
    }

    switch (cs_mode)
    {
        case SPDIF_CS_CategoryCode:
            switch(status)
            {
                case SPDIF_CS_Category_BroadCast:
                    stDigitalChannelStatusTx1.u8Category = 0x20;
                    stDigitalChannelStatusTx2.u8Category = 0x20;
                    break;

                case SPDIF_CS_Category_General:
                    stDigitalChannelStatusTx1.u8Category = 0x00;
                    stDigitalChannelStatusTx2.u8Category = 0x00;
                    break;

                case SPDIF_CS_Category_BroadCast_JP:
                    stDigitalChannelStatusTx1.u8Category = 0x20;
                    stDigitalChannelStatusTx2.u8Category = 0x20;
                    break;

                case SPDIF_CS_Category_BroadCast_EU:
                    stDigitalChannelStatusTx1.u8Category = 0x30;
                    stDigitalChannelStatusTx2.u8Category = 0x30;
                    break;

                case SPDIF_CS_Category_BroadCast_USA:
                    stDigitalChannelStatusTx1.u8Category = 0x26;
                    stDigitalChannelStatusTx2.u8Category = 0x26;
                    break;

                default :
                    stDigitalChannelStatusTx1.u8Category = 0x00;
                    stDigitalChannelStatusTx2.u8Category = 0x00;
                    break;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_CopyRight:
            if(status == SPDIF_CS_CopyRight_CP)
            {
                stDigitalChannelStatusTx1.stCopyRight.CBit = 0;
                stDigitalChannelStatusTx2.stCopyRight.CBit = 0;
            }
            if(status == SPDIF_CS_CopyRight_NonCP)
            {
                stDigitalChannelStatusTx1.stCopyRight.CBit = 1;
                stDigitalChannelStatusTx2.stCopyRight.CBit = 1;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_SourceNumber:
            if(status == SPDIF_CS_SourceNumber_2)
            {
                stDigitalChannelStatusTx1.u8SourceNumber = 0x40;
                stDigitalChannelStatusTx2.u8SourceNumber = 0x40;
            }
            if(status == SPDIF_CS_SourceNumber_5)
            {
                stDigitalChannelStatusTx1.u8SourceNumber = 0xA0;
                stDigitalChannelStatusTx2.u8SourceNumber = 0xA0;
            }
            if(status == SPDIF_CS_SourceNumber_6)
            {
                stDigitalChannelStatusTx1.u8SourceNumber = 0xA0;
                stDigitalChannelStatusTx2.u8SourceNumber = 0xA0;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_ChannelNumber:
            if(status == SPDIF_CS_ChannelNumber_A_Left)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x08;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x08;
            }
            if(status == SPDIF_CS_ChannelNumber_B_Right)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x04;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x04;
            }
            if(status == SPDIF_CS_ChannelNumber_C)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x0C;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x0C;
            }
            if(status == SPDIF_CS_ChannelNumber_D)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x02;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x02;
            }
            if(status == SPDIF_CS_ChannelNumber_E)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x0A;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x0A;
            }
            if(status == SPDIF_CS_ChannelNumber_F)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x06;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x06;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_SmpFreq:
            switch (status)
            {
                case SPDIF_CS_SmpFreq_192:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x70;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x70;
                    break;

                case SPDIF_CS_SmpFreq_96:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x50;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x50;
                    break;

                case SPDIF_CS_SmpFreq_48:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x40;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x40;
                    break;

                case SPDIF_CS_SmpFreq_176:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x30;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x30;
                    break;

                case SPDIF_CS_SmpFreq_88:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x10;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x10;
                    break;

                case SPDIF_CS_SmpFreq_44:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x00;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x00;
                    break;

                case SPDIF_CS_SmpFreq_64:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x00;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x00;
                    break;

                case SPDIF_CS_SmpFreq_32:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0xC0;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0xC0;
                    break;

                default:
                    status_rtn = FALSE;
                    break;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_ClockAcc:
            status_rtn = TRUE;
            break;

        case SPDIF_ChannelAB:
            if (status == SPDIF_ChannelAB_Invalid_ON)
            {
                stDigitalChannelStatusTx1.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;   // Tx1 Invalid bit=high in non-PCM mode
                stDigitalChannelStatusTx2.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;   // Tx2 Invalid bit=high in non-PCM mode
            }
            if (status == SPDIF_ChannelAB_Invalid_OFF)
            {
                stDigitalChannelStatusTx1.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;      // Tx1 Invalid bit=low in PCM mode
                stDigitalChannelStatusTx2.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;      // Tx2 Invalid bit=low in PCM mode
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_CopyProtect:
            switch (status)
            {
                case SPDIF_CS_CopyProtect_CopyFree:
                    stDigitalChannelStatusTx1.stCopyRight.CBit = 1;
                    stDigitalChannelStatusTx1.stCopyRight.LBit = 0;
                    stDigitalChannelStatusTx2.stCopyRight.CBit = 1;
                    stDigitalChannelStatusTx2.stCopyRight.LBit = 0;
                    status_rtn = TRUE;
                    break;

                 case SPDIF_CS_CopyProtect_CopyOnce:
                    stDigitalChannelStatusTx1.stCopyRight.CBit = 0;
                    stDigitalChannelStatusTx1.stCopyRight.LBit = 0;
                    stDigitalChannelStatusTx2.stCopyRight.CBit = 0;
                    stDigitalChannelStatusTx2.stCopyRight.LBit = 0;
                    status_rtn = TRUE;
                    break;

                 case SPDIF_CS_CopyProtect_CopyNever:
                    stDigitalChannelStatusTx1.stCopyRight.CBit = 0;
                    stDigitalChannelStatusTx1.stCopyRight.LBit = 1;
                    stDigitalChannelStatusTx2.stCopyRight.CBit = 0;
                    stDigitalChannelStatusTx2.stCopyRight.LBit = 1;
                    status_rtn = TRUE;
                    break;

                default:
                    status_rtn = FALSE;
                    break;
            }
            break;

        default:
            status_rtn = FALSE;
            break;

    }

    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

    return(status_rtn);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_Monitor_SamplingRate()
/// @brief \b Function \b Description: This routine is to set ADC clock for SPDIF raw out
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SPDIF_Monitor_SamplingRate(void)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetOutputType()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF output mode
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SPDIF_SetOutputType(AUDIO_SOURCE_INFO_TYPE eSource, AUDIO_SPDIF_OUTPUT_TYPE eType)
{
    eSource=eSource;
    eType=eType;
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO HDMI Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_HDMI_Tx_SetMute()
/// @brief \b Function \b Description:  This routine is used to mute HDMI Tx output(by DSP)
/// @param <IN>        \b bEnMute    :    0--Disable mute
///                                       1--Enable mute
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_HDMI_Tx_SetMute(MS_BOOL bEnMute)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_NonpcmMonitor()
/// @brief \b Function \b Description:  Report PCM/HDMI non-PCM status
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : return HDMI PCM/non-PCM status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_HDMI_NonpcmMonitor(void)
{
    HAL_AUDIO_AbsWriteMaskByte(0x1027C8, 0x02, 0x02);        // Add  audio bank offset
    if(((HAL_AUDIO_ReadByte(REG_AUDIO_STATUS_INPUT) & 0xC0)==0x40)||(HAL_AUDIO_HDMI_NonpcmMonitor2() == 0x1))
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_NonpcmMonitor2()
/// @brief \b Function \b Description:  Report PCM/HDMI non-PCM status from DSP
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : return HDMI PCM/non-PCM status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_HDMI_NonpcmMonitor2(void)
{
    MS_U32 HDMI_lock_status_tmp;
    HDMI_lock_status_tmp = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_hdmi_npcm_lock, DSP_MEM_TYPE_DM);
    if ((HDMI_lock_status_tmp&0x30) == 0x30)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_DolbyMonitor()  @@Need_Modify
/// @brief \b Function \b Description:  Report HDMI non-PCM Dolby mod status
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  u8DolbyModeType    :
///                                0-- Other mode
///                                1-- Dolby mode
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_HDMI_DolbyMonitor(void)
{
    return HAL_AUDIO_ReadByte(REG_AUDIO_STATUS_HDMI_PC);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_SetNonpcm()  @@Need_Modify
/// @brief \b Function \b Description:  Set HDMI PCM/non-PCM Mode
/// @param <IN>        \b nonPCM_en    :
///                                -- 0: PCM mode
///                                -- 1: non-PCM mode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en)
{
//=======================================================
    Hdmi_Input_Path HDMI_INPUT_PATH ;
    MS_BOOL bIsNonPCM= FALSE;
    MS_U8 u8DecoderType = 0;

    bIsNonPCM = (MS_BOOL)(nonPCM_en & HDMI_NonPCM_Mask);  // Use bit 1 mask check PCM/nonPCM
    HDMI_INPUT_PATH = (Hdmi_Input_Path)(nonPCM_en & HDMI_Input_Path_Mask); // Use bit2 mask check main or sub in PIP

    if (bIsNonPCM)  //====== HDMI nonpcm mode =======
    {
        u8DecoderType = HAL_AUDIO_HDMI_DolbyMonitor();

        switch(u8DecoderType)
        {
            case AU_HDMI_DTS_TYPE1:
            case AU_HDMI_DTS_TYPE2:
            case AU_HDMI_DTS_TYPE3:
                if (HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE) == 1)
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB2_DTS);
                else
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB_DTS);

                break;
            case AU_HDMI_AC3:
            case AU_HDMI_AC3P:
                if (HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE) == 1)
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB2_AC3);
                else
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB_AC3);
                break;

            case AU_HDMI_AAC:
                if (HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE) == 1)
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB2_AAC);
                else
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB_MS10_DDT);
                break;
        };

     // Audio Input path config
        if (HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE) == 1)
        {
           // HDMI in 2nd Decoder
            if(HDMI_INPUT_PATH == Hdmi_Input_Sub)    //sub
        {
                HAL_AUDIO_SetInputPath(AUDIO_DSP3_HDMI_INPUT, AUDIO_PATH_5);
                HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_5,0x11);
            }
            else
            {
                HAL_AUDIO_SetInputPath(AUDIO_DSP3_HDMI_INPUT, AUDIO_PATH_MAIN);
                HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_MAIN,0x11);
            }
            //Marked because of AJ's patch
            //HAL_AUR2_WriteByte(REG_R2_DECODE2_CMD, AU_DEC_R2_CMD_STOP);       // Stop
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG,0x07,0x04);
            //MsOS_DelayTask(10);
            //HAL_AUR2_WriteByte(REG_R2_DECODE2_CMD, AU_DEC_R2_CMD_PLAY);       // Play
        }
        else // HDMI in 1st Decoder
        {
            if(HDMI_INPUT_PATH == Hdmi_Input_Sub)    //sub
            {
                HAL_AUDIO_SetInputPath(AUDIO_DSP1_HDMI_INPUT, AUDIO_PATH_5);
                HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_5,0x11);
            }
            else
            {
                HAL_AUDIO_SetInputPath(AUDIO_DSP1_HDMI_INPUT, AUDIO_PATH_MAIN);
                HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_MAIN,0x11);
            }

            //HAL_AUR2_WriteByte(REG_R2_DECODE1_CMD, AU_DEC_R2_CMD_STOP);       // Stop
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG,0x07,0x04);
            //MsOS_DelayTask(10);
            //HAL_AUR2_WriteByte(REG_R2_DECODE1_CMD, AU_DEC_R2_CMD_PLAY);       // Play
        }

   //========= Set for S/PDIF out ===========
        HAL_AUDIO_SPDIF_SetMode(_HAL_AUDIO_SPDIF_GetMode(), E_AUDIO_INFO_HDMI_IN);

         //Byte swap setting
        if (HAL_AUDIO_HDMI_DolbyMonitor() == AU_HDMI_AC3P)
        {
            HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_BYTESWAP, 1, 0);
        }
        else
        {
            HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_BYTESWAP, 0, 0);
        }
    }
    else  //====== HDMI pcm mode =======
    {
        if(HDMI_INPUT_PATH == Hdmi_Input_Sub)    //sub
        {
            HAL_AUDIO_SetInputPath(AUDIO_HDMI_INPUT, AUDIO_PATH_5);
            HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_5,0x19);
        }
        else
        {
            HAL_AUDIO_SetInputPath(AUDIO_HDMI_INPUT, AUDIO_PATH_MAIN);
            HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_MAIN,0x19);
        }

        HAL_AUDIO_SPDIF_SetMode(SPDIF_OUT_PCM, E_AUDIO_INFO_HDMI_IN);
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_AC3_PathCFG()  @@Need_Modify
/// @brief \b Function \b Description:  Set DVB/HDMI AC3 path Control
/// @param <IN>        \b u8Ctrl    :
///                                -- 0: DVB2_AD path
///                                -- 1: HDMI path
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMI_AC3_PathCFG(MS_U8 u8Ctrl)
{
    if (u8Ctrl == 1)
    {   // HDMI
        /* use CH1 decode HDMI AC3 */
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG,0x07,0x04);        // HDMI_AC3 REG CFG
    }
    else
    {   // DTV
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG,0x07,0x00);        // DVB1 REG CFG
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_Get_SYNTH_FREQ()  @@Need_Modify
/// @brief \b Function \b Description:  Audio HDMI CTS-N synthesizer input signal detect.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16  : return freq. If no signal, return 0
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////

MS_U16 HAL_AUDIO_HDMI_Get_SYNTH_FREQ(void)
{
    MS_U16 freq;

    freq = HAL_AUDIO_ReadByte(REG_AUDIO_STATUS_SYNTH) + 0x100 * HAL_AUDIO_ReadByte(REG_AUDIO_STATUS_SYNTH+1);

    if(freq & 0x8000)
      return 0;   //no signal

    return freq;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_Set_DwonSample()  @@Cathy
/// @brief \b Function \b Description:  set HDMI downsample rate
/// @param <IN>        \b MS_U8    :    donwsample ratio ; no need in T3
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////

void HAL_AUDIO_HDMI_Set_DwonSample(MS_U8 ratio)
{
    ratio=ratio; // No need in T3
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMIAutoMute()  @@Need_Modify
/// @brief \b Function \b This routine sets the automute function of HDMI.
/// @param <IN>        \b MS_U8    :     ch : 0~5
///                                                     value: register value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMIAutoMute(MS_U8 ch, MS_U8 value)
{
    if(ch < 4)
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_MUTE_CTRL1+(ch<<1),value);
    }
    else if(ch == 4)
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_MUTE_CTRL1+1,value);
    }
    else if(ch == 5)
    {
      HAL_AUDIO_WriteByte(REG_AUDIO_MUTE_CTRL2+1,value);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_Tx_GetStatus()
/// @brief \b Function \b Description:  This routine is used to get HDMI Tx status
/// @param onOff     \b : HDMI tx function is availible in this chip ?
/// @param hdmi_SmpFreq \b : Audio current sample freq for tx
/// @param outType \b   : PCM mode or nonPCM mode
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
{
        *onOff = FALSE;
        *hdmi_SmpFreq = AUDIO_FS_UNSUPPORT;
        *outType = HDMI_OUT_PCM;
}



///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO Internel Use Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_InitStep1()  @@Need_Modify
/// @brief \b Function \b Description: This routine is the initialization for Audio module(Step1).
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_InitStep1(void)
{
    HAL_AUDIO_WriteInitTable();
    HAL_AUDIO_SetPowerOn(1);

    HAL_AUDIO_SwResetMAD();

 // Cathy need remove
    HAL_AUDIO_WriteByte(REG_DEC_IDMA_CTRL0, 0x0002);
    AUDIO_DELAY1MS(2);
    HAL_AUDIO_WriteByte(REG_DEC_IDMA_CTRL0, 0x0003);

    HAL_AUDIO_WriteByte(REG_SE_IDMA_CTRL0, 0x0002);
    AUDIO_DELAY1MS(2);
    HAL_AUDIO_WriteByte(REG_SE_IDMA_CTRL0, 0x0003);
  // Reset Audio MIU Request for DEC-DSP
    HAL_MAD_DisEn_MIUREQ();

}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDspBaseAddr()
/// @brief \b Function \b Description:  This function is used to set bin file base address in flash for Audio module.
/// @param <IN>        \b u8Index    :
/// @param <IN>        \b Bin_Base_Address    :
/// @param <IN>        \b Mad_Base_Buffer_Adr    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_U32 u32Bin_Base_Address, MS_U32 u32Mad_Base_Buffer_Adr)
{
    HALAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_DSPMadMIUBank[u8Index] = (MS_U8) (u32Mad_Base_Buffer_Adr & 0x0000000F);
    u32Mad_Base_Buffer_Adr &= 0xFFFFFFF0;
    HALAUDIO_PRINT ("In %s, line(%d), u8Index(0x%x), u32Bin_Base_Address(0x%lx), u32Mad_Base_Buffer_Adr(0x%lx)\r\n", __FUNCTION__, __LINE__, u8Index, u32Bin_Base_Address, u32Mad_Base_Buffer_Adr);
    switch(u8Index)
    {
        case(DSP_ADV):   //DSP_ADV=R2=2
            g_AudioVars2->g_DSPBinBaseAddress[u8Index] = u32Bin_Base_Address;                                  //R2 ADDR setting
            g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index] = u32Mad_Base_Buffer_Adr;
            g_DSPMadMIUBank[u8Index] = g_AudioVars2->g_DSPMadMIUBank[u8Index];
            g_DSPBinBaseAddress[u8Index] = g_AudioVars2->g_DSPBinBaseAddress[u8Index];
            g_DSPMadBaseBufferAdr[u8Index] = g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index];

            g_AudioVars2->g_DSPBinBaseAddress[u8Index-1] = u32Bin_Base_Address;                                 //SE ADEC ADDR setting
            g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index-1] = u32Mad_Base_Buffer_Adr + (ADEC__R2_DDR_SIZE + ASND__R2_DDR_SIZE);
            g_DSPMadMIUBank[u8Index-1] = g_AudioVars2->g_DSPMadMIUBank[u8Index];
            g_DSPBinBaseAddress[u8Index-1] = g_AudioVars2->g_DSPBinBaseAddress[u8Index-1];
            g_DSPMadBaseBufferAdr[u8Index-1] = g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index-1];

            g_AudioVars2->g_DSPBinBaseAddress[u8Index-2] = u32Bin_Base_Address;                                    //Comm ADDR setting
            g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index-2] = g_DSPMadBaseBufferAdr[u8Index-1] + ASND_DSP_DDR_SIZE;
            g_DSPMadMIUBank[u8Index-2] = g_AudioVars2->g_DSPMadMIUBank[u8Index];
            g_DSPBinBaseAddress[u8Index-2] = g_AudioVars2->g_DSPBinBaseAddress[u8Index-2];
            g_DSPMadBaseBufferAdr[u8Index-2] = g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index-2];
            break;
        default:
            u8Index = u8Index;
            break;
     }

#ifndef MSOS_TYPE_NUTTX
    AUDIO_TEE_INFO_SHM_CHECK_NULL;
    REE_TO_TEE_MBX_MSG_SET_PARAM(0, u8Index);
    REE_TO_TEE_MBX_MSG_SET_PARAM(1, (MS_U8)(u32Bin_Base_Address&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(2, (MS_U8)((u32Bin_Base_Address>>8)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(3, (MS_U8)((u32Bin_Base_Address>>16)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(4, (MS_U8)((u32Bin_Base_Address>>24)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(5, (MS_U8)(u32Mad_Base_Buffer_Adr&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(6, (MS_U8)((u32Mad_Base_Buffer_Adr>>8)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(7, (MS_U8)((u32Mad_Base_Buffer_Adr>>16)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(8, (MS_U8)((u32Mad_Base_Buffer_Adr>>24)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM_COUNT(9);
    HAL_AUDIO_SendMBXMsg(REE_TO_TEE_MBX_MSG_HAL_AUDIO_SetDspBaseAddr | REE_TO_TEE_MBX_MSG_TYPE_GENERAL);
#endif
    HALAUDIO_PRINT ("In %s, line(%d)\r\n", __FUNCTION__, __LINE__);
    HALAUDIO_PRINT ("%d DSPBinBaseAddress = 0x%lx\n",   u8Index, g_AudioVars2->g_DSPBinBaseAddress[u8Index]);
    HALAUDIO_PRINT ("%d DSPMadBaseBufferAdr = 0x%lx\n", u8Index, g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index]);
    HALAUDIO_PRINT ("%d DSPMadMIUBank = %d\n",          u8Index, g_AudioVars2->g_DSPMadMIUBank[u8Index]);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDspMadBaseAddr()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b u8Index    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32: return the MAD base address
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index)
{
    HALAUDIO_CHECK_SHM_INIT;

    if (u8Index == DSP_DEC)  // MM case
    {
        return g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE];
    }
    return g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDspBinBaseAddr()
/// @brief \b Function \b Description:  This function is used to get the Bin file base address.
/// @param <IN>        \b u8Index    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32: return the Bin file base address
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_GetDspBinBaseAddr(MS_U8 u8Index)
{
    HALAUDIO_CHECK_SHM_INIT;

    return g_AudioVars2->g_DSPBinBaseAddress[u8Index];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDsp2CodeType()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the DSP2 code type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDsp2CodeType(MS_U8 u8Type, MS_BOOL bFlag)
{
    HALAUDIO_CHECK_SHM_INIT;

    if(((u8Type & 0xF0) == 0x40))
    {
        return;
    }

    if(bFlag == FALSE)
    {
        DBG_AUDIO_ERROR(printf("  [Audio] DSP2 Load code fail !!! SetDsp2CodeType Fail !!! \r\n"));
        return;
    }

    g_AudioVars2->g_u8Dsp2CodeType = u8Type;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDsp2CodeType()  @@Cathy
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AUDIO_DSP_CODE_TYPE HAL_AUDIO_GetDsp2CodeType(void)
{
    HALAUDIO_CHECK_SHM_INIT;

    return (AUDIO_DSP_CODE_TYPE)g_AudioVars2->g_u8Dsp2CodeType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDecR2_ADEC1_DecType()
/// @brief \b Function \b Description:  This function is used to set DEC R2 adec1 type.
/// @param <IN>        \b u8Type  : decoder type
/// @param <IN>        \b bFlag   : true: set codec type, false: do not set codec type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDecR2_ADEC1_DecType(MS_U8 u8Type, MS_BOOL bFlag)
{
    HALAUDIO_CHECK_SHM_INIT;

    if(bFlag == FALSE)
    {
        DBG_AUDIO_ERROR(printf(" %s fail !!!\r\n", __FUNCTION__));
        return;
    }

    g_AudioVars2->g_u8DecR2Adec1DecType = u8Type;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDecR2_ADEC1_DecType()
/// @brief \b Function \b Description:  This function is used to get DEC R2 adec1 type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AUDIO_DSP_CODE_TYPE HAL_AUDIO_GetDecR2_ADEC1_DecType(void)
{
    HALAUDIO_CHECK_SHM_INIT;

    return (AUDIO_DSP_CODE_TYPE)g_AudioVars2->g_u8DecR2Adec1DecType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDecR2_ADEC2_DecType()
/// @brief \b Function \b Description:  This function is used to set DEC R2 adec2 type.
/// @param <IN>        \b u8Type  : decoder type
/// @param <IN>        \b bFlag   : true: set codec type, false: do not set codec type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDecR2_ADEC2_DecType(MS_U8 u8Type, MS_BOOL bFlag)
{
    HALAUDIO_CHECK_SHM_INIT;

    if(bFlag == FALSE)
    {
        DBG_AUDIO_ERROR(printf(" %s fail !!!\r\n", __FUNCTION__));
        return;
    }

    g_AudioVars2->g_u8DecR2Adec2DecType = u8Type;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDecR2_ADEC2_DecType()
/// @brief \b Function \b Description:  This function is used to get DEC R2 adec2 type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AUDIO_DSP_CODE_TYPE HAL_AUDIO_GetDecR2_ADEC2_DecType(void)
{
    HALAUDIO_CHECK_SHM_INIT;

    return (AUDIO_DSP_CODE_TYPE)g_AudioVars2->g_u8DecR2Adec2DecType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDSPalive()
/// @brief \b Function \b Description:  This function is used to check if DSP alive.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP alive status.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_GetDSPalive(void)
{
    HALAUDIO_CHECK_SHM_INIT;

    return g_AudioVars2->g_u8DspAliveFlag;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDSPalive()
/// @brief \b Function \b Description:  This function is used to set flag after DSP alive.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDSPalive(MS_U8 alive)
{
    HALAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_u8DspAliveFlag = alive;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetAdvSndEffDspCodeType()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the DSP AdvSndEff code type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetAdvSndEffDspCodeType(MS_U8 u8Type, MS_BOOL bFlag)
{
    HALAUDIO_CHECK_SHM_INIT;

    if(bFlag == FALSE)
    {
        DBG_AUDIO_ERROR(printf("  [Audio] ADV_DSP Load code fail !!! SetAdvSndEffDspCodeType Fail !!! \r\n"));
        return;
    }

    g_AudioVars2->g_u8AdvSndEffDspCodeType = u8Type;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetAdvSndEffDspCodeType()  @@Cathy
/// @brief \b Function \b Description:  This function is used to get the DSP AdvSndEff code type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_GetAdvSndEffDspCodeType(void)
{
    HALAUDIO_CHECK_SHM_INIT;

    return g_AudioVars2->g_u8AdvSndEffDspCodeType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_BackupMailbox()    @@Need_Modify
/// @brief \b Function \b Description:  This function is used to backup SIF mailbox.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_BackupMailbox(void)
{
    HAL_SIF_BackupMailbox();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_RestoreMailbox()   @@Need_Modify
/// @brief \b Function \b Description:  This function is used to restore SIF mailbox.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_RestoreMailbox(void)
{
    HAL_SIF_RestoreMailbox();
    HAL_AUDIO_WriteMaskByte(0x2BCE, 0xFF, 0xFF); // Toggle 2BCE to update N.F value
    HAL_AUDIO_WriteMaskByte(0x2BCE, 0xFF, 0x00);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DmaReader_Init()
/// @brief \b Function \b Description: This routine is used to initilize DMA Reader.
/// @param <IN>        \b   eType   : sampleRate of PCM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DmaReader_Init(PCM_OUTPUT_FORMAT* pSettings)
{
    MS_U8 u8Path = pSettings->u8PathIdx;

    if (u8Path >= NUM_OF_HW_DMA)
    {
        HALAUDIO_ERROR("hw dma index out of bound\n");
        return;
    }

    stPcmSettings[u8Path].bLittleEndian = pSettings->bLittleEndian;
    stPcmSettings[u8Path].eFs = pSettings->eFs;
    stPcmSettings[u8Path].u8ByteWidth = pSettings->u8ByteWidth;
    stPcmSettings[u8Path].u8Channels = pSettings->u8Channels;
    stPcmSettings[u8Path].bMainChannel = pSettings->bMainChannel;
    stPcmSettings[u8Path].OMX_ArenderPathID = pSettings->OMX_ArenderPathID;
    stPcmSettings[u8Path].u8PathIdx = pSettings->u8PathIdx;

    MS_U32 audio_pcm_dmaRdr_base = 0;
    if (stPcmSettings[u8Path].u8PathIdx == 0)
    {
        audio_pcm_dmaRdr_base = HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC) + OFFSET_DMA_READER_DRAM_BASE + ASND_DSP_DDR_SIZE;  // kochien noted:  suggested by wilson, return value is SE header
    }
    else
    {
        audio_pcm_dmaRdr_base = HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC) + OFFSET_HW_DMA_READER2_DRAM_BASE + ASND_DSP_DDR_SIZE;  // kochien noted:  suggested by wilson, return value is SE header
    }

    MS_U16 synthrate, divisor;
//    unsigned char tmp1, tmp2, tmp3, tmp4;
    unsigned int sample_rate;

    /* New DMA Reader setting
     * Formula is :
     * Synthesizer Rate = 216MHz / Divisor(1, 2, 4 or 8) * 1024 / 256 / Sampling Rate(32Khz, 44.1KHz or 48KHz)
     */
    switch(stPcmSettings[u8Path].eFs)
    {
        case SAMPLE_RATE_96000:
            sample_rate = 96000;
            divisor = 0;
            synthrate = 0x2328;
            break;

        case SAMPLE_RATE_48000:
            sample_rate = 48000;
            divisor = 0;
            synthrate = 0x4650;
            break;

        case SAMPLE_RATE_44100:
            sample_rate = 44100;
            divisor = 0;
            synthrate = 0x4C87;
            break;

        case SAMPLE_RATE_32000:
            sample_rate = 32000;
            divisor = 0;
            synthrate = 0x6978;
            break;

        case SAMPLE_RATE_24000:
            sample_rate = 24000;
            divisor = 1;
            synthrate = 0x4650;
            break;

        case SAMPLE_RATE_22050:
            sample_rate = 22050;
            divisor = 1;
            synthrate = 0x4C87;
            break;

        case SAMPLE_RATE_16000:
            sample_rate = 16000;
            divisor = 1;
            synthrate = 0x6978;
            break;

        case SAMPLE_RATE_11025:
            sample_rate = 11025;
            divisor = 2;
            synthrate = 0x4C87;
            break;

        case SAMPLE_RATE_12000:
            sample_rate = 12000;
            divisor = 2;
            synthrate = 0x4650;
            break;

        case SAMPLE_RATE_8000:
            sample_rate = 8000;
            divisor = 2;
            synthrate = 0x6978;
            break;

        default:
            sample_rate = 48000;
            divisor = 0;
            synthrate = 0x4650;
            break;
    }

    /* configure DMA Reader's buffer size, if it's not aligned by "Line Size", re-configure it to be aligned */
    if (stPcmSettings[u8Path].u8PathIdx == 0)
    {
        audio_pcm_dmaRdr_bufSz = ((sample_rate * DMA_RDR_PCM_BUF_TIME) << 3) / 1000; //x8 = STEREO, 32bits, 1st HW's DMA Reader specification
    }
    else
    {
        audio_pcm_dmaRdr_bufSz = ((sample_rate * DMA_RDR_PCM_BUF_TIME) << 2) / 1000; //x4 = STEREO, 16bits, 2nd HW's DMA Reader specification
    }

    if ((audio_pcm_dmaRdr_bufSz % DMA_RDR_BYTES_IN_LINE) > 0)
    {
        audio_pcm_dmaRdr_bufSz += (DMA_RDR_BYTES_IN_LINE - (audio_pcm_dmaRdr_bufSz % DMA_RDR_BYTES_IN_LINE));
    }

    /* init DMA writer address */
    RingBuffer_DmaReader[u8Path].BufferSize = audio_pcm_dmaRdr_bufSz;
    RingBuffer_DmaReader[u8Path].StartAddress = (MS_U8 *)MS_PA2KSEG1(audio_pcm_dmaRdr_base);
    RingBuffer_DmaReader[u8Path].EndAddress = (MS_U8 *)MS_PA2KSEG1(audio_pcm_dmaRdr_base + RingBuffer_DmaReader[u8Path].BufferSize);
    RingBuffer_DmaReader[u8Path].WritePointer = RingBuffer_DmaReader[u8Path].StartAddress;

    if (stPcmSettings[u8Path].u8PathIdx == 0)
    {
        g_OMX_ArenderPathID = AUDIO_OMX_ARENDER_PATH_ID_NULL; //[LM14A]

        /* initial DMA Reader path & clk select */          //DMA reader -> CH8
        HAL_AUDIO_WriteMaskByte(0x2CB0, 0x07, 0x04);        //SEL_CLK_DMA_READER

        HAL_AUDIO_WriteMaskReg(0x2B82, 0xFFFF, 0x0000);     //reset dma reader

        HAL_AUDIO_WriteMaskByte(0x2B8E, 0xFF, ((audio_pcm_dmaRdr_base/DMA_RDR_BYTES_IN_LINE) & 0xFF));          //DMA Reader Base Addr[7:0]
        HAL_AUDIO_WriteMaskReg(0x2B84, 0xFFFF,(((audio_pcm_dmaRdr_base/DMA_RDR_BYTES_IN_LINE)>>8) & 0xFFFF));   //DMA Reader Base Addr[23:8]
        HAL_AUDIO_WriteMaskByte(0x2B8F, 0x0F, (((audio_pcm_dmaRdr_base/DMA_RDR_BYTES_IN_LINE)>>24) & 0x0F));    //DMA Reader Base Addr[27:24]

        HAL_AUDIO_WriteMaskReg(0x2B86, 0xFFFF, audio_pcm_dmaRdr_bufSz/DMA_RDR_BYTES_IN_LINE);  //setting : DMA Reader Size
        HAL_AUDIO_WriteMaskReg(0x2B88, 0xFFFF, audio_pcm_dmaRdr_bufSz - 12);     //setting : DMA Reader Overrun Thr
        HAL_AUDIO_WriteMaskReg(0x2B8A, 0xFFFF, 0x00012);                         //setting : DMA Reader Underrun Thr

        // Decide Path
        HAL_AUDIO_WriteMaskByte(0x2C6B, 0xFF, 0x8F );       //CH8 sel to DMA Rdr

        /* synthersizer setting update */                   //DMA reader
        HAL_AUDIO_WriteMaskByte(0x2CA4, 0x20, 0x20);        //enable DMA synthesizer
        HAL_AUDIO_WriteMaskReg(0x2CB0, 0x6000, (divisor << 13));  //Configure REG_SEL_CLK_214MHZ_DMA_RD_FIX_SYNTH
        HAL_AUDIO_WriteMaskReg(0x2C20, 0xFFFF, synthrate);  //DMA synthesizer N.F.

        /* reset and Start DMA Reader */
        HAL_AUDIO_WriteMaskReg(0x2B80, 0xFFFF, 0x6402);
#ifdef MSOS_TYPE_LINUX_KERNEL
        mdelay(1);
#else
        AUDIO_DELAY1MS(1);
#endif
        HAL_AUDIO_WriteMaskReg(0x2B80, 0xFFFF, 0x2402);
    }
    else if (stPcmSettings[u8Path].u8PathIdx == 1)
    {
        HAL_AUR2_WriteReg(NewDMAreader_ctrl, 0x0);
        HAL_AUR2_WriteReg(NewDMAreader_BaseAddress_Lo, (audio_pcm_dmaRdr_base/DMA_RDR_BYTES_IN_LINE) & 0xFFFF);
        HAL_AUR2_WriteReg(NewDMAreader_BaseAddress_Hi, (audio_pcm_dmaRdr_base/DMA_RDR_BYTES_IN_LINE>>16) & 0xFFFF);

        HAL_AUR2_WriteReg(NewDMAreader_DRAM_size, audio_pcm_dmaRdr_bufSz/DMA_RDR_BYTES_IN_LINE);

        HAL_AUR2_WriteMaskReg(NewDMAreader_DRAM_underrun_threshold, 0xFFFF, 0x00012);                         //setting : DMA Reader Underrun Thr
        /* synthersizer setting update */                   //DMA reader
        HAL_AUR2_WriteMaskReg(NewDMAreader_Enable_ctrl, 0x0030, divisor << 4); //Set divisor
        HAL_AUR2_WriteReg(NewDMAreader_Syth, synthrate);  //DMA synthesizer N.F.

        HAL_AUR2_WriteMaskReg(NewDMAreader_Enable_ctrl, 0x145, 0x145); //enable DMA synthesizer

        // Decide Path
        if (stPcmSettings[u8Path].OMX_ArenderPathID == 0)
        {
            g_OMX_ArenderPathID = AUDIO_OMX_ARENDER_PATH_ID_CH5; //[LM14A]
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH5_CFG, 0xFF,0x90); // CH5
        }
        else
        {
            g_OMX_ArenderPathID = AUDIO_OMX_ARENDER_PATH_ID_CH6; //[LM14A]
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0xFF,0x90); // CH6
        }
        /* Reset and Start DMA Reader */
        HAL_AUR2_WriteMaskReg(NewDMAreader_ctrl, 0xFFFF, 0x800B); //SW reset & enable DMA reader, MIU request
#ifdef MSOS_TYPE_LINUX_KERNEL
        mdelay(1);
#else
        AUDIO_DELAY1MS(1);
#endif
        HAL_AUR2_WriteMaskReg(NewDMAreader_ctrl, 0xFFFF, 0x000B); //SW reset & enable DMA reader, MIU request

        /* Clear DMA Reader buffer */

    }

    if (RingBuffer_DmaReader[u8Path].StartAddress != NULL)
    {
        memset((void *)RingBuffer_DmaReader[u8Path].StartAddress, 0x00, RingBuffer_DmaReader[u8Path].BufferSize);
        MsOS_FlushMemory();
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DmaReader_Init()
/// @brief \b Function \b Description: This routine is used to initilize DMA Reader.
/// @param <IN>        \b   eType   : sampleRate of PCM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DmaReader_AllInput_Init(void)
{
    HAL_AUDIO_DmaReader_Init(SAMPLE_RATE_48000);
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DmaReader_GetLevel()
/// @brief \b Function \b Description: This routine is used to get PCM data level in HW DMA reader
/// @param <IN>        \b   eType   : DMA reader index
/// @param <OUT>     \b   MS_U8   : DMA reader PCM data level
/// @param <RET>      \b   NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_DmaReader_GetLevel(MS_U8 u8Idx)
{
    MS_U32 u32Level = 0;

    if(u8Idx >= NUM_OF_HW_DMA)
    {
        return u32Level;
    }

    if(u8Idx == 0)
    {
        HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0010);
        u32Level = HAL_AUDIO_ReadReg(0x2B8C);
        HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0000);
    }
    else
    {
        HAL_AUR2_WriteMaskReg(NewDMAreader_ctrl, 0x0020, 0x0020);
        u32Level =  HAL_AUR2_ReadReg(NewDMAreader_DRAM_levelcnt);
        HAL_AUR2_WriteMaskReg(NewDMAreader_ctrl, 0x0020, 0x0000);
    }
    return u32Level;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DmaReader_Reset()
/// @brief \b Function \b Description: This routine is used to reset DMA reader
/// @param <IN>        \b eType   : DMA reader index
/// @param <OUT>     \b MS_U8   : DMA reader reset flag
/// @param <RET>      \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_DmaReader_Reset(MS_U8 u8Idx)
{
    MS_U32 replay_flag = 0;

    if(u8Idx >= NUM_OF_HW_DMA)
    {
        return replay_flag;
    }

    if(u8Idx == 0)
    {
        if ((HAL_AUDIO_ReadReg(0x2B80) & 0x0001) == 0x0001)
        {
            replay_flag = 1;
        }

        /* clean dma reader buffer */
        if (RingBuffer_DmaReader[u8Idx].StartAddress!= NULL)
        {
            memset((void *)RingBuffer_DmaReader[u8Idx].StartAddress, 0x00, RingBuffer_DmaReader[u8Idx].BufferSize);
            MsOS_FlushMemory();
        }

        /* reset Write Pointer */
        RingBuffer_DmaReader[u8Idx].WritePointer = RingBuffer_DmaReader[u8Idx].StartAddress;

        /* reset dma reader */
        HAL_AUDIO_WriteMaskReg(0x2B82, 0xFFFF, 0x0000);    //clear wr cntrs

        /* reset and start DMA Reader */
        HAL_AUDIO_WriteMaskReg(0x2B80, 0xFFFE, 0x6402);
#ifdef MSOS_TYPE_LINUX_KERNEL
        mdelay(1);
#else
        AUDIO_DELAY1MS(1);
#endif
        HAL_AUDIO_WriteMaskReg(0x2B80, 0xFFFE, 0x2402);
    }
    else
    {
        // Need check 2nd HW DMA reader under flow bit
        if ((HAL_AUR2_ReadReg(NewDMAreader_DRAM_flag) & 0x0002) == 0x0002)
        {
            replay_flag = 1;
        }

        /* clean dma reader buffer */
        if (RingBuffer_DmaReader[u8Idx].StartAddress != NULL)
        {
            memset((void *)RingBuffer_DmaReader[u8Idx].StartAddress, 0x00, RingBuffer_DmaReader[u8Idx].BufferSize);
            MsOS_FlushMemory();
        }

        /* reset Write Pointer */
        RingBuffer_DmaReader[u8Idx].WritePointer = RingBuffer_DmaReader[u8Idx].StartAddress;

        HAL_AUR2_WriteMaskReg(NewDMAreader_CPU_triggersize, 0xFFFF, 0x0000); // Clear Wiite counter

        HAL_AUR2_WriteMaskReg(NewDMAreader_ctrl, 0xFFFF, 0x800B); //SW reset & enable DMA reader, MIU request
#ifdef MSOS_TYPE_LINUX_KERNEL
        mdelay(1);
#else
        AUDIO_DELAY1MS(1);
#endif
        HAL_AUR2_WriteMaskReg(NewDMAreader_ctrl, 0xFFFF, 0x000B); //SW reset & enable DMA reader, MIU request
    }
    return replay_flag;
}

void HAL_AUDIO_Set_DMA_Buffer(MS_U32 ms)
{
    /* remain in buffer.1: 80ms, 2: 60ms, 3: 40ms, 4: 20ms */
    DMA_Buffer_limit = ms;
    printf("DMA_Buffer_limit:%x\r\n", (unsigned int)DMA_Buffer_limit);
}

#if HAL_AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
#define HAL_AUDIO_FILE_PATH           "/tmp/halAUDIO.log"
#define HAL_AUDIO_DUMP_PCM_FILE_PATH  "/tmp/dump.pcm"

struct file *hal_audio_filp = NULL;
struct file *dump_pcm_filp = NULL;

char hal_str[128];

int hal_audio_write_file(struct file *filp, char *filename, void *data, int count)
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

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DmaReader_WritePCM()
/// @brief \b Function \b Description: This routine is used to write PCM data into DMA reader Buffer
/// @param <IN>        \b   eType   : buffer bytes
/// @param <OUT>       \b NONE    : TRUE or FALSE
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_DmaReader_WritePCM(MS_U8 u8Idx, void* buffer, MS_U32 bytes)
{

    if(u8Idx >= NUM_OF_HW_DMA)
    {
        return TRUE;
    }

    unsigned char *bufptr = (unsigned char *)buffer; //MS_PA2KSEG1((MS_U32)buffer);
    unsigned char tmp1, tmp2, tmp3, tmp4, replay_flag=0;
    unsigned int i, level_cnt;
    static MS_U32 u32Cnt = 0;
    u32Cnt++;

//    printf("\r\n === HAL_AUDIO_DmaReader_WritePCM,  size(0x%x) === \r\n", (unsigned int)bytes);

    // Mask LEVEL_CNT_MASK before read

    level_cnt = HAL_AUDIO_DmaReader_GetLevel(u8Idx);

    if((u32Cnt > 500)&&(u32Cnt%500 == 0))
    {
        //HALAUDIO_ERROR("\r\n  === (0x%x),  (0x%x), (0x%x), (0x%x), level (0x%x) === \r\n", (unsigned int)(*bufptr), (unsigned int)(*(bufptr+1)), (unsigned int)(*(bufptr+2)), (unsigned int)(*(bufptr+3)), (unsigned int)level_cnt);
    }

    if ( level_cnt <= 6 )
    {
        replay_flag = HAL_AUDIO_DmaReader_Reset(u8Idx);
        //HALAUDIO_ERROR( "\r\n\n***** DMAReader(%d) Buffer empty !! ***** \r\n\n", u8Idx);

#if HAL_AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
        sprintf(hal_str, "[%010lu] ***** DMAReader(%d) Buffer empty !! ***** \n", (long unsigned int)jiffies_to_msecs(jiffies), (int)u8Idx);
        hal_audio_write_file(hal_audio_filp, HAL_AUDIO_FILE_PATH, hal_str, strlen(hal_str));
#endif
#endif
    }

    level_cnt = HAL_AUDIO_DmaReader_GetLevel(u8Idx);

        if (u8Idx == 0)
        {
            // Following code assume input data is 2ch and 2byte per sample
            if(level_cnt < (RingBuffer_DmaReader[u8Idx].BufferSize - bytes*2*DMA_Buffer_limit)/DMA_RDR_BYTES_IN_LINE)
            {
#if HAL_AUDIO_MIXER_THREAD_DEBUG
#ifdef MSOS_TYPE_LINUX_KERNEL
                sprintf(hal_str, "[%010lu] write DMA time , size:%d\n", (long unsigned int)jiffies_to_msecs(jiffies), (int)bytes);
                hal_audio_write_file(hal_audio_filp, HAL_AUDIO_FILE_PATH, hal_str, strlen(hal_str));
                hal_audio_write_file(dump_pcm_filp, HAL_AUDIO_DUMP_PCM_FILE_PATH, buffer, bytes);
#endif
#endif

                for ( i = 0; i < (bytes/2); i++ )
                {
                    tmp1 = 0;
                    tmp2 = *bufptr++;
                    tmp3 = *bufptr++;
                    tmp4 = 0;

                    *(RingBuffer_DmaReader[u8Idx].WritePointer)++ = tmp1;
                    *(RingBuffer_DmaReader[u8Idx].WritePointer)++ = tmp2;
                    *(RingBuffer_DmaReader[u8Idx].WritePointer)++ = tmp3;
                    *(RingBuffer_DmaReader[u8Idx].WritePointer)++ = tmp4;

                    if ( RingBuffer_DmaReader[u8Idx].WritePointer >= RingBuffer_DmaReader[u8Idx].EndAddress)
                    {
                        RingBuffer_DmaReader[u8Idx].WritePointer = RingBuffer_DmaReader[u8Idx].StartAddress;
                    }
                }

                /* flush memory */
                MsOS_FlushMemory();

                HAL_AUDIO_WriteMaskReg(0x2B82, 0xFFFF, bytes*2/DMA_RDR_BYTES_IN_LINE);
#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(5);
#else
                AUDIO_DELAY1US(5);
#endif

                if (replay_flag == 1)
                {
                    HAL_AUDIO_WriteMaskReg(0x2B80, 0x0001, 0x0001);
#ifdef MSOS_TYPE_LINUX_KERNEL
                    udelay(50);
#else
                    AUDIO_DELAY1US(50);
#endif
                    HAL_AUDIO_WriteMaskReg(0x2B80, 0x0100, 0x0100);
                }

                return TRUE;
            }
        }
        else if (u8Idx == 1)
        {
            if(level_cnt < (RingBuffer_DmaReader[u8Idx].BufferSize - bytes*DMA_Buffer_limit)/DMA_RDR_BYTES_IN_LINE)
            {
                for ( i = 0; i < (bytes/4); i++ ) // write 2ch 16bit sample each time
                {
                    tmp1 = *bufptr++;
                    tmp2 = *bufptr++;
                    tmp3 = *bufptr++;
                    tmp4 = *bufptr++;

                    *(RingBuffer_DmaReader[u8Idx].WritePointer)++ = tmp1;
                    *(RingBuffer_DmaReader[u8Idx].WritePointer)++ = tmp2;
                    *(RingBuffer_DmaReader[u8Idx].WritePointer)++ = tmp3;
                    *(RingBuffer_DmaReader[u8Idx].WritePointer)++ = tmp4;

                    if ( RingBuffer_DmaReader[u8Idx].WritePointer >= RingBuffer_DmaReader[u8Idx].EndAddress)
                    {
                        RingBuffer_DmaReader[u8Idx].WritePointer = RingBuffer_DmaReader[u8Idx].StartAddress;
                    }
                }
                /* flush memory */
                MsOS_FlushMemory();

                HAL_AUR2_WriteMaskReg(NewDMAreader_ctrl, 0x0010, 0x0000);
                HAL_AUR2_WriteMaskReg(NewDMAreader_CPU_triggersize, 0xFFFF, bytes/DMA_RDR_BYTES_IN_LINE);

#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(5);
#else
                AUDIO_DELAY1US(5);
#endif
                HAL_AUR2_WriteMaskReg(NewDMAreader_ctrl, 0x0010, 0x0010);

                //seems not necessary to reset again in 2nd HW DMA
                if (replay_flag == 1)
                {

                }

                return TRUE;
            }
        }

        return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetEntertainmentMode()
/// @brief \b Function \b Description: This routine is used to switch the DTV/KTV/GAME mode
/// @param <IN>        \b   eMode   :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetEntertainmentMode(AUDIO_ENTERTAINMENT_MODE eMode)
{
    switch((int)eMode)
    {
        case AUDIO_ETMODE_DTV :
            //HAL_AUDIO_SetInputPath(AUDIO_DSP2_DVB_INPUT , AUDIO_PATH_5);
            HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2D47 , 0x80, 0x00);   // disable KTV function
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x00);   // ADC0 gain   0dB
            HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_PCM);  // Fix PCM in
            //HAL_AUDIO_WriteMaskReg(REG_AUR2_ADVSND_SEL, 0xFF00, 0x0000); // Switch R2 to normal mode
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_STEREO, 0);  // Switch SE-DSP sound mode to stereo
            break;

        case AUDIO_ETMODE_KTV :
            //HAL_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT , AUDIO_PATH_5);
            HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x08);
            HAL_AUDIO_WriteMaskByte(0x2D47 , 0x80, 0x80);   // enable KTV function
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x80);   // ADC0 gain    6dB
            HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_MIXER); // MIC + PCM
            HAL_AUR2_WriteMaskReg(REG_SNDR2_ADVSND_SEL, 0xFF00, 0x1E00);  // Switch R2 to KTV mode
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_MIXED_LR, 0);  // Switch SE-DSP sound mode to L+R for MIC0=L_CH, MIC1=R_CH
            break;

        case AUDIO_ETMODE_KTV2 :  // Use I2S input ; not MicroPhone in
            //HAL_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT , AUDIO_PATH_5);
            HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x08);
            HAL_AUDIO_WriteMaskByte(0x2D47 , 0xF0, 0xA0);   // enable KTV mode; Skyworth (stone) model
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x40);   // ADC0 gain   -6dB
            HAL_AUDIO_WriteMaskByte(0x2D5D , 0x80, 0x80);   // Mute MIC volume (only mix MP3 & PCM DMA data)
            HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_MIXER); // MIC + PCM
            //HAL_AUDIO_WriteMaskReg(REG_AUR2_ADVSND_SEL, 0xFF00, 0x0000);  // Switch R2 to normal mode
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_MIXED_LR, 0);  // Switch SE-DSP sound mode to L+R for MIC0=L_CH, MIC1=R_CH
            break;

        case AUDIO_ETMODE_GAME :
            //HAL_AUDIO_SetInputPath(AUDIO_DSP2_DVB_INPUT , AUDIO_PATH_5);
            HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x10);
            HAL_AUDIO_WriteMaskByte(0x2D47 , 0x80, 0x00);   // disable KTV function
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x40);   // ADC0 gain   -6dB
            HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_PCM);   // Fix PCM in
            //HAL_AUDIO_WriteMaskReg(REG_AUR2_ADVSND_SEL, 0xFF00, 0x0000);   // Switch R2 to normal mode
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_STEREO, 0);  // Switch SE-DSP sound mode to stereo
            break;

        case AUDIO_ETMODE_VOIP_ON:
            //Voip will setup snd system to GAME MODE,
            //In GAME MODE, Voip used 0x2D46[7:0] to
            //upload Raw_Delay_SE to DRAM.
            HAL_AUDIO_WriteMaskByte(0x2D46, 0xFF, 0xA1);

            HAL_AUDIO_WriteMaskByte(0x2C9C, 0xFF, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2C9E, 0xFF, 0xE0);
            HAL_AUDIO_WriteMaskByte(0x2C9F, 0xFF, 0xC4);

            HAL_AUDIO_WriteMaskByte(0x2C9D, 0x40, 0x40);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2C9D, 0x40, 0x00);
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_STEREO, 0);  // Switch SE-DSP sound mode to stereo

#if 0
            //config SRC to 8KHz
            HAL_AUDIO_WriteMaskByte(0x2BBC, 0xFF, 0xF0);//0xE0},  // Group A SRC rate
            HAL_AUDIO_WriteMaskByte(0x2BBD, 0xFF, 0x03);//0x07},
            HAL_AUDIO_WriteMaskByte(0x2BBE, 0xFF, 0xF0);//0x50},
            HAL_AUDIO_WriteMaskByte(0x2BBF, 0xFF, 0xD2);//0x46},
            HAL_AUDIO_WriteMaskByte(0x2BC0, 0xFF, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2BC1, 0xFF, 0x04);//0x08},
            HAL_AUDIO_WriteMaskByte(0x2BC2, 0xFF, 0xF0);//0x50},
            HAL_AUDIO_WriteMaskByte(0x2BC3, 0xFF, 0xD2);//0x46},
            //update SYNTH
            //HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x40);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x00);
            //reset GroupA SRC
            //HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x80);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x00);
#endif

            HALAUDIO_PRINT("HAL_AUDIO_SetEntertainmentMode: AUDIO_ETMODE_VOIP_ON\r\n");
            break;

        case AUDIO_ETMODE_VOIP_OFF:
            //disable upload Raw_Delay_SE to DRAM.
            HAL_AUDIO_WriteMaskByte(0x2D46, 0xFF, 0x00);

            HAL_AUDIO_WriteMaskByte(0x2C9C, 0xFF, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2C9E, 0xFF, 0xD0);
            HAL_AUDIO_WriteMaskByte(0x2C9F, 0xFF, 0x20);

            HAL_AUDIO_WriteMaskByte(0x2C9D, 0x40, 0x40);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2C9D, 0x40, 0x00);
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_STEREO, 0);  // Switch SE-DSP sound mode to stereo

#if 0
            //set SRC back to 48KHz
            HAL_AUDIO_WriteMaskByte(0x2BBC, 0xFF, 0xE0);  // Group A SRC rate
            HAL_AUDIO_WriteMaskByte(0x2BBD, 0xFF, 0x07);
            HAL_AUDIO_WriteMaskByte(0x2BBE, 0xFF, 0x50);
            HAL_AUDIO_WriteMaskByte(0x2BBF, 0xFF, 0x46);
            HAL_AUDIO_WriteMaskByte(0x2BC0, 0xFF, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2BC1, 0xFF, 0x08);
            HAL_AUDIO_WriteMaskByte(0x2BC2, 0xFF, 0x50);
            HAL_AUDIO_WriteMaskByte(0x2BC3, 0xFF, 0x46);
            //update SYNTH
            //HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x40);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x00);
            //reset GroupA SRC
            //HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x80);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x00);
#endif
            HALAUDIO_PRINT("HAL_AUDIO_SetEntertainmentMode: AUDIO_ETMODE_VOIP_OFF\r\n");
            break;

        default :
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetSystem()
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
MS_BOOL HAL_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
{
    MS_BOOL bRet = FALSE;

    HAL_AUDIO_SetIsDtvFlag(TRUE);
    HAL_MAD_SetCommInfo(Audio_Comm_infoType_setBypassSPDIF_PAPB_chk, FALSE, 0);

    // set decoder system
    switch (enDecSystem)
    {
            ////////////////////////////////////////////////
            //      Audio Decoder 1
            ////////////////////////////////////////////////
            case MSAPI_AUD_DVB_MPEG:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_mpeg);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_MPEG, TRUE);
                break;

            case MSAPI_AUD_DVB_AC3:
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x08, 0x00);  // select ADEC DSP NPCM
                AUDIO_DELAY1MS(1);
                HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_ac3);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_AC3, TRUE);
                break;

            case MSAPI_AUD_DVB_AC3P:
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x08, 0x00);  // select ADEC DSP NPCM
                AUDIO_DELAY1MS(1);
                HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_ac3);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_AC3P, TRUE);
                break;

            case MSAPI_AUD_DVB_MP3:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_mpeg);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_MP3, TRUE);
                break;

            case MSAPI_AUD_DVB_AAC:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_aac);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_AAC, TRUE);
                break;

            case MSAPI_AUD_DVB_XPCM:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_xpcm);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_XPCM, TRUE);
                break;

            case MSAPI_AUD_DVB_RA8LBR:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_ra8);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_RA8LBR, TRUE);
                break;

            case MSAPI_AUD_DVB_WMA:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_wma);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_WMA, TRUE);
                break;

            case MSAPI_AUD_DVB_DTS:
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x08, 0x00);  // select ADEC DSP NPCM
                AUDIO_DELAY1MS(1);
                HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
                HAL_MAD_SetCommInfo(Audio_Comm_infoType_setBypassSPDIF_PAPB_chk, TRUE, 0);
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_dts);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_DTS, TRUE);
                break;

            case MSAPI_AUD_DVB_DTSLBR:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_dts);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_DTSLBR, TRUE);
                break;

            case MSAPI_AUD_DVB_MS10_DDT:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_aac);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_MS10_DDT, TRUE);
                break;

            case MSAPI_AUD_DVB_MS10_DDC:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_ac3);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_MS10_DDC, TRUE);
                break;

            case MSAPI_AUD_DVB_WMA_PRO:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_wma);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_WMA_PRO, TRUE);
                break;

            case MSAPI_AUD_DVB_FLAC:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_flac);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_FLAC, TRUE);
                break;

            case MSAPI_AUD_DVB_VORBIS:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_vorbis);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_VORBIS, TRUE);
                break;

            case MSAPI_AUD_DVB_AMR_NB:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_amrnb);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_INVALID, TRUE);
                break;

            case MSAPI_AUD_DVB_AMR_WB:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_amrwb);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_INVALID, TRUE);
                break;

            case MSAPI_AUD_DVB_DRA:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_dra);
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_DRA, TRUE);
                break;

            case MSAPI_AUD_DVB_DolbyTrueHDBypass:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_dolby_truhd_bypass);  /*  select r2 decoder to olby TrueHD bypss */
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_DolbyTrueHD_Bypass,TRUE); //set decoder type for HDMI Tx handler
                break;

            ////////////////////////////////////////////////
            //      Audio Decoder 2
            ////////////////////////////////////////////////
            case MSAPI_AUD_DVB2_MPEG:
            case MSAPI_AUD_DVB2_MP3:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_mpeg); // MP3 & MPEG use the same decoder
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_MP3, TRUE);
                break;

            case MSAPI_AUD_DVB2_AC3:
            case MSAPI_AUD_DVB2_AC3P:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_ac3);
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_AC3, TRUE);
                break;

            case MSAPI_AUD_DVB2_AAC:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_aac);
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_AAC, TRUE);
                break;

            case MSAPI_AUD_DVB2_GAAC:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_gaac);
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_AAC, TRUE);
                break;

            case MSAPI_AUD_DVB2_DTS:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_dts);
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_DTS, TRUE);
                break;

            case MSAPI_AUD_DVB2_XPCM:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_xpcm);
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_XPCM, TRUE);
                break;

             case MSAPI_AUD_DVB2_DolbyTrueHDBypass:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_dolby_truhd_bypass);
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_DolbyTrueHD_Bypass, TRUE);
                break;

            default:
            HAL_AUDIO_SetIsDtvFlag(FALSE);
            bRet = TRUE;
                break;
    }
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDecSysSupportStatus()
/// @brief \b Function \b Description: This function will report Audio Decoder support feature status when giving DecSystem type
/// @param <IN>        \b   enDecSystem : Decoder type
/// @param <RET>       \b   BOOL        : True or False
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
{
        MS_BOOL SetDecSys_RtnStatus = FALSE;
        switch (enDecSystem)
        {
            ////////////////////////////////////////////////
            //      Audio Decoder 1
            ////////////////////////////////////////////////
            case MSAPI_AUD_DVB_MPEG:
            case MSAPI_AUD_DVB_AC3:
            case MSAPI_AUD_DVB_AC3P:
            case MSAPI_AUD_DVB_MP3:
            case MSAPI_AUD_DVB_AAC:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_MS10_DDT:
            case MSAPI_AUD_DVB_MS10_DDC:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_XPCM:
            case MSAPI_AUD_DVB_RA8LBR:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_WMA:
            case MSAPI_AUD_DVB_WMA_PRO:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_DTS:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_FLAC:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_VORBIS:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_AMR_NB:
                SetDecSys_RtnStatus = FALSE;
                break;

            case MSAPI_AUD_DVB_AMR_WB:
                SetDecSys_RtnStatus = FALSE;
                break;

            ////////////////////////////////////////////////
            //      Audio Decoder 2
            ////////////////////////////////////////////////
            case MSAPI_AUD_DVB2_MPEG:
            case MSAPI_AUD_DVB2_MP3:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB2_AC3:
            case MSAPI_AUD_DVB2_AC3P:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB2_AAC:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB2_DDE:
                SetDecSys_RtnStatus = FALSE;
                break;

            case MSAPI_AUD_DVB2_DTSE:
                SetDecSys_RtnStatus = FALSE;
                break;

            case MSAPI_AUD_DVB2_XPCM:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB2_KTV:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_TONE:
            case MSAPI_AUD_DVB_NONE:
            case MSAPI_AUD_DVB2_NONE:
            default:
                SetDecSys_RtnStatus = FALSE;
                break;
        }
        return(SetDecSys_RtnStatus);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_AlgReloadCode()
/// @brief \b Function \b Description: This routine is used to reload DSP code
/// @param u8Type      \b :
/// @return MS_BOOL    \b : TRUE --DEC-DSP reload code okay
///                         FALSE--DEC-DSP reload code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_AlgReloadCode(MS_U8 u8Type)
{
    // No DEC-DSP in Monaco
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ALSA_Check()
/// @brief \b Function \b Description:  Check if ALSA Interface is supported
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if it's supported, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_ALSA_Check(void)
{
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ALSA_Enable()
/// @brief \b Function \b Description:  Enable/ Disable the path of ALSA
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if ok, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_ALSA_Enable(MS_BOOL bEnable)
{
    if (bEnable == TRUE)
    {
        /* Set as GAME mode */
        HAL_AUDIO_SetInputPath(AUDIO_DSP3_DVB_INPUT , AUDIO_PATH_5);
        HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x10);
        HAL_AUDIO_WriteMaskByte(0x2D47 , 0x80, 0x00);   // disable KTV function
        HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x40);   // ADC0 gain   -6dB
        HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_PCM);  // Fix PCM in

        return TRUE;
    }
    else
    {
        /* Do nothing for the moment */
        return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Alg2ReloadCode()
/// @brief \b Function \b Description: This routine is used to reload the algorithm in se-dsp .
/// @param u8Type      \b : information of algorithm code
/// @return MS_BOOL    \b : TRUE --DEC-DSP reload code okay
///                         FALSE--DEC-DSP reload code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_Alg2ReloadCode(MS_U8 u8Type)
{
    MS_U16 time_out;
    MS_BOOL ret_status = FALSE;

    HAL_MAD2_SetDspIDMA();
    // Enter MCU/DSP hand-shake
    if(((u8Type & 0xF0) == 0x40))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ADVSND_BEG/0x100);
    }
    else if (IS_AUDIO_DSP_CODE_TYPE_SIF(u8Type))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_SIF_BEG/0x100);
    }
    else if (IS_AUDIO_DSP_CODE_TYPE_SE_ENC(u8Type))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ENC_BEG/0x100);
    }
    else
    {
        //No Decoder in DSP
        DBG_AUDIO("Warning! un-support DSP version decoder.\n");
        return FALSE;
    }
    // PIO[8] interrupt
    HAL_MAD2_TriggerPIO8();

    //Wait Dsp Start reload Ack
    time_out = 0;
    while(time_out++<2000)
    {
        if(HAL_MAD2_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK1)
            break;
        AUDIO_DELAY1MS(1);
    }
    if(time_out>=2000)
    {
        DBG_AUDIO_ERROR("  DSP Reload timeOut1: %d\r\n",time_out);
        return FALSE;
    }

    // Change to IDMA Port
    HAL_MAD2_SetDspIDMA();

    // Start to Reload DSP code
    ret_status = HAL_AUDSP_DspLoadCode(u8Type);

    // Enter MCU/DSP hand-shake
    if(((u8Type & 0xF0) == 0x40))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ADVSND_END/0x100);
    }
    else if (IS_AUDIO_DSP_CODE_TYPE_SIF(u8Type))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_SIF_END/0x100);
    }
    else if (IS_AUDIO_DSP_CODE_TYPE_SE_ENC(u8Type))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ENC_END/0x100);
    }
    else
    {
        //No Decoder in DSP
        DBG_AUDIO("Warning! un-support DSP version decoder.\n");
        return FALSE;
    }
    HAL_MAD2_TriggerPIO8();

    // Wait Dsp End Reload Ack
    time_out = 0;
    while(time_out++<3000)
    {
        if(HAL_MAD2_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK2)
            break;
        AUDIO_DELAY1MS(1);
    }

    if(time_out>=3000)
    {
        DBG_AUDIO_ERROR("  DSP Reload timeOut2: %d\r\n",time_out);
        return FALSE;
    }

    DBG_AUDIO("HAL_AUDIO_SeReloadCode finish\r\n");

    HAL_MAD2_SetMcuCmd(0x00);   // In T3, clear 0x2DDC after reload finish

    return TRUE;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetUniDecodeDoneFlag()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_GetUniDecodeDoneTag(void)
{
    MS_U16 temp1, temp2, i;

    for (i = 0; i < 100; i++)
    {
        temp1 = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_UNI_DECODE_DONE_CNT, ADEC1 );
        temp2 = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_UNI_DECODE_DONE_CNT, ADEC1 );

        if (temp1 == temp2)
        {
            return (temp1);
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name:         HAL_AUDIO_GetUniDecodeFrameCnt()
/// @brief \b Function \b Description:  This function is used to get the Decoder decoded frame count
/// @param <IN>        \b NONE:
/// @param <OUT>       \b NONE:
/// @param <RET>       \b NONE:
/// @param <GLOBAL>    \b NONE:
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_GetUniDecodeFrameCnt(AUDIO_DEC_ID eDSPId)
{
    MS_U16 temp1, temp2, i;
    Audio_id  r2AudioID = ADEC1;

     if (eDSPId == AU_DEC_ID3)
     {
        r2AudioID = ADEC2;
     }

    for ( i = 0; i < 100; i++ )
    {
        temp1 = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_UNI_DECODE_DONE_CNT, r2AudioID);
        temp2 = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_UNI_DECODE_DONE_CNT, r2AudioID);

        if ( temp1 == temp2 )
        {
            return (temp1);
        }
    }
    return 0;
}

//******************************************************************************
//  [Function Name]:
//      HAL_AUDIO_PTS_info
//  [Description]:
//      get PTS and OS time or set to reset these reference
//  [Arguments]:
//
//*******************************************************************************
void HAL_AUDIO_PTS_info( MS_U32 access_PTS_info )
{
    AUDIO_PTS_INFO PTS_info;

     memcpy((void *)&PTS_info,(void *)access_PTS_info,sizeof(AUDIO_PTS_INFO));

    //    printf("HAL_AUDIO_PTS_info((0x%x, %d, %lld))\n",(unsigned int)access_PTS_info,(int)PTS_info.os_Time,PTS_info.curPTS);
    if(PTS_info.set2Clr) //setting value
    {
         curr_OS_Time = PTS_info.os_Time;
         curr_PTS = PTS_info.curPTS;
    }
    else // get PTS information
    {
        MsOS_DisableInterrupt(E_INT_FIQ_DEC_DSP2UP);

        PTS_info.set2Clr = 0 ;
        PTS_info.os_Time = curr_OS_Time;
        PTS_info.curPTS = curr_PTS;

        memcpy((void *)access_PTS_info,(void *)&PTS_info,sizeof(AUDIO_PTS_INFO));

        MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);
    }
}

void HAL_AUDIO_DmaWriter_Init(void)
{
    MS_U32 audio_pcm_dmaWtr_bufSz = HW_DMA_WTR1_BUF_SIZE;  //must be multiple of DMA_RDR_PCM_BUF_UNIT*2 = 0x2000
    MS_U32 audio_pcm_dmaWtr_base = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + HW_DMA_WTR1_BUF_ADDR; // DMA Reader Input buffer (DM_Prefetch)
    ///MS_U16 synthrate, divisor;
    //MS_U8 tmp1, tmp2, tmp3, tmp4;
    //MS_U32 i;
    //MS_U32 loop;

    /* init DMA writer address */
    dmaWriter_readPtr = (MS_U8 *) MS_PA2KSEG1(audio_pcm_dmaWtr_base);
    dmaWriter_bufStartAddress =  (MS_U8 *) MS_PA2KSEG1(audio_pcm_dmaWtr_base);
    dmaWriter_bufEndAddress =  (MS_U8 *) MS_PA2KSEG1(audio_pcm_dmaWtr_base + audio_pcm_dmaWtr_bufSz);
    // printf("\n\ndmaReader_writePtr  dmaReader_bufStartAddress: %lx\n", audio_pcm_dmaWtr_base);

    HAL_AUDIO_AbsWriteReg(NewDMAwriter_ctrl, 0x0);
    HAL_AUDIO_AbsWriteReg(NewDMAwriter_BaseAddress_Lo, (audio_pcm_dmaWtr_base/BYTES_IN_MIU_LINE) & 0xFFFF);
    HAL_AUDIO_AbsWriteReg(NewDMAwriter_BaseAddress_Hi, (audio_pcm_dmaWtr_base/BYTES_IN_MIU_LINE>>16) & 0xFFFF);
    HAL_AUDIO_AbsWriteReg(NewDMAwriter_DRAM_size, audio_pcm_dmaWtr_bufSz/BYTES_IN_MIU_LINE);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_DRAM_underrun_threshold, 0xFFFF, 0x00012);                         //setting : DMA Reader Underrun Thr

    /* Reset and Start DMA Writer */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0xFFFF, 0x800B); //SW reset & enable DMA reader, MIU request
    AUDIO_DELAY1US(1);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0xFFFF, 0x000B); //SW reset & enable DMA reader, MIU request

    /* Reset Write Pointer */
    dmaWriter_readPtr = dmaWriter_bufStartAddress;
}

MS_BOOL HAL_AUDIO_DmaWriter_Read(void* buffer, MS_U32 bytes)
{
    MS_U8 *bufptr;
    MS_U16 DRAM_levelcnt = 0, DRAM_levelcnt1;
    MS_U32 DRAM_levelcnt_bytes = 0;
    MS_U32 i; //, level_cnt, level_cnt1;

    bytes = (bytes/BYTES_IN_MIU_LINE)*BYTES_IN_MIU_LINE;
    DRAM_levelcnt1 = HAL_AUDIO_AbsReadReg(NewDMAwriter_DRAM_levelcnt);
    DRAM_levelcnt = HAL_AUDIO_AbsReadReg(NewDMAwriter_DRAM_levelcnt);
    while (DRAM_levelcnt1 != DRAM_levelcnt)
    {
        DRAM_levelcnt1 = HAL_AUDIO_AbsReadReg(NewDMAwriter_DRAM_levelcnt);
        DRAM_levelcnt = HAL_AUDIO_AbsReadReg(NewDMAwriter_DRAM_levelcnt);
    }
    if (DRAM_levelcnt < (bytes/BYTES_IN_MIU_LINE))
        return FALSE;

    bufptr = buffer;
    DRAM_levelcnt_bytes = DRAM_levelcnt*BYTES_IN_MIU_LINE;
    DRAM_levelcnt_bytes = (DRAM_levelcnt_bytes > bytes) ? bytes : DRAM_levelcnt_bytes;

    for (i = 0; i < (DRAM_levelcnt_bytes/4); i++)
    {
        *bufptr++ = *dmaWriter_readPtr++;
        *bufptr++ = *dmaWriter_readPtr++;
        *bufptr++ = *dmaWriter_readPtr++;
        *bufptr++ = *dmaWriter_readPtr++;

        if ( dmaWriter_readPtr >= dmaWriter_bufEndAddress)
            dmaWriter_readPtr = dmaWriter_bufStartAddress;
    }

    //printf("D %x\n", DRAM_levelcnt);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0010, 0x0010);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_CPU_triggersize, 0xFFFF, DRAM_levelcnt_bytes/BYTES_IN_MIU_LINE);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0010, 0x0000);
    bytes -= DRAM_levelcnt_bytes;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_RebootDecDSP()
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_RebootDecDSP(void)
{
    AU_DVB_DECCMD deccmd_status, dec2cmd_status;
    AUD_ERRMSG("\n*MAD Auto-Recovery DEC-R2* \n");

    HALAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_u8AudioCrashFlag = TRUE;

    deccmd_status = HAL_MAD_GetDecCmd();
    dec2cmd_status = HAL_MAD_GetDSP2DecCmd();

    HAL_AUDIO_BackupMailbox();

    // Reset MAD module
    HAL_MAD_RSTMAD_DisEn_MIUREQ();
    AUDIO_DELAY1MS(2);

    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, FALSE, 0);    // Audio SIF channel enable setting -> disable
    HAL_SIF_SendCmd(AU_SIF_CMD_SET_SIFFIFO_MODE, 0, 0);

    AUDIO_DELAY1MS(2);

    AUD_DBGMSG("*** Load code and reset DEC-R2 \n");

    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
    HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STOP);
    //HAL_MAD2_SetMemInfo();

    if(bReBoot_All_Aud_Proc == FALSE)
    {
        HAL_AUDSP_DECR2LoadCode(); // Reload DEC-R2 code
    }
    else  //protection
    {
        bReBoot_All_Aud_Proc = FALSE;
    }

    HAL_MAD_SetDecCmd(deccmd_status);
    HAL_MAD_SetDSP2DecCmd(dec2cmd_status);

    AUDIO_DELAY1MS(50);
    HAL_SIF_SendCmd(AU_SIF_CMD_SET_SIFFIFO_MODE, 1, 0);
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, TRUE, 0);    // Audio SIF channel enable setting -> enable
    HAL_AUDIO_RestoreMailbox();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DspReboot()
/// @brief \b Function \b Description: This routine is used to do dsp reboot and some specific setting
/// @param <IN>        \b   eType   : param
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DspReboot(MS_U8 alive)
{
    alive = alive;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ReadDspCounter()
/// @brief \b Function \b Description:  Read the DSP running counter
/// @param CounterType \b  :
///                 - DSP_DEC_MAIN_COUNTER ==> Decoder DSP Main loop counter
///                 - DSP_DEC_TIMER_COUNTER ==> Decoder Timer counter
///                 - DSP_SE_MAIN_COUNTER ==> SE DSP Main loop counter
///                 - DSP_SE_TIMER_COUNTER ==> SE Timer counter
///                 - DSP_SE_ISR_COUNTER ==> SE Interrupt ISR counter
///                 - DEC_R2_MAIN_COUNTER ==> DEC R2 Main loop counter
///                 - DEC_R2_TIMER_COUNTER ==> DEC R2 Timer counter
///                 - SND_R2_MAIN_COUNTER ==> SND R2 Main loop counter
///                 - SND_R2_TIMER_COUNTER ==> SND R2 Timer counter
/// @return MS_U8      \b  : Running counter value
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_ReadDspCounter(AUDIO_DSP_COUNTER_TYPE  CounterType)
{
    MS_U8  u8CounterValue=0;

    switch(CounterType)
    {
        case DSP_DEC_MAIN_COUNTER:
            break;

        case DSP_DEC_TIMER_COUNTER:
            break;

        case DSP_SE_MAIN_COUNTER:
            u8CounterValue = HAL_AUDIO_ReadByte(REG_SOUND_MAIN_COUNTER);
            break;

        case DSP_SE_TIMER_COUNTER:
            u8CounterValue = HAL_AUDIO_ReadByte(REG_SOUND_TIMER_COUNTER);
            break;

        case DSP_SE_ISR_COUNTER:
            u8CounterValue = HAL_AUDIO_ReadByte(REG_SOUND_ISR_COUNTER);
            break;

        case DEC_R2_MAIN_COUNTER:
            u8CounterValue = HAL_AUR2_ReadByte(REG_DECR2_MAIN_COUNTER);
            break;

        case DEC_R2_TIMER_COUNTER:
            u8CounterValue = HAL_AUR2_ReadByte(REG_DECR2_TIMER_COUNTER);
            break;

        case SND_R2_MAIN_COUNTER:
            u8CounterValue = HAL_AUR2_ReadByte(REG_SNDR2_MAIN_COUNTER);
            break;

        case SND_R2_TIMER_COUNTER:
            u8CounterValue = HAL_AUR2_ReadByte(REG_SNDR2_TIMER_COUNTER);
            break;

        default:
            break;
    }
    return u8CounterValue;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AuProcessPowerDown_Wait()
/// @brief \b Function \b Description: This routine is used to DSP power-down wait function.
/// @param bEnable     \b : TRUE --Not wait,
///                         FALSE--wait
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetPowerDownWait(MS_BOOL bEnable)
{
    bEnable = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SendIntrupt()
/// @brief \b Function \b Description:  send a PIO8 interrupt to DSP
/// @param bDspType    \b :
/// @param u8Cmd       \b :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U16 u8Cmd)
{
    if (bDspType == DSP_DEC)
    {
        HAL_MAD_SetPIOCmd(u8Cmd);
        HAL_MAD_TriggerPIO8();
    }
    else
    {
            HAL_MAD2_SetPIOCmd(u8Cmd);
            HAL_MAD2_TriggerPIO8();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SeSystemLoadCode()
/// @brief \b Function \b Description: This routine is used to load DSP code for SE-DSP .
/// @return MS_BOOL    \b : TRUE --DEC-DSP load code okay,
///                         FALSE--DEC-DSP load code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SeSystemLoadCode(void)
{
        MS_U16 time_out = 0;

        HAL_MAD2_SetDspIDMA();

        HAL_AUDSP_DspLoadCode(AU_SE_SYSTEM);

        //Wait Dsp init finished Ack
    #ifndef CONFIG_MBOOT
        while(time_out++<100) {
            if(HAL_MAD2_GetLoadCodeAck() == 0xE3)
                break;
            AUDIO_DELAY1MS(2);
        }
    #endif

        if(time_out>=100) {
            printf("DSP Re-Active\n");
        }
        else {
            //remove msg for enhance str boot time
            //AUD_DBGMSG("audio DSP_SE LoadCode success..\n");
        }

        //inform DSP to start to run
        HAL_MAD2_SetMcuCmd(0xF3);

        return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DecoderLoadCode()
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DecoderLoadCode()
////////////////////////////////////////////////////////////////////////////////
MS_BOOL  HAL_AUDIO_DecoderLoadCode(void)
{
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_init()
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_Init(MS_BOOL Initype)
{
    Initype = Initype;
    return Initype;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_TX_SetMode()
/// @brief \b Function \b Description:  This routine is used to set HDMI output mode
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType, AUDIO_SOURCE_INFO_TYPE eSource)
{
    outType = outType;
    eSource = eSource;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_PCMCaptureInit()
/// @brief \b Function \b Description:  Select source for pcm capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    MS_BOOL ret = TRUE;

    switch(eID)
    {
        case E_DEVICE0:
           g_pcm_capture_base_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + PCM_CAPTURE1_BUF_ADDR;
           ret = HAL_AUDIO_SetDataCaptureSource(eID, eSource);
           ret = HAL_AUDIO_PCMCapture_Stop(eID);
           break;

        case E_DEVICE1:
           g_pcm_capture2_base_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + PCM_CAPTURE2_BUF_ADDR;
           ret = HAL_AUDIO_SetDataCaptureSource(eID, eSource);
           ret = HAL_AUDIO_PCMCapture_Stop(eID);
           break;

        case E_DEVICE2:
           MP3Encode_baseaddr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_MP3_ENC_DRAM_BASE ;
           ret = HAL_AUDIO_SetDataCaptureSource(eID, eSource);
           break;

        default:
           HALAUDIO_ERROR("\r\n =====PCMCapture_Init : unsupported DEVICE TYPE!!!=======");
           ret = FALSE;
           break;
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_PCMCapture_Start()
/// @brief \b Function \b Description:  captrue pcm data to DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID)
{
    MS_BOOL ret = TRUE;

    switch(eID)
    {
        case E_DEVICE0:
            HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_Size, DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE); // start
            break;

        case E_DEVICE1:
            g_pcm_capture_device1_enable = TRUE; //[LM14A]
            HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_Size, DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE); //start
            break;

        case E_DEVICE2:
            HAL_AUDIO_DigitalOut_Set_Encode(MP3_ENCODE, TRUE);
            MP3EncodeReadIndex = (MS_U32)HAL_AUDIO_ReadReg(REG_MPEG_ENCODER_BUF_IDX);
            break;

        default:
            HALAUDIO_ERROR("\r\n =====PCMCapture_Start : unsupported DEVICE TYPE!!!=======");
            ret = FALSE;
            break;
    }

    return ret;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_PCMCapture_Stop()
/// @brief \b Function \b Description:  stop captrue pcm data from DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID)
{
    MS_BOOL ret = TRUE;
    MS_U8 flagCount = 0;

    switch(eID)
    {
        case E_DEVICE0:
           HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_Size, 0);  // stop
           HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_RdPtr, 0);
           do
           {
               MsOS_DelayTask(2);
               flagCount ++;
               if (flagCount > 100)
               {
                    HALAUDIO_ERROR("\r\n =====PCMCapture_Stop : Device%d timeout =======", eID);
                    break;
               }
           } while (HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE_DDR_WrPtr) > 0);
           break;

        case E_DEVICE1:
            g_pcm_capture_device1_enable = FALSE; //[LM14A]
            HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_Size, 0); //stop
            HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr, 0);
            do
            {
                MsOS_DelayTask(2);
                flagCount ++;
                if (flagCount > 100)
                {
                    HALAUDIO_ERROR("\r\n =====PCMCapture_Stop : Device%d timeout =======", eID);
                    break;
                }
            } while (HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr) > 0);
            break;

        case E_DEVICE2:
            HAL_AUDIO_DigitalOut_Set_Encode(MP3_ENCODE, FALSE);
            MP3EncodeReadIndex = 0;
            break;

        default:
            HALAUDIO_ERROR("\r\n =====PCMCapture_Stop : unsupported DEVICE TYPE!!!=======");
            ret = FALSE;
            break;
    }

    return ret;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_PCMCapture_Read()
/// @brief \b Function \b Description:  captrue pcm data from DDR to device
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b void* : destination buffer pointer
/// @param <IN>        \b MS_U32 : buffer size need transfered in byte
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes)
{
    MS_BOOL ret = TRUE;
    MS_U32 buffer_tmp = (MS_U32)buffer;
    MS_U32 request_size = bytes;
    MS_U32 read_ptr_mailbox = 0;
    MS_U32 write_ptr_mailbox = 0;
    MS_U32 pcm_capture_base_addr_tmp = 0;
    MS_U32 pcm_capture_read_addr_tmp = 0;
    MS_U32 pcm_capture_write_addr_tmp = 0;
    MS_U32 mp3_encode_read_addr_tmp = 0;
    MS_S32 avail_size = 0;

    switch(eID)
    {
        case E_DEVICE0:
            read_ptr_mailbox = M2S_MBOX_PCM_CAPTURE_DDR_RdPtr;
            write_ptr_mailbox = S2M_MBOX_PCM_CAPTURE_DDR_WrPtr;
            pcm_capture_base_addr_tmp = g_pcm_capture_base_addr;
            break;

        case E_DEVICE1:
            read_ptr_mailbox = M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr;
            write_ptr_mailbox = S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr;
            pcm_capture_base_addr_tmp = g_pcm_capture2_base_addr;
            break;

        case E_DEVICE2:
            {
                mp3_encode_read_addr_tmp = (MS_U32)HAL_AUDIO_ReadReg(REG_MPEG_ENCODER_BUF_IDX);
                avail_size = mp3_encode_read_addr_tmp - MP3EncodeReadIndex;
                if (avail_size < 0)
                {
                    avail_size += MP3_ENC_DRAM_SIZE;
                }
                if(avail_size >= (MP3_ENC_DRAM_SIZE - MP3EncodeCopySize))
                {
                    HALAUDIO_ERROR("\n\033[1;36m ===== MP3_ENC DATA OVERFLOW !!!=======\033[0m \n");
                    MP3EncodeReadIndex = (MS_U32)HAL_AUDIO_ReadReg(REG_MPEG_ENCODER_BUF_IDX);
                    ret = FALSE;
                    return ret;
                }
                // if no enough data, do nothing.
                if (avail_size < request_size)
                {
                    ret = FALSE;
                    return ret;
                }

                mp3_encode_read_addr_tmp = MP3Encode_baseaddr + MP3EncodeReadIndex;
                //copy data to destination.
                do
                {
                    MS_U32 size_tmp = (MP3Encode_baseaddr + MP3_ENC_DRAM_SIZE) - mp3_encode_read_addr_tmp;

                    size_tmp = (avail_size >= size_tmp) ? size_tmp : avail_size;
                    size_tmp = (request_size >= size_tmp) ? size_tmp : request_size;

                    memcpy((void *)buffer_tmp, (void *)MS_PA2KSEG1(mp3_encode_read_addr_tmp), size_tmp);
                    MsOS_FlushMemory();

                    mp3_encode_read_addr_tmp += size_tmp;
                    if (mp3_encode_read_addr_tmp == (MP3Encode_baseaddr + MP3_ENC_DRAM_SIZE))
                        mp3_encode_read_addr_tmp = MP3Encode_baseaddr;

                    buffer_tmp += size_tmp;
                    avail_size -= size_tmp;
                    request_size -= size_tmp;

                } while (request_size > 0);
                //update read pointer
                MP3EncodeReadIndex = mp3_encode_read_addr_tmp - MP3Encode_baseaddr;
                return ret;
            }
            break;

        default:
           HALAUDIO_ERROR("\r\n =====PCMCapture_Read : unsupported DEVICE TYPE!!!=======\n");
           ret = FALSE;
           break;
    }

    if (ret == FALSE)
        return ret;

    //get read & write pointer
    pcm_capture_read_addr_tmp = pcm_capture_base_addr_tmp + (HAL_AUDIO_ReadReg(read_ptr_mailbox) * BYTES_IN_MIU_LINE);
    pcm_capture_write_addr_tmp = pcm_capture_base_addr_tmp + (HAL_AUDIO_ReadReg(write_ptr_mailbox) * BYTES_IN_MIU_LINE);
    avail_size = (MS_S32)(pcm_capture_write_addr_tmp - pcm_capture_read_addr_tmp);
    if (avail_size < 0)
    {
        avail_size += (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE);
    }

    // if overflow , return false
    if (avail_size >= ((DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE - DSP2_PCM_CAPTURE_COPY_LINESIZE) * BYTES_IN_MIU_LINE))
    {
        HALAUDIO_ERROR("\r\n ===== PCM DATA OVERFLOW !!!=======\n");

        HAL_AUDIO_PCMCapture_Stop(eID);
        HAL_AUDIO_PCMCapture_Start(eID);
        ret = FALSE;
        return ret;
    }

    // if no enough data, do nothing.
    if (avail_size < request_size)
    {
        ret = FALSE;
        return ret;
    }

    //copy data to destination.
    do
    {
        MS_U32 size_tmp = (pcm_capture_base_addr_tmp + (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE)) - pcm_capture_read_addr_tmp;

        size_tmp = (avail_size >= size_tmp) ? size_tmp : avail_size;
        size_tmp = (request_size >= size_tmp) ? size_tmp : request_size;

        memcpy((void *)buffer_tmp, (void *)MS_PA2KSEG1(pcm_capture_read_addr_tmp), size_tmp);
        MsOS_FlushMemory();

        pcm_capture_read_addr_tmp += size_tmp;
        if (pcm_capture_read_addr_tmp == (pcm_capture_base_addr_tmp + (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE)))
            pcm_capture_read_addr_tmp = pcm_capture_base_addr_tmp;

        buffer_tmp += size_tmp;
        avail_size -= size_tmp;
        request_size -= size_tmp;

    } while (request_size > 0);

    //update read pointer
    HAL_AUDIO_WriteReg(read_ptr_mailbox, ((pcm_capture_read_addr_tmp - pcm_capture_base_addr_tmp) / BYTES_IN_MIU_LINE));

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDataCaptureSource()
/// @brief \b Function \b Description:  Select source for data capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    MS_U32 u32ControlAddress;
    MS_BOOL ret = TRUE;

    HALAUDIO_CHECK_SHM_INIT;

    switch(eID)
    {
        case E_DEVICE0:
           u32ControlAddress=M2S_MBOX_CAPTURE_CTRL;
           break;

        case E_DEVICE1:
           u32ControlAddress=M2S_MBOX_CAPTURE_CTRL+1;
           break;

        case E_DEVICE2:
           u32ControlAddress=M2S_MBOX_CAPTURE_CTRL+1;
           break;

        default:
           HALAUDIO_ERROR("\r\n =====SetDataCaptureSource : unsupport DEVICE TYPE!!!=======");
           return FALSE;
    }

    switch(eSource)
    {
        case E_CAPTURE_CH5:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_CH5);
           break;

        case E_CAPTURE_CH6:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_CH6);
           break;

        case E_CAPTURE_CH7:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_CH7);
           break;

        case E_CAPTURE_CH8:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_CH8);
           break;

        case E_CAPTURE_ADC:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_ADC1);
           break;

        case E_CAPTURE_ADC2:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_ADC2);
           break;

        case E_CAPTURE_PCM_SE:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_Raw_Delay_SE);
           break;

        case E_CAPTURE_PCM:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_Raw);
           break;

        case E_CAPTURE_MIXER:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_MIXER);
           break;

        case E_CAPTURE_ADC1_AUIN0_INPUT:
        case E_CAPTURE_ADC1_AUIN1_INPUT:
        case E_CAPTURE_ADC1_AUIN2_INPUT:
        case E_CAPTURE_ADC1_AUIN3_INPUT:
        case E_CAPTURE_ADC1_AUIN4_INPUT:
        case E_CAPTURE_ADC1_AUIN5_INPUT:
        case E_CAPTURE_ADC1_AUMIC_INPUT:
        {
            MS_U8 u8temp = 0;

            //check if ADC1 is occupied by main or sub channel
            if((LONIBBLE(g_AudioVars2->eMainAudioSource) == 2 || LONIBBLE(g_AudioVars2->eSubAudioSource) == 2))
            {
                HALAUDIO_ERROR("===The audio capture setting is failed by the following reason:=== \n");
                HALAUDIO_ERROR("The input setting of ADC1 is occupied by main or sub channel\n");
                ret = FALSE;
                break;
            }

            //switch source of ADC1
            if (eSource == E_CAPTURE_ADC1_AUIN0_INPUT)
            {
                u8temp = 0x00;
            }
            else if (eSource == E_CAPTURE_ADC1_AUIN1_INPUT)
            {
                u8temp = 0x01;
             }
            else if (eSource == E_CAPTURE_ADC1_AUIN2_INPUT)
            {
                u8temp = 0x02;
             }
            else if (eSource == E_CAPTURE_ADC1_AUIN3_INPUT)
            {
                u8temp = 0x03;
             }
            else if (eSource == E_CAPTURE_ADC1_AUIN4_INPUT)
            {
                u8temp = 0x04;
             }
            else if (eSource == E_CAPTURE_ADC1_AUIN5_INPUT)
            {
                u8temp = 0x05;
             }
            else if (eSource == E_CAPTURE_ADC1_AUMIC_INPUT)
            {
                u8temp = 0x07;
             }

            HAL_AUDIO_WriteMaskByte(0x2CE2, 0xF0, (u8temp<<4));
            HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_ADC1);
            break;
        }

        case E_CAPTURE_ADC2_AUIN0_INPUT:
        case E_CAPTURE_ADC2_AUIN1_INPUT:
        case E_CAPTURE_ADC2_AUIN2_INPUT:
        case E_CAPTURE_ADC2_AUIN3_INPUT:
        case E_CAPTURE_ADC2_AUIN4_INPUT:
        case E_CAPTURE_ADC2_AUIN5_INPUT:
        case E_CAPTURE_ADC2_AUMIC_INPUT:
        {
            MS_U8 u8temp = 0;

            //check if ADC2 is occupied by main or sub channel
            if((LONIBBLE(g_AudioVars2->eMainAudioSource) == 9 || LONIBBLE(g_AudioVars2->eSubAudioSource) == 9))
            {
                HALAUDIO_ERROR("===The audio capture setting is faiedl by the following reason:=== \n");
                HALAUDIO_ERROR("The input setting of ADC2 is occupied by main or sub channel \n");
                ret = FALSE;
                break;
            }

            //switch source of ADC2
            if (eSource == E_CAPTURE_ADC2_AUIN0_INPUT)
            {
                u8temp = 0x00;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN1_INPUT)
            {
                u8temp = 0x01;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN2_INPUT)
            {
                u8temp = 0x02;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN3_INPUT)
            {
                u8temp = 0x03;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN4_INPUT)
            {
                u8temp = 0x04;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN5_INPUT)
            {
                u8temp = 0x05;
            }
            else if (eSource == E_CAPTURE_ADC2_AUMIC_INPUT)
            {
                u8temp = 0x07;
            }
            HAL_AUDIO_WriteMaskByte(0x2CE2, 0x0F, u8temp);
            HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_ADC2);
            break;
         }

         default:
             HALAUDIO_ERROR("\r\n =====SetDataCaptureSource : unsupport DEVICE TYPE!!!=======");
             ret = FALSE;
             break;
    }

    return ret;
}

MS_BOOL _AudioReloadCodePreProcess(AUDIO_DSP_ID eDspId, AUDIO_DSP_CODE_TYPE code_type)
{
    MS_U16 time_out;

    code_type = code_type;
    if ( eDspId >= AUDIO_DSP_ID_R2 )
    {
        HALAUDIO_ERROR ("%s: invalid DSP ID (%d) !!\r\n", __FUNCTION__, eDspId);
        return FALSE;
    }

    if ( eDspId == AUDIO_DSP_ID_SND )
    {
        Dvb2DecCmd_tmp = HAL_AUDIO_Se_Status();
        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STOP);                      // Stop

        MsOS_DisableInterrupt(E_INT_FIQ_DEC_DSP2UP);

        // Reset MAD module
        // HAL_MAD2_DisEn_MIUREQ();

        HAL_MAD2_SetDspIDMA();
        // Enter MCU/DSP hand-shake
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_SIF_BEG >> 8);

        // PIO[8] interrupt
        HAL_MAD2_TriggerPIO8();

        //Wait Dsp Start reload Ack
        time_out = 0;
        while ( time_out++ < 2000 )
        {
            if ( HAL_MAD2_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK1 )
            {
                break;
            }

            AUDIO_DELAY1MS(1);
        }

        if ( time_out >= 2000 )
        {
            HALAUDIO_ERROR ("  SND DSP Reload timeOut1: %d\r\n", time_out);
            return FALSE;
        }

        // Change to IDMA Port
        HAL_MAD2_SetDspIDMA();
    }

    return TRUE;
}

MS_BOOL _AudioReloadCodePostProcess(AUDIO_DSP_ID eDspId,  AUDIO_DSP_CODE_TYPE code_type)
{
    MS_U16 time_out;

    code_type = code_type;

    if ( eDspId >= AUDIO_DSP_ID_R2 )
    {
        HALAUDIO_ERROR ("%s: invalid DSP ID (%d) !!\r\n", __FUNCTION__, eDspId);
        return FALSE;
    }

    if ( eDspId == AUDIO_DSP_ID_SND )
    {
        // Enter MCU/DSP hand-shake
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_SIF_END >> 8);

        HAL_MAD2_TriggerPIO8();

        // Wait Dsp End Reload Ack
        time_out = 0;
        while ( time_out++ < 3000 )
        {
            if ( HAL_MAD2_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK2 )
            {
                break;
            }

            AUDIO_DELAY1MS(1);
        }

        if ( time_out >= 3000 )
        {
            HALAUDIO_ERROR ("  SND DSP Reload timeOut2\r\n");
            return FALSE;
        }

        HAL_MAD2_SetMcuCmd(0x00);   // In T3, clear 0x2DDC after reload finish

        MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);
        HAL_MAD_SetDSP2DecCmd((AU_DVB_DECCMD) Dvb2DecCmd_tmp);
    }

    return TRUE;
}

MS_BOOL HAL_AUDIO_AlgReloadCode2(AUDIO_DSP_ID id, AUDIO_DSP_CODE_TYPE code_type, void * pau_info)
{
    if ( _AudioReloadCodePreProcess(id, code_type) == FALSE )
    {
        return FALSE;
    }

    // Start to Reload DSP code
    if ( HAL_AUDSP_DspLoadCode2((MS_U8) id, pau_info) == FALSE )
    {
        HALAUDIO_ERROR ("  %s DSP Reload Fail !!\r\n", (id==AUDIO_DSP_ID_DEC ? "DEC" : "SND"));
        return FALSE;
    }

    if ( _AudioReloadCodePostProcess(id, code_type) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_AUDIO_SetSystem2(AUDIO_DEC_ID DecId, En_DVB_decSystemType Param)
{
    MS_U32 regDecoderType = REG_R2_DECODE1_TYPE;
    MS_BOOL bRet = TRUE;

    if ( DecId == AU_DEC_ID3 )
    {
        regDecoderType = REG_R2_DECODE2_TYPE;
    }
    else if (DecId == AU_DEC_ID1)
    {
        regDecoderType = REG_R2_DECODE1_TYPE;
    }
    else if (DecId == AU_DEC_ID2)
    {
        // AU_DEC_ID2 only for ATV SIF
        // For other case should not use AU_DEC_ID2
        return bRet;
    }

    switch ( Param )
    {
        case MSAPI_AUD_DVB_MPEG:
        case MSAPI_AUD_DVB_MP3:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_mpeg);
            if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_MPEG, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_MPEG, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_MS10_DDC:
        case MSAPI_AUD_DVB_AC3P:
        case MSAPI_AUD_DVB_AC3:
            HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
            HAL_AUR2_WriteByte(regDecoderType, adec_type_ac3);
            MS_BOOL ret_status = FALSE;
            ret_status = HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_MS10_DDE);
            HAL_AUDIO_SetDsp2CodeType(AU_DVB2_ENCODE_MS10_DDE, ret_status);
            if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_AC3P, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_AC3P, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_MS10_DDT:
        case MSAPI_AUD_DVB_AAC:
        {
            HAL_AUR2_WriteByte(regDecoderType, adec_type_aac);

            //if(!(g_AudioVars2->g_bATVExist))
            {
                MS_BOOL ret_status = FALSE;
                ret_status = HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_MS10_DDE);
                HAL_AUDIO_SetDsp2CodeType(AU_DVB2_ENCODE_MS10_DDE, ret_status);
            }

            if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_MS10_DDT, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_MS10_DDT, TRUE);
            }
        }
        break;

        case MSAPI_AUD_DVB_XPCM:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_xpcm);
             if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_XPCM, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_XPCM, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_RA8LBR:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_ra8);
             if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_RA8LBR, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_RA8LBR, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_WMA:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_wma);
             if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_WMA, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_WMA, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_DTS:
        case MSAPI_AUD_DVB_DTSLBR:
        case MSAPI_AUD_DVB_DTSXLL:
        case MSAPI_AUD_DVB_DTSHDADO:
        {
            AUDIO_DSP_CODE_TYPE eType;

            HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
            HAL_AUR2_WriteByte(regDecoderType, adec_type_dts);

            //if(!(g_AudioVars2->g_bATVExist))
            {
                MS_BOOL ret_status = FALSE;
                ret_status = HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_DTSE);
                HAL_AUDIO_SetDsp2CodeType(AU_DVB2_ENCODE_DTSE, ret_status);
            }

            HAL_MAD_SetCommInfo(Audio_Comm_infoType_setBypassSPDIF_PAPB_chk, TRUE, 0);
            if (Param == MSAPI_AUD_DVB_DTSLBR)
            {
                eType = AU_DVB_STANDARD_DTSLBR;
            }
            else if (Param == MSAPI_AUD_DVB_DTSXLL)
            {
                eType = AU_DVB_STANDARD_DTSXLL;
            }
            else
            {
                eType = AU_DVB_STANDARD_DTS;
            }
            if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(eType, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(eType, TRUE);
            }
            break;
        }

        case MSAPI_AUD_DVB_WMA_PRO:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_wmap);
             if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_WMA_PRO, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_WMA_PRO, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_DRA:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_dra);
             if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_DRA, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_DRA, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_FLAC:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_flac);
             if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_FLAC, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_FLAC, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_VORBIS:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_vorbis);
             if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_VORBIS, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_VORBIS, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_AMR_NB:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_amrnb);
            if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_INVALID, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_INVALID, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_AMR_WB:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_amrwb);
            if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_INVALID, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_INVALID, TRUE);
            }
            break;

        case MSAPI_AUD_DVB_DolbyTrueHDBypass:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_dolby_truhd_bypass);
            if(DecId == AU_DEC_ID3)
            {
                HAL_AUDIO_SetDecR2_ADEC2_DecType(AU_DVB_STANDARD_DolbyTrueHD_Bypass, TRUE);
            }
            else
            {
                HAL_AUDIO_SetDecR2_ADEC1_DecType(AU_DVB_STANDARD_DolbyTrueHD_Bypass, TRUE);
            }
            break;

        default:
            HALAUDIO_ERROR ("%s: UN-KNOW DECODER TYPE (0x%x) !!\r\n", __FUNCTION__, Param);
            break;
    }
    return bRet;
}

AUDIO_DEC_ID AU_GetDecID(AUDIO_DEC_ID *DecPriority, AudioDecStatus_t * p_AudioDecStatus)
{
    AUDIO_DEC_ID DecID = AU_DEC_INVALID;
#if 0 //LM15U use force AUDIO ID
    int Index = 0;

    for(Index=0; Index<AU_DEC_MAX; Index++)
    {
        // Get Dec ID by priority
        DecID = DecPriority[Index];
        HALAUDIO_PRINT("DecID = %d %s() %d \n",DecID,__FUNCTION__,__LINE__);
        if ((HAL_MAD_GetAudioCapability(DecID, p_AudioDecStatus) == TRUE) &&
             (g_AudioVars2->AudioDecStatus[DecID].bConnect == FALSE))
        {
            HALAUDIO_PRINT("DecID = %d %s() %d \n",DecID,__FUNCTION__,__LINE__);
            break;
        }
    }
#endif

    switch(p_AudioDecStatus->eAudio_Force_ID)
    {
        case AUDIO_FORCE_ID1:
        {
            DecID = AU_DEC_ID1;
        }
        break;
        case AUDIO_FORCE_ID2:
        {
            DecID = AU_DEC_ID2;
        }
        break;
        case AUDIO_FORCE_ID3:
        {
            DecID = AU_DEC_ID3;
        }
        break;
        default:
        {
            DecID = AUDIO_FORCE_INVALID;
        }
        break;
    }

#if 0
    printf("\033[1;36m [AUDIO][%s] [%s] [%d] [DecID = %d] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, DecID);/*@audio*/
    printf("\033[1;36m [AUDIO][%s] [%s] [%d] [DecID = %d] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, DecID);/*@audio*/
    printf("\033[1;36m [AUDIO][%s] [%s] [%d] [DecID = %d] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, DecID);/*@audio*/
    printf("\033[1;36m [AUDIO][%s] [%s] [%d] [DecID = %d] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, DecID);/*@audio*/
    printf("\033[1;36m [AUDIO][%s] [%s] [%d] [DecID = %d] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, DecID);/*@audio*/
    printf("\033[1;36m [AUDIO][%s] [%s] [%d] [DecID = %d] \033[0m \n", __FILE__, __FUNCTION__, __LINE__, DecID);/*@audio*/
#endif

    HALAUDIO_PRINT("DecID = %d %s() %d \n",DecID,__FUNCTION__,__LINE__);
    return DecID;
}

AUDIO_DEC_ID HAL_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus)
{
    AUDIO_DEC_ID DecRet = AU_DEC_INVALID;

    switch (p_AudioDecStatus->eSourceType)
    {
        case E_AUDIO_INFO_ATV_IN:
            DecRet = AU_DEC_ID2;
            break;

        case E_AUDIO_INFO_HDMI_IN:
            DecRet = AU_GetDecID(HDMIDecPriority, p_AudioDecStatus);
            break;

        case E_AUDIO_INFO_DTV_IN:
            DecRet = AU_GetDecID(DTVDecPriority, p_AudioDecStatus);
            break;

        case E_AUDIO_INFO_MM_IN:
        case E_AUDIO_INFO_GAME_IN:
            if (p_AudioDecStatus->eMMType == AUDIO_MM_VD)
            {
                DecRet = AU_GetDecID(VDDecPriority, p_AudioDecStatus);
            }
            else
            {
                DecRet = AU_GetDecID(MMDecPriority, p_AudioDecStatus);
            }
            break;

        default:
            DecRet = AU_GetDecID(DTVDecPriority, p_AudioDecStatus);
            break;
    }

    HALAUDIO_PRINT("DecID = %d %s() %d \n",DecRet,__FUNCTION__,__LINE__);
    return DecRet;
}

MS_BOOL HAL_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId, AudioDecStatus_t *p_AudioDecStatus)
{
    AUDIO_DSP_ID DSP_id = AUDIO_DSP_ID_ALL;
    MS_BOOL bRet = FALSE;

    HALAUDIO_CHECK_SHM_INIT;

    if((g_AudioVars2->AudioDecStatus[DecId].eAudFormat == p_AudioDecStatus->eAudFormat) &&
      (g_AudioVars2->AudioDecStatus[DecId].eSourceType == p_AudioDecStatus->eSourceType) &&
      (g_AudioVars2->AudioDecStatus[DecId].eGroup == p_AudioDecStatus->eGroup) &&
      (g_AudioVars2->AudioDecStatus[DecId].eStcSource == p_AudioDecStatus->eStcSource) &&
      (g_AudioVars2->AudioDecStatus[DecId].bIsAD == p_AudioDecStatus->bIsAD))
    {
        HALAUDIO_PRINT("%s() - DSP code is the same\n",__FUNCTION__);
        if ((p_AudioDecStatus->eAudFormat & MSAPI_AUD_ATV_NONE) == MSAPI_AUD_ATV_NONE)
        {
            HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, TRUE, NULL);

            //[LM14A]
            if(g_pcm_capture_device1_enable)
            {
                //For avoid in APVR, ATV -> HDMI -> ATV, recorded file has pop noise
            }
            else
            {
                HAL_SIF_TriggerSifPLL();
            }
        }
        return bRet;
    }

    // update occupied decoder
    if (DecId == AU_DEC_ID3)
    {
        DSP_id = AUDIO_DSP_ID_SND;
    }
    else if (DecId == AU_DEC_ID1)
    {
        DSP_id = AUDIO_DSP_ID_DEC;
    }
    else if (DecId == AU_DEC_ID2)
    {
        // Patch, SIF only decode in SND_DSP, but this enum is same as AUDIO_DSP_ID_SND
        DSP_id = AUDIO_DSP_ID_ALL;
    }

    if ((p_AudioDecStatus->eAudFormat & MSAPI_AUD_ATV_NONE) == MSAPI_AUD_ATV_NONE)
    {
#if 0
        AUDIO_PATH_TYPE u8SifPath;
        HAL_SIF_TriggerSifPLL();
        if (p_AudioDecStatus->eGroup == E_CONNECT_SUB)
        {
            u8SifPath = AUDIO_PATH_6;
        }
        else
        {
            u8SifPath = AUDIO_PATH_MAIN;
        }
        //HAL_AUDIO_SetInputPath(AUDIO_DSP4_SIF_INPUT , u8SifPath);
#endif
    }
    else
    {
        HAL_MAD_DvbFLockSynthesizer_En();
    }
    g_AudioVars2->AudioDecStatus[DecId].eDSPId = DSP_id;
    p_AudioDecStatus->eDSPId = DSP_id;

    switch ( p_AudioDecStatus->eAudFormat)
    {
        case MSAPI_AUD_DVB_MPEG:
        case MSAPI_AUD_DVB_AC3:
        case MSAPI_AUD_DVB_AC3P:
        case MSAPI_AUD_DVB_MP3:
        case MSAPI_AUD_DVB_AAC:
        case MSAPI_AUD_DVB_XPCM:
        case MSAPI_AUD_DVB_RA8LBR:
        case MSAPI_AUD_DVB_WMA:
        case MSAPI_AUD_DVB_DTS:
        case MSAPI_AUD_DVB_MS10_DDT:
        case MSAPI_AUD_DVB_MS10_DDC:
        case MSAPI_AUD_DVB_WMA_PRO:
        case MSAPI_AUD_DVB_FLAC:
        case MSAPI_AUD_DVB_VORBIS:
        case MSAPI_AUD_DVB_AMR_NB:
        case MSAPI_AUD_DVB_AMR_WB:
        case MSAPI_AUD_DVB_DRA:
        case MSAPI_AUD_DVB_DTSLBR:
        case MSAPI_AUD_DVB_DTSXLL:
        case MSAPI_AUD_DVB_DTSHDADO:
            bRet = HAL_AUDIO_SetSystem2(DecId, p_AudioDecStatus->eAudFormat);
            break;

        case MSAPI_AUD_ATV_BTSC:
            bRet = HAL_SIF_SetSystem(AU_SIF_BTSC);

            //[LM14A]
            if(g_pcm_capture_device1_enable)
            {
                //For avoid in APVR, ATV -> HDMI -> ATV, recorded file has pop noise
            }
            else
            {
                HAL_SIF_TriggerSifPLL();
            }
            break;

        case MSAPI_AUD_ATV_PAL:
            bRet = HAL_SIF_SetSystem(AU_SIF_PALSUM);

            //[LM14A]
            if(g_pcm_capture_device1_enable)
            {
                //For avoid in APVR, ATV -> HDMI -> ATV, recorded file has pop noise
            }
            else
            {
                HAL_SIF_TriggerSifPLL();
            }
            break;

        case MSAPI_AUD_ATV_EIAJ:
        case MSAPI_AUD_ATV_FM_RADIO:
            break;

        default:
            HALAUDIO_ERROR ("%s: UN-KNOW DECODER TYPE (0x%x) !!\r\n", __FUNCTION__, p_AudioDecStatus->eAudFormat);
            break;
    }

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDDRInfo()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo)
{
    MS_U32 DDR_ADDR = 0;
    MS_U32 DDR_ADDR_LINE_BASE = 0;
    MS_U16 DDR_ADDR_TMP;
    MS_U32 DDR_Value = 0;
    if (DecId == AU_DEC_ID1)
    {
        switch(DDRInfo)
        {
            case E_AUD_MEMORY_BASE:             //use DSP2 base instead
                DDR_ADDR_TMP = HAL_AUDIO_ReadByte((REG_SE_MAD_OFFSET_BASE_L + 1)); // [7:0]
                DDR_ADDR_LINE_BASE = DDR_ADDR_TMP & 0xFF;
                DDR_ADDR_TMP = HAL_AUDIO_ReadReg(REG_SE_MAD_OFFSET_BASE_H); // [23:8]
                DDR_ADDR_TMP = DDR_ADDR_TMP & 0xFFFF;
                DDR_ADDR_LINE_BASE = DDR_ADDR_LINE_BASE + (DDR_ADDR_TMP <<8);
                DDR_ADDR_TMP = HAL_AUDIO_ReadReg(REG_SE_MAD_OFFSET_BASE_EXT); // [27:24]
                DDR_ADDR_TMP = DDR_ADDR_TMP & 0xF;
                DDR_ADDR_LINE_BASE = DDR_ADDR_LINE_BASE + (DDR_ADDR_TMP <<24);
                DDR_ADDR = DDR_ADDR + (DDR_ADDR_LINE_BASE<<BYTES_IN_MIU_LINE_LOG2);
                DDR_Value = DDR_ADDR;
                break;
            case  E_AUD_MEMORY_SIZE:
                //TODO
                break;
            default:
                HALAUDIO_ERROR ("%s: UN-KNOW DDRInfo (0x%x) !!\r\n", __FUNCTION__, DDRInfo);
                break;
        }
    }
    else if(DecId == AU_DEC_ID3)
    {
        switch(DDRInfo)
        {
            case E_AUD_MEMORY_BASE:
                DDR_ADDR_TMP = HAL_AUDIO_ReadByte((REG_SE_MAD_OFFSET_BASE_L + 1)); // [7:0]
                DDR_ADDR_LINE_BASE = DDR_ADDR_TMP & 0xFF;
                DDR_ADDR_TMP = HAL_AUDIO_ReadReg(REG_SE_MAD_OFFSET_BASE_H); // [23:8]
                DDR_ADDR_TMP = DDR_ADDR_TMP & 0xFFFF;
                DDR_ADDR_LINE_BASE = DDR_ADDR_LINE_BASE + (DDR_ADDR_TMP <<8);
                DDR_ADDR_TMP = HAL_AUDIO_ReadReg(REG_SE_MAD_OFFSET_BASE_EXT); // [27:24]
                DDR_ADDR_TMP = DDR_ADDR_TMP & 0xF;
                DDR_ADDR_LINE_BASE = DDR_ADDR_LINE_BASE + (DDR_ADDR_TMP <<24);
                DDR_ADDR = DDR_ADDR + (DDR_ADDR_LINE_BASE<<BYTES_IN_MIU_LINE_LOG2);
                DDR_Value = DDR_ADDR;
                break;
            case  E_AUD_MEMORY_SIZE:
                //TODO
                break;
            default:
                HALAUDIO_ERROR ("%s: UN-KNOW DDRInfo (0x%x) !!\r\n", __FUNCTION__, DDRInfo);
                break;
        }
    }
    else
    {
        HALAUDIO_ERROR ("%s: UN-KNOW DecId (0x%x) !!\r\n", __FUNCTION__, DecId);
    }
    return DDR_Value;
}

long long long_div(long long a, int b)
{
    long long r = 0;

    while (a >= b)
    {
        a -= b;
        r++;
    }
    return r;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DumpDspInfo()
/// @brief \b Function \b Description:  Dump DSP infomation
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DumpDspInfo(void)
{
//#ifdef NoLongDiv
    int tmp_H, tmp_L, tmp_M;
    long long result = 0;
    unsigned int ES_Lvl, PCM_Lvl;
    static MS_U32 preSysTime, prv_mmFile_APTS;
    MS_U32 sysTime, mmFile_APTS;

    if(g_AudioDumpInfoCtrl == Audio_DumpInfo_Off)
    {
        return;
    }

    ES_Lvl = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, ADEC1);
    PCM_Lvl = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, ADEC1);

    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_Avsync) != 0)
    {
        sysTime = MsOS_GetSystemTime();
        printf("[%08ld]", sysTime);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ACCUM_ES_BYTECNT, ADEC1)&0xFFFF;
        printf("ES=%03X(%04X),", ES_Lvl, tmp_L);

        //Eric.C need to refine
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCMISR_EMPTY_CNT, ADEC1)&0xFFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ELAPSED_PCM_CNT, ADEC1)&0xFFFF;
        printf("PCM=%04X(%04X,%04X)|", PCM_Lvl, tmp_H, tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAYSMPFLAG, ADEC1)&0x1;
        printf("play=%X",   tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNTH_RATE, ADEC1)&0xFFFF;
        printf("<%04X>,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_STATE, ADEC1)&0x0F;
        tmp_M = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, 0, 0)&0x0F;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_FREERUN_TYPE, ADEC1)&0x0F;
        printf("State=%X,%X(%X)|", tmp_L,tmp_M, tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1)&0x0000FFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1)&0x0000FFFF;
        printf("frmCnt:%04X,",     tmp_L);
        printf("%04X,",    tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SKIP_FRMCNT, ADEC1)&0x0000FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_REPT_FRMCNT, ADEC1)&0x0000FF;
        printf("%02X,",    tmp_L);
        printf("%02X,",    tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_WAIT_1STPTS_CNT, ADEC1)&0x00FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_WAIT_STC_CNT, ADEC1)&0x00FF;
        tmp_M = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_FLUSH_CNT, ADEC1)&0x00FF;
        printf("%02X,%02X,%02X|",  tmp_L, tmp_H, tmp_M);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS_TAG_WR_IDX, ADEC1)&0xF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS_TAG_RD_IDX, ADEC1)&0xF;
        printf("tag:%01X,%01X|",  tmp_L, tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_OFFSET, ADEC1)&0x00FFFFFF;
        printf("AvDly=%05X,", tmp_L);

        result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_STC, ADEC1);
        printf("STC=%08lld,", long_div(result, 45));

        result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS, ADEC1);
        printf("PTS=%08lld,", long_div(result, 45));

        result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC1);
        printf("cPTS=%08lld,", long_div(result, 45));

        result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TD, ADEC1);
        printf("|Td=%lld\n", long_div(result, 45));

        // reserved
        //tmp_L = HAL_MAD_Read_DSP_sram(DSP1DmAddr_sys_AvSync+SYS_AVSYNC_VIDEO_PTS_L, DSP_MEM_TYPE_DM)&0x00FFFFFF;
        //tmp_H = HAL_MAD_Read_DSP_sram(DSP1DmAddr_sys_AvSync+SYS_AVSYNC_VIDEO_PTS_H, DSP_MEM_TYPE_DM)&0x00FFFFFF;
        //result = tmp_H;
        //result <<= 24;
        //result += tmp_L;
        //result <<= 31;
        //result >>= 31;
        //printf("VedioTd=0x%012llX, ", result);

    }

    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_Dec1) != 0)
    {
        sysTime = MsOS_GetSystemTime();
        printf("[%08ld]", sysTime);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ACCUM_ES_BYTECNT, ADEC1)&0xFFFF;
        printf("ES=%03X(%04X),", ES_Lvl, tmp_L);

        //Eric.C need to refine
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCMISR_EMPTY_CNT, ADEC1)&0xFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ELAPSED_PCM_CNT, ADEC1)&0xFFFF;
        printf("PCM=%04X(%04X,%02X)|", PCM_Lvl, tmp_H, tmp_L);

        tmp_L = (MS_U32)HAL_AUR2_ReadByte(REG_R2_DECODE1_TYPE);
        printf("ID=%02X,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_TYPE, ADEC1)&0x00FF;
        printf("Type=%02X,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAY_STATE, ADEC1)&0x00FFFFFF;
        printf("pState=%06X,", tmp_L);

        tmp_L = (MS_U32)HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD);
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_RECEIVE_STOP_CNT, ADEC1)&0x00FF;
        printf("|Cmd=%02X,Stop=%02X|",  tmp_L, tmp_H);

        tmp_L = HAL_AUDIO_ReadReg(REG_AUDIO_DECODER1_CFG);
        tmp_H = HAL_AUDIO_ReadReg(REG_AUDIO_DECODER2_CFG);
        printf("Input_Mux:%02X,", tmp_L&0x7);
        printf("%02X,", tmp_H&0x7);
        printf("%02X|", (tmp_L>>16)&0x7);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1)&0x0000FFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1)&0x0000FFFF;
        printf("frmCnt:%04X,",     tmp_L);
        printf("%04X,",    tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SKIP_FRMCNT, ADEC1)&0x0000FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_REPT_FRMCNT, ADEC1)&0x0000FF;
        printf("%02X,",  tmp_L);
        printf("%02X|",  tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CALLING_CNT, ADEC1)&0x0000FFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNC_MISS_CNT, ADEC1)&0x0000FFFF;
        printf("Call:%04X,",    tmp_L);
        printf("sMiss:%04X|",  tmp_H);

        //Eric.C need to refine
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_OUT_L, ADEC1)&0x00FFFFFF;
        printf("pcm=%06X,",  tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_OUT_R, ADEC1)&0x00FFFFFF;
        printf("%06X|",  tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAYSMPFLAG, ADEC1)&0x1;
        printf("play=%X\n",   tmp_L);
    }


    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_IoInfo) != 0)
    {
        tmp_L = (HAL_AUDIO_ReadByte(REG_AUDIO_CH5_CFG)) & 0x000F;
        printf("CH5 mux=0x%02X,",  tmp_L);

        tmp_L = (HAL_AUDIO_ReadByte(REG_AUDIO_CH6_CFG)) & 0x000F;
        printf("Ch6 mux=0x%02X,",  tmp_L);

        tmp_L = (HAL_AUDIO_ReadByte(REG_AUDIO_CH7_CFG)) & 0x000F;
        printf("Ch7 mux=0x%02X",  tmp_L);

        //Eric.C need to refine
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_OUT_L, ADEC1)&0x00FFFFFF;
        printf("|DEC1=0x%06X,",  tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_OUT_L, ADEC2)&0x00FFFFFF;
        printf("DEC2=0x%06X",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info2+IO_INFO2_RAW+0, DSP_MEM_TYPE_DM);
        printf("|CH5=0x%06X,",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info2+IO_INFO2_MUL_CH6+0, DSP_MEM_TYPE_DM);
        printf("CH6=0x%06X,",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info2+IO_INFO2_SCART+0, DSP_MEM_TYPE_DM);
        printf("CH7=0x%06X",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info1+IO_INFO1_DAC1_OUT+0, DSP_MEM_TYPE_DM);
        printf("|DAC1=0x%06X,",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info1+IO_INFO1_IIS1_OUT+0, DSP_MEM_TYPE_DM);
        printf("I2S=0x%06X,",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info1+IO_INFO1_SPDIF_OUT+0, DSP_MEM_TYPE_DM);
        printf("SPDIF=0x%06X\n",  tmp_L);
    }

    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_AutoTest) != 0)
    {
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1)&0x00FFFFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1)&0x00FFFFFF;
        printf("[AutoTest][AUDIO][FrameCount][%d]\n",     tmp_L);
        printf("[AutoTest][AUDIO][ErrorCount][%d]\n",     tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
        printf("[AutoTest][AUDIO][SampleRate][%d]\n",     tmp_L);
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MODE, ADEC1);
        printf("[AutoTest][AUDIO][ACMod][%d]\n",     tmp_L);
    }

    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_mmFileAVsync) != 0)
    {
        sysTime = MsOS_GetSystemTime();
        printf("[%08ld](%03ld):", sysTime, sysTime - preSysTime);
        preSysTime = sysTime;

        mmFile_APTS = HAL_MAD_GetCommInfo(Audio_Comm_infoType_1ms_PTS);
        printf("APTS:%07ld(%03ld)|", mmFile_APTS, mmFile_APTS - prv_mmFile_APTS);
        prv_mmFile_APTS = mmFile_APTS;

        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC1)&0xFFFF;
        printf("MM:%04X|", tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ACCUM_ES_BYTECNT, ADEC1)&0xFFFF;
        printf("ES=%04X(%04X),", ES_Lvl, tmp_L);

        //Eric.C need to refine
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCMISR_EMPTY_CNT, ADEC1)&0xFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ELAPSED_PCM_CNT, ADEC1)&0xFFFFFF;
        printf("PCM=%04X(%06X,%02X)|", PCM_Lvl, tmp_H, tmp_L);

        tmp_L = (MS_U32)HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD);
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_RECEIVE_STOP_CNT, ADEC1)&0x00FF;
        printf("Cmd=%02X,Stop=%02X,",  tmp_L, tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAYSMPFLAG, ADEC1)&0x1;
        printf("play=%X", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNTH_RATE, ADEC1)&0xFFFF;
        printf("<%04X>,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_STATE, ADEC1)&0x0F;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_FREERUN_TYPE, ADEC1)&0x0F;
        printf("State=%X(%X)|", tmp_L, tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1)&0x0000FFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1)&0x0000FFFF;
        printf("frmCnt:%04X,",     tmp_L);
        printf("%04X,",    tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SKIP_FRMCNT, ADEC1)&0x0000FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_REPT_FRMCNT, ADEC1)&0x0000FF;
        printf("%02X,",    tmp_L);
        printf("%02X|",    tmp_H);

        //tmp_L = HAL_MAD_Read_DSP_sram(DSP1DmAddr_sys_DecStatus+SYS_DECSTATUS_ID, DSP_MEM_TYPE_DM)&0x00FF;
        //printf("DEC_ID=%02X,", tmp_L);

        //tmp_L = HAL_MAD_Read_DSP_sram(DSP1DmAddr_sys_DecStatus+SYS_DECSTATUS_DEC_TYPE, DSP_MEM_TYPE_DM)&0x00FF;
        //printf("Type=%02X,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAY_STATE, ADEC1)&0x00FFFF;
        printf("pState=%04X,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CALLING_CNT, ADEC1)&0x0000FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNC_MISS_CNT, ADEC1)&0x00000FFF;
        printf("Call:%02X,",    tmp_L);
        printf("Miss:%03X\n",  tmp_H);
    }
//#endif
}


#ifndef MSOS_TYPE_NUTTX
MS_BOOL HAL_AUDIO_ResetTeeInfoShmToDefault(void)
{
    if (pAudioTeeInfoShm == NULL)
    {
        return FALSE;
    }
    memset(pAudioTeeInfoShm, 0x00, sizeof(AUDIO_TEE_INFO_SHARE_MEM));
    audio_tee_enabled = g_bAudioTeeEnabled;
    audio_tee_mbx_initialized = g_bAudioTeeMbxInitialized;
    dec_dsp_secure_tee_accessed = g_bDecDspSecureTeeAccessed;
    snd_dsp_secure_tee_accessed = g_bSndDspSecureTeeAccessed;
    return TRUE;
}

AUDIO_TEE_INFO_SHARE_MEM *HAL_AUDIO_AllocateTeeInfoShm(void)
{
#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
    MS_U32 u32ShmId = 0;
    MS_U32 u32Addr = 0;
    MS_U32 u32BufSize = 0;

    if (MsOS_SHM_GetId((MS_U8 *)AUDIO_TEE_INFO_SHM_NAME, sizeof(AUDIO_TEE_INFO_SHARE_MEM), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY) == TRUE)
    {
        pAudioTeeInfoShm = (AUDIO_TEE_INFO_SHARE_MEM *) u32Addr;

        if (pAudioTeeInfoShm->g_u32ClientCounter == 0)
        {
            HALAUDIO_PRINT("reset Audio TEE Info SHM data ...\n");
            HAL_AUDIO_ResetTeeInfoShmToDefault();
        }
    }
    else
    {
        if (MsOS_SHM_GetId((MS_U8 *)AUDIO_TEE_INFO_SHM_NAME, sizeof(AUDIO_TEE_INFO_SHARE_MEM), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE) == TRUE)
        {
            HALAUDIO_PRINT("Audio TEE Info SHM is created...\n");
            pAudioTeeInfoShm = (AUDIO_TEE_INFO_SHARE_MEM *) u32Addr;

            HALAUDIO_PRINT("reset Audio TEE Info SHM data ...\n");
            HAL_AUDIO_ResetTeeInfoShmToDefault();
        }
        else
        {
            HALAUDIO_ERROR("SHM allocation for Audio TEE Info failed!\n");
            return NULL;
        }
    }

    if (g_bAudioTeeInfoShmInitFlag == FALSE)
    {
        g_bAudioTeeInfoShmInitFlag = TRUE;
        pAudioTeeInfoShm->g_u32ClientCounter++;
    }
#else
    pAudioTeeInfoShm = &gAudioTeeInfoShm;

    if (g_bAudioTeeInfoShmInitFlag == FALSE)
    {
        HALAUDIO_PRINT("Audio TEE Info SHM is created...\n");
        g_bAudioTeeInfoShmInitFlag = TRUE;

        HAL_AUDIO_ResetTeeInfoShmToDefault();
        pAudioTeeInfoShm->g_u32ClientCounter++;
    }
#endif
    return pAudioTeeInfoShm;
}

MS_BOOL HAL_AUDIO_DeAllocateTeeInfoShm(void)
{
    AUDIO_TEE_INFO_SHARE_MEM * pAUDIOShared = NULL;

#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
    MS_U32 u32ShmId = 0;
    MS_U32 u32Addr = 0;
    MS_U32 u32BufSize = 0;

    if (g_bAudioTeeInfoShmInitFlag == TRUE)
    {
        g_bAudioTeeInfoShmInitFlag = FALSE;

        if (MsOS_SHM_GetId((MS_U8 *)AUDIO_TEE_INFO_SHM_NAME, sizeof(AUDIO_TEE_INFO_SHARE_MEM), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY) == FALSE)
        {
            HALAUDIO_ERROR("%s() : No AUDIO TEE Info SHM is created!\n", __FUNCTION__);
            return FALSE;
        }

        pAUDIOShared = (AUDIO_TEE_INFO_SHARE_MEM *)u32Addr;
        pAUDIOShared->g_u32ClientCounter--;

        if (pAUDIOShared->g_u32ClientCounter == 0)
        {
            HALAUDIO_PRINT("free Audio TEE Info SHM data ...\n");

#if defined(MSOS_TYPE_LINUX)
            if (MsOS_SHM_FreeId((MS_U8 *)AUDIO_TEE_INFO_SHM_NAME, u32ShmId) == TRUE)
            {
                HALAUDIO_PRINT("Audio TEE Info SHM data is freed\n");
            }
            else
            {
                HALAUDIO_ERROR("%s() : AUDIO TEE Info SHM de-allocation failed!\n", __FUNCTION__);
                return FALSE;
            }
#endif
        }
    }
#else
    pAUDIOShared = &gAudioTeeInfoShm;

    if (g_bAudioTeeInfoShmInitFlag == TRUE)
    {
        HALAUDIO_PRINT("free Audio TEE Info SHM data ...\n");
        g_bAudioTeeInfoShmInitFlag = FALSE;

        memset(pAUDIOShared, 0x00, sizeof(AUDIO_TEE_INFO_SHARE_MEM));
    }
#endif

    return TRUE;
}

MS_BOOL HAL_AUDIO_RegisterMBX(void)
{
#ifdef NoMBX
    MS_U8 ClassNum=0;
    MBX_Result result;
    MBX_CPU_ID eHKCPU;
    MS_U32 u32TimeoutMillSecs = 10000;

    if (pAudioTeeInfoShm == NULL)
        return FALSE;
    if (audio_tee_mbx_initialized == TRUE)
        return TRUE;

#if 1
    eHKCPU = E_MBX_CPU_MIPS;
    if ( E_MBX_SUCCESS != MApi_MBX_Init(eHKCPU, E_MBX_ROLE_HK, u32TimeoutMillSecs))
    {
        DBG_AUDIO_ERROR("Error> MBX init failed !!\n");
        return FALSE;
    }
    else
    {
        MApi_MBX_Enable(TRUE);
    }
#endif

    if (E_MBX_SUCCESS != MApi_MBX_QueryDynamicClass (E_MBX_CPU_MIPS_VPE1, "MAD", (MS_U8 *)&ClassNum))
    {
        DBG_AUDIO_ERROR("MAD MApi_MBX_QueryDynamicClass fail\n");

        return FALSE;
    }
    result = MApi_MBX_RegisterMSG(ClassNum, 10);
    if (( E_MBX_SUCCESS != result) && ( E_MBX_ERR_SLOT_AREADY_OPENNED != result ))
    {
        DBG_AUDIO_ERROR("HAL_AUDIO_RegisterMBX fail");
        return FALSE;
    }
    else
    {
        TEE_MBX_MSG_CLASS_SET(ClassNum);
        DBG_AUDIO("HAL_AUDIO_RegisterMBX ok");
        return TRUE;
    }
#else
    return FALSE;
#endif
}

TEE_TO_REE_MBX_ACK_MSG_TYPE HAL_AUDIO_SendMBXMsg(REE_TO_TEE_MBX_MSG_TYPE msg_type)
{
    MS_U8 u8Index;
#ifdef NoMBX
    MBX_Result result;
#endif
    REE_TO_TEE_MBX_MSG_TYPE msg_type1 = REE_TO_TEE_MBX_MSG_NULL;

    AUDIO_TEE_INFO_SHM_CHECK_NULL;

    if (audio_tee_enabled == FALSE)
    {
        return TEE_TO_REE_MBX_ACK_MSG_NO_TEE;
    }
    if ((msg_type & REE_TO_TEE_MBX_MSG_TYPE_DEC) == (REE_TO_TEE_MBX_MSG_TYPE_DEC))
    {
        if (dec_dsp_secure_tee_accessed == FALSE)
        {
            return TEE_TO_REE_MBX_ACK_MSG_NO_TEE;
        }
        else
        {
            msg_type1 = msg_type & (~REE_TO_TEE_MBX_MSG_TYPE_DEC);
        }
    }
    else if ((msg_type & REE_TO_TEE_MBX_MSG_TYPE_SE) == (REE_TO_TEE_MBX_MSG_TYPE_SE))
    {
        if (snd_dsp_secure_tee_accessed == FALSE)
        {
            return TEE_TO_REE_MBX_ACK_MSG_NO_TEE;
        }
        else
        {
            msg_type1 = msg_type & (~REE_TO_TEE_MBX_MSG_TYPE_SE);
        }
    }
    else if ((msg_type & REE_TO_TEE_MBX_MSG_TYPE_GENERAL) == (REE_TO_TEE_MBX_MSG_TYPE_GENERAL))
    {

        msg_type1 = msg_type & (~REE_TO_TEE_MBX_MSG_TYPE_GENERAL);
    }

    if (audio_tee_mbx_initialized == FALSE)
    {
        return TEE_TO_REE_MBX_ACK_MSG_INVALID;
    }
    REE_TO_TEE_MBX_MSG_INIT;
    MBX_MSG_SET_CLASSID(ree_to_tee_mbx_msg, tee_mbx_msg_class);
    MBX_MSG_SET_CMD(ree_to_tee_mbx_msg, msg_type1);

#ifdef NoMBX
    result = MApi_MBX_SendMsg(&(ree_to_tee_mbx_msg));
    if (E_MBX_SUCCESS!= result)
    {
        return TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
    }
#endif
    // Receive Reply ACK from TEE side.
    memset(&(tee_to_ree_mbx_msg), 0, sizeof(MBX_Msg));
    MBX_MSG_SET_CLASSID(tee_to_ree_mbx_msg, tee_mbx_msg_class);

#ifdef NoMBX
    do
    {
        result = MApi_MBX_RecvMsg(tee_mbx_msg_class, &(tee_to_ree_mbx_msg), 2000, MBX_CHECK_INSTANT_MSG);
    } while(E_MBX_SUCCESS != result);
#endif

    u8Index = tee_to_ree_mbx_msg.u8Index;
    DBG_AUDIO("Recv TEE Ack Msg OK cmd:%x\n", u8Index);

    if (TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL == u8Index)
    {
        DBG_AUDIO_ERROR("RecvMsg TEE_TO_REE_MBX_MSG_ACK_FAIL..\n");
        return TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
    }
    if (TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS == u8Index)
    {
        DBG_AUDIO("RecvMsg TEE_TO_REE_MBX_MSG_ACK_SUCCESS..\n");
    }

    return TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS;
}
#else
static void _MAD_Proc(void)
{
    MS_U32              u32Events;
    MS_U8 u8Index = 0;
    MBX_Result result;

    DBG_AUDIO("_MAD_Proc...........\n");

    while (1)
    {
        MsOS_WaitEvent(_s32MadEventId, 1, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        u8Index = msg.u8Index;

        memset(&TeeToReeMbxAckMsg, 0, sizeof(MBX_Msg));
        TEE_TO_REE_MBX_MSG_INIT(TeeToReeMbxAckMsg);
        MBX_MSG_SET_CLASSID(TeeToReeMbxAckMsg, msg.u8MsgClass);
        MBX_MSG_SET_CMD(TeeToReeMbxAckMsg, TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS);

        switch ( u8Index )
        {
            case REE_TO_TEE_MBX_MSG_HAL_AUDSP_DspLoadCode:
            {
                if (HAL_AUDSP_DspLoadCode(msg.u8Parameters[0]) == FALSE)
                    MBX_MSG_SET_CMD(TeeToReeMbxAckMsg, TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL);

                break;
            }

            case REE_TO_TEE_MBX_MSG_HAL_AUDIO_SetDspBaseAddr:
            {
                MS_U32 u32Bin_Base_Address, u32Mad_Base_Buffer_Adr;
                u32Bin_Base_Address = (((MS_U32)(msg.u8Parameters[1])) | ((MS_U32)(msg.u8Parameters[2]) << 8) | ((MS_U32)(msg.u8Parameters[3]) << 16) | ((MS_U32)(msg.u8Parameters[4]) << 24));
                u32Mad_Base_Buffer_Adr = (((MS_U32)(msg.u8Parameters[5])) | ((MS_U32)(msg.u8Parameters[6]) << 8) | ((MS_U32)(msg.u8Parameters[7]) << 16) | ((MS_U32)(msg.u8Parameters[8]) << 24));
                HAL_AUDIO_SetDspBaseAddr(msg.u8Parameters[0], u32Bin_Base_Address, u32Mad_Base_Buffer_Adr);
                break;
            }

            default:
            {
                MBX_MSG_SET_CMD(TeeToReeMbxAckMsg, TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL);
                break;
            }
        }

#ifdef NoMBX
        result = MApi_MBX_SendMsg(&TeeToReeMbxAckMsg);
        if (E_MBX_SUCCESS != result)
            DBG_AUDIO("MAD Reply Ack fail classid: %x cmd:%x \n", msg.u8MsgClass, TeeToReeMbxAckMsg.u8Index);
        else
            DBG_AUDIO("MAD Reply Ack ok classid:%x cmd:%x\n", msg.u8MsgClass, TeeToReeMbxAckMsg.u8Index);
#endif
    }
}

static void HAL_AUDIO_HandleDynamicMBX(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue)
{
    if(pMsg==NULL)
    {
        DBG_AUDIO("pMsg is a null pointer\n");
        return;
    }
    if (_s32MadEventId < 0)
    {
        DBG_AUDIO("MAD EventGroup Id is not valid...\n");
        return;
    }
    memcpy(&msg, pMsg, sizeof(MBX_Msg));
    MsOS_SetEvent(_s32MadEventId, 1);
    return;
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_update_DspUsage()
/// @brief \b Function \b Description:  update DSP resource status when select input source
/// @param <IN>        \b AUDIO_SOURCE_INFO_TYPE : select input source type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_update_DspUsage(AUDIO_SOURCE_INFO_TYPE eSourceType)
{
    HALAUDIO_CHECK_SHM_INIT;

    /* same input source, not update */
    if ( g_AudioVars2->eMainSourceType == eSourceType )
    {
        return;
    }

    OS_OBTAIN_MUTEX(_s32MutexLoadCode, MSOS_WAIT_FOREVER);

    HALAUDIO_PRINT ("%s: PrevSource %d NowSource %d NewSource %d !!\r\n", __FUNCTION__, g_AudioVars2->ePrevSourceType, g_AudioVars2->eMainSourceType, eSourceType);

    /* release prev DSP resource */
    if ( (g_AudioVars2->eMainSourceType == E_AUDIO_INFO_DTV_IN && eSourceType != E_AUDIO_INFO_DTV_IN) )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID1].bDecInUsed = FALSE;
        g_AudioVars2->DspUsage[AU_DEC_ID1].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID1].eUsageType = AUDIO_USAGE_TYPE_NONE;
        HALAUDIO_PRINT ("%s: Release DEC DSP from DTV !!\r\n", __FUNCTION__);
    }
    else if ( (g_AudioVars2->eMainSourceType == E_AUDIO_INFO_ATV_IN && eSourceType != E_AUDIO_INFO_ATV_IN) )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID3].bDecInUsed = FALSE;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eUsageType = AUDIO_USAGE_TYPE_NONE;
        HALAUDIO_PRINT ("%s: Release SND DSP from ATV !!\r\n", __FUNCTION__);
    }
    else if ( (g_AudioVars2->eMainSourceType == E_AUDIO_INFO_HDMI_IN && eSourceType != E_AUDIO_INFO_HDMI_IN) )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID3].bDecInUsed = FALSE;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eUsageType = AUDIO_USAGE_TYPE_NONE;
        HALAUDIO_PRINT ("%s: Release SND DSP from HDMI !!\r\n", __FUNCTION__);
    }

    g_AudioVars2->ePrevSourceType = g_AudioVars2->eMainSourceType;
    g_AudioVars2->eMainSourceType = eSourceType;

    /* lock main input source DSP resource */
    if ( g_AudioVars2->eMainSourceType == E_AUDIO_INFO_DTV_IN )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID1].bDecInUsed = TRUE;
        g_AudioVars2->DspUsage[AU_DEC_ID1].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID1].eUsageType = AUDIO_USAGE_TYPE_DTV;
        HALAUDIO_PRINT ("%s: Alloc DEC DSP for DTV !!\r\n", __FUNCTION__);
    }
    else if ( g_AudioVars2->eMainSourceType == E_AUDIO_INFO_ATV_IN )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID3].bDecInUsed = TRUE;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eUsageType = AUDIO_USAGE_TYPE_ATV;
        HALAUDIO_PRINT ("%s: Alloc DEC DSP for ATV !!\r\n", __FUNCTION__);
    }
    else if ( g_AudioVars2->eMainSourceType == E_AUDIO_INFO_HDMI_IN )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID3].bDecInUsed = TRUE;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eUsageType = AUDIO_USAGE_TYPE_HDMI;
        HALAUDIO_PRINT ("%s: Alloc DEC DSP for HDMI !!\r\n", __FUNCTION__);
    }

    HALAUDIO_PRINT ("%s: update PrevSource %d NewSource %d !!\r\n", __FUNCTION__, g_AudioVars2->ePrevSourceType, g_AudioVars2->eMainSourceType);

    OS_RELEASE_MUTEX(_s32MutexLoadCode);
}

Audio_id HAL_AUDIO_Convert_DecId_to_ADECId(AUDIO_DEC_ID dec_id)  //temp
{
    Audio_id ADEC_id;
    switch(dec_id)
    {
        case  AU_DEC_ID1:
            ADEC_id = ADEC1;
            break;

        case AU_DEC_ID2:
        case AU_DEC_ID3:
            ADEC_id = ADEC2;
            break;

        case AU_DEC_INVALID:
        case AU_DEC_MAX:
        default:
            printf("[Error] Can't convert...\n");
            ADEC_id = ADEC1;
            break;
        }
    return ADEC_id;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetCaps()
/// @brief \b Function \b Description: Get Audio Capabilities
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b MS_U32 * : Audio Capabilites pointer
/// @param <OUT>        \b MS_BOOL : return TRUE if success, else return FALSE
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_GetCaps(AUDIO_DEC_ID DecId, MS_U32 *pCaps)
{
    AUDIO_CAPABILITIES *pCapsTmp = NULL;
    MS_U32 u32AudioCapsStructSize = sizeof(AUDIO_CAPABILITIES);

    HALAUDIO_CHECK_SHM_INIT;

    if (pCaps == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pCaps should not be NULL!\n", __FUNCTION__);
        return FALSE;
    }

    if ((DecId <= AU_DEC_INVALID) || (DecId >= AU_DEC_MAX) || (DecId == AU_DEC_ID2))
    {
        HALAUDIO_ERROR("%s: Error! Invalid DecId (%d)!\n", __FUNCTION__, DecId);
        return FALSE;
    }
    else if ((g_AudioVars2->AudioCaps[AU_DEC_ID1].u8MaxDecoderNum == 1) && (DecId > AU_DEC_ID1))
    {
        HALAUDIO_ERROR("%s: Error! DecId (%d) is greater than max decoder number (%d)!\n", __FUNCTION__, DecId, g_AudioVars2->AudioCaps[AU_DEC_ID1].u8MaxDecoderNum);
        return FALSE;
    }

    pCapsTmp = (AUDIO_CAPABILITIES *)pCaps;

    if (pCapsTmp->u32AudioCapsVersion == 0)
    {
        HALAUDIO_ERROR("%s: Error! Invalid version number (%lu) from function caller!\n", __FUNCTION__, pCapsTmp->u32AudioCapsVersion);
        return FALSE;
    }

    if (pCapsTmp->u32AudioCapsStructSize == 0)
    {
        HALAUDIO_ERROR("%s: Error! Invalid structure size (%lu) from function caller!\n", __FUNCTION__, pCapsTmp->u32AudioCapsStructSize);
        return FALSE;
    }

    if (pCapsTmp->u32AudioCapsVersion != g_AudioVars2->AudioCaps[DecId].u32AudioCapsVersion)
    {
        HALAUDIO_ERROR("%s: Warning! function caller's version (%lu) is not matched to driver's version (%lu) at DecId (%d)\n", __FUNCTION__, pCapsTmp->u32AudioCapsVersion, g_AudioVars2->AudioCaps[DecId].u32AudioCapsVersion, DecId);

        u32AudioCapsStructSize = (u32AudioCapsStructSize <= pCapsTmp->u32AudioCapsStructSize) ? u32AudioCapsStructSize : pCapsTmp->u32AudioCapsStructSize;
    }

    memcpy((void *)pCapsTmp, (void *)&g_AudioVars2->AudioCaps[DecId], u32AudioCapsStructSize);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Set_DecimationMode()
/// @brief \b Function \b Description: HAL_AUDIO_Set_Decimation register
/// @param <IN>        \b AUDIO_OUTPUT_TYPE Output_Type: Audio Output type
/// @param <IN>        \b MS_BOOL Enable : Audio ID: ADEC ID
////////////////////////////////////////////////////////////////////////////////
void  HAL_AUDIO_Set_DecimationMode(AUDIO_OUTPUT_TYPE Output_Type, Audio_id ID)
{

    switch(Output_Type)
    {
        case AUDIO_HDMI_OUTPUT:  //high rate nonPCM application need decimation
            if( (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_HDMITx_SMP_RATE, ID) > 48000) && (HAL_AUDIO_ReadByte(REG_M2D_MAILBOX_SPDIF_CTRL+1)&0x20))
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x04, 0x04);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x10, 0x10);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x04, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x10, 0x00);
            }
            break;

        case AUDIO_HDMI_ARC_OUTPUT:
            if( (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_HDMITx_SMP_RATE, ID) > 48000) && (HAL_AUDIO_ReadByte(REG_M2D_MAILBOX_SPDIF_CTRL)&0x02))
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x02, 0x02);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x08, 0x08);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x02, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x08, 0x00);
            }
            break;

        case AUDIO_SPDIF_OUTPUT:
            if( (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SPDIFTx_SMP_RATE, ID) > 48000) && (HAL_AUDIO_ReadByte(REG_M2D_MAILBOX_SPDIF_CTRL)&0x02))
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x01, 0x01);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x04, 0x04);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x01, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x04, 0x00);
            }
            break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_CheckPlayDone()
/// @brief \b Function \b Description: Check file play done. If file play done, will return TRUE
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_BOOL : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_CheckPlayDone(void)
{
    if (((HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_okFrmCnt) !=0) ||
          (HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_errFrmCnt) !=0)) &&
        (HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_pcmBuf_currLevel) < AUD_R2_DMA_BURST_BYTES))
    {
        CheckPlayDoneCnt = CheckPlayDoneCnt + 1;

        if (CheckPlayDoneCnt == 3)
        {
            CheckPlayDoneCnt = 0;
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        CheckPlayDoneCnt = 0;
        return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_CheckVersionInfo()
/// @brief \b Function \b Description: Check Version info
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_BOOL : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_CheckVersionInfo(void)
{
    HALAUDIO_PRINT("\r\n \033[1;32m==========Audio R2 & DSP Version Check==================\033[0m");
    HALAUDIO_PRINT("\r\n \033[1;32m         DEC-R2:0x%06X     SND-R2:%06X    DSP System:0x%06X    \033[0m \r\n", DEC_R2_VERSION, SND_R2_VERSION, system_version_num);

    if(HAL_AUDIO_AbsReadReg(MBOX_D2M_03AA) != DEC_R2_VERSION) // Compare DEC R2 Version with Mail Box
    {
        HALAUDIO_PRINT("\r\n \033[1;32m        Audio  DEC-R2  Version  Mismatch!!!!!!!    \033[0m \r\n");
    }
    return TRUE;
}

MS_S32 HAL_AUDIO_PCM_Dump_Setting(MS_U32 u32alue, MS_U32 *pData)
{
    return 0;
}

MS_S32 HAL_AUDIO_PCM_Check_U32_Param_Valid(MS_U32 u32alue, MS_U32 *pData, MS_U32 u32Count)
{
    MS_U32 u32Loop = 0;
    MS_S32 s32Ret = -A_EINVAL;

    if (u32Count == 0)
    {
        return -A_EINVAL;
    }

    if (pData != NULL)
    {
        for (u32Loop = 0; u32Loop < u32Count ; u32Loop++)
        {
            if (pData[u32Loop] == 0)
            {
                break;
            }

            if (u32alue == pData[u32Loop])
            {
                s32Ret = 0;
                break;
            }
        }
    }
    else
    {
        s32Ret = -A_EFAULT;
    }

    return s32Ret;
}

#if AUDIO_HW_DMA_READER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Divisor = 0;
    MS_U16 u16Synthrate = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    /*
     * synthesizer Rate = 216MHz / divisor(1, 2, 4 or 8) * 1024 / 256 / sampling rate(32Khz, 44.1KHz or 48KHz)
     */
    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Divisor = 2;
            u16Synthrate = 0x6978;
            break;
        }

        case 11025:
        {
            u16Divisor = 2;
            u16Synthrate = 0x4C87;
            break;
        }

        case 12000:
        {
            u16Divisor = 2;
            u16Synthrate = 0x4650;
            break;
        }

        case 16000:
        {
            u16Divisor = 1;
            u16Synthrate = 0x6978;
            break;
        }

        case 22050:
        {
            u16Divisor = 1;
            u16Synthrate = 0x4C87;
            break;
        }

        case 24000:
        {
            u16Divisor = 1;
            u16Synthrate = 0x4650;
            break;
        }

        case 32000:
        {
            u16Divisor = 0;
            u16Synthrate = 0x6978;
            break;
        }

        case 44100:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4C87;
            break;
        }

        case 48000:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4650;
            break;
        }

        case 96000:
        {
            u16Divisor = 0;
            u16Synthrate = 0x2328;
            break;
        }

        default:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4650;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    /* enable DMA synthesizer */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CLK_CFG0, 0x20, 0x20);

    /* Configure REG_SEL_CLK_214MHZ_DMA_RD_FIX_SYNTH &  synthesizer N.F. */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_CLK_CFG6, 0x6000, (u16Divisor << 13));
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_SYNTH_NF, 0xFFFF, u16Synthrate);

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > (HW_DMA_RDR1_BUF_SIZE / 2))
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%lu) is required! Please check the settings: duration %lu, sample rate %lu\n", __FUNCTION__, u32TargetBufferSize, pPcmInfo->u32BufferDuration, pPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_RDR1_BUF_SIZE / 2;
        pPcmInfo->u32BufferDuration = (u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8));
        HALAUDIO_ERROR("%s: Error! Force duration to %lu, which is the maximun value\n", __FUNCTION__, pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    u32TargetBufferSize = u32TargetBufferSize * 2; /* for HW DMA Reader specification */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_3, 0xFFFF, u32TargetBufferSize / BYTES_IN_MIU_LINE);

    /* set overrun & underrun threshold */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_4, 0xFFFF, u32TargetBufferSize - 12);
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_5, 0xFFFF, 0x0012);

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, HW_DMA_RDR1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* clear engine's write pointer */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_1, 0xFFFF, 0x0000);

    /* reset & start engine */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0xFFFE, 0x6402);
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0xFFFE, 0x2402);

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(%lu, %d)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%lu, %u)!\n", __FUNCTION__, pUserPcmInfo->u32StructSize, sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%lu)!\n", __FUNCTION__, pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%lu)!\n", __FUNCTION__, pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > (HW_DMA_RDR1_BUF_SIZE / 2))
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%lu) is required! Please check the settings: duration %lu, sample rate %lu\n", __FUNCTION__, u32TargetBufferSize, pUserPcmInfo->u32BufferDuration, pUserPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_RDR1_BUF_SIZE / 2;
        pUserPcmInfo->u32BufferDuration = (u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8));
        HALAUDIO_ERROR("%s: Error! Force duration to %lu, which is the maximun value\n", __FUNCTION__, pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + HW_DMA_RDR1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* set PCM buffer address */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_DMA_RD_CTRL_7, 0xFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) & 0xFF)); /* RD_BASE_ADDR[7:0] */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_2, 0xFFFF, (((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 8) & 0xFFFF)); /* RD_BASE_ADDR[23:8] */
    HAL_AUDIO_WriteMaskByte((REG_AUDIO_DMA_RD_CTRL_7 + 1), 0x0F, (((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 24) & 0x0F)); /* RD_BASE_ADDR[27:24] */

    /* set SEL_CLK_DMA_READER */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CLK_CFG6, 0x07, 0x04);

    /* apply setting */
    HAL_AUDIO_PCM_HwDma_Reader1_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_HwDma_Reader1_Restart();

    /* TODO : force channel8 to HW DMA Reader1, it's a hardcode */
    HAL_AUDIO_WriteMaskByte(0x2CB0, 0x07, 0x04);
    HAL_AUDIO_WriteMaskByte(0x2C6B, 0xFF, 0x8F);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Reader1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_HwDma_Reader1_Apply_Setting();
            HAL_AUDIO_PCM_HwDma_Reader1_Restart();
        }

        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0001, 0x0001);
        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0100, 0x0100);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0001, 0x0000);
        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0100, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_EASING:
        {
            //FIX ME: HW DMA reader channel fixed in CH8 current status
            //FIX ME: HW DMA reader use Easing Gain A

            memcpy(&pPcmInfo->tEaseParam, pData, sizeof(AUDIO_EASE_PARAM_t));

            //printf("u32FadeDuration = %d\r\n", pPcmInfo->tEaseParam.u32FadeDuration);
            //printf("u32FadeTargetGain = %d\r\n", pPcmInfo->tEaseParam.u32FadeTargetGain);
            //printf("u32FadeType = %d\r\n", pPcmInfo->tEaseParam.eFadeType);

            MS_U32 u32TargetVolumeHi;
            MS_U32 u32TargetVolumeLow;
            HAL_SOUND_EnableEaseVol(TRUE);

            u32TargetVolumeHi =  (pPcmInfo->tEaseParam.u32FadeTargetGain & 0x7F0000) >> 16;
            u32TargetVolumeLow = pPcmInfo->tEaseParam.u32FadeTargetGain & 0xFFFF;

            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Target_Channel_A, AUDIO_PATH_7, 0);
            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Type_A, pPcmInfo->tEaseParam.eFadeType, 0);
            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Duration_A, pPcmInfo->tEaseParam.u32FadeDuration, 0);
            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Gain_A, u32TargetVolumeHi, u32TargetVolumeLow);
            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, (unsigned int)u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%u)!\n", __FUNCTION__, (unsigned int)u32Channel);
                return -A_EINVAL;
            }

            pPcmInfo->u32Channel = u32Channel;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%u)!\n", __FUNCTION__, (unsigned int)u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, (unsigned int)u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, (unsigned int)u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, (unsigned int)u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0010, 0x0010);
            pPcmInfo->u32BufferLevel = (HAL_AUDIO_ReadReg(REG_AUDIO_DMA_RD_CTRL_6) * BYTES_IN_MIU_LINE) / 2;
            HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0010, 0x0000);
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;
            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_EASING:
        {
            //FIX ME: HW DMA reader channel fixed in CH8 current status
            //FIX ME: HW DMA reader use Easing Gain A

            *((MS_U32 *)pData) = (MS_U32)HAL_SND_GetParam(Sound_GET_PARAM_Ease_Gain_A, 0);

            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = (HAL_AUDIO_ReadReg(REG_AUDIO_DMA_RD_CTRL_3) * BYTES_IN_MIU_LINE) / 2; /* return valid size */
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_HwDma_Reader1_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSampleCount = 0;
    MS_U32 u32Loop = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_HwDma_Reader1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    u32PcmLevel = u32PcmLevel * 2;
    if ((u32PcmLevel / BYTES_IN_MIU_LINE) <= 6)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);

        u32PcmLevel = 0;
        HAL_AUDIO_PCM_HwDma_Reader1_Stop();
        HAL_AUDIO_PCM_HwDma_Reader1_Restart();
        HAL_AUDIO_PCM_HwDma_Reader1_Start();
    }

    u32RequestSize = u32Size * 2;
    u32RequestSampleCount = u32Size / 2;

    HAL_AUDIO_PCM_HwDma_Reader1_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);
    u32BufferSize = u32BufferSize * 2;

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        for (u32Loop = 0; u32Loop < u32RequestSampleCount; u32Loop++)
        {
            *pPCM->tPcmBufferInfo.pWritePtr++ = 0;
            *pPCM->tPcmBufferInfo.pWritePtr++ = *pBufTmp++; /* L low, R low, L low, R low, ... */
            *pPCM->tPcmBufferInfo.pWritePtr++ = *pBufTmp++; /* L high, R high, L high, R high, ... */
            *pPCM->tPcmBufferInfo.pWritePtr++ = 0;

            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }
        }

        /* flush MIU */
        MsOS_FlushMemory();

        /* update copied size to engine */
        u32RequestSize += pPCM->tPcmBufferInfo.u32RemainSize;
        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_1, 0xFFFF, (u32RequestSize/BYTES_IN_MIU_LINE));
        pPCM->tPcmBufferInfo.u32RemainSize = u32RequestSize % BYTES_IN_MIU_LINE;
#ifdef MSOS_TYPE_LINUX_KERNEL
        udelay(50);
#else
        AUDIO_DELAY1US(50);
#endif

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Reader1_Restart();
        HAL_AUDIO_PCM_HwDma_Reader1_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_HW_DMA_READER2_SUPPORT
MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Divisor = 0;
    MS_U16 u16Synthrate = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    /*
     * synthesizer Rate = 216MHz / divisor(1, 2, 4 or 8) * 1024 / 256 / sampling rate(32Khz, 44.1KHz or 48KHz)
     */
    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Divisor = 2;
            u16Synthrate = 0x6978;
            break;
        }

        case 11025:
        {
            u16Divisor = 2;
            u16Synthrate = 0x4C87;
            break;
        }

        case 12000:
        {
            u16Divisor = 2;
            u16Synthrate = 0x4650;
            break;
        }

        case 16000:
        {
            u16Divisor = 1;
            u16Synthrate = 0x6978;
            break;
        }

        case 22050:
        {
            u16Divisor = 1;
            u16Synthrate = 0x4C87;
            break;
        }

        case 24000:
        {
            u16Divisor = 1;
            u16Synthrate = 0x4650;
            break;
        }

        case 32000:
        {
            u16Divisor = 0;
            u16Synthrate = 0x6978;
            break;
        }

        case 44100:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4C87;
            break;
        }

        case 48000:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4650;
            break;
        }

        default:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4650;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    /* Configure REG_SEL_CLK_214MHZ_DMA_RD_FIX_SYNTH &  synthesizer N.F. */
    HAL_AUDIO_AbsWriteReg(NewDMAreader_ctrl, 0x0000);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_Enable_ctrl, 0x0030, (u16Divisor << 4));
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_Syth, 0xFFFF, u16Synthrate);

    /* enable DMA synthesizer */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_Enable_ctrl, 0x0145, 0x0145);

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > HW_DMA_RDR2_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, (unsigned int)u32TargetBufferSize, (unsigned int)pPcmInfo->u32BufferDuration, (unsigned int)pPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_RDR2_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_DRAM_size, 0xFFFF, u32TargetBufferSize / BYTES_IN_MIU_LINE);

    /* set overrun & underrun threshold */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_DRAM_underrun_threshold, 0xFFFF, 0x0012);

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, HW_DMA_RDR2_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* clear engine's write pointer */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0xFFFF, 0x0000);

    /* reset & start engine */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0xFFFF, 0x800B); //SW reset & enable DMA reader, MIU request
#ifdef MSOS_TYPE_LINUX_KERNEL
    udelay(50);
#else
    AUDIO_DELAY1US(50);
#endif
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0xFFFF, 0x000B); //SW reset & enable DMA reader, MIU request

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructSize, sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > HW_DMA_RDR2_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, (unsigned int)u32TargetBufferSize, (unsigned int)pUserPcmInfo->u32BufferDuration, (unsigned int)pUserPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_RDR2_BUF_SIZE;
        pUserPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);

    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + HW_DMA_RDR2_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* set PCM buffer address */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_BaseAddress_Lo, 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) & 0xFFFF)); /* RD_BASE_ADDR[15:0] */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_BaseAddress_Hi, 0xFFFF, (((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 16) & 0xFFFF)); /* RD_BASE_ADDR[31:15] */

    /* apply setting */
    HAL_AUDIO_PCM_HwDma_Reader2_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_HwDma_Reader2_Restart();

    /* TODO : force channel6 to HW DMA Reader2, it's a hardcode */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0xFF, 0x90);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Reader2_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_HwDma_Reader2_Apply_Setting();
            HAL_AUDIO_PCM_HwDma_Reader2_Restart();
        }

        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0001, 0x0001);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0002, 0x0002);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0001, 0x0000);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0002, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, (unsigned int)u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%u)!\n", __FUNCTION__, (unsigned int)u32Channel);
                return -A_EINVAL;
            }

            pPcmInfo->u32Channel = u32Channel;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%u)!\n", __FUNCTION__, (unsigned int)u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, (unsigned int)u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, (unsigned int)u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, (unsigned int)u32Mute);
                s32Ret = -A_EINVAL;
            }

            if (s32Ret == 0)
            {
                HAL_SOUND_SetMute(AUDIO_PATH_HW_DMA_2, u32Mute);
            }

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32BufferLevel1 = 0;
            MS_U32 u32BufferLevel2 = 0;
            MS_U32 u32Timeout = 20;

            while (u32Timeout > 0)
            {
                u32BufferLevel1 = HAL_AUDIO_AbsReadReg(NewDMAreader_DRAM_levelcnt) * BYTES_IN_MIU_LINE;
                u32BufferLevel2 = HAL_AUDIO_AbsReadReg(NewDMAreader_DRAM_levelcnt) * BYTES_IN_MIU_LINE;

                if (u32BufferLevel1 == u32BufferLevel2)
                {
                    break;
                }

                u32Timeout--;

#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(1);
#else
                AUDIO_DELAY1US(1);
#endif
            }

            if (u32BufferLevel1 != u32BufferLevel2)
            {
                HALAUDIO_ERROR("%s: Error! fail to get correct buffer level (%u, %u)\n", __FUNCTION__, (unsigned int)u32BufferLevel1, (unsigned int)u32BufferLevel2);
            }

            pPcmInfo->u32BufferLevel = u32BufferLevel2;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = HAL_AUDIO_AbsReadReg(NewDMAreader_DRAM_size) * BYTES_IN_MIU_LINE;
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_HwDma_Reader2_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_HwDma_Reader2_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel == 0)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    HAL_AUDIO_PCM_HwDma_Reader2_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pWritePtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pPCM->tPcmBufferInfo.pWritePtr, (void *)pBufTmp, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pWritePtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update copied size to engine */
        u32RequestSize += pPCM->tPcmBufferInfo.u32RemainSize;
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_CPU_triggersize, 0xFFFF, u32RequestSize/BYTES_IN_MIU_LINE);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0010, 0x0000);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0010, 0x0010);
        pPCM->tPcmBufferInfo.u32RemainSize = u32RequestSize % BYTES_IN_MIU_LINE;

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Reader2_Restart();
        HAL_AUDIO_PCM_HwDma_Reader2_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_SW_DMA_READER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Synthrate_H = 0;
    MS_U16 u16Synthrate_L = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Synthrate_H = 0x6978;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 11025:
        {
            u16Synthrate_H = 0x4C87;
            u16Synthrate_L = 0xD634;
            break;
        }

        case 12000:
        {
            u16Synthrate_H = 0x4650;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 16000:
        {
            u16Synthrate_H = 0x34BC;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 22050:
        {
            u16Synthrate_H = 0x2643;
            u16Synthrate_L = 0xEB1A;
            break;
        }

        case 24000:
        {
            u16Synthrate_H = 0x2328;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 32000:
        {
            u16Synthrate_H = 0x1A5E;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 44100:
        {
            u16Synthrate_H = 0x1321;
            u16Synthrate_L = 0xF58D;
            break;
        }

        case 48000:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 96000:
        {
            u16Synthrate_H = 0x08CA;
            u16Synthrate_L = 0x0000;
            break;
        }

        default:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    pPcmInfo->u16Synth_H = u16Synthrate_H;
    pPcmInfo->u16Synth_L = u16Synthrate_L;

    //Need to fix. Current S/W DMA use DEC5, synth is controlled by 0x112C bank, and only output 2 channels...
    //HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_SynthFreq_H, 0xFFFF, u16Synthrate_H);
    //HAL_MAD2_Write_DSP_sram(DSP2DmAddr_swDmaRdr_synthFreq_L, u16Synthrate_L, DSP_MEM_TYPE_DM);

#ifdef MSOS_TYPE_LINUX_KERNEL
    udelay(50);
#else
    AUDIO_DELAY1US(50);
#endif

    /* switch to CH6 */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG, 0x0F,0x00);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0x1F,0x02);

    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > SW_DMA_RDR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%lu) is required! Please check the settings: duration %lu, sample rate %lu, channel %lu\n", __FUNCTION__, u32TargetBufferSize, pPcmInfo->u32BufferDuration, pPcmInfo->u32SampleRate, u32TargetChannel);
        u32TargetBufferSize = SW_DMA_RDR1_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * 2));
        HALAUDIO_ERROR("%s: Error! Force duration to %lu, which is the maximun value\n", __FUNCTION__, pPcmInfo->u32BufferDuration);
    }

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

#define TIMEOUT_CNT 50
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    MS_U32 u32Cnt;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, SW_DMA_RDR1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* clear engine's write pointer */
    HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_WtPtr, 0xFFFF, 0x0000);

    /* reset engine, no start engine here! */
    HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0xFFFF, 0x0001);

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    /* this is to ensure reset succeed*/
    for(u32Cnt = 0; u32Cnt < TIMEOUT_CNT; u32Cnt++)
    {
        if(HAL_AUDIO_ReadReg(S2M_MBOX_SW_DMA_READER_DDR_Level) == 0)
            break;
        AUDIO_DELAY1US(10);
    }

    if(u32Cnt == TIMEOUT_CNT)
    {
        HALAUDIO_ERROR("[%s][%d] Reset fail ! \n", __FUNCTION__, __LINE__);
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(%lu, %d)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%lu, %u)!\n", __FUNCTION__, pUserPcmInfo->u32StructSize, sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%lu)!\n", __FUNCTION__, pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%lu)!\n", __FUNCTION__, pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > SW_DMA_RDR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%lu) is required! Please check the settings: duration %lu, sample rate %lu, channel %lu\n", __FUNCTION__, u32TargetBufferSize, pUserPcmInfo->u32BufferDuration, pUserPcmInfo->u32SampleRate, u32TargetChannel);
        u32TargetBufferSize = SW_DMA_RDR1_BUF_SIZE;
        pUserPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + SW_DMA_RDR1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* apply setting */
    HAL_AUDIO_PCM_SwDma_Reader1_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_SwDma_Reader1_Restart();

    /* TODO : force channel6 to SW DMA Reader, it's a hardcode */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG, 0x0F,0x00);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0xFF, 0x82);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_SwDma_Reader1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_SwDma_Reader1_Apply_Setting();
            HAL_AUDIO_PCM_SwDma_Reader1_Restart();
        }

        /* start engine */
        if (pPcmInfo->u32Channel == 10)
        {
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x7FFF, 0x4002);   //[14]: SW DMA 10CH setting
        }
        else
        {
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x7FFF, 0x0002);   //[14]: SW DMA 10CH setting
        }

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* stop engine */
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x7FFF, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

extern long long long_div(long long a, int b);

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %lu to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_EASING:
        {
            //FIX ME: SW DMA reader channel fixed in CH6 current status
            //FIX ME: SW DMA reader use Easing Gain B

            memcpy(&pPcmInfo->tEaseParam, pData, sizeof(AUDIO_EASE_PARAM_t));

            //printf("u32FadeDuration = %d\r\n", pPcmInfo->tEaseParam.u32FadeDuration);
            //printf("u32FadeTargetGain = %d\r\n", pPcmInfo->tEaseParam.u32FadeTargetGain);
            //printf("u32FadeType = %d\r\n", pPcmInfo->tEaseParam.eFadeType);
            MS_U32 u32TargetVolumeHi;
            MS_U32 u32TargetVolumeLow;

            u32TargetVolumeHi =  (pPcmInfo->tEaseParam.u32FadeTargetGain & 0x7F0000) >> 16;
            u32TargetVolumeLow = pPcmInfo->tEaseParam.u32FadeTargetGain & 0xFFFF;

            HAL_SOUND_EnableEaseVol(TRUE);

            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Target_Channel_B, AUDIO_PATH_5, 0);
            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Type_B, pPcmInfo->tEaseParam.eFadeType, 0);
            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Duration_B, pPcmInfo->tEaseParam.u32FadeDuration, 0);
            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Gain_B, u32TargetVolumeHi, u32TargetVolumeLow);
            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = u8MultiChFlag;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%lu)!\n", __FUNCTION__, u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%lu)!\n", __FUNCTION__, u32Channel);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32Channel != u32Channel)
            {
                pPcmInfo->u32Channel = u32Channel;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%lu)!\n", __FUNCTION__, u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, (unsigned int)u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, (unsigned int)u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%lu)!\n", __FUNCTION__, u32Mute);
                s32Ret = -A_EINVAL;
            }

            if (s32Ret == 0)
            {
                HAL_SOUND_SetMute(AUDIO_PATH_SW_DMA_1, u32Mute);
            }

            break;
        }

        case AUDIO_PCM_CMD_FASTPLAY:
        {
            MS_U32 u32PlayRate = *((MS_U32 *)pData);
            if(u32PlayRate == 1000) // no fast play
            {
                if(pPcmInfo->bFastPlay == TRUE)
                {
                    /* set synthesizer */
                    HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_L, 0xFFFF, pPcmInfo->u16Synth_L);
                    HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_H, 0xFFFF, pPcmInfo->u16Synth_H);
                    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);
                    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0101); /* SYTH1 bit8RIU control, bit0 update */
                    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);
                    pPcmInfo->bFastPlay = FALSE;
                }
            }
            else
            {
                MS_U64 freq = 0;
                MS_U16 synthrate_H = 0;
                MS_U16 synthrate_L = 0;
                MS_U32 tmp1 = pPcmInfo->u32SampleRate / 25;
                MS_U32 tmp2 = u32PlayRate;
                MS_U32 tmp3 = tmp1 * tmp2;
                MS_U32 arr[3] = {160000000, 262144, 135};
                MS_U32 i = 0;
                MS_U32 temp, numerator, denominator;

                /*
                * 160000000 = ((10000000 / 25) >> 1) * 16 * 25 * 2
                * 262144 = 33554432 / 64 / 2
                * 135 = 843.75 * 4 / 25
                * The original formula:
                * ((843.75 * 33554432 * 10000000) / (pSW_DMA_FS->uSample_rate * pSW_DMA_FS->uPlayRate)) >>1;
                */
                for (i = 0; i < 3; i++)
                {
                    numerator = tmp3;
                    denominator = arr[i];
                    // compute GCD

                    while (denominator != 0)
                    {
                        temp = numerator % denominator;
                        numerator = denominator;
                        denominator = temp;
                    }
                    tmp3 /= numerator;
                    arr[i] /= numerator;
                }
                freq = long_div((MS_U64)arr[2] * (MS_U64)arr[1] * (MS_U64)arr[0], tmp3);

                synthrate_L = (MS_U16) freq;
                synthrate_H = (MS_U16) (freq >> 16);

                //DMR Fixed Me Current S/W DMA use DEC5, synth is controlled by 0x112C bank, and only output 2 channels...
                /* set synthesizer */
                //HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_SynthFreq_H, 0xFFFF, synthrate_H);
                //HAL_MAD2_Write_DSP_sram(DSP2DmAddr_swDmaRdr_synthFreq_L, synthrate_L, DSP_MEM_TYPE_DM);
                pPcmInfo->bFastPlay = TRUE;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%lu)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %lu from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(%lu, %d)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, (unsigned int)AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x0008, 0x0008);
            pPcmInfo->u32BufferLevel = HAL_AUDIO_ReadReg(S2M_MBOX_SW_DMA_READER_DDR_Level) * BYTES_IN_MIU_LINE;
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x0008, 0x0000);
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;
            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_EASING:
        {
            //FIX ME: SW DMA reader channel fixed in CH6 current status
            //FIX ME: SW DMA reader use Easing Gain B

            *((MS_U32 *)pData) = (MS_U32)HAL_SND_GetParam(Sound_GET_PARAM_Ease_Gain_B, 0);

            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            MS_U32 u32TargetBufferSize = 0;
            MS_U32 u32TargetChannel = 0;
            MS_U32 u32TargetBitWidth = 0;
            MS_U32 u32TargetAlignmentSize = 0;

            if (pPcmInfo->u8MultiChFlag == TRUE)
            {
                /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
                u32TargetChannel = 10;
            }
            else
            {
                /* stereo = 2 channels */
                u32TargetChannel = 2;
            }

            /* calculate buffer size */
            u32TargetBitWidth = 16;
            u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
            u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
            AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);

            *((MS_U32 *)pData) = u32TargetBufferSize;

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%lu)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_SwDma_Reader1_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    MS_U32 u32WptrOffset = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%lu) is invalid!\n", __FUNCTION__, u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_SwDma_Reader1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel == 0)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    HAL_AUDIO_PCM_SwDma_Reader1_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + SW_DMA_RDR1_BUF_SIZE) - pPCM->tPcmBufferInfo.pWritePtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pPCM->tPcmBufferInfo.pWritePtr, (void *)pBufTmp, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pWritePtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + SW_DMA_RDR1_BUF_SIZE))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + SW_DMA_RDR1_BUF_SIZE))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update write pointers to engine */
        u32WptrOffset = pPCM->tPcmBufferInfo.pWritePtr - pPCM->tPcmBufferInfo.pBuffer;
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_WtPtr, 0xFFFF, (u32WptrOffset / BYTES_IN_MIU_LINE));

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_SwDma_Reader1_Restart();
        HAL_AUDIO_PCM_SwDma_Reader1_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_R2_DMA_READER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Val = 0;
    MS_U16 u16Synthrate_H = 0;
    MS_U16 u16Synthrate_L = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    if (pPcmInfo == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pPcmInfo should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Synthrate_H = 0x6978;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 11025:
        {
            u16Synthrate_H = 0x4C87;
            u16Synthrate_L = 0xD634;
            break;
        }

        case 12000:
        {
            u16Synthrate_H = 0x4650;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 16000:
        {
            u16Synthrate_H = 0x34BC;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 22050:
        {
            u16Synthrate_H = 0x2643;
            u16Synthrate_L = 0xEB1A;
            break;
        }

        case 24000:
        {
            u16Synthrate_H = 0x2328;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 32000:
        {
            u16Synthrate_H = 0x1A5E;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 44100:
        {
            u16Synthrate_H = 0x1321;
            u16Synthrate_L = 0xF58D;
            break;
        }

        case 48000:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 64000:
        {
            u16Synthrate_H = 0x0D2F;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 88200:
        {
            u16Synthrate_H = 0x0990;
            u16Synthrate_L = 0xFAC6;
            break;
        }

        case 96000:
        {
            u16Synthrate_H = 0x08CA;
            u16Synthrate_L = 0x0000;
            break;
        }

        default:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    pPcmInfo->u16Synth_H = u16Synthrate_H;
    pPcmInfo->u16Synth_L = u16Synthrate_L;


    /* disable output */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x7FFF, 0x0000);

    /* set synthesizer */
    HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_L, 0xFFFF, u16Synthrate_L);
    HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_H, 0xFFFF, u16Synthrate_H);
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0101); /* SYTH1 bit8RIU control, bit0 update */
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);

    /* switch to CH5 */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG, 0x0F,0x00);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH5_CFG, 0xFF,0x80);

    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > R2_DMA_RDR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u, channel %u\n", __FUNCTION__, (unsigned int)u32TargetBufferSize, (unsigned int)pPcmInfo->u32BufferDuration, (unsigned int)pPcmInfo->u32SampleRate, (unsigned int)u32TargetChannel);
        u32TargetBufferSize = R2_DMA_RDR1_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 3), 0xFFFF, (u32TargetBufferSize / BYTES_IN_MIU_LINE)); /* SIZE [15:0] */

    /* set overrun & underrun threshold */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 5), 0xFFFF, ((u32TargetBufferSize - (u32TargetBufferSize >> 3)) / BYTES_IN_MIU_LINE));
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 6), 0xFFFF, ((u32TargetBufferSize >> 3) / BYTES_IN_MIU_LINE));

    /* set channel mode */
    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* Multi-Channel mode : 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        HAL_AUR2_WriteMaskByte(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x80, 0x00);
        u16Val = 19; /* MIU burst length = 19 */
        u16Val += (1 << 7); /* MIU Step En = 1 */
        u16Val += (1 << 15); /* Sync Valid En = 1 */
        u16Val += (1 << 8); /* Sync Step Count = 1 */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 7), 0xFFFF, u16Val);
    }
    else
    {
        /* Stereo mode : stereo = 2 channels */
        HAL_AUR2_WriteMaskByte(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x80, 0x80);
        u16Val = 3; /* MIU burst length = 3 */
        u16Val += (0 << 7); /* MIU Step En = 0 */
        u16Val += (0 << 15); /* Sync Valid En = 0 */
        u16Val += (1 << 8); /* Sync Step Count = 1 */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 7), 0xFFFF, u16Val);
    }

    /* set 16 Bit mode */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0860, 0x0020);

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* stop engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0002, 0x0000);

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, R2_DMA_RDR1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* initial dlocal buffer */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x2000, 0x2000);
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x2000, 0x0000);

    /* reset engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x1000, 0x1000);
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x1000, 0x0000);

    /* start engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0002, 0x0002);

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%lu, %u)!\n", __FUNCTION__, pUserPcmInfo->u32StructSize, sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%lu)!\n", __FUNCTION__, pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%lu)!\n", __FUNCTION__, pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > R2_DMA_RDR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%lu) is required! Please check the settings: duration %lu, sample rate %lu, channel %lu\n", __FUNCTION__, u32TargetBufferSize, pUserPcmInfo->u32BufferDuration, pUserPcmInfo->u32SampleRate, u32TargetChannel);
        u32TargetBufferSize = R2_DMA_RDR1_BUF_SIZE;
        pUserPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    pPcmInfo->bFastPlay = FALSE;
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init DPGA */
    HAL_AUR2_WriteMaskByte(R2_DMARDR1_EN, 0x03, 0x03);
    HAL_AUR2_WriteMaskByte((R2_DMARDR1_DPGA_REG_L_CTRL + 1), 0x07,0x07); /* DPGA fading duartion 18ms */
    HAL_AUR2_WriteMaskByte((R2_DMARDR1_DPGA_REG_R_CTRL + 1), 0x07,0x07); /* DPGA fading duartion 18ms */
    HAL_AUR2_WriteMaskByte(R2_DMARDR1_DPGA_REG_L_CTRL, 0x12, 0x12); /* auto mode and fading */
    HAL_AUR2_WriteMaskByte(R2_DMARDR1_DPGA_REG_R_CTRL, 0x12, 0x12);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + R2_DMA_RDR1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* switch R2_DMA & DPGA control to MCU */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x8000, 0x8000);
    HAL_AUR2_WriteMaskReg(R2_DMARDR1_EN, 0x0001, 0x0001);

    /* set PCM buffer address */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 1), 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) & 0xFFFF)); /* BASE [15:0] */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 2), 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 16)); /* BASE [26:16] */

    /* apply setting */
    HAL_AUDIO_PCM_R2Dma_Reader1_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_R2Dma_Reader1_Restart();

    /* TODO : force channel 1~4 to decoder1 when multi-channel output is configured, it's a hardcode */
    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_CH1_CFG, 0x80);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH2_CFG, 0x80);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH3_CFG, 0x80);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH4_CFG, 0x80);
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_R2Dma_Reader1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    /* Switch R2_DMA & DPGA control to R2 */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x8000, 0x0000);
    HAL_AUR2_WriteMaskByte(R2_DMARDR1_EN, 0x01, 0x00);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_R2Dma_Reader1_Apply_Setting();
            HAL_AUDIO_PCM_R2Dma_Reader1_Restart();
        }

        /* enable engine's MIU access */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0001, 0x0001);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* disable engine's MIU access */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0001, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %lu to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_EASING:
        {
            //FIX ME: SW DMA reader channel fixed in CH6 current status
            //FIX ME: SW DMA reader use Easing Gain B

            memcpy(&pPcmInfo->tEaseParam, pData, sizeof(AUDIO_EASE_PARAM_t));

            //printf("u32FadeDuration = %d\r\n", pPcmInfo->tEaseParam.u32FadeDuration);
            //printf("u32FadeTargetGain = %d\r\n", pPcmInfo->tEaseParam.u32FadeTargetGain);
            //printf("u32FadeType = %d\r\n", pPcmInfo->tEaseParam.eFadeType);
            MS_U32 u32TargetVolumeHi;
            MS_U32 u32TargetVolumeLow;

            u32TargetVolumeHi =  (pPcmInfo->tEaseParam.u32FadeTargetGain & 0x7F0000) >> 16;
            u32TargetVolumeLow = pPcmInfo->tEaseParam.u32FadeTargetGain & 0xFFFF;

            HAL_SOUND_EnableEaseVol(TRUE);

            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Target_Channel_A, AUDIO_PATH_4, 0);
            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Type_A, pPcmInfo->tEaseParam.eFadeType, 0);
            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Duration_A, pPcmInfo->tEaseParam.u32FadeDuration, 0);
            HAL_SND_SetParam(Sound_SET_PARAM_Ease_Gain_A, u32TargetVolumeHi, u32TargetVolumeLow);
            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = u8MultiChFlag;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%lu)!\n", __FUNCTION__, u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%lu)!\n", __FUNCTION__, u32Channel);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32Channel != u32Channel)
            {
                pPcmInfo->u32Channel = u32Channel;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%lu)!\n", __FUNCTION__, u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, (unsigned int)u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, (unsigned int)u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);
            MS_BOOL bDecoderMute = FALSE;

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%lu)!\n", __FUNCTION__, u32Mute);
                s32Ret = -A_EINVAL;
            }

            if (s32Ret == 0)
            {
                g_AudioVars2->g_bAdec1GSTMute = u32Mute;
                bDecoderMute = g_AudioVars2->g_bAdec1GSTMute | g_AudioVars2->g_bAdec1TVMute;
                HAL_SOUND_SetMute(AUDIO_PATH_R2_DMA_1, bDecoderMute);
                printf("\r\n \033[1;36m === mute R2 dma 1 rdr:  (%u) === \033[0m \r\n", (unsigned int)u32Mute);
            }

            break;
        }

        case AUDIO_PCM_CMD_FASTPLAY:
        {
            MS_U32 u32PlayRate = *((MS_U32 *)pData);
            if(u32PlayRate == 1000) // no fast play
            {
                if(pPcmInfo->bFastPlay == TRUE)
                {
                    /* set synthesizer */
                    HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_L, 0xFFFF, pPcmInfo->u16Synth_L);
                    HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_H, 0xFFFF, pPcmInfo->u16Synth_H);
                    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);
                    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0101); /* SYTH1 bit8RIU control, bit0 update */
                    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);
                    pPcmInfo->bFastPlay = FALSE;
                }
            }
            else
            {
                MS_U64 freq = 0;
                MS_U16 synthrate_H = 0;
                MS_U16 synthrate_L = 0;
                MS_U32 tmp1 = pPcmInfo->u32SampleRate / 25;
                MS_U32 tmp2 = u32PlayRate;
                MS_U32 tmp3 = tmp1 * tmp2;
                MS_U32 arr[3] = {160000000, 262144, 135};
                MS_U32 i = 0;
                MS_U32 temp, numerator, denominator;

                /*
                * 160000000 = ((10000000 / 25) >> 1) * 16 * 25 * 2
                * 262144 = 33554432 / 64 / 2
                * 135 = 843.75 * 4 / 25
                * The original formula:
                * ((843.75 * 33554432 * 10000000) / (pSW_DMA_FS->uSample_rate * pSW_DMA_FS->uPlayRate)) >>1;
                */
                for (i = 0; i < 3; i++)
                {
                    numerator = tmp3;
                    denominator = arr[i];
                    // compute GCD

                    while (denominator != 0)
                    {
                        temp = numerator % denominator;
                        numerator = denominator;
                        denominator = temp;
                    }
                    tmp3 /= numerator;
                    arr[i] /= numerator;
                }
                freq = long_div((MS_U64)arr[2] * (MS_U64)arr[1] * (MS_U64)arr[0], tmp3);

                synthrate_L = (MS_U16) freq;
                synthrate_H = (MS_U16) (freq >> 16);

                /* set synthesizer */
                HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_L, 0xFFFF, synthrate_L);
                HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_H, 0xFFFF, synthrate_H);
                HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);
                HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0101); /* SYTH1 bit8RIU control, bit0 update */
                HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);
                pPcmInfo->bFastPlay = TRUE;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%lu)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %lu from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(%lu, %d)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32BufferLevel1 = 0;
            MS_U32 u32BufferLevel2 = 0;
            MS_U32 u32Timeout = 20;

            while (u32Timeout > 0)
            {
                u32BufferLevel1 = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 10))) * BYTES_IN_MIU_LINE;
                u32BufferLevel2 = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 10))) * BYTES_IN_MIU_LINE;

                if (u32BufferLevel1 == u32BufferLevel2)
                {
                    break;
                }

                u32Timeout--;

#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(1);
#else
                AUDIO_DELAY1US(1);
#endif
            }

            if (u32BufferLevel1 != u32BufferLevel2)
            {
                HALAUDIO_ERROR("%s: Error! fail to get correct buffer level (%lu, %lu)\n", __FUNCTION__, u32BufferLevel1, u32BufferLevel2);
            }

            pPcmInfo->u32BufferLevel = u32BufferLevel2;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_EASING:
        {
            //FIX ME: R2 DMA reader channel fixed in CH5 current status
            //FIX ME: R2 DMA reader use Easing Gain A

            *((MS_U32 *)pData) = (MS_U32)HAL_SND_GetParam(Sound_GET_PARAM_Ease_Gain_A, 0);

            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 3))) * BYTES_IN_MIU_LINE;
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%lu)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_R2Dma_Reader1_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    MS_U32 u32TargetChannel = 0;
    //HALAUDIO_ERROR("%s: Write %lu bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%lu) is invalid!\n", __FUNCTION__, u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_R2Dma_Reader1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel == 0)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    HAL_AUDIO_PCM_R2Dma_Reader1_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pWritePtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pPCM->tPcmBufferInfo.pWritePtr, (void *)pBufTmp, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pWritePtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update copied size to engine */
        u32RequestSize += pPCM->tPcmBufferInfo.u32RemainSize;
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 4), 0xFFFF, (u32RequestSize / BYTES_IN_MIU_LINE));
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0400, 0x0400);
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0400, 0x0000);
        pPCM->tPcmBufferInfo.u32RemainSize = u32RequestSize % BYTES_IN_MIU_LINE;

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_R2Dma_Reader1_Restart();
        HAL_AUDIO_PCM_R2Dma_Reader1_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_R2_DMA_READER2_SUPPORT
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Val = 0;
    MS_U16 u16Synthrate_H = 0;
    MS_U16 u16Synthrate_L = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    if (pPcmInfo == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pPcmInfo should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Synthrate_H = 0x6978;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 11025:
        {
            u16Synthrate_H = 0x4C87;
            u16Synthrate_L = 0xD634;
            break;
        }

        case 12000:
        {
            u16Synthrate_H = 0x4650;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 16000:
        {
            u16Synthrate_H = 0x34BC;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 22050:
        {
            u16Synthrate_H = 0x2643;
            u16Synthrate_L = 0xEB1A;
            break;
        }

        case 24000:
        {
            u16Synthrate_H = 0x2328;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 32000:
        {
            u16Synthrate_H = 0x1A5E;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 44100:
        {
            u16Synthrate_H = 0x1321;
            u16Synthrate_L = 0xF58D;
            break;
        }

        case 48000:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 64000:
        {
            u16Synthrate_H = 0x0D2F;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 88200:
        {
            u16Synthrate_H = 0x0990;
            u16Synthrate_L = 0xFAC6;
            break;
        }

        case 96000:
        {
            u16Synthrate_H = 0x08CA;
            u16Synthrate_L = 0x0000;
            break;
        }

        default:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    /* disable output */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x7FFF, 0x0000);

    /* set synthesizer */
    HAL_AUR2_WriteMaskReg(R2_DMARDR2_REG_SYNTH_L, 0xFFFF, u16Synthrate_L);
    HAL_AUR2_WriteMaskReg(R2_DMARDR2_REG_SYNTH_H, 0xFFFF, u16Synthrate_H);
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0404, 0x0400);
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0404, 0x0404); /* SYTH3 bit10 RIU control, bit3 update */
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0404, 0x0400);

    /* switch to CH6 */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG, 0x0F,0x00);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0xFF,0x82);

    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > R2_DMA_RDR2_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u, channel %u\n", __FUNCTION__, (unsigned int)u32TargetBufferSize, (unsigned int)pPcmInfo->u32BufferDuration, (unsigned int)pPcmInfo->u32SampleRate, (unsigned int)u32TargetChannel);
        u32TargetBufferSize = R2_DMA_RDR2_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 3), 0xFFFF, (u32TargetBufferSize / BYTES_IN_MIU_LINE)); /* SIZE [15:0] */

    /* set overrun & underrun threshold */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 5), 0xFFFF, ((u32TargetBufferSize - (u32TargetBufferSize>> 3)) / BYTES_IN_MIU_LINE));
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 6), 0xFFFF, ((u32TargetBufferSize >> 3) / BYTES_IN_MIU_LINE));

    /* set channel mode */
    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* Multi-Channel mode : 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        HAL_AUR2_WriteMaskByte(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x80, 0x00);
        u16Val = 19; /* MIU burst length = 19 */
        u16Val += (1 << 7); /* MIU Step En = 1 */
        u16Val += (1 << 15); /* Sync Valid En = 1 */
        u16Val += (1 << 8); /* Sync Step Count = 1 */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 7), 0xFFFF, u16Val);
    }
    else
    {
        /* Stereo mode : stereo = 2 channels */
        HAL_AUR2_WriteMaskByte(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x80, 0x80);
        u16Val = 3; /* MIU burst length = 3 */
        u16Val += (0 << 7); /* MIU Step En = 0 */
        u16Val += (0 << 15); /* Sync Valid En = 0 */
        u16Val += (1 << 8); /* Sync Step Count = 1 */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 7), 0xFFFF, u16Val);
    }

    /* set 16 Bit mode */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0860, 0x0020);

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* stop engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0002, 0x0000);

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, R2_DMA_RDR2_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* initial dlocal buffer */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x2000, 0x2000);
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x2000, 0x0000);

    /* reset engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x1000, 0x1000);
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x1000, 0x0000);

    /* start engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0002, 0x0002);

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructSize, (unsigned int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > R2_DMA_RDR2_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u, channel %u\n", __FUNCTION__, (unsigned int)u32TargetBufferSize, (unsigned int)pUserPcmInfo->u32BufferDuration, (unsigned int)pUserPcmInfo->u32SampleRate, (unsigned int)u32TargetChannel);
        u32TargetBufferSize = R2_DMA_RDR2_BUF_SIZE;
        pUserPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init DPGA */
    HAL_AUR2_WriteMaskByte(R2_DMARDR2_EN, 0x03, 0x03);
    HAL_AUR2_WriteMaskByte((R2_DMARDR2_DPGA_REG_L_CTRL + 1), 0x07,0x07); /* DPGA fading duartion 18ms */
    HAL_AUR2_WriteMaskByte((R2_DMARDR2_DPGA_REG_R_CTRL + 1), 0x07,0x07); /* DPGA fading duartion 18ms */
    HAL_AUR2_WriteMaskByte(R2_DMARDR2_DPGA_REG_L_CTRL, 0x12, 0x12); /* auto mode and fading */
    HAL_AUR2_WriteMaskByte(R2_DMARDR2_DPGA_REG_R_CTRL, 0x12, 0x12);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + R2_DMA_RDR2_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* switch R2_DMA & DPGA control to MCU */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x8000, 0x8000);
    HAL_AUR2_WriteMaskReg(R2_DMARDR2_EN, 0x0001, 0x0001);

    /* set PCM buffer address */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 1), 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) & 0xFFFF)); /* BASE [15:0] */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 2), 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 16)); /* BASE [26:16] */

    /* apply setting */
    HAL_AUDIO_PCM_R2Dma_Reader2_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_R2Dma_Reader2_Restart();

    /* TODO : force channel 1~4 to decoder3 when multi-channel output is configured, it's a hardcode */
    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_CH1_CFG, 0x82);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH2_CFG, 0x82);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH3_CFG, 0x82);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH4_CFG, 0x82);
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_R2Dma_Reader2_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    /* switch R2_DMA & DPGA control to R2 */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x8000, 0x0000);
    HAL_AUR2_WriteMaskByte(R2_DMARDR2_EN, 0x01, 0x00);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_R2Dma_Reader2_Apply_Setting();
            HAL_AUDIO_PCM_R2Dma_Reader2_Restart();
        }

        /* enable engine's MIU access */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0001, 0x0001);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* disable engine's MIU access */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0001, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = u8MultiChFlag;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, (unsigned int)u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%u)!\n", __FUNCTION__, (unsigned int)u32Channel);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32Channel != u32Channel)
            {
                pPcmInfo->u32Channel = u32Channel;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%u)!\n", __FUNCTION__, (unsigned int)u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, (unsigned int)u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, (unsigned int)u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);
            MS_BOOL bDecoderMute = FALSE;

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, (unsigned int)u32Mute);
                s32Ret = -A_EINVAL;
            }

            if (s32Ret == 0)
            {
                g_AudioVars2->g_bAdec2GSTMute = u32Mute;
                bDecoderMute = g_AudioVars2->g_bAdec2GSTMute | g_AudioVars2->g_bAdec2TVMute;
                HAL_SOUND_SetMute(AUDIO_PATH_R2_DMA_2, bDecoderMute);
                printf("\r\n \033[1;36m === mute R2 dma 2 rdr:  (%u) === \033[0m \r\n", (unsigned int)u32Mute);
            }

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32BufferLevel1 = 0;
            MS_U32 u32BufferLevel2 = 0;
            MS_U32 u32Timeout = 20;

            while (u32Timeout > 0)
            {
                u32BufferLevel1 = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 10))) * BYTES_IN_MIU_LINE;
                u32BufferLevel2 = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 10))) * BYTES_IN_MIU_LINE;

                if (u32BufferLevel1 == u32BufferLevel2)
                {
                    break;
                }

                u32Timeout--;

#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(1);
#else
                AUDIO_DELAY1US(1);
#endif
            }

            if (u32BufferLevel1 != u32BufferLevel2)
            {
                HALAUDIO_ERROR("%s: Error! fail to get correct buffer level (%u, %u)\n", __FUNCTION__, (unsigned int)u32BufferLevel1, (unsigned int)u32BufferLevel2);
            }

            pPcmInfo->u32BufferLevel = u32BufferLevel2;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 3))) * BYTES_IN_MIU_LINE;
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_R2Dma_Reader2_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_R2Dma_Reader2_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel == 0)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    HAL_AUDIO_PCM_R2Dma_Reader2_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pWritePtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pPCM->tPcmBufferInfo.pWritePtr, (void *)pBufTmp, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pWritePtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update copied size to engine */
        u32RequestSize += pPCM->tPcmBufferInfo.u32RemainSize;
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 4), 0xFFFF, (u32RequestSize / BYTES_IN_MIU_LINE));
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0400, 0x0400);
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0400, 0x0000);
        pPCM->tPcmBufferInfo.u32RemainSize = u32RequestSize % BYTES_IN_MIU_LINE;

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_R2Dma_Reader2_Restart();
        HAL_AUDIO_PCM_R2Dma_Reader2_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_PCM_CAPTURE1_SUPPORT
MS_S32 HAL_AUDIO_PCM_Capture1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U32 u32WritePtrOffset = 0;
    MS_U32 u32TimeoutCounter = 0;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_Size, 0x0000);

    do {
        u32WritePtrOffset = HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE_DDR_WrPtr) * BYTES_IN_MIU_LINE;

        if (u32WritePtrOffset != 0)
        {
#ifdef MSOS_TYPE_LINUX_KERNEL
            udelay(50);
#else
            AUDIO_DELAY1US(50);
#endif
            u32TimeoutCounter++;
        }
        else
        {
            break;
        }
    } while (u32TimeoutCounter < 10000);

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, PCM_CAPTURE1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* reset PCM capture1 read & write pointer */
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
    HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_RdPtr, 0x0000);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructSize, (unsigned int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MixingFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag != TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support playback mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    pPcmInfo->u32BufferDuration = ((PCM_CAPTURE1_BUF_SIZE * 1000) / (pUserPcmInfo->u32Channel * 2)) / pUserPcmInfo->u32SampleRate;
    pPcmInfo->u32Channel = pUserPcmInfo->u32Channel;
    pPcmInfo->u32SampleRate = pUserPcmInfo->u32SampleRate;
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + PCM_CAPTURE1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* restart */
    HAL_AUDIO_PCM_Capture1_Restart();

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_Capture1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U32 u32BufferSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
        HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_Size, (u32BufferSize / BYTES_IN_MIU_LINE));

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* TODO, there is no such control in DSP */

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %lu to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure buffer duration!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure channel!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure sample rate!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, (unsigned int)u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, (unsigned int)u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, (unsigned int)u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %lu from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32WritePtrOffset = 0;
            MS_U32 u32BufferSize = 0;
            MS_S32 s32PcmLevel = 0;

            u32WritePtrOffset = HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE_DDR_WrPtr) * BYTES_IN_MIU_LINE;
            pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer + u32WritePtrOffset;

            s32PcmLevel = pPCM->tPcmBufferInfo.pWritePtr - pPCM->tPcmBufferInfo.pReadPtr;
            if (s32PcmLevel < 0)
            {
                u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
                s32PcmLevel += u32BufferSize;
            }

            pPcmInfo->u32BufferLevel = (MS_U32)s32PcmLevel;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = PCM_CAPTURE1_BUF_SIZE;

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_Capture1_Read(void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    MS_U32 u32RptrOffset = 0;
    //HALAUDIO_ERROR("%s: Write %lu bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
    HAL_AUDIO_PCM_Capture1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel >= (u32BufferSize - PCM_CAPTURE1_BUF_UNIT))
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

        u32PcmLevel = 0;
        HAL_AUDIO_PCM_Capture1_Stop();
        HAL_AUDIO_PCM_Capture1_Restart();
        HAL_AUDIO_PCM_Capture1_Start();
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    /* copy data from PCM buffer */
    if (u32PcmLevel >= u32RequestSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pReadPtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pBufTmp, (void *)pPCM->tPcmBufferInfo.pReadPtr, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pReadPtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pReadPtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pReadPtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update read pointer to engine */
        u32RptrOffset = pPCM->tPcmBufferInfo.pReadPtr - pPCM->tPcmBufferInfo.pBuffer;
        HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_RdPtr, (u32RptrOffset / BYTES_IN_MIU_LINE));

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_Capture1_Restart();
        HAL_AUDIO_PCM_Capture1_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_PCM_CAPTURE2_SUPPORT
MS_S32 HAL_AUDIO_PCM_Capture2_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U32 u32WritePtrOffset = 0;
    MS_U32 u32TimeoutCounter = 0;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_Size, 0x0000);

    do {
        u32WritePtrOffset = HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr) * BYTES_IN_MIU_LINE;

        if (u32WritePtrOffset != 0)
        {
#ifdef MSOS_TYPE_LINUX_KERNEL
            udelay(50);
#else
            AUDIO_DELAY1US(50);
#endif
            u32TimeoutCounter++;
        }
        else
        {
            break;
        }
    } while (u32TimeoutCounter < 10000);

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, PCM_CAPTURE2_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* reset PCM capture1 read & write pointer */
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
    HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr, 0x0000);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructSize, (unsigned int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MixingFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag != TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support playback mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    pPcmInfo->u32BufferDuration = ((PCM_CAPTURE2_BUF_SIZE * 1000) / (pUserPcmInfo->u32Channel * 2)) / pUserPcmInfo->u32SampleRate;
    pPcmInfo->u32Channel = pUserPcmInfo->u32Channel;
    pPcmInfo->u32SampleRate = pUserPcmInfo->u32SampleRate;
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + PCM_CAPTURE2_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* restart */
    HAL_AUDIO_PCM_Capture2_Restart();

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_Capture2_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U32 u32BufferSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
        HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_Size, (u32BufferSize / BYTES_IN_MIU_LINE));

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* TODO, there is no such control in DSP */

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %lu to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure buffer duration!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure channel!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure sample rate!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, (unsigned int)u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, (unsigned int)u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, (unsigned int)u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %lu from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32WritePtrOffset = 0;
            MS_U32 u32BufferSize = 0;
            MS_S32 s32PcmLevel = 0;

            u32WritePtrOffset = HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr) * BYTES_IN_MIU_LINE;
            pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer + u32WritePtrOffset;

            s32PcmLevel = pPCM->tPcmBufferInfo.pWritePtr - pPCM->tPcmBufferInfo.pReadPtr;
            if (s32PcmLevel < 0)
            {
                u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
                s32PcmLevel += u32BufferSize;
            }

            pPcmInfo->u32BufferLevel = (MS_U32)s32PcmLevel;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = PCM_CAPTURE2_BUF_SIZE;

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_Capture2_Read(void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    MS_U32 u32RptrOffset = 0;
    //HALAUDIO_ERROR("%s: Write %lu bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
    HAL_AUDIO_PCM_Capture2_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel >= (u32BufferSize - PCM_CAPTURE2_BUF_UNIT))
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

        u32PcmLevel = 0;
        HAL_AUDIO_PCM_Capture2_Stop();
        HAL_AUDIO_PCM_Capture2_Restart();
        HAL_AUDIO_PCM_Capture2_Start();
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    /* copy data from PCM buffer */
    if (u32PcmLevel >= u32RequestSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pReadPtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pBufTmp, (void *)pPCM->tPcmBufferInfo.pReadPtr, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pReadPtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pReadPtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pReadPtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update read pointer to engine */
        u32RptrOffset = pPCM->tPcmBufferInfo.pReadPtr - pPCM->tPcmBufferInfo.pBuffer;
        HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr, (u32RptrOffset / BYTES_IN_MIU_LINE));

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_Capture2_Restart();
        HAL_AUDIO_PCM_Capture2_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_HW_DMA_WRITER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    //MS_U32 u32WritePtrOffset = 0;
    //MS_U32 u32TimeoutCounter = 0;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, HW_DMA_WTR1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();
    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructSize, sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MixingFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag != TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support playback mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);

    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    pPcmInfo->u32BufferDuration = ((HW_DMA_WTR1_BUF_SIZE * 1000) / (pUserPcmInfo->u32Channel * 2)) / pUserPcmInfo->u32SampleRate;
    pPcmInfo->u32Channel = pUserPcmInfo->u32Channel;
    pPcmInfo->u32SampleRate = pUserPcmInfo->u32SampleRate;;
    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + HW_DMA_WTR1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* set PCM buffer address */
    HAL_AUDIO_AbsWriteReg(NewDMAwriter_ctrl, 0x0);
    HAL_AUDIO_AbsWriteReg(NewDMAwriter_BaseAddress_Lo, (u32TargetBufferAddrPa/BYTES_IN_MIU_LINE) & 0xFFFF);
    HAL_AUDIO_AbsWriteReg(NewDMAwriter_BaseAddress_Hi, (u32TargetBufferAddrPa/BYTES_IN_MIU_LINE>>16) & 0xFFFF);

    HAL_AUDIO_WriteMaskReg(M2S_MBOX_HW_DMA_WRITER1_CTRL, 0x80, 0x80);
    HAL_AUDIO_WriteMaskReg(M2S_MBOX_HW_DMA_WRITER1_CTRL, 0x0F, M2S_MBOX_GET_Raw);

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > HW_DMA_WTR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, (unsigned int)u32TargetBufferSize, (unsigned int)pPcmInfo->u32BufferDuration, (unsigned int)pPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_WTR1_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_DRAM_size, 0xFFFF, u32TargetBufferSize / BYTES_IN_MIU_LINE);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_DRAM_underrun_threshold, 0xFFFF, 0x00012);                         //setting : DMA Reader Underrun Thr

    /* Reset and Start DMA Writer */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0xFFFF, 0x800B); //SW reset & enable DMA reader, MIU request
    AUDIO_DELAY1US(1);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0xFFFF, 0x000B); //SW reset & enable DMA reader, MIU request

    /* restart */
    HAL_AUDIO_PCM_HwDma_Writer1_Restart();

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Writer1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    //MS_U32 u32BufferSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0001, 0x0001);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0002, 0x0002);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* TODO, there is no such control in DSP */

        HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0001, 0x0000);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0002, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, (unsigned int)u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure channel!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure sample rate!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, (unsigned int)u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, (unsigned int)u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, (unsigned int)u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32BufferLevel1 = 0;
            MS_U32 u32BufferLevel2 = 0;
            MS_U32 u32Timeout = 20;

            while (u32Timeout > 0)
            {
                u32BufferLevel1 = HAL_AUDIO_AbsReadReg(NewDMAwriter_DRAM_levelcnt) * BYTES_IN_MIU_LINE;
                u32BufferLevel2 = HAL_AUDIO_AbsReadReg(NewDMAwriter_DRAM_levelcnt) * BYTES_IN_MIU_LINE;

                if (u32BufferLevel1 == u32BufferLevel2)
                {
                    break;
                }

                u32Timeout--;

#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(1);
#else
                AUDIO_DELAY1US(1);
#endif
            }

            if (u32BufferLevel1 != u32BufferLevel2)
            {
                HALAUDIO_ERROR("%s: Error! fail to get correct buffer level (%u, %u)\n", __FUNCTION__, (unsigned int)u32BufferLevel1, (unsigned int)u32BufferLevel2);
            }

            pPcmInfo->u32BufferLevel = u32BufferLevel2;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = HW_DMA_WTR1_BUF_SIZE;

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_HwDma_Writer1_Read(void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
    HAL_AUDIO_PCM_HwDma_Writer1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    /* copy data from PCM buffer */
    if (u32PcmLevel >= u32RequestSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pReadPtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pBufTmp, (void *)pPCM->tPcmBufferInfo.pReadPtr, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pReadPtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pReadPtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pReadPtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        HAL_AUR2_WriteMaskReg(NewDMAwriter_ctrl, 0x0010, 0x0010);
        HAL_AUR2_WriteMaskReg(NewDMAwriter_CPU_triggersize, 0xFFFF, u32RequestSize/BYTES_IN_MIU_LINE);
        HAL_AUR2_WriteMaskReg(NewDMAwriter_ctrl, 0x0010, 0x0000);

        return u32Size;
    }
    return 0;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Writer1_Restart();
        HAL_AUDIO_PCM_HwDma_Writer1_Stop();
    }

    return s32Ret;
}
#endif

// [LM14]
//0x1027_5a[15:0] = channel_status[15:0]
//0x1027_5b[15:0] = channel_status[31:16]
//0x1027_5c[7:0]  = channel_status[39:32]
MS_U16 HAL_AUDIO_HDMI_RX_GetHdmiInChannelStatus(void)
{
    return (_AU_AbsRead2Byte(0x10275A));
}

//[LM14]
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_ARC_HWEN()  @@Lugo
/// @brief \b Function \b Description:  This routine is used to enable ARC output (Hardware)
/// @param <IN>        \b bEnable:    0--Disable ARC out
///                                    1--Enable ARC out
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_ARC_HWEN(MS_BOOL bEnable)
{
    #if 0
    [DMR]
    8-bit address
    101E_FE[5] = 1 (PAD_ARC MODE)
    #endif

    if(bEnable)
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x18, 0x00);
        HAL_AUDIO_AbsWriteMaskByte(0x101EFE, 0x20, 0x20);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x18, 0x00);
        HAL_AUDIO_AbsWriteMaskByte(0x101EFE, 0x20, 0x00);
    }
}

//[M2]
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_ARC_HWEN_BY_PORT()  @@Lugo
/// @brief \b Function \b Description: This routine is used to enable ARC output by port(Hardware)
/// @param <IN>        \b bEnable: 0--Disable ARC out
///                                1--Enable ARC out
/// @param <OUT>       \b NONE:
/// @param <RET>       \b NONE:
/// @param <GLOBAL>    \b NONE:
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_ARC_HWEN_BY_PORT(AUDIO_HDMI_INDEX_T hdmiIndex, MS_BOOL bOnOff)
{
    #if 0
    [DMR]
    8-bit address
    101E_FE[5] = 1 (PAD_ARC MODE)
    #endif

    if(bOnOff)
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x18, 0x00);
        HAL_AUDIO_AbsWriteMaskByte(0x101EFE, 0x20, 0x20);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x18, 0x00);
        HAL_AUDIO_AbsWriteMaskByte(0x101EFE, 0x20, 0x00);
    }
}

// [LM14]
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetCaptureEnable()
/// @brief \b Function \b Description:  For Samsung PCM capture
////////////////////////////////////////////////////////////////////////////////
MS_BOOL bIsPCMCaptureFunctionEnabled = FALSE; // check if PCM Capture Function is Enabled, avoid multiple init
void HAL_AUDIO_SetCaptureEnable(MS_BOOL bEnable, void* PCM_CB)
{
    if ( (bEnable == TRUE) && (bIsPCMCaptureFunctionEnabled == FALSE) )
    {
        HAL_AUDIO_WriteMaskByte(M2S_MBOX_CAPTURE_CTRL+1, 0x80, 0x80);
        PCMCaptureBaseAddr = (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + PCM_CAPTURE_BUF_ADDR_BASE);
        PCM_capture_callback = PCM_CB;
        bIsPCMCaptureFunctionEnabled = TRUE;

    }
    else if( (bEnable == FALSE) && (bIsPCMCaptureFunctionEnabled == TRUE) )
    {
        HAL_AUDIO_WriteMaskByte(M2S_MBOX_CAPTURE_CTRL+1, 0x80, 0x00);
        PCM_capture_callback = NULL;
        bIsPCMCaptureFunctionEnabled = FALSE;
    }
}

// [LM14]
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SwDmaReader_Init()
/// @brief \b Function \b Description: This routine is used to initilize DMA Reader.
/// @param <IN>        \b   eType   : sampleRate of PCM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SwDmaReader_Init(PCM_OUTPUT_FORMAT* pSettings)
{
    MS_U16 synthrate_H = 0;
    MS_U16 synthrate_L = 0;
    MS_U32 u32BaseAddr;
    MS_U8 u8Path = 0;
    unsigned int sample_rate = 44100;

    if(pSettings->u8PathIdx < NUM_OF_SW_DMA)
    {
        u8Path = pSettings->u8PathIdx;
    }

    // Save PCM format
    stSwDMAPcmSettings[u8Path].bLittleEndian = pSettings->bLittleEndian;
    stSwDMAPcmSettings[u8Path].eFs = pSettings->eFs;
    stSwDMAPcmSettings[u8Path].u8ByteWidth = pSettings->u8ByteWidth;
    stSwDMAPcmSettings[u8Path].u8Channels = pSettings->u8Channels;
    stSwDMAPcmSettings[u8Path].bMainChannel = pSettings->bMainChannel;
    stSwDMAPcmSettings[u8Path].OMX_ArenderPathID = pSettings->OMX_ArenderPathID;
    stSwDMAPcmSettings[u8Path].u8PathIdx = pSettings->u8PathIdx;

    // formulate:
    // 216M/FS
    // to get high/low bytes --> 216M*2^16/FS = 14155776000000/FS
    // ex. 44.1k case.   14155776000000/44100 = 0x1321F58D
    switch(stSwDMAPcmSettings[u8Path].eFs)
    {
        case SAMPLE_RATE_8000:
        {
            sample_rate = 8000;
            synthrate_H = 0x6978;
            synthrate_L = 0x0000;
            break;
        }

        case SAMPLE_RATE_11025:
        {
            sample_rate = 11025;
            synthrate_H = 0x4C87;
            synthrate_L = 0xD634;
            break;
        }

        case SAMPLE_RATE_12000:
        {
            sample_rate = 12000;
            synthrate_H = 0x4650;
            synthrate_L = 0x0000;
            break;
        }

        case SAMPLE_RATE_16000:
        {
            sample_rate = 16000;
            synthrate_H = 0x34BC;
            synthrate_L = 0x0000;
            break;
        }

        case SAMPLE_RATE_22050:
        {
            sample_rate = 22050;
            synthrate_H = 0x2643;
            synthrate_L = 0xEB1A;
            break;
        }

        case SAMPLE_RATE_24000:
        {
            sample_rate = 24000;
            synthrate_H = 0x2328;
            synthrate_L = 0x0000;
            break;
        }

        case SAMPLE_RATE_32000:
        {
            sample_rate = 32000;
            synthrate_H = 0x1A5E;
            synthrate_L = 0x0000;
            break;
        }

        case SAMPLE_RATE_44100:
        {
            sample_rate = 44100;
            synthrate_H = 0x1321;
            synthrate_L = 0xF58D;
            break;
        }

        case SAMPLE_RATE_48000:
        {
            sample_rate = 48000;
            synthrate_H = 0x1194;
            synthrate_L = 0x0000;
            break;
        }

        case SAMPLE_RATE_96000:
        {
            sample_rate = 96000;
            synthrate_H = 0x08CA;
            synthrate_L = 0x0000;
            break;
        }

        default:
        {
            HALAUDIO_ERROR("Error! un-supported sample rate %u, set default as 48k !!!\n", stSwDMAPcmSettings[u8Path].eFs);

            synthrate_H = 0x1194;
            synthrate_L = 0x0000;
            break;
        }
    }

    #if 0
    printf("\r\n");
    printf(" === path(%d) === \r\n", (int)pSettings->u8PathIdx);
    printf(" === %s === \r\n", __FUNCTION__);
    printf(" === bLittleEndian = %d\r\n", stSwDMAPcmSettings[u8Path].bLittleEndian);
    printf(" === Fs = %d\r\n", (int)sample_rate);
    printf(" === u8ByteWidth = %d\r\n", stSwDMAPcmSettings[u8Path].u8ByteWidth);
    printf(" === u8Channels = %d\r\n", stSwDMAPcmSettings[u8Path].u8Channels);
    printf(" === OMX_ArenderPathID = %d\r\n", stSwDMAPcmSettings[u8Path].OMX_ArenderPathID);
    printf("\r\n");
    #endif

#if 1 // Munich use S/W DMA reader 0
    //if(u8Path == 1)
    {
        u32BaseAddr = HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC) +OFFSET_SW_DMA_READER_DRAM_BASE +  ASND_DSP_DDR_SIZE;
        //DMR Fixed Me Current S/W DMA use DEC5, synth is controlled by 0x112C bank, and only output 2 channels...
        //HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_SynthFreq_H, 0xFFFF, synthrate_H);
        //HAL_MAD2_Write_DSP_sram(DSP2DmAddr_swDmaRdr_synthFreq_L, synthrate_L, DSP_MEM_TYPE_DM);
#ifdef MSOS_TYPE_LINUX_KERNEL
        mdelay(1);
#else
        AUDIO_DELAY1MS(1);
#endif
        //Rest SW DMA
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x7FFF, 0x0001); // 0x2D36

        /* clear PCM playback2 write pointer */
        u32SwDmaLimitSize[u8Path] = ((sample_rate * DMA_RDR_PCM_BUF_TIME) << 2) / 1000;
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_WtPtr, 0xFFFF, 0x0000); // 0x2D34
#ifdef MSOS_TYPE_LINUX_KERNEL
        mdelay(1);
#else
        AUDIO_DELAY1MS(1);
#endif
        RingBuffer_SwDmaReader[u8Path].StartAddress = (MS_U8 *)MsOS_PA2KSEG1(u32BaseAddr);
        RingBuffer_SwDmaReader[u8Path].BufferSize = SW_DMA_READER_DRAM_SIZE;
        RingBuffer_SwDmaReader[u8Path].EndAddress = RingBuffer_SwDmaReader[u8Path].StartAddress + RingBuffer_SwDmaReader[u8Path].BufferSize;
        RingBuffer_SwDmaReader[u8Path].WritePointer = RingBuffer_SwDmaReader[u8Path].StartAddress;
        RingBuffer_SwDmaReader[u8Path].ReadPointer =  RingBuffer_SwDmaReader[u8Path].StartAddress;
        RingBuffer_SwDmaReader[u8Path].u32wPtr = 0;
        /* clear PCM playback2 pcm buffer */
        memset((void *)RingBuffer_SwDmaReader[u8Path].StartAddress, 0x0, RingBuffer_SwDmaReader[u8Path].BufferSize);
        MsOS_FlushMemory();

        //Hard code temp, need refine
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG, 0x0F,0x00);

        if (stSwDMAPcmSettings[u8Path].OMX_ArenderPathID == 0)
        {
            g_OMX_ArenderPathID = AUDIO_OMX_ARENDER_PATH_ID_CH5;   //[LM14A]
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH5_CFG, 0x1F,0x02);
        }
        else
        {
            g_OMX_ArenderPathID = AUDIO_OMX_ARENDER_PATH_ID_CH6;   //[LM14A]
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0x1F,0x02);
        }
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x7FFF, 0x0002);        // bit 1 --> start
    }
#else
    //else if(u8Path == 0)
    {
        u32BaseAddr = HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC) +OFFSET_SW_DMA_READER2_DRAM_BASE +  ASND_DSP_DDR_SIZE;
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER2_DDR_SynthFreq_H, 0xFFFF, synthrate_H);
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER2_DDR_SynthFreq_L, 0xFFFF, synthrate_L);
#ifdef MSOS_TYPE_LINUX_KERNEL
        mdelay(1);
#else
        AUDIO_DELAY1MS(1);
#endif
        //Rest SW DMA
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER2_DDR_Ctrl, 0x7FFF, 0x0001); // 0x2D36

        /* clear PCM playback2 write pointer */
        u32SwDmaLimitSize[u8Path] = ((sample_rate * DMA_RDR_PCM_BUF_TIME) << 2) / 1000;
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER2_DDR_WtPtr, 0xFFFF, 0x0000); // 0x2D34
#ifdef MSOS_TYPE_LINUX_KERNEL
        mdelay(1);
#else
        AUDIO_DELAY1MS(1);
#endif
        RingBuffer_SwDmaReader[u8Path].StartAddress = (MS_U8 *)MsOS_PA2KSEG1(u32BaseAddr);
        RingBuffer_SwDmaReader[u8Path].BufferSize = SW_DMA_READER2_DRAM_SIZE;
        RingBuffer_SwDmaReader[u8Path].EndAddress = RingBuffer_SwDmaReader[u8Path].StartAddress + RingBuffer_SwDmaReader[u8Path].BufferSize;
        RingBuffer_SwDmaReader[u8Path].WritePointer = RingBuffer_SwDmaReader[u8Path].StartAddress;
        RingBuffer_SwDmaReader[u8Path].ReadPointer =  RingBuffer_SwDmaReader[u8Path].StartAddress;
        RingBuffer_SwDmaReader[u8Path].u32wPtr = 0;
        /* clear PCM playback2 pcm buffer */
        memset((void *)RingBuffer_SwDmaReader[u8Path].StartAddress, 0x0, RingBuffer_SwDmaReader[u8Path].BufferSize);
        MsOS_FlushMemory();

        //Hard code temp, need refine
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG, 0x0F,0x00);

        if (stSwDMAPcmSettings[u8Path].OMX_ArenderPathID == 0)
        {
            g_OMX_ArenderPathID = AUDIO_OMX_ARENDER_PATH_ID_CH5;   //[LM14A]
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH5_CFG, 0x1F,0x02);
        }
        else
        {
            g_OMX_ArenderPathID = AUDIO_OMX_ARENDER_PATH_ID_CH6;   //[LM14A]
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0x1F,0x02);
        }
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER2_DDR_Ctrl, 0x7FFF, 0x0002);        // bit 1 --> start// bit 1 --> start
    }
#endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SwDmaReader_GetLevel()
/// @brief \b Function \b Description: This routine is used to write PCM data into DMA reader Buffer
/// @param <IN>        \b   eType   : buffer bytes
/// @param <OUT>       \b NONE    : TRUE or FALSE
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_SwDmaReader_GetLevel(MS_U8 u8Idx)
{
    MS_U32 u32Level = 0;

#if 1 // Munich use SW DMA reader0
    //if(u8Idx == 1)
    {
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x0008, 0x0008);        // bit 3 --> disable update level
        u32Level = BYTES_IN_MIU_LINE*HAL_AUDIO_ReadReg(S2M_MBOX_SW_DMA_READER_DDR_Level);   // in bytes
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x0008, 0x0000);        // bit 3 --> enable update level
    }
#else
    //else
    {
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER2_DDR_Ctrl, 0x0008, 0x0008);        // bit 3 --> disable update level
        u32Level = BYTES_IN_MIU_LINE*HAL_AUDIO_ReadReg(S2M_MBOX_SW_DMA_READER2_DDR_Level);   // in bytes
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER2_DDR_Ctrl, 0x0008, 0x0000);        // bit 3 --> enable update level
    }
#endif
    return u32Level;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SwDmaReader_WritePCM()
/// @brief \b Function \b Description: This routine is used to write PCM data into DMA reader Buffer
/// @param <IN>        \b   eType   : buffer bytes
/// @param <OUT>       \b NONE    : TRUE or FALSE
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SwDmaReader_WritePCM(MS_U8 u8Idx, void* buffer, MS_U32 bytes)
{

    unsigned char *bufptr = (unsigned char *)(buffer);
    unsigned char tmp1, tmp2;
    unsigned int i;
    MS_U16 u16Temp = 0;
    static MS_U32 u32Cnt = 0;
    MS_U8* pBuf = (MS_U8 *)buffer;
    u32Cnt++;
    MS_U8 u8Path = u8Idx;

    if (u8Path >= NUM_OF_SW_DMA)
    {
        HALAUDIO_ERROR("sw dma index out of bound\n");
        return FALSE;
    }

    if (u8Path == 0)
    {
        //Patch for PCM case,need R2 decoder to use R2 synthesizer
        if (HAL_AUR2_ReadByte(REG_R2_DECODE2_TYPE) == 0x00)
        {
            HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_mpeg);
        }
        //Patch for PCM case,need R2 play cmd to use R2 synthesizer
        if (HAL_AUR2_ReadByte(REG_R2_DECODE2_CMD) != AU_DEC_R2_CMD_PLAYUNI)
        {
            HAL_AUR2_WriteByte(REG_R2_DECODE2_CMD, AU_DEC_R2_CMD_PLAYUNI);
        }

        if ((HAL_AUDIO_ReadReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl) & 0x0002) != 0x0002)
        {
            printf("====Force reset sw dma 0====\r\n");
            HAL_AUDIO_SwDmaReader_Init(&(stSwDMAPcmSettings[0]));
        }
    }

    MS_U16 u16Level = HAL_AUDIO_SwDmaReader_GetLevel(u8Path);
    MS_U32 u32RemainingBufferSize = RingBuffer_SwDmaReader[u8Path].BufferSize - u16Level;

    //printf("\r\n === %s,  Level(0x%x), BufSize(0x%lx) === \r\n", __FUNCTION__, u16Level, u32SwDmaLimitSize[u8Path]);
    if (u16Level > u32SwDmaLimitSize[u8Path])
    {
        u32RemainingBufferSize = 0;
    }
    else
    {
        u32RemainingBufferSize = u32SwDmaLimitSize[u8Path] - u16Level;
    }

    //printf("\r\n === %s,  u32RemainingBufferSize (0x%lu) === \r\n", __FUNCTION__, u32RemainingBufferSize);
    #if 0
    printf(" %s, remain_level(0x%x), remain_free_size(0x%x) \r\n",
        __FUNCTION__,
        u16Level,
        u32RemainingBufferSize);
    #endif

    if(u16Level <=  0x20)
    {
        printf("\r\n === sw dma reader empty ID(%d)=== \r\n ", (int)u8Idx);
    }

    if((u32Cnt > 500)&&(u32Cnt%500 == 0))
    {
        if(bytes > 4)
        {
            printf("\r\n  === %s, ID(%d), (0x%x),  (0x%x), (0x%x), (0x%x), level (0x%x) === \r\n",
                __FUNCTION__, (int)u8Path, (unsigned int)(*pBuf), (unsigned int)(*(pBuf+1)), (unsigned int)(*(pBuf+2)), (unsigned int)(*(pBuf+3)), (unsigned int)u16Level);
        }
    }

#if 0
    if(stSwDMAPcmSettings.u8Channels == 1)
    {
        u32BuffSizeBySample = u32BuffSizeBySample>>1;
    }
#endif
    //printf(" u32BuffSizeBySample = 0x%lx\r\n", u32BuffSizeBySample);

    if(u32RemainingBufferSize > bytes)
    {
        for ( i = 0; i < (bytes/2); i++ )
        {
            if(stSwDMAPcmSettings[u8Path].bLittleEndian == TRUE)
            {
                if(stSwDMAPcmSettings[u8Path].u8ByteWidth == 2)
                {
                    /* 16 bits -> 16 bits */
                    tmp1 = *bufptr++;
                    tmp2 = *bufptr++;
                }
                else
                {
                    u16Temp = (((*bufptr) - 128) << 8);
                    /* 8 bits -> 16 bits */
                    tmp2 = (u16Temp >> 8);
                    tmp1 = (u16Temp & 0xFF);
                    bufptr++;
                }
            }
            else
            {
                if(stSwDMAPcmSettings[u8Path].u8ByteWidth == 2)
                {
                    /* 16 bits -> 16 bits */
                    tmp2 = *bufptr++;
                    tmp1 = *bufptr++;
                }
                else
                {
                    u16Temp = (((*bufptr) - 128) << 8);
                    /* 8 bits -> 16 bits */
                    tmp2 = (u16Temp >> 8);
                    tmp1 = (u16Temp & 0xFF);
                    bufptr++;
                }
            }

            *(RingBuffer_SwDmaReader[u8Path].WritePointer)++ = tmp1;
            *(RingBuffer_SwDmaReader[u8Path].WritePointer)++ = tmp2;

            if(stSwDMAPcmSettings[u8Path].u8Channels == 1)   // extend mono as stereo
            {
                *(RingBuffer_SwDmaReader[u8Path].WritePointer)++ = tmp1;
                *(RingBuffer_SwDmaReader[u8Path].WritePointer)++ = tmp2;
            }

            if ( RingBuffer_SwDmaReader[u8Path].WritePointer >= RingBuffer_SwDmaReader[u8Path].EndAddress)
            {
                RingBuffer_SwDmaReader[u8Path].WritePointer = RingBuffer_SwDmaReader[u8Path].StartAddress;
            }
        }

        /* flush memory */
        MsOS_FlushMemory();

        #if 0
        MS_U32 u32DataFed = 0;
        u32DataFed = (u32SampleCount * stSwDMAPcmSettings[u8Path].u8ByteWidth);

        if(stSwDMAPcmSettings.u8Channels == 1)
        {
            u32DataFed = u32DataFed * 2;
        }
        #endif
        //MS_U32 u32rPtr = HAL_MAD2_Read_DSP_sram(0x398F, DSP_MEM_TYPE_DM);
        MS_U32 u32wPtr = (RingBuffer_SwDmaReader[u8Path].WritePointer - RingBuffer_SwDmaReader[u8Path].StartAddress)/BYTES_IN_MIU_LINE;
        //printf(" wptr (0x%x), rptr(0x%x) \r\n", (unsigned int)u32wPtr, (unsigned int)u32rPtr);
        RingBuffer_SwDmaReader[u8Path].u32wPtr = u32wPtr;

#if 1 // Munich use S/W DMA reader 0
        //if(u8Path == 1)
        {
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_WtPtr, 0xFFFF, u32wPtr);
        }
#else
        else if(u8Path == 0)
        {
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER2_DDR_WtPtr, 0xFFFF, u32wPtr);
        }
        else
        {
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_WtPtr, 0xFFFF, u32wPtr);
        }
#endif
        return TRUE;
    }
    return FALSE;
}

void HAL_AUDIO_DumpRegInfo(void)
{
    int i = 0, j = 0, intOffset = 0;
    printf(" ====[Audio]==== \n");
    printf("--------------------------------------[112A Bank]--------------------------------------------------\n");
    //--------------------------------------------112A_A0 ~ 112A_AF--------------------------------------------
    intOffset = (int)0x112a00;
    i = 0xa;
    for(j = 0; j<= 7; j++)
    {
        unsigned int temp_reg = intOffset + 16*i + 2*j;
        printf("[%X]=0x%-4X,", temp_reg, HAL_AUDIO_AbsReadReg((MS_U32)temp_reg));
    }
    printf(" @@ \n");

    printf("--------------------------------------[112C Bank]--------------------------------------------------\n");
    //--------------------------------------------112C_00 ~ 112C_0F--------------------------------------------
    intOffset = (int)0x112c00;
    i = 0;
    for(j = 0; j<= 7; j++)
    {
        unsigned int temp_reg = intOffset + 16*i + 2*j;
        printf("[%X]=0x%-4X,", temp_reg, HAL_AUDIO_AbsReadReg((MS_U32)temp_reg));
    }
    printf(" @@ \n");

    //--------------------------------------------112C_40 ~ 112C_4F--------------------------------------------
    intOffset = (int)0x112c00;
    i = 4;
    for(j = 0; j<= 7; j++)
    {
        unsigned int temp_reg = intOffset + 16*i + 2*j;
        printf("[%X]=0x%-4X,", temp_reg, HAL_AUDIO_AbsReadReg((MS_U32)temp_reg));
    }
    printf(" @@ \n");

    //--------------------------------------------112C_60 ~ 112C_6F--------------------------------------------
    intOffset = (int)0x112c00;
    i = 6;
    for(j = 0; j<= 7; j++)
    {
        unsigned int temp_reg = intOffset + 16*i + 2*j;
        printf("[%X]=0x%-4X,", temp_reg, HAL_AUDIO_AbsReadReg((MS_U32)temp_reg));
    }
    printf(" @@ \n");

    printf("--------------------------------------[112D Bank]--------------------------------------------------\n");
    //--------------------------------------------112D_00 ~ 112D_0F--------------------------------------------
    intOffset = (int)0x112d00;
    i = 0;
    for(j = 0; j<= 7; j++)
    {
        unsigned int temp_reg = intOffset + 16*i + 2*j;
        printf("[%X]=0x%-4X,", temp_reg, HAL_AUDIO_AbsReadReg((MS_U32)temp_reg));
    }
    printf(" @@ \n");

    //--------------------------------------------112D_30 ~ 112D_3F--------------------------------------------
    intOffset = (int)0x112d00;
    i = 3;
    for(j = 0; j<= 7; j++)
    {
        unsigned int temp_reg = intOffset + 16*i + 2*j;
        printf("[%X]=0x%-4X,", temp_reg, HAL_AUDIO_AbsReadReg((MS_U32)temp_reg));
    }
    printf(" @@ \n");

    //--------------------------------------------112D_50 ~ 112D_5F--------------------------------------------
    intOffset = (int)0x112d00;
    i = 5;
    for(j = 0; j<= 7; j++)
    {
        unsigned int temp_reg = intOffset + 16*i + 2*j;
        printf("[%X]=0x%-4X,", temp_reg, HAL_AUDIO_AbsReadReg((MS_U32)temp_reg));
    }
    printf(" @@ \n");

    //--------------------------------------------112D_80 ~ 112D_8F--------------------------------------------
    intOffset = (int)0x112d00;
    i = 8;
    for(j = 0; j<= 7; j++)
    {
        unsigned int temp_reg = intOffset + 16*i + 2*j;
        printf("[%X]=0x%-4X,", temp_reg, HAL_AUDIO_AbsReadReg((MS_U32)temp_reg));
    }
    printf(" @@ \n");

    printf("--------------------------------------[1603 Bank]--------------------------------------------------\n");
    //--------------------------------------------1603_90 ~ 1603_9F--------------------------------------------
    intOffset = (int)0x160300;
    i = 9;
    for(j = 0; j<= 7; j++)
    {
        unsigned int temp_reg = intOffset + 16*i + 2*j;
        printf("[%X]=0x%-4X,", temp_reg, HAL_AUDIO_AbsReadReg((MS_U32)temp_reg));
    }
    printf(" @@ \n");
    printf(" ====[Audio]==== \n");
}


