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
//  [for Doxygen]
/// @file drvSIF.h
/// @brief Subroutine for SIF
/// @author MStarSemi Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVSIF_H_
#define _DRVSIF_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsTypes.h"
#include "drvAUDIO_if.h"

#ifdef __cplusplus
extern "C"
{
#endif
////////////////////////////////////////////////////////////////////////////////
// Customer Define & data type [LM14]
////////////////////////////////////////////////////////////////////////////////
#define Thr_C2ONOFF_Gap             3

#define _BIT0                       0x0001
#define _BIT1                       0x0002
#define _BIT2                       0x0004
#define _BIT3                       0x0008
#define _BIT4                       0x0010
#define _BIT5                       0x0020
#define _BIT6                       0x0040
#define _BIT7                       0x0080
#define _BIT8                       0x0100
#define _BIT9                       0x0200
#define _BIT10                      0x0400
#define _BIT11                      0x0800
#define _BIT12                      0x1000
#define _BIT13                      0x2000
#define _BIT14                      0x4000
#define _BIT15                      0x8000

#define _bit0_(val)                 ((val & _BIT0))
#define _bit1_(val)                 ((val & _BIT1))
#define _bit2_(val)                 ((val & _BIT2))
#define _bit3_(val)                 ((val & _BIT3))
#define _bit4_(val)                 ((val & _BIT4))
#define _bit5_(val)                 ((val & _BIT5))
#define _bit6_(val)                 ((val & _BIT6))
#define _bit7_(val)                 ((val & _BIT7))
#define _bit8_(val)                 ((val & _BIT8))
#define _bit9_(val)                 ((val & _BIT9))
#define _bit10_(val)                ((val & _BIT10))
#define _bit11_(val)                ((val & _BIT11))
#define _bit12_(val)                ((val & _BIT12))
#define _bit13_(val)                ((val & _BIT13))
#define _bit14_(val)                ((val & _BIT14))
#define _bit15_(val)                ((val & _BIT15))

//================================================================
//  Customer Enum [LM14]
//================================================================
typedef enum
{
	ADEC_SIF_ABSENT	 = 0,
	ADEC_SIF_PRESENT 		 = 1,
	ADEC_SIF_DETECTING_EXSISTANCE
}ADEC_SIF_EXISTENCE_INFO_T;

typedef enum
{
	ADEC_SIF_SYSTEM_BG	= 0x00,	// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_I,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_DK,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_L,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_MN,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_UNKNOWN = 0xf0
} ADEC_SIF_SOUNDSYSTEM_T;

typedef enum ADEC_SIF_STANDARD{
    ADEC_SIF_MODE_DETECT = 0,
    ADEC_SIF_BG_NICAM,
    ADEC_SIF_BG_FM,
    ADEC_SIF_BG_A2,
    ADEC_SIF_I_NICAM,
    ADEC_SIF_I_FM,
    ADEC_SIF_DK_NICAM,
    ADEC_SIF_DK_FM,
    ADEC_SIF_DK1_A2,
    ADEC_SIF_DK2_A2,    // Richard.Ni add
    ADEC_SIF_DK3_A2,
    ADEC_SIF_L_NICAM,
    ADEC_SIF_L_AM,
    ADEC_SIF_MN_A2,
    ADEC_SIF_MN_BTSC,
    ADEC_SIF_MN_EIAJ,
    ADEC_SIF_NUM_SOUND_STD
} ADEC_SIF_STANDARD_T;

typedef enum ADEC_SIF_AVAILE_STANDARD {
    ADEC_SIF_NICAM = 0,
    ADEC_SIF_A2,
    ADEC_SIF_FM,
    ADEC_SIF_DETECTING_AVAILABILITY
} ADEC_SIF_AVAILE_STANDARD_T ;

typedef enum
{
	ATV_GET_PAL_MONO				=	0x00,	// PAL Mono
	ATV_GET_PAL_STEREO				=	0x01,	// PAL Stereo
	ATV_GET_PAL_DUAL				=	0x02,	// PAL Dual
	ATV_GET_PAL_NICAM_MONO			=	0x03,	// PAL NICAM Mono
	ATV_GET_PAL_NICAM_STEREO		=	0x04,	// PAL NICAM Stereo
	ATV_GET_PAL_NICAM_DUAL			=	0x05,	// PAL NICAM Dual
	ATV_GET_PAL_UNKNOWN				=	0x06,	// PAL Unkown State
	ATV_GET_NTSC_A2_MONO			=	0x10,	// NTSC(A2) Mono
	ATV_GET_NTSC_A2_STEREO			=	0x11,	// NTSC(A2) Stereo
	ATV_GET_NTSC_A2_SAP				=	0x12,	// NTSC(A2) SAP
	ATV_GET_NTSC_A2_UNKNOWN			=	0x13,	// NTSC(A2) Unkown State
	ATV_GET_NTSC_BTSC_MONO			=	0x20,	// NTSC(BTSC) Mono
	ATV_GET_NTSC_BTSC_STEREO		=	0x21,	// NTSC(BTSC) Stereo
	ATV_GET_NTSC_BTSC_SAP_MONO		=	0x22,	// NTSC(BTSC) SAP Mono
	ATV_GET_NTSC_BTSC_SAP_STEREO	=	0x23,	// NTSC(BTSC) SAP Stereo
	ATV_GET_NTSC_BTSC_UNKNOWN		=	0x24,	// NTSC(BTSC) Unkown State
} ATV_AUDIO_MODE_GET_T;

typedef enum
{
    ATV_SET_PAL_MONO				=	0x00,	// PAL Mono
	ATV_SET_PAL_MONO_FORCED			=	0x01,	// PAL Mono Force Mono
	ATV_SET_PAL_STEREO				=	0x02,	// PAL Stereo
	ATV_SET_PAL_STEREO_FORCED		=	0x03,	// PAL Stereo Force Mono
	ATV_SET_PAL_DUALI				=	0x04,	// PAL Dual I
	ATV_SET_PAL_DUALII				=	0x05,	// PAL Dual II
	ATV_SET_PAL_DUALI_II			=	0x06,	// PAL Dual I+II
	ATV_SET_PAL_NICAM_MONO			=	0x07,	// PAL NICAM Mono
	ATV_SET_PAL_NICAM_MONO_FORCED	=	0x08,	// PAL NICAM Mono Force Mono
	ATV_SET_PAL_NICAM_STEREO		=	0x09,	// PAL NICAM Stereo
	ATV_SET_PAL_NICAM_STEREO_FORCED	=	0x0A,	// PAL NICAM Stereo Force Mono
	ATV_SET_PAL_NICAM_DUALI			=	0x0B,	// PAL NICAM Dual I
	ATV_SET_PAL_NICAM_DUALII		=	0x0C,	// PAL NICAM Dual II
	ATV_SET_PAL_NICAM_DUALI_II		=	0x0D,	// PAL NICAM Dual I+II
	ATV_SET_PAL_NICAM_DUAL_FORCED	=	0x0E,	// PAL NICAM Dual Forced Mono(Not Supported)
	ATV_SET_PAL_UNKNOWN				=	0x0F,	// PAL Unkown State
	ATV_SET_NTSC_A2_MONO			=	0x10,	// NTSC(A2) Mono
	ATV_SET_NTSC_A2_STEREO			=	0x11,	// NTSC(A2) Stereo
	ATV_SET_NTSC_A2_SAP				=	0x12,	// NTSC(A2) SAP
	ATV_SET_NTSC_A2_UNKNOWN			=	0x13,	// NTSC(A2) Unkown State
	ATV_SET_NTSC_BTSC_MONO			=	0x20,	// NTSC(BTSC) Mono
	ATV_SET_NTSC_BTSC_STEREO		=	0x21,	// NTSC(BTSC) Stereo
	ATV_SET_NTSC_BTSC_SAP_MONO		=	0x22,	// NTSC(BTSC) SAP Mono
	ATV_SET_NTSC_BTSC_SAP_STEREO	=	0x23,	// NTSC(BTSC) SAP Stereo
	ATV_SET_NTSC_BTSC_UNKNOWN		=	0x24,	// NTSC(BTSC) Unkown State
} ATV_AUDIO_MODE_SET_T;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    MDrv_SIF_Init(void);
MS_BOOL MDrv_SIF_ReLoadCode(MS_U8 u8Type);
MS_BOOL MDrv_SIF_SetSoundMode(AUDIO_SIF_SOUND_MODE mode_type);
MS_BOOL MDrv_SIF_SetSystem(MS_U8 u8SystemType);
MS_BOOL MDrv_SIF_StartAutoDetection (void);
MS_U32 MDrv_SIF_AccessThreshold(AUDIO_SIF_SYSTEM_TYPE rw_standard_type, AUDIO_SIF_THRESHOLD_TYPE  u8Threshold_type, MS_U16 u16Value);
void    MDrv_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl, MS_U8 start_i, MS_U8 num, AUDIO_SIF_SYSTEM_TYPE standard_type);
MS_BOOL MDrv_SIF_GetOrginalGain(void);
MS_BOOL MDrv_SIF_SetPrescale(AUDIO_SIF_GAIN_TYPE u8Type,int db_value);

//////////////////////////////////////////////////////
// add for the Chakara2  & for the ¦n¥Í¬¡ coding
MS_BOOL MDrv_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type);
MS_BOOL MDrv_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type);
void  MDrv_SIF_SetStandardType(AUDIO_SIF_SET_STANDARD sif_type);
MS_U8 MDrv_SIF_GetDspType(void);
MS_U8 MDrv_SIF_GetDspCodeType(void);

MS_U8 MDrv_SIF_SendCmd(AU_SIF_CMD u8DecCmd, MS_U8 comm_arg1, MS_U8 comm_arg2);

//////////////////////////////////////////////////////
//  Customer Function [LM14]
//////////////////////////////////////////////////////
MS_BOOL MDrv_MAD_SIF_GetBtscA2StereoLevel(MS_U16 *);
void MDrv_MAD_SIF_GetCurAnalogMode(MS_U8*);
MS_U16 MDrv_MAD_SIF_SetUserAnalogMode(MS_U16);
MS_BOOL MDrv_MAD_SIF_SetModeSetup(MS_U8);
MS_BOOL MDrv_MAD_SIF_SetHDEVMode(MS_U8);
MS_BOOL MDrv_MAD_SIF_GetBandDetect(MS_U8, MS_U32 *);
MS_BOOL   MDrv_MAD_SIF_GetBandDetect_Amp(MS_U8, MS_U32 *);
MS_BOOL MDrv_MAD_SIF_SetBandSetup(MS_U8);
MS_BOOL MDrv_MAD_SIF_CheckAvailableSystem(MS_U8, MS_U8 *);
MS_BOOL MDrv_MAD_SIF_CheckA2DK(MS_U8 , MS_U8 *);
void MDrv_MAD_CheckNicamDigital(MS_U8 *);
MS_BOOL MDrv_MAD_SIF_GetSoundStandard(MS_U8 *);
MS_BOOL MDrv_MAD_SIF_RedoSetStandard(void);
MS_BOOL MDrv_MAD_SetBtscA2ThresholdLevel(MS_U16, MS_U16 *);
MS_BOOL MDrv_MAD_SIF_AccessThreshold(AUDIO_SIF_SYSTEM_TYPE, AUDIO_SIF_THRESHOLD_TYPE, MS_U16 *);
void MDrv_MAD_SIF_ADC_Reset(void);


#ifdef __cplusplus
}
#endif

#endif
