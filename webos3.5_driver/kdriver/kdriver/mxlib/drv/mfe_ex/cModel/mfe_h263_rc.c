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

//#include <math.h>
//#include <stdio.h>
#include "mfe_h263_rc.h"
#include "ms_dprintf.h"
#include "mdrv_mfe_math.h"
#include "mfe_type.h"
#include "mfe_common.h"

//! How many seconds of frames are responsible for compensation of bitrate usage.
#define CBR_DEPUTY_SECOND     (1)
#define CVBR_DEPUTY_SECOND    (5)
#define VBR_DEPUTY_SECOND     (10)

#define MIN_DEPUTY_FACTOR 10

//! Default I-frame weighting over inter-frame
#define IFRAME_WEIGHT   (3)

//! Dynamic-framerate mechanism
// Minimal QP when STRICT_CONST_BITRATE
#define SCBR_BEST_QP         (4)
// Bad-frame QP-thr
#define BAD_FRAME_QP_THR    (12)
// Too-good-frame QP-thr
#define GOOD_FRAME_QP_THR   (SCBR_BEST_QP+0)


// Prototypes
static int _rc_ComputeFrameQP(RateCtrl_t* rcctx, char chFrameType);

void rc_Create(RateCtrl_t* rcctx)
{
	rcctx->m_nFrameCount = 0;
}

void rc_Destroy(RateCtrl_t* rcctx)
{

}

void rc_Init(RateCtrl_t* rcctx, H263RCInfo* pRCInfo)
{
    unsigned int i;
    int A;

#ifdef DROP_FRAME
    rcctx->bitrate_count = 0;
#endif
    // Copy parameters, no error checking yet.
    rcctx->m_nWidth = pRCInfo->nWidth;
    rcctx->m_nHeight = pRCInfo->nHeight;
    rcctx->m_fTargetFrameRate = pRCInfo->fTargetFrameRate;
    if (rcctx->m_fTargetFrameRate>MAX_FRAMERATE)
        rcctx->m_fTargetFrameRate = MAX_FRAMERATE;  // For m_SkipFrameHistory[]
    rcctx->m_fMinFrameRate = pRCInfo->fMinFrameRate;
    if (rcctx->m_fMinFrameRate>rcctx->m_fTargetFrameRate)
        rcctx->m_fMinFrameRate = rcctx->m_fTargetFrameRate;
    rcctx->m_nBitrate = pRCInfo->m_nBitrate;
    rcctx->m_nPCount = pRCInfo->nPCount;
    if(rcctx->m_nPCount < 0)
        rcctx->m_nPCount = 0x7FFFFFFF; // nP_is_infinite
    rcctx->m_rcGranularity = pRCInfo->rcGranularity;
    rcctx->m_rcMethod = pRCInfo->rcMethod;

    // Bitrate usage monitoring
    A = 2;   /* delay requirements */
    rcctx->m_nMinDeputyCount = (int)(rcctx->m_fTargetFrameRate*CBR_DEPUTY_SECOND);
    rcctx->m_nTargetFullness = rcctx->m_fAvgBitsPerFrame / 2;
    switch (rcctx->m_rcMethod) {
        case VARIABLE_BITRATE:
            rcctx->m_nDeputyCount = (int)(rcctx->m_fTargetFrameRate*VBR_DEPUTY_SECOND);
            break;
        case CONSTRAINED_VARIABLE_BITRATE:
            rcctx->m_nDeputyCount = (int)(rcctx->m_fTargetFrameRate*CVBR_DEPUTY_SECOND);
            break;
        case STRICT_CONST_BITRATE:
            rcctx->m_nWinIndex = 0;
            rcctx->m_nWinIndexMax = (int)rcctx->m_fTargetFrameRate;
            for (i=0; i<rcctx->m_nWinIndexMax; i++)
                rcctx->m_SkipFrameHistory[i] = 0;    // Not skipped.
            rcctx->m_nSkipCount = 0;
            rcctx->m_nContSkipCount = 0;
            rcctx->m_nBitrateOverUsage = 0;
            rcctx->m_nCompensateBitrate = 0;
            rcctx->m_nCompensateFrame = 0;
            // Treat original bitrate as ceiling, and create internal target bitrate.
            rcctx->m_nBitrateMax = pRCInfo->m_nBitrate;
            rcctx->m_nBitrate = rcctx->m_nBitrateMax * 9 / 10;
            rcctx->m_nContGoodFrameCount = 0;
            break;
        case CONST_BITRATE:
        default:
            rcctx->m_nDeputyCount = (int)(rcctx->m_fTargetFrameRate*CBR_DEPUTY_SECOND);
            break;
    }
    // Derived
    rcctx->m_fAvgBitsPerFrame = rcctx->m_nBitrate / rcctx->m_fTargetFrameRate;

    // Adaptive frame-rate
    if (rcctx->m_rcMethod == STRICT_CONST_BITRATE) {
        rcctx->m_bDropFrameEnabled = rcctx->m_fMinFrameRate < rcctx->m_fTargetFrameRate ? 1 : 0;
        rcctx->m_nSkipCountMax = (int)(rcctx->m_fTargetFrameRate - rcctx->m_fMinFrameRate);
    }
    else {
        rcctx->m_fMinFrameRate = rcctx->m_fTargetFrameRate;
        rcctx->m_bDropFrameEnabled = 0;
        rcctx->m_nSkipCountMax = 0;
    }

    rcctx->m_nFrameCount = 0;
    rcctx->m_nTotalBits = 0;
    rcctx->m_nLastFrameAvgQP = rcctx->m_nLastFrameBits = 0;
    rcctx->m_nBufFullness = 0;
    rcctx->m_fLongTermQP = 0;
    rcctx->m_nNFrame = (rcctx->m_nWidth>>4)*(rcctx->m_nHeight>>4);
    rcctx->m_nWidthInMb = rcctx->m_nWidth>>4;

#if defined(CHECK_1SEC_BITRATE)
    // Runtime bitrate
    rcctx->m_nBrHistIndex = 0;
    for (i=0; i<rcctx->m_fTargetFrameRate; i++)
        rcctx->m_BitrateHistory[i] = (int)rcctx->m_fAvgBitsPerFrame;
    rcctx->m_nUnsentBits = 0;
#endif

    ms_dprintk(DRV_L7,
        "[SWVENC] rc_Init() w=%d, h=%d, FPS=%2.3f, MinFPS=%2.3f, Bitrate=%d,nSkipCountMax=%d\n",
        rcctx->m_nWidth, rcctx->m_nHeight, rcctx->m_fTargetFrameRate, rcctx->m_fMinFrameRate, rcctx->m_nBitrate,rcctx->m_nSkipCountMax);
    ms_dprintk(DRV_L7,
        "[SWVENC] rc_Init() PCount=%d rcGranularity=%d\n",
        rcctx->m_nPCount, rcctx->m_rcGranularity);
}


void rc_Finish(RateCtrl_t* rcctx)
{
#if 0
    if (m_pfDev) {
        free(m_pfDev);
        m_pfDev = NULL;
    }
    if (m_pfAlpha) {
        free(m_pfAlpha);
        m_pfAlpha = NULL;
    }
    if (m_pfVar) {
        free(m_pfVar);
        m_pfVar = NULL;
    }
#endif
}

int rc_CheckSkippedFrame(RateCtrl_t* rcctx)
{
    if (!rcctx->m_bDropFrameEnabled)
        return 0;

    return (rcctx->m_nContSkipCount>0 ? 1:0);
}

int rc_InitFrame(RateCtrl_t* rcctx, char chFrameType)
{
    int nDeputyCount, nInitQP;

    /* Target frame bitcount */

    if (rcctx->m_nFrameCount>0) {
        int delta = 0;

        // 1. Determine the number of future frame to compensate for current bitrate mismatch.
        if (rcctx->m_nFrameCount>rcctx->m_nDeputyCount*MIN_DEPUTY_FACTOR)
            nDeputyCount = rcctx->m_nDeputyCount;
        else if (rcctx->m_nFrameCount<rcctx->m_nMinDeputyCount)
            nDeputyCount = rcctx->m_nMinDeputyCount;
        else
            nDeputyCount = rcctx->m_nMinDeputyCount +
            (rcctx->m_nFrameCount-rcctx->m_nMinDeputyCount)*(rcctx->m_nDeputyCount-rcctx->m_nMinDeputyCount)/(rcctx->m_nDeputyCount*MIN_DEPUTY_FACTOR-rcctx->m_nMinDeputyCount);
        // 2. Calculate the bitcount that this frame should be compensate for.
        if (rcctx->m_rcMethod==CONST_BITRATE) {
            delta = (rcctx->m_nBufFullness>rcctx->m_nTargetFullness) ?
                (int)((rcctx->m_nBufFullness-rcctx->m_nTargetFullness)/rcctx->m_fTargetFrameRate) : rcctx->m_nBufFullness-rcctx->m_nTargetFullness;
        }
        else if (rcctx->m_rcMethod==STRICT_CONST_BITRATE) {
            int d = rcctx->m_nBitrateOverUsage / (int)(rcctx->m_fTargetFrameRate);
            //int d = rcctx->m_nBitrateOverUsage / ((int)(rcctx->m_fTargetFrameRate)-rcctx->m_nSkipCount);
            delta = d - rcctx->m_nCompensateBitrate;
        }
        else if (rcctx->m_rcMethod==VARIABLE_BITRATE) {
            delta = (rcctx->m_nBufFullness-rcctx->m_nTargetFullness) / nDeputyCount;
            if (delta>0 && (rcctx->m_nLastFrameAvgQP>rcctx->m_fLongTermQP))
                delta = delta>>1;  // Make it more variable bitrate to allow better quality
        }
        // 3. Finally, calculate the target bitcount.
        if (rcctx->m_nPCount>0 && chFrameType=='I')
            rcctx->m_nTargetBits = (int)((rcctx->m_fAvgBitsPerFrame*IFRAME_WEIGHT) - delta);
        else {
            rcctx->m_nTargetBits = (int)(rcctx->m_fAvgBitsPerFrame - delta);
        }
        if (rcctx->m_nTargetBits<rcctx->m_nNFrame)
            rcctx->m_nTargetBits = rcctx->m_nNFrame;  // At least one bit per MB. _rc_ComputeFrameQP() don't accept rcctx->m_nTargetBits<=0
    }
    else {
        rcctx->m_nBufFullness = rcctx->m_nTargetFullness;    // Initialization
        rcctx->m_nTargetBits = (int)(rcctx->m_fAvgBitsPerFrame*IFRAME_WEIGHT);    // Must be I-frame
    }

    /* Return initial frame QP */

    nInitQP = _rc_ComputeFrameQP(rcctx, chFrameType);
    rcctx->m_nLastFrameAvgQP = 0;  // Will calculate average value later
    ms_dprintk(DRV_L7,
        "[SWVENC] %c#%5d rc_InitFrame() TargetBits=%7d InitQP=%2d Buffer=%7d Deputy=%d\n",
        chFrameType, rcctx->m_nFrameCount, (int)(rcctx->m_nTargetBits), nInitQP, rcctx->m_nBufFullness, nDeputyCount);
    return nInitQP;
}

void rc_UpdateFrame(RateCtrl_t* rcctx, const int totalUsedBits,const int lastAvgQP, MFE_BOOL bDummyFrame)
{
    int nBufBits = totalUsedBits;

    // Update counter
    rcctx->m_nFrameCount++;
    rcctx->m_nTotalBits += totalUsedBits;

    if (!bDummyFrame) {
        rcctx->m_nLastTargetBits = rcctx->m_nTargetBits;
        rcctx->m_nLastFrameBits = totalUsedBits;
        rcctx->m_nLastFrameAvgQP = lastAvgQP;
        // Variable bitrate
        if (rcctx->m_rcMethod==VARIABLE_BITRATE)
            rcctx->m_fLongTermQP += (rcctx->m_nLastFrameAvgQP-rcctx->m_fLongTermQP) / rcctx->m_nFrameCount;
    }

    // Update buffer status
    rcctx->m_nBufFullness += (int)(nBufBits - rcctx->m_fAvgBitsPerFrame);

    // Skipped frame status update
    if (rcctx->m_bDropFrameEnabled) {
        if (bDummyFrame) {
            if (rcctx->m_nContSkipCount>0)
                rcctx->m_nContSkipCount--;
            if (rcctx->m_nCompensateFrame>0) {
                rcctx->m_nCompensateBitrate += (int)(rcctx->m_fAvgBitsPerFrame)-totalUsedBits;
                rcctx->m_nCompensateFrame--;
            }
            rcctx->m_nBitrateOverUsage += (totalUsedBits-(int)rcctx->m_fAvgBitsPerFrame);
            rcctx->m_nContGoodFrameCount = 0;
        }
        else {
            rcctx->m_nBitrateOverUsage += (totalUsedBits-(int)rcctx->m_fAvgBitsPerFrame);
            if (rcctx->m_nBitrateOverUsage < rcctx->m_nBitrate-rcctx->m_nBitrateMax) {
                rcctx->m_nBitrateOverUsage = rcctx->m_nBitrate-rcctx->m_nBitrateMax;
                ms_dprintk(DRV_L7, "m_nBitrateOverUsage under-run!\n");
            }
            rcctx->m_nContGoodFrameCount++;
        }

        // Update skip history
        rcctx->m_nSkipCount -= rcctx->m_SkipFrameHistory[rcctx->m_nWinIndex];
        rcctx->m_nSkipCount += bDummyFrame;
        if (rcctx->m_nSkipCount>rcctx->m_nSkipCountMax)
            rcctx->m_nSkipCount = rcctx->m_nSkipCountMax;
        rcctx->m_SkipFrameHistory[rcctx->m_nWinIndex++] = bDummyFrame;
        if (rcctx->m_nWinIndex==rcctx->m_nWinIndexMax)
            rcctx->m_nWinIndex = 0;

        // Obeying the minimal framerate; trying to avoid too long continuous skip-frame duration.
        if (rcctx->m_nSkipCount<rcctx->m_nSkipCountMax && !bDummyFrame) {
            rcctx->m_nContSkipCount = 0;
            rcctx->m_nCompensateBitrate = 0;
            rcctx->m_nCompensateFrame = 0;
            // Determine how many next frames need to be skipped.
            if (rcctx->m_nLastFrameAvgQP > BAD_FRAME_QP_THR) {
                // Below equation: Min=0, Max=m_nSkipCountMax-m_nSkipCount
                rcctx->m_nContSkipCount = (rcctx->m_nLastFrameAvgQP-BAD_FRAME_QP_THR)*(rcctx->m_nSkipCountMax-rcctx->m_nSkipCount) / (SPEC_MAX_QP-BAD_FRAME_QP_THR);
                rcctx->m_nCompensateFrame = rcctx->m_nContSkipCount;
            }
            else if (rcctx->m_nContGoodFrameCount>=4 && rcctx->m_nLastFrameAvgQP<=GOOD_FRAME_QP_THR) {
                rcctx->m_nContSkipCount = 1;
                rcctx->m_nCompensateFrame = rcctx->m_nContSkipCount;
                rcctx->m_nContGoodFrameCount = 0;
            }
            if (rcctx->m_nFrameCount>0 && rcctx->m_nBitrateOverUsage>(rcctx->m_nBitrateMax-rcctx->m_nBitrate)) {
                // Below equation: Min=1
                rcctx->m_nContSkipCount += (rcctx->m_nBitrateOverUsage-(rcctx->m_nBitrateMax-rcctx->m_nBitrate)) / (int)rcctx->m_fAvgBitsPerFrame;
                // Marked-out means we do allow the minimal framerate be broken when accumulated bitrate over-usage is too large.
//                 if (rcctx->m_nContSkipCount > rcctx->m_nSkipCountMax-rcctx->m_nSkipCount)
//                     rcctx->m_nContSkipCount = rcctx->m_nSkipCountMax-rcctx->m_nSkipCount;
            }
        }

        ms_dprintk(DRV_L7, "[%d] LastAvgQP=%d, SkipCount=%d, ContSkipCount=%d, OverUsage=%d\n",
            rcctx->m_nFrameCount,
            rcctx->m_nLastFrameAvgQP,
            rcctx->m_nSkipCount,
            rcctx->m_nContSkipCount,
            rcctx->m_nBitrateOverUsage);
    }   // m_bDropFrameEnabled




#if defined(CHECK_1SEC_BITRATE)
    {
        int i, nRuntimeBitrate;
        // Runtime bitrate
        rcctx->m_BitrateHistory[rcctx->m_nBrHistIndex++] = totalUsedBits;
        if (rcctx->m_nBrHistIndex==(int)rcctx->m_fTargetFrameRate)
            rcctx->m_nBrHistIndex = 0;
        nRuntimeBitrate = 0;
        for (i=0; i<(int)rcctx->m_fTargetFrameRate; i++)
            nRuntimeBitrate += rcctx->m_BitrateHistory[i];
        // Runtime delay
        rcctx->m_nUnsentBits += totalUsedBits - (int)(rcctx->m_nBitrateMax/rcctx->m_fTargetFrameRate);
        if (rcctx->m_nUnsentBits<0)
            rcctx->m_nUnsentBits = 0;
        ms_dprintk(DRV_L7,
            "[SWVENC] rc_UpdateFrame(%7d bits, %3d%%) AvgQ=%2d LTQ=%2d LtBitrate=%8d, StBitrate=%8d, Delay=%dms\n",
            totalUsedBits, bDummyFrame?0:(totalUsedBits-rcctx->m_nTargetBits)*100/rcctx->m_nTargetBits, rcctx->m_nLastFrameAvgQP,
            (int)rcctx->m_fLongTermQP, (int)(rcctx->m_nTotalBits*rcctx->m_fTargetFrameRate/rcctx->m_nFrameCount),
            nRuntimeBitrate, rcctx->m_nUnsentBits*1000/rcctx->m_nBitrateMax);
    }
#else
#endif
}

#define SMOOTH_PERIOD 1.0f
static int _rc_ComputeFrameQP(RateCtrl_t* rcctx, char chFrameType)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    ms_dprintk(DRV_L7,"unsupport VT mode (_rc_ComputeFrameQP)!!\n");
    return -1;
#else
    int newQP=0, dQP;
    int buf_rest;
    int buf_rest_pic;
    int frames_left;
	int nAdjust;

    // For the very first frame, guess one qp!
    if (rcctx->m_nFrameCount==0) {
        int nbpMb = 0;
        nbpMb = (int)(rcctx->m_nBitrate/(10*rcctx->m_nNFrame*rcctx->m_fTargetFrameRate));
        if (nbpMb>100)
            return 2;//1;
        else if (nbpMb>20)
            return 4;
        else if (nbpMb>10)
            return 8;
        else if (nbpMb>6)
            return 12;
        else if (nbpMb>4)
            return 16;
        else
            return 20;
    }

    if (rcctx->m_rcMethod==STRICT_CONST_BITRATE) {
        newQP = rcctx->m_nLastFrameAvgQP;
        if (rcctx->m_nTargetBits > rcctx->m_nLastFrameBits)
            newQP -= MFE_MIN(4, rcctx->m_nTargetBits/rcctx->m_nLastFrameBits);
        else
            newQP += MFE_MIN(4, rcctx->m_nLastFrameBits/rcctx->m_nTargetBits);
        newQP = MFE_MIN (SPEC_MAX_QP, newQP);
        newQP = MFE_MAX (/*SPEC_MIN_QP*/SCBR_BEST_QP, newQP);
    }
    else
    if (rcctx->m_rcMethod==CONST_BITRATE/* || rcctx->m_rcMethod==STRICT_CONST_BITRATE*/) {
        buf_rest = ((rcctx->m_nFrameCount/rcctx->m_fTargetFrameRate+SMOOTH_PERIOD) * rcctx->m_nBitrate) - rcctx->m_nTotalBits;
        newQP = rcctx->m_nLastFrameAvgQP;
        frames_left = (int)(SMOOTH_PERIOD * rcctx->m_fTargetFrameRate);
        //if (frames_left > 0)
        {
            buf_rest_pic = (int)(buf_rest / frames_left);
            dQP = MFE_MAX (1, rcctx->m_nLastFrameAvgQP>>3);
            if (rcctx->m_nLastFrameBits > (buf_rest_pic*9)>>3) {
                newQP = MFE_MIN (SPEC_MAX_QP, rcctx->m_nLastFrameAvgQP+dQP);
            }
            else if (rcctx->m_nLastFrameBits < (buf_rest_pic*7)>>3) {
                newQP = MFE_MAX (SPEC_MIN_QP, rcctx->m_nLastFrameAvgQP-dQP);
            }
        }
    }
    else if (rcctx->m_rcMethod==VARIABLE_BITRATE) {
        int nLowQ, nHighQ;
        int nLowBound, nHighBound;
        if (rcctx->m_nPCount>0 && chFrameType=='I') {
            newQP = (int)(rcctx->m_nLastFrameAvgQP-1);
            newQP = MFE_MAX(SPEC_MIN_QP, newQP);
        }
        else {
            int nAdjLTQ;
            if (rcctx->m_nFrameCount>=(int)(rcctx->m_fTargetFrameRate) || rcctx->m_nPCount==0) {
                nAdjLTQ = (int)(rcctx->m_fLongTermQP * (rcctx->m_nTotalBits/rcctx->m_nFrameCount) / rcctx->m_fAvgBitsPerFrame);
                nAdjLTQ = MFE_MIN(SPEC_MAX_QP, MFE_MAX(SPEC_MIN_QP, nAdjLTQ));
            }
            else {
                nAdjLTQ = (int)(rcctx->m_fLongTermQP);   // Wait for stabilization
            }
            MFE_ASSERT(rcctx->m_nTargetBits>0);
            newQP = (int)(nAdjLTQ * rcctx->m_fAvgBitsPerFrame / rcctx->m_nTargetBits);

/*
            nLowBound = nHighBound = 2;
            if (rcctx->m_nLastFrameBits>rcctx->m_nLastTargetBits) {
                nHighBound += int(rcctx->m_nLastFrameBits/rcctx->m_nLastTargetBits);
                if (nHighBound>4) nHighBound=4;
            }
            if (rcctx->m_fAvgBitsPerFrame>rcctx->m_nTargetBits) {
            nHighQ = MIN(31, int(nAdjLTQ)<<nHighBound);
            newQP = MIN(nHighQ, newQP);
            }
            else {
            nLowQ = MAX(1, int(nAdjLTQ)>>nLowBound);
            newQP = MAX(nLowQ, newQP);
            }
*/
            nAdjust = MFE_MAX(2, (int)(nAdjLTQ)>>2);
            nLowBound = (int)nAdjLTQ - nAdjust;
            nHighBound = (int)nAdjLTQ + nAdjust;
            if (rcctx->m_nLastFrameBits>rcctx->m_nLastTargetBits) {
                nAdjust = (int)(rcctx->m_nLastFrameBits/rcctx->m_nLastTargetBits);
                if (nAdjust>2) nAdjust=2;
                nHighBound += nAdjust;
            }
            if (rcctx->m_fAvgBitsPerFrame>rcctx->m_nTargetBits) {
                nHighQ = MFE_MIN(SPEC_MAX_QP, nHighBound);
                newQP = MFE_MIN(nHighQ, newQP);
            }
            else {
                nLowQ = MFE_MAX(SPEC_MIN_QP, nLowBound);
                newQP = MFE_MAX(nLowQ, newQP);
            }
        }
    }

    return newQP;
#endif
}

