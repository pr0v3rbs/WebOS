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
#define _MSACE_C_

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "apiXC_Ace.h"
#include "mhal_ace.h"
#include "hwreg_ace.h"
#include "ace_hwreg_utility2.h"
#include "drvACE.h"
#include "MsAce_LIB.h"

//--------------------------------------------------------------------------------------------------
// msACE version history:
// v100(2008-04-16): Add version infromation
//       Change internal contrast/RGB gain variable to WORD(12bits)
//       Add new function for adjust contrast/RGB gain
//          void msAdjustVideoContrast_10Bits( BOOL bScalerWin, WORD wContrast );
//          void msAdjustVideoRGB_10Bits( BOOL bScalerWin, WORD ucRCon, WORD ucGCon, WORD ucBCon);
//          void msAdjustPCContrast_10Bits( BOOL bScalerWin, WORD wContrast );
//          void msAdjustPCRGB_10Bits( BOOL bScalerWin, WORD wRed, WORD wGreen, WORD wBlue );
// v101(2008-04-16): Modify msACE_GetVersion()
// v102(2008-09-04):
//  1.Fine tune color correction control.
//  2.Fine tune sRGB control
// v103(2008-09-08):
//  1.Add new function for get more infromation from ACE library
//  2.Seperate chip relative code to another file.
// v104(2008-09-17):
//  1.Use WriteByteMask to access REG_ADDR_CM_CTL_MAIN
//  2.Fix msACE_GetInfo() uncall warning.
// v105(2009-07-14):
//  1.Fine tune the define in LIB_Group.
//  2.Add new group for external chip.
// v106(2009-10-28):
//  1.Add C51 version info.
//  2.Add debug-info-YUVtoRGB Matrix selection
//  3.Add debug function to get Matrix
//  4.Add option for using internal sin/cos function.
//  5.Change some function interface to static.
// v107(2010-01-26):
//  1.Refine Sine&Cosine function: Using lookup table.
// v108(2010-04-13):
//  1.Enable Sine&Cosine function using lookup table.
//  2.Add 2 function: msACE_SetColorMatrixControl(), msACE_SetRBChannelRange()
#define MSACE_VERSION   (108)
//----------------------------------------------------------------------------------------------------

#if defined(__C51__)
#define STR_C51_VERSION(x) "C51:"##  #x

#define C51_VERSION(x) STR_C51_VERSION(x)

char* ace_c51version = C51_VERSION(__C51__);
#endif

#define MAIN_WINDOW     0
#define SUB_WINDOW      1

typedef short Typ3x3Array[3][3];

#if 0
typedef struct
{
    WORD wContrast;
    WORD wRCon;
    WORD wGCon;
    WORD wBCon;
    BYTE ucSaturation;
    BYTE ucHue;
    short sContrastRGBMatrix[3][3];
    short sVideoSatHueMatrix[3][3];
    short* psColorCorrectionMatrix; // this variable must pointer to a array[3][3]
    short* psPCsRGBMatrix; // sRGB matrix for PC, this variable must pointer to a array[3][3]
    short* psYVUtoRGBMatrix; // this variable must pointer to a array[3][3]
    BYTE bForceYUVtoRGB : 1;
    BYTE ucYUVtoRGBMatrixSel : 2;
}StruAceInfo;
#endif

#include "ACE_private.h"
#include "utopia_dapi.h"
#include "utopia.h"

extern void* pu32ACEInst;
extern void* pu32ACERes[E_ACE_POOL_ID_MAX];
extern MS_BOOL bIsIREMode_Open;
BOOL bColorMatrixEnable = FALSE;


#if 0
static StruAceInfo s_AceInfo[XC_ACE_EX_MAX_DEVICE_NUM][2];
static BOOL bBypassColorMatrix[XC_ACE_EX_MAX_DEVICE_NUM];
#else
#define s_AceInfo psACEResPri->stdrvACE.s_AceInfo
#define bBypassColorMatrix psACEResPri->stdrvACE.bBypassColorMatrix

#endif

///////////////////////////////////////////////////////////////////////////////////////////////

void msAdjustPCContrast_10Bits(void* pInstance, BOOL bScalerWin, WORD wContrast );
void msAdjustPCRGB_10Bits(void* pInstance, BOOL bScalerWin, WORD wRed, WORD wGreen, WORD wBlue );
void msAdjustVideoContrast_10Bits(void* pInstance, BOOL bScalerWin, WORD wContrast );
void msAdjustVideoRGB_10Bits(void* pInstance, BOOL bScalerWin, WORD wRCon, WORD wGCon, WORD wBCon);

///////////////////////////////////////////////////////////////////////////////////////////////
WORD msACE_GetVersion(void)
{
    return MSACE_VERSION;
}

static WORD _TransferColorCorrectionOffsetValue(short s)
{
    WORD w;

    w = s;
    if( w > 0x7F )
    {
        w = s*-1;
        w |= 0x80;
    }
    return w;
}
WORD msACE_GetInfo(void* pInstance, BOOL bWindow, EnuACEInfoType ucInfoType )
{
    WORD wReturnValue = 0xFFFF;
    _ACE_GET_VARIABLE();

    switch( ucInfoType )
    {
        case ACE_INFO_TYPE_VERSION:
            wReturnValue = MSACE_VERSION;
            break;
        case ACE_INFO_TYPE_CONTRAST:
            wReturnValue = s_AceInfo[bWindow].wContrast;
            break;
        case ACE_INFO_TYPE_R_GAIN:
            wReturnValue = s_AceInfo[bWindow].wRCon;
            break;
        case ACE_INFO_TYPE_G_GAIN:
            wReturnValue = s_AceInfo[bWindow].wGCon;
            break;
        case ACE_INFO_TYPE_B_GAIN:
            wReturnValue = s_AceInfo[bWindow].wBCon;
            break;
        case ACE_INFO_TYPE_SATURATION:
            wReturnValue = s_AceInfo[bWindow].ucSaturation;
            break;
        case ACE_INFO_TYPE_HUE:
            wReturnValue = s_AceInfo[bWindow].ucHue;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_XY_R:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = s_AceInfo[bWindow].psColorCorrectionMatrix[26]*-1;
                wReturnValue = ((wReturnValue&0xFF00)>>4)|(wReturnValue&0xFF);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_XY_G:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = s_AceInfo[bWindow].psColorCorrectionMatrix[27];
                wReturnValue = ((wReturnValue&0xFF00)>>4)|(wReturnValue&0xFF);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_XY_B:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = s_AceInfo[bWindow].psColorCorrectionMatrix[28]*-1;
                wReturnValue = ((wReturnValue&0xFF00)>>4)|(wReturnValue&0xFF);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_R:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = _TransferColorCorrectionOffsetValue(s_AceInfo[bWindow].psColorCorrectionMatrix[29]);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_G:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = _TransferColorCorrectionOffsetValue(s_AceInfo[bWindow].psColorCorrectionMatrix[30]);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_B:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = _TransferColorCorrectionOffsetValue(s_AceInfo[bWindow].psColorCorrectionMatrix[31]);
            }
            else
                wReturnValue = 0;
            break;

        case ACE_INFO_TYPE_COMPILER_VERSION:
    #if defined(__C51__)
            wReturnValue = __C51__;
    #else
            wReturnValue = 0;
    #endif
            break;

        case ACE_INFO_TYPE_YUV_TO_RGB_MATRIX_SEL:
            wReturnValue = s_AceInfo[bWindow].ucYUVtoRGBMatrixSel;
            break;

        case ACE_INFO_TYPE_DUMMY:
        default:
            wReturnValue = 0;
            break;
    }

    return wReturnValue;
}

// matrix used to convert RGB color space to YUV color space for SDTV
//code short tSDTVRgb2yuv[3][3] =
//{
//    {  0x0107,  0x0204,  0x0064 }, // 0.257,  0.504,  0.098
//    { -0x0098, -0x012A,  0x01C2 }, // -0.148, -0.291, 0.439
//    {  0x01C2, -0x0179, -0x0049 }  // 0.439,  -0.368, -0.071
//};

// matrix used to convert YUV color space to RGB color space, used for video input for SDTV
static code short tSDTVYuv2rgb[3][3] =
#if 0
{
    {   0x0662, 0x04A8,  0x0000 }, // 1.596,  1.164, 0
    {  -0x0341, 0x04A8, -0x0190 }, // -0.813, 1.164, -0.391
    {   0x0000, 0x04A8,  0x0812 }  // 0,      1.164, 2.018
};
#else
{
    {   0x0667, 0x04AC,  0x0000 }, // 1.6005,  1.1679, 0
    {  -0x0343, 0x04AC, -0x0192 }, // -0.8154, 1.1679, -0.3925
    {   0x0000, 0x04AC,  0x0818 }  // 0,      1.1679, 2.0234
};
#endif

// matrix used to convert RGB color space to YUV color space for HDTV
//code short tHDTVRgb2yuv[3][3] =
//{
//    {  0x00DA,  0x02DC,  0x004A }, // 0.213,  0.715,  0.072
//    { -0x0078, -0x0193,  0x020B }, // -0.117, -0.394, 0.511
//    {  0x020B, -0x01DB, -0x0030 }  // 0.511,  -0.464, -0.047
//};

// matrix used to convert YUV color space to RGB color space, used for video input for HDTV
static code short tHDTVYuv2rgb[3][3] =
#if 0
{
    {   0x072C, 0x04A8,  0x0000 }, // 1.793,  1.164, 0
    {  -0x0223, 0x04A8, -0x00DA }, // -0.534, 1.164, -0.213
    {   0x0000, 0x04A8,  0x0876 }  // 0,      1.164, 2.115
};
#else
{
    {   0x0731, 0x04AC,  0x0000 }, // 1.7978,  1.1679, 0
    {  -0x0223, 0x04AC, -0x00DB }, // -0.5341, 1.1679, -0.2138
    {   0x0000, 0x04AC,  0x0879 }  // 0,      1.1679, 2.1181
};
#endif
static void CopyMatrix(WORD* pwBufSrc, WORD* pwBufTgt, BYTE ucLen )
{
    BYTE i;
    for( i = 0; i < ucLen; ++ i )
    {
        pwBufTgt[i] = pwBufSrc[i];
    }
}
BOOL msACE_GetMatrix(void* pInstance, BOOL bWindow, BYTE ucMatrixId, WORD* pwBuf, BYTE ucBufLen)
{
    BYTE ucCopyLen = 9;

    _ACE_GET_VARIABLE();

    // Check parameter ...

    //if( ucBufLen < ucCopyLen )
        ucCopyLen = ucBufLen;

    switch(ucMatrixId)
    {
        case ACE_MATRIX_ID__YUV_TO_RGB:
            if( s_AceInfo[bWindow].ucYUVtoRGBMatrixSel == ACE_YUV_TO_RGB_MATRIX_SDTV )
            {
                CopyMatrix( (WORD*)tSDTVYuv2rgb, pwBuf, ucCopyLen );
            }
            else if( s_AceInfo[bWindow].ucYUVtoRGBMatrixSel == ACE_YUV_TO_RGB_MATRIX_HDTV )
            {
                CopyMatrix( (WORD*)tHDTVYuv2rgb, pwBuf, ucCopyLen );
            }
            else if( s_AceInfo[bWindow].ucYUVtoRGBMatrixSel == ACE_YUV_TO_RGB_MATRIX_USER )
            {
                if( s_AceInfo[bWindow].psYVUtoRGBMatrix == NULL )
                    return FALSE;
                CopyMatrix( (WORD*)s_AceInfo[bWindow].psYVUtoRGBMatrix, pwBuf, ucCopyLen );
            }
            break;
        case ACE_MATRIX_ID__COLOR_CORRECTION:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix == NULL)
                return FALSE;
            CopyMatrix( (WORD*)s_AceInfo[bWindow].psColorCorrectionMatrix, pwBuf, ucCopyLen );
            break;

        case ACE_MATRIX_ID__SRGB:
            if( s_AceInfo[bWindow].psPCsRGBMatrix == NULL)
                return FALSE;
            CopyMatrix( (WORD*)s_AceInfo[bWindow].psPCsRGBMatrix, pwBuf, ucCopyLen );
            break;
    }
    return TRUE;
}

void msACEPCForceYUVtoRGB(void* pInstance, BOOL bWin, BOOL bEnable )
{
    _ACE_GET_VARIABLE();
    if( bEnable )
        s_AceInfo[bWin].bForceYUVtoRGB = 1;
    else
        s_AceInfo[bWin].bForceYUVtoRGB = 0;
}
void msACESelectYUVtoRGBMatrix(void* pInstance, BOOL bWindow, BYTE ucMatrix, short* psUserYUVtoRGBMatrix )
{
    DECLARA_BANK_VARIABLE

    _ACE_GET_VARIABLE();

    s_AceInfo[bWindow].ucYUVtoRGBMatrixSel = ucMatrix;
    if( ucMatrix == ACE_YUV_TO_RGB_MATRIX_USER )
    {
		//printf("Color table User mode\n");
        s_AceInfo[bWindow].psYVUtoRGBMatrix = psUserYUVtoRGBMatrix;
    }
    else if( ucMatrix == ACE_YUV_TO_RGB_MATRIX_HDTV )
    {
		//printf("Color table HD mode\n");
	    BACKUP_SC_BANK
		SET_SC_BANK(BANK_ACE);
		if(bWindow==0) // 0: main, 1:sub
		{
			//printf("Prepare Load Main color table\n");
	    	if(ACE_READ_CTL(psACEInstPri->u32DeviceID, REG_ADDR_ACE_R2Y_EQ_SEL,0x04)) // (BK18_6F[2] for main)
    		{
				//printf("Load SD table\n");
    			s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tSDTVYuv2rgb;
    		}
			else
			{
				//printf("Load HD table\n");
				s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tHDTVYuv2rgb;
			}
		}
		else
		{
			//printf("Prepare Load Sub color table\n");
	    	if(ACE_READ_CTL(psACEInstPri->u32DeviceID, REG_ADDR_ACE_R2Y_EQ_SEL,0x40))//(BK18_6F[6] for sub)
    		{
				//printf("Load SD table\n");
    			s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tSDTVYuv2rgb;
    		}
			else
			{
				//printf("Load HD table\n");
				s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tHDTVYuv2rgb;
			}
		}
	    RECOVER_SC_BANK
    }
    else // ACE_YUV_TO_RGB_MATRIX_SDTV
    {
		//printf("Color table SD mode\n");
        s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tSDTVYuv2rgb;
    }
}
//---------------------------------------------------------------------------
// Specify the color correction table.
//---------------------------------------------------------------------------
void msACESetColorCorrectionTable(void* pInstance, BOOL bWindow, short* psColorCorrectionTable )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bWindow].psColorCorrectionMatrix = psColorCorrectionTable;
}

//---------------------------------------------------------------------------
// Specify the sRGB table.
//---------------------------------------------------------------------------
void msACESetPCsRGBTable(void* pInstance, BOOL bWin, short* psPCsRGBTable )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bWin].psPCsRGBMatrix = psPCsRGBTable;
}

void InitACEVar(void* pInstance)
{
    BYTE i;
    _ACE_GET_VARIABLE();

    for( i = 0; i < 2; ++ i )
    {
        s_AceInfo[i].wRCon = 1024;
        s_AceInfo[i].wGCon = 1024;
        s_AceInfo[i].wBCon = 1024;
        s_AceInfo[i].wContrast = 1024;
        s_AceInfo[i].ucSaturation = 0x80;
        s_AceInfo[i].ucHue = 50;

        s_AceInfo[i].sContrastRGBMatrix[0][0] =
        s_AceInfo[i].sContrastRGBMatrix[1][1] =
        s_AceInfo[i].sContrastRGBMatrix[2][2] = 1024;
        s_AceInfo[i].sContrastRGBMatrix[0][1] =
        s_AceInfo[i].sContrastRGBMatrix[1][0] =
        s_AceInfo[i].sContrastRGBMatrix[2][0] =
        s_AceInfo[i].sContrastRGBMatrix[0][2] =
        s_AceInfo[i].sContrastRGBMatrix[1][2] =
        s_AceInfo[i].sContrastRGBMatrix[2][1] = 0;

        s_AceInfo[i].sVideoSatHueMatrix[0][0] =
        s_AceInfo[i].sVideoSatHueMatrix[1][1] =
        s_AceInfo[i].sVideoSatHueMatrix[2][2] = 1024;
        s_AceInfo[i].sVideoSatHueMatrix[0][1] =
        s_AceInfo[i].sVideoSatHueMatrix[1][0] =
        s_AceInfo[i].sVideoSatHueMatrix[2][0] =
        s_AceInfo[i].sVideoSatHueMatrix[0][2] =
        s_AceInfo[i].sVideoSatHueMatrix[1][2] =
        s_AceInfo[i].sVideoSatHueMatrix[2][1] = 0;


        //s_AceInfo[i].psColorCorrectionMatrix = NULL;
        msACESetColorCorrectionTable(pInstance, i, NULL );

        //s_AceInfo[i].psPCsRGBMatrix = NULL;
        msACESetPCsRGBTable(pInstance, i, NULL );

        //s_AceInfo[i].bForceYUVtoRGB = 0;
        msACEPCForceYUVtoRGB(pInstance, i, FALSE );

        //s_AceInfo[i].ucYUVtoRGBMatrixSel = ACE_YUV_TO_RGB_MATRIX_SDTV;
        //s_AceInfo[i].psYVUtoRGBMatrix = (short*)tSDTVYuv2rgb;
        msACESelectYUVtoRGBMatrix(pInstance, i, ACE_YUV_TO_RGB_MATRIX_SDTV, NULL );
    }

#if defined(__C51__)
    // For uncall warning...
    if( i == 0 )
    {
        msACE_GetVersion();
        msACE_GetInfo(pInstance, 0, 0);
        msAdjustPCContrast_10Bits(pInstance, 0,0);
        msAdjustPCRGB_10Bits(pInstance, 0, 0, 0, 0 );
        msAdjustVideoContrast_10Bits(pInstance, 0,0);
        msAdjustVideoRGB_10Bits(pInstance, 0, 0, 0, 0 );
        msACE_GetMatrix(pInstance, 0, 0, NULL, 0);
        msACE_SetColorMatrixControl(pInstance, 0, 0 );
        msACE_SetRBChannelRange(pInstance, 0, 0 );
    }
#endif
}

#define ENABLE_SINE_COSINE_USING_LOOKUP_TABLE   1

#if( ENABLE_SINE_COSINE_USING_LOOKUP_TABLE ) // sine/cosine use lookup table

static short sine(BYTE u8Hue)
{
    code short tHueToSine[] =
    {
        784, // 0
        772, // 1
        760, // 2
        748, // 3
        736, // 4
        724, // 5
        711, // 6
        698, // 7
        685, // 8
        671, // 9
        658, // 10
        644, // 11
        630, // 12
        616, // 13
        601, // 14
        587, // 15
        572, // 16
        557, // 17
        542, // 18
        527, // 19
        511, // 20
        496, // 21
        480, // 22
        464, // 23
        448, // 24
        432, // 25
        416, // 26
        400, // 27
        383, // 28
        366, // 29
        350, // 30
        333, // 31
        316, // 32
        299, // 33
        282, // 34
        265, // 35
        247, // 36
        230, // 37
        212, // 38
        195, // 39
        177, // 40
        160, // 41
        142, // 42
        124, // 43
        107, // 44
        89, // 45
        71, // 46
        53, // 47
        35, // 48
        17, // 49
        0, // 50
        -17, // 51
        -35, // 52
        -53, // 53
        -71, // 54
        -89, // 55
        -107, // 56
        -124, // 57
        -142, // 58
        -160, // 59
        -177, // 60
        -195, // 61
        -212, // 62
        -230, // 63
        -247, // 64
        -265, // 65
        -282, // 66
        -299, // 67
        -316, // 68
        -333, // 69
        -350, // 70
        -366, // 71
        -383, // 72
        -400, // 73
        -416, // 74
        -432, // 75
        -448, // 76
        -464, // 77
        -480, // 78
        -496, // 79
        -512, // 80
        -527, // 81
        -542, // 82
        -557, // 83
        -572, // 84
        -587, // 85
        -601, // 86
        -616, // 87
        -630, // 88
        -644, // 89
        -658, // 90
        -671, // 91
        -685, // 92
        -698, // 93
        -711, // 94
        -724, // 95
        -736, // 96
        -748, // 97
        -760, // 98
        -772, // 99
        -784, // 100
    };
    if( u8Hue > 100 )
        u8Hue = 100;
    return tHueToSine[u8Hue];
}
// cos function
static short cosine(BYTE u8Hue)
{
    code short tHueToCosine[] =
    {
        658, // 0
        671, // 1
        685, // 2
        698, // 3
        711, // 4
        724, // 5
        736, // 6
        748, // 7
        760, // 8
        772, // 9
        784, // 10
        795, // 11
        806, // 12
        817, // 13
        828, // 14
        838, // 15
        848, // 16
        858, // 17
        868, // 18
        877, // 19
        886, // 20
        895, // 21
        904, // 22
        912, // 23
        920, // 24
        928, // 25
        935, // 26
        942, // 27
        949, // 28
        955, // 29
        962, // 30
        968, // 31
        973, // 32
        979, // 33
        984, // 34
        989, // 35
        993, // 36
        997, // 37
        1001, // 38
        1005, // 39
        1008, // 40
        1011, // 41
        1014, // 42
        1016, // 43
        1018, // 44
        1020, // 45
        1021, // 46
        1022, // 47
        1023, // 48
        1023, // 49
        1024, // 50
        1023, // 51
        1023, // 52
        1022, // 53
        1021, // 54
        1020, // 55
        1018, // 56
        1016, // 57
        1014, // 58
        1011, // 59
        1008, // 60
        1005, // 61
        1001, // 62
        997, // 63
        993, // 64
        989, // 65
        984, // 66
        979, // 67
        973, // 68
        968, // 69
        962, // 70
        955, // 71
        949, // 72
        942, // 73
        935, // 74
        928, // 75
        920, // 76
        912, // 77
        904, // 78
        895, // 79
        886, // 80
        877, // 81
        868, // 82
        858, // 83
        848, // 84
        838, // 85
        828, // 86
        817, // 87
        806, // 88
        795, // 89
        784, // 90
        772, // 91
        760, // 92
        748, // 93
        736, // 94
        724, // 95
        711, // 96
        698, // 97
        685, // 98
        671, // 99
        658, // 100
    };
    if( u8Hue > 100 )
        u8Hue = 100;
    return tHueToCosine[u8Hue];
}

#else

#define PI  (3.14159265358979323846)

#if( USE_ACE_INTERNAL_SIN_COS ) // Use user define
static double _ACE_sin( double f0)
{
    double  f2, f4;
    int     r0;

    f0 *= 6.3661977236758134e-01;
    if (f0 < 0)
    {
        if (f0 < -1073741824.0)
        {

            if (f0 < -1.0e+18)
                    f0 = 0;

            f0 /= 1073741824.0;
            r0 = (int)f0;

            f2 = (double) r0;
            f0 -= f2;
            f0 *= 1073741824.0;
        }

        f2 = f0;
        f2 -= 0.5;
    }
    else
    {
        if (f0 >= 1073741824.0)
        {
            if (f0 > 1.0e+18)
                f0 = 0;
            f0 /= 1073741824.0;

            r0 = f0;
            f2 = (double) r0;
            f0 -= f2;
            f0 *= 1073741824.0;
        }

        f2 = f0;
        f2 += 0.5;
    }

    r0 = (int) f2;
    f2 = (double) r0;
    f0 -= f2;
    if (r0 & 1)
    {
        f0 *= f0;
        f2 = 6.5659631149794723e-11;
        f2 *= f0;
        f2 += -6.3866030837918522e-09;
        f2 *= f0;
        f2 += 4.7108747788181715e-07;
        f2 *= f0;
        f2 += -2.5202042373060605e-05;
        f2 *= f0;
        f2 += 9.1926027483942659e-04;
        f2 *= f0;
        f2 += -2.0863480763352961e-02;
        f2 *= f0;
        f2 += 2.5366950790104802e-01;
        f2 *= f0;
        f2 += -1.2337005501361698e+00;
        f0 *= f2;
        f0 += 1.0;
    }
    else
    {
        f4 = f0;
        f0 *= f0;
        f2 = -6.6880351098114673e-10;
        f2 *= f0;
        f2 += 5.6921729219679268e-08;
        f2 *= f0;
        f2 += -3.5988432352120853e-06;
        f2 *= f0;
        f2 += 1.6044118478735982e-04;
        f2 *= f0;
        f2 += -4.6817541353186881e-03;
        f2 *= f0;
        f2 += 7.9692626246167046e-02;
        f2 *= f0;
        f2 += -6.4596409750624625e-01;
        f0 *= f2;
        f0 += 1.5707963267948966e+00;
        f0 *= f4;
    }

    if (r0 & 2)
      f0 = -f0;

    return(f0);
}

static double _ACE_cos(double f0)
{
    double f2,f4;
    int r0;

    f0 *= 6.3661977236758134e-01;
    if (f0 < 0)
      f0 = -f0;

    if (f0 > 1073741824.0)
    {
        if (f0 > 1.0e+18)
            f0 = 0;

        f0 /= 1073741824.0;
        r0 = f0;
        f0 = (double) r0;
        f0 -= f2;
        f0 *= 1073741824.0;
    }

    f2 = f0;
    f0 += 0.5;
    r0 = (int)f0;
    f0 = (double) r0;
    f0 -= f2;

    if (r0 & 1)
    {
        f4 = f0;
        f0 *= f0;
        f2 = -6.6880351098114673e-10;
        f2 *= f0;
        f2 += 5.6921729219679268e-08;
        f2 *= f0;
        f2 += -3.5988432352120853e-06;
        f2 *= f0;
        f2 += 1.6044118478735982e-04;
        f2 *= f0;
        f2 += -4.6817541353186881e-03;
        f2 *= f0;
        f2 += 7.9692626246167046e-02;
        f2 *= f0;
        f2 += -6.4596409750624625e-01;
        f0 *= f2;
        f0 += 1.5707963267948966e+00;
        f0 *= f4;
    }
    else
    {
        f0 *= f0;
        f2 = 6.5659631149794723e-11;
        f2 *= f0;
        f2 += -6.3866030837918522e-09;
        f2 *= f0;
        f2 += 4.7108747788181715e-07;
        f2 *= f0;
        f2 += -2.5202042373060605e-05;
        f2 *= f0;
        f2 += 9.1926027483942659e-04;
        f2 *= f0;
        f2 += -2.0863480763352961e-02;
        f2 *= f0;
        f2 += 2.5366950790104802e-01;
        f2 *= f0;
        f2 += -1.2337005501361698e+00;
        f0 *= f2;
        f0 += 1.0;
    }

    if (r0 & 2)
        f0 = -f0;

    return(f0);
}
#define sin     _ACE_sin
#define cos     _ACE_cos

#endif

//#define PI  (3.14159265)
// sin function
static short sine(WORD u16Hue)
{
    return (short)(sin((u16Hue * PI) / 180.) * 1024.);
}
// cos function
static short cosine(WORD u16Hue)
{
    return (short)(cos((u16Hue * PI) / 180.) * 1024.);
}
#endif

static void SetVideoSatHueMatrix(void *pInstance, BOOL bWindow )
{
    short sTmp;
    WORD u16Hue;
    _ACE_GET_VARIABLE();

#if( ENABLE_SINE_COSINE_USING_LOOKUP_TABLE )
    u16Hue = s_AceInfo[bWindow].ucHue;
#else
    u16Hue = ((s_AceInfo[bWindow].ucHue <= 50) ? (50 - s_AceInfo[bWindow].ucHue) : (360-(s_AceInfo[bWindow].ucHue-50)));
#endif

    sTmp = ((short)s_AceInfo[bWindow].ucSaturation * 8);

    s_AceInfo[bWindow].sVideoSatHueMatrix[2][2] = ((((long)cosine(u16Hue) * sTmp))>>10);
    s_AceInfo[bWindow].sVideoSatHueMatrix[0][0] = ((( (long)cosine(u16Hue)* sTmp))>>10);
    s_AceInfo[bWindow].sVideoSatHueMatrix[2][0] = ((((long)sine(u16Hue)   * sTmp))>>10);
    s_AceInfo[bWindow].sVideoSatHueMatrix[0][2] = (((-(long)sine(u16Hue)  * sTmp))>>10);

    s_AceInfo[bWindow].sVideoSatHueMatrix[1][1] = 1024;
    s_AceInfo[bWindow].sVideoSatHueMatrix[0][1] = s_AceInfo[bWindow].sVideoSatHueMatrix[1][0] = s_AceInfo[bWindow].sVideoSatHueMatrix[1][2] = s_AceInfo[bWindow].sVideoSatHueMatrix[2][1] = 0;
}

static void SetVideoContrastMatrix(void *pInstance, BOOL bWindow )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bWindow].sContrastRGBMatrix[0][0] = ( (DWORD)s_AceInfo[bWindow].wRCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[1][1] = ( (DWORD)s_AceInfo[bWindow].wGCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[2][2] = ( (DWORD)s_AceInfo[bWindow].wBCon * (s_AceInfo[bWindow].wContrast)) >> 10;
}

static void ArrayMultiply(short sFirst[3][3], short sSecond[3][3], short sResult[3][3])
{
    BYTE ucRow, ucCol;

    //go line by line
    for(ucRow=0; ucRow!=3; ucRow++)
    {
        // go column by column
        for(ucCol=0; ucCol!=3; ucCol++)
        {
            sResult[ucRow][ucCol] = (((long)sFirst[ucRow][0] * sSecond[0][ucCol]) +
                                     ((long)sFirst[ucRow][1] * sSecond[1][ucCol]) +
                                     ((long)sFirst[ucRow][2] * sSecond[2][ucCol])) >> 10;
        } // for
    } // for
}
static void msWriteBt2020ColoreMatrix(void *pInstance, BOOL bWindow, short* psMatrix )
{
    BYTE i,j;
    BYTE ucAddr = 0;
    short sTmp = 0;
    _ACE_GET_INST_PRIVATE();
    DECLARA_BANK_VARIABLE

    BACKUP_SC_BANK
    if( bWindow == MAIN_WINDOW )
    {
        SET_SC_BANK(BANK_BT2020_MAIN);
        ucAddr = REG_ADDR_CM_BT2020&0xFF;
    }
    else
    {
        
         RECOVER_SC_BANK
	  return ; 	 	
    }

    for(i=0; i!=2; i++)
    {
        for(j=0; j!=2; j++)
        {
            sTmp = psMatrix[i*2+j];
            if( sTmp >= 0 )
            {
                if( sTmp > 0xfff )
                {
                    sTmp = 0xfff;
                }
            }
            else
            {
                sTmp = sTmp * -1;
                if( sTmp > 0xfff )
                {
                    sTmp = 0xfff;
                }
                #ifdef USE_NEW_ACE_MATRIX_RULE
                    sTmp = ~sTmp +1;
                #else
                sTmp |= 0x1000;
                #endif

            }
            ACE_WRITE_CM(psACEInstPri->u32DeviceID, ucAddr, sTmp );

            ucAddr += 2;
        } // for
    } // for

    RECOVER_SC_BANK
}

static void msWriteColorMatrix(void *pInstance, BOOL bWindow, short* psMatrix )
{
    BYTE i,j;
    BYTE ucAddr;
    short sTmp;
    _ACE_GET_INST_PRIVATE();
    DECLARA_BANK_VARIABLE

    BACKUP_SC_BANK
    if( bWindow == MAIN_WINDOW )
    {
        SET_SC_BANK(BANK_CM_MAIN);
        ucAddr = REG_ADDR_CM_MAIN&0xFF;
    }
    else
    {
        SET_SC_BANK(BANK_CM_SUB);
        ucAddr = REG_ADDR_CM_SUB&0xFF;
    }

    for(i=0; i!=3; i++)
    {
        for(j=0; j!=3; j++)
        {
            sTmp = psMatrix[i*3+j];
            if( sTmp >= 0 )
            {
                if( sTmp > 0xfff )
                {
                    sTmp = 0xfff;
                }
                //SC_WRITE_2BYTE( ucAddr, sTmp );
            }
            else
            {
                sTmp = sTmp * -1;
                if( sTmp > 0xfff )
                {
                    sTmp = 0xfff;
                }
                #ifdef USE_NEW_ACE_MATRIX_RULE
                    sTmp = ~sTmp +1;
                #else
                sTmp |= 0x1000;
                #endif

                //SC_WRITE_2BYTE( ucAddr, sTmp );
            }
            ACE_WRITE_CM(psACEInstPri->u32DeviceID, ucAddr, sTmp );

            ucAddr += 2;
        } // for
    } // for

    RECOVER_SC_BANK
}

void msSetVideoColorMatrix(void *pInstance, BOOL bScalerWin )
{
    short sResultTmp1[3][3];
    short sResultTmp2[3][3];
    short* psLastResult;
    short* psBT2020;	
    short tBT2020_NCLYUVtoRGB[3][3]=
    {
        { 0x06BC, 0x04AC,  0x0000},
        { -0x029B, 0x04AC, - 0x00C0},
        { 0x0000, 0x04AC,  0x0898}
    };
    short tBT2020_CLYUVtoRGB[3][3]=
    {
        { 0x07D9, 0x04AC,  0x0000},
        { 0x0000, 0x0400,  0x0000},
        { 0x0000, 0x04AC,  0x08DD}
    };
    short tBT2020_CL[2][2]=
    {
        { 0x87D9, 0x0484},
        { 0x08DD, 0x073A}
    };
    short tBT2020_Off[2][2]=
    {
        { 0, 0},
        { 0, 0}
    };	
    DECLARA_BANK_VARIABLE
    _ACE_GET_VARIABLE();

    U8 i,j;
    short ColorMatrix_1[3][3];
    short ColorMatrix_2[3][3];

    if( TRUE == bBypassColorMatrix)
    {
        return ;
    }

    BACKUP_SC_BANK


    // Adjust hue&saturation, and then YUV to RGB
    //ArrayMultiply( (Typ3x3Array)(s_AceInfo[bScalerWin].psYVUtoRGBMatrix), s_AceInfo[bScalerWin].sVideoSatHueMatrix, sResultTmp1 );

        for ( i=0;i<3;i++ )
        {
            for ( j=0;j<3;j++ )
            {
               ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psYVUtoRGBMatrix +(i*3)+j);;
               ColorMatrix_2[i][j]= s_AceInfo[bScalerWin].sVideoSatHueMatrix[i][j];
            }
        }
       ArrayMultiply(ColorMatrix_1,ColorMatrix_2, sResultTmp1 );




    // Do color correction
    if( NULL != s_AceInfo[bScalerWin].psColorCorrectionMatrix )
    {
      // ArrayMultiply( (Typ3x3Array)(s_AceInfo[bScalerWin].psColorCorrectionMatrix), sResultTmp1, sResultTmp2 );
        for ( i=0;i<3;i++ )
        {
            for ( j=0;j<3;j++ )
            {
               ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psColorCorrectionMatrix +(i*3)+j);
            }
         }
         ArrayMultiply( ColorMatrix_1, sResultTmp1, sResultTmp2 );


        // Adjust contrast-RGB
        ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, sResultTmp2, sResultTmp1 );

        psLastResult = (short*)sResultTmp1;
    }
    else // No color correction
    {
        // Adjust contrast-RGB
        ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, sResultTmp1, sResultTmp2 );
        psLastResult = (short*)sResultTmp2;
    }
	
    if(MDrv_ACE_GetBT2020_Colormetry() == E_ACE_BT2020_NCL)
    {
           //psLastResult = (short*)tBT2020_NCLYUVtoRGB;
	psBT2020 =tBT2020_Off ;
    }
    else if (MDrv_ACE_GetBT2020_Colormetry() == E_ACE_BT2020_CL)	
    {
        //psLastResult = (short*)tBT2020_CLYUVtoRGB;
	psBT2020 = tBT2020_CL ;	
    }
    else
    {
          //psLastResult = psLastResult ;
	psBT2020 = tBT2020_Off;	 
    }
	
    if (MDrv_ACE_IsSupportMLoad(pInstance) && (!MDrv_ACE_GetSkipWaitVsync(pInstance, bScalerWin)))
    {
  
         MDrv_ACE_WriteColorMatrixBurst(pInstance, bScalerWin, (MS_U16*)psLastResult );//ACE
	 MDrv_ACE_Write_BT2020_ColorMatrixBurst(pInstance, bScalerWin, (MS_U16*)psBT2020 );//BT2020	 
    }
    else
    {
         msWriteColorMatrix(pInstance, bScalerWin, psLastResult );
	 msWriteBt2020ColoreMatrix(pInstance, bScalerWin, psBT2020);	 
    }

    if( bScalerWin == MAIN_WINDOW )
    {   SET_SC_BANK(BANK_CM_MAIN);
        //if(ACE_READ_CTL(psACEInstPri->u32DeviceID,REG_ADDR_CM_VOP_PATTERN, 0x1000) == 0x0)
        if(bIsIREMode_Open == TRUE)
        {
            SET_SC_BANK(BANK_CM_MAIN);
            ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, ENABLE_CM_CTL1, MASK_CM_VOP_PATTERN_CTL ); // Enable color matrix&Change R/B range
            bColorMatrixEnable = TRUE;
        }
        else
        {
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, ENABLE_CM_CTL1, MASK_CM_CTL ); // Enable color matrix&Change R/B range
            bColorMatrixEnable = TRUE;
        //ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, 0x35, MASK_CM_CTL ); // Enable color matrix&Change R/B range
    }
#if 0
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, ENABLE_CM_CTL1, MASK_CM_CTL ); // Enable color matrix&Change R/B range
        //ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, 0x35, MASK_CM_CTL ); // Enable color matrix&Change R/B range
#endif
    }
    else // Sub window
    {
        SET_SC_BANK(BANK_CM_SUB);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, ENABLE_CM_CTL1, MASK_CM_CTL ); // Enable color matrix&Change R/B range
        //ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, 0x35, MASK_CM_CTL ); // Enable color matrix&Change R/B range
    }

    RECOVER_SC_BANK
}

void msAdjustHSC(void *pInstance, BOOL bScalerWin, BYTE ucHue, BYTE ucSaturation, BYTE ucContrast)
{
    _ACE_GET_VARIABLE();

    s_AceInfo[bScalerWin].ucSaturation = ucSaturation;
    s_AceInfo[bScalerWin].wContrast = (WORD)ucContrast<<3;
    s_AceInfo[bScalerWin].ucHue = ucHue;
    SetVideoSatHueMatrix(pInstance, bScalerWin );
    SetVideoContrastMatrix(pInstance, bScalerWin );

    msSetVideoColorMatrix(pInstance, bScalerWin );
}

void msAdjustVideoContrast(void *pInstance, BOOL bScalerWin, BYTE ucContrast )
{
    _ACE_GET_VARIABLE();

    s_AceInfo[bScalerWin].wContrast = (WORD)ucContrast<<3;
    SetVideoContrastMatrix(pInstance, bScalerWin );
    msSetVideoColorMatrix(pInstance, bScalerWin );
}
void msAdjustVideoContrast_10Bits(void *pInstance, BOOL bScalerWin, WORD wContrast )
{
    _ACE_GET_VARIABLE();

    s_AceInfo[bScalerWin].wContrast = wContrast<<1;
    SetVideoContrastMatrix(pInstance, bScalerWin );
    msSetVideoColorMatrix(pInstance, bScalerWin );
}

void msAdjustVideoSaturation(void *pInstance, BOOL bScalerWin, BYTE ucSaturation )
{
    _ACE_GET_VARIABLE();

    s_AceInfo[bScalerWin].ucSaturation = ucSaturation;
    SetVideoSatHueMatrix(pInstance, bScalerWin );
    msSetVideoColorMatrix(pInstance, bScalerWin );
}

void msAdjustVideoHue(void *pInstance, BOOL bScalerWin, BYTE ucHue )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].ucHue = ucHue;
    SetVideoSatHueMatrix(pInstance, bScalerWin );
    msSetVideoColorMatrix(pInstance, bScalerWin );
}
void msAdjustVideoRGB(void *pInstance, BOOL bScalerWin, BYTE ucRCon, BYTE ucGCon, BYTE ucBCon)
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wRCon = (WORD)ucRCon<<3;
    s_AceInfo[bScalerWin].wGCon = (WORD)ucGCon<<3;
    s_AceInfo[bScalerWin].wBCon = (WORD)ucBCon<<3;
    SetVideoContrastMatrix(pInstance, bScalerWin );
    msSetVideoColorMatrix(pInstance, bScalerWin );
}
void msAdjustVideoRGB_10Bits(void *pInstance, BOOL bScalerWin, WORD wRCon, WORD wGCon, WORD wBCon)
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wRCon = wRCon<<1;
    s_AceInfo[bScalerWin].wGCon = wGCon<<1;
    s_AceInfo[bScalerWin].wBCon = wBCon<<1;
    SetVideoContrastMatrix(pInstance, bScalerWin );
    msSetVideoColorMatrix(pInstance, bScalerWin );
}

static void SetPCConRGBMatrix(void *pInstance, BOOL bWindow )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bWindow].sContrastRGBMatrix[0][0] = ( (DWORD)s_AceInfo[bWindow].wRCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[1][1] = ( (DWORD)s_AceInfo[bWindow].wGCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[2][2] = ( (DWORD)s_AceInfo[bWindow].wBCon * (s_AceInfo[bWindow].wContrast)) >> 10;
}

void msSetPCColorMatrix(void *pInstance, BOOL bScalerWin )
{
    _ACE_GET_VARIABLE();
    short sResultTmp1[3][3];
    short sResultTmp2[3][3];
    short* psOutTab;
    //BYTE ucCMCtlVal;
    DECLARA_CMCTL_VARIABLE
    DECLARA_BANK_VARIABLE

    U8 i,j;
    short ColorMatrix_1[3][3];
    short ColorMatrix_2[3][3];

    if( TRUE == bBypassColorMatrix)
    {
        return ;
    }

    BACKUP_SC_BANK


    psOutTab = (short*)s_AceInfo[bScalerWin].sContrastRGBMatrix;

    // YUV => RGB
    if( (s_AceInfo[bScalerWin].bForceYUVtoRGB) || (bScalerWin == SUB_WINDOW) )
    {
        //ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, (Typ3x3Array)(s_AceInfo[bScalerWin].psYVUtoRGBMatrix), sResultTmp1 );
        for ( i=0;i<3;i++ )
        {
            for ( j=0;j<3;j++ )
            {
               ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psYVUtoRGBMatrix +(i*3)+j);;
            }
        }
        ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, ColorMatrix_1, sResultTmp1 );


        psOutTab = (short*)sResultTmp1;
        //ucCMCtlVal = 0x35;
        CTLVal = ENABLE_CM_CTL1;
        bColorMatrixEnable = TRUE;
    }
    else
    {
        CTLVal = ENABLE_CM_CTL2;
        bColorMatrixEnable = TRUE;
        //ucCMCtlVal = 0x30;
    }

    // Convert to sRGB
    if( s_AceInfo[bScalerWin].psPCsRGBMatrix != NULL )
    {
        //ArrayMultiply( (Typ3x3Array)s_AceInfo[bScalerWin].psPCsRGBMatrix, (Typ3x3Array)psOutTab, sResultTmp2 );
        for ( i=0;i<3;i++ )
        {
            for ( j=0;j<3;j++ )
            {
               ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psPCsRGBMatrix +(i*3)+j);;
               ColorMatrix_2[i][j]=*(psOutTab +(i*3)+j);;
            }
        }
        ArrayMultiply( ColorMatrix_1, ColorMatrix_2, sResultTmp2 );


        psOutTab = (short*)sResultTmp2;
    }

    if (MDrv_ACE_IsSupportMLoad(pInstance)&& (!MDrv_ACE_GetSkipWaitVsync(pInstance,bScalerWin)))
    {
        MDrv_ACE_WriteColorMatrixBurst(pInstance, bScalerWin, (MS_U16*)psOutTab );
    }
    else
    {
        msWriteColorMatrix(pInstance, bScalerWin, psOutTab );
    }

    if( bScalerWin == MAIN_WINDOW )
    {

        SET_SC_BANK(BANK_CM_MAIN);
        //if(ACE_READ_CTL(psACEInstPri->u32DeviceID,REG_ADDR_CM_VOP_PATTERN,0x1000) == 0x0)
        if(bIsIREMode_Open == TRUE)
        {
            SET_SC_BANK(BANK_CM_MAIN);
            ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_VOP_PATTERN_CTL );
        }
        else
        {
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_CTL );
    }

#if 0
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_CTL );
#endif
    }
    else
    {
        SET_SC_BANK(BANK_CM_MAIN);
        //if(ACE_READ_CTL(psACEInstPri->u32DeviceID,REG_ADDR_CM_VOP_PATTERN,0x1000) == 0x0)
        if(bIsIREMode_Open == TRUE)
        {   
            SET_SC_BANK(BANK_CM_SUB);
            ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_VOP_PATTERN_CTL );
        }
        else
        {  
        SET_SC_BANK(BANK_CM_SUB);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, CTLVal, MASK_CM_CTL );
    }
    }
    RECOVER_SC_BANK
}

void msAdjustPCContrast(void *pInstance, BOOL bScalerWin, BYTE ucContrast )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wContrast = (WORD)ucContrast<<3;
    SetPCConRGBMatrix(pInstance, bScalerWin );
    msSetPCColorMatrix(pInstance, bScalerWin );
}
void msAdjustPCContrast_10Bits(void *pInstance, BOOL bScalerWin, WORD wContrast )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wContrast = wContrast<<1;
    SetPCConRGBMatrix(pInstance, bScalerWin );
    msSetPCColorMatrix(pInstance, bScalerWin );
}

void msAdjustPCRGB(void *pInstance, BOOL bScalerWin, BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wRCon = (WORD)ucRed<<3;
    s_AceInfo[bScalerWin].wGCon = (WORD)ucGreen<<3;
    s_AceInfo[bScalerWin].wBCon = (WORD)ucBlue<<3;
    SetPCConRGBMatrix(pInstance,  bScalerWin );
    msSetPCColorMatrix(pInstance,  bScalerWin );
}
void msAdjustPCRGB_10Bits(void *pInstance, BOOL bScalerWin, WORD wRed, WORD wGreen, WORD wBlue )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wRCon = wRed<<1;
    s_AceInfo[bScalerWin].wGCon = wGreen<<1;
    s_AceInfo[bScalerWin].wBCon = wBlue<<1;
    SetPCConRGBMatrix(pInstance, bScalerWin );
    msSetPCColorMatrix(pInstance, bScalerWin );
}

//-------------------------------------------------------------------------------
// Directly control color matrix on/off
//-------------------------------------------------------------------------------
void msACE_SetColorMatrixControl(void *pInstance, BOOL bScalerWin, BOOL bEnable )
{
    DECLARA_CMCTL_VARIABLE
    DECLARA_BANK_VARIABLE

    BACKUP_SC_BANK
    _ACE_GET_INST_PRIVATE();

    if( bEnable )
        CTLVal = MASK_CM_CTL_EN;
    else
        CTLVal = 0;

    if( bScalerWin == MAIN_WINDOW )
    {

        SET_SC_BANK(BANK_CM_MAIN);
        //if((ACE_READ_CTL(psACEInstPri->u32DeviceID,REG_ADDR_CM_VOP_PATTERN,0x1000) == 0x0)&&(CTLVal == MASK_CM_CTL_EN))
        if(bIsIREMode_Open == TRUE)
        {
            SET_SC_BANK(BANK_CM_MAIN);
            ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_VOP_PATTERN_CTL );
        }
        else
        {
            SET_SC_BANK(BANK_CM_MAIN);
            ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_CTL_EN );
        }
#if 0
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_CTL_EN );
#endif
    }
    else
    {
        SET_SC_BANK(BANK_CM_SUB);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, CTLVal, MASK_CM_CTL_EN );
    }
    RECOVER_SC_BANK
}

//-------------------------------------------------------------------------------
// Directly control color matrix R/B range
// bRange: 0 -> 0~255, 1 -> -128~127
//-------------------------------------------------------------------------------
void msACE_SetRBChannelRange(void *pInstance, BOOL bScalerWin, BOOL bRange )
{
    DECLARA_CMCTL_VARIABLE

    DECLARA_BANK_VARIABLE

    BACKUP_SC_BANK

    if( bRange )
        CTLVal = MASK_CM_CTL_RB_RANGE;
    else
        CTLVal = 0;
    _ACE_GET_INST_PRIVATE();

   
    bColorMatrixEnable = FALSE;
    if( bScalerWin == MAIN_WINDOW )
    {
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_CTL_RB_RANGE );
    }
    else
    {
        SET_SC_BANK(BANK_CM_SUB);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, CTLVal, MASK_CM_CTL_RB_RANGE );
    }
    RECOVER_SC_BANK
}

MS_BOOL msACESetBypassColorMatrix(void *pInstance, MS_BOOL bEnable)
{
    _ACE_GET_VARIABLE();
    bBypassColorMatrix = bEnable;
    return TRUE ;
}
