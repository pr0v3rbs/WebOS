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
#ifndef _MFE_RATECTRL_H_
#define _MFE_RATECTRL_H_

//#define CHECK_1SEC_BITRATE

#include "mfe_type.h"
#include "msRateCtrl.h"
#define MAX_FRAMERATE   30

#define SPEC_MAX_QP (31)
#define SPEC_MIN_QP (1)
/* Input for rate control */
typedef struct {
    int nWidth, nHeight;
    int fTargetFrameRate, fMinFrameRate;
    int m_nBitrate;
    int nPCount;    // Number of P-frames betweeen I-frames
    RC_GRANULARITY rcGranularity;
    RC_METHOD rcMethod;
} H263RCInfo;

typedef struct RateCtrl {
    /* Input parameters */

    int m_nWidth, m_nHeight;
    int m_fTargetFrameRate, m_fMinFrameRate;
    int m_nBitrate;
    int m_nPCount;
    RC_GRANULARITY m_rcGranularity;
    RC_METHOD m_rcMethod;
    // Derived variables
    int m_fAvgBitsPerFrame;

    /* rate control variables */

    int m_nFrameCount;  // How many frame coded
    int m_nBufFullness;  // Rate control buffer
    MFE_U64 m_nTotalBits;
    // Last-frame status
    int m_nLastFrameBits, m_nLastFrameAvgQP;
    int m_nLastTargetBits;

    /* Bitrate usage compensation */
    int m_nTargetFullness;
    int m_nDeputyCount, m_nMinDeputyCount;

    /* Variable bitrate */
    int m_fLongTermQP;

    // Model parameters
    int m_nTargetBits;   // target number of bits of current frame
    int m_nNFrame;    // number of macroblocks in a frame
    int m_nWidthInMb;   // Frame width in MB

    /* Below are for STRICT_CONST_BITRATE */

    // Dynamic framerate
    unsigned char m_bDropFrameEnabled;  // Enabled when min-framerate < max-framerate.
    int m_nSkipCount;          // The historic accumulated skip-frame count in one second
    int m_nSkipCountMax;       // Constant: The allowable maximal dropped frame count in 1sec, depends on Min&Max framerate.
    int m_nContSkipCount;   // Result: The determined continuous skip-frame count.
    // Keep track of historic skip-frame status.
    unsigned int m_nWinIndex;
    unsigned int m_nWinIndexMax;    // Constant: corresponds to 1 sec.
    unsigned char m_SkipFrameHistory[MAX_FRAMERATE]; // Skip-frame history: 1 is Skipped, 0 is not.
    // Bitrate monitoring
    // Over-usage: Skip following frame(s) to compensate immediately
    // Shortage: Keep a lower-bound to assure low-delay in constant-bandwidth network.
    int m_nBitrateOverUsage;   // Accumulated bitrate over-usage.
    // Determine how many spare bitcount of skipped frames will be transfered into next coded frame.
    int m_nCompensateBitrate;    // Bitrate transferred to next coded frame.
    int m_nCompensateFrame;     // How many skipped frame will contribute bitrate compenstaion.
    // The maximal bitrate is set to caller-input bitrate
    // And re-calculate internal target bitrate < m_nBitrateMax.
    int m_nBitrateMax;
    // When continuous good frame, gently reducing frameerate to avoid CPU high usage.
    int m_nContGoodFrameCount;

#if defined(CHECK_1SEC_BITRATE)
    // Runtime bitrate
    int m_nBrHistIndex;
    int m_BitrateHistory[MAX_FRAMERATE];
    // Runtime delay: Assuming constant-bandwidth channel
    int m_nUnsentBits;
#endif
} RateCtrl_t;

void rc_Create(RateCtrl_t* rcctx);
void rc_Destroy(RateCtrl_t* rcctx);
// Global
void rc_Init(RateCtrl_t* rcctx, H263RCInfo* pRCInfo);
void rc_Finish(RateCtrl_t* rcctx);
// Each frame
int rc_CheckSkippedFrame(RateCtrl_t* rcctx);
int rc_InitFrame(RateCtrl_t* rcctx, char chFrameType);    // Return the initial frame qp
void rc_UpdateFrame(RateCtrl_t* rcctx, const int totalUsedBits,const int lastAvgQP, MFE_BOOL bDummyFrame);

#endif