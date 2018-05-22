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



#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "ms_dprintf.h"
#include "mhal_mfe.h"
#ifdef __MOBILE_CASE__
#include <stdio.h>
#include <string.h>
#endif

#include "msRateCtrl.h"
#include "OutStrm.h"
#include "m4v_header.h"
#include "BufMng.h"
#include "mfe_reg.h"
#if defined(WIN32)
	#include "mdrv_mfe.h"
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) &&!defined(_KERNEL_MODE_)
    #include "mdrv_mfe.h"
#endif
#include "msRateCtrl.h"
#if defined(_MFE_T8_)
#define MAX_REF_FRAME 2
#define SEARCH_RANGE_X 32
#elif defined(_MFE_M1_)
#define MAX_REF_FRAME 1
#define SEARCH_RANGE_X 16
#endif

// Sync with cModel QExp.h
#define CLOCK_GATING	// Enable clock gating
#define FME_PIPELINE_OPEN	// Enable David's FME speedup version



#define FIELD_DCT_DIFF_THR  (350>>2)	// Subsampled by 4



static const int rgiDefaultIntraQMatrix [64] = {
	8,	17,	18,	19,	21,	23,	25,	27,
	17,	18,	19,	21,	23,	25,	27,	28,
	20,	21,	22,	23,	24,	26,	28,	30,
	21,	22,	23,	24,	26,	28,	30,	32,
	22,	23,	24,	26,	28,	30,	32,	35,
	23,	24,	26,	28,	30,	32,	35,	38,
	25,	26,	28,	30,	32,	35,	38,	41,
	27,	28,	30,	32,	35,	38,	41,	45
};

static const int rgiDefaultInterQMatrix [64] = {
	16,	17,	18, 19, 20,	21,	22,	23,
	17,	18,	19,	20,	21,	22,	23,	24,
	18,	19,	20,	21,	22,	23,	24,	25,
	19,	20,	21,	22,	23,	24,	26,	27,
	20,	21,	22,	23,	25,	26,	27,	28,
	21,	22,	23,	24,	26,	27,	28,	30,
	22,	23,	24,	26,	27,	28,	30,	31,
	23,	24,	25,	27,	28,	30,	31,	33
};
#ifdef MFE_MIU_PROTECT
extern void MHal_MFE_Enable_MIU_Protection(int MIU_TEST_MODE,MFE_CONFIG* pConfig);
#endif
void OutputSwCfg1_Mp4(int nFrmNum, MFE_CONFIG* pConfig);

static unsigned int CeilLog2( unsigned int uiVal)
{
	unsigned int uiTmp = uiVal-1;
	unsigned int uiRet = 0;

	while( uiTmp != 0 )
	{
		uiTmp >>= 1;
		uiRet++;
	}
	return uiRet;
}

void mfeM4VE_DeInit(MFE_CONFIG *pConfig)
{
    IntraUpdateClose(pConfig);
}

void mfeM4VE_Init(MFE_CONFIG *pConfig)
{
	int i;
	M4VEINFO* pInfo = &pConfig->ctxM4veInfo;

	memset(pInfo, 0, sizeof(M4VEINFO));

	switch (pConfig->nCodecType)
	{
	case REG_ENC_MODE_MPG4:
		pInfo->bShortHeader = 0;
             pConfig->bQuantType = 1;
		break;
	case REG_ENC_MODE_H263:
		pInfo->bShortHeader = 1;
             pConfig->bQuantType = 0;
		break;
	default:
		MFE_ASSERT(0);
		pInfo->bShortHeader = 0;
	}

	switch (pConfig->FrameRatex100) {
	case 3000:
		pInfo->nTimeResolution = 30;
		pInfo->nFixedIncrement = 1;
		pInfo->iClockRate = 30;
		break;
	case 1500:
		pInfo->nTimeResolution = 15;
		pInfo->nFixedIncrement = 1;
		pInfo->iClockRate = 15;
		break;
	case 2500:
		pInfo->nTimeResolution = 25;
		pInfo->nFixedIncrement = 1;
		pInfo->iClockRate = 25;
		break;
	case 2997:
		pInfo->nTimeResolution = 30000;
		pInfo->nFixedIncrement = 1001;
		pInfo->iClockRate = 2997;
		break;
	default:
            if(pConfig->nCodecType == REG_ENC_MODE_MPG4) {
                pInfo->nTimeResolution = pConfig->TimeIncreamentRes;
                pInfo->nFixedIncrement = pConfig->VopTimeIncreament;
                 ms_dprintk(DRV_L4,"Setting from API\npInfo->nTimeResolution=%d,pInfo->nFixedIncrement =%d\n"
                    , pInfo->nTimeResolution, pInfo->nFixedIncrement);
            pInfo->iClockRate = pInfo->nTimeResolution;
            }
            else {
                pInfo->nTimeResolution = pConfig->FrameRatex100 / 100;
                pInfo->nFixedIncrement = 1;
                pInfo->iClockRate = pInfo->nTimeResolution;
            }
             MFE_ASSERT(pInfo->nTimeResolution>0);
             MFE_ASSERT(pInfo->nFixedIncrement>0);
	}
	{
		int iClockRate = pInfo->iClockRate-1;
		MFE_ASSERT (iClockRate < 65536);
		//printf("mfeM4VE_Init: iClockRate=%d\n", iClockRate);
		if(iClockRate>0)
		{
			for (pInfo->nNumBitsTimeIncr = 1; pInfo->nNumBitsTimeIncr < 16; pInfo->nNumBitsTimeIncr++)	{
				if (iClockRate == 1)
					break;
				iClockRate = (iClockRate >> 1);
			}
		}
		else
			pInfo->nNumBitsTimeIncr = 1;
		//printf("mfeM4VE_Init: nNumBitsTimeIncr=%d\n", pInfo->nNumBitsTimeIncr);
	}

	if (pConfig->nCodecType==REG_ENC_MODE_MPG4) {
		pInfo->bInterlacedCoding = (pConfig->bInterlace != PROGRESSIVE);
		pInfo->bInterlace = pInfo->bInterlacedCoding;
		pInfo->iSearchRangeForward = SEARCH_RANGE_X;
		pInfo->bQuantizerType = pConfig->bQuantType;

		for (i=0; i<64; i++) {
			pInfo->rgiIntraQuantizerMatrix[i] = rgiDefaultIntraQMatrix[i];
			pInfo->rgiInterQuantizerMatrix[i] = rgiDefaultInterQMatrix[i];
		}

		pInfo->iRoundingControlSwitch = 1;
	}
	else {
		pInfo->iSearchRangeForward = 16;
		pInfo->bQuantizerType = 0;

		pInfo->iRoundingControlSwitch = 0;
	}

	pInfo->SEARCH_RANGE_Y = 16;
	pInfo->IME_ADAPTIVE_WINDOW = 1;
	if (pInfo->SEARCH_RANGE_Y<=16 && pInfo->iSearchRangeForward<=16)
		pInfo->iFCode = 1;
	else
		pInfo->iFCode = 2;

	pInfo->iRoundingControl = pInfo->iRoundingControlSwitch;
	pConfig->vopPredType = I_VOP;
	pInfo->g_rec_en = 1;
	pInfo->g_ref_en = 0;
	pInfo->m_nLastZZ = 63;
	pInfo->m_nFmePrec = 1;	// Default: Half-pixel

	pInfo->m_nBitsResyncMarker = 0;
	pInfo->nNumBitsVPMBnum = CeilLog2((pConfig->nBufWidth*pConfig->nBufHeight)>>8);	// number of bits for macroblock_number

	pInfo->m_tModuloBaseDecd = 0;
	pInfo->m_tModuloBaseDisp = 0;
	pInfo->m_tFutureRef = 0;
	pInfo->m_tPastRef = 0;
	pInfo->m_t = 0;
	pInfo->m_nBitsModuloBase = 0;
	pInfo->m_iVopTimeIncr = 0;

	pInfo->intQP = 0;

	MFE_ASSERT((pConfig->nBufWidth&0xF)==0);
	MFE_ASSERT((pConfig->nBufHeight&0xF)==0);

	pInfo->bAllowSkippedPMBs = !(pConfig->nBbetweenP>0);
	pInfo->nAllowDirectBMBs = 1;

	pInfo->bHECEnabled = 1;
	pInfo->nHECPeriod = 3;

	// H263
	if (pConfig->nCodecType==REG_ENC_MODE_H263) {
		if (pConfig->nBufHeight<=400)
			pInfo->nGobUnit = 0;
		else if (pConfig->nBufHeight<=800)
			pInfo->nGobUnit = 1;
		else if (pConfig->nBufHeight<=1152)
			pInfo->nGobUnit = 2;
		else
			MFE_ASSERT(0);
		pInfo->m_iGobFrameId = 0;
	}

	// Rate control
	MfeDrvRateControlInit(pConfig);

	// FDC header initialization
	osCreate(&pConfig->m_OutStream);
	osSetWriteBuffer(&pConfig->m_OutStream, pConfig->m_FdcBuffer);

	// Intra-update initialization
	IntraUpdateInit(pConfig);
}


void mfeM4VE_EncodeFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo)
{
    M4VEINFO* pInfo = &pConfig->ctxM4veInfo;
    BitsInfo* pBitsInfo = &pConfig->ctxBitsInfo;
    OutStream* pStream = &pConfig->m_OutStream;
    const unsigned char gBITMASK[8] = { 0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe };
	// This is done in msAPI_MFE_EnOneFrm()
	//mfeSetVopType(pConfig, pGopInfo); // Will set pConfig->vopPredType

	// Config
	// Frame type
	pInfo->bInterlace = pInfo->bInterlacedCoding;
	if (!pInfo->bShortHeader) {
		if (pConfig->vopPredType==P_VOP) {
			pInfo->iRoundingControlSwitch ^= 1;
			pInfo->iRoundingControl = pInfo->iRoundingControlSwitch;
		}
		else if (pConfig->vopPredType==B_VOP)
			pInfo->iRoundingControl = 0;
	}
	pInfo->g_rec_en = (MFE_U8)((pConfig->nPbetweenI==0 || pConfig->vopPredType==B_VOP) ? /*0*/(pGopInfo->nCodingOrder&1) : 1);	// nCodingOrder&1 is for testing.
	pInfo->g_ref_en = pConfig->vopPredType==I_VOP ? 0 : (pConfig->vopPredType==P_VOP?1:3);
	pInfo->bMvStore = (pConfig->nBbetweenP>0) & ((pConfig->vopPredType==P_VOP) | (/*IVOP_MVSTORE*/1 & (pConfig->vopPredType==I_VOP) & (pGopInfo->nCodingOrder!=0)));

#ifdef _GenSkipHeader_
    if(pConfig->VTMode && rc_CheckSkippedFrame(&pConfig->VTRateCtrl)){
        codeNonCodedVOPShortHead(pConfig, pStream);
        // Finalize
        osFlushAll(&pConfig->m_OutStream);
        return;
    }
#endif
    if(pConfig->VTMode) {
        char chFrameType;
        if(pConfig->vopPredType==I_VOP)
            chFrameType = 'I';
        else
            chFrameType = 'P';
        {
            //update original ratecontrol struct for set reg.
            CVBRRateControl* rcCtx = &pConfig->ctxRateControl;
            RateCtrl_t* rcVTCtx = &pConfig->VTRateCtrl;
            rcCtx->m_rcGranularity=MBLEVELRC;
            rcCtx->m_nFrameQStep = rcQP2Qstep(rcCtx,rc_InitFrame(&pConfig->VTRateCtrl, chFrameType));
            rcCtx->m_nTargetMbBits = rcVTCtx->m_nTargetBits / rcVTCtx->m_nNFrame;
            rcCtx->m_nMinQP = SPEC_MIN_QP;
            rcCtx->m_nMaxQP = SPEC_MAX_QP;
            rcCtx->m_nMinQStep = SPEC_MIN_QP<<QS_SHIFT_FACTOR;
            rcCtx->m_nMaxQStep = SPEC_MAX_QP<<QS_SHIFT_FACTOR;
            rcCtx->m_nCodecType = pConfig->nCodecType;
            pInfo->intQP = rcQstep2QP(rcCtx, rcCtx->m_nFrameQStep);
            ms_dprintf(DRV_L4,"m_rcGranularity = %d,rcCtx->m_nTargetMbBits=%d",rcCtx->m_rcGranularity,rcCtx->m_nTargetMbBits);
        }
    }
    else
        pInfo->intQP = cvbr_InitFrame(&pConfig->ctxRateControl, pConfig->vopPredType, 0);

	// Frame time
	pInfo->m_t = pGopInfo->nDispOrder;
	if (pConfig->vopPredType != B_VOP) {
		pInfo->m_tPastRef = pInfo->m_tFutureRef;
		pInfo->m_tFutureRef = pInfo->m_t;
	}
	// Handled in codeVOPHeadInitial()
	//m_tModuloBaseDecd
	//m_tModuloBaseDisp
	//m_nBitsModuloBase
	//m_iVopTimeIncr

	// VideoPacket, GOB parameters
	pInfo->m_nBitsResyncMarker = 17/*NUMBITS_VP_RESYNC_MARKER*/;
	if(pConfig->vopPredType == P_VOP)
		pInfo->m_nBitsResyncMarker += (pInfo->iFCode - 1);
	else if(pConfig->vopPredType == B_VOP) {
		// tung : VP 4.21
		pInfo->m_nBitsResyncMarker += (pInfo->iFCode - 1);
		if (pInfo->m_nBitsResyncMarker<18)
			pInfo->m_nBitsResyncMarker = 18;
		// ~tung
	}

	// H263
	pInfo->m_iGobFrameId = (pConfig->vopPredType==P_VOP ? 1 : 0);

	// Buffer management
	m4veGetBufferAddr(pConfig);

	//MODE 0: (Checking range > real range ) 	reg_mfe_s_marb_miu_bound_err = 0
	//MODE 1: (Checking range < real range ) 	reg_mfe_s_marb_miu_bound_err = 1
	//#define TEST_MIU_PROTECTION_MODE 0
#ifdef MFE_MIU_PROTECT
	MHal_MFE_Enable_MIU_Protection(TEST_MIU_PROTECTION_MODE,pConfig);
#endif


	// Prepare header
	osReset(&pConfig->m_OutStream);
	if (!pInfo->bShortHeader)
	{
		// VOS and VO header
		if (pGopInfo->nCodingOrder==0)
		{
			codeSequenceHead(pConfig, pStream);
			codeVOHead(pConfig, pStream);
		}
		// VOL header
#if defined(_MFE_T8_)
		if (pGopInfo->nCodingOrder==0||pConfig->vopPredType==I_VOP)
#else
		if (pGopInfo->nCodingOrder==0)
#endif
		{
			codeVOLHead(pConfig, pStream);
		}
	}

	// VOP header
	if (pConfig->m_cvbrFrameSkip>0 && pConfig->vopPredType==P_VOP) {
		if (pConfig->m_bGenSkipVopHeader) {
			if (pInfo->bShortHeader)
				codeNonCodedVOPShortHead(pConfig, pStream);
			else
				codeNonCodedVOPHead(pConfig, pStream);
		}
	}
	else {
		if (pInfo->bShortHeader)
			codeVOPShortHead(pConfig, pStream);
		else
			codeVOPHead(pConfig, pStream);
	}
	// Finalize
	osFlushAll(&pConfig->m_OutStream);
	pBitsInfo->ptr = pStream->m_pbFrameBuffer;
	pBitsInfo->len = pStream->m_nByteCount;
	if ((pStream->BC_nCumulativeBits&7)==0) {
		pBitsInfo->bit_len = 0;
		pBitsInfo->bits = 0;
	}
	else {
		pBitsInfo->len--;
		pBitsInfo->bit_len = pStream->BC_nCumulativeBits&7;
		pBitsInfo->bits = pStream->m_pbFrameBuffer[pStream->BC_nCumulativeBits>>3] & gBITMASK[pBitsInfo->bit_len];
	}
/*
#if DEBUG_LEVEL
	{	// DEBUG codes
		int i;
		ms_dprintk(DRV_L4,"FDC bitcount %d ==> ", pBitsInfo->len*8+pBitsInfo->bit_len);
		for (i=0; i<pBitsInfo->len; i++) {
			ms_dprintk(DRV_L4,"%02x ", pBitsInfo->ptr[i]);
		}
		ms_dprintk(DRV_L4,"%02x(msb %d bits)\n", pBitsInfo->bits, pBitsInfo->bit_len);
	}
#endif
*/
	// Set reg and start encoding
	OutputSwCfg1_Mp4(pGopInfo->nCodingOrder, pConfig);
	ms_dprintk(DRV_L4,"In mfeM4VE_EncodeFrame() After OutputSwCfg1_Mp4\n");
}



void mfeM4VE_UpdateFrame(MFE_CONFIG *pConfig)
{
	if (pConfig->vopPredType==I_VOP)
		IntraUpdateInit(pConfig);
	IntraUpdateFrame(pConfig);

	// Rate control
	MfeDrvRateControlUpdate(pConfig, 0);

}

// Trd must >= Trb, no checking here.
static void TrbTrdReduction(int* Trb, int* Trd)
{
	int r;
	int d = *Trd;
	int b = *Trb;

	if (b==d) {
		*Trb = *Trd = 1;
		return;
	}

	// b < d
	while(1) {
		r = d%b;
		if(r==0) {
			*Trb /= b;
			*Trd /= b;
			return;
		}
		d = b;
		b = r;
	}
}

void OutputSwCfg1_Mp4(int nFrmNum, MFE_CONFIG* pConfig)
{

	int nTarWriteCount;
	int nRegWriteCount;
	int nTarFDCCount;
	int nRegFDCCount;

	M4VEINFO* pM4veInfo = &pConfig->ctxM4veInfo;
	BufInfo* pBufInfo = &pConfig->ctxBufInfo;
	BitsInfo* pBitsInfo = &pConfig->ctxBitsInfo;
	CVBRRateControl* rcCtx = &pConfig->ctxRateControl;
    MFE_REG* mfe_reg = &pConfig->mfe_reg;

	//////////////////////////////////////////////////////////////////////////
	// Sequence-wide settings

	if (nFrmNum==0) {
		memset(mfe_reg, 0, sizeof(MFE_REG));	// Initial
		mfe_reg->reg_mfe_g_enc_mode = pM4veInfo->bShortHeader ? REG_ENC_MODE_H263 : REG_ENC_MODE_MPG4;
		mfe_reg->reg_mfe_g_pic_width = pConfig->nBufWidth;
		mfe_reg->reg_mfe_g_pic_height = pConfig->nBufHeight;
#ifdef _MFE_M1_
		mfe_reg->reg_mfe_g_jpe_buffer_mode=0;
#endif
		if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {	// MPEG-4
			// Qtable
			mfe_reg->reg_mfe_g_qmode = pM4veInfo->bQuantizerType;

			mfe_reg->reg_mfe_g_mp4_itlc = pM4veInfo->bInterlacedCoding;
			mfe_reg->reg_mfe_g_mp4_pskip_off = (pM4veInfo->bAllowSkippedPMBs==0);
			mfe_reg->reg_mfe_g_mp4_acp = (0<<1) | 1;	// disabled
			mfe_reg->reg_mfe_g_er_hec = pM4veInfo->bHECEnabled;
			mfe_reg->reg_mfe_g_er_hec_t = pM4veInfo->nHECPeriod;
			mfe_reg->reg_mfe_g_mp4_direct_en = pM4veInfo->nAllowDirectBMBs==0 ? 0 : 1;
			mfe_reg->reg_mfe_g_inter_pref = 512;	// inter-intra selection

			// ME partition type
			mfe_reg->reg_mfe_s_me_16x16_disable = 0;
			mfe_reg->reg_mfe_s_me_8x8_disable = 0;
			mfe_reg->reg_mfe_s_me_16x8_disable = 1;
			mfe_reg->reg_mfe_s_me_8x16_disable = 1;
			mfe_reg->reg_mfe_s_me_8x4_disable = 1;
			mfe_reg->reg_mfe_s_me_4x8_disable = 1;
			mfe_reg->reg_mfe_s_me_4x4_disable = 1;

			// MDC
			mfe_reg->reg_mfe_s_mdc_total_mb_bw = pM4veInfo->nNumBitsVPMBnum;
			mfe_reg->reg_mfe_s_mdc_m4vop_tinc_bw = pM4veInfo->nNumBitsTimeIncr;

			// Field DCT
			mfe_reg->reg_mfe_g_mp4_flddct_en = pM4veInfo->bInterlace ? 1 : 0;
			mfe_reg->reg_mfe_g_mp4_flddct_diff_thr = FIELD_DCT_DIFF_THR;
		}
		if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_H263) {	// H263
			// Qtable
			mfe_reg->reg_mfe_g_qmode = 0;

			mfe_reg->reg_mfe_g_er_h263_unit = pM4veInfo->nGobUnit;
			mfe_reg->reg_mfe_g_inter_pref = 512;	// inter-intra selection

			// ME partition type
			mfe_reg->reg_mfe_s_me_16x16_disable = 0;
			mfe_reg->reg_mfe_s_ime_sr16 = 1;
			mfe_reg->reg_mfe_s_ime_umv_disable = 1;
			mfe_reg->reg_mfe_s_me_8x8_disable = 1;
			mfe_reg->reg_mfe_s_me_16x8_disable = 1;
			mfe_reg->reg_mfe_s_me_8x16_disable = 1;
			mfe_reg->reg_mfe_s_me_8x4_disable = 1;
			mfe_reg->reg_mfe_s_me_4x8_disable = 1;
			mfe_reg->reg_mfe_s_me_4x4_disable = 1;
		}
#ifdef _MFE_M1_
            // pre-fetch
            mfe_reg->reg_mfe_s_prfh_cryc_en = 1;
            mfe_reg->reg_mfe_s_prfh_refy_en = 1;
            mfe_reg->reg_mfe_s_prfh_cryc_idle_cnt = 0;
            mfe_reg->reg_mfe_s_prfh_refy_idle_cnt = 0;

		if (pConfig->m_bFrameMode) {
			mfe_reg->reg_mfe_g_jpe_buffer_mode = 1;	// frame-mode
			mfe_reg->reg_mfe_g_jpe_fsvs_mode = 0;
		}
		else {
			mfe_reg->reg_mfe_g_jpe_buffer_mode = 0;	// row-mode
			mfe_reg->reg_mfe_g_jpe_multibuf_mode = 0;
			mfe_reg->reg_mfe_g_jpe_enc_mode = 0;	// 420
			mfe_reg->reg_mfe_g_jpe_qfactor = 3;
#if defined(TEST_INPUT_ROW_MODE_HWAUTO_OK) || defined(TEST_INPUT_ROW_MODE_HWAUTO_FAIL1) || defined(TEST_INPUT_ROW_MODE_HWAUTO_FAIL2) || defined(TEST_INPUT_ROW_MODE_HWAUTO_FAIL3)
			mfe_reg->reg_mfe_g_jpe_fsvs_mode=3;
#elif defined(TEST_INPUT_ROW_MODE_HW_OK) || defined(TEST_INPUT_ROW_MODE_HW_FAIL)
			mfe_reg->reg_mfe_g_jpe_fsvs_mode=2;
#elif defined(TEST_INPUT_ROW_MODE_SWHW_OK) || defined(TEST_INPUT_ROW_MODE_SWHW_FAIL)
			mfe_reg->reg_mfe_g_jpe_fsvs_mode=1;
#else
			mfe_reg->reg_mfe_g_jpe_fsvs_mode=2;
#endif
		}

#ifdef SW_BUF_MODE
		mfe_reg->reg_mfe_g_sw_buffer_mode = 1;
#else
		mfe_reg->reg_mfe_g_sw_buffer_mode = 0;
#endif
#endif //_MFE_M1_
	}
	else {
		mfe_reg->reg_mfe_g_frame_start_sw = 0;
	}


	//////////////////////////////////////////////////////////////////////////
	// Frame-wide settings
#ifdef CLOCK_GATING
    mfe_reg->reg16 = 0xffff;	// clock gating
#endif

	// Input buffer address: Must be 256-byte aligned.
	MFE_ASSERT((pBufInfo->m_nCurYAddr.miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nCurCAddr.miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefYAddr[0].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefCAddr[0].miuAddress&0xFF)==0);
    if(pConfig->m_bFrameMode==0) {
	MFE_ASSERT((pBufInfo->m_nRefYAddr[1].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefCAddr[1].miuAddress&0xFF)==0);
    }
	MFE_ASSERT((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress&0x7)==0);
	mfe_reg->reg_mfe_g_cur_y_adr_low  = (MFE_U16)((pBufInfo->m_nCurYAddr.miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_cur_y_adr_high = (MFE_U16)(pBufInfo->m_nCurYAddr.miuAddress>>(8+16));
	mfe_reg->reg_mfe_g_cur_c_adr_low  = (MFE_U16)((pBufInfo->m_nCurCAddr.miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_cur_c_adr_high = (MFE_U16)(pBufInfo->m_nCurCAddr.miuAddress>>(8+16));

	mfe_reg->reg_mfe_g_ref_y_adr0_low  = (MFE_U16)((pBufInfo->m_nRefYAddr[0].miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_ref_y_adr0_high = (MFE_U16)(pBufInfo->m_nRefYAddr[0].miuAddress>>(8+16));
	mfe_reg->reg_mfe_g_ref_c_adr0_low  = (MFE_U16)((pBufInfo->m_nRefCAddr[0].miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_ref_c_adr0_high = (MFE_U16)(pBufInfo->m_nRefCAddr[0].miuAddress>>(8+16));
     if(pConfig->m_bFrameMode==0) {
	mfe_reg->reg_mfe_g_ref_y_adr1_low  = (MFE_U16)((pBufInfo->m_nRefYAddr[1].miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_ref_y_adr1_high = (MFE_U16)(pBufInfo->m_nRefYAddr[1].miuAddress>>(8+16));
	mfe_reg->reg_mfe_g_ref_c_adr1_low  = (MFE_U16)((pBufInfo->m_nRefCAddr[1].miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_ref_c_adr1_high = (MFE_U16)(pBufInfo->m_nRefCAddr[1].miuAddress>>(8+16));
    }
#ifdef USE_CUR_AS_REC
	MFE_ASSERT(pBufInfo->m_nRecYAddr.miuAddress==pBufInfo->m_nCurYAddr.miuAddress);
	MFE_ASSERT(pBufInfo->m_nRecCAddr.miuAddress==pBufInfo->m_nCurCAddr.miuAddress);
#endif
	mfe_reg->reg_mfe_g_rec_y_adr_low  = (MFE_U16)((pBufInfo->m_nRecYAddr.miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_rec_y_adr_high = (MFE_U16)(pBufInfo->m_nRecYAddr.miuAddress>>(8+16));
	mfe_reg->reg_mfe_g_rec_c_adr_low  = (MFE_U16)((pBufInfo->m_nRecCAddr.miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_rec_c_adr_high = (MFE_U16)(pBufInfo->m_nRecCAddr.miuAddress>>(8+16));

	// Output buffers: Must be 8-byte aligned.
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
	mfe_reg->reg_mfe_s_bspobuf_hw_en = 0;
#if defined(USE_HW_DBL_OBUF)
	mfe_reg->reg_mfe_s_bspobuf_hw_en = 1;
#endif
#else
	mfe_reg->reg_mfe_s_bspobuf_sadr_low  = (MFE_U16)((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress>>3)&0xFFFF);
	mfe_reg->reg_mfe_s_bspobuf_sadr_high = (MFE_U16)(pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress>>(3+16));
	mfe_reg->reg_mfe_s_bspobuf_eadr_low  = (MFE_U16)(((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress+pBufInfo->m_OutBufferSize-8)>>3)&0xFFFF);
	mfe_reg->reg_mfe_s_bspobuf_eadr_high = (MFE_U16)((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress+pBufInfo->m_OutBufferSize-8)>>(3+16));
#endif
	// GN
	mfe_reg->reg_mfe_s_gn_sadr_low  = (MFE_U16)((pBufInfo->m_nGNAddr.miuAddress>>3)&0xFFFF);
	mfe_reg->reg_mfe_s_gn_sadr_high = (MFE_U16)(pBufInfo->m_nGNAddr.miuAddress>>(3+16));
#ifdef _MFE_M1_
    mfe_reg->reg_mfe_s_gn_sadr_mode =
#if defined(GN_WHOLE_FRAME)
        1;
#else
        0;
#endif
#endif

	// MV store
	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {
		mfe_reg->reg_mfe_g_mp4_direct_mvstore = pBufInfo->m_bEnableMvStore;
		if (mfe_reg->reg_mfe_g_mp4_direct_mvstore) {
			mfe_reg->reg_mfe_s_mvobuf_sadr_low  = (MFE_U16)((pBufInfo->m_nMvStoreAddr.miuAddress>>3)&0xFFFF);
			mfe_reg->reg_mfe_s_mvobuf_sadr_high = (MFE_U16)(pBufInfo->m_nMvStoreAddr.miuAddress>>(3+16));
		}
		if (mfe_reg->reg_mfe_g_mp4_direct_en) {
			mfe_reg->reg_mfe_s_gn_mvibuf_sadr_low  = (MFE_U16)((pBufInfo->m_nMvStoreAddr.miuAddress>>3)&0xFFFF);
			mfe_reg->reg_mfe_s_gn_mvibuf_sadr_high = (MFE_U16)(pBufInfo->m_nMvStoreAddr.miuAddress>>(3+16));
		}
	}
#ifdef _MFE_M1_
	// IMI buffer
    if(pConfig->MfeAdvInfo.input_imi_en) {
        mfe_reg->reg_mfe_s_marb_eimi_block = 0x1;
        WriteRegMFE(0x68, mfe_reg->reg68, "[%d] reg68", nRegWriteCount, "IMI enable");
    }
    else
        mfe_reg->reg_mfe_s_marb_eimi_block = 0x0;

	mfe_reg->reg_mfe_s_marb_lbwd_mode = 0;
	mfe_reg->reg_mfe_s_marb_imi_sadr_low = 0;
	mfe_reg->reg_mfe_s_marb_imi_sadr_high = 0;

    if (pConfig->vopPredType!=I_VOP && pConfig->MfeAdvInfo.low_bandwidth_en && pConfig->imi_size>0) {
        mfe_reg->reg_mfe_s_marb_eimi_block = 1;
        mfe_reg->reg_mfe_s_marb_lbwd_mode = 1;
        mfe_reg->reg_mfe_s_marb_imi_sadr_low = (pConfig->imi_addr>>3)&0xFFFF;
        mfe_reg->reg_mfe_s_marb_imi_sadr_high = pConfig->imi_addr>>(3+16);
        switch (pConfig->imi_size) {
            // 0: 64kB, 1:32kB, 2:16kB, 3:8kB
        case 0x10000:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 0;
            break;
        case 0x8000:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 1;
            break;
        case 0x4000:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 2;
            break;
        case 0x2000:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 3;
            break;
        default:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 0;
            MFE_ASSERT(0);
        }
    }
#endif
	mfe_reg->reg_mfe_g_frame_type = pConfig->vopPredType==I_VOP ? 0 : (pConfig->vopPredType==P_VOP?1:2);
	mfe_reg->reg_mfe_g_ref_no = pConfig->vopPredType==P_VOP ? 0 : (pConfig->vopPredType==B_VOP&&(!pM4veInfo->nAllowDirectBMBs)&&pM4veInfo->g_ref_en!=3 ? 0 : 1);

	// ME setting
	mfe_reg->reg_mfe_s_ime_mesr_max_addr = (pM4veInfo->SEARCH_RANGE_Y==16 ? 95 : 83);//0x5f;
	mfe_reg->reg_mfe_s_ime_mesr_min_addr = (pM4veInfo->SEARCH_RANGE_Y==16 ? 0 : 16);//0;
	mfe_reg->reg_mfe_s_ime_mvx_min = -(pConfig->vopPredType==I_VOP?0:pM4veInfo->iSearchRangeForward) + 32;	// Min X is -pVopMd->iSearchRangeForward
	mfe_reg->reg_mfe_s_ime_mvx_max = ((pConfig->vopPredType==I_VOP?0:pM4veInfo->iSearchRangeForward)-2) + 32;	// Max X is pVopMd->iSearchRangeForward-1
	mfe_reg->reg_mfe_s_ime_mvy_min = -pM4veInfo->SEARCH_RANGE_Y + 16;					// Min Y
	mfe_reg->reg_mfe_s_ime_mvy_max = (pM4veInfo->SEARCH_RANGE_Y==16 ? 15 : 8) + 16;		// Max Y
	mfe_reg->reg_mfe_s_ime_sr16 = (mfe_reg->reg_mfe_s_ime_mvx_min>=16 ? 1 : 0);
	mfe_reg->reg_mfe_s_mesr_adapt = pM4veInfo->IME_ADAPTIVE_WINDOW?1:0;
	mfe_reg->reg_mfe_s_me_ref_en_mode = pM4veInfo->g_ref_en;
	mfe_reg->reg_mfe_s_ime_ime_wait_fme = 1;
#ifdef FME_PIPELINE_OPEN
	mfe_reg->reg_mfe_s_fme_pipeline_on = mfe_reg->reg_mfe_s_ime_ime_wait_fme ? 1 : 0;
#else
	mfe_reg->reg_mfe_s_fme_pipeline_on = 0x0;
#endif

	// Intra update
    mfe_reg->reg_mfe_s_txip_irfsh_en = 0;

    if(mfe_reg->reg_mfe_s_txip_irfsh_en)
    {
        int i;
        int count, prv_intra;
        int start[2], end[2];
        IntraUpdateContext* ctx = &pConfig->m_IUContext;

        count = 0;
        prv_intra = 0;
        for (i=0; i<ctx->nTotalMb; i++) {
        	if (ctx->pHwMbMap[i].intra) {
                if (prv_intra==0) {
                    count++;
                    if (count>2) {
                        MFE_ASSERT(0);
                    }
                    start[count-1] = end[count-1] = i;
                }
                else
                    end[count-1] = i;
        	}
        	prv_intra = ctx->pHwMbMap[i].intra;
        }
        if (count>0) {
            mfe_reg->reg_mfe_s_txip_irfsh_en |= 1;
            mfe_reg->reg_mfe_s_txip_irfsh_mb_s0 = start[0];
            mfe_reg->reg_mfe_s_txip_irfsh_mb_e0 = end[0];
        }
        if (count>1) {
        	mfe_reg->reg_mfe_s_txip_irfsh_en |= 2;
        	mfe_reg->reg_mfe_s_txip_irfsh_mb_s1 = start[1];
        	mfe_reg->reg_mfe_s_txip_irfsh_mb_e1 = end[1];
        }
    }



	// LastZZ
	mfe_reg->reg_mfe_s_quan_idx_last = pM4veInfo->m_nLastZZ;
	if (mfe_reg->reg_mfe_s_quan_idx_last<63)
		mfe_reg->reg_mfe_s_quan_idx_swlast = 1;
		else
		mfe_reg->reg_mfe_s_quan_idx_swlast = 0;

	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {	// MPEG-4
		mfe_reg->reg_mfe_s_fme_quarter_disable = 1;
		mfe_reg->reg_mfe_s_fme_half_disable = (pM4veInfo->m_nFmePrec==0);
		//mfe_reg->reg_mfe_s_fme_one_mode = 0;
		mfe_reg->reg_mfe_s_fme_pmv_enable = 0;
		mfe_reg->reg_mfe_s_fme_mode_no = pConfig->vopPredType==P_VOP ? 1 : 0;
		mfe_reg->reg_mfe_s_fme_mode0_refno = pConfig->vopPredType==P_VOP ? 0 : ((pConfig->vopPredType==B_VOP&&pM4veInfo->g_ref_en!=3)?0:1);
		mfe_reg->reg_mfe_s_fme_mode1_refno = 0;
		mfe_reg->reg_mfe_s_fme_mode2_refno = 0;

		mfe_reg->reg_mfe_g_mp4_rounding_ctrl = pM4veInfo->iRoundingControl;
		// codeVideoPacketHeader
		MFE_ASSERT(pM4veInfo->m_nBitsResyncMarker==17||pM4veInfo->m_nBitsResyncMarker==18);
		mfe_reg->reg_mfe_s_mdc_m4vpktpzero = (pM4veInfo->m_nBitsResyncMarker-1==17) ? 1 : 0;
		mfe_reg->reg_mfe_s_mdc_m4time = pM4veInfo->m_nBitsModuloBase;
		mfe_reg->reg_mfe_s_mdc_m4vop_tinc = pM4veInfo->m_iVopTimeIncr;

		// B-direct mode
		mfe_reg->reg_mfe_g_mp4_direct_pref = 129;
		{
			int trd = pM4veInfo->m_tFutureRef - pM4veInfo->m_tPastRef;
			int trb = pM4veInfo->m_t - pM4veInfo->m_tPastRef;
			if (trb&trb)
				TrbTrdReduction(&trb, &trd);
			mfe_reg->reg_mfe_g_mp4_direct_trb = trb;
			mfe_reg->reg_mfe_g_mp4_direct_trd = trd;
		}
	}


	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_H263) {	// H263
		mfe_reg->reg_mfe_s_fme_quarter_disable = 1;
		mfe_reg->reg_mfe_s_fme_half_disable = (pM4veInfo->m_nFmePrec==0);
		//mfe_reg->reg_mfe_s_fme_one_mode = 1;
		mfe_reg->reg_mfe_s_fme_pmv_enable = 0;
		mfe_reg->reg_mfe_s_fme_mode_no = 0;
		mfe_reg->reg_mfe_s_fme_mode0_refno = 0;
		mfe_reg->reg_mfe_s_fme_mode1_refno = 0;
		mfe_reg->reg_mfe_s_fme_mode2_refno = 0;

		mfe_reg->reg_mfe_s_mdc_gob_frame_id = pM4veInfo->m_iGobFrameId;
	}
            ms_dprintf(DRV_L4,"m_rcGranularity = %d",rcCtx->m_rcGranularity);
	// MBR
	mfe_reg->reg_mfe_g_mbr_en = rcCtx->m_rcGranularity==MBLEVELRC ? 1 : 0;
	mfe_reg->reg_mfe_s_mbr_pqp_dlimit = LEFT_QP_DIFF_LIMIT;
	mfe_reg->reg_mfe_s_mbr_uqp_dlimit = TOP_QP_DIFF_LIMIT;
	// er_en
	if (rcCtx->m_nVPMbRow>0 && rcCtx->m_nVPSize<=0)
		mfe_reg->reg_mfe_g_er_mode = 0;
	else if (rcCtx->m_nVPMbRow<=0 && rcCtx->m_nVPSize>0)
		mfe_reg->reg_mfe_g_er_mode = 1;
	else if (rcCtx->m_nVPMbRow>0 && rcCtx->m_nVPSize>0)
		mfe_reg->reg_mfe_g_er_mode = 2;
	else
		mfe_reg->reg_mfe_g_er_mode = 3;

	if (rcCtx->m_nVPMbRow==0 || rcCtx->m_nVPMbRow==1)
		mfe_reg->reg_mfe_g_er_mby = 0;
	else if (rcCtx->m_nVPMbRow==2)
		mfe_reg->reg_mfe_g_er_mby = 1;
	else if (rcCtx->m_nVPMbRow==4)
		mfe_reg->reg_mfe_g_er_mby = 2;
	else if (rcCtx->m_nVPMbRow==8)
		mfe_reg->reg_mfe_g_er_mby = 3;
	else if (rcCtx->m_nVPMbRow>0)
		MFE_ASSERT(0);
	if (mfe_reg->reg_mfe_g_er_mode==1 || mfe_reg->reg_mfe_g_er_mode==2)
		mfe_reg->reg_mfe_g_er_bs_th = rcCtx->m_nVPSize;
	mfe_reg->reg_mfe_g_qscale = pM4veInfo->intQP;
	mfe_reg->reg_mfe_s_mbr_frame_qstep = rcCtx->m_nFrameQStep;
	mfe_reg->reg_mfe_s_mbr_tmb_bits = rcCtx->m_nTargetMbBits;
	// QP/QStep: Min, max
	mfe_reg->reg_mfe_s_mbr_qp_min = rcCtx->m_nMinQP;
	mfe_reg->reg_mfe_s_mbr_qp_max = rcCtx->m_nMaxQP;
	MFE_ASSERT(rcCtx->m_nMinQStep<(2<<7));
	mfe_reg->reg_mfe_s_mbr_qstep_min = rcCtx->m_nMinQStep;
	mfe_reg->reg_mfe_s_mbr_qstep_max = rcCtx->m_nMaxQStep;

	mfe_reg->reg_mfe_g_rec_en = pM4veInfo->g_rec_en;

#ifdef HW_ECO_STARTCODE_PREVENTION
    mfe_reg->reg_eco_bsp_rdy_fix = 1;
#if defined(_MFE_EDISON_)||defined(_MFE_EAGLE_)
    //agate U02 cannot set this.
    mfe_reg->reg_eco_bsp_multi_slice_fix = 1;
#endif
#endif
	//////////////////////////////////////////////////////////////////////////
	// swcfg1 output
	nTarWriteCount = 0;
	nRegWriteCount = 0;
	nTarFDCCount = 0;
	nRegFDCCount = 0;
	if (nFrmNum==0)
		WriteQTable(mfe_reg, pM4veInfo->rgiIntraQuantizerMatrix, pM4veInfo->rgiInterQuantizerMatrix);

	nRegWriteCount = 0;
#ifdef _MFE_M1_
	//FDC
	mfe_reg->reg_mfe_s_bsp_fdc_skip = 0;
    mfe_reg->reg_mfe_s_bsp_fdc_offset = 0;
	nTarFDCCount = PutFDC(mfe_reg, pBitsInfo, 1);
#endif

	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {
		nTarWriteCount = (pBufInfo->m_bEnableMvStore?62:59) + (nTarFDCCount*3);
	}
	else if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_H263) {
		nTarWriteCount = 53 + (nTarFDCCount*3);
	}
	nTarWriteCount++;	// reg to set fdc round

#ifdef CLOCK_GATING
    nTarWriteCount++;
#endif

	nTarFDCCount *= 3;
	nTarFDCCount++;	// reg to set fdc round

	// SW reset
	mfe_reg->reg_mfe_g_soft_rstz = 0;
	WriteRegMFE(0x0, mfe_reg->reg00, "[%d] reg00", nRegWriteCount++, "SW reset 0");
	mfe_reg->reg_mfe_g_soft_rstz = 1;
	WriteRegMFE(0x0, mfe_reg->reg00, "[%d] reg00", nRegWriteCount++, "SW reset 1");
	WriteRegMFE(0x1, mfe_reg->reg01, "[%d] reg01", nRegWriteCount++, "picture width");
	WriteRegMFE(0x2, mfe_reg->reg02, "[%d] reg02", nRegWriteCount++, "picture height");
	WriteRegMFE(0x3, mfe_reg->reg03, "[%d] reg03", nRegWriteCount++, "value");
	WriteRegMFE(0x4, mfe_reg->reg04, "[%d] reg04", nRegWriteCount++, "er_bs mode threshold");
	WriteRegMFE(0x5, mfe_reg->reg05, "[%d] reg05", nRegWriteCount++, "inter prediction preference");

	WriteRegMFE(0x20, mfe_reg->reg20, "[%d] reg20", nRegWriteCount++, "ME partition setting");
	WriteRegMFE(0x21, mfe_reg->reg21, "[%d] reg21", nRegWriteCount++, "value");
	WriteRegMFE(0x22, mfe_reg->reg22, "[%d] reg22", nRegWriteCount++, "me search range max depth");
	WriteRegMFE(0x23, mfe_reg->reg23, "[%d] reg23", nRegWriteCount++, "me mvx");
	WriteRegMFE(0x24, mfe_reg->reg24, "[%d] reg24", nRegWriteCount++, "me mvy");
	WriteRegMFE(0x25, mfe_reg->reg25, "[%d] reg25", nRegWriteCount++, "FME");

#ifdef CLOCK_GATING
    WriteRegMFE(0x16, mfe_reg->reg16, "[%d] reg16", nRegWriteCount++, "Clock gating");
#endif

	// Input buffers
	//
	WriteRegMFE(0x06, mfe_reg->reg06, "[%d] reg06", nRegWriteCount++, "current luma base address");
	WriteRegMFE(0x07, mfe_reg->reg07, "[%d] reg07", nRegWriteCount++, "current luma base address high");
	WriteRegMFE(0x08, mfe_reg->reg08, "[%d] reg08", nRegWriteCount++, "current chroma base address");
	WriteRegMFE(0x09, mfe_reg->reg09, "[%d] reg09", nRegWriteCount++, "current chroma base address high");
	WriteRegMFE(0x0a, mfe_reg->reg0a, "[%d] reg0a", nRegWriteCount++, "reference luma base address0");
	WriteRegMFE(0x0b, mfe_reg->reg0b, "[%d] reg0b", nRegWriteCount++, "reference luma base address0 high");
	WriteRegMFE(0x0c, mfe_reg->reg0c, "[%d] reg0c", nRegWriteCount++, "reference luma base address1");
	WriteRegMFE(0x0d, mfe_reg->reg0d, "[%d] reg0d", nRegWriteCount++, "reference luma base address1 high");
	WriteRegMFE(0x0e, mfe_reg->reg0e, "[%d] reg0e", nRegWriteCount++, "reference chroma base address0");
	WriteRegMFE(0x0f, mfe_reg->reg0f, "[%d] reg0f", nRegWriteCount++, "reference chroma base address0 high");
	WriteRegMFE(0x10, mfe_reg->reg10, "[%d] reg10", nRegWriteCount++, "reference chroma base address1");
	WriteRegMFE(0x11, mfe_reg->reg11, "[%d] reg11", nRegWriteCount++, "reference chroma base address1 high");
	WriteRegMFE(0x12, mfe_reg->reg12, "[%d] reg12", nRegWriteCount++, "reconstructed luma base address:");
	WriteRegMFE(0x13, mfe_reg->reg13, "[%d] reg13", nRegWriteCount++, "reconstructed luma base address high");
	WriteRegMFE(0x14, mfe_reg->reg14, "[%d] reg14", nRegWriteCount++, "reconstructed chroma base address:");
	WriteRegMFE(0x15, mfe_reg->reg15, "[%d] reg15", nRegWriteCount++, "reconstructed chroma base address: high");

#if defined(_MFE_T8_)&&!defined(_MFE_AGATE_)
	// Output buffer
    WriteRegMFE(0x3c, mfe_reg->reg3c, "[%d] reg3c", nRegWriteCount++, "bsp obuf start address: ");
    WriteRegMFE(0x3d, mfe_reg->reg3d, "[%d] reg3d", nRegWriteCount++, "bsp obuf start address high");
    WriteRegMFE(0x3e, mfe_reg->reg3e, "[%d] reg3e", nRegWriteCount++, "bsp obuf end address: ");
    WriteRegMFE(0x3f, mfe_reg->reg3f, "[%d] reg3f", nRegWriteCount++, "bsp obuf end address high");
    mfe_reg->reg_mfe_s_bspobuf_set_adr = 1;
#endif
	//
    mfe_reg->reg_mfe_s_bspobuf_fifo_th = 1;
    mfe_reg->reg_mfe_s_mvobuf_set_adr = 0;
    mfe_reg->reg_mfe_s_mvobuf_fifo_th = 0;

#if defined(_MFE_T8_)&&!defined(_MFE_AGATE_)
    WriteRegMFE(0x3b, mfe_reg->reg3b, "[%d] reg3b", nRegWriteCount++, "set bsp obuf");
       mfe_reg->reg_mfe_s_bspobuf_set_adr = 0;    // HW is write-one-clear
#elif defined(_MFE_M1_)||defined(_MFE_AGATE_)
    // Enable set-obuf
    mfe_reg->reg_mfe_s_bspobuf_update_adr = 1;
    WriteRegMFE(0x3f, mfe_reg->reg3f, "[%d] reg3f", nRegWriteCount++, "reg_mfe_s_bspobuf_update_adr");
    mfe_reg->reg_mfe_s_bspobuf_update_adr = 0;  // write-one-clear
#if defined(WIN32)
    mfe_reg->enable_obufadr_update = 0;
    UDMA_RIURead16(REG_BANK_MFE+0x6a, (unsigned short*)&mfe_reg->reg6a);
    while (mfe_reg->enable_obufadr_update!=1) {
        UDMA_RIURead16(REG_BANK_MFE+0x6a, (unsigned short*)&mfe_reg->reg6a);
        printf("Wait for enable_obufadr_update=1!\n");
    }
#endif
#if defined(USE_HW_DBL_OBUF)
    nRegWriteCount += SetObufAddr(mfe_reg, (MFE_U32)pBufInfo->m_nOutBufAddr, pBufInfo->m_OutBufferSize, 0, 0);
    nRegWriteCount += SetObufAddr(mfe_reg, (MFE_U32)pBufInfo->m_nOutBufAddr+pBufInfo->m_OutBufferSize, pBufInfo->m_OutBufferSize, 1, 1);
#else
    nRegWriteCount += SetObufAddr(mfe_reg, (MFE_U32)pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress, pBufInfo->m_OutBufferSize, 0, 1);
#endif
#endif

	// MV-store
	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {
		WriteRegMFE(0x19, mfe_reg->reg19, "[%d] reg19", nRegWriteCount++, "enable mv-store");
		if (mfe_reg->reg_mfe_g_mp4_direct_mvstore) {
			WriteRegMFE(0x40, mfe_reg->reg40, "[%d] reg40", nRegWriteCount++, "reg_mfe_s_mvobuf_sadr_low");
			WriteRegMFE(0x41, mfe_reg->reg41, "[%d] reg41", nRegWriteCount++, "reg_mfe_s_mvobuf_sadr_high");
			//
			mfe_reg->reg_mfe_s_bspobuf_set_adr = 0;
			mfe_reg->reg_mfe_s_bspobuf_fifo_th = 0;
			mfe_reg->reg_mfe_s_mvobuf_set_adr = 1;
			mfe_reg->reg_mfe_s_mvobuf_fifo_th = 1;
			WriteRegMFE(0x3b, mfe_reg->reg3b, "[%d] reg3b", nRegWriteCount++, "set mv-sotre addr");
			mfe_reg->reg_mfe_s_mvobuf_set_adr = 0;	// HW is write-one-clear
		}
		// mvibuf
		WriteRegMFE(0x4e, mfe_reg->reg4e, "[%d] reg4e", nRegWriteCount++, "reg_mfe_s_gn_mvibuf_sadr_low");
		WriteRegMFE(0x4f, mfe_reg->reg4f, "[%d] reg4f", nRegWriteCount++, "reg_mfe_s_gn_mvibuf_sadr_high");
	}


	// GN
	WriteRegMFE(0x4c, mfe_reg->reg4c, "[%d] reg1c", nRegWriteCount++, "reg_mfe_s_gn_sadr_low");
	WriteRegMFE(0x4d, mfe_reg->reg4d, "[%d] reg4d", nRegWriteCount++, "reg_mfe_s_gn_sadr_high");

	WriteRegMFE(0x19, mfe_reg->reg19, "[%d] reg19", nRegWriteCount++, "value");
	// MBR
	WriteRegMFE(0x26, mfe_reg->reg26, "[%d] reg26", nRegWriteCount++, "MBR: mbbits");
	WriteRegMFE(0x27, mfe_reg->reg27, "[%d] reg27", nRegWriteCount++, "MBR: frame qstep");
	WriteRegMFE(0x29, mfe_reg->reg29, "[%d] reg29", nRegWriteCount++, "264 qp-offset");
	WriteRegMFE(0x2a, mfe_reg->reg2a, "[%d] reg2a", nRegWriteCount++, "QP min/max");
	WriteRegMFE(0x6e, mfe_reg->reg6e, "[%d] reg6e", nRegWriteCount++, "QStep min");
	WriteRegMFE(0x6f, mfe_reg->reg6f, "[%d] reg6f", nRegWriteCount++, "QStep max");

	// MDC
	WriteRegMFE(0x39, mfe_reg->reg39, "[%d] reg39", nRegWriteCount++, "value");

	// Intra Update
	WriteRegMFE(0x2f, mfe_reg->reg2f, "[%d] reg2f", nRegWriteCount++, "value");
	WriteRegMFE(0x30, mfe_reg->reg30, "[%d] reg30", nRegWriteCount++, "value");
	WriteRegMFE(0x31, mfe_reg->reg31, "[%d] reg31", nRegWriteCount++, "value");
	WriteRegMFE(0x32, mfe_reg->reg32, "[%d] reg32", nRegWriteCount++, "value");

	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {	// MPEG-4
		WriteRegMFE(0x37, mfe_reg->reg37, "[%d] reg37", nRegWriteCount++, "MPEG4 MDC");
		WriteRegMFE(0x38, mfe_reg->reg38, "[%d] reg38", nRegWriteCount++, "MPEG4: vop_time_increment");
		// B-direct
		WriteRegMFE(0x1a, mfe_reg->reg1a, "[%d] reg1a", nRegWriteCount++, "MPEG4 BDirect");
	}


    mfe_reg->reg_mfe_g_crc_mode = 0xC;
    mfe_reg->reg_mfe_g_debug_tcycle_chk_en = 0x1;
    mfe_reg->reg_mfe_g_debug_tcycle_chk_sel = 0x0;
	mfe_reg->reg_mfe_g_debug_en = 0; // TEST
    WriteRegMFE(0x73, mfe_reg->reg73, "[%d] reg73", nRegWriteCount++, "crc mode");



   	WriteRegMFE(0x2c, mfe_reg->reg2c, "[%d] reg2c", nRegWriteCount++, "Last zigzag");

	// Cross-format wrong reg setting prevention
	WriteRegMFE(0x18, mfe_reg->reg18, "[%d] reg18", nRegWriteCount++, "JPE encode mode");
	WriteRegMFE(0x1b, mfe_reg->reg1b, "[%d] reg1b", nRegWriteCount++, "MPEG4 FieldDCT");

#ifdef _MFE_M1_
    // Prefetch & Low bandwidth mode
    WriteRegMFE(0x68,mfe_reg->reg68,"[%d] reg68", nRegWriteCount++, "Prefetch enable/disable");
    // Prefetch
    WriteRegMFE(0x6d,mfe_reg->reg6d,"[%d] reg6d", nRegWriteCount++, "Prefetch MB idle count");
	//Low bandwidth
	WriteRegMFE(0x6b, mfe_reg->reg6b, "[%d] reg6b", nRegWriteCount++, "Low Bandwidth: IMI addr low");
	WriteRegMFE(0x6c, mfe_reg->reg6c, "[%d] reg6c", nRegWriteCount++, "Low Bandwidth: IMI addr high");

	// Reset any StopAndGo or StopAndDrop setting.
	mfe_reg->reg_mfe_s_txip_sng_mb = 0;
	WriteRegMFE(0x2d, mfe_reg->reg2d, "[%d] reg2d", nRegWriteCount++, "reg_mfe_s_txip_sng_mb=0");

#endif

    //enable eco item
    WriteRegMFE(0x7d, mfe_reg->reg7d, "[%d] reg7d", nRegWriteCount++, "reg_mfe_s_txip_eco0=1");

	ms_dprintk(DRV_L5, "In mfe_reg_m4ve.c Before En HW..\n");
//	DumpAllReg(mfe_reg);

	// Enable HW
	mfe_reg->reg_mfe_g_frame_start_sw = 1;

	WriteRegMFE(0x00, mfe_reg->reg00, "[%d] reg00", nRegWriteCount++, "frame start");
    mfe_reg->reg_mfe_g_frame_start_sw = 0;	// HW is write-one-clear

	ms_dprintk(DRV_L4,"After RESET\n");
	// FDC
	nRegFDCCount = PutFDC(mfe_reg, pBitsInfo, 0);
	nRegWriteCount += nRegFDCCount;

    if(nRegFDCCount != nTarFDCCount) {
        ms_dprintk(DRV_L4,"nRegFDCCount = %d,nTarFDCCount = %d \n",nRegFDCCount,nTarFDCCount);
    }
    if(nRegWriteCount != nTarWriteCount) {
        ms_dprintk(DRV_L4,"nRegWriteCount = %d,nTarWriteCount = %d \n",nRegWriteCount,nTarWriteCount);
    }
	// Only for debug
//	MFE_ASSERT(nRegFDCCount==nTarFDCCount);
//	MFE_ASSERT(nRegWriteCount==nTarWriteCount);

}

int codeM4vConfigHeaders(MFE_CONFIG *pConfig,MFE_BOOL IsSkipHeader)
{
	OutStream* pStream = &pConfig->m_OutStream;
//	BitsInfo* pBitsInfo = &pConfig->ctxBitsInfo;

    if(IsSkipHeader) {
	osReset(pStream);
	codeNonCodedVOPShortHead(pConfig, pStream);
	osFlushAll(&pConfig->m_OutStream);
    } else {
	osReset(pStream);
	codeSequenceHead(pConfig, pStream);
	codeVOHead(pConfig, pStream);
	codeVOLHead(pConfig, pStream);
	// Finalize
	osFlushAll(&pConfig->m_OutStream);
    }
	// The generated bytes start from pStream->m_pbFrameBuffer and with pStream->m_nByteCount bytes.
	return pStream->m_nByteCount;
}



